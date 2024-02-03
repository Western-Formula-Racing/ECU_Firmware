#pragma once
#include "required_includes.h"
#include "sensor.h"

class Pedal
{
    private:
        float pedalPostion;
        FS_CAN::CAN_SIGNAL sensorPlausibilitySignal{&plausibilityFault, 0,1, true, 1.0f,0};
        FS_CAN::CAN_SIGNAL sensor1PositionSignal{&sensor1Position, 24,8, true, 0.01f,0};
        FS_CAN::CAN_SIGNAL sensor2PositionSignal{&sensor2Position, 16,8, true, 0.01f,0};
        FS_CAN::CAN_SIGNAL pedalPositionSignal{&pedalPostion, 8,8, true, 0.01f,0};
        FS_CAN::CAN_MSG pedalInfoMessage{2000,
        {
        &sensorPlausibilitySignal, 
        &sensor1PositionSignal,
        &sensor2PositionSignal,
        &pedalPositionSignal
        }};

    public:
        uint8_t pin1 = A0; //If this is wrong yell at faris
        uint8_t pin2 = A16;

        float sensor1Position;
        float sensor2Position;

        

        Sensor *sensor1;
        Sensor *sensor2;

        float plausibilityFault; // float so we can Make it a CAN signal
        
        float getPedalPosition();
        Pedal(Sensor *s1_p, Sensor *s2_p);
};