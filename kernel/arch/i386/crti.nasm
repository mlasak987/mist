section .init
global _init:function
_init:
  push ebp
  mov ebp, esp
  ; Constructors call

section .fini
global _fini:function
_fini:
  push ebp
  mov ebp, esp
  ; Destructors call
