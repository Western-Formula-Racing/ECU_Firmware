#include "tasks.h"
#include "config/devices.h"
extern State state;
extern std::array<state_function_t, 8> states;

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
    xTaskCreate(task1, "task1", 5028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
    xTaskCreate(VCU_stateMachine, "VCU_stateMachine", 1028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
    vTaskDelete(nullptr);
}

void task1(void *) // mostly just a task for testing
{

    while (true)
    {
        digitalWriteFast(LED_BUILTIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(50));
        // BlackBox::log(LOG_INFO, std::format("DCBus: {:.1f} TorqueCmd: {:.1f}, requested torque: {:.1f}", inverter.dcBusVoltage, inverter.commandedTorque, inverter.getTorqueRequest()).c_str());
        // BlackBox::log(LOG_INFO, std::format("inverter state: {}, run mode {:.1f} enable state {:.1f}", static_cast<int> (inverter.getInverterState()), inverter.runMode, inverter.enableState).c_str());
        int i = 0;
        for (auto *sensor : Devices::Get().GetSensors())
        {
            Serial.printf("sensor %d\n", i);
            sensor->read();
            Serial.print(sensor->rawValue);
            Serial.print(" ");
            Serial.print(sensor->value);
            Serial.print(" ");
            Serial.print(sensor->filteredValue);
            Serial.println();
            i++;
        }
        float pedalPos = Devices::Get().GetPedal().getPedalPosition();
        Serial.printf("pedal postion: %f\n", pedalPos);
        Serial.printf("sensor1: %f sensor2: %f\n", Devices::Get().GetPedal().sensor1Position, Devices::Get().GetPedal().sensor2Position);
        vTaskDelay(pdMS_TO_TICKS(50));
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
        state = states[state]();
        BlackBox::log(LOG_INFO, std::format("currentState: {}", static_cast<int>(state)).c_str());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
