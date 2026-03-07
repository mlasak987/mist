#include "stdio.h"
#include <stdint.h>

static void reverse(char *str, int len) {
  int start = 0, end = len - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}

char *utoa(uint64_t value, char *str, int base) {
  int idx = 0;
  if (!value) {
    str[idx++] = '0';
    str[idx] = '\0';
    return str;
  }
  while (value) {
    uint64_t rem = value % base;
    str[idx++] = rem > 9 ? 'a' + (rem - 10) : '0' + rem;
    value /= base;
  }
  str[idx] = '\0';
  reverse(str, idx);
  return str;
}
