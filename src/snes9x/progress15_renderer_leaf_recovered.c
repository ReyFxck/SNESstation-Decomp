/*
 * Progress 15 -- isolated renderer leaf recovered from SNES Station v0.23.
 */
#include <stdint.h>

/*
 * 0x00143780..0x001437a4.
 *
 * This JAL target uses the caller-carried $t4 value as an implicit index:
 *   *(0x0035d480 + (t4 << 2) + 0x70) = -1;
 *   *(uint8_t *)0x0035c26d = 0;
 *
 * The host model exposes those two target globals as explicit parameters.
 */
void snes_p15_renderer_4bpp_setup_00143780(
    int32_t *renderer_words, unsigned implicit_t4, uint8_t *ready_byte)
{
    renderer_words[implicit_t4 + (0x70u / sizeof(int32_t))] = -1;
    *ready_byte = 0;
}
