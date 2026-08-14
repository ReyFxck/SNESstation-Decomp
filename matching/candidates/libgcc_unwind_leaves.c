/*
 * Progress 29 compiler-fingerprint candidates for the compact GCC 3.2.2-b1
 * unwind-dw2 / unwind-pe helpers embedded in SNES Station v0.23.
 *
 * This file is intentionally self-contained: the stage-one EE compiler was
 * built --without-headers, so a matching experiment must not depend on host
 * libc headers.  Function names use a _candidate suffix so the object can be
 * compared without claiming that this is Hiryu's original source.
 */

typedef unsigned char p29_u8;
typedef signed short p29_s16;
typedef unsigned short p29_u16;
typedef signed int p29_s32;
typedef unsigned int p29_u32;
typedef signed long long p29_s64;
typedef unsigned long long p29_u64;
#if defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 8
typedef unsigned long p29_uptr;
#else
typedef unsigned int p29_uptr;
#endif

extern void abort(void) __attribute__((noreturn));

#define P29_DW_EH_PE_ABSPTR   0x00u
#define P29_DW_EH_PE_UDATA2   0x02u
#define P29_DW_EH_PE_UDATA4   0x03u
#define P29_DW_EH_PE_UDATA8   0x04u
#define P29_DW_EH_PE_PCREL    0x10u
#define P29_DW_EH_PE_TEXTREL  0x20u
#define P29_DW_EH_PE_DATAREL  0x30u
#define P29_DW_EH_PE_FUNCREL  0x40u
#define P29_DW_EH_PE_ALIGNED  0x50u
#define P29_DW_EH_PE_OMIT     0xffu

/*
 * These four leaves are eight bytes each in the target.  Keeping direct
 * lvalue accesses (rather than memcpy-based host-safe modeling) gives the old
 * EE compiler a chance to reproduce the load in the jr $ra delay slot.
 */
p29_u32 unwind_get_lsda_candidate(const void *context)
{
    return *(const p29_u32 *)((const p29_u8 *)context + 0x148);
}

p29_u32 unwind_get_region_start_candidate(const void *context)
{
    return *(const p29_u32 *)((const p29_u8 *)context + 0x154);
}

p29_u32 unwind_get_data_rel_base_candidate(const void *context)
{
    return *(const p29_u32 *)((const p29_u8 *)context + 0x150);
}

p29_u32 unwind_get_text_rel_base_candidate(const void *context)
{
    return *(const p29_u32 *)((const p29_u8 *)context + 0x14c);
}

/* 0x001a3dc0: compact DW_EH_PE encoded-value size helper. */
unsigned int unwind_size_of_encoded_value_candidate(p29_u8 encoding)
{
    if (encoding == P29_DW_EH_PE_OMIT)
        return 0;

    switch (encoding & 7u) {
    case P29_DW_EH_PE_ABSPTR:
        return 4;
    case P29_DW_EH_PE_UDATA2:
        return 2;
    case P29_DW_EH_PE_UDATA4:
        return 4;
    case P29_DW_EH_PE_UDATA8:
        return 8;
    default:
        abort();
    }
}

/* 0x001a3e30: base selector. Calls deliberately remain external-looking. */
p29_u32 unwind_base_of_encoded_value_candidate(p29_u8 encoding, const void *context)
{
    if (encoding == P29_DW_EH_PE_OMIT)
        return 0;

    switch (encoding & 0x70u) {
    case P29_DW_EH_PE_ABSPTR:
    case P29_DW_EH_PE_PCREL:
    case P29_DW_EH_PE_ALIGNED:
        return 0;
    case P29_DW_EH_PE_TEXTREL:
        return unwind_get_text_rel_base_candidate(context);
    case P29_DW_EH_PE_DATAREL:
        return unwind_get_data_rel_base_candidate(context);
    case P29_DW_EH_PE_FUNCREL:
        return unwind_get_region_start_candidate(context);
    default:
        abort();
    }
}

/* 0x001a3ee8 */
const p29_u8 *unwind_read_uleb128_candidate(const p29_u8 *p, p29_u64 *value)
{
    unsigned int shift;
    p29_u64 result;
    p29_u8 byte;

    shift = 0;
    result = 0;
    do {
        byte = *p++;
        result |= (p29_u64)(byte & 0x7fu) << shift;
        shift += 7;
    } while ((byte & 0x80u) != 0);

    *value = result;
    return p;
}

/* 0x001a3f28 */
const p29_u8 *unwind_read_sleb128_candidate(const p29_u8 *p, p29_s64 *value)
{
    unsigned int shift;
    p29_u64 result;
    p29_u8 byte;

    shift = 0;
    result = 0;
    do {
        byte = *p++;
        result |= (p29_u64)(byte & 0x7fu) << shift;
        shift += 7;
    } while ((byte & 0x80u) != 0);

    if (shift < 64u && (byte & 0x40u) != 0)
        result |= (~(p29_u64)0) << shift;

    *value = (p29_s64)result;
    return p;
}

/*
 * 0x001a5c80 / 0x001a5c98.  Each context register entry is a 32-bit EE
 * address pointing at the 64-bit saved register value.
 */
p29_u64 unwind_get_gr_candidate(const void *context, unsigned int index)
{
    const p29_u32 *slots;
    p29_u32 address;

    slots = (const p29_u32 *)context;
    address = slots[index];
    return *(const p29_u64 *)(p29_uptr)address;
}

void unwind_set_gr_candidate(void *context, unsigned int index, p29_u64 value)
{
    p29_u32 *slots;
    p29_u32 address;

    slots = (p29_u32 *)context;
    address = slots[index];
    *(p29_u64 *)(p29_uptr)address = value;
}

/* 0x001a5cb0 / 0x001a5cb8 */
p29_u32 unwind_get_ip_candidate(const void *context)
{
    return *(const p29_u32 *)((const p29_u8 *)context + 0x144);
}

void unwind_set_ip_candidate(void *context, p29_u32 value)
{
    *(p29_u32 *)((p29_u8 *)context + 0x144) = value;
}
