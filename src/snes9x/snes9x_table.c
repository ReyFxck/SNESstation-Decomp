/*
 * SNES Station v0.23 - SNES9X Table Initialization
 * Canonical source from progress11_core.c (0x0014308c range)
 */

#include <stddef.h>
#include <stdint.h>

/* 0x0014308c: initialize 8x256 16-bit table */
void snes9x_table_init_0014308c(uint16_t table[8u * 256u], uint8_t *ready_flag)
{
    uint32_t group;

    for (group = 0u; group < 8u; ++group) {
        uint32_t value;
        for (value = 0u; value < 256u; ++value) {
            uint32_t part_a = ((value & 0xc0u) >> 3) | (group & 4u);
            uint32_t part_b = ((value & 0x38u) >> 1) | (group & 2u);
            uint32_t part_c = ((value & 7u) << 2) | ((group & 1u) << 1);
            table[(group << 8) + value] =
                (uint16_t)((part_a << 10) | (part_b << 5) | part_c);
        }
    }
    if (ready_flag != NULL)
        *ready_flag = 0u;
}