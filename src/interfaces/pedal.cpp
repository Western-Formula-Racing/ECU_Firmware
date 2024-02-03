#include "interfaces/pedal.h"
#include "config/devices.h"

extern FS_CAN FS_CAN0;

Pedal::Pedal(Sensor *s1_p, Sensor *s2_p, Sensor *s3_p)
{
    Serial.println("pedal constructor called");
    sensor1 = s1_p;
    sensor2 = s2_p;
    sensor3 = s3_p;
    FS_CAN0.publish_CAN_msg(&pedalInfoMessage, FS_CAN::TEN_MS);
}

float Pedal::getPedalPosition()
{
    float value;
    sensor1Position = min(1,max(0,(sensor1->filteredValue - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE)));
    sensor2Position = min(1,max(0,(sensor2->filteredValue - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE)));

    if (max(sensor1Position, sensor2Position) - min(sensor1Position, sensor2Position) > APPS_PLAUSIBILITY_THRESHOLD)
    {
        value = 0;
    plausibilityFault = true;
    }
    else
    {
        value = (sensor1Position + sensor2Position) / 2;
        plausibilityFault = false;
    }
    pedalPostion = value;
    return value;
}

float Pedal::getFrontBreakPressure(){
    return sensor3->filteredValue;
}
