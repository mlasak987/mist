#include "drivers/tty.h"
#include "drivers/font.h"
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint32_t* fb_addr;
static size_t fb_width;
static size_t fb_height;
static size_t fb_pitch;

static size_t terminal_x = 0;
static size_t terminal_y = 0;
static uint32_t terminal_color_fg = 0xFFFFFFFF;
static uint32_t terminal_color_bg = 0x00000000;
static bool terminal_expect_color_code = false;

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

void terminal_clear()
{
  for (size_t y = 0; y < fb_height; y++)
    for (size_t x = 0; x < fb_width; x++)
    {
      uint32_t* pixel = (uint32_t*)((uint8_t*)fb_addr + (y * fb_pitch) + (x * 4));
      *pixel = terminal_color_bg;
    }

  terminal_x = 0;
  terminal_y = 0;
}

static void terminal_draw_char(char c, size_t x, size_t y, uint32_t color)
{
  if (c < 32 || c >= 127) c = '?';
  const uint8_t* glyph = font8x8_basic[(uint8_t)c];

  for (size_t cy = 0; cy < CHAR_HEIGHT; cy++)
    for (size_t cx = 0; cx < CHAR_WIDTH; cx++)
    {
      uint32_t g_color = (glyph[cy] & (0x80 >> cx)) ? color : terminal_color_bg;
      uint32_t* pixel = (uint32_t*)((uint8_t*)fb_addr + ((y + cy) * fb_pitch) + ((x + cx) * 4));
      *pixel = g_color;
    }
}

void terminal_init(void* framebuffer_address, size_t width, size_t height, size_t pitch)
{
  fb_addr = (uint32_t*)framebuffer_address;
  fb_width = width;
  fb_height = height;
  fb_pitch = pitch;

  terminal_y = 0;
  terminal_x = 0;
  
  terminal_clear();
}

static void terminal_scroll()
{
  memmove((uint8_t *)fb_addr,
          (uint8_t *)fb_addr + (CHAR_WIDTH * fb_pitch), 
          fb_pitch * (fb_height - CHAR_HEIGHT));

  uint32_t *clear = (uint32_t *)((uint8_t *)fb_addr + ((fb_height - CHAR_HEIGHT) * fb_pitch));
  for (size_t idx = 0; idx < (fb_pitch * CHAR_HEIGHT) / 4; idx++)
    clear[idx] = terminal_color_bg;
}

static void terminal_newline()
{
  terminal_x = 0;
  if (++terminal_y * CHAR_HEIGHT >= fb_height)
  {
    terminal_scroll();
    terminal_y--;
  }
}

static void terminal_set_color_by_code(char c)
{
  switch (c)
  {
    case '0': terminal_color_fg = 0xFF000000; break; // Black
    case '1': terminal_color_fg = 0xFF0000AA; break; // Blue
    case '2': terminal_color_fg = 0xFF00AA00; break; // Green
    case '3': terminal_color_fg = 0xFF00AAAA; break; // Cyan
    case '4': terminal_color_fg = 0xFFAA0000; break; // Red
    case '5': terminal_color_fg = 0xFFAA00AA; break; // Magenta
    case '6': terminal_color_fg = 0xFFFFAA00; break; // Gold
    case '7': terminal_color_fg = 0xFFAAAAAA; break; // Light Gray
    case 'a': terminal_color_fg = 0xFF55FF55; break; // Light Green
    case 'c': terminal_color_fg = 0xFFFF5555; break; // Light Red
    case 'e': terminal_color_fg = 0xFFFFFF55; break; // Yellow
    case 'f': terminal_color_fg = 0xFFFFFFFF; break; // White
    case 'r': terminal_color_fg = 0xFFFFFFFF; break; // Reset
    default:  terminal_color_fg = 0xFFFFFFFF; break;
  }
}
void terminal_putchar(char c)
{
  if (terminal_expect_color_code)
  {
    terminal_set_color_by_code(c);
    terminal_expect_color_code = false;
    return;
  }

  switch (c)
  {
    case '\n':
      terminal_newline();
      break;
    case '\b':
      terminal_x--;
      terminal_draw_char(' ', terminal_x * CHAR_WIDTH, terminal_y * CHAR_HEIGHT, terminal_color_fg);
      break;
    case 0x1B:
      terminal_expect_color_code = true;
      break;
    default:
      terminal_draw_char(c, terminal_x * CHAR_WIDTH, terminal_y * CHAR_HEIGHT, terminal_color_fg);
      if (++terminal_x * CHAR_WIDTH >= fb_width)
        terminal_newline();

  }
}

void terminal_write(const char* data, size_t size)
{
  for (size_t idx = 0; idx < size; idx++)
    terminal_putchar(data[idx]);
}
