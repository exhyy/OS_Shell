#include "buildin.h"
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>
#include <string.h>

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
            fprintf(stderr, "错误：路径不存在或权限不足：%s\n", commands[1]);
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

void ls(const char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH], int commands_length)
{
    int show_hidden = 0;

    if (commands_length == 1)
    {
        char path[] = "./";
        print_file_names(path, show_hidden, 0);
    }
    else
    {
        // 查找是否有"-a"参数
        for (int i = 1; i < commands_length; i++)
        {
            if (strcmp(commands[i], "-a") == 0)
            {
                show_hidden = 1;
                break;
            }
        }

        // 输出文件名
        if (commands_length == 2)
        {
            // 输入的命令为"ls -a"或"ls <路径>"
            if (show_hidden)
            {
                char path[] = "./";
                print_file_names(path, show_hidden, 0);
            }
            else
            {
                if(!print_file_names(commands[1], show_hidden, 0))
                {
                    fprintf(stderr, "错误：路径不存在或权限不足：%s\n", commands[1]);
                }
            }
        }
        else if (commands_length == 3)
        {
            // 输入的命令为"ls -a <路径>"或"ls <路径> <路径>"
            for (int i = 1; i < commands_length; i++)
            {
                if (strcmp(commands[i], "-a") != 0)
                {
                    if(!print_file_names(commands[i], show_hidden, !show_hidden))
                    {
                        fprintf(stderr, "错误：路径不存在或权限不足：%s\n", commands[i]);
                    }
                    else if (i < commands_length - 1 && !show_hidden)
                    {
                        fprintf(stdout, "\n");
                    }
                }
            }
        }
        else
        {
            // 输出多个路径
            for (int i = 1; i < commands_length; i++)
            {
                if (strcmp(commands[i], "-a") != 0)
                {
                    if(!print_file_names(commands[i], show_hidden, 1))
                    {
                        fprintf(stderr, "错误：路径不存在或权限不足：%s\n", commands[i]);
                    }
                    else if (i < commands_length - 1)
                    {
                        fprintf(stdout, "\n");
                    }
                }
            }
        }
    }
}