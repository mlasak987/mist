#include <stdio.h>

#include "arch/x86_64/gdt.h"
#include "arch/x86_64/idt.h"
#include "arch/x86_64/pic.h"

#include "drivers/tty.h"
#include "drivers/ps2_kbd.h"
#include "mm/pmm.h"
#include "mm/kheap.h"
#include "shell/shell.h"

void init(void)
{
  printf("[ %c7INFO%cr ] Mist: Booting Mist on %s...\n", 0x1B, 0x1B, __KERNEL_ARCH_);

  gdt_init();
  idt_init();

  pic_remap();
  asm volatile ("sti");

  pmm_init();
  kheap_init();

  printf("[ %c7INFO%cr ] Mist: System initialized.\n", 0x1B, 0x1B);
  printf("[ %c7INFO%cr ] Mist: Press 'ENTER' to enter the Mist Kernel Shell.\n", 0x1B, 0x1B);
  
  if (kbd_getchar() == '\n')
  {
    kernel_shell_init();
    kernel_shell_loop();
  }
}

