#include "interfaces/inverter.h"

Inverter::Inverter()
{
    torqueRequest = 100.0f;
    inverterEnable = 1.0f;
    commandMessage.signals.push_back(&directionCommandSignal);
    commandMessage.signals.push_back(&inverterDischargeSignal);
    commandMessage.signals.push_back(&inverterEnableSignal);
    commandMessage.signals.push_back(&speedModeEnableSignal);
    commandMessage.signals.push_back(&torqueRequestSignal);
    commandMessage.signals.push_back(&torqueLimitSignal);

    FS_CAN0.publish_CAN_msg(&commandMessage, FS_CAN::TEN_MS);

    
}
