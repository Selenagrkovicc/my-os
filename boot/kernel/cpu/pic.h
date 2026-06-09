#ifndef PIC_H
#define PIC_H

#include <stdint.h>

//---------Common Definitions
#define PIC1        0x20
#define PIC2        0xA0

#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)

#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

// End-of-interrupt command code 

#define PIC_EOI 0x20

//--- reinitialize the PIC controllers, giving them specified vector offsets rather than 8h and 70h, as configured by default */

#define ICW1_ICW4      0x01
#define ICW1_SINGLE    0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL     0x08
#define ICW1_INIT      0x10

#define ICW4_8086      0x01
#define ICW4_AUTO      0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM      0x10

#define CASCADE_IRQ 2




#define PIC_READ_IRR 0x0A  /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR 0x0B  /* OCW3 irq service next CMD read */


//--------DEFINISEM FIJE

void PIC_sendEOI(uint8_t irq);
void PIC_remap(int offset1, int offset2);
void PIC_disable(void);

void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);

uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);

#endif