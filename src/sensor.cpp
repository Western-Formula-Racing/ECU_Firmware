#include "sensor.h"

Sensor::Sensor(ADC *adc_handle, uint8_t pin_, float offset, float scale, uint8_t filter_order, float f0, float fs, bool adaptive)
{
    Serial.printf("sensor %d with address:%#010x initalized\n", pin, (int)this);
    adc = adc_handle;
    pin = pin_;
    offset_ = offset;
    scale_ = scale;
    filter = LowPass{filter_order, f0, fs, adaptive};
}

void Sensor::read()
{
    if(overridden == false){
        rawValue = adc->analogRead(pin);
        voltage = (rawValue / pow(2, ADC_RES)) * VREF;
        value = (voltage * scale_) + offset_;
        filteredValue = filter.filt(value);
    }
    else{
        value = override_value;
        filteredValue = filter.filt(value);
    }
}

void Sensor:: override(float value){
    overridden = true;
    override_value = value;
}
void Sensor:: disable_override(){
    overridden = false;
}