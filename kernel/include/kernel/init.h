#ifndef _KERNEL_INIT_H
#define _KERNEL_INIT_H

#include <stdint.h>
#include "kernel/multiboot.h"

void init(uint32_t magic, multiboot_info_t *mbd);

#endif
