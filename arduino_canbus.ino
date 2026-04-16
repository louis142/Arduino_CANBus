#include <SPI.h>
#include <mcp2515.h>
#include <Wire.h>
#include <I2C_RTC.h>
#include "CANLib.h"

#define SPI_BASE 53
/* SPI bus uses four pins: 10 (SS), 11 (MISO), 12 (MOSI) and 13 (SCK)
MPC2515 - CS <-> 10 (SPI_SS)
MPC2515 - SO <-> 12 (SPI_MISO)
MPC2515 - SI <-> 11 (SPI_MOSI)
MPC2515 - SCK <-> 13 (SPI_SCK)
MPC2515 - INT <-> 2 (if interrupt is required)
*/
MCP2515 mcp2515(SPI_BASE);
CANLib can(mcp2515);

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
void loop(){
  if(hb){
    can.SendHB();
    can.SetLED(3, 0, true);
    hb = false;
  }
  
  can.MessageCheck();
}