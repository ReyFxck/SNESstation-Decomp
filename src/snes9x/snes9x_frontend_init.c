/*
 * SNES Station v0.23 SNES9X - Frontend Table Initialization
 * Canonical source from progress13_frontend_more_recovered.c @ 0x00153674
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef void (*Snes9xInitCallback)(uint8_t *object, void *opaque);

/* 0x00153674 -- bulk frontend table initialization followed by one callback. */
void snes9x_frontend_table_init(uint8_t *object,
                                 int extended_tables,
                                 uint32_t external_base,
                                 Snes9xInitCallback finish,
                                 void *opaque)
{
    /* Implementation would follow the target's table initialization logic */
    if (finish != NULL)
        finish(object, opaque);
}