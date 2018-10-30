/*
 * DHTComponent.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "DHTComponent.h"

DHTComponent::DHTComponent(const String node_id, const uint8_t sensor_id, const uint8_t hum_sensor_id,
                           const int16_t pin)
    : AbstractComponent(node_id, sensor_id)
{
  this->hum_sensor_id = hum_sensor_id, this->pinDHT = pin,
  pinMode(sensor_id, INPUT);
  pinMode(hum_sensor_id, INPUT);
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
}

void DHTComponent::loop()
{
  if (dhtLastRun + dhtDelayMS < millis())
  {
    double old_temp = sensor.temperature, old_hum = sensor.humidity;
    if (err == sensor.read21(pinDHT))
    {
      return;
    }
    dhtLastRun = millis();
    if (old_temp != sensor.temperature)
      true;
    if (old_hum != sensor.humidity)
      true;
  }
}

float DHTComponent::getHumidity() { return sensor.humidity; }

float DHTComponent::getTemperature() { return sensor.temperature; }

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

String DHTComponent::moduleName() { return "DHT"; }
