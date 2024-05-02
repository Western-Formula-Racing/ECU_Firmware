#include "interfaces/inverter.h"
#include "config/devices.h"
extern State state;
extern FS_CAN dataCAN;
extern FS_CAN controlCAN;
Inverter::Inverter()
{
    Serial.println("Inverter Constructor called");
    #ifndef REAR
    controlCAN.publish_CAN_msg(&commandMessage, FS_CAN::TEN_MS);
    #endif
    controlCAN.subscribe_to_message(&fastInfoMessage);
    controlCAN.subscribe_to_message(&internalStatesMessage);
    controlCAN.subscribe_to_message(&M168_Flux_ID_IQ_Info);
    controlCAN.subscribe_to_message(&M166_Current_Info);
    controlCAN.subscribe_to_message(&M162_Temperature_Set_3);

}

void Inverter::setTorqueRequest(float torque)
{
    if (state == DRIVE)
    {
        torqueRequest = torque;
    }
    else
    {
        torqueRequest = 0;
    }
}

float Inverter::getTorqueRequest()
{
    return torqueRequest;
}

void Inverter::enableInverter()
{
    inverterEnable = 1.0f;
}

void Inverter::disableInverter()
{
    inverterEnable = 0.0f;
}

Inverter::INVERTER_STATE Inverter::getInverterState()
{
    return static_cast<Inverter::INVERTER_STATE>(inverterState);
}
