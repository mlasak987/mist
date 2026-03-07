#include <stdio.h>
#include "log.h"

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
  log(LOG_INFO, "Mist", "Booting Mist on %s...", __KERNEL_ARCH_);

  gdt_init();
  idt_init();

  pic_remap();
  asm volatile ("sti");

  pmm_init();
  kheap_init();

  log(LOG_INFO, "Mist", "System initialized.");
  log(LOG_INFO, "Mist", "Press 'ENTER' to enter the Mist Kernel Shell.");

  if (kbd_getchar() == '\n')
  {
    kernel_shell_init();
    kernel_shell_loop();
  }
}

