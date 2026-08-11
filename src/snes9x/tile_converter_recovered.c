/*
 * SNES Station v0.23 WIP — recovered from R5900 code.
 * Original function: 0x00183e04
 *
 * High-confidence reconstruction of the classic Snes9x ConvertTile routine.
 * The names were assigned only after the binary behaviour was recovered;
 * a close-era Snes9x 1.43 source snapshot was then used as validation.
 */

#include <stdint.h>

#define SNES_TRUE 1u
#define BLANK_TILE 2u

/* Runtime globals recovered from the SNES Station image. */
extern uint8_t *g_memory_vram;             /* EE VA 0x0034e2b8 */
extern uint32_t g_bg_bitshift;             /* EE VA 0x0035d454 */
extern uint32_t g_odd_high[4][16];         /* EE VA 0x0035f9a0 */
extern uint32_t g_odd_low[4][16];          /* EE VA 0x0035faa0 */
extern uint32_t g_even_high[4][16];        /* EE VA 0x0035fba0 */
extern uint32_t g_even_low[4][16];         /* EE VA 0x0035fca0 */

static inline void decode_plane_pair(uint8_t a, uint8_t b,
                                     unsigned plane,
                                     uint32_t *p1, uint32_t *p2)
{
    if (a != 0) {
        *p1 |= g_odd_high[plane][a >> 4];
        *p2 |= g_odd_low[plane][a & 0x0f];
    }
    if (b != 0) {
        *p1 |= g_even_high[plane][b >> 4];
        *p2 |= g_even_low[plane][b & 0x0f];
    }
}

uint8_t ConvertTile_recovered(uint8_t *pCache, uint32_t TileAddr)
{
    const uint8_t *tp = g_memory_vram + TileAddr;
    uint32_t *out = (uint32_t *)pCache;
    uint32_t non_zero = 0;
    unsigned line;

    /*
     * The binary has three explicit cases selected by BG.BitShift.
     * Every scanline emits two uint32 words = eight decoded 8-bit pixels.
     * Eight scanlines therefore consume 64 bytes of cache per tile.
     */
    switch (g_bg_bitshift) {
    case 2:
        for (line = 0; line < 8; ++line, tp += 2) {
            uint32_t p1 = 0, p2 = 0;
            decode_plane_pair(tp[0], tp[1], 0, &p1, &p2);
            *out++ = p1;
            *out++ = p2;
            non_zero |= p1 | p2;
        }
        break;

    case 4:
        for (line = 0; line < 8; ++line, tp += 2) {
            uint32_t p1 = 0, p2 = 0;
            decode_plane_pair(tp[0],  tp[1],  0, &p1, &p2);
            decode_plane_pair(tp[16], tp[17], 1, &p1, &p2);
            *out++ = p1;
            *out++ = p2;
            non_zero |= p1 | p2;
        }
        break;

    case 8:
        for (line = 0; line < 8; ++line, tp += 2) {
            uint32_t p1 = 0, p2 = 0;
            decode_plane_pair(tp[0],  tp[1],  0, &p1, &p2);
            decode_plane_pair(tp[16], tp[17], 1, &p1, &p2);
            decode_plane_pair(tp[32], tp[33], 2, &p1, &p2);
            decode_plane_pair(tp[48], tp[49], 3, &p1, &p2);
            *out++ = p1;
            *out++ = p2;
            non_zero |= p1 | p2;
        }
        break;

    default:
        /* Matches the binary's non-2/4/8 fallthrough. */
        return BLANK_TILE;
    }

    return non_zero ? SNES_TRUE : BLANK_TILE;
}
