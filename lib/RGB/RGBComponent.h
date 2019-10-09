/*
 * RGBComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef RGBCOMPONENT_H_
#define RGBCOMPONENT_H_

#include "Color.h"
#include "Colors.h"
#include "AbstractComponent.h"
#include <Arduino.h>
#include <TimeLib.h>
#include <math.h>
#include <stdio.h>
#include "Candle.h"
#include "common.h"

#define MODE_DEFAULT 0
#define MODE_DAYTIME 1
#define MODE_CANDLE 2
#define MODE_UNDEF 0xff
#define BLEND_TIME 80000

#define LIGHT_OFF 0
#define LIGHT_ON 1

#define DAYTIMECOLOR Color(238, 12, 0)

class RGBComponent : public virtual AbstractComponent
{
  uint32_t last_light_msg = millis();
  ESP8266WebServer *webServer = nullptr;
  uint32_t last_render = 0;
  uint32_t render_finish = 0;
  Color current_color;
  Color desired_color = _BLACK;
  const uint8_t red_pin;
  const uint8_t green_pin;
  const uint8_t blue_pin;
  Candle candle;

  void blend(Color c, uint32_t time = BLEND_TIME);
  void doOnRest();
  void sendMessage();

public:
  uint8_t mode = MODE_UNDEF;
  int16_t light_state = LIGHT_OFF;
  Color rgb = _BLACK;
  RGBComponent(const String node_id, const uint8_t, const uint16_t, const uint16_t, const uint16_t);
  virtual ~RGBComponent();
  virtual void setup();
  virtual void loop();
  //virtual void receive(String topic, String data, bool cont);
  void registerRest(ESP8266WebServer &);
  virtual void reportStatus(JsonObject &);
  static const Color h2c(const String);
  static const Color cn(Color rgb);
  static const Color daytimeColor();
  const String getModeName();
  virtual String moduleName();
  virtual void updateCandle();
};

#endif /* RGBCOMPONENT_H_ */
