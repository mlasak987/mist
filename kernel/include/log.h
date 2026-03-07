#ifndef _LOG_H
#define _LOG_H

typedef enum
{
  LOG_DEBUG,
  LOG_INFO,
  LOG_OK,
  LOG_WARNING,
  LOG_ERROR,
  LOG_PANIC
} log_level_t;

void log(log_level_t level, const char *component, const char *format, ...);

#endif
