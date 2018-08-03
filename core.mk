
ARDUINO_CDT = ${HOME}/.arduino15

#BAUDRATE = 230400
PLATFORM_VERSION=2.3.0

LIBRARIES =	\
	${CORE_PATH}/RGBLEDBlender/src \

ARDUINO_LIBRARIES = \
	IRremoteESP8266/2.0.3 \
	DHT_sensor_library/1.3.0 \
	ArduinoLog/1.0.1 \
	MySensors/2.1.1 \
	esp8266_mdns/1.1.6 \
	Adafruit_Unified_Sensor/1.0.2 \
	Blynk/0.4.8 \
	NtpClientLib/2.0.5 \
	Time/1.5.0 \
	SparkFun_HTU21D_Humidity_and_Temperature_Sensor_Breakout/1.1.3 \
	Adafruit_HTU21DF_Library/1.0.1 \
	Sodaq_SHT2x/1.2.0 \
	ArduinoJson/5.9.0 \
	OneWire/2.3.4 \
	DallasTemperature/3.8.0 \
	Ultrasonic_hc_sr04/0.4.0 \
#	I2C-Sensor-Lib_iLib/0.8.2 \

ARDUINO_CORE = \
	libraries/ESP8266httpUpdate/src \
	libraries/Ethernet/src \
	libraries/ESP8266HTTPUpdateServer/src \
	libraries/DNSServer/src \
	libraries/ESP8266WiFi/src \
	libraries/esp8266/src \
	libraries/EEPROM \
	libraries/ESP8266mDNS \
	libraries/ESP8266HTTPClient/src \
	libraries/ESP8266WebServer/src \
	libraries/ArduinoOTA \
	libraries/ESP8266WiFi/src \
	libraries/SPI \
	libraries/Wire \
	cores/esp8266 \
	variants/generic \

PLATFORM_PATH=packages/esp8266/hardware/esp8266/$(PLATFORM_VERSION)/

#########################################################

SHELL=/bin/bash

BUILDDIR := build/target
BIN := $(BUILDDIR)/image.bin

-include $(HOME)/.arduino.inc

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	 # Do linux things
	RMDIR = rm -fr
	RM = rm -f
	mymkdir = mkdir -p $1
else
	ifneq (, $(findstring mingw, $(SYS)))
		 # Do mingw things
		RMDIR = rm -fr
		RM = rm -f
		mymkdir = mkdir -p $1
	else
		ifneq (, $(findstring cygwin, $(SYS)))
			 # Do cygwin things
			RMDIR = rm -fr
			RM = rm -f
			mymkdir = mkdir -p $1
		else
		 # Do	 things for others
			ifeq ($(OS),Windows_NTx)
				SHELL = $(ComSpec)
				RMDIR = rmdir /s /q
				RM = del /q
				mymkdir = if not exist "$1" mkdir "$1"
			else
				RMDIR = rm -fr
				RM = rm -f
				mymkdir = mkdir -p $1
			endif
		endif
	endif
endif

DEFAULT: $(BIN)

LIBS := $(foreach d,$(addprefix $(ARDUINO_CDT)/libraries/,$(ARDUINO_LIBRARIES)),$(if $(wildcard $d/src),$d/src,$d))
SOURCES := $(addprefix ${ARDUINO_CDT}/${PLATFORM_PATH},$(ARDUINO_CORE)) \
	$(LIBS) \
	$(LIBRARIES) \
	$(CORE_PATH)/src \

CYGPATH=cygpath -m
SRC := $(shell find $(SOURCES) \( -name '*.c' -o -name '*.cpp' -o -name '*.S' \) -not -name 'time.c' )

INC := $(foreach d,$(SOURCES), -I'$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $d),$d)')

OBJECTS := $(addprefix $(BUILDDIR)/,$(foreach E,$(SRC:%=%.o),$(if $(findstring MySensors,$E),,$E)))
#OBJECTS := $(addprefix $(BUILDDIR)/,$(SRC:%=%.o))

#$(info $(OBJECTS))

TOOLS_PATH=${ARDUINO_CDT}/packages/esp8266/tools
XTENSA_PATH = ${TOOLS_PATH}/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin
CC = "$(XTENSA_PATH)/xtensa-lx106-elf-gcc"
CXX = "$(XTENSA_PATH)/xtensa-lx106-elf-g++"
ELF_SIZE = "$(XTENSA_PATH)/xtensa-lx106-elf-size"
AR = "$(XTENSA_PATH)/xtensa-lx106-elf-ar"
ESP_TOOL = "${TOOLS_PATH}/esptool/0.4.9/esptool"

SYS_INC=${ARDUINO_CDT}/${PLATFORM_PATH}/tools/sdk/include ${ARDUINO_CDT}/${PLATFORM_PATH}/tools/sdk/lwip/include ${ARDUINO_CDT}/${PLATFORM_PATH}/cores/esp8266/

MODULE := build/target/image.bin

