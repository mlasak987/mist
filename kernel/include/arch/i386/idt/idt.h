#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

typedef struct idt_entry_struct {
  uint16_t base_low;
  uint16_t segment_selector;
  uint8_t  reserved;
  uint8_t  flags;
  uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void idt_init(void);

#endif
