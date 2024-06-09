// This file defines all the global objects as well as the key parameters for the vehicle
#include "config/config.h"

// Constants
const float VREF = 3.3f / 0.615; // reference voltage for all signal measurements (the 0.6 is for the voltage divider)
const float ADC_RES = 10;        // resolution of the Analog to Digital Converter in bits

float APPS1_MIN_VOLTAGE = 2.13;
float APPS1_MAX_VOLTAGE = 0.62;
float APPS2_MIN_VOLTAGE = 2.80;
float APPS2_MAX_VOLTAGE = 4.27;
const float APPS_PLAUSIBILITY_THRESHOLD = 0.35;
const float APPS_DEADZONE = 0.05;
const float BRAKE_LIGHT_THRESHOLD = 0.70;
const float BRAKE_RTD_THRESHOLD = 0.70;
const float BRAKE_PLAUSABILITY_THRESHOLD = 5.0;
const int RTD_TIMER = 2500;
const int PRECHARGE_TIMEOUT = 30000;
const float PRECHARGE_THRESHOLD = 0.83;
const float TORQUE_LIMIT = 230.0;
const float INVERTER_CUTOFF_VOLTAGE = 200.0;
const float GLV_CUTOFF_VOLTAGE = -1.0;