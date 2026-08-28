#include "keyboard.h"
#include "screen.h"
#include "../low_level.h"
#include "../shell.h"

#define KEYBOARD_DATA_PORT 0x60 //PORT SA KOG PROCITA

static char scancode_to_ascii[] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,
    '\\','z','x','c','v','b','n','m',',','.','/',
    0,
    '*',
    0,
    ' '
};

void keyboard_callback(void)
{
    uint8_t scancode = port_byte_in(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) { //IGNORISEM PUSTANJE TASTERA
        return;
    }

    if (scancode < sizeof(scancode_to_ascii)) {
        char c = scancode_to_ascii[scancode];

        if (c) { //AKO NIJE NULA IDE U SHELL
            shell_input(c);
        }
    }
}