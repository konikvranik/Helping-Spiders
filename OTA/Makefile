ARDUINO_LIBRARIES := \
	ArduinoJson/5.9.0 \
	ESP8266MQTTClient/1.0.5 \
	ArduinoLog/1.0.1 \

ARDUINO_CORE := \
	variants/generic \
	libraries/ESP8266WiFi/src \
	libraries/ArduinoOTA \
	libraries/ESP8266HTTPUpdateServer/src \
	libraries/ESP8266WebServer/src \
	libraries/ESP8266mDNS \

NAME := OTAComponent

DEFAULT: $(NAME).cpp.o

clean:
	-$(RM) $(NAME).cpp.o

BUILDDIR := .
include ../shared.mk
