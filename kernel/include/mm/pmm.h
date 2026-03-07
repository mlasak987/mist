#ifndef _PMM_H
#define _PMM_H

#include <stdint.h>

#define PMM_BLOCK_SIZE 4096

void pmm_init(void);
void* pmm_alloc_block(void);
void pmm_free_block(void* ptr);

#endif
