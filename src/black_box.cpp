#include "black_box.h"
#include "helpers/logging.h"

void BlackBox::log(QueueHandle_t queue, LogMessage_t msg)
{
    if (queue != NULL)
    {
        if (xQueueSendToBack(queue, &msg, 0) != pdTRUE)
        {
            Serial.println("BlackBox.log(): failed to send message to queue");
        }
    }
}

void BlackBox::log(QueueHandle_t queue, LogLevel level, const char *message)
{
    if (queue != NULL)
    {
        LogMessage_t msg;

        msg.level = level;
        strncpy(msg.message, message, 256);

        // Ensure the message is null terminated
        if (sizeof(msg.message) > 0)
        {
            msg.message[sizeof(msg.message) - 1] = 0;
        }

        BlackBox::log(queue, msg);
    }
}

void BlackBox::task(void *queuePtr)
{
    QueueHandle_t queue = *(QueueHandle_t *)queuePtr;

    while (true)
    {
        if (queue != NULL)
        {
            LogMessage_t msg;
            while (xQueueReceive(queue, &msg, 0) == pdTRUE)
            {
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
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1'000));
    }
}