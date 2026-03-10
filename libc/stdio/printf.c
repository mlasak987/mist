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

    format++; // Skip %
    if (*format == '\0') break;

    bool pad_zero = false;
    if (*format == '0')
    {
      pad_zero = true;
      format++;
    }

    int width = 0;
    while (*format >= '0' && *format <= '9')
    {
      width = width * 10 + (*format - '0');
      format++;
    }

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
        putchar((char)va_arg(parameters, int));
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
      case 'u':
      case 'x':
      {
        uint64_t num;
        if (*format == 'd' || *format == 'i')
          num = is_long ? va_arg(parameters, int64_t) : va_arg(parameters, int);
        else
          num = is_long ? va_arg(parameters, uint64_t) : va_arg(parameters, unsigned int);

        char buffer[64];
        if (*format == 'd' || *format == 'i') itoa(num, buffer, 10);
        else utoa(num, buffer, (*format == 'x' ? 16 : 10));

        int len = strlen(buffer);
        while (len < width)
        {
          putchar(pad_zero ? '0' : ' ');
          len++;
          written++;
        }
        print(buffer, strlen(buffer));
        written += strlen(buffer);
        break;
      }
      case 'p':
      {
        uintptr_t ptr = va_arg(parameters, uintptr_t);
        print("0x", 2);
        char buffer[64];
        utoa(ptr, buffer, 16);
        print(buffer, strlen(buffer));
        written += strlen(buffer) + 2;
        break;
      }
      default:
        putchar(*format);
        written++;
        break;
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
