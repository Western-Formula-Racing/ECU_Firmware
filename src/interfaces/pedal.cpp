#include "interfaces/pedal.h"
#include "config/devices.h"
Pedal::Pedal(Sensor *s1_p, Sensor *s2_p){
    delay(5000);
    Serial.println("pedal constructor called");
    sensor1 = s1_p;
    sensor2 = s2_p;
    Devices::GetFS_Can0().publish_CAN_msg(&pedalInfoMessage, FS_CAN::HUNDRED_MS);
}