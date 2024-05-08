#ifndef _INVERTER_
#define _INVERTER_

#include "required_includes.h"
#include "state_machine.h"

class Inverter
{
public:
    float inverterEnable ;
    float torqueLimit = TORQUE_LIMIT;

private:
    // Command variables
    float directionCommand=1;
    float inverterDischarge = 0; // we don't use the discharge
    float speedModeEnable = 0; // look at page 32 of the CAN documentation
    float torqueRequest;

    // Command messages

    FS_CAN::CAN_SIGNAL directionCommandSignal{&directionCommand, 32, 1, true, 1, 0}; // need to figure out how to cast boolean pointer to float
    FS_CAN::CAN_SIGNAL inverterDischargeSignal{&inverterDischarge, 41, 1, true, 1, 0};
    FS_CAN::CAN_SIGNAL inverterEnableSignal{&inverterEnable, 40, 1, true, 1, 0};
    FS_CAN::CAN_SIGNAL speedModeEnableSignal{&speedModeEnable, 42, 1, true, 1, 0};
    FS_CAN::CAN_SIGNAL torqueRequestSignal{&torqueRequest, 0, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL torqueLimitSignal{&torqueLimit, 48, 16, true, 0.1f, 0};
    FS_CAN::CAN_MSG commandMessage{192, {&directionCommandSignal, &inverterDischargeSignal, &inverterEnableSignal, &speedModeEnableSignal, &torqueRequestSignal, &torqueLimitSignal}};

    // fast info message
    FS_CAN::CAN_SIGNAL DCBusVoltageSignal{&dcBusVoltage, 48, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL motorSpeedSignal{&motorSpeed, 32, 16, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL torqueCommandSignal{&commandedTorque, 0, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL torqueFeedbackSignal{&torqueFeedback, 16, 16, true, 0.1f, 0};
    FS_CAN::CAN_MSG fastInfoMessage{176, {&DCBusVoltageSignal, &motorSpeedSignal, &torqueCommandSignal, &torqueFeedbackSignal}};

    // Inverter state info
    FS_CAN::CAN_SIGNAL enableStateSignal{&enableState, 48, 1, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL runModeSignal{&runMode, 32, 1, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL stateSignal{&inverterState, 16, 8, true, 1.0f, 0};
    FS_CAN::CAN_MSG internalStatesMessage{170, {&enableStateSignal, &runModeSignal, &stateSignal}};

    //M168_Flux_ID_IQ_Info
    FS_CAN::CAN_SIGNAL iqSignal{&iq, 48, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL idSignal{&id, 32, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL vq_ff_Signal{&vq_ff, 16, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL vd_ff_Signal{&vd_ff, 0, 16, true, 0.1f, 0};
    FS_CAN::CAN_MSG M168_Flux_ID_IQ_Info{168, {&iqSignal, &idSignal, &vq_ff_Signal, &vd_ff_Signal}};

    //M166_Current_Info
    FS_CAN::CAN_SIGNAL INV_DC_Bus_Current_Signal{&INV_DC_Bus_Current, 48, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL INV_Phase_C_Current_Signal{&INV_Phase_C_Current, 32, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL INV_Phase_B_Current_Signal{&INV_Phase_B_Current, 16, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL INV_Phase_A_Current_Signal{&INV_Phase_A_Current, 0, 16, true, 0.1f, 0};
    FS_CAN::CAN_MSG M166_Current_Info{166, {&INV_DC_Bus_Current_Signal, &INV_Phase_C_Current_Signal, &INV_Phase_B_Current_Signal, &INV_Phase_A_Current_Signal}};

    //M162_Temperature_Set_3
    FS_CAN::CAN_SIGNAL INV_Torque_Shudder_Signal{&INV_Torque_Shudder, 48, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL INV_Motor_Temp_Signal{&INV_Motor_Temp, 32, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL INV_Hot_Spot_Temp_Signal{&INV_Hot_Spot_Temp, 16, 16, true, 0.1f, 0};
    FS_CAN::CAN_SIGNAL INV_Coolant_Temp_Signal{&INV_Coolant_Temp, 0, 16, true, 0.1f, 0};
    FS_CAN::CAN_MSG M162_Temperature_Set_3{162, {&INV_Coolant_Temp_Signal, &INV_Hot_Spot_Temp_Signal, &INV_Motor_Temp_Signal, &INV_Torque_Shudder_Signal}};

public:
    // Inverter Status Variables:
    float commandedTorque;
    float torqueFeedback;
    float motorSpeed;
    float dcBusVoltage;

    float enableState;
    float runMode;
    float inverterState;

    //M168_Flux_ID_IQ_Info
    float iq;
    float id;
    float vq_ff;
    float vd_ff;

    //M166_Current_Info
    float INV_DC_Bus_Current;
    float INV_Phase_C_Current;
    float INV_Phase_B_Current;
    float INV_Phase_A_Current;

    //M162_Temperature_Set_3
    float INV_Torque_Shudder;
    float INV_Motor_Temp;
    float INV_Hot_Spot_Temp;
    float INV_Coolant_Temp;
 
    
    struct INVERTER_FAULTS
    {
        //@todo fill this in with all the fault states
    };

    enum INVERTER_STATE
    {
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