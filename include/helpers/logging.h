#ifndef _LOGGING_
#define _LOGGING_

enum LogLevel
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

typedef struct
{
    // The log level of the message
    LogLevel level;
    // The current time
    time_t time;
    // Fixed length of 256 characters
    char message[256];
} LogMessage_t;
#endif