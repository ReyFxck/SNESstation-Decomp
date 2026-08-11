/*
 * Progress 13 -- frontend/PPU/date helpers recovered from complete target
 * control flow.  Address-labelled names are retained where an original symbol
 * has not yet been proven by historical source.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static void p13_store_u32(uint8_t *p, uint32_t value)
{
    memcpy(p, &value, sizeof(value));
}

/* 0x00101924 -- two-stage path construction wrapper. */
typedef void (*SnesP13PathSeedFn)(void *config, char *a, char *b,
                                  char *c, char *d, void *opaque);
typedef void (*SnesP13PathFormatFn)(char *out, const char *a, const char *b,
                                    const char *c, const char *suffix,
                                    void *opaque);
char *snes_p13_path_wrapper_00101924(
    char *out, char *scratch0, char scratch1[0x410],
    char scratch2[0x410], char scratch3[0x410], const char *suffix,
    void *config, SnesP13PathSeedFn seed, SnesP13PathFormatFn format,
    void *opaque)
{
    /* Target frame: sp, sp+0x10, sp+0x420 and sp+0x830 feed 0x105ae8;
     * 0x1059cc then formats into the independent sp+0xc40 result buffer. */
    if (seed != NULL)
        seed(config, scratch0, scratch1, scratch2, scratch3, opaque);
    if (format != NULL)
        format(out, scratch0, scratch1, scratch2, suffix, opaque);
    return out;
}

/*
 * 0x0012e374 -- transpose eight groups of four source bytes into four bit-plane
 * bytes.  This is a direct expression of the target shifts/masks.
 */
void snes_p13_bitplane_transpose_0012e374(const uint8_t src[32],
                                          uint8_t dst_a[16],
                                          uint8_t dst_b[16])
{
    unsigned i;
    for (i = 0; i < 8; ++i) {
        uint8_t b0 = src[i * 4 + 0];
        uint8_t b1 = src[i * 4 + 1];
        uint8_t b2 = src[i * 4 + 2];
        uint8_t b3 = src[i * 4 + 3];

        dst_a[i * 2 + 0] = (uint8_t)(
            ((b0 & 0x10u) << 3) | ((b0 & 0x01u) << 6) |
            ((b1 & 0x10u) << 1) | ((b1 & 0x01u) << 4) |
            ((b2 & 0x10u) >> 1) | ((b2 & 0x01u) << 2) |
            ((b3 & 0x10u) >> 3) |  (b3 & 0x01u));
        dst_a[i * 2 + 1] = (uint8_t)(
            ((b0 & 0x20u) << 2) | ((b0 & 0x02u) << 5) |
             (b1 & 0x20u)       | ((b1 & 0x02u) << 3) |
            ((b2 & 0x20u) >> 2) | ((b2 & 0x02u) << 1) |
            ((b3 & 0x20u) >> 4) | ((b3 & 0x02u) >> 1));
        dst_b[i * 2 + 0] = (uint8_t)(
            ((b0 & 0x40u) << 1) | ((b0 & 0x04u) << 4) |
            ((b1 & 0x40u) >> 1) | ((b1 & 0x04u) << 2) |
            ((b2 & 0x40u) >> 3) |  (b2 & 0x04u)       |
            ((b3 & 0x40u) >> 5) | ((b3 & 0x04u) >> 2));
        dst_b[i * 2 + 1] = (uint8_t)(
             (b0 & 0x80u)       | ((b0 & 0x08u) << 3) |
            ((b1 & 0x80u) >> 2) | ((b1 & 0x08u) << 1) |
            ((b2 & 0x80u) >> 4) | ((b2 & 0x08u) >> 1) |
            ((b3 & 0x80u) >> 6) | ((b3 & 0x08u) >> 3));
    }
}

