/*
 * SNES Station v0.23 - SNES9X Record/RAM Initialization
 * Canonical source from progress13_runtime_more.c (0x0012b9a4 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* 0x0012b9a4 -- initialize eight 0x16-byte records and RAM I/O sentinels. */
void snes9x_record_ram_init_0012b9a4(uint8_t *records, uint8_t *ram)
{
    unsigned i;
    for (i = 0; i < 8; ++i) {
        uint8_t *r = records + i * 0x16u;
        int16_t minus_one = -1;
        r[0] = 0;
        r[1] = 1;
        r[2] = 0;
        r[3] = 0xff;
        r[4] = 0xff;
        memcpy(r + 6, &minus_one, sizeof(minus_one));
        memcpy(r + 8, &minus_one, sizeof(minus_one));
        r[0x0a] = 0xff;
        memcpy(r + 0x0c, &minus_one, sizeof(minus_one));
        r[0x0e] = 0;
    }
    for (i = 0; i < 8; ++i) {
        unsigned base = 0x4300u + i * 0x10u;
        memset(ram + base, 0xff, 0x0cu);
        ram[base + 0x0f] = 0xff;
    }
}