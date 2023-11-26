#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <format>
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "TeensyDebug.h"
#include "FS_CAN.h"
#include "interfaces/inverter.h"
#include "interfaces/bms.h"
#include "black_box.h"

#pragma GCC optimize("O0")

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;
FS_CAN FS_CAN0(&Can0);
time_t getTeensy3Time();

void can_sniff(const CAN_message_t &msg)
{
    FS_CAN0.CAN_RX_ISR(msg);
}

static void task1(void *)
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

void setup_task(void *)
{
    Can0.begin();
    Can0.setBaudRate(500000);
    Can0.setMaxMB(64);
    Can0.enableFIFO();
    Can0.enableMBInterrupt(FIFO);
    Can0.enableFIFOInterrupt();
    Can0.onReceive(can_sniff);

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

void setup()
{
    setSyncProvider(getTeensy3Time);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWriteFast(LED_BUILTIN, HIGH);

    if (CrashReport)
    {
        Serial2.print(CrashReport);
        Serial2.println();
        Serial2.flush();
    }

    Serial.println(PSTR("\r\nBooting FreeRTOS kernel " tskKERNEL_VERSION_NUMBER ". Built by gcc " __VERSION__ " (newlib " _NEWLIB_VERSION ") on " __DATE__ ". ***\r\n"));

    xTaskCreate(setup_task, "setup_task", 1024, nullptr, tskIDLE_PRIORITY + 1, nullptr);

    Serial.println("setup(): starting scheduler...");
    Serial.flush();

    vTaskStartScheduler();
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get()+ 3600*5+1;//shift time forward 5 hours and 1s because that's needed for some reason
}


void loop() {}