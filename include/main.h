#ifndef _MAIN_
#define _MAIN_

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <format>
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "CAN/FS_CAN.h"
#include "interfaces/inverter.h"
#include "interfaces/bms.h"
#include "black_box.h"
#include "tasks.h"


//objects and handles
extern FS_CAN FS_CAN0;




#endif