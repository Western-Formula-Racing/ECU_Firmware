#include <iostream>
#include <arduino_freertos.h>
#include "helpers/logging.h"

class BlackBox
{
private:
    static QueueHandle_t queue;
    static File logFile;
    static void task(void *queuePtr);

public:
    // Delete the default constructor
    BlackBox() = delete;

    // Begin the black box task
    static void begin(int queueSize, int taskPriority);

    // Log a message to the black box
    static void log(LogMessage_t msg);

    // Log a message to the black box
    static void log(LogLevel level, const char *message);
};
