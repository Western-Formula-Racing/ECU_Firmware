#include "interfaces/bms.h"

extern State state;

BMS::BMS()
{
    FS_CAN0.subscribe_to_message(&packStateMessage);
    FS_CAN0.subscribe_to_message(&packFaultsMessage);
    FS_CAN0.publish_CAN_msg(&VCUFaultMessage, FS_CAN::HUNDRED_MS);
}

bool BMS::hasFault()
{
    return packFaults1 != 0 || packFaults2 != 0;
}
