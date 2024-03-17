#include "state_machine.h"
#include "config/devices.h"
State state = START;
extern float precharge_enable;
extern float precharge_ok;
std::array<state_function_t, 8> states = {
    handle_start,
    handle_startup_delay,
    handle_precharge_enable,
    handle_drive,
    handle_precharge_error,
    handle_device_fault,
    handle_low_soc,
};
int startTime; 


State handle_start()
{
    State nextState = START; 
    precharge_enable = 0;
    precharge_ok = 0;
    if (Devices::Get().GetRTDButton().value == 1 and Devices::Get().GetPedal().getFrontBreakPressure() > BRAKE_THRESHOLD){
        nextState = STARTUP_DELAY;
        startTime = millis();
    }
    return nextState;
}
State handle_startup_delay()
{
    State nextState = STARTUP_DELAY;
    precharge_enable = 0;
    precharge_ok = 0;
    int currentTime = millis();
    if (Devices::Get().GetRTDButton().value == 1 and Devices::Get().GetPedal().getFrontBreakPressure() > BRAKE_THRESHOLD and (currentTime-startTime) < RTD_TIMER ){
        nextState = STARTUP_DELAY;
    }
    else if (Devices::Get().GetRTDButton().value == 1 and Devices::Get().GetPedal().getFrontBreakPressure() > BRAKE_THRESHOLD and (currentTime-startTime) >= RTD_TIMER ){
        nextState = PRECHARGE_ENABLE;
        startTime = millis();
    }
    else{
        nextState = START;
    }
    Serial.println("In dealay");
    return nextState;
}
State handle_precharge_enable()
{
    State nextState = PRECHARGE_ERROR;
    precharge_enable = 1;
    precharge_ok = 0;
    int currentTime = millis();
    if (Devices::Get().GetInverter().dcBusVoltage < Devices::Get().GetBMS().packVoltage*0.0 and (currentTime-startTime) < PRECHARGE_TIMEOUT){ //CHANGE BACK BEFORE HV
        Serial.println("waiting for inverter voltage");
        nextState = PRECHARGE_ENABLE;
    }
    else if(Devices::Get().GetInverter().dcBusVoltage >= (Devices::Get().GetBMS().packVoltage*0.0) and (currentTime-startTime) < PRECHARGE_TIMEOUT){
        Serial.println("inverter voltage met");
        nextState = DRIVE;
    }
    return nextState;
    Serial.println("In enable");

}
State handle_drive()
{
    Serial.println("In Drive");
    State nextState = START;
    precharge_enable = 1;
    precharge_ok = 1;
    if(Devices::Get().GetInverter().dcBusVoltage >= 200.0*0){ //CHANGE BEFORE HV
        Devices::Get().GetInverter().setTorqueRequest(Devices::Get().GetPedal().getPedalPosition() * 200);
        nextState = DRIVE;
    }
    return nextState;
}
State handle_precharge_error()
{
    precharge_enable = 0;
    precharge_ok = 0;
    BlackBox::log(LOG_ERROR, "precharge error");
    vTaskDelay(pdMS_TO_TICKS(1000));
    return START;
}
State handle_device_fault()
{
    precharge_enable = 0;
    precharge_ok = 0;
    return DEVICE_FAULT;
}
State handle_low_soc()
{
    precharge_enable = 0;
    precharge_ok = 0;
    return START;
}