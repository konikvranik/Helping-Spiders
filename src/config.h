#ifndef CONFIG_h
#define CONFIG_h

#define MY_DEBUG
#define IR_CHILD_ID 2
#define TEMP_CHILD_ID 3
#define HUM_CHILD_ID 4
#define DHT_TYPE DHT21
#define RGB_CHILD_ID 5
#define ILIFE_CHILD_ID 7
#define SN "multisensor " NODE_ID
#define SV "1.0"
#define MY_GATEWAY_MAX_CLIENTS 3
#define MY_ESP8266_SSID SSID
#define MY_ESP8266_PASSWORD WIFI_PASSWORD
#define BLYNK_ESP8266_SSID SSID
#define BLYNK_ESP8266_PASSWORD WIFI_PASSWORD

#ifdef DEVELOPMENT
#define NODE_ID "esp12f"
#define RED_PIN 15
#define GREEN_PIN 12
#define BLUE_PIN 13
#define DHT_PIN 3
#define IRRX_PIN 5
#define IRTX_PIN 5
#else
#define RED_PIN 12
#define GREEN_PIN 14
#define BLUE_PIN 13
#define DHT_PIN 1
#define IRRX_PIN 3
#define IRTX_PIN 4
#define DISABLE_LOGGING
#undef MY_DEBUG
#endif

#endif
