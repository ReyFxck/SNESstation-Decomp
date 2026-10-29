/*
 * SNES Station v0.23 - SNES9X Memory Map Initialization
 * Canonical source from progress11_core.c (0x0016f9b0, 0x0016fa18 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* 0x0016f9b0: initialize map entries for a group/page */
void snes9x_map_init_group_0016f9b0(uint32_t *map, uint32_t data_base_address,
                                    uint32_t group, uint32_t page)
{
    uint32_t block;

    for (block = 0u; block < 256u; block += 16u) {
        uint32_t pointer = data_base_address + (page << 20) + (block << 12);
        uint32_t i;
        for (i = 0u; i < 16u; ++i)
            map[0xc0au + (group << 8) + block + i] = pointer;
    }
}

/* 0x0016fa18: initialize chip memory and map for 4 groups */
void snes9x_map_init_chip_0016fa18(uint8_t *chip_memory, uint32_t *map,
                                    uint32_t data_base_address)
{
    uint32_t i;

    memset(chip_memory + 0x4800u, 0, 4u);
    for (i = 0u; i < 4u; ++i) {
        chip_memory[0x4804u + i] = (uint8_t)i;
        snes9x_map_init_group_0016f9b0(map, data_base_address, i, i);
    }
}