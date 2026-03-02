global isr33
extern keyboard_handler

; keyboard_handler interrupt call
isr33:
  pusha
  cld
  call keyboard_handler
  popa
  iret
