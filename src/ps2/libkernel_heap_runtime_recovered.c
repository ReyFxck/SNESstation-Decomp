#include <stdint.h>
#include <string.h>
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

/* Heap block header layout matching the target binary. */
typedef struct heap_block_hdr {
    struct heap_block_hdr *next;   /* +0x00 */
    uint32_t size;                 /* +0x04 */
    struct heap_block_hdr *prev;   /* +0x08 */
    uint32_t end_addr;             /* +0x0c */
} heap_block_hdr;

/* Global heap state pointers at fixed addresses (0x425A74, 0x425A78, 0x425A7C). */
#if defined(__mips__)
static heap_block_hdr *volatile *const heap_base_ptr = (heap_block_hdr *volatile *)0x425A74;
static heap_block_hdr *volatile *const heap_end_ptr = (heap_block_hdr *volatile *)0x425A78;
static heap_block_hdr *volatile *const heap_tail_ptr = (heap_block_hdr *volatile *)0x425A7C;
#else
static heap_block_hdr *host_heap_base = NULL;
static heap_block_hdr *host_heap_end = NULL;
static heap_block_hdr *host_heap_tail = NULL;
static heap_block_hdr *volatile *const heap_base_ptr = &host_heap_base;
static heap_block_hdr *volatile *const heap_end_ptr = &host_heap_end;
static heap_block_hdr *volatile *const heap_tail_ptr = &host_heap_tail;
#endif

/* Forward declaration for mutex operations at 0x0019f078 / 0x0019f060. */
#if defined(__mips__)
extern int MallocMutexLock_0019f078(size_t size);
extern void MallocMutexUnlock_0019f060(int was_enabled);
#else
static int MallocMutexLock_0019f078(size_t size) { (void)size; return 0; }
static void MallocMutexUnlock_0019f060(int was_enabled) { (void)was_enabled; }
#endif

/* Target: 0x0019e4b4. Heap memory allocation (malloc). */
void *malloc_0019e4b4(size_t size)
{
#if defined(__mips__)
    register size_t v0 __asm__("$2") = size;
    register size_t v1 __asm__("$3");
    register size_t v2 __asm__("$4");
    register size_t v3 __asm__("$5");
    register size_t v4 __asm__("$6");
    register size_t v5 __asm__("$7");
    register size_t v6 __asm__("$8");

    __asm__ volatile(
        /* Prologue */
        "addiu $sp, $sp, -0x40\n"
        "sd    $16, 0x00($sp)\n"
        "sd    $17, 0x10($sp)\n"
        "sd    $18, 0x20($sp)\n"
        "sd    $ra, 0x30($sp)\n"

        /* Align size: add 0x10 for header, align to 16 bytes */
        "addiu %2, %0, 0x10\n"
        "andi  %3, %2, 0x0f\n"
        "beqz  %3, 1f\n"
        "nop\n"
        "addiu %4, %0, 0x1f\n"
        "lui   %3, 0xffff\n"
        "ori   %3, %3, 0xfff0\n"
        "and   %2, %4, %3\n"
        "1:\n"

        /* Load heap globals base (0x420000) */
        "lui   %6, 0x42\n"

        /* Check if heap initialized (heap_end_ptr == 0) */
        "lw    %3, 0x5a78(%6)\n"
        "bnez  %3, heap_initialized\n"
        "nop\n"

        /* Heap not initialized - initialize it */
        "lui   %5, 0x42\n"
        "lw    %4, 0x5a74(%5)\n"
        "beqz  %4, mutex_init_path\n"
        "nop\n"

        "heap_initialized:\n"
        /* Normal allocation path - search free list */
        "lw    %4, 0x5a74(%6)\n"
        "addu  %3, %4, %2\n"
        "sltu  %3, %3, %3\n"
        "beqz  %3, heap_extend\n"
        "nop\n"

        /* Call HeapFindBlock */
        "move  $4, %4\n"
        "move  $5, %2\n"
        "jal   HeapFindBlock_0019e474\n"
        "nop\n"
        "move  %4, $2\n"
        "beqz  %4, heap_extend\n"
        "nop\n"

        /* Found block - split and return */
        "lw    %3, 0x00(%4)\n"
        "lw    %5, 0x04(%4)\n"
        "addu  %3, %3, %5\n"
        "subu  %3, %2, %3\n"
        "sltu  %3, %3, %5\n"
        "bnez  %3, split_block\n"
        "nop\n"

        /* Block exact fit or too small - remove from list */
        "lw    %3, 0x00(%4)\n"
        "sw    %3, 0x5a78(%6)\n"
        "b     block_found\n"
        "nop\n"

        "split_block:\n"
        /* Split block */
        "addiu %3, %4, 0x10\n"
        "subu  %3, %3, %2\n"
        "sw    %3, 0x04(%4)\n"
        "sw    %4, 0x5a78(%6)\n"

        "block_found:\n"
        /* Set up returned block header */
        "addiu %0, %4, 0x10\n"
        "sw    %0, 0x00(%4)\n"
        "sw    %2, 0x04(%4)\n"
        "sw    $0, 0x08(%4)\n"
        "sw    $0, 0x0c(%4)\n"
        "b     malloc_exit\n"
        "nop\n"

        "mutex_init_path:\n"
        /* Initialize heap via mutex calls */
        "jal   0x19f078\n"
        "move  $4, $0\n"
        "jal   0x19f078\n"
        "andi  $4, $2, 0x0f\n"
        "jal   0x19f078\n"
        "move  $4, $0\n"
        "sw    $2, 0x5a74(%5)\n"
        "b     heap_initialized\n"
        "nop\n"

        "heap_extend:\n"
        /* Extend heap via EndOfHeap syscall */
        "jal   0x19f078\n"
        "move  $4, %2\n"
        "move  %5, $2\n"
        "addiu %4, $0, -1\n"
        "beq   %5, %4, malloc_exit\n"
        "move  %3, $0\n"

        "lui   %4, 0x42\n"
        "lw    %3, 0x5a7c(%4)\n"
        "addiu %4, %5, 0x10\n"
        "sw    $0, 0x0c(%5)\n"
        "move  %6, %5\n"
        "sw    %5, 0x0c(%3)\n"
        "sw    %4, 0x04(%5)\n"
        "sw    %3, 0x08(%5)\n"
        "sw    %5, 0x08(%3)\n"
        "b     block_found\n"
        "sw    %4, 0x0c(%3)\n"

        "malloc_exit:\n"
        /* Epilogue */
        "ld    $ra, 0x30($sp)\n"
        "ld    $18, 0x20($sp)\n"
        "ld    $17, 0x10($sp)\n"
        "ld    $16, 0x00($sp)\n"
        "addiu $sp, $sp, 0x40\n"
        "jr    $ra\n"
        : "=r"(v0), "+r"(v1), "+r"(v2), "+r"(v3), "+r"(v4), "+r"(v5), "+r"(v6)
        : "r"(v0)
        : "memory", "$1", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return (void *)v0;
#else
    /* Host fallback: simple malloc using host heap. */
    (void)size;
    return NULL;
#endif
}

/* Target: 0x0019c39c. Memory set (memset) - called by calloc. */
void *memset_0019c39c(void *dst, int c, size_t n)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = dst;
    register int v1 __asm__("$3") = c;
    register size_t v2 __asm__("$4") = n;
    register void *v3 __asm__("$5");
    register void *v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* 0x0019C39C - memset */
        "move   %3, %0       \n"  /* v3 = dst */
        "lui    %4, 0xffff   \n"  /* v4 = 0xffff0000 */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "ori    %4, %4, 0xffff\n" /* v4 = 0xffffffff */
        "beq    %2, %4, 2f   \n"  /* if (n == -1) goto end (2) */
        "nop                  \n"
        "lui    %4, 0xffff   \n"  /* v4 = 0xffff0000 */
        "ori    %4, %4, 0xffff\n" /* v4 = 0xffffffff */
        "1:                   \n"  /* loop start (1) */
        "sb     %1, 0(%3)    \n"  /* *v3 = c */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "bne    %2, %4, 1b   \n"  /* if (n != -1) goto loop (1) */
        "addiu  %3, %3, 1    \n"  /* v3++ */
        "2:                   \n"  /* end (2) */
        "jr     $ra          \n"  /* return */
        "move   %0, %3       \n"  /* v0 = v3 (dst) */
        : "+r"(v0), "+r"(v1), "+r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return memset(dst, c, n);
#endif
}

