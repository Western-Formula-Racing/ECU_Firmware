#include "state_machine.h"
#include "config/devices.h"
#include "config/config.h"
State state = START;
extern float precharge_enable;
extern float precharge_ok;
int hornTimer = 10;
int hornTicker = hornTimer; 
bool hornToggle = false;

std::array<state_function_t, 8> states = {
    handle_start,
    handle_precharge_enable,
    handle_precharge_ok,
    handle_startup_delay,
    handle_drive,
    handle_precharge_error,
    handle_device_fault,
};
int startTime;

State handle_start()
{
    State nextState = START;
    Serial.println("Startup!");
    precharge_enable = 0;
    precharge_ok = 0;
    // inverter command message must be all zeros at least once to "enable" the inverter
    Devices::Get().GetInverter().inverterEnable = 0;
    Devices::Get().GetInverter().torqueLimit = 0;
    Devices::Get().GetInverter().setTorqueRequest(0);

    //@todo add a device check once CAN timeouts are introduced
    nextState = PRECHARGE_ENABLE;
    startTime = millis();
    return nextState;
}

State handle_precharge_enable()
{
    State nextState = PRECHARGE_ERROR;
    precharge_enable = 1;
    precharge_ok = 0;
    Devices::Get().GetInverter().setTorqueRequest(0);
    int currentTime = millis();
    if (Devices::Get().GetInverter().dcBusVoltage >=  max( (Devices::Get().GetBMS().packVoltage * PRECHARGE_THRESHOLD),INVERTER_CUTOFF_VOLTAGE))
    {
        Serial.println("inverter voltage met");
        nextState = PRECHARGE_OK;
    }
    else if (Devices::Get().GetInverter().dcBusVoltage < max( (Devices::Get().GetBMS().packVoltage * PRECHARGE_THRESHOLD),INVERTER_CUTOFF_VOLTAGE))
    {
        //Serial.println("waiting for inverter voltage");
        nextState = PRECHARGE_ENABLE;
    }
    else
    {
        Serial.println("Precharge Timed out! Please Power Cycle");
        nextState = PRECHARGE_ERROR;
    }
    return nextState;
}

State handle_precharge_ok()
{
    State nextState = PRECHARGE_OK;
    precharge_enable = 1;
    precharge_ok = 1;
    Devices::Get().GetPDM().setPin(HSDIN4,0);
    if (Devices::Get().GetRTDButton().value == 1 and Devices::Get().GetPedal().getFrontBreakPressure() > BRAKE_RTD_THRESHOLD)
    {
        nextState = STARTUP_DELAY;
        startTime = millis();
    }
    if (Devices::Get().GetInverter().dcBusVoltage < (Devices::Get().GetBMS().packVoltage * PRECHARGE_THRESHOLD))
    {
        Serial.println("inverter voltage lost unexpectedly");
        nextState = PRECHARGE_ERROR;
    }
    if(Devices::Get().GetInverter().INV_glv_voltage <= GLV_CUTOFF_VOLTAGE){
        BlackBox::log(LOG_ERROR, "GLV Voltage too low, undervoltage lockout");
        nextState = DEVICE_FAULT;
    }
    return nextState;
}

State handle_startup_delay()
{
    State nextState = STARTUP_DELAY;
    Devices::Get().GetInverter().setTorqueRequest(0);
    int currentTime = millis();
    hornTicker++;
    Serial.printf(">hornTicker:%d\n", hornTicker);
    Serial.printf(">hornToggle:%d\n", hornToggle);
    if (hornTicker >= hornTimer){
        hornTicker = 0;
        hornToggle = !hornToggle;
        Devices::Get().GetPDM().setPin(HSDIN4,hornToggle);
    }
    if (Devices::Get().GetRTDButton().value == 1 and Devices::Get().GetPedal().getFrontBreakPressure() > BRAKE_RTD_THRESHOLD and (currentTime - startTime) < RTD_TIMER)
    {
        nextState = STARTUP_DELAY;
    }
    else if (Devices::Get().GetRTDButton().value == 1 and Devices::Get().GetPedal().getFrontBreakPressure() > BRAKE_RTD_THRESHOLD and (currentTime - startTime) >= RTD_TIMER)
    {
        nextState = DRIVE;
        startTime = millis();
    }
    else
    {
        nextState = PRECHARGE_OK;
    }
    return nextState;
}

State handle_drive()
{
    Devices::Get().GetInverter().inverterEnable = 1;
    Devices::Get().GetInverter().torqueLimit = TORQUE_LIMIT;
    Devices::Get().GetPDM().setPin(HSDIN4,0);
    State nextState = START;

    float throttle = 0;
    if (Devices::Get().GetPedal().getPedalPosition() > APPS_DEADZONE)
    {
        throttle = Devices::Get().GetPedal().getPedalPosition();
    }

    if (Devices::Get().GetInverter().dcBusVoltage >= INVERTER_CUTOFF_VOLTAGE)
    {
        Devices::Get().GetInverter().setTorqueRequest(throttle * TORQUE_LIMIT);
        nextState = DRIVE;
    }
    if(Devices::Get().GetInverter().INV_glv_voltage <= GLV_CUTOFF_VOLTAGE){
        BlackBox::log(LOG_ERROR, "GLV Voltage too low, undervoltage lockout");
        nextState = DEVICE_FAULT;
    }
    return nextState;
}
State handle_precharge_error()
{
    precharge_enable = 0;
    precharge_ok = 0;
    Devices::Get().GetInverter().setTorqueRequest(0);
    vTaskDelay(pdMS_TO_TICKS(1000));
    return PRECHARGE_ERROR;
}
State handle_device_fault()
{
    precharge_enable = 0;
    precharge_ok = 0;
    Devices::Get().GetInverter().setTorqueRequest(0);
    vTaskDelay(pdMS_TO_TICKS(1000));
    return DEVICE_FAULT;
}