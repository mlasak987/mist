#include "arch/i386/gdt/gdt.h"

// Assembly function
extern void gdt_flush(uint32_t);

gdt_entry_t gdt[3];
gdt_ptr_t gp;

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  gdt[num].limit_low = (limit & 0xFFFF);

  gdt[num].granularity = ((limit >> 16) & 0x0F);
  gdt[num].granularity |= (gran & 0xF0);

  gdt[num].access = access;
}

void gdt_init(void)
{
  gp.limit = (sizeof(gdt_entry_t) * 3) - 1;
  gp.base = (uint32_t)&gdt;

  // Null
  gdt_set_gate(0, 0, 0, 0, 0);

  // Code (4GB)
  // 0x9A = 10011010 (Present=1, Privilege=00, Type=Code/Exec/Read)
  // 0xCF = 11001111 (Granularity=4KB, 32-bit operation, Limit=F)
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

  // Data (4GB)
  // 0x92 = 10010010 (Present=1, Privilege=00, Type=Data/Read/Write)
  // 0xCF = 10010010 (Present=1, Privilege=00, Type=Data/Read/Write)
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

  gdt_flush((uint32_t)&gp);
}
