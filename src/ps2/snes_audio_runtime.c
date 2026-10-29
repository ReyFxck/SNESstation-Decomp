/*
 * SNES Station v0.23 - Audio Runtime
 * Canonical source from progress23_audio_helpers_recovered.c
 * Target entries: 0x00174564, 0x00174728, 0x00177cec, 0x00177db0
 */

#include <stdint.h>
#include <string.h>

#define SNES_AUDIO_SLOT_COUNT    8u
#define SNES_AUDIO_SLOT_BYTES    0xe0u

/* Offsets relative to target slot base 0x0034db80. */
enum {
    SNES_AUDIO_SLOT_KIND       = 0x00,
    SNES_AUDIO_SLOT_OVERRIDE   = 0x04,
    SNES_AUDIO_SLOT_RATE       = 0x10,
    SNES_AUDIO_SLOT_ARG2       = 0x40,
    SNES_AUDIO_SLOT_ARG3       = 0x48,
    SNES_AUDIO_SLOT_ARG4       = 0x50,
    SNES_AUDIO_SLOT_ARG6       = 0x58,
    SNES_AUDIO_SLOT_LENGTH     = 0x60,
    SNES_AUDIO_SLOT_MODE       = 0xa4
};

typedef struct SnesAudioHooks {
    void (*reset_all)(void *opaque, int full_reset);
    void (*set_enabled)(void *opaque, int enabled);
    int (*configure_runtime)(void *opaque, uint64_t mode,
                             uint32_t option, uint64_t arg);
    void (*show_message)(void *opaque, int group, int code,
                         const char *text);
    void (*configure_slot)(void *opaque, unsigned slot, uint64_t value,
                           int64_t selector, int scale);
    void *opaque;
} SnesAudioHooks;

typedef struct SnesAudioRuntime {
    uint8_t slots[SNES_AUDIO_SLOT_COUNT][SNES_AUDIO_SLOT_BYTES];

    /* Logical views of the target globals */
    uint32_t init_marker;       /* 0x003ab718 */
    uint32_t master_level;      /* 0x003ab71c */
    uint32_t rate_divisor;      /* 0x003ab720 */
    uint32_t runtime_flags;     /* 0x003ab724 */
    uint32_t option_byte;       /* 0x003ab730 */
    uint8_t  saved_option;      /* logical source 0x0034554f */
    uint8_t  latched_option;    /* 0x003ab734 */
    uint8_t  latch_clear;       /* 0x003ab735 */

    uint8_t  override_selector; /* logical source 0x0034542f */
    uint8_t  curve_enabled;     /* logical source 0x00345562 */
    int32_t  override_table[32]; /* logical source 0x003f3fc0 */
    uint64_t curve_scale_df;    /* raw DFmode constant at 0x001b83e8 */

    const char *init_error_text; /* target pointer 0x001b8450 */
    SnesAudioHooks hooks;
} SnesAudioRuntime;

static uint32_t snes_audio_load_u32(const uint8_t *slot, unsigned offset)
{
    uint32_t value;
    memcpy(&value, slot + offset, sizeof(value));
    return value;
}

static uint64_t snes_audio_load_u64(const uint8_t *slot, unsigned offset)
{
    uint64_t value;
    memcpy(&value, slot + offset, sizeof(value));
    return value;
}

static void snes_audio_store_u32(uint8_t *slot, unsigned offset, uint32_t value)
{
    memcpy(slot + offset, &value, sizeof(value));
}

static void snes_audio_store_u64(uint8_t *slot, unsigned offset, uint64_t value)
{
    memcpy(slot + offset, &value, sizeof(value));
}

/* External libgcc/goFast behavioral models. */
extern uint64_t snes___udivdi3(uint64_t numerator, uint64_t denominator);
extern uint64_t snes_fptodp(float value);
extern uint64_t snes_dpmul(uint64_t a, uint64_t b);
extern uint64_t snes___fixunsdfdi(uint64_t raw);

/*
 * 0x00174564 -- Configure audio slot with derived scale and mode-specific args.
 */
