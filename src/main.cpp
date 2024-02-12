#include "main.h"
#include "config/devices.h"
#pragma GCC optimize("O0")
#define debug 1
time_t getTeensy3Time();
// These guys ABSOLUTELY HAVE TO BE GLOBAL VARIABLES
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can2;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can1;
FS_CAN dataCAN(&Can2);
FS_CAN controlCAN(&Can1);
// DO NOT TOUCH
void dataCan_sniff(const CAN_message_t &msg)
{
    dataCAN.CAN_RX_ISR(msg);
}
void controlCan_sniff(const CAN_message_t &msg)
{
    controlCAN.CAN_RX_ISR(msg);
}

void setup()
{

    setSyncProvider(getTeensy3Time);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWriteFast(LED_BUILTIN, HIGH);

    if (CrashReport)
    {
        while (!Serial)
            ;
        Serial.print(CrashReport);
        Serial.println();
        Serial.flush();
    }
    Serial.println("bruh");

    Serial.println("can2 and dataCAN created");
    Can2.begin();
    Can2.setBaudRate(500000);
    Can2.setMaxMB(64);
    Can2.enableFIFO();
    Can2.enableMBInterrupt(FIFO);
    Can2.enableFIFOInterrupt();
    Can2.onReceive(dataCan_sniff);

    Serial.println("can1 and controlCAN created");
    Can1.begin();
    Can1.setBaudRate(500000);
    Can1.setMaxMB(64);
    Can1.enableFIFO();
    Can1.enableMBInterrupt(FIFO);
    Can1.enableFIFOInterrupt();
    Can1.onReceive(controlCan_sniff);

    Serial.println(PSTR("\r\nBooting FreeRTOS kernel " tskKERNEL_VERSION_NUMBER ". Built by gcc " __VERSION__ " (newlib " _NEWLIB_VERSION ") on " __DATE__ ". ***\r\n"));

    xTaskCreate(setup_task, "setup_task", 1024, nullptr, tskIDLE_PRIORITY + 1, nullptr);

    Serial.println("setup(): starting scheduler...");
    Serial.flush();

    vTaskStartScheduler();
    Serial.println("scheduler returned");
}

time_t getTeensy3Time()
{
    return Teensy3Clock.get() + 3600 * 5; // shift time forward 5 hours and 1s because that's needed for some reason
}

void loop() {}