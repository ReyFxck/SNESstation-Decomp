/*
 * SNES Station v0.23 -- linked libsupc++ RTTI / exception-runtime recovery.
 *
 * Target corridor: 0x001a9fa8..0x001ab3bc.
 *
 * This is an independently written C behavioral model of the stripped target,
 * not a copy of libstdc++ source and not a matching-build claim.  Target
 * assembly, target vtables/typeinfo objects, and target strings establish the
 * names/layouts first. Historical GCC/libsupc++ material is used only as a
 * cross-check after those facts are visible in the binary.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* 32-bit EE pointers are represented as target virtual addresses here. */
typedef uint32_t SnesAddr32;

typedef struct {
    SnesAddr32 vptr;
    SnesAddr32 name;
} SnesTypeInfo32;

typedef struct {
    SnesAddr32 vptr;
} SnesStdException32;

typedef struct {
    SnesAddr32 obj_ptr;
    uint32_t path;
    uint32_t src_details;
    uint32_t base_type;
} SnesUpcastResult32;

/*
 * Target __cxa_exception is 0x50 bytes.  Its 0x20-byte _Unwind_Exception tail
 * starts at +0x30; __cxa_begin_catch receives that tail and backs up 0x30.
 */
typedef struct {
    SnesAddr32 exception_type;       /* +0x00 */
    SnesAddr32 exception_destructor; /* +0x04 */
    SnesAddr32 unexpected_handler;   /* +0x08 */
    SnesAddr32 terminate_handler;    /* +0x0c */
    SnesAddr32 next_exception;       /* +0x10 */
    int32_t handler_count;           /* +0x14 */
    int32_t handler_switch_value;    /* +0x18 */
    SnesAddr32 action_record;        /* +0x1c */
    SnesAddr32 language_specific;    /* +0x20 */
    SnesAddr32 catch_temp;           /* +0x24 */
    SnesAddr32 adjusted_ptr;         /* +0x28 */
    uint32_t reserved_2c;            /* +0x2c */
    uint8_t unwind_exception[0x20];  /* +0x30 */
} SnesCxaException32;

typedef struct {
    SnesCxaException32 *caught_exceptions;
    uint32_t uncaught_exceptions;
} SnesCxaEhGlobalsModel;

typedef void (*SnesDeleteFn)(void *ptr, void *opaque);
typedef void (*SnesTerminateFn)(void *opaque);
typedef void (*SnesUnwindDeleteFn)(SnesCxaException32 *header, void *opaque);

typedef int (*SnesRttiCatchDispatch)(
    const SnesTypeInfo32 *dynamic_type,
    const SnesTypeInfo32 *wanted_type,
    SnesAddr32 *thrown_object,
    unsigned outer,
    void *opaque);

typedef int (*SnesRttiUpcastDispatch)(
    const SnesTypeInfo32 *dynamic_type,
    const SnesTypeInfo32 *wanted_type,
    SnesAddr32 object,
    SnesUpcastResult32 *result,
    void *opaque);

#define SNES_STATIC_ASSERT(name, expr) typedef char name[(expr) ? 1 : -1]
SNES_STATIC_ASSERT(snes_cxa_exception_must_be_0x50,
                   sizeof(SnesCxaException32) == 0x50);

/* Target address points for the linked RTTI/standard-exception vtables. */
enum {
    SNES_VPTR_TYPE_INFO       = 0x00426d08u,
    SNES_VPTR_CLASS_TYPE_INFO = 0x00426ca8u,
    SNES_VPTR_SI_TYPE_INFO    = 0x00426c78u,
    SNES_VPTR_VMI_TYPE_INFO   = 0x00426c48u,
    SNES_VPTR_BAD_CAST        = 0x00426cf0u,
    SNES_VPTR_BAD_TYPEID      = 0x00426cd8u,
    SNES_VPTR_EXCEPTION       = 0x00426d98u,
    SNES_VPTR_BAD_EXCEPTION   = 0x00426d80u,
    SNES_VPTR_BAD_ALLOC       = 0x00426dc8u
};

