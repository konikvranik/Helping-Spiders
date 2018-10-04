/*
 * AbstractComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef ABSTRACT_TEMPERATURE_COMPONENT_H_
#define ABSTRACT_TEMPERATURE_COMPONENT_H_

//#include "core.h"

class AbstractTemperatureComponent {
public:
	virtual float getTemperature();
	virtual String getType() {
		return "temperature";
	}
};

#endif /* ABSTRACT_COMPONENT_H_ */
