/******************************************************************************
*    RGBLEDBlender.cpp
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
*   You should have received a copy of the GNU General Public License
*
*   along with RGBLEDBlender.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "RGBLEDBlender.h"

//init function
void RGBLEDBlender::Init(void){
    start_millis_ = 0;
    end_millis_ = 0;
    cycle_index_ = 0;
    srand(analogRead(0));
    color_list_ = NULL;
}

//Initialize variables
RGBLEDBlender::RGBLEDBlender(void){
    rgbled_.Init();
    Init();
}

//Initialize variables
RGBLEDBlender::RGBLEDBlender(const uint8_t red_pin, const uint8_t green_pin, const uint8_t blue_pin){
    SetPins(red_pin, green_pin, blue_pin);   
}

//Blend two colors together
void RGBLEDBlender::Blend(const Color start_color, const Color finish_color, const uint32_t blend_millis){
    start_millis_ = millis();                   //Store the time that the blend will begin and end
    end_millis_ = start_millis_ + blend_millis;
    start_color_ = start_color;                 //Set starting and ending colors
    end_color_ = finish_color;
    diff_color_ = finish_color - start_color;               //Calculate the difference between the two colors
    rgbled_.color = start_color;                //Initialize the LED's color
}

//Blend random colors
void RGBLEDBlender::Random(const uint32_t blend_millis){
    //Update the blend
    if(Update() == true){
        //Use the last color as the starting point and a random end color
        Blend(end_color_, {rand() % 255, rand() % 255, rand() % 255}, blend_millis);
    }
}

//Find how far through the blend time we are without using floating point math
int16_t RGBLEDBlender::IntPercent(const uint32_t a, const uint32_t b) const{
    return (255 * a + b / 2) / b;
}

//Cycle through random colors
void RGBLEDBlender::RandomCycle(Color *color_list, const uint8_t size_, const uint32_t blend_millis){
    color_list_ = color_list;
    if(Update() == true){
        Blend(end_color_, color_list_[rand() % size_], blend_millis);
    }
}

//Cycle through a list of colors
void RGBLEDBlender::Cycle(Color *color_list, const uint8_t size_, const uint32_t blend_millis){
    //color_list_ = color_list;
    
    //Reset cycle index at the end of the list
    if(cycle_index_ == size_){
        cycle_index_ = 0;
    }
    //Update the color
    if(Update() == true){
        //Make sure we're not at the end of the list
        if(cycle_index_ != size_ - 1){
            Blend(color_list[cycle_index_], color_list[cycle_index_ + 1], blend_millis);
        }else{
            //End of list, loop back to zero
            Blend(color_list[size_ - 1], color_list[0],  blend_millis);
        }
        //Increment the list counter
        ++cycle_index_;
    }
}

//Update RGB LED colors, return true if the blend is done
bool RGBLEDBlender::Update(void){
    bool result = true;
    uint32_t now = millis();
    if(now <= end_millis_){                     //Check to see if we're done blending
                                                //Find out how far through the blend time we are
        uint16_t percentage = IntPercent(now - start_millis_, end_millis_ - start_millis_);
        rgbled_.color = start_color_ + ((diff_color_) * percentage) / 255;    
        rgbled_.Update();                       //Update the LED color
        result = false;                         //Not done blending so return false
    }
    return result;
}

//Hold color
void RGBLEDBlender::Hold(const Color color){
    rgbled_.color = color;
    rgbled_.Update();
}

void RGBLEDBlender::SetPins(const uint8_t red_pin, const uint8_t green_pin, const uint8_t blue_pin){
    rgbled_.Assign(red_pin, green_pin, blue_pin);
    Init();
}

//Turn off LED,
void RGBLEDBlender::TurnOff(void){
    Hold(_BLACK);
}

Color RGBLEDBlender::GetColor(void) const{
    return rgbled_.color;
}