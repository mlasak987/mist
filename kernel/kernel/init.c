#include <stdio.h>
#include "log.h"

#include "arch/gdt.h"
#include "arch/idt.h"
#include "arch/pic.h"
#include "arch/pit.h"

#include "drivers/tty.h"
#include "drivers/ps2_kbd.h"
#include "mm/pmm.h"
#include "mm/kheap.h"
#include "shell/shell.h"
#include "fs/initramfs.h"

void init(void)
{
  log(LOG_INFO, "Mist", "Booting Mist on %s...", __KERNEL_ARCH_);
  log(LOG_DEBUG, "Init", "Debug is on.");

  gdt_init();
  idt_init();

  pit_init(1000);

  pmm_init();
  kheap_init();
  initramfs_init();

  log(LOG_INFO, "Mist", "System initialized.");
}

