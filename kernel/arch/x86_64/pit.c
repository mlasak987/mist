#include "arch/x86_64/io.h"
#include <stdint.h>
#include <stdio.h>

volatile uint64_t timer_ticks = 0;

void pit_init(uint32_t frequency)
{
  uint32_t divisor = 1193182 / frequency;
  outb(0x43, 0x36);
  outb(0x40, (uint8_t)(divisor & 0xFF));
  outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
  printf("[ %caOK%cr ] Mist: PIT initialized at %d Hz\n", 0x1B, 0x1B, frequency);
}

void sleep(uint32_t ms)
{
  uint64_t start = timer_ticks;
  while (timer_ticks < start + ms)
    asm volatile ("hlt");
}
