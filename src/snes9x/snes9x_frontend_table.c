/*
 * SNES Station v0.23 - SNES9X Frontend Table Initialization
 * Canonical source from progress13_frontend_more.c (0x00153674, 0x00153780 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

static void snes9x_store_u32(uint8_t *p, uint32_t value)
{
    memcpy(p, &value, sizeof(value));
}

typedef void (*Snes9xInitCallback)(uint8_t *object, void *opaque);

/* 0x00153674 -- bulk frontend table initialization followed by one callback. */
void snes9x_frontend_table_init_00153674(uint8_t *object,
                                          int extended_tables,
                                          uint32_t external_base,
                                          Snes9xInitCallback finish,
                                          void *opaque)
{
    unsigned outer, inner;

    if (extended_tables) {
        for (outer = 0; outer < 15; ++outer) {
            for (inner = 0; inner < 8; ++inner) {
                unsigned q = (outer * 16u + inner) * 4u;
                snes9x_store_u32(object + q + 0x3c28, 3);
                snes9x_store_u32(object + q + 0x1c28, 3);
                object[outer * 16u + inner + 0xa728] = 0;
                object[outer * 16u + inner + 0x9728] = 1;
                object[outer * 16u + inner + 0xaf28] = 0;
                object[outer * 16u + inner + 0x9f28] = 1;
            }
        }
    }

    {
        uint32_t first = 0;
        memcpy(&first, object, sizeof(first));
        for (inner = 0; inner < 16; ++inner) {
            snes9x_store_u32(object + inner * 4u + 0x1fe8,
                              first + 0x10000u);
            snes9x_store_u32(object + inner * 4u + 0x1fa8, first);
            object[inner + 0xa808] = 0;
            object[inner + 0x9808] = 1;
            object[inner + 0x9818] = 1;
            object[inner + 0xa818] = 0;
        }
    }

    (void)external_base; /* target uses its global external base in other lanes */
    if (finish != NULL)
        finish(object, opaque);
}

/* 0x00153780 -- initialize the two 16-entry pointer/flag banks. */
void snes9x_frontend_bank_init_00153780(uint8_t *object,
                                         uint32_t external_base)
{
    unsigned i;
    uint32_t first;
    memcpy(&first, object, sizeof(first));

    for (i = 0; i < 16; ++i) {
        snes9x_store_u32(object + i * 4u + 0x1fe8, first + 0x10000u);
        snes9x_store_u32(object + i * 4u + 0x1fa8, first);
        object[i + 0xa808] = 0;
        object[i + 0x9808] = 1;
        object[i + 0x9818] = 1;
        object[i + 0xa818] = 0;
    }

    for (i = 0; i < 16; ++i) {
        snes9x_store_u32(object + i * 4u + 0x1c28, external_base);
        snes9x_store_u32(object + i * 4u + 0x1c68, external_base + 0x8000u);
        snes9x_store_u32(object + i * 4u + 0x1ca8, external_base + 0x10000u);
        snes9x_store_u32(object + i * 4u + 0x1ce8, external_base + 0x18000u);
        object[i + 0x9728] = 1;
        object[i + 0xa728] = 0;
        object[i + 0x9738] = 1;
        object[i + 0xa738] = 0;
        object[i + 0x9748] = 1;
        object[i + 0xa748] = 0;
        object[i + 0x9758] = 1;
        object[i + 0xa758] = 0;
    }
}