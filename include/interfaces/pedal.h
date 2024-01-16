#pragma once
#include "required_includes.h"
#include "sensor.h"

class Pedal
{
    private:
        float pedalPostion;

    public:
        uint8_t pin1 = A0; //If this is wrong yell at faris
        uint8_t pin2 = A16;

        float sensor1MinVoltage;
        float sensor1MaxVoltage;
        float sensor2MinVoltage;
        float sensor2MaxVoltage;

        Sensor sensor1;
        Sensor sensor2;

        Sensor *sensor1_p = &sensor1; //pointers to the sensors for adding to the sensor list
        Sensor *sensor2_p = &sensor2;

        float plausabilityFault; // float so we can Make it a CAN signal
        
        float getPedalPosition();
};