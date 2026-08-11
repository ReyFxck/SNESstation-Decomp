#ifndef SNESSTATION_RECOVERED_SYMBOLS_H
#define SNESSTATION_RECOVERED_SYMBOLS_H

#include <stdint.h>

/*
 * Recovered/identified addresses for SNES Station v0.23 WIP (24 Jan 2004).
 * These are analysis labels, not symbols from the stripped ELF.
 */

enum {
    SNES_ADDR_START              = 0x00100008,
    SNES_ADDR_LOAD_MODULE_BUFFER = 0x00104e7c,
    SNES_ADDR_MAIN               = 0x00104f18,

    SNES_ADDR_SIF_INIT_RPC       = 0x0019cc0c,
    SNES_ADDR_SIF_LOAD_MODULE    = 0x0019d600,
    SNES_ADDR_SIF_ALLOC_IOP_HEAP = 0x0019d63c,
    SNES_ADDR_SIF_FREE_IOP_HEAP  = 0x0019d6b8,
    SNES_ADDR_SIF_IOP_RESET      = 0x0019d740,

    SNES_ADDR_CDVD_INIT          = 0x0019be70,
    SNES_ADDR_MC_INIT            = 0x001a08ec,

    SNES_ADDR_PRINTF             = 0x0019e388,
    SNES_ADDR_SPRINTF            = 0x0019e3d0,
    SNES_ADDR_PUTS               = 0x0019e414,

    SNES_ADDR_MEMORY_INIT        = 0x00151074,
    SNES_ADDR_MEMORY_LOAD_ROM    = 0x001513bc,
    SNES_ADDR_MEMORY_LOAD_SRAM   = 0x00153354,
    SNES_ADDR_MEMORY_SAVE_SRAM   = 0x001534b8,

    SNES_ADDR_TILE_LOOKUP_INIT   = 0x00142a78,
    SNES_ADDR_CONVERT_TILE       = 0x00183e04,
    SNES_ADDR_DRAW_TILE          = 0x0018428c,
    SNES_ADDR_DRAW_CLIPPED_CAND  = 0x001845a8,
};

#endif