/* Target addresses of the mangled type-name strings reached by what(). */
enum {
    SNES_NAME_BAD_CAST      = 0x001bae50u,
    SNES_NAME_BAD_TYPEID    = 0x001bae40u,
    SNES_NAME_EXCEPTION     = 0x001bae88u,
    SNES_NAME_BAD_EXCEPTION = 0x001bae70u,
    SNES_NAME_BAD_ALLOC     = 0x001bae98u
};

/* 0x001a9fa8 / 0x001a9fb8 -- std::type_info D2/D1. */
void snes_type_info_dtor_D2(SnesTypeInfo32 *self)
{
    self->vptr = SNES_VPTR_TYPE_INFO;
}

void snes_type_info_dtor_D1(SnesTypeInfo32 *self)
{
    self->vptr = SNES_VPTR_TYPE_INFO;
}

/* 0x001a9fc8 -- deleting std::type_info destructor. */
void snes_type_info_dtor_D0(SnesTypeInfo32 *self,
                            SnesDeleteFn delete_fn, void *opaque)
{
    self->vptr = SNES_VPTR_TYPE_INFO;
    if (delete_fn != NULL)
        delete_fn(self, opaque);
}

/* 0x001aa100 / 0x001aa108 -- std::type_info virtual leaves. */
int snes_type_info_is_pointer_p(const SnesTypeInfo32 *self)
{
    (void)self;
    return 0;
}

int snes_type_info_is_function_p(const SnesTypeInfo32 *self)
{
    (void)self;
    return 0;
}

/* 0x001aa110 -- std::type_info::__do_catch. */
int snes_type_info_do_catch(const SnesTypeInfo32 *self,
                            const SnesTypeInfo32 *thrown_type)
{
    return self->name == thrown_type->name;
}

/* 0x001aa128 -- base std::type_info::__do_upcast always fails. */
int snes_type_info_do_upcast_base(const SnesTypeInfo32 *self)
{
    (void)self;
    return 0;
}

/*
 * Destructor families below preserve the target's explicit vptr stores and
 * base-destructor chaining. D0 wrappers then delegate to operator delete.
 */
void snes_std_exception_dtor_D2(SnesStdException32 *self)
{
    self->vptr = SNES_VPTR_EXCEPTION;
}

void snes_std_exception_dtor_D1(SnesStdException32 *self)
{
    self->vptr = SNES_VPTR_EXCEPTION;
}

void snes_std_exception_dtor_D0(SnesStdException32 *self,
                                SnesDeleteFn delete_fn, void *opaque)
{
    self->vptr = SNES_VPTR_EXCEPTION;
    if (delete_fn != NULL)
        delete_fn(self, opaque);
}

static void derived_exception_dtor(SnesStdException32 *self,
                                   SnesAddr32 derived_vptr)
{
    self->vptr = derived_vptr;
    snes_std_exception_dtor_D2(self);
}

static void derived_exception_dtor_delete(SnesStdException32 *self,
                                          SnesAddr32 derived_vptr,
                                          SnesDeleteFn delete_fn,
                                          void *opaque)
{
    derived_exception_dtor(self, derived_vptr);
    if (delete_fn != NULL)
        delete_fn(self, opaque);
}

/* 0x001a9ff0 / 0x001aa018 / 0x001aa040 -- std::bad_cast. */
void snes_bad_cast_dtor_D2(SnesStdException32 *self)
{
    derived_exception_dtor(self, SNES_VPTR_BAD_CAST);
}

void snes_bad_cast_dtor_D1(SnesStdException32 *self)
{
    derived_exception_dtor(self, SNES_VPTR_BAD_CAST);
}

void snes_bad_cast_dtor_D0(SnesStdException32 *self,
                           SnesDeleteFn delete_fn, void *opaque)
{
    derived_exception_dtor_delete(self, SNES_VPTR_BAD_CAST, delete_fn, opaque);
}

/* 0x001aa078 / 0x001aa0a0 / 0x001aa0c8 -- std::bad_typeid. */
void snes_bad_typeid_dtor_D2(SnesStdException32 *self)
{
    derived_exception_dtor(self, SNES_VPTR_BAD_TYPEID);
}

