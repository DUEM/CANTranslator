#include "TestOut.hpp"
namespace CANHelper
{
	void CanMsgHandler::DispatchMsg(can_frame msg)
	{
		switch(msg.can_id)
		{
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno1
		case 0x1B1:
			Messages::Testbed::_TestbedEnvirocanIno1::processMessage((Testbed::_TestbedEnvirocanIno1&)msg);
#endif
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno2
		case 0x1B2:
			Messages::Testbed::_TestbedEnvirocanIno2::processMessage((Testbed::_TestbedEnvirocanIno2&)msg);
#endif
#ifdef USE_MSG_DriverControls_SpeedValCurrVal
		case 0x501:
			Messages::DriverControls::_SpeedValCurrVal::processMessage((DriverControls::_SpeedValCurrVal&)msg);
#endif
#ifdef USE_MSG_DriverControls_0Maxbuscurrent
		case 0x502:
			Messages::DriverControls::_0Maxbuscurrent::processMessage((DriverControls::_0Maxbuscurrent&)msg);
#endif
#ifdef USE_MSG_Tritium_IdentificationInfo
		case 0x600:
			Messages::Tritium::_IdentificationInfo::processMessage((Tritium::_IdentificationInfo&)msg);
#endif
#ifdef USE_MSG_Tritium_StausInformation
		case 0x601:
			Messages::Tritium::_StausInformation::processMessage((Tritium::_StausInformation&)msg);
#endif
#ifdef USE_MSG_Tritium_BusMeasurement
		case 0x602:
			Messages::Tritium::_BusMeasurement::processMessage((Tritium::_BusMeasurement&)msg);
#endif
#ifdef USE_MSG_Tritium_VelocityMSRpm
		case 0x603:
			Messages::Tritium::_VelocityMSRpm::processMessage((Tritium::_VelocityMSRpm&)msg);
#endif
#ifdef USE_MSG_Tritium_PhaseCurrent
		case 0x604:
			Messages::Tritium::_PhaseCurrent::processMessage((Tritium::_PhaseCurrent&)msg);
#endif
#ifdef USE_MSG_Tritium_MotorVoltageVector
		case 0x605:
			Messages::Tritium::_MotorVoltageVector::processMessage((Tritium::_MotorVoltageVector&)msg);
#endif
#ifdef USE_MSG_Tritium_MotorCurrentVector
		case 0x606:
			Messages::Tritium::_MotorCurrentVector::processMessage((Tritium::_MotorCurrentVector&)msg);
#endif
#ifdef USE_MSG_Tritium_MotorBackemfVector
		case 0x607:
			Messages::Tritium::_MotorBackemfVector::processMessage((Tritium::_MotorBackemfVector&)msg);
#endif
#ifdef USE_MSG_Tritium_15165VoltRail
		case 0x608:
			Messages::Tritium::_15165VoltRail::processMessage((Tritium::_15165VoltRail&)msg);
#endif
#ifdef USE_MSG_Tritium_2512VoltRail
		case 0x609:
			Messages::Tritium::_2512VoltRail::processMessage((Tritium::_2512VoltRail&)msg);
#endif
#ifdef USE_MSG_Tritium_FanSpeedMeasrement
		case 0x60A:
			Messages::Tritium::_FanSpeedMeasrement::processMessage((Tritium::_FanSpeedMeasrement&)msg);
#endif
#ifdef USE_MSG_Tritium_SinkMotorTemperature
		case 0x60B:
			Messages::Tritium::_SinkMotorTemperature::processMessage((Tritium::_SinkMotorTemperature&)msg);
#endif
#ifdef USE_MSG_Tritium_AirInCpuTemperature
		case 0x60C:
			Messages::Tritium::_AirInCpuTemperature::processMessage((Tritium::_AirInCpuTemperature&)msg);
#endif
#ifdef USE_MSG_Tritium_AirOutCapTemperature
		case 0x60D:
			Messages::Tritium::_AirOutCapTemperature::processMessage((Tritium::_AirOutCapTemperature&)msg);
#endif
#ifdef USE_MSG_Tritium_OdoBusAmphours
		case 0x60E:
			Messages::Tritium::_OdoBusAmphours::processMessage((Tritium::_OdoBusAmphours&)msg);
#endif
#ifdef USE_MSG_Bms_CellVoltages
		case 0x700:
			Messages::Bms::_CellVoltages::processMessage((Bms::_CellVoltages&)msg);
#endif
		}
	}
}
