#include "cli_tool.h"
#include "black_box.h"
#include <format>

const int MAX_BUFFER_SIZE = 100;
const CLI_Command cli_commands[]{
    {"test", testCommand, "Test command"}};

void testCommand(int argc, char *argv[])
{
    BlackBox::log(LOG_INFO, "Test command");
    for (int i = 0; i < argc; i++)
    {
        Serial.println(argv[i]);
    }
}

void CLI_Tool::task(void *)
{
    while (1)
    {
        if (Serial.available() > 0)
        {
            char buffer[MAX_BUFFER_SIZE];
            int i = 0;
            while (Serial.available() > 0 && i < MAX_BUFFER_SIZE - 1)
            {
                int c = Serial.read();

                // Is valid character or number between 0 and 9
                if ((c >= 32 && c <= 126) || (c >= 48 && c <= 57))
                {
                    buffer[i] = c;
                    i++;
                }
            }

            if (i == 0)
                continue;

            buffer[i] = '\0';

            char *argv[MAX_BUFFER_SIZE];
            int argc = 0;
            argv[argc] = strtok(buffer, " ");

            while (argv[argc] != NULL)
            {
                argc++;
                argv[argc] = strtok(NULL, " ");
            }

            for (const CLI_Command &command : cli_commands)
            {
                if (strcmp(argv[0], command.command) == 0)
                {
                    // Prevent null pointer dereference
                    if (command.function != nullptr)
                    {
                        command.function(argc, argv);
                    }
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}