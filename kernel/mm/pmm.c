#include <string.h>
#include <stdio.h>
#include "mm/pmm.h"
#include "arch/limine.h"
#include "log.h"

__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = { .id = LIMINE_MEMMAP_REQUEST_ID, .revision = 0 };

__attribute__((used, section(".requests")))
static volatile struct limine_hhdm_request hhdm_request = { .id = LIMINE_HHDM_REQUEST_ID, .revision = 0 };

#define BITMAP_SIZE 131072
static uint8_t pmm_bitmap[BITMAP_SIZE];
static uintptr_t max_blocks = 0;
static uintptr_t hhdm_offset = 0;

static inline void bitmap_set(uintptr_t bit) { pmm_bitmap[bit / 8] |= (1 << (bit % 8)); }
static inline void bitmap_clear(uintptr_t bit) { pmm_bitmap[bit / 8] &= ~(1 << (bit % 8)); }
static inline int bitmap_test(uintptr_t bit) { return pmm_bitmap[bit / 8] & (1 << (bit % 8)); }

void pmm_init(void)
{
  memset(pmm_bitmap, 0xFF, BITMAP_SIZE);

  if (memmap_request.response == NULL)
  {
    log(LOG_PANIC, "Mist", "Missing memory map from Limine!");
    while(1);
  }

  if (hhdm_request.response != NULL)
    hhdm_offset = hhdm_request.response->offset;

  struct limine_memmap_response *mmap = memmap_request.response;

  for (uint64_t idx = 0; idx < mmap->entry_count; idx++)
  {
    struct limine_memmap_entry *entry = mmap->entries[idx];
    if (entry->type == LIMINE_MEMMAP_USABLE)
    {
      uintptr_t start_block = entry->base / PMM_BLOCK_SIZE;
      uintptr_t blocks = entry->length / PMM_BLOCK_SIZE;

      for (uintptr_t j = 0; j < blocks; j++)
        bitmap_clear(start_block + j);

      if (start_block + blocks > max_blocks)
        max_blocks = start_block + blocks;
    }
  }

  uint64_t mem = (max_blocks * 4) / 1000;
  log(LOG_OK, "Mist", "PMM initialized. Max blocks: %lu (%lu MB)", max_blocks, mem);
}

void* pmm_alloc_block(void)
{
  for (uintptr_t idx = 0; idx < max_blocks; idx++)
  {
    if (!bitmap_test(idx))
    {
      bitmap_set(idx);
      return (void*)(uintptr_t)(idx * PMM_BLOCK_SIZE + hhdm_offset);
    }
  }
  return NULL;
}

void pmm_free_block(void* ptr)
{
  uintptr_t addr = (uintptr_t)ptr - hhdm_offset;
  uintptr_t block = addr / PMM_BLOCK_SIZE;
  bitmap_clear(block);
}
