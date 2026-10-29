/*
 * SNES Station v0.23 PS2 - Accelerator Runtime Corridor
 * Canonical source from progress26_dsp_runtime_batch.c @ 0x001305a0, 0x001306f8, 0x001309c4
 */

#include <stdint.h>
#include <string.h>

typedef int (*SnesAccelCallback)(uint64_t arg);

typedef struct {
    uint32_t mode;
    uint8_t *output;
    uint32_t bank_count;
    uint32_t bank_base;
    uint32_t page_count;
    uint32_t page_base;
} SnesAccelInitArgs;

typedef struct {
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

    SnesAccelCallback *dispatch_a_sets[4];
    void *dispatch_b_sets[4];
    void *dispatch_c_sets[4];
    SnesAccelCallback *dispatch_a;
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
} SnesAccelState;

void snes_accel_refresh_output_001305a0(SnesAccelState *s)
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

void snes_accel_init_001306f8(SnesAccelState *s, const SnesAccelInitArgs *args)
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

int snes_accel_dispatch_001309c4(SnesAccelState *s, uint64_t arg)
{
    int result;

    if (s->refresh != NULL)
        s->refresh(s->opaque);

    if (s->probe(s->opaque) == 0) {
        s->status &= ~0x20u;
        snes_accel_refresh_output_001305a0(s);
        return 0;
    }

    s->status &= ~0x8000u;
    result = s->dispatch_a[s->dispatch_select != 0 ? 1 : 0](arg);
    snes_accel_refresh_output_001305a0(s);

    return s->dispatch_override == 0 ? result : s->dispatch_override;
}