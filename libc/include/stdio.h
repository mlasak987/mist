#ifndef _STDIO_H
#define _STDIO_H 1

#define EOF (-1)

#ifdef _cplusplus
extern "C" {
#endif

#include <stdint.h>

char *utoa(uint64_t value, char *str, int base);
char *itoa(int64_t value, char *str, int base);

int putchar(int ic);
int printf(const char * restrict format, ...);

#ifdef _cplusplus
}
#endif

#endif
