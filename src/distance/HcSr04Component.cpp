/*
 * HcSr04Component.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "HcSr04Component.h"

HcSr04Component::HcSr04Component(const uint8_t sensor_id, const int16_t trigPin,
                                 const int16_t echoPin)
    : AbstractComponent(sensor_id) {
  this->trigPin = trigPin, this->echoPin = echoPin,
  distance_msg = MyMessage(sensor_id, V_DISTANCE);
  pinMode(sensor_id, INPUT);
  this->sensor = Ultrasonic(trigPin, echoPin);
}

HcSr04Component::~HcSr04Component() {
  // TODO Auto-generated destructor stub
}

void HcSr04Component::setup() {
  // Set delay between sensor readings based on sensor details.
  if (delayMS < HC_SR04_DELAY)
    delayMS = HC_SR04_DELAY;
}

void HcSr04Component::presentation() {
  present(sensor_id, S_DISTANCE, "Distance");
}

void HcSr04Component::loop() {
  if (lastRun + delayMS < millis()) {
    double old_distance = this->distance;
    this->sensor.measure();
    this->distance = this->sensor.get_cm();
    lastRun = millis();
    if (old_distance != this->distance)
      send(distance_msg.set(this->distance, 2));
  }
}

float HcSr04Component::getDistance() { return this->distance; }

void HcSr04Component::reportStatus(JsonObject &jo) {
  JsonObject &id = jo.createNestedObject("ID");
  id["distance"] = this->sensor_id;
  jo["Distance"] = String(this->getDistance()) + " mm";
}

void HcSr04Component::receive(const MyMessage &) {}

String HcSr04Component::moduleName() { return "HC-SR04"; }