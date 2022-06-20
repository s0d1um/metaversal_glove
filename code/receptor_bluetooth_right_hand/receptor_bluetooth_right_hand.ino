//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Victor Tchistiak - 2019
//
//This example demostrates master mode bluetooth connection and pin 
//it creates a bridge between Serial and Classical Bluetooth (SPP)
//this is an extention of the SerialToSerialBT example by Evandro Copercini - 2018
//

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String MACadd = "AA:BB:CC:11:22:33";
uint8_t address[6]  = {0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33};
//uint8_t address[6]  = {0x00, 0x1D, 0xA5, 0x02, 0xC3, 0x22};
String name = "plc";//metaversal_glove_v1
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

int letra;
int letraB;//letra recibida por bluetoot
int letra_anterior;
char inChar;
char char_vacio;////siempre debe estar vacio
char char_anterior = '0';
String inputString;         // a String to hold incoming data
String  anterior;/////
bool stringComplete = false;  // whether the string is complete
void setup() {
  Serial.begin(115200);
  //SerialBT.setPin(pin);
  //SerialBT.begin("receptor_right_hand", true); 
  //SerialBT.setPin(pin);
  //Serial.println("The device started in master mode, make sure remote BT device is on!");
  
  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
  //connected = SerialBT.connect(name);
  //connected = SerialBT.connect(address);
  SerialBT.begin(name); 
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  // disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();
}
void loop() {
  /*
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
   // Serial.write(SerialBT.read());
    Serial.println(SerialBT.read());
  }
  */
  lectura_serial();
  
  //delay(5);
}
void lectura_serial(){  
  //letra = Serial.read();
  letraB = SerialBT.read();
  ///// 13 = \n " espacio" = 32 line_feed = 10 
  //letraB != letra_anterior && 
    if(letraB != 10 && letraB != -1 && letraB != 0){
      serialEvent();
      if(letraB != 13){
        letra_anterior = letraB;
        SerialBT.write(letra_anterior);
      }
    }
    /*
  if (Serial.available()) {
    if(letra != letra_anterior && letra != 10 && letra != -1 && letra != 0){
      serialEvent();
      if(letra != 13){
        letra_anterior = letra;
      }
    }
  }
  */
 }
void serialEvent() {
     inChar = (char)letra_anterior;// get the new byte://inChar = (char)Serial.read(); 
    
     inputString += inChar;// add it to the inputString:
    // Serial.print(inputString);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (letraB == 32 || letraB == 13) {//\n
      stringComplete = true;
      char_anterior = inChar; 
    }
    if (stringComplete == true) {
    inputString.remove(0, 1);///(position,how_many_chars_remove)
    // anterior = inputString;
    Serial.println(inputString);
    comandos_bluetooth();
    // clear the string:
    inputString = "";
    stringComplete = false;
   }
}
void send_data(){
  
}
void comandos_bluetooth(){
  if(inputString=="tron"){
   
  }
  
}
