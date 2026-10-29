/*
 * SNES Station v0.23 PS2 - CPU Stack/Status Push Helper
 * Canonical source from progress24_core_helpers.c @ 0x0016ded4
 */

#include <stdint.h>

typedef struct {
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
} SnesCpuPushState;

void snes_cpu_push_status_0016ded4(SnesCpuPushState *state)
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