//======konstante=======

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE_ON_BLACK 0x0f

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0X3D5

//=====funkcije=========

#ifndef SCREEN_H
#define SCREEN_H


int get_screen_offset(int col, int row);
int get_cursor() ;
void set_cursor(int offset);
void backspace(void);

void print_char(char c, int col, int row, char attr);
void print_at(char *msg, int col, int row);
void print(char *msg);

void print_hex_digit(unsigned char x);
void print_hex32(unsigned int value);

void clear_screen(void);

#endif