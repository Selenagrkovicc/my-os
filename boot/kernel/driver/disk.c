#include "disk.h"
#include "virtio_blk.h"

int disk_read_sector(unsigned int sector, unsigned char *buffer) {
    return virtio_blk_read_sector(sector, buffer);
}

int disk_write_sector(unsigned int sector, unsigned char *buffer) {
    return virtio_blk_write_sector(sector, buffer);
}