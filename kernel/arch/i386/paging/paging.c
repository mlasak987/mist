#include "arch/i386/paging/paging.h"
#include <stdio.h>

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void paging_init(void)
{
  for(int i = 0; i < 1024; i++)
    page_directory[i] = 0x00000002; // 0x02 -> Read/Write = 1, Present = 0

  for(int i = 0; i < 1024; i++)
    first_page_table[i] = (i * 4096) | 3; // 0x03 -? Read/Write = 1, Present = 1

  page_directory[0] = ((uint32_t)first_page_table) | 3; // Present, R/W

  load_page_directory(page_directory);
  enable_paging();

  printf("[ INFO ] Mist: Paging enabled.\n");
}

void page_fault_handler(void)
{
  uint32_t faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

  printf("[ ERROR ] Mist: Page Fault! Trying to access unmapped address: 0x%x\n", faulting_address);

  asm volatile("cli");
  while(1) { asm volatile("hlt"); }
}
