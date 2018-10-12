/* Copyright 2018 <hpa@suteren.net>
 * DS18Component.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "DS18.h"

String DS18Component::addr2string(DeviceAddress deviceAddr)
{
	String result = "";
	for (uint8_t i = 0; i < 8; i++)
	{
		if (i > 0)
			result = result + "-";
		char hexString[20];
		itoa(deviceAddr[i], hexString, 16);
		result = result + String(hexString);
	}
	return result;
}

DS18Component::DS18Component(String node_id, const uint8_t sensor_id,
							 const int16_t pin) : AbstractComponent(node_id, sensor_id)
{
	pinMode(pin, INPUT);
	pinMode(this->pin, INPUT);
	this->_wire = new OneWire(this->pin);
	this->pin = pin;
	this->sensor = DallasTemperature();
	this->sensor.setOneWire(this->_wire);
	this->sensor.begin();
}

DS18Component::~DS18Component()
{
	// TODO(hPa) Auto-generated destructor stub
}

void DS18Component::setup()
{
}

void DS18Component::loop()
{
}

void DS18Component::readTemps(JsonArray &jo)
{
	if (0 < lastRun && lastRun < millis() - delayMS)
		return;

	this->sensor.begin();

	DeviceAddress deviceAddress;

	_wire->reset_search();

	while (_wire->search(deviceAddress))
	{
		JsonObject &t = jo.createNestedObject();
		t["parasite"] = String(this->sensor.isParasitePowerMode());
		if (this->sensor.validAddress(deviceAddress))
		{
			if (this->sensor.validFamily(deviceAddress))
			{
				t["address"] = addr2string(deviceAddress);
				this->sensor.requestTemperaturesByAddress(deviceAddress);
				float tempC = this->sensor.getTempC(deviceAddress);
				t["value"] =
					tempC == DEVICE_DISCONNECTED_C ? "N/A" : String(tempC);
				t["resolution"] = String(
					this->sensor.getResolution(deviceAddress));
				t["type"] = "temperature";
				t["unit"] = "°C";
			}
		}
	}

	// Get temperature event and print its value.
	this->lastRun = millis();
}

float DS18Component::getTemperature()
{
	return NULL;
}

void DS18Component::reportStatus(JsonObject &jo)
{
	jo["ID"] = this->sensor_id;
	JsonArray &devices = jo.createNestedArray("devices");
	readTemps(devices);
	JsonObject &params = jo.createNestedObject("params");
	params["parasite"] = this->sensor.isParasitePowerMode();
	params["count"] = this->sensor.getDeviceCount();
	params["DS18count"] = this->sensor.getDS18Count();
	params["complete"] = this->sensor.isConversionComplete();
}

String DS18Component::prometheus()
{
	String s = "";
	Prometheus *p;
	JsonArray &arr = tempBuffer.createNestedArray();
	readTemps(arr);

	for (auto d : tempBuffer)
	{
		if (d["value"] == "N/A")
			continue;
		p = new Prometheus(
			"DS18_" + this->AbstractTemperatureComponent::getType(),
			d["value"], "gauge",
			String("Temperature measured by DS18b20 with address ") + d["address"].asString());
		p->attribute("address", d["address"]);
		p->attribute("unit", "°C");
		p->attribute("type", "temperature");
		s += p->to_string(true);
		delete p;
	}
	return s;
}

String DS18Component::moduleName()
{
	return "DS18";
}
