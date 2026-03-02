#include <stddef.h>
#include <string.h>

#include "kernel/tty.h"

#include "vga.h"

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_init(void)
{
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = VGA_MEMORY;

  uint16_t empty = vga_entry(' ', terminal_color);
  memsetw(terminal_buffer, empty, VGA_WIDTH * VGA_HEIGHT);
}

void terminal_scroll()
{
  uint16_t empty = vga_entry(' ', terminal_color);
  memmove(terminal_buffer,
          terminal_buffer + VGA_WIDTH,
          VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(uint16_t));
  memsetw(terminal_buffer + VGA_WIDTH * (VGA_HEIGHT - 1),
          empty,
          VGA_WIDTH);
}

void terminal_newline()
{
  terminal_column = 0;
  if (++terminal_row == VGA_HEIGHT)
  {
    terminal_scroll();
    terminal_row--;
  }
}

void terminal_putchar(char c)
{
  unsigned char uc = (unsigned)c;

  if (uc == '\n')
    terminal_newline();
  else
  {
    const size_t idx = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[idx] = vga_entry(uc, terminal_color);
    if (++terminal_column == VGA_WIDTH)
      terminal_newline();
  }
}

void terminal_write(const char *data, size_t size)
{
  for (size_t idx = 0; idx < size; idx++)
    terminal_putchar(data[idx]);
}

void terminal_writestr(const char *data)
{
  size_t len = strlen(data);
  terminal_write(data, len);
}
