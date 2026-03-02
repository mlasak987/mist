#include "string.h"

size_t strlcpy(char *dst, const char *src, size_t num)
{
  size_t src_len = strlen(src);
  if (num)
  {
    size_t cpy_len = src_len >= num ? num - 1 : src_len;
    for (size_t idx = 0; idx < cpy_len; idx++)
      dst[idx] = src[idx];
    dst[cpy_len] = '\0';
  }
  return src_len;
}
