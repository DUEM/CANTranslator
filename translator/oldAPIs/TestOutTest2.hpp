#include "CANHelper.hpp"
#define USE_MSG_Testbed_TestbedEnvirocanIno
namespace CANHelper::Messages
{
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno
#define CAN_ID_Testbed_TestbedEnvirocanIno 0x1B1
#define CAN_DLC_Testbed_TestbedEnvirocanIno 8
	inline namespace Testbed
	{
		/*struct _TestbedEnvirocanIno //: public Messages::CANMsg {
		{
			can_frame frame;
			struct castedFrame
			{
				MsgMetadata metadata;
				struct payload
				{
					float TempDegC;
					float Humidity;
				} __attribute__((aligned(8)));
			};
		};*/
		struct _TestbedEnvirocanIno //: public Messages::CANMsg {
		{
			MsgMetadata metadata;
			struct canData
			{
				float TempDegC;
				float Humidity;
			} __attribute__((aligned(8)));
			canData data;
		};
	}
	processMessage(Testbed::_TestbedEnvirocanIno& msg);
#endif
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno
#define CAN_ID_Testbed_TestbedEnvirocanIno 0x1B2
#define CAN_DLC_Testbed_TestbedEnvirocanIno 8
	inline namespace Testbed
	{
		struct _TestbedEnvirocanIno : public Messages::CANMsg {
			float HeatIndexDegC;
			int LdrReading;
			inline can_id id() { return CAN_ID_Testbed_TestbedEnvirocanIno; }
			inline __u8 dlc() { return CAN_DLC_Testbed_TestbedEnvirocanIno; }
		} __attribute__((aligned(8)));
		
	}
	processMessage(Testbed::_TestbedEnvirocanIno& msg);
#endif
#ifdef USE_MSG_DriverControls_SpeedValCurrVal
#define CAN_ID_DriverControls_SpeedValCurrVal 0x501
#define CAN_DLC_DriverControls_SpeedValCurrVal 8
	inline namespace DriverControls
	{
		struct _SpeedValCurrVal : public Messages::CANMsg {
			float DriverSetSpeed;
			float DriverSetCurrent;
			inline can_id id() { return CAN_ID_DriverControls_SpeedValCurrVal; }
			inline __u8 dlc() { return CAN_DLC_DriverControls_SpeedValCurrVal; }
		} __attribute__((aligned(8)));
	}
	processMessage(DriverControls::_SpeedValCurrVal& msg);
#endif
#ifdef USE_MSG_DriverControls_0Maxbuscurrent
#define CAN_ID_DriverControls_0Maxbuscurrent 0x502
#define CAN_DLC_DriverControls_0Maxbuscurrent 8
	inline namespace DriverControls
	{
		struct _0Maxbuscurrent : public Messages::CANMsg {
			float Zero;
			float MaxBusCurrent;
			inline can_id id() { return CAN_ID_DriverControls_0Maxbuscurrent; }
			inline __u8 dlc() { return CAN_DLC_DriverControls_0Maxbuscurrent; }
		} __attribute__((aligned(8)));
	}
	processMessage(DriverControls::_0Maxbuscurrent& msg);
#endif
#ifdef USE_MSG_Tritium_IdentificationInfo
#define CAN_ID_Tritium_IdentificationInfo 0x600
#define CAN_DLC_Tritium_IdentificationInfo 8
	inline namespace Tritium
	{
		struct _IdentificationInfo : public Messages::CANMsg {
			uint32_t SerialNumber;
			char* TritiumId;
			inline can_id id() { return CAN_ID_Tritium_IdentificationInfo; }
			inline __u8 dlc() { return CAN_DLC_Tritium_IdentificationInfo; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_IdentificationInfo& msg);
#endif
#ifdef USE_MSG_Tritium_StausInformation
#define CAN_ID_Tritium_StausInformation 0x601
#define CAN_DLC_Tritium_StausInformation 8
	inline namespace Tritium
	{
		struct _StausInformation : public Messages::CANMsg {
			uint16_t ActiveMotor;
			uint16_t ErrorFlags;
			uint16_t LimitFlags;
			inline can_id id() { return CAN_ID_Tritium_StausInformation; }
			inline __u8 dlc() { return CAN_DLC_Tritium_StausInformation; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_StausInformation& msg);
#endif
#ifdef USE_MSG_Tritium_BusMeasurement
#define CAN_ID_Tritium_BusMeasurement 0x602
#define CAN_DLC_Tritium_BusMeasurement 8
	inline namespace Tritium
	{
		struct _BusMeasurement : public Messages::CANMsg {
			float Buscurrent;
			float Busvoltage;
			inline can_id id() { return CAN_ID_Tritium_BusMeasurement; }
			inline __u8 dlc() { return CAN_DLC_Tritium_BusMeasurement; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_BusMeasurement& msg);
#endif
#ifdef USE_MSG_Tritium_VelocityMSRpm
#define CAN_ID_Tritium_VelocityMSRpm 0x603
#define CAN_DLC_Tritium_VelocityMSRpm 8
	inline namespace Tritium
	{
		struct _VelocityMSRpm : public Messages::CANMsg {
			float Vehiclevelocity;
			float Motorvelocity;
			inline can_id id() { return CAN_ID_Tritium_VelocityMSRpm; }
			inline __u8 dlc() { return CAN_DLC_Tritium_VelocityMSRpm; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_VelocityMSRpm& msg);
#endif
#ifdef USE_MSG_Tritium_PhaseCurrent
#define CAN_ID_Tritium_PhaseCurrent 0x604
#define CAN_DLC_Tritium_PhaseCurrent 8
	inline namespace Tritium
	{
		struct _PhaseCurrent : public Messages::CANMsg {
			float Phaseacurrent;
			float Phasebcurrent;
			inline can_id id() { return CAN_ID_Tritium_PhaseCurrent; }
			inline __u8 dlc() { return CAN_DLC_Tritium_PhaseCurrent; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_PhaseCurrent& msg);
#endif
#ifdef USE_MSG_Tritium_MotorVoltageVector
#define CAN_ID_Tritium_MotorVoltageVector 0x605
#define CAN_DLC_Tritium_MotorVoltageVector 8
	inline namespace Tritium
	{
		struct _MotorVoltageVector : public Messages::CANMsg {
			float Vectvoltreal;
			float Vectvoltimag;
			inline can_id id() { return CAN_ID_Tritium_MotorVoltageVector; }
			inline __u8 dlc() { return CAN_DLC_Tritium_MotorVoltageVector; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_MotorVoltageVector& msg);
#endif
#ifdef USE_MSG_Tritium_MotorCurrentVector
#define CAN_ID_Tritium_MotorCurrentVector 0x606
#define CAN_DLC_Tritium_MotorCurrentVector 8
	inline namespace Tritium
	{
		struct _MotorCurrentVector : public Messages::CANMsg {
			float Vectcurrreal;
			float Vectcurrimag;
			inline can_id id() { return CAN_ID_Tritium_MotorCurrentVector; }
			inline __u8 dlc() { return CAN_DLC_Tritium_MotorCurrentVector; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_MotorCurrentVector& msg);
#endif
#ifdef USE_MSG_Tritium_MotorBackemfVector
#define CAN_ID_Tritium_MotorBackemfVector 0x607
#define CAN_DLC_Tritium_MotorBackemfVector 8
	inline namespace Tritium
	{
		struct _MotorBackemfVector : public Messages::CANMsg {
			float Backemfreal;
			float Backemfimag;
			inline can_id id() { return CAN_ID_Tritium_MotorBackemfVector; }
			inline __u8 dlc() { return CAN_DLC_Tritium_MotorBackemfVector; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_MotorBackemfVector& msg);
#endif
#ifdef USE_MSG_Tritium_15165VoltRail
#define CAN_ID_Tritium_15165VoltRail 0x608
#define CAN_DLC_Tritium_15165VoltRail 8
	inline namespace Tritium
	{
		struct _15165VoltRail : public Messages::CANMsg {
			float Fifteenvsupply;
			float Onesixfivevsupply;
			inline can_id id() { return CAN_ID_Tritium_15165VoltRail; }
			inline __u8 dlc() { return CAN_DLC_Tritium_15165VoltRail; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_15165VoltRail& msg);
#endif
#ifdef USE_MSG_Tritium_2512VoltRail
#define CAN_ID_Tritium_2512VoltRail 0x609
#define CAN_DLC_Tritium_2512VoltRail 8
	inline namespace Tritium
	{
		struct _2512VoltRail : public Messages::CANMsg {
			float Twofivevsupply;
			float Onetwovsupply;
			inline can_id id() { return CAN_ID_Tritium_2512VoltRail; }
			inline __u8 dlc() { return CAN_DLC_Tritium_2512VoltRail; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_2512VoltRail& msg);
#endif
#ifdef USE_MSG_Tritium_FanSpeedMeasrement
#define CAN_ID_Tritium_FanSpeedMeasrement 0x60A
#define CAN_DLC_Tritium_FanSpeedMeasrement 8
	inline namespace Tritium
	{
		struct _FanSpeedMeasrement : public Messages::CANMsg {
			float Fanspeed;
			float Fandrive;
			inline can_id id() { return CAN_ID_Tritium_FanSpeedMeasrement; }
			inline __u8 dlc() { return CAN_DLC_Tritium_FanSpeedMeasrement; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_FanSpeedMeasrement& msg);
#endif
#ifdef USE_MSG_Tritium_SinkMotorTemperature
#define CAN_ID_Tritium_SinkMotorTemperature 0x60B
#define CAN_DLC_Tritium_SinkMotorTemperature 8
	inline namespace Tritium
	{
		struct _SinkMotorTemperature : public Messages::CANMsg {
			float Heatsinktemp;
			float Motortemp;
			inline can_id id() { return CAN_ID_Tritium_SinkMotorTemperature; }
			inline __u8 dlc() { return CAN_DLC_Tritium_SinkMotorTemperature; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_SinkMotorTemperature& msg);
#endif
#ifdef USE_MSG_Tritium_AirInCpuTemperature
#define CAN_ID_Tritium_AirInCpuTemperature 0x60C
#define CAN_DLC_Tritium_AirInCpuTemperature 8
	inline namespace Tritium
	{
		struct _AirInCpuTemperature : public Messages::CANMsg {
			float Airinlettemp;
			float Processortemp;
			inline can_id id() { return CAN_ID_Tritium_AirInCpuTemperature; }
			inline __u8 dlc() { return CAN_DLC_Tritium_AirInCpuTemperature; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_AirInCpuTemperature& msg);
#endif
#ifdef USE_MSG_Tritium_AirOutCapTemperature
#define CAN_ID_Tritium_AirOutCapTemperature 0x60D
#define CAN_DLC_Tritium_AirOutCapTemperature 8
	inline namespace Tritium
	{
		struct _AirOutCapTemperature : public Messages::CANMsg {
			float Airoutlettemp;
			float Capacitortemp;
			inline can_id id() { return CAN_ID_Tritium_AirOutCapTemperature; }
			inline __u8 dlc() { return CAN_DLC_Tritium_AirOutCapTemperature; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_AirOutCapTemperature& msg);
#endif
#ifdef USE_MSG_Tritium_OdoBusAmphours
#define CAN_ID_Tritium_OdoBusAmphours 0x60E
#define CAN_DLC_Tritium_OdoBusAmphours 8
	inline namespace Tritium
	{
		struct _OdoBusAmphours : public Messages::CANMsg {
			float Dcbusamphours;
			float Odometer;
			inline can_id id() { return CAN_ID_Tritium_OdoBusAmphours; }
			inline __u8 dlc() { return CAN_DLC_Tritium_OdoBusAmphours; }
		} __attribute__((aligned(8)));
	}
	processMessage(Tritium::_OdoBusAmphours& msg);
#endif
#ifdef USE_MSG_Bms_CellVoltages
#define CAN_ID_Bms_CellVoltages 0x700
#define CAN_DLC_Bms_CellVoltages 6
	inline namespace Bms
	{
		struct _CellVoltages : public Messages::CANMsg {
			uint16_t HighCellVoltage;
			uint16_t LowCellVoltage;
			uint16_t AvgCellVoltage;
			inline can_id id() { return CAN_ID_Bms_CellVoltages; }
			inline __u8 dlc() { return CAN_DLC_Bms_CellVoltages; }
		} __attribute__((aligned(8)));
	}
	processMessage(Bms::_CellVoltages& msg);
#endif
}
