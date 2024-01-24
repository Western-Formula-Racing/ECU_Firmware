#include "interfaces/pedal.h"
#include "config/devices.h"
extern FS_CAN FS_CAN0;
Pedal::Pedal(Sensor *s1_p, Sensor *s2_p){
    delay(5000);
    Serial.println("pedal constructor called");
    sensor1 = s1_p;
    sensor2 = s2_p;
    FS_CAN0.publish_CAN_msg(&pedalInfoMessage, FS_CAN::HUNDRED_MS);
}

float Pedal::getPedalPosition(){
    sensor1Position = (sensor1->filteredValue - APPS1_MIN_VOLTAGE)/( APPS1_MIN_VOLTAGE - APPS2_MAX_VOLTAGE);
    sensor2Position = (sensor2->filteredValue - APPS2_MIN_VOLTAGE)/( APPS2_MIN_VOLTAGE - APPS2_MAX_VOLTAGE);

    return (sensor1Position + sensor2Position)/2;

}