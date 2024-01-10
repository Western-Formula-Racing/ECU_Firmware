#include "interfaces/bms.h"

extern State state;

BMS::BMS()
{
    FS_CAN0.subscribe_to_message(&packStateMessage);
    FS_CAN0.subscribe_to_message(&packFaultsMessage);
}

bool BMS::hasFault()
{
    return packFaults1 != 0 || packFaults2 != 0;
}
