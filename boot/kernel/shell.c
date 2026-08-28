#include "shell.h"
#include "driver/screen.h"
#include "driver/timer.h"
#include "storage.h"
#include "util.h"


static char buffer[SHELL_BUFFER_SIZE]; //cuva sta korisnik unese

static int buffer_index = 0; //pamti de upisujem sl karakter

static void shell_prompt(void) // kao na linuksu $
{
    print("\n> ");
}

static void shell_clear_buffer(void) //brisem prethodnu komandu
{
    for (int i = 0; i < SHELL_BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
    buffer_index = 0;
}

static int str_equal(char *a, char *b) //pravim strcmp
{
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) {
            return 0;
        }
        i++;
    }
    return a[i] == b[i];
}
static void shell_execute(void) //poziva se kad enter pritisne
{
    print("\n");

    if (str_equal(buffer, "help")) {
        print("Komande: help, clear, ticks, about, save, load\n");

    } else if (str_equal(buffer, "clear")) {
        clear_screen();

    } else if (str_equal(buffer, "ticks")) {
        print("Timer radi.\n");

    } else if (str_equal(buffer, "about")) {
        print("SelenaOS shell\n");

    } else if (str_equal(buffer, "save")) {
        print("SAVE KOMANDA PREPOZNATA\n");
        storage_save_text("Selena grkovic");

    } else if (str_equal(buffer, "load")) {
        print("LOAD KOMANDA PREPOZNATA\n");
        storage_load_text();

    } else if (buffer_index == 0) {
        /* prazna komanda */

    } else {
        print("Nepoznata komanda\n");
    }

    shell_clear_buffer(); //brise staru
    shell_prompt(); //ispisuje ponovo >
}

void shell_init(void) 
{
    print("Shell pokrenut");
    shell_prompt();
}

//dobija svaki karatker
void shell_input(char c)
{
    if (c == '\n') { //ako je enter 
        shell_execute(); 
        return;
    }
    if (c == '\b') { //ako se brise karakter
        if (buffer_index > 0) { 
            buffer_index--; //brisem ga iz buffera
            buffer[buffer_index] = 0;
            backspace(); //brise ga sa ekrana
        }
        return;
    }
    if (c == '\t') { //tab
        print("   ");
        return;
    }
    if (buffer_index < SHELL_BUFFER_SIZE - 1) { 
        buffer[buffer_index] = c; //cuva karakter u buffer
        buffer_index++;
        print_char(c, -1, -1, 0); //ispisuje
    }
}