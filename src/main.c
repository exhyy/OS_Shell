#include "command.h"
#include <stdio.h>

extern char command_buffer[MAX_COMMAND_LENGTH];
extern char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH];
extern int commands_length;

int main()
{
    while(1)
    {
        run_command(input_command());
    }
}