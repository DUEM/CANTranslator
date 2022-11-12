#include "CANHelper.hpp"
namespace CANHelper::Messages
{
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno1
#define CAN_ID_Testbed_TestbedEnvirocanIno1 0x1B1
#define CAN_DLC_Testbed_TestbedEnvirocanIno1 8
	inline namespace Testbed
	{
		struct _TestbedEnvirocanIno1 : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float TempDegC;
				float Humidity;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Testbed::_TestbedEnvirocanIno1& msg);
#endif
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno2
#define CAN_ID_Testbed_TestbedEnvirocanIno2 0x1B2
#define CAN_DLC_Testbed_TestbedEnvirocanIno2 8
	inline namespace Testbed
	{
		struct _TestbedEnvirocanIno2 : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float HeatIndexDegC;
				int LdrReading;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Testbed::_TestbedEnvirocanIno2& msg);
#endif
#ifdef USE_MSG_DriverControls_SpeedValCurrVal
#define CAN_ID_DriverControls_SpeedValCurrVal 0x501
#define CAN_DLC_DriverControls_SpeedValCurrVal 8
	inline namespace DriverControls
	{
		struct _SpeedValCurrVal : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float DriverSetSpeed;
				float DriverSetCurrent;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(DriverControls::_SpeedValCurrVal& msg);
#endif
#ifdef USE_MSG_DriverControls_0Maxbuscurrent
#define CAN_ID_DriverControls_0Maxbuscurrent 0x502
#define CAN_DLC_DriverControls_0Maxbuscurrent 8
	inline namespace DriverControls
	{
		struct _0Maxbuscurrent : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Zero;
				float MaxBusCurrent;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(DriverControls::_0Maxbuscurrent& msg);
#endif
#ifdef USE_MSG_Tritium_IdentificationInfo
#define CAN_ID_Tritium_IdentificationInfo 0x600
#define CAN_DLC_Tritium_IdentificationInfo 8
	inline namespace Tritium
	{
		struct _IdentificationInfo : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				uint32_t SerialNumber;
				char* TritiumId;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_IdentificationInfo& msg);
#endif
#ifdef USE_MSG_Tritium_StausInformation
#define CAN_ID_Tritium_StausInformation 0x601
#define CAN_DLC_Tritium_StausInformation 8
	inline namespace Tritium
	{
		struct _StausInformation : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				uint16_t ActiveMotor;
				uint16_t ErrorFlags;
				uint16_t LimitFlags;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_StausInformation& msg);
#endif
#ifdef USE_MSG_Tritium_BusMeasurement
#define CAN_ID_Tritium_BusMeasurement 0x602
#define CAN_DLC_Tritium_BusMeasurement 8
	inline namespace Tritium
	{
		struct _BusMeasurement : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Buscurrent;
				float Busvoltage;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_BusMeasurement& msg);
#endif
#ifdef USE_MSG_Tritium_VelocityMSRpm
#define CAN_ID_Tritium_VelocityMSRpm 0x603
#define CAN_DLC_Tritium_VelocityMSRpm 8
	inline namespace Tritium
	{
		struct _VelocityMSRpm : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Vehiclevelocity;
				float Motorvelocity;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_VelocityMSRpm& msg);
#endif
#ifdef USE_MSG_Tritium_PhaseCurrent
#define CAN_ID_Tritium_PhaseCurrent 0x604
#define CAN_DLC_Tritium_PhaseCurrent 8
	inline namespace Tritium
	{
		struct _PhaseCurrent : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Phaseacurrent;
				float Phasebcurrent;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_PhaseCurrent& msg);
#endif
#ifdef USE_MSG_Tritium_MotorVoltageVector
#define CAN_ID_Tritium_MotorVoltageVector 0x605
#define CAN_DLC_Tritium_MotorVoltageVector 8
	inline namespace Tritium
	{
		struct _MotorVoltageVector : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Vectvoltreal;
				float Vectvoltimag;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_MotorVoltageVector& msg);
#endif
#ifdef USE_MSG_Tritium_MotorCurrentVector
#define CAN_ID_Tritium_MotorCurrentVector 0x606
#define CAN_DLC_Tritium_MotorCurrentVector 8
	inline namespace Tritium
	{
		struct _MotorCurrentVector : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Vectcurrreal;
				float Vectcurrimag;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_MotorCurrentVector& msg);
#endif
#ifdef USE_MSG_Tritium_MotorBackemfVector
#define CAN_ID_Tritium_MotorBackemfVector 0x607
#define CAN_DLC_Tritium_MotorBackemfVector 8
	inline namespace Tritium
	{
		struct _MotorBackemfVector : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Backemfreal;
				float Backemfimag;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_MotorBackemfVector& msg);
#endif
#ifdef USE_MSG_Tritium_15165VoltRail
#define CAN_ID_Tritium_15165VoltRail 0x608
#define CAN_DLC_Tritium_15165VoltRail 8
	inline namespace Tritium
	{
		struct _15165VoltRail : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Fifteenvsupply;
				float Onesixfivevsupply;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_15165VoltRail& msg);
#endif
#ifdef USE_MSG_Tritium_2512VoltRail
#define CAN_ID_Tritium_2512VoltRail 0x609
#define CAN_DLC_Tritium_2512VoltRail 8
	inline namespace Tritium
	{
		struct _2512VoltRail : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Twofivevsupply;
				float Onetwovsupply;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_2512VoltRail& msg);
#endif
#ifdef USE_MSG_Tritium_FanSpeedMeasrement
#define CAN_ID_Tritium_FanSpeedMeasrement 0x60A
#define CAN_DLC_Tritium_FanSpeedMeasrement 8
	inline namespace Tritium
	{
		struct _FanSpeedMeasrement : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Fanspeed;
				float Fandrive;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_FanSpeedMeasrement& msg);
#endif
#ifdef USE_MSG_Tritium_SinkMotorTemperature
#define CAN_ID_Tritium_SinkMotorTemperature 0x60B
#define CAN_DLC_Tritium_SinkMotorTemperature 8
	inline namespace Tritium
	{
		struct _SinkMotorTemperature : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Heatsinktemp;
				float Motortemp;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_SinkMotorTemperature& msg);
#endif
#ifdef USE_MSG_Tritium_AirInCpuTemperature
#define CAN_ID_Tritium_AirInCpuTemperature 0x60C
#define CAN_DLC_Tritium_AirInCpuTemperature 8
	inline namespace Tritium
	{
		struct _AirInCpuTemperature : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Airinlettemp;
				float Processortemp;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_AirInCpuTemperature& msg);
#endif
#ifdef USE_MSG_Tritium_AirOutCapTemperature
#define CAN_ID_Tritium_AirOutCapTemperature 0x60D
#define CAN_DLC_Tritium_AirOutCapTemperature 8
	inline namespace Tritium
	{
		struct _AirOutCapTemperature : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Airoutlettemp;
				float Capacitortemp;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_AirOutCapTemperature& msg);
#endif
#ifdef USE_MSG_Tritium_OdoBusAmphours
#define CAN_ID_Tritium_OdoBusAmphours 0x60E
#define CAN_DLC_Tritium_OdoBusAmphours 8
	inline namespace Tritium
	{
		struct _OdoBusAmphours : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				float Dcbusamphours;
				float Odometer;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Tritium::_OdoBusAmphours& msg);
#endif
#ifdef USE_MSG_Bms_CellVoltages
#define CAN_ID_Bms_CellVoltages 0x700
#define CAN_DLC_Bms_CellVoltages 6
	inline namespace Bms
	{
		struct _CellVoltages : public Messages::CANMsg {
			MsgMetadata metadata;
			struct canData
			{
				uint16_t HighCellVoltage;
				uint16_t LowCellVoltage;
				uint16_t AvgCellVoltage;
			} __attribute__((aligned(8)));
			canData data;
		}
	}
	processMessage(Bms::_CellVoltages& msg);
#endif
}
