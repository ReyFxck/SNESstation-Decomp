/*
 * Progress 13 -- additional compact runtime/core helpers recovered from full
 * target control flow.  Original global EE addresses are represented as
 * explicit state/parameter objects so the behavior remains host-testable.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* 0x001a7580 -- libgcc signed DI -> double conversion helper (__floatdidf). */
double snes_p13_floatdidf_001a7580(int64_t value)
{
    /* The target builds high*2^32 plus an unsigned-corrected low word. */
    int32_t high = (int32_t)(value >> 32);
    int32_t low_signed = (int32_t)(uint32_t)value;
    double result = (double)high * 65536.0 * 65536.0;
    double low = (double)low_signed;
    if (low_signed < 0)
        low += 4294967296.0;
    return result + low;
}

/* 0x001825e4 / 0x00183778 -- identical month-length helpers. */
static int snes_p13_days_in_month_impl(unsigned month, unsigned year)
{
    unsigned index = month - 2u;
    if (index >= 10u)
        return 31;
    if (index == 0u)
        return (year & 3u) != 0 ? 28 : 29;
    switch (index) {
    case 1: case 3: case 5: case 6: case 8:
        return 31;
    default:
        return 30;
    }
}
int snes_p13_days_in_month_001825e4(unsigned month, unsigned year)
{
    return snes_p13_days_in_month_impl(month, year);
}
int snes_p13_days_in_month_00183778(unsigned month, unsigned year)
{
    return snes_p13_days_in_month_impl(month, year);
}

/* 0x0015cdf8 -- state machine; target intentionally falls through stages. */
typedef struct {
    uint32_t state;
    uint8_t flag_553f;
    uint8_t flag_553e;
    uint8_t flag_5611;
    uint8_t flag_553d;
} SnesP13StageState;

void snes_p13_stage_advance_0015cdf8(SnesP13StageState *s)
{
    switch (s->state) {
    default:
    case 0:
        s->state = 1;
        return;
    case 1:
        if (s->flag_553f != 0) { s->state = 2; return; }
        /* fall through */
    case 2:
        if (s->flag_553f != 0) { s->state = 3; return; }
        /* fall through */
    case 3:
        if (s->flag_553e != 0) { s->state = 4; return; }
        /* fall through */
    case 4:
        if (s->flag_5611 != 0) { s->state = 5; return; }
        /* fall through */
    case 5:
        if (s->flag_5611 != 0) { s->state = 6; return; }
        /* fall through */
    case 6:
        if (s->flag_553d == 0) s->state = 1;
        else s->state = 0;
        return;
    }
}

/* 0x0012b9a4 -- initialize eight 0x16-byte records and RAM I/O sentinels. */
void snes_p13_record_ram_init_0012b9a4(uint8_t *records, uint8_t *ram)
{
    unsigned i;
    for (i = 0; i < 8; ++i) {
        uint8_t *r = records + i * 0x16u;
        int16_t minus_one = -1;
        r[0] = 0;
        r[1] = 1;
        r[2] = 0;
        r[3] = 0xff;
        r[4] = 0xff;
        memcpy(r + 6, &minus_one, sizeof(minus_one));
        memcpy(r + 8, &minus_one, sizeof(minus_one));
        r[0x0a] = 0xff;
        memcpy(r + 0x0c, &minus_one, sizeof(minus_one));
        r[0x0e] = 0;
    }
    for (i = 0; i < 8; ++i) {
        unsigned base = 0x4300u + i * 0x10u;
        memset(ram + base, 0xff, 0x0cu);
        ram[base + 0x0f] = 0xff;
    }
}

/* 0x0010a18c -- packed-channel weighted blend using the target masks. */
uint32_t snes_p13_color_blend_0010a18c(uint32_t a, uint32_t b,
                                       uint32_t control,
                                       uint32_t low_mask,
                                       uint32_t high_mask)
{
    unsigned weight_b;
    uint64_t packed_a, packed_b, sum;
    if (a == b)
        return b;

    weight_b = (control >> 11) & 31u;
    packed_a = (uint64_t)(a & low_mask) |
               ((uint64_t)(a & high_mask) << 16);
    packed_b = (uint64_t)(b & low_mask) |
               ((uint64_t)(b & high_mask) << 16);
    sum = (uint64_t)(32u - weight_b) * packed_a +
          (uint64_t)weight_b * packed_b;
    return (uint32_t)(((sum >> 5) & low_mask) |
                      ((sum >> 21) & high_mask));
}

