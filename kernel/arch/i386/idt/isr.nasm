; Interrupt handlers

global isr33
extern kbd_handler

isr33:
  pusha
  cld
  call kbd_handler
  popa
  iret

global isr14
extern page_fault_handler

isr14:
  pusha
  cld
  call page_fault_handler
  popa
  add esp, 4
  iret
