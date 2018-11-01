/*
 * HcSr04Component.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "HCSR04.h"

HcSr04Component::HcSr04Component(String node_id, const uint8_t sensor_id,
								 const int16_t trigPin, const int16_t echoPin) : AbstractComponent(node_id, sensor_id)
{
	this->trigPin = trigPin, this->echoPin = echoPin;
}

void HcSr04Component::setup()
{
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	this->sensor = new NewPing(trigPin, echoPin, 250);
}

void HcSr04Component::loop()
{
}

float HcSr04Component::getDistance()
{
	if (lastRun == 0 || lastRun < millis() - delayMS)
	{
		float tmp = this->sensor->ping_cm() / 100.000;
		lastRun = millis();
		if (tmp > 0)
		{
			if (this->distance != tmp)
			{
				const String topic = this->makeTopic("");
			}
			this->distance = tmp;
		}
	}
	return this->distance;
}

void HcSr04Component::reportStatus(JsonObject &jo)
{
	jo["ID"] = this->sensor_id;
	jo["topic"] = this->makeTopic("");
	jo["type"] = this->AbstractDistanceComponent::getType();
	jo["value"] = this->getDistance();
	jo["unit"] = "m";
}

String HcSr04Component::prometheus()
{
	if (this->getDistance() <= 0)
		return "";
	Prometheus p("distance", this->getDistance(), "gauge",
				 "Distance measured by HC-SR04 ultrasonic sensor.");
	p.attribute("unit", "m");
	p.attribute("type", "distance");
	return p.to_string(true);
}

String HcSr04Component::moduleName()
{
	return "HC-SR04";
}
