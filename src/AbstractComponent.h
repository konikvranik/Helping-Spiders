/*
 * AbstractComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef ABSTRACT_COMPONENT_H_
#define ABSTRACT_COMPONENT_H_

#include "core.h"
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <core/MyMessage.h>
#include <core/MySensorsCore.h>

#define MSG_PERIOD 3000

class AbstractComponent {
public:
  uint8_t sensor_id;
  AbstractComponent(const uint8_t sensor_id) { this->sensor_id = sensor_id; }
  virtual void setup();
  virtual void loop();
  virtual void receive(const MyMessage &);
  virtual void presentation();
  virtual void reportStatus(JsonObject &);
  virtual String moduleName();
};

#endif /* ABSTRACT_COMPONENT_H_ */
