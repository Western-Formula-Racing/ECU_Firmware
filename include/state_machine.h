#ifndef _STATE_MACHINE_
#define _STATE_MACHINE_

#include <array>

enum State
{
    START,
    STARTUP_DELAY,
    PRECHARGE_ENABLE,
    DRIVE,
    PEDAL_IMPLAUSIBILITY,
    PRECHARGE_ERROR,
    DEVICE_FAULT,
    LOW_SOC
};

// Creating a type for state function pointers
typedef State (*state_function_t)();

State handle_start();
State handle_startup_delay();
State handle_precharge_enable();
State handle_drive();
State handle_pedal_implausibility();
State handle_precharge_error();
State handle_device_fault();
State handle_low_soc();

#endif