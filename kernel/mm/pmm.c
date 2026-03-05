#include <string.h>
#include <stdio.h>

#include "mm/pmm.h"

// 131072 bytes * 8 bits = 1 048 576 blocks (4 KB) = 4 GB physical memory
#define BITMAP_SIZE 131072

static uint8_t pmm_bitmap[BITMAP_SIZE];
static uintptr_t max_blocks = 0;

extern uintptr_t _end;

static inline void bitmap_set(uintptr_t bit)
{
  pmm_bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void bitmap_clear(uintptr_t bit)
{
  pmm_bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline int bitmap_test(uintptr_t bit)
{
  return pmm_bitmap[bit / 8] & (1 << (bit % 8));
}

void pmm_init(multiboot_info_t* mbd)
{
  memset(pmm_bitmap, 0xFF, BITMAP_SIZE);

  if (!(mbd->flags & (1 << 6)))
  {
    printf("[ ERROR ] Mist: Missing memory map from bootloader!\n");
    return;
  }

  multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)(uintptr_t)mbd->mmap_addr;
  uintptr_t mmap_end = mbd->mmap_addr + mbd->mmap_length;

  while ((uintptr_t)mmap < mmap_end)
  {
    if (mmap->type == 1) // 1 => memory available
    {
      uintptr_t start_addr = mmap->base_addr_low;
      uintptr_t length = mmap->length_low;

      uintptr_t start_block = start_addr / PMM_BLOCK_SIZE;
      uintptr_t blocks = length / PMM_BLOCK_SIZE;

      for (uintptr_t i = 0; i < blocks; i++)
        bitmap_clear(start_block + i);

      uintptr_t end_block = start_block + blocks;
      if (end_block > max_blocks)
        max_blocks = end_block;
    }
    mmap = (multiboot_memory_map_t *) ((uintptr_t)mmap + mmap->size + sizeof(mmap->size));
  }

  uintptr_t kernel_end_addr = (uintptr_t)&_end;
  uintptr_t kernel_blocks = (kernel_end_addr / PMM_BLOCK_SIZE) + 1;

  for (uintptr_t i = 0; i < kernel_blocks; i++)
    bitmap_set(i);

  printf("[ INFO ] Mist: PMM initialized. Max blocks: %d (4KB)\n", max_blocks);
}

void* pmm_alloc_block(void)
{
  for (uintptr_t i = 0; i < max_blocks; i++)
    if (!bitmap_test(i))
    {
      bitmap_set(i);
      return (void*)(uintptr_t)(i * PMM_BLOCK_SIZE);
    }
  return NULL;
}

void pmm_free_block(void* ptr)
{
  uintptr_t addr = (uintptr_t)ptr;
  uintptr_t block = addr / PMM_BLOCK_SIZE;
  bitmap_clear(block);
}
