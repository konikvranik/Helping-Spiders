/*
 * DHTComponent.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "DHTComponent.h"

DHTComponent::DHTComponent(const String node_id, const uint8_t sensor_id, const uint8_t hum_sensor_id, const int16_t pin)
    : AbstractComponent(node_id, sensor_id)
{
  this->hum_sensor_id = hum_sensor_id;
  this->sensor = new DHT(pin, DHT11);
}

DHTComponent::~DHTComponent()
{
  // TODO Auto-generated destructor stub
}

void DHTComponent::setup()
{
  // Set delay between sensor readings based on sensor details.
  if (dhtDelayMS < DHT_DELAY)
    dhtDelayMS = DHT_DELAY;

  this->sensor->begin();
}

void DHTComponent::loop()
{
}

double DHTComponent::getHumidity()
{
  return this->sensor->readHumidity();
}

double DHTComponent::getTemperature()
{
  return this->sensor->readTemperature();
}

void DHTComponent::reportStatus(JsonObject &jo)
{
  JsonObject &temp = jo.createNestedObject("temperature");
  temp["ID"] = this->sensor_id;
  temp["topic"] = makeTopic("");
  temp["type"] = "temperature";
  temp["value"] = String(this->getTemperature());
  temp["unit"] = "°C";
  JsonObject &hum = jo.createNestedObject("humidity");
  hum["ID"] = this->hum_sensor_id;
  hum["topic"] = makeTopic("");
  hum["type"] = "humidity";
  hum["value"] = String(this->getHumidity());
  hum["unit"] = "%";
}

String DHTComponent::prometheus()
{
  String s = "";
  Prometheus *p;
  p = new Prometheus(
      this->AbstractTemperatureComponent::getType(), this->getTemperature(), "gauge", String("Temperature measured by DHT11"));
  p->attribute("unit", "°C");
  p->attribute("type", this->AbstractTemperatureComponent::getType());
  p->attribute("sensor", "DHT11");
  s += p->to_string(true);
  delete p;

  p = new Prometheus(
      this->AbstractHumidityComponent::getType(), this->getHumidity(), "gauge", String("Humidity measured by DHT11"));
  p->attribute("unit", "%");
  p->attribute("type", this->AbstractHumidityComponent::getType());
  p->attribute("sensor", "DHT11");
  s += p->to_string(true);
  delete p;

  p = new Prometheus(
      "heatIndex", this->sensor->computeHeatIndex(this->getTemperature(), this->getHumidity(), false), "gauge", String("Heat index measured by DHT11"));
  p->attribute("unit", "°C");
  p->attribute("type", "heatIndex");
  p->attribute("sensor", "DHT11");
  s += p->to_string(true);
  delete p;

  return s;
}

String DHTComponent::moduleName() { return "DHT"; }
