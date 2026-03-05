#include "drivers/tty.h"
#include "drivers/font.h"
#include <stdint.h>
#include <stddef.h>

static uint32_t* fb_addr;
static size_t fb_width;
static size_t fb_height;
static size_t fb_pitch;

static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint32_t terminal_color_fg = 0xFFFFFFFF;
static uint32_t terminal_color_bg = 0x00000000;

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

void terminal_init(void* framebuffer_address, size_t width, size_t height, size_t pitch)
{
  fb_addr = (uint32_t*)framebuffer_address;
  fb_width = width;
  fb_height = height;
  fb_pitch = pitch;

  terminal_row = 0;
  terminal_column = 0;

  for (size_t y = 0; y < fb_height; y++)
    for (size_t x = 0; x < fb_width; x++)
    {
      uint32_t* pixel = (uint32_t*)((uint8_t*)fb_addr + (y * fb_pitch) + (x * 4));
      *pixel = terminal_color_bg;
    }
}

static void draw_char(char c, size_t x, size_t y)
{
  if (c < 32 || c >= 127) c = '?';
  const uint8_t* glyph = font8x8_basic[(uint8_t)c];

  for (size_t cy = 0; cy < CHAR_HEIGHT; cy++)
    for (size_t cx = 0; cx < CHAR_WIDTH; cx++)
    {
      uint32_t color = (glyph[cy] & (0x80 >> cx)) ? terminal_color_fg : terminal_color_bg;
      uint32_t* pixel = (uint32_t*)((uint8_t*)fb_addr + ((y + cy) * fb_pitch) + ((x + cx) * 4));
      *pixel = color;
    }
}

void terminal_putchar(char c)
{
  if (c == '\n')
  {
    terminal_column = 0;
    terminal_row++;
  }
  else if (c == '\b')
  {
    if (terminal_column > 0)
    {
      terminal_column--;
      draw_char(' ', terminal_column * CHAR_WIDTH, terminal_row * CHAR_HEIGHT);
    }
  }
  else
  {
    draw_char(c, terminal_column * CHAR_WIDTH, terminal_row * CHAR_HEIGHT);
    terminal_column++;
  }

  if (terminal_column * CHAR_WIDTH >= fb_width)
  {
    terminal_column = 0;
    terminal_row++;
  }

  if (terminal_row * CHAR_HEIGHT >= fb_height)
      terminal_row = 0;
}

void terminal_write(const char* data, size_t size)
{
  for (size_t idx = 0; idx < size; idx++)
    terminal_putchar(data[idx]);
}
