/*
 * SNES Station v0.23 - SNES9X PPU/Frontend Register Management
 * Canonical source from progress13_frontend_more.c (0x0015d9ac..0x0015f030 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t default_base0; /* target state +0x00 */
    uint32_t map_c;
    uint32_t map_10;
    uint32_t current_ptr;
    uint32_t current_base;
    uint32_t runtime28;
    uint32_t runtime10; /* separate runtime object +0x10 written by 0x15db74 */
    uint8_t bit4, bit5, bit6, bit7;
    uint8_t active18, active1c;
    uint8_t window51;
    uint8_t phase53;
} Snes9xPpuFrontend;

typedef void (*Snes9xAddressSelectFn)(uint32_t address, void *opaque);
typedef void (*Snes9xRefreshFn)(void *opaque);
typedef void (*Snes9xModeFn)(uint8_t value, void *opaque);
typedef uint16_t (*Snes9xRead16Fn)(uint32_t address, void *opaque);

/* 0x0015d9ac -- reset the compact PPU/frontend register set. */
void snes9x_ppu_frontend_reset_0015d9ac(
    uint8_t regs[0x10], uint8_t *ram, Snes9xPpuFrontend *s,
    uint32_t map_default_base, uint32_t runtime28,
    Snes9xAddressSelectFn select_address, Snes9xRefreshFn refresh,
    void *opaque)
{
    uint16_t address = (uint16_t)(ram[0x2203] | ((uint16_t)ram[0x2204] << 8));
    uint16_t zero16 = 0, value134 = 0x134;

    memset(regs, 0, 0x10);
    regs[8] = 0xff;
    regs[9] = 1;
    memcpy(regs + 2, &value134, 2);
    memcpy(regs + 6, &zero16, 2);
    memcpy(regs + 0x0e, &address, 2);

    s->map_c = 0;
    s->map_10 = 0;
    s->current_ptr = 0;
    s->current_base = 0;
    if (select_address != NULL)
        select_address(address, opaque);
    /* Target writes 0x003f5040 to state +0x00 after SetPCBase returns;
     * this is distinct from the +0x24 current-base field set by 0x15e07c. */
    s->default_base0 = map_default_base;

    s->bit5 = (uint8_t)((((regs[2] >> 1) ^ 1u) & 1u));
    s->bit6 = (uint8_t)(regs[2] & 0x80u);
    s->bit4 = (uint8_t)(regs[2] & 1u);
    s->bit7 = (uint8_t)((regs[2] & 0x40u) >> 6);
    if (refresh != NULL)
        refresh(opaque);
    s->active18 = 1;
    s->runtime28 = runtime28;
    ram[0x2225] = 0;
}

/* 0x0015db74 -- reload the map cursor and PPU mode bits from register RAM. */
void snes9x_ppu_frontend_reload_0015db74(
    const uint8_t regs[0x10], uint8_t *ram, Snes9xPpuFrontend *s,
    uint32_t runtime_c, Snes9xAddressSelectFn select_address,
    Snes9xRefreshFn refresh, Snes9xModeFn apply_mode, void *opaque)
{
    uint16_t address;
    memcpy(&address, regs + 0x0e, sizeof(address));
    s->map_c = (uint32_t)regs[0] << 16;
    s->map_10 = (uint32_t)regs[1] << 16;
    if (select_address != NULL)
        select_address(s->map_c + address, opaque);

    s->bit5 = (uint8_t)((((regs[2] >> 1) ^ 1u) & 1u));
    s->bit4 = (uint8_t)(regs[2] & 1u);
    s->bit7 = (uint8_t)((regs[2] & 0x40u) >> 6);
    s->bit6 = (uint8_t)(regs[2] & 0x80u);
    if (refresh != NULL)
        refresh(opaque);

    s->window51 = (ram[0x223f] & 0x80u) != 0 ? 2 : 4;
    s->runtime10 = runtime_c + ((uint32_t)(ram[0x2224] & 7u) << 13);
    if (apply_mode != NULL)
        apply_mode(ram[0x2225], opaque);
    s->active1c = (ram[0x2200] & 0x60u) != 0;
    s->active18 = (uint8_t)(s->active1c ^ 1u);
}

/* 0x0015f030 -- fetch a shifted 16-bit window and optionally advance source. */
void snes9x_ppu_shift_window_0015f030(
    int write_back, int zero_advance, uint8_t *ram, Snes9xPpuFrontend *s,
    Snes9xRead16Fn read16, void *opaque)
{
    uint32_t advance = ram[0x2258] & 0x0fu;
    uint32_t bit;
    uint32_t address;
    uint32_t value;

    if (advance == 0)
        advance = 16;
    if (zero_advance)
        advance = 0;

    bit = (uint8_t)(s->phase53 + advance);
    address = (uint32_t)ram[0x2259] |
              ((uint32_t)ram[0x225a] << 8) |
              ((uint32_t)ram[0x225b] << 16);
    if (bit >= 16u) {
        address += 2u * (bit >> 4);
        bit &= 15u;
    }

    value = read16 != NULL ? read16(address, opaque) : 0;
    value |= (uint32_t)(read16 != NULL ? read16(address + 2, opaque) : 0) << 16;
    value >>= bit;
    ram[0x230c] = (uint8_t)value;
    ram[0x230d] = (uint8_t)(value >> 8);

    if (write_back) {
        s->phase53 = (uint8_t)((s->phase53 + advance) & 0x0fu);
        ram[0x2259] = (uint8_t)address;
        ram[0x225a] = (uint8_t)(address >> 8);
        ram[0x225b] = (uint8_t)(address >> 16);
    }
}