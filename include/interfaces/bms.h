#ifndef _BMS_
#define _BMS_

// Includes

#include "required_includes.h"
#include "state_machine.h"

extern FS_CAN FS_CAN0;

class BMS
{
private:
    // PREFACE: The BMS uses Moterola byte ordering, which is big endian. This is the opposite of what the inverter uses.
    FS_CAN::CAN_SIGNAL packCurrentSignal{&packCurrent, 8, 16, false, 0.1f, 0};
    FS_CAN::CAN_SIGNAL packInstantaneousVoltageSignal{&packVoltage, 24, 16, false, 0.1f, 0};
    FS_CAN::CAN_SIGNAL packSOCSignal{&packSOC, 32, 8, false, 0.5f, 0};
    FS_CAN::CAN_MSG packStateMessage{1712, {&packCurrentSignal, &packInstantaneousVoltageSignal, &packSOCSignal}};

    FS_CAN::CAN_SIGNAL packFlags1Signal{&packFaults1, 8, 16, false, 1.0f, 0};
    FS_CAN::CAN_SIGNAL packFlags2Signal{&packFaults2, 24, 16, false, 1.0f, 0};
    FS_CAN::CAN_MSG packFaultsMessage{1713, {&packFlags1Signal, &packFlags2Signal}};

    FS_CAN::CAN_SIGNAL BMSFaultSignal{&BMSFault, 0,8, false, 1.0f,0};
    FS_CAN::CAN_MSG VCUFaultMessage{500,{&BMSFaultSignal}};

public:
    // BMS Status Variables:
    float packCurrent; // amps
    float packVoltage; // volts
    float packSOC;     // percent

    // BMS Fault Variables:
    float packFaults1; // bitfield
    float packFaults2; // bitfield
    
    float BMSFault; 

    // public function declarations
    BMS();
    bool hasFault();
};
#endif