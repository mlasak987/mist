export ARCH ?= i386
export LOG_LEVEL ?= INFO
export VER ?= 0.1.0

.PHONY: all libc libk kernel clean run

all: kernel

libk:
	$(MAKE) -C libc libk.a

libc:
	$(MAKE) -C libc libc.a

kernel: libk
	$(MAKE) -C kernel

clean:
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean

run: kernel
	$(MAKE) -C kernel run
