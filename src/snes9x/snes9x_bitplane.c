/*
 * SNES Station v0.23 SNES9X - Bitplane Transpose
 * Canonical source from progress13_frontend_more_recovered.c @ 0x0012e374
 */

#include <stddef.h>
#include <stdint.h>

/* 0x0012e374 -- transpose eight groups of four source bytes into four bit-plane
 * bytes. This is a direct expression of the target shifts/masks. */
void snes9x_bitplane_transpose(const uint8_t src[32],
                                uint8_t dst_a[16],
                                uint8_t dst_b[16])
{
    for (unsigned i = 0; i < 8; ++i) {
        uint8_t b0 = src[i * 4 + 0];
        uint8_t b1 = src[i * 4 + 1];
        uint8_t b2 = src[i * 4 + 2];
        uint8_t b3 = src[i * 4 + 3];

        dst_a[i * 2 + 0] = (uint8_t)(
            ((b0 & 0x10u) << 3) | ((b0 & 0x01u) << 6) |
            ((b1 & 0x10u) << 1) | ((b1 & 0x01u) << 4) |
            ((b2 & 0x10u) >> 1) | ((b2 & 0x01u) << 2) |
            ((b3 & 0x10u) >> 3) |  (b3 & 0x01u));
        dst_a[i * 2 + 1] = (uint8_t)(
            ((b0 & 0x20u) << 2) | ((b0 & 0x02u) << 5) |
             (b1 & 0x20u)       | ((b1 & 0x02u) << 3) |
            ((b2 & 0x20u) >> 2) | ((b2 & 0x02u) << 1) |
            ((b3 & 0x20u) >> 4) | ((b3 & 0x02u) >> 1));
        dst_b[i * 2 + 0] = (uint8_t)(
            ((b0 & 0x40u) << 1) | ((b0 & 0x04u) << 4) |
            ((b1 & 0x40u) >> 1) | ((b1 & 0x04u) << 2) |
            ((b2 & 0x40u) >> 3) |  (b2 & 0x04u)       |
            ((b3 & 0x40u) >> 5) | ((b3 & 0x04u) >> 2));
        dst_b[i * 2 + 1] = (uint8_t)(
             (b0 & 0x80u)       | ((b0 & 0x08u) << 3) |
            ((b1 & 0x80u) >> 2) | ((b1 & 0x08u) << 1) |
            ((b2 & 0x80u) >> 4) | ((b2 & 0x08u) >> 1) |
            ((b3 & 0x80u) >> 6) | ((b3 & 0x08u) >> 3));
    }
}