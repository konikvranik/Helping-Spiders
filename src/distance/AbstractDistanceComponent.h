/*
 * AbstractComponent.h
 *
 *  Created on: 20. 6. 2017
 *      Author: hpa
 */

#ifndef ABSTRACT_DISTANCE_COMPONENT_H_
#define ABSTRACT_DISTANCE_COMPONENT_H_

class AbstractDistanceComponent {
public:
  virtual float getDistance();
	virtual String getType() {
		return "distance";
	}
};

#endif /* ABSTRACT_DISTANCE_COMPONENT_H_ */
