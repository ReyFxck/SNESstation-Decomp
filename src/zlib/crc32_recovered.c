/*
 * SNES Station v0.23 WIP — zlib 1.1.3 CRC-32 recovery.
 *
 * Target VAs:
 *   0x00193094 get_crc_table
 *   0x001930a0 crc32
 *
 * The target uses a static 256-entry table at 0x001c8a80. Under -mlong64
 * each zlib uLong/uLongf entry is loaded as 64 bits, while the CRC polynomial
 * itself remains the standard 32-bit CRC-32 value in the low word.
 */
#include "../../include/zlib_1_1_3_recovered.h"

extern const uint64_t crc_table_001c8a80[256];

/* Target VA 0x00193094. */
const uint64_t *get_crc_table_recovered(void)
{
    return crc_table_001c8a80;
}

#define CRC_DO1() \
    do { \
        crc = crc_table_001c8a80[((unsigned)crc ^ *buf++) & 0xffu] ^ \
              (crc >> 8); \
    } while (0)

/* Target VA 0x001930a0. */
uint64_t crc32_recovered(uint64_t crc, const uint8_t *buf, uint32_t len)
{
    if (buf == NULL)
        return 0;

    crc ^= UINT64_C(0xffffffff);
    while (len >= 8) {
        CRC_DO1(); CRC_DO1(); CRC_DO1(); CRC_DO1();
        CRC_DO1(); CRC_DO1(); CRC_DO1(); CRC_DO1();
        len -= 8;
    }
    while (len != 0) {
        CRC_DO1();
        len--;
    }
    return crc ^ UINT64_C(0xffffffff);
}
