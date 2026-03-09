#include <stdint.h>
#include <stdio.h>

#include "arch/io.h"
#include "arch/pit.h"

#include "log.h"

volatile tick_t timer_ticks = 0;

void pit_init(uint32_t frequency)
{
  uint32_t divisor = 1193182 / frequency;
  outb(0x43, 0x36);
  outb(0x40, (uint8_t)(divisor & 0xFF));
  outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
  log(LOG_OK, "Mist", "PIT initialized at %d Hz", frequency);
}

tick_t get_ticks()
{
  return timer_ticks;
}

void inc_ticks()
{
  timer_ticks++;
}

void sleep(uint32_t ms)
{
  uint64_t start = timer_ticks;
  while (timer_ticks < start + ms)
    asm volatile ("hlt");
}
