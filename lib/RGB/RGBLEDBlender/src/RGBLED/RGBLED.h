/******************************************************************************
*    RGBLED.h
*    by Erik Sikich
*    November 2016
*
*   This file is part of RGBLEDBlender.
*
*   RGBLEDBlender is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   RGBLEDBlender is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with RGBLEDBlender.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef RGBLED_h
#define RGBLED_h

struct LED{
    LED(uint8_t pin = 0, int16_t *value = NULL) : pin(pin), value(value) {}
    uint8_t pin;
    int16_t *value;
};

struct RGBLED{
    Color color;
    LED led[3];
    //Set up pins and pointers
    void Assign(const uint8_t red_pin, const uint8_t green_pin, const uint8_t blue_pin){
        led[0].pin = red_pin;
        led[1].pin = green_pin;
        led[2].pin = blue_pin;
        pinMode(red_pin, OUTPUT);
        pinMode(green_pin, OUTPUT);
        pinMode(blue_pin, OUTPUT);
        Init();
    }
    //Have the LED value point at the RGB color
    void Init(void){
        led[0].value = (int16_t*)&color.red;
        led[1].value = (int16_t*)&color.green;
        led[2].value = (int16_t*)&color.blue;
    }
    //Write to pins
    void Update(void) const{
        for(uint8_t i = 0; i < 3; i++){
            analogWrite(led[i].pin, *led[i].value);
        }
    }
};

#endif
