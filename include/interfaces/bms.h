#ifndef _BMS_
#define _BMS_

#include "required_includes.h"
#include "state_machine.h"

class BMS
{
private:
    // PREFACE: The BMS uses Motorola byte ordering, which is big endian. This is the opposite of what the inverter uses.
    FS_CAN::CAN_SIGNAL packCurrentSignal{&packCurrent, 0, 8, false, 1.0f, 0};
    FS_CAN::CAN_SIGNAL packInstantaneousVoltageSignal{&packVoltage, 8, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL packSOCSignal{&packSOC, 24, 8, false, 1.0f, 0};
    FS_CAN::CAN_MSG packStateMessage{1712, {&packCurrentSignal, &packInstantaneousVoltageSignal, &packSOCSignal}};

    FS_CAN::CAN_SIGNAL packFlags1Signal{&packFaults1, 8, 16, false, 1.0f, 0};
    FS_CAN::CAN_SIGNAL packFlags2Signal{&packFaults2, 24, 16, false, 1.0f, 0};
    FS_CAN::CAN_MSG packFaultsMessage{1713, {&packFlags1Signal, &packFlags2Signal}};

    FS_CAN::CAN_SIGNAL BMSFaultSignal{&BMSFault, 0, 8, false, 1.0f, 0};
    FS_CAN::CAN_MSG BMSFaultMessage{500, {&BMSFaultSignal}};

public:
    // BMS Status Variables:
    float packCurrent; // amps
    float packVoltage; // volts
    float packSOC;     // percent

    // BMS Fault Variables:
    float packFaults1; // bitfield
    float packFaults2; // bitfield

    float BMSFault;

    BMS();
    bool hasFault();
};
#endif