"""Tests for ApiGen/CanApiGen.py and the API it generates.

The generated C++ is compiled with g++ (the laptop's compiler, not avr-gcc) against a stub of the
MCP2515 library (Tests/stubs/mcp2515.h) and the CANHelper files in the CANApi submodule, with
every message enabled, and then run on test frames.
"""
import os
import shutil
import struct
import subprocess
import sys

import pytest
from openpyxl import Workbook

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'ApiGen'))
import CanApiGen  # noqa: E402

CONFIG = os.path.join(ROOT, 'config', 'CANConfig.xlsm')
CANAPI = os.path.join(ROOT, 'CANApi')
STUBS = os.path.join(ROOT, 'Tests', 'stubs')

needs_gpp = pytest.mark.skipif(shutil.which('g++') is None, reason='g++ is not installed')
needs_canapi = pytest.mark.skipif(not os.path.isfile(os.path.join(CANAPI, 'CANHelper.hpp')),
                                  reason='CANApi submodule not checked out (git submodule update --init)')


@pytest.fixture(scope='module')
def messages():
    messages, _, errors = CanApiGen.read_messages(CONFIG)
    assert errors == []
    return messages


# ---------------------------------------------------------------- the committed spreadsheet and outputs

def test_config_is_valid():
    messages, skipped, errors = CanApiGen.read_messages(CONFIG)
    assert errors == []
    assert len(messages) == 31
    assert len(skipped) == 4  # the Testbed and Tritium Reserved rows are marked N


def read(path):
    with open(path, newline='') as f:
        return f.read()


def test_example_output_is_up_to_date(messages):
    header, impl = CanApiGen.generate(messages, 'test')
    assert read(os.path.join(ROOT, 'ApiGen', 'generatedAPIs', 'test.hpp')) == header
    assert read(os.path.join(ROOT, 'ApiGen', 'generatedAPIs', 'test.cpp')) == impl


@needs_canapi
def test_canapi_is_up_to_date(messages):
    """CANApi/CanApiv10.* must be what the spreadsheet generates: regenerate it after changing either."""
    header, impl = CanApiGen.generate(messages, 'CanApiv10')
    assert read(os.path.join(CANAPI, 'CanApiv10.hpp')) == header
    assert read(os.path.join(CANAPI, 'CanApiv10.cpp')) == impl


def test_big_endian_field_offsets(messages):
    pack = {m.can_id: m for m in messages}['0x700']
    assert pack.big_endian
    assert [(name, size, offset) for _, name, size, offset in pack.fields] == [
        ('PackCurrent', 2, 0), ('PackInstVoltage', 2, 2), ('PackSoc', 1, 4), ('RelayState', 2, 5)]


def test_camel_case_matches_vba():
    assert CanApiGen.camel_case('GPS_fixquality') == 'GpsFixquality'
    assert CanApiGen.camel_case('Pack Inst. Voltage') == 'PackInstVoltage'
    assert CanApiGen.camel_case('15 Volt. Rail') == 'VoltRail'  # digits are dropped, as in the VBA
    assert CanApiGen.camel_case(None) == ''


# ---------------------------------------------------------------- spreadsheet checks (made-up spreadsheets)

HEADINGS = ['Active/Confirmed', 'Item', 'Source', 'Period (s)', 'CAN_ID', 'CAN_ID (dec)', 'DLC'] + \
    ['BYTE_%d' % i for i in range(8)] + ['freq', 'rate (byte/s)', 'data type', 'Endianness',
                                         'struct unpack code', 'c struct', 'Labels', 'ItemCC', 'SourceCC'] + \
    ['BYTE_%dCC' % i for i in range(8)]


def row(item, can_id, dlc, byte_names, types, fields, active='Y', source='Test', endian='LITTLE'):
    """A CAN Data row with the formula columns filled in as plain values."""
    item_cc, source_cc = CanApiGen.camel_case(item), CanApiGen.camel_case(source)
    names = (byte_names + ['-'] * 8)[:8]
    cc = ['-' if n in ('IN USE', '-') else CanApiGen.camel_case(n) for n in names]
    struct_text = 'struct %s {_x000D_\n%s}' % (item_cc, ''.join('\t%s %s;_x000D_\n' % f for f in fields))
    return [active, item, source, None, can_id, None, dlc] + names + \
        [None, None, types, endian, None, struct_text, None, item_cc, source_cc] + cc


def write_config(path, rows, end=True):
    wb = Workbook()
    sheet = wb.active
    sheet.title = 'CAN Data'
    sheet.append(HEADINGS)
    for r in rows:
        sheet.append(r)
    if end:
        sheet.append(['END'])
    types = wb.create_sheet('DataTypes')
    types.append(['Type', 'c type', 'struct unpack', 'size (bytes)'])
    for t in (['float', 'float', 'f', 4], ['uint8', 'uint8_t', 'B', 1], ['uint16', 'uint16_t', 'H', 2]):
        types.append(t)
    wb.save(path)
    return str(path)


