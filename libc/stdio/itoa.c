#include "stdio.h"

char *itoa(int value, char *str, int base) {
  if (base == 10 && value < 0) {
    str[0] = '-';
    utoa((unsigned int)(-value), str + 1, base);
    return str;
  }
  return utoa((unsigned int)value, str, base);
}
