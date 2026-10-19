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

/* Canonical EE aliases used by CMemory::Init and the SuperFX setup path. */
struct RecoveredSuperFXState {
    uint32_t unknown_0000;
    uint32_t pv_registers;
    uint32_t n_ram_banks;
    uint32_t pv_ram;
    uint32_t n_rom_banks;
    uint32_t pv_rom;
};

extern uint32_t g_global_sram;
extern uint32_t g_global_rom;
extern uint32_t g_global_regram;
extern volatile uint32_t g_memory_fillram;
extern struct RecoveredSuperFXState g_superfx_state;

/* Keep one direct malloc call site per allocation, as in the EE target CFG. */
#define alloc32(size) ((uint32_t)(uintptr_t)malloc(size))

int CMemory_Init_recovered(void *memory)
{
    uint32_t rom_storage, sram_storage, rom_pointer;

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

    /* Target order: clear BSRAM and null FillRAM before cache allocation. */
    memset((void *)(uintptr_t)FIELD32(memory, 0xd480), 0, 0x80000);
    FIELD32(memory, 0x0014) = 0;

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

    if (!FIELD32(memory, 0x0000) ||
        !(sram_storage = FIELD32(memory, 0x000c)) ||
        !FIELD32(memory, 0x0008) ||
        !(rom_storage = FIELD32(memory, 0x0004)) ||
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

    /*
     * Preserve the raw ROM pointer in FillRAM first, then expose ROM+0x8000
     * and the C4RAM window at ROM+0x410000, matching the EE target's CFG.
     */
    FIELD32(memory, 0x0014) = rom_storage;
    rom_pointer = rom_storage + 0x8000;
    FIELD32(memory, 0x0004) = rom_pointer;
    FIELD32(memory, 0x0018) = rom_pointer + 0x410000;

    /* Rebuild the historical SuperFX register/RAM/ROM aliases. */
    g_superfx_state.pv_registers = g_memory_fillram + 0x3000;
    g_superfx_state.n_ram_banks = 1;
    g_superfx_state.pv_ram = sram_storage;
    g_superfx_state.n_rom_banks = 64;

    /* Reuse the recovered pointer values rather than reloading the fields. */
    g_global_rom = rom_pointer;
    g_superfx_state.pv_rom = FIELD32(memory, 0x0004);
    g_global_regram = rom_storage;
    g_global_sram = sram_storage;

    /* The decoded caches need not be cleared; validity maps gate their use. */
    memset((void *)(uintptr_t)g_tile_cache_state.tile_2bpp_valid, 0, 0x1000);
    memset((void *)(uintptr_t)g_tile_cache_state.tile_4bpp_valid, 0, 0x0800);
    memset((void *)(uintptr_t)g_tile_cache_state.tile_8bpp_valid, 0, 0x0400);

    /* SDD1 data/index fields in the recovered CMemory object. */
    FIELD32(memory, 0x3068) = 0;
    FIELD32(memory, 0x3064) = 0;
    return 1;
}
