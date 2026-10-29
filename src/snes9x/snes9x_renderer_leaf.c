/*
 * SNES Station v0.23 - Renderer 4bpp Setup Leaf
 * Canonical source from progress15_renderer_leaf.c @ 0x00143780
 */

#include <stdint.h>

/* 0x00143780: Renderer 4bpp setup */
void snes9x_renderer_4bpp_setup_00143780(
    int32_t *renderer_words, unsigned implicit_t4, uint8_t *ready_byte)
{
    renderer_words[implicit_t4 + (0x70u / sizeof(int32_t))] = -1;
    *ready_byte = 0;
}