#include "main.h"
#include "config/devices.h"
#pragma GCC optimize("O0")

time_t getTeensy3Time();

void can_sniff(const CAN_message_t &msg)
{
    Devices::GetFS_Can0().CAN_RX_ISR(msg);
}

void setup()
{
    while (!Serial);
    
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
    
    FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;
    FS_CAN FS_CAN0(&Can0);
    
    Serial.println("can0 and FS_can created");
    Can0.begin();
    Can0.setBaudRate(500000);
    Can0.setMaxMB(64);
    Can0.enableFIFO();
    Can0.enableMBInterrupt(FIFO);
    Can0.enableFIFOInterrupt();
    Can0.onReceive(can_sniff);

    Devices::Create(&FS_CAN0);

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