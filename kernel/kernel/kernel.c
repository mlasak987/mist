#include <stdio.h>
#include "kernel/init.h"

void kernel_main()
{
  //init(magic, (multiboot_info_t *)multiboot_info_addr);
  printf("Hello, World!");
}
