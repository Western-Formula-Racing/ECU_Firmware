#ifndef _CLI_TOOL_
#define _CLI_TOOL_

#include <iostream>
#include <arduino_freertos.h>

typedef struct {
    const char *command;
    void (*function)(int, char **);
    const char *helpText;
} CLI_Command;

namespace CLI_Tool
{
    // Begin the CLI task
    void begin(int taskPriority);

    // Add a command to the CLI
    void addCommand(CLI_Command command);
}

#endif