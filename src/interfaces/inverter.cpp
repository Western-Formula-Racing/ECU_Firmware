#include "interfaces/inverter.h"
extern State state;
Inverter::Inverter()
{

    FS_CAN0.publish_CAN_msg(&commandMessage, FS_CAN::TEN_MS);
    FS_CAN0.subscribe_to_message(&fastInfoMessage);
    FS_CAN0.subscribe_to_message(&internalStatesMessage);


}

void Inverter::setTorqueRequest(float torque){
    if (state == DRIVE){
        torqueRequest = torque;
    }
    else{
        torqueRequest = 0;
    }
}

float Inverter::getTorqueRequest(){
    return torqueRequest;
}

void  Inverter::enableInverter(){
    inverterEnable = 1.0f;
}

void Inverter::disableInverter(){
    inverterEnable = 0.0f;
}

Inverter::INVERTER_STATE Inverter::getInverterState(){
    return static_cast<Inverter::INVERTER_STATE>(inverterState);
}
