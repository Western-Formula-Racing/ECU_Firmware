#ifndef _PDM_
#define _PDM_

#include "required_includes.h"

// PDM Pin Definitions
enum PDM_PINS
{
    HSDIN6 = 5,
    HSDIN5 = 7,
    HSDIN4 = 8,
    HSDIN2 = 2,
    HSDIN7 = 4,
    HSDIN8 = 2,
    HSDIN1 = 32,
    HSDIN3 = 10 
};

namespace PDM
{
    // Setup the pins on the Teensy for the correct mode
    void begin();
    // Set the state of a pin
    void setPin(PDM_PINS pin, bool state);
}

#endif