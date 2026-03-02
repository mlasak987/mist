#include "string.h"

char *strncpy(char *dst, const char *src, size_t num)
{
  size_t idx = 0;
  for (; idx < num && src[idx] != '\0'; idx++)
    dst[idx] = src[idx];
  for (; idx < num; idx++)
    dst[idx] = '\0';
  return dst;
}
