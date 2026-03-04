#ifndef _STDIO_H
#define _STDIO_H 1

#define EOF (-1)

#ifdef _cplusplus
extern "C" {
#endif

char *utoa(unsigned int value, char *str, int base);
char *itoa(int value, char *str, int base);

int putchar(int ic);
int printf(const char * restrict format, ...);

#ifdef _cplusplus
}
#endif

#endif
