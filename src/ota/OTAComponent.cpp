/*
 * OTAComponent.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "OTAComponent.h"

#include <ArduinoLog.h>
#include <WString.h>
#include <unordered_map>

OTAComponent::OTAComponent(const String node_id, ESP8266WebServer *server) :
		httpUpdater(true), AbstractComponent(-1) {
	_server = server;
	nodeId = node_id;
}

OTAComponent::~OTAComponent() {
	// TODO Auto-generated destructor stub
}

void OTAComponent::loop() {
	_server->handleClient();
	ArduinoOTA.handle();
}

void OTAComponent::setup() {
	// Port defaults to 8266
	//ArduinoOTA.setPort(8266);
	String hostname("ESP3-" + sensor_id);

	Log.notice("OTA Begin"CR);
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Log.notice("OTA: waiting for connection"CR);
		delay(300);
	}
	Log.notice("IP address: %s"CR, String(WiFi.localIP()).c_str());

	MDNS.begin(hostname.c_str());
	httpUpdater.setup(_server);
	_server->begin();
	MDNS.addService("http", "tcp", 80);

	ArduinoOTA.begin();

	Log.verbose("Open http://%s.local/update in your browser"CR,
			hostname.c_str());
}

void OTAComponent::presentation() {

}

void OTAComponent::receive(const MyMessage&) {

}

void OTAComponent::reportStatus(JsonObject&) {
}

String OTAComponent::moduleName() {
	return "OTA";
}
