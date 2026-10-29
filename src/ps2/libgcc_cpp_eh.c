/*
 * Canonical libgcc C++ Exception Handling & Personality Routines
 * for SNES Station v0.23 (EE/R5900 GCC 3.2.2).
 *
 * Consolidated from:
 *   - progress13_cpp_eh_recovered.c (__terminate, __gxx_exception_cleanup, __cxa_throw)
 *   - progress13_cpp_personality_recovered.c (_Unwind_DeleteException, parse_lsda_header, get_ttype_entry)
 *
 * The stripped target EE image is authoritative; raw 32-bit target addresses
 * are kept explicit so host validation does not accidentally change the target ABI.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* ========================================================================= */
/* Forward Declarations - Unwind Primitives                                 */
/* ========================================================================= */

typedef struct SnesUnwindContext SnesUnwindContext;
typedef uint32_t (*SnesEeRead32)(uint32_t address, void *opaque);

extern unsigned snes_size_of_encoded_value(uint8_t encoding);
extern const uint8_t *snes_read_uleb128(const uint8_t *p, uint64_t *value);
extern uint32_t snes_base_of_encoded_value(uint8_t encoding, const SnesUnwindContext *ctx);
extern uint32_t snes_Unwind_GetRegionStart(const SnesUnwindContext *ctx);
extern const uint8_t *snes_read_encoded_value_with_base(
    uint8_t encoding, uint32_t base, const uint8_t *p, uint64_t *value,
    SnesEeRead32 read32, void *opaque);

/* ========================================================================= */
/* C++ Exception Header Structures                                          */
/* ========================================================================= */

typedef struct SnesCxaException {
    uint32_t exception_type;       /* +0x00 */
    uint32_t exception_destructor; /* +0x04 */
    uint32_t unexpected_handler;   /* +0x08 */
    uint32_t terminate_handler;    /* +0x0c */
    uint32_t next_exception;       /* +0x10 */
    int32_t  handler_count;        /* +0x14 */
    int32_t  handler_switch_value; /* +0x18 */
    uint32_t action_record;        /* +0x1c */
    uint32_t language_specific;    /* +0x20 */
    uint32_t catch_temp;           /* +0x24 */
    uint32_t adjusted_ptr;         /* +0x28 */
    uint32_t reserved_2c;          /* +0x2c */
    uint8_t  unwind_exception[0x20]; /* +0x30 */
} SnesCxaException;

typedef char snes_cxa_exception_size[(sizeof(SnesCxaException) == 0x50) ? 1 : -1];

typedef struct {
    SnesCxaException *caught_exceptions;
    uint32_t uncaught_exceptions;
} SnesCxaGlobals;

/* ========================================================================= */
/* Callback Type Definitions                                                */
/* ========================================================================= */

typedef void (*SnesEhVoidFn)(void *opaque);
typedef void (*SnesEhObjectFn)(void *object, void *opaque);
typedef void *(*SnesEhAllocFn)(size_t size, void *opaque);
typedef int (*SnesEhRaiseFn)(void *unwind_exception, void *opaque);
typedef void (*SnesEhFreeExceptionFn)(void *object, void *opaque);
typedef void (*SnesEhThrowBadAllocFn)(void *opaque);

/* ========================================================================= */
/* __terminate (0x001a9ca8)                                                 */
/* ========================================================================= */

void snes___terminate(SnesEhVoidFn handler, SnesEhVoidFn abort_fn, void *opaque)
{
    if (handler != NULL)
        handler(opaque);
    if (abort_fn != NULL)
        abort_fn(opaque);
}

/* ========================================================================= */
/* __gxx_exception_cleanup (0x001a9d58)                                     */
/* ========================================================================= */

void snes___gxx_exception_cleanup(int reason,
                                   SnesCxaException *header,
                                   SnesEhObjectFn destructor,
                                   SnesEhFreeExceptionFn free_exception,
                                   SnesEhVoidFn terminate_fn,
                                   void *opaque)
{
    void *object = (uint8_t *)header + sizeof(*header);

    if (reason != 1) {
        if (terminate_fn != NULL)
            terminate_fn(opaque);
        return;
    }

    if (destructor != NULL)
        destructor(object, opaque);
    if (free_exception != NULL)
        free_exception(object, opaque);
}

/* ========================================================================= */
/* __cxa_throw (0x001a9db8)                                                 */
/* ========================================================================= */

