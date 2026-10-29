/*
 * SNES Station v0.23 WIP — recovered from R5900 code.
 * Original functions:
 *   0x001acd04 - normal 4-pixel writer
 *   0x001ace28 - horizontally reversed 4-pixel writer
 *
 * The global GFX structure is intentionally represented by byte offsets until
 * its exact 1.41 layout is reconstructed.
 */
#include <stdint.h>

#define PTR32(base, off) (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U8(base, off)    (*(uint8_t  *)((uint8_t *)(base) + (off)))

extern uint8_t g_gfx_blob[]; /* EE VA 0x0035d480 */

static void write_one(unsigned dst_index, uint8_t pixel,
                      uint8_t *screen, uint8_t *depth,
                      const uint8_t *color_table,
                      uint8_t z_compare, uint8_t z_write)
{
    if (depth[dst_index] < z_compare && pixel != 0) {
        /* Original indexes this table with pixel << 1, then stores one byte. */
        screen[dst_index] = color_table[(unsigned)pixel << 1];
        depth[dst_index] = z_write;
    }
}

void WRITE_4PIXELS_recovered(uint32_t Offset, const uint8_t *Pixels)
{
    uint8_t *screen = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    uint8_t *depth  = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x40) + Offset;
    const uint8_t *colors = (const uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44);
    uint8_t z_compare = U8(g_gfx_blob, 0x4c);
    uint8_t z_write   = U8(g_gfx_blob, 0x4d);

    write_one(0, Pixels[0], screen, depth, colors, z_compare, z_write);
    write_one(1, Pixels[1], screen, depth, colors, z_compare, z_write);
    write_one(2, Pixels[2], screen, depth, colors, z_compare, z_write);
    write_one(3, Pixels[3], screen, depth, colors, z_compare, z_write);
}

void WRITE_4PIXELS_FLIPPED_recovered(uint32_t Offset, const uint8_t *Pixels)
{
    uint8_t *screen = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    uint8_t *depth  = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x40) + Offset;
    const uint8_t *colors = (const uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44);
    uint8_t z_compare = U8(g_gfx_blob, 0x4c);
    uint8_t z_write   = U8(g_gfx_blob, 0x4d);

    write_one(0, Pixels[3], screen, depth, colors, z_compare, z_write);
    write_one(1, Pixels[2], screen, depth, colors, z_compare, z_write);
    write_one(2, Pixels[1], screen, depth, colors, z_compare, z_write);
    write_one(3, Pixels[0], screen, depth, colors, z_compare, z_write);
}
