/*
 * SNES Station v0.23 - Transfer Block Operations
 * Canonical source from progress12_io_state.c (0x00183c58, 0x00183d40 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct SnesTransferBlock {
    uint8_t bytes[0x1c];
} SnesTransferBlock;

typedef void (*SnesRefreshCallback)(void *opaque);

static size_t snes_transfer_offset(uint8_t selector)
{
    size_t offset = 0u;

    if (selector != 0u) {
        unsigned shift = ((unsigned)selector + 3u) & 31u;
        offset = (size_t)((uint32_t)0x80u << shift);
        if (offset > 0x20000u)
            offset = 0x20000u;
    }
    return offset;
}

static void snes_copy_template_to_block(uint8_t *dst, const SnesTransferBlock *src)
{
    dst[0] = src->bytes[0];
    dst[1] = src->bytes[1];
    memcpy(dst + 2u, src->bytes + 2u, 13u);
    dst[15] = src->bytes[15];
    dst[16] = src->bytes[16];
    memcpy(dst + 17u, src->bytes + 20u, 8u);
}

static void snes_copy_block_to_template(SnesTransferBlock *dst, const uint8_t *src)
{
    dst->bytes[0] = src[0];
    dst->bytes[1] = src[1];
    memcpy(dst->bytes + 2u, src + 2u, 13u);
    dst->bytes[15] = src[15];
    dst->bytes[16] = src[16];
    memcpy(dst->bytes + 20u, src + 17u, 8u);
}

/* 0x00183c58: refresh, then copy selected template fields into backing store. */
void snes_transfer_to_backing_00183c58(uint8_t enabled, uint8_t selector,
                                        uint8_t *backing, const SnesTransferBlock *templ,
                                        SnesRefreshCallback refresh, void *opaque)
{
    if (enabled == 0u)
        return;
    if (refresh != NULL)
        refresh(opaque);
    snes_copy_template_to_block(backing + snes_transfer_offset(selector), templ);
}

/* 0x00183d40: inverse copy, then refresh. */
void snes_transfer_from_backing_00183d40(uint8_t enabled, uint8_t selector,
                                          const uint8_t *backing, SnesTransferBlock *templ,
                                          SnesRefreshCallback refresh, void *opaque)
{
    if (enabled == 0u)
        return;
    snes_copy_block_to_template(templ,
                                backing + snes_transfer_offset(selector));
    if (refresh != NULL)
        refresh(opaque);
}