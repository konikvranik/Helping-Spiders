/*
 * iLifeComponent.h
 *
 *  Created on: 27. 6. 2017
 *      Author: hpa
 */

#ifndef ILIFECOMPONENT_H_
#define ILIFECOMPONENT_H_

#include "IRComponent.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>

#define ILF_CLEAN 0x2AA22DD
#define ILF_HOME 0x2AA8877
#define ILF_SPOT 0x2AA7788
#define ILF_CORNER 0x2AA9966
#define ILF_DOWN 0x2AA6699
#define ILF_UP 0x2AA55AA
#define ILF_RIGHT 0x2AA44BB
#define ILF_LEFT 0x2AA33CC
#define ILF_CLOCK 0x25557a8

struct code {
	uint16_t d = 0;
	uint32_t c = 0;
	code(uint16_t d, uint32_t c) :
			d(d), c(c) {
	}
};

static code clean_seq[] = { { 0, ILF_CLEAN }, { 1100, ILF_CLEAN }, { 1100,
ILF_SPOT }, { 1100, ILF_CLEAN }, { 1100, ILF_CLEAN } };
static code home_seq[] = { { 0, ILF_CLEAN }, { 1100, ILF_HOME } };

class iLifeComponent: public AbstractComponent {
	IRsend irsend;

public:
	uint32_t last_sent = millis();
	code* seq = clean_seq;
	int8_t status = -1, len = 0, pos = -1;
	String ir_code = String("~~~");
	iLifeComponent(const uint8_t sensor_id, const uint16_t tx_pin,
	bool inverted_tx = false);
	virtual ~iLifeComponent();
	virtual void setup();
	virtual void loop();
	virtual void receive(String topic, String data, bool cont);
	virtual void presentation(MQTTClient mqtt);
	virtual void reportStatus(JsonObject&);
	virtual String moduleName();
	String seq2s();
};

#endif /* ILIFECOMPONENT_H_ */
