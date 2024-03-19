#include "interfaces/bms.h"
#include "config/devices.h"

extern State state;
extern FS_CAN dataCAN;

BMS::BMS()
{
    Serial.println("BMS constructor called");
    dataCAN.subscribe_to_message(&packStateMessage);
    dataCAN.subscribe_to_message(&packFaultsMessage);
    #ifndef REAR
    dataCAN.publish_CAN_msg(&BMSFaultMessage, FS_CAN::THOUSAND_MS);
    #endif
}

bool BMS::hasFault()
{
    return packFaults1 != 0 || packFaults2 != 0;
}
