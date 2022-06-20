#include <Wire.h>
 
//Direccion I2C de la IMU
#define MPU1 0x69
#define MPU2 0x68
//Ratios de conversion
#define A_R 16384.0
#define G_R 131.0
 
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779
 
//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
 int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 int X1,Y1,X2,Y2;
 
//Angulos
float Acc[2];
float Gy[2];
float Angle[2];

float Angle1[2];
float Angle2[2]; 

//#include <ESP8266WiFi.h>//esp8266
#include <WiFi.h>//esp32
#include "BluetoothSerial.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        13 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


String MACadd = "AC:67:B2:36:E4:CE";//set this to be the MAC of the other ESP32
uint8_t address[6]  = {0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33};
//uint8_t address[6]  = {0x00, 0x1D, 0xA5, 0x02, 0xC3, 0x22};
String my_Bluetooth_name = "Bluetooth device";//Bluetooth device name
//String name = "metaversal_glove_v1_server";// <------- set this to be the name of the other ESP32!!!!!!!!!
String name = "plc";// <------- set this to be the name of the other ESP32!!!!!!!!!

char *pin = "1234"; //<- standard pin would be provided by default
char *ssid = "metaversal_glove_v1";//my ssid
char *password = "12345678";

bool connected;
// Variables will change:
bool ledState = LOW;             // ledState used to set the LED

int sensorValue = 0;        // value read from the pot
int sensorValue_previous = 1;
int outputValue = 0;        // value output to the PWM (analog out)
char intToPrint;
char *mensaje_bluetooth = "c1";
char *mensaje_bluetooth_anterior;
bool bandera_bluetooth = true;

unsigned long currentMillis;
unsigned long previousMillis = 0;       
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
unsigned long previousMillis4 = 0;

long interval = 1;           //revision de sensores 
long interval2 = 15;         //cambio de led< 60hz (cada 15ms)
long interval3 = 50;        //envio de datos periodico bluetooth
long interval4 = 100;       // neopixels
const int ledPin = 2;//2 para esp32 y 13 para esp8266
int sensor_pin1 = 39;
int sensor_pin2 = 36;
int sensor_pin3 = 35;
int sensor_pin4 = 34;
int sensor_pin5 = 32;
int sensor_pin6 = 33;
int sensor_pin7 = 14;
int sensor_pin8 = 27;
int sensor_pin9 = 25;
int sensor_pin10= 26;
int sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,sensor7,sensor8,sensor9,sensor10;//valor actual del sensor
int sensor1A,sensor2A,sensor3A,sensor4A,sensor5A,sensor6A,sensor7A,sensor8A,sensor9A,sensor10A;//valor anterior
int finger1,finger2,finger3,finger4,finger5;//valor actual del dedo
unsigned char finger1_cont,finger2_cont,finger3_cont,finger4_cont,finger5_cont; //cuentan cuantas veces han recibido la misma secuencia
unsigned char finger1_sec,finger2_sec,finger3_sec,finger4_sec,finger5_sec;//secuencia actual
unsigned char finger1_secA,finger2_secA,finger3_secA,finger4_secA,finger5_secA;//secuencia Anterior
int fingerMAX,fingerMIN;//valor maximo, minimo general
int fingerMAX1,fingerMIN1,fingerMAX2,fingerMIN2,fingerMAX3,fingerMIN3,fingerMAX4,fingerMIN4,fingerMAX5,fingerMIN5;
int finger1A,finger2A,finger3A,finger4A,finger5A;
  
