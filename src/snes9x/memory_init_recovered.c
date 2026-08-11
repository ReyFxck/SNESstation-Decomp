/*
 * SNES Station v0.23 WIP — recovered from R5900 code.
 * Original function: 0x00151074
 *
 * Behavioural reconstruction from the binary.  Field names that can now be
 * validated against the old Snes9x memory layout are used; uncertain fields
 * remain offset-labelled.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define FIELD32(base, off) (*(uint32_t *)((uint8_t *)(base) + (off)))

/* Original IPPU-related global starts at EE VA 0x0035c268. */
struct RecoveredTileCacheState {
    uint8_t unknown_0000[0x1c];
    uint32_t tile_2bpp_cache;   /* +0x1c, allocation 0x80000 */
    uint32_t tile_4bpp_cache;   /* +0x20, allocation 0x40000 */
    uint32_t tile_8bpp_cache;   /* +0x24, allocation 0x20000 */
    uint32_t tile_2bpp_valid;   /* +0x28, 0x1000 bytes */
    uint32_t tile_4bpp_valid;   /* +0x2c, 0x0800 bytes */
    uint32_t tile_8bpp_valid;   /* +0x30, 0x0400 bytes */
};

extern struct RecoveredTileCacheState g_tile_cache_state;
extern void rom_cleanup_00151330(void *memory);

static uint32_t alloc32(size_t size)
{
    return (uint32_t)(uintptr_t)malloc(size);
}

int CMemory_Init_recovered(void *memory)
{
    uint32_t rom_storage;

    /* Old CMemory leading fields, recovered independently from allocation use. */
    if (!FIELD32(memory, 0x0000)) /* RAM */
        FIELD32(memory, 0x0000) = alloc32(0x20000);
    if (!FIELD32(memory, 0x000c)) /* SRAM */
        FIELD32(memory, 0x000c) = alloc32(0x20000);
    if (!FIELD32(memory, 0x0008)) /* VRAM */
        FIELD32(memory, 0x0008) = alloc32(0x10000);
    if (!FIELD32(memory, 0x0004)) /* raw ROM storage before +0x8000 bias */
        FIELD32(memory, 0x0004) = alloc32(0x808200);

    /* 0x80000 scratch/BSRAM-sized allocation at object offset 0xd480. */
    if (!FIELD32(memory, 0xd480))
        FIELD32(memory, 0xd480) = alloc32(0x80000);

    /*
     * Important preservation detail: the original allocates 128 bytes per
     * possible tile here (0x80000/0x40000/0x20000), even though ConvertTile
     * and DrawTile use a 64-byte stride for decoded tile data.  This same
     * over-allocation pattern exists in close-era Snes9x source, so it is not
     * evidence of a PS2-only 16-bit decoded cache.
     */
    if (!g_tile_cache_state.tile_2bpp_cache)
        g_tile_cache_state.tile_2bpp_cache = alloc32(0x80000);
    if (!g_tile_cache_state.tile_4bpp_cache)
        g_tile_cache_state.tile_4bpp_cache = alloc32(0x40000);
    if (!g_tile_cache_state.tile_8bpp_cache)
        g_tile_cache_state.tile_8bpp_cache = alloc32(0x20000);
    if (!g_tile_cache_state.tile_2bpp_valid)
        g_tile_cache_state.tile_2bpp_valid = alloc32(0x1000);
    if (!g_tile_cache_state.tile_4bpp_valid)
        g_tile_cache_state.tile_4bpp_valid = alloc32(0x0800);
    if (!g_tile_cache_state.tile_8bpp_valid)
        g_tile_cache_state.tile_8bpp_valid = alloc32(0x0400);

    if (!FIELD32(memory, 0x0000) || !FIELD32(memory, 0x000c) ||
        !FIELD32(memory, 0x0008) || !FIELD32(memory, 0x0004) ||
        !FIELD32(memory, 0xd480) ||
        !g_tile_cache_state.tile_2bpp_cache ||
        !g_tile_cache_state.tile_4bpp_cache ||
        !g_tile_cache_state.tile_8bpp_cache ||
        !g_tile_cache_state.tile_2bpp_valid ||
        !g_tile_cache_state.tile_4bpp_valid ||
        !g_tile_cache_state.tile_8bpp_valid) {
        rom_cleanup_00151330(memory);
        return 0;
    }

    memset((void *)(uintptr_t)FIELD32(memory, 0xd480), 0, 0x80000);

    /* Classic Snes9x ROM-storage bias: keep raw pointer and expose ROM+0x8000. */
    rom_storage = FIELD32(memory, 0x0004);
    FIELD32(memory, 0x0014) = rom_storage;
    FIELD32(memory, 0x0004) = rom_storage + 0x8000;
    FIELD32(memory, 0x0018) = rom_storage + 0x8000 + 0x410000;

    /* The decoded caches need not be cleared; validity maps gate their use. */
    memset((void *)(uintptr_t)g_tile_cache_state.tile_2bpp_valid, 0, 0x1000);
    memset((void *)(uintptr_t)g_tile_cache_state.tile_4bpp_valid, 0, 0x0800);
    memset((void *)(uintptr_t)g_tile_cache_state.tile_8bpp_valid, 0, 0x0400);

    /* Global aliases/strides at 0x1511e0..0x151278 remain to be typed exactly. */
    return 1;
}
