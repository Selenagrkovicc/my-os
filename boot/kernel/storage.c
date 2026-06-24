#include "storage.h"
#include "driver/disk.h"
#include "driver/screen.h"

static unsigned char storage_buffer[512];

void storage_save_text(char *text) { //prima tekst

    for (int i = 0; i < 512; i++) {
        storage_buffer[i] = 0; //brisem stare podatke
    }

    int i = 0; //brojac za karaktere

    while (text[i] != 0 && i < 511) { //kopiram tekst u bafer karakter po karakter (jedno mesto je rez za \0)
        storage_buffer[i] = text[i];
        i++;
    }

    if (disk_write_sector(2, storage_buffer) == 0) { //upisujem u drugi sektor (ako je 0 onda je)
        print("Saved to disk\n");
    } else {
        print("Save failed\n");
    }
}

void storage_load_text() {

    for (int i = 0; i < 512; i++) { //brisem
        storage_buffer[i] = 0;
    }

    if (disk_read_sector(2, storage_buffer) == 0) { //sektor 2 na disku → storage_buffer

        print("text: "); 

        int i = 0;

        while (storage_buffer[i] != 0 && i < 512) { 
            print_char(storage_buffer[i], -1, -1, 0); //ispis
            i++;
        }

        print("\n");

    } else {
        print("Load failed\n");
    }
}