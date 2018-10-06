ARDUINO_LIBRARIES := \
	ArduinoJson/5.9.0 \
	ESP8266MQTTClient/1.0.5 \
	Ultrasonic_hc_sr04/0.4.0 \

ARDUINO_CORE := \
	variants/generic \
	libraries/ESP8266WiFi/src \

NAME := HC-SR04

DEFAULT: $(NAME).cpp.o

clean:
	-$(RM) $(NAME).cpp.o

BUILDDIR := .
include ../shared.mk
