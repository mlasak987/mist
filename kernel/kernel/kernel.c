#include "kernel/tty.h"

void kernel_main(void)
{
  terminal_init();
  for (;;)
  {
    terminal_writestr("Hello, World!\n");
    terminal_writestr("Kernel\n");
    terminal_writestr("Test\n");
  }
}
