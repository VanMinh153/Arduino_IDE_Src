#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define ONE_WIRE_BUS 4

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void) {
  Serial.begin(57600);

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true);
  display.display();
  delay(1250);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  sensors.begin();
}

void loop(void) {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);

    display.clearDisplay();
    display.setCursor(15,30);
    display.print((int) tempC, DEC);
    display.print(" ");
    display.print((char)247); // degree symbol
    display.print("C");
    display.display();
  } else {
    Serial.println("Error: Could not read temperature data");
  }
  delay(1000);
}
