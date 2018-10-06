/*
 * DHTComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef DHTCOMPONENT_H_
#define DHTCOMPONENT_H_

#include "../AbstractComponent.h"
#include "AbstractHumidityComponent.h"
#include "AbstractTemperatureComponent.h"
#include "dht.h"
#include "Prometheus.h"

#define DHT_DELAY 2000 // 15*60*1000

class DHTComponent : public AbstractComponent,
                     public AbstractHumidityComponent,
                     public AbstractTemperatureComponent {
  int16_t temp = -273, hum = -1;
  uint32_t dhtDelayMS = 2000, dhtLastRun = 0;
  int16_t pinDHT = 1;

public:
  uint8_t hum_sensor_id;
  dht sensor;
  uint8_t err = DHTLIB_OK;
  DHTComponent(const uint8_t, const uint8_t, const int16_t);
  virtual ~DHTComponent();
  virtual void setup();
  virtual void loop();
  virtual void presentation(MQTTClient* mqtt);
  virtual float getHumidity();
  virtual float getTemperature();
  virtual void reportStatus(JsonObject &);
  virtual String prometheus();
  virtual String moduleName();
};

#endif /* DHTCOMPONENT_H_ */
