#ifndef _REARECU_
#define _REARECU_

#include "required_includes.h"
#include "state_machine.h"

class rearECU
{
private:
    FS_CAN::CAN_SIGNAL HSD1_Enable_Signal{&HSD1_Enable, 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD2_Enable_Signal{&HSD2_Enable, 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD3_Enable_Signal{&HSD3_Enable, 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD4_Enable_Signal{&HSD4_Enable, 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD5_Enable_Signal{&HSD5_Enable, 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD6_Enable_Signal{&HSD6_Enable, 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD7_Enable_Signal{&HSD7_Enable, 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD8_Enable_Signal{&HSD8_Enable, 0, 8, true, 1.0f, 0};
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
    float HSD1_Enable;
    float HSD2_Enable;
    float HSD3_Enable;
    float HSD4_Enable;
    float HSD5_Enable;
    float HSD6_Enable;
    float HSD7_Enable;
    float HSD8_Enable;
    void setHSD(int HSD, bool enable);
};
#endif