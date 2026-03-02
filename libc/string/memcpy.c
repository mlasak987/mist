#include "string.h"

void *memcpy(void *dst, const void *src, size_t num)
{
  unsigned char *c_dst = (unsigned char *)dst;
  const unsigned char *c_src = (unsigned char *)src;
  for (size_t idx = 0; idx < num; idx++)
    c_dst[idx] = c_src[idx];
  return dst;
}