void snes_bad_typeid_dtor_D1(SnesStdException32 *self)
{
    derived_exception_dtor(self, SNES_VPTR_BAD_TYPEID);
}

void snes_bad_typeid_dtor_D0(SnesStdException32 *self,
                             SnesDeleteFn delete_fn, void *opaque)
{
    derived_exception_dtor_delete(self, SNES_VPTR_BAD_TYPEID, delete_fn, opaque);
}

/* 0x001ab270 / 0x001ab298 / 0x001ab2c0 -- std::bad_exception. */
void snes_bad_exception_dtor_D2(SnesStdException32 *self)
{
    derived_exception_dtor(self, SNES_VPTR_BAD_EXCEPTION);
}

void snes_bad_exception_dtor_D1(SnesStdException32 *self)
{
    derived_exception_dtor(self, SNES_VPTR_BAD_EXCEPTION);
}

void snes_bad_exception_dtor_D0(SnesStdException32 *self,
                                SnesDeleteFn delete_fn, void *opaque)
{
    derived_exception_dtor_delete(self, SNES_VPTR_BAD_EXCEPTION,
                                  delete_fn, opaque);
}

/* 0x001ab338 / 0x001ab360 / 0x001ab388 -- std::bad_alloc. */
void snes_bad_alloc_dtor_D2(SnesStdException32 *self)
{
    derived_exception_dtor(self, SNES_VPTR_BAD_ALLOC);
}

void snes_bad_alloc_dtor_D1(SnesStdException32 *self)
{
    derived_exception_dtor(self, SNES_VPTR_BAD_ALLOC);
}

void snes_bad_alloc_dtor_D0(SnesStdException32 *self,
                            SnesDeleteFn delete_fn, void *opaque)
{
    derived_exception_dtor_delete(self, SNES_VPTR_BAD_ALLOC,
                                  delete_fn, opaque);
}

/*
 * 0x001ab2f8 -- shared std::exception::what() body.
 *
 * The machine code resolves vtable[-1] -> type_info -> name.  For the five
 * linked standard-exception vtables this switch is exactly the same target
 * result without requiring the original read-only data image on a host build.
 */
SnesAddr32 snes_std_exception_what_target_name(const SnesStdException32 *self)
{
    switch (self->vptr) {
    case SNES_VPTR_BAD_CAST:      return SNES_NAME_BAD_CAST;
    case SNES_VPTR_BAD_TYPEID:    return SNES_NAME_BAD_TYPEID;
    case SNES_VPTR_BAD_EXCEPTION: return SNES_NAME_BAD_EXCEPTION;
    case SNES_VPTR_BAD_ALLOC:     return SNES_NAME_BAD_ALLOC;
    case SNES_VPTR_EXCEPTION:     return SNES_NAME_EXCEPTION;
    default:                      return 0;
    }
}

/* __class_type_info / __si_class_type_info / __vmi_class_type_info dtors. */
static void class_type_info_dtor(SnesTypeInfo32 *self, SnesAddr32 own_vptr)
{
    self->vptr = own_vptr;
    snes_type_info_dtor_D2(self);
}

static void class_type_info_dtor_delete(SnesTypeInfo32 *self,
                                        SnesAddr32 own_vptr,
                                        SnesDeleteFn delete_fn,
                                        void *opaque)
{
    class_type_info_dtor(self, own_vptr);
    if (delete_fn != NULL)
        delete_fn(self, opaque);
}

/* 0x001aa130 / 0x001aa158 / 0x001aa180. */
void snes_class_type_info_dtor_D2(SnesTypeInfo32 *self)
{
    class_type_info_dtor(self, SNES_VPTR_CLASS_TYPE_INFO);
}

void snes_class_type_info_dtor_D1(SnesTypeInfo32 *self)
{
    class_type_info_dtor(self, SNES_VPTR_CLASS_TYPE_INFO);
}

void snes_class_type_info_dtor_D0(SnesTypeInfo32 *self,
                                  SnesDeleteFn delete_fn, void *opaque)
{
    class_type_info_dtor_delete(self, SNES_VPTR_CLASS_TYPE_INFO,
                                delete_fn, opaque);
}

