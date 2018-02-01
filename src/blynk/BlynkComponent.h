/*
 * BlynkComponent.h
 *
 *  Created on: 4. 7. 2017
 *      Author: hpa
 */

#ifndef BLYNKCOMPONENT_H_
#define BLYNKCOMPONENT_H_

#include "../AbstractComponent.h"
#include <BlynkSimpleEsp8266.h>

class BlynkComponent: public AbstractComponent {
	String token, ssid, pwd, server;
	uint16_t port;
public:
	BlynkComponent(String token, String ssid, String pwd, String server =
	BLYNK_DEFAULT_DOMAIN, uint16_t port = BLYNK_DEFAULT_PORT);
	virtual ~BlynkComponent();
	virtual void setup();
	virtual void loop();
	virtual void receive(const MyMessage&);
	virtual void presentation();
	virtual void reportStatus(const JsonObject&);
	virtual String moduleName();
};

#endif /* BLYNKCOMPONENT_H_ */
