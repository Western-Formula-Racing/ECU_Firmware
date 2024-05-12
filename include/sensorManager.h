#ifndef _SENSORMANAGER_
#define _SENSORMANAGER_

#include "sensor.h"
#include "CAN/FS_CAN.h"
class SensorManager
{
public:
    Sensor *sensors;
    SensorManager();
    void readSensors();

    //CAN setup
    std::array<float, 18> sensorValues;

    std::array<FS_CAN::CAN_SIGNAL, 18> sensorSignals = 
    {{
        {&sensorValues[0], 0, 16, true, 0.01f, 0},
        {&sensorValues[1], 0, 16, true, 0.01f, 0},
        {&sensorValues[2], 0, 16, true, 0.01f, 0},
        {&sensorValues[3], 0, 16, true, 0.01f, 0},
        {&sensorValues[4], 0, 16, true, 0.01f, 0},
        {&sensorValues[5], 0, 16, true, 0.01f, 0},
        {&sensorValues[6], 0, 16, true, 0.01f, 0},
        {&sensorValues[7], 0, 16, true, 0.01f, 0},
        {&sensorValues[8], 0, 16, true, 0.01f, 0},
        {&sensorValues[9], 0, 16, true, 0.01f, 0},
        {&sensorValues[10], 0, 16, true, 0.01f, 0},
        {&sensorValues[11], 0, 16, true, 0.01f, 0},
        {&sensorValues[12], 0, 16, true, 0.01f, 0},
        {&sensorValues[13], 0, 16, true, 0.01f, 0},
        {&sensorValues[14], 0, 16, true, 0.01f, 0},
        {&sensorValues[15], 0, 16, true, 0.01f, 0},
        {&sensorValues[16], 0, 16, true, 0.01f, 0},
        {&sensorValues[17], 0, 16, true, 0.01f, 0},
    
    }};


};

#endif