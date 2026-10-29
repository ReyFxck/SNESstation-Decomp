/*
 * SNES Station v0.23 - SNES9X I/O Helpers
 * Canonical source from progress11_core.c (0x0015e034, 0x0015de14 range)
 */

#include <stddef.h>
#include <stdint.h>

typedef uint8_t (*Snes9xReadByte)(uint32_t address, void *opaque);
typedef void (*Snes9xWriteByte)(uint8_t value, uint32_t address, void *opaque);

/* 0x0015e034: 16-bit write via byte callback */
void snes9x_write16_0015e034(uint16_t value, uint32_t address,
                             Snes9xWriteByte write_byte, void *opaque)
{
    if (write_byte == NULL)
        return;
    write_byte((uint8_t)value, address, opaque);
    write_byte((uint8_t)(value >> 8), address + 1u, opaque);
}

/* 0x0015de14: 16-bit read via byte callback */
uint16_t snes9x_read16_0015de14(uint32_t address, uint8_t *open_bus_shadow,
                                Snes9xReadByte read_byte, void *opaque)
{
    uint8_t lo = 0u;
    uint8_t hi = 0u;

    if (read_byte != NULL) {
        lo = read_byte(address, opaque);
        if (open_bus_shadow != NULL)
            *open_bus_shadow = lo;
        hi = read_byte(address + 1u, opaque);
    }
    return (uint16_t)((uint16_t)lo | ((uint16_t)hi << 8));
}