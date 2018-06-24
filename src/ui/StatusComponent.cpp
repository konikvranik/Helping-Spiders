/*
 * StatusComponent.cpp
 *
 *  Created on: 27. 6. 2017
 *      Author: hpa
 */

#include "StatusComponent.h"

StatusComponent::StatusComponent(String nodeid, AbstractComponent *components[],
                                 uint8_t ccount, Print *logOutput)
    : AbstractComponent(-1) {
  this->logOutput = logOutput;
  init(nodeid, components, ccount);
}

void StatusComponent::init(String nodeid, AbstractComponent *components[],
                           uint8_t ccount) {
  nodeId = nodeid;
  this->components = components;
  this->components_count = ccount;
}

StatusComponent::StatusComponent(String nodeid, AbstractComponent *components[],
                                 uint8_t ccount, ESP8266WebServer *webServer)
    : AbstractComponent(-1) {
  this->webServer = webServer;
  init(nodeid, components, ccount);
}

StatusComponent::~StatusComponent() {
  // TODO Auto-generated destructor stub
}

void StatusComponent::setup() {
  Log.trace(("Status initializing with " + String(components_count) +
             " components" CR)
                .c_str());
  if (webServer != NULL) {
    webServer->on("/status", HTTP_GET, [&]() {
      webServer->sendHeader("Refresh", "20; url=/status");
      webServer->send(
          200, "text/plain; charset=utf-8",
          "Node ID:   " + nodeId +
              "\n"
              "Out Queue: " MY_MQTT_PUBLISH_TOPIC_PREFIX "\n"
              "In Queue:  " MY_MQTT_SUBSCRIBE_TOPIC_PREFIX "\n"
              "Time:      " +
              NTP.getTimeDateString() +
              (NTP.isSummerTime() ? " Summer" : " Winter") + " Time.\n"
                                                             "Uptime:    " +
              NTP.getUptimeString() + " since " +
              NTP.getTimeDateString(NTP.getFirstSync()).c_str() +
              "\n"
              "WiFi is    " +
              (WiFi.isConnected() ? "connected" : "not connected") +
              ".\n"
              "------------------------------------\n\n" +
              String(this->components_count) + "" +
              " modules: " + this->moduleNames() + "\n"
                                                   "~~~~~~~~~~\n" +
              this->ModulesStatus() + "\nChip ID:               " +
              String(ESP.getChipId()) + "\n"
                                        "Flash ID:              " +
              String(ESP.getFlashChipId()) + "\n"
                                             "Core version:          " +
              ESP.getCoreVersion() + "\n"
                                     "Flash real size:       " +
              String(ESP.getFlashChipRealSize()) + "\n"
                                                   "Flash size by chip ID: " +
              String(ESP.getFlashChipSizeByChipId()) +
              "\n"
              "Flash size:            " +
              String(ESP.getFlashChipSize()) + "\n"
                                               "Boot mode:             " +
              String(ESP.getBootMode()) + "\n"
                                          "Boot version:          " +
              String(ESP.getBootVersion()) + "\n"
                                             "Flash mode:            " +
              String(ESP.getFlashChipMode()) + "\n"
                                               "SDK version:           " +
              String(ESP.getSdkVersion()) + "\n"
                                            "VCC:                   " +
              String(ESP.getVcc()) + " V\n"
                                     "Loops:                 " +
              (lps < 0 ? "UNKNOWN" : String(lps)) +
              "/s\n"
              "------------------------------------\n"
#ifdef DEVELOPMENT
              "+++ DEVELOPMENT MODE +++\n"
#endif
          );
    });
    webServer->on("/status.json", HTTP_GET, [&]() {
      webServer->send(200, "application/json; charset=utf-8",
                      this->ModulesJson());
    });
  }
}

String StatusComponent::ModulesStatus() {
  String result = "";
  for (int i = 0; i < this->components_count; i++) {
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject &jo = jsonBuffer.createObject();
    this->components[i]->reportStatus(jo);
    if (jo.size() > 0) {
      result += "\n" + components[i]->moduleName() + ":\n";
      for (auto it = jo.begin(); it != jo.end(); ++it) {
        String str;
        it->value.prettyPrintTo(str);
        result += String(it->key) + String(": ") + str + String("\n");
      }

      result += "\n------------------------------------\n";
    }
  }
  return result;
}

String StatusComponent::ModulesJson() {
  StaticJsonBuffer<2000> jsonBuffer;
  JsonObject &p = jsonBuffer.createObject();
  for (int i = 0; i < this->components_count; i++) {
    JsonObject &jo = p.createNestedObject(components[i]->moduleName());
    this->components[i]->reportStatus(jo);
  }
  String s;
  p.prettyPrintTo(s);
  return s;
}

void StatusComponent::loop() {
  loop_count++;
  uint32_t now = millis();
  if (last_log + LOG_PERIOD < now) {
    lps = loop_count * 1000 / (now - last_log);
    Log.notice("Loops/s: %d" CR, lps);
    Log.trace("Time:   %s %s Time." CR, NTP.getTimeDateString().c_str(),
              (NTP.isSummerTime() ? " Summer" : " Winter"));
    loop_count = 0;
    last_log = now;
  }
}

void StatusComponent::presentation() {}

void StatusComponent::receive(const MyMessage &message) {}

void StatusComponent::reportStatus(JsonObject &) {}

String StatusComponent::moduleName() { return "status"; }

String StatusComponent::moduleNames() {
  String result = "";
  for (int i = 0; i < this->components_count; i++) {
    result += ((i > 0 ? ", " : "") + this->components[i]->moduleName());
  }
  return result;
}
