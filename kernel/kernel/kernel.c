#include "kernel/tty.h"

void kernel_main(void)
{
  terminal_init();
  terminal_writestr("Hello, World!\n");
  terminal_writestr("Kernel\n");
}
