#include <stdint.h>
#include <stdio.h>
#include "arch/x86_64/idt.h"
#include "drivers/ps2_kbd.h"

struct idt_entry
{
  uint16_t base_low;
  uint16_t sel;
  uint8_t  ist;
  uint8_t  flags;
  uint16_t base_mid;
  uint32_t base_high;
  uint32_t always0;
} __attribute__((packed));

struct idt_ptr
{
  uint16_t limit;
  uint64_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_flush(uint64_t);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr33(void);

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags)
{
  idt[num].base_low = base & 0xFFFF;
  idt[num].base_mid = (base >> 16) & 0xFFFF;
  idt[num].base_high = (base >> 32) & 0xFFFFFFFF;
  idt[num].sel = sel;
  idt[num].always0 = 0;
  idt[num].ist = 0;
  idt[num].flags = flags | 0x80; 
}

void idt_init(void)
{
  idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtp.base = (uint64_t)&idt;

  for (int i = 0; i < 256; i++)
    idt_set_gate(i, 0, 0, 0);

  idt_set_gate(0, (uint64_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint64_t)isr1, 0x08, 0x8E);
  idt_set_gate(2, (uint64_t)isr2, 0x08, 0x8E);
  idt_set_gate(3, (uint64_t)isr3, 0x08, 0x8E);
  idt_set_gate(33, (uint64_t)isr33, 0x08, 0x8E);

  idt_flush((uint64_t)&idtp);
  printf("[ %caOK%cr ] Mist: x86_64 IDT initialized.\n", 0x1B, 0x1B);
}

void isr_handler(uint64_t int_no, uint64_t err_code)
{
  if (int_no == 33) 
  {
    kbd_handler();
    return;
  }
  printf("[ %ccPANIC%cr ] Unhandled Exception CPU! Number: %ld, Error Code: %ld\n", 0x1B, 0x1B, int_no, err_code);
  while(1) asm volatile("hlt");
}
