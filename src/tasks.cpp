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
    float testData = 0;
    float testData2 = 0;
    FS_CAN::CAN_MSG testMsg;
    FS_CAN::CAN_MSG testMsgTx;
    testMsg.id = 256;
    testMsgTx.id = 200;
    testMsg.signal_count = 1;
    FS_CAN::CAN_SIGNAL testSignal{&testData, 0, 16, false,1,0};
    FS_CAN::CAN_SIGNAL testSignal2{&testData2, 8, 8, false,1,0};
    FS_CAN::CAN_SIGNAL testSignal3{&testData, 0, 16, false,1,0};
    testMsg.signals.push_back(&testSignal);
    testMsg.signals.push_back(&testSignal2);
    testMsgTx.signals.push_back(&testSignal3);
    FS_CAN0.publish_CAN_msg(&testMsgTx, FS_CAN::HUNDRED_MS);
    Serial.printf("didnt' crash yet\n");
    FS_CAN0.subscribe_to_message(&testMsg);
    Inverter inverter = Inverter();

    while (true)
    {
        digitalWriteFast(LED_BUILTIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
        BlackBox::log(LOG_INFO, std::format("data1: {:.1f} data2: {:.1f}", testData, testData2).c_str());
        BlackBox::log(LOG_INFO, std::format("DCBus: {:.1f} TorqueCmd: {:.1f}", inverter.dcBusVoltage, inverter.commandedTorque).c_str());
        digitalWriteFast(LED_BUILTIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void DAQ_task(void *){

}


void VCU_stateMachine(void *){
    
}
