/*
 * SNES Station v0.23 - SNES9X APU I/O Write
 * Canonical source from progress13_core_helpers.c (0x0010b72c range)
 */

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *ram;
    uint16_t timer_target[3];
    uint8_t timer_target_written[3];
} Snes9xApuIoState;

/* 0x0010b72c -- mirrored APU I/O/timer-target byte write. */
void snes9x_apu_io_write_0010b72c(Snes9xApuIoState *state,
                                   uint16_t address, uint8_t value)
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