/* Copyright 2018 <hpa@suteren.net>
 * DS18B20Component.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef SRC_HT_SENSOR_DS18B20COMPONENT_H_
#define SRC_HT_SENSOR_DS18B20COMPONENT_H_

#include "../AbstractComponent.h"
#include "AbstractHumidityComponent.h"
#include "AbstractTemperatureComponent.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHT_DELAY 2000 // 15*60*1000

class DS18B20Component : public AbstractComponent,
                         public AbstractHumidityComponent,
                         public AbstractTemperatureComponent {
  MyMessage temp_msg, hum_msg;
  int16_t temp = -273, hum = -1;
  uint32_t dhtDelayMS = 2000, dhtLastRun = 0;
  int16_t pinDHT = 1;

public:
  uint8_t hum_sensor_id;
  dht sensor;
  DHTComponent(const uint8_t, const uint8_t, const int16_t);
  virtual ~DHTComponent();
  virtual void setup();
  virtual void loop();
  virtual void receive(const MyMessage &);
  virtual void presentation();
  virtual float getHumidity();
  virtual float getTemperature();
  virtual void reportStatus(JsonObject &);
  virtual String moduleName();
};

#endif // SRC_HT_SENSOR_DS18B20COMPONENT_H_
