#include "mcp2515.h"

namespace CANHelper::Messages
{
    using byte_t = unsigned char;

    class CANMsg
    {
    protected:
        byte_t data[CAN_MAX_DLEN]; //just set to max data size (defined in mcp2515.h). Avoids having to reallocate. Can just sit in memory until power is cut
    public:
        CANMsg(can_frame frame);
        CANMsg(); //for constructing and sending new CAN message
    };

    
    

    class Msg_2float32 : public CANMsg //decided to add definitions for each struct type returned to avoid repetitive code (and hopefully reduction in errors). Most CAN messages return 2 floats
    {
    public:
        using CANMsg::CANMsg; //use CANMsg constructor
        float& get0();
        float& get1();
    };

    class Msg_int16_2uint16_2uint8 : public CANMsg
    {
    public: //where get0 gets the variable at byte 0 and 1. get1 gets byte 2 and 3...
        using CANMsg::CANMsg;
        int16_t get0(); //short is 2 bytes on all arduinos. Int is 2 bytes on uno but 4 bytes on others (need to check that these types are valid as well)
        uint16_t get1();
        uint16_t get2();
        uint8_t get3();
        uint8_t get4(); //no processing should be necessary here. It should simply do a static cast to this datatype so that the compiler knows how to interpret the data. No actual extra instructions should be needed at runtime
    };
}