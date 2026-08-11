/*
 * SNES Station v0.23 -- Snes9x mapped-memory access core.
 * Target: 0x001ab4e8..0x001ac603 plus GetBasePointer @ 0x001ac734.
 *
 * Independent behavioural reconstruction driven by the stripped EE binary.
 * Historical Snes9x getset code is used only for names/semantic vocabulary.
 * The target is authoritative for its 4 KiB map, MAP_LAST=18, cycle accounting,
 * SRAM formulas, open-bus cases, SA-1 wake bookkeeping and dispatch quirks.
 */
#include <stddef.h>
#include <stdint.h>

#define SNES_MEMMAP_SHIFT 12u
#define SNES_MEMMAP_MASK  0x0fffu
#define SNES_MAP_LAST     18u

enum SnesMapCode {
    SNES_MAP_PPU = 0,
    SNES_MAP_CPU = 1,
    SNES_MAP_DSP = 2,
    SNES_MAP_LOROM_SRAM = 3,
    SNES_MAP_HIROM_SRAM = 4,
    SNES_MAP_NONE = 5,
    SNES_MAP_DEBUG = 6,
    SNES_MAP_C4 = 7,
    SNES_MAP_BWRAM = 8,
    SNES_MAP_BWRAM_BITMAP = 9,
    SNES_MAP_BWRAM_BITMAP2 = 10,
    SNES_MAP_SA1RAM = 11,
    SNES_MAP_SPC7110_ROM = 12,
    SNES_MAP_SPC7110_DRAM = 13,
    SNES_MAP_RONLY_SRAM = 14,
    SNES_MAP_OBC_RAM = 15,
    SNES_MAP_SETA_DSP = 16,
    SNES_MAP_SETA_RISC = 17
};

typedef uint8_t (*SnesSpecialRead)(uint32_t address, void *opaque);
typedef void (*SnesSpecialWrite)(uint8_t value, uint32_t address, void *opaque);
typedef uint8_t *(*SnesSpecialPointer)(uint32_t address, void *opaque);

typedef struct {
    uintptr_t map[4096];
    uintptr_t write_map[4096];
    uint8_t memory_speed[4096];
    uint8_t block_is_ram[4096];
    uint8_t *ram;
    uint8_t *sram;
    uint8_t *bwram;
    uint8_t *fillram;
    uint8_t *c4ram;
    uint8_t *spc7110_dram;
    uint32_t sram_mask;
    uint8_t open_bus;
    uint8_t sram_modified;
    uint8_t spc7110_enabled;
} SnesMemoryMapModel;

typedef struct {
    uint32_t flags;
    uintptr_t pc;
    uintptr_t pc_base;
    uintptr_t pc_at_opcode_start;
    uintptr_t wait_address;
    uint32_t wait_counter;
    uint64_t cycles;
    uint64_t next_event;
    uint64_t mem_speed;
    uint64_t mem_speed_x2;
    uint8_t in_dma;
} SnesCpuMapModel;

typedef struct {
    SnesMemoryMapModel *memory;
    SnesCpuMapModel *cpu;
    SnesSpecialRead read_special[SNES_MAP_LAST];
    SnesSpecialWrite write_special[SNES_MAP_LAST];
    SnesSpecialPointer pointer_special[SNES_MAP_LAST];
    SnesSpecialPointer base_pointer_special[SNES_MAP_LAST];
    void *opaque;

    /* Target SA-1 wake state touched by direct writes and MAP_SA1RAM. */
    uintptr_t sa1_wake_address1;
    uintptr_t sa1_wake_address2;
    uintptr_t sa1_pc;
    uint32_t sa1_wait_counter;
    uint8_t sa1_executing;
    uint8_t sa1_waiting;
} SnesGetSetContext;

static unsigned block_for(uint32_t address)
{
    return (address >> SNES_MEMMAP_SHIFT) & SNES_MEMMAP_MASK;
}

