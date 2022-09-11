#include "command.h"
#include <stdio.h>

extern char command_buffer[MAX_COMMAND_LENGTH];
extern char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH];
extern int commands_length;

int main()
{
    while(1)
    {
        // int t;
        // if (t = input_command())
        // {
        //     printf("错误代码：%d\n", t);
        //     continue;
        // }
        run_command(input_command());
        // printf(">>> %s\n", command_buffer);
        // // printf("commands_length=%d", commands_length);
        // for (int i = 0; i < commands_length; i++)
        //     printf("*** %s\n", commands[i]);
        
    }
}