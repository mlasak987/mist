#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "arch/io.h"
#include "drivers/ps2_kbd.h"

#define KBD_BUFFER_SIZE 256

static char kbd_buffer[KBD_BUFFER_SIZE];
static volatile int kbd_head = 0;
static volatile int kbd_tail = 0;

// 72 - up, 75 - left, 80 - down, 77 - right

const char scancode_to_ascii[] = {
  0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
  0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
  '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  '-', 0, 0, 0, '+', 0, 0, 0, 0, 127, 0, 0, 0, 0, 0, 0
};

void kbd_handler(void)
{
  uint8_t scancode = inb(0x60);

  if (!(scancode & 0x80))
  {
    if (scancode < sizeof(scancode_to_ascii))
    {
      char c = scancode_to_ascii[scancode];
      if (c != 0)
      {
        int next_head = (kbd_head + 1) % KBD_BUFFER_SIZE;

        if (next_head != kbd_tail)
        {
          kbd_buffer[kbd_head] = c;
          kbd_head = next_head;
        }
      }
    }
  }
  outb(0x20, 0x20);
}

bool kbd_has_key(void)
{
  return kbd_head != kbd_tail;
}

char kbd_getchar(void)
{
  while (kbd_head == kbd_tail)
    asm volatile("hlt");
    
  char c = kbd_buffer[kbd_tail];
  kbd_tail = (kbd_tail + 1) % KBD_BUFFER_SIZE;
  return c;
}
