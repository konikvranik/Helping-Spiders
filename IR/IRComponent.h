/*
 * IRComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef IRCOMPONENT_H_
#define IRCOMPONENT_H_

#include "../AbstractComponent.h"
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define MY_RAWBUF 50

#define Type2String(x) TYPE2STRING[x < 0 ? 0 : x]
#define AddrTxt F(" addres: 0x")
#define ValueTxt F(" value: 0x")
#define NATxt F(" - not implemented/found")

class IRComponent : public AbstractComponent {

  String code = "init";
  String oldCode = "init";
  uint32_t receivedCode;
  uint32_t sentCode;
  IRrecv irrecv;
  IRsend irsend;

  static constexpr const char *TYPE2STRING[] = {
      "UNKONWN",    "RC5",     "RC6",     "NEC",          "Sony", "Panasonic",
      "JVC",        "SAMSUNG", "Whynter", "AIWA RC T501", "LG",   "Sanyo",
      "Mitsubishi", "Dish",    "Sharp",   "Denon"};

public:
  IRComponent(const uint8_t sensor_id, const uint16_t rx_pin,
              const uint16_t tx_pin, bool inverted_tx = false);
  virtual ~IRComponent();
  virtual void setup();
  virtual void loop();
  virtual void receive(String topic, String data, bool cont);
  virtual void presentation(MQTTClient* mqtt);
  virtual void reportStatus(JsonObject &);
  virtual String moduleName();
};

#endif /* IRCOMPONENT_H_ */
