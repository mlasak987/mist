#include <stdio.h>
#include <string.h>

#include "shell/shell.h"
#include "shell/cmd.h"
#include "drivers/tty.h"
#include "fs/initramfs.h"

const shell_command_t commands[] =
{
  {"help",  cmd_help,  "Prints available commands", "help"},
  {"clear", cmd_clear, "Clears the terminal screen", "clear"},
  {"echo",  cmd_echo,  "Prints given text to the console", "echo [text...]"},
  {"cat",   cmd_cat,   "Reads and prints file contents", "cat <filename>"},
  {"ls",    cmd_ls,    "Lists files in initramfs", "ls [--all | -a]"}
};

void cmd_help(int argc, char** argv)
{
  printf("Available commands:\n");
  for (size_t idx = 0; idx < NUM_COMMANDS; idx++)
    printf("\n%s - %s\n Usage: %s\n", commands[idx].name, commands[idx].description, commands[idx].usage);
}

void cmd_clear(int argc, char** argv)
{
  terminal_clear();
}

void cmd_echo(int argc, char** argv)
{
  for (int idx = 1; idx < argc; idx++)
    printf("%s ", argv[idx]);
  printf("\n");
}

void cmd_cat(int argc, char** argv)
{
  if (argc < 2)
  {
    printf("Usage: cat <filename>\n");
    return;
  }

  uintn_t file_size = 0;
  char *file_data = initramfs_read_file(argv[1], &file_size);
  if (file_data)
    for (uintn_t idx = 0; idx < file_size; idx++)
      printf("%c", file_data[idx]);
  else
    printf("cat: %s: No such file or directory\n", argv[1]);
}

void cmd_ls(int argc, char** argv)
{
  if (argc == 1)
    initramfs_list_files(1);
  else if (argc > 1 && (!strcmp(argv[1], "-a") || !strcmp(argv[1], "--all")))
    initramfs_list_files(0);
}
