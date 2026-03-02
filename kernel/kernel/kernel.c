#include "kernel/tty.h"
#include "stdio.h"

void kernel_main(void)
{
  terminal_init();
  int a = -50;
  for (int i = 0; i < 30; i++)
  {
    printf("For: %d\n", a++);
  }
}
