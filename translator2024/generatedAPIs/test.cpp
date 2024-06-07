#include "test.hpp"
namespace CANHelper {
	void CanMsgHandler::DispatchMsg() {
		switch(LATEST_MSG_ID) {
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
#if defined(USE_MSG_Telemetry_Longitude) && (USE_MSG_Telemetry_Longitude & 0b10 == 2)
		case 0x0F9:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Telemetry_Longitude);
			break;
#endif
#if defined(USE_MSG_Telemetry_AltitudeAndSatellites) && (USE_MSG_Telemetry_AltitudeAndSatellites & 0b10 == 2)
		case 0x0FA:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Telemetry_AltitudeAndSatellites);
			break;
#endif
#if defined(USE_MSG_Telemetry_SystemStatusMessages) && (USE_MSG_Telemetry_SystemStatusMessages & 0b10 == 2)
		case 0x111:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Telemetry_SystemStatusMessages);
			break;
#endif
#if defined(USE_MSG_Testbed_TestbedEnvirocanIno) && (USE_MSG_Testbed_TestbedEnvirocanIno & 0b10 == 2)
		case 0x1B1:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Testbed_TestbedEnvirocanIno);
			break;
#endif
#if defined(USE_MSG_Testbed_TestbedEnvirocanIno) && (USE_MSG_Testbed_TestbedEnvirocanIno & 0b10 == 2)
		case 0x1B2:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Testbed_TestbedEnvirocanIno);
			break;
#endif
#if defined(USE_MSG_DriverControls_SpeedValCurrVal) && (USE_MSG_DriverControls_SpeedValCurrVal & 0b10 == 2)
		case 0x501:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_DriverControls_SpeedValCurrVal);
			break;
#endif
#if defined(USE_MSG_DriverControls_Maxbuscurrent) && (USE_MSG_DriverControls_Maxbuscurrent & 0b10 == 2)
		case 0x502:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_DriverControls_Maxbuscurrent);
			break;
#endif
#if defined(USE_MSG_Tritium_IdentificationInfo) && (USE_MSG_Tritium_IdentificationInfo & 0b10 == 2)
		case 0x600:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_IdentificationInfo);
			break;
#endif
#if defined(USE_MSG_Tritium_StausInformation) && (USE_MSG_Tritium_StausInformation & 0b10 == 2)
		case 0x601:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_StausInformation);
			break;
#endif
#if defined(USE_MSG_Tritium_BusMeasurement) && (USE_MSG_Tritium_BusMeasurement & 0b10 == 2)
		case 0x602:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_BusMeasurement);
			break;
#endif
#if defined(USE_MSG_Tritium_Velocity) && (USE_MSG_Tritium_Velocity & 0b10 == 2)
		case 0x603:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_Velocity);
			break;
#endif
#if defined(USE_MSG_Tritium_PhaseCurrent) && (USE_MSG_Tritium_PhaseCurrent & 0b10 == 2)
		case 0x604:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_PhaseCurrent);
			break;
#endif
#if defined(USE_MSG_Tritium_MotorVoltageVector) && (USE_MSG_Tritium_MotorVoltageVector & 0b10 == 2)
		case 0x605:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_MotorVoltageVector);
			break;
#endif
#if defined(USE_MSG_Tritium_MotorCurrentVector) && (USE_MSG_Tritium_MotorCurrentVector & 0b10 == 2)
		case 0x606:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_MotorCurrentVector);
			break;
#endif
#if defined(USE_MSG_Tritium_MotorBackemfVector) && (USE_MSG_Tritium_MotorBackemfVector & 0b10 == 2)
		case 0x607:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_MotorBackemfVector);
			break;
#endif
#if defined(USE_MSG_Tritium_Rail_15V) && (USE_MSG_Tritium_Rail_15V & 0b10 == 2)
		case 0x608:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_Rail_15V);
			break;
