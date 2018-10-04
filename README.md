# Helping Spiders

Platform for building customized arduino sensors/acutators to use with [Home Assistant](http://home-assistant.io).

You can select existing modules and customize those. Combination of several modules is allowed. E.g.: you can make module controlling LED strip, listening to IR signals and controlling iLife vacuum cleaner all together.

Actually supported are:

* IR sender/receiver
* iLife vacuume cleaner IR control.
* LED strip
* OTA
* status reporting
* DHT and HTU temperature and humidity sensors

## Prerequisities

Arduino SDK has to be installed to `~/.arduinocdt` directory. You can use `sdk/download.sh` tool to download platform and libraries.
Use `sdk/download.sh -p esp8266 2.3.0` to download version 2.3.0 of esp8266 platform.

Libraries slecified in `core.mk` in `ARDUINO_LIBRARIES` variable has to be installed to `~/.arduinocdt` direcotry.
Use `sdk/download.sh NAME VERSION` to download tho.

## Usage

Make directory for your module build and config. Copy there `Makefile.temlplate` and rename it to `Makefile`.
Edit values in this `Makefile`. Make sure that `PATH_TO_PROJECT_DIR/helpingspiders` is correctly replaced by path to helpingspiders project directory.
Remove directives you don't want to use, select wanted modules by using directives `ENABLE_<MODULE>` and `make`.

To upload ocer serial port, use `make upload`. To upload over the air, use `make netupload`. `MODULE_IP` has to be set in `Makefile` for this purpose. Could be IP or hostname.