static int is_direct(uintptr_t entry)
{
    return entry >= SNES_MAP_LAST;
}

static uint8_t *direct_ptr(uintptr_t entry, uint32_t address)
{
    return (uint8_t *)entry + (address & 0xffffu);
}

static uint32_t lorom_sram_offset(const SnesMemoryMapModel *m, uint32_t a)
{
    return ((((a & 0xff0000u) >> 1) | (a & 0x7fffu)) & m->sram_mask);
}

static uint32_t hirom_sram_offset(const SnesMemoryMapModel *m, uint32_t a)
{
    return (((a & 0x7fffu) - 0x6000u + ((a & 0x0f0000u) >> 3)) & m->sram_mask);
}

static uint8_t call_read(SnesGetSetContext *ctx, unsigned code, uint32_t address)
{
    return ctx->read_special[code] != NULL
        ? ctx->read_special[code](address, ctx->opaque)
        : ctx->memory->open_bus;
}

static void call_write(SnesGetSetContext *ctx, unsigned code,
                       uint8_t value, uint32_t address)
{
    if (ctx->write_special[code] != NULL)
        ctx->write_special[code](value, address, ctx->opaque);
}

static uint8_t special_read_value(SnesGetSetContext *ctx,
                                  unsigned code, uint32_t address)
{
    SnesMemoryMapModel *m = ctx->memory;
    switch (code) {
    case SNES_MAP_PPU:
    case SNES_MAP_CPU:
    case SNES_MAP_DSP:
    case SNES_MAP_C4:
    case SNES_MAP_OBC_RAM:
        return call_read(ctx, code, address & 0xffffu);
    case SNES_MAP_LOROM_SRAM:
    case SNES_MAP_SA1RAM:
        return m->sram[lorom_sram_offset(m, address)];
    case SNES_MAP_HIROM_SRAM:
    case SNES_MAP_RONLY_SRAM:
        return m->sram[hirom_sram_offset(m, address)];
    case SNES_MAP_BWRAM:
        return m->bwram[(address & 0x7fffu) - 0x6000u];
    case SNES_MAP_SPC7110_ROM:
        return call_read(ctx, code, address);
    case SNES_MAP_SPC7110_DRAM:
        return call_read(ctx, code, 0x4800u);
    case SNES_MAP_SETA_DSP:
    case SNES_MAP_SETA_RISC:
        return call_read(ctx, code, address);
    case SNES_MAP_NONE:
    case SNES_MAP_DEBUG:
    case SNES_MAP_BWRAM_BITMAP:
    case SNES_MAP_BWRAM_BITMAP2:
    default:
        return m->open_bus;
    }
}

static void sa1_direct_write_wake(SnesGetSetContext *ctx, uint8_t *p)
{
    uintptr_t address = (uintptr_t)p;
    if (address == ctx->sa1_wake_address1 || address == ctx->sa1_wake_address2) {
        ctx->sa1_wait_counter = 0;
        ctx->sa1_executing = ctx->sa1_pc != 0;
    }
}

static void sa1_ram_write_state(SnesGetSetContext *ctx)
{
    ctx->sa1_executing = ctx->sa1_waiting == 0;
}

