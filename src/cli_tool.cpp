#include "cli_tool.h"

// Max 10 commands
std::array<CLI_Command, 10> commands;

void task(void *pvParameters)
{
    char input[100];
    char *args[10];
    int argCount;
    while (1)
    {
        // Get the input
        Serial.readBytesUntil('\n', input, 100);
        argCount = 0;
        args[argCount] = strtok(input, " ");

        // Tokenize the input
        while (args[argCount] != NULL)
        {
            argCount++;
            args[argCount] = strtok(NULL, " ");
        }

        // Execute the command
        for (int i = 0; i < (int)commands.size(); i++)
        {
            if (commands[i].command != NULL && strcmp(commands[i].command, args[0]) == 0)
            {
                commands[i].function(argCount, args);
            }
        }

        // Reset buffers
        for (int i = 0; i < 100; i++)
        {
            input[i] = 0;
        }

        for (int i = 0; i < 10; i++)
        {
            args[i] = NULL;
        }
    }
}

void testCommand(int argc, char **argv)
{
    Serial.println("Test Command");
}

void CLI_Tool::begin(int taskPriority)
{
    for (int i = 0; i < commands.size(); i++)
    {
        commands[i].command = NULL;
    }

    CLI_Command test;
    test.command = "test";
    test.function = testCommand;
    test.helpText = "This is a test command";

    addCommand(test);

    xTaskCreate(task, "CLI_Task", 1000, NULL, taskPriority, NULL);
}

void CLI_Tool::addCommand(CLI_Command command)
{
    for (int i = 0; i < (int)commands.size(); i++)
    {
        if (commands[i].command == NULL)
        {
            commands[i] = command;
            break;
        }
    }
}