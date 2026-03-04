#include "string.h"

char *strcpy(char *dst, const char *src)
{
  char *org = dst;
  while ((*dst++ = *src++));
  return org;
}
