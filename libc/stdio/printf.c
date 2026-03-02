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

int printf(const char *restrict format, ...)
{
  va_list parameters;
  va_start(parameters, format);

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

    format++; // Skip %
    if (*format == '\0') break;

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
          int num = va_arg(parameters, int);
          char buffer[32];
          itoa(num, buffer, 10);
          size_t len = strlen(buffer);
          print (buffer, len);
          written += len;
          break;
        }
      case 'x':
        {
          unsigned int num = va_arg(parameters, unsigned int);
          char buffer[32];
          utoa(num, buffer, 16);
          size_t len = strlen(buffer);
          print(buffer, len);
          written += len;
          break;
        }
      default:
        {
          putchar('%');
          putchar(*format);
          written += 2;
          break;
        }
    }
    format++;
  }

  va_end(parameters);
  return written;
}
