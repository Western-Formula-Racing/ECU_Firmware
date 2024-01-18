#include "interfaces/pedal.h"

Pedal::Pedal(Sensor *s1_p, Sensor *s2_p){
    delay(5000);
    Serial.println("sensor constructor called");
    sensor1 = s1_p;
    sensor2 = s2_p;
    FS_CAN0.publish_CAN_msg(&pedalInfoMessage, FS_CAN::HUNDRED_MS);
}