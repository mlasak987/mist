#ifndef _PIC_H
#define _PIC_H

#include <stdint.h>

void pic_remap(void);
void pic_send_eoi(uint8_t irq);

#endif
