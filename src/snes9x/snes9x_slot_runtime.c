/*
 * SNES Station v0.23 - SNES9X Controller/Slot Runtime
 * Canonical source from progress12_slot_runtime.c (0x00173f24..0x00174830 range)
 */

#include <stddef.h>
#include <stdint.h>

typedef struct Snes9xAxisSlot {
    int32_t state;
    int16_t raw_x;
    int16_t raw_y;
    int32_t phase;
    int32_t period;
    int32_t scale;
    int16_t scaled_x;
    int16_t scaled_y;
    int32_t mode;
    int32_t value_3c;
    int32_t value_4c;
    int32_t active_a4;
    uint32_t packed_a8;
} Snes9xAxisSlot;

typedef void (*Snes9xConfigureSlot)(Snes9xAxisSlot *slot, int32_t arg1,
                                     int32_t arg2, int32_t arg3, void *opaque);
typedef void (*Snes9xIndexSlotCallback)(uint32_t index, Snes9xAxisSlot *slot,
                                        void *opaque);
typedef void (*Snes9xTimingCallback)(uint8_t value, void *opaque);
typedef void (*Snes9xRecomputeCallback)(uint32_t index, int32_t value,
                                        void *opaque);

static int32_t snes9x_mul32(int32_t a, int32_t b)
{
    return (int32_t)((uint32_t)a * (uint32_t)b);
}

static int16_t snes9x_scale_div128(int16_t value, int32_t scale)
{
    int32_t product = snes9x_mul32((int32_t)value, scale);
    return (int16_t)(product / 128);
}

/* 0x00173f24: index*0xe0 wrapper forwarding the remaining three arguments. */
void snes9x_slot_configure_00173f24(Snes9xAxisSlot *slots, uint32_t index,
                                     int32_t arg1, int32_t arg2, int32_t arg3,
                                     Snes9xConfigureSlot configure, void *opaque)
{
    if (configure != NULL)
        configure(&slots[index], arg1, arg2, arg3, opaque);
}

/*
 * 0x00173f50: raw-axis update. When the target mode flag is clear the first
 * axis becomes (abs(x)+abs(y))/2 before the per-slot Q7-style scaling.
 */
void snes9x_slot_update_raw_00173f50(Snes9xAxisSlot *slot, int16_t x, int16_t y,
                                      uint32_t keep_first_axis)
{
    int32_t ax = x;
    int32_t ay = y;

    if (keep_first_axis == 0u) {
        if (ax < 0)
            ax = -ax;
        if (ay < 0)
            ay = -ay;
        x = (int16_t)((ax + ay) / 2);
    }

    slot->raw_x = x;
    slot->raw_y = y;
    slot->scaled_x = snes9x_scale_div128(x, slot->scale);
    slot->scaled_y = snes9x_scale_div128(y, slot->scale);
}

/*
 * 0x0017422c: derive a period from mode*rate, optionally double it, fold the
 * current phase with signed remainder, then forward the target config byte.
 */
void snes9x_slot_set_period_0017422c(Snes9xAxisSlot *slot, int32_t mode, int32_t rate,
                                      uint32_t double_period, uint8_t config_byte,
                                      Snes9xTimingCallback apply, void *opaque)
{
    int32_t product = snes9x_mul32(mode, rate);
    int32_t numerator = (int32_t)((uint32_t)product << 9);
    int32_t period = numerator / 32000;

    if (double_period != 0u)
        period = (int32_t)((uint32_t)period << 1);
    slot->period = period;
    if (period == 0)
        slot->phase = 0;
    else
        slot->phase %= period;

    if (apply != NULL)
        apply(config_byte, opaque);
}

/* 0x001742f8: mark a nonzero slot active and reconfigure it as (8,-1,0). */
void snes9x_slot_activate_001742f8(Snes9xAxisSlot *slots, uint32_t index,
                                    Snes9xConfigureSlot configure, void *opaque)
{
    Snes9xAxisSlot *slot = &slots[index];

    if (slot->state == 0)
        return;
    slot->active_a4 = 4;
    slot->state = 4;
    if (configure != NULL)
        configure(slot, 8, -1, 0, opaque);
}

/* 0x00174618: set scale/mode fields, recalc axes, optionally dispatch stop. */
void snes9x_slot_set_scale_00174618(Snes9xAxisSlot *slots, uint32_t index, int32_t scale,
                                     Snes9xIndexSlotCallback stop, void *opaque)
{
    Snes9xAxisSlot *slot = &slots[index];

    slot->packed_a8 = (uint32_t)scale << 24;
    slot->scale = scale;
    slot->scaled_x = snes9x_scale_div128(slot->raw_x, scale);
    slot->scaled_y = snes9x_scale_div128(slot->raw_y, scale);

    if (scale == 0 && slot->state != 0 && slot->state != 5 && stop != NULL)
        stop(index, slot, opaque);
}

/* 0x001746a4: target's two-global gate around slot +0x30/+0x4c. */
int32_t snes9x_slot_gate_001746a4(const Snes9xAxisSlot *slots, uint32_t index,
                                   uint8_t gate_a, uint8_t gate_b)
{
    const Snes9xAxisSlot *slot = &slots[index];

    if (slot->state == 0)
        return 0;
    if (slot->state == 5)
        return gate_b != 0u ? slot->value_4c : 0;
    return (gate_a != 0u || gate_b != 0u) ? slot->value_4c : 0;
}

/* 0x00174830: store +0x3c then call the slot recomputation entry. */
void snes9x_slot_recompute_00174830(Snes9xAxisSlot *slots, uint32_t index, int32_t value,
                                     Snes9xRecomputeCallback recompute, void *opaque)
{
    slots[index].value_3c = value;
    if (recompute != NULL)
        recompute(index, value, opaque);
}