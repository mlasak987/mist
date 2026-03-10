#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>
#include "types.h"

void gdt_init(void);
void tss_set_kernel_stack(uintn_t stack_top);

#endif
