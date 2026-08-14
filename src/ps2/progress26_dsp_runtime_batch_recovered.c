/*
 * Progress 26: large typed behavioral batch from the warning-free Progress-16
 * R5900 structural decompile.
 *
 * Target entries represented here:
 *   0x00129250  0x0012c02c  0x0012c188  0x0012c2dc  0x0012c398
 *   0x0012d334  0x0012d4ac  0x0012d624  0x0012e5b8
 *   0x001305a0  0x001306f8  0x001309c4
 *   0x0015e298  0x0015ecec  0x0015eeac
 *
 * These are typed behavioral/source models, not compiler-match claims.  Raw
 * address ownership is intentionally replaced only where the observed layout
 * is sufficiently narrow to model without guessing project-wide structures.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* ------------------------------------------------------------------------- */
/* 0x00129250: ADC / decimal-mode accumulator helper                         */
/* ------------------------------------------------------------------------- */

typedef struct P26AdcState {
    uint8_t accumulator;
    uint8_t carry;
    uint8_t decimal_mode;
    uint8_t overflow;
    uint8_t result_shadow;
    uint8_t operand_shadow;
    uint16_t wide_sum_shadow;
    uint8_t (*read_operand)(void *opaque);
    void *opaque;
} P26AdcState;

void snes_p26_00129250(P26AdcState *s)
{
    uint32_t operand = s->read_operand(s->opaque);
    uint32_t a = s->accumulator;
    uint32_t result;
    uint32_t overflow_bits;

    s->operand_shadow = (uint8_t)operand;

    if ((s->decimal_mode & 8u) == 0) {
        uint32_t sum = a + (operand & 0xffu) + (uint32_t)s->carry;
        s->carry = (uint8_t)(sum > 0xffu);
        result = sum & 0xffu;
        overflow_bits = ~(operand ^ a) & (operand ^ result);
        s->wide_sum_shadow = (uint16_t)sum;
    } else {
        uint32_t lo = (a & 0x0fu) + (operand & 0x0fu) + (uint32_t)s->carry;
        uint32_t hi = a >> 4;
        uint32_t op_hi = operand >> 4;

        if ((lo & 0xffu) > 9u) {
            hi += 1u;
            lo = (lo - 10u) & 0x0fu;
        }

        hi += op_hi;
        s->carry = (uint8_t)(hi > 9u);
        if (s->carry)
            hi = (hi - 10u) & 0x0fu;

        result = (lo & 0x0fu) | ((hi << 4) & 0xf0u);
        overflow_bits = ~(a ^ operand) & (operand ^ result);
        s->wide_sum_shadow = (uint16_t)result;
    }

    s->overflow = (uint8_t)((overflow_bits & 0x80u) != 0);
    s->result_shadow = (uint8_t)result;
    s->accumulator = (uint8_t)result;
}

/* ------------------------------------------------------------------------- */
/* 0x0012c02c: 2048-entry waveform lookup-table construction                 */
/* ------------------------------------------------------------------------- */

typedef struct P26WaveHooks {
    uint64_t (*u32_to_double)(uint32_t value);
    uint64_t (*double_mul)(uint64_t a, uint64_t b);
    float (*double_to_float)(uint64_t raw);
    float (*wave_a)(float value);
    float (*wave_b)(float value);
} P26WaveHooks;

typedef struct P26WaveTables {
    float table_a[0x800];
    float table_b[0x800];
    uint64_t scale_a;
    uint64_t scale_b;
    uint64_t fraction_scale;
    P26WaveHooks hooks;
} P26WaveTables;

void snes_p26_0012c02c(P26WaveTables *s)
{
    uint32_t i;

    for (i = 0; i < 0x800u; ++i) {
        uint64_t raw = s->hooks.u32_to_double(i);
        float f;

        raw = s->hooks.double_mul(raw, s->scale_a);
        raw = s->hooks.double_mul(raw, s->fraction_scale);
        f = s->hooks.double_to_float(raw);
        s->table_a[i] = s->hooks.wave_a(f);

        raw = s->hooks.u32_to_double(i);
        raw = s->hooks.double_mul(raw, s->scale_b);
        raw = s->hooks.double_mul(raw, s->fraction_scale);
        f = s->hooks.double_to_float(raw);
        s->table_b[i] = s->hooks.wave_b(f);
    }
}

/* ------------------------------------------------------------------------- */
/* 0x0012c188 / 0x0012c2dc / 0x0012c398: fixed-point helpers                */
/* ------------------------------------------------------------------------- */

