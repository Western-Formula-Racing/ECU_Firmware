#include "interfaces/pdm.h"

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
}

void PDM::setPin(PDM_PINS pin, bool state)
{
    Serial.printf("pin %d state: %d\n", pin, state);
    digitalWrite(pin, state);    
}