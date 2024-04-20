#ifndef _BLACK_BOX_
#define _BLACK_BOX_

#include <SD.h>
#include <TimeLib.h>
#include <iostream>
#include <arduino_freertos.h>
#include "helpers/logging.h"

namespace BlackBox
{
    // Begin the black box task
    void begin(int queueSize, int taskPriority);

    // Log a message to the black box
    void log(LogMessage_t msg);

    // Log a message to the black box
    void log(LogLevel level, const char *message);

    // Log a sensor message to the black box
    void log(SensorMessage_t msg);

    // Log a float sensor message to the black box
    void logSensor(const char *signalName, float value);

    // Log a int sensor message to the black box
    void logSensor(const char *signalName, int value);

    // Log a boolean sensor message to the black box
    void logSensor(const char *signalName, bool value);

    // Clear power cycle counter
    void clearPowerCycleCounter();
}

#endif