#include "UseMsgList.hpp"
#include "CanApiDraft.hpp"

#define LATEST_MSG_ID this->messageRead.raw.can_id //NOTE: only can be used in member functions of CanMsgHandlerBase and CanMsgHander
                                                //(which extends CanMsgHandlerBase). This includes all the processMessage functions
#define LATEST_MSG_ID this->messageRead.raw.can_dlc

namespace CANHelper {
	class CANHandler : CanMsgHandler {   
	private:
		MCP2515 mcp2515;
	public:
		CANHandler(int CSPin, CAN_SPEED canSpeed);
        void read(); //should be in loop. Polls the can controller for new messages
        void testRead(can_frame& testFrame);
		void send(CANHelperBuffer& toSend);
        void send(can_frame& toSend);
        void DispatchMsg(); //internal function to cast and handle data
	};
}