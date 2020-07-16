#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>

int IR_PIN1=3;
bool IR_DATA1=HIGH;

constexpr uint8_t RST_PIN1 = 9;  
constexpr uint8_t SS_PIN1 = 10; 

int correctCard[]={151,19,128,91,136,180,182,1};
bool correctCardCon = false;
int correctCardLenght = sizeof(correctCard) / sizeof(int);

MFRC522 rfid(SS_PIN1, RST_PIN1);

Servo myServo1;
int servoData1=0;
bool hasCar1=false;

void setup() {
  Serial.begin(9600);

  pinMode(IR_PIN1,INPUT);
  myServo1.attach(2);
  myServo1.write(servoData1);

  SPI.begin(); 
  rfid.PCD_Init();

  Wire.begin();

}

void loop() {
  if(hasCar1){
    IR_DATA1=digitalRead(IR_PIN1);
    if(IR_DATA1){
      delay(1000);
      servoData1=0;
      myServo1.write(servoData1);
      hasCar1=false;
      Wire.beginTransmission(8);
      Wire.write(255);
      Wire.endTransmission();
    }else{
      servoData1=90;
      myServo1.write(servoData1);
    }
  }
  if ( ! rfid.PICC_IsNewCardPresent()){
      return;
  }
  if ( ! rfid.PICC_ReadCardSerial()){
      return;
  }
  printHex1(rfid.uid.uidByte, rfid.uid.size);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  IR_DATA1=digitalRead(IR_PIN1);
  if(correctCardCon && !IR_DATA1){
    hasCar1=true;
  }
}
void printHex1(byte *buffer, byte bufferSize){
  String s="";
  byte a=0;
  for (byte i = 0; i < bufferSize; i++) {
    //s+=buffer[i];
    a+=buffer[i];
  }

  for(int i=0;i<correctCardLenght;i++){
    if(correctCard[i]==a || 255==a){
      correctCardCon=true;
      break;
    }else{
      correctCardCon=false;
      hasCar1=false;
    }
  }
  
  Wire.beginTransmission(8);
  Wire.write(a);
  Wire.endTransmission();
  
  Serial.print("In:");
  Serial.println(s);
}
