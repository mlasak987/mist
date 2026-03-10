#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>

void paging_init(void);

// Assembly functions
extern void load_page_directory(uint32_t*);
extern void enable_paging(void);

#endif
