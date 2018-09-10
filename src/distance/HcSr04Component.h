/*
 * HcSr04Component.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef HC_SR_04_COMPONENT_H_
#define HC_SR_04_COMPONENT_H_

#include "../AbstractComponent.h"
#include "AbstractDistanceComponent.h"
#include "Ultrasonic.h"

#define HC_SR04_DELAY 60000 // 15*60*1000

class HcSr04Component : public AbstractComponent,
                        public AbstractDistanceComponent {
  MyMessage distance_msg;
  float distance = 0;
  uint32_t delayMS = 2000, lastRun = 0;
  int16_t trigPin = 0, echoPin = 0;

public:
  Ultrasonic sensor;
  HcSr04Component(const uint8_t, const int16_t, const int16_t);
  virtual ~HcSr04Component();
  virtual void setup();
  virtual void loop();
  virtual void receive(const MyMessage &);
  virtual void presentation();
  virtual float getDistance();
  virtual void reportStatus(JsonObject &);
  virtual String moduleName();
};

#endif /* HC_SR_04_COMPONENT_H_ */
