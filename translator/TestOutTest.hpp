#include "structDefinitions.hpp"
#include "mcp2515.h"
namespace CANHelper
{
	class CanMsgHandler
	{
	private:
		MCP2515 canCtrl;
		bool messageReceived;
	public:
		CanMsgHandler(int CSPin) : canCtrl(CSPin) {}
		void DispatchMsg(can_frame msg);
		void send(Messages::CANMsg& toSend);
		void interruptServiceRoutine(); //ISR to use with attachInterrupt. I think interrupts are a better way than constant polling
	};
}
namespace CANHelper::Messages
{
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno
	inline namespace Testbed
	{
		class _TestbedEnvirocanIno : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto TempDegC() { return this->get0(); }
			inline auto Humidity() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Testbed_TestbedEnvirocanIno
	inline namespace Testbed
	{
		class _TestbedEnvirocanIno : public MSG_float32_int32
		{
		public:
			using MSG_float32_int32::MSG_float32_int32;
			inline auto HeatIndexDegC() { return this->get0(); }
			inline auto LdrReading() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_DriverControls_SpeedValCurrVal
	inline namespace DriverControls
	{
		class _SpeedValCurrVal : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto DriverSetSpeed() { return this->get0(); }
			inline auto DriverSetCurrent() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_DriverControls_0Maxbuscurrent
	inline namespace DriverControls
	{
		class _0Maxbuscurrent : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Zero() { return this->get0(); }
			inline auto MaxBusCurrent() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_IdentificationInfo
	inline namespace Tritium
	{
		class _IdentificationInfo : public MSG_uint32_char4
		{
		public:
			using MSG_uint32_char4::MSG_uint32_char4;
			inline auto SerialNumber() { return this->get0(); }
			inline auto TritiumId() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_StausInformation
	inline namespace Tritium
	{
		class _StausInformation : public MSG_3uint16
		{
		public:
			using MSG_3uint16::MSG_3uint16;
			inline auto ActiveMotor() { return this->get0(); }
			inline auto ErrorFlags() { return this->get1(); }
			inline auto LimitFlags() { return this->get2(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_BusMeasurement
	inline namespace Tritium
	{
		class _BusMeasurement : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Buscurrent() { return this->get0(); }
			inline auto Busvoltage() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_VelocityMSRpm
	inline namespace Tritium
	{
		class _VelocityMSRpm : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Vehiclevelocity() { return this->get0(); }
			inline auto Motorvelocity() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_PhaseCurrent
	inline namespace Tritium
	{
		class _PhaseCurrent : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Phaseacurrent() { return this->get0(); }
			inline auto Phasebcurrent() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_MotorVoltageVector
	inline namespace Tritium
	{
		class _MotorVoltageVector : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Vectvoltreal() { return this->get0(); }
			inline auto Vectvoltimag() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_MotorCurrentVector
	inline namespace Tritium
	{
		class _MotorCurrentVector : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Vectcurrreal() { return this->get0(); }
			inline auto Vectcurrimag() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_MotorBackemfVector
	inline namespace Tritium
	{
		class _MotorBackemfVector : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Backemfreal() { return this->get0(); }
			inline auto Backemfimag() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_15165VoltRail
	inline namespace Tritium
	{
		class _15165VoltRail : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Fifteenvsupply() { return this->get0(); }
			inline auto Onesixfivevsupply() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_2512VoltRail
	inline namespace Tritium
	{
		class _2512VoltRail : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Twofivevsupply() { return this->get0(); }
			inline auto Onetwovsupply() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_FanSpeedMeasrement
	inline namespace Tritium
	{
		class _FanSpeedMeasrement : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Fanspeed() { return this->get0(); }
			inline auto Fandrive() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_SinkMotorTemperature
	inline namespace Tritium
	{
		class _SinkMotorTemperature : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Heatsinktemp() { return this->get0(); }
			inline auto Motortemp() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_AirInCpuTemperature
	inline namespace Tritium
	{
		class _AirInCpuTemperature : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Airinlettemp() { return this->get0(); }
			inline auto Processortemp() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_AirOutCapTemperature
	inline namespace Tritium
	{
		class _AirOutCapTemperature : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Airoutlettemp() { return this->get0(); }
			inline auto Capacitortemp() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Tritium_OdoBusAmphours
	inline namespace Tritium
	{
		class _OdoBusAmphours : public MSG_2float32
		{
		public:
			using MSG_2float32::MSG_2float32;
			inline auto Dcbusamphours() { return this->get0(); }
			inline auto Odometer() { return this->get1(); }
		};
	}
#endif
#ifdef USE_MSG_Bms_CellVoltages
	inline namespace Bms
	{
		class _CellVoltages : public MSG_3uint16
		{
		public:
			using MSG_3uint16::MSG_3uint16;
			inline auto HighCellVoltage() { return this->get0(); }
			inline auto LowCellVoltage() { return this->get1(); }
			inline auto AvgCellVoltage() { return this->get2(); }
		};
	}
#endif
}
