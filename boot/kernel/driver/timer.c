#include "timer.h"
#include "../low_level.h"
#include "../driver/screen.h"

static uint32_t t = 0; // brojac koji svaki put kad timer pošalje IRQ0 poveca za 1

void init_timer(uint32_t frequency)
{
    uint32_t divisor = FREQUENCY / frequency; //frequency - kolko interaptova u sekundi 

    port_byte_out(COMMAND, 0x36); //pitu saljem komandu
    port_byte_out(CHANNEL0, divisor & 0xFF); // donji bitovi
    port_byte_out(CHANNEL0, (divisor >> 8) & 0xFF); // gornji bitovi
}

void timer_callback(void) //svaki put kad stigne interupt on se poziva
{
    t++;
    if (t % 100 == 0) { 
       print("");
    }
}

uint32_t timer_get_t(void) 
{
    return t;  // vraca broj taktova
} 