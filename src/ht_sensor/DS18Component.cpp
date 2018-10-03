/* Copyright 2018 <hpa@suteren.net>
 * DS18Component.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "DS18Component.h"

String DS18Component::addr2string(DeviceAddress deviceAddr) {
	String result = "";
	for (uint8_t i = 0; i < 8; i++) {
		if (i > 0)
			result = result + "-";
		char hexString[20];
		itoa(deviceAddr[i], hexString, 16);
		result = result + String(hexString);
	}
	return result;
}

// Continue to check if the IC has responded with a temperature
bool DS18Component::deviceIsReady() {
	if (this->sensor.getCheckForConversion()
			&& !this->sensor.isParasitePowerMode()) {
		return (this->sensor.isConversionComplete()
				|| (millis() - conversionTime >= this->lastRun));
	} else {
		return millis() - conversionTime >= this->lastRun;
	}
}

DS18Component::DS18Component(const uint8_t sensor_id, const int16_t pin) :
		AbstractComponent(sensor_id) {
	for (int i = 0; i < MAX_DEVICES; i++)
		this->temps[i] = -127;
	pinMode(pin, INPUT);
	pinMode(this->pin, INPUT);
	this->_wire = new OneWire(this->pin);
	this->pin = pin;
	this->sensor = DallasTemperature();
	this->sensor.setOneWire(this->_wire);
	this->sensor.begin();
	conversionTime = this->sensor.millisToWaitForConversion(
			this->sensor.getResolution());
}

DS18Component::~DS18Component() {
	// TODO(hPa) Auto-generated destructor stub
}

void DS18Component::setup() {
	this->sensor.setWaitForConversion(!async);
	for (int i = 0; i < MAX_DEVICES; i++) {
		DeviceAddress da;
		if (this->sensor.getAddress(da, i)) {
			if (this->sensor.validFamily(da)) {
				memcpy(this->devices[this->ds18Count++], da, sizeof(da));
			}
		} else {
			break;
		}
	}
	// Set delay between sensor readings based on sensor details.
	if (delayMS < DS18_DELAY)
		delayMS = DS18_DELAY;
}

void DS18Component::presentation() {
	this->temp_msg = new MyMessage[this->ds18Count];
	for (uint8_t i = 0; i < this->ds18Count; i++) {
		this->temp_msg[i] = MyMessage(this->sensor_id + i, V_TEMP);
		present(this->sensor_id + i, S_TEMP,
				(String(addr2string(this->devices[i]))).c_str());
	}
}

void DS18Component::loop() {
	if (lastRun == 0 || lastRun + delayMS < millis()) {
		// Get temperature event and print its value.
		this->lastRun = millis();
		this->sensor.requestTemperatures();
		this->request = true;
	}
	if (this->request && (!async || this->deviceIsReady())) {
		for (int8_t i = 0; i < this->ds18Count; i++) {
			float tmp = this->sensor.getTempC(this->devices[i]);
			if (tmp != -127) {
				if (tmp != this->temps[i]) {
					this->temps[i] = tmp;
					send(this->temp_msg[i].set(this->temps[i], 2));
				}

			}
		}
		this->request = false;
	}
}

float DS18Component::getTemperature() {
	return NULL;
}

void DS18Component::reportStatus(JsonObject &jo) {
	jo["ID"] = this->sensor_id;
	JsonObject &params = jo.createNestedObject("params");
	params["parasite"] = this->sensor.isParasitePowerMode();
	params["count"] = this->sensor.getDeviceCount();
	params["DS18count"] = this->sensor.getDS18Count();
	params["complete"] = this->sensor.isConversionComplete();
	JsonObject &devices = jo.createNestedObject("devices");
	for (int8_t i = 0; i < this->ds18Count; i++) {
		JsonObject &dv = devices.createNestedObject(
				String(this->sensor_id + i));
		dv["address"] = addr2string(this->devices[i]);
		dv["topic"] = String(MY_MQTT_PUBLISH_TOPIC_PREFIX "/0/") + String(this->sensor_id + i) + String("/1/0/0");
		dv["type"] = "temperature";
		dv["value"] = this->temps[i];
		dv["unit"] = "°C";
	}
}

void DS18Component::receive(const MyMessage &) {
}

String DS18Component::prometheus() {
	String s = "";
	Prometheus* p;
	for (int8_t i = 0; i < this->ds18Count; i++) {
		if (this->temps[i] == -127) continue;
		p = new Prometheus(NODE_ID"_" + String(i) + "_temperature", this->temps[i], "gauge", "Temperature measured by DS18b20 with address " + addr2string(this->devices[i]) + " on "+ String(i)+". position.");
		p->attribute("device", String(i));
		p->attribute("address", addr2string(this->devices[i]));
		p->attribute("unit", "°C");
		p->attribute("type", "temperature");
		s += p->to_string(true);
		delete p;
	}
	return s;
}

String DS18Component::moduleName() {
	return "DS18";
}
