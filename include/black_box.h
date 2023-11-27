#ifndef _BLACK_BOX_
#define _BLACK_BOX_

#include <iostream>
#include <arduino_freertos.h>
#include "helpers/logging.h"
#include <SD.h>
#include <TimeLib.h>

namespace BlackBox
{
    // Begin the black box task
    void begin(int queueSize, int taskPriority);

    // Log a message to the black box
    void log(LogMessage_t msg);

    // Log a message to the black box
    void log(LogLevel level, const char *message);
}

#endif