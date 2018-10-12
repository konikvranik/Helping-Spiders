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
	this->pin = pin;
	this->_wire = new OneWire(this->pin);
	this->sensor = DallasTemperature();
	this->sensor.setOneWire(this->_wire);
}

DS18Component::~DS18Component()
{
	// TODO(hPa) Auto-generated destructor stub
}

void DS18Component::setup()
{
	this->sensor.begin();
}

void DS18Component::loop()
{
}

void DS18Component::readTemps(JsonArray &jo)
{
	if (!(0 < lastRun && lastRun < millis() - delayMS))
		this->sensor.requestTemperatures();
	for (int i = 0; i < this->sensor.getDeviceCount(); i++)
	{
		DeviceAddress addr;
		this->sensor.getAddress(addr, i);
		if (this->sensor.validFamily(addr))
		{
			JsonObject &t = jo.createNestedObject();
			t["address"] = addr2string(addr);
			//this->sensor.requestTemperaturesByAddress(devices[i]);
			float tempC = this->sensor.getTempC(addr);
			t["value"] =
				tempC == DEVICE_DISCONNECTED_C ? "N/A" : String(tempC);
			t["resolution"] = String(
				this->sensor.getResolution(addr));
			t["type"] = "temperature";
			t["unit"] = "°C";
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
	JsonObject &params = jo.createNestedObject("params");
	params["parasite"] = this->sensor.isParasitePowerMode();
	params["count"] = this->sensor.getDeviceCount();
	params["DS18count"] = this->sensor.getDS18Count();
	params["complete"] = this->sensor.isConversionComplete();
	params["resolution"] = this->sensor.getResolution();
	readTemps(jo.createNestedArray("devices"));
}

String DS18Component::prometheus()
{
	String s = "";
	Prometheus *p;
	StaticJsonBuffer<1000> jsonBuff;
	JsonArray &arr = jsonBuff.createArray();
	readTemps(arr);
	for (auto d : arr)
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
