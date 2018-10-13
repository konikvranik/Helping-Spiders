#include "core.h"

ADC_MODE(ADC_VCC);

int module_count =
	sizeof(modules) > 0 ? sizeof(modules) / sizeof(modules[0]) : 0;

StatusComponent status_component(str(NODE_ID), modules, module_count, &http_server);

void onSTAGotIP(WiFiEventStationModeGotIP ipInfo)
{
	Log.notice("Got IP: %s" CR, ipInfo.ip.toString().c_str());
	NTP.begin("pool.ntp.org", 1, true);
	NTP.setInterval(63);
	http_server.begin();
}

void onSTADisconnected(WiFiEventStationModeDisconnected event_info)
{
	Log.warning("Disconnected from SSID: %s" CR, event_info.ssid.c_str());
	Log.warning("Reason: %d" CR, event_info.reason);
	http_server.stop();
	digitalWrite(2, HIGH);
	digitalWrite(0, HIGH);
	digitalWrite(15, LOW);
	ESP.reset();
}

void setupNTP()
{
	Log.notice("Initializing NTP...\r\n");
	NTP.begin("pool.ntp.org", 1, true);
	NTP.setInterval(63);
	NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
		if (ntpEvent)
		{
			Log.error("Time Sync error: ");
			if (ntpEvent == noResponse)
				Log.error("NTP server not reachable\r\n");
			else if (ntpEvent == invalidAddress)
				Log.error("Invalid NTP server address\r\n");
		}
		else
		{
			Log.notice("Got NTP time: %s\r\n",
					   NTP.getTimeDateString(NTP.getLastNTPSync()).c_str());
		}
	});
}

// ======================== SETUP ============================
void setup()
{
#ifndef DISABLE_LOGGING
	DEBUG_ESP_PORT.begin(115200);
	Log.begin(LOG_LEVEL_TRACE, &DEBUG_ESP_PORT);
#else
	DEBUG_ESP_PORT.end();
#endif
	pinMode(0, PINMODE0);
	pinMode(2, PINMODE2);
	pinMode(15, PINMODE15);
#ifdef INIT_STUFF
	INIT_STUFF
#endif

	Log.trace("SSID: ");
	Log.trace(str(WIFI_SSID));
	WiFi.begin(str(WIFI_SSID), str(WIFI_PASSWORD));
	WiFi.onStationModeGotIP(onSTAGotIP); // As soon WiFi is connected, start NTP Client
	WiFi.onStationModeDisconnected(onSTADisconnected);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(100);
		yield;
	}
	Log.notice(
		(CR "starting with " + String(module_count) + " modules..." CR).c_str());

	for (int i = 0; i < module_count; i++)
	{
		Log.trace("Initializing module ");
		Log.trace((modules[i]->moduleName() + CR).c_str());
		modules[i]->setup();
		Log.notice(
			("Module " + modules[i]->moduleName() + " initalized." CR).c_str());
	}
	Log.trace("Initializing module status" CR);
	status_component.setup();
	Log.notice("Module status initalized." CR);

	setupNTP();
	Log.notice("NTP initialized." CR);

	Log.notice("Setup done." CR);
}

// ====================== MAIN LOOP ===============

void loop()
{
	// Send locally attach sensors data here
	Log.verbose("loop start" CR);
	for (int i = 0; i < module_count; i++)
	{
		Log.verbose(
			("Processing module " + modules[i]->moduleName() + CR).c_str());
		modules[i]->loop();
		Log.verbose(
			("Module " + modules[i]->moduleName() + " processed." CR).c_str());
		yield();
	}
	Log.verbose("Processing module status" CR);
	status_component.loop();
	Log.verbose("Module status processed." CR);

	Log.verbose("loop end" CR);
}
