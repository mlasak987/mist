#ifndef _SHELL_CMD_H
#define _SHELL_CMD_H

typedef void (*shell_cmd_func_t)(int argc, char** argv);

typedef struct
{
  const char* name;
  shell_cmd_func_t func;
  const char* description;
  const char *usage;
} shell_command_t;

void cmd_help(int argc, char** argv);
void cmd_clear(int argc, char** argv);
void cmd_echo(int argc, char** argv);
void cmd_cat(int argc, char** argv);
void cmd_ls(int argc, char** argv);

extern const shell_command_t commands[];
#define NUM_COMMANDS 5

#endif
