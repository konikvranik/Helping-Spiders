/*
 * HTUComponent.cpp
 *
 *  Created on: 8. 7. 2017
 *      Author: petr
 */

#include "HTUComponent.h"

HTUComponent::HTUComponent(const String node_id, const uint8_t sensor_id, const uint8_t hum_sensor_id,
                           const uint8_t scl, const uint8_t sda)
    : AbstractComponent(node_id, sensor_id), hum_sensor_id(hum_sensor_id), sda(sda), scl(scl)
{
}

HTUComponent::~HTUComponent()
{
  // TODO Auto-generated destructor stub
}

void HTUComponent::setup()
{
  // Set delay between sensor readings based on sensor details.
  if (delayMS < HTU_DELAY)
    delayMS = HTU_DELAY;
  // TwoWire mywire;
  // Wire.pins(sda,scl);
  sensor.begin();
  // sensor.setResolution(USER_REGISTER_RESOLUTION_RH11_TEMP11);
}

void HTUComponent::loop()
{
  if (lastRun + delayMS < millis())
  {
    double old_temp = temp, old_hum = hum;
    temp = (sensor.readTemperature() - 32.0) * 5.0 / 9.0;
    //	    temp = SHT2x.GetTemperature();
    hum = sensor.readHumidity();
    //		hum = SHT2x.GetHumidity();
    lastRun = millis();
    if (old_temp != temp)
      true;
    if (old_hum != hum)
      true;
  }
}

float HTUComponent::getHumidity() { return hum; }

float HTUComponent::getTemperature() { return temp; }

void HTUComponent::reportStatus(JsonObject &jo)
{
  JsonObject &temp = jo.createNestedObject("temperature");
  temp["ID"] = this->sensor_id;
  temp["topic"] = makeTopic("temp");
  temp["type"] = "temperature";
  temp["value"] = String(this->getTemperature());
  temp["unit"] = "°C";
  JsonObject &hum = jo.createNestedObject("humidity");
  hum["ID"] = this->hum_sensor_id;
  hum["topic"] = makeTopic("hum");
  hum["type"] = "humidity";
  hum["value"] = String(this->getHumidity());
  hum["unit"] = "%";
}

String HTUComponent::moduleName() { return "HTU"; }