void snes_audio_configure_slot_00174564(SnesAudioRuntime *runtime, unsigned slot_index,
                                       uint64_t arg2, uint64_t arg3, uint64_t arg4,
                                       int count, uint64_t arg6)
{
    uint8_t *slot = runtime->slots[slot_index];
    int kind = (int32_t)snes_audio_load_u32(slot, SNES_AUDIO_SLOT_KIND);
    int scale = (count * 0x7f + 0x7f) >> 3;
    int expected;
    int64_t selector = -1;

    snes_audio_store_u64(slot, SNES_AUDIO_SLOT_ARG6, arg6);
    snes_audio_store_u64(slot, SNES_AUDIO_SLOT_LENGTH, (uint64_t)(int64_t)(count + 1));
    snes_audio_store_u64(slot, SNES_AUDIO_SLOT_ARG2, arg2);
    snes_audio_store_u64(slot, SNES_AUDIO_SLOT_ARG3, arg3);
    snes_audio_store_u64(slot, SNES_AUDIO_SLOT_ARG4, arg4);

    if (kind != 2) {
        if (kind < 3) {
            selector = 1;
            scale = 0x7f;
            expected = 1;
            arg4 = arg2;
        } else {
            scale = 0;
            expected = 3;
        }
        arg3 = arg4;
        if (kind != expected)
            return;
    }

    runtime->hooks.configure_slot(runtime->hooks.opaque, slot_index,
                                  arg3, selector, scale);
}

/*
 * 0x00174728 -- Recompute slot rate from 16.16 numerator / rate_divisor,
 * with optional fptodp -> dpmul -> __fixunsdfdi curve application.
 */
void snes_audio_recompute_rate_00174728(SnesAudioRuntime *runtime, unsigned slot_index,
                                         int64_t value)
{
    uint8_t *slot = runtime->slots[slot_index];
    uint32_t rate;

    if (runtime->rate_divisor == 0)
        return;

    if (snes_audio_load_u32(slot, SNES_AUDIO_SLOT_OVERRIDE) == 1) {
        value = runtime->override_table[runtime->override_selector & 0x1f];
    }

    rate = (uint32_t)snes___udivdi3((uint64_t)value << 16,
                                    runtime->rate_divisor);
    snes_audio_store_u32(slot, SNES_AUDIO_SLOT_RATE, rate);

    if (runtime->curve_enabled != 0) {
        uint64_t raw = snes_fptodp((float)rate);
        raw = snes_dpmul(raw, runtime->curve_scale_df);
        rate = (uint32_t)snes___fixunsdfdi(raw);
        snes_audio_store_u32(slot, SNES_AUDIO_SLOT_RATE, rate);
    }
}

/*
 * 0x00177cec -- Initialize audio runtime globals, perform full reset,
 * and conditionally enable/configure audio.
 */
int snes_audio_init_00177cec(SnesAudioRuntime *runtime, uint64_t mode,
                             uint32_t option, uint64_t arg)
{
    runtime->init_marker = UINT32_MAX;
    runtime->master_level = 0xff;
    runtime->rate_divisor = 0;
    runtime->runtime_flags = 0;
    runtime->latched_option = runtime->saved_option;
    runtime->latch_clear = 0;
    runtime->option_byte = option & 0xff;

    runtime->hooks.reset_all(runtime->hooks.opaque, 1);

    if ((mode & 7u) != 0) {
        runtime->hooks.set_enabled(runtime->hooks.opaque, 1);
        if (runtime->hooks.configure_runtime(runtime->hooks.opaque, mode,
                                             option & 0xff, arg) == 0) {
            runtime->hooks.show_message(runtime->hooks.opaque, 4, 4,
                                        runtime->init_error_text);
            return 0;
        }
    }

    return 1;
}

/*
 * 0x00177db0 -- Per-slot state transition gate.
 * Accepts 1 from states 0/1, accepts 4 from any nonzero state,
 * accepts 5..9 unless current state is 4. In 5..9 case, nonzero kind mirrors new state.
 */
