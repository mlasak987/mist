#include "mm/kheap.h"
#include "mm/pmm.h"
#include <stdio.h>
#include <stdint.h>

static heap_block_t* heap_head = NULL;

void kheap_init(void)
{
  void* initial_page = pmm_alloc_block();
  for (int i = 1; i < 64; i++)
    pmm_alloc_block(); 

  heap_head = (heap_block_t*)initial_page;
  
  heap_head->size = (64 * PMM_BLOCK_SIZE) - sizeof(heap_block_t);
  heap_head->is_free = 1;
  heap_head->next = NULL;

  printf("[ INFO ] Mist: Kernel heap initialized on address 0x%x (256 KB)\n", (unsigned int)heap_head);
}

void* kmalloc(size_t size)
{
  if (size == 0) return NULL;
  size = (size + 3) & ~3;

  heap_block_t* current = heap_head;

  while (current != NULL)
  {
    if (current->is_free && current->size >= size)
    {
      if (current->size >= size + sizeof(heap_block_t) + 4)
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

  printf("[ ERROR ] Mist: Kernel heap is out of memory!\n");
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
