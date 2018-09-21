/*
 * AbstractComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef ABSTRACT_COMPONENT_H_
#define ABSTRACT_COMPONENT_H_

#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <core/MyMessage.h>
#include <core/MySensorsCore.h>

#define MSG_PERIOD 3000

class AbstractComponent {
public:
	uint8_t sensor_id;
	AbstractComponent(const uint8_t sensor_id) {
		this->sensor_id = sensor_id;
	}
	virtual void setup()=0;
	virtual void loop()=0;
	virtual void receive(const MyMessage &)=0;
	virtual void presentation()=0;
	virtual void reportStatus(JsonObject &)=0;
	virtual String prometheus() {
		return "";
	}
	virtual String moduleName()=0;
};

/*
 String AbstractComponent::prometheus() {
 return "none{} 0" CR;
 }
 */

#endif /* ABSTRACT_COMPONENT_H_ */
