
#include "low_level.h"

// sluzi za komunikaciju instrukijama
//prima broj porta, procitaj bajt odatle
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__ volatile( //asembler kod, nemoj da izbacis instrukciju ovu 
        "inb %1, %0" //procitaj bajt sa porta b, port, rezultat
        : "=a"(result) // reg al
        : "Nd"(port)); // prosledi instrukciji
    return result; //vracamo procitan bajt
}


void port_byte_out(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port)); //posalji, data, port
}
//16bitni
unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data) {
    __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}
//zbog pci 32bitni
unsigned int port_long_in(unsigned short port) {
    unsigned int result;
    __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void port_long_out(unsigned short port, unsigned int data) {
    __asm__ volatile("outl %0, %1" : : "a"(data), "Nd"(port));
}