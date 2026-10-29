/*
 * SNES Station v0.23 - SNES9X Runtime Block Initialization
 * Canonical source from progress13_core_helpers.c (0x001832a4 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* 0x001832a4 -- zero/init one fixed runtime block and its 64 KiB tail. */
void snes9x_runtime_block_init_001832a4(uint8_t *block)
{
    memset(block, 0, 0x30);
    block[0x27] = 1;
    block[0x28] = 2;
    block[0x2d] = 1;
    memset(block + 0x38, 0, 4);
    memset(block + 0x3c, 0, 0x10000);
}