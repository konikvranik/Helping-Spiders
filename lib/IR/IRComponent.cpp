/*
 * IRComponent.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "IRComponent.h"

IRComponent::IRComponent(const String node_id, const uint8_t sensor_id, const uint16_t rx_pin,
						 const uint16_t tx_pin, bool inverted_tx) : AbstractComponent(node_id, sensor_id), irrecv(rx_pin), irsend(tx_pin, inverted_tx)
{
	pinMode(tx_pin, OUTPUT);
	pinMode(rx_pin, INPUT_PULLUP);
	digitalWrite(tx_pin, inverted_tx ? HIGH : LOW);
}

IRComponent::~IRComponent()
{
	// TODO Auto-generated destructor stub
}

void IRComponent::setup()
{
	// Start the ir receiver
	irrecv.enableIRIn(false);
}

void IRComponent::receive(String topic, String data, bool cont)
{
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

void IRComponent::loop()
{
	decode_results ircode;
	if (irrecv.decode(&ircode))
	{
		this->receivedTime = millis();
		this->receivedCode = ircode.value;
		this->receivedCommand = ircode.command;
		this->receivedType = ircode.decode_type;
		this->receivedAddres = ircode.address;
		this->receivedRepeat = ircode.repeat;
		irrecv.resume();
	}
}

void IRComponent::reportStatus(JsonObject &jo)
{
	jo["ID"] = this->sensor_id;
	jo["Received"] = typeToString(this->receivedType) + ":" + String(this->receivedAddres, HEX) + ":" + String(this->receivedCommand, HEX) + ":" + uint64ToString(this->receivedCode, HEX) + ":" + this->receivedRepeat;
	jo["Send"] = uint64ToString(this->sentCode, HEX);
}

String IRComponent::moduleName()
{
	return "IR";
}