typedef struct P26TrigTables {
    int16_t fine[256];       /* logical target 0x00340f98 */
    int16_t coarse[256];     /* logical target 0x00341198 */
    int16_t reciprocal[128]; /* logical target 0x00340e90 */
} P26TrigTables;

void snes_p26_0012c188(const P26TrigTables *t, int16_t mantissa,
                       int16_t exponent, int16_t *out_mantissa,
                       int16_t *out_exponent)
{
    int32_t m = mantissa;
    int32_t e = exponent;
    int32_t sign = 1;
    int16_t result;
    int16_t e16;

    if (m == 0) {
        *out_mantissa = INT16_C(0x7fff);
        *out_exponent = INT16_C(0x002f);
        return;
    }

    if (m < 0) {
        if (m < -0x7fff)
            m = -0x7fff;
        sign = -1;
        m = (int16_t)(-m);
    }

    while (m < 0x4000) {
        m = (int16_t)(m << 1);
        e = (int16_t)(e - 1);
    }

    e16 = (int16_t)e;
    if (m == 0x4000) {
        if (sign == 1) {
            result = INT16_C(0x7fff);
        } else {
            result = (int16_t)-0x4000;
            e16 = (int16_t)(e16 - 1);
        }
    } else {
        int32_t r = t->reciprocal[(m - 0x4000) >> 7];
        int32_t x;

        x = ((-r * ((m * r) >> 15)) >> 15) + r;
        x = (int16_t)(x * 2);
        x = ((-x * ((m * x) >> 15)) >> 15) + x;
        result = (int16_t)((int16_t)(x * 2) * sign);
    }

    *out_mantissa = result;
    *out_exponent = (int16_t)(1 - e16);
}

int16_t snes_p26_0012c2dc(const P26TrigTables *t, int16_t angle)
{
    uint16_t u;
    int32_t value;

    if ((uint16_t)angle == 0x8000u)
        return 0;

    if (angle < 0)
        return (int16_t)-snes_p26_0012c2dc(t, (int16_t)-angle);

    u = (uint16_t)angle;
    value = (int32_t)t->coarse[u >> 8] +
            (((int32_t)t->fine[u & 0xffu] *
              (int32_t)t->coarse[(uint16_t)(u + 0x4000u) >> 8]) >> 15);

    if (value > 0x7fff)
        value = 0x7fff;
    if (value < -0x8000)
        value = -0x7fff;
    return (int16_t)value;
}

int16_t snes_p26_0012c398(const P26TrigTables *t, int16_t angle)
{
    uint16_t u;
    int32_t value;

    if ((uint16_t)angle == 0x8000u)
        return INT16_MIN;

    if (angle < 0)
        angle = (int16_t)-angle;

    u = (uint16_t)angle;
    value = (int32_t)t->coarse[(uint16_t)(u + 0x4000u) >> 8] -
            (((int32_t)t->fine[u & 0xffu] *
              -(int32_t)t->coarse[(uint16_t)(u + 0x8000u) >> 8]) >> 15);

    if (value > 0x7fff)
        value = 0x7fff;
    if (value < -0x8000)
        value = -0x7fff;
    return (int16_t)value;
}

/* ------------------------------------------------------------------------- */
/* 0x0012d334 / 0x0012d4ac / 0x0012d624: three identical matrix builders   */
/* ------------------------------------------------------------------------- */

typedef struct P26MatrixGroup {
    int16_t out[9];
    int16_t scale;
    int16_t angle_a;
    int16_t angle_c;
    int16_t angle_b;
} P26MatrixGroup;

static void p26_build_matrix(P26MatrixGroup *g, const P26TrigTables *t)
{
    int32_t sa = snes_p26_0012c2dc(t, g->angle_a);
    int32_t ca = snes_p26_0012c398(t, g->angle_a);
    int32_t sb = snes_p26_0012c2dc(t, g->angle_b);
    int32_t cb = snes_p26_0012c398(t, g->angle_b);
    int32_t sc = snes_p26_0012c2dc(t, g->angle_c);
    int32_t cc = snes_p26_0012c398(t, g->angle_c);
    int32_t half = (int16_t)g->scale >> 1;
    int32_t hca, hsa, hca_cc, hsa_sc, hsa_cc, hca_sc;

    g->scale = (int16_t)(g->scale >> 1);

    hca = (half * ca) >> 15;
    hsa = (half * sa) >> 15;
    hca_cc = (hca * cc) >> 15;
    hsa_sc = (hsa * sc) >> 15;
    hsa_cc = (hsa * cc) >> 15;
    hca_sc = (hca * sc) >> 15;

    g->out[0] = (int16_t)((hca * cb) >> 15);
    g->out[1] = (int16_t)-((hsa * cb) >> 15);
    g->out[7] = (int16_t)(hca_sc + ((hsa_cc * sb) >> 15));
    g->out[8] = (int16_t)((((half * cc) >> 15) * cb) >> 15);
    g->out[2] = (int16_t)((half * sb) >> 15);
    g->out[3] = (int16_t)(hsa_cc + ((hca_sc * sb) >> 15));
    g->out[4] = (int16_t)(hca_cc - ((hsa_sc * sb) >> 15));
    g->out[5] = (int16_t)-((((half * sc) >> 15) * cb) >> 15);
    g->out[6] = (int16_t)(hsa_sc - ((hca_cc * sb) >> 15));
}

