#include <string.h>

#include <arch/limine.h>
#include "drivers/tty.h"

#include "kernel/main.h"
#include "kernel/init.h"

__attribute__((used, section(".limine_requests"))) static volatile uint64_t
    limine_base_revision[] = LIMINE_BASE_REVISION(5);

__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};

static void halt(void)
{
  for (;;)
    asm volatile("hlt");
}

void _start(void)
{
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1)
    halt();

  struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
  terminal_init(framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch);

  init();
  kernel_main();
  halt();
}
