/* Copyright 2018 <hpa@suteren.net>
 * DS18Component.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "DS18Component.h"

DS18Component::DS18Component(const uint8_t sensor_id, const int16_t pin)
    : AbstractComponent(sensor_id) {
  pinMode(pin, INPUT);
  OneWire oneWireDS(pin);
  this->sensor = DallasTemperature(&oneWireDS);
  // this->sensor.setWaitForConversion(false);
}

DS18Component::~DS18Component() {
  // TODO(hPa) Auto-generated destructor stub
}

void DS18Component::setup() {
  this->sensor.begin();
  // Set delay between sensor readings based on sensor details.
  if (delayMS < DS18_DELAY)
    delayMS = DS18_DELAY;
}

void DS18Component::presentation() {
  this->sensor.requestTemperatures();
  uint8_t cnt = this->sensor.getDeviceCount();
  temp_msg = new MyMessage[this->sensor.getDS18Count()];
  uint8_t *deviceAddress;
  for (uint8_t i = 0, j = 0; i < cnt; i++) {
    if (this->sensor.getAddress(deviceAddress, i) &&
        this->sensor.validFamily(deviceAddress)) {
      temp_msg[j] = MyMessage(sensor_id + j, V_TEMP);
      present(sensor_id + i, S_TEMP, (String("Temperature addr:") +
                                      String(atoi((const char *)deviceAddress)))
                                         .c_str());
      j++;
    }
  }
}

void DS18Component::loop() {
  if (lastRun + delayMS < millis()) {
    // Get temperature event and print its value.
    this->sensor.requestTemperatures();
    uint8_t *deviceAddress;
    for (int8_t i = 0, j = 0; i < this->sensor.getDeviceCount(); i++) {
      if (this->sensor.getAddress(deviceAddress, i) &&
          this->sensor.validFamily(deviceAddress)) {
        send(temp_msg[sensor_id + j++].set(this->sensor.getTempC(deviceAddress),
                                           2));
      }
    }
    lastRun = millis();
  }
}

float DS18Component::getTemperature() { return false; }

void DS18Component::reportStatus(JsonObject &jo) {
  JsonObject &id = jo.createNestedObject("ID");
  id["temperature"] = this->sensor_id;
  jo["Temperature"] = String(this->getTemperature()) + " °C";
}

void DS18Component::receive(const MyMessage &) {}

String DS18Component::moduleName() { return "DS18"; }
