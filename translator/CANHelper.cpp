#include "CANHelper.hpp"

namespace CANHelper
{
    CanMsgHandler::CanMsgHandler(int CSPin) : mcp2515(CSPin) //initialiser list to pass specified CSPin to MCP2515 class
    {
        this->mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz (CHECK THIS)
        this->mcp2515.setNormalMode();
    }
    
    //void CanMsgHandler::DispatchMsg(can_frame msg) //handled in generated file

    void CanMsgHandler::read()
    {
        if (this->mcp2515.readMessage(&(this->messageRead)) == MCP2515::ERROR_OK) { //To receive data (Poll Read). If true, message was received //https://how2electronics.com/interfacing-mcp2515-can-bus-module-with-arduino/
            this->DispatchMsg(this->messageRead);
        }
    }

    void CanMsgHandler::testRead(can_frame& testFrame)
    {
      memcpy(&(this->messageRead), &testFrame, sizeof(can_frame));
      this->DispatchMsg(this->messageRead);
    }
    
    void CanMsgHandler::send(Messages::CANMsg& toSend)
    {
        this->mcp2515.sendMessage((can_frame*)&toSend);
    }
}