#ifndef CANLib_h
#define CANLib_h

#include <Arduino.h>
#include <mcp2515.h>
#include <Wire.h>

enum CAN_ID {
  BUTTON_PRESS    = 0x10,
  LED_STATUS      = 0x20,
  FAN_SPEED       = 0x30,
  TEMPERATURE     = 0x40,
  HEARTBEAT       = 0xF0,
  SET_LED         = 0x21,
  SET_MULT_LED    = 0x22,
  SET_FAN         = 0x31
};

class CANLib {
  private:
    uint16_t hbCount;
    MCP2515& _canbus;
    bool _send(const struct can_frame& sendframe);
  public:
    CANLib(MCP2515& canbus);
    void SetLED(const int& Target, const int& LED, const bool& Status);
    void SetMultLED();
    void SetFan();
    void SendHB();
    void MessageCheck();
};
#endif