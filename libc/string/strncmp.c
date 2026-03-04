#include "string.h"

int strncmp(const char *astr, const char *bstr, size_t num)
{
  if (!num) return 0;
  const unsigned char *c_astr = (const unsigned char *)astr;
  const unsigned char *c_bstr = (const unsigned char *)bstr;
  while (num > 1 && *c_astr && (*c_astr == *c_bstr))
  {
    c_astr++;
    c_bstr++;
    num--;
  }
  return *c_astr - *c_bstr;
}
