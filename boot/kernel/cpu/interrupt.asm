[bits 32]

global idt_load ;da mogu da je pozivam u c

idt_load:
    mov eax, [esp + 4] ;prvi argument
    lidt [eax]
    ret