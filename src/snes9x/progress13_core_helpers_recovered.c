/*
 * Progress 13 -- small SNES Station/Snes9x helpers recovered directly from
 * complete target control flow.  Global EE addresses are converted to explicit
 * parameters so the models remain host-testable without pretending the final
 * original type names are already known.
 */
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* 0x0010b72c -- mirrored APU I/O/timer-target byte write. */
typedef struct {
    uint8_t *ram;
    uint16_t timer_target[3];
    uint8_t timer_target_written[3];
} SnesP13ApuIoState;

void snes_p13_apu_io_write_0010b72c(SnesP13ApuIoState *state,
                                    uint16_t address, uint8_t value)
{
    state->ram[address] = value;

    if (address >= 0xfau && address <= 0xfcu) {
        unsigned timer = (unsigned)(address - 0xfau);
        uint16_t target = state->ram[address];
        if (target == 0)
            target = 0x100;
        state->timer_target[timer] = target;
        state->timer_target_written[timer] = 1;
    }
}

/* 0x001308f8 -- renderer/core gate predicate from the +0x342a38 state block. */
typedef struct {
    uint32_t cursor_3c;
    uint32_t mode_4c;
    uint32_t window_58;
    uint8_t enabled_5ec;
    uint8_t object98_byte3a;
    uint8_t final_byte3a;
} SnesP13GateState;

int snes_p13_gate_predicate_001308f8(const SnesP13GateState *s)
{
    if (s->enabled_5ec != 0 && s->cursor_3c >= s->window_58 &&
        s->cursor_3c < s->window_58 + 0x200u)
        return 1;

    if (s->mode_4c < 0x40u && s->cursor_3c <= 0x7fffu)
        return 0;

    if ((uint32_t)(s->mode_4c - 0x60u) < 0x10u)
        return 0;
    if (s->mode_4c >= 0x74u)
        return 0;

    if ((uint32_t)(s->mode_4c - 0x70u) < 4u &&
        (s->object98_byte3a & 0x08u) == 0)
        return 0;

    return (s->final_byte3a & 0x10u) != 0;
}

/*
 * 0x001591a8 -- expand 256 packed 15-bit palette entries through a 32-entry
 * component lookup.  The target also selects a 32-byte bank before the loop.
 */
typedef struct {
    const uint8_t *bank_base;
    const uint8_t *selected_bank;
    uint32_t component0[256];
    uint32_t component1[256];
    uint32_t component2[256];
    uint16_t packed[256];
} SnesP13PaletteExpand;

void snes_p13_palette_expand_001591a8(SnesP13PaletteExpand *out,
                                      uint8_t bank, int enabled,
                                      const uint16_t source[256],
                                      const uint8_t component_lut[32])
{
    unsigned i;

    out->selected_bank = out->bank_base + ((unsigned)bank << 5);
    if (!enabled)
        return;

    for (i = 0; i < 256; ++i) {
        uint16_t raw = source[i];
        uint32_t c0 = component_lut[raw & 31u];
        uint32_t c1 = component_lut[(raw >> 5) & 31u];
        uint32_t c2 = component_lut[(raw >> 10) & 31u];
        out->component0[i] = c0;
        out->component1[i] = c1;
        out->component2[i] = c2;
        out->packed[i] = (uint16_t)(c0 | (c1 << 5) | (c2 << 10));
    }
}

/* 0x0015d8ec -- PPU/reset state plus RAM I/O defaults. */
typedef struct {
    uint32_t word14;
    uint8_t byte18;
    uint8_t byte19;
    uint8_t byte1a;
    uint8_t byte1b;
    uint8_t byte1c;
    uint16_t secondary40;
    uint16_t secondary42;
    uint32_t secondary44;
    uint64_t secondary48;
    uint8_t secondary50;
} SnesP13PpuResetState;

void snes_p13_ppu_reset_0015d8ec(SnesP13PpuResetState *state, uint8_t *ram)
{
    state->byte18 = 0;
    state->byte19 = 0;
    state->byte1a = 0;
    state->byte1b = 0;
    state->byte1c = 0;
    state->word14 = 0;

    memset(ram + 0x2200, 0, 0x200);
    ram[0x2200] = 0x20;
    ram[0x2220] = 0;
    ram[0x2221] = 1;
    ram[0x2222] = 2;
    ram[0x2223] = 3;
    ram[0x2228] = 0xff;

    state->secondary40 = 0;
    state->secondary42 = 0;
    state->secondary44 = 0;
    state->secondary48 = 0;
    state->secondary50 = 0;
}

/*
 * 0x0015e198 -- fill the two mirrored pointer-map tables.  The target uses
 * 32-bit EE addresses; uint32_t keeps the same arithmetic and table width.
 */
void snes_p13_map_window_0015e198(unsigned region, uint8_t selector,
                                  uint32_t ram_base,
                                  uint32_t *ppu_map,
                                  uint32_t *memory_map)
{
    unsigned outer;
    unsigned selector7 = selector & 7u;
    unsigned region_index = region << 9;

    if (region >= 2u)
        region_index += 0x400u;

    for (outer = 0; outer < 256; outer += 16) {
        unsigned j;
        uint32_t base = ram_base + (selector7 << 20) + (outer << 12);
        for (j = 0; j < 16; ++j) {
            ppu_map[outer + j] = base;
            memory_map[outer + j] = base;
        }
    }

    for (outer = 0; outer < 512; outer += 16) {
        unsigned j;
        uint32_t base = ram_base + (selector7 << 20) + (outer << 11) - 0x8000u;
        for (j = 8; j < 16; ++j) {
            unsigned index = outer + j + region_index;
            ppu_map[index] = base;
            memory_map[index] = base;
        }
    }
}

/* 0x0016fd08 -- signed-angle 2-D transform used by frontend state. */
void snes_p13_rotate_pair_0016fd08(int16_t angle, int16_t y, int16_t x,
                                   int16_t *out_sum, int16_t *out_diff)
{
    const float radians = (float)angle * 9.58738019107841e-05f;
    const float s = sinf(radians);
    const float c = cosf(radians);
    const float sum = 2.0f * (s * (float)x + c * (float)y);
    const float diff = 2.0f * (c * (float)x - s * (float)y);

    *out_sum = (int16_t)lrintf(sum);
    *out_diff = (int16_t)lrintf(diff);
}

/* 0x001832a4 -- zero/init one fixed runtime block and its 64 KiB tail. */
void snes_p13_runtime_block_init_001832a4(uint8_t *block)
{
    memset(block, 0, 0x30);
    block[0x27] = 1;
    block[0x28] = 2;
    block[0x2d] = 1;
    memset(block + 0x38, 0, 4);
    memset(block + 0x3c, 0, 0x10000);
}
