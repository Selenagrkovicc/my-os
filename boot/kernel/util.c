#include "driver/screen.h"

void memory_copy(char *source, char *dest, int no_bytes) {
    for (int i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}





int handle_scrolling(int cursor_offset) {

    // Ako je kursor još uvijek unutar ekrana, ništa ne radimo
    if (cursor_offset < MAX_ROWS * MAX_COLS * 2) {
        return cursor_offset;
    }

    int i;

    // Pomjeri sve redove jednu liniju gore
    for (i = 1; i < MAX_ROWS; i++) {

        memory_copy(
            (char *)(VIDEO_ADDRESS + get_screen_offset(0, i)),
            (char *)(VIDEO_ADDRESS + get_screen_offset(0, i - 1)),
            MAX_COLS * 2
        );
    }

    // Očisti posljednji red
    char *last_line = (char *)(VIDEO_ADDRESS + get_screen_offset(0, MAX_ROWS - 1));

    for (i = 0; i < MAX_COLS * 2; i++) {
        last_line[i] = 0;
    }

    // Vrati kursor jedan red gore (jer je ekran “pomerен”)
    cursor_offset -= 2 * MAX_COLS;

    return cursor_offset;
}