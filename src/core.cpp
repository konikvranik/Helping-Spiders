#include "core.h"

#include <HardwareSerial.h>

ADC_MODE(ADC_VCC);

int module_count =
	sizeof(modules) > 0 ? sizeof(modules) / sizeof(modules[0]) : 0;

StatusComponent status_component(str(NODE_ID), modules, module_count, &http_server);
bool online = false;

void onSTAGotIP(WiFiEventStationModeGotIP ipInfo)
{
	online = true;
	NTP.begin("pool.ntp.org", 1, true);
	NTP.setInterval(63);
	http_server.begin();
}

void onSTADisconnected(WiFiEventStationModeDisconnected event_info)
{
	online = false;
	http_server.stop();
	digitalWrite(2, HIGH);
	digitalWrite(0, HIGH);
	digitalWrite(15, LOW);
	ESP.reset();
}

void setupNTP()
{
	NTP.begin("pool.ntp.org", 1, true);
	NTP.setInterval(63);
	NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
		if (ntpEvent)
		{
			if (ntpEvent == noResponse);
			else if (ntpEvent == invalidAddress);
		}
		else
		{
		}
	});
}

// ======================== SETUP ============================
void setup()
{
	Serial.begin(115200);

	pinMode(0, PINMODE0);
	pinMode(2, PINMODE2);
	pinMode(15, PINMODE15);
#ifdef INIT_STUFF
	INIT_STUFF
#endif

	WiFi.begin(str(WIFI_SSID), str(WIFI_PASSWORD));
	WiFi.onStationModeGotIP(onSTAGotIP); // As soon WiFi is connected, start NTP Client
	WiFi.onStationModeDisconnected(onSTADisconnected);
	while (WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		delay(100);
		yield;
	}
	while (WiFi.localIP() == IPAddress(0U))
	{
		delay(100);
		yield;
	}

	for (int i = 0; i < module_count; i++)
	{
		modules[i]->setup();
	}
	status_component.setup();

	setupNTP();
}

// ====================== MAIN LOOP ===============

void loop()
{
	// Send locally attach sensors data here
	for (int i = 0; i < module_count; i++)
	{
		modules[i]->loop();
		yield();
	}
	status_component.loop();
}
