#include "CANHelper.hpp"

namespace CANHelper
{
    CANHandler::CANHandler(int CSPin, CAN_SPEED canSpeed) : mcp2515(CSPin) //initialiser list to pass specified CSPin to MCP2515 class
    {
        this->mcp2515.setBitrate(canSpeed, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz (CHECK THIS)
        this->mcp2515.setNormalMode();
        this->messageReadPayloadBuffer.data = this->messageRead.data; //note messageReadPayloadBuffer.data is of type __u8& (i.e. reference)
                                                                      //Therefore this assignment is only needed once
    }
    
    //void CanMsgHandler::DispatchMsg(can_frame msg) //handled in generated file

    void CANHandler::read()
    {
        if (this->mcp2515.readMessage(&(this->messageRead.messageReadBuffer)) == MCP2515::ERROR_OK) { //To receive data (Poll Read). If true, message was received //https://how2electronics.com/interfacing-mcp2515-can-bus-module-with-arduino/
            this->DispatchMsg(this->messageRead.messageReadBuffer.can_id);
        }
    }

    void CANHandler::testRead(can_frame& testFrame)
    {
      memcpy(&(this->messageRead.messageReadBuffer), &testFrame, sizeof(can_frame));
      this->DispatchMsg(this->messageRead.messageReadBuffer.can_id);
    }
    
    void CANHandler::send(Messages::CANMsg& toSend)
    {
        this->mcp2515.sendMessage((can_frame*)&toSend);
    }

    void CANHandler::send(can_frame& toSend)
    {
        this->mcp2515.sendMessage(&toSend);
    }
}