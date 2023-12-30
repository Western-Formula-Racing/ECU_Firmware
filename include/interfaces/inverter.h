#ifndef _INVERTER_
#define _INVERTER_

// Includes
#include "main.h"
#include <format>

class Inverter
{
private:
    // Commmand Variables
    float directionCommand;
    float inverterDischarge = 0; // we don't use the discharge
    float inverterEnable;
    float speedModeEnable = 0;// look at page 32 of the CAN documentation
    float torqueRequest;
    float torqueLimit;

    //command message
    
    FS_CAN::CAN_SIGNAL directionCommandSignal{&directionCommand, 32, 1, true, 1, 0}; // need to figure out how to cast boolean pointer to float
    FS_CAN::CAN_SIGNAL inverterDischargeSignal{&inverterDischarge, 41, 1, true, 1, 0};
    FS_CAN::CAN_SIGNAL inverterEnableSignal{&inverterEnable, 40, 1, true, 1, 0};
    FS_CAN::CAN_SIGNAL speedModeEnableSignal{&speedModeEnable, 42, 1, true, 1, 0};
    FS_CAN::CAN_SIGNAL torqueRequestSignal{&torqueRequest, 0, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL torqueLimitSignal{&torqueLimit, 48, 16, true, 0.1f, 0};
    FS_CAN::CAN_MSG commandMessage{192,{&directionCommandSignal, &inverterDischargeSignal, &inverterEnableSignal, &speedModeEnableSignal, &torqueRequestSignal, &torqueLimitSignal}};

    //fast info message
    FS_CAN::CAN_SIGNAL DCBusVoltageSignal{&dcBusVoltage, 48, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL motorSpeedSignal{&motorSpeed, 32, 16, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL torqueCommandSignal{&commandedTorque, 0, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL torqueFeedbackSignal{&torqueFeedback, 16, 16, true, 0.1f, 0};
    FS_CAN::CAN_MSG fastInfoMessage{176,{&DCBusVoltageSignal, &motorSpeedSignal, &torqueCommandSignal, &torqueFeedbackSignal}};

    //inverter state info
    
    FS_CAN::CAN_SIGNAL enableStateSignal{&enableState, 48, 1, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL runModeSignal{&runMode, 32, 1, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL stateSignal{&state, 16, 8, true, 1.0f, 0};
    FS_CAN::CAN_MSG internalStatesMessage{170, {&enableStateSignal,&runModeSignal, &stateSignal}};

public:
    /***public variables**/

    // Inverter Status Variables:
    float commandedTorque;
    float torqueFeedback;
    float motorSpeed;
    float dcBusVoltage;

    float enableState;
    float runMode;
    float state;
    struct INVERTER_FAULTS
    {
        //@todo fill this in with all the fault states
    };

    enum INVERTER_STATE{
        POWERUP0,
        STOP1,
        OPENLOOP2,
        CLOSEDLOOP3,
        INTERNALSTATE4,
        INTERNALSTATE5,
        INTERNALSTATE6,
        INTERNALSTATE7,
        IDLERUN8,
        IDLESTOP9,
        INTERNALSTATE10,
        INTERNALSTATE11,
        INTERNALSTATE12
    };

    // public function declarations
    Inverter();

    void setTorqueRequest(float torque);
    float getTorqueRequest();
    void enableInverter();
    void disableInverter();
    INVERTER_STATE getInverterState();
};
#endif