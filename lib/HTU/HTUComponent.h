/*
 * HTUComponent.h
 *
 *  Created on: 8. 7. 2017
 *      Author: petr
 */

#ifndef HTUCOMPONENT_H_
#define HTUCOMPONENT_H_

#include "AbstractComponent.h"
#include "AbstractHumidityComponent.h"
#include "AbstractTemperatureComponent.h"
#include <ArduinoLog.h>
#include <Wire.h>
//#include <SparkFunHTU21D.h>
#include <Adafruit_HTU21DF.h>
//#include <Sodaq_SHT2x.h>

#define HTU_DELAY 2000 //15*60*1000

class HTUComponent: public AbstractComponent,
		public AbstractTemperatureComponent,
		public AbstractHumidityComponent {
	float temp = -273, hum = -1;
	uint32_t delayMS = 2000, lastRun = 0;
	//HTU21D sensor ;

	Adafruit_HTU21DF sensor;
	uint8_t sda, scl;

public:
	uint8_t hum_sensor_id;
	HTUComponent(const String node_id, const uint8_t sensor_id, const uint8_t hum_sensor_id,
				 const uint8_t scl, const uint8_t sda);
	virtual ~HTUComponent();
	virtual void setup();
	virtual void loop();
	virtual float getHumidity();
	virtual float getTemperature();
	virtual void reportStatus(JsonObject&);
	virtual String moduleName();
};

#endif /* HTUCOMPONENT_H_ */
