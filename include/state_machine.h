#ifndef _STATEMACHINE_
#define _STATEMACHINE_

#include <array>
#include "interfaces/inverter.h"

enum State { START, STARTUP_DELAY, PRECHARGE_ENABLE, DRIVE, PEDAL_IMPLAUSABILITY, PRECHARGE_ERROR, DEVICE_FAULT, LOW_SOC};
typedef State (*state_function_t)(); // creating a type for state function pointers
State handle_start();
State handle_startup_delay();
State handle_precharge_enable();
State handle_drive();
State handle_pedal_implausability();
State handle_precharge_error();
State handle_device_fault();
State handle_low_soc();





#endif