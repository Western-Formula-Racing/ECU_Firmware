#include "black_box.h"

#include <EEPROM.h>

static QueueHandle_t logQueue = NULL;
static QueueHandle_t sensorQueue = NULL;
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
                Serial.print(msg.time);
                Serial.print(" - ");

                switch (msg.level)
                {
                case LOG_INFO:
                    Serial.print("INFO: ");
                    break;
                case LOG_WARNING:
                    Serial.print("WARNING: ");
                    break;
                case LOG_ERROR:
                    Serial.print("ERROR: ");
                    break;
                default:
                    Serial.print("UNKNOWN: ");
                    break;
                }

                Serial.println(msg.message);

                if (logFile)
                {
                    logFile.print(msg.time);
                    logFile.print(" - ");

                    switch (msg.level)
                    {
                    case LOG_INFO:
                        logFile.print("INFO: ");
                        break;
                    case LOG_WARNING:
                        logFile.print("WARNING: ");
                        break;
                    case LOG_ERROR:
                        logFile.print("ERROR: ");
                        break;
                    default:
                        logFile.print("UNKNOWN: ");
                        break;
                    }

                    logFile.println(msg.message);
                    logFile.flush();
                }
            }
        }
        if (sensorQueue != NULL)
        {
            SensorMessage_t msg;
            while (xQueueReceive(sensorQueue, &msg, 0) == pdTRUE)
            {
                if (sensorFile)
                {
                    sensorFile.print(msg.sensorName);
                    sensorFile.print(",");
                    sensorFile.print(msg.time);
                    sensorFile.print(",");
                    sensorFile.println(msg.sensorValue);
                    sensorFile.flush();
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void BlackBox::begin(int queueSize, int taskPriority)
{
    // Create the logQueue
    logQueue = xQueueCreate(queueSize, sizeof(LogMessage_t));

    // Create the sensorQueue
    sensorQueue = xQueueCreate(queueSize, sizeof(SensorMessage_t));

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
    xTaskCreate(task, "BlackBox", 1024, nullptr, taskPriority, nullptr);
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
