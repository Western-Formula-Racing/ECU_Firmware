#include "interfaces/inverter.h"

Inverter::Inverter(FS_CAN FS_CAN_handle){
    FS_CAN::CAN_MSG commandMessage;
    FS_CAN::CAN_SIGNAL directionCommandSignal;
    FS_CAN::CAN_SIGNAL inverterDischargeSignal;
    FS_CAN::CAN_SIGNAL inverterEnableSignal;
    FS_CAN::CAN_SIGNAL speedModeEnableSignal;
    FS_CAN::CAN_SIGNAL torqueRequestSignal;
    FS_CAN::CAN_SIGNAL torqueLimitSignal; 

}
