#include <SD.h>
#include <TimeLib.h>

#include "black_box.h"
#include "helpers/logging.h"

QueueHandle_t BlackBox::queue = NULL;
File BlackBox::logFile = NULL;

void BlackBox::begin(int queueSize, int taskPriority)
{
    // Create the queue
    BlackBox::queue = xQueueCreate(queueSize, sizeof(LogMessage_t));

    // Setup SD card
    if (!SD.begin(BUILTIN_SDCARD))
    {
        Serial.println("setup(): failed to initialize SD card");
    }

    BlackBox::logFile = SD.open("log.txt", FILE_WRITE);

    if (!BlackBox::logFile)
    {
        Serial.println("setup(): failed to open log file");
    }
    else
    {
        BlackBox::logFile.println("BlackBox initialized!");
        BlackBox::logFile.flush();
    }

    // Create the task
    xTaskCreate(BlackBox::task, "BlackBox", 1024, nullptr, taskPriority, nullptr);
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
    if (BlackBox::queue != NULL)
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

void BlackBox::task(void *)
{
    while (true)
    {
        if (BlackBox::queue != NULL)
        {
            LogMessage_t msg;
            while (xQueueReceive(BlackBox::queue, &msg, 0) == pdTRUE)
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

                if (BlackBox::logFile)
                {
                    BlackBox::logFile.print(msg.time);
                    BlackBox::logFile.print(" - ");

                    switch (msg.level)
                    {
                    case LOG_INFO:
                        BlackBox::logFile.print("INFO: ");
                        break;
                    case LOG_WARNING:
                        BlackBox::logFile.print("WARNING: ");
                        break;
                    case LOG_ERROR:
                        BlackBox::logFile.print("ERROR: ");
                        break;
                    default:
                        BlackBox::logFile.print("UNKNOWN: ");
                        break;
                    }

                    BlackBox::logFile.println(msg.message);
                    BlackBox::logFile.flush();
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1'000));
    }
}