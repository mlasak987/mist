#include <stdio.h>
#include <types.h>

#include "arch/pit.h"

#include "log.h"

void log(log_level_t level, const char *component, const char *format, ...)
{
  uintn_t sec = get_ticks() / 1000;
  uintn_t ms = get_ticks() % 1000;
  printf("\x1b[90m[%5lu.%03lu]\x1b[0m ", sec, ms);

  switch (level)
  {
    #if defined (LOG_LEVEL_DEBUG)
    case LOG_DEBUG: printf("\x1b[94m[DEBUG ]\x1b[0m "); break;
    case LOG_INFO:  printf("\x1b[37m[ INFO ]\x1b[0m "); break;
    case LOG_OK:    printf("\x1b[32m[  OK  ]\x1b[0m "); break;
    #elif defined (LOG_LEVEL_INFO)
    case LOG_INFO:  printf("\x1b[37m[ INFO ]\x1b[0m "); break;
    case LOG_OK:    printf("\x1b[32m[  OK  ]\x1b[0m "); break;
    #endif
    case LOG_WARNING: printf("\x1b[33m[ WARN ]\x1b[0m "); break;
    case LOG_ERROR:   printf("\x1b[31m[ERROR ]\x1b[0m "); break;
    case LOG_PANIC:   printf("\x1b[41;97m[PANIC ]\x1b[0m "); break;
    default:
      return;
  }

  printf("%s: ", component);

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  
  printf("\n");
}
