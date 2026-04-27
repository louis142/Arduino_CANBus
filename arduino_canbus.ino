#include <SPI.h>
#include <mcp2515.h>
#include <Wire.h>
#include <I2C_RTC.h>
#include "types.h"
#include "CANLib.h"
#include "BluetoothLib.h"

#define LED1 27
#define LED2 29
#define LED3 31
#define SPI_BASE 53
/* SPI bus uses four pins: 10 (SS), 11 (MISO), 12 (MOSI) and 13 (SCK)
MPC2515 - CS <-> 53 (SPI_SS)
MPC2515 - SO <-> 50 (SPI_MISO)
MPC2515 - SI <-> 51 (SPI_MOSI)
MPC2515 - SCK <-> 52 (SPI_SCK)
MPC2515 - INT <-> 2 (if interrupt is required)
*/

//use array for multiple can devices
CANSensorNode node1;

MCP2515 mcp2515(SPI_BASE);
CANLib can(mcp2515, node1);

BluetoothLib bt;

static DS1307 RTC;

volatile bool hb = false;
void hbISR(){
  hb = true;
}

void setup() {
  Serial.begin(9600);//communication between arduino + serial monitor
  Serial.println("starting");

  SPI.begin();//communication between arduino + MCP2515 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  
  // bt.SetUp();
  bt.Begin();
  
  node1.NodeId = 1;
  
  Serial1.begin(115200);//communication between Arduino+ESP32

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  RTC.begin();
  RTC.setOutPin(SQW001Hz);
  attachInterrupt(digitalPinToInterrupt(2), hbISR, RISING);
} 

uint8_t buffer[8];
int i = 0;
bool recieving = false;

void loop(){
  if(hb){
    can.SendHB();
    //can.SetLED(3, 0, 1);
    //can.SetMultLED(1, 0b00000111, 0);
    //can.SetFan(1, 120);
    hb = false;   
  }

  can.MessageCheck();
  // bt.MessageCheck();

  if(node1.update){
    Serial.println("there has been an update!");
    node1.update = false;

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);

    if((node1.LEDBank >> 0) & 0x01){
      digitalWrite(LED1, HIGH);
    }
    if((node1.LEDBank >> 1) & 0x01){
      digitalWrite(LED2, HIGH);
    }
    if((node1.LEDBank >> 2) & 0x01){
      digitalWrite(LED3, HIGH);
    }
    Serial1.write(0xAA);//Starting byte
    Serial1.write(node1.NodeId);//node id
    Serial1.write(node1.LEDBank);//LEDBank
    Serial1.write(node1.LastButton);//Last button pressed
    Serial1.write((node1.FanSpeed >> 8) & 0xFF);//fanspeed pt1 MSB
    Serial1.write(node1.FanSpeed & 0xFF);//pt2 LSB
    Serial1.write((node1.Temp1 >> 8) & 0xFF);//Temp1 pt1 
    Serial1.write(node1.Temp1 & 0xFF);//pt2
    Serial1.write((node1.Temp2 >> 8) & 0xFF);//Temp2 pt1
    Serial1.write(node1.Temp2 & 0xFF);//pt2
    Serial1.write(0xCC);//Ending byte

  }

  while(Serial1.available()){
    uint8_t msgbyte = Serial1.read();
    if(!recieving && msgbyte == 0xAA){
      recieving = true;
      Serial.println("Message from arduino");
      i = 0;
    }
    else if(recieving && msgbyte == 0xCC){
      recieving = false;
    }
    else if(recieving && i < 8){
      buffer[i] = msgbyte;
      i++;
    }
  }
  if(buffer[0] != 0x00){
    can.SetLED(buffer[0], buffer[1], buffer[2]);
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
  }
}