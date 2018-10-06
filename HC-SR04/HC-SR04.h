/*
 * HcSr04Component.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef HC_SR_04_COMPONENT_H_
#define HC_SR_04_COMPONENT_H_

#include <Ultrasonic.h>
#include "../src/AbstractDistanceComponent.h"
#include "../src/Prometheus.h"

class HcSr04Component: public AbstractDistanceComponent {
	float distance = 0;
	uint32_t delayMS = 2000, lastRun = 0;
	int16_t trigPin = 0, echoPin = 0;

public:
	Ultrasonic sensor;
	HcSr04Component(String, const uint8_t, const int16_t, const int16_t);
	virtual ~HcSr04Component();
	virtual void setup();
	virtual void loop();
	virtual void presentation(MQTTClient mqtt);
	virtual float getDistance();
	virtual void reportStatus(JsonObject &);
	String prometheus();

	virtual String moduleName();

};

#endif /* HC_SR_04_COMPONENT_H_ */
