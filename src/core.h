#ifndef INIT_INCLUDE_h
#define INIT_INCLUDE_h

//#define ARDUINO 100
//#define CR "\r\n"

#include <Arduino.h>
#include <HardwareSerial.h>
#include <ESP8266WiFi.h>
#include <Esp.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ArduinoLog.h>
#include <FS.h>

#include "AbstractComponent.h"

#define str(s) xstr(s)
#define xstr(s) #s

// Set this node's subscribe and publish topic prefix

// Enable these if your MQTT broker requires usenrame/password

#define DEBUG_ESP_PORT Serial
#ifndef DEBUG
#define DISABLE_LOGGING
#endif

#include <StatusComponent.h>
#include <OTAComponent.h>

#ifdef ENABLE_RGB
#include "RGBComponent.h"
#endif
#ifdef ENABLE_IR
#include "IRComponent.h"
#endif
#ifdef ENABLE_ILIFE
#include "iLifeComponent.h"
#endif
#ifdef ENABLE_DHT
#include "DHTComponent.h"
#endif
#ifdef ENABLE_HTU
#include "HTUComponent.h"
#endif
#ifdef ENABLE_DS18
#include "DS18.h"
#endif
#ifdef ENABLE_HCSR04
#include "HCSR04.h"
#endif

ESP8266WebServer http_server(80);

AbstractComponent *modules[] =
    {
        new OTAComponent(str(NODE_ID), &http_server),
#ifdef ENABLE_RGB
        new RGBComponent(str(NODE_ID), RGB_CHILD_ID, RED_PIN, GREEN_PIN, BLUE_PIN),
#endif
#ifdef ENABLE_IR
        new IRComponent(str(NODE_ID), IR_CHILD_ID, IRRX_PIN, IRTX_PIN),
#endif
#ifdef ENABLE_ILIFE
        new iLifeComponent(ILIFE_CHILD_ID, IRTX_PIN),
#endif
#ifdef ENABLE_DHT
        new DHTComponent(str(NODE_ID), TEMP_CHILD_ID, HUM_CHILD_ID, DHT_PIN),
#endif
#ifdef ENABLE_HTU
        new HTUComponent(TEMP_CHILD_ID, HUM_CHILD_ID, HTU_SCL, HTU_SDA),
#endif
#ifdef ENABLE_DS18
        new DS18Component(str(NODE_ID), TEMP_CHILD_ID, DS18_PIN),
#endif
#ifdef ENABLE_HCSR04
        new HcSr04Component(str(NODE_ID), DIST_CHILD_ID, HCSR04_TRIG_PIN, HCSR04_ECHO_PIN),
#endif
};

#endif

#ifndef PINMODE2
#define PINMODE2 OUTPUT
#endif

#ifndef PINMODE0
#define PINMODE0 OUTPUT
#endif

#ifndef PINMODE15
#define PINMODE15 OUTPUT
#endif