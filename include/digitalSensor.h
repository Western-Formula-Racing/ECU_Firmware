#include "sensor.h"

class DigitalSensor : public Sensor
{
public:
    DigitalSensor(ADC *adc_handle, uint8_t pin);
    void read();

};