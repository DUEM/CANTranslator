#include "CANHelper.hpp"
namespace CANHelper
{ //Each USE_MSG macro has a 2 bit value. MSB is read enable (adds processMessage definition), LSB is write enable (adds struct only)
	void CANHandler::DispatchMsg()
	{
		switch(this->messageRead.messageReadBuffer.can_id)
		{
#if defined(USE_MSG_Telemetry_TimeAndFix) && (USE_MSG_Telemetry_TimeAndFix & 0b10 == 2)
		case 0x0F6:
			Messages::processMessage(this->messageRead.payloadBuffer.as_Telemetry_TimeAndFix);
			break;
#endif
#if defined(USE_MSG_Telemetry_SpeedAndAngle) && (USE_MSG_Telemetry_SpeedAndAngle & 0b10 == 2)
		case 0x0F7:
			Messages::processMessage(this->messageRead.payloadBuffer.as_Telemetry_SpeedAndAngle);
			break;
#endif
#if defined(USE_MSG_Telemetry_Latitude) && (USE_MSG_Telemetry_Latitude & 0b10 == 2)
		case 0x0F8:
			Messages::processMessage(this->messageRead.payloadBuffer.as_Telemetry_Latitude);
			break;
#endif
		}
#ifdef PROCESS_ALL_MSG
		processAll((Messages::CANMsg&) msg);
#endif
	}
}
