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
#include <ESP8266MQTTClient.h>

#define MSG_PERIOD 3000

class AbstractComponent {
public:
	uint8_t sensor_id;
	AbstractComponent(const uint8_t sensor_id) {
		this->sensor_id = sensor_id;
	}
	virtual void setup()=0;
	virtual void loop()=0;
	virtual void receive(String topic, String data, bool cont) {
	}
	virtual void presentation(MQTTClient* mqtt) {
		this->mqtt = mqtt;
	}
	virtual String getType() {
		return "unknown";
	}
	virtual String makeTopic(String suffix) {
		return String(NODE_ID"/")+ String(this->sensor_id) + String("/") + getType() + String("/")
				 + suffix;
	}
	virtual void reportStatus(JsonObject &)=0;
	virtual String prometheus() {
		return "";
	}
	virtual String moduleName()=0;
private:
	MQTTClient* mqtt;
};

/*
 String AbstractComponent::prometheus() {
 return "none{} 0" CR;
 }
 */

#endif /* ABSTRACT_COMPONENT_H_ */
