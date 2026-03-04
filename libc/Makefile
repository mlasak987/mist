CC = i686-elf-gcc
AR = i686-elf-ar

BUILD_DIR = build

CFLAGS = -ffreestanding -O2 -Wall -Wextra -I./include -I../kernel/include -DLOG_LEVEL_$(LOG_LEVEL)
LIBK_CFLAGS = $(CFLAGS) -D__is_libk

SRCS = $(shell find string stdio -type f -name '*.c')

SRC_NAMES = $(notdir $(SRCS))
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_NAMES))
LIBK_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.libk.o, $(SRC_NAMES))

VPATH = $(sort $(dir $(SRCS)))

LIBK = libk.a
LIBC = libc.a

.PHONY: all clean

all: $(LIBK) $(LIBC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIBC): $(OBJS)
	$(AR) rcs $@ $^

$(LIBK): $(LIBK_OBJS)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/%.libk.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(LIBK_CFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(LIBK) $(LIBC)
