/*
 * SNES Station v0.23 — per-ROM cleanup helpers recovered from target R5900
 * code at 0x00151330..0x001513bb.
 *
 * The target does not prove the historical C++ method names, so the public
 * labels retain semantic/address names only.
 */

#include <stdint.h>
#include <stdlib.h>

/*
 * 0x00150f54 is a proven JAL target called with (memory, 0), but its exact
 * historical symbol/semantics are not claimed by this translation unit.
 */
extern void snes_memory_helper_00150f54(void *memory, int mode);

void per_rom_buffer_cleanup_00151360(void *memory)
{
    uint8_t *base = (uint8_t *)memory;
    uint32_t *buffer_b064 = (uint32_t *)(void *)(base + 0xb064);
    uint32_t *buffer_b068 = (uint32_t *)(void *)(base + 0xb068);

    /*
     * In the target this is expressed as (memory + 0x8000) + 0x3064/0x3068.
     * Preserve the effective object offsets and the exact free/null order.
     */
    if (*buffer_b064) {
        free((void *)(uintptr_t)*buffer_b064);
        *buffer_b064 = 0;
    }

    if (*buffer_b068) {
        free((void *)(uintptr_t)*buffer_b068);
        *buffer_b068 = 0;
    }
}

/*
 * Keep this symbol spelling because memory_init_recovered.c already references
 * it. The progress manifest's semantic label for 0x00151330 is
 * "per_rom_cleanup"; neither spelling is claimed to be the historical name.
 */
void rom_cleanup_00151330(void *memory)
{
    per_rom_buffer_cleanup_00151360(memory);
    snes_memory_helper_00150f54(memory, 0);
}
