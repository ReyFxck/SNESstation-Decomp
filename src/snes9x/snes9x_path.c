/*
 * SNES Station v0.23 SNES9X - Path Construction
 * Canonical source from progress13_frontend_more_recovered.c @ 0x00101924
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static void snes9x_store_u32(uint8_t *p, uint32_t value)
{
    memcpy(p, &value, sizeof(value));
}

typedef void (*Snes9xPathSeedFn)(void *config, char *a, char *b,
                                  char *c, char *d, void *opaque);
typedef void (*Snes9xPathFormatFn)(char *out, const char *a, const char *b,
                                    const char *c, const char *suffix,
                                    void *opaque);

/* 0x00101924 -- two-stage path construction wrapper. */
char *snes9x_path_wrapper(char *out, char *scratch0, char scratch1[0x410],
                          char scratch2[0x410], char scratch3[0x410],
                          const char *suffix, void *config,
                          Snes9xPathSeedFn seed, Snes9xPathFormatFn format,
                          void *opaque)
{
    /* Target frame: sp, sp+0x10, sp+0x420 and sp+0x830 feed 0x105ae8;
     * 0x1059cc then formats into the independent sp+0xc40 result buffer. */
    if (seed != NULL)
        seed(config, scratch0, scratch1, scratch2, scratch3, opaque);
    if (format != NULL)
        format(out, scratch0, scratch1, scratch2, suffix, opaque);
    return out;
}