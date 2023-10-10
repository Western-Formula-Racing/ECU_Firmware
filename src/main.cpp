#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "TeensyDebug.h"
#pragma GCC optimize ("O0")

#define Serial if(Serial)Serial //fixes issue where prints in ISR hangs the program, might wanna look into a threadsafe alternative though

FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;

static void task1(void*) {
    while (true) {
        digitalWriteFast(LED_BUILTIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(500));

        digitalWriteFast(LED_BUILTIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void task2(void*) {
    while (true) {
        Serial.println("TICK");
        vTaskDelay(pdMS_TO_TICKS(1'000));

        Serial.println("TOCK");
        vTaskDelay(pdMS_TO_TICKS(1'000));
    }
}
void canSniff(const CAN_message_t &msg) {
  Serial.print(msg.id);
  Serial.print(" ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i]); Serial.print(" ");
  } Serial.println();
}


FLASHMEM __attribute__((noinline)) void setup() {
    
    debug.begin(SerialUSB1);
    halt_cpu();

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWriteFast(LED_BUILTIN, HIGH);
    
    delay(5'000);

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

    xTaskCreate(task1, "task1", 128, nullptr, 2, nullptr);
    xTaskCreate(task2, "task2", 128, nullptr, 2, nullptr);

    Serial.println("setup(): starting scheduler...");
    Serial.flush();

    vTaskStartScheduler();
}

void loop() {}