/* 0x0015e07c -- Snes9x map-entry -> current base/pointer selector. */
typedef struct {
    uint32_t current_ptr; /* target +0x20 */
    uint32_t current_base;/* target +0x24 */
    uint32_t alternate28; /* target +0x28 */
} SnesP13MapCursor;

void snes_p13_set_map_cursor_0015e07c(
    uint32_t address, const uint32_t *map_entries,
    uint32_t main_ram, uint32_t secondary_ram, uint32_t default_ram,
    SnesP13MapCursor *cursor)
{
    uint32_t slot = map_entries[(address >> 12) & 0xfffu];
    uint32_t base;
    uint32_t low = address & 0xffffu;

    if (slot >= 12u) {
        base = slot;
    } else {
        switch (slot) {
        case 0: base = main_ram - 0x2000u; break;
        case 1: base = main_ram - 0x4000u; break;
        case 2: base = main_ram - 0x6000u; break;
        case 3: case 4: case 11:
            base = secondary_ram; break;
        case 8:
            base = cursor->alternate28 - 0x6000u; break;
        default:
            base = default_ram; break;
        }
    }
    cursor->current_base = base;
    cursor->current_ptr = base + low;
}

/* 0x00173e6c -- configure one 0xe0-byte controller/runtime slot. */
typedef struct {
    uint32_t table_index;      /* +0x00 */
    uint8_t pad04[0x20];
    int16_t value24;           /* +0x24 */
    uint8_t pad26[0x0a];
    uint64_t period30;         /* +0x30 */
    uint32_t pad38;            /* +0x38 */
} SnesP13RuntimeSlot;

/* Field offsets match the 0x3c-byte target record.  A 64-bit host may add
 * tail padding to sizeof() because period30 is naturally 8-byte aligned. */
typedef char snes_p13_runtime_slot_period30_offset[
    (offsetof(SnesP13RuntimeSlot, period30) == 0x30) ? 1 : -1];
typedef char snes_p13_runtime_slot_pad38_offset[
    (offsetof(SnesP13RuntimeSlot, pad38) == 0x38) ? 1 : -1];

void snes_p13_config_slot_00173e6c(SnesP13RuntimeSlot *slot,
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

/* 0x00174120 -- gate, clear-on-enable, and install eight slot callbacks. */
typedef struct {
    uint32_t enabled08;
    uint32_t available18;
    uint32_t requested1c;
} SnesP13ControllerGate;

typedef void (*SnesP13ClearFn)(void *opaque);
void snes_p13_controller_mask_00174120(
    uint32_t requested_mask, SnesP13ControllerGate *gate,
    uint8_t global_disable, uint32_t callback_enabled,
    uint32_t callback_disabled, uint32_t callback_slots[8],
    SnesP13ClearFn clear_large, SnesP13ClearFn clear_small, void *opaque)
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

/* 0x001836d0 -- weekday helper over the target's decimal digit fields. */
int snes_p13_weekday_001836d0(uint8_t digit8, uint8_t digit9,
                              uint8_t month, uint8_t year_hundreds,
                              uint8_t year_tens, uint8_t year_ones)
{
    static const int offset_by_month[13] = {0,1,4,4,0,2,5,0,3,6,1,4,6};
    uint32_t year = (uint32_t)year_hundreds * 100u +
                    (uint32_t)year_tens * 10u + (uint32_t)year_ones;
    uint32_t day = (uint32_t)digit9 * 10u + (uint32_t)digit8;
    uint32_t m = month < 13u ? month : 1u;
    uint32_t y = year - 900u;
    uint32_t value = y + (y >> 2) + (uint32_t)offset_by_month[m] + day;

    value -= 1u;
    if ((y & 3u) == 0 && m < 3u)
        value -= 1u;
    return (int)(value % 7u);
}
