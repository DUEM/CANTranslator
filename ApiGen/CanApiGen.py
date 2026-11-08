"""Generates the CANApi message API (e.g. CanApiv10.hpp/.cpp) from the CAN message spreadsheet.

Usage (from the top of the repository):
    python3 ApiGen/CanApiGen.py -c config/CANConfig.xlsm -h CANApi/CanApiv10

See ApiGen/README.md for what is generated, and config/README.md for the spreadsheet.
The spreadsheet is checked first; if anything is wrong, the problems are listed and no
files are written.
"""
import getopt
import os
import re
import sys

from openpyxl import load_workbook

USAGE = """usage: python3 ApiGen/CanApiGen.py -c <spreadsheet .xlsm> -h <output path without extension>

  -c, --configTable   the CAN message spreadsheet, e.g. config/CANConfig.xlsm
  -h, --headerStream  where to write the API, without an extension: writes <path>.hpp and <path>.cpp,
                      e.g. CANApi/CanApiv10 (the name CANHelper.hpp includes)
      --help          show this message"""

# Columns of the "CAN Data" sheet (0-based: A = 0)
COL_ACTIVE = 0      # A  Active/Confirmed: Y or P = generate, N = skip; END marks the end of the list
COL_ITEM = 1        # B  Item
COL_SOURCE = 2      # C  Source
COL_ID = 4          # E  CAN_ID, e.g. 0x0F6
COL_DLC = 6         # G  DLC
COL_BYTE0 = 7       # H..O  BYTE_0..BYTE_7
COL_TYPES = 17      # R  data type, e.g. "2*uint16, uint8"
COL_ENDIAN = 18     # S  Endianness: BIG or LITTLE
COL_STRUCT = 20     # U  c struct (VBA CStruct)
COL_ITEMCC = 22     # W  ItemCC (VBA CamelCase of B, or typed over)
COL_SOURCECC = 23   # X  SourceCC (VBA CamelCase of C)
COL_BYTE0CC = 24    # Y..AF  BYTE_0CC..BYTE_7CC
NUM_COLS = 32

IDENTIFIER = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*$')
CAN_ID = re.compile(r'^0x[0-9A-Fa-f]{1,3}$')
FIELD_LINE = re.compile(r'^\t(\w+) (\w+);$')
RECALCULATE = "recalculate (Ctrl+Alt+F9) and save in Excel"


class Message:
    """One generated row of the spreadsheet."""

    def __init__(self, row, can_id, dlc, big_endian, struct_text, source, item, fields):
        self.row = row
        self.can_id = can_id            # as written in the spreadsheet, e.g. "0x0F6"
        self.dlc = dlc
        self.big_endian = big_endian
        self.struct_text = struct_text  # "struct Item {\n\t<c type> <name>;\n...}"
        self.source = source
        self.item = item
        self.fields = fields            # [(c type, name, size, offset)]

    @property
    def full_name(self):
        return self.source + "_" + self.item


def camel_case(text):
    """Same as the spreadsheet's VBA CamelCase(): every non-letter becomes a space,
    each word is capitalised (rest lower case), then the spaces are removed."""
    return ''.join(w[:1].upper() + w[1:].lower() for w in re.sub(r'[^a-zA-Z]', ' ', text or '').split())


def is_formula(value):
    """True for a formula cell, as read with openpyxl without data_only."""
    if isinstance(value, str):
        return value.startswith('=')
    return value is not None and not isinstance(value, (int, float, bool))  # ArrayFormula


def read_data_types(workbook):
    """The DataTypes sheet: type name -> (c type, size in bytes)."""
    types = {}
    for row in workbook['DataTypes'].iter_rows(min_row=2, values_only=True):
        if row[0] is not None and row[1] is not None and isinstance(row[3], (int, float)):
            types[str(row[0]).strip()] = (str(row[1]).strip(), int(row[3]))
    return types


