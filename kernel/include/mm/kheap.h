#ifndef _KHEAP_H
#define _KHEAP_H

#include <stdint.h>
#include <stddef.h>

typedef struct heap_block {
    size_t size;
    uint8_t is_free;
    struct heap_block* next;
} heap_block_t;

void kheap_init(void);
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
