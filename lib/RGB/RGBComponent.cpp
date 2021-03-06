/*
 * RGBComponent.cpp
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#include "RGBComponent.h"

// RGBComponent::mode_names = { "normal", "daytime", "candle" };

RGBComponent::RGBComponent(const String node_id, const uint8_t sensor_id, const uint16_t red_pin,
						   const uint16_t green_pin, const uint16_t blue_pin) : AbstractComponent(node_id, sensor_id), red_pin(red_pin), green_pin(green_pin), blue_pin(blue_pin) {}

RGBComponent::~RGBComponent()
{
	// TODO Auto-generated destructor stub
}

void RGBComponent::loop()
{
	uint32_t now = millis();
	Color prev_color = this->current_color;
	if (now - this->last_write < 30)
		return;
	bool render = this->transformation.valid(now);
	if (this->light_state != LIGHT_ON && !(render && this->desired_color == _BLACK))
	{
		if (this->desired_color != _BLACK)
		{
			this->mode = MODE_DEFAULT;
			this->desired_color = _BLACK;
			this->transformation = Transformation(now, BLEND_TIME, this->current_color, this->desired_color);
		}
	}
	else if (!render)
		switch (this->mode)
		{
		case MODE_DAYTIME:
			this->blend(this->daytimeColor());
			break;
		case MODE_CANDLE:
			this->blend(this->candle.getColor(), this->candle.getTime());
			break;
		case MODE_DEFAULT:
			if (this->current_color != this->rgb)
				this->blend(this->rgb);
			break;
		}

	if (this->transformation.valid(now))
	{
		this->current_color = this->transformation.getColor(now);
	}
	else
	{
		this->current_color = Color(this->desired_color);
	}
	if (prev_color != this->current_color)
	{
		analogWrite(this->red_pin, min(this->current_color.red * PWMRANGE / 255, PWMRANGE));
		analogWrite(this->green_pin, min(this->current_color.green * PWMRANGE / 255, PWMRANGE));
		analogWrite(this->blue_pin, min(this->current_color.blue * PWMRANGE / 255, PWMRANGE));
		this->last_write = now;
	}
}

void RGBComponent::doOnRest()
{
	if (this->mode == MODE_UNDEF)
		this->mode = MODE_DEFAULT;
	bool fail = false;
	if (this->webServer == nullptr)
		return;
	if (this->webServer->hasArg("state"))
	{
		String state = this->webServer->arg("state");
		if (state == "on")
			this->light_state = LIGHT_ON;
		else if (state == "off")
			this->light_state = LIGHT_OFF;
		else
			fail = true;
	}
	if (!fail && this->webServer->hasArg("mode"))
	{
		String req_mode = this->webServer->arg("mode");
		if (req_mode == "candle")
		{
			this->mode = MODE_CANDLE;
			this->blend(this->candle.getColor(), this->candle.getTime());
		}
		else if (req_mode == "daytime")
		{
			this->mode = MODE_DAYTIME;
			this->blend(this->daytimeColor());
		}
		else if (req_mode == "normal")
			this->mode = MODE_DEFAULT;
		else
			fail = true;
	}

	if (fail)
	{
		this->webServer->send(400, "application/json; charset=utf-8", "{ \"error\":\"wrong parameter\" }");
	}
	else
	{
		if (this->webServer->hasArg("color"))
			this->rgb = Color(this->webServer->arg("color"));
		if (this->webServer->hasArg("r"))
			this->rgb.red = this->webServer->arg("r").toInt();
		if (this->webServer->hasArg("g"))
			this->rgb.green = this->webServer->arg("g").toInt();
		if (this->webServer->hasArg("b"))
			this->rgb.blue = this->webServer->arg("b").toInt();
		if (this->webServer->hasArg("brightness"))
			this->rgb = this->rgb.brightness(this->webServer->arg("brightness").toInt());
		if (this->mode == MODE_DEFAULT)
			this->blend(this->rgb);
		this->save();
		this->webServer->send(200, "application/json; charset=utf-8", String("{ \"r\":") + String(this->rgb.red) + String(", \"g\":") + String(this->rgb.green) + String(",\"b\":") + String(this->rgb.blue) + String(", \"mode\":\"") + String(this->mode) + String("\", \"state\":") + String(this->light_state));
	}
}

void RGBComponent::save()
{
	bool save = true;
	uint8_t actual_config[4];
	actual_config[0] = (this->light_state & 0b10000000) | (this->mode & 0b01111111);
	actual_config[1] = this->rgb.red;
	actual_config[2] = this->rgb.green;
	actual_config[3] = this->rgb.blue;
	uint8_t stored_config[4];
	File f = SPIFFS.open("/RGB", "r");
	if (f)
	{
		save = false;
		f.read(stored_config, 4);
		for (uint8_t i = 0; i < 4; i++)
			save = save || (actual_config[i] != stored_config[i]);
	}
	f.close();
	if (save)
	{
		f = SPIFFS.open("/RGB", "w");
		f.write(actual_config, 4);
		f.flush();
		f.close();
	}
}

void RGBComponent::registerRest(ESP8266WebServer &webServer)
{
	webServer.on(String("/rgb/") + String(this->sensor_id), HTTP_GET, std::bind(&RGBComponent::doOnRest, this));
	this->webServer = &webServer;
}

const Color RGBComponent::daytimeColor()
{
	uint32_t h = hour() * 60 + minute();
	double s = (h - 60) * M_PI / 12 / 60 + M_PI * 1.5;
	int16_t b = 82 + fmax(0, fmin(1, sin(s) * 2 + .5)) * 173;
	b = b * BRIGHTNESS_MAX_VALUE / RGB_MAX_VALUE;
	return Color(DAYTIMECOLOR).brightness(b);
}

void RGBComponent::blend(Color c, uint32_t time)
{
	uint32_t now = millis();
	this->desired_color = c;
	this->transformation = Transformation(now, time, this->current_color, this->desired_color);
}

void RGBComponent::setup()
{
	pinMode(this->red_pin, OUTPUT_OPEN_DRAIN);
	pinMode(this->green_pin, OUTPUT_OPEN_DRAIN);
	pinMode(this->blue_pin, OUTPUT_OPEN_DRAIN);

	File f = SPIFFS.open("/RGB", "r");
	uint8_t config[4];
	if (f)
	{
		f.read(config, 4);
		this->light_state = 0b10000000 & config[0];
		this->mode = 0b01111111 & config[0];
		this->rgb = Color(config[1], config[2], config[3]);
	}
	else
	{
		this->light_state = LIGHT_OFF;
	}
	f.close();
}

String time(uint32_t time)
{
	String result = String(time % 1000);
	time /= 1000;
	result = String(time % 60) + "." + result;
	time /= 60;
	result = String(time % 60) + ":" + result;
	time /= 60;
	result = String(time) + ":" + result;
	return result;
}

void RGBComponent::reportStatus(JsonObject &jo)
{
	jo["ID"] = this->sensor_id;
	jo["Mode"] = this->getModeName();
	jo["State"] = this->light_state ? "ON" : "OFF";
	jo["Brightness"] = String(this->rgb.brightness());
	JsonObject &c = jo.createNestedObject("Candle");
	c["color"] = this->candle.rand_color;
	c["colorString"] =
		this->candle.color_list[this->candle.rand_color].toHex();
	c["delay"] = this->candle.timer;
	jo["Color"] = this->rgb.toHex();
	jo["Desired color"] = this->desired_color.toHex();
	jo["Current color"] = this->current_color.toHex();
	jo["current time"] = time(millis());
	jo["rendering"] = this->transformation.valid(millis());
	jo["transformarion valid"] = this->transformation.valid(millis());
	jo["transformarion color"] = this->transformation.getColor(millis()).toHex();
	jo["last write"] = time(this->last_write);
	File f = SPIFFS.open("/RGB", "r");
	uint8_t config[4];
	if (f)
	{
		f.read(config, 4);
		jo["saved"] = String(config[0], HEX) + ":" + String(config[1], HEX) + ":" + String(config[2], HEX) + ":" + String(config[3], HEX);
	}
	f.close();
}

String RGBComponent::moduleName()
{
	return "RGB";
}

const String RGBComponent::getModeName()
{
	switch (mode)
	{
	case MODE_CANDLE:
		return "candle";
	case MODE_DAYTIME:
		return "daytime";
	case MODE_DEFAULT:
		return "normal";
	}
	return "UNKNOWN";
}
