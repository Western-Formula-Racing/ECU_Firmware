#include "interfaces/rearECU.h"
#include "config/devices.h"

extern State state;
extern FS_CAN dataCAN;
extern FS_CAN controlCAN;



rearECU::rearECU()
{
    Serial.println("RearECU constructor called");
    #ifndef REAR
    controlCAN.publish_CAN_msg(&VCU_rearECU_command, FS_CAN::HUNDRED_MS);
    #endif
}

void rearECU::setHSD(int HSD, bool enable)
{
    HSDEnable[HSD] = static_cast<float>(enable);
}
