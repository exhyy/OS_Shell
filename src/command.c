#include "command.h"
#include "buildin.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char SHELL_NAME[] = "YuyaoShell";
char command_buffer[MAX_COMMAND_LENGTH];
char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH];
int commands_length = 0;

// 显示等待输入信息，并保存用户输入的命令
int input_command()
{
    memset(command_buffer, 0, sizeof(command_buffer));
    char cwd[256];
    getcwd(cwd, 256);
    printf("%s:%s $ ", SHELL_NAME, cwd);
    fgets(command_buffer, MAX_COMMAND_LENGTH, stdin);
    char ch;
    int input_overflow = 0;
    if (command_buffer[MAX_COMMAND_LENGTH - 2] != '\n' && command_buffer[MAX_COMMAND_LENGTH - 2] != '\0')
    {
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF)
        {
            input_overflow = 1;
        }
    }
    if (input_overflow == 1)
    {
        return INPUT_OVERFLOW;
    }
    // printf("check 1\n");

    return parse_command();
}

// 将command_buffer的内容按照空格拆分，并存放到commands中
int parse_command()
{
    char *left = command_buffer, *right;
    int cnt = 0;
    // 去除空格
    while (*left == ' ')
    {
        left++;
    }
    if (*left == '\n' || *left == '\0')
    {
        commands_length = 0;
        return INPUT_OK;
    }
    right = left + 1;
    while(1)
    {
        if (*right == '\n' || *right == '\0')
        {
            if (cnt == MAX_COMMAND_ARGC)
                return ARGS_OVERFLOW;
            *right = '\0';
            strcpy(commands[cnt], left);
            cnt++;
            break;
        }
        if (*right == ' ')
        {
            if (cnt == MAX_COMMAND_ARGC)
                return ARGS_OVERFLOW;
            *right = '\0';
            strcpy(commands[cnt], left);
            *right = ' ';
            cnt++;
            left = right;

            // 去除空格
            while (*left == ' ')
            {
                left++;
            }
            if (*left == '\n' || *left == '\0')
                break;
            right = left;
        }
        right++;
    }
    commands_length = cnt;

    // 将每段命令开头的~转换为HOME目录
    char *home = getenv("HOME");
    int home_length = strlen(home);
    char temp[MAX_COMMAND_LENGTH];
    for (int i = 0; i < commands_length; i++)
    {
        if (commands[i][0] == '~' && (commands[i][1] == '/' || commands[i][1] == '\0'))
        {
            strcpy(temp, home);
            for (int j = 1; commands[i][j] != '\0'; j++)
            {
                temp[home_length + j - 1] = commands[i][j];
            }
            strcpy(commands[i], temp);
        }
    }
    return INPUT_OK;
}

// 执行命令
void run_command(int command_flag)
{
    if (command_flag == INPUT_OK)
    {
        if (commands_length == 0)
        {
            // 空命令，直接结束
            return;
        }
        else
        {
            if (strcmp(commands[0], "cd") == 0)
            {
                cd(commands, commands_length);
            }
            else if (strcmp(commands[0], "exit") == 0)
            {
                exit_shell(commands, commands_length);
            }
            else if (strcmp(commands[0], "ls") == 0)
            {
                ls(commands, commands_length);
            }
            else
            {
                fprintf(stderr, "错误：命令不存在：%s\n", commands[0]);
            }
        }
    }
    else if (command_flag == INPUT_OVERFLOW)
    {
        printf("命令过长！\n");
    }
    else if (command_flag == ARGS_OVERFLOW)
    {
        printf("命令参数过多！\n");
    }
}