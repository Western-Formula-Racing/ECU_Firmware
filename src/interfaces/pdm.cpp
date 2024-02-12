#include "interfaces/pdm.h"
extern FS_CAN dataCAN;
PDM::PDM()
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
    dataCAN.publish_CAN_msg(&pdmInfo, FS_CAN::THOUSAND_MS);
}

void PDM::setPin(PDM_PINS pin, int state)
{
    pinState[hsdMap[pin]] = static_cast<float>(state);
    Serial.printf(">pin%d_state:%f\n", pin, pinState[hsdMap[pin]]);
    digitalWrite(pin, state);    
}