void snes_p26_0012d334(P26MatrixGroup *g, const P26TrigTables *t)
{
    p26_build_matrix(g, t);
}

void snes_p26_0012d4ac(P26MatrixGroup *g, const P26TrigTables *t)
{
    p26_build_matrix(g, t);
}

void snes_p26_0012d624(P26MatrixGroup *g, const P26TrigTables *t)
{
    p26_build_matrix(g, t);
}

/* ------------------------------------------------------------------------- */
/* 0x0012e5b8: packed-nibble resampler                                       */
/* ------------------------------------------------------------------------- */

typedef struct P26NibbleResampler {
    int length;
    int source_scale;
    const uint8_t *source;
    uint8_t *destination;
} P26NibbleResampler;

void snes_p26_0012e5b8(P26NibbleResampler *s)
{
    uint8_t temporary[512];
    int i;

    if (s->length == 0)
        __builtin_trap();

    for (i = 0; i < s->length * 2; ++i) {
        uint32_t index = (uint32_t)((i * s->source_scale) / s->length);
        uint8_t packed = s->source[index >> 1];

        temporary[i] = (index & 1u) == 0 ? (packed >> 4) : (packed & 0x0fu);
    }

    for (i = 0; i < s->length; ++i)
        s->destination[i] = (uint8_t)(temporary[i * 2] << 4) |
                            temporary[i * 2 + 1];
}

/* ------------------------------------------------------------------------- */
/* 0x001305a0 / 0x001306f8 / 0x001309c4: accelerator/runtime corridor       */
/* ------------------------------------------------------------------------- */

typedef int (*P26AccelCallback)(uint64_t arg);

typedef struct P26AccelInitArgs {
    uint32_t mode;
    uint8_t *output;
    uint32_t bank_count;
    uint32_t bank_base;
    uint32_t page_count;
    uint32_t page_base;
} P26AccelInitArgs;

typedef struct P26AccelState {
    uint8_t scratch[0x7fc];
    uint32_t mirror_words[16];

    uint8_t *output;
    uint32_t bank_count;
    uint32_t bank_base;
    uint32_t page_count;
    uint32_t page_base;

    uint32_t page_map[256];
    uint32_t bank_map_a[4];
    uint32_t bank_map_b[4];

    P26AccelCallback *dispatch_a_sets[4];
    void *dispatch_b_sets[4];
    void *dispatch_c_sets[4];
    P26AccelCallback *dispatch_a;
    void *dispatch_b;
    void *dispatch_c;

    uint32_t cached_mode;
    uint32_t cached_format;
    uint32_t status;
    int16_t zero_test;
    uint16_t bit15_test;
    int32_t signed_range_test;
    uint8_t byte_84;
    uint16_t half_88;
    uint8_t byte_8c;
    uint16_t half_90;
    uint8_t dispatch_select;
    int dispatch_override;
    uint8_t initialized;
    uint8_t *current_output;

    void (*refresh)(void *opaque);
    int (*probe)(void *opaque);
    void *opaque;
} P26AccelState;

