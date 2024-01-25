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
}

void PDM::setPin(PDM_PINS pin, bool state)
{
    digitalWriteFast(pin, state);
}