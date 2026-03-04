#include "string.h"

void *memset(void *ptr, int value, size_t num)
{
  unsigned char *c_ptr = (unsigned char *)ptr;
  while (num--)
    *c_ptr++ = (unsigned char)value;
  return ptr;
}