/* 0x00153674 -- bulk frontend table initialization followed by one callback. */
typedef void (*SnesP13InitCallback)(uint8_t *object, void *opaque);
void snes_p13_frontend_table_init_00153674(uint8_t *object,
                                           int extended_tables,
                                           uint32_t external_base,
                                           SnesP13InitCallback finish,
                                           void *opaque)
{
    unsigned outer, inner;

    if (extended_tables) {
        for (outer = 0; outer < 15; ++outer) {
            for (inner = 0; inner < 8; ++inner) {
                unsigned q = (outer * 16u + inner) * 4u;
                p13_store_u32(object + q + 0x3c28, 3);
                p13_store_u32(object + q + 0x1c28, 3);
                object[outer * 16u + inner + 0xa728] = 0;
                object[outer * 16u + inner + 0x9728] = 1;
                object[outer * 16u + inner + 0xaf28] = 0;
                object[outer * 16u + inner + 0x9f28] = 1;
            }
        }
    }

    {
        uint32_t first = 0;
        memcpy(&first, object, sizeof(first));
        for (inner = 0; inner < 16; ++inner) {
            p13_store_u32(object + inner * 4u + 0x1fe8,
                          first + 0x10000u);
            p13_store_u32(object + inner * 4u + 0x1fa8, first);
            object[inner + 0xa808] = 0;
            object[inner + 0x9808] = 1;
            object[inner + 0x9818] = 1;
            object[inner + 0xa818] = 0;
        }
    }

    (void)external_base; /* target uses its global external base in other lanes */
    if (finish != NULL)
        finish(object, opaque);
}

/* 0x00153780 -- initialize the two 16-entry pointer/flag banks. */
void snes_p13_frontend_bank_init_00153780(uint8_t *object,
                                          uint32_t external_base)
{
    unsigned i;
    uint32_t first;
    memcpy(&first, object, sizeof(first));

    for (i = 0; i < 16; ++i) {
        p13_store_u32(object + i * 4u + 0x1fe8, first + 0x10000u);
        p13_store_u32(object + i * 4u + 0x1fa8, first);
        object[i + 0xa808] = 0;
        object[i + 0x9808] = 1;
        object[i + 0x9818] = 1;
        object[i + 0xa818] = 0;
    }

    for (i = 0; i < 16; ++i) {
        p13_store_u32(object + i * 4u + 0x1c28, external_base);
        p13_store_u32(object + i * 4u + 0x1c68, external_base + 0x8000u);
        p13_store_u32(object + i * 4u + 0x1ca8, external_base + 0x10000u);
        p13_store_u32(object + i * 4u + 0x1ce8, external_base + 0x18000u);
        object[i + 0x9728] = 1;
        object[i + 0xa728] = 0;
        object[i + 0x9738] = 1;
        object[i + 0xa738] = 0;
        object[i + 0x9748] = 1;
        object[i + 0xa748] = 0;
        object[i + 0x9758] = 1;
        object[i + 0xa758] = 0;
    }
}

/* Existing 0x00158b3c predicate, duplicated locally only as behavior. */
static int p13_is_lead_byte(uint8_t value)
{
    if ((value & 0x80u) == 0)
        return 0;
    return (((uint8_t)(value - 0x20u)) & 0x40u) != 0;
}

/* 0x00158a58 -- target's bounded multibyte-name validator. */
int snes_p13_multibyte_validate_00158a58(const uint8_t *p)
{
    int remaining = 16;
    int pairs = 0;
    uint8_t ch = *p++;

    for (;;) {
        if (p13_is_lead_byte(ch)) {
            ch = *p++;
            if (ch < 0x20u) {
                if (pairs != 11)
                    return -1;
                if (ch != 0)
                    return 1;
            }
            ++pairs;
            --remaining;
            --remaining;
        } else {
            if (ch == 0) {
                if (pairs == 0)
                    return -1;
                --remaining;
            } else if (ch < 0x20u) {
                return -1;
            } else if (ch < 0x80u) {
                ++pairs;
                --remaining;
            } else if ((uint8_t)(ch - 0xa0u) < 0x50u) {
                --remaining;
            } else {
                return -1;
            }
        }

        if (remaining <= 0)
            return pairs > 0 ? 0 : -1;
        ch = *p++;
    }
}

