#include "CanApiv03.hpp"
namespace CANHelper
{
	void CanMsgHandler::DispatchMsg(can_frame msg)
	{
		switch(msg.can_id)
		{
#ifdef USE_MSG_Source_Item
		case CAN_ID:
			Messages::processMessage((Messages::Source::_Item&)msg);
			break;
#endif
#ifdef USE_MSG_Telemetry_TimeAndFix
		case 0x0F6:
			Messages::processMessage((Messages::Telemetry::_TimeAndFix&)msg);
			break;
#endif
#ifdef USE_MSG_Telemetry_SpeedAndAngle
		case 0x0F7:
			Messages::processMessage((Messages::Telemetry::_SpeedAndAngle&)msg);
			break;
#endif
#ifdef USE_MSG_Telemetry_Latitude
		case 0x0F8:
			Messages::processMessage((Messages::Telemetry::_Latitude&)msg);
			break;
#endif
#ifdef USE_MSG_Telemetry_Longitude
		case 0x0F9:
			Messages::processMessage((Messages::Telemetry::_Longitude&)msg);
			break;
#endif
#ifdef USE_MSG_Telemetry_AltitudeAndSatellites
		case 0x0FA:
			Messages::processMessage((Messages::Telemetry::_AltitudeAndSatellites&)msg);
			break;
#endif
#ifdef USE_MSG_Telemetry_SystemStatusMessages
		case 0x111:
			Messages::processMessage((Messages::Telemetry::_SystemStatusMessages&)msg);
			break;
#endif
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno1
		case 0x1B1:
			Messages::processMessage((Messages::Testbed::_TestbedEnvirocanIno1&)msg);
			break;
#endif
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno2
		case 0x1B2:
			Messages::processMessage((Messages::Testbed::_TestbedEnvirocanIno2&)msg);
			break;
#endif
#ifdef USE_MSG_DriverControls_SpeedValCurrVal
		case 0x501:
			Messages::processMessage((Messages::DriverControls::_SpeedValCurrVal&)msg);
			break;
#endif
#ifdef USE_MSG_DriverControls_0Maxbuscurrent
		case 0x502:
			Messages::processMessage((Messages::DriverControls::_0Maxbuscurrent&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_IdentificationInfo
		case 0x600:
			Messages::processMessage((Messages::Tritium::_IdentificationInfo&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_StausInformation
		case 0x601:
			Messages::processMessage((Messages::Tritium::_StausInformation&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_BusMeasurement
		case 0x602:
			Messages::processMessage((Messages::Tritium::_BusMeasurement&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_VelocityMSRpm
		case 0x603:
			Messages::processMessage((Messages::Tritium::_VelocityMSRpm&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_PhaseCurrent
		case 0x604:
			Messages::processMessage((Messages::Tritium::_PhaseCurrent&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_MotorVoltageVector
		case 0x605:
			Messages::processMessage((Messages::Tritium::_MotorVoltageVector&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_MotorCurrentVector
		case 0x606:
			Messages::processMessage((Messages::Tritium::_MotorCurrentVector&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_MotorBackemfVector
		case 0x607:
			Messages::processMessage((Messages::Tritium::_MotorBackemfVector&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_15165VoltRail
		case 0x608:
			Messages::processMessage((Messages::Tritium::_15165VoltRail&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_2512VoltRail
		case 0x609:
			Messages::processMessage((Messages::Tritium::_2512VoltRail&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_FanSpeedMeasrement
		case 0x60A:
			Messages::processMessage((Messages::Tritium::_FanSpeedMeasrement&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_SinkMotorTemperature
		case 0x60B:
			Messages::processMessage((Messages::Tritium::_SinkMotorTemperature&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_AirInCpuTemperature
		case 0x60C:
			Messages::processMessage((Messages::Tritium::_AirInCpuTemperature&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_AirOutCapTemperature
		case 0x60D:
			Messages::processMessage((Messages::Tritium::_AirOutCapTemperature&)msg);
			break;
#endif
#ifdef USE_MSG_Tritium_OdoBusAmphours
		case 0x60E:
			Messages::processMessage((Messages::Tritium::_OdoBusAmphours&)msg);
			break;
#endif
#ifdef USE_MSG_Bms_CellVoltages
		case 0x700:
			Messages::processMessage((Messages::Bms::_CellVoltages&)msg);
			break;
#endif
#ifdef USE_MSG_Bms_CellTempsDischargeChageCurrentLimit
		case 0x701:
			Messages::processMessage((Messages::Bms::_CellTempsDischargeChageCurrentLimit&)msg);
			break;
#endif
#ifdef USE_MSG_Bms_BmsMessage
		case 0x702:
			Messages::processMessage((Messages::Bms::_BmsMessage&)msg);
			break;
#endif
		}
#ifdef PROCESS_ALL_MSG
		processAll((Messages::CANMsg&) msg);
#endif
	}
}
