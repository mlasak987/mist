#include <stdio.h>
#include <stdint.h>
#include <types.h>

#include "mm/kheap.h"
#include "mm/pmm.h"

#include "log.h"

static heap_block_t* heap_head = NULL;

#if defined __32b__
  static int all = 4;
#elif defined __64b__
  static int all = 16;
#endif

void kheap_init(void)
{
  void* initial_page = pmm_alloc_block();
  for (int i = 1; i < 64; i++)
    pmm_alloc_block(); 

  heap_head = (heap_block_t*)initial_page;
  
  heap_head->size = (64 * PMM_BLOCK_SIZE) - sizeof(heap_block_t);
  heap_head->is_free = 1;
  heap_head->next = NULL;

  log(LOG_OK, "Mist", "Kernel heap initialized on address %p (256 KB)", (void *)heap_head);
}

void* kmalloc(size_t size)
{
  if (size == 0) return NULL;
  size = (size + (all - 1)) & ~(all - 1);

  heap_block_t* current = heap_head;

  while (current != NULL)
  {
    if (current->is_free && current->size >= size)
    {
      if (current->size >= size + sizeof(heap_block_t) + all)
      {
        heap_block_t* new_block = (heap_block_t*)((uint8_t*)current + sizeof(heap_block_t) + size);
        new_block->size = current->size - size - sizeof(heap_block_t);
        new_block->is_free = 1;
        new_block->next = current->next;

        current->size = size;
        current->next = new_block;
      }

      current->is_free = 0;
      return (void*)((uint8_t*)current + sizeof(heap_block_t));
    }
    current = current->next;
  }

  log(LOG_PANIC, "Mist", "Kernel heap is out of memory!");
  return NULL;
}

void kfree(void* ptr)
{
  if (!ptr) return;

  heap_block_t* block = (heap_block_t*)((uint8_t*)ptr - sizeof(heap_block_t));
  block->is_free = 1;

  heap_block_t* current = heap_head;
  while (current != NULL && current->next != NULL)
    if (current->is_free && current->next->is_free)
    {
      current->size += sizeof(heap_block_t) + current->next->size;
      current->next = current->next->next;
    }
    else
      current = current->next;
}
