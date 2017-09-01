//*********************************************************************************
// State Button Debouncer - Platform Independent
// 
// Revision: 1.6
// 
// Description: Debounces buttons on a single port being used by the application.
// This module takes what the signal on a GPIO port is doing and removes
// the oscillations caused by a bouncing button and tells the application if
// the button(s) are debounced. This algorithm is robust against noise if the 
// amount of allowable debouncing states is adequate. Below is an example of how 
// the button debouncer would work in practice in relation to a single button:
// 
// Real Signal:     0011111111111110000000000000011111111111111111110000000000
// Bouncy Signal:   0010110111111111010000000000001010111011111111110101000000
// Debounced Sig:   0000000000000011000000000000000000000000000001110000000000
// 
// The debouncing algorithm used in this library is based partly on Jack
// Ganssle's state button debouncer example shown in, "A Guide to Debouncing" 
// Rev 4. http://www.ganssle.com/debouncing.htm
// 
// Revisions can be found here:
// https://github.com/tcleg
// 
// Copyright (C) 2014 Trent Cleghorn , <trentoncleghorn@gmail.com>
// 
//                                  MIT License
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//*********************************************************************************

//*********************************************************************************
// Headers
//*********************************************************************************
#include "button_debounce.h"

//*********************************************************************************
// Class Functions
//*********************************************************************************
Debouncer::
Debouncer(uint8_t pulledUpButtons)
{
    uint8_t i;
    
    index = 0;
    debouncedState = 0x00;
    changed = 0x00;
    pullType = pulledUpButtons;
    
    // Initialize the state array
    for(i = 0; i < NUM_BUTTON_STATES; i++)
    {
        state[i] = 0x00;
    }
}

void Debouncer::
ButtonProcess(uint8_t portStatus)
{
    uint8_t i;
    uint8_t lastDebouncedState = debouncedState;
    
    // If a button is high and is pulled down or
    // if a button is low and is pulled high, use a 1 bit
    // to denote the button has changed state. Else, a 0 bit
    // shows it is in a normal position.
    // Then, save the port status info into the state array
    state[index] = portStatus ^ pullType;
    
    // Debounce the buttons
    for(i = 0, debouncedState = 0xFF; i < NUM_BUTTON_STATES; i++)
    {
        debouncedState &= state[i];
    }
    
    // Check to make sure the index hasn't gone over the limit
    index++;
    if(index >= NUM_BUTTON_STATES)
    {
        index = 0;
    }
    
    // Calculate what changed.
    // If the switch was high and is now low, 1 and 0 xORed with
    // each other produces a 1. If the switch was low and is now
    // high, 0 and 1 xORed with each other produces a 1. Otherwise,
    // it is 0
    changed = debouncedState ^ lastDebouncedState;
}

uint8_t Debouncer::
ButtonPressed(uint8_t GPIOButtonPins)
{
    // If the button changed and it changed to a 1, then the
    // user just pressed the button.
    return (changed & debouncedState) & GPIOButtonPins;
}

uint8_t Debouncer:: 
ButtonReleased(uint8_t GPIOButtonPins)
{
    // If the button changed and it changed to a 0, then the
    // user just released the button.
    return (changed & (~debouncedState)) & GPIOButtonPins;
}

uint8_t Debouncer::
ButtonCurrent(uint8_t GPIOButtonPins)
{
    // Current pressed or not pressed states of the buttons expressed
    // as one 8 bit byte. A 0 bit denotes the button is not pressed,
    // and a 1 bit denotes it is being pressed.
    return debouncedState & GPIOButtonPins;
}

