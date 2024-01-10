#include "config/devices.h"
/*
This file defines the sensors and devices connnected to the ECU
*/



//sensors
ADC *adc = new ADC(); // adc object
Sensor sense1(adc,A0, 0, 1, 2, 2, 1e3, true);
std::array <Sensor*, 1>sensors = {&sense1};