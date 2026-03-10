; Multiboot header
MB_ALIGN     equ 1 << 0
MB_MEMINFO   equ 1 << 1

MB_FLAGS     equ MB_ALIGN
MB_MAGIC     equ 0x1BADB002
MB_CHECKSUM  equ -(MB_MAGIC + MB_FLAGS)

section .multiboot
  align 4
  dd MB_MAGIC
  dd MB_FLAGS
  dd MB_CHECKSUM

section .bss
  align 16
stack_bottom:
  resb 16384 ; Reserve 16KiB for kernel.
stack_top:

section .text
  global _start
  extern kernel_main
  extern _init

_start:
  mov esp, stack_top
  push ebx
  push eax

  call _init
  call kernel_main

	; Hang if kernel_main unexpectedly returns.
  cli
.hang:
  hlt
  jmp .hang
