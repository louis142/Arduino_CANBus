#ifndef BluetoothLib_h
#define BluetoothLib_h

#include <Arduino.h>

// enum CAN_ID {
//   BUTTON_PRESS    = 0x10,
//   LED_STATUS      = 0x20,
//   FAN_SPEED       = 0x30,
//   TEMPERATURE     = 0x40,
//   HEARTBEAT       = 0xF0,
//   SET_LED         = 0x21,
//   SET_MULT_LED    = 0x22,
//   SET_FAN         = 0x31
// };

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

    void PrintLS();
    void PrintT(const uint16_t& Temp1, const uint16_t& Temp2);
    void PrintHB(const uint16_t& Heartbeats);
};
#endif