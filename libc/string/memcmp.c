#include "string.h"

int memcmp(const void *astr, const void *bstr, size_t num)
{
  const unsigned char *c_astr = (const unsigned char *)astr;
  const unsigned char *c_bstr = (const unsigned char *)bstr;
  for (size_t idx = 0; idx < num; idx++)
    if (c_astr[idx] < c_bstr[idx])
      return -1;
    else if (c_astr[idx] > c_bstr[idx])
      return 1;
  return 0;
}
