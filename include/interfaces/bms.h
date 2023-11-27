#ifndef _BMS_
#define _BMS_

// Includes
#include "main.h"

class BMS
{
private:
    // Commmand Variables

public:
    /***public variables**/

    // BMS Status Variables:
    uint16_t packVoltage;

    struct BMS_FAULTS
    {
        //@todo fill this in with all the fault states
    };

    // public function declarations
    BMS(FS_CAN FS_CAN_handle);
};
#endif