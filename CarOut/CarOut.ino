#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int IR_PIN1=8;
bool IR_DATA1=HIGH;

int correctCard[]={151,19,128,91,136,180,182,1};
bool correctCardCon = false;
int cardNo[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
long parkingDuration[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int cardLength=sizeof(cardNo) / sizeof(int);
int correctCardLenght = sizeof(correctCard) / sizeof(int);
constexpr uint8_t RST_PIN1 = 9;  
constexpr uint8_t SS_PIN1 = 10; 

MFRC522 rfid(SS_PIN1, RST_PIN1);

Servo myServo1;
int servoData1=0;
bool hasCar1=false;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  myServo1.attach(A0);
  myServo1.write(servoData1);
  lcd.begin(16, 2);

  SPI.begin(); 
  rfid.PCD_Init();
  
}

void loop() {
  if(hasCar1){
    IR_DATA1=digitalRead(IR_PIN1);
    Serial.print("IR2: ");
    Serial.println(IR_DATA1);
    if(IR_DATA1){
      delay(1000);
      servoData1=0;
      myServo1.write(servoData1);
      hasCar1=false;
      lcd.clear();
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
    Serial.print("IR1: ");
    Serial.println(IR_DATA1);
  if(!IR_DATA1){
     hasCar1=true;
  }
}
void receiveEvent(int howMany) {
  int j=Wire.read();

  for(int i=0;i<correctCardLenght;i++){
    if(correctCard[i]==j || 255==j){
      correctCardCon=true;
      break;
    }else{
      correctCardCon=false;
      hasCar1=false;
    }
  }
  Serial.print(j);
  Serial.print(":");
  Serial.println(correctCardCon);
  if(correctCardCon){
    if(j==255){
      lcd.clear();
    }else{
      String text=String(j);
      text="In : "+text;
      Serial.println(text);
      for(int i=0;i<cardLength;i++){
          if(cardNo[i]==0){
            cardNo[i]=j;
            parkingDuration[i]=millis();
            break;
          }
       }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(text);
    }
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wrong Card");
  }
}
void printHex1(byte *buffer, byte bufferSize){
  byte b=0;
  for (byte i = 0; i < bufferSize; i++) {
    b+=buffer[i];
  }
    for(int i=0;i<cardLength;i++){
      
      if(cardNo[i]==b){
        long difDuration=millis()-parkingDuration[i];
        difDuration/=1000;
        Serial.print(difDuration);
        Serial.println(" sec");
        lcd.clear();
        String text=String(difDuration);
        String text3=String(b);
        lcd.print(text3+" : "+text+" sec");
        lcd.setCursor(0,1);
        String text2=String(difDuration*100);
        lcd.print(text2+" MMK");
        cardNo[i]=0;
        parkingDuration[i]=0;
        break;
      }
    }
    Serial.print("Out:");
    Serial.println(b);
}
