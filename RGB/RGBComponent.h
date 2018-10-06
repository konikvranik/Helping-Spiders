/*
 * RGBComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef RGBCOMPONENT_H_
#define RGBCOMPONENT_H_

#include "../../lib/RGBLEDBlender/src/RGBLEDBlender.h"
#include "../AbstractComponent.h"
#include <Arduino.h>
#include <TimeLib.h>
#include <math.h>
#include <stdio.h>
#include "../RGB/Candle.h"

#define MODE_DEFAULT 0
#define MODE_DAYTIME 1
#define MODE_CANDLE 2
#define MODE_UNDEF 0xff
#define BLEND_TIME 800

#define LIGHT_OFF 0
#define LIGHT_ON 1

#define RGB_MAX_VALUE 0x100
#define BRIGHTNESS_MAX_VALUE 100

#define DAYTIMECOLOR Color(238, 12, 0)

class RGBComponent : public virtual AbstractComponent {
  uint32_t last_light_msg = millis();
  RGBLEDBlender rgbBlender;
  void blend(Color c);
  void sendMessage();

public:
  Candle candle;
  bool result = true;
  uint8_t mode = MODE_UNDEF;
  int16_t light_state = LIGHT_OFF;
  Color rgb = _BLACK;
  RGBComponent(const uint8_t, const uint16_t, const uint16_t, const uint16_t);
  virtual ~RGBComponent();
  virtual void setup();
  virtual void loop();
  virtual void receive(String topic, String data, bool cont);
  virtual void presentation(MQTTClient* mqtt);
  virtual void reportStatus(JsonObject &);
  static const String c2s(const Color);
  static const Color h2c(const String);
  static const Color cn(Color rgb);
  static const int16_t c2b(const Color);
  static const Color b2c(Color rgb, const int16_t);
  static const Color daytimeColor();
  const String getModeName();
  const Color getColor();
  virtual String moduleName();
};

#endif /* RGBCOMPONENT_H_ */
