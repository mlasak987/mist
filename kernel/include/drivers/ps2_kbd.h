#ifndef _KBD_H
#define _KBD_H

#include <stdbool.h>

void kbd_handler(void);
bool kbd_has_key(void);
char kbd_getchar(void);

#endif
