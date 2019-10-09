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

void RGBComponent::loop()
{
	uint32_t now = millis();
	uint16_t remaining = this->render_finish - now;
	if (remaining <= 0)
		switch (this->mode)
		{
		case MODE_DAYTIME:
			this->blend(this->daytimeColor());
			break;
		case MODE_CANDLE:
			this->updateCandle();
			break;
		}

	if (this->current_color == this->desired_color)
		return;
	if (remaining > 0)
	{
		uint16_t l = (this->render_finish - this->last_render);
		this->current_color = (this->desired_color - this->current_color) * (1 - remaining / l);
		this->last_render = now;
	}
	else
	{
		this->current_color = this->desired_color;
	}
	analogWrite(this->red_pin, this->current_color.red);
	analogWrite(this->green_pin, this->current_color.green);
	analogWrite(this->blue_pin, this->current_color.blue);
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
			result = false;
		}
		else if (req_mode == "daytime")
		{
			this->mode = MODE_DAYTIME;
			result = false;
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
			this->rgb = h2c(this->webServer->arg("color"));
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
		this->webServer->send(200, "application/json; charset=utf-8", String("{ \"r\":") + String(this->rgb.red) + String(", \"g\":") + String(this->rgb.green) + String(",\"b\":") + String(this->rgb.blue) + String(", \"mode\":\"") + String(this->mode) + String("\", \"state\":") + String(this->light_state));
	}
}

void RGBComponent::registerRest(ESP8266WebServer &webServer)
{
	webServer.on(String("/rgb/") + String(this->sensor_id), HTTP_GET, std::bind(&RGBComponent::doOnRest, this));
	this->webServer = &webServer;
}

const Color RGBComponent::h2c(const String rgb)
{
	long int color = strtol(rgb.c_str(), NULL, 16);
	int16_t blue = int16_t(color % 0x100);
	color = color / 0x100;
	int16_t green = int16_t(color % 0x100);
	int16_t red = int16_t(color / 0x100);
	return Color(red, green, blue);
}

int16_t colorRange(int16_t color)
{
	return color < 0 ? 0 : ((color > RGB_MAX_VALUE - 1) ? RGB_MAX_VALUE - 1 : color);
}

const Color RGBComponent::cn(Color rgb)
{
	return Color(colorRange(rgb.red), colorRange(rgb.green), colorRange(rgb.blue));
}

const Color RGBComponent::daytimeColor()
{
	uint32_t h = hour() * 60 + minute();
	double s = (h - 60) * M_PI / 12 / 60 + M_PI * 1.5;
	int16_t b = 82 + fmax(0, fmin(1, sin(s) * 2 + .5)) * 173;
	b = b * BRIGHTNESS_MAX_VALUE / RGB_MAX_VALUE;
	return Color(DAYTIMECOLOR).brightness(b);
}

void RGBComponent::updateCandle()
{
	this->desired_color = this->candle.getColor();
	this->render_finish = millis() + candle.getTime();
}

void RGBComponent::blend(Color c)
{
	// If last dimmer state is zero, set dimmer to 100
	if (light_state != LIGHT_ON)
	{
		mode = MODE_DEFAULT;
		c = _BLACK;
	}
	this->desired_color = c;
	this->render_finish = millis() + BLEND_TIME;
}

void RGBComponent::setup()
{
}

void RGBComponent::reportStatus(JsonObject &jo)
{
	jo["ID"] = this->sensor_id;
	jo["Mode"] = this->getModeName();
	jo["State"] = this->light_state ? "ON" : "OFF";
	jo["Result"] = this->result ? "TRUE" : "FALSE";
	jo["Dimm"] = String(this->rgb.brightness()) + "%";
	JsonObject &c = jo.createNestedObject("Candle");
	c["color"] = this->candle.rand_color;
	c["colorString"] =
		this->candle.color_list[this->candle.rand_color].toHex();
	c["delay"] = this->candle.timer;
	jo["Color"] = "0x" + this->rgb.toHex();
	jo["DEsired"] = "0x" + this->desired_color.toHex();
	jo["Real color"] = "0x" + this->current_color.toHex();
}

String RGBComponent::moduleName()
{
	return "RGB";
}
