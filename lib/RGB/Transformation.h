/******************************************************************************
*	Color.h
*	by Erik Sikich
*	November 2016
*
*   This file was part of RGBLEDBlender.
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

#ifndef Transformation_h
#define Transformation_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Color.h"

class Transformation
{
    uint32_t t_from;
    Color c_from;
    Color c_to;
    uint32_t duration;
    Color c_delta;

public:
    Transformation(){};
    Transformation(uint32_t t_from, uint32_t duration, Color c_from, Color c_to) : t_from(t_from), c_from(c_from), c_to(c_to), duration(duration), c_delta(c_to - c_from) {}
    Transformation(const Transformation &t) : t_from(t.t_from), c_from(t.c_from), c_to(t.c_to), duration(t.duration), c_delta(t.c_delta) {}

    int32_t calc_(int32_t c, uint32_t time)
    {
        return c * (time - t_from) / duration;
    }
    Color getColor(uint32_t time)
    {
        return c_from + Color(calc_(c_delta.red, time), calc_(c_delta.green, time), calc_(c_delta.blue, time));
    }

    boolean valid(uint32_t time)
    {
        return time - t_from < duration;
    }
};
#endif