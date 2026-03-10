#include "shell/shell.h"
#include "drivers/ps2_kbd.h"
#include "log.h"

void kernel_main()
{
  log(LOG_INFO, "Mist", "Press 'ENTER' to enter the Mist Kernel Shell.");

  if (kbd_getchar() == '\n')
  {
    kernel_shell_init();
    kernel_shell_loop();
  }
}
