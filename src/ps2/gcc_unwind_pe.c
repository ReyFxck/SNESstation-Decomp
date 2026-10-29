/*
 * Independently reconstructed small helpers from the GCC 3.2.2-b1
 * unwind-dw2 / unwind-pe corridor in SNES Station v0.23.
 *
 * The complete historical objects are mapped in docs/LIBGCC_MAP.md, but only
 * the compact helpers below are claimed as reconstructed source here.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DW_EH_PE_ABSPTR   0x00U
#define DW_EH_PE_ULEB128  0x01U
#define DW_EH_PE_UDATA2   0x02U
#define DW_EH_PE_UDATA4   0x03U
#define DW_EH_PE_UDATA8   0x04U
#define DW_EH_PE_SLEB128  0x09U
#define DW_EH_PE_SDATA2   0x0aU
#define DW_EH_PE_SDATA4   0x0bU
#define DW_EH_PE_SDATA8   0x0cU
#define DW_EH_PE_PCREL    0x10U
#define DW_EH_PE_TEXTREL  0x20U
#define DW_EH_PE_DATAREL  0x30U
#define DW_EH_PE_FUNCREL  0x40U
#define DW_EH_PE_ALIGNED  0x50U
#define DW_EH_PE_INDIRECT 0x80U
#define DW_EH_PE_OMIT     0xffU

/* The target EE context uses 32-bit addresses even with -mlong64. */
typedef struct SnesUnwindContext {
    uint8_t raw[0x158];
} SnesUnwindContext;

static uint32_t load_u32_unaligned(const uint8_t *p)
{
    uint32_t v;
    memcpy(&v, p, sizeof(v));
    return v;
}

static uint64_t load_u64_unaligned(const uint8_t *p)
{
    uint64_t v;
    memcpy(&v, p, sizeof(v));
    return v;
}

static int16_t load_s16_unaligned(const uint8_t *p)
{
    int16_t v;
    memcpy(&v, p, sizeof(v));
    return v;
}

static int32_t load_s32_unaligned(const uint8_t *p)
{
    int32_t v;
    memcpy(&v, p, sizeof(v));
    return v;
}

static int64_t load_s64_unaligned(const uint8_t *p)
{
    int64_t v;
    memcpy(&v, p, sizeof(v));
    return v;
}

/* 0x001a3dc0 */
unsigned snes_size_of_encoded_value(uint8_t encoding)
{
    if (encoding == DW_EH_PE_OMIT)
        return 0;

    switch (encoding & 7U) {
    case DW_EH_PE_ABSPTR: return 4; /* sizeof(void *) on the EE */
    case DW_EH_PE_UDATA2: return 2;
    case DW_EH_PE_UDATA4: return 4;
    case DW_EH_PE_UDATA8: return 8;
    default: abort();
    }
}

/* 0x001a3ee8 */
const uint8_t *snes_read_uleb128(const uint8_t *p, uint64_t *value)
{
    unsigned shift = 0;
    uint64_t result = 0;
    uint8_t byte;

    do {
        byte = *p++;
        result |= (uint64_t)(byte & 0x7fU) << shift;
        shift += 7;
    } while (byte & 0x80U);

    *value = result;
    return p;
}

/* 0x001a3f28 */
const uint8_t *snes_read_sleb128(const uint8_t *p, int64_t *value)
{
    unsigned shift = 0;
    uint64_t result = 0;
    uint8_t byte;

    do {
        byte = *p++;
        result |= (uint64_t)(byte & 0x7fU) << shift;
        shift += 7;
    } while (byte & 0x80U);

    if (shift < 64U && (byte & 0x40U))
        result |= UINT64_MAX << shift;

    *value = (int64_t)result;
    return p;
}

/* 0x001a40e8..0x001a4100 — exact target context offsets. */
uint32_t snes_Unwind_GetLanguageSpecificData(const SnesUnwindContext *ctx)
{
    return load_u32_unaligned(ctx->raw + 0x148);
}

uint32_t snes_Unwind_GetRegionStart(const SnesUnwindContext *ctx)
{
    return load_u32_unaligned(ctx->raw + 0x154);
}

uint32_t snes_Unwind_GetDataRelBase(const SnesUnwindContext *ctx)
{
    return load_u32_unaligned(ctx->raw + 0x150);
}

