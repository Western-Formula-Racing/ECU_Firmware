#pragma once
#include "required_includes.h"
#include "sensor.h"

class Pedal
{
private:
    float pedalPostion;
    FS_CAN::CAN_SIGNAL sensorPlausibilitySignal{&plausibilityFault, 0, 1, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL appsSensor1PosSignal{&appsSensor1Position, 24, 8, true, 0.01f, 0};
    FS_CAN::CAN_SIGNAL appsSensor2PosSignal{&appsSensor2Position, 16, 8, true, 0.01f, 0};
    FS_CAN::CAN_SIGNAL pedalPositionSignal{&pedalPostion, 8, 8, true, 0.01f, 0};
    FS_CAN::CAN_SIGNAL brakePressure1Signal{&brakePressure1, 32, 16, true, 0.01f, 0};
    FS_CAN::CAN_SIGNAL brakePressure2Signal{&brakePressure2, 48, 16, true, 0.01f, 0};
    FS_CAN::CAN_MSG pedalInfoMessage{2000,
                                     {&sensorPlausibilitySignal,
                                      &appsSensor1PosSignal,
                                      &appsSensor2PosSignal,
                                      &pedalPositionSignal,
                                      &brakePressure1Signal,
                                      &brakePressure2Signal,
                                      }};

public:
    uint8_t pin1 = A0; // If this is wrong yell at faris
    uint8_t pin2 = A16;
    uint8_t pin3 = A7;
    float appsSensor1Position;
    float appsSensor2Position;
    float brakePressure1;
    float brakePressure2;
    float avgbrakePressure;
    Sensor *sensor1;
    Sensor *sensor2;
    Sensor *sensor3;
    Sensor *sensor4;
    float plausibilityFault; // float so we can Make it a CAN signal

    float getPedalPosition();
    float getFrontBreakPressure();
    Pedal(Sensor *s1_p, Sensor *s2_p, Sensor *s3_p, Sensor *s4_p);
};