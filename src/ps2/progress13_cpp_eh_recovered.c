/*
 * Progress 13 -- compact behavioral models for the GCC 3.2.2-era C++ EH
 * leaves embedded in SNES Station v0.23.  Target offsets/call order come from
 * the stripped EE image; callback parameters keep the research model host-safe.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef void (*SnesEhVoidFn)(void *opaque);
typedef void (*SnesEhObjectFn)(void *object, void *opaque);
typedef void *(*SnesEhAllocFn)(size_t size, void *opaque);
typedef int (*SnesEhRaiseFn)(void *unwind_exception, void *opaque);
typedef void (*SnesEhFreeExceptionFn)(void *object, void *opaque);
typedef void (*SnesEhThrowBadAllocFn)(void *opaque);

typedef struct SnesCxaExceptionP13 {
    uint32_t exception_type;       /* +0x00 */
    uint32_t exception_destructor; /* +0x04 */
    uint32_t unexpected_handler;   /* +0x08 */
    uint32_t terminate_handler;    /* +0x0c */
    uint32_t next_exception;       /* +0x10 */
    int32_t handler_count;         /* +0x14 */
    int32_t handler_switch_value;  /* +0x18 */
    uint32_t action_record;        /* +0x1c */
    uint32_t language_specific;    /* +0x20 */
    uint32_t catch_temp;           /* +0x24 */
    uint32_t adjusted_ptr;         /* +0x28 */
    uint32_t reserved_2c;          /* +0x2c */
    uint8_t unwind_exception[0x20];/* +0x30 */
} SnesCxaExceptionP13;

typedef char snes_p13_cxa_header_size[(sizeof(SnesCxaExceptionP13) == 0x50) ? 1 : -1];

typedef struct {
    SnesCxaExceptionP13 *caught_exceptions;
    uint32_t uncaught_exceptions;
} SnesCxaGlobalsP13;

/* 0x001a9ca8 -- __terminate: invoke handler, then abort if it returns. */
void snes_p13___terminate(SnesEhVoidFn handler, SnesEhVoidFn abort_fn,
                          void *opaque)
{
    if (handler != NULL)
        handler(opaque);
    if (abort_fn != NULL)
        abort_fn(opaque);
}

/*
 * 0x001a9d58 -- __gxx_exception_cleanup.
 * reason==1 is the target's normal foreign-catch cleanup path.  The unwind
 * pointer is +0x30 inside the 0x50-byte __cxa_exception header, and the thrown
 * object begins immediately after that header.
 */
void snes_p13___gxx_exception_cleanup(int reason,
                                      SnesCxaExceptionP13 *header,
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

/*
 * 0x001a9db8 -- __cxa_throw.  The target fills the 0x50-byte header immediately
 * before the object, snapshots unexpected/terminate handlers, installs the GNU
 * C++ exception class/cleanup, increments uncaughtExceptions, then raises the
 * +0x30 unwind tail.  Numeric function addresses are represented by callbacks
 * in this host model; the structural state transition is preserved.
 */
int snes_p13___cxa_throw(void *object,
                         uint32_t type_info,
                         uint32_t destructor_addr,
                         uint32_t unexpected_handler,
                         uint32_t terminate_handler,
                         SnesCxaGlobalsP13 *globals,
                         SnesEhRaiseFn raise_exception,
                         SnesEhVoidFn terminate_fn,
                         void *opaque)
{
    SnesCxaExceptionP13 *header =
        (SnesCxaExceptionP13 *)((uint8_t *)object - sizeof(SnesCxaExceptionP13));

    header->exception_type = type_info;
    header->exception_destructor = destructor_addr;
    header->unexpected_handler = unexpected_handler;
    header->terminate_handler = terminate_handler;
    /* Target writes the cleanup pointer and GNUCC++ class into unwind tail. */
    {
        const uint64_t exception_class = UINT64_C(0x474e5543432b2b00);
        memset(header->unwind_exception, 0, sizeof(header->unwind_exception));
        memcpy(header->unwind_exception, &exception_class, sizeof(exception_class));
    }

    if (globals != NULL)
        ++globals->uncaught_exceptions;

    if (raise_exception != NULL) {
        int rc = raise_exception(header->unwind_exception, opaque);
        if (rc != 0)
            return rc;
    }

    if (terminate_fn != NULL)
        terminate_fn(opaque);
    return 0;
}

/* 0x001a9e40 -- __cxa_rethrow. */
int snes_p13___cxa_rethrow(SnesCxaGlobalsP13 *globals,
                           SnesEhRaiseFn raise_exception,
                           SnesEhObjectFn begin_catch,
                           SnesEhVoidFn terminate_fn,
                           void *opaque)
{
    SnesCxaExceptionP13 *header = globals != NULL ? globals->caught_exceptions : NULL;

    if (header != NULL) {
        header->handler_count = -header->handler_count;
        if (raise_exception != NULL)
            (void)raise_exception(header->unwind_exception, opaque);
        if (begin_catch != NULL)
            begin_catch(header->unwind_exception, opaque);
    }

    if (terminate_fn != NULL)
        terminate_fn(opaque);
    return 0;
}

/*
 * 0x001a9e88 -- operator new(size_t).  Zero becomes one byte; allocation is
 * retried after each installed new_handler.  If no handler exists, the target
 * constructs std::bad_alloc and throws it.  The host model exposes that final
 * action through throw_bad_alloc.
 */
void *snes_p13_operator_new(size_t size,
                            SnesEhAllocFn alloc_fn,
                            SnesEhVoidFn new_handler,
                            SnesEhThrowBadAllocFn throw_bad_alloc,
                            void *opaque)
{
    void *ptr;

    if (size == 0)
        size = 1;

    for (;;) {
        ptr = alloc_fn != NULL ? alloc_fn(size, opaque) : NULL;
        if (ptr != NULL)
            return ptr;

        if (new_handler == NULL) {
            if (throw_bad_alloc != NULL)
                throw_bad_alloc(opaque);
            return NULL;
        }
        new_handler(opaque);
    }
}
