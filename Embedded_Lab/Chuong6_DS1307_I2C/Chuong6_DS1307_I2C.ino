#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

RTC_DS1307 rtc;

void setup() {
  Serial.begin(57600);

  delay(250);  // wait for the OLED to power up
  display.begin(i2c_Address, true);
  display.display();
  delay(1250);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DS1307_SetTime();
  DS1307_GetTime();



  // DateTime now = rtc.now();
  // // “yyyy/mm/dd  hh:mm:ss”
  // display.clearDisplay();
  // display.setCursor(15, 30);
  // display.print(now.year(), DEC);
  // display.print('/');
  // display.print(now.month(), DEC);
  // display.print('/');
  // display.print(now.day(), DEC);
  // display.print(" ");
  // display.print(now.hour(), DEC);
  // display.print(':');
  // display.print(now.minute(), DEC);
  // display.print(':');
  // display.print(now.second(), DEC);
  // display.display();
  delay(1000);
}

//________________________________
void DS1307_GetTime() {
  Wire.beginTransmission(rtc);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom(rtc, 7);
  second = bcd2dec(Wire.read() & 0x7f);
  minute = bcd2dec(Wire.read());
  hour = bcd2dec(Wire.read() & 0x3f);  // chế độ 24h.
  wday = bcd2dec(Wire.read());
  day = bcd2dec(Wire.read());
  month = bcd2dec(Wire.read());
  year = bcd2dec(Wire.read());
  year += 2000;
}


void DS1307_SetTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr) {
  Wire.beginTransmission(rtc);
  Wire.write(byte(0x00));
  Wire.write(dec2bcd(sec));
  Wire.write(dec2bcd(min));
  Wire.write(dec2bcd(hr));
  Wire.write(dec2bcd(wd));  // day of week: Sunday = 1, Saturday = 7
  Wire.write(dec2bcd(d));
  Wire.write(dec2bcd(mth));
  Wire.write(dec2bcd(yr));
  Wire.endTransmission();
}

/* BCD to Decimal */
int bcd2dec(byte num) {
  return ((num / 16 * 10) + (num % 16));
}
/* Decimal to BCD */
int dec2bcd(byte num) {
  return ((num / 10 * 16) + (num % 10));
}
