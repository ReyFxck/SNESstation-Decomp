#ifndef SNESSTATION_ROM_OFFSETS_RECOVERED_H
#define SNESSTATION_ROM_OFFSETS_RECOVERED_H
#include <stdint.h>
/* EE addresses are 32 bits. These are byte offsets into the loaded ROM, not
 * globals at a numerically identical address in the application's image. */
#define P28_ROM_AT(base, offset) \
    ((uint8_t *)(uintptr_t)((uint32_t)(base) + (uint32_t)(offset)))
#endif
