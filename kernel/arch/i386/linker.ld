ENTRY(_start)

SECTIONS
{
  /* Begin putting sections at 1MiB. */
  . = 1M;

  /* Make all sections 4KiB and align them to pages (4KiB).
     Each section will start with address 0x1000, 0x2000...*/
  .text BLOCK(4K) : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  }

  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }

  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }

  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }
}
