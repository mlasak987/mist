#include "arch/i386/gdt/gdt.h"
#include "arch/i386/idt/idt.h"
#include "kernel/tty.h"
#include "stdio.h"

void kernel_main(void)
{
  terminal_init();
  gdt_init();
  idt_init();

  printf("kernel: Initialized.\n");
  while (1);
}
