#ifndef _PDM_
#define _PDM_

#include "required_includes.h"
#include <unordered_map>

enum PDM_PINS
{
    HSDIN6 = 5,
    HSDIN5 = 7,
    HSDIN4 = 8,
    HSDIN2 = 28,
    HSDIN7 = 4,
    HSDIN8 = 2,
    HSDIN1 = 32,
    HSDIN3 = 10 
};
class PDM
{

public:
    // PDM Pin Definitions

std::unordered_map<int,int>  hsdMap{
    {HSDIN1,0},
    {HSDIN2,1},
    {HSDIN3,2},
    {HSDIN4,3},
    {HSDIN5,4},
    {HSDIN6,5},
    {HSDIN7,6},
};

    std::array<float,7> pinState;
    std::array<FS_CAN::CAN_SIGNAL,7> pinStateSignals{{
        {&pinState[0], 0, 1, true, 1.0f, 0},
        {&pinState[1], 1, 1, true, 1.0f, 0},
        {&pinState[2], 2, 1, true, 1.0f, 0},
        {&pinState[3], 3, 1, true, 1.0f, 0},
        {&pinState[4], 4, 1, true, 1.0f, 0},
        {&pinState[5], 5, 1, true, 1.0f, 0},
        {&pinState[6], 6, 1, true, 1.0f, 0}
    }};
    
    #ifndef REAR
    FS_CAN::CAN_MSG pdmInfo{2001, {&pinStateSignals[0],&pinStateSignals[1],&pinStateSignals[2],&pinStateSignals[3],&pinStateSignals[4],&pinStateSignals[5],&pinStateSignals[6]}};
    #endif
    #ifdef REAR
    FS_CAN::CAN_MSG pdmInfo{2004, {&pinStateSignals[0],&pinStateSignals[1],&pinStateSignals[2],&pinStateSignals[3],&pinStateSignals[4],&pinStateSignals[5],&pinStateSignals[6]}};
    #endif
    // Setup the pins on the Teensy for the correct mode
    PDM();
    // Set the state of a pin
    void setPin(PDM_PINS pin, int state);
};

#endif