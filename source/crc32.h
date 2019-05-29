#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>
#include <stddef.h>

uint32_t crc32_part(const void *buf, size_t size, uint32_t crc);
uint32_t crc32(const void *buf, size_t size);
uint64_t hash40(const char* data);

#endif // CRC32_H