GOOD = row('Speed', '0x100', 8, ['speed', 'IN USE', 'IN USE', 'IN USE', 'angle', 'IN USE', 'IN USE', 'IN USE'],
           '2*float', [('float', 'Speed'), ('float', 'Angle')])


def check(tmp_path, rows, **kwargs):
    return CanApiGen.read_messages(write_config(tmp_path / 'config.xlsx', rows, **kwargs))


def test_good_config(tmp_path):
    messages, skipped, errors = check(tmp_path, [GOOD])
    assert errors == [] and skipped == [] and [m.full_name for m in messages] == ['Test_Speed']


def test_rows_marked_n_are_skipped(tmp_path):
    other = row('Other', '0x101', 1, ['x'], 'uint8', [('uint8_t', 'X')], active='N')
    messages, skipped, errors = check(tmp_path, [GOOD, other])
    assert errors == [] and len(messages) == 1 and len(skipped) == 1


@pytest.mark.parametrize('rows, expected', [
    ([GOOD, row('Speed', '0x101', 1, ['x'], 'uint8', [('uint8_t', 'X')])], 'name Test_Speed is used by rows 2, 3'),
    ([GOOD, row('Other', '0x100', 1, ['x'], 'uint8', [('uint8_t', 'X')])], 'CAN ID 0x100 is used by rows 2, 3'),
    ([row('Two', '0x100', 2, ['blank', 'blank'], '2*uint8', [('uint8_t', 'Blank'), ('uint8_t', 'Blank')])],
     "field name 'Blank' is used more than once"),
    ([row('Short', '0x100', 3, ['a', 'IN USE'], 'uint16', [('uint16_t', 'A')])], 'add up to 2 bytes, but the DLC (G) is 3'),
    ([row('Odd', '0x100', 1, ['a'], 'uint8', [('uint8_t', 'A')], active='Maybe')], 'must be Y, P or N'),
    ([row('Bad', '100', 1, ['a'], 'uint8', [('uint8_t', 'A')])], 'standard (11-bit) ID'),
    ([row('Bad', '0x100', 1, ['a'], 'int64', [('int64_t', 'A')])], "data type (R) 'int64' is not valid"),
    ([row('Bad', '0x100', 1, ['a'], 'uint8', [('uint8_t', 'A')], endian='MIDDLE')], 'must be BIG or LITTLE'),
    ([row('Stale', '0x100', 2, ['a', 'b'], '2*uint8', [('uint8_t', 'A')])], "does not match columns R, W and Y to AF"),
])
def test_config_errors(tmp_path, rows, expected):
    _, _, errors = check(tmp_path, rows)
    assert any(expected in e for e in errors), errors


def test_missing_end_row(tmp_path):
    _, _, errors = check(tmp_path, [GOOD], end=False)
    assert any('no END row' in e for e in errors)


def test_nothing_written_if_config_has_errors(tmp_path):
    config = write_config(tmp_path / 'config.xlsx', [GOOD, GOOD])
    out = tmp_path / 'Out'
    assert CanApiGen.main(['-c', config, '-h', str(out)]) == 1
    assert not os.path.exists(str(out) + '.hpp')


def test_usage(capsys):
    assert CanApiGen.main([]) == 2
    assert CanApiGen.main(['--help']) == 0
    assert '-h, --headerStream' in capsys.readouterr().out


# ---------------------------------------------------------------- compiling and running the generated C++

HARNESS = r'''
#include <stdio.h>
#include "src/CANApi/CANHelper.hpp"
using namespace CANHelper;

%(handlers)s

void CanMsgHandler::processAll(CANHelperBuffer& msg) {
    printf("ALL %%03X:", (unsigned)msg.raw.can_id);
    for (int i = 0; i < msg.raw.can_dlc; i++) printf(" %%02X", msg.raw.data[i]);
    printf("\n");
}

// Reads lines "<hex CAN ID> <hex bytes...>" (or "R <hex bytes...>" to test reverseBytes) from stdin
int main() {
    CANHandler handler(10, CAN_125KBPS);
    char line[256];
    while (fgets(line, sizeof line, stdin)) {
        CANHelperBuffer buffer;
        memset(&buffer, 0, sizeof buffer);
        char* p = line;
        int n = 0;
        bool reverse = (*p == 'R');
        if (reverse) p++;
        else buffer.raw.can_id = (canid_t)strtoul(p, &p, 16);
        for (char* end; n < 8; p = end, n++) {
            unsigned long b = strtoul(p, &end, 16);
            if (end == p) break;
            buffer.raw.data[n] = (__u8)b;
        }
        buffer.raw.can_dlc = n;
        if (reverse) {
            handler.reverseBytes(buffer);
            printf("REV:");
            for (int i = 0; i < n; i++) printf(" %%02X", buffer.raw.data[i]);
            printf("\n");
        } else {
            handler.testRead(buffer.raw);
        }
    }
    return 0;
}
'''


