#ifndef _INITRAMFS_H
#define _INITRAMFS_H

#include <types.h>
#include <stdbool.h>

void initramfs_init(void);
char* initramfs_read_file(const char* filename, uintn_t* out_size);
void initramfs_list_files(bool hide);

#endif
