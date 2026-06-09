#include "driver/screen.h"
#include "cpu/idt.h"
#include "driver/timer.h"

void clear_screen() {
    volatile char *vidmem = (volatile char *)0xb8000;

    for (int i = 0; i < 80 * 25; i++) {
        vidmem[i * 2] = ' ';
        vidmem[i * 2 + 1] = 0x0f;
    }
}

void main() {
    clear_screen();
    print_at("Kernel radi ",0,10);
    idt_init();
    print_at("idt radi ",0,11);

    print("\n");


    init_timer(100);
    print_at("Timer podesen", 0, 13);

    __asm__ __volatile__("sti");
    print_at("Interrupti ukljuceni", 0, 14);
       print("\n");

    while (1) {
        __asm__ __volatile__("hlt");
    }
}

