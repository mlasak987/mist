export ARCH ?= x86_64
export LOG_LEVEL ?= INFO
export VER ?= 0.1.1

ISO_NAME = mist-$(VER)-$(ARCH).iso

.PHONY: all libc libk kernel clean run run-iso iso limine

all: kernel

libk:
	$(MAKE) -C libc libk.a

libc:
	$(MAKE) -C libc libc.a

kernel: libk
	$(MAKE) -C kernel

limine:
	@if [ ! -d "limine" ]; then \
		git clone https://github.com/limine-bootloader/limine.git --branch v8.x-binary --depth=1; \
		$(MAKE) -C limine; \
	fi

iso: kernel limine
	@rm -rf isodir
	@mkdir -p isodir/boot/limine
	@mkdir -p isodir/EFI/BOOT
	@cp kernel/mist.kernel isodir/boot/mist.kernel
	@sed -e "s/@@VER@@/$(VER)/g" -e "s/@@ARCH@@/$(ARCH)/g" limine.conf.in > isodir/boot/limine/limine.conf
	@cp limine/limine-bios.sys limine/limine-bios-cd.bin limine/limine-uefi-cd.bin isodir/boot/limine/
	@cp limine/BOOTX64.EFI isodir/EFI/BOOT/
	@cp limine/BOOTIA32.EFI isodir/EFI/BOOT/
	@xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		isodir -o $(ISO_NAME)
	@./limine/limine bios-install $(ISO_NAME)
	@rm -rf isodir

run-iso: iso
	qemu-system-$(ARCH) -M q35 -m 2G -cdrom $(ISO_NAME) -boot d

clean:
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean
	rm -f $(ISO_NAME)
	rm -rf isodir

run: kernel
	$(MAKE) -C kernel run
