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
    uint32_t testData = 0;
    uint32_t testData2 = 0;
    FS_CAN::CAN_MSG testMsg;
    testMsg.id = 256;
    testMsg.signal_count = 1;
    FS_CAN::CAN_SIGNAL testSignal{&testData, 0, 16, false};
    FS_CAN::CAN_SIGNAL testSignal2{&testData2, 8, 8, false};
    testMsg.signals.push_back(&testSignal);
    testMsg.signals.push_back(&testSignal2);
    Serial.printf("didnt' crash yet\n");
    FS_CAN0.subscribe_to_message(&testMsg);

    while (true)
    {
        digitalWriteFast(LED_BUILTIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
        BlackBox::log(LOG_INFO, std::format("data1: {} data2: {}", testData, testData2).c_str());
        digitalWriteFast(LED_BUILTIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void DAQ_task(void *){

}


void VCU_stateMachine(void *){
    
}
