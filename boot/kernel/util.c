#include "driver/screen.h"

void memory_copy(char *source, char *dest, int no_bytes) {
    for (int i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}


int strcmp(char *a, char *b) {
    int i = 0;

    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
        i++;
    }

    return a[i] - b[i];
}