def parse_type_list(text, data_types):
    """'2*uint16, uint8' -> ['uint16', 'uint16', 'uint8'] (same rules as the VBA CStruct)."""
    result = []
    for part in str(text or '').split(','):
        part = part.replace(' ', '')
        repeats, _, name = part.rpartition('*')
        if (repeats and not repeats.isdigit()) or name not in data_types:
            raise ValueError("data type (R) %r is not valid (see the DataTypes sheet)" % text)
        result.extend([name] * (int(repeats) if repeats else 1))
    return result


def parse_struct(text):
    """The saved 'c struct' cell -> (struct name, [(c type, field name)])."""
    lines = str(text).replace('_x000D_', '').replace('\r', '').split('\n')
    first = re.match(r'^struct (\w+) \{$', lines[0])
    fields = [FIELD_LINE.match(line) for line in lines[1:-1]]
    if not first or lines[-1] != '}' or not all(fields):
        raise ValueError("the 'c struct' cell (U) is not a struct but %r" % str(text)[:60])
    return first.group(1), [(m.group(1), m.group(2)) for m in fields]


def read_row(row_number, values, formulas, data_types, error):
    """Checks one active row. Returns a Message, or None if it has errors."""
    ok = True

    def fail(message):
        nonlocal ok
        ok = False
        error(message)

    can_id = str(values[COL_ID] or '').strip()
    if not CAN_ID.match(can_id) or int(can_id, 16) > 0x7FF:
        fail("CAN_ID (E) must be a standard (11-bit) ID written like 0x0F6, not %r" % values[COL_ID])
    dlc = values[COL_DLC]
    if not isinstance(dlc, (int, float)) or dlc != int(dlc) or not 1 <= dlc <= 8:
        fail("DLC (G) must be a whole number from 1 to 8, not %r" % dlc)
        return None
    dlc = int(dlc)
    endian = re.sub(r'[^a-zA-Z]', '', str(values[COL_ENDIAN] or '')).upper()
    if endian not in ('BIG', 'LITTLE'):
        fail("Endianness (S) must be BIG or LITTLE, not %r" % values[COL_ENDIAN])

    # Names: valid C++ names and, where they are formulas, what Excel would calculate now
    source, item = values[COL_SOURCECC], values[COL_ITEMCC]
    for col, typed_col, label in ((COL_SOURCECC, COL_SOURCE, 'SourceCC (X)'), (COL_ITEMCC, COL_ITEM, 'ItemCC (W)')):
        name, expected = values[col], camel_case(values[typed_col])
        if not isinstance(name, str) or not IDENTIFIER.match(name):
            fail("%s is %r, which is not a valid C++ name" % (label, name))
        elif is_formula(formulas[col]) and name != expected:
            fail("%s is %r but should be %r: %s" % (label, name, expected, RECALCULATE))
    byte_names = []
    for i in range(8):
        typed, saved = values[COL_BYTE0 + i], values[COL_BYTE0CC + i]
        expected = '-' if typed in ('IN USE', '-') else camel_case(typed)
        if is_formula(formulas[COL_BYTE0CC + i]) and (saved or '') != expected:
            fail("BYTE_%dCC is %r but should be %r: %s" % (i, saved, expected, RECALCULATE))
        byte_names.append(saved)

    # Struct: the types and sizes come from the data type column; the saved struct must agree
    try:
        types = parse_type_list(values[COL_TYPES], data_types)
        struct_name, struct_fields = parse_struct(values[COL_STRUCT])
    except ValueError as e:
        fail(str(e))
        return None
    sizes = [data_types[t][1] for t in types]
    if sum(sizes) != dlc:
        fail("the data types (R) add up to %d bytes, but the DLC (G) is %d" % (sum(sizes), dlc))
        return None
    offsets = [sum(sizes[:i]) for i in range(len(sizes))]
    expected_fields = [(data_types[t][0], byte_names[o]) for t, o in zip(types, offsets)]
    if struct_name != item or struct_fields != expected_fields:
        fail("the 'c struct' cell (U) does not match columns R, W and Y to AF: " + RECALCULATE)
        return None
    names = [name for _, name in struct_fields]
    for name in sorted(set(n for n in names if names.count(n) > 1)):
        fail("field name %r is used more than once: rename the bytes in H to O" % name)

    if not ok:
        return None
    fields = [(ctype, name, size, offset) for (ctype, name), size, offset in zip(struct_fields, sizes, offsets)]
    return Message(row_number, can_id, dlc, endian == 'BIG',
                   values[COL_STRUCT].replace('_x000D_', ''), source, item, fields)


