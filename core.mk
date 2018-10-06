
ARDUINO_CDT = ${HOME}/.arduino15

#BAUDRATE = 230400
PLATFORM_VERSION=2.3.0

LIBRARIES =	\
	${CORE_PATH}/RGBLEDBlender/src \

ARDUINO_LIBRARIES = \
	ArduinoLog/1.0.1 \
	NtpClientLib/2.0.5 \
	Time/1.5.0 \
	WiFiMan/1.0.7 \
	ArduinoJson/5.9.0 \
	ESP8266OTA/1.0.1 \
	ESP8266MQTTClient/1.0.5 \
	OneWire/2.3.4 \
	DallasTemperature/3.8.0 \
	Ultrasonic_hc_sr04/0.4.0 \
#	IRremoteESP8266/2.0.3 \
#	DHT_sensor_library/1.3.0 \
#	esp8266_mdns/1.1.6 \
#	Adafruit_Unified_Sensor/1.0.2 \
#	SparkFun_HTU21D_Humidity_and_Temperature_Sensor_Breakout/1.1.3 \
#	Adafruit_HTU21DF_Library/1.0.1 \
#	Sodaq_SHT2x/1.2.0 \
#	I2C-Sensor-Lib_iLib/0.8.2 \

ARDUINO_CORE = \
	variants/generic \
	libraries/ESP8266WiFi/src \
	libraries/ESP8266WebServer/src \
	libraries/Hash/src \
	libraries/SPI \
	libraries/ArduinoOTA \
	libraries/ESP8266HTTPUpdateServer/src \
	libraries/DNSServer/src \
	libraries/ESP8266mDNS \
	cores/esp8266 \

#	libraries/ESP8266HTTPClient/src \
#	libraries/EEPROM \
#	libraries/Ethernet/src \
#	libraries/esp8266/src \
#	libraries/ESP8266httpUpdate/src \
#	libraries/Wire \

#########################################################

BIN := $(BUILDDIR)/image.bin

DEFAULT: $(BIN)

all: $(BIN)

$(BUILDDIR)/src/core.cpp.o: modules */*.o

include $(CORE_PATH)/shared.mk

$(BUILDDIR)/arduino.ar:	$(foreach E,$(OBJECTS),$(if $(findstring /cores/esp8266/,$E),$E))
	$(AR) cru  "$(BUILDDIR)/arduino.ar" $^

.PRECIOUS: $(OBJECTS) %.elf

clean:
	-$(RMDIR) $(BUILDDIR)/src $(BUILDDIR)/image.* $(BUILDDIR)/arduino.ar $(addprefix $(BUILDDIR)/,$(MODULES))

distclean:
	-$(RMDIR) $(BUILDDIR) 

size: $(BUILDDIR)/image.elf
	${ELF_SIZE} -A "$<"
.PHONY: size

flash: $(MODULE)
	${ESP_TOOL} -vv -cd ck -cb ${BAUDRATE} -cp ${COM_PORT} -ca 0x00000 -cf "$(MODULE)"
	rm $(MODULE)
.PHONY: flash

upload: $(MODULE)
	@date
	time curl -X POST --progress-bar -F "update=@$(MODULE)" http://$(MODULE_IP)/update >&2
	@date
.PHONY: upload

modules:
	$(foreach dir,$(MODULES),$(MAKE) -C $(dir);)
.PHONY: modules

modulesclean:
	$(foreach dir,$(MODULES),$(MAKE) -C $(dir) clean;)
	$(MAKE) clean
.PHONY: modulesclean

%.bin: %.elf
	${ESP_TOOL} -v -eo "$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) ${ARDUINO_CDT}/${PLATFORM_PATH}/bootloaders/eboot/eboot.elf),${ARDUINO_CDT}/${PLATFORM_PATH}/bootloaders/eboot/eboot.elf)" -bo "$@" -bm qio -bf 40 -bz ${FLASH_SIZE} -bs .text -bp 4096 -ec -eo "$<" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec

export
%.elf: $(foreach E,$(OBJECTS),$(if $(findstring /cores/esp8266/,$E),,$E)) $(BUILDDIR)/arduino.ar
	$(MAKE) modules
	$(CC) $(LDFLAGS) -o "$@" -Wl,--start-group $^ -lm -lgcc -lhal -lphy -lpp -lnet80211 -lwpa -lcrypto -lmain -lwps -laxtls -lsmartconfig -lmesh -lwpa2 -llwip_gcc -lstdc++ -Wl,--end-group  "-L."
