#include "buildin.h"
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void cd(const char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH], int commands_length)
{
    if (commands_length == 1)
    {
        // 无参数，不执行任何操作
        return;
    }
    else if (commands_length == 2)
    {
        if(chdir(commands[1]))
        {
            fprintf(stderr, "错误：路径%s不存在！\n", commands[1]);
        }
    }
    else
    {
        fprintf(stderr, "错误：参数过多！\n");
    }
}

void exit_shell(const char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH], int commands_length)
{
    if (commands_length == 1)
    {
        exit(0);
    }
    else if (commands_length == 2)
    {
        // 使用指定代码退出shell
        if (str_is_int(commands[1]))
        {
            int exit_code = atoi(commands[1]);
            exit(exit_code);
        }
        else
        {
            fprintf(stderr, "错误：%s非整数！\n", commands[1]);
        }
    }
    else
    {
        fprintf(stderr, "错误：参数过多！\n");
    }
}