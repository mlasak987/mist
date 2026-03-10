global gdt_flush
global tss_flush
global idt_flush
global enter_usermode

extern isr_handler

gdt_flush:
    lgdt [rdi]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push 0x08
    lea rax, [rel .flush]
    push rax
    retfq
.flush:
    ret

tss_flush:
    mov ax, 0x28
    ltr ax
    ret

idt_flush:
    lidt [rdi]
    ret

enter_usermode:
    cli

    mov ax, 0x1B
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x1B
    push rsi
    push 0x202
    push 0x23
    push rdi

    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    xor rsi, rsi
    xor rdi, rdi
    xor rbp, rbp
    xor r8,  r8
    xor r9,  r9
    xor r10, r10
    xor r11, r11
    xor r12, r12
    xor r13, r13
    xor r14, r14
    xor r15, r15

    iretq

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

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 32
ISR_NOERRCODE 33

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

    cld
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
