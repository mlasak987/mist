#include "stdio.h"
#include <stdint.h>

char *itoa(int64_t value, char *str, int base) {
  if (base == 10 && value < 0) {
    str[0] = '-';
    utoa((uint64_t)(-value), str + 1, base);
    return str;
  }
  return utoa((uint64_t)value, str, base);
}
