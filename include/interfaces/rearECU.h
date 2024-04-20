#ifndef _REARECU_
#define _REARECU_

#include "required_includes.h"
#include "state_machine.h"

class rearECU
{
private:
    FS_CAN::CAN_SIGNAL HSD1_Enable_Signal{&HSDEnable[0], 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD2_Enable_Signal{&HSDEnable[1], 8, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD3_Enable_Signal{&HSDEnable[2], 16, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD4_Enable_Signal{&HSDEnable[3], 24, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD5_Enable_Signal{&HSDEnable[4], 32, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD6_Enable_Signal{&HSDEnable[5], 40, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD7_Enable_Signal{&HSDEnable[6], 48, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD8_Enable_Signal{&HSDEnable[7], 56, 8, true, 1.0f, 0};
    FS_CAN::CAN_MSG VCU_rearECU_command{2012, 
    {
    &HSD1_Enable_Signal,
    &HSD2_Enable_Signal,
    &HSD3_Enable_Signal,
    &HSD4_Enable_Signal,
    &HSD5_Enable_Signal,
    &HSD6_Enable_Signal,
    &HSD7_Enable_Signal,
    &HSD7_Enable_Signal,
    }};

public:
    rearECU();
    float HSDEnable[8];
    void setHSD(int HSD, bool enable);
};
#endif