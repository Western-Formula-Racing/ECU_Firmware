#include "state_machine.h"

State state = START;
extern Inverter inverter;
std::array<state_function_t, 8> states = {
    handle_start,
    handle_startup_delay,
    handle_precharge_enable,
    handle_drive,
    handle_pedal_implausability,
    handle_precharge_error,
    handle_device_fault,
    handle_low_soc,
};



State handle_start(){
    inverter.setTorqueRequest(10.0f);
    return START;
}
State handle_startup_delay(){
    return START;
}
State handle_precharge_enable(){
    return START;
}
State handle_drive(){
    inverter.setTorqueRequest(10.0f);
    return DRIVE;
}
State handle_pedal_implausability(){
    return START;
}
State handle_precharge_error(){
    return START;
}
State handle_device_fault(){
    return START;
}
State handle_low_soc(){
    return START;
}