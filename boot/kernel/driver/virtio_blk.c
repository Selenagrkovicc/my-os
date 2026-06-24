#include "virtio_blk.h"
#include "screen.h"
#include "../low_level.h"


static unsigned char virtqueue_mem[8192] __attribute__((aligned(4096))); //memorija za virtqueue strukturu
 //pokazivaci
static struct virtq_desc *desc;
static struct virtq_avail *avail;
static struct virtq_used *used;
//bazna io adresa iz bar0
static unsigned short virtio_io_base = 0;
//pomocni baferi za jedan zahtev  
static struct virtio_blk_req req __attribute__((aligned(16)));
static unsigned char status_byte __attribute__((aligned(16)));
//dokle sam procitala used rings, kad uredjaj zavrsi zahtel poveca se 
static unsigned short last_used_idx = 0;


void virtio_blk_init(unsigned short io_base) { //inicijalizuuje virtio blok uredjaj

    virtio_io_base = io_base; //bazna adresa za read write 

    port_byte_out(io_base + VIRTIO_PCI_DEVICE_STATUS, 0); //resetujem virtio uredjaj

    port_byte_out(io_base + VIRTIO_PCI_DEVICE_STATUS, VIRTIO_STATUS_ACKNOWLEDGE); // buk kaze da je video uredjaj da postoji

    port_byte_out(io_base + VIRTIO_PCI_DEVICE_STATUS, VIRTIO_STATUS_ACKNOWLEDGE | VIRTIO_STATUS_DRIVER); // ima drajver

    unsigned int features = port_long_in(io_base + VIRTIO_PCI_DEVICE_FEATURES); // uzimam mogucnosti uredjaj

   /* print("VirtIO features=");
    print_hex32(features);
    print("\n"); */

    port_long_out(io_base + VIRTIO_PCI_DRIVER_FEATURES, 0);

    port_byte_out(io_base + VIRTIO_PCI_DEVICE_STATUS, VIRTIO_STATUS_ACKNOWLEDGE | VIRTIO_STATUS_DRIVER |  VIRTIO_STATUS_FEATURES_OK);

    unsigned char status = port_byte_in(io_base + VIRTIO_PCI_DEVICE_STATUS); 

    /*print("VirtIO status=");
    print_hex32(status);
    print("\n"); */

    if (!(status & VIRTIO_STATUS_FEATURES_OK)) { //da li sadrzi fetures_ok
        print("VirtIO features NOT accepted\n");
        port_byte_out(io_base + VIRTIO_PCI_DEVICE_STATUS, status | VIRTIO_STATUS_FAILED);  //javljam uredjaju da inicijalizacija nije uspela
        return;
    }


    print("virtqueu setting up...\n");

    port_word_out(io_base + VIRTIO_PCI_QUEUE_SELECT, 0); //queue broj 0  

    unsigned short qsize = port_word_in(io_base + VIRTIO_PCI_QUEUE_SIZE); //kolko el queue podrzava

    /*print("Queue size=");
    print_hex32(qsize);
    print("\n");*/

    if (qsize == 0) { // ako je nula onda ne postoji
        print("Queue 0 does not exist\n");
        return;
    }

    if (qsize < QUEUE_SIZE) { // da li podrzava bar kolko sam definisala
        print("Queue too small\n");
        return;
    }

    desc = (struct virtq_desc *)virtqueue_mem; //pokazuje na pocetak memorije i pocinje desc tabela

    avail = (struct virtq_avail *)
        (virtqueue_mem + sizeof(struct virtq_desc) * QUEUE_SIZE); //odma posle desc tabele


    used = (struct virtq_used *)
        (virtqueue_mem + 4096);

    for (int i = 0; i < QUEUE_SIZE; i++) {
        desc[i].addr = 0;
        desc[i].len = 0;
        desc[i].flags = 0;
        desc[i].next = 0;

        avail->ring[i] = 0;

        used->ring[i].id = 0;
        used->ring[i].len = 0;
    } //prolazim kroz sve el i svaki dekr praznim

    avail->flags = 0;
    avail->idx = 0;

    used->flags = 0;
    used->idx = 0;

    last_used_idx = 0;
// SVE krece od nule
    unsigned int queue_page = ((unsigned int)virtqueue_mem) >> 12; //broj stranice (delim adresu sa 4096)

    /*print("Queue page=");
    print_hex32(queue_page);
    print("\n");*/

    port_long_out(io_base + VIRTIO_PCI_QUEUE_ADDRESS, queue_page); //kazem na kojoj strani se nalazi virtqueue,znaci zna za desc, avail, used

    print("Virtqueue setup OK\n");


    //kazem uredjaju da je driver spreman
    port_byte_out(io_base + VIRTIO_PCI_DEVICE_STATUS,VIRTIO_STATUS_ACKNOWLEDGE | VIRTIO_STATUS_DRIVER |VIRTIO_STATUS_FEATURES_OK | VIRTIO_STATUS_DRIVER_OK);

    print("VirtIO driver ok\n");
}


