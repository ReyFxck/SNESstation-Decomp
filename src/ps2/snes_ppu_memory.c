/*
 * SNES Station v0.23 PS2 - PPU Memory Helpers
 * Canonical source from progress26_dsp_runtime_batch.c @ 0x0015e298, 0x0015ecec, 0x0015eeac
 */

#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t *io;
    uint8_t *main_memory;
    uint8_t *alt_memory;
    uint8_t *tile_scratch; /* logical main_memory + 0x7f0000 */
    uintptr_t page_map[4096];
    uint32_t address_mask;

    uint8_t cpu_status;
    uint64_t math_result;
    uint8_t ring_index;

    uint32_t pending_flags;
    uint8_t pending_mask;

    void (*debug_read)(void *opaque, uint32_t address);
    void (*trigger_transfer)(void *opaque, int a, int b);
    void *opaque;
} SnesPpuMemory;

static uint16_t snes_ppu_read_u16(const uint8_t *p)
{
    return (uint16_t)p[0] | (uint16_t)((uint16_t)p[1] << 8);
}

static uint32_t snes_ppu_read_u24(const uint8_t *p)
{
    return (uint32_t)p[0] |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16);
}

/* 0x0015e298: PPU memory read helper */
uint8_t snes_ppu_memory_read_0015e298(SnesPpuMemory *s, uint32_t address)
{
    uint64_t value;

    switch (address) {
    case 0x2300:
        return (uint8_t)((s->cpu_status & 0xa0u) | (s->io[0x2209] & 0x5fu));
    case 0x2301:
        return (uint8_t)((s->io[0x2301] & 0xf0u) | (s->io[0x2200] & 0x0fu));
    case 0x2306:
        return (uint8_t)s->math_result;
    case 0x2307:
        return (uint8_t)(s->math_result >> 8);
    case 0x2308:
        return (uint8_t)(s->math_result >> 16);
    case 0x2309:
        return (uint8_t)(s->math_result >> 24);
    case 0x230a:
        return (uint8_t)(s->math_result >> 32);
    case 0x230c:
        return s->io[0x230c];
    case 0x230d:
        value = s->io[0x230d];
        if ((s->io[0x2258] & 0x80u) != 0 && s->trigger_transfer != NULL)
            s->trigger_transfer(s->opaque, 1, 0);
        return (uint8_t)value;
    default:
        if (s->debug_read != NULL)
            s->debug_read(s->opaque, address);
        return s->io[address];
    }
}

/* 0x0015ecec: PPU tile transfer helper */
void snes_ppu_tile_transfer_0015ecec(SnesPpuMemory *s)
{
    uint32_t mode = s->io[0x2231] & 3u;
    uint32_t planes = mode == 0 ? 8u : (mode == 1 ? 4u : 2u);
    uint32_t bank = (uint32_t)((s->ring_index & 7u) == 0);
    uint8_t *src = s->tile_scratch + bank * 0x40u;
    uint8_t *dst = s->io + 0x3000u + snes_ppu_read_u16(s->io + 0x2235) +
                   bank * planes * 8u;
    unsigned row;

    if (planes != 8u)
        return;

    for (row = 0; row < 8u; ++row) {
        uint8_t b = src[row];

        dst[0x00] = (uint8_t)((dst[0x00] << 1) | ((b >> 0) & 1u));
        dst[0x01] = (uint8_t)((dst[0x01] << 1) | ((b >> 1) & 1u));
        dst[0x10] = (uint8_t)((dst[0x10] << 1) | ((b >> 2) & 1u));
        dst[0x11] = (uint8_t)((dst[0x11] << 1) | ((b >> 3) & 1u));
        dst[0x20] = (uint8_t)((dst[0x20] << 1) | ((b >> 4) & 1u));
        dst[0x21] = (uint8_t)((dst[0x21] << 1) | ((b >> 5) & 1u));
        dst[0x30] = (uint8_t)((dst[0x30] << 1) | ((b >> 6) & 1u));
        dst[0x31] = (uint8_t)((dst[0x31] << 1) | ((b >> 7) & 1u));

        dst += 2;
        src += 8;
    }
}

/* 0x0015eeac: PPU memory copy helper */
void snes_ppu_memory_copy_0015eeac(SnesPpuMemory *s)
{
    uint32_t source24 = snes_ppu_read_u24(s->io + 0x2232);
    uint32_t dest24 = snes_ppu_read_u24(s->io + 0x2235);
    uint32_t mode = s->io[0x2230] & 3u;
    uint32_t count = snes_ppu_read_u16(s->io + 0x2238);
    uint8_t *src;
    uint8_t *dst;

    if (mode == 1u) {
        count &= s->address_mask;
        src = s->alt_memory + (source24 & s->address_mask);
    } else if (mode == 0u) {
        uintptr_t page = s->page_map[source24 >> 12];
        if (page < 0x12u)
            src = s->main_memory + (uint16_t)source24;
        else
            src = (uint8_t *)page + (uint16_t)source24;
    } else {
        count = (uint32_t)s->io[0x2238] |
                ((uint32_t)(s->io[0x2239] & 3u) << 8);
        src = s->io + 0x3000u +
              ((uint32_t)s->io[0x2232] |
               ((uint32_t)(s->io[0x2233] & 3u) << 8));
    }

    if ((s->io[0x2230] & 4u) == 0) {
        count &= 0x3ffu;
        dst = s->io + 0x3000u +
              ((uint32_t)s->io[0x2235] |
               ((uint32_t)(s->io[0x2236] & 3u) << 8));
    } else {
        count &= s->address_mask;
        dst = s->alt_memory + (dest24 & s->address_mask);
    }

    memmove(dst, src, count);
    s->io[0x2301] |= 0x20u;

    if ((s->io[0x220a] & 0x20u) != 0) {
        s->pending_flags |= 0x800u;
        s->pending_mask |= 0x20u;
    }
}