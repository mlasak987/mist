#include "string.h"

void *memsetw(void *ptr, uint16_t val, size_t num)
{
  uint16_t *u16_ptr = (uint16_t *)ptr;
  while (num--)
    *u16_ptr++ = val;
  return ptr;
}
