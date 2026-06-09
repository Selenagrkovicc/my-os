#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>


#define FREQUENCY 1193182  //1.193182 MHz 
#define CHANNEL0  0x40     // IRQ0
#define COMMAND   0x43     // port za slanje komandi pit-u


void init_timer(uint32_t frequency);
void timer_callback(void);
uint32_t timer_get_t(void);

#endif