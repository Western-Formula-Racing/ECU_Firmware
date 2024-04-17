#include "cli_tool.h"
#include "black_box.h"
#include <format>
#include "config/devices.h"
extern State state;
const int MAX_BUFFER_SIZE = 100;
const CLI_Command cli_commands[]{
    {"test", testCommand, "Test command"},
    {"fake_rtd", fake_rtd, "Command to fake brake and RTD"},
    {"reset_state", reset_state, "Command to restart state-machine"},
    {"set_hsd", set_hsd, "Command to set HSD"},
    };


void fake_rtd(int argc, char *argv[])
{
    BlackBox::log(LOG_INFO, "faking rtd sequence");
    Devices::Get().GetRTDButton().override(1);
    Devices::Get().GetSensors()[2]->override(3.0);
    Devices::Get().GetSensors()[3]->override(3.0);
    vTaskDelay(pdMS_TO_TICKS(2000));
    Devices::Get().GetRTDButton().disable_override();
    Devices::Get().GetSensors()[2]->disable_override();
    Devices::Get().GetSensors()[3]->disable_override();
}

void reset_state(int argc, char *argv[])
{
    BlackBox::log(LOG_INFO, "state machine reset");
    state = START;
}

void set_hsd(int argc, char *argv[]){
    std::vector<PDM_PINS> hsdMap ({HSDIN1,HSDIN2,HSDIN3,HSDIN4,HSDIN5,HSDIN6,HSDIN7});
    Serial.printf("argc:%d 0:%d 1:%d\n", argc, atoi(argv[1]), atoi(argv[2]));
    if(argc == 3 and atoi(argv[1]) <= 7 and atoi(argv[1]) >= 0){
        Devices::Get().GetPDM().setPin(hsdMap[atoi(argv[1])], atoi(argv[2]));
    }
    else{
        Serial.println("wrong arguments mf");
    }

}


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