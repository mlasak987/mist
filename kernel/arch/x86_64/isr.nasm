global gdt_flush
global idt_flush

gdt_flush:
  lgdt [rdi]
  mov ax, 0x10 ; 0x10 Kernel Data Segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  push 0x08 ; 0x08 Kernel Code Segment
  lea rax, [rel .flush]
  push rax
  retfq
.flush:
  ret

idt_flush:
  lidt [rdi]
  ret

extern isr_handler

%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    push 0
    push %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    push %1
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0 ; Zero division 
ISR_NOERRCODE 1 ; Debug
ISR_NOERRCODE 2 ; NMI
ISR_NOERRCODE 3 ; Breakpoint
ISR_NOERRCODE 32 ; PIT
ISR_NOERRCODE 33 ; Keyboard

isr_common_stub:
  push rax
  push rcx
  push rdx
  push rbx
  push rbp
  push rsi
  push rdi
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15

  mov rdi, [rsp + 15*8]
  mov rsi, [rsp + 16*8]

  call isr_handler

  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rdi
  pop rsi
  pop rbp
  pop rbx
  pop rdx
  pop rcx
  pop rax

  add rsp, 16
  iretq
