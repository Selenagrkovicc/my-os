//radjeno po uzoru na pci iz xv6-riscV

#include "pci.h"
#include "screen.h"
#include "../low_level.h"



#include "virtio_blk.h"


//konfigaracioni  prostor ima svaki pci uredjaj  preko njega os saznaje sta je taj uredjaj i kako da komunicira sa njim

static unsigned int pci_config_read(unsigned char bus, unsigned char device, unsigned char function, unsigned char offset) { //Ova funkcija čita 32-bitni podatak iz PCI konfiguracionog prostora.
    unsigned int address;

    address = ((unsigned int)bus << 16) //pci magistrala
            | ((unsigned int)device << 11) // bira uredjaj na toj magistrali
            | ((unsigned int)function << 8) //bira fju uredjaja
            | (offset & 0xFC) // bira koji registar citam 
            | 0x80000000; //ukljucuje PCI CONF ADRESU

    port_long_out(PCI_CONFIG_ADDRESS, address);
    return port_long_in(PCI_CONFIG_DATA); //CITAM PODATKE
}

void pci_scan() {
    print("PCI scan start...\n");

    for (uint16_t bus = 0; bus < 256; bus++) { //do 256 magistrala
    for (uint8_t device = 0; device < 32; device++) { //na svakoj gagistrali 32 uredjaja

        uint8_t function = 0;

        uint32_t vendor_device = pci_config_read(bus, device, function, 0x00); //Čitaš prvi PCI registar na offsetu 0x00.

        uint16_t vendor_id = vendor_device & 0xFFFF;
        uint16_t device_id = (vendor_device >> 16) & 0xFFFF;

        if (vendor_id == 0xFFFF) { //nma uredjaja
            continue;
        }

        if (vendor_id == 0x1AF4) {  //BILO KOJI UREDJAJ
            // ispis radi debug-ovanja
            print("ID nadjenog virto uredjaja je: ");
            print_hex32(device_id);
            print("\n");

            uint32_t bar0 = pci_config_read(bus, device, function, 0x10); //registar u PCI konfiguracionom prostoru koji sadrži adresu preko koje se pristupa uređaju.
            
            // ispis radi debug-ovanja
            print("BAR0="); //Base Address Register
            print_hex32(bar0);
            print("\n");

            unsigned short io_base = bar0 & ~0x3; //u najnizim bitovim asu zastavice al brisem poslednja dva bita

            print("IO BASE=");
            print_hex32(io_base); //baza svih virtio redistara
            print("\n");

            if (device_id == 0x1001 || device_id == 0x1042) { //trazim virtio  blok device

                print("VirtIO disk je detektovan.\n");

                virtio_blk_init(io_base); //prosledjujem drajveru adresu uredjaja
            }
        }
            }
        }
}