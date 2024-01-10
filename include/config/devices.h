#pragma once
#include <ADC.h>
#include "sensor.h"



/*
This file defines the sensors and devices connnected to the ECU
*/


//sensors
extern ADC *adc; // adc object
extern Sensor sense1;
extern std::array <Sensor*, 1>sensors; 