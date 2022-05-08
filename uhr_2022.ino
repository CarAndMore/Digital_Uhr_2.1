// Idee: http://werner.rothschopf.net/201802_arduino_esp8266_ntp.htm

#define STASSID         "yourSSID"    // your SSID
#define STAPSK          "password123" // your wifi password
#define HOSTNAME        "ntp-uhr-21"  // Name Device

#define OTA_PASSWD      "password123" // password OTA

/* Configuration for "Deuchland-Berlin" 
    #define MY_NTP_SERVER   "de.pool.ntp.org"
    #define MY_TimeZone     "CET-1CEST,M3.5.0,M10.5.0/3"
*/
#define MY_NTP_SERVER   "de.pool.ntp.org"
#define MY_TimeZone     "CET-1CEST,M3.5.0,M10.5.0/3"

#define ht16k33_Addr    0x75           // 7 Seg. Clock Display
#define DS3231_Addr     0x68           // RTC DS3231

#define Brightness      4

#define myTimeout_Sync  300000         // waiting ms for NTC Sync.
#define myTimeout_refr  100            // waiting ms for Display-refresh

long myTimer_Sync = 0;                 // Timer for NTC Sync.
long myTimer_refresh  = 0;             // Timer for Display-refresh
 
/*************************************/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#include <time.h>
#include <RTClib.h>

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"      // Adafruit_7segment matrix.***()

/* Globals */
Adafruit_7segment matrix = Adafruit_7segment();
RTC_DS3231 rtc;
time_t now_NTC;
tm tm;

/*************************************/
void init_Wifi() {
  Serial.print( "Verbinde mit W-Lan" );
  
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  
  WiFi.hostname(HOSTNAME);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print ( "." );
  }
  
  Serial.println( "\nVerbunden." );
  
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWD);
  ArduinoOTA.begin();
}

void init_LED() {
  
  matrix.begin(ht16k33_Addr);
  matrix.setBrightness(Brightness);
  // write  8888
  matrix.writeDigitNum(0, 8);
  matrix.writeDigitNum(1, 8);
  matrix.writeDigitNum(3, 8);
  matrix.writeDigitNum(4, 8);
  matrix.writeDisplay();
}

/*************************************/
void setFromNTC() {
  // Abfrage NTC Server. -> umwandeln in hour, minute, second
  time(&now_NTC);
  localtime_r(&now_NTC, &tm);
  // Stelle RTC to NTC time.
  rtc.adjust(DateTime((tm.tm_year + 1900), (tm.tm_mon + 1), tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec));
}

void showTimeRTC () {
  // Abfrage RTC -> umwandeln in hour, minute, second
  DateTime now_RTC = rtc.now();
  
  // wenn kleiner Null "Nullen" (1:4 -> 01:04)
  String Uhrzeit = kleinerNull(now_RTC.hour()) + ":" + kleinerNull(now_RTC.minute());

  // Mini Function: Positon & Digit ausgeben
  #define timeDigit(s) s,Uhrzeit.substring(s,(s+1)).toInt()
  
  // write Time on Display
  matrix.writeDigitNum(timeDigit(0));
  matrix.writeDigitNum(timeDigit(1));
  matrix.writeDigitNum(timeDigit(3));
  matrix.writeDigitNum(timeDigit(4));

  // den Doppel Punkt in Sek. Takt blinken lassen...
  matrix.drawColon(now_RTC.second() % 2);
  
  // write Data
  matrix.writeDisplay();
}

String kleinerNull(int value) {
  if (value < 10) {
    return "0" + String(value);
  } else {
    return String(value);
  }
}

/*************************************/
void setup() {
  Serial.begin(9600);
  init_LED();  
  init_Wifi();
  
  configTime(MY_TimeZone, MY_NTP_SERVER);
  rtc.begin();
  setFromNTC();
}

void loop() {
  ArduinoOTA.handle();
  if (millis() > myTimeout_Sync + myTimer_Sync ) {
    myTimer_Sync = millis();
    setFromNTC();
  }
  if (millis() > myTimeout_refr + myTimer_refresh ) {
    myTimer_refresh = millis();
    showTimeRTC();
  }
}
