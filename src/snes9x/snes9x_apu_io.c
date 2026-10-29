/*
 * SNES Station v0.23 SNES9X - APU I/O
 * Canonical source from progress13_core_helpers_recovered.c @ 0x0010b72c
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "ee_intrinsics_recovered.h"

#define lrintf(value) snes_ee_lrintf(value)

/* 0x0010b72c -- mirrored APU I/O/timer-target byte write. */
typedef struct {
    uint8_t *ram;
    uint16_t timer_target[3];
    uint8_t timer_target_written[3];
} Snes9xApuIoState;

void snes9x_apu_io_write(Snes9xApuIoState *state, uint16_t address, uint8_t value)
{
    state->ram[address] = value;

    if (address >= 0xfau && address <= 0xfcu) {
        unsigned timer = (unsigned)(address - 0xfau);
        uint16_t target = state->ram[address];
        if (target == 0)
            target = 0x100;
        state->timer_target[timer] = target;
        state->timer_target_written[timer] = 1;
    }
}