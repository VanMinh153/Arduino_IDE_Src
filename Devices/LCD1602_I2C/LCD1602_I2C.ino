#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  #ifdef ESP32-C3
    Wire.begin(2, 3);
  #endif
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Digital Scale");
  lcd.setCursor(1, 1);
  lcd.print("SOICT - HUST");
}

void loop() {
}