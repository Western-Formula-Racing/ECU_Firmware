#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "TeensyDebug.h"
#include "FS_CAN.h"
#include "interfaces/inverter.h"
#include "interfaces/bms.h"
#include "main.h"
#pragma GCC optimize ("O0")

//#define Serial if(Serial)Serial //fixes issue where prints in ISR hangs the program, might wanna look into a threadsafe alternative though
void log_message(LogLevel level, const char *message);
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;
FS_CAN FS_CAN0(&Can0); 
static QueueHandle_t logQueue = NULL;

static void task1(void*) {
    uint32_t testData = 0;
    uint32_t testData2 = 0;
    FS_CAN::CAN_MSG testMsg;
    testMsg.id = 256;
    testMsg.signal_count = 1;
    FS_CAN::CAN_SIGNAL testSignal{&testData, 0, 16};
    FS_CAN::CAN_SIGNAL testSignal2{&testData2, 8, 8};
    testMsg.signals.push_back(&testSignal);
    testMsg.signals.push_back(&testSignal2);
    Serial.printf("didnt' crash yet\n");
    FS_CAN0.subscribe_to_message(&testMsg);
    logQueue = xQueueCreate(50, sizeof(LogMessage_t));
    while (true) {
        digitalWriteFast(LED_BUILTIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));
        Serial.printf("data1:%d data2:%d\n", testData, testData2);
        char str [20];
        sprintf(str,"Hello from task1");
        log_message(LOG_INFO, str);
        digitalWriteFast(LED_BUILTIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void task2(void*) {
    //vTaskDelay(pdMS_TO_TICKS(5000));
    while (true) {
        

        if (logQueue != NULL){
            LogMessage_t msg;
        if (xQueueReceive(logQueue, &msg, 0) == pdTRUE)
        {
            Serial.println("black_box(): received message from queue");
            Serial.printf("%d: %s\n", msg.level, msg.message);
        }
        }
        vTaskDelay(pdMS_TO_TICKS(1'000));
    }
}
void canSniff(const CAN_message_t &msg) {
  FS_CAN0.CAN_RX_ISR(msg);
}

void log_message(LogLevel level, const char *message)
{
    LogMessage_t msg;
    msg.level = level;

    strncpy(msg.message, message, 256);

    // Ensure the message is null terminated
    if (sizeof(msg.message) > 0)
    {
        msg.message[sizeof(msg.message) - 1] = 0;
    }

    if (xQueueSendToBack(logQueue, &msg, portMAX_DELAY) == pdPASS)
    {
        Serial.println("log_message(): sent message to queue");
    }
    else
    {
        Serial.println("log_message(): failed to send message to queue");
    }
}




FLASHMEM __attribute__((noinline)) void setup() {

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWriteFast(LED_BUILTIN, HIGH);
    
    //Serial.begin(96000);

    if (CrashReport) {
        Serial.print(CrashReport);
        Serial.println();
        Serial.flush();
    }
    Can0.begin();
    Can0.setBaudRate(500000);
    Can0.setMaxMB(64);
    Can0.enableFIFO();
    Can0.enableMBInterrupt(FIFO);
    Can0.enableFIFOInterrupt();
    Can0.onReceive(canSniff);
    Serial.println(PSTR("\r\nBooting FreeRTOS kernel " tskKERNEL_VERSION_NUMBER ". Built by gcc " __VERSION__ " (newlib " _NEWLIB_VERSION ") on " __DATE__ ". ***\r\n"));

    xTaskCreate(task1, "task1", 5028, nullptr, 2, nullptr);
    xTaskCreate(task2, "task2", 1028, nullptr, 2, nullptr);
    delay(10000);
    Serial.println("setup(): starting scheduler...");
    Serial.flush();

    vTaskStartScheduler();
}

void loop() {}