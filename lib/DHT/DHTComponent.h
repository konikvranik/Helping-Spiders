/*
 * DHTComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef DHTCOMPONENT_H_
#define DHTCOMPONENT_H_

#include "AbstractComponent.h"
#include "AbstractHumidityComponent.h"
#include "AbstractTemperatureComponent.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "Prometheus.h"

#define DHT_DELAY 2000 // 15*60*1000

class DHTComponent : public AbstractComponent,
                     public AbstractHumidityComponent,
                     public AbstractTemperatureComponent
{
  uint32_t dhtDelayMS = 10000, dhtLastRun = 0;

public:
  uint8_t hum_sensor_id;
  DHT *sensor;
  DHTComponent(const String node_id, const uint8_t, const uint8_t, const int16_t);
  virtual ~DHTComponent();
  virtual void setup();
  virtual double getHumidity();
  virtual double getTemperature();
  virtual void reportStatus(JsonObject &);
  virtual String prometheus();
  virtual String moduleName();
};

#endif /* DHTCOMPONENT_H_ */
