#ifndef CANLib_h
#define CANLib_h

#include <Arduino.h>
#include <mcp2515.h>
#include <Wire.h>
#include "types.h"

class CANLib {
  private:
    uint16_t hbCount;
    MCP2515& _canbus;
    CANSensorNode* node1Data;
    bool _send(const struct can_frame& sendframe);
  public:
    CANLib(MCP2515& canbus, CANSensorNode&);
    void SetLED(const int& Target, const int& LED, const bool& Status);
    void SetMultLED(const int& Target, const uint8_t& LEDBank0, const uint8_t& LEDBank1);
    void SetFan(const int& Target, const int& Speed);
    void SendHB();
    void MessageCheck();

    void PrintButtonPress(const int& Sender, const int& Button);
    void PrintLEDStatus(const int& Sender, const bool LEDStatus[7]);
    void PrintFanSpeed(const int& Sender, const uint16_t& FanSpeed);
    void PrintTemperature(const int& Sender, const uint16_t& Temp1, const uint16_t& Temp2);
    void PrintHeartbeat(const int& Sender, const uint16_t& Heartbeats);
};
#endif