/* 0x001aa1b8 / 0x001aa1e0 / 0x001aa208. */
void snes_si_class_type_info_dtor_D2(SnesTypeInfo32 *self)
{
    class_type_info_dtor(self, SNES_VPTR_SI_TYPE_INFO);
}

void snes_si_class_type_info_dtor_D1(SnesTypeInfo32 *self)
{
    class_type_info_dtor(self, SNES_VPTR_SI_TYPE_INFO);
}

void snes_si_class_type_info_dtor_D0(SnesTypeInfo32 *self,
                                     SnesDeleteFn delete_fn, void *opaque)
{
    class_type_info_dtor_delete(self, SNES_VPTR_SI_TYPE_INFO,
                                delete_fn, opaque);
}

/* 0x001aa240 / 0x001aa268 / 0x001aa290. */
void snes_vmi_class_type_info_dtor_D2(SnesTypeInfo32 *self)
{
    class_type_info_dtor(self, SNES_VPTR_VMI_TYPE_INFO);
}

void snes_vmi_class_type_info_dtor_D1(SnesTypeInfo32 *self)
{
    class_type_info_dtor(self, SNES_VPTR_VMI_TYPE_INFO);
}

void snes_vmi_class_type_info_dtor_D0(SnesTypeInfo32 *self,
                                      SnesDeleteFn delete_fn, void *opaque)
{
    class_type_info_dtor_delete(self, SNES_VPTR_VMI_TYPE_INFO,
                                delete_fn, opaque);
}

/*
 * 0x001aa2c8 -- __class_type_info::__do_catch.
 * Name identity is the fast success path.  With outer >= 4 the target dispatches
 * thrown_type->__do_catch(this, ...); smaller outer values fail immediately.
 */
int snes_class_type_info_do_catch(const SnesTypeInfo32 *self,
                                  const SnesTypeInfo32 *thrown_type,
                                  SnesAddr32 *thrown_object,
                                  unsigned outer,
                                  SnesRttiCatchDispatch recurse,
                                  void *opaque)
{
    if (self->name == thrown_type->name)
        return 1;
    if (outer < 4 || recurse == NULL)
        return 0;
    return recurse(thrown_type, self, thrown_object, outer, opaque);
}

/* 0x001aab20 -- __class_type_info protected three-argument upcast. */
int snes_class_type_info_do_upcast3(const SnesTypeInfo32 *self,
                                    const SnesTypeInfo32 *dst,
                                    SnesAddr32 object,
                                    SnesUpcastResult32 *result)
{
    if (self->name != dst->name)
        return 0;
    result->obj_ptr = object;
    result->path = 6;
    result->base_type = 8;
    return 1;
}

/*
 * 0x001aab58 -- __si_class_type_info protected upcast.
 * Its +8 base_type pointer is externalized as base_type + recursive dispatch.
 */
int snes_si_class_type_info_do_upcast3(const SnesTypeInfo32 *self,
                                       const SnesTypeInfo32 *dst,
                                       SnesAddr32 object,
                                       SnesUpcastResult32 *result,
                                       const SnesTypeInfo32 *base_type,
                                       SnesRttiUpcastDispatch recurse,
                                       void *opaque)
{
    if (snes_class_type_info_do_upcast3(self, dst, object, result))
        return 1;
    if (base_type == NULL || recurse == NULL)
        return 0;
    return recurse(base_type, dst, object, result, opaque);
}

/* 0x001aa320 -- public two-argument upcast wrapper. */
int snes_class_type_info_do_upcast2(const SnesTypeInfo32 *self,
                                    const SnesTypeInfo32 *dst,
                                    SnesAddr32 *object,
                                    SnesRttiUpcastDispatch dispatch,
                                    void *opaque)
{
    SnesUpcastResult32 result;

    result.obj_ptr = 0;
    result.path = 0;
    result.src_details = 0x10;
    result.base_type = 0;

    if (dispatch != NULL)
        (void)dispatch(self, dst, *object, &result, opaque);

    if ((result.path & 6u) != 6u)
        return 0;
    *object = result.obj_ptr;
    return 1;
}

