#include "interfaces/bms.h"
#include "config/devices.h"

extern State state;
extern FS_CAN FS_CAN0;

BMS::BMS()
{
    Serial.println("BMS constructor called");
    FS_CAN0.subscribe_to_message(&packStateMessage);
    FS_CAN0.subscribe_to_message(&packFaultsMessage);
    FS_CAN0.publish_CAN_msg(&VCUFaultMessage, FS_CAN::HUNDRED_MS);
}

bool BMS::hasFault()
{
    return packFaults1 != 0 || packFaults2 != 0;
}
