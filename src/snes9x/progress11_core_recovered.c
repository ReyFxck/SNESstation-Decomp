/*
 * Progress 11: compact Snes9x-side helpers proven from the v0.23 target.
 * Address-labelled names are retained where the historical source symbol is
 * not independently established.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t (*P11ReadByte)(uint32_t address, void *opaque);
typedef void (*P11WriteByte)(uint8_t value, uint32_t address, void *opaque);

void snes_p11_0015e034(uint16_t value, uint32_t address,
                       P11WriteByte write_byte, void *opaque)
{
    if (write_byte == NULL)
        return;
    write_byte((uint8_t)value, address, opaque);
    write_byte((uint8_t)(value >> 8), address + 1u, opaque);
}

uint16_t snes_p11_0015de14(uint32_t address, uint8_t *open_bus_shadow,
                           P11ReadByte read_byte, void *opaque)
{
    uint8_t lo = 0u;
    uint8_t hi = 0u;

    if (read_byte != NULL) {
        lo = read_byte(address, opaque);
        if (open_bus_shadow != NULL)
            *open_bus_shadow = lo;
        hi = read_byte(address + 1u, opaque);
    }
    return (uint16_t)((uint16_t)lo | ((uint16_t)hi << 8));
}

void snes_p11_0016f9b0(uint32_t *map, uint32_t data_base_address,
                       uint32_t group, uint32_t page)
{
    uint32_t block;

    for (block = 0u; block < 256u; block += 16u) {
        uint32_t pointer = data_base_address + (page << 20) + (block << 12);
        uint32_t i;
        for (i = 0u; i < 16u; ++i)
            map[0xc0au + (group << 8) + block + i] = pointer;
    }
}

void snes_p11_0016fa18(uint8_t *chip_memory, uint32_t *map,
                       uint32_t data_base_address)
{
    uint32_t i;

    memset(chip_memory + 0x4800u, 0, 4u);
    for (i = 0u; i < 4u; ++i) {
        chip_memory[0x4804u + i] = (uint8_t)i;
        snes_p11_0016f9b0(map, data_base_address, i, i);
    }
}

static uint32_t p11_select_336_table(uint32_t status)
{
    if ((status & UINT32_C(0x100)) != 0u)
        return UINT32_C(0x003367f8);
    if ((status & UINT32_C(0x20)) != 0u)
        return (status & UINT32_C(0x10)) != 0u
                   ? UINT32_C(0x003367f8)
                   : UINT32_C(0x00336bf8);
    return (status & UINT32_C(0x10)) != 0u
               ? UINT32_C(0x003373f8)
               : UINT32_C(0x00336ff8);
}

static uint32_t p11_select_3f5_table(uint32_t status)
{
    if ((status & UINT32_C(0x100)) != 0u)
        return UINT32_C(0x003f5040);
    if ((status & UINT32_C(0x20)) != 0u)
        return (status & UINT32_C(0x10)) != 0u
                   ? UINT32_C(0x003f5040)
                   : UINT32_C(0x003f5440);
    return (status & UINT32_C(0x10)) != 0u
               ? UINT32_C(0x003f5c40)
               : UINT32_C(0x003f5840);
}

uint32_t snes_p11_001291e4(uint32_t status, uint32_t *selected)
{
    uint32_t value = p11_select_336_table(status);
    *selected = value;
    return value;
}

uint32_t snes_p11_00171160(uint32_t status, uint32_t *selected)
{
    uint32_t value = p11_select_336_table(status);
    *selected = value;
    return value;
}

uint32_t snes_p11_00173c24(uint32_t status, uint32_t *selected)
{
    uint32_t value = p11_select_336_table(status);
    *selected = value;
    return value;
}

uint32_t snes_p11_0015f15c(uint32_t status, uint32_t *selected)
{
    uint32_t value = p11_select_3f5_table(status);
    *selected = value;
    return value;
}

uint32_t snes_p11_0016f0a0(uint32_t status, uint32_t *selected)
{
    uint32_t value = p11_select_3f5_table(status);
    *selected = value;
    return value;
}

typedef struct P11CheatRecord {
    uint32_t address;
    uint8_t replacement;
    uint8_t saved;
    uint8_t active;
    uint8_t saved_valid;
    uint8_t padding[0x18];
} P11CheatRecord;

_Static_assert(sizeof(P11CheatRecord) == 0x20u, "target cheat record is 0x20 bytes");

typedef struct P11CheatContext {
    P11CheatRecord records[75];
    uint32_t count;
    uint8_t enabled;
    P11ReadByte read_byte;
    P11WriteByte set_byte;
    uint32_t (*map_entry)(uint32_t address, void *opaque);
    void (*direct_write)(uint32_t map_entry, uint32_t address,
                         uint8_t value, void *opaque);
    void *opaque;
} P11CheatContext;

static void p11_cheat_write(P11CheatContext *ctx, uint32_t address, uint8_t value)
{
    uint32_t entry = 0u;

    if (ctx->map_entry != NULL)
        entry = ctx->map_entry(address, ctx->opaque);
    if (entry >= 0x12u && ctx->direct_write != NULL)
        ctx->direct_write(entry, address, value, ctx->opaque);
    else if (ctx->set_byte != NULL)
        ctx->set_byte(value, address, ctx->opaque);
}

void snes_p11_00114118(P11CheatContext *ctx, uint32_t save_current,
                       uint32_t address, uint32_t replacement)
{
    P11CheatRecord *record;

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

void snes_p11_00114328(P11CheatContext *ctx, uint32_t index)
{
    P11CheatRecord *record = &ctx->records[index];

    if (record->saved_valid == 0u)
        return;
    p11_cheat_write(ctx, record->address, record->saved);
}

void snes_p11_0011439c(P11CheatContext *ctx, uint32_t index)
{
    P11CheatRecord *record = &ctx->records[index];

    if (record->saved_valid == 0u && ctx->read_byte != NULL)
        record->saved = ctx->read_byte(record->address, ctx->opaque);
    p11_cheat_write(ctx, record->address, record->replacement);
    record->saved_valid = 1u;
}

void snes_p11_0011444c(P11CheatContext *ctx)
{
    uint32_t i;

    if (ctx->enabled == 0u)
        return;
    for (i = 0u; i < ctx->count; ++i) {
        if (ctx->records[i].active != 0u)
            snes_p11_0011439c(ctx, i);
    }
}

void snes_p11_001144d0(P11CheatContext *ctx)
{
    uint32_t i;

    for (i = 0u; i < ctx->count; ++i) {
        if (ctx->records[i].active != 0u)
            snes_p11_00114328(ctx, i);
    }
}

static uint32_t p11_special_bank(uint32_t address, uint8_t bank_d,
                                 uint8_t bank_e, uint8_t bank_f)
{
    uint32_t type = (address >> 16) & 0xf0u;

    if (type == 0xd0u)
        return (uint32_t)bank_d << 20;
    if (type == 0xe0u)
        return (uint32_t)bank_e << 20;
    if (type == 0xf0u)
        return (uint32_t)bank_f << 20;
    return 0u;
}

uint32_t snes_p11_00182910(uint32_t address, uint32_t memory_base_address,
                           uint8_t bank_d, uint8_t bank_e, uint8_t bank_f)
{
    return memory_base_address + p11_special_bank(address, bank_d, bank_e, bank_f)
           + (address & UINT32_C(0x000f0000));
}

uint8_t snes_p11_0018255c(const uint8_t *memory, uint32_t address,
                          uint32_t base_offset, uint8_t bank_d,
                          uint8_t bank_e, uint8_t bank_f)
{
    uint32_t offset = p11_special_bank(address, bank_d, bank_e, bank_f)
                      + base_offset + (address & UINT32_C(0x000fffff));
    return memory[offset];
}

typedef struct P11CodeStreamState {
    uint8_t bytes[13];
    int8_t index;
    uint8_t blocked;
} P11CodeStreamState;

uint8_t snes_p11_00183bdc(P11CodeStreamState *state,
                          void (*refill)(P11CodeStreamState *, void *),
                          void *opaque)
{
    int index;

    if (state->blocked != 0u)
        return 0u;
    index = state->index;
    if (index >= 0) {
        if (index < 13) {
            uint8_t result = state->bytes[index];
            state->index = (int8_t)(index + 1);
            return result;
        }
        state->index = -1;
        return 15u;
    }
    if (refill != NULL)
        refill(state, opaque);
    state->index = (int8_t)((uint8_t)state->index + 1u);
    return 15u;
}

void snes_p11_0014308c(uint16_t table[8u * 256u], uint8_t *ready_flag)
{
    uint32_t group;

    for (group = 0u; group < 8u; ++group) {
        uint32_t value;
        for (value = 0u; value < 256u; ++value) {
            uint32_t part_a = ((value & 0xc0u) >> 3) | (group & 4u);
            uint32_t part_b = ((value & 0x38u) >> 1) | (group & 2u);
            uint32_t part_c = ((value & 7u) << 2) | ((group & 1u) << 1);
            table[(group << 8) + value] =
                (uint16_t)((part_a << 10) | (part_b << 5) | part_c);
        }
    }
    if (ready_flag != NULL)
        *ready_flag = 0u;
}

void snes_p11_0012e2e0(uint8_t *base, uint32_t count, uint8_t transparent_nibble)
{
    uint32_t i;
    uint8_t key = transparent_nibble & 0x0fu;

    for (i = 0u; i < count; ++i) {
        uint8_t incoming = base[count + i];
        uint8_t old = base[i];
        uint8_t high = (uint8_t)(old & 0xf0u);
        uint8_t low = (uint8_t)(incoming & 0x0fu);

        if ((incoming >> 4) != key)
            high = (uint8_t)(incoming & 0xf0u);
        if ((incoming & 0x0fu) == key)
            low = (uint8_t)(old & 0x0fu);
        base[0x200u + i] = (uint8_t)(high | low);
    }
}

static uint32_t p11_load_u32(const uint8_t *base, size_t offset)
{
    uint32_t value;
    memcpy(&value, base + offset, sizeof(value));
    return value;
}

static void p11_store_u32(uint8_t *base, size_t offset, uint32_t value)
{
    memcpy(base + offset, &value, sizeof(value));
}

void snes_p11_00130c78(uint8_t *state,
                       void (*reset_helper)(void *opaque), void *opaque,
                       uint32_t target_self_address)
{
    uint32_t pc = p11_load_u32(state, 0x3cu);
    uint32_t block = pc & UINT32_C(0x0000fff0);
    uint32_t old_block = p11_load_u32(state, 0x58u);

    if (old_block != block || state[0x5ecu] == 0u) {
        if (reset_helper != NULL)
            reset_helper(opaque);
        p11_store_u32(state, 0x58u, block);
        state[0x5ecu] = 1u;
    }
    p11_store_u32(state, 0x3cu, pc + 1u);
    p11_store_u32(state, 0x48u,
                  p11_load_u32(state, 0x48u) & UINT32_C(0xffffecff));
    p11_store_u32(state, 0x64u, target_self_address);
    p11_store_u32(state, 0x68u, target_self_address);
}
