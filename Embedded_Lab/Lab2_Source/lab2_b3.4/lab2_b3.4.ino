#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()   {
  
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true);
  display.display();
  delay(1250);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(15, 30);
  display.println("Hello from ESP32");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {

}