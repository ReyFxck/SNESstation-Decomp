/*
 * Progress 23: typed behavioral migration of four short audio/runtime helpers
 * from the committed Progress-16 R5900 decompile.
 *
 * Target entries represented here:
 *   0x00174564
 *   0x00174728
 *   0x00177cec
 *   0x00177db0
 *
 * The original code stores eight records 0xe0 bytes apart.  This source keeps
 * byte-accurate offsets for the fields touched by these helpers while leaving
 * final project-wide ownership/naming for a later milestone.  No compiler-
 * matching claim is made here.
 */

#include <stdint.h>
#include <string.h>

#define P23_SLOT_COUNT 8u
#define P23_SLOT_BYTES 0xe0u

/* Offsets relative to target slot base 0x0034db80. */
enum {
    P23_SLOT_KIND = 0x00,
    P23_SLOT_OVERRIDE = 0x04,
    P23_SLOT_RATE = 0x10,
    P23_SLOT_ARG2 = 0x40,
    P23_SLOT_ARG3 = 0x48,
    P23_SLOT_ARG4 = 0x50,
    P23_SLOT_ARG6 = 0x58,
    P23_SLOT_LENGTH = 0x60,
    P23_SLOT_MODE = 0xa4
};

typedef struct P23AudioHooks {
    void (*reset_all)(void *opaque, int full_reset);
    void (*set_enabled)(void *opaque, int enabled);
    int (*configure_runtime)(void *opaque, uint64_t mode,
                             uint32_t option, uint64_t arg);
    void (*show_message)(void *opaque, int group, int code,
                         const char *text);
    void (*configure_slot)(void *opaque, unsigned slot, uint64_t value,
                           int64_t selector, int scale);
    void *opaque;
} P23AudioHooks;

typedef struct P23AudioRuntime {
    uint8_t slots[P23_SLOT_COUNT][P23_SLOT_BYTES];

    /* Logical views of the target globals used by 0x00174728/0x00177cec. */
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
    int32_t  override_table[32];/* logical source 0x003f3fc0 */
    uint64_t curve_scale_df;    /* raw DFmode constant at 0x001b83e8 */

    const char *init_error_text;/* target pointer 0x001b8450 */
    P23AudioHooks hooks;
} P23AudioRuntime;

static uint32_t p23_load_u32(const uint8_t *slot, unsigned offset)
{
    uint32_t value;
    memcpy(&value, slot + offset, sizeof(value));
    return value;
}

static uint64_t p23_load_u64(const uint8_t *slot, unsigned offset)
{
    uint64_t value;
    memcpy(&value, slot + offset, sizeof(value));
    return value;
}

static void p23_store_u32(uint8_t *slot, unsigned offset, uint32_t value)
{
    memcpy(slot + offset, &value, sizeof(value));
}

static void p23_store_u64(uint8_t *slot, unsigned offset, uint64_t value)
{
    memcpy(slot + offset, &value, sizeof(value));
}

/* Existing recovered libgcc/goFast behavioral models. */
extern uint64_t snes___udivdi3(uint64_t numerator, uint64_t denominator);
extern uint64_t snes_fptodp(float value);
extern uint64_t snes_dpmul(uint64_t a, uint64_t b);
extern uint64_t snes___fixunsdfdi(uint64_t raw);

/*
 * 0x00174564
 *
 * Stores the five incoming configuration values into the selected 0xe0-byte
 * record, derives the target's integer scale, and only forwards modes 1, 2 or
 * 3 to the lower-level slot configurator.  Mode 1 substitutes arg2 and fixed
 * {selector=1, scale=127}; mode 3 substitutes arg4 and {-1, 0}; mode 2 keeps
 * the caller's arg3 and derived scale.
 */
