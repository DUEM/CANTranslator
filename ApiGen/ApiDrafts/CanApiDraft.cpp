//Draft of the format of what will be the generated API implementation
#include "CANHelper.hpp"
namespace CANHelper
{ //Each USE_MSG macro has a 2 bit value. MSB is read enable (adds processMessage definition), LSB is write enable (adds struct only)
	void CANHandler::DispatchMsg()
	{
		switch(LATEST_MSG_ID)
		{
#if defined(USE_MSG_Telemetry_TimeAndFix) && (USE_MSG_Telemetry_TimeAndFix & 0b10 == 2)
		case 0x0F6:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Telemetry_TimeAndFix);
			break;
#endif
#if defined(USE_MSG_Telemetry_SpeedAndAngle) && (USE_MSG_Telemetry_SpeedAndAngle & 0b10 == 2)
		case 0x0F7:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Telemetry_SpeedAndAngle);
			break;
#endif
#if defined(USE_MSG_Telemetry_Latitude) && (USE_MSG_Telemetry_Latitude & 0b10 == 2)
		case 0x0F8:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Telemetry_Latitude);
			break;
#endif
		}
#ifdef PROCESS_ALL_MSG
		processAll(LATEST_MSG);
#endif
	}
}
