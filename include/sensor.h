#ifndef _SENSOR_
#define _SENSOR_

#include <ADC.h>
#include "required_includes.h"
#include "low_pass.h"

class Sensor
{
private:
    float offset_;
    float scale_;
    uint8_t order;
    ADC *adc;
    

public:
    uint8_t pin;
    float value;
    int rawValue;
    float filteredValue;
    LowPass filter;
    bool overridden; 
    float override_value;
    Sensor(ADC *adc_handle, uint8_t pin, float offset, float scale, uint8_t filter_order, float f0, float fs, bool adaptive);
    void read();
    void override(float value);
    void disable_override();
};
#endif