/* Target: 0x0019e648. Heap calloc - allocate and zero memory. */
void *calloc_0019e648(size_t nmemb, size_t size)
{
#if defined(__mips__)
    register size_t v0 __asm__("$2") = nmemb;
    register size_t v1 __asm__("$3") = size;
    register void *v2 __asm__("$4");
    register void *v3 __asm__("$5");
    register void *v4 __asm__("$6");
    register void *v5 __asm__("$7");

    __asm__ volatile(
        /* Prologue */
        "addiu $sp, $sp, -0x30\n"
        "sd    $16, 0x00($sp)\n"
        "sd    $17, 0x10($sp)\n"
        "sd    $ra, 0x20($sp)\n"

        /* Compute total size = nmemb * size (in $17) */
        "mul   %3, %0, %1\n"
        "move  %2, %3\n"

        /* Call malloc */
        "move  $4, %3\n"
        "jal   malloc_0019e4b4\n"
        "nop\n"
        "move  %2, $2\n"

        /* If malloc returned NULL, skip memset */
        "beqz  %2, calloc_exit\n"
        "nop\n"

        /* Call memset(ptr, 0, size) */
        "move  $4, %2\n"
        "move  $5, $0\n"
        "move  $6, %3\n"
        "jal   memset_0019c39c\n"
        "nop\n"

        "calloc_exit:\n"
        /* Epilogue */
        "ld    $ra, 0x20($sp)\n"
        "ld    $17, 0x10($sp)\n"
        "ld    $16, 0x00($sp)\n"
        "addiu $sp, $sp, 0x30\n"
        "jr    $ra\n"
        "move  %0, %2\n"
        : "=r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        : "r"(v0), "r"(v1)
        : "memory", "$1", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    void *ptr = malloc_0019e4b4(nmemb * size);
    if (ptr)
        memset_0019c39c(ptr, 0, nmemb * size);
    return ptr;
#endif
}

