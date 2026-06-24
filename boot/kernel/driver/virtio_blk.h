#ifndef VIRTIO_BLK_H
#define VIRTIO_BLK_H



#define VIRTIO_PCI_DEVICE_FEATURES  0x00
#define VIRTIO_PCI_DRIVER_FEATURES  0x04
#define VIRTIO_PCI_QUEUE_ADDRESS    0x08
#define VIRTIO_PCI_QUEUE_SIZE       0x0C
#define VIRTIO_PCI_QUEUE_SELECT     0x0E
#define VIRTIO_PCI_QUEUE_NOTIFY     0x10
#define VIRTIO_PCI_DEVICE_STATUS    0x12
#define VIRTIO_PCI_ISR_STATUS       0x13

#define VIRTIO_STATUS_ACKNOWLEDGE   1
#define VIRTIO_STATUS_DRIVER        2
#define VIRTIO_STATUS_DRIVER_OK     4
#define VIRTIO_STATUS_FEATURES_OK   8
#define VIRTIO_STATUS_FAILED        128

#define QUEUE_SIZE 128

#define VRING_DESC_F_NEXT  1
#define VRING_DESC_F_WRITE 2

#define VIRTIO_BLK_T_IN  0
#define VIRTIO_BLK_T_OUT 1


struct virtq_desc { //deskriptor kaze Podaci su na ovoj adresi, ovoliko su dugi, i ovo su pravila.
    unsigned long long addr;
    unsigned int len;
    unsigned short flags;
    unsigned short next;
};

struct virtq_avail { // available ring
    unsigned short flags;
    unsigned short idx;
    unsigned short ring[QUEUE_SIZE];
};

struct virtq_used_elem { //jedan zavrsen zahtev, koji, i kolko bajtova
    unsigned int id;
    unsigned int len;
};

struct virtq_used {  // used rings
    unsigned short flags;
    unsigned short idx;
    struct virtq_used_elem ring[QUEUE_SIZE];
};

struct virtio_blk_req { //zahtev za dish
    unsigned int type; //read write
    unsigned int reserved; //0
    unsigned long long sector; //sektor diska
};




#include <stdint.h>

void virtio_blk_init(unsigned short io_base);
int virtio_blk_read_sector(unsigned int sector, unsigned char *buffer);
int virtio_blk_write_sector(unsigned int sector, unsigned char *buffer);




#endif