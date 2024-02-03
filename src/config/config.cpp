// This file defines all the global objects as well as the key parameters for the vehicle
#include "config/config.h"

// Constants
const float VREF = 3.3f / 0.615; // reference voltage for all signal measurements (the 0.6 is for the voltage divider)
const float ADC_RES = 10;        // resolution of the Analog to Digital Converter in bits

const float APPS1_MIN_VOLTAGE = 0.01;
const float APPS1_MAX_VOLTAGE = 5.05;
const float APPS2_MIN_VOLTAGE = 5.05;
const float APPS2_MAX_VOLTAGE = 0.01;
const float APPS_PLAUSIBILITY_THRESHOLD = 0.1;
const float BRAKE_THRESHOLD = 100;
std::array<std::array<int,10>,10> torqueMap{{
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},

}};