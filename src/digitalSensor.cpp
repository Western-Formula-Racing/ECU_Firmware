#include "digitalSensor.h"

DigitalSensor::DigitalSensor(ADC *adc_handle, uint8_t pin) : Sensor{adc_handle, pin, 0,1,2,100,1e3,true}{

}

void DigitalSensor::read(){
    if (overridden == false){
        value = static_cast<float>(!digitalRead(pin));
    }
    else{
        value = override_value;
    }
}