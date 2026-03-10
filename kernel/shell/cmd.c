#include <stdio.h>
#include <string.h>

#include "shell/shell.h"
#include "shell/cmd.h"
#include "drivers/tty.h"
#include "fs/initramfs.h"

#include "mm/kheap.h"
#include "arch/gdt.h"
#include "log.h"

extern void enter_usermode(uintn_t rip, uintn_t rsp);

const shell_command_t commands[] =
{
  {"help",  cmd_help,  "Prints available commands", "help"},
  {"clear", cmd_clear, "Clears the terminal screen", "clear"},
  {"echo",  cmd_echo,  "Prints given text to the console", "echo [text...]"},
  {"cat",   cmd_cat,   "Reads and prints file contents", "cat <filename>"},
  {"ls",    cmd_ls,    "Lists files in initramfs", "ls [--all | -a]"},
  {"test-ring3", cmd_ring3, "Enters User Space expecting Kernel Panic (Interrupt Code 14)", "test-ring3"}
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

static void user_mode_function(void)
{
  volatile int *trap = (int*)0x0;
  *trap = 42;
  while (1);
}

void cmd_ring3(int argc, char **argv)
{
  uint8_t* user_stack = kmalloc(4096);
  uint8_t* kernel_int_stack = kmalloc(8192);
  tss_set_kernel_stack((uintn_t)(kernel_int_stack + 8192));

  log(LOG_WARNING, "Mist", "Preparing to jump to User Space (Ring 3)...");
  enter_usermode((uintn_t)user_mode_function, (uintn_t)(user_stack + 4096));
  log(LOG_PANIC, "Mist", "Jump to User Space failed!");
}