/* 0x001ab4e8 */
uint8_t *S9xGetMemPointer_001ab4e8(SnesGetSetContext *ctx, uint32_t address)
{
    uintptr_t entry = ctx->memory->map[block_for(address)];
    unsigned code = (unsigned)entry;
    uint32_t lo = address & 0xffffu;

    if (is_direct(entry))
        return direct_ptr(entry, address);

    /* Target tests this before its <17 jump-table dispatch. */
    if (ctx->memory->spc7110_enabled &&
        (address & 0x007fffffu) == 0x4800u)
        return ctx->memory->spc7110_dram;

    if (code >= 17u)
        return NULL;

    switch (code) {
    case SNES_MAP_PPU: return ctx->memory->fillram - 0x2000 + lo;
    case SNES_MAP_CPU: return ctx->memory->fillram - 0x4000 + lo;
    case SNES_MAP_DSP: return ctx->memory->fillram - 0x6000 + lo;
    case SNES_MAP_LOROM_SRAM:
    case SNES_MAP_SA1RAM: return ctx->memory->sram + lo;
    case SNES_MAP_HIROM_SRAM: return ctx->memory->sram + lo;
    case SNES_MAP_C4: return ctx->memory->c4ram != NULL ? ctx->memory->c4ram + lo : NULL;
    case SNES_MAP_BWRAM: return ctx->memory->bwram + lo;
    case SNES_MAP_SPC7110_DRAM:
        return ctx->memory->spc7110_dram != NULL
            ? ctx->memory->spc7110_dram + lo : NULL;
    case SNES_MAP_OBC_RAM:
        return ctx->pointer_special[code] != NULL
            ? ctx->pointer_special[code](address, ctx->opaque) : NULL;
    case SNES_MAP_SETA_DSP:
        /* Target returns the SRAM base itself here, with no low-address add. */
        return ctx->memory->sram;
    default:
        return NULL;
    }
}

/* 0x001ab63c */
uint8_t S9xGetByte_001ab63c(SnesGetSetContext *ctx, uint32_t address)
{
    unsigned block = block_for(address);
    uintptr_t entry = ctx->memory->map[block];
    unsigned code;

    if (is_direct(entry)) {
        ctx->cpu->cycles += ctx->memory->memory_speed[block];
        if (ctx->memory->block_is_ram[block])
            ctx->cpu->wait_address = ctx->cpu->pc_at_opcode_start;
        return *direct_ptr(entry, address);
    }

    code = (unsigned)entry;
    switch (code) {
    case SNES_MAP_PPU:
        if (!ctx->cpu->in_dma) ctx->cpu->cycles += 6;
        break;
    case SNES_MAP_CPU:
        ctx->cpu->cycles += 6;
        break;
    case SNES_MAP_DEBUG:
    case SNES_MAP_C4:
        break;
    default:
        ctx->cpu->cycles += 8;
        break;
    }
    return special_read_value(ctx, code, address);
}

/* 0x001ab900 */
void S9xSetByte_001ab900(SnesGetSetContext *ctx, uint8_t value, uint32_t address)
{
    SnesMemoryMapModel *m = ctx->memory;
    unsigned block = block_for(address);
    uintptr_t entry = m->write_map[block];
    unsigned code;

    ctx->cpu->wait_address = 0;
    if (is_direct(entry)) {
        uint8_t *p = direct_ptr(entry, address);
        ctx->cpu->cycles += m->memory_speed[block];
        sa1_direct_write_wake(ctx, p);
        *p = value;
        return;
    }

    code = (unsigned)entry;
    switch (code) {
    case SNES_MAP_PPU:
        if (!ctx->cpu->in_dma) ctx->cpu->cycles += 6;
        call_write(ctx, code, value, address & 0xffffu);
        return;
    case SNES_MAP_CPU:
        ctx->cpu->cycles += 6;
        call_write(ctx, code, value, address & 0xffffu);
        return;
    case SNES_MAP_DSP:
        ctx->cpu->cycles += 8;
        call_write(ctx, code, value, address & 0xffffu);
        return;
    case SNES_MAP_LOROM_SRAM:
        ctx->cpu->cycles += 8;
        if (m->sram_mask != 0) {
            m->sram[lorom_sram_offset(m, address)] = value;
            m->sram_modified = 1;
        }
        return;
    case SNES_MAP_HIROM_SRAM:
        ctx->cpu->cycles += 8;
        if (m->sram_mask != 0) {
            m->sram[hirom_sram_offset(m, address)] = value;
            m->sram_modified = 1;
        }
        return;
    case SNES_MAP_C4:
        /* Byte C4 writes are the one write-special route with no cycle add. */
        call_write(ctx, code, value, address & 0xffffu);
        return;
    case SNES_MAP_BWRAM:
        ctx->cpu->cycles += 8;
        m->bwram[(address & 0x7fffu) - 0x6000u] = value;
        m->sram_modified = 1;
        return;
    case SNES_MAP_DEBUG:
    case SNES_MAP_SA1RAM:
        ctx->cpu->cycles += 8;
        m->sram[address & 0xffffu] = value;
        sa1_ram_write_state(ctx);
        return;
    case SNES_MAP_SPC7110_DRAM:
        ctx->cpu->cycles += 8;
        if (m->spc7110_dram != NULL)
            m->spc7110_dram[address & 0xffffu] = value;
        return;
    case SNES_MAP_OBC_RAM:
        ctx->cpu->cycles += 8;
        call_write(ctx, code, value, address & 0xffffu);
        return;
    case SNES_MAP_SETA_DSP:
    case SNES_MAP_SETA_RISC:
        ctx->cpu->cycles += 8;
        call_write(ctx, code, value, address);
        return;
    case SNES_MAP_NONE:
    case SNES_MAP_BWRAM_BITMAP:
    case SNES_MAP_BWRAM_BITMAP2:
    case SNES_MAP_SPC7110_ROM:
    case SNES_MAP_RONLY_SRAM:
    default:
        ctx->cpu->cycles += 8;
        return;
    }
}

