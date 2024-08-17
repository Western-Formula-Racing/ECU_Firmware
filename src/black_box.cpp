#include "black_box.h"

#include <EEPROM.h>

static const int BUFFER_SIZE = 100;

// 2 commas and a newline
static const int MAX_SENSOR_MESSAGE_SIZE = (sizeof(SensorMessage_t) + 10 + 3);

// 4 bytes for time, 4 bytes for dash and spaces, 10 bytes for log level, 256 bytes for message
static const int MAX_LOG_MESSAGE_SIZE = (10 + 4 + 10 + 256);

static QueueHandle_t logQueue = NULL;
static LogMessage_t logBuffer[BUFFER_SIZE];
static char logBufferString[BUFFER_SIZE * MAX_SENSOR_MESSAGE_SIZE];

static QueueHandle_t sensorQueue = NULL;
static SensorMessage_t sensorBuffer[BUFFER_SIZE];

static int logBufferIndex = 0;
static int sensorBufferIndex = 0;

static File logFile = NULL;
static File sensorFile = NULL;

static void task(void *)
{
    while (true)
    {
        if (logQueue != NULL)
        {
            LogMessage_t msg;
            while (xQueueReceive(logQueue, &msg, 0) == pdTRUE)
            {
                // Store the message in the active buffer
                logBuffer[logBufferIndex++] = msg;

                // Check if the buffer is full
                if (logBufferIndex >= BUFFER_SIZE)
                {
                    int bufferPosition = 0;

                    // Write the processing buffer to the SD card
                    for (int i = 0; i < BUFFER_SIZE; i++)
                    {
                        bufferPosition += snprintf(&logBufferString[bufferPosition], MAX_LOG_MESSAGE_SIZE, "%lu - ", static_cast<unsigned long>(logBuffer[i].time));

                        switch (logBuffer[i].level)
                        {
                        case LOG_INFO:
                            bufferPosition += snprintf(&logBufferString[bufferPosition], MAX_LOG_MESSAGE_SIZE, "INFO: ");
                            break;
                        case LOG_WARNING:
                            bufferPosition += snprintf(&logBufferString[bufferPosition], MAX_LOG_MESSAGE_SIZE, "WARNING: ");
                            break;
                        case LOG_ERROR:
                            bufferPosition += snprintf(&logBufferString[bufferPosition], MAX_LOG_MESSAGE_SIZE, "ERROR: ");
                            break;
                        default:
                            bufferPosition += snprintf(&logBufferString[bufferPosition], MAX_LOG_MESSAGE_SIZE, "UNKNOWN: ");
                            break;
                        }

                        bufferPosition += snprintf(&logBufferString[bufferPosition], MAX_LOG_MESSAGE_SIZE, "%s\n", logBuffer[i].message);
                    }

                    logFile.write(logBufferString, bufferPosition);
                    logFile.flush();

                    // Reset the index for the new active buffer
                    logBufferIndex = 0;
                }
            }
        }

        // Check if sensorQueue has messages
        if (sensorQueue != NULL)
        {
            SensorMessage_t msg;
            while (xQueueReceive(sensorQueue, &msg, 0) == pdTRUE)
            {
                // Store the message in the active buffer
                sensorBuffer[sensorBufferIndex++] = msg;

                // Check if the buffer is full
                if (sensorBufferIndex >= BUFFER_SIZE)
                {
                    int bufferPosition = 0;

                    // Write the processing buffer to the SD card
                    for (int i = 0; i < BUFFER_SIZE; i++)
                    {
                        bufferPosition += snprintf(&logBufferString[bufferPosition], MAX_SENSOR_MESSAGE_SIZE, "%s,%lu,%s\n", sensorBuffer[i].sensorName, static_cast<unsigned long>(sensorBuffer[i].time), sensorBuffer[i].sensorValue);
                    }

                    sensorFile.write(logBufferString, bufferPosition);
                    sensorFile.flush();

                    // Reset the index for the new active buffer
                    sensorBufferIndex = 0;
                }
            }
        }
    }
}

