#ifndef PCI_H
#define PCI_H

#define PCI_CONFIG_ADDRESS 0xCF8 //sta hocu da citam
#define PCI_CONFIG_DATA    0xCFC// odakle citam podatak
#include <stdint.h>

static unsigned int pci_config_read(unsigned char bus, unsigned char device, unsigned char function, unsigned char offset) ;

void pci_scan();

#endif