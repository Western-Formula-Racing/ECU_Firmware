#include "digitalSensor.h"

DigitalSensor::DigitalSensor(ADC *adc_handle, uint8_t pin) : Sensor{adc_handle, pin, 0,1,2,1,1e3,true}{

}

void DigitalSensor::read(){
    value = static_cast<float>(digitalRead(pin));
    Serial.printf(">RawButton:%d\n", static_cast<int>(value));
    filteredValue = filter.filt(value);
}