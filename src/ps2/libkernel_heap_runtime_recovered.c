#include <stdint.h>
#include "../../include/ps2_libkernel_recovered.h"

/*
 * EE interrupt/heap helpers immediately following the small libc block.
 * These functions intentionally model the old R5900/libkernel behaviour,
 * including the target's 32-bit program-break arithmetic.
 */

#if defined(__mips__)
static uint32_t ee_status_recovered(void)
{
    uint32_t status;
    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    return status;
}
#else
static int host_interrupt_enabled = 1;
static uint32_t ee_status_recovered(void)
{
    return host_interrupt_enabled ? UINT32_C(0x00010000) : 0u;
}
#endif

/* Target: 0x0019f018. Returns whether interrupts had been enabled. */
int DIntr(void)
{
    int was_enabled = (ee_status_recovered() & UINT32_C(0x00010000)) != 0u;

    if (!was_enabled)
        return 0;

#if defined(__mips__)
    do {
        __asm__ volatile("di\n\tsync" ::: "memory");
    } while ((ee_status_recovered() & UINT32_C(0x00010000)) != 0u);
#else
    host_interrupt_enabled = 0;
#endif
    return 1;
}

/* Target: 0x0019f060. EI is executed unconditionally. */
int EIntr(void)
{
    int was_enabled = (ee_status_recovered() & UINT32_C(0x00010000)) != 0u;
#if defined(__mips__)
    __asm__ volatile("ei" ::: "memory");
#else
    host_interrupt_enabled = 1;
#endif
    return was_enabled;
}

/* Target: 0x0019f5c0, syscall 0x3e. */
ee_addr32_t EndOfHeap_0019f5c0(void)
{
#if defined(__mips__)
    register uint32_t v0 __asm__("$2");
    register uint32_t v1 __asm__("$3") = 0x3eu;
    __asm__ volatile("syscall" : "=r"(v0), "+r"(v1) : : "memory");
    return v0;
#else
    /* Host validation has no EE linker heap ceiling. */
    return UINT32_MAX;
#endif
}

