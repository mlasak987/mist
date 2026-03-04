#include "string.h"

void *memmove(void *dst, const void *src, size_t num)
{
  unsigned char *c_dst = (unsigned char *)dst;
  const unsigned char *c_src = (const unsigned char *)src;
  if (num == 0 || c_dst == c_src)
    return dst;
  if (c_dst < c_src)
    for (size_t idx = 0; idx < num; idx++)
      c_dst[idx] = c_src[idx];
  else
    for (size_t idx = num; idx != 0; idx--)
      c_dst[idx - 1] = c_src[idx - 1];
  return dst;
}
