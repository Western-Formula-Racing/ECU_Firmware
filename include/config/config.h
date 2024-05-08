#include <array>
#ifndef _CONFIG_
#define _CONFIG_

extern const float VREF;    // reference voltage for all signal measurements (5v on final ECU)
extern const float ADC_RES; // resolution of the Analog to Digital Converter
extern float APPS1_MIN_VOLTAGE;
extern float APPS1_MAX_VOLTAGE;
extern float APPS2_MIN_VOLTAGE;
extern float APPS2_MAX_VOLTAGE;
extern const float APPS_PLAUSIBILITY_THRESHOLD;
extern const float APPS_DEADZONE;
extern const float BRAKE_LIGHT_THRESHOLD;
extern const float BRAKE_RTD_THRESHOLD;
extern const float BRAKE_PLAUSABILITY_THRESHOLD;
extern const int RTD_TIMER;
extern const int PRECHARGE_TIMEOUT;
extern const float PRECHARGE_THRESHOLD;
extern const float TORQUE_LIMIT;
extern const float INVERTER_CUTOFF_VOLTAGE;
extern const float GLV_CUTOFF_VOLTAGE;

#endif