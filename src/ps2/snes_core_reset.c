/*
 * SNES Station v0.23 PS2 - Core Reset/Initialization Sequencing
 * Canonical source from progress24_core_helpers.c @ 0x00115b58
 */

#include <stdint.h>
#include <string.h>

typedef struct {
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
} SnesCoreResetState;

static void snes_core_call(SnesCoreResetState *state, uint32_t address)
{
    state->call_by_address(state->opaque, address);
}

void snes_core_reset_00115b58(SnesCoreResetState *state)
{
    if (state->flag_345540 != 0)
        snes_core_call(state, UINT32_C(0x001159f4));

    memset(state->region_34e2c4, 0x00, 0x8000);
    memset(state->region_34e2b8, 0x00, 0x10000);
    memset(state->region_34e2b0, 0x55, 0x20000);

    if (state->flag_345545 != 0)
        snes_core_call(state, UINT32_C(0x001832a4));

    snes_core_call(state, UINT32_C(0x00115a18));
    snes_core_call(state, UINT32_C(0x0015c124));
    snes_core_call(state, UINT32_C(0x00183660));

    if (state->flag_345544 != 0)
        snes_core_call(state, UINT32_C(0x0016fa18));

    snes_core_call(state, UINT32_C(0x0012b9a4));
    snes_core_call(state, UINT32_C(0x0010a934));
    snes_core_call(state, UINT32_C(0x0012e6c4));
    snes_core_call(state, UINT32_C(0x0015d8ec));

    if (state->flag_345543 != 0)
        snes_core_call(state, UINT32_C(0x0010c300));

    snes_core_call(state, UINT32_C(0x001140e0));

    if (state->flag_345547 != 0)
        snes_core_call(state, UINT32_C(0x00158ff0));

    state->latch_345501 = 0;
}