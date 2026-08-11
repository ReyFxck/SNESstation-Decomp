/*
 * Progress 12: compact 0xe0-byte slot/controller helpers recovered directly
 * from SNES Station v0.23 target assembly.
 *
 * The target uses fixed global arrays around 0x0034db50/0x0034db80.  The host
 * models expose the observed fields as structures and keep address-labelled
 * entry names rather than inventing historical symbols.
 */
#include <stddef.h>
#include <stdint.h>

typedef struct P12AxisSlot {
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
} P12AxisSlot;

typedef void (*P12ConfigureSlot)(P12AxisSlot *slot, int32_t arg1,
                                 int32_t arg2, int32_t arg3, void *opaque);
typedef void (*P12IndexSlotCallback)(uint32_t index, P12AxisSlot *slot,
                                    void *opaque);
typedef void (*P12TimingCallback)(uint8_t value, void *opaque);
typedef void (*P12RecomputeCallback)(uint32_t index, int32_t value,
                                    void *opaque);

static int32_t p12_mul32(int32_t a, int32_t b)
{
    return (int32_t)((uint32_t)a * (uint32_t)b);
}

static int16_t p12_scale_div128(int16_t value, int32_t scale)
{
    int32_t product = p12_mul32((int32_t)value, scale);
    return (int16_t)(product / 128);
}

/* 0x00173f24: index*0xe0 wrapper forwarding the remaining three arguments. */
void snes_p12_00173f24(P12AxisSlot *slots, uint32_t index,
                       int32_t arg1, int32_t arg2, int32_t arg3,
                       P12ConfigureSlot configure, void *opaque)
{
    if (configure != NULL)
        configure(&slots[index], arg1, arg2, arg3, opaque);
}

/*
 * 0x00173f50: raw-axis update.  When the target mode flag is clear the first
 * axis becomes (abs(x)+abs(y))/2 before the per-slot Q7-style scaling.
 */
void snes_p12_00173f50(P12AxisSlot *slot, int16_t x, int16_t y,
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
    slot->scaled_x = p12_scale_div128(x, slot->scale);
    slot->scaled_y = p12_scale_div128(y, slot->scale);
}

/*
 * 0x0017422c: derive a period from mode*rate, optionally double it, fold the
 * current phase with signed remainder, then forward the target config byte.
 */
void snes_p12_0017422c(P12AxisSlot *slot, int32_t mode, int32_t rate,
                       uint32_t double_period, uint8_t config_byte,
                       P12TimingCallback apply, void *opaque)
{
    int32_t product = p12_mul32(mode, rate);
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
void snes_p12_001742f8(P12AxisSlot *slots, uint32_t index,
                       P12ConfigureSlot configure, void *opaque)
{
    P12AxisSlot *slot = &slots[index];

    if (slot->state == 0)
        return;
    slot->active_a4 = 4;
    slot->state = 4;
    if (configure != NULL)
        configure(slot, 8, -1, 0, opaque);
}

/* 0x00174618: set scale/mode fields, recalc axes, optionally dispatch stop. */
void snes_p12_00174618(P12AxisSlot *slots, uint32_t index, int32_t scale,
                       P12IndexSlotCallback stop, void *opaque)
{
    P12AxisSlot *slot = &slots[index];

    slot->packed_a8 = (uint32_t)scale << 24;
    slot->scale = scale;
    slot->scaled_x = p12_scale_div128(slot->raw_x, scale);
    slot->scaled_y = p12_scale_div128(slot->raw_y, scale);

    if (scale == 0 && slot->state != 0 && slot->state != 5 && stop != NULL)
        stop(index, slot, opaque);
}

/* 0x001746a4: target's two-global gate around slot +0x30/+0x4c. */
int32_t snes_p12_001746a4(const P12AxisSlot *slots, uint32_t index,
                          uint8_t gate_a, uint8_t gate_b)
{
    const P12AxisSlot *slot = &slots[index];

    if (slot->state == 0)
        return 0;
    if (slot->state == 5)
        return gate_b != 0u ? slot->value_4c : 0;
    return (gate_a != 0u || gate_b != 0u) ? slot->value_4c : 0;
}

/* 0x00174830: store +0x3c then call the slot recomputation entry. */
void snes_p12_00174830(P12AxisSlot *slots, uint32_t index, int32_t value,
                       P12RecomputeCallback recompute, void *opaque)
{
    slots[index].value_3c = value;
    if (recompute != NULL)
        recompute(index, value, opaque);
}
