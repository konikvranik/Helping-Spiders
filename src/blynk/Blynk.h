#ifndef BLYNK_INCLUDE_h
#define BLYNK_INCLUDE_h

//#define BLYNK_SERVER IPAddress(192,168,1,6)
//#define BLYNK_ESP8266_SSID ""
//#define BLYNK_ESP8266_PASSWORD ""

//#define BLYNK_DEBUG
//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save
// space
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

#include <ArduinoLog.h>
#include <BlynkSimpleEsp8266.h>

void setupBlynk() {
  Log.notice("Starting blynk" CR);
#ifdef BLYNK_ESP8266_SSID
#ifdef BLYNK_SERVER
  Blynk.begin(BLYNK_TOKEN, BLYNK_ESP8266_SSID, BLYNK_ESP8266_PASSWORD,
              BLYNK_SERVER);
#else
  Blynk.begin(BLYNK_TOKEN, BLYNK_ESP8266_SSID, BLYNK_ESP8266_PASSWORD);
#endif
#else
#ifdef BLYNK_SERVER
  Blynk.begin(BLYNK_TOKEN, BLYNK_SERVER);
#else
  Blynk.config(BLYNK_TOKEN);
#endif
#endif
  Log.notice("Connecting blynk ");
  while (Blynk.connect() == false) {
    Log.notice(".");
  }
  Log.notice(" Connected" CR);
}

void loopBlynk() { Blynk.run(); }
#endif
