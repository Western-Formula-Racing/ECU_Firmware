#pragma once
#include "sensor.h"
#include <ADC.h>
#include "lowPass.h"

ADC *adc = new ADC(); // adc object
Sensor sens1(adc,A0, 0, 1, 2, 2, 1e3, true);
std::array<Sensor*, 1> sensors = {&sens1};