#endif
#if defined(USE_MSG_Tritium_Rail_3_3VAnd1_9V) && (USE_MSG_Tritium_Rail_3_3VAnd1_9V & 0b10 == 2)
		case 0x609:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_Rail_3_3VAnd1_9V);
			break;
#endif
#if defined(USE_MSG_Tritium_Reserved) && (USE_MSG_Tritium_Reserved & 0b10 == 2)
		case 0x60A:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_Reserved);
			break;
#endif
#if defined(USE_MSG_Tritium_SinkMotorTemperature) && (USE_MSG_Tritium_SinkMotorTemperature & 0b10 == 2)
		case 0x60B:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_SinkMotorTemperature);
			break;
#endif
#if defined(USE_MSG_Tritium_DspBoardTemperature) && (USE_MSG_Tritium_DspBoardTemperature & 0b10 == 2)
		case 0x60C:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_DspBoardTemperature);
			break;
#endif
#if defined(USE_MSG_Tritium_Reserved) && (USE_MSG_Tritium_Reserved & 0b10 == 2)
		case 0x60D:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_Reserved);
			break;
#endif
#if defined(USE_MSG_Tritium_OdoBusAmphours) && (USE_MSG_Tritium_OdoBusAmphours & 0b10 == 2)
		case 0x60E:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_OdoBusAmphours);
			break;
#endif
#if defined(USE_MSG_Tritium_SlipSpeedMeasurement) && (USE_MSG_Tritium_SlipSpeedMeasurement & 0b10 == 2)
		case 0x617:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Tritium_SlipSpeedMeasurement);
			break;
#endif
#if defined(USE_MSG_Orion_CellVoltages) && (USE_MSG_Orion_CellVoltages & 0b10 == 2)
		case 0x6B0:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Orion_CellVoltages);
			break;
#endif
#if defined(USE_MSG_Orion_CellTempsDischargeChageCurrentLimit) && (USE_MSG_Orion_CellTempsDischargeChageCurrentLimit & 0b10 == 2)
		case 0x6B1:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Orion_CellTempsDischargeChageCurrentLimit);
			break;
#endif
#if defined(USE_MSG_Orion_CellVoltages) && (USE_MSG_Orion_CellVoltages & 0b10 == 2)
		case 0x700:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Orion_CellVoltages);
			break;
#endif
#if defined(USE_MSG_Orion_CellTempsDischargeChageCurrentLimit) && (USE_MSG_Orion_CellTempsDischargeChageCurrentLimit & 0b10 == 2)
		case 0x701:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Orion_CellTempsDischargeChageCurrentLimit);
			break;
#endif
#if defined(USE_MSG_Orion_CellTempsDischargeChageCurrentLimit) && (USE_MSG_Orion_CellTempsDischargeChageCurrentLimit & 0b10 == 2)
		case 0x702:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Orion_CellTempsDischargeChageCurrentLimit);
			break;
#endif
#if defined(USE_MSG_Telemetry_MpptPollJaved) && (USE_MSG_Telemetry_MpptPollJaved & 0b10 == 2)
		case 0x711:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Telemetry_MpptPollJaved);
			break;
#endif
#if defined(USE_MSG_Telemetry_MpptPollWoof) && (USE_MSG_Telemetry_MpptPollWoof & 0b10 == 2)
		case 0x712:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Telemetry_MpptPollWoof);
			break;
#endif
#if defined(USE_MSG_Mppt_Javed) && (USE_MSG_Mppt_Javed & 0b10 == 2)
		case 0x771:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Mppt_Javed);
			break;
#endif
#if defined(USE_MSG_Mppt_Woof) && (USE_MSG_Mppt_Woof & 0b10 == 2)
		case 0x772:
			CanMsgHandler::processMessage(LATEST_MSG_DATA.as_Mppt_Woof);
			break;
#endif
		}
#ifdef PROCESS_ALL_MSG
		processAll(LATEST_MSG_DATA);
#endif
	}
}
