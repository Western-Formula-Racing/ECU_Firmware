#include "interfaces/pedal.h"
#include "config/devices.h"

extern FS_CAN dataCAN;
extern FS_CAN controlCAN;

Pedal::Pedal(Sensor *s1_p, Sensor *s2_p, Sensor *s3_p, Sensor *s4_p)
{
    Serial.println("pedal constructor called");
    sensor1 = s1_p;
    sensor2 = s2_p;
    sensor3 = s3_p;
    sensor4 = s4_p;
    float apps1_min;
    float apps1_max;
    float apps2_min;
    float apps2_max;
    EEPROM.get(0x8, apps1_min);
    EEPROM.get(0x8 + sizeof(float), apps1_max);
    EEPROM.get(0x8 + (2*0x8+sizeof(float)), apps2_min);
    EEPROM.get(0x8 + (3*0x8+sizeof(float)), apps2_max);

    if(apps1_min<5.0 and apps1_min>0.0 and apps1_max<5.0 and apps1_max >0.0 and
        apps2_min<5.0 and apps2_min>0.0 and apps2_max<5.0 and apps2_max >0.0 ){
            APPS1_MIN_VOLTAGE = apps1_min;
            APPS1_MAX_VOLTAGE = apps1_max;
            APPS2_MIN_VOLTAGE = apps2_min;
            APPS2_MAX_VOLTAGE = apps2_max;
            Serial.println("pedal values loaded");  

        }
    Serial.printf(">APPS1_MAX:%.2f\n", APPS1_MAX_VOLTAGE);
    Serial.printf(">APPS2_MAX:%.2f\n", APPS2_MAX_VOLTAGE);
    Serial.printf(">APPS1_MIN:%.2f\n", APPS1_MIN_VOLTAGE);
    Serial.printf(">APPS2_MIN:%.2f\n", APPS2_MIN_VOLTAGE);


#ifndef REAR
    controlCAN.publish_CAN_msg(&pedalInfoMessage, FS_CAN::HUNDRED_MS);
    controlCAN.publish_CAN_msg(&brakeLightMsg, FS_CAN::HUNDRED_MS);

#endif
}

float Pedal::getPedalPosition()
{
    float value=0;
    appsSensor1Position = min(1, max(0, (sensor1->filteredValue - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE)));
    appsSensor2Position = min(1, max(0, (sensor2->filteredValue - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE)));
    brakePressure1 = sensor3->filteredValue;
    brakePressure2 = sensor4->filteredValue;
    avgbrakePressure = (brakePressure1 + brakePressure2) / 2;
    float pedalDisagreement = max(appsSensor1Position, appsSensor2Position) - min(appsSensor1Position, appsSensor2Position);
    float pedalAvg = ((appsSensor1Position + appsSensor2Position) / 2);
    if (avgbrakePressure >= BRAKE_LIGHT_THRESHOLD){
        Devices::Get().GetRearECU().setHSD(0,1);// break light
    }
    else{
        Devices::Get().GetRearECU().setHSD(0,0);
    }
    if (avgbrakePressure >= BRAKE_PLAUSABILITY_THRESHOLD){
        brakeLatch = true;
    }

    if (pedalDisagreement > APPS_PLAUSIBILITY_THRESHOLD or sensor1->filteredValue <= 0.01 or sensor2->filteredValue <= 0.01 ) // check for open circuit condition basically
    {
        value = 0;
        plausibilityFault = true;
    }

    if (pedalAvg <= 0.05)
    { // only clear plausability faults if throttle is bellow 5%
        plausibilityFault = false;
        brakeLatch = false;
    }

    if (plausibilityFault == false and brakeLatch == false)
    {
        value = pedalAvg;
        plausibilityFault = false;
    }

    pedalPostion = value;
    return value;
}

float Pedal::getFrontBreakPressure()
{
    return brakePressure1;
}
