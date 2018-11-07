/* Copyright 2018 <hpa@suteren.net>
 * DS18Component.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef SRC_HT_SENSOR_DS18COMPONENT_H_
#define SRC_HT_SENSOR_DS18COMPONENT_H_

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ArduinoJson.h>
#include <AbstractTemperatureComponent.h>
#include <AbstractTemperatureComponent.h>
#include <Prometheus.h>

#define DS18_DELAY 2000

class DS18Component : public AbstractComponent,
					  public AbstractTemperatureComponent
{
	uint32_t delayMS = DS18_DELAY, lastRun = 0;
	int16_t pin;
	DallasTemperature sensor;
	String addr2string(DeviceAddress deviceAddr);
	String scratchpad2string(uint8_t *deviceAddr);
	void readTemps(JsonArray &jo);
	OneWire *_wire;

  public:
	DS18Component(String nodeid, const uint8_t, const int16_t);
	virtual ~DS18Component();
	virtual void setup();
	virtual void loop();
	virtual double getTemperature();
	virtual void reportStatus(JsonObject &);
	String prometheus();
	virtual String moduleName();
};

#endif // SRC_HT_SENSOR_DS18COMPONENT_H_
