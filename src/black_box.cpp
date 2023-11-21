#include <SD.h>
#include <TimeLib.h>

#include "black_box.h"
#include "helpers/logging.h"

static QueueHandle_t queue = NULL;
static File logFile = NULL;
static void task(void *)
{
    while (true)
    {
        if (queue != NULL)
        {
            LogMessage_t msg;
            while (xQueueReceive(queue, &msg, 0) == pdTRUE)
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
        vTaskDelay(pdMS_TO_TICKS(1'000));
    }
}

void BlackBox::begin(int queueSize, int taskPriority)
{
    // Create the queue
    queue = xQueueCreate(queueSize, sizeof(LogMessage_t));

    // Setup SD card
    if (!SD.begin(BUILTIN_SDCARD))
    {
        Serial.println("setup(): failed to initialize SD card");
    }

    logFile = SD.open("log.txt", FILE_WRITE);

    if (!logFile)
    {
        Serial.println("setup(): failed to open log file");
    }
    else
    {
        logFile.println("BlackBox initialized!");
        logFile.flush();
    }

    // Create the task
    xTaskCreate(task, "BlackBox", 1024, nullptr, taskPriority, nullptr);
}

void BlackBox::log(LogMessage_t msg)
{
    if (queue != NULL)
    {
        if (xQueueSendToBack(queue, &msg, 0) != pdTRUE)
        {
            Serial.println("BlackBox.log(): failed to send message to queue");
        }
    }
}

void BlackBox::log(LogLevel level, const char *message)
{
    if (queue != NULL)
    {
        LogMessage_t msg;

        msg.level = level;
        msg.time = now();
        strncpy(msg.message, message, 256);

        // Ensure the message is null terminated
        if (sizeof(msg.message) > 0)
        {
            msg.message[sizeof(msg.message) - 1] = 0;
        }

        BlackBox::log(msg);
    }
}