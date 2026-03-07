#include "stdio.h"
#include "string.h"
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>

static bool print(const char *data, size_t len)
{
  const unsigned char *u_data = (const unsigned char *)data;
  for (size_t idx = 0; idx < len; idx++)
    if (putchar(u_data[idx]) == EOF)
      return false;
  return true;
}

int vprintf(const char *restrict format, va_list parameters)
{
  int written = 0;

  while (*format != '\0')
  {
    if (*format != '%')
    {
      putchar(*format);
      format++;
      written++;
      continue;
    }

    format++;
    if (*format == '\0') break;

    bool is_long = false;
    if (*format == 'l')
    {
      is_long = true;
      format++;
    }

    switch (*format)
    {
      case '%':
        {
          putchar('%');
          written++;
          break;
        }
      case 'c':
        {
          char c = (char)va_arg(parameters, int);
          putchar(c);
          written++;
          break;
        }
      case 's':
        {
          const char *str = va_arg(parameters, const char *);
          if (!str) str = "(null)";
          size_t len = strlen(str);
          print(str, len);
          written += len;
          break;
        }
      case 'd':
      case 'i':
        {
          int64_t num = is_long ? va_arg(parameters, int64_t) : va_arg(parameters, int);
          char buffer[64];
          itoa(num, buffer, 10);
          size_t len = strlen(buffer);
          print (buffer, len);
          written += len;
          break;
        }
      case 'u':
        {
          uint64_t num = is_long ? va_arg(parameters, uint64_t) : va_arg(parameters, unsigned int);
          char buffer[64];
          utoa(num, buffer, 10);
          size_t len = strlen(buffer);
          print(buffer, len);
          written += len;
          break;
        }
      case 'x':
        {
          uint64_t num = is_long ? va_arg(parameters, uint64_t) : va_arg(parameters, unsigned int);
          char buffer[64];
          utoa(num, buffer, 16);
          size_t len = strlen(buffer);
          print(buffer, len);
          written += len;
          break;
        }
      case 'p':
        {
          uintptr_t ptr = va_arg(parameters, uintptr_t);
          print("0x", 2);
          char buffer[64];
          utoa(ptr, buffer, 16);
          size_t len = strlen(buffer);
          print(buffer, len);
          written += len + 2;
          break;
        }
      default:
        {
          putchar('%');
          if (is_long) putchar('l');
          putchar(*format);
          written += (is_long ? 3 : 2);
          break;
        }
    }
    format++;
  }

  return written;
}

int printf(const char *restrict format, ...)
{
  va_list parameters;
  va_start(parameters, format);
  int written = vprintf(format, parameters);
  va_end(parameters);
  return written;
}
