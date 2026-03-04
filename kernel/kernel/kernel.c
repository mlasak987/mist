#include <stdio.h>
#include "kernel/init.h"

void kernel_main(uint32_t magic, uint32_t multiboot_info_addr)
{
  init(magic, (multiboot_info_t *)multiboot_info_addr);
}
