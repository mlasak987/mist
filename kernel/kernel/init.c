#include <stdio.h>

#include "arch/i386/gdt/gdt.h"
#include "arch/i386/idt/idt.h"
#include "arch/i386/paging/paging.h"

#include "drivers/tty.h"
#include "drivers/ps2_kbd.h"
#include "mm/pmm.h"
#include "mm/kheap.h"
#include "shell/shell.h"

void init(uint32_t magic, multiboot_info_t *mbd)
{
  // terminal_init();
  // printf("[ INFO ] Mist: Booting Mist on %s...\n", __KERNEL_ARCH_);
  //
  // gdt_init();
  // idt_init();
  //
  // if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
  // {
  //   printf("[ ERROR ] Mist: Incorrect bootloader magic.\n");
  //   while (1);
  // }
  // pmm_init(mbd);
  // paging_init();
  // kheap_init();
  //
  // printf("[ INFO ] Mist: System initialized.\n");
  // printf("[ INFO ] Mist: Press 'ENTER' to enter the Mist Kernel Shell.\n");
  //
  // if (kbd_getchar() == '\n')
  // {
  //   kernel_shell_init();
  //   kernel_shell_loop();
  // }
}

