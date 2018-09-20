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
  this->sensor = Ultrasonic(trigPin, echoPin);
}

HcSr04Component::~HcSr04Component() {
  // TODO Auto-generated destructor stub
}

void HcSr04Component::setup() {
  pinMode(1, FUNCTION_3);
  pinMode(3, FUNCTION_3);
  // Set delay between sensor readings based on sensor details.
  if (delayMS < HC_SR04_DELAY)
    delayMS = HC_SR04_DELAY;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void HcSr04Component::presentation() {
  present(sensor_id, S_DISTANCE, "Distance");
}

void HcSr04Component::loop() {
  if (lastRun == 0 || lastRun + delayMS < millis()) {
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
  jo["ID"] = this->sensor_id;
  jo["topic"] = String(MY_MQTT_PUBLISH_TOPIC_PREFIX "/0/") +
                String(this->sensor_id) + String("/1/0/13");
  jo["type"] = "distance";
  jo["value"] = this->getDistance();
  jo["unit"] = "cm";
}

void HcSr04Component::receive(const MyMessage &) {}

String HcSr04Component::prometheus() {
  return String("NODE_ID_") + String("_distance") + String("{ node=\"") +
         String("NODE_ID") + String("\", unit=\"cm\", type=\"distance\"} ") +
         String(this->getDistance()) + String(CR);
}

String HcSr04Component::moduleName() { return "HC-SR04"; }
