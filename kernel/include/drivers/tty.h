#ifndef _TTY_H
#define _TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_init(void* framebuffer_address, size_t width, size_t height, size_t pitch);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_clear();

#endif
