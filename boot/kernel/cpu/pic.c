#include "pic.h"
#include "../low_level.h"



static void io_wait(void)
{
    port_byte_out(0x80, 0);
}

void PIC_sendEOI(uint8_t irq)
{
    if (irq >= 8)
        port_byte_out(PIC2_COMMAND, PIC_EOI);

    port_byte_out(PIC1_COMMAND, PIC_EOI);
}

void PIC_remap(int offset1, int offset2)
{
    unsigned char a1;
    unsigned char a2;

    a1 = port_byte_in(PIC1_DATA);
    a2 = port_byte_in(PIC2_DATA);

    port_byte_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    port_byte_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    port_byte_out(PIC1_DATA, offset1);
    io_wait();
    port_byte_out(PIC2_DATA, offset2);
    io_wait();
    port_byte_out(PIC1_DATA, 1 << CASCADE_IRQ);
    io_wait();
    port_byte_out(PIC2_DATA, 2);
    io_wait();
    port_byte_out(PIC1_DATA, ICW4_8086);
    io_wait();
    port_byte_out(PIC2_DATA, ICW4_8086);
    io_wait();

    port_byte_out(PIC1_DATA, a1);
    port_byte_out(PIC2_DATA, a2);
}


void PIC_disable(void){
    port_byte_out(PIC1_DATA, 0xFF);
    port_byte_out(PIC2_DATA, 0xFF);
}

void IRQ_set_mask(uint8_t IRQline)
{
    uint16_t port;
    uint8_t value;

    if (IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = port_byte_in(port) | (1 << IRQline);
    port_byte_out(port, value);
}

void IRQ_clear_mask(uint8_t IRQline)
{
    uint16_t port;
    uint8_t value;

    if (IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = port_byte_in(port) & ~(1 << IRQline);
    port_byte_out(port, value);
}



static uint16_t __pic_get_irq_reg(int ocw3)
{
    port_byte_out(PIC1_COMMAND, ocw3);
    port_byte_out(PIC2_COMMAND, ocw3);

    return (port_byte_in(PIC2_COMMAND) << 8) | port_byte_in(PIC1_COMMAND);
}

uint16_t pic_get_irr(void)
{
    return __pic_get_irq_reg(PIC_READ_IRR);
}

uint16_t pic_get_isr(void)
{
    return __pic_get_irq_reg(PIC_READ_ISR);
}