void BlackBox::begin(int taskPriority)
{
    // Create the logQueue
    logQueue = xQueueCreate(BUFFER_SIZE, sizeof(LogMessage_t));

    // Create the sensorQueue
    sensorQueue = xQueueCreate(BUFFER_SIZE, sizeof(SensorMessage_t));

    // Setup SD card
    if (!SD.begin(BUILTIN_SDCARD))
    {
        Serial.println("setup(): failed to initialize SD card");
    }

    unsigned int powerCycleCount = 0;

    // Use the EEPROM to store the power cycle count
    if (EEPROM.read(0) != 0)
    {
        EEPROM.get(1, powerCycleCount);
    }

    // Increment the power cycle count
    powerCycleCount++;

    // Store the power cycle count
    EEPROM.write(0, 1);
    EEPROM.put(1, powerCycleCount);

    // Log file at log_{powerCycleCount}.txt
    char logFileName[32];
    sprintf(logFileName, "/log_%u.txt", powerCycleCount);

    logFile = SD.open(logFileName, FILE_WRITE);

    // Sensor file at sensors_{powerCycleCount}.csv
    char sensorFileName[32];
    sprintf(sensorFileName, "/sensors_%u.csv", powerCycleCount);

    sensorFile = SD.open(sensorFileName, FILE_WRITE);

    if (sensorFile)
    {
        sensorFile.println("signal_name,time_stamp,value");
        sensorFile.flush();
    }
    else
    {
        Serial.println("setup(): failed to open sensor file");
    }

    if (logFile)
    {
        logFile.println("BlackBox initialized!");
        logFile.flush();
    }
    else
    {
        Serial.println("setup(): failed to open log file");
    }

    // Create the task
    xTaskCreate(task, "BlackBox", 5024, nullptr, taskPriority, nullptr);
}

void BlackBox::log(LogMessage_t msg)
{
    if (logQueue != NULL)
    {
        if (xQueueSendToBack(logQueue, &msg, 0) != pdTRUE)
        {
            Serial.println("BlackBox.log(): failed to send message to logQueue");
        }
    }
}

void BlackBox::log(LogLevel level, const char *message)
{
    if (logQueue != NULL)
    {
        LogMessage_t msg;

        msg.level = level;
        msg.time = millis();
        strncpy(msg.message, message, 256);

        // Ensure the message is null terminated
        if (sizeof(msg.message) > 0)
        {
            msg.message[sizeof(msg.message) - 1] = 0;
        }

        BlackBox::log(msg);
    }
}

void BlackBox::log(SensorMessage_t msg)
{
    if (sensorQueue != NULL)
    {
        if (xQueueSendToBack(sensorQueue, &msg, 0) != pdTRUE)
        {
            Serial.println("BlackBox.log(): failed to send message to sensorQueue");
        }
    }
}

void BlackBox::logSensor(const char *signalName, float value)
{
    if (sensorQueue != NULL)
    {
        SensorMessage_t msg;

        msg.time = millis();
        strncpy(msg.sensorName, signalName, 32);

        const int len = snprintf(nullptr, 0, "%f", value);
        snprintf(msg.sensorValue, len + 1, "%f", value);

        // Ensure the message is null terminated
        if (sizeof(msg.sensorName) > 0)
        {
            msg.sensorName[sizeof(msg.sensorName) - 1] = 0;
        }

        BlackBox::log(msg);
    }
}

void BlackBox::logSensor(const char *signalName, int value)
{
    if (sensorQueue != NULL)
    {
        SensorMessage_t msg;

        msg.time = millis();
        strncpy(msg.sensorName, signalName, 32);

        const int len = snprintf(nullptr, 0, "%d", value);
        snprintf(msg.sensorValue, len + 1, "%d", value);

        // Ensure the message is null terminated
        if (sizeof(msg.sensorName) > 0)
        {
            msg.sensorName[sizeof(msg.sensorName) - 1] = 0;
        }

        BlackBox::log(msg);
    }
}

void BlackBox::logSensor(const char *signalName, bool value)
{
    if (sensorQueue != NULL)
    {
        SensorMessage_t msg;

        msg.time = millis();
        strncpy(msg.sensorName, signalName, 32);

        snprintf(msg.sensorValue, 6, "%s", value ? "true" : "false");

        // Ensure the message is null terminated
        if (sizeof(msg.sensorName) > 0)
        {
            msg.sensorName[sizeof(msg.sensorName) - 1] = 0;
        }

        BlackBox::log(msg);
    }
}

void BlackBox::clearPowerCycleCounter()
{
    EEPROM.write(0, 0);
    EEPROM.write(1, 0);
    EEPROM.write(2, 0);
    EEPROM.write(3, 0);
    EEPROM.write(4, 0);
}
