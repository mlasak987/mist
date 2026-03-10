#include <stdint.h>
#include <stdio.h>
#include "types.h"
#include "arch/gdt.h"
#include "log.h"

typedef struct
{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t  base_middle;
  uint8_t  access;
  uint8_t  granularity;
  uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
  uint16_t limit;
  uint64_t base;
} __attribute__((packed)) gdt_ptr_t;

typedef struct
{
  uint32_t reserved0;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint64_t reserved1;
  uint64_t ist1, ist2, ist3, ist4, ist5, ist6, ist7;
  uint64_t reserved2;
  uint16_t reserved3;
  uint16_t iopb_offset;
} __attribute__((packed)) tss_entry_t;

gdt_entry_t gdt[7];
gdt_ptr_t gdt_p;
tss_entry_t tss;

extern void gdt_flush(uint64_t);
extern void tss_flush(void); 

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

static void gdt_set_tss_gate(int num, uint64_t base, uint32_t limit)
{
  gdt_set_gate(num, (uint32_t)base, limit, 0x89, 0x00);

  gdt[num + 1].limit_low = (base >> 32) & 0xFFFF;
  gdt[num + 1].base_low = (base >> 48) & 0xFFFF;
  gdt[num + 1].base_middle = 0;
  gdt[num + 1].access = 0;
  gdt[num + 1].granularity = 0;
  gdt[num + 1].base_high = 0;
}

void gdt_init(void)
{
  gdt_p.limit = (sizeof(gdt_entry_t) * 7) - 1;
  gdt_p.base = (uint64_t)&gdt;

  gdt_set_gate(0, 0, 0, 0, 0);       // Null
  gdt_set_gate(1, 0, 0, 0x9A, 0x20); // Kernel Code
  gdt_set_gate(2, 0, 0, 0x92, 0x00); // Kernel Data
  gdt_set_gate(3, 0, 0, 0xF2, 0x00); // User Data  (DPL 3)
  gdt_set_gate(4, 0, 0, 0xFA, 0x20); // User Code  (DPL 3)
  
  __builtin_memset(&tss, 0, sizeof(tss_entry_t));

  tss.rsp0 = 0xFFFF800000000000; 
  tss.iopb_offset = sizeof(tss_entry_t);
  gdt_set_tss_gate(5, (uint64_t)&tss, sizeof(tss_entry_t) - 1);

  gdt_flush((uint64_t)&gdt_p);
  tss_flush();

  log(LOG_OK, "Mist", "x86_64 GDT and TSS initialized.");
}

void tss_set_kernel_stack(uintn_t stack_top)
{
  tss.rsp0 = stack_top;
}
