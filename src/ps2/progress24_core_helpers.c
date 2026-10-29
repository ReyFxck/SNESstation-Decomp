/*
 * Progress 24: typed behavioral migration of four short Progress-16 helpers.
 *
 * Target entries represented here:
 *   0x0010a264
 *   0x0010b7f8
 *   0x00115b58
 *   0x0016ded4
 *
 * The models deliberately preserve address-labelled ownership where historical
 * names and final project-wide structures are still unproven.  No compiler-
 * matching claim is made here.
 */

#include <stdint.h>
#include <string.h>

/* Existing recovered libgcc helper used by the target at 0x0010a264. */
extern uint64_t snes___muldi3(uint64_t a, uint64_t b);

/*
 * 0x0010a264
 *
 * Four-way packed blend.  The target derives two 5-bit weights from the top
 * inputs, builds four packed lane values with the two global masks, multiplies
 * them through __muldi3 and folds the accumulated 64-bit result back into the
 * original lane positions.
 */
uint32_t snes_p24_0010a264(uint32_t mask_lo, uint32_t mask_hi,
                           uint32_t value1, uint32_t value2,
                           uint32_t value3, uint32_t value4,
                           uint32_t weight_src1, uint32_t weight_src2)
{
    uint32_t w1 = (weight_src1 >> 11) & 0x1fu;
    uint32_t w2 = (weight_src2 >> 11) & 0x1fu;
    int64_t cross = (int64_t)((w1 * w2) >> 5);
    int64_t c3 = (int64_t)w2 - cross;
    int64_t c4 = cross;
    int64_t c1 = (cross - (int64_t)w1 - (int64_t)w2) + 0x20;
    int64_t c2 = (int64_t)w1 - cross;
    uint32_t p1 = (value1 & mask_lo) | ((value1 & mask_hi) << 16);
    uint32_t p2 = (value2 & mask_lo) | ((value2 & mask_hi) << 16);
    uint32_t p3 = (value3 & mask_lo) | ((value3 & mask_hi) << 16);
    uint32_t p4 = (value4 & mask_lo) | ((value4 & mask_hi) << 16);
    uint64_t sum;

    sum  = snes___muldi3((uint64_t)c3, (uint64_t)p3);
    sum += snes___muldi3((uint64_t)c4, (uint64_t)p4);
    sum += snes___muldi3((uint64_t)c1, (uint64_t)p1);
    sum += snes___muldi3((uint64_t)c2, (uint64_t)p2);

    return ((uint32_t)(sum >> 5) & mask_lo) |
           ((uint32_t)(sum >> 21) & mask_hi);
}

/*
 * 0x0010b7f8
 *
 * The original masks the current register selector to seven bits, returns the
 * default shadow byte for most selectors, and gives the x8/x9 register families
 * special per-slot handling.
 */
typedef struct P24RegisterReadState {
    uint8_t selector_f2;
    uint8_t shadow[0x80];

    uint32_t slot_active[8];
    uint8_t slot_status_hi[8];
    uint8_t slot_status_lo[8];

    uint8_t (*read_slot_gate)(void *opaque, unsigned slot);
    void *opaque;
} P24RegisterReadState;

uint8_t snes_p24_0010b7f8(P24RegisterReadState *state)
{
    uint32_t selector = state->selector_f2 & 0x7fu;
    unsigned slot = selector >> 4;
    uint8_t result = state->shadow[selector];

    if ((selector & 0x0fu) == 8u) {
        result = state->read_slot_gate(state->opaque, slot);
    } else if ((selector & 0x0fu) == 9u) {
        result = 0;
        if (state->slot_active[slot] != 0) {
            result = state->slot_status_hi[slot] |
                     state->slot_status_lo[slot];
        }
    }

    return result;
}

/*
 * 0x00115b58
 *
 * Reset/initialization sequencing for the core.  The three large stores are
 * kept explicit because the target clears/fills exactly 0x8000, 0x10000 and
 * 0x20000 bytes before executing the remaining subsystem callbacks.
 */
typedef struct P24CoreResetState {
    uint8_t *region_34e2c4; /* target DAT_0034e2c4, 0x8000 bytes */
    uint8_t *region_34e2b8; /* target DAT_0034e2b8, 0x10000 bytes */
    uint8_t *region_34e2b0; /* target DAT_0034e2b0, 0x20000 bytes */

    uint8_t flag_345540;
    uint8_t flag_345545;
    uint8_t flag_345544;
    uint8_t flag_345543;
    uint8_t flag_345547;
    uint8_t latch_345501;

    void (*call_by_address)(void *opaque, uint32_t address);
    void *opaque;
} P24CoreResetState;

