# CAN message spreadsheet (CANConfig.xlsm)

[CANConfig.xlsm](CANConfig.xlsm) lists every CAN message the telemetry board firmware knows about: one row per message, with its CAN ID, length, the name of each data byte, the data types and the byte order. The generator ([ApiGen/CanApiGen.py](../ApiGen/CanApiGen.py)) turns it into the C++ structs in CANApi.

It is a macro-enabled Excel workbook. Three VBA worksheet functions in it (`CamelCase`, `CStruct`, `structUnpackCode`; exported copy in [ApiGen/excelMacroModule.vba](../ApiGen/excelMacroModule.vba)) build the names and the C struct for each row. They only calculate cell values. Nothing runs when the workbook is opened.

## Sheets

| Sheet | Used by the generator? | Contents |
|---|---|---|
| `CAN Data` | **Yes** (must be the active sheet when saved) | One row per CAN message (described below) |
| `DataTypes` | Yes, through the VBA functions | The data types you can use in column R: their C type, Python `struct` code and size |
| `Data Types` | No | Older notes: the GPS and Tritium values and their types, from 2022 |
| `Info` | No | A note copied from the Adafruit GPS library on `nmea_float_t` (it is a 4-byte `float` on the Arduino) |

## Columns of the CAN Data sheet

**Only type in the columns marked "You"** (and, if needed, over column W). The others are formulas.

