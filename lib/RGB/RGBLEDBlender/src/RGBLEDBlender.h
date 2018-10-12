/******************************************************************************
*    RGBLEDBlender.h
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

#ifndef RGBLEDBlender_h
#define RGBLEDBlender_h


#include "Arduino.h"
#include "Color/Color.h"
#include "RGBLED/RGBLED.h"
#include "Colors/Colors.h"

class RGBLEDBlender{
public:
    RGBLEDBlender(void);
    RGBLEDBlender(const uint8_t, const uint8_t, const uint8_t); //Constructors
    void Blend(const Color,const Color,const  uint32_t);        //Blend two colors
    void Random(const uint32_t);                                //Blend random colors
    void RandomCycle(Color*, const uint8_t, const uint32_t); //Blend random colors from a list
    void Cycle(Color*, const uint8_t, const uint32_t);                  //Cycle through list of colors
    bool Update(void);                                          //Update the blend
    void Hold(const Color);                                     //Hold color
    void SetPins(const uint8_t, const uint8_t, const uint8_t);
    void TurnOff(void);                                         //Turn off LED}
    Color GetColor(void) const;                                 //Get current color
private:
    void Init(void);
    int16_t IntPercent(const uint32_t, const uint32_t) const;   //Non floating point division function
    uint32_t start_millis_;                                     //Start and end times
    uint32_t end_millis_;
    Color diff_color_;                                          //Difference between the two colors
    Color start_color_;                                         //Starting and ending colors
    Color end_color_;
    RGBLED rgbled_;                                             //Our RGBLED
    Color *color_list_;                                         //List of colors
    uint8_t cycle_index_;                                       //Color list counter
};

#endif