int snes___cxa_throw(void *object,
                      uint32_t type_info,
                      uint32_t destructor_addr,
                      uint32_t unexpected_handler,
                      uint32_t terminate_handler,
                      SnesCxaGlobals *globals,
                      SnesEhRaiseFn raise_exception,
                      SnesEhVoidFn terminate_fn,
                      void *opaque)
{
    SnesCxaException *header =
        (SnesCxaException *)((uint8_t *)object - sizeof(SnesCxaException));

    header->exception_type = type_info;
    header->exception_destructor = destructor_addr;
    header->unexpected_handler = unexpected_handler;
    header->terminate_handler = terminate_handler;
    header->next_exception = (uint32_t)globals->caught_exceptions;
    header->handler_count = 0;
    header->handler_switch_value = 0;
    header->action_record = 0;
    header->language_specific = 0;
    header->catch_temp = 0;
    header->adjusted_ptr = 0;
    header->reserved_2c = 0;

    globals->caught_exceptions = header;
    globals->uncaught_exceptions++;

    return raise_exception(&header->unwind_exception[0], opaque);
}

/* ========================================================================= */
/* DW_EH_PE Encoding Constants                                              */
/* ========================================================================= */

#define SNES_DW_EH_PE_OMIT 0xffu

/* ========================================================================= */
/* LSDA Header Structure                                                    */
/* ========================================================================= */

typedef struct {
    uint32_t region_start;      /* +0x00 */
    uint32_t lp_start;          /* +0x04 */
    uint32_t ttype_base;        /* +0x08, filled by personality caller */
    uint32_t ttype_table;       /* +0x0c */
    uint32_t action_table;      /* +0x10 */
    uint8_t  ttype_encoding;    /* +0x14 */
    uint8_t  call_site_encoding; /* +0x15 */
    uint8_t  pad_16[2];
} SnesLsdaHeader;

typedef char snes_lsda_header_size[(sizeof(SnesLsdaHeader) == 0x18) ? 1 : -1];

/* ========================================================================= */
/* _Unwind_DeleteException (0x001a5c58)                                     */
/* ========================================================================= */

typedef void (*SnesUnwindCleanup)(int reason, void *exception_object, void *opaque);

void snes_Unwind_DeleteException(void *exception_object,
                                  SnesUnwindCleanup cleanup,
                                  void *opaque)
{
    /* Target loads the cleanup function at unwind_exception + 8. */
    if (cleanup != NULL)
        cleanup(1, exception_object, opaque);
}

/* ========================================================================= */
/* parse_lsda_header (0x001a9488)                                           */
/* ========================================================================= */

const uint8_t *snes_parse_lsda_header(const SnesUnwindContext *ctx,
                                       const uint8_t *p,
                                       SnesLsdaHeader *out,
                                       SnesEeRead32 read32,
                                       void *opaque)
{
    uint64_t value;
    uint8_t encoding;

    out->region_start = ctx != NULL ? snes_Unwind_GetRegionStart(ctx) : 0;

    /* LPStart encoding */
    encoding = *p++;
    if (encoding == SNES_DW_EH_PE_OMIT) {
        out->lp_start = out->region_start;
    } else {
        uint32_t base = snes_base_of_encoded_value(encoding, ctx);
        p = snes_read_encoded_value_with_base(encoding, base, p, &value, read32, opaque);
        out->lp_start = (uint32_t)value;
    }

    /* TType encoding */
    out->ttype_encoding = *p++;
    if (out->ttype_encoding == SNES_DW_EH_PE_OMIT) {
        out->ttype_table = 0;
    } else {
        p = snes_read_uleb128(p, &value);
        out->ttype_table = (uint32_t)(uintptr_t)p + (uint32_t)value;
    }

    /* Call-site encoding */
    out->call_site_encoding = *p++;
    p = snes_read_uleb128(p, &value);
    out->action_table = (uint32_t)(uintptr_t)p + (uint32_t)value;

    return p;
}

/* ========================================================================= */
/* get_ttype_entry (0x001a9588)                                             */
/* ========================================================================= */

uint32_t snes_get_ttype_entry(const SnesLsdaHeader *header,
                               int32_t type_index,
                               const uint8_t *target_address_zero,
                               SnesEeRead32 read32,
                               void *opaque)
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

/* ========================================================================= */
/* GCC 3.2.2 Personality Routine - Forward Declaration                      */
/* ========================================================================= */

/* The actual __gxx_personality_v0 implementation is in the target binary.
 * This file provides the helper functions it calls. */