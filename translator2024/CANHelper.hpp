#include "UseMsgList.hpp"
#include "CanApiDraft.hpp"

#define LATEST_MSG_ID this->messageRead.can_id //NOTE: only can be used in member functions of CanMsgHandlerBase and CanMsgHander
                                                //(which extends CanMsgHandlerBase). This includes all the processMessage functions
#define LATEST_MSG_ID this->messageRead.can_dlc

namespace CANHelper
{

	class CANHandler : CanMsgHandler
	{
        union CANReadBuffer //to try and reduce memory copies (waste of instructions)
        {
            can_frame messageReadBuffer;
            char padding[offsetof(can_frame, can_frame.data)]; //aligns messageReadPayloadBuffer with can_frame.data (without memcpy)
		    Messages::CastedCANPayload messageReadPayloadBuffer;
        };
        
	protected:
		MCP2515 mcp2515;
        CANReadBuffer messageRead; //holds the latest received message from the CAN bus
	public:
		CANHandler(int CSPin, CAN_SPEED canSpeed);
        void read(); //should be in loop
        void testRead(can_frame& testFrame);
		void send(Messages::CANMsg& toSend);
        void send(can_frame& toSend);
        void DispatchMsg(canid_t msgID); //internal function to cast and handle data
	};
}