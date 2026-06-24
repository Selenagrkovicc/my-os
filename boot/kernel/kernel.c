#include "driver/screen.h"
#include "driver/pci.h"
#include "driver/keyboard.h"
#include "driver/timer.h"
#include "cpu/idt.h"
#include "shell.h"
#include "storage.h"


void main() {
    clear_screen();

    print("Kernel radi\n");

    idt_init();
    init_timer(100);
    void timer_callback(void);
    __asm__ __volatile__("sti");

    pci_scan();

    //shell_init();

    while (1) {
        __asm__ __volatile__("hlt");
    }
}