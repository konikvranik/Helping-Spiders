/*
 * OTAComponent.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "OTAComponent.h"
//#include <WString.h>
//#include <unordered_map>

OTAComponent::OTAComponent(const String node_id, ESP8266WebServer *server) : AbstractComponent(node_id, -1)
{
	_server = server;
	nodeId = node_id;
	WiFiUDP udpClient;
}

OTAComponent::~OTAComponent()
{
	// TODO Auto-generated destructor stub
}

void OTAComponent::loop()
{
	_server->handleClient();
	ArduinoOTA.handle();
}

void OTAComponent::setup()
{
	// Port defaults to 8266
	// ArduinoOTA.setPort(8266);
	String hostname("ESP3-" + sensor_id);

	while (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		delay(300);
	}

	MDNS.begin(hostname.c_str());
	httpUpdater.setup(_server);
	_server->begin();
	MDNS.addService("http", "tcp", 80);

	ArduinoOTA.onStart([&]() {
		this->updating = true;
	});

	ArduinoOTA.onEnd([&]() {
		this->updating = false;
		delay(3000);
		ESP.restart();
	});

	ArduinoOTA.onError([&](ota_error_t error) {
		(void)error;
		this->updating = false;
	});

	ArduinoOTA.begin();
}

void OTAComponent::reportStatus(JsonObject &)
{
}

String OTAComponent::moduleName()
{
	return "OTA";
}

boolean OTAComponent::isUpdating()
{
	return this->updating;
}