| Column | Heading | Who fills it | Meaning |
|---|---|---|---|
| A | `Active/Confirmed` | You | `Y`, `N` or `P`. For information only: **the generator ignores it.** The row containing `END` here marks the end of the list. |
| B | `Item` | You | The message name in plain words, e.g. `Time and fix` |
| C | `Source` | You | The device that sends it, e.g. `Telemetry`, `Tritium`, `Orion`, `MPPT` |
| D | `Period (s)` | You | How often the message is sent, in seconds (information only) |
| E | `CAN_ID` | You | The CAN ID in hexadecimal, **with** the `0x`, e.g. `0x0F6`. Copied into the C++ code as written. |
| F | `CAN_ID (dec)` | Formula | The CAN ID in decimal |
| G | `DLC` | You | The number of data bytes (1 to 8) |
| H to O | `BYTE_0` to `BYTE_7` | You | The name of the value that **starts** in each byte. See [Byte columns](#byte-columns). |
| P, Q | `freq`, `rate (byte/s)` | Formula | Messages per second and bytes per second, from D and G (information only, for estimating bus and radio load) |
| R | `data type` | You | The type of each value, in order. See [Data types](#data-types). |
| S | `Endianness` | You | `LITTLE` or `BIG`: the byte order of multi-byte values. `BIG` makes the generated code reverse the bytes before handling the message (see [Known limitations](../README.md#known-limitations)). |
| T | `struct unpack code` | `structUnpackCode` | A Python `struct` format string, e.g. `<fc`. Was used by the old hex decoder; not used now. |
| U | `c struct` | `CStruct` | **The C struct the generator copies into the header.** Shows `#VALUE!` if R does not match G. |
| V | `Labels to be used in code` | | Empty, not used |
| W | `ItemCC` | `CamelCase` of B (can be typed over) | The message name in code, e.g. `TimeAndFix` |
| X | `SourceCC` | `CamelCase` of C | The sender name in code, used as a namespace, e.g. `Telemetry` |
| Y to AF | `BYTE_0CC` to `BYTE_7CC` | `CamelCase` of H to O | The field names in code, e.g. `GpsHour`. `-` for `IN USE` and `-` bytes. |

The rows below `END` ("Suggested format for custom CAN code") are old notes and are ignored.

## Byte columns

Write the value's name in the column of the byte it **starts** in. Then:

* `IN USE` in the other bytes of a multi-byte value
* `-` (or leave empty) in the bytes after the DLC

For example, `Latitude` (0x0F8, DLC 5, data type `float, char`):

| BYTE_0 | BYTE_1 | BYTE_2 | BYTE_3 | BYTE_4 | BYTE_5 | BYTE_6 | BYTE_7 |
|---|---|---|---|---|---|---|---|
| `GPS_latitude` | `IN USE` | `IN USE` | `IN USE` | `GPS_lat` | `-` | `-` | `-` |

which gives:

```cpp
struct Latitude {
	float GpsLatitude;
	char GpsLat;
}
```

`CStruct` takes the field name from the first byte of each value and skips the rest by the type's size, so the struct is right as long as the data type column is right. `IN USE` is there for people reading the sheet.

## Data types

Column R lists the types of the values in order, separated by commas. `n*type` means `n` values of that type in a row. Spaces are ignored. Examples from the sheet:

| Column R | Meaning | DLC |
|---|---|---|
| `8*uint8` | eight 1-byte unsigned values | 8 |
| `2*float` | two 4-byte floats | 8 |
| `float, char` | a float then a character | 5 |
| `3*uint16, 2*uint8` | three 2-byte unsigned values, then two 1-byte values | 8 |
| `2*uint16,uint8,uint16` | (Orion, big-endian) | 7 |

The types available are those in the `DataTypes` sheet:

| Type | C type | Python `struct` | Bytes |
|---|---|---|---|
| `float` | `float` | `f` | 4 |
| `char` | `char` | `c` | 1 |
| `int8` | `int8_t` | `b` | 1 |
| `uint8` | `uint8_t` | `B` | 1 |
| `int16` | `int16_t` | `h` | 2 |
| `uint16` | `uint16_t` | `H` | 2 |
| `int32` | `int32_t` | `i` | 4 |
| `uint32` | `uint32_t` | `I` | 4 |

The sizes must add up to the DLC in column G. There are no bit fields: a message that packs several values into one byte (e.g. the MPPT flags and the top bits of its voltage) has to be described as whole bytes (the MPPT rows use `7*uint8`), and split up by the code that uses it.

## Names in code

The names in columns W to AF are made by `CamelCase`: every character that is not a letter becomes a space, then each word is capitalised and the spaces removed. So `GPS_fixquality` becomes `GpsFixquality` and `Pack Inst. Voltage` becomes `PackInstVoltage`.

**Digits are removed as well**, so `15 Volt. Rail` would become `VoltRail`, and `Testbed enviroCAN.ino1` and `Testbed enviroCAN.ino2` both become `TestbedEnvirocanIno`. Where that matters, type the name into column W over the formula, as was done for `Velocity`, `Rail_15V` and `Rail_3_3VAnd1_9V` (0x603, 0x608, 0x609).

The firmware refers to a message as `<SourceCC>_<ItemCC>` (e.g. `USE_MSG_Telemetry_TimeAndFix`, `as_Telemetry_TimeAndFix`), so this pair **must be unique** among the messages a firmware enables. Some rows share a name at the moment; see [Known limitations](../README.md#known-limitations).

Changing a name changes the API, so any firmware using the old name will need updating.

## Editing the spreadsheet

* Use **Microsoft Excel** on Windows or macOS (desktop, not the web version). The formula columns need the workbook's VBA functions, which other spreadsheet programs may not run.
* When Excel shows the yellow security bar, click **Enable Content**. If Excel says macros are blocked because the file came from the internet, close it, open the file's **Properties** in Windows Explorer, tick **Unblock**, and open it again.
* After editing, press **Ctrl+Alt+F9** to recalculate everything before saving. Excel does not know the VBA functions depend on the `DataTypes` sheet, so it might not update every cell otherwise.
* Save as `.xlsm` with `CAN Data` as the active sheet. The generator reads the values Excel saved, so a file that has not been recalculated and saved in Excel gives wrong output.

## Adding a message

1. Select the `END` row and insert a new row above it.
2. Copy an existing row of the same kind into it, so that the formula columns (F, P, Q, T, U, W to AF) are copied too.
3. Fill in columns A to E, G, H to O, R and S as described above.
4. Check that:
   * column U shows a struct with the right fields, not `#VALUE!`
   * column W and X give a `<SourceCC>_<ItemCC>` that no other row has (type over W if needed)
5. Press Ctrl+Alt+F9, then save.
6. Generate the API and update the firmware as described in the [top-level README](../README.md#how-to-add-or-change-a-can-message).

## Keeping the DBC files in step

The Telemetry2021 receiver does not use this spreadsheet. It decodes messages using the DBC files in Telemetry2021's `dbc/` directory, which also hold the scaling, units and descriptions that the spreadsheet does not. A message added here is relayed by the telemetry board, but the receiver stores it as `ERROR/ID UNRECOGNISED` until it is in a DBC file. See Telemetry2021's `dbc/README.md`.

The DBC files mostly use the same `SourceCC` and `ItemCC` names (as sender and message name), with these differences (Nov 2026):

| CAN ID | Here | In the DBC files |
|---|---|---|
| 0x601 | `Tritium::StausInformation` | `StatusInformation` |
| 0x60A | `Tritium::Reserved` | `Reserved1` |
| 0x6B0, 0x6B1 | `Orion::CellVoltages`, `Orion::CellTempsDischargeChageCurrentLimit` | not present |
| 0x700 | `Orion::CellVoltages` | `PackParameters` |
| 0x701 | `Orion::CellTempsDischargeChageCurrentLimit` | `CellTempsDCL` |
| 0x702 | `Orion::CellTempsDischargeChageCurrentLimit` | `CellVoltages` |
| 0x1B1, 0x1B2 | `Testbed::TestbedEnvirocanIno` | not present |

The DBC files also include messages that are not here, e.g. the driver controls commands (0x023) and the second (right) motor controller (0x041 to 0x08E).