FLAGSPREFIX=-D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ $(foreach d, $(SYS_INC), -I'$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $d),$d)') -c
FLAGSSUFFIX=-DF_CPU=80000000L -DLWIP_OPEN_SRC -DARDUINO=10608 -DARDUINO_$(BOARD) -DARDUINO_ARCH_$(PLATFORM) -DARDUINO_BOARD=\"$(BOARD)\" -D$(PLATFORM) -DWIFI_SSID=\"$(WIFI_SSID)\" -DWIFI_PASSWORD=\"$(WIFI_PASSWORD)\" -DNODE_ID=\"$(NODE_ID)\" $(addprefix -DENABLE_,$(MODULES)) $(FLAGS) -include $(CORE_PATH)/src/config.h
ASMFLAGS=$(FLAGSPREFIX) -g -x assembler-with-cpp -MMD -mlongcalls $(FLAGSSUFFIX)
CFLAGS =$(FLAGSPREFIX) -w -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -MMD -std=gnu99 -ffunction-sections -fdata-sections $(FLAGSSUFFIX)
CXXFLAGS=$(FLAGSPREFIX) -w -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections -fdata-sections $(FLAGSSUFFIX)
LDPATH=$(foreach d,"${ARDUINO_CDT}/${PLATFORM_PATH}/tools/sdk/lib" "${ARDUINO_CDT}/${PLATFORM_PATH}/tools/sdk/ld","-L$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $d),$d)")
LDFLAGS=-w -Os -g -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static $(LDPATH) "-Teagle.flash.${FLASH_MEMORY}.ld" -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy

%.bin: %.elf
	${ESP_TOOL} -v -eo "$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) ${ARDUINO_CDT}/${PLATFORM_PATH}/bootloaders/eboot/eboot.elf),${ARDUINO_CDT}/${PLATFORM_PATH}/bootloaders/eboot/eboot.elf)" -bo "$@" -bm qio -bf 40 -bz ${FLASH_SIZE} -bs .text -bp 4096 -ec -eo "$<" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec

%.elf: $(foreach E,$(OBJECTS),$(if $(findstring /cores/esp8266/,$E),,$E)) $(BUILDDIR)/arduino.ar
	$(CC) $(LDFLAGS) -o "$@" -Wl,--start-group $^ -lm -lgcc -lhal -lphy -lpp -lnet80211 -lwpa -lcrypto -lmain -lwps -laxtls -lsmartconfig -lmesh -lwpa2 -llwip_gcc -lstdc++ -Wl,--end-group  "-L."

$(BUILDDIR)/arduino.ar:	$(foreach E,$(OBJECTS),$(if $(findstring /cores/esp8266/,$E),$E))
	$(AR) cru  "$(BUILDDIR)/arduino.ar" $^

$(BUILDDIR)/core.cpp.o : ../multisensor/core.cpp ;

.PRECIOUS: $(OBJECTS) %.elf

clean:
	-$(RMDIR) $(BUILDDIR)/../lib/RGBLEDBlender $(BUILDDIR)/../multisensor $(BUILDDIR) $(dir $(MODULE))

size: $(BUILDDIR)/image.elf
	${ELF_SIZE} -A "$<"
.PHONY: size

flash: $(MODULE)
	${ESP_TOOL} -vv -cd ck -cb ${BAUDRATE} -cp ${COM_PORT} -ca 0x00000 -cf "$(MODULE)"
	rm $(MODULE)
.PHONY: flash

upload: $(MODULE)
	curl -X POST --progress-bar -F "update=@$(MODULE)" http://$(MODULE_IP)/update
	rm $(MODULE)
.PHONY: upload

query_library.%:
	curl http://downloads.arduino.cc/libraries/library_index.json | fgrep '"url"' | fgrep $*

install_library.%:
	cmd=`curl http://downloads.arduino.cc/libraries/library_index.json | jq -r '.libraries[] | select( .name == "$*" ) | "dest=\"$(ARDUINO_CDT)/libraries/"+ (.name | gsub(" "; "_")) + "/"+ .version+ "\"; mkdir -p \"$$dest\"; zip=$$(mktemp); curl -o $$zip " + .url +"; unzip -d \"$$dest\" \"$$zip\" && f=(\"$$dest\"/*) && mv \"$$dest\"/*/* \"$$dest\" && rmdir \"$${f[@]}\";" '`;bash -c "$$cmd"

$(ARDUINO_CDT)/libraries/%:
	`eval "echo $* | sed 's/\([^\/]*\)\/\([^\/]*\)/export NAME=\1 VERSION=\2/'"`; cmd=`curl http://downloads.arduino.cc/libraries/library_index.json | jq -r '.libraries[] | select( .name == "'$$NAME'" and .version == "'$$VERSION'" ) | "dest=\"$(ARDUINO_CDT)/libraries/$*\"; mkdir -p \"$$dest\"; zip=$$(mktemp); curl -o $$zip " + .url +"; unzip -d \"$$dest\" \"$$zip\" && f=(\"$$dest\"/*) && mv \"$$dest\"/*/* \"$$dest\" && rmdir \"$${f[@]}\";" '`;echo $$cmd; bash -c "$$cmd"

#http://downloads.arduino.cc/packages/package_index.json
#http://arduino.esp8266.com/versions/2.3.0/package_esp8266com_index.json

$(BUILDDIR)/%.S.o : %.S
	@$(call mymkdir,$(dir $@))
	$(CC) $(ASMFLAGS) $(INC) '$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $<),$<)' -o "$@"

$(BUILDDIR)/%.c.o: %.c $(BUILDDIR)/%.c.d
	@$(call mymkdir,$(dir $@))
	$(CC) $(CFLAGS)  $(INC) '$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $<),$<)' -o "$@"

$(BUILDDIR)/%.cpp.o: %.cpp $(BUILDDIR)/%.cpp.d
	@$(call mymkdir,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INC) '$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $<),$<)' -o "$@"

%.d: ;

-include $(BUILDDIR)/%.cpp.d $(BUILDDIR)/%.c.d