void snes_p26_001305a0(P26AccelState *s)
{
    unsigned i;
    uint32_t status;

    for (i = 0; i < 16; ++i) {
        s->output[i * 2] = (uint8_t)s->mirror_words[i];
        s->output[i * 2 + 1] = (uint8_t)(s->mirror_words[i] >> 8);
    }

    status = s->status;
    status = s->zero_test == 0 ? (status | 2u) : (status & ~2u);
    status = (s->bit15_test & 0x8000u) != 0 ? (status | 8u) : (status & ~8u);
    status = ((uint32_t)s->signed_range_test + 0x8000u < 0x10000u)
           ? (status & ~0x10u) : (status | 0x10u);
    status = s->signed_range_test == 0 ? (status & ~4u) : (status | 4u);
    s->status = status;

    s->output[0x30] = (uint8_t)status;
    s->output[0x31] = (uint8_t)(status >> 8);
    s->output[0x34] = s->byte_84;
    s->output[0x36] = (uint8_t)s->half_88;
    s->output[0x3c] = s->byte_8c;
    s->output[0x3e] = (uint8_t)s->half_90;
    s->output[0x3f] = (uint8_t)(s->half_90 >> 8);

    if (s->refresh != NULL)
        s->refresh(s->opaque);
}

void snes_p26_001306f8(P26AccelState *s, const P26AccelInitArgs *args)
{
    uint32_t mode = args->mode & 3u;
    uint32_t i;

    s->dispatch_a = s->dispatch_a_sets[mode];
    s->dispatch_b = s->dispatch_b_sets[mode];
    s->dispatch_c = s->dispatch_c_sets[mode];

    memset(s->scratch, 0, sizeof(s->scratch));
    s->output = args->output;
    s->bank_count = args->bank_count;
    s->bank_base = args->bank_base;
    s->page_count = args->page_count > 0x20u ? 0x20u : args->page_count;
    s->page_base = args->page_base;
    s->cached_mode = UINT32_MAX;
    s->cached_format = UINT32_MAX;

    memset(s->output, 0, 0x300);
    s->output[0x3b] = 0;

    if (s->page_count == 0)
        __builtin_trap();

    for (i = 0; i < 0x100u; ++i) {
        uint32_t index = i & 0x7fu;

        if (index < 0x40u) {
            uint32_t divisor = s->page_count << 1;
            if (divisor == 0)
                __builtin_trap();
            s->page_map[i] = s->page_base +
                             (index % divisor) * 0x10000u +
                             0x200000u;
        } else {
            if (s->page_count < 2u)
                index &= 1u;
            else
                index %= s->page_count;
            s->page_map[i] = s->page_base + index * 0x10000u;
        }
    }

    if (s->bank_count == 0)
        __builtin_trap();

    for (i = 0; i < 4u; ++i) {
        uint32_t address = s->bank_base + (i % s->bank_count) * 0x10000u;
        s->bank_map_a[i] = address;
        s->bank_map_b[i] = address;
    }

    s->initialized = 1;
    s->current_output = s->output + 0x100;

    if (s->refresh != NULL)
        s->refresh(s->opaque);
}

int snes_p26_001309c4(P26AccelState *s, uint64_t arg)
{
    int result;

    if (s->refresh != NULL)
        s->refresh(s->opaque);

    if (s->probe(s->opaque) == 0) {
        s->status &= ~0x20u;
        snes_p26_001305a0(s);
        return 0;
    }

    s->status &= ~0x8000u;
    result = s->dispatch_a[s->dispatch_select != 0 ? 1 : 0](arg);
    snes_p26_001305a0(s);

    return s->dispatch_override == 0 ? result : s->dispatch_override;
}

/* ------------------------------------------------------------------------- */
/* 0x0015e298 / 0x0015ecec / 0x0015eeac: PPU/memory helpers                 */
/* ------------------------------------------------------------------------- */

typedef struct P26PpuMemory {
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
} P26PpuMemory;

static uint16_t p26_read_u16(const uint8_t *p)
{
    return (uint16_t)p[0] | (uint16_t)((uint16_t)p[1] << 8);
}

static uint32_t p26_read_u24(const uint8_t *p)
{
    return (uint32_t)p[0] |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16);
}

uint8_t snes_p26_0015e298(P26PpuMemory *s, uint32_t address)
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

void snes_p26_0015ecec(P26PpuMemory *s)
{
    uint32_t mode = s->io[0x2231] & 3u;
    uint32_t planes = mode == 0 ? 8u : (mode == 1 ? 4u : 2u);
    uint32_t bank = (uint32_t)((s->ring_index & 7u) == 0);
    uint8_t *src = s->tile_scratch + bank * 0x40u;
    uint8_t *dst = s->io + 0x3000u + p26_read_u16(s->io + 0x2235) +
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

void snes_p26_0015eeac(P26PpuMemory *s)
{
    uint32_t source24 = p26_read_u24(s->io + 0x2232);
    uint32_t dest24 = p26_read_u24(s->io + 0x2235);
    uint32_t mode = s->io[0x2230] & 3u;
    uint32_t count = p26_read_u16(s->io + 0x2238);
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
