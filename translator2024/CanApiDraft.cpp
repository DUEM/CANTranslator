#include "CANHelper.hpp"
namespace CANHelper
{
	void CANHandler::DispatchMsg(canid_t msgID)
	{
		switch(msgID)
		{
#ifdef USE_MSG_Telemetry_TimeAndFix
		case 0x0F6:
			Messages::processMessage(this->messageReadPayloadBuffer.as_Telemetry_TimeAndFix);
			break;
#endif
#ifdef USE_MSG_Telemetry_SpeedAndAngle
		case 0x0F7:
			Messages::processMessage(this->messageReadPayloadBuffer.);
			break;
#endif
#ifdef USE_MSG_Telemetry_Latitude
		case 0x0F8:
			Messages::processMessage(this->messageReadPayloadBuffer.);
			break;
#endif
		}
#ifdef PROCESS_ALL_MSG
		processAll((Messages::CANMsg&) msg);
#endif
	}
}
