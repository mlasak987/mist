#include "string.h"

int strcmp(const char *astr, const char *bstr)
{
  const unsigned char *c_astr = (const unsigned char *)astr;
  const unsigned char *c_bstr = (const unsigned char *)bstr;
  while (*c_astr && (*c_astr == *c_bstr))
  {
    c_astr++;
    c_bstr++;
  }
  return *c_astr - *c_bstr;
}
