/*
 * SNES Station v0.23 - SNES9X Cheat System
 * Canonical source from progress11_core.c (0x00114118..0x001144d0 range)
 */

#include <stddef.h>
#include <stdint.h>

typedef struct Snes9xCheatRecord {
    uint32_t address;
    uint8_t replacement;
    uint8_t saved;
    uint8_t active;
    uint8_t saved_valid;
    uint8_t padding[0x18];
} Snes9xCheatRecord;

_Static_assert(sizeof(Snes9xCheatRecord) == 0x20u, "target cheat record is 0x20 bytes");

typedef struct Snes9xCheatContext {
    Snes9xCheatRecord records[75];
    uint32_t count;
    uint8_t enabled;
    uint8_t (*read_byte)(uint32_t address, void *opaque);
    void (*set_byte)(uint8_t value, uint32_t address, void *opaque);
    uint32_t (*map_entry)(uint32_t address, void *opaque);
    void (*direct_write)(uint32_t map_entry, uint32_t address,
                         uint8_t value, void *opaque);
    void *opaque;
} Snes9xCheatContext;

static void snes9x_cheat_write(Snes9xCheatContext *ctx, uint32_t address, uint8_t value)
{
    uint32_t entry = 0u;

    if (ctx->map_entry != NULL)
        entry = ctx->map_entry(address, ctx->opaque);
    if (entry >= 0x12u && ctx->direct_write != NULL)
        ctx->direct_write(entry, address, value, ctx->opaque);
    else if (ctx->set_byte != NULL)
        ctx->set_byte(value, address, ctx->opaque);
}

/* 0x00114118: add a cheat record */
void snes9x_cheat_add_00114118(Snes9xCheatContext *ctx, uint32_t save_current,
                                uint32_t address, uint32_t replacement)
{
    Snes9xCheatRecord *record;

    if (ctx->count >= 75u)
        return;
    record = &ctx->records[ctx->count];
    record->address = address;
    record->replacement = (uint8_t)replacement;
    record->active = 1u;
    if ((save_current & 0xffu) != 0u) {
        if (ctx->read_byte != NULL)
            record->saved = ctx->read_byte(address, ctx->opaque);
        record->saved_valid = 1u;
    }
    ++ctx->count;
}

/* 0x00114328: restore saved value for a cheat */
void snes9x_cheat_restore_00114328(Snes9xCheatContext *ctx, uint32_t index)
{
    Snes9xCheatRecord *record = &ctx->records[index];

    if (record->saved_valid == 0u)
        return;
    snes9x_cheat_write(ctx, record->address, record->saved);
}

/* 0x0011439c: apply cheat replacement */
void snes9x_cheat_apply_0011439c(Snes9xCheatContext *ctx, uint32_t index)
{
    Snes9xCheatRecord *record = &ctx->records[index];

    if (record->saved_valid == 0u && ctx->read_byte != NULL)
        record->saved = ctx->read_byte(record->address, ctx->opaque);
    snes9x_cheat_write(ctx, record->address, record->replacement);
    record->saved_valid = 1u;
}

/* 0x0011444c: apply all active cheats */
void snes9x_cheat_apply_all_0011444c(Snes9xCheatContext *ctx)
{
    uint32_t i;

    if (ctx->enabled == 0u)
        return;
    for (i = 0u; i < ctx->count; ++i) {
        if (ctx->records[i].active != 0u)
            snes9x_cheat_apply_0011439c(ctx, i);
    }
}

/* 0x001144d0: restore all active cheats */
void snes9x_cheat_restore_all_001144d0(Snes9xCheatContext *ctx)
{
    uint32_t i;

    for (i = 0u; i < ctx->count; ++i) {
        if (ctx->records[i].active != 0u)
            snes9x_cheat_restore_00114328(ctx, i);
    }
}