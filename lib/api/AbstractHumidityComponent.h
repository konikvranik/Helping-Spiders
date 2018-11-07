/*
 * AbstractComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef ABSTRACT_HUMIDITY_COMPONENT_H_
#define ABSTRACT_HUMIDITY_COMPONENT_H_

#include "AbstractComponent.h"

class AbstractHumidityComponent {
public:
  virtual double getHumidity();
  virtual String getType()
  {
	  return "humidity";
	}

};

#endif /* ABSTRACT_COMPONENT_H_ */
