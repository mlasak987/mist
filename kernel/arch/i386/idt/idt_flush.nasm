global idt_flush

; void idt_flush(uint32_t idt_ptr);
idt_flush:
  mov eax, [esp+4]
  lidt [eax]
  ret
