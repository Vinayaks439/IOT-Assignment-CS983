#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal grouplcd(12, 11, 10, 9, 8, 7);

LiquidCrystal_I2C lcd(0x27,  20, 4);
const float BETA = 3950; 
#define tempSenor A0
#define ledout 13
#define pidSenorOut 3
int motion = 0; 
#define pidLED 4
#define disLED 7
#define PIN_TRIG 6
#define PIN_ECHO 5

void groupDetails() {
  grouplcd.setCursor(0,1);
  grouplcd.println("CS983-IOT-Security");
  grouplcd.setCursor(0,2);
  grouplcd.print("Vinayak S,Abhinav R");
  grouplcd.setCursor(0,3);
  grouplcd.println("Amber S");
}

void distancemeasure() {
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  int discm = 0;
  // Read the result:
  int duration = pulseIn(PIN_ECHO, HIGH);
  discm = duration / 58;
  lcd.setCursor(0,3);
  lcd.print("Distance in CM: ");
  lcd.print(discm);
  Serial.print("Distance in CM: ");
  Serial.println(discm);
  if (discm > 100) {
    digitalWrite(disLED,HIGH);
  } else {
    digitalWrite(disLED,LOW);
  }
}


void motionSenor() {
  motion = digitalRead(pidSenorOut);
  if (motion == LOW){
    //no motion
    digitalWrite(pidLED, LOW);
    lcd.setCursor(0, 2);
    lcd.println("Motion: False");
    Serial.println("Motion Not detected!");
  }
  else {
    digitalWrite(pidLED, HIGH);
    lcd.setCursor(0, 2);
    lcd.println("Motion: True");
    Serial.println("Motion detected!");
  }
}

void temperatureSensor() {
  int analogValue = analogRead(tempSenor);
  float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(celsius);
  lcd.print("C");
  if (celsius > 24) {
    digitalWrite(ledout, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Cooling is On ");
  }  
  else if (celsius < 21) {
    digitalWrite(ledout, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Heating is ON ");

  } 
  else {
    digitalWrite(ledout, LOW);
    lcd.setCursor(0, 1);
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  grouplcd.begin(20, 4);
  pinMode(ledout, OUTPUT);
  pinMode(pidLED,OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(disLED,OUTPUT);
  Serial.begin(9600);
}
void loop() {
  delay(1000);
  lcd.setCursor(0, 0);
  temperatureSensor();
  motionSenor();
  distancemeasure();
  groupDetails();
  delay(1000);
}