#include "interfaces/inverter.h"

Inverter::Inverter()
{

    FS_CAN0.publish_CAN_msg(&commandMessage, FS_CAN::TEN_MS);
    FS_CAN0.subscribe_to_message(&fastInfoMessage);
    FS_CAN0.subscribe_to_message(&internalStatesMessage);


}

void Inverter::setTorqueRequest(float torque){
    //it might be nice if there is an easy way to check the 
    //global state to prevent this from being called outside specific states
    torqueRequest = torque;
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
