CC = i686-elf-gcc
AR = i686-elf-ar

BUILD_DIR = build

CFLAGS = -ffreestanding -O2 -Wall -Wextra -I./include

SRCS = string/strlen.c

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))
vpath %.c $(sort $(dir $(SRCS)))

LIBK = libk.a

all: $(LIBK)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(LIBK): $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) $(LIBK)
