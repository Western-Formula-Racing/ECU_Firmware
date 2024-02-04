#include "tasks.h"
#include "config/devices.h"
extern State state;
extern std::array<state_function_t, 8> states;
float stateS;
float rtdButton;
extern FS_CAN FS_CAN0;
FS_CAN::CAN_SIGNAL stateSignal{&stateS,8,8,true,1,0};
FS_CAN::CAN_SIGNAL rtdButtonSignal{&rtdButton,0,1,true,1,0};
FS_CAN::CAN_MSG VCU_StateInfo{2002, {&stateSignal,&rtdButtonSignal}};

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
    FS_CAN0.publish_CAN_msg(&VCU_StateInfo, FS_CAN::TEN_MS);
    xTaskCreate(task1, "task1", 5028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
    xTaskCreate(VCU_stateMachine, "VCU_stateMachine", 1028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
    vTaskDelete(nullptr);
}

void task1(void *) // mostly just a task for testing
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
            // Serial.printf("sensor %d\n", i);
            // Serial.print(sensor->rawValue);
            // Serial.print(" ");
            // Serial.print(sensor->value);
            // Serial.print(" ");
            // Serial.print(sensor->filteredValue);
            // Serial.println();
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
        vTaskDelay(pdMS_TO_TICKS(10));
        Devices::Get().GetPDM().setPin(HSDIN1, HIGH);
        digitalWriteFast(LED_BUILTIN, LOW);
    }
}
void DAQ_task(void *)
{
}

void VCU_stateMachine(void *)
{
    state = START;
    while (true)
    {   
        stateS = static_cast<float>(state);
        state = states[state]();
        // BlackBox::log(LOG_INFO, std::format("currentState: {}", static_cast<int>(state)).c_str());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
