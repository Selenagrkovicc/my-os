#include "idt.h"
#include "isr.h"
#include "pic.h"
#include "irq.h"

struct idt_entry idt[MESTA_ZA_INTERAPTE];  //pravim tabelu
struct idt_ptr idtp;                //adresa i velicina tabele

extern void idt_load(unsigned int);     //definnisem interrupts.asm

void idt_set_gate(int n, unsigned int handler) {  //popunjavam mesta u tabeli
    idt[n].offset_low = handler & 0xFFFF;  //donje bajtove uzimam (hendler adresa fje koju treba da pozovem)
    idt[n].selector = KERNEL_CODE_SEGMENT; // sektor kernela
    idt[n].zero = 0; 
    idt[n].type_attr = 0x8E; // 
    idt[n].offset_high = (handler >> 16) & 0xFFFF; //gornji bajtovi
}

void idt_init() { //pravim idt
    idtp.limit = sizeof(struct idt_entry) * MESTA_ZA_INTERAPTE - 1; //velicina tabele (oduzimam sa 1 jer brojanje krece od)
    idtp.base = (uint32_t)&idt; //definisem pocetak

    for (int i = 0; i < MESTA_ZA_INTERAPTE; i++) {
        idt_set_gate(i, 0); //popunjava jedno mesto
    }
    isr_install();
     PIC_remap(0x20, 0x28);
    irq_install();
    idt_load((unsigned int)&idtp); //instrukcija da se sad ovo koristi
   
}