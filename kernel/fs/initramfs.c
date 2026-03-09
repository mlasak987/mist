#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "fs/initramfs.h"
#include "arch/limine.h"

#include "log.h"

__attribute__((used, section(".requests")))
static volatile struct limine_module_request module_request = { .id = LIMINE_MODULE_REQUEST_ID, .revision = 0 };

typedef struct tar_header_struct {
  char filename[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char chksum[8];
  char typeflag;
} __attribute__((packed)) tar_header_t;

static uint8_t* initramfs_start = NULL;

static uintn_t octal_to_int(const char *str)
{
  uintn_t size = 0;
  while ((*str >= '0' && *str <= '7') || *str == ' ')
  {
    if (*str != ' ') size = size * 8 + (*str - '0');
    str++;
  }
  return size;
}

void initramfs_init()
{
  if (module_request.response == NULL || module_request.response->module_count == 0)
  {
    log(LOG_ERROR, "VFS", "No initramfs module provided by Limine.");
    return;
  }

  struct limine_file *module = module_request.response->modules[0];
  initramfs_start = (uint8_t *)(uintn_t)module->address;
  
  log(LOG_OK, "VFS", "Initramfs mounted at %p", (uintn_t)initramfs_start);

  uint8_t *ptr = initramfs_start;
  uintn_t file_count = 0;
  
  while (1)
  {
    tar_header_t *header = (tar_header_t *)ptr;
    if (header->filename[0] == '\0')
      break; 

    uintn_t file_size = octal_to_int(header->size);
    if (header->typeflag == '0' || header->typeflag == '\0')
    {
      log(LOG_DEBUG, "VFS", "Found file: %s (%lu bytes)", header->filename, file_size);
      file_count++;
    }

    uintn_t block_jump = ((file_size + 511) / 512) * 512;
    ptr += 512 + block_jump;
  }
  
  log(LOG_OK, "VFS", "Loaded %lu files into the virtual file system.", file_count);
}

char *initramfs_read_file(const char *filename, uintn_t *out_size)
{
  if (!initramfs_start)
    return NULL;

  uint8_t *ptr = initramfs_start;
  while (1)
  {
    tar_header_t *header = (tar_header_t *)ptr;
    if (header->filename[0] == '\0')
      break;

    uintn_t file_size = octal_to_int(header->size);
    if (!strcmp(header->filename, filename))
    {
      if (out_size) *out_size = file_size;
      return (char *)(ptr + 512);
    }

    uintn_t block_jump = ((file_size + 511) / 512) * 512;
    ptr += 512 + block_jump;
  }

  return NULL;
}

void initramfs_list_files(bool hide)
{
  if (!initramfs_start)
    return;

  uint8_t *ptr = initramfs_start;
  while (1)
  {
    tar_header_t *header = (tar_header_t *)ptr;
    if (header->filename[0] == '\0')
      break;

    uintn_t file_size = octal_to_int(header->size);
    uintn_t block_jump = ((file_size + 511) / 512) * 512;
    if (header->filename[0] == '.' && hide)
    {
      ptr += 512 + block_jump;
      continue;
    }

    printf("%s ", header->filename);
    ptr += 512 + block_jump;
  }
  printf("\n");
}
