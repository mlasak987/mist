CC = i686-elf-gcc
AR = i686-elf-ar

BUILD_DIR = build

CFLAGS = -ffreestanding -O2 -Wall -Wextra -I./include -I../kernel/include
LIBK_CFLAGS = $(CFLAGS) -D__is_libk

SRCS = string/memset.c \
			 string/memsetw.c \
			 string/memcpy.c \
			 string/memmove.c \
			 string/memcmp.c \
			 string/strlen.c \
			 string/strcpy.c \
			 string/strncpy.c \
			 string/strlcpy.c \
			 string/strcmp.c \
			 string/strncmp.c \
			 stdio/utoa.c \
			 stdio/itoa.c \
			 stdio/printf.c \
			 stdio/putchar.c

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))
LIBK_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.libk.o, $(notdir $(SRCS)))
vpath %.c $(sort $(dir $(SRCS)))

LIBK = libk.a
LIBC = libc.a

all: $(LIBK) $(LIBC)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(LIBC): $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(LIBK): $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/%.libk.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(LIBK_CFLAGS)

clean:
	rm -f $(OBJS) $(LIBK)
