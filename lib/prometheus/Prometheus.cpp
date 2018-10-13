/*
 * Prometheus.cpp
 *
 *  Created on: 21. 9. 2018
 *      Author: hpa
 */

#include "Prometheus.h"

Prometheus::Prometheus(const String name, float value, String type,
					   String description)
{
	this->name = name;
	this->type = type;
	this->description = description;
	this->value = value;
	this->attribute("node", str(NODE_ID));
	this->attribute("version", str(SV));
}

Prometheus::Prometheus(const String name, float value, String type,
					   String description, uint16_t timestamp)
{
	Prometheus(name, value, type, description);
	this->timestamp = timestamp;
}

void Prometheus::attribute(String name, String value)
{
	if (name.length() == 0)
		return;
	if (attributes.length() > 0)
		attributes += ",";
	attributes += name + "=\"" + value + '"';
}

String Prometheus::to_string()
{
	return this->to_string(false);
}

String Prometheus::to_string(boolean header)
{
	return (header ? this->help() + this->value_type() : "") + name + "{" + attributes + "}\t" + String(value) + (timestamp > 0 ? "\t" + String(timestamp) : "") + "\n";
}

String Prometheus::value_type()
{
	return "# TYPE " + name + " " + this->type + "\n";
}

String Prometheus::help()
{
	return "# HELP " + name + " " + description + "\n";
}
