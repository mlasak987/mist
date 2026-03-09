#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

#if defined __i686__
#define __32b__
#elif defined __x86_64__
#define __64b__
#else
#define __32b__
#endif

#if defined __32b__
typedef int32_t intn_t;
typedef uint32_t uintn_t;
typedef uint32_t tick_t;
#elif defined __64b__
typedef int64_t intn_t;
typedef uint64_t uintn_t;
typedef uint64_t tick_t;
#endif

#endif
