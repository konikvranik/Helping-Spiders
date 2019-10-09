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

#ifndef Color_h
#define Color_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define RGB_MAX_VALUE 0x100
#define BRIGHTNESS_MAX_VALUE 0x100

struct Color
{
    Color(int16_t red = 0, int16_t green = 0, int16_t blue = 0) : red(red), green(green), blue(blue) {}
    Color(String rgb)
    {
        long int color = strtol(rgb.c_str(), NULL, 16);
        blue = int16_t(color % 0x100);
        color = color / 0x100;
        green = int16_t(color % 0x100);
        red = int16_t(color / 0x100);
    }

    int16_t red;
    int16_t green;
    int16_t blue;
    Color &operator=(const Color &C)
    {
        red = C.red;
        green = C.green;
        blue = C.blue;
        return *this;
    }
    Color &operator+=(const Color &c)
    {
        red += c.red;
        green += c.green;
        blue += c.blue;
        return *this;
    }
    Color &operator+=(const int16_t &c)
    {
        red += c;
        green += c;
        blue += c;
        return *this;
    }
    Color &operator-=(const Color &c)
    {
        red -= c.red;
        green -= c.green;
        blue -= c.blue;
        return *this;
    }
    Color &operator-=(const int16_t &c)
    {
        red -= c;
        green -= c;
        blue -= c;
        return *this;
    }
    Color &operator*=(const Color &c)
    {
        red *= c.red;
        green *= c.green;
        blue *= c.blue;
        return *this;
    }
    Color &operator*=(const int16_t &c)
    {
        red *= c;
        green *= c;
        blue *= c;
        return *this;
    }
    Color &operator/=(const Color &c)
    {
        red /= c.red;
        green /= c.green;
        blue /= c.blue;
        return *this;
    }
    Color &operator/=(const int16_t &c)
    {
        red %= c;
        green %= c;
        blue %= c;
        return *this;
    }
    Color &operator%=(const Color &c)
    {
        red %= c.red;
        green %= c.green;
        blue %= c.blue;
        return *this;
    }
    Color &operator%=(const int16_t &c)
    {
        red %= c;
        green %= c;
        blue %= c;
        return *this;
    }
    String toHex()
    {
        String result = String(red, HEX) + ":";
        result.concat(String(green, HEX) + ":");
        result.concat(String(blue, HEX));
        return result;
    }
    const int16_t _shift_ray(const int16_t l)
    {
        return (l + RGB_MAX_VALUE) % RGB_MAX_VALUE;
    }
    Color &brightness(const int16_t brightness)
    {
        if (this->brightness() == 0)
        {
            this->red = brightness;
            this->green = brightness;
            this->blue = brightness;
        }
        int16_t difference = brightness - this->brightness();
        if (difference == 0)
            return *this;

        int16_t r = difference > 0 ? RGB_MAX_VALUE - this->red : this->red;
        int16_t g = difference > 0 ? RGB_MAX_VALUE - this->green : this->green;
        int16_t b = difference > 0 ? RGB_MAX_VALUE - this->blue : this->blue;

        const int16_t t = (r + g + b);
        this->red += difference * 3 * r / t;
        this->green += difference * 3 * g / t;
        this->blue += difference * 3 * b / t;
        return *this;
    }
    int16_t brightness()
    {
        return (red + green + blue) * BRIGHTNESS_MAX_VALUE / RGB_MAX_VALUE / 3;
    }
    Color &normalize()
    {
        this->red = _cn(this->red);
        this->green = _cn(this->green);
        this->blue = _cn(this->blue);
        return *this;
    }
    static int16_t _cn(int16_t color)
    {
        return color < 0 ? 0 : ((color > RGB_MAX_VALUE - 1) ? RGB_MAX_VALUE - 1 : color);
    }
};

inline Color operator+(Color lhs, const Color &rhs) { return Color(lhs) += rhs; }
inline Color operator+(Color lhs, const int16_t k) { return Color(lhs) += k; }
inline Color operator+(const int16_t k, Color rhs) { return Color(rhs) += k; }
inline Color operator-(Color lhs, const Color &rhs) { return Color(lhs) -= rhs; }
inline Color operator-(Color lhs, const int16_t k) { return Color(lhs) -= k; }
inline Color operator-(const int16_t k, Color rhs) { return Color(rhs) -= k; }
inline Color operator*(Color lhs, const Color &rhs) { return Color(lhs) *= rhs; }
inline Color operator*(Color lhs, const int16_t k) { return Color(lhs) *= k; }
inline Color operator*(const int16_t k, Color rhs) { return Color(rhs) *= k; }
inline Color operator/(Color lhs, const Color &rhs) { return Color(lhs) /= rhs; }
inline Color operator/(Color lhs, const int16_t k) { return Color(lhs) /= k; }
inline Color operator/(const int16_t k, Color rhs) { return Color(rhs) /= k; }
inline Color operator%(Color lhs, const Color &rhs) { return Color(lhs) %= rhs; }
inline Color operator%(Color lhs, const int16_t k) { return Color(lhs) %= k; }
inline Color operator%(const int16_t k, Color rhs) { return Color(rhs) %= k; }

inline bool operator==(const Color &lhs, const Color &rhs) { return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue; }
inline bool operator!=(const Color &lhs, const Color &rhs) { return !operator==(lhs, rhs); }

#endif