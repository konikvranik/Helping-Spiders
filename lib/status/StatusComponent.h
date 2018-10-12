/*
 * StatusComponent.h
 *
 *  Created on: 27. 6. 2017
 *      Author: hpa
 */

#ifndef STATUSCOMPONENT_H_
#define STATUSCOMPONENT_H_

#include <ESP8266WebServer.h>
#include <NtpClientLib.h>
#include <ArduinoLog.h>
#include "AbstractComponent.h"
#include "AbstractHumidityComponent.h"
#include "AbstractTemperatureComponent.h"
#include "AbstractDistanceComponent.h"
#include "Prometheus.h"

#define LOG_PERIOD 5000 // ms

class StatusComponent: public AbstractComponent {
	Print *logOutput = NULL;
	ESP8266WebServer *webServer = NULL;
	uint32_t last_log = millis(), loop_count = 0;
	String ModulesStatus();
	String ModulesJson();
	String nodeId;
	uint8_t temp_id, hum_id;
	AbstractComponent **components;
	uint8_t components_count = 0;
	void init(String, AbstractComponent *[], uint8_t);
	String moduleNames();
	void jsonPrefix(JsonObject &);
	void jsonSuffix(JsonObject &);

public:
	float lps = -1;
	StatusComponent(String, AbstractComponent *[], uint8_t, Print *);
	StatusComponent(String, AbstractComponent *[], uint8_t, ESP8266WebServer *);
	virtual ~StatusComponent();
	virtual void setup();
	virtual void loop();
	virtual void reportStatus(JsonObject &);
	virtual String moduleName();
	String prometheusReport();
};

#endif /* STATUSCOMPONENT_H_ */