void snes_p23_00174564(P23AudioRuntime *runtime, unsigned slot_index,
                       uint64_t arg2, uint64_t arg3, uint64_t arg4,
                       int count, uint64_t arg6)
{
    uint8_t *slot = runtime->slots[slot_index];
    int kind = (int32_t)p23_load_u32(slot, P23_SLOT_KIND);
    int scale = (count * 0x7f + 0x7f) >> 3;
    int expected;
    int64_t selector = -1;

    p23_store_u64(slot, P23_SLOT_ARG6, arg6);
    p23_store_u64(slot, P23_SLOT_LENGTH, (uint64_t)(int64_t)(count + 1));
    p23_store_u64(slot, P23_SLOT_ARG2, arg2);
    p23_store_u64(slot, P23_SLOT_ARG3, arg3);
    p23_store_u64(slot, P23_SLOT_ARG4, arg4);

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
 * 0x00174728
 *
 * Recomputes slot +0x10 from a 16.16 numerator divided by 0x003ab720.  When
 * the slot's override flag is one, the numerator comes from the 32-entry
 * table indexed by 0x0034542f & 0x1f.  The optional second stage follows the
 * target's fptodp -> dpmul -> __fixunsdfdi conversion chain.
 */
void snes_p23_00174728(P23AudioRuntime *runtime, unsigned slot_index,
                       int64_t value)
{
    uint8_t *slot = runtime->slots[slot_index];
    uint32_t rate;

    if (runtime->rate_divisor == 0)
        return;

    if (p23_load_u32(slot, P23_SLOT_OVERRIDE) == 1) {
        value = runtime->override_table[runtime->override_selector & 0x1f];
    }

    rate = (uint32_t)snes___udivdi3((uint64_t)value << 16,
                                    runtime->rate_divisor);
    p23_store_u32(slot, P23_SLOT_RATE, rate);

    if (runtime->curve_enabled != 0) {
        uint64_t raw = snes_fptodp((float)rate);
        raw = snes_dpmul(raw, runtime->curve_scale_df);
        rate = (uint32_t)snes___fixunsdfdi(raw);
        p23_store_u32(slot, P23_SLOT_RATE, rate);
    }
}

/*
 * 0x00177cec
 *
 * Initializes the short runtime-global group, performs the full-reset helper,
 * and only opens/configures audio when mode&7 is nonzero.  A zero return from
 * the target configuration routine triggers the existing UI/error path and
 * changes the wrapper result from one to zero.
 */
int snes_p23_00177cec(P23AudioRuntime *runtime, uint64_t mode,
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
 * 0x00177db0
 *
 * Small per-slot state transition gate.  The target accepts 1 only from
 * states 0/1, accepts 4 from any nonzero state, and accepts 5..9 unless the
 * current state is 4.  In the 5..9 case a nonzero slot kind mirrors the new
 * state into slot +0x00.
 */
int snes_p23_00177db0(P23AudioRuntime *runtime, unsigned slot_index,
                      int requested)
{
    uint8_t *slot = runtime->slots[slot_index];
    int current = (int32_t)p23_load_u32(slot, P23_SLOT_MODE);

    switch (requested) {
    case 1:
        if ((uint32_t)current < 2u) {
            p23_store_u32(slot, P23_SLOT_MODE, 1u);
            return 1;
        }
        return 0;

    case 4:
        if (current == 0)
            return 0;
        p23_store_u32(slot, P23_SLOT_MODE, 4u);
        return 1;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if (current == 4)
            return 0;
        p23_store_u32(slot, P23_SLOT_MODE, (uint32_t)requested);
        if (p23_load_u32(slot, P23_SLOT_KIND) != 0)
            p23_store_u32(slot, P23_SLOT_KIND, (uint32_t)requested);
        return 1;

    default:
        return 0;
    }
}

/* Keep the raw read helper referenced so offset/layout experiments can reuse it. */
uint64_t snes_p23_audio_slot_arg3(const P23AudioRuntime *runtime,
                                  unsigned slot_index)
{
    return p23_load_u64(runtime->slots[slot_index], P23_SLOT_ARG3);
}
