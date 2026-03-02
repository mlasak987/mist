.PHONY: all libc kernel clean run

all: kernel

libc:
	$(MAKE) -C libc

kernel: libc
	$(MAKE) -C kernel

clean:
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean

run: kernel
	$(MAKE) -C kernel run
