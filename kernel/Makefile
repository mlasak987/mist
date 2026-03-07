ARCH ?= x86_64

CC = $(ARCH)-elf-gcc
AS = nasm
LD = $(ARCH)-elf-gcc

LOG_LEVEL ?= INFO
VER ?= 0.1.1

CFLAGS = -ffreestanding -O2 -Wall -Wextra -I./include/ -I../libc/include/ \
         -DLOG_LEVEL_$(LOG_LEVEL) -D__KERNEL_ARCH_=\"$(ARCH)\" -D__OS_VERSION_=\"$(VER)\" -MD -MP

ifeq ($(ARCH), x86_64)
    CFLAGS += -mno-red-zone -mcmodel=kernel -mno-mmx -mno-sse -mno-sse2
    ASFLAGS = -f elf64
		LDFLAGS = -ffreestanding -O2 -nostdlib -z max-page-size=0x1000
else
    ASFLAGS = -f elf32
    LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc
endif

SRC_DIRS = arch/$(ARCH) kernel drivers mm shell
BUILD_DIR = build
LINKER_SCRIPT = arch/$(ARCH)/linker.ld

ALL_C_SRCS = $(shell find $(SRC_DIRS) -type f -name '*.c')
ALL_NASM_SRCS = $(shell find $(SRC_DIRS) -type f -name '*.nasm')

CRTI_SRC = $(shell find arch/$(ARCH) -name 'crti.nasm' -o -name 'crti.c' | head -n 1)
CRTN_SRC = $(shell find arch/$(ARCH) -name 'crtn.nasm' -o -name 'crtn.c' | head -n 1)

C_SRCS = $(filter-out $(CRTI_SRC) $(CRTN_SRC), $(ALL_C_SRCS))
NASM_SRCS = $(filter-out $(CRTI_SRC) $(CRTN_SRC), $(ALL_NASM_SRCS))

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(C_SRCS))) \
       $(patsubst %.nasm, $(BUILD_DIR)/%.o, $(notdir $(NASM_SRCS)))

CRTI_OBJ = $(BUILD_DIR)/$(basename $(notdir $(CRTI_SRC))).o
CRTN_OBJ = $(BUILD_DIR)/$(basename $(notdir $(CRTN_SRC))).o

VPATH = $(sort $(dir $(ALL_C_SRCS) $(ALL_NASM_SRCS)))

KERNEL = mist.kernel
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
	qemu-system-$(ARCH) -kernel $(KERNEL)

-include $(OBJS:.o=.d)
