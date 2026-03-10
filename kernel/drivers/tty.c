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

static ansi_state_t ansi_state;
#define MAX_ANSI_ARGS 8
static int ansi_args[MAX_ANSI_ARGS];
static int ansi_arg_count = 0;
static int ansi_current_arg = 0;

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

static const uint32_t ansi_colors[8] = {
  0xFF000000, // 30 (40): Black
  0xFFAA0000, // 31 (41): Red
  0xFF00AA00, // 32 (42): Green
  0xFFFFAA00, // 33 (43): Yellow (Gold)
  0xFF0000AA, // 34 (44): Blue
  0xFFAA00AA, // 35 (45): Magenta
  0xFF00AAAA, // 36 (46): Cyan
  0xFFAAAAAA  // 37 (47): Light Gray
};

static const uint32_t ansi_bright_colors[8] = {
  0xFF555555, // 90: Dark Gray
  0xFFFF5555, // 91: Light Red
  0xFF55FF55, // 92: Light Green
  0xFFFFFF55, // 93: Light Yellow
  0xFF5555FF, // 94: Light Blue
  0xFFFF55FF, // 95: Light Magenta
  0xFF55FFFF, // 96: Light Cyan
  0xFFFFFFFF  // 97: White
};

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
      uint32_t* pixel = (uint32_t*)((uint8_t*)fb_addr + ((y + cy) * fb_pitch) + ((x + cx) * 4));
      uint32_t g_color = (glyph[cy] & (0x80 >> cx)) ? color : terminal_color_bg;
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

void terminal_draw_cursor(bool visible)
{
  char cursor_char = visible ? '_' : ' ';
  terminal_draw_char(cursor_char, terminal_x * CHAR_WIDTH, terminal_y * CHAR_HEIGHT, terminal_color_fg);
}

static void terminal_scroll()
{
  memmove((uint8_t *)fb_addr,
          (uint8_t *)fb_addr + (CHAR_HEIGHT * fb_pitch), 
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

static void terminal_apply_ansi()
{
  bool bold = false;

  if (ansi_arg_count == 0)
  {
    ansi_args[0] = 0;
    ansi_arg_count = 1;
  }

  for (int i = 0; i < ansi_arg_count; i++)
  {
    int code = ansi_args[i];

    if (code == 0)
    {
      terminal_color_fg = 0xFFFFFFFF;
      terminal_color_bg = 0x00000000;
      bold = false;
    } 
    else if (code == 1)
      bold = true;
    else if (code >= 30 && code <= 37)
      terminal_color_fg = bold ? ansi_bright_colors[code - 30] : ansi_colors[code - 30];
    else if (code >= 40 && code <= 47)
      terminal_color_bg = ansi_colors[code - 40];
    else if (code >= 90 && code <= 97)
      terminal_color_fg = ansi_bright_colors[code - 90];
  }
}

void terminal_putchar(char c)
{
  switch (ansi_state)
  {
    case ANSI_STATE_NORMAL:
      if (c == 0x1B)
        ansi_state = ANSI_STATE_ESC;
      else
      {
        switch (c)
        {
          case '\n':
            terminal_newline();
            break;
          case '\b':
            if (terminal_x > 0) terminal_x--;
            terminal_draw_char(' ', terminal_x * CHAR_WIDTH, terminal_y * CHAR_HEIGHT, terminal_color_fg);
            break;
          default:
            terminal_draw_char(c, terminal_x * CHAR_WIDTH, terminal_y * CHAR_HEIGHT, terminal_color_fg);
            if (++terminal_x * CHAR_WIDTH >= fb_width)
              terminal_newline();
        }
      }
      break;

    case ANSI_STATE_ESC:
      if (c == '[')
      {
        ansi_state = ANSI_STATE_CSI;
        ansi_arg_count = 0;
        ansi_current_arg = 0;
      }
      else
        ansi_state = ANSI_STATE_NORMAL;
      break;

    case ANSI_STATE_CSI:
      if (c >= '0' && c <= '9')
        ansi_current_arg = ansi_current_arg * 10 + (c - '0');
      else if (c == ';')
      {
        if (ansi_arg_count < MAX_ANSI_ARGS)
          ansi_args[ansi_arg_count++] = ansi_current_arg;
        ansi_current_arg = 0;
      }
      else if (c == 'm')
      {
        if (ansi_arg_count < MAX_ANSI_ARGS)
          ansi_args[ansi_arg_count++] = ansi_current_arg;
        terminal_apply_ansi();
        ansi_state = ANSI_STATE_NORMAL;
      }
      else
        ansi_state = ANSI_STATE_NORMAL;
      break;
  }
}

void terminal_write(const char* data, size_t size)
{
  for (size_t idx = 0; idx < size; idx++)
    terminal_putchar(data[idx]);
}
