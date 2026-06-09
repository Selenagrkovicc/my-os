
#ifndef ISR_H
#define ISR_H

#include <stdint.h>

typedef struct registers { // struktura sta je na steku
    uint32_t ds;

    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // dodajem mora sama da ga ima

    uint32_t int_no; //push dword %1
    uint32_t err_code; //push dword 0

    uint32_t eip, cs, eflags, useresp, ss; //sam stavlja
} registers_t; 

void isr_install(void);
void isr_handler(registers_t *regs);

#endif