/*
 * SNES Station v0.23 - SNES9X Code Stream State
 * Canonical source from progress11_core.c (0x00183bdc range)
 */

#include <stddef.h>
#include <stdint.h>

typedef struct Snes9xCodeStreamState {
    uint8_t bytes[13];
    int8_t index;
    uint8_t blocked;
} Snes9xCodeStreamState;

/* 0x00183bdc: fetch next byte from code stream */
uint8_t snes9x_code_stream_fetch_00183bdc(Snes9xCodeStreamState *state,
                                          void (*refill)(Snes9xCodeStreamState *, void *),
                                          void *opaque)
{
    int index;

    if (state->blocked != 0u)
        return 0u;
    index = state->index;
    if (index >= 0) {
        if (index < 13) {
            uint8_t result = state->bytes[index];
            state->index = (int8_t)(index + 1);
            return result;
        }
        state->index = -1;
        return 15u;
    }
    if (refill != NULL)
        refill(state, opaque);
    state->index = (int8_t)((uint8_t)state->index + 1u);
    return 15u;
}