void setup() {    
  Serial.begin(115200);
  delay(50);
  
  passwords();
  
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
  // SerialBT.begin("ESP32test", true);
  SerialBT.begin(my_Bluetooth_name,true); //SerialBT.begin(my_Bluetooth_name,true); //Bluetooth device name, master mode = true
  //Serial.println("The device started, now you can pair it with bluetooth!");
  //////Serial.println("The device started in master mode, make sure remote BT device is on!");
  delay(15);
  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
connected = SerialBT.connect(name);//conecting to the device whit "name"
  //connected = SerialBT.connect(address);//conecting to the device whit "address"
    if(connected) {
      if(bandera_bluetooth == true){
        Serial.print("Connected Succesfully to ");
        Serial.println(name);
        bandera_bluetooth = false;
      }
    } else {
     // if(!SerialBT.connected(10000)) {
     // Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
     // }
    }
  pinMode(ledPin, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  pinMode(sensor6, INPUT);
  pinMode(sensor7, INPUT);
  pinMode(sensor8, INPUT);
  pinMode(sensor9, INPUT);
  pinMode(sensor10,INPUT);
  finger1 = 1;fingerMAX1 = 25;fingerMIN1 = 0;
  finger2 = 1;fingerMAX2 = 40;fingerMIN2 = 0;
  finger3 = 1;fingerMAX3 = 40;fingerMIN3 = 0;
  finger4 = 1;fingerMAX4 = 40;fingerMIN4 = 0;
  finger5 = 1;fingerMAX5 = 40;fingerMIN5 = 0;
   
  
   
}
void loop() {
  currentMillis = millis();
  //currentMillis++;
  //delayMicroseconds(500);//1000 microseconds = 1 ms
  //delay(1);
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sensor1 = analogRead(sensor_pin1);
    sensor2 = analogRead(sensor_pin2);
    sensor3 = analogRead(sensor_pin3);
    sensor4 = analogRead(sensor_pin4);
    sensor5 = analogRead(sensor_pin5);
    sensor6 = analogRead(sensor_pin6);
    sensor7 = analogRead(sensor_pin7);
    sensor8 = analogRead(sensor_pin8);
    sensor9 = analogRead(sensor_pin9);
    sensor10 =analogRead(sensor_pin10);
    if(sensor1 > 0){
      sensor1 = 1; 
    }
    if(sensor2 > 0){
      sensor2 = 1; 
    }
    if(sensor3 > 0){
      sensor3 = 1; 
    }
    if(sensor4 > 0){
      sensor4 = 1; 
    }
    if(sensor5 > 0){
      sensor5 = 1; 
    }
    if(sensor6 > 0){
      sensor6 = 1;
    }
    if(sensor7 > 0){
      sensor7 = 1;
    }
    if(sensor8 > 0){
      sensor8 = 1;
    }
    if(sensor9 > 0){
      sensor9 = 1;
    }
    if(sensor10 > 0){
      sensor10 = 1;
    }
    if(sensor1 != sensor1A){
    mostrar_sensores();
    sensores1(sensor1,sensor2);
    sensor1A = sensor1;
    }
    if(sensor2 != sensor2A){
    mostrar_sensores();      
    sensores1(sensor1,sensor2);
    sensor2A = sensor2;
    }
    if(sensor3 != sensor3A){
    mostrar_sensores();
    sensores2(sensor3,sensor4);
    sensor3A = sensor3;
    }
    if(sensor4 != sensor4A){
    mostrar_sensores();
    sensores2(sensor3,sensor4);
    sensor4A = sensor4;
    }
    if(sensor5 != sensor5A){
    mostrar_sensores();
    sensores3(sensor5,sensor6);
    sensor5A = sensor5;
    }
    if(sensor6 != sensor6A){
    mostrar_sensores();
    sensores3(sensor5,sensor6);
    sensor6A = sensor6;
    }
    if(sensor7 != sensor7A){
    mostrar_sensores();
    sensores4(sensor7,sensor8);
    sensor7A = sensor7;
    } 
    if(sensor8 != sensor8A){
    mostrar_sensores();
    sensores4(sensor7,sensor8);
    sensor8A = sensor8;
    } 
    if(sensor9 != sensor9A){
    mostrar_sensores();
    sensores5(sensor9,sensor10);
    sensor9A = sensor9;
    } 
    if(sensor10 != sensor10A){
    mostrar_sensores();
    sensores5(sensor9,sensor10);
    sensor10A = sensor10;
    } 
  } 
  if (currentMillis - previousMillis2 >= interval2) {//muestra datos en ploter y led
      previousMillis2 = currentMillis;
      digitalWrite(ledPin, ledState);
      
  }
  if (currentMillis - previousMillis3 >= interval3) {//manda datos al receptor
    previousMillis3 = currentMillis;
    data_bluetooth(); 
    //data_VR();
  }
  if (currentMillis - previousMillis4 >= interval4) {//actualiza los neopixel
    previousMillis4 = currentMillis;
   // pixels.show();   // Send the updated pixel colors to the hardware.  
   // acelerometros();
   /*
    Serial.print("finger1:");
    Serial.print(finger1);
    Serial.print(" ");
    Serial.print("finger2:");
    Serial.print(finger2);
    Serial.print(" ");
    Serial.print("finger3:");
    Serial.print(finger3);
    Serial.print(" ");
    Serial.print("finger4:");
    Serial.print(finger4);
    Serial.print(" ");
    Serial.print("finger5:");
    Serial.print(finger5);
    Serial.println(" ");
    */
   /* 
    Serial.print("x1:");
    Serial.print(Angle1[0]);
    Serial.print(" ");
    Serial.print("y1:");
    Serial.print(Angle1[1]);
    Serial.print(" ");
    Serial.print("x2:");
    Serial.print(Angle2[0]);
    Serial.print(" ");
    Serial.print("y2:");
    Serial.print(Angle2[1]);
    Serial.print(" ");
    
    Serial.print("finger1:");
    Serial.print(finger1);
    Serial.print(" ");
    Serial.print("finger2:");
    Serial.print(finger2);
    Serial.print(" ");
    Serial.print("finger3:");
    Serial.print(finger3);
    Serial.print(" ");
    Serial.print("finger4:");
    Serial.print(finger4);
    Serial.print(" ");
    Serial.print("finger5:");
    Serial.print(finger5);
    Serial.println(" ");

    Serial.print("A");
    Serial.print(finger1);
    Serial.print("B");
    Serial.print(finger2);
    Serial.print("C");
    Serial.print(finger3);
    Serial.print("D");
    Serial.print(finger4);
    Serial.print("E");
    Serial.println(finger5);
    */
  }
}
void data_bluetooth(){
  /*
  if(finger1>15&&finger2>35&&finger3>35&&finger4>35&&finger5>40){
    mensaje_bluetooth = "c4 ";
  }
  if(finger1<10&&finger2<10&&finger3>20&&finger4>20&&finger5<10){
    mensaje_bluetooth = "c16 ";
  }
 */
if(finger1>5){
    mensaje_bluetooth = "c5 ";
  }
  if(finger2>35){
    mensaje_bluetooth = "c1 ";
  }
  if(finger3>35){
    mensaje_bluetooth = "c2 ";
  }
  if(finger4>35){
    mensaje_bluetooth = "c3 ";
  }
  if(finger5>35){
    mensaje_bluetooth = "c4 ";
  }
  

/*
  // disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();
  }
*/ 

  if(mensaje_bluetooth != mensaje_bluetooth_anterior){
    mensaje_bluetooth_anterior = mensaje_bluetooth;
    int f = 0;
    Serial.println(mensaje_bluetooth);
    while (mensaje_bluetooth[f] != 0) SerialBT.write((uint8_t)mensaje_bluetooth[f++]);
  }
}
void mostrar_sensores(){///muestra el cambio de cualquier sensor
/*    Serial.print(sensor1);
    Serial.print(" ");
    Serial.print(sensor2);
    Serial.print(" ");
    Serial.print(sensor3);
    Serial.print(" ");
    Serial.print(sensor4);
    Serial.print(" ");
    Serial.print(sensor5);
    Serial.print(" ");
    Serial.print(sensor6);
    Serial.print(" ");
    Serial.print(sensor7);
    Serial.print(" ");
    Serial.print(sensor8);
    Serial.print(" ");
    Serial.print(sensor9);
    Serial.print(" ");
    Serial.print(sensor10);
    Serial.println(" ");
*/
   if (ledState == LOW) {
       ledState = HIGH;
       } else {
       ledState = LOW;
       }
}
void data_VR(){
  
  char finger_char1[4],finger_char2[4],finger_char3[4],finger_char4[4],finger_char5[4];
    String str;
    str=String(finger1);
    str.toCharArray(finger_char1,4);
    str=String(finger2);
    str.toCharArray(finger_char2,4);
    str=String(finger3);
    str.toCharArray(finger_char3,4);
    str=String(finger4);
    str.toCharArray(finger_char4,4);
    str=String(finger5);
    str.toCharArray(finger_char5,4);
    
 //myString.toCharArray(integer, lenght)
 //strcat(base_text , text_to_concatenate);
 //strcat(text_to_wipe , text_to_save);
 
    char var[strlen(finger_char1)+strlen(finger_char2)+strlen(finger_char3)+strlen(finger_char4)+strlen(finger_char5)+2];
    strcpy(var, "A");
    strcat(var, finger_char1);
    strcat(var, "B");
    strcat(var, finger_char2);
    strcat(var, "C");
    strcat(var, finger_char3);
    strcat(var, "D");
    strcat(var, finger_char4);
    strcat(var, "E");
    strcat(var, finger_char5);
    strcat(var, " ");
    //Serial.println(var);
    mensaje_bluetooth = var;
    int f = 0;
    while (mensaje_bluetooth[f] != 0) SerialBT.write((uint8_t)mensaje_bluetooth[f++]);
    /*
    Serial.print("A");
    Serial.print(finger1);
    Serial.print("B");
    Serial.print(finger2);
    Serial.print("C");
    Serial.print(finger3);
    Serial.print("D");
    Serial.print(finger4);
    Serial.print("E");
    Serial.println(finger5);
    */
}
void sensores1(int s1,int s2){
  if(s1 == 1 && s2 == 1){
    finger1_sec = 1;
  }
  if(s1 == 0 && s2 == 1){
    finger1_sec = 2;
  }
  if(s1 == 0 && s2 == 0){
    finger1_sec = 3;
  }
  if(s1 == 1 && s2 == 0){
    finger1_sec = 4;
  }
  if(finger1_sec > finger1_secA){
    finger1++;
  }
  if(finger1_sec < finger1_secA){
    finger1--;
  }
  if(finger1_sec == 1 && finger1_secA == 4){
    finger1=finger1+2;
  }
  if(finger1_sec == 4 && finger1_secA == 1){
    finger1=finger1-2;
  } 
  if(finger1_sec != finger1_secA){
    finger1_secA = finger1_sec;
  } 
  if(finger1 > fingerMAX1){//re calibra los sensores
      finger1=fingerMAX1;
    }
  if(finger1 < fingerMIN1){//re calibra los sensores
      finger1=fingerMIN1;
    }
}
void sensores2(int s3,int s4){
  if(s3 == 1 && s4 == 1){
    finger2_sec = 1;
  }
  if(s3 == 0 && s4 == 1){
    finger2_sec = 2;
  }
  if(s3 == 0 && s4 == 0){
    finger2_sec = 3;
  }
  if(s3 == 1 && s4 == 0){
    finger2_sec = 4;
  }
  if(finger2_sec > finger2_secA){
    finger2++;
  }
  if(finger2_sec < finger2_secA){
    finger2--;
  }
  if(finger2_sec == 1 && finger2_secA == 4){
    finger2=finger2+2;
  }
  if(finger2_sec == 4 && finger2_secA == 1){
    finger2=finger2-2;
  } 
  if(finger2_sec != finger2_secA){
    finger2_secA = finger2_sec;
  } 
  if(finger2 > fingerMAX2){//re calibra los sensores
      finger2=fingerMAX2;
    }
  if(finger2 < fingerMIN2){//re calibra los sensores
      finger2=fingerMIN2;
    }
}
void sensores3(int s5,int s6){
  if(s5 == 1 && s6 == 1){
    finger3_sec = 1;
  }
  if(s5 == 0 && s6 == 1){
    finger3_sec = 2;
  }
  if(s5 == 0 && s6 == 0){
    finger3_sec = 3;
  }
  if(s5 == 1 && s6 == 0){
    finger3_sec = 4;
  }
  if(finger3_sec > finger3_secA){
    finger3++;
  }
  if(finger3_sec < finger3_secA){
    finger3--;
  }
  if(finger3_sec == 1 && finger3_secA == 4){
    finger3=finger3+2;
  }
  if(finger3_sec == 4 && finger3_secA == 1){
    finger3=finger3-2;
  } 
  if(finger3_sec != finger3_secA){
    finger3_secA = finger3_sec;
  } 
  if(finger3 > fingerMAX3){//re calibra los sensores
      finger3=fingerMAX3;
    }
  if(finger3 < fingerMIN3){//re calibra los sensores
      finger3=fingerMIN3;
    }
}
void sensores4(int s7,int s8){
  if(s7 == 1 && s8 == 1){
    finger4_sec = 1;
  }
  if(s7 == 0 && s8 == 1){
    finger4_sec = 2;
  }
  if(s7 == 0 && s8 == 0){
    finger4_sec = 3;
  }
  if(s7 == 1 && s8 == 0){
    finger4_sec = 4;
  }
  if(finger4_sec > finger4_secA){
    finger4++;
  }
  if(finger4_sec < finger4_secA){
    finger4--;
  }
  if(finger4_sec == 1 && finger4_secA == 4){
    finger4=finger4+2;
  }
  if(finger4_sec == 4 && finger4_secA == 1){
    finger4=finger4-2;
  } 
  if(finger4_sec != finger4_secA){
    finger4_secA = finger4_sec;
  } 
  if(finger4 > fingerMAX4){//re calibra los sensores
      finger4=fingerMAX4;
    }
  if(finger4 < fingerMIN4){//re calibra los sensores
      finger4=fingerMIN4;
    }
}
void sensores5(int s9,int s10){
  if(s9 == 1 && s10 == 1){
    finger5_sec = 1;
  }
  if(s9 == 0 && s10 == 1){
    finger5_sec = 2;
  }
  if(s9 == 0 && s10 == 0){
    finger5_sec = 3;
  }
  if(s9 == 1 && s10 == 0){
    finger5_sec = 4;
  }
  if(finger5_sec > finger5_secA){
    finger5++;
  }
  if(finger5_sec < finger5_secA){
    finger5--;
  }
  if(finger5_sec == 1 && finger5_secA == 4){
    finger5=finger5+2;
  }
  if(finger5_sec == 4 && finger5_secA == 1){
    finger5=finger5-2;
  } 
  if(finger5_sec != finger5_secA){
    finger5_secA = finger5_sec;
  } 
  if(finger5 > fingerMAX5){//re calibra los sensores
      finger5=fingerMAX5;
    }
  if(finger5 < fingerMIN5){//re calibra los sensores
      finger5=fingerMIN5;
    }
}
void neopixels(){
  pixels.setPixelColor(0, pixels.Color(finger1, finger1, finger1));
  pixels.setPixelColor(1, pixels.Color(finger2, finger2, finger2));
  pixels.setPixelColor(2, pixels.Color(finger3, finger3, finger3));
  pixels.setPixelColor(3, pixels.Color(finger4, finger4, finger4));
  pixels.setPixelColor(4, pixels.Color(finger5, finger5, finger5));
  /*
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(1, 1, 1));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }*/
}
void golpe_de_suerte(){
    sensor3 = analogRead(sensor_pin3);
    sensor4 = analogRead(sensor_pin4);
    if(sensor3 > 0){
      sensor3 = 1023; 
      sensor3A = 1023;
    }
    if(sensor4 > 0){
      sensor4 = 1023;
      sensor4A = 1023;
    }
    if(sensor3 != sensor3A || sensor4 != sensor4A){
    
    sensor3A = sensor3;
    sensor4A = sensor4;
    
    
    if (ledState == LOW) {
       ledState = HIGH;
       } else {
       ledState = LOW;
       }
      digitalWrite(ledPin, ledState);
     // Serial.println(finger1);
    }
}
void basura(){
}
