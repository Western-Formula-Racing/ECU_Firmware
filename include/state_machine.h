#ifndef _STATE_MACHINE_
#define _STATE_MACHINE_

#include <array>
#include "black_box.h"
enum State
{
    START,
    PRECHARGE_ENABLE,
    PRECHARGE_OK,
    STARTUP_DELAY,
    DRIVE,
    PRECHARGE_ERROR,
    DEVICE_FAULT,
};

// Creating a type for state function pointers
typedef State (*state_function_t)();

State handle_start();
State handle_precharge_enable();
State handle_precharge_ok();
State handle_startup_delay();
State handle_drive();
State handle_precharge_error();
State handle_device_fault();


#endif