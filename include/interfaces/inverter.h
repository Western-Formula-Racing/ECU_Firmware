#ifndef _INVERTER_
#define _INVERTER_

// Includes
#include "main.h"

class Inverter
{
private:
    // Commmand Variables
    uint16_t torqueRequest;
    uint8_t directionCommand;
    uint8_t modeSelectByte; // look at page 32 of the CAN documentation
    uint8_t torqueLimit;

public:
    /***public variables**/

    // Inverter Status Variables:
    uint16_t commandedTorque;
    uint16_t torqueFeedback;
    uint16_t motorSpeed;
    uint16_t dcBusVoltage;

    struct INVERTER_FAULTS
    {
        //@todo fill this in with all the fault states
    };

    // public function declarations
    Inverter(FS_CAN FS_CAN_handle);

    void setTorqueRequest(uint16_t torque);
};
#endif