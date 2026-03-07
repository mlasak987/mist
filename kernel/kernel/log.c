#include "log.h"
#include <stdio.h>
#include <stdarg.h>

void log(log_level_t level, const char *component, const char *format, ...)
{
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
