#include "tasks.h"
#include "config/devices.h"
extern State state;
extern std::array<state_function_t, 8> states;
float stateS;
float rtdButton;
extern FS_CAN dataCAN;
extern FS_CAN controlCAN;
float amsOK = 0;
float imdOK = 0;
float precharge_enable = 0;
float precharge_ok = 0;
FS_CAN::CAN_SIGNAL precharge_enable_signal{&precharge_enable, 0, 1, true, 1, 0};
FS_CAN::CAN_SIGNAL precharge_ok_signal{&precharge_ok, 1, 1, true, 1, 0};
FS_CAN::CAN_MSG VCU_Precharge{2003, {&precharge_enable_signal, &precharge_ok_signal}};

#ifndef REAR
FS_CAN::CAN_SIGNAL stateSignal{&stateS, 8, 8, true, 1, 0};
FS_CAN::CAN_SIGNAL rtdButtonSignal{&rtdButton, 0, 1, true, 1, 0};
FS_CAN::CAN_MSG VCU_StateInfo{2002, {&stateSignal, &rtdButtonSignal}};

FS_CAN::CAN_SIGNAL amsOKSignal{&amsOK, 40, 8, true, 1, 0};
FS_CAN::CAN_SIGNAL imdOKSignal{&imdOK, 48, 8, true, 1, 0};
FS_CAN::CAN_MSG AccMB_Info{2010, {&amsOKSignal, &imdOKSignal}};
#endif

void setup_task(void *)
{
    BlackBox::begin(100, tskIDLE_PRIORITY + 1);

    // Delay to allow the serial port to be opened and queue to be created (for some reason it needs this)
    vTaskDelay(pdMS_TO_TICKS(5000));

    if (timeStatus() != timeSet)
    {
        while (!Serial)
            ;
        Serial.printf("my god this sh borked\n");
    }

    xTaskCreate(CLI_Tool::task, "CLI_Task", 2048, nullptr, tskIDLE_PRIORITY + 1, nullptr);

#ifndef REAR
    controlCAN.publish_CAN_msg(&VCU_StateInfo, FS_CAN::HUNDRED_MS);
    controlCAN.publish_CAN_msg(&VCU_Precharge, FS_CAN::HUNDRED_MS);
    controlCAN.subscribe_to_message(&AccMB_Info);
    xTaskCreate(frontDAQ, "frontDAQ", 5028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
    xTaskCreate(VCU_stateMachine, "VCU_stateMachine", 1028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
#endif
#ifdef REAR
    controlCAN.subscribe_to_message(&VCU_Precharge);
    xTaskCreate(rearECU_task, "rearECU_task", 5028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
#endif
    vTaskDelete(nullptr);
}

void frontDAQ(void *)
{
    while (true)
    {
        digitalWriteFast(LED_BUILTIN, HIGH);
        // BlackBox::log(LOG_INFO, std::format("DCBus: {:.1f} TorqueCmd: {:.1f}, requested torque: {:.1f}", inverter.dcBusVoltage, inverter.commandedTorque, inverter.getTorqueRequest()).c_str());
        // BlackBox::log(LOG_INFO, std::format("inverter state: {}, run mode {:.1f} enable state {:.1f}", static_cast<int> (inverter.getInverterState()), inverter.runMode, inverter.enableState).c_str());
        int i = 0;
        for (auto *sensor : Devices::Get().GetSensors())
        {
            sensor->read();
            Serial.printf(">sensor%draw:", i);
            Serial.print(sensor->voltage);
            Serial.println();
            i++;
        }
        Devices::Get().GetRTDButton().read();
        float pedalPos = Devices::Get().GetPedal().getPedalPosition();
        Serial.printf(">pedal_postion:%f\n", pedalPos);
        Serial.printf(">sensor1:%f\n>sensor2:%f\n", Devices::Get().GetPedal().appsSensor1Position, Devices::Get().GetPedal().appsSensor2Position);
        Serial.printf(">RTDButton:%f\n", Devices::Get().GetRTDButton().value);
        rtdButton = static_cast<float>(Devices::Get().GetRTDButton().value);
        Serial.printf(">brake:%f\n", Devices::Get().GetPedal().getFrontBreakPressure());
        Serial.printf(">state:%d\n", static_cast<int>(state));
        Serial.printf(">torqueRequest:%f\n", Devices::Get().GetInverter().getTorqueRequest());
        Serial.printf(">MotorSpeed:%f\n", Devices::Get().GetInverter().motorSpeed);
        Serial.printf(">InvEnable:%f\n", Devices::Get().GetInverter().inverterEnable);
        Serial.printf(">TorqueLimit:%f\n", Devices::Get().GetInverter().torqueLimit);
        Serial.printf(">InvState:%f\n", Devices::Get().GetInverter().enableState);
        Serial.printf(">packVoltage:%f\n", Devices::Get().GetBMS().packVoltage);
        Serial.printf(">inverterVoltage:%f\n", Devices::Get().GetInverter().dcBusVoltage);
        Serial.printf(">precharge thresh:%f\n", Devices::Get().GetBMS().packVoltage * 0.90);
        vTaskDelay(pdMS_TO_TICKS(100));
        digitalWriteFast(LED_BUILTIN, LOW);
    }
}
void rearECU_task(void *)
{
    while (1)
    {
        int i = 0;
        for (auto *sensor : Devices::Get().GetSensors())
        {
            sensor->read();
            i++;
        }
        Devices::Get().GetPDM().setPin(HSDIN2, precharge_enable);
        Devices::Get().GetPDM().setPin(HSDIN3, precharge_ok);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void VCU_stateMachine(void *)
{
    state = START;
    while (true)
    {
        stateS = static_cast<float>(state);
        state = states[state]();
        Devices::Get().GetPDM().setPin(HSDIN7, !amsOK); // first light 
        Devices::Get().GetPDM().setPin(HSDIN5, !imdOK); // second light
        Serial.printf(">amsOK:%.0f\n>imdOK:%.0f\n", amsOK, imdOK);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
