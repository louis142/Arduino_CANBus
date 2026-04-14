#include <SPI.h>
#include <mcp2515.h>

#include <Wire.h>
#include <I2C_RTC.h>

#define SPI_BASE 53
/* SPI bus uses four pins: 10 (SS), 11 (MISO), 12 (MOSI) and 13 (SCK)
MPC2515 - CS <-> 10 (SPI_SS)
MPC2515 - SO <-> 12 (SPI_MISO)
MPC2515 - SI <-> 11 (SPI_MOSI)
MPC2515 - SCK <-> 13 (SPI_SCK)
MPC2515 - INT <-> 2 (if interrupt is required)
*/

MCP2515 mcp2515(SPI_BASE);

static DS1307 RTC;

volatile bool hb = false;

void hbISR(){
  hb = true;
}

void setup() {
  Serial.begin(9600);//communication between arduino + serial monitor
  SPI.begin();//communication between arduino + MCP2515 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  
  RTC.begin();
  RTC.setOutPin(SQW001Hz);
  attachInterrupt(digitalPinToInterrupt(2), hbISR, RISING);
} 

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

struct can_frame canSend;
struct can_frame canRecv;

void loop(){
  if(hb){
    Serial.println("Sending Heartbeat");
    canSend.can_id = 0xF0;//	Heartbeat
    canSend.can_dlc = 8; // Data size in bytes
    canSend.data[0] = 0x128; // Student code: 202114241
    canSend.data[1] = 0x00;//Count MSB
    canSend.data[2] = 0x00;//Count LSB
    canSend.data[3] = 0x00; // ~
    canSend.data[4] = 0x00; // ~
    canSend.data[5] = 0x00; // ~
    canSend.data[6] = 0x00; // ~
    canSend.data[7] = 0x00; // ~
    mcp2515.sendMessage(&canSend);
    hb = false;
  }
  
  if (mcp2515.readMessage(&canRecv) == MCP2515::ERROR_OK){
  
    switch(canRecv.can_id){
      case CAN_ID::BUTTON_PRESS:
        Serial.print("Received CAN from: ");
        Serial.print(canRecv.data[0]);
        Serial.print(" -> Button Press ");
        break;
      case CAN_ID::LED_STATUS:
        Serial.print("Received CAN from: ");
        Serial.print(canRecv.data[0]);
        Serial.print(" -> Button Press ");
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