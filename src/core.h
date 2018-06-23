#ifndef INIT_INCLUDE_h
#define INIT_INCLUDE_h

#define ARDUINO 100

#include "config.h"
#include <Arduino.h>
#include <ArduinoLog.h>
#include <Esp.h>
#include <HardwareSerial.h>
#include <NtpClientLib.h>
#include <TimeLib.h>
#include <WString.h>
#include <string>

//#include <core/MyMessage.h>

#define CR "\r\n"

// Controller ip address. Enables client mode (default is "server" mode).
// Also enable this if MY_USE_UDP is used and you want sensor data sent
// somewhere.
//#define MY_CONTROLLER_IP_ADDRESS 192, 168, 1, 6
#define MY_CONTROLLER_URL_ADDRESS "mqtt.home"
// The MQTT broker port to to open
#define MY_PORT 1883

#define MAX_SENSOR_ID 6

// How many clients should be able to connect to this gateway (default 1)
//#define MY_GATEWAY_MAX_CLIENTS 2

// Use a bit lower baudrate for serial prints on ESP8266 than default in
// MyConfig.h
#define MY_BAUD_RATE 115200

#define MY_NODE_ID 0

#define EXPAND(N) QUOTE(N)
#define QUOTE(NODE_ID) #NODE_ID
// Set this node's subscribe and publish topic prefix

#define MY_GATEWAY_ESP8266
#define MY_GATEWAY_MQTT_CLIENT
#define MY_MQTT_CLIENT_ID "mysensors-" NODE_ID
#define MY_MQTT_PUBLISH_TOPIC_PREFIX MY_MQTT_CLIENT_ID "-out"
#define MY_MQTT_SUBSCRIBE_TOPIC_PREFIX MY_MQTT_CLIENT_ID "-in"
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

#endif
