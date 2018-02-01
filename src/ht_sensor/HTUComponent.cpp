/*
 * HTUComponent.cpp
 *
 *  Created on: 8. 7. 2017
 *      Author: petr
 */

#include "HTUComponent.h"

HTUComponent::HTUComponent(const uint8_t sensor_id, const uint8_t hum_sensor_id,
		const uint8_t scl, const uint8_t sda) :
		AbstractComponent(sensor_id), hum_sensor_id(hum_sensor_id), sda(sda), scl(
				scl) {
}

HTUComponent::~HTUComponent() {
	// TODO Auto-generated destructor stub
}

void HTUComponent::setup() {
	// Set delay between sensor readings based on sensor details.
	if (delayMS < HTU_DELAY)
		delayMS = HTU_DELAY;
	Log.notice("beginning HTU");
	//TwoWire mywire;
	//Wire.pins(sda,scl);
	sensor.begin();
	//sensor.setResolution(USER_REGISTER_RESOLUTION_RH11_TEMP11);
	Log.notice("HTU started");
}

void HTUComponent::presentation() {
	present(sensor_id, S_TEMP, "Temperature");
	present(hum_sensor_id, S_HUM, "Humidity");
}

void HTUComponent::loop() {
	Log.notice("HTU loop begin... ");
	if (lastRun + delayMS < millis()) {
		Log.notice("HTU loop inside");
		double old_temp = temp, old_hum = hum;
		temp = (sensor.readTemperature() - 32.0) * 5.0 / 9.0;
//	    temp = SHT2x.GetTemperature();
		Log.notice("HTU temp");
		hum = sensor.readHumidity();
//		hum = SHT2x.GetHumidity();
		Log.notice("HTU hum");
		lastRun = millis();
		if (old_temp != temp)
			send(temp_msg.set(temp, 2));
		if (old_hum != hum)
			send(hum_msg.set(hum, 2));
	}
	Log.notice("HTU loop end\r\n");
}

float HTUComponent::getHumidity() {
	return hum;
}

float HTUComponent::getTemperature() {
	return temp;
}

void HTUComponent::reportStatus(JsonObject& jo) {
	JsonObject& id = jo.createNestedObject("ID");
	id["temperature"] = this->sensor_id;
	id["humidity"] = this->hum_sensor_id;
	jo["Temperature"] = String(this->getTemperature()) + " °C";
	jo["Humidity"] = String(this->getHumidity()) + " %";
}

void HTUComponent::receive(const MyMessage&) {

}

String HTUComponent::moduleName() {
	return "HTU";
}
