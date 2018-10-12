/******************************************************************************
*	Color.h
*	by Erik Sikich
*	November 2016
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

#ifndef Color_h
#define Color_h
/*
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif*/

struct Color{
    Color(int32_t red = 0, int32_t green = 0, int32_t blue = 0) : red(red), green(green), blue(blue) {}
    int32_t red;
    int32_t green;
    int32_t blue;
    Color& operator=(const Color &C) {red = C.red; green = C.green; blue = C.blue; return *this;}
    Color& operator+=(const Color& c) {red += c.red; green += c.green; blue += c.blue; return *this;}
    Color& operator+=(const int32_t& c) {red += c; green += c; blue += c; return *this;}
    Color& operator-=(const Color& c) {red -= c.red; green -= c.green; blue -= c.blue; return *this;}
    Color& operator-=(const int32_t& c) {red -= c; green -= c; blue -= c; return *this;}
    Color& operator*=(const Color& c) {red *= c.red; green *= c.green; blue *= c.blue; return *this;}
    Color& operator*=(const int32_t& c) {red *= c; green *= c; blue *= c; return *this;}
    Color& operator/=(const Color& c) {red /= c.red; green /= c.green; blue /= c.blue; return *this;}
    Color& operator/=(const int32_t& c) {red /= c; green /= c; blue /= c; return *this;}
};

inline Color operator+(Color lhs, const Color& rhs) { return Color(lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue); }
inline Color operator+(Color lhs, const int32_t k)  { return Color(lhs.red + k, lhs.green + k, lhs.blue + k); }
inline Color operator+(const int32_t k, Color rhs)  { return Color(k+ rhs.red, k+ rhs.green, k+ rhs.blue); }
inline Color operator-(Color lhs, const Color& rhs) { return Color(lhs.red - rhs.red, lhs.green - rhs.green, lhs.blue - rhs.blue); }
inline Color operator-(Color lhs, const int32_t k)  { return Color(lhs.red - k, lhs.green - k, lhs.blue - k); }
inline Color operator-(const int32_t k, Color rhs)  { return Color(k - rhs.red, k - rhs.green, k - rhs.blue); }
inline Color operator*(Color lhs, const Color& rhs) { return Color(lhs.red * rhs.red, lhs.green * rhs.green, lhs.blue * rhs.blue); }
inline Color operator*(Color lhs, const int32_t k)  { return Color(lhs.red * k, lhs.green * k, lhs.blue * k); }
inline Color operator*(const int32_t k, Color rhs)  { return Color(k * rhs.red, k * rhs.green, k * rhs.blue); }
inline Color operator/(Color lhs, const Color& rhs) { return Color(lhs.red / rhs.red, lhs.green / rhs.green, lhs.blue / rhs.blue); }
inline Color operator/(Color lhs, const int32_t k)  { return Color(lhs.red / k, lhs.green / k, lhs.blue / k); }
inline Color operator/(const int32_t k, Color rhs)  { return Color(k / rhs.red, k / rhs.green, k / rhs.blue); }
inline Color operator-(Color c)  { return Color(-c.red, -c.green, -c.blue); }

inline bool operator==(const Color& lhs, const Color& rhs){return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue; }
inline bool operator!=(const Color& lhs, const Color& rhs){return !operator==(lhs,rhs);}

#endif
