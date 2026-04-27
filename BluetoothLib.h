#ifndef BluetoothLib_h
#define BluetoothLib_h

#include <Arduino.h>

class BluetoothLib {
  private:
    uint16_t hbCount;
    bool _send();
  public:
    BluetoothLib();
    void SetUp();
    void Begin();
    bool MessageCheck();
    void SetLED();
    void SendHB();

    void PrintLEDStatus();
    void PrintTemperature(const uint16_t& Temp1, const uint16_t& Temp2);
    void PrintHeartbeat(const uint16_t& Heartbeats);
};
#endif