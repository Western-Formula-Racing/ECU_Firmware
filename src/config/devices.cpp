#include "config/devices.h"

/*
This file defines the sensors and devices connnected to the ECU
*/
Devices* Devices::s_instance = nullptr;
FS_CAN* Devices::s_fs_can0 = nullptr;

void Devices::Create(FS_CAN* canHandle)
{
    if (s_instance != nullptr) return;

    s_fs_can0 = canHandle;
    s_instance = new Devices();
}





