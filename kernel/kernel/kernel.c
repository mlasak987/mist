#include "arch/i386/gdt/gdt.h"
#include "arch/i386/idt/idt.h"
#include "arch/i386/paging/paging.h"
#include "kernel/tty.h"
#include "kernel/pmm.h"
#include "stdio.h"

#include "arch/i386/ps2_kbd.h"

static void init_all(uint32_t magic, multiboot_info_t *mbd)
{
  terminal_init();
  gdt_init();
  idt_init();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
  {
      printf("kernel: Fatal error. Incorrect bootloader magic.\n");
      while (1);
  }
  pmm_init(mbd);
  paging_init();

  printf("kernel: Initialized.\n");
}

void kernel_main(uint32_t magic, uint32_t multiboot_info_addr)
{
  init_all(magic, (multiboot_info_t *)multiboot_info_addr);

  printf("\nWelcome to the OS!\n");
  printf("root@os:~# ");

  char command_buffer[128];
  int cmd_idx = 0;

  while (1)
  {
    char c = kbd_getchar();
    
    if (c == '\n')
    {
      command_buffer[cmd_idx] = '\0';
      printf("\nExecuting: %s\n", command_buffer);
      cmd_idx = 0;
      printf("root@os:~# ");
    } 
    else if (c == '\b')
    {
      if (cmd_idx > 0)
      {
        cmd_idx--;
        printf("\b");
      }
    }
    else
      if (cmd_idx < 127)
      {
        printf("%c", c);
        command_buffer[cmd_idx++] = c;
      }
  }
}