static void p24_call(P24CoreResetState *state, uint32_t address)
{
    state->call_by_address(state->opaque, address);
}

void snes_p24_00115b58(P24CoreResetState *state)
{
    if (state->flag_345540 != 0)
        p24_call(state, UINT32_C(0x001159f4));

    memset(state->region_34e2c4, 0x00, 0x8000);
    memset(state->region_34e2b8, 0x00, 0x10000);
    memset(state->region_34e2b0, 0x55, 0x20000);

    if (state->flag_345545 != 0)
        p24_call(state, UINT32_C(0x001832a4));

    p24_call(state, UINT32_C(0x00115a18));
    p24_call(state, UINT32_C(0x0015c124));
    p24_call(state, UINT32_C(0x00183660));

    if (state->flag_345544 != 0)
        p24_call(state, UINT32_C(0x0016fa18));

    p24_call(state, UINT32_C(0x0012b9a4));
    p24_call(state, UINT32_C(0x0010a934));
    p24_call(state, UINT32_C(0x0012e6c4));
    p24_call(state, UINT32_C(0x0015d8ec));

    if (state->flag_345543 != 0)
        p24_call(state, UINT32_C(0x0010c300));

    p24_call(state, UINT32_C(0x001140e0));

    if (state->flag_345547 != 0)
        p24_call(state, UINT32_C(0x00158ff0));

    state->latch_345501 = 0;
}

/*
 * 0x0016ded4
 *
 * CPU stack/status push helper.  It conditionally pushes the accumulator,
 * pushes the PC offset high/low bytes, synthesizes the status byte from the
 * split target latches, updates SP/status state, and finally refreshes the
 * 0x2207-derived runtime state.
 */
typedef struct P24CpuPushState {
    uint16_t stack_pointer;      /* target 0x00345af0 */
    uint16_t status;             /* target 0x00345aea */
    uint8_t accumulator;         /* target 0x00345ae8 */

    uint32_t current_pc;         /* logical target 0x00345b18 */
    uint32_t pc_base;            /* logical target 0x00345b1c */

    uint8_t carry_latch;         /* 0x00345afc */
    uint8_t zero_latch;          /* 0x00345afd */
    uint8_t negative_latch;      /* 0x00345afe, bit 7 significant */
    uint8_t overflow_latch;      /* 0x00345aff, bit 0 -> status bit 6 */

    uint8_t status_shadow;       /* 0x0035b768 */
    uint8_t wait_latch;          /* 0x00345b04 */
    uint16_t io_2207;

    void (*write_byte)(void *opaque, uint8_t value, uint16_t address);
    void (*refresh_2207)(void *opaque, uint16_t value);
    void *opaque;
} P24CpuPushState;

void snes_p24_0016ded4(P24CpuPushState *state)
{
    uint16_t sp_before = state->stack_pointer;
    uint32_t pc_delta;
    uint8_t pushed_status;

    if ((state->status & 0x0100u) == 0) {
        state->stack_pointer = (uint16_t)(state->stack_pointer - 1u);
        state->write_byte(state->opaque, state->accumulator, sp_before);
    }

    pc_delta = state->current_pc - state->pc_base;
    state->write_byte(state->opaque,
                      (uint8_t)((pc_delta >> 8) & 0xffu),
                      state->stack_pointer);
    state->write_byte(state->opaque,
                      (uint8_t)(pc_delta & 0xffu),
                      (uint16_t)(state->stack_pointer - 1u));

    pushed_status = (uint8_t)(state->status & 0x003cu);
    pushed_status |= (uint8_t)(state->carry_latch & 1u);
    pushed_status |= (uint8_t)(state->negative_latch & 0x80u);
    pushed_status |= (uint8_t)((state->overflow_latch & 1u) << 6);
    if (state->zero_latch == 0)
        pushed_status |= 2u;

    state->write_byte(state->opaque, pushed_status,
                      (uint16_t)(state->stack_pointer - 2u));
    state->stack_pointer = (uint16_t)(state->stack_pointer - 3u);

    state->status = (uint16_t)((state->status & 0xff00u) | pushed_status);
    state->accumulator = 0;
    state->status_shadow = (uint8_t)state->status;
    state->status = (uint16_t)((state->status & 0xfff7u) | 4u);
    state->wait_latch = 0;

    state->refresh_2207(state->opaque, state->io_2207);
}
