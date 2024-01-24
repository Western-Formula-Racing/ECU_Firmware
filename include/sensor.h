#pragma once
#include "required_includes.h"
#include <ADC.h>
#include "lowPass.h"


class Sensor
{
private:
    float offset_;
    float scale_;

    uint8_t order;
    LowPass filter;

    ADC *adc;

public:
    uint8_t pin;
    float value;
    int rawValue;
    float filteredValue; 

    Sensor(ADC *adc_handle, uint8_t pin, float offset, float scale, uint8_t filter_order, float f0, float fs, bool adaptive);
    void read();
};