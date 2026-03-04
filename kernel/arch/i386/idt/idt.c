#include "arch/i386/idt/idt.h"
#include "arch/i386/io.h"
#include "arch/i386/pic.h"
#include <string.h>

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

// Assembly functions
extern void isr33(void);
extern void isr14(void);
extern void idt_flush(uint32_t);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
  idt_entries[num].base_low = base & 0xFFFF;
  idt_entries[num].base_high = (base >> 16) & 0xFFFF;
  idt_entries[num].segment_selector = sel;
  idt_entries[num].reserved = 0;
  idt_entries[num].flags = flags;
}

void idt_init(void)
{
  idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
  idt_ptr.base = (uint32_t)&idt_entries;

  memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

  // Register interrupt handlers
  idt_set_gate(33, (uint32_t)isr33, 0x08, 0x8E); // Keyboard
  idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E); // Page fault

  pic_remap(0x20, 0x28);

  outb(0x21, 0xFD);
  outb(0xA1, 0xFF);

  idt_flush((uint32_t)&idt_ptr);

  // Set Interrupt Flag
  asm volatile("sti");
}