int snes_audio_slot_transition_00177db0(SnesAudioRuntime *runtime, unsigned slot_index,
                                         int requested)
{
    uint8_t *slot = runtime->slots[slot_index];
    int current = (int32_t)snes_audio_load_u32(slot, SNES_AUDIO_SLOT_MODE);

    switch (requested) {
    case 1:
        if ((uint32_t)current < 2u) {
            snes_audio_store_u32(slot, SNES_AUDIO_SLOT_MODE, 1u);
            return 1;
        }
        return 0;

    case 4:
        if (current == 0)
            return 0;
        snes_audio_store_u32(slot, SNES_AUDIO_SLOT_MODE, 4u);
        return 1;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if (current == 4)
            return 0;
        snes_audio_store_u32(slot, SNES_AUDIO_SLOT_MODE, (uint32_t)requested);
        if (snes_audio_load_u32(slot, SNES_AUDIO_SLOT_KIND) != 0)
            snes_audio_store_u32(slot, SNES_AUDIO_SLOT_KIND, (uint32_t)requested);
        return 1;

    default:
        return 0;
    }
}

/* Accessor functions for the audio slots */
uint32_t snes_audio_slot_get_kind(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u32(rt->slots[slot], SNES_AUDIO_SLOT_KIND);
}

void snes_audio_slot_set_kind(SnesAudioRuntime *rt, unsigned slot, uint32_t kind)
{
    snes_audio_store_u32(rt->slots[slot], SNES_AUDIO_SLOT_KIND, kind);
}

uint32_t snes_audio_slot_get_override(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u32(rt->slots[slot], SNES_AUDIO_SLOT_OVERRIDE);
}

void snes_audio_slot_set_override(SnesAudioRuntime *rt, unsigned slot, uint32_t override)
{
    snes_audio_store_u32(rt->slots[slot], SNES_AUDIO_SLOT_OVERRIDE, override);
}

uint32_t snes_audio_slot_get_rate(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u32(rt->slots[slot], SNES_AUDIO_SLOT_RATE);
}

void snes_audio_slot_set_rate(SnesAudioRuntime *rt, unsigned slot, uint32_t rate)
{
    snes_audio_store_u32(rt->slots[slot], SNES_AUDIO_SLOT_RATE, rate);
}

uint64_t snes_audio_slot_get_arg2(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u64(rt->slots[slot], SNES_AUDIO_SLOT_ARG2);
}

void snes_audio_slot_set_arg2(SnesAudioRuntime *rt, unsigned slot, uint64_t arg)
{
    snes_audio_store_u64(rt->slots[slot], SNES_AUDIO_SLOT_ARG2, arg);
}

uint64_t snes_audio_slot_get_arg3(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u64(rt->slots[slot], SNES_AUDIO_SLOT_ARG3);
}

void snes_audio_slot_set_arg3(SnesAudioRuntime *rt, unsigned slot, uint64_t arg)
{
    snes_audio_store_u64(rt->slots[slot], SNES_AUDIO_SLOT_ARG3, arg);
}

uint64_t snes_audio_slot_get_arg4(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u64(rt->slots[slot], SNES_AUDIO_SLOT_ARG4);
}

void snes_audio_slot_set_arg4(SnesAudioRuntime *rt, unsigned slot, uint64_t arg)
{
    snes_audio_store_u64(rt->slots[slot], SNES_AUDIO_SLOT_ARG4, arg);
}

uint64_t snes_audio_slot_get_arg6(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u64(rt->slots[slot], SNES_AUDIO_SLOT_ARG6);
}

void snes_audio_slot_set_arg6(SnesAudioRuntime *rt, unsigned slot, uint64_t arg)
{
    snes_audio_store_u64(rt->slots[slot], SNES_AUDIO_SLOT_ARG6, arg);
}

uint32_t snes_audio_slot_get_length(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u32(rt->slots[slot], SNES_AUDIO_SLOT_LENGTH);
}

void snes_audio_slot_set_length(SnesAudioRuntime *rt, unsigned slot, uint32_t length)
{
    snes_audio_store_u32(rt->slots[slot], SNES_AUDIO_SLOT_LENGTH, length);
}

uint32_t snes_audio_slot_get_mode(SnesAudioRuntime *rt, unsigned slot)
{
    return snes_audio_load_u32(rt->slots[slot], SNES_AUDIO_SLOT_MODE);
}

void snes_audio_slot_set_mode(SnesAudioRuntime *rt, unsigned slot, uint32_t mode)
{
    snes_audio_store_u32(rt->slots[slot], SNES_AUDIO_SLOT_MODE, mode);
}