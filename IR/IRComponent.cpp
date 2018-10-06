/*
 * IRComponent.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "IRComponent.h"

IRComponent::IRComponent(const uint8_t sensor_id, const uint16_t rx_pin,
		const uint16_t tx_pin, bool inverted_tx) :
		irrecv(rx_pin), irsend(tx_pin, inverted_tx), AbstractComponent(
				sensor_id) {
	pinMode(tx_pin, OUTPUT);
	pinMode(rx_pin, INPUT);
	digitalWrite(tx_pin, inverted_tx ? HIGH : LOW);
}

IRComponent::~IRComponent() {
	// TODO Auto-generated destructor stub
}

void IRComponent::presentation(MQTTClient* mqtt) {
	AbstractComponent::presentation(mqtt);
}

void IRComponent::setup() {
	Log.notice("Recall EEPROM settings"CR);
	// Start the ir receiver
	irrecv.enableIRIn();
	Log.notice("Init done..."CR);
}

void IRComponent::receive(String topic, String data, bool cont) {
	/*
	if (message.sensor != sensor_id)
		return;
	switch (message.type) {
	case V_IR_SEND:
		// Retrieve the IR code value from the incoming message.
		code = message.getString();
		Log.notice("Changing code to %s"CR, code.c_str());
		if (message.type == V_IR_SEND) {
			// Send an IR code from offset: paramvalue - no check for legal value
			Log.notice("Send IR preset: %d"CR, message.getULong());
			irsend.begin();
			this->sentCode = message.getULong();
			irsend.sendNEC(this->sentCode, 32);
			send(msgIrReceive.set(this->sentCode));
		}

		// Start receiving ir again...
		irrecv.enableIRIn();
		break;
	}
	*/
}

void IRComponent::loop() {
	decode_results ircode;
	if (irrecv.decode(&ircode)) {
		decode_type_t decodeType = (decode_type_t) ircode.decode_type;
		this->receivedCode = ircode.value;
		irrecv.resume();
		mqtt->publish(makeTopic("out"),String(this->receivedCode));
	}
}

void IRComponent::reportStatus(JsonObject& jo) {
	jo["ID"] = this->sensor_id;
	jo["Received"] = this->receivedCode;
	jo["Send"] = this->sentCode;
}

String IRComponent::moduleName() {
	return "IR";
}
