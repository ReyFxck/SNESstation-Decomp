/*
 * SNES Station v0.23 - SNES9X State Management
 * Canonical source from progress11_core.c (0x00130c78 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static uint32_t snes9x_load_u32(const uint8_t *base, size_t offset)
{
    uint32_t value;
    memcpy(&value, base + offset, sizeof(value));
    return value;
}

static void snes9x_store_u32(uint8_t *base, size_t offset, uint32_t value)
{
    memcpy(base + offset, &value, sizeof(value));
}

/* 0x00130c78: state update with PC block tracking */
void snes9x_state_update_00130c78(uint8_t *state,
                                  void (*reset_helper)(void *opaque), void *opaque,
                                  uint32_t target_self_address)
{
    uint32_t pc = snes9x_load_u32(state, 0x3cu);
    uint32_t block = pc & UINT32_C(0x0000fff0);
    uint32_t old_block = snes9x_load_u32(state, 0x58u);

    if (old_block != block || state[0x5ecu] == 0u) {
        if (reset_helper != NULL)
            reset_helper(opaque);
        snes9x_store_u32(state, 0x58u, block);
        state[0x5ecu] = 1u;
    }
    snes9x_store_u32(state, 0x3cu, pc + 1u);
    snes9x_store_u32(state, 0x48u,
                      snes9x_load_u32(state, 0x48u) & UINT32_C(0xffffecff));
    snes9x_store_u32(state, 0x64u, target_self_address);
    snes9x_store_u32(state, 0x68u, target_self_address);
}