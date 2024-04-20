#include "interfaces/bms.h"
#include "config/devices.h"

extern State state;
extern FS_CAN dataCAN;
extern FS_CAN controlCAN;



BMS::BMS()
{
    Serial.println("BMS constructor called");
    controlCAN.subscribe_to_message(&packStateMessage);
    controlCAN.subscribe_to_message(&packFaultsMessage);
    #ifndef REAR
    controlCAN.subscribe_to_message(&BMSFaultMessage);
    #endif
}

bool BMS::hasFault()
{
    return packFaults1 != 0 || packFaults2 != 0;
}
