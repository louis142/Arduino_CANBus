#include "CANLib.h"

CANLib::CANLib(MCP2515& canbus) : _canbus(canbus) {}

void CANLib::SetLED(const int& Target, const int& LED, const bool& Status) {
  struct can_frame canSend;
  canSend.can_id = CAN_ID::SET_LED;
  canSend.can_dlc = 8; // Data size in bytes
  canSend.data[0] = 128; // Student code: 202114241
  canSend.data[1] = Target;//Target ID
  canSend.data[2] = LED;//LED ID
  canSend.data[3] = Status; // Status
  canSend.data[4] = 0x00; // ~
  canSend.data[5] = 0x00; // ~
  canSend.data[6] = 0x00; // ~
  canSend.data[7] = 0x00; // ~
  if(_send(canSend)){
    Serial.println("LED status sent");
  }
}

void CANLib::SetMultLED() {
  struct can_frame canSend;
  canSend.can_id = CAN_ID::SET_MULT_LED;
  canSend.can_dlc = 8; // Data size in bytes
  canSend.data[0] = 128; // Student code: 202114241
  canSend.data[1] = highByte(hbCount);//Count MSB
  canSend.data[2] = lowByte(hbCount);//Count LSB
  canSend.data[3] = 0x00; // ~
  canSend.data[4] = 0x00; // ~
  canSend.data[5] = 0x00; // ~
  canSend.data[6] = 0x00; // ~
  canSend.data[7] = 0x00; // ~
  if(_send(canSend)){
    Serial.println("Heartbeat sent");
  }
}

void CANLib::SetFan() {
  struct can_frame canSend;
  canSend.can_id = CAN_ID::SET_FAN;
  canSend.can_dlc = 8; // Data size in bytes
  canSend.data[0] = 128; // Student code: 202114241
  canSend.data[1] = highByte(hbCount);//Count MSB
  canSend.data[2] = lowByte(hbCount);//Count LSB
  canSend.data[3] = 0x00; // ~
  canSend.data[4] = 0x00; // ~
  canSend.data[5] = 0x00; // ~
  canSend.data[6] = 0x00; // ~
  canSend.data[7] = 0x00; // ~
  if(_send(canSend)){
    Serial.println("Heartbeat sent");
  }
}

void CANLib::SendHB() {
  struct can_frame canSend;
  canSend.can_id = CAN_ID::HEARTBEAT;
  canSend.can_dlc = 8; // Data size in bytes
  canSend.data[0] = 128; // Student code: 202114241
  canSend.data[1] = highByte(hbCount);//Count MSB
  canSend.data[2] = lowByte(hbCount);//Count LSB
  canSend.data[3] = 0x00; // ~
  canSend.data[4] = 0x00; // ~
  canSend.data[5] = 0x00; // ~
  canSend.data[6] = 0x00; // ~
  canSend.data[7] = 0x00; // ~
  if(_send(canSend)){
    Serial.print("Heartbeat #");
    Serial.print(hbCount);
    Serial.print(" sent!");
    Serial.println();
    hbCount++;
  }
}

bool CANLib::_send(const struct can_frame& sendframe){
  return (_canbus.sendMessage(&sendframe) == MCP2515::ERROR_OK);
}

void CANLib::MessageCheck(){
  struct can_frame canRecv;
  if (_canbus.readMessage(&canRecv) == MCP2515::ERROR_OK){
    switch(canRecv.can_id){
      case CAN_ID::BUTTON_PRESS:
        Serial.print("Received CAN from: ");
        Serial.print(canRecv.data[0]);
        Serial.print(" -> Button Press ");
        break;
      case CAN_ID::LED_STATUS:
        Serial.print("Received CAN from: ");
        Serial.print(canRecv.data[0]);
        Serial.print(" -> LED Status ");
        break;
      case CAN_ID::TEMPERATURE:
        Serial.print("Received CAN from: ");
        Serial.print(canRecv.data[0]);
        Serial.print(" -> Temperature ");
        break;
      case CAN_ID::FAN_SPEED:
        Serial.print("Received CAN from: ");
        Serial.print(canRecv.data[0]);
        Serial.print(" -> Fan speed ");
        break;
      case CAN_ID::HEARTBEAT:
        Serial.print("Received CAN from: ");
        Serial.print(canRecv.data[0]);
        Serial.print(" -> Heartbeat ");
        break;
    }
    Serial.println();
  }
}