uint32_t snes_Unwind_GetTextRelBase(const SnesUnwindContext *ctx)
{
    return load_u32_unaligned(ctx->raw + 0x14c);
}

/* 0x001a3e30 */
uint32_t snes_base_of_encoded_value(uint8_t encoding, const SnesUnwindContext *ctx)
{
    if (encoding == DW_EH_PE_OMIT)
        return 0;

    switch (encoding & 0x70U) {
    case DW_EH_PE_ABSPTR:
    case DW_EH_PE_PCREL:
    case DW_EH_PE_ALIGNED:
        return 0;
    case DW_EH_PE_TEXTREL:
        return snes_Unwind_GetTextRelBase(ctx);
    case DW_EH_PE_DATAREL:
        return snes_Unwind_GetDataRelBase(ctx);
    case DW_EH_PE_FUNCREL:
        return snes_Unwind_GetRegionStart(ctx);
    default:
        abort();
    }
}

/*
 * 0x001a3f88.  INDIRECT is represented through an optional callback because
 * a host research build cannot dereference a raw 32-bit EE address safely.
 */
typedef uint32_t (*SnesEeRead32)(uint32_t address, void *opaque);

const uint8_t *snes_read_encoded_value_with_base(
    uint8_t encoding,
    uint32_t base,
    const uint8_t *p,
    uint64_t *value,
    SnesEeRead32 read32,
    void *opaque)
{
    const uint8_t *start = p;
    uint64_t result = 0;

    if (encoding == DW_EH_PE_ALIGNED) {
        uintptr_t a = (uintptr_t)p;
        a = (a + 3U) & ~(uintptr_t)3U;
        result = load_u32_unaligned((const uint8_t *)a);
        p = (const uint8_t *)(a + 4U);
    } else {
        switch (encoding & 0x0fU) {
        case DW_EH_PE_ABSPTR:
            result = load_u32_unaligned(p); p += 4; break;
        case DW_EH_PE_ULEB128:
            p = snes_read_uleb128(p, &result); break;
        case DW_EH_PE_SLEB128: {
            int64_t s;
            p = snes_read_sleb128(p, &s);
            result = (uint64_t)s;
            break;
        }
        case DW_EH_PE_UDATA2:
            result = (uint16_t)load_s16_unaligned(p); p += 2; break;
        case DW_EH_PE_UDATA4:
            result = load_u32_unaligned(p); p += 4; break;
        case DW_EH_PE_UDATA8:
            result = load_u64_unaligned(p); p += 8; break;
        case DW_EH_PE_SDATA2:
            result = (uint64_t)(int64_t)load_s16_unaligned(p); p += 2; break;
        case DW_EH_PE_SDATA4:
            result = (uint64_t)(int64_t)load_s32_unaligned(p); p += 4; break;
        case DW_EH_PE_SDATA8:
            result = (uint64_t)load_s64_unaligned(p); p += 8; break;
        default:
            abort();
        }

        if (result != 0) {
            if ((encoding & 0x70U) == DW_EH_PE_PCREL)
                result += (uint32_t)(uintptr_t)start;
            else
                result += base;

            if ((encoding & DW_EH_PE_INDIRECT) && read32 != NULL)
                result = read32((uint32_t)result, opaque);
        }
    }

    *value = result;
    return p;
}

/* 0x001a5c80 / 0x001a5c98 — register save slots are 32-bit EE pointers. */
uint64_t snes_Unwind_GetGR(const SnesUnwindContext *ctx, unsigned index)
{
    const uint32_t slot = load_u32_unaligned(ctx->raw + index * 4U);
    uint64_t value;
    memcpy(&value, (const void *)(uintptr_t)slot, sizeof(value));
    return value;
}

void snes_Unwind_SetGR(SnesUnwindContext *ctx, unsigned index, uint64_t value)
{
    const uint32_t slot = load_u32_unaligned(ctx->raw + index * 4U);
    memcpy((void *)(uintptr_t)slot, &value, sizeof(value));
}

/* 0x001a5cb0 / 0x001a5cb8 */
uint32_t snes_Unwind_GetIP(const SnesUnwindContext *ctx)
{
    return load_u32_unaligned(ctx->raw + 0x144);
}

void snes_Unwind_SetIP(SnesUnwindContext *ctx, uint32_t value)
{
    memcpy(ctx->raw + 0x144, &value, sizeof(value));
}
