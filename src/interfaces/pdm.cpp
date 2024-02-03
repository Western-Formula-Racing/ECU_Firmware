#include "interfaces/pdm.h"
extern FS_CAN FS_CAN0;
void PDM::begin()
{
    pinMode(HSDIN6, OUTPUT);
    pinMode(HSDIN5, OUTPUT);
    pinMode(HSDIN4, OUTPUT);
    pinMode(HSDIN2, OUTPUT);
    pinMode(HSDIN7, OUTPUT);
    pinMode(HSDIN8, OUTPUT);
    pinMode(HSDIN1, OUTPUT);
    pinMode(HSDIN3, OUTPUT);

    digitalWrite(HSDIN1,LOW);
    digitalWrite(HSDIN2,LOW);
    digitalWrite(HSDIN3,LOW);
    digitalWrite(HSDIN4,LOW);
    digitalWrite(HSDIN5,LOW);
    digitalWrite(HSDIN6,LOW);
    digitalWrite(HSDIN7,LOW);
    for(auto pin : pinState){
        pin = 0;
    }
    FS_CAN0.publish_CAN_msg(&pdmInfo, FS_CAN::THOUSAND_MS);
}

void PDM::setPin(PDM_PINS pin, bool state)
{
    pinState[hsdMap[pin]] = state;
    //Serial.printf("pin %d state: %d\n", pin, state);
    digitalWrite(pin, state);    
}