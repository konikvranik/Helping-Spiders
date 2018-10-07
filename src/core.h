#ifndef INIT_INCLUDE_h
#define INIT_INCLUDE_h

#define ARDUINO 100
#define CR "\r\n"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Esp.h>
#include <HardwareSerial.h>
#include <NtpClientLib.h>
#include <TimeLib.h>
#include <WString.h>
#include <string>
#include <ESP8266WiFi.h>
#include <ESP8266MQTTClient.h>

#include "AbstractComponent.h"

#define EXPAND(N) QUOTE(N)
#define QUOTE(NODE_ID) #NODE_ID
// Set this node's subscribe and publish topic prefix

// Enable these if your MQTT broker requires usenrame/password

//#define DEBUG_ESP_PORT Serial
#define DISABLE_LOGGING

#ifdef ENABLE_OTA
#include "../OTA/OTAComponent.h"
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
#include "../HTU/HTUComponent.h"
#endif
#ifdef ENABLE_DS18
#include "../DS18/DS18.h"
#endif
#ifdef ENABLE_HC-SR04
#include "../HC-SR04/HC-SR04.h"
#endif

#endif