/* 0x001abc28 -- target boundary slow path is (Address & 0xfff) == 0xfff. */
uint16_t S9xGetWord_001abc28(SnesGetSetContext *ctx, uint32_t address)
{
    unsigned block;
    uintptr_t entry;
    unsigned code;
    uint8_t lo, hi;

    if ((address & 0xfffu) == 0xfffu) {
        lo = S9xGetByte_001ab63c(ctx, address);
        /* Target writes the first byte to OpenBus before the second read. */
        ctx->memory->open_bus = lo;
        hi = S9xGetByte_001ab63c(ctx, address + 1u);
        return (uint16_t)(lo | ((uint16_t)hi << 8));
    }

    block = block_for(address);
    entry = ctx->memory->map[block];
    if (is_direct(entry)) {
        uint8_t *p = direct_ptr(entry, address);
        ctx->cpu->cycles += (uint64_t)ctx->memory->memory_speed[block] * 2u;
        if (ctx->memory->block_is_ram[block])
            ctx->cpu->wait_address = ctx->cpu->pc_at_opcode_start;
        return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
    }

    code = (unsigned)entry;
    switch (code) {
    case SNES_MAP_PPU:
        if (!ctx->cpu->in_dma) ctx->cpu->cycles += 12;
        break;
    case SNES_MAP_CPU:
        ctx->cpu->cycles += 12;
        break;
    case SNES_MAP_DEBUG:
    case SNES_MAP_C4:
        break;
    default:
        ctx->cpu->cycles += 16;
        break;
    }

    if (code == SNES_MAP_DEBUG || code == SNES_MAP_NONE ||
        code == SNES_MAP_BWRAM_BITMAP || code == SNES_MAP_BWRAM_BITMAP2) {
        lo = ctx->memory->open_bus;
        hi = lo;
    } else {
        lo = special_read_value(ctx, code, address);
        hi = special_read_value(ctx, code, address + 1u);
    }
    return (uint16_t)(lo | ((uint16_t)hi << 8));
}

