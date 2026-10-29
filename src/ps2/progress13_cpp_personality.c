/*
 * Progress 13 -- compact GCC 3.2.2-era C++ personality helpers recovered
 * directly from SNES Station v0.23.  The EE image is authoritative; raw
 * 32-bit target addresses are kept explicit so host validation does not
 * accidentally change the target ABI.
 */
#include <stddef.h>
#include <stdint.h>

/* Existing independently recovered unwind-pe primitives. */
typedef struct SnesUnwindContext SnesUnwindContext;
typedef uint32_t (*SnesEeRead32P13)(uint32_t address, void *opaque);
unsigned snes_size_of_encoded_value(uint8_t encoding);
const uint8_t *snes_read_uleb128(const uint8_t *p, uint64_t *value);
uint32_t snes_base_of_encoded_value(uint8_t encoding,
                                    const SnesUnwindContext *ctx);
uint32_t snes_Unwind_GetRegionStart(const SnesUnwindContext *ctx);
const uint8_t *snes_read_encoded_value_with_base(
    uint8_t encoding, uint32_t base, const uint8_t *p, uint64_t *value,
    SnesEeRead32P13 read32, void *opaque);

#define P13_DW_EH_PE_OMIT 0xffu

typedef struct {
    uint32_t region_start;      /* +0x00 */
    uint32_t lp_start;          /* +0x04 */
    uint32_t ttype_base;        /* +0x08, filled by personality caller */
    uint32_t ttype_table;       /* +0x0c */
    uint32_t action_table;      /* +0x10 */
    uint8_t ttype_encoding;     /* +0x14 */
    uint8_t call_site_encoding; /* +0x15 */
    uint8_t pad_16[2];
} SnesP13LsdaHeader;

typedef char snes_p13_lsda_header_size[
    (sizeof(SnesP13LsdaHeader) == 0x18) ? 1 : -1];

/* 0x001a5c58 -- _Unwind_DeleteException. */
typedef void (*SnesP13UnwindCleanup)(int reason, void *exception_object,
                                     void *opaque);
void snes_p13_Unwind_DeleteException_001a5c58(
    void *exception_object, SnesP13UnwindCleanup cleanup, void *opaque)
{
    /* Target loads the cleanup function at unwind_exception + 8. */
    if (cleanup != NULL)
        cleanup(1, exception_object, opaque);
}

/*
 * 0x001a9488 -- parse_lsda_header.  The target consumes the LPStart encoding,
 * optional type-table displacement, call-site encoding and call-site length in
 * exactly this order.  `ttype_base` is intentionally untouched here: the
 * personality routine fills it immediately after this helper returns.
 */
const uint8_t *snes_p13_parse_lsda_header_001a9488(
    const SnesUnwindContext *ctx, const uint8_t *p, SnesP13LsdaHeader *out,
    SnesEeRead32P13 read32, void *opaque)
{
    uint64_t value;
    uint8_t encoding;

    out->region_start = ctx != NULL ? snes_Unwind_GetRegionStart(ctx) : 0;

    encoding = *p++;
    if (encoding == P13_DW_EH_PE_OMIT) {
        out->lp_start = out->region_start;
    } else {
        uint32_t base = snes_base_of_encoded_value(encoding, ctx);
        p = snes_read_encoded_value_with_base(encoding, base, p, &value,
                                              read32, opaque);
        out->lp_start = (uint32_t)value;
    }

    out->ttype_encoding = *p++;
    if (out->ttype_encoding == P13_DW_EH_PE_OMIT) {
        out->ttype_table = 0;
    } else {
        p = snes_read_uleb128(p, &value);
        out->ttype_table = (uint32_t)(uintptr_t)p + (uint32_t)value;
    }

    out->call_site_encoding = *p++;
    p = snes_read_uleb128(p, &value);
    out->action_table = (uint32_t)(uintptr_t)p + (uint32_t)value;
    return p;
}

/* 0x001a9588 -- get_ttype_entry. */
uint32_t snes_p13_get_ttype_entry_001a9588(
    const SnesP13LsdaHeader *header, int32_t type_index,
    const uint8_t *target_address_zero, SnesEeRead32P13 read32, void *opaque)
{
    unsigned size = snes_size_of_encoded_value(header->ttype_encoding);
    int64_t displacement = (int64_t)type_index * (int64_t)size;
    uint32_t source_ee = header->ttype_table - (uint32_t)displacement;
    const uint8_t *source = target_address_zero + source_ee;
    uint64_t decoded = 0;

    (void)snes_read_encoded_value_with_base(header->ttype_encoding,
                                            header->ttype_base, source,
                                            &decoded, read32, opaque);
    return (uint32_t)decoded;
}

/*
 * 0x001a95f8 -- get_adjusted_ptr.  These callbacks correspond to the two
 * target virtual calls: exception_type->__is_pointer_p() at vtable +8 and
 * catch_type->__do_catch(..., outer=1) at vtable +0x10.
 */
typedef int (*SnesP13IsPointerType)(uint32_t exception_type, void *opaque);
typedef int (*SnesP13DoCatch)(uint32_t catch_type, uint32_t exception_type,
                              uint32_t *adjusted_ptr, int outer, void *opaque);
typedef uint32_t (*SnesP13ReadTargetWord)(uint32_t address, void *opaque);

int snes_p13_get_adjusted_ptr_001a95f8(
    uint32_t catch_type, uint32_t exception_type, uint32_t *adjusted_ptr,
    SnesP13IsPointerType is_pointer_type, SnesP13DoCatch do_catch,
    SnesP13ReadTargetWord read_word, void *opaque)
{
    uint32_t temporary = *adjusted_ptr;

    if (is_pointer_type != NULL && is_pointer_type(exception_type, opaque)) {
        if (read_word != NULL)
            temporary = read_word(temporary, opaque);
    }

    if (do_catch != NULL &&
        do_catch(catch_type, exception_type, &temporary, 1, opaque)) {
        *adjusted_ptr = temporary;
        return 1;
    }
    return 0;
}

/* 0x001a9698 -- check_exception_spec. */
int snes_p13_check_exception_spec_001a9698(
    const SnesP13LsdaHeader *header, uint32_t exception_type,
    uint32_t adjusted_ptr, int32_t filter, const uint8_t *target_address_zero,
    SnesEeRead32P13 read32, SnesP13IsPointerType is_pointer_type,
    SnesP13DoCatch do_catch, SnesP13ReadTargetWord read_word, void *opaque)
{
    const uint8_t *p = target_address_zero +
                       (header->ttype_table - (uint32_t)filter - 1u);

    for (;;) {
        uint64_t type_index;
        uint32_t catch_type;
        p = snes_read_uleb128(p, &type_index);
        if (type_index == 0)
            return 0;

        catch_type = snes_p13_get_ttype_entry_001a9588(
            header, (int32_t)type_index, target_address_zero, read32, opaque);
        if (snes_p13_get_adjusted_ptr_001a95f8(
                catch_type, exception_type, &adjusted_ptr, is_pointer_type,
                do_catch, read_word, opaque))
            return 1;
    }
}
