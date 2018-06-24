/* Copyright 2018 <hpa@suteren.net>
 * DS18Component.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef SRC_HT_SENSOR_DS18COMPONENT_H_
#define SRC_HT_SENSOR_DS18COMPONENT_H_

#include "../AbstractComponent.h"
#include "AbstractTemperatureComponent.h"
#include <DallasTemperature.h>
#include <OneWire.h>

#define DS18_DELAY 7000
#define MAX_DEVICES 255

class DS18Component : public AbstractComponent,
                      public AbstractTemperatureComponent {
  MyMessage *temp_msg;
  uint32_t delayMS = 2000, lastRun = 0;
  int16_t pin;
  DallasTemperature sensor;
  String addr2string(DeviceAddress deviceAddr);
  String scratchpad2string(uint8_t *deviceAddr);
  bool deviceIsReady();
  DeviceAddress devices[MAX_DEVICES];
  float temps[MAX_DEVICES];
  uint8_t ds18Count = 0;
  bool request = false;
  OneWire *_wire;

public:
  DS18Component(const uint8_t, const int16_t);
  virtual ~DS18Component();
  virtual void setup();
  virtual void loop();
  virtual void receive(const MyMessage &);
  virtual void presentation();
  virtual float getTemperature();
  virtual void reportStatus(JsonObject &);
  virtual String moduleName();
};

#endif // SRC_HT_SENSOR_DS18COMPONENT_H_
