#ifndef _TASKS_
#define _TASKS_

#include "required_includes.h"
#include "state_machine.h"
#include "interfaces/inverter.h"
#include "config/devices.h"
#include "interfaces/bms.h"
extern Inverter inverter;
extern BMS bms;

void setup_task(void *);
void task1(void *);
void VCU_stateMachine (void *);
void DAQ_task(void *);

#endif