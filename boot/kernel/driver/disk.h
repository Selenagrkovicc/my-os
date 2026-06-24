#ifndef DISK_H
#define DISK_H

int disk_read_sector(unsigned int sector, unsigned char *buffer);
int disk_write_sector(unsigned int sector, unsigned char *buffer);

#endif