/*
 * SNES Station v0.23 - SNES9X Multibyte/Name Validation
 * Canonical source from progress13_frontend_more.c (0x00158a58, 0x00158974 range)
 */

#include <stddef.h>
#include <stdint.h>

/* Existing 0x00158b3c predicate, duplicated locally only as behavior. */
static int snes9x_is_lead_byte(uint8_t value)
{
    if ((value & 0x80u) == 0)
        return 0;
    return (((uint8_t)(value - 0x20u)) & 0x40u) != 0;
}

/* 0x00158a58 -- target's bounded multibyte-name validator. */
int snes9x_multibyte_validate_00158a58(const uint8_t *p)
{
    int remaining = 16;
    int pairs = 0;
    uint8_t ch = *p++;

    for (;;) {
        if (snes9x_is_lead_byte(ch)) {
            ch = *p++;
            if (ch < 0x20u) {
                if (pairs != 11)
                    return -1;
                if (ch != 0)
                    return 1;
            }
            ++pairs;
            --remaining;
            --remaining;
        } else {
            if (ch == 0) {
                if (pairs == 0)
                    return -1;
                --remaining;
            } else if (ch < 0x20u) {
                return -1;
            } else if (ch < 0x80u) {
                ++pairs;
                --remaining;
            } else if ((uint8_t)(ch - 0xa0u) < 0x50u) {
                --remaining;
            } else {
                return -1;
            }
        }

        if (remaining <= 0)
            return pairs > 0 ? 0 : -1;
        ch = *p++;
    }
}

/* 0x00158974 -- structured header validation around the helper above. */
int snes9x_header_validate_00158974(const uint8_t *o)
{
    uint32_t pair;
    uint8_t b18;

    if ((o[0x19] & 0x4fu) != 0)
        return -1;
    if (o[0x1a] != 0x33u && o[0x1a] != 0xffu)
        return -1;

    pair = ((uint32_t)o[0x17] << 8) | o[0x16];
    if (pair != 0 && pair != 0xffffu) {
        if ((pair & 0x040fu) != 0 || (pair & 0xffu) > 0xc0u)
            return -1;
    }

    b18 = o[0x18];
    if ((b18 & 0xceu) != 0 || (b18 & 0x30u) == 0)
        return -1;
    if ((o[0x15] & 3u) != 0)
        return -1;
    if (o[0x13] != 0 && o[0x13] != 0xffu)
        return -1;
    if (o[0x14] != 0)
        return -1;

    /* 0x158a48 branches to success only when the subsidiary returns zero. */
    return snes9x_multibyte_validate_00158a58(o) == 0 ? 0 : -1;
}