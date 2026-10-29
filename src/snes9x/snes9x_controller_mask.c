/*
 * SNES Station v0.23 - SNES9X Controller Mask Configuration
 * Canonical source from progress13_runtime_more.c (0x00173e6c, 0x00174120 range)
 */

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t table_index;      /* +0x00 */
    uint8_t pad04[0x20];
    int16_t value24;           /* +0x24 */
    uint8_t pad26[0x0a];
    uint64_t period30;         /* +0x30 */
    uint32_t pad38;            /* +0x38 */
} Snes9xRuntimeSlot;

/* Field offsets match the 0x3c-byte target record. */
typedef char snes9x_runtime_slot_period30_offset[
    (offsetof(Snes9xRuntimeSlot, period30) == 0x30) ? 1 : -1];
typedef char snes9x_runtime_slot_pad38_offset[
    (offsetof(Snes9xRuntimeSlot, pad38) == 0x38) ? 1 : -1];

/* 0x00173e6c -- configure one 0xe0-byte controller/runtime slot. */
void snes9x_config_slot_00173e6c(Snes9xRuntimeSlot *slot,
                                  int32_t divisor, uint32_t mode,
                                  int16_t value24,
                                  const uint32_t table[/* at least index+1 */],
                                  uint32_t factor)
{
    slot->value24 = value24;
    if (divisor == -1) {
        divisor = 0;
        slot->pad38 = 0;
    } else {
        slot->pad38 = mode;
    }

    if (divisor == 0 || factor == 0) {
        slot->period30 = 0;
        return;
    }

    {
        uint64_t numerator = (uint64_t)table[slot->table_index] *
                             UINT64_C(0x03e80000);
        uint64_t denominator = (uint64_t)(uint32_t)divisor * factor;
        slot->period30 = denominator != 0 ? numerator / denominator : 0;
    }
}

typedef struct {
    uint32_t enabled08;
    uint32_t available18;
    uint32_t requested1c;
} Snes9xControllerGate;

typedef void (*Snes9xClearFn)(void *opaque);

/* 0x00174120 -- gate, clear-on-enable, and install eight slot callbacks. */
void snes9x_controller_mask_00174120(
    uint32_t requested_mask, Snes9xControllerGate *gate,
    uint8_t global_disable, uint32_t callback_enabled,
    uint32_t callback_disabled, uint32_t callback_slots[8],
    Snes9xClearFn clear_large, Snes9xClearFn clear_small, void *opaque)
{
    unsigned i;
    uint32_t requested = requested_mask & 0xffu;

    gate->requested1c = requested;
    if (gate->available18 == 0 || global_disable != 0)
        requested = 0;

    if (requested != 0 && gate->enabled08 == 0) {
        if (clear_large != NULL) clear_large(opaque);
        if (clear_small != NULL) clear_small(opaque);
    }
    gate->enabled08 = requested;

    for (i = 0; i < 8; ++i)
        callback_slots[i] = ((requested >> i) & 1u) != 0
                                ? callback_enabled : callback_disabled;
}