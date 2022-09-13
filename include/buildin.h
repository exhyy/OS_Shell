#ifndef BUILDIN_H_
#define BUILDIN_H_

#include "command.h"

void cd(const char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH], int commands_length);
void exit_shell(const char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH], int commands_length);
void ls(const char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH], int commands_length);
void pwd(const char commands[MAX_COMMAND_ARGC][MAX_COMMAND_LENGTH], int commands_length);

#endif