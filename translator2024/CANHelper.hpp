#include "UseMsgList.hpp"
#include "CanApiDraft.hpp"

#define LATEST_MSG_ID this->messageRead.raw.can_id //NOTE: only can be used in member functions of CanMsgHandlerBase and CanMsgHander
                                                //(which extends CanMsgHandlerBase). This includes all the processMessage functions
#define LATEST_MSG_ID this->messageRead.raw.can_dlc
#define LATEST_MSG_DATA this->messageRead.payloadBuffer

namespace CANHelper {
	struct canMeta { //used in send functions to declare id and dlc for buffer
        canid_t can_id;
        __u8    can_dlc;
    };

	class CANHandler : public CanMsgHandler {   
	private:
		MCP2515 mcp2515;
	public:
		CANHandler(int CSPin, CAN_SPEED canSpeed);
        void read(); //should be in loop. Polls the can controller for new messages
        void testRead(can_frame& testFrame);
		void send(CANHelperBuffer& toSend);
        void send(can_frame& toSend);
		void setCanMeta(CANHelperBuffer& buffer, canMeta meta);
        void DispatchMsg(); //internal function to cast and handle data
	};
}