def read_messages(config_file):
    """Reads and checks the spreadsheet. Returns (messages, skipped rows, errors)."""
    # The formula columns are calculated by VBA functions in the workbook, which openpyxl cannot
    # run, so their values are the ones Excel saved. The formulas are read as well, to tell
    # typed-over names from formulas and to spot values that Excel has not recalculated.
    values_wb = load_workbook(config_file, read_only=True, data_only=True)
    formulas_wb = load_workbook(config_file, read_only=True)
    sheet = values_wb.active
    data_types = read_data_types(values_wb)

    messages, skipped, errors = [], [], []
    rows = zip(sheet.iter_rows(min_row=2, values_only=True),
               formulas_wb[sheet.title].iter_rows(min_row=2, values_only=True))
    found_end = False
    for row_number, (values, formulas) in enumerate(rows, start=2):
        values = list(values) + [None] * (NUM_COLS - len(values))
        formulas = list(formulas) + [None] * (NUM_COLS - len(formulas))
        active = str(values[COL_ACTIVE] or '').strip().upper()
        if active == 'END':
            found_end = True
            break
        where = "row %d (%s %s)" % (row_number, values[COL_ID], values[COL_ITEM])
        if active == 'N':
            skipped.append(where)
        elif active not in ('Y', 'P'):
            errors.append("%s: Active/Confirmed (A) must be Y, P or N, not %r" % (where, values[COL_ACTIVE]))
        else:
            message = read_row(row_number, values, formulas, data_types,
                               lambda text: errors.append("%s: %s" % (where, text)))
            if message:
                messages.append(message)
    if not found_end:
        errors.append("no END row: column A of the row after the last message must be END")

    # Names and CAN IDs must be unique among the generated messages, or the C++ does not compile
    for label, key in (('name', lambda m: m.full_name), ('CAN ID', lambda m: int(m.can_id, 16))):
        rows_by_key = {}
        for m in messages:
            rows_by_key.setdefault(key(m), []).append(m)
        for same in rows_by_key.values():
            if len(same) > 1:
                errors.append("%s %s is used by rows %s: it must be unique (see config/README.md)" % (
                    label, same[0].full_name if label == 'name' else same[0].can_id,
                    ", ".join(str(m.row) for m in same)))
    return messages, skipped, errors


def read_enabled(name):
    return "#if defined(USE_MSG_%s) && ((USE_MSG_%s & 0b10) == 2)\n" % (name, name)


