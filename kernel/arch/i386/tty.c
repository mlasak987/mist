#include <stddef.h>
#include <string.h>

#include "kernel/tty.h"
#include "arch/i386/io.h"
#include "vga.h"

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static void terminal_scroll()
{
  uint16_t empty = vga_entry(' ', terminal_color);
  memmove(terminal_buffer,
          terminal_buffer + VGA_WIDTH,
          VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(uint16_t));
  memsetw(terminal_buffer + VGA_WIDTH * (VGA_HEIGHT - 1),
          empty,
          VGA_WIDTH);
}

static void terminal_backspace()
{
  if (terminal_column > 0)
  {
    terminal_column--;
    terminal_putchar(' ');
    terminal_column--;
  }
}

static void terminal_newline()
{
  terminal_column = 0;
  if (++terminal_row == VGA_HEIGHT)
  {
    terminal_scroll();
    terminal_row--;
  }
}

static void terminal_enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

static void terminal_update_cursor(void)
{
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
 
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_init(void)
{
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = VGA_MEMORY;

  uint16_t empty = vga_entry(' ', terminal_color);
  memsetw(terminal_buffer, empty, VGA_WIDTH * VGA_HEIGHT);
  terminal_enable_cursor(14, 15);
}

void terminal_putchar(char c)
{
  unsigned char uc = (unsigned)c;

  if (uc == '\n')
    terminal_newline();
  else if (uc == '\b')
    terminal_backspace();
  else
  {
    const size_t idx = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[idx] = vga_entry(uc, terminal_color);
    if (++terminal_column == VGA_WIDTH)
      terminal_newline();
  }

  terminal_update_cursor();
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
