//Draft of the format of what will be the generated API header
#ifndef CanApiv04_INCLUDE_GUARD
#define CanApiv04_INCLUDE_GUARD
#include "mcp2515.h" //for certain datatypes used in this file

namespace CANHelper::Messages //union containing all messages is at bottom
{
#ifdef USE_MSG_Telemetry_TimeAndFix
#define CAN_ID_Telemetry_TimeAndFix 0x0F6
#define CAN_DLC_Telemetry_TimeAndFix 8
	struct Telemetry::_TimeAndFix { //Python code will remove underscores from item names (just havnt changed them here)
		uint8_t GpsHour;
		uint8_t GpsMinute;
		uint8_t GpsSeconds;
		uint8_t GpsDay;
		uint8_t GpsMonth;
		uint8_t GpsYear;
		uint8_t GpsFix;
		uint8_t GpsFixquality;
	} __attribute__((aligned(8))); //data address padding. For arduino avr compiler this is the syntax to control it
#endif
#ifdef USE_MSG_Telemetry_SpeedAndAngle
#define CAN_ID_Telemetry_SpeedAndAngle 0x0F7
#define CAN_DLC_Telemetry_SpeedAndAngle 8
	struct Telemetry::_SpeedAndAngle {
		float GpsSpeed;
		float GpsAngle;
	} __attribute__((aligned(8)));
#endif
#ifdef USE_MSG_Telemetry_Latitude
#define CAN_ID_Telemetry_Latitude 0x0F8
#define CAN_DLC_Telemetry_Latitude 5
	struct Telemetry::_Latitude {
		float GpsLatitude;
		char GpsLat;
	} __attribute__((aligned(8)));
#endif
	union CastedCANPayload
    {
		__u8 data[CAN_MAX_DLEN] __attribute__((aligned(8))); //generic buffer
#ifdef USE_MSG_Telemetry_TimeAndFix && (USE_MSG_Telemetry_TimeAndFix & 0b01 == 1)
        Telemetry::_TimeAndFix as_Telemetry_TimeAndFix;
#endif
#ifdef USE_MSG_Telemetry_SpeedAndAngle && (USE_MSG_Telemetry_SpeedAndAngle & 0b01 == 1)
		Telemetry::_SpeedAndAngle as_Telemetry_SpeedAndAngle;
#endif
#ifdef USE_MSG_Telemetry_Latitude && (USE_MSG_Telemetry_Latitude & 0b01 == 1)
		Telemetry::_Latitude as_Telemetry_Latitude;
#endif
    };
}
#endif

namespace CANHelper {
	union CANHelperBuffer { //to try and reduce memory copies (waste of instructions)
		can_frame raw;
		struct {
			char padding[offsetof(can_frame, can_frame.data)]; //aligns payloadBuffer with can_frame.data (without memcpy)
			Messages::CastedCANPayload payloadBuffer;
		};
	};
	
	class CanMsgHandler {
	protected:
		CANHelperBuffer messageRead; //holds the latest received message from the CAN bus
	public:
		CanMsgHandler() = default;

        //all processMessage functions go here
#ifdef PROCESS_ALL_MSG
		void processAll(Messages::CastedCANPayload& msg);
#endif
#if defined(USE_MSG_Telemetry_TimeAndFix) && (USE_MSG_Telemetry_TimeAndFix & 0b10 == 2)
		void processMessage(Messages::Telemetry::TimeAndFix& msg);
#endif
#if defined(USE_MSG_Telemetry_SpeedAndAngle) && (USE_MSG_Telemetry_SpeedAndAngle & 0b10 == 2)
		void processMessage(Messages::Telemetry::SpeedAndAngle& msg);
#endif
#if defined(USE_MSG_Telemetry_Latitude) && (USE_MSG_Telemetry_Latitude & 0b10 == 2)
		void processMessage(Messages::Telemetry::Latitude& msg);
#endif
	};
}
