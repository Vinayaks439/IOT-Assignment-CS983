#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,  20, 4);
const float BETA = 3950; 
#define tempSenor A0
#define ledout 13

void temperatureSensor() {
  int analogValue = analogRead(tempSenor);
  float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(celsius);
  lcd.print("*C");
  if (celsius > 24) {
    digitalWrite(ledout, HIGH);
    lcd.print(" ( ");
    lcd.write(byte(0));
    lcd.print(" )");
    lcd.setCursor(0, 1);
    lcd.print("Cooling: ON ");
  }  
  else if (celsius < 21) {
    digitalWrite(ledout, HIGH);
    lcd.print(" ( ");
    lcd.write(byte(1));
    lcd.print(" )");
    lcd.setCursor(0, 1);
    lcd.print("Heating: ON ");

  } 
  else {
    digitalWrite(ledout, LOW);
    lcd.print("       ");
    lcd.setCursor(0, 1);
    lcd.print("            ");
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(ledout, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  delay(1000);
  lcd.setCursor(0, 0);
  temperatureSensor();
  delay(1000);
}