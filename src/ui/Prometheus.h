/*
 * Prometheus.h
 *
 *  Created on: 21. 9. 2018
 *      Author: hpa
 */

#ifndef SRC_UI_PROMETHEUS_H_
#define SRC_UI_PROMETHEUS_H_

#include <Arduino.h>
#include <WString.h>
#include <cstdint>

class Prometheus {
public:
	Prometheus(String name, float value, String type, String description);
	Prometheus(String name, float value, String type, String description,
			uint16_t timestamp);
	void attribute(String name, String value);
	String value_type();
	String help();
	String to_string(boolean header);
	String to_string();

private:
	String attributes = "";
	String name;
	String type;
	String description;
	float value;
	uint16_t timestamp = 0;
};

#endif /* SRC_UI_PROMETHEUS_H_ */
