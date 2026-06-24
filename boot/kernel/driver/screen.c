#include "screen.h"
#include "../low_level.h"
#include "../util.h"

int cursor_offset = 0;

void print_char(char karakter, int col, int row, char opisni_bajt) {
    unsigned char *vidmem = (unsigned char *)VIDEO_ADDRESS;

    if (!opisni_bajt) {
        opisni_bajt = WHITE_ON_BLACK;
    }

    int offset;

    if (col >= 0 && row >= 0) {
        offset = get_screen_offset(col, row);
    } else {
        offset = cursor_offset;
    }

    if (karakter == '\n') {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(0, rows + 1);
    } else {
        vidmem[offset] = karakter;
        vidmem[offset + 1] = opisni_bajt;
        offset += 2;
    }

    cursor_offset = offset;
    set_cursor(offset);
}

void print_at(char *message, int col, int row) {
    if (col >= 0 && row >= 0) {
        cursor_offset = get_screen_offset(col, row);
        set_cursor(cursor_offset);
    }

    int i = 0;
    while (message[i] != 0) {
        print_char(message[i], -1, -1, WHITE_ON_BLACK);
        i++;
    }
}

void print(char *message) {
    print_at(message, -1, -1);
}

int get_screen_offset(int col, int row) {
    return (row * MAX_COLS + col) * 2;
}

int get_cursor() {
    return cursor_offset;
}

void set_cursor(int offset) {
    offset /= 2;

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
        print_hex_digit((value >> i) & 0xF);
}


void clear_screen(void) {
    volatile char *vidmem = (volatile char *)0xb8000;

    for (int i = 0; i < 80 * 25; i++) {
        vidmem[i * 2] = ' ';
        vidmem[i * 2 + 1] = 0x0f;
    }
} 