/* 0x00158974 -- structured header validation around the helper above. */
int snes_p13_header_validate_00158974(const uint8_t *o)
{
    uint32_t pair;
    uint8_t b18;

    if ((o[0x19] & 0x4fu) != 0)
        return -1;
    if (o[0x1a] != 0x33u && o[0x1a] != 0xffu)
        return -1;

    pair = ((uint32_t)o[0x17] << 8) | o[0x16];
    if (pair != 0 && pair != 0xffffu) {
        if ((pair & 0x040fu) != 0 || (pair & 0xffu) > 0xc0u)
            return -1;
    }

    b18 = o[0x18];
    if ((b18 & 0xceu) != 0 || (b18 & 0x30u) == 0)
        return -1;
    if ((o[0x15] & 3u) != 0)
        return -1;
    if (o[0x13] != 0 && o[0x13] != 0xffu)
        return -1;
    if (o[0x14] != 0)
        return -1;

    /* 0x158a48 branches to success only when the subsidiary returns zero. */
    return snes_p13_multibyte_validate_00158a58(o) == 0 ? 0 : -1;
}

/* Shared compact state used by 0x15d9ac/0x15db74/0x15f030 models. */
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
} SnesP13PpuFrontend;

typedef void (*SnesP13AddressSelectFn)(uint32_t address, void *opaque);
typedef void (*SnesP13RefreshFn)(void *opaque);
typedef void (*SnesP13ModeFn)(uint8_t value, void *opaque);
typedef uint16_t (*SnesP13Read16Fn)(uint32_t address, void *opaque);

/* 0x0015d9ac -- reset the compact PPU/frontend register set. */
void snes_p13_ppu_frontend_reset_0015d9ac(
    uint8_t regs[0x10], uint8_t *ram, SnesP13PpuFrontend *s,
    uint32_t map_default_base, uint32_t runtime28,
    SnesP13AddressSelectFn select_address, SnesP13RefreshFn refresh,
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
void snes_p13_ppu_frontend_reload_0015db74(
    const uint8_t regs[0x10], uint8_t *ram, SnesP13PpuFrontend *s,
    uint32_t runtime_c, SnesP13AddressSelectFn select_address,
    SnesP13RefreshFn refresh, SnesP13ModeFn apply_mode, void *opaque)
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
void snes_p13_ppu_shift_window_0015f030(
    int write_back, int zero_advance, uint8_t *ram, SnesP13PpuFrontend *s,
    SnesP13Read16Fn read16, void *opaque)
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

/* 0x001833a4 / 0x001834f0 -- fixed 24-byte record persistence. */
typedef int (*SnesP13OpenFn)(const char *path, int flags, void *opaque);
typedef int (*SnesP13IoFn)(int fd, void *buffer, int size, void *opaque);
typedef int (*SnesP13CloseFn)(int fd, void *opaque);

int snes_p13_write_record_001833a4(const char *path, const uint8_t record[24],
                                   SnesP13OpenFn open_fn, SnesP13IoFn write_fn,
                                   SnesP13CloseFn close_fn, void *opaque)
{
    int fd, i;
    if (open_fn == NULL || write_fn == NULL)
        return 0;
    fd = open_fn(path, 0x202, opaque);
    if (fd < 0)
        return 0;
    for (i = 0; i < 16; ++i) {
        uint8_t b = record[i];
        (void)write_fn(fd, &b, 1, opaque);
    }
    for (i = 16; i < 24; ++i) {
        uint8_t b = record[i];
        (void)write_fn(fd, &b, 1, opaque);
    }
    if (close_fn != NULL)
        (void)close_fn(fd, opaque);
    return 1;
}

int snes_p13_read_record_001834f0(const char *path, uint8_t record[24],
                                  SnesP13OpenFn open_fn, SnesP13IoFn read_fn,
                                  SnesP13CloseFn close_fn, void *opaque)
{
    int fd, i;
    if (open_fn == NULL || read_fn == NULL)
        return 0;
    fd = open_fn(path, 1, opaque);
    if (fd < 0)
        return 0;
    for (i = 0; i < 24; ++i) {
        uint8_t b = 0;
        (void)read_fn(fd, &b, 1, opaque);
        record[i] = b;
    }
    if (close_fn != NULL)
        (void)close_fn(fd, opaque);
    return 1;
}
