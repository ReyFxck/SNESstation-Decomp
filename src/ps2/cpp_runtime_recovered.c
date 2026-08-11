/*
 * Small C++ runtime leaves recovered from SNES Station v0.23.
 *
 * These are kept as C-callable behavioral models because the public repository
 * is not yet a complete C++ rebuild.  The target symbols are Itanium ABI
 * _ZdlPv / _ZdaPv at 0x001a90f8 / 0x001a9118.
 */

#include <stddef.h>

typedef void (*SnesFreeFn)(void *ptr, void *opaque);

/* 0x001a90f8 — operator delete(void *): null is ignored, otherwise free(). */
void snes_operator_delete(void *ptr, SnesFreeFn free_fn, void *opaque)
{
    if (ptr != NULL && free_fn != NULL)
        free_fn(ptr, opaque);
}

/* 0x001a9118 — operator delete[](void *): exact forwarding leaf. */
void snes_operator_delete_array(void *ptr, SnesFreeFn free_fn, void *opaque)
{
    snes_operator_delete(ptr, free_fn, opaque);
}

/*
 * 0x001a9cf0..0x001a9d54 — tiny handler wrappers from eh_terminate.cc.
 * The actual target __terminate leaf has an EH landing pad so the handler-call
 * core stays mapped separately; these wrappers are exact state/dispatch leaves.
 */
typedef void (*SnesVoidHandler)(void);

void snes_std_terminate(SnesVoidHandler installed, SnesVoidHandler terminate_core)
{
    (void)installed;
    if (terminate_core != NULL)
        terminate_core();
}

void snes_unexpected_core(SnesVoidHandler handler, SnesVoidHandler terminate_fn)
{
    if (handler != NULL)
        handler();
    if (terminate_fn != NULL)
        terminate_fn();
}

void snes_std_unexpected(SnesVoidHandler installed, SnesVoidHandler unexpected_core)
{
    (void)installed;
    if (unexpected_core != NULL)
        unexpected_core();
}

SnesVoidHandler snes_set_terminate(SnesVoidHandler *slot, SnesVoidHandler replacement)
{
    SnesVoidHandler old = *slot;
    *slot = replacement;
    return old;
}

SnesVoidHandler snes_set_unexpected(SnesVoidHandler *slot, SnesVoidHandler replacement)
{
    SnesVoidHandler old = *slot;
    *slot = replacement;
    return old;
}

/* 0x001a9f68 — operator new[](size_t) forwards directly to operator new. */
typedef void *(*SnesNewFn)(size_t size, void *opaque);

void *snes_operator_new_array(size_t size, SnesNewFn new_fn, void *opaque)
{
    return new_fn != NULL ? new_fn(size, opaque) : NULL;
}
