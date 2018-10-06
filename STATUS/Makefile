ARDUINO_LIBRARIES := \
	ArduinoJson/5.9.0 \
	ESP8266MQTTClient/1.0.5 \
	ArduinoLog/1.0.1 \
	Time/1.5.0 \
	NtpClientLib/2.0.5 \

ARDUINO_CORE := \
	variants/generic \
	libraries/ESP8266WiFi/src \
	libraries/ESP8266WebServer/src \

NAME := StatusComponent

DEFAULT: $(NAME).cpp.o

clean:
	-$(RM) $(NAME).cpp.o

BUILDDIR := .
include ../shared.mk
