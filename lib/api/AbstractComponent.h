/*
 * AbstractComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef ABSTRACT_COMPONENT_H_
#define ABSTRACT_COMPONENT_H_

#include <ArduinoJson.h>

class AbstractComponent {
public:
	uint8_t sensor_id;
	String node_id;
	AbstractComponent(const String node_id, const uint8_t sensor_id) { // @suppress("Class members should be properly initialized")
		this->sensor_id = sensor_id;
		this->node_id = node_id;
	}
	virtual void setup()=0;
	virtual void loop()=0;
	virtual void receive(String topic, String data, bool cont) {
	}
	virtual String getType() {
		return "unknown";
	}
	virtual String makeTopic(String suffix) {
		return node_id + String("/") + String(this->sensor_id)
				+ String("/") + getType() + String("/") + suffix;
	}
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
