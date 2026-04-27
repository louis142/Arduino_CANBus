#include "BluetoothLib.h"

BluetoothLib::BluetoothLib(){
  
}

void BluetoothLib::SetUp(){
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Serial2.begin(38400);

  Serial.println("Setting up bluetooth module");
  Serial2.write("AT+ORGL\r\n");
  Serial2.write("AT+RESET\r\n");
  
  int step = 0;

  while(true){
    switch(step){
      case 0://Confirms connected to HC-05
        Serial.println("Sending connection check");
        Serial2.write("AT\r\n");
        delay(1000);
        if(MessageCheck()){
          step = 1;
        }
        break;

      case 1://sets name of bluetooth module
        Serial.println("Sending rename command");
        Serial2.write("AT+NAME=BT-LDN142\r\n");
        delay(1000);
        if(MessageCheck()){
          step = 2;
        }
        break;

      case 2://sets role as master
        Serial.println("Sending role command");
        Serial2.write("AT+ROLE=1\r\n");
        delay(1000);
        if(MessageCheck()){
          step = 3;
        }
        break;

      case 3://set to connect to specifc address only
      Serial.println("Sending mode command");
        Serial2.write("AT+CMODE=0\r\n");
        delay(1000);
        if(MessageCheck()){
          step = 4;
        }
        break;

      case 4://binds to slave address
        Serial.println("Sending bind command");
        Serial2.write("AT+BIND=0022,12,021BFB\r\n");
        // Serial2.write("AT+BIND=98d3,31,F6F3DF\r\n");
        delay(1000);
        if(MessageCheck()){
          step = 5;
        }
        break;

      default:
        Serial.println("Boot into data mode now");
        //digitalWrite(4, LOW);
        Serial2.write("AT+VERSION?\r\n");
        delay(1000);
        
        if(Serial2.available()){
          Serial.print(Serial2.read());
          //return;
        }
        break;
    }  
  }
}

void BluetoothLib::Begin(){
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  Serial2.begin(9600);
}

bool BluetoothLib::MessageCheck(){
  while(Serial2.available()){
    // Serial.print(Serial2.read(), HEX);
    // Serial.println();
    char c = Serial2.read();
    if(c == '\n'){
      Serial.println();
      return true;
    }
    if(c != '\r'){
      Serial.write(c);
    }
  }
  return false;

  // if(Serial2.available()){
  //   Serial.println(Serial2.read(), HEX);
  //   return true;
  // }

  // return false;

}

void BluetoothLib::SetLED() {

}


void BluetoothLib::SendHB() {
  // BluetoothSend.Bluetooth_id = Bluetooth_ID::HEARTBEAT;
  // BluetoothSend.Bluetooth_dlc = 8;                  // Data size in bytes
  // BluetoothSend.data[0] = 128;                // Student code: 202114241
  // BluetoothSend.data[1] = highByte(hbCount);  //Count MSB
  // BluetoothSend.data[2] = lowByte(hbCount);   //Count LSB
  // BluetoothSend.data[3] = 0x00;               // ~
  // BluetoothSend.data[4] = 0x00;               // ~
  // BluetoothSend.data[5] = 0x00;               // ~
  // BluetoothSend.data[6] = 0x00;               // ~
  // BluetoothSend.data[7] = 0x00;               // ~
  // if (_send(BluetoothSend)) {
  //   // Serial.print("Heartbeat #");
  //   // Serial.print(hbCount);
  //   // Serial.print(" sent!");
  //   // Serial.println();
  //   hbCount++;
  // }
}

// bool BluetoothLib::_send(const struct Bluetooth_frame& sendframe) {
//   return (_Bluetoothbus.sendMessage(&sendframe) == MCP2515::ERROR_OK);
// }

void BluetoothLib::PrintLEDStatus() {//LED Status unfinished

}
void BluetoothLib::PrintTemperature(const uint16_t& Temp1, const uint16_t& Temp2) {
  Serial.print("Recieved message from Bluetooth node -> ");
  Serial.print("Temperature at sensor 1: ");
  Serial.print(Temp1);
  Serial.print(" & Temperature at sensor 2: ");
  Serial.print(Temp2);
  Serial.println();
}
void BluetoothLib::PrintHeartbeat(const uint16_t& Heartbeats) {
  Serial.print("Recieved message from Bluetooth node -> ");
  Serial.print("Heartbeat #");
  Serial.print(Heartbeats);
  Serial.println();
}