def generate(messages, guard_name):
    """Returns (header text, implementation text)."""
    header, impl, process_decls, union_members = [], [], [], []

    header.append("#ifndef " + guard_name + "_INCLUDE_GUARD\n")
    header.append("#define " + guard_name + "_INCLUDE_GUARD\n")
    header.append("#include \"mcp2515.h\"\n")
    header.append("namespace CANHelper::Messages {\n")

    impl.append('#include "CANHelper.hpp"\n')
    impl.append("namespace CANHelper {\n\tvoid CANHandler::DispatchMsg() {\n\t\tswitch(LATEST_MSG_ID) {\n")

    for m in messages:
        name = m.full_name
        header.append("#ifdef USE_MSG_" + name + "\n")
        header.append("#define CAN_ID_" + name + " " + m.can_id + "\n")
        header.append("#define CAN_DLC_" + name + " " + str(m.dlc) + "\n")
        header.append("#define CAN_META_" + name + " {" + m.can_id + ", " + str(m.dlc) + "}\n")
        header.append("\tnamespace " + m.source + " {\n")
        struct = "\t" + m.struct_text + " __attribute__((aligned(8)));\n"
        header.append("\t" + struct.replace("\n", "\n\t\t"))
        header.append("};\n")
        header.append("#endif\n")

        impl.append(read_enabled(name))
        impl.append("\t\tcase " + m.can_id + ":\n")
        if m.big_endian:
            impl.append("\t\t\tthis->reverseBytes(LATEST_MSG);\n")
        impl.append("\t\t\tthis->processMessage(LATEST_MSG_DATA.as_" + name + ");\n")
        impl.append("\t\t\tbreak;\n")
        impl.append("#endif\n")

        process_decls.append(read_enabled(name))
        process_decls.append("\t\tvoid processMessage(Messages::" + m.source + "::" + m.item + "& msg);\n")
        process_decls.append("#endif\n")

        union_members.append("#if defined(USE_MSG_" + name + ")\n")
        union_members.append("\t\t" + m.source + "::" + m.item + " as_" + name + ";\n")
        union_members.append("#endif\n")

    process_decls.append("#ifdef PROCESS_ALL_MSG\n")
    process_decls.append("\t\tvoid processAll(CANHelperBuffer& msg);\n")
    process_decls.append("#endif\n")

    header.append("\tunion CastedCANPayload {\n" + "".join(union_members) + "\t};\n")
    header.append("}\n")  # close Messages namespace
    header.append("""namespace CANHelper {
	union CANHelperBuffer { //to try and reduce memory copies (waste of instructions)
		can_frame raw;
		struct {
			char padding[offsetof(can_frame, data)]; //aligns payloadBuffer with can_frame.data (without memcpy)
			Messages::CastedCANPayload payloadBuffer;
		};
	};

	class CanMsgHandler {
	protected:
		CANHelperBuffer messageRead; //holds the latest received message from the CAN bus
	public:
		CanMsgHandler() = default;\n""")
    header.append("".join(process_decls))
    header.append("\t};\n}\n")  # close class and namespace
    header.append("#endif\n")  # close include guard

    impl.append("\t\t}\n")
    impl.append("#ifdef PROCESS_ALL_MSG\n")
    impl.append("\t\tthis->processAll(LATEST_MSG);\n")
    impl.append("#endif\n")
    impl.append("\t}\n}\n")
    return "".join(header), "".join(impl)


def main(argv):
    try:
        opts, _ = getopt.getopt(argv, "c:h:", ["configTable=", "headerStream=", "help"])
    except getopt.GetoptError as e:
        print("%s\n%s" % (e, USAGE), file=sys.stderr)
        return 2
    config_file = header_file = ''
    for opt, arg in opts:
        if opt == "--help":
            print(USAGE)
            return 0
        if opt in ("-c", "--configTable"):
            config_file = arg
        elif opt in ("-h", "--headerStream"):
            header_file = arg
    if not config_file or not header_file:
        print(USAGE, file=sys.stderr)
        return 2
    guard_name = os.path.basename(header_file)
    if not IDENTIFIER.match(guard_name):
        print("The output name %r must be a valid C++ name (it is used for the include guard)" % guard_name,
              file=sys.stderr)
        return 2

    print("Config file: " + config_file)
    messages, skipped, errors = read_messages(config_file)
    for m in messages:
        print("  %-5s %s (%d bytes%s)" % (m.can_id, m.full_name, m.dlc, ", big-endian" if m.big_endian else ""))
    for where in skipped:
        print("  skipped %s: Active/Confirmed is N" % where)
    if errors:
        print("\n%d problem(s) in %s, so nothing was written:" % (len(errors), config_file), file=sys.stderr)
        for e in errors:
            print("  " + e, file=sys.stderr)
        return 1

    header_text, impl_text = generate(messages, guard_name)
    with open(header_file + '.hpp', 'w', newline='\n') as f:
        f.write(header_text)
    with open(header_file + '.cpp', 'w', newline='\n') as f:
        f.write(impl_text)
    print("Generated %s.hpp and %s.cpp: %d messages, %d skipped" % (header_file, header_file, len(messages), len(skipped)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
