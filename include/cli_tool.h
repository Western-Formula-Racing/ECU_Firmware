#ifndef _CLI_TOOL_
#define _CLI_TOOL_

#include <iostream>
#include <arduino_freertos.h>
#include "interfaces/pdm.h"


void testCommand(int argc, char *argv[]);
void fake_rtd(int argc, char *argv[]);
void reset_state(int argc, char *argv[]);
void set_hsd(int argc, char *argv[]);

typedef void (*CommandFunction)(int argc, char *argv[]);

struct CLI_Command
{
    const char *command;
    CommandFunction function;
    const char *description;
};

namespace CLI_Tool
{
    // Begin the CLI task
    void task(void *);
}

#endif