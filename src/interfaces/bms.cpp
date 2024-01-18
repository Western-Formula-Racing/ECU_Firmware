#include "interfaces/bms.h"
#include "config/devices.h"
extern State state;

BMS::BMS()
{
    Devices::GetFS_Can0().subscribe_to_message(&packStateMessage);
    Devices::GetFS_Can0().subscribe_to_message(&packFaultsMessage);
    Devices::GetFS_Can0().publish_CAN_msg(&VCUFaultMessage, FS_CAN::HUNDRED_MS);
}

bool BMS::hasFault()
{
    return packFaults1 != 0 || packFaults2 != 0;
}
