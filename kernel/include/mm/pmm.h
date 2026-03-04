#ifndef _PMM_H
#define _PMM_H

#include <stdint.h>
#include "kernel/multiboot.h"

#define PMM_BLOCK_SIZE 4096

void pmm_init(multiboot_info_t* mbd);
void* pmm_alloc_block(void);
void pmm_free_block(void* ptr);

#endif
