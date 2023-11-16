#include <iostream>
#include <arduino_freertos.h>
#include "helpers/logging.h"

class BlackBox
{
public:
    static void log(QueueHandle_t queue, LogMessage_t msg);
    static void log(QueueHandle_t queue, LogLevel level, const char *message);
    static void task(void *queuePtr);
};
