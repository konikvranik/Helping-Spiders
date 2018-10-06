/*
 * iLifeComponent.cpp
 *
 *  Created on: 27. 6. 2017
 *      Author: hpa
 */

#include "iLifeComponent.h"

iLifeComponent::iLifeComponent(const uint8_t sensor_id, const uint16_t tx_pin,
bool inverted_tx) :
		irsend(tx_pin, inverted_tx), AbstractComponent(sensor_id) {
	pinMode(tx_pin, OUTPUT);
	digitalWrite(tx_pin, inverted_tx ? HIGH : LOW);
}

iLifeComponent::~iLifeComponent() {

}

void iLifeComponent::presentation(MQTTClient* mqtt) {
	AbstractComponent::presentation(mqtt);
}

static code* singleCode(int c) {
	static code sq[] = { { 0, c } };
	return sq;
}

void iLifeComponent::receive(String topic, String data, bool cont) {
	/*
	String topicPrefix = makeTopic("");
	if (pos < 0) {
		switch (message.type) {
		case V_STATUS:
			if (message.getInt() == status) {
				status = message.getInt();
				send(status_msg.set(status));
			} else {
				status = message.getInt();
				pos = 0;
				switch (status) {
				case 1:
					len = 5;
					status = 1;
					seq = clean_seq;
					ir_code = String("turn_on");
					break;
				case 0:
					len = 2;
					status = 0;
					seq = home_seq;
					ir_code = String("turn_off");
					break;
				default:
					pos = -1;
					break;
				}
			}
			break;
		case V_IR_SEND:
			ir_code = message.getString();
			pos = 0;
			len = 1;
			if (ir_code.equals("start_pause")) {
				status = status == 0 ? 1 : 0;
				seq = singleCode(ILF_CLEAN);
			} else if (ir_code.equals("turn_on")) {
				len = 5;
				status = 1;
				seq = clean_seq;
			} else if (ir_code.equals("turn_off")) {
				len = 2;
				status = 0;
				seq = home_seq;
			} else if (ir_code.equals("return_to_base")) {
				status = 0;
				seq = singleCode(ILF_HOME);
			} else if (ir_code.equals("clean_spot")) {
				status = 1;
				seq = singleCode(ILF_SPOT);
			} else if (ir_code.equals("corner")) {
				status = 1;
				seq = singleCode(ILF_CORNER);
			} else if (ir_code.equals("down")) {
				status = 1;
				seq = singleCode(ILF_DOWN);
			} else if (ir_code.equals("up")) {
				status = 1;
				seq = singleCode(ILF_UP);
			} else if (ir_code.equals("left")) {
				status = 1;
				seq = singleCode(ILF_LEFT);
			} else if (ir_code.equals("right")) {
				status = 1;
				seq = singleCode(ILF_RIGHT);
			} else if (ir_code.equals("clock")) {
				seq = singleCode(ILF_CLOCK);
			} else {
				pos = -1;
			}
			break;
		}
	}
	*/
}

void iLifeComponent::setup() {
}

void iLifeComponent::loop() {
	if (pos >= 0 && last_sent + seq[pos].d < millis()) {
		irsend.begin();
		irsend.sendNEC(seq[pos].c, 32);
		last_sent = millis();
		pos++;
	}
	if (pos >= len) {
		mqtt->publish(makeTopic(""), String(""));
		pos = -1;
	}
}

String iLifeComponent::seq2s() {
	String st = "";
	for (uint8_t i = 0; i < len; i++) {
		st += String(seq[i].d) + "->" + String(seq[i].c) + ", ";
	}
	return st;
}

void iLifeComponent::reportStatus(JsonObject& jo) {
	jo["ID"] = this->sensor_id;
	jo["State"] =
			this->status == 1 ? "ON" : (this->status == 0 ? "OFF" : "UNKNOWN");
	jo["Command"] = ir_code;
	jo["Sequence"] = this->seq2s();
	jo["Len"] = this->len;
	jo["Pos"] = this->pos;
}

String iLifeComponent::moduleName() {
	return "iLife";
}
