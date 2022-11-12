#include "structDefinitions.hpp"

namespace CANHelper::Messages
{
    CANMsg::CANMsg(can_frame frame)
    {
        this->data = frame.data;
    }

    float& Msg_2float32::get0()
    {
        //return (float)(((int64_t)(this->data)) & 0b11110000) //first float. Would like to
        //return *((float)(this->data[0])); //will need testing. Hoping it just takes the trailing 3 bytes since float is size of 4
        return (float*)(this->data);
    }

    float& Msg_2float32::get1()
    {
        //return (float)(((int64_t)(this->data)) & 0b00001111); //second float
        return *((float)(this->data[4])); //Don't want to use pointer due to differences in syntax. So will try a reference first.
    }
}
