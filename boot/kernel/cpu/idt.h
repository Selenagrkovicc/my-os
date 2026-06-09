#ifndef IDT_H
#define IDT_H

#include <stdint.h> // biblioteka za uint16_t

#define MESTA_ZA_INTERAPTE 256
#define KERNEL_CODE_SEGMENT 0x08

struct idt_entry {
    uint16_t offset_low; // donjih 16 bitova handlera
    uint16_t selector;     // code segment selector iz GDT-a
    uint8_t zero; 
    uint8_t type_attr;     // tip interapta
    uint16_t offset_high;  // gornjih 16 bitova handlera
} __attribute__((packed)); // kaze da se ne dodaju prazni bajtovi jer nam je bitno da bude 256

struct idt_ptr {
    uint16_t limit;        // velicina IDT tabele -1 zbog kretanja od nule
    uint32_t base;         // pocetka IDT tabele u mem
} __attribute__((packed)); 

void idt_init(void);
void idt_set_gate(int n, uint32_t handler);

#endif