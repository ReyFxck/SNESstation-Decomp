/*
 * SNES Station v0.23 SNES9X - Controller/Slot Runtime
 * Canonical source from progress12_slot_runtime_recovered.c
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
void snes9x_slot_configure(Snes9xAxisSlot *slots, uint32_t index,
                           int32_t arg1, int32_t arg2, int32_t arg3,
                           Snes9xConfigureSlot configure, void *opaque)
{
    if (configure != NULL)
        configure(&slots[index], arg1, arg2, arg3, opaque);
}

/* 0x00173f50: raw-axis update. When the target mode flag is clear the first
 * axis becomes (abs(x)+abs(y))/2 before the per-slot Q7-style scaling. */
void snes9x_slot_update_raw(Snes9xAxisSlot *slot, int16_t x, int16_t y,
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