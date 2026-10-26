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

/* Target: 0x0019e474. Heap block search for available space. */
void *HeapFindBlock_0019e474(void *head, size_t min_space)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = head;
    register void *v1 __asm__("$3") = head;
    register size_t v2 __asm__("$4") = (size_t)head;
    register size_t v3 __asm__("$5") = min_space;
    register void *v4 __asm__("$6") = 0;
    register void *v5 __asm__("$7");
    register size_t v6 __asm__("$8");

    __asm__ volatile(
        "1:\n"
        "beqz %2, 2f\n"
        "nop\n"
        "lw %4, 0xc(%2)\n"
        "beqz %4, 2f\n"
        "nop\n"
        "lw %5, 0x0(%2)\n"
        "lw %6, 0x4(%2)\n"
        "addu %5, %5, %6\n"
        "subu %5, %4, %5\n"
        "sltu %5, %5, %3\n"
        "beqz %5, 2f\n"
        "nop\n"
        "bnez %4, 1b\n"
        "move %2, %4\n"
        "2:\n"
        "jr $ra\n"
        "move %0, %2\n"
        : "=r"(v0), "+r"(v1), "+r"(v2), "+r"(v3), "=r"(v4), "=r"(v5), "=r"(v6)
        :
        : "memory"
    );
    return v0;
#else
    /* Host fallback: simple linear scan of a notional block list. */
    struct heap_block {
        struct heap_block *next;
        uint32_t start;
        uint32_t size;
    };
    struct heap_block *cur = (struct heap_block *)head;
    while (cur && cur->next) {
        uint32_t end = cur->start + cur->size;
        if ((uint32_t)cur->next - end >= min_space)
            return cur;
        cur = cur->next;
    }
    return cur;
#endif
}

