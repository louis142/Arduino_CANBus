#include <SPI.h>
#include <mcp2515.h>
#include <Wire.h>
#include <I2C_RTC.h>
#include "CANLib.h"

#define SPI_BASE 53
/* SPI bus uses four pins: 10 (SS), 11 (MISO), 12 (MOSI) and 13 (SCK)
MPC2515 - CS <-> 53 (SPI_SS)
MPC2515 - SO <-> 50 (SPI_MISO)
MPC2515 - SI <-> 51 (SPI_MOSI)
MPC2515 - SCK <-> 52 (SPI_SCK)
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
  
  pinMode(4, OUTPUT);
  digitalWrite(22, HIGH);
  Serial1.begin(38400); // HC-05 default speed in AT command mode

  RTC.begin();
  RTC.setOutPin(SQW001Hz);
  attachInterrupt(digitalPinToInterrupt(2), hbISR, RISING);
} 

struct can_frame canRecv;

void loop(){
  if(hb){
    can.SendHB();
    //can.SetLED(1, 0, 0);
    //can.SetMultLED(1, 0b00000111, 0);
    //can.SetFan(1, 32);
    hb = false;
    Serial1.write("AT+NAME?\r\n");    
  }

  //can.MessageCheck(canRecv);

  // Read from HC05 and send to Arduino
  if (Serial1.available()){
    Serial.write(Serial1.read());
  }
  // Read from serial monitor and send to HC05
  if (Serial.available()){
    Serial.println("recieved");
    Serial1.write(Serial.read());
  }
}