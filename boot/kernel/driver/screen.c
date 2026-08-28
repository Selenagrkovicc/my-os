#include "screen.h"
#include "../low_level.h"
#include "../util.h"

int cursor_offset = 0;

void print_char(char karakter, int col, int row, char opisni_bajt) {
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS; //pokazivac na video mem

    if (!opisni_bajt) { //ako nije data boja, belo i crno
        opisni_bajt = WHITE_ON_BLACK;
    }

    int offset;

    if (col >= 0 && row >= 0) { // ako sam dalal kolonu i red na njih ako ne onda ova obicna
        offset = get_screen_offset(col, row);
    } else {
        offset = cursor_offset;
    }

    if (karakter == '\n') {// novi red, pomeri u novi red
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(0, rows + 1);
    } else {
        vidmem[offset] = karakter;  //ako je obican bajt ispisi njega i boju
        vidmem[offset + 1] = opisni_bajt;
        offset += 2;
    }

    cursor_offset = offset;  
    set_cursor(offset);
}

void print_at(char *message, int col, int row) { // odredjena pozicija
    if (col >= 0 && row >= 0) {
        cursor_offset = get_screen_offset(col, row);
        set_cursor(cursor_offset); //pomeri kursor
    }

    int i = 0;
    while (message[i] != 0) {
        print_char(message[i], -1, -1, WHITE_ON_BLACK);
        i++;
    }
}

void print(char *message) { //ispisi
    print_at(message, -1, -1);
}

int get_screen_offset(int col, int row) {
    return (row * MAX_COLS + col) * 2;
}

int get_cursor() {
    return cursor_offset;
}

void set_cursor(int offset) {
    offset /= 2; //hardverski kusor

    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));

    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xFF));
}

void backspace(void)
{
    unsigned char *vidadr = (unsigned char *)VIDEO_ADDRESS;

    if (cursor_offset > 0) {
        cursor_offset -= 2;

        vidadr[cursor_offset] = ' ';
        vidadr[cursor_offset + 1] = WHITE_ON_BLACK;

        set_cursor(cursor_offset);
    }
}

void print_hex_digit(unsigned char x)
{
    if (x < 10)
        print_char('0' + x, -1, -1, 0);
    else
        print_char('A' + x - 10, -1, -1, 0);
}

void print_hex32(unsigned int value)
{
    print("0x");
    for (int i = 28; i >= 0; i -= 4)
        print_hex_digit((value >> i) & 0xF); // pomeramo 4bita udesno delimo sa 16
}



void clear_screen(void) {
    volatile char *vidmem = (volatile char *)VIDEO_ADDRESS;

    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        vidmem[i * 2] = ' ';                 // obriši karakter
        vidmem[i * 2 + 1] = WHITE_ON_BLACK;  // postavi boju
    }

    cursor_offset = 0;   // vrati softverski kursor na početak
    set_cursor(0);       // pomeri i hardverski VGA kursor na početak
}