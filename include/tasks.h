#ifndef _TASKS_
#define _TASKS_

#include "required_includes.h"
#include "state_machine.h"
#include "interfaces/pdm.h"

void setup_task(void *);
void frontDAQ(void *);
void VCU_stateMachine(void *);
void rearECU_task(void *);

#endif