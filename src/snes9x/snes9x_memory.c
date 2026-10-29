/*
 * SNES Station v0.23 - SNES9X Memory Address Translation
 * Canonical source from progress11_core.c (0x00182910, 0x0018255c range)
 */

#include <stddef.h>
#include <stdint.h>

static uint32_t snes9x_special_bank(uint32_t address, uint8_t bank_d,
                                     uint8_t bank_e, uint8_t bank_f)
{
    uint32_t type = (address >> 16) & 0xf0u;

    if (type == 0xd0u)
        return (uint32_t)bank_d << 20;
    if (type == 0xe0u)
        return (uint32_t)bank_e << 20;
    if (type == 0xf0u)
        return (uint32_t)bank_f << 20;
    return 0u;
}

/* 0x00182910: translate address with special bank handling */
uint32_t snes9x_translate_address_00182910(uint32_t address,
                                            uint32_t memory_base_address,
                                            uint8_t bank_d, uint8_t bank_e,
                                            uint8_t bank_f)
{
    return memory_base_address + snes9x_special_bank(address, bank_d, bank_e, bank_f)
           + (address & UINT32_C(0x000f0000));
}

/* 0x0018255c: read byte with special bank handling */
uint8_t snes9x_read_byte_0018255c(const uint8_t *memory, uint32_t address,
                                   uint32_t base_offset, uint8_t bank_d,
                                   uint8_t bank_e, uint8_t bank_f)
{
    uint32_t offset = snes9x_special_bank(address, bank_d, bank_e, bank_f)
                      + base_offset + (address & UINT32_C(0x000fffff));
    return memory[offset];
}