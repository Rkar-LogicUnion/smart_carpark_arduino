#include <LiquidCrystal.h>
int IR_PIN []= {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
int OUT_PIN [] = {24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39};

bool data =HIGH;
int IR_COUNT = sizeof(IR_PIN) / sizeof(int);

LiquidCrystal lcdup(40, 41, 42, 43, 18, 19);
bool lcdUpVolt = LOW;
LiquidCrystal lcddown(46, 47, 48, 49, 50, 51);
bool lcdDownVolt = LOW;
bool lcdVolt = LOW;
void setup() {
  Serial.begin(9600);
  for(int i=0;i<IR_COUNT;i++){
    pinMode(OUT_PIN[i],OUTPUT);
  }
  for(int i=0;i<IR_COUNT;i++){
    pinMode(IR_PIN[i],INPUT);
  }
  pinMode(20,OUTPUT);
  lcdup.begin(16, 2);
  lcddown.begin(16, 2);
  lcdup.setCursor(0,0);
  lcddown.setCursor(0,0);
  lcdup.print("1 - AVAILABLE");
  lcddown.print("2 - AVAILABLE");
}

void loop() {
  for(int i=0;i<IR_COUNT;i++){
    data=digitalRead(IR_PIN[i]);
    digitalWrite(OUT_PIN[i],!data);
  }
  
  for(int i=0;i<IR_COUNT/2;i++){
    data=digitalRead(IR_PIN[i]);
    if(!data){
      lcdUpVolt = HIGH;
    }else{
      lcdUpVolt = LOW;
      break;
    }
  }

  for(int i=IR_COUNT/2;i<IR_COUNT;i++){
    data=digitalRead(IR_PIN[i]);
    if(!data){
      lcdDownVolt = HIGH;
    }else{
      lcdDownVolt = LOW;
      break;
    }
  }
  if(lcdUpVolt){
    lcdup.setCursor(0,0);
    lcdup.print("1 - FULL     ");
  }else{
    lcdup.setCursor(0,0);
    lcdup.print("1 - AVALIABLE");
  }

  if(lcdDownVolt){
    lcddown.setCursor(0,0);
    lcddown.print("2 - FULL     ");
  }else{
    lcddown.setCursor(0,0);
    lcddown.print("2 - AVALIABLE");
  }

  if(lcdUpVolt && lcdDownVolt){
    digitalWrite(20,HIGH);
  }
  
}
