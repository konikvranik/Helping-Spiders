/*
 * OTAComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef OTACOMPONENT_H_
#define OTACOMPONENT_H_

#include "../AbstractComponent.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

class OTAComponent : public AbstractComponent {
  ESP8266WebServer *_server;
  ESP8266HTTPUpdateServer httpUpdater;
  String nodeId;

public:
  OTAComponent(const String, ESP8266WebServer *);
  virtual ~OTAComponent();
  virtual void setup();
  virtual void loop();
  virtual void receive(const MyMessage &);
  virtual void presentation();
  virtual void reportStatus(JsonObject &);
  virtual String moduleName();
};

#endif /* OTACOMPONENT_H_ */
