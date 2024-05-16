#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <EEPROM.h>
#define EEPROM_SIZE 512

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define ONE_WIRE_BUS 4

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;
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

  EEPROM.begin(EEPROM_SIZE);
  // erase EEPROM
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0);
  }

  sensors.begin();

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

// Main
String oldCommand = "";
String command = "";
struct record_t {
  float temp;
  DateTime time;
};
int recordCount = 0;


void loop() {
  if (oldCommand == "")
    Serial.print("Please command...\n");
  command = Serial.readStringUntil('\n');
  command.trim();
  Serial.println(command);
  if (command == "START" | command == "STOP" | command == "GETMIN" | command == "GETMAX")
    oldCommand = command;
  else
    command = oldCommand;

  if (command == "START") {
    startRecord();
  } else if (command == "STOP") {
    stopRecord();
  } else if (command == "GETMIN") {
    startRecord();
    sendMinTemp();
  } else if (command == "GETMAX") {
    startRecord();
    getMaxTemp();
  }

  delay(4800);
  // Thời gian thực hiện một vòng lặp khoảng 5s
}


void startRecord() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  DateTime time = rtc.now();
  saveRecord(temp, time);
  
  String timeStr = String(time.year(), DEC) + "/" + String(time.month(), DEC) + "/" + String(time.day(), DEC) + " " 
                      + String(time.hour(), DEC) + ":" + String(time.minute(), DEC) + ":" + String(time.second(), DEC);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Current Temperature:\n\n");

  display.setTextSize(2);
  String tempStr = String((int)temp, DEC) + " " + String((char) 247) + "C";
  display.print(tempStr);

  display.setTextSize(1);
  display.print("\n\n");
  display.print(timeStr);
  display.display();

  String message = "Temp:" + String(temp) + "|Date:" + toDate(time) + "|Time:" + toTime(time);
  Serial.println(message);
}


void saveRecord(float temp, DateTime time) {
  if (recordCount < (EEPROM_SIZE / sizeof(record_t))) {
    record_t record;
    record.temp = temp;
    record.time = time;

    int address = recordCount * sizeof(record_t);
    EEPROM.put(address, record);
    recordCount++;
  }
}


void stopRecord() {
  Serial.println("Recording stopped");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Recording stopped");
  display.display();
}


void sendMinTemp() {
  float temp = 1000.0;
  DateTime time;

  for (int i = 0; i < recordCount; i++) {
    record_t record;
    int address = i * sizeof(record_t);
    EEPROM.get(address, record);

    if (record.temp < temp) {
      temp = record.temp;
      time = record.time;
    }
  }

  String message = "Temp:" + String(temp) + "|Date:" + toDate(time) + "|Time:" + toTime(time);
  Serial.println(message);
}

void getMaxTemp() {
  float temp = -100.0;
  DateTime time;

  for (int i = 0; i < recordCount; i++) {
    record_t record;
    int address = i * sizeof(record_t);
    EEPROM.get(address, record);

    if (record.temp > temp) {
      temp = record.temp;
      time = record.time;
    }
  }

  String message = "Temp:" + String(temp) + "|Date:" + toDate(time) + "|Time:" + toTime(time);
  Serial.println(message);
}

String toDate(DateTime time) {
  return String(time.year()) + "/" + String(time.month()) + "/" + String(time.day());
}
String toTime(DateTime time) {
  return String(time.hour()) + ":" + String(time.minute()) + ":" + String(time.second());
}