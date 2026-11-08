# API generator (ApiGen)

[CanApiGen.py](CanApiGen.py) reads the CAN message spreadsheet [config/CANConfig.xlsm](../config/CANConfig.xlsm) and writes the generated half of CANApi v1.0: a header (`.hpp`) with a C++ struct for every message, and an implementation (`.cpp`) with the code that sends each received message to the right handler.

The other half of CANApi, `CANHelper.hpp/.cpp`, is hand-written and lives in the [CANApi repository](https://github.com/DUEM/CANApi). It wraps the MCP2515 CAN controller and provides `read()` and `send()`.

## Running

From the top of the repository, with openpyxl installed (see the [top-level README](../README.md#first-time-setup)):

```shell
python3 ApiGen/CanApiGen.py -c config/CANConfig.xlsm -h CANApi/CanApiv10
```

| Option | Meaning |
|---|---|
| `-c`, `--configTable` | The spreadsheet to read |
| `-h`, `--headerStream` | Output path **without** an extension. Writes `<path>.hpp` and `<path>.cpp`, deleting any existing files first. (Not help.) |

The header's include guard is named after the file (`CanApiv10_INCLUDE_GUARD`). The hand-written `CANHelper.hpp` includes `CanApiv10.hpp` by name, so **keep the name `CanApiv10`** unless you also change `CANHelper.hpp`.

The script prints each row as it reads it (this is debug output, and it prints the row *after* the one being processed). It finishes with `c++ files generated file generated`.

## What it reads from the spreadsheet

It reads the first (active) sheet, `CAN Data`, from row 2 down to the row whose column A is `END`. For each row it uses:

| Column | Heading | Used for |
|---|---|---|
| E | `CAN_ID` | The CAN ID, as written (e.g. `0x0F6`) |
| G | `DLC` | The number of data bytes |
| S | `Endianness` | `BIG` adds a byte reversal before the message is handled |
| U | `c struct` | The struct definition, built by the VBA function `CStruct` |
| W | `ItemCC` | The message name (e.g. `TimeAndFix`) |
| X | `SourceCC` | The sender name, used as a namespace (e.g. `Telemetry`) |

Columns U, W and X are formulas. openpyxl cannot run them, so the script reads the values Excel saved with the file (`data_only=True`). See [config/README.md](../config/README.md) for all the columns.

## What it generates

In the examples below, `<Name>` stands for `<SourceCC>_<ItemCC>`, e.g. `Telemetry_TimeAndFix`.

**Header (`CanApiv10.hpp`)**, for each message, only if `USE_MSG_<Name>` is defined:

```cpp
#ifdef USE_MSG_Telemetry_TimeAndFix
#define CAN_ID_Telemetry_TimeAndFix 0x0F6
#define CAN_DLC_Telemetry_TimeAndFix 8
#define CAN_META_Telemetry_TimeAndFix {0x0F6, 8}
	namespace Telemetry {
		struct TimeAndFix {
			uint8_t GpsHour;
			...
		} __attribute__((aligned(8)));
		};
#endif
```

then, after all the messages:

| Item | What it is |
|---|---|
| `CANHelper::Messages::CastedCANPayload` | A union of every enabled message struct, as members `as_<Name>`. Lets the same 8 data bytes be read as any message without copying. |
| `CANHelper::CANHelperBuffer` | A union of the MCP2515 `can_frame` (`.raw`) and the payload union (`.payloadBuffer`), lined up so that `payloadBuffer` is exactly `raw.data`. Use one of these for every message you send or receive. |
| `CANHelper::CanMsgHandler` | Base class holding the last received message (`messageRead`), with one `processMessage(Messages::<Source>::<Item>&)` declaration per message whose read bit is set, plus `processAll(CANHelperBuffer&)` if `PROCESS_ALL_MSG` is defined. |

**Implementation (`CanApiv10.cpp`)**: `CANHandler::DispatchMsg()`, which is called by `CANHandler::read()` for every received frame:

```cpp
switch(LATEST_MSG_ID) {
#if defined(USE_MSG_Orion_CellVoltages) && ((USE_MSG_Orion_CellVoltages & 0b10) == 2)
case 0x6B0:
	this->reverseBytes(LATEST_MSG);          // only for rows marked BIG
	this->processMessage(LATEST_MSG_DATA.as_Orion_CellVoltages);
	break;
#endif
...
}
#ifdef PROCESS_ALL_MSG
this->processAll(LATEST_MSG);               // every frame, whatever its ID
#endif
```

The generator only **declares** `processMessage()` and `processAll()`. The firmware must **define** every one it enables, or linking fails.

## Choosing messages (UseMsgList.hpp)

`CANHelper.hpp` includes `../../UseMsgList.hpp`, i.e. a file in the sketch folder, two levels above `src/CANApi/`. It defines one macro for each message the firmware uses, with a 2-bit value:

| Value | Meaning | Struct, `CAN_ID_`/`CAN_DLC_`/`CAN_META_` macros and `as_<Name>` | `processMessage()` and dispatch `case` |
|---|---|---|---|
| `0b01` | Write only: the board sends this message, or just uses the struct | Yes | No |
| `0b10` | Read: the board handles this message when it receives it | Yes | Yes |
| `0b11` | Both | Yes | Yes |

The low (write) bit is not actually checked: any defined `USE_MSG_` macro produces the struct. **The macro must have a value.** `#define USE_MSG_X` with no value, as in the CanApiv04-era `UseMsgList.hpp` on the Telemetry2021 Peri branch, gives a compile error in the `#if` lines.

`#define PROCESS_ALL_MSG` (no value needed) adds `processAll()`, which is called for every received frame.

Example, from Telemetry2021 `nick-22Library`:

```cpp
//Values for macros: 2 bits: MSB is read enable, LSB is write enable
#define USE_MSG_Telemetry_SystemStatusMessages 0b01
#define USE_MSG_Telemetry_TimeAndFix 0b01
...
#define USE_MSG_DriverControls_SpeedValCurrVal 0b10
#define USE_MSG_Telemetry_MpptPollJaved 0b01
#define USE_MSG_Telemetry_MpptPollWoof 0b01
#define PROCESS_ALL_MSG
```

Only enable messages whose names are unique (see [Known limitations](../README.md#known-limitations)).

## Using the API in firmware

The pattern used by Telemetry2021 `nick-22Library` (`Telemetry.ino`, `SensorInputs.cpp`):

```cpp
#include "src/CANApi/CANHelper.hpp"

CANHelper::CANHandler canHandler(10, CAN_125KBPS);    // MCP2515 chip select pin, bus speed (8 MHz crystal assumed)

void loop() {
  canHandler.read();                                   // polls the MCP2515; calls DispatchMsg() if a frame arrived
}

// Sending: fill a buffer through the message's struct, set its ID and DLC, send
CANHelper::CANHelperBuffer poll;
poll.payloadBuffer.as_Telemetry_MpptPollJaved.Blank = 0;
canHandler.setCanMeta(poll, CAN_META_Telemetry_MpptPollJaved);
canHandler.send(poll);

// Receiving: define a handler for each message with the read bit set ...
void CANHelper::CanMsgHandler::processMessage(CANHelper::Messages::DriverControls::SpeedValCurrVal& msg) {
  // msg.DriverSetSpeed, msg.DriverSetCurrent
}

// ... and processAll() if PROCESS_ALL_MSG is defined (the raw frame is msg.raw)
void CANHelper::CanMsgHandler::processAll(CANHelper::CANHelperBuffer& msg) {
  // e.g. forward msg.raw over the radio
}
```

(Check the chip select pin and bus speed for your board. Telemetry2021 `nick-22Library` passes `CAN_50KBPS`; the Peri firmware uses 125 kbps.)

The macros `LATEST_MSG`, `LATEST_MSG_ID`, `LATEST_MSG_DLC` and `LATEST_MSG_DATA` in `CANHelper.hpp` give the last received frame, its ID, its DLC and its payload union. They only work inside `CanMsgHandler`/`CANHandler` member functions, including `processMessage()` and `processAll()`.

## Files in this directory

| File | Status |
|---|---|
| [CanApiGen.py](CanApiGen.py) | **The generator.** |
| [excelMacroModule.vba](excelMacroModule.vba) | An exported copy of the VBA module inside `CANConfig.xlsm` (`CamelCase`, `CStruct`, `structUnpackCode`), so it can be read and diffed. Excel uses the copy in the workbook; if you change one, change the other (it matched the workbook in Nov 2026). |
| [CANHelper.hpp](CANHelper.hpp), [CANHelper.cpp](CANHelper.cpp) | An **older copy** of the hand-written half of CANApi. Out of date (see [Known limitations](../README.md#known-limitations)): use the files in the CANApi repository. |
| [ApiDrafts/](ApiDrafts) | Hand-written drafts of the v1.0 output format, made before the generator was written (June 2024). They do not compile and are kept for reference only. |
| [generatedAPIs/test.hpp, test.cpp](generatedAPIs) | Generator output from its first test run (7 June 2024). **Not current**: its `#if` lines read `(X & 0b10 == 2)`, which C++ evaluates as `X & (0b10 == 2)`, i.e. `X & 1`, so it tests the write bit instead of the read bit. It also has no `reverseBytes()` calls and uses the old `processAll(CastedCANPayload&)` signature. Compare with CANApi `v1.0-beta.4` instead. |
