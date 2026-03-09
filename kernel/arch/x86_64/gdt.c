#include <stdint.h>
#include <stdio.h>

#include "arch/gdt.h"

#include "log.h"

typedef struct gdt_entry_struct
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t  base_middle;
  uint8_t  access;
  uint8_t  granularity;
  uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr_struct
{
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) gdt_ptr_t;

gdt_entry_t gdt[5];
gdt_ptr_t gdt_p;

extern void gdt_flush(uint64_t);

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = (limit >> 16) & 0x0F;
  gdt[num].granularity |= gran & 0xF0;
  gdt[num].access = access;
}

void gdt_init(void)
{
  gdt_p.limit = (sizeof(gdt_entry_t) * 5) - 1;
  gdt_p.base = (uint64_t)&gdt;

  // 0: Null descriptor
  gdt_set_gate(0, 0, 0, 0, 0);
  // 1: Kernel Code Segment
  gdt_set_gate(1, 0, 0, 0x9A, 0x20); 
  // 2: Kernel Data Segment
  gdt_set_gate(2, 0, 0, 0x92, 0x00); 
  // 3: User Data Segment
  gdt_set_gate(3, 0, 0, 0xF2, 0x00); 
  // 4: User Code Segment
  gdt_set_gate(4, 0, 0, 0xFA, 0x20); 

  gdt_flush((uint64_t)&gdt_p);
  log(LOG_OK, "Mist", "x86_64 GDT initialized.");
}
