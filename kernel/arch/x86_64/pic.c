#include "arch/x86_64/io.h"
#include <stdint.h>
#include <stdio.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01
#define ICW4_8086    0x01

#define PIC1_OFFSET  0x20
#define PIC2_OFFSET  0x28

void pic_remap(void)
{
  inb(PIC1_DATA);
  inb(PIC2_DATA);

  outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  io_wait();

  outb(PIC1_DATA, PIC1_OFFSET);
  io_wait();
  outb(PIC2_DATA, PIC2_OFFSET);
  io_wait();

  outb(PIC1_DATA, 4);
  io_wait();
  outb(PIC2_DATA, 2);
  io_wait();

  outb(PIC1_DATA, ICW4_8086);
  io_wait();
  outb(PIC2_DATA, ICW4_8086);
  io_wait();

  outb(PIC1_DATA, 0xFD);
  outb(PIC2_DATA, 0xFF);

  printf("[ %caOK%cr ] Mist: Legacy PIC remapped to IDT 0x20\n", 0x1B, 0x1B);
}

void pic_send_eoi(uint8_t irq)
{
  if (irq >= 8)
    outb(PIC2_COMMAND, 0x20);
  outb(PIC1_COMMAND, 0x20);
}
