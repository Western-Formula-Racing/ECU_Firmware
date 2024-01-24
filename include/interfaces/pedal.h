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

        float sensor1Position;
        float sensor2Position;
        float sensorPlausability;

        FS_CAN::CAN_SIGNAL sensorPlausabilitySignal{&sensorPlausability, 0,8, false, 1.0f,0};
        FS_CAN::CAN_MSG pedalInfoMessage{400,{&sensorPlausabilitySignal}};

        Sensor *sensor1;
        Sensor *sensor2;

        float plausabilityFault; // float so we can Make it a CAN signal
        
        float getPedalPosition();
        Pedal(Sensor *s1_p, Sensor *s2_p);
};