#pragma once

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
    // Fixed length of 256 characters
    char message[256];
} LogMessage_t;