/* 0x001aa390 -- __class_type_info::__do_find_public_src leaf. */
unsigned snes_class_type_info_find_public_src(SnesAddr32 object,
                                               SnesAddr32 source)
{
    return object == source ? 6u : 1u;
}

/*
 * 0x001aafb8 / 0x001ab080 -- exception allocation and free.
 * The target uses four 512-byte emergency slots and a 32-bit used mask.
 */
typedef union {
    uint64_t align;
    uint8_t slot[4][512];
} SnesEmergencyPool;

static SnesEmergencyPool snes_emergency_pool;
static uint32_t snes_emergency_used;

void *snes_cxa_allocate_exception(size_t thrown_size,
                                  SnesTerminateFn terminate_fn,
                                  void *terminate_opaque)
{
    const size_t total = thrown_size + sizeof(SnesCxaException32);
    void *ret = malloc(total);

    if (ret == NULL) {
        uint32_t used = snes_emergency_used;
        unsigned which = 0;

        if (total <= 512) {
            while ((used & 1u) != 0u) {
                used >>= 1;
                if (++which >= 4)
                    break;
            }
            if (which < 4) {
                snes_emergency_used |= UINT32_C(1) << which;
                ret = snes_emergency_pool.slot[which];
            }
        }

        if (ret == NULL) {
            if (terminate_fn != NULL)
                terminate_fn(terminate_opaque);
            return NULL;
        }
    }

    memset(ret, 0, sizeof(SnesCxaException32));
    return (uint8_t *)ret + sizeof(SnesCxaException32);
}

void snes_cxa_free_exception(void *thrown_object)
{
    uint8_t *ptr = (uint8_t *)thrown_object;
    uint8_t *begin = &snes_emergency_pool.slot[0][0];
    uint8_t *end = begin + sizeof(snes_emergency_pool.slot);

    if (ptr >= begin && ptr < end) {
        unsigned which = (unsigned)(ptr - begin) / 512u;
        snes_emergency_used &= ~(UINT32_C(1) << which);
    } else if (ptr != NULL) {
        free(ptr - sizeof(SnesCxaException32));
    }
}

/* 0x001ab308 / 0x001ab318 -- fast and regular globals are the same target. */
SnesCxaEhGlobalsModel *snes_cxa_get_globals_fast(SnesCxaEhGlobalsModel *globals)
{
    return globals;
}

SnesCxaEhGlobalsModel *snes_cxa_get_globals(SnesCxaEhGlobalsModel *globals)
{
    return globals;
}

/* 0x001ab0f0 -- __cxa_begin_catch state transition. */
SnesAddr32 snes_cxa_begin_catch(SnesCxaEhGlobalsModel *globals,
                                SnesCxaException32 *exception)
{
    int32_t count = exception->handler_count;

    if (count < 0)
        count = 1 - count;
    else
        ++count;
    exception->handler_count = count;

    --globals->uncaught_exceptions;
    if (globals->caught_exceptions != exception) {
        exception->next_exception =
            (SnesAddr32)(uintptr_t)globals->caught_exceptions;
        globals->caught_exceptions = exception;
    }

    return exception->adjusted_ptr;
}

/* 0x001ab158 -- __cxa_end_catch state transition. */
void snes_cxa_end_catch(SnesCxaEhGlobalsModel *globals,
                        SnesUnwindDeleteFn unwind_delete,
                        void *opaque)
{
    SnesCxaException32 *exception = globals->caught_exceptions;
    int32_t count;

    if (exception == NULL)
        return;

    count = exception->handler_count;
    if (count < 0) {
        ++count;
        if (count == 0) {
            ++globals->uncaught_exceptions;
            globals->caught_exceptions =
                (SnesCxaException32 *)(uintptr_t)exception->next_exception;
        }
        exception->handler_count = count;
        return;
    }

    --count;
    exception->handler_count = count;
    if (count != 0)
        return;

    globals->caught_exceptions =
        (SnesCxaException32 *)(uintptr_t)exception->next_exception;
    if (unwind_delete != NULL)
        unwind_delete(exception, opaque);
}

