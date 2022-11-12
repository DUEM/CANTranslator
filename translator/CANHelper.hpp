#include "mcp2515.h"
#include "UseMsgList.hpp"

namespace CANHelper::Messages
{
    struct MsgMetadata
	{
		canid_t id;
		__u8 dlc;
        MsgMetadata(canid_t i, __u8 d) : id(i), dlc(d) {}
    };

    struct CANMsg {
        MsgMetadata metadata;
        CANMsg(canid_t i, __u8 d) : metadata(i, d) {}
    };//__attribute__((aligned(8))); //empty struct
}
namespace CANHelper
{
	class CanMsgHandler
	{
	private:
		MCP2515 mcp2515;
        can_frame messageRead;

        void DispatchMsg(can_frame msg); //internal function to cast and handle data
	public:
		CanMsgHandler(int CSPin);
        void read(); //should be in loop
        void testRead(can_frame& testFrame);
		void send(Messages::CANMsg& toSend);
	};
}