/* 0x001ac024 */
void S9xSetPCBase_001ac024(SnesGetSetContext *ctx, uint32_t address)
{
    unsigned block = block_for(address);
    uintptr_t entry = ctx->memory->map[block];
    uint8_t *base;
    uint64_t speed;

    if (is_direct(entry)) {
        base = (uint8_t *)entry;
        speed = ctx->memory->memory_speed[block];
    } else {
        unsigned code = (unsigned)entry;
        switch (code) {
        case SNES_MAP_PPU:
            base = ctx->memory->fillram - 0x2000;
            speed = 6;
            break;
        case SNES_MAP_CPU:
            base = ctx->memory->fillram - 0x4000;
            speed = 6;
            break;
        case SNES_MAP_DSP:
            base = ctx->memory->fillram - 0x6000;
            speed = 8;
            break;
        case SNES_MAP_HIROM_SRAM:
            base = ctx->memory->sram - 0x6000;
            speed = 8;
            break;
        case SNES_MAP_C4:
            base = ctx->memory->c4ram - 0x6000;
            speed = 8;
            break;
        case SNES_MAP_BWRAM:
            base = ctx->memory->bwram - 0x6000;
            speed = 8;
            break;
        default:
            /* Target's default path is SRAM even for NONE/debug/chip slots. */
            base = ctx->memory->sram;
            speed = 8;
            break;
        }
    }

    ctx->cpu->pc_base = (uintptr_t)base;
    ctx->cpu->pc = (uintptr_t)(base + (address & 0xffffu));
    ctx->cpu->mem_speed = speed;
    ctx->cpu->mem_speed_x2 = speed * 2u;
}

/* 0x001ac190 */
void S9xSetWord_001ac190(SnesGetSetContext *ctx, uint16_t value, uint32_t address)
{
    SnesMemoryMapModel *m = ctx->memory;
    unsigned block;
    uintptr_t entry;
    unsigned code;
    uint8_t lo = (uint8_t)value;
    uint8_t hi = (uint8_t)(value >> 8);

    if ((address & 0xfffu) == 0xfffu) {
        S9xSetByte_001ab900(ctx, lo, address);
        S9xSetByte_001ab900(ctx, hi, address + 1u);
        return;
    }

    ctx->cpu->wait_address = 0;
    block = block_for(address);
    entry = m->write_map[block];
    if (is_direct(entry)) {
        uint8_t *p = direct_ptr(entry, address);
        ctx->cpu->cycles += (uint64_t)m->memory_speed[block] * 2u;
        sa1_direct_write_wake(ctx, p);
        p[0] = lo;
        p[1] = hi;
        return;
    }

    code = (unsigned)entry;
    switch (code) {
    case SNES_MAP_PPU:
        if (!ctx->cpu->in_dma) ctx->cpu->cycles += 12;
        call_write(ctx, code, lo, address & 0xffffu);
        call_write(ctx, code, hi, (address + 1u) & 0xffffu);
        return;
    case SNES_MAP_CPU:
        ctx->cpu->cycles += 12;
        call_write(ctx, code, lo, address & 0xffffu);
        call_write(ctx, code, hi, (address + 1u) & 0xffffu);
        return;
    case SNES_MAP_DSP:
        ctx->cpu->cycles += 16;
        call_write(ctx, code, lo, address & 0xffffu);
        call_write(ctx, code, hi, (address + 1u) & 0xffffu);
        return;
    case SNES_MAP_LOROM_SRAM:
        ctx->cpu->cycles += 16;
        if (m->sram_mask != 0) {
            m->sram[lorom_sram_offset(m, address)] = lo;
            m->sram[lorom_sram_offset(m, address + 1u)] = hi;
            m->sram_modified = 1;
        }
        return;
    case SNES_MAP_HIROM_SRAM:
        ctx->cpu->cycles += 16;
        if (m->sram_mask != 0) {
            m->sram[hirom_sram_offset(m, address)] = lo;
            m->sram[hirom_sram_offset(m, address + 1u)] = hi;
            m->sram_modified = 1;
        }
        return;
    case SNES_MAP_C4:
        /* Unlike byte C4 writes, the target word path charges 16 cycles. */
        ctx->cpu->cycles += 16;
        call_write(ctx, code, lo, address & 0xffffu);
        call_write(ctx, code, hi, (address + 1u) & 0xffffu);
        return;
    case SNES_MAP_BWRAM:
        ctx->cpu->cycles += 16;
        m->bwram[(address & 0x7fffu) - 0x6000u] = lo;
        m->bwram[((address + 1u) & 0x7fffu) - 0x6000u] = hi;
        m->sram_modified = 1;
        return;
    case SNES_MAP_SA1RAM:
        /* Target's slot 11 path is only +8 even for a 16-bit write. */
        ctx->cpu->cycles += 8;
        m->sram[address & 0xffffu] = lo;
        m->sram[(address + 1u) & 0xffffu] = hi;
        sa1_ram_write_state(ctx);
        return;
    case SNES_MAP_DEBUG:
        /*
         * Target quirk: SetWord jump-table slot 6 shares the fixed SPC7110
         * storage path with slot 13. S9xGetByte treats slot 6 as open-bus and
         * S9xSetByte shares the SA1RAM path. Preserve the ELF, do not normalize.
         */
        ctx->cpu->cycles += 16;
        if (m->spc7110_dram != NULL) {
            m->spc7110_dram[address & 0xffffu] = lo;
            m->spc7110_dram[(address + 1u) & 0xffffu] = hi;
        }
        return;
    case SNES_MAP_SPC7110_DRAM:
        ctx->cpu->cycles += 16;
        if (m->spc7110_dram != NULL) {
            m->spc7110_dram[address & 0xffffu] = lo;
            m->spc7110_dram[(address + 1u) & 0xffffu] = hi;
        }
        return;
    case SNES_MAP_OBC_RAM:
        ctx->cpu->cycles += 16;
        call_write(ctx, code, lo, address & 0xffffu);
        call_write(ctx, code, hi, (address + 1u) & 0xffffu);
        return;
    case SNES_MAP_SETA_DSP:
    case SNES_MAP_SETA_RISC:
        ctx->cpu->cycles += 16;
        call_write(ctx, code, lo, address);
        call_write(ctx, code, hi, address + 1u);
        return;
    case SNES_MAP_NONE:
    case SNES_MAP_BWRAM_BITMAP:
    case SNES_MAP_BWRAM_BITMAP2:
    case SNES_MAP_SPC7110_ROM:
    case SNES_MAP_RONLY_SRAM:
    default:
        ctx->cpu->cycles += 16;
        return;
    }
}

/* 0x001ac734 */
uint8_t *GetBasePointer_001ac734(SnesGetSetContext *ctx, uint32_t address)
{
    uintptr_t entry = ctx->memory->map[block_for(address)];
    unsigned code = (unsigned)entry;

    if (is_direct(entry))
        return (uint8_t *)entry;

    if (ctx->memory->spc7110_enabled &&
        (address & 0x007fffffu) == 0x4800u)
        return ctx->memory->spc7110_dram;

    if (code >= 17u)
        return NULL;

    switch (code) {
    case SNES_MAP_PPU:
    case SNES_MAP_CPU: return ctx->memory->fillram;
    case SNES_MAP_DSP: return ctx->memory->fillram - 0x6000;
    case SNES_MAP_LOROM_SRAM:
    case SNES_MAP_SA1RAM:
    case SNES_MAP_SETA_DSP: return ctx->memory->sram;
    case SNES_MAP_HIROM_SRAM: return ctx->memory->sram - 0x6000;
    case SNES_MAP_C4: return ctx->memory->c4ram - 0x6000;
    case SNES_MAP_BWRAM: return ctx->memory->bwram - 0x6000;
    case SNES_MAP_SPC7110_ROM:
    case SNES_MAP_OBC_RAM:
        return ctx->base_pointer_special[code] != NULL
            ? ctx->base_pointer_special[code](address, ctx->opaque) : NULL;
    default:
        return NULL;
    }
}