// cita jedan sektor
int virtio_blk_read_sector(unsigned int sector, unsigned char *buffer) {

    print("VirtIO read sector...\n");

    req.type = VIRTIO_BLK_T_IN; //read
    req.reserved = 0;
    req.sector = sector; //koji sektor citam

    status_byte = 0xFF;//random vr koja nije 0
//buffer za zahtev
    desc[0].addr = (unsigned int)&req; //Prvi descriptor pokazuje na req
    desc[0].len = sizeof(struct virtio_blk_req); //Dužina je veličina request header-a
    desc[0].flags = VRING_DESC_F_NEXT; //posle ovog sledeci deskr
    desc[0].next = 1; // sledeci je 1
//bufer za podatke
    desc[1].addr = (unsigned int)buffer;  //pokazuje tako de ce podaci biti upisani
    desc[1].len = 512; //jedan sektor od 512 bajtova
    desc[1].flags = VRING_DESC_F_WRITE | VRING_DESC_F_NEXT; // uređaj sme da piše u ovaj buffer (iako je read moraju podaci da se upisu u memoriju) ,sledeci postoji
    desc[1].next = 2;

    desc[2].addr = (unsigned int)&status_byte; //pokazuje na statusni bajt
    desc[2].len = 1; //jedan bajt
    desc[2].flags = VRING_DESC_F_WRITE;// upisuje status u mem
    desc[2].next = 0; //kraj lanca

    avail->ring[avail->idx % QUEUE_SIZE] = 0; //obradi lanac pocni od desc0
    avail->idx++; //povecavam indeks pa kazem da je dodat novi zahtev

    port_word_out(virtio_io_base + VIRTIO_PCI_QUEUE_NOTIFY, 0); //javljam uredjaju za novi zahtev

    while (used->idx == last_used_idx) { //dok ne zavrsi
    }

    last_used_idx = used->idx; //kazem da sam obradila

    if (status_byte == 0) { //ako je statusni bajt 0 uspelooo je 
        print("Read OK\n");
        return 0;
    }

    print("Read FAILED\n");
    return -1;
}


//write deo 
int virtio_blk_write_sector(unsigned int sector, unsigned char *buffer) {
    print("VirtIO write sector...\n");

    req.type = VIRTIO_BLK_T_OUT;
    req.reserved = 0;
    req.sector = sector;

    status_byte = 0xFF;

    desc[0].addr = (unsigned int)&req;
    desc[0].len = sizeof(struct virtio_blk_req);
    desc[0].flags = VRING_DESC_F_NEXT;
    desc[0].next = 1;

    desc[1].addr = (unsigned int)buffer;
    desc[1].len = 512;

    /*
     * Kod WRITE operacije uredjaj cita podatke iz buffera.
     * Zato ovde NEMA VRING_DESC_F_WRITE.
     */
    desc[1].flags = VRING_DESC_F_NEXT;
    desc[1].next = 2;

    desc[2].addr = (unsigned int)&status_byte;
    desc[2].len = 1;
    desc[2].flags = VRING_DESC_F_WRITE;
    desc[2].next = 0;

    avail->ring[avail->idx % QUEUE_SIZE] = 0;
    avail->idx++;

    port_word_out(virtio_io_base + VIRTIO_PCI_QUEUE_NOTIFY, 0);

    while (used->idx == last_used_idx) {
    }

    last_used_idx = used->idx;

    if (status_byte == 0) {
        print("Write OK\n");
        return 0;
    }

    print("Write FAILED\n");
    return -1;
}