// Minimal stand-in for the Arduino MCP2515 library (autowp/arduino-mcp2515), so that the
// generated API and CANHelper can be compiled and run on a laptop by the tests.
// Only what CANApi uses is declared; can_frame has the same layout as the real library.
#ifndef MCP2515_STUB_H
#define MCP2515_STUB_H
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t __u8;
typedef uint32_t __u32;
typedef __u32 canid_t;

#define CAN_MAX_DLEN 8

struct can_frame {
    canid_t can_id;
    __u8 can_dlc;
    __u8 data[CAN_MAX_DLEN] __attribute__((aligned(8)));
};

enum CAN_SPEED { CAN_50KBPS, CAN_125KBPS, CAN_500KBPS };
enum CAN_CLOCK { MCP_8MHZ, MCP_16MHZ };

class MCP2515 {
public:
    enum ERROR { ERROR_OK, ERROR_FAIL, ERROR_NOMSG };
    explicit MCP2515(int) {}
    ERROR setBitrate(CAN_SPEED, CAN_CLOCK) { return ERROR_OK; }
    ERROR setNormalMode() { return ERROR_OK; }
    ERROR readMessage(can_frame*) { return ERROR_NOMSG; }
    ERROR sendMessage(const can_frame*) { return ERROR_OK; }
};
#endif
