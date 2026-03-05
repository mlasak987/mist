#include <stdio.h>
#include <string.h>

#include "shell/shell.h"
#include "drivers/tty.h"
#include "drivers/ps2_kbd.h"
#include "mm/kheap.h"

#define MAX_ARGS 16

void kernel_shell_init()
{
  //terminal_init();
  printf(" __  __ _     _   \n"
                 "|  \\/  (_)___| |_ \n"
                 "| |\\/| | / __| __|\n"
                 "| |  | | \\__ \\ |_ \n"
                 "|_|  |_|_|___/\\__|\n"
                 "                  \n"
                 "Welcome to the Mist Kernel Shell.\n"
                 "Mist v%s (%s) - MKS v%s\n"
                 "Type 'help' to begin.\n", __OS_VERSION_, __KERNEL_ARCH_, __KERNEL_SHELL_VERSION_);
}

static void execute_command(char* input)
{
  char** argv = (char**)kmalloc(MAX_ARGS * sizeof(char*));
  if (!argv)
  {
    printf("[ ERROR ] MKS: Out of memory during command parsing!\n");
    return;
  }

  int argc = 0;
  char* current = input;

  while (*current != '\0' && argc < MAX_ARGS)
  {
    while (*current == ' ') current++;
    if (*current == '\0') break;

    argv[argc++] = current;
    while (*current != ' ' && *current != '\0') current++;

    if (*current != '\0')
    {
      *current = '\0';
      current++;
    }
  }

  if (argc > 0)
  {
    if (!strcmp(argv[0], "help"))
      printf("Available commands: help, clear, echo\n");
    //else if (!strcmp(argv[0], "clear"))
      //terminal_init();
    else if (!strcmp(argv[0], "echo"))
    {
      for (int i = 1; i < argc; i++)
        printf("%s ", argv[i]);
      printf("\n");
    } 
    else
      printf("Unknown command: %s\n", argv[0]);
  }

  kfree(argv);
}

void kernel_shell_loop()
{
  char* command_buffer = (char*)kmalloc(256);
  if (!command_buffer)
  {
    printf("[ ERROR ] MKS: Failed to allocate command buffer.\n");
    while(1);
  }

  int cmd_idx = 0;
  printf(">> ");

  while (1)
  {
    char c = kbd_getchar();
    
    if (c == '\n')
    {
      command_buffer[cmd_idx] = '\0';
      printf("\n");
      
      execute_command(command_buffer);
      
      cmd_idx = 0;
      printf(">> ");
    } 
    else if (c == '\b' && cmd_idx > 0)
    {
      cmd_idx--;
      printf("\b");
    }
    else if (cmd_idx < 255)
    {
      printf("%c", c);
      command_buffer[cmd_idx++] = c;
    }
  }

  kfree(command_buffer);
}
