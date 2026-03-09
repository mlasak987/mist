#ifndef _PIT_H
#define _PIT_H

#include <stdint.h>

extern volatile uint64_t timer_ticks;

void pit_init(uint32_t frequency);
void sleep(uint32_t ms);

#endif