/* 0x001ab200 -- std::uncaught_exception(). */
int snes_std_uncaught_exception(const SnesCxaEhGlobalsModel *globals)
{
    return globals->uncaught_exceptions != 0;
}

/* 0x001ab328 -- std::set_new_handler(new_handler). */
typedef void (*SnesNewHandler)(void);

SnesNewHandler snes_set_new_handler(SnesNewHandler *slot,
                                    SnesNewHandler replacement)
{
    SnesNewHandler old = *slot;
    *slot = replacement;
    return old;
}

/* Progress 13: the small single-inheritance/public-source RTTI walkers. */
typedef struct {
    SnesTypeInfo32 base;
    SnesAddr32 base_type; /* +0x08 */
} SnesSiClassTypeInfo32;

typedef struct {
    SnesAddr32 dst_ptr; /* +0x00 */
    int32_t whole2dst;  /* +0x04 */
    int32_t whole2src;  /* +0x08 */
    int32_t dst2src;    /* +0x0c */
} SnesDyncastResult32;

typedef unsigned (*SnesFindPublicSrcDispatch)(
    SnesAddr32 base_type, int32_t src2dst, SnesAddr32 object,
    const SnesTypeInfo32 *src_type, SnesAddr32 src_ptr, void *opaque);

typedef int (*SnesDyncastDispatch)(
    SnesAddr32 base_type, int32_t src2dst, int32_t access_path,
    const SnesTypeInfo32 *dst_type, SnesAddr32 object,
    const SnesTypeInfo32 *src_type, SnesAddr32 src_ptr,
    SnesDyncastResult32 *result, void *opaque);

/*
 * 0x001aa3a8 -- __si_class_type_info::__do_find_public_src.
 * The sole base is recursively queried unless the current object is exactly
 * src_ptr and this type name is already src_type, in which case the target
 * returns the public-contained mask 6 immediately.
 */
unsigned snes_si_class_type_info_find_public_src(
    const SnesSiClassTypeInfo32 *self, int32_t src2dst, SnesAddr32 object,
    const SnesTypeInfo32 *src_type, SnesAddr32 src_ptr,
    SnesFindPublicSrcDispatch recurse, void *opaque)
{
    if (src_ptr == object && self->base.name == src_type->name)
        return 6u;
    if (recurse == NULL)
        return 1u;
    return recurse(self->base_type, src2dst, object, src_type, src_ptr, opaque);
}

/* 0x001aa548 -- __class_type_info::__do_dyncast. */
int snes_class_type_info_do_dyncast(
    const SnesTypeInfo32 *self, int32_t src2dst, int32_t access_path,
    const SnesTypeInfo32 *dst_type, SnesAddr32 object,
    const SnesTypeInfo32 *src_type, SnesAddr32 src_ptr,
    SnesDyncastResult32 *result)
{
    (void)src2dst;

    if (object == src_ptr && self->name == src_type->name) {
        result->whole2src = access_path;
        return 0;
    }

    if (self->name == dst_type->name) {
        result->dst_ptr = object;
        result->whole2dst = access_path;
        result->dst2src = 1;
    }
    return 0;
}

/* 0x001aa590 -- __si_class_type_info::__do_dyncast. */
int snes_si_class_type_info_do_dyncast(
    const SnesSiClassTypeInfo32 *self, int32_t src2dst, int32_t access_path,
    const SnesTypeInfo32 *dst_type, SnesAddr32 object,
    const SnesTypeInfo32 *src_type, SnesAddr32 src_ptr,
    SnesDyncastResult32 *result, SnesDyncastDispatch recurse, void *opaque)
{
    if (self->base.name == dst_type->name) {
        result->whole2dst = access_path;
        result->dst_ptr = object;

        if (src2dst >= 0)
            result->dst2src = (object + (SnesAddr32)src2dst == src_ptr) ? 6 : 1;
        else if (src2dst == -2)
            result->dst2src = 1;
        return 0;
    }

    if (object == src_ptr && self->base.name == src_type->name) {
        result->whole2src = access_path;
        return 0;
    }

    if (recurse == NULL)
        return 0;
    return recurse(self->base_type, src2dst, access_path, dst_type, object,
                   src_type, src_ptr, result, opaque);
}
