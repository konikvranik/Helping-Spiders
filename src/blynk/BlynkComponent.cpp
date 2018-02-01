/*
 * BlynkComponent.cpp
 *
 *  Created on: 4. 7. 2017
 *      Author: hpa
 */

#include "BlynkComponent.h"

BlynkComponent::BlynkComponent(String token, String ssid, String pwd,
		String server, uint16_t port) :
		AbstractComponent(-1), token(token), ssid(ssid), pwd(pwd), server(
				server), port(port) {

}

BlynkComponent::~BlynkComponent() {
	// TODO Auto-generated destructor stub
}

void BlynkComponent::setup() {
	Log.notice("Starting blynk"CR);

	if (WiFi.status() != WL_CONNECTED) {
		Blynk.begin(token.c_str(), ssid.c_str(), pwd.c_str(), server.c_str(),
				port);
	} else {
		Blynk.config(token.c_str(), server.c_str());
	}
	Log.notice("Connecting blynk ");

}

void BlynkComponent::loop() {
	if (Blynk.connect() == false) {
		Log.notice("Waiting for Blynk...");
	} else {
		Blynk.run();
	}
}

void BlynkComponent::presentation() {

}

void BlynkComponent::receive(const MyMessage &message) {

}

void BlynkComponent::reportStatus(const JsonObject&) {
}

String BlynkComponent::moduleName() {
	return "blynk";
}
