/*
 * DHTComponent.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "DHTComponent.h"

DHTComponent::DHTComponent(const uint8_t sensor_id, const uint8_t hum_sensor_id,
                           const int16_t pin)
    : AbstractComponent(sensor_id) {
  this->hum_sensor_id = hum_sensor_id, this->pinDHT = pin,
  temp_msg = MyMessage(sensor_id, V_TEMP);
  hum_msg = MyMessage(hum_sensor_id, V_HUM);
  pinMode(sensor_id, INPUT);
  pinMode(hum_sensor_id, INPUT);
}

DHTComponent::~DHTComponent() {
  // TODO Auto-generated destructor stub
}

void DHTComponent::setup() {
  // Set delay between sensor readings based on sensor details.
  if (dhtDelayMS < DHT_DELAY)
    dhtDelayMS = DHT_DELAY;
}

void DHTComponent::presentation() {
  present(sensor_id, S_TEMP, "Temperature");
  present(hum_sensor_id, S_HUM, "Humidity");
}

void DHTComponent::loop() {
  if (dhtLastRun + dhtDelayMS < millis()) {
    double old_temp = sensor.temperature, old_hum = sensor.humidity;
    // Get temperature event and print its value.
    if (err = sensor.read21(pinDHT)) {
      Log.error("Read DHT22 failed.");
      return;
    }
    dhtLastRun = millis();
    if (old_temp != sensor.temperature)
      send(temp_msg.set(sensor.temperature, 2));
    if (old_hum != sensor.humidity)
      send(hum_msg.set(sensor.humidity, 2));
  }
}

float DHTComponent::getHumidity() { return sensor.humidity; }

float DHTComponent::getTemperature() { return sensor.temperature; }

void DHTComponent::reportStatus(JsonObject &jo) {
  JsonObject &id = jo.createNestedObject("ID");
  id["temperature"] = this->sensor_id;
  id["humidity"] = this->hum_sensor_id;
  jo["Temperature"] = String(this->getTemperature()) + " °C";
  jo["Humidity"] = String(this->getHumidity()) + " %";
}

void DHTComponent::receive(const MyMessage &) {}

String DHTComponent::moduleName() { return "DHT"; }
