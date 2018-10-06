/*
 * OTAComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef OTACOMPONENT_H_
#define OTACOMPONENT_H_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include "../src/AbstractComponent.h"

class OTAComponent: public AbstractComponent {
	ESP8266WebServer *_server;
	ESP8266HTTPUpdateServer httpUpdater;
	String nodeId;
	boolean updating = false;

public:
	OTAComponent(const String, ESP8266WebServer *);
	virtual ~OTAComponent();
	virtual void setup();
	virtual void loop();
	virtual void reportStatus(JsonObject &);
	virtual String moduleName();
	virtual boolean isUpdating();
};

#endif /* OTACOMPONENT_H_ */
