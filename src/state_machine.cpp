#include "state_machine.h"
#include "config/devices.h"
State state = START;


std::array<state_function_t, 8> states = {
    handle_start,
    handle_startup_delay,
    handle_precharge_enable,
    handle_drive,
    handle_pedal_implausibility,
    handle_precharge_error,
    handle_device_fault,
    handle_low_soc,
};
int startTime; 


State handle_start()
{
    State nextState = START; 
    if (Devices::Get().GetRTDButton().value == 1 and Devices::Get().GetPedal().getFrontBreakPressure() > BRAKE_THRESHOLD){
        nextState = STARTUP_DELAY;
        startTime = millis();
    }
    return nextState;
}
State handle_startup_delay()
{
    State nextState = STARTUP_DELAY;
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
    
    return nextState;
}
State handle_precharge_enable()
{
    State nextState = PRECHARGE_ERROR;
    //@todo turn on precharge-enable lol
    int currentTime = millis();
    if (Devices::Get().GetInverter().dcBusVoltage < MIN_PRECHARGE_VOLTAGE and (currentTime-startTime) < PRECHARGE_TIMEOUT){
        nextState = PRECHARGE_ENABLE;
    }
    else if(Devices::Get().GetInverter().dcBusVoltage >= MIN_PRECHARGE_VOLTAGE and (currentTime-startTime) < PRECHARGE_TIMEOUT){
        nextState = DRIVE;
    }
    return nextState;

}
State handle_drive()
{
    Devices::Get().GetInverter().setTorqueRequest(Devices::Get().GetPedal().getPedalPosition() * 200);
    return DRIVE;
}
State handle_pedal_implausibility()
{
    return START;
}
State handle_precharge_error()
{
    return PRECHARGE_ERROR;
}
State handle_device_fault()
{
    return DEVICE_FAULT;
}
State handle_low_soc()
{
    return START;
}