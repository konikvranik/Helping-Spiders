#ifndef CONFIG_h
#define CONFIG_h

#define SN "multisensor " NODE_ID
#define SV "1.0"
#define MY_GATEWAY_MAX_CLIENTS 3
#define MY_ESP8266_SSID WIFI_SSID
#define MY_ESP8266_PASSWORD WIFI_PASSWORD
#define BLYNK_ESP8266_SSID SSID
#define BLYNK_ESP8266_PASSWORD WIFI_PASSWORD

#ifdef DEVELOPMENT
#define MY_DEBUG
#define NODE_ID "esp12f"
#undef DISABLE_LOGGING
#else
#define DISABLE_LOGGING
#undef MY_DEBUG
#endif

// Controller ip address. Enables client mode (default is "server" mode).
// Also enable this if MY_USE_UDP is used and you want sensor data sent
// somewhere.
//#define MY_CONTROLLER_IP_ADDRESS 192, 168, 1, 2
#define MY_CONTROLLER_URL_ADDRESS "mqtt.home"
// The MQTT broker port to to open
#define MY_PORT 1883

#define MAX_SENSOR_ID 6

// How many clients should be able to connect to this gateway (default 1)
//#define MY_GATEWAY_MAX_CLIENTS 2

// Use a bit lower baudrate for serial prints on ESP8266 than default in
// MyConfig.h
#define MY_BAUD_RATE 115200

#define MY_NODE_ID 0

#define MY_GATEWAY_ESP8266
#define MY_GATEWAY_MQTT_CLIENT
#define MY_MQTT_CLIENT_ID "mysensors-" NODE_ID
#define MY_MQTT_PUBLISH_TOPIC_PREFIX MY_MQTT_CLIENT_ID "-out"
#define MY_MQTT_SUBSCRIBE_TOPIC_PREFIX MY_MQTT_CLIENT_ID "-in"

#endif
