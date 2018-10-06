PLATFORM_PATH=packages/esp8266/hardware/esp8266/$(PLATFORM_VERSION)/

SHELL=/bin/bash

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

LIBS := $(foreach d,$(addprefix $(ARDUINO_CDT)/libraries/,$(ARDUINO_LIBRARIES)),$(if $(wildcard $d/src),$d/src,$d))
SOURCES := $(addprefix $(ARDUINO_CDT)/$(PLATFORM_PATH),$(ARDUINO_CORE)) \
	$(LIBS) \
	$(LIBRARIES) \
	$(CORE_PATH)/src \

CYGPATH=cygpath -m
SRC = $(shell find $(SOURCES) \( -name '*.c' -o -name '*.cpp' -o -name '*.S' \))

INC = $(foreach d,$(SOURCES), -I'$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $d),$d)')

OBJECTS = $(addprefix $(BUILDDIR)/,$(foreach E,$(SRC:%=%.o),$(if $(findstring MySensors,$E),,$E)))

TOOLS_PATH=${ARDUINO_CDT}/packages/esp8266/tools
XTENSA_PATH = ${TOOLS_PATH}/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin
CC = "$(XTENSA_PATH)/xtensa-lx106-elf-gcc"
CXX = "$(XTENSA_PATH)/xtensa-lx106-elf-g++"
ELF_SIZE = "$(XTENSA_PATH)/xtensa-lx106-elf-size"
AR = "$(XTENSA_PATH)/xtensa-lx106-elf-ar"
ESP_TOOL = "${TOOLS_PATH}/esptool/0.4.9/esptool"

SYS_INC=${ARDUINO_CDT}/${PLATFORM_PATH}/tools/sdk/include ${ARDUINO_CDT}/${PLATFORM_PATH}/tools/sdk/lwip/include ${ARDUINO_CDT}/${PLATFORM_PATH}/cores/esp8266/

FLAGSPREFIX=-D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ $(foreach d, $(SYS_INC), -I'$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $d),$d)') -c
FLAGSSUFFIX=-DF_CPU=80000000L -DLWIP_OPEN_SRC -DARDUINO=10608 -DARDUINO_$(BOARD) -DARDUINO_ARCH_$(PLATFORM) -DARDUINO_BOARD=\"$(BOARD)\" -D$(PLATFORM) -DWIFI_SSID=\"$(WIFI_SSID)\" -DWIFI_PASSWORD=\"$(WIFI_PASSWORD)\" -DNODE_ID=\"$(NODE_ID)\" $(addprefix -DENABLE_,$(MODULES)) -DINIT_STUFF="$(INIT_STUFF)" $(addprefix -D,$(FLAGS)) -DSV="$(SV)" -DSN="$(SN)"
ASMFLAGS=$(FLAGSPREFIX) -g -x assembler-with-cpp -MMD -mlongcalls $(FLAGSSUFFIX)
CFLAGS =$(FLAGSPREFIX) -w -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -MMD -std=gnu99 -ffunction-sections -fdata-sections $(FLAGSSUFFIX)
CXXFLAGS=$(FLAGSPREFIX) -w -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections -fdata-sections $(FLAGSSUFFIX)
LDPATH=$(foreach d,"${ARDUINO_CDT}/${PLATFORM_PATH}/tools/sdk/lib" "${ARDUINO_CDT}/${PLATFORM_PATH}/tools/sdk/ld","-L$(if $(findstring cygwin, $(SYS)),$(shell $(CYGPATH) $d),$d)")
LDFLAGS=-w -Os -g -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static $(LDPATH) "-Teagle.flash.${FLASH_MEMORY}.ld" -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy

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
