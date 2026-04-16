#include "CANLib.h"

CANLib::CANLib(MCP2515& canbus)
  : _canbus(canbus) {}

void CANLib::SetLED(const int& Target, const int& LED, const bool& Status) {
  struct can_frame canSend;
  canSend.can_id = CAN_ID::SET_LED;
  canSend.can_dlc = 8;       // Data size in bytes
  canSend.data[0] = 128;     // Student code: 202114241
  canSend.data[1] = Target;  //Target ID
  canSend.data[2] = LED;     //LED ID
  canSend.data[3] = Status;  // Status
  canSend.data[4] = 0x00;    // ~
  canSend.data[5] = 0x00;    // ~
  canSend.data[6] = 0x00;    // ~
  canSend.data[7] = 0x00;    // ~
  if (_send(canSend)) {
    Serial.print("Sent CAN to node #");
    Serial.print(Target);
    Serial.print(" -> switch LED #");
    Serial.print(LED);
    Serial.print(Status ? " on" : " off");
    Serial.println();
  }
}

void CANLib::SetMultLED(const int& Target, const uint8_t& LEDBank0, const uint8_t& LEDBank1) {
  struct can_frame canSend;
  canSend.can_id = CAN_ID::SET_MULT_LED;
  canSend.can_dlc = 8;         // Data size in bytes
  canSend.data[0] = 128;       // Student code: 202114241
  canSend.data[1] = Target;    //Target ID
  canSend.data[2] = LEDBank0;  //LEDs 0-7
  canSend.data[3] = LEDBank1;  //LEDs 8-15
  canSend.data[4] = 0x00;      // ~
  canSend.data[5] = 0x00;      // ~
  canSend.data[6] = 0x00;      // ~
  canSend.data[7] = 0x00;      // ~
  if (_send(canSend)) {
    Serial.print("Sent CAN to node #");
    Serial.print(Target);
    Serial.print(" -> switch LED bank");
    Serial.println();
  }
}

void CANLib::SetFan(const int& Target, const int& Speed) {
  struct can_frame canSend;
  canSend.can_id = CAN_ID::SET_FAN;
  canSend.can_dlc = 8;                // Data size in bytes
  canSend.data[0] = 128;              // Student code: 202114241
  canSend.data[1] = Target;           //Target ID
  canSend.data[2] = highByte(Speed);  //Count MSB
  canSend.data[3] = lowByte(Speed);   //Count LSB
  canSend.data[4] = 0x00;             // ~
  canSend.data[5] = 0x00;             // ~
  canSend.data[6] = 0x00;             // ~
  canSend.data[7] = 0x00;             // ~
  if (_send(canSend)) {
    Serial.print("Sent CAN to node #");
    Serial.print(Target);
    Serial.print(" -> set fan speed to ");
    Serial.print(Speed);
    Serial.println();
  }
}

void CANLib::SendHB() {
  struct can_frame canSend;
  canSend.can_id = CAN_ID::HEARTBEAT;
  canSend.can_dlc = 8;                  // Data size in bytes
  canSend.data[0] = 128;                // Student code: 202114241
  canSend.data[1] = highByte(hbCount);  //Count MSB
  canSend.data[2] = lowByte(hbCount);   //Count LSB
  canSend.data[3] = 0x00;               // ~
  canSend.data[4] = 0x00;               // ~
  canSend.data[5] = 0x00;               // ~
  canSend.data[6] = 0x00;               // ~
  canSend.data[7] = 0x00;               // ~
  if (_send(canSend)) {
    // Serial.print("Heartbeat #");
    // Serial.print(hbCount);
    // Serial.print(" sent!");
    // Serial.println();
    hbCount++;
  }
}

bool CANLib::_send(const struct can_frame& sendframe) {
  return (_canbus.sendMessage(&sendframe) == MCP2515::ERROR_OK);
}

void CANLib::MessageCheck(struct can_frame& canRecv) {
  if (_canbus.readMessage(&canRecv) == MCP2515::ERROR_OK) {
    // Serial.print("Received CAN ");
    // Serial.print(canRecv.can_id, HEX);
    // Serial.print(" -> data ");
    // Serial.print(canRecv.data[0]);
    // Serial.print(" ");
    // Serial.print(canRecv.data[1]);
    // Serial.print(" ");
    // Serial.print(canRecv.data[2]);
    // Serial.print(" ");
    // Serial.print(canRecv.data[3]);
    // Serial.print(" ");
    // Serial.print(canRecv.data[4]);
    // Serial.print(" ");
    // Serial.print(canRecv.data[5]);
    // Serial.print(" ");
    // Serial.print(canRecv.data[6]);
    // Serial.print(" ");
    // Serial.print(canRecv.data[7]);
    // Serial.println();
    int sender = canRecv.data[0];
    switch (canRecv.can_id) {
      case CAN_ID::BUTTON_PRESS:
        {
          int buttonID = canRecv.data[1];
          PrintBP(sender, buttonID);
          break;
        }
      case CAN_ID::LED_STATUS:
        {
          Serial.println("MULT LED STATUS!");
          break;
        }
      case CAN_ID::TEMPERATURE:
        {
          uint16_t combinedtemp1 = (uint16_t)canRecv.data[1] << 8 | canRecv.data[2];
          uint16_t combinedtemp2 = (uint16_t)canRecv.data[3] << 8 | canRecv.data[4];
          PrintT(sender, combinedtemp1, combinedtemp2);
          break;
        }
      case CAN_ID::FAN_SPEED:
        {
          uint16_t combinedfanrpm = (uint16_t)canRecv.data[1] << 8 | canRecv.data[2];
          PrintFS(sender, combinedfanrpm);
          break;
        }
      case CAN_ID::HEARTBEAT:
        {
          uint16_t combined = (uint16_t)canRecv.data[1] << 8 | canRecv.data[2];
          PrintHB(sender, combined);
          break;
        }
    }
  }
}

void CANLib::PrintBP(const int& Sender, const int& Button) {
  Serial.print("Recieved CAN from node #");
  Serial.print(Sender);
  Serial.print(" -> Button #");
  Serial.print(Button);
  Serial.print(" has been pressed");
  Serial.println();
}
void CANLib::PrintLS(const int& Sender, const int& Button) {
  Serial.print("Recieved CAN from node #");
  Serial.print(Sender);
  Serial.print(" -> Button #");
  Serial.print(Button);
  Serial.print(" has been pressed");
  Serial.println();
}
void CANLib::PrintFS(const int& Sender, const uint16_t& FanSpeed) {
  Serial.print("Recieved CAN from node #");
  Serial.print(Sender);
  Serial.print(" -> Fan Speed: ");
  Serial.print(FanSpeed);
  Serial.println();
}
void CANLib::PrintT(const int& Sender, const uint16_t& Temp1, const uint16_t& Temp2) {
  Serial.print("Recieved CAN from node #");
  Serial.print(Sender);
  Serial.print(" -> Temperature at sensor 1: ");
  Serial.print(Temp1);
  Serial.print(" & Temperature at sensor 2: ");
  Serial.print(Temp2);
  Serial.println();
}
void CANLib::PrintHB(const int& Sender, const uint16_t& Heartbeats) {
  Serial.print("Recieved CAN from node #");
  Serial.print(Sender);
  Serial.print(" -> Heartbeat #");
  Serial.print(Heartbeats);
  Serial.println();
}