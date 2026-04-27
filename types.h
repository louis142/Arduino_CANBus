#ifndef types_h
#define types_h

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

struct CANSensorNode{
  uint8_t NodeId;
  uint8_t LEDBank;
  uint8_t LastButton;
  uint16_t FanSpeed;
  uint16_t Temp1;
  uint16_t Temp2;
  uint16_t Heartbeat;
  bool update;
};
#endif