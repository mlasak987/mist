#ifndef _PIT_H
#define _PIT_H

#include <stdint.h>
#include <types.h>

void pit_init(uint32_t frequency);
void sleep(uint32_t ms);

tick_t get_ticks();
void inc_ticks();

#endif
