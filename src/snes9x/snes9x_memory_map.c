/*
 * SNES Station v0.23 SNES9X - Memory Mapping
 * Canonical source from progress11_core_recovered.c
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t (*Snes9xReadByte)(uint32_t address, void *opaque);
typedef void (*Snes9xWriteByte)(uint8_t value, uint32_t address, void *opaque);

void snes9x_write16(uint16_t value, uint32_t address,
                     Snes9xWriteByte write_byte, void *opaque)
{
    if (write_byte == NULL)
        return;
    write_byte((uint8_t)value, address, opaque);
    write_byte((uint8_t)(value >> 8), address + 1u, opaque);
}

uint16_t snes9x_read16(uint32_t address, uint8_t *open_bus_shadow,
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

void snes9x_map_page(uint32_t *map, uint32_t data_base_address,
                      uint32_t group, uint32_t page)
{
    for (uint32_t block = 0u; block < 256u; block += 16u) {
        uint32_t pointer = data_base_address + (page << 20) + (block << 12);
        for (uint32_t i = 0u; i < 16u; ++i)
            map[0xc0au + (group << 8) + block + i] = pointer;
    }
}

void snes9x_init_chip_memory(uint8_t *chip_memory, uint32_t *map,
                              uint32_t data_base_address)
{
    memset(chip_memory + 0x4800u, 0, 4u);
    for (uint32_t i = 0u; i < 4u; ++i) {
        chip_memory[0x4804u + i] = (uint8_t)i;
        snes9x_map_page(map, data_base_address, i, i);
    }
}

static uint32_t snes9x_select_336_table(uint32_t status)
{
    if ((status & UINT32_C(0x100)) != 0u)
        return UINT32_C(0x003367f8);
    if ((status & UINT32_C(0x20)) != 0u)
        return (status & UINT32_C(0x10)) != 0u
                   ? UINT32_C(0x003367f8)
                   : UINT32_C(0x00336bf8);
    return (status & UINT32_C(0x10)) != 0u
               ? UINT32_C(0x003373f8)
               : UINT32_C(0x00336ff8);
}

static uint32_t snes9x_select_3f5_table(uint32_t status)
{
    if ((status & UINT32_C(0x100)) != 0u)
        return UINT32_C(0x003f5040);
    if ((status & UINT32_C(0x20)) != 0u)
        return (status & UINT32_C(0x10)) != 0u
                   ? UINT32_C(0x003f5440)
                   : UINT32_C(0x003f5840);
    return (status & UINT32_C(0x10)) != 0u
               ? UINT32_C(0x003f5c40)
               : UINT32_C(0x003f6040);
}