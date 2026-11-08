#ifndef test_INCLUDE_GUARD
#define test_INCLUDE_GUARD
#include "mcp2515.h"
namespace CANHelper::Messages {
#ifdef USE_MSG_Telemetry_TimeAndFix
#define CAN_ID_Telemetry_TimeAndFix 0x0F6
#define CAN_DLC_Telemetry_TimeAndFix 8
#define CAN_META_Telemetry_TimeAndFix {0x0F6, 8}
	namespace Telemetry {
		struct TimeAndFix {
			uint8_t GpsHour;
			uint8_t GpsMinute;
			uint8_t GpsSeconds;
			uint8_t GpsDay;
			uint8_t GpsMonth;
			uint8_t GpsYear;
			uint8_t GpsFix;
			uint8_t GpsFixquality;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Telemetry_SpeedAndAngle
#define CAN_ID_Telemetry_SpeedAndAngle 0x0F7
#define CAN_DLC_Telemetry_SpeedAndAngle 8
#define CAN_META_Telemetry_SpeedAndAngle {0x0F7, 8}
	namespace Telemetry {
		struct SpeedAndAngle {
			float GpsSpeed;
			float GpsAngle;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Telemetry_Latitude
#define CAN_ID_Telemetry_Latitude 0x0F8
#define CAN_DLC_Telemetry_Latitude 5
#define CAN_META_Telemetry_Latitude {0x0F8, 5}
	namespace Telemetry {
		struct Latitude {
			float GpsLatitude;
			char GpsLat;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Telemetry_Longitude
#define CAN_ID_Telemetry_Longitude 0x0F9
#define CAN_DLC_Telemetry_Longitude 5
#define CAN_META_Telemetry_Longitude {0x0F9, 5}
	namespace Telemetry {
		struct Longitude {
			float GpsLongitude;
			char GpsLon;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Telemetry_AltitudeAndSatellites
#define CAN_ID_Telemetry_AltitudeAndSatellites 0x0FA
#define CAN_DLC_Telemetry_AltitudeAndSatellites 5
#define CAN_META_Telemetry_AltitudeAndSatellites {0x0FA, 5}
	namespace Telemetry {
		struct AltitudeAndSatellites {
			float GpsAltitude;
			uint8_t GpsSatellites;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Telemetry_SystemStatusMessages
#define CAN_ID_Telemetry_SystemStatusMessages 0x111
#define CAN_DLC_Telemetry_SystemStatusMessages 8
#define CAN_META_Telemetry_SystemStatusMessages {0x111, 8}
	namespace Telemetry {
		struct SystemStatusMessages {
			uint8_t Power;
			uint8_t WritingToSd;
			uint8_t GpsTimeObtained;
			uint8_t LoadedConfig;
			uint8_t Flag;
			uint8_t Spare1;
			uint8_t Spare2;
			uint8_t Spare3;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_DriverControls_SpeedValCurrVal
#define CAN_ID_DriverControls_SpeedValCurrVal 0x501
#define CAN_DLC_DriverControls_SpeedValCurrVal 8
#define CAN_META_DriverControls_SpeedValCurrVal {0x501, 8}
	namespace DriverControls {
		struct SpeedValCurrVal {
			float DriverSetSpeed;
			float DriverSetCurrent;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_DriverControls_Maxbuscurrent
#define CAN_ID_DriverControls_Maxbuscurrent 0x502
#define CAN_DLC_DriverControls_Maxbuscurrent 8
#define CAN_META_DriverControls_Maxbuscurrent {0x502, 8}
	namespace DriverControls {
		struct Maxbuscurrent {
			float Zero;
			float MaxBusCurrent;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_IdentificationInfo
#define CAN_ID_Tritium_IdentificationInfo 0x600
#define CAN_DLC_Tritium_IdentificationInfo 8
#define CAN_META_Tritium_IdentificationInfo {0x600, 8}
	namespace Tritium {
		struct IdentificationInfo {
			uint32_t TritiumId;
			uint32_t SerialNumber;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_StatusInformation
#define CAN_ID_Tritium_StatusInformation 0x601
#define CAN_DLC_Tritium_StatusInformation 8
#define CAN_META_Tritium_StatusInformation {0x601, 8}
	namespace Tritium {
		struct StatusInformation {
			uint16_t LimitFlags;
			uint16_t ErrorFlags;
			uint16_t ActiveMotor;
			uint8_t TransmitErrorCount;
			uint8_t ReceiveErrorCount;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_BusMeasurement
#define CAN_ID_Tritium_BusMeasurement 0x602
#define CAN_DLC_Tritium_BusMeasurement 8
#define CAN_META_Tritium_BusMeasurement {0x602, 8}
	namespace Tritium {
		struct BusMeasurement {
			float BusVoltage;
			float BusCurrent;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_Velocity
#define CAN_ID_Tritium_Velocity 0x603
#define CAN_DLC_Tritium_Velocity 8
#define CAN_META_Tritium_Velocity {0x603, 8}
	namespace Tritium {
		struct Velocity {
			float MotorVelocity;
			float VehicleVelocity;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_PhaseCurrent
#define CAN_ID_Tritium_PhaseCurrent 0x604
#define CAN_DLC_Tritium_PhaseCurrent 8
#define CAN_META_Tritium_PhaseCurrent {0x604, 8}
	namespace Tritium {
		struct PhaseCurrent {
			float PhaseBCurrent;
			float PhaseCCurrent;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_MotorVoltageVector
#define CAN_ID_Tritium_MotorVoltageVector 0x605
#define CAN_DLC_Tritium_MotorVoltageVector 8
#define CAN_META_Tritium_MotorVoltageVector {0x605, 8}
	namespace Tritium {
		struct MotorVoltageVector {
			float VectVoltImag;
			float VectVoltReal;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_MotorCurrentVector
#define CAN_ID_Tritium_MotorCurrentVector 0x606
#define CAN_DLC_Tritium_MotorCurrentVector 8
#define CAN_META_Tritium_MotorCurrentVector {0x606, 8}
	namespace Tritium {
		struct MotorCurrentVector {
			float VectCurrImag;
			float VectCurrReal;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_MotorBackemfVector
#define CAN_ID_Tritium_MotorBackemfVector 0x607
#define CAN_DLC_Tritium_MotorBackemfVector 8
#define CAN_META_Tritium_MotorBackemfVector {0x607, 8}
	namespace Tritium {
		struct MotorBackemfVector {
			float PeakBackEmfPhaseToNeutral;
			float BackEmfD;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_Rail_15V
#define CAN_ID_Tritium_Rail_15V 0x608
#define CAN_DLC_Tritium_Rail_15V 8
#define CAN_META_Tritium_Rail_15V {0x608, 8}
	namespace Tritium {
		struct Rail_15V {
			float Reserved;
			float FifteenVSupply;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_Rail_3_3VAnd1_9V
#define CAN_ID_Tritium_Rail_3_3VAnd1_9V 0x609
#define CAN_DLC_Tritium_Rail_3_3VAnd1_9V 8
#define CAN_META_Tritium_Rail_3_3VAnd1_9V {0x609, 8}
	namespace Tritium {
		struct Rail_3_3VAnd1_9V {
			float OneNineVSupply;
			float ThreeThreeVSupply;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_SinkMotorTemperature
#define CAN_ID_Tritium_SinkMotorTemperature 0x60B
#define CAN_DLC_Tritium_SinkMotorTemperature 8
#define CAN_META_Tritium_SinkMotorTemperature {0x60B, 8}
	namespace Tritium {
		struct SinkMotorTemperature {
			float MotorTemp;
			float HeatSinkTemp;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_DspBoardTemperature
#define CAN_ID_Tritium_DspBoardTemperature 0x60C
#define CAN_DLC_Tritium_DspBoardTemperature 8
#define CAN_META_Tritium_DspBoardTemperature {0x60C, 8}
	namespace Tritium {
		struct DspBoardTemperature {
			float DspBoardTemp;
			float Reserved;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_OdoBusAmphours
#define CAN_ID_Tritium_OdoBusAmphours 0x60E
#define CAN_DLC_Tritium_OdoBusAmphours 8
#define CAN_META_Tritium_OdoBusAmphours {0x60E, 8}
	namespace Tritium {
		struct OdoBusAmphours {
			float Odometer;
			float DcBusAmpHours;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Tritium_SlipSpeedMeasurement
#define CAN_ID_Tritium_SlipSpeedMeasurement 0x617
#define CAN_DLC_Tritium_SlipSpeedMeasurement 8
#define CAN_META_Tritium_SlipSpeedMeasurement {0x617, 8}
	namespace Tritium {
		struct SlipSpeedMeasurement {
			float Reserved;
			float SlipSpeed;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Orion_PackParametersDefaultId
#define CAN_ID_Orion_PackParametersDefaultId 0x6B0
#define CAN_DLC_Orion_PackParametersDefaultId 7
#define CAN_META_Orion_PackParametersDefaultId {0x6B0, 7}
	namespace Orion {
		struct PackParametersDefaultId {
			uint16_t PackCurrent;
			uint16_t PackInstVoltage;
			uint8_t PackSoc;
			uint16_t RelayState;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Orion_CellTempsDclDefaultId
#define CAN_ID_Orion_CellTempsDclDefaultId 0x6B1
#define CAN_DLC_Orion_CellTempsDclDefaultId 7
#define CAN_META_Orion_CellTempsDclDefaultId {0x6B1, 7}
	namespace Orion {
		struct CellTempsDclDefaultId {
			int16_t PackDcl;
			int16_t Unknown;
			int8_t HighTemperatureSensor;
			int8_t LowTemperatureSensors;
			int8_t Blank;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Orion_PackParameters
#define CAN_ID_Orion_PackParameters 0x700
#define CAN_DLC_Orion_PackParameters 7
#define CAN_META_Orion_PackParameters {0x700, 7}
	namespace Orion {
		struct PackParameters {
			uint16_t PackCurrent;
			uint16_t PackInstVoltage;
			uint8_t PackSoc;
			uint16_t RelayState;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Orion_CellTempsDcl
#define CAN_ID_Orion_CellTempsDcl 0x701
#define CAN_DLC_Orion_CellTempsDcl 7
#define CAN_META_Orion_CellTempsDcl {0x701, 7}
	namespace Orion {
		struct CellTempsDcl {
			int16_t PackDcl;
			int16_t Unknown;
			int8_t HighTemperatureSensor;
			int8_t LowTemperatureSensors;
			int8_t Blank;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Orion_CellVoltages
#define CAN_ID_Orion_CellVoltages 0x702
#define CAN_DLC_Orion_CellVoltages 7
#define CAN_META_Orion_CellVoltages {0x702, 7}
	namespace Orion {
		struct CellVoltages {
			int8_t MinCellVoltage;
			int8_t MaxCellVoltage;
			int8_t PackKwPower;
			int8_t ReqFanSpeed;
			int8_t IntakeTemperature;
			int8_t BlankA;
			int8_t BlankB;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Telemetry_MpptPollJaved
#define CAN_ID_Telemetry_MpptPollJaved 0x711
#define CAN_DLC_Telemetry_MpptPollJaved 1
#define CAN_META_Telemetry_MpptPollJaved {0x711, 1}
	namespace Telemetry {
		struct MpptPollJaved {
			int8_t Blank;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Telemetry_MpptPollWoof
#define CAN_ID_Telemetry_MpptPollWoof 0x712
#define CAN_DLC_Telemetry_MpptPollWoof 1
#define CAN_META_Telemetry_MpptPollWoof {0x712, 1}
	namespace Telemetry {
		struct MpptPollWoof {
			int8_t Blank;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Mppt_Javed
#define CAN_ID_Mppt_Javed 0x771
#define CAN_DLC_Mppt_Javed 7
#define CAN_META_Mppt_Javed {0x771, 7}
	namespace Mppt {
		struct Javed {
			uint8_t FlagsAndMsbVoltageIn;
			uint8_t LsbVoltageIn;
			uint8_t MsbCurrentIn;
			uint8_t LsbCurrentIn;
			uint8_t MsbVoltageOut;
			uint8_t LsbVoltageOut;
			uint8_t AmbientTemperature;
		} __attribute__((aligned(8)));
		};
#endif
#ifdef USE_MSG_Mppt_Woof
#define CAN_ID_Mppt_Woof 0x772
#define CAN_DLC_Mppt_Woof 7
#define CAN_META_Mppt_Woof {0x772, 7}
	namespace Mppt {
		struct Woof {
			uint8_t FlagsAndMsbVoltageIn;
			uint8_t LsbVoltageIn;
			uint8_t MsbCurrentIn;
			uint8_t LsbCurrentIn;
			uint8_t MsbVoltageOut;
			uint8_t LsbVoltageOut;
			uint8_t AmbientTemperature;
		} __attribute__((aligned(8)));
		};
#endif
	union CastedCANPayload {
#if defined(USE_MSG_Telemetry_TimeAndFix)
		Telemetry::TimeAndFix as_Telemetry_TimeAndFix;
#endif
#if defined(USE_MSG_Telemetry_SpeedAndAngle)
		Telemetry::SpeedAndAngle as_Telemetry_SpeedAndAngle;
#endif
#if defined(USE_MSG_Telemetry_Latitude)
		Telemetry::Latitude as_Telemetry_Latitude;
#endif
#if defined(USE_MSG_Telemetry_Longitude)
		Telemetry::Longitude as_Telemetry_Longitude;
#endif
#if defined(USE_MSG_Telemetry_AltitudeAndSatellites)
		Telemetry::AltitudeAndSatellites as_Telemetry_AltitudeAndSatellites;
#endif
#if defined(USE_MSG_Telemetry_SystemStatusMessages)
		Telemetry::SystemStatusMessages as_Telemetry_SystemStatusMessages;
#endif
#if defined(USE_MSG_DriverControls_SpeedValCurrVal)
		DriverControls::SpeedValCurrVal as_DriverControls_SpeedValCurrVal;
#endif
#if defined(USE_MSG_DriverControls_Maxbuscurrent)
		DriverControls::Maxbuscurrent as_DriverControls_Maxbuscurrent;
#endif
#if defined(USE_MSG_Tritium_IdentificationInfo)
		Tritium::IdentificationInfo as_Tritium_IdentificationInfo;
#endif
#if defined(USE_MSG_Tritium_StatusInformation)
		Tritium::StatusInformation as_Tritium_StatusInformation;
#endif
#if defined(USE_MSG_Tritium_BusMeasurement)
		Tritium::BusMeasurement as_Tritium_BusMeasurement;
#endif
#if defined(USE_MSG_Tritium_Velocity)
		Tritium::Velocity as_Tritium_Velocity;
#endif
#if defined(USE_MSG_Tritium_PhaseCurrent)
		Tritium::PhaseCurrent as_Tritium_PhaseCurrent;
#endif
#if defined(USE_MSG_Tritium_MotorVoltageVector)
		Tritium::MotorVoltageVector as_Tritium_MotorVoltageVector;
#endif
#if defined(USE_MSG_Tritium_MotorCurrentVector)
		Tritium::MotorCurrentVector as_Tritium_MotorCurrentVector;
#endif
#if defined(USE_MSG_Tritium_MotorBackemfVector)
		Tritium::MotorBackemfVector as_Tritium_MotorBackemfVector;
#endif
#if defined(USE_MSG_Tritium_Rail_15V)
		Tritium::Rail_15V as_Tritium_Rail_15V;
#endif
#if defined(USE_MSG_Tritium_Rail_3_3VAnd1_9V)
		Tritium::Rail_3_3VAnd1_9V as_Tritium_Rail_3_3VAnd1_9V;
#endif
#if defined(USE_MSG_Tritium_SinkMotorTemperature)
		Tritium::SinkMotorTemperature as_Tritium_SinkMotorTemperature;
#endif
#if defined(USE_MSG_Tritium_DspBoardTemperature)
		Tritium::DspBoardTemperature as_Tritium_DspBoardTemperature;
#endif
#if defined(USE_MSG_Tritium_OdoBusAmphours)
		Tritium::OdoBusAmphours as_Tritium_OdoBusAmphours;
#endif
#if defined(USE_MSG_Tritium_SlipSpeedMeasurement)
		Tritium::SlipSpeedMeasurement as_Tritium_SlipSpeedMeasurement;
#endif
#if defined(USE_MSG_Orion_PackParametersDefaultId)
		Orion::PackParametersDefaultId as_Orion_PackParametersDefaultId;
#endif
#if defined(USE_MSG_Orion_CellTempsDclDefaultId)
		Orion::CellTempsDclDefaultId as_Orion_CellTempsDclDefaultId;
#endif
#if defined(USE_MSG_Orion_PackParameters)
		Orion::PackParameters as_Orion_PackParameters;
#endif
#if defined(USE_MSG_Orion_CellTempsDcl)
		Orion::CellTempsDcl as_Orion_CellTempsDcl;
#endif
#if defined(USE_MSG_Orion_CellVoltages)
		Orion::CellVoltages as_Orion_CellVoltages;
#endif
#if defined(USE_MSG_Telemetry_MpptPollJaved)
		Telemetry::MpptPollJaved as_Telemetry_MpptPollJaved;
#endif
#if defined(USE_MSG_Telemetry_MpptPollWoof)
		Telemetry::MpptPollWoof as_Telemetry_MpptPollWoof;
#endif
#if defined(USE_MSG_Mppt_Javed)
		Mppt::Javed as_Mppt_Javed;
#endif
#if defined(USE_MSG_Mppt_Woof)
		Mppt::Woof as_Mppt_Woof;
#endif
	};
}
namespace CANHelper {
	union CANHelperBuffer { //to try and reduce memory copies (waste of instructions)
		can_frame raw;
		struct {
			char padding[offsetof(can_frame, data)]; //aligns payloadBuffer with can_frame.data (without memcpy)
			Messages::CastedCANPayload payloadBuffer;
		};
	};

	class CanMsgHandler {
	protected:
		CANHelperBuffer messageRead; //holds the latest received message from the CAN bus
	public:
		CanMsgHandler() = default;
#if defined(USE_MSG_Telemetry_TimeAndFix) && ((USE_MSG_Telemetry_TimeAndFix & 0b10) == 2)
		void processMessage(Messages::Telemetry::TimeAndFix& msg);
#endif
#if defined(USE_MSG_Telemetry_SpeedAndAngle) && ((USE_MSG_Telemetry_SpeedAndAngle & 0b10) == 2)
		void processMessage(Messages::Telemetry::SpeedAndAngle& msg);
#endif
#if defined(USE_MSG_Telemetry_Latitude) && ((USE_MSG_Telemetry_Latitude & 0b10) == 2)
		void processMessage(Messages::Telemetry::Latitude& msg);
#endif
#if defined(USE_MSG_Telemetry_Longitude) && ((USE_MSG_Telemetry_Longitude & 0b10) == 2)
		void processMessage(Messages::Telemetry::Longitude& msg);
#endif
#if defined(USE_MSG_Telemetry_AltitudeAndSatellites) && ((USE_MSG_Telemetry_AltitudeAndSatellites & 0b10) == 2)
		void processMessage(Messages::Telemetry::AltitudeAndSatellites& msg);
#endif
#if defined(USE_MSG_Telemetry_SystemStatusMessages) && ((USE_MSG_Telemetry_SystemStatusMessages & 0b10) == 2)
		void processMessage(Messages::Telemetry::SystemStatusMessages& msg);
#endif
#if defined(USE_MSG_DriverControls_SpeedValCurrVal) && ((USE_MSG_DriverControls_SpeedValCurrVal & 0b10) == 2)
		void processMessage(Messages::DriverControls::SpeedValCurrVal& msg);
#endif
#if defined(USE_MSG_DriverControls_Maxbuscurrent) && ((USE_MSG_DriverControls_Maxbuscurrent & 0b10) == 2)
		void processMessage(Messages::DriverControls::Maxbuscurrent& msg);
#endif
#if defined(USE_MSG_Tritium_IdentificationInfo) && ((USE_MSG_Tritium_IdentificationInfo & 0b10) == 2)
		void processMessage(Messages::Tritium::IdentificationInfo& msg);
#endif
#if defined(USE_MSG_Tritium_StatusInformation) && ((USE_MSG_Tritium_StatusInformation & 0b10) == 2)
		void processMessage(Messages::Tritium::StatusInformation& msg);
#endif
#if defined(USE_MSG_Tritium_BusMeasurement) && ((USE_MSG_Tritium_BusMeasurement & 0b10) == 2)
		void processMessage(Messages::Tritium::BusMeasurement& msg);
#endif
#if defined(USE_MSG_Tritium_Velocity) && ((USE_MSG_Tritium_Velocity & 0b10) == 2)
		void processMessage(Messages::Tritium::Velocity& msg);
#endif
#if defined(USE_MSG_Tritium_PhaseCurrent) && ((USE_MSG_Tritium_PhaseCurrent & 0b10) == 2)
		void processMessage(Messages::Tritium::PhaseCurrent& msg);
#endif
#if defined(USE_MSG_Tritium_MotorVoltageVector) && ((USE_MSG_Tritium_MotorVoltageVector & 0b10) == 2)
		void processMessage(Messages::Tritium::MotorVoltageVector& msg);
#endif
#if defined(USE_MSG_Tritium_MotorCurrentVector) && ((USE_MSG_Tritium_MotorCurrentVector & 0b10) == 2)
		void processMessage(Messages::Tritium::MotorCurrentVector& msg);
#endif
#if defined(USE_MSG_Tritium_MotorBackemfVector) && ((USE_MSG_Tritium_MotorBackemfVector & 0b10) == 2)
		void processMessage(Messages::Tritium::MotorBackemfVector& msg);
#endif
#if defined(USE_MSG_Tritium_Rail_15V) && ((USE_MSG_Tritium_Rail_15V & 0b10) == 2)
		void processMessage(Messages::Tritium::Rail_15V& msg);
#endif
#if defined(USE_MSG_Tritium_Rail_3_3VAnd1_9V) && ((USE_MSG_Tritium_Rail_3_3VAnd1_9V & 0b10) == 2)
		void processMessage(Messages::Tritium::Rail_3_3VAnd1_9V& msg);
#endif
#if defined(USE_MSG_Tritium_SinkMotorTemperature) && ((USE_MSG_Tritium_SinkMotorTemperature & 0b10) == 2)
		void processMessage(Messages::Tritium::SinkMotorTemperature& msg);
#endif
#if defined(USE_MSG_Tritium_DspBoardTemperature) && ((USE_MSG_Tritium_DspBoardTemperature & 0b10) == 2)
		void processMessage(Messages::Tritium::DspBoardTemperature& msg);
#endif
#if defined(USE_MSG_Tritium_OdoBusAmphours) && ((USE_MSG_Tritium_OdoBusAmphours & 0b10) == 2)
		void processMessage(Messages::Tritium::OdoBusAmphours& msg);
#endif
#if defined(USE_MSG_Tritium_SlipSpeedMeasurement) && ((USE_MSG_Tritium_SlipSpeedMeasurement & 0b10) == 2)
		void processMessage(Messages::Tritium::SlipSpeedMeasurement& msg);
#endif
#if defined(USE_MSG_Orion_PackParametersDefaultId) && ((USE_MSG_Orion_PackParametersDefaultId & 0b10) == 2)
		void processMessage(Messages::Orion::PackParametersDefaultId& msg);
#endif
#if defined(USE_MSG_Orion_CellTempsDclDefaultId) && ((USE_MSG_Orion_CellTempsDclDefaultId & 0b10) == 2)
		void processMessage(Messages::Orion::CellTempsDclDefaultId& msg);
#endif
#if defined(USE_MSG_Orion_PackParameters) && ((USE_MSG_Orion_PackParameters & 0b10) == 2)
		void processMessage(Messages::Orion::PackParameters& msg);
#endif
#if defined(USE_MSG_Orion_CellTempsDcl) && ((USE_MSG_Orion_CellTempsDcl & 0b10) == 2)
		void processMessage(Messages::Orion::CellTempsDcl& msg);
#endif
#if defined(USE_MSG_Orion_CellVoltages) && ((USE_MSG_Orion_CellVoltages & 0b10) == 2)
		void processMessage(Messages::Orion::CellVoltages& msg);
#endif
#if defined(USE_MSG_Telemetry_MpptPollJaved) && ((USE_MSG_Telemetry_MpptPollJaved & 0b10) == 2)
		void processMessage(Messages::Telemetry::MpptPollJaved& msg);
#endif
#if defined(USE_MSG_Telemetry_MpptPollWoof) && ((USE_MSG_Telemetry_MpptPollWoof & 0b10) == 2)
		void processMessage(Messages::Telemetry::MpptPollWoof& msg);
#endif
#if defined(USE_MSG_Mppt_Javed) && ((USE_MSG_Mppt_Javed & 0b10) == 2)
		void processMessage(Messages::Mppt::Javed& msg);
#endif
#if defined(USE_MSG_Mppt_Woof) && ((USE_MSG_Mppt_Woof & 0b10) == 2)
		void processMessage(Messages::Mppt::Woof& msg);
#endif
#ifdef PROCESS_ALL_MSG
		void processAll(CANHelperBuffer& msg);
#endif
	};
}
#endif
