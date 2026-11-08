# API generator (ApiGen)

[CanApiGen.py](CanApiGen.py) reads the CAN message spreadsheet [config/CANConfig.xlsm](../config/CANConfig.xlsm) and writes the generated half of CANApi v1.0: a header (`.hpp`) with a C++ struct for every message, and an implementation (`.cpp`) with the code that sends each received message to the right handler.

The other half of CANApi, `CANHelper.hpp/.cpp`, is hand-written and lives in the [CANApi repository](https://github.com/DUEM/CANApi) (the [CANApi/](../CANApi) submodule). It wraps the MCP2515 CAN controller and provides `read()` and `send()`.

## Running

From the top of the repository, after `pip install -r ApiGen/requirements.txt`:

```shell
python3 ApiGen/CanApiGen.py -c config/CANConfig.xlsm -h CANApi/CanApiv10
```

| Option | Meaning |
|---|---|
| `-c`, `--configTable` | The spreadsheet to read |
| `-h`, `--headerStream` | Output path **without** an extension. Writes `<path>.hpp` and `<path>.cpp`, replacing any already there. (Not help.) |
| `--help` | Show the usage |

The header's include guard is named after the file (`CanApiv10_INCLUDE_GUARD`). The hand-written `CANHelper.hpp` includes `CanApiv10.hpp` by name, so **keep the name `CanApiv10`** unless you also change `CANHelper.hpp`.

The script lists each message it generates and each row it skips, then ends with `Generated ...: 31 messages, 4 skipped`. It exits with status 1 if the spreadsheet has problems, and 2 for a usage error.

Also regenerate the example copy, `-h ApiGen/generatedAPIs/test`, so that the tests pass (see [Files in this directory](#files-in-this-directory)).

## What it reads from the spreadsheet

It reads the first (active) sheet, `CAN Data`, from row 2 down to the row whose column A is `END`, plus the `DataTypes` sheet. For each row it uses:

| Column | Heading | Used for |
|---|---|---|
| A | `Active/Confirmed` | `Y` or `P`: generate. `N`: skip the row. |
| E | `CAN_ID` | The CAN ID, as written (e.g. `0x0F6`) |
| G | `DLC` | The number of data bytes |
| R | `data type` | The type of each field, giving its size and position (for the checks and for big-endian messages) |
| S | `Endianness` | `BIG`: swap the bytes of each field before the message is handled |
| U | `c struct` | The struct definition, built by the VBA function `CStruct` |
| W | `ItemCC` | The message name (e.g. `TimeAndFix`) |
| X | `SourceCC` | The sender name, used as a namespace (e.g. `Telemetry`) |
| Y to AF | `BYTE_0CC` to `BYTE_7CC` | The field names (to check column U) |

Columns U to AF are formulas. openpyxl cannot run them, so the script reads the values Excel saved with the file. See [config/README.md](../config/README.md) for all the columns.

## Checks

Before writing anything, the generator checks every row it is going to generate. If any check fails, it lists every problem (with the row number) and writes **nothing**, so a broken spreadsheet cannot overwrite a working API.

| Check | Why |
|---|---|
| `Active/Confirmed` is `Y`, `P` or `N`; there is an `END` row | So that it is clear which rows are generated |
| The CAN ID is written like `0x0F6` and is a standard 11-bit ID; the DLC is 1 to 8; the endianness is `BIG` or `LITTLE` | The values are copied into the C++ |
| The data types exist in the `DataTypes` sheet and add up to the DLC | Otherwise the struct does not match the message |
| The saved names (W, X, Y to AF) are what the formulas would give now, unless typed over | Finds a spreadsheet that has not been recalculated, or was saved by a program that cannot run the VBA |
| The saved struct (U) matches the data types and field names | Finds `#VALUE!` and out-of-date structs |
| `<SourceCC>_<ItemCC>` names, and CAN IDs, are unique; field names are unique within each message | Duplicates do not compile (or, for CAN IDs, give two `case`s with the same value) |

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
		} __attribute__((packed, aligned(8)));
		};
#endif
```

`packed` means there is no padding between fields on any compiler, so the struct always lines up with the CAN data bytes. (The AVR never adds padding, but 32-bit processors would, e.g. before a `uint16_t` that follows a `uint8_t`.) `aligned(8)` gives the struct the same alignment as `can_frame.data`.

then, after all the messages:

| Item | What it is |
|---|---|
| `CANHelper::Messages::CastedCANPayload` | A union of every enabled message struct, as members `as_<Name>`. Lets the same 8 data bytes be read as any message without copying. |
| `CANHelper::CANHelperBuffer` | A union of the MCP2515 `can_frame` (`.raw`) and the payload union (`.payloadBuffer`), lined up so that `payloadBuffer` is exactly `raw.data`. Use one of these for every message you send or receive. |
| `CANHelper::CanMsgHandler` | Base class holding the last received message (`messageRead`), with one `processMessage(Messages::<Source>::<Item>&)` declaration per message whose read bit is set, plus `processAll(CANHelperBuffer&)` if `PROCESS_ALL_MSG` is defined. |

**Implementation (`CanApiv10.cpp`)**: `CANHandler::DispatchMsg()`, which is called by `CANHandler::read()` for every received frame:

```cpp
switch(LATEST_MSG_ID) {
#if defined(USE_MSG_Telemetry_SpeedAndAngle) && (((USE_MSG_Telemetry_SpeedAndAngle + 0) & 0b10) == 2)
case 0x0F7:                                  // little-endian: handled in place
	this->processMessage(LATEST_MSG_DATA.as_Telemetry_SpeedAndAngle);
	break;
#endif
#if defined(USE_MSG_Orion_PackParameters) && (((USE_MSG_Orion_PackParameters + 0) & 0b10) == 2)
case 0x700: {                                // big-endian: each field swapped, in a copy
	Messages::CastedCANPayload swapped = LATEST_MSG_DATA;
	swapBytes(reinterpret_cast<__u8*>(&swapped) + 0, 2); //PackCurrent
	swapBytes(reinterpret_cast<__u8*>(&swapped) + 2, 2); //PackInstVoltage
	swapBytes(reinterpret_cast<__u8*>(&swapped) + 5, 2); //RelayState
	this->processMessage(swapped.as_Orion_PackParameters);
	break;
}
#endif
...
}
#ifdef PROCESS_ALL_MSG
this->processAll(LATEST_MSG);                // every frame, whatever its ID, exactly as received
#endif
```

For a big-endian message, only the bytes **within** each multi-byte field are reversed; the fields stay in order. It is done on a copy, so `processAll()` (which the telemetry board uses to relay frames over the radio) still gets the bytes as they came off the bus.

The generator only **declares** `processMessage()` and `processAll()`. The firmware must **define** every one it enables, or linking fails.

## Choosing messages (UseMsgList.hpp)

`CANHelper.hpp` includes `../../UseMsgList.hpp`, i.e. a file in the sketch folder, two levels above `src/CANApi/`. It defines one macro for each message the firmware uses, with a 2-bit value:

| Value | Meaning | Struct, `CAN_ID_`/`CAN_DLC_`/`CAN_META_` macros and `as_<Name>` | `processMessage()` and dispatch `case` |
|---|---|---|---|
| `0b01` (or no value) | Write only: the board sends this message, or just uses the struct | Yes | No |
| `0b10` | Read: the board handles this message when it receives it | Yes | Yes |
| `0b11` | Both | Yes | Yes |

The low (write) bit is not actually checked: any defined `USE_MSG_` macro produces the struct. A macro with no value (`#define USE_MSG_X`, as in the CanApiv04-era `UseMsgList.hpp` on the Telemetry2021 Peri branch) counts as write only.

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

The macros `LATEST_MSG`, `LATEST_MSG_ID`, `LATEST_MSG_DLC` and `LATEST_MSG_DATA` in `CANHelper.hpp` give the last received frame, its ID, its DLC and its payload union. They only work inside `CanMsgHandler`/`CANHandler` member functions, including `processMessage()` and `processAll()`. Note that for big-endian messages `LATEST_MSG_DATA` holds the bytes as received; the swapped values are in the `msg` passed to `processMessage()`.

`CANHandler::reverseBytes(buffer)` reverses the order of all `can_dlc` data bytes. The generated code does not use it (see above).

## Files in this directory

| File | Status |
|---|---|
| [CanApiGen.py](CanApiGen.py) | **The generator.** |
| [requirements.txt](requirements.txt) | Its Python requirements (openpyxl) |
| [excelMacroModule.vba](excelMacroModule.vba) | An exported copy of the VBA module inside `CANConfig.xlsm` (`CamelCase`, `CStruct`, `structUnpackCode`), so it can be read and diffed. Excel uses the copy in the workbook; if you change one, change the other (it matched the workbook in Nov 2026). |
| [ApiDrafts/](ApiDrafts) | Hand-written drafts of the v1.0 output format, made before the generator was written (June 2024). They do not compile and are kept for reference only. |
| [generatedAPIs/test.hpp, test.cpp](generatedAPIs) | An example of the generator's output, the same as `CANApi/CanApiv10.*` apart from the include guard, so that it can be read without checking out the submodule. The tests check it is up to date. |

The copy of `CANHelper.hpp/.cpp` that used to be here was out of date and has been removed: the files in CANApi are the ones used.
