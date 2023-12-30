#include "tasks.h"

void setup_task(void *)
{
    BlackBox::begin(100, tskIDLE_PRIORITY + 1);

    // Delay to allow the serial port to be opened and queue to be created (for some reason it needs this)
    vTaskDelay(pdMS_TO_TICKS(5000));
    if (timeStatus()!= timeSet) {
        while(!Serial);
        Serial.printf("my god this sh borked\n");
    }
    xTaskCreate(task1, "task1", 5028, nullptr, tskIDLE_PRIORITY + 2, nullptr);

    vTaskDelete(nullptr);
}

void task1(void *) // mostly just a task for testing 
{

    Inverter inverter = Inverter();

    while (true)
    {
        digitalWriteFast(LED_BUILTIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
        BlackBox::log(LOG_INFO, std::format("DCBus: {:.1f} TorqueCmd: {:.1f}", inverter.dcBusVoltage, inverter.commandedTorque).c_str());
        BlackBox::log(LOG_INFO, std::format("inverter state: {}, run mode {:.1f} enable state {:.1f}", static_cast<int> (inverter.getInverterState()), inverter.runMode, inverter.enableState).c_str());
        digitalWriteFast(LED_BUILTIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void DAQ_task(void *){

}


void VCU_stateMachine(void *){
    
}
