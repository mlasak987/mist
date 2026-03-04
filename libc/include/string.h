#ifndef _STRING_H
#define _STRING_H 1

#include "stddef.h"
#include "stdint.h"

#ifdef _cplusplus
extern "C" {
#endif

void *memset(void *ptr, int val, size_t num);
void *memsetw(void *ptr, uint16_t val, size_t num);
void *memcpy(void * dst, const void *src, size_t num);
void *memmove(void * dst, const void *src, size_t num);
int memcmp(const void *astr, const void *bstr, size_t num);

size_t strlen(const char *str);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t num);
size_t strlcpy(char *dst, const char *src, size_t num);
int strcmp(const char *astr, const char *bstr);
int strncmp(const char *astr, const char *bstr, size_t num);

#ifdef _cplusplus
}
#endif

#endif
