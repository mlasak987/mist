CC = i686-elf-gcc
AS = nasm
LD = i686-elf-gcc

CFLAGS = -ffreestanding -O2 -Wall -Wextra -I./include/ -I../libc/include/
ASFLAGS = -f elf32
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

ARCH_DIR = arch/i386
KERNEL_DIR = kernel
BUILD_DIR = build
LINKER_SCRIPT = $(ARCH_DIR)/linker.ld

vpath %.c $(ARCH_DIR) $(KERNEL_DIR)
vpath %.nasm $(ARCH_DIR)

CRTI_OBJ = $(BUILD_DIR)/crti.o
CRTN_OBJ = $(BUILD_DIR)/crtn.o

OBJS = $(BUILD_DIR)/boot.o \
       $(BUILD_DIR)/tty.o \
       $(BUILD_DIR)/kernel.o

CRTBEGIN_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ   := $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

KERNEL = os.kernel
LIBK = ../libc/libk.a

.PHONY: all clean run

all: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/%.o: %.nasm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL): $(CRTI_OBJ) $(OBJS) $(CRTN_OBJ) $(LIBK)
	$(LD) -T $(LINKER_SCRIPT) -o $(KERNEL) \
		$(CRTI_OBJ) \
		$(CRTBEGIN_OBJ) \
		$(OBJS) \
		$(LIBK) \
		$(CRTEND_OBJ) \
		$(CRTN_OBJ) \
		$(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(KERNEL)

run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)
