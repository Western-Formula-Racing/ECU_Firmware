#include "interfaces/pedal.h"
#include "config/devices.h"

extern FS_CAN dataCAN;

Pedal::Pedal(Sensor *s1_p, Sensor *s2_p, Sensor *s3_p, Sensor *s4_p)
{
    Serial.println("pedal constructor called");
    sensor1 = s1_p;
    sensor2 = s2_p;
    sensor3 = s3_p;
    sensor4 = s4_p;
    #ifndef REAR
    dataCAN.publish_CAN_msg(&pedalInfoMessage, FS_CAN::HUNDRED_MS);
    #endif
}

float Pedal::getPedalPosition()
{
    float value;
    appsSensor1Position = min(1,max(0,(sensor1->filteredValue - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE)));
    appsSensor2Position = min(1,max(0,(sensor2->filteredValue - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE)));
    brakePressure1 = sensor3->filteredValue;
    brakePressure2 = sensor4->filteredValue;
    if (max(appsSensor1Position, appsSensor2Position) - min(appsSensor1Position, appsSensor2Position) > APPS_PLAUSIBILITY_THRESHOLD)
    {
        value = 0;
        plausibilityFault = true;
    }
    else if(brakePressure1 > BRAKE_THRESHOLD){
        value = 0;
    }
    else
    {
        value = (appsSensor1Position + appsSensor2Position) / 2;
        plausibilityFault = false;
    }
    pedalPostion = value;
    return value;
}

float Pedal::getFrontBreakPressure(){ 
    return brakePressure1;
}
