#ifndef INIT_INCLUDE_h
#define INIT_INCLUDE_h

#define ARDUINO 100
#define CR "\r\n"

#include <Arduino.h>
#include "config.h"
#include <ArduinoLog.h>
#include <ArduinoLog.h>
#include <Esp.h>
#include <HardwareSerial.h>
#include <NtpClientLib.h>
#include <TimeLib.h>
#include <WString.h>
#include <string>
#include <ESP8266WebServer.h>

#include "AbstractComponent.h"

//#include <core/MyMessage.h>

#define EXPAND(N) QUOTE(N)
#define QUOTE(NODE_ID) #NODE_ID
// Set this node's subscribe and publish topic prefix

// Enable these if your MQTT broker requires usenrame/password
//#define MY_MQTT_USER "username"
//#define MY_MQTT_PASSWORD "password"

#if defined(MY_USE_UDP)
#include <WiFiUDP.h>
#else

#include <ESP8266WiFi.h>

#endif

#define DEBUG_ESP_PORT Serial
#define BLYNK_PRINT                                                            \
  DEBUG_ESP_PORT // Comment this out to disable prints and save space
#ifdef DISABLE_LOGGING
#undef BLYNK_PRINT
#define MY_DISABLED_SERIAL
#endif

#include <MySensors.h>

#ifdef ENABLE_OTA
#include "ota/OTAComponent.h"
#endif
#ifdef ENABLE_BLYNK
#include "blynk/BlynkComponent.h"
#endif
#ifdef ENABLE_RGB
#include "rgb/RGBComponent.h"
#endif
#ifdef ENABLE_IR
#include "ir/IRComponent.h"
#endif
#ifdef ENABLE_ILIFE
#include "ir/iLifeComponent.h"
#endif
#ifdef ENABLE_DHT
#include "ht_sensor/DHTComponent.h"
#endif
#ifdef ENABLE_HTU
#include "ht_sensor/HTUComponent.h"
#endif
#ifdef ENABLE_DS18
#include "ht_sensor/DS18Component.h"
#endif

#endif
