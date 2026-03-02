#ifndef _STRING_H
#define _STRING_H 1

#include "stddef.h"
#include "stdint.h"

#ifdef _cplusplus
extern "C" {
#endif

void *memset(void *ptr, int val, size_t num);
void *memsetw(void *ptr, uint16_t val, size_t num);
size_t strlen(const char *);

#ifdef _cplusplus
}
#endif

#endif