def handler(m):
    prints = ''.join(
        '    printf(" %s=%%%s", %s(msg.%s));\n' % (name, 'g' if ctype == 'float' else 'ld',
                                                   '(double)' if ctype == 'float' else '(long)', name)
        for ctype, name, _, _ in m.fields)
    return ('void CanMsgHandler::processMessage(Messages::%s::%s& msg) {\n    printf("%s");\n%s    printf("\\n");\n}\n'
            % (m.source, m.item, m.full_name, prints))


def build(directory, use_msg_list, handlers=''):
    """Lays out a sketch (UseMsgList.hpp, src/CANApi/...) like the firmware and compiles it."""
    api = directory / 'src' / 'CANApi'
    api.mkdir(parents=True)
    assert CanApiGen.main(['-c', CONFIG, '-h', str(api / 'CanApiv10')]) == 0
    for name in ('CANHelper.hpp', 'CANHelper.cpp'):
        shutil.copy(os.path.join(CANAPI, name), str(api / name))
    (directory / 'UseMsgList.hpp').write_text(use_msg_list)
    (directory / 'main.cpp').write_text('#include <stdlib.h>\n' + HARNESS % {'handlers': handlers})
    program = str(directory / 'harness')
    result = subprocess.run(['g++', '-std=gnu++11', '-Wall', '-I', STUBS, '-o', program, 'main.cpp',
                             'src/CANApi/CANHelper.cpp', 'src/CANApi/CanApiv10.cpp'],
                            cwd=str(directory), capture_output=True, text=True)
    assert result.returncode == 0, result.stderr
    errors = [line for line in result.stderr.splitlines()
              if 'warning' in line and 'nested namespace' not in line]  # C++17 namespaces, as on the Arduino
    assert errors == [], result.stderr
    return program


def run(program, frames):
    return subprocess.run([program], input='\n'.join(frames) + '\n', capture_output=True,
                          text=True, check=True).stdout.splitlines()


@pytest.fixture(scope='module')
def all_messages(tmp_path_factory, messages):
    """Every message enabled for reading and writing, plus processAll."""
    if shutil.which('g++') is None or not os.path.isfile(os.path.join(CANAPI, 'CANHelper.hpp')):
        pytest.skip('needs g++ and the CANApi submodule')
    use = ''.join('#define USE_MSG_%s 0b11\n' % m.full_name for m in messages) + '#define PROCESS_ALL_MSG\n'
    return build(tmp_path_factory.mktemp('all'), use, ''.join(handler(m) for m in messages))


def hexbytes(data):
    return ' '.join('%02X' % b for b in data)


def test_little_endian_message(all_messages):
    frame = struct.pack('<ff', 1.5, -2.25)
    assert run(all_messages, ['0F7 ' + hexbytes(frame)]) == [
        'Telemetry_SpeedAndAngle GpsSpeed=1.5 GpsAngle=-2.25',
        'ALL 0F7: ' + hexbytes(frame)]


def test_big_endian_message_is_swapped_per_field(all_messages):
    frame = bytes([0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE])
    assert run(all_messages, ['700 ' + hexbytes(frame)]) == [
        'Orion_PackParameters PackCurrent=4660 PackInstVoltage=22136 PackSoc=154 RelayState=48350',
        'ALL 700: ' + hexbytes(frame)]  # processAll gets the frame exactly as received


def test_big_endian_signed_message(all_messages):
    frame = struct.pack('>hhbbb', -2, 300, -10, 10, 0)
    assert run(all_messages, ['701 ' + hexbytes(frame)])[0] == \
        'Orion_CellTempsDcl PackDcl=-2 Unknown=300 HighTemperatureSensor=-10 LowTemperatureSensors=10 Blank=0'


def test_unknown_id_only_goes_to_process_all(all_messages):
    assert run(all_messages, ['123 01 02']) == ['ALL 123: 01 02']


@pytest.mark.parametrize('data, expected', [
    ('01 02 03 04 05 06 07', '07 06 05 04 03 02 01'),
    ('01 02 03 04', '04 03 02 01'),
    ('AA', 'AA'),
])
def test_reverse_bytes(all_messages, data, expected):
    assert run(all_messages, ['R ' + data]) == ['REV: ' + expected]


@needs_gpp
@needs_canapi
def test_macro_without_value_means_struct_only(tmp_path):
    """#define USE_MSG_X with no value (as in older UseMsgList.hpp files) compiles, and only gives the struct."""
    use = '#define USE_MSG_Telemetry_TimeAndFix\n#define USE_MSG_Mppt_Javed 0b01\n#define PROCESS_ALL_MSG\n'
    program = build(tmp_path, use)
    assert run(program, ['0F6 01 02 03 04 05 06 07 08']) == ['ALL 0F6: 01 02 03 04 05 06 07 08']
