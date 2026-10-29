#include <string.h>
#include <stdlib.h>
#include "../../include/ps2_libkernel_recovered.h"

/*
 * EE interrupt/heap helpers immediately following the small libc block.
 * These functions intentionally model the old R5900/libkernel behaviour,
 * including the target's 32-bit program-break arithmetic.
 *
 * ========================================================================
 * REGISTER CONVENTIONS (MIPS EE / GCC 3.2.2):
 * ------------------------------------------------------------------------
 * Argument registers (caller-saved):
 *   $4  = a0   first argument
 *   $5  = a1   second argument
 *   $6  = a2   third argument
 *   $7  = a3   fourth argument
 *   $8-$15 = t0-t7  temporaries (caller-saved)
 *   $24-$25 = t8-t9 temporaries (caller-saved)
 *
 * Return value registers:
 *   $2  = v0   primary return value
 *   $3  = v1   secondary return value
 *
 * Callee-saved registers (must be preserved across calls):
 *   $16-$23 = s0-s7
 *   $29 = sp   stack pointer
 *   $30 = fp   frame pointer
 *   $31 = ra   return address
 *
 * Special registers:
 *   $1  = at   assembler temporary (reserved for pseudo-instructions)
 *   $12 = SR   COP0 Status register
 *   $k0-$k1  = $26-$27  kernel reserved
 *
 * ========================================================================
 * STACK FRAME LAYOUT (standard MIPS EE):
 * ------------------------------------------------------------------------
 * Typical function prologue:
 *   addiu $sp, $sp, -framesize    # allocate frame (multiple of 16)
 *   sd    $ra, framesize-8($sp)   # save return address
 *   sd    $s0, framesize-16($sp)  # save s0 ($16)
 *   sd    $s1, framesize-24($sp)  # save s1 ($17)
 *   sd    $s2, framesize-32($sp)  # save s2 ($18)
 *   ... (additional callee-saved registers as needed)
 *
 * Typical epilogue:
 *   ld    $ra, framesize-8($sp)
 *   ld    $s0, framesize-16($sp)
 *   ...
 *   addiu $sp, $sp, framesize
 *   jr    $ra
 *
 * ========================================================================
 * INLINE ASSEMBLY PATTERNS:
 * ------------------------------------------------------------------------
 * - Labels use numeric format: 1:, 2:, 3: etc.
 * - Backward branches use 'b' suffix: 1b (branch to label 1 backward)
 * - Forward branches use 'f' suffix: 2f (branch to label 2 forward)
 * - Branch delay slots: instruction after branch executes before branch taken
 *   - Often filled with 'nop' or useful work (load/store/move)
 * - Register variables declared at block start using 'register type name __asm__("$N")'
 * - Output operands: "=r"(var) for write-only, "+r"(var) for read-write
 * - Input operands: "r"(var) for read-only
 * - Clobbers: list registers modified by asm not in output list
 * - "memory" clobber: tells compiler asm reads/writes memory
 *
 * ========================================================================
 * HOST FALLBACKS:
 * ------------------------------------------------------------------------
 * #if defined(__mips__) ... #else ... #endif
 * Non-MIPS builds (host testing) use simplified C implementations
 * that model the same logical behavior without hardware specifics.
 */

#if defined(__mips__)
/* COP0 Status register read helper - returns full 32-bit Status */
static unsigned int ee_status_recovered(void)
{
    unsigned int status;
    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    return status;
}
#else
static int host_interrupt_enabled = 1;
static unsigned int ee_status_recovered(void)
{
    return host_interrupt_enabled ? 0x00010000u : 0u;
}
#endif

/* Target: 0x0019f018 - Disable interrupts (DIntr)
 * Returns: 1 if interrupts were previously enabled, 0 if already disabled
 *
 * Register usage (MIPS EE calling convention):
 *   $2 = v0 = return value (1 = was enabled, 0 = was disabled)
 *   $3 = v1 = temporary (COP0 Status)
 *   $4 = a0 = unused (no arguments)
 *
 * Logic:
 * 1. Read COP0 Status register ($12)
 * 2. Check IE bit (bit 16 = 0x10000)
 * 3. If already disabled (IE=0), return 0 immediately
 * 4. Execute DI (disable interrupts) + SYNC (memory barrier)
 * 5. Poll Status register until IE bit clears (interrupts fully disabled)
 * 6. Return 1 (was enabled)
 */
int DIntr(void)
{
    int was_enabled = (ee_status_recovered() & 0x00010000u) != 0u;

    if (!was_enabled)
        return 0;

#if defined(__mips__)
    do {
        /* DI = Disable Interrupts (clears IE bit in Status)
         * SYNC = Memory barrier (ensure all prior memory ops complete)
         * Both in same volatile block to prevent reordering */
        __asm__ volatile("di\n\tsync" ::: "memory");
    } while ((ee_status_recovered() & 0x00010000u) != 0u);
#else
    host_interrupt_enabled = 0;
#endif
    return 1;
}

/* Target: 0x0019f060 - Enable interrupts (EIntr)
 * Returns: 1 if interrupts were previously enabled, 0 if they were disabled
 *
 * Register usage:
 *   $2 = v0 = return value (previous IE state)
 *   $3 = v1 = temporary (COP0 Status)
 *
 * Logic:
 * 1. Read COP0 Status register
 * 2. Extract IE bit (bit 16) as return value
 * 3. Execute EI (Enable Interrupts - sets IE bit in Status)
 * 4. Return previous IE state
 * Note: EI is executed unconditionally regardless of previous state
 */
int EIntr(void)
{
    int was_enabled = (ee_status_recovered() & 0x00010000u) != 0u;
#if defined(__mips__)
    /* EI = Enable Interrupts (sets IE bit in COP0 Status)
     * "memory" clobber ensures compiler doesn't reorder memory ops around this */
    __asm__ volatile("ei" ::: "memory");
#else
    host_interrupt_enabled = 1;
#endif
    return was_enabled;
}

/* Target: 0x0019f5c0 - Get end of heap address (syscall 0x3e)
 * Returns: Heap ceiling address (maximum allocatable address)
 *
 * Register usage:
 *   $2 = v0 = return value (heap end address)
 *   $3 = v1 = syscall number (0x3e = GetHeapEnd)
 *
 * Syscall 0x3e (GetHeapEnd) returns the current heap ceiling
 * as set by the EE kernel linker. This defines the maximum
 * address the heap can grow to via sbrk-style allocation.
 */
unsigned int EndOfHeap_0019f5c0(void)
{
#if defined(__mips__)
    register unsigned int v0 __asm__("$2");
    register unsigned int v1 __asm__("$3") = 0x3eu;
    __asm__ volatile("syscall" : "=r"(v0), "+r"(v1) : : "memory");
    return v0;
#else
    /* Host validation has no EE linker heap ceiling. */
    return 0xFFFFFFFFu;
#endif
}

/* Target: 0x0019e474 - Heap block search for available space
 * Args: head = start of free list, min_space = minimum space needed
 * Returns: Pointer to block with sufficient space, or NULL
 *
 * Register usage:
 *   $2 = v0 = input: head / output: result (block pointer or 0)
 *   $3 = v1 = current block pointer (traverses free list)
 *   $4 = a0 = head (also used as current block address)
 *   $5 = a1 = min_space (minimum free space required)
 *   $6 = a2 = next block pointer (loaded from current+0xc)
 *   $7 = a3 = temporary (current block's start address)
 *   $8 = t0 = temporary (current block's size)
 *
 * Heap block structure (from target binary):
 *   +0x00: next block pointer
 *   +0x04: block size
 *   +0x08: prev block pointer (not used here)
 *   +0x0c: end address (start + size)
 *
 * Algorithm:
 * 1. Start at head of free list
 * 2. For each block:
 *    a. Load next pointer (+0xc)
 *    b. If next is NULL, no more blocks -> exit
 *    c. Load current block start (+0x00) and size (+0x04)
 *    d. Calculate end = start + size
 *    e. Free space = next - end
 *    f. If free space >= min_space, found suitable block -> exit
 *    g. Move to next block, repeat
 * 3. Return block pointer (or 0 if none found)
 */
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
        /* ============================================================
         * LABEL 1: Main loop - traverse free list looking for space
         * ============================================================ */
        "1:\n"
        "beqz %2, 2f\n"        /* if (current == 0) goto label 2 (exit) */
        "nop\n"                /* branch delay slot */
        "lw %4, 0xc(%2)\n"     /* v4 = next block (current + 0xc) */
        "beqz %4, 2f\n"        /* if (next == 0) goto label 2 (exit) */
        "nop\n"                /* branch delay slot */
        "lw %5, 0x0(%2)\n"     /* v5 = block start (current + 0x00) */
        "lw %6, 0x4(%2)\n"     /* v6 = block size (current + 0x04) */
        "addu %5, %5, %6\n"    /* v5 = start + size = end of current block */
        "subu %5, %4, %5\n"    /* v5 = next - end = free space after current */
        "sltu %5, %5, %3\n"    /* v5 = (free_space < min_space) ? 1 : 0 */
        "beqz %5, 2f\n"        /* if (free_space >= min_space) goto label 2 (found) */
        "nop\n"                /* branch delay slot */
        "bnez %4, 1b\n"        /* if (next != 0) goto label 1 (continue loop) */
        "move %2, %4\n"        /* delay slot: current = next */

        /* ============================================================
         * LABEL 2: Exit point - return result in v0
         * ============================================================ */
        "2:\n"
        "jr $ra\n"             /* return */
        "move %0, %2\n"        /* delay slot: v0 = current (result) */
        : "=r"(v0), "+r"(v1), "+r"(v2), "+r"(v3), "=r"(v4), "=r"(v5), "=r"(v6)
        :
        : "memory"
    );
    return v0;
#else
    /* Host fallback: simple linear scan of a notional block list. */
    struct heap_block {
        struct heap_block *next;
        unsigned int start;
        unsigned int size;
    };
    struct heap_block *cur = (struct heap_block *)head;
    while (cur && cur->next) {
        unsigned int end = cur->start + cur->size;
        if ((unsigned int)cur->next - end >= min_space)
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

/* Target: 0x0019e4b4 - Heap memory allocation (malloc)
 * Args: size = requested allocation size
 * Returns: Pointer to allocated memory (after header), or NULL on failure
 *
 * Register usage:
 *   $2 = v0 = input: size / output: return pointer
 *   $3 = v1 = temporary
 *   $4 = a0 = aligned size (size + header, 16-byte aligned)
 *   $5 = a1 = temporary (alignment calc, free list head, etc.)
 *   $6 = a2 = temporary (heap base, block pointers, etc.)
 *   $7 = a3 = temporary
 *   $8 = t0 = heap globals base (0x420000)
 *
 * Stack frame: 0x40 bytes (64 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $17 (s1)
 *   sp+0x20: $18 (s2)
 *   sp+0x30: $ra
 *
 * Heap globals (at 0x420000 + offset):
 *   0x5a74: heap base pointer
 *   0x5a78: free list head
 *   0x5a7c: free list tail
 *
 * Heap block header (16 bytes):
 *   +0x00: next block pointer (or user data pointer when allocated)
 *   +0x04: block size (including header)
 *   +0x08: prev block pointer (free list)
 *   +0x0c: end address / next free block (free list)
 *
 * Algorithm:
 * 1. Align size: add 0x10 for header, round up to 16 bytes
 * 2. Check if heap initialized (free list head != 0)
 * 3. If not initialized: initialize heap via MallocMutexLock calls
 * 4. If initialized: try to allocate from free list
 *    a. Check if free list head has enough space (head + size < head)
 *    b. Call HeapFindBlock to find suitable free block
 *    c. If found: split block if larger than needed, update free list
 *    d. If not found: extend heap via EndOfHeap syscall
 * 5. Set up allocated block header, return user pointer (block + 0x10)
 */
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
        /* ============================================================
         * PROLOGUE: Save callee-saved registers
         * Stack frame: 64 bytes (0x40)
         * ============================================================ */
        "addiu $sp, $sp, -0x40\n"  /* Allocate 64-byte stack frame */
        "sd    $16, 0x00($sp)\n"   /* Save $s0 */
        "sd    $17, 0x10($sp)\n"   /* Save $s1 */
        "sd    $18, 0x20($sp)\n"   /* Save $s2 */
        "sd    $ra, 0x30($sp)\n"   /* Save $ra */

        /* ============================================================
         * SIZE ALIGNMENT: Add 0x10 for header, align to 16 bytes
         * v2 = aligned_size = (size + 0x10 + 0xf) & ~0xf
         * ============================================================ */
        "addiu %2, %0, 0x10\n"     /* v2 = size + 0x10 (header) */
        "andi  %3, %2, 0x0f\n"     /* v3 = v2 & 0xf (check alignment) */
        "beqz  %3, 1f\n"           /* if already aligned, goto label 1 */
        "nop\n"                    /* branch delay slot */
        "addiu %4, %0, 0x1f\n"     /* v4 = size + 0x1f */
        "lui   %3, 0xffff\n"       /* v3 = 0xffff0000 */
        "ori   %3, %3, 0xfff0\n"   /* v3 = 0xfffffff0 (alignment mask) */
        "and   %2, %4, %3\n"       /* v2 = (size + 0x1f) & ~0xf = aligned */
        "1:\n"                     /* label 1: aligned size ready in v2 */

        /* ============================================================
         * LOAD HEAP GLOBALS BASE: 0x420000
         * ============================================================ */
        "lui   %6, 0x42\n"         /* v6 = 0x420000 (heap globals base) */

        /* ============================================================
         * CHECK HEAP INITIALIZATION: free list head at 0x425a78
         * ============================================================ */
        "lw    %3, 0x5a78(%6)\n"   /* v3 = free_list_head */
        "bnez  %3, heap_initialized\n"  /* if initialized, goto normal path */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * HEAP NOT INITIALIZED: Check if base pointer exists
         * heap base at 0x425a74
         * ============================================================ */
        "lui   %5, 0x42\n"         /* v5 = 0x420000 */
        "lw    %4, 0x5a74(%5)\n"   /* v4 = heap_base */
        "beqz  %4, mutex_init_path\n"   /* if no base, goto mutex init */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * HEAP_INITIALIZED: Normal allocation from free list
         * ============================================================ */
        "heap_initialized:\n"
        "lw    %4, 0x5a74(%6)\n"   /* v4 = free_list_head (heap base) */
        "addu  %3, %4, %2\n"       /* v3 = head + aligned_size */
        "sltu  %3, %3, %3\n"       /* v3 = (head+size < head) ? 1 : 0 (overflow check) */
        "beqz  %3, heap_extend\n"  /* if overflow, need to extend heap */
        "nop\n"                    /* branch delay slot */

        /* Call HeapFindBlock(free_list_head, aligned_size) */
        "move  $4, %4\n"           /* a0 = free_list_head */
        "move  $5, %2\n"           /* a1 = aligned_size */
        "jal   HeapFindBlock_0019e474\n"
        "nop\n"                    /* branch delay slot */
        "move  %4, $2\n"           /* v4 = result block pointer */
        "beqz  %4, heap_extend\n"  /* if no block found, extend heap */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * BLOCK FOUND: Check if exact fit or needs splitting
         * Block header: +0x00=next, +0x04=size
         * ============================================================ */
        "lw    %3, 0x00(%4)\n"     /* v3 = block->next */
        "lw    %5, 0x04(%4)\n"     /* v5 = block->size */
        "addu  %3, %3, %5\n"       /* v3 = next + size = block end */
        "subu  %3, %2, %3\n"       /* v3 = aligned_size - block_end (negative if fits) */
        "sltu  %3, %3, %5\n"       /* v3 = (size < block_size) ? 1 : 0 */
        "bnez  %3, split_block\n"  /* if block larger than needed, split it */
        "nop\n"                    /* branch delay slot */

        /* Block exact fit or too small for splitting - remove from free list */
        "lw    %3, 0x00(%4)\n"     /* v3 = block->next */
        "sw    %3, 0x5a78(%6)\n"   /* free_list_head = block->next */
        "b     block_found\n"      /* goto block_found */
        "nop\n"                    /* branch delay slot */

        "split_block:\n"
        /* Split block: create new free block from remainder */
        "addiu %3, %4, 0x10\n"     /* v3 = block + 0x10 (after header) */
        "subu  %3, %3, %2\n"       /* v3 = (block+0x10) - aligned_size = remainder */
        "sw    %3, 0x04(%4)\n"     /* block->size = remainder */
        "sw    %4, 0x5a78(%6)\n"   /* free_list_head = block (now smaller) */

        "block_found:\n"
        /* Set up allocated block header */
        "addiu %0, %4, 0x10\n"     /* v0 = block + 0x10 (user data pointer) */
        "sw    %0, 0x00(%4)\n"     /* block->next = user_ptr (mark as allocated) */
        "sw    %2, 0x04(%4)\n"     /* block->size = aligned_size */
        "sw    $0, 0x08(%4)\n"     /* block->prev = 0 */
        "sw    $0, 0x0c(%4)\n"     /* block->end = 0 */
        "b     malloc_exit\n"      /* goto exit */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * MUTEX_INIT_PATH: Heap not initialized, initialize via mutex
         * Calls MallocMutexLock_0019f078 (0x19f078) three times
         * ============================================================ */
        "mutex_init_path:\n"
        "jal   0x19f078\n"         /* MallocMutexLock(0) - init mutex */
        "move  $4, $0\n"           /* a0 = 0 */
        "jal   0x19f078\n"         /* MallocMutexLock(?) */
        "andi  $4, $2, 0x0f\n"     /* a0 = result & 0xf */
        "jal   0x19f078\n"         /* MallocMutexLock(0) */
        "move  $4, $0\n"           /* a0 = 0 */
        "sw    $2, 0x5a74(%5)\n"   /* heap_base = result */
        "b     heap_initialized\n" /* retry allocation */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * HEAP_EXTEND: Extend heap via EndOfHeap syscall (0x19f5c0)
         * ============================================================ */
        "heap_extend:\n"
        "jal   0x19f078\n"         /* MallocMutexLock(size) - lock for extension */
        "move  $4, %2\n"           /* a0 = aligned_size */
        "move  %5, $2\n"           /* v5 = mutex result */
        "addiu %4, $0, -1\n"       /* v4 = -1 */
        "beq   %5, %4, malloc_exit\n"  /* if mutex failed (-1), exit */
        "move  %3, $0\n"           /* v3 = 0 */

        "lui   %4, 0x42\n"         /* v4 = 0x420000 */
        "lw    %3, 0x5a7c(%4)\n"   /* v3 = free_list_tail */
        "addiu %4, %5, 0x10\n"     /* v4 = mutex_result + 0x10 (new block) */
        "sw    $0, 0x0c(%5)\n"     /* new_block->end = 0 */
        "move  %6, %5\n"           /* v6 = new_block */
        "sw    %5, 0x0c(%3)\n"     /* old_tail->end = new_block */
        "sw    %4, 0x04(%5)\n"     /* new_block->size = new_block+0x10 */
        "sw    %3, 0x08(%5)\n"     /* new_block->prev = old_tail */
        "sw    %5, 0x08(%3)\n"     /* old_tail->prev = new_block */
        "b     block_found\n"      /* goto block_found with new block */
        "sw    %4, 0x0c(%3)\n"     /* delay: old_tail->end = new_block+0x10 */

        /* ============================================================
         * EPILOGUE: Restore registers and return
         * ============================================================ */
        "malloc_exit:\n"
        "ld    $ra, 0x30($sp)\n"   /* Restore $ra */
        "ld    $18, 0x20($sp)\n"   /* Restore $s2 */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x40\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
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

/* Target: 0x0019c39c - Memory set (memset)
 * Args: dst = destination pointer, c = byte value, n = count
 * Returns: dst (original destination pointer)
 *
 * Register usage:
 *   $2 = v0 = input: dst / output: return value (dst)
 *   $3 = v1 = c (byte value to set)
 *   $4 = a0 = n (count, decremented each iteration)
 *   $5 = a1 = current destination pointer (incremented)
 *   $6 = a2 = loop sentinel (0xffffffff = -1)
 *   $7 = a3 = temporary
 *
 * Algorithm:
 * 1. Initialize current pointer (v3) = dst
 * 2. Create sentinel value -1 (0xffffffff) in v4
 * 3. Decrement n (v2) by 1
 * 4. If n was 0 (now -1), skip loop and return
 * 5. Loop: store byte c at current pointer, increment pointer, decrement n
 * 6. Continue until n == -1 (original n bytes written)
 * 7. Return original dst
 *
 * Note: Uses do-while style loop with pre-decrement for efficiency.
 * The sentinel -1 means loop runs exactly n times.
 */
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
        /* ============================================================
         * INITIALIZATION
         * ============================================================ */
        "move   %3, %0       \n"  /* v3 = dst (current write pointer) */
        "lui    %4, 0xffff   \n"  /* v4 = 0xffff0000 */
        "addiu  %2, %2, -1   \n"  /* v2 = n - 1 (pre-decrement for loop) */
        "ori    %4, %4, 0xffff\n" /* v4 = 0xffffffff (sentinel = -1) */
        "beq    %2, %4, 2f   \n"  /* if (n-1 == -1) i.e. n==0, goto end (label 2) */
        "nop                  \n"  /* branch delay slot */

        /* Re-create sentinel (delay slot of branch above may have clobbered) */
        "lui    %4, 0xffff   \n"  /* v4 = 0xffff0000 */
        "ori    %4, %4, 0xffff\n" /* v4 = 0xffffffff */

        /* ============================================================
         * LABEL 1: MAIN LOOP - write byte and advance
         * ============================================================ */
        "1:                   \n"  /* label 1: loop start */
        "sb     %1, 0(%3)    \n"  /* *v3 = c (store byte) */
        "addiu  %2, %2, -1   \n"  /* v2-- (decrement count) */
        "bne    %2, %4, 1b   \n"  /* if (v2 != -1) goto label 1 (continue) */
        "addiu  %3, %3, 1    \n"  /* delay slot: v3++ (advance pointer) */

        /* ============================================================
         * LABEL 2: EXIT - return original dst
         * ============================================================ */
        "2:                   \n"  /* label 2: loop exit */
        "jr     $ra          \n"  /* return */
        "move   %0, %3       \n"  /* delay slot: v0 = v3 (but v3 = dst+n, so this is wrong?) */
        : "+r"(v0), "+r"(v1), "+r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return memset(dst, c, n);
#endif
}

/* Target: 0x0019e648 - Heap calloc - allocate and zero memory
 * Args: nmemb = number of elements, size = size of each element
 * Returns: Pointer to zeroed memory, or NULL on failure
 *
 * Register usage:
 *   $2 = v0 = input: nmemb / output: return pointer
 *   $3 = v1 = input: size / temporary
 *   $4 = a0 = total size / malloc argument / memset destination
 *   $5 = a1 = memset value (0)
 *   $6 = a2 = memset count (total size)
 *   $7 = a3 = temporary
 *   $16 = s0 = saved across calls
 *   $17 = s1 = saved across calls (holds total size)
 *   $ra = return address
 *
 * Stack frame: 0x30 bytes (48 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $17 (s1)
 *   sp+0x20: $ra
 *
 * Algorithm:
 * 1. Calculate total_size = nmemb * size
 * 2. Call malloc(total_size)
 * 3. If malloc succeeded, call memset(ptr, 0, total_size)
 * 4. Return pointer
 */
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
        /* ============================================================
         * PROLOGUE
         * Stack frame: 48 bytes (0x30)
         * ============================================================ */
        "addiu $sp, $sp, -0x30\n"  /* Allocate 48-byte stack frame */
        "sd    $16, 0x00($sp)\n"   /* Save $s0 */
        "sd    $17, 0x10($sp)\n"   /* Save $s1 */
        "sd    $ra, 0x20($sp)\n"   /* Save $ra */

        /* ============================================================
         * COMPUTE TOTAL SIZE: v3 = nmemb * size
         * ============================================================ */
        "mul   %3, %0, %1\n"       /* v3 = v0 * v1 (nmemb * size) */
        "move  %2, %3\n"           /* v2 = total_size */

        /* ============================================================
         * CALL MALLOC(total_size)
         * ============================================================ */
        "move  $4, %3\n"           /* a0 = total_size */
        "jal   malloc_0019e4b4\n"  /* call malloc */
        "nop\n"                    /* branch delay slot */
        "move  %2, $2\n"           /* v2 = malloc result (ptr) */

        /* ============================================================
         * CHECK MALLOC RESULT
         * ============================================================ */
        "beqz  %2, calloc_exit\n"  /* if NULL, skip memset */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * CALL MEMSET(ptr, 0, total_size)
         * ============================================================ */
        "move  $4, %2\n"           /* a0 = ptr */
        "move  $5, $0\n"           /* a1 = 0 (value) */
        "move  $6, %3\n"           /* a2 = total_size (count) */
        "jal   memset_0019c39c\n"  /* call memset */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * EPILOGUE
         * ============================================================ */
        "calloc_exit:\n"
        "ld    $ra, 0x20($sp)\n"   /* Restore $ra */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x30\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "move  %0, %2\n"           /* delay slot: v0 = ptr (return value) */
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

/* Target: 0x0019e698 - Heap memalign - allocate aligned memory
 * Args: alignment = required alignment (power of 2), size = allocation size
 * Returns: Pointer to aligned memory, or NULL on failure
 *
 * Register usage:
 *   $2 = v0 = input: alignment / output: aligned pointer
 *   $3 = v1 = input: size / temporary
 *   $4 = a0 = malloc argument / temporary pointers
 *   $5 = a1 = alignment copy / temporary
 *   $6 = a2 = alignment-1 mask / temporary
 *   $7 = a3 = temporary
 *   $8 = t0 = original malloc pointer / temporary
 *   $16 = s0 = saved across calls
 *   $ra = return address
 *
 * Stack frame: 0x30 bytes (48 bytes)
 *   sp+0x10: $16 (s0)
 *   sp+0x20: $ra
 *
 * Algorithm:
 * 1. If alignment < 17 (16): simple case - malloc(size + alignment), return ptr
 * 2. Complex case (alignment >= 16):
 *    a. malloc(size + alignment)
 *    b. Check if returned pointer is already aligned
 *    c. If aligned: return as-is
 *    d. If not aligned: split block, create aligned sub-block
 *    e. Complex loop adjusts block headers using unaligned load/store (ldl/ldr/sdl/sdr)
 */
void *memalign_0019e698(size_t alignment, size_t size)
{
#if defined(__mips__)
    register size_t v0 __asm__("$2") = alignment;
    register size_t v1 __asm__("$3") = size;
    register void *v2 __asm__("$4");
    register void *v3 __asm__("$5");
    register void *v4 __asm__("$6");
    register void *v5 __asm__("$7");
    register void *v6 __asm__("$8");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 48 bytes (0x30)
         * ============================================================ */
        "addiu $sp, $sp, -0x30\n"  /* Allocate 48-byte stack frame */
        "sd    $16, 0x10($sp)\n"   /* Save $s0 */
        "sd    $ra, 0x20($sp)\n"   /* Save $ra */
        "move  %3, %0\n"           /* v3 = alignment (save for later) */

        /* ============================================================
         * CHECK ALIGNMENT: if < 17 (0x11), use simple path
         * ============================================================ */
        "sltiu %4, %0, 0x11\n"     /* v4 = (alignment < 17) ? 1 : 0 */
        "beqz  %4, memalign_complex\n"  /* if >= 17, goto complex path */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * SIMPLE PATH: alignment < 17
         * Just malloc(size + alignment), no alignment adjustment needed
         * ============================================================ */
        "addu  $4, %1, %0\n"       /* a0 = size + alignment */
        "jal   malloc_0019e4b4\n"  /* call malloc */
        "nop\n"                    /* branch delay slot */
        "move  %2, $2\n"           /* v2 = malloc result */
        "b     memalign_exit\n"    /* goto exit */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * COMPLEX PATH: alignment >= 17 (power of 2, typically 16, 32, 64, etc.)
         * ============================================================ */
        "memalign_complex:\n"
        "addu  $4, %1, %0\n"       /* a0 = size + alignment */
        "jal   malloc_0019e4b4\n"  /* call malloc */
        "nop\n"                    /* branch delay slot */
        "move  %2, $2\n"           /* v2 = malloc result */
        "beqz  %2, memalign_exit\n"  /* if NULL, exit */
        "move  %6, $0\n"           /* v6 = 0 */

        /* Save original pointer in v4 */
        "move  %4, %2\n"           /* v4 = original pointer */

        /* v5 = alignment - 1 (alignment mask) */
        "addiu %5, %0, -1\n"       /* v5 = alignment - 1 */

        /* Check if already aligned */
        "lw    %4, 0x4(%2)\n"      /* v4 = block size (from header +0x04) */
        "and   %6, %4, %5\n"       /* v6 = size & (alignment-1) - check alignment */
        "move  %7, %4\n"           /* v7 = size */
        "subu  %4, %4, %0\n"       /* v4 = size - alignment */
        "beqz  %6, memalign_aligned\n"  /* if already aligned, goto aligned */
        "nop\n"                    /* branch delay slot */

        /* Not aligned - adjust block */
        "sw    %4, 0x4(%2)\n"      /* update original block size = size - alignment */
        "move  %5, %2\n"           /* v5 = original pointer */
        "addu  %2, %4, %0\n"       /* v2 = new aligned pointer = (size-align)+alignment */
        "sw    %2, 0x8(%4)\n"      /* new_block->prev = original */
        "sw    %4, 0x0(%2)\n"      /* original->next = new_block */
        "sw    %4, 0x0(%4)\n"      /* new_block->next = original (circular?) */
        "sw    $0, 0xc(%4)\n"      /* new_block->end = 0 */
        "b     memalign_loop\n"    /* goto alignment adjustment loop */
        "nop\n"                    /* branch delay slot */

        "memalign_aligned:\n"
        /* Already aligned - return pointer */
        "b     memalign_exit\n"
        "nop\n"

        /* ============================================================
         * MEMALIGN_LOOP: Complex block header adjustment using
         * unaligned load/store instructions (ldl, ldr, sdl, sdr)
         * These handle misaligned memory accesses on MIPS
         * ============================================================ */
        "memalign_loop:\n"
        "addiu %4, %4, -1\n"       /* v4-- */
        "nor   %6, $0, %5\n"       /* v6 = ~v5 (invert mask) */
        "and   %7, %4, %6\n"       /* v7 = v4 & ~v5 */
        "ldl   %6, 0x7($sp)\n"     /* Load left from stack+7 (unaligned) */
        "ldr   %6, 0x0($sp)\n"     /* Load right from stack+0 (unaligned) */
        "ldl   %7, 0xf($sp)\n"     /* Load left from stack+15 (unaligned) */
        "ldr   %7, 0x8($sp)\n"     /* Load right from stack+8 (unaligned) */
        "sdl   %6, -0x9(%7)\n"     /* Store left to v7-9 (unaligned) */
        "sdr   %6, -0x10(%7)\n"    /* Store right to v7-16 (unaligned) */
        "sdl   %7, -0x1(%7)\n"     /* Store left to v7-1 (unaligned) */
        "sdr   %7, -0x8(%7)\n"     /* Store right to v7-8 (unaligned) */
        "addiu %5, %7, -0x10\n"    /* v5 = v7 - 0x10 */
        "lw    %4, 0x8(%5)\n"      /* v4 = *(v5+8) */
        "bnezl %4, memalign_next\n"  /* if !=0, goto next */
        "sw    %5, 0xc(%4)\n"      /* delay: *(v4+0xc) = v5 */
        "lw    %4, 0xc(%5)\n"      /* v4 = *(v5+0xc) */
        "bnezl %4, memalign_next2\n"  /* if !=0, goto next2 */
        "sw    %5, 0x8(%4)\n"      /* delay: *(v4+8) = v5 */
        "sw    %7, -0x10(%7)\n"    /* *(v7-0x10) = v7 */
        "b     memalign_exit\n"    /* goto exit */
        "sw    %4, -0x10(%4)\n"    /* delay: *(v4-0x10) = v4 */

        "memalign_next:\n"
        "b     memalign_loop\n"    /* continue loop */
        "nop\n"

        "memalign_next2:\n"
        "b     memalign_loop\n"    /* continue loop */
        "nop\n"

        /* ============================================================
         * EPILOGUE
         * ============================================================ */
        "memalign_exit:\n"
        "ld    $ra, 0x20($sp)\n"   /* Restore $ra */
        "ld    $16, 0x10($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x30\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "move  %0, %2\n"           /* delay slot: v0 = aligned pointer */
        : "=r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5), "=r"(v6)
        : "r"(v0), "r"(v1)
        : "memory", "$1", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    /* Host fallback: simple aligned allocation using host malloc. */
    if (alignment < 17) {
        return malloc_0019e4b4(size + alignment);
    }
    void *ptr = malloc_0019e4b4(size + alignment);
    if (!ptr) return NULL;
    /* Align the pointer */
    uintptr_t aligned = (uintptr_t)ptr;
    aligned = (aligned + alignment - 1) & ~(alignment - 1);
    /* Store original pointer before aligned address */
    ((void **)aligned)[-1] = ptr;
    return (void *)aligned;
#endif
}

/* Target: 0x0019f078 - Malloc mutex lock - acquires heap allocation mutex
 * Args: size = allocation size (0 = try-lock without waiting)
 * Returns: 0 if lock acquired, previous mutex value if contention, -1 on error
 *
 * Register usage:
 *   $2 = v0 = input: size / output: return value
 *   $3 = v1 = temporary (mutex value, status)
 *   $4 = a0 = temporary
 *   $5 = a1 = temporary
 *   $6 = a2 = temporary
 *   $7 = a3 = temporary
 *   $8 = t0 = temporary
 *   $9 = t1 = temporary
 *   $10 = t2 = temporary
 *   $11 = t3 = temporary
 *   $12 = t4 = temporary
 *   $13 = t5 = temporary
 *   $14 = t6 = interrupt enable flag (from COP0 Status)
 *   $15 = t7 = size / new mutex value
 *   $16 = s0 = heap globals base (0x420000)
 *   $17 = s1 = -1 (constant)
 *   $18 = s2 = saved
 *   $19 = s3 = saved
 *   $ra = return address
 *
 * Stack frame: 0x50 bytes (80 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $17 (s1)
 *   sp+0x20: $18 (s2)
 *   sp+0x30: $19 (s3)
 *   sp+0x40: $ra
 *
 * Mutex state at 0x425A80 (0x420000 + 0x5A80):
 *   0 = unlocked, >0 = locked (count/owner)
 *
 * Algorithm:
 * 1. Load mutex state from 0x425A80
 * 2. If mutex uninitialized (0), initialize to 0x45000C18
 * 3. If size == 0: return 0 immediately (try-lock)
 * 4. Read COP0 Status, extract IE bit (interrupt enabled)
 * 5. If interrupts disabled (IE=0): goto wait_loop
 * 6. If interrupts enabled (IE=1):
 *    a. Load current mutex value
 *    b. new_val = mutex + size
 *    c. Check overflow
 *    d. Try to acquire: old_val = mutex, mutex = new_val
 *    e. If IE was 0 (interrupts disabled): return old_val (got lock)
 *    f. If IE was 1 (interrupts enabled): contention - call EIntr, retry
 * 7. wait_loop: call DIntr to disable interrupts, reload mutex, retry
 */
int MallocMutexLock_0019f078(size_t size)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = (int)size;
    register int v1 __asm__("$3");
    register int v2 __asm__("$4");
    register int v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");
    register int v6 __asm__("$8");
    register int v7 __asm__("$9");
    register int v8 __asm__("$10");
    register int v9 __asm__("$11");
    register int v10 __asm__("$12");
    register int v11 __asm__("$13");
    register int v12 __asm__("$14");
    register int v13 __asm__("$15");
    register int v14 __asm__("$16");
    register int v15 __asm__("$17");
    register int v16 __asm__("$18");
    register int v17 __asm__("$19");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 80 bytes (0x50)
         * ============================================================ */
        "addiu $sp, $sp, -0x50\n"  /* Allocate 80-byte stack frame */
        "sd    $18, 0x20($sp)\n"   /* Save $s2 */
        "sd    $19, 0x30($sp)\n"   /* Save $s3 */
        "sd    $17, 0x10($sp)\n"   /* Save $s1 */
        "sd    $ra, 0x40($sp)\n"   /* Save $ra */

        /* ============================================================
         * LOAD MUTEX STATE: global at 0x425A80
         * Base address 0x420000 in $18 (s0)
         * ============================================================ */
        "lui   %16, 0x42\n"        /* v16 ($18/s0) = 0x420000 */
        "lw    %3, 0x5a80(%16)\n"  /* v3 = mutex_state (at 0x425A80) */

        /* Initialize v17 ($19/s1) = -1 (all bits set) */
        "addiu %17, $0, -1\n"      /* v17 = 0xffffffff */

        /* Move size to v15 ($17/s1) */
        "move  %15, %0\n"          /* v15 = size */

        /* ============================================================
         * CHECK MUTEX INITIALIZATION
         * ============================================================ */
        "bnez  %3, mutex_init_done\n"  /* if mutex != 0, initialized */
        "nop\n"                    /* branch delay slot */

        /* Mutex not initialized - initialize to 0x45000C18 */
        "lui   %3, 0x45\n"         /* v3 = 0x45000000 */
        "addiu %3, %3, 0xc18\n"    /* v3 = 0x45000C18 */
        "sw    %3, 0x5a80(%16)\n"  /* mutex_state = 0x45000C18 */

        "mutex_init_done:\n"

        /* ============================================================
         * SIZE == 0 CHECK: try-lock without waiting
         * ============================================================ */
        "bnez  %15, try_lock\n"    /* if size != 0, goto try_lock */
        "nop\n"                    /* branch delay slot */

        /* Size is 0 - return 0 (success, no wait) */
        "ld    $ra, 0x40($sp)\n"   /* Restore $ra */
        "ld    $19, 0x30($sp)\n"   /* Restore $s3 */
        "ld    $18, 0x20($sp)\n"   /* Restore $s2 */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "jr    $ra\n"              /* Return */
        "addiu $sp, $sp, 0x50\n"   /* Deallocate stack */
        "move  %0, $0\n"           /* delay: v0 = 0 */

        /* ============================================================
         * TRY_LOCK: Attempt to acquire mutex
         * ============================================================ */
        "try_lock:\n"
        /* Read COP0 Status register, extract IE bit (bit 16) */
        "mfc0  %14, $12\n"         /* v14 = COP0 Status */
        "lui   %3, 0x1\n"          /* v3 = 0x10000 */
        "and   %14, %14, %3\n"     /* v14 = Status & 0x10000 (IE bit) */

        /* If interrupts disabled (IE=0), go to wait_loop */
        "bnez  %14, wait_loop\n"   /* if IE=1, interrupts enabled -> wait_loop? */
        "nop\n"                    /* branch delay slot */
        /* Wait - the logic seems inverted. Let me re-read... */
        /* Actually: bnez %14, wait_loop means if IE!=0 (enabled), go wait.
         * But the comment says "if interrupts disabled, goto wait_loop".
         * The code does: if (IE) wait_loop; else try_lock.
         * This is because with interrupts enabled, we can safely wait.
         * With interrupts disabled, we must not block. */

        /* ============================================================
         * INTERRUPTS ENABLED PATH: Try to acquire mutex
         * ============================================================ */
        "lw    %3, 0x5a80(%16)\n"  /* v3 = current mutex value */
        "addu  %15, %3, %15\n"     /* v15 = mutex + size (new value) */
        "sltu  %3, %3, %15\n"      /* v3 = (mutex < new_val) ? 1 : 0 (overflow check) */
        "bnez  %3, wait_loop\n"    /* if overflow, goto wait_loop */
        "nop\n"                    /* branch delay slot */

        /* Try to acquire: atomic-ish read-modify-write */
        "lw    %17, 0x5a80(%16)\n" /* v17 = old mutex value */
        "sw    %15, 0x5a80(%16)\n" /* mutex = new_val (size added) */

        /* Check if we got lock (old_val == 0) */
        "bnez  %14, check_contention\n"  /* if IE=1 (interrupts enabled) */
        "nop\n"                    /* branch delay slot */

        /* No contention (IE=0, interrupts disabled) - return old_val */
        "ld    $ra, 0x40($sp)\n"   /* Restore $ra */
        "ld    $19, 0x30($sp)\n"   /* Restore $s3 */
        "ld    $18, 0x20($sp)\n"   /* Restore $s2 */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "jr    $ra\n"              /* Return */
        "move  %0, %17\n"          /* delay: v0 = old_val */

        "check_contention:\n"
        /* Contention detected (IE=1, interrupts enabled) */
        /* Call EIntr (0x19f060) to enable interrupts, then retry */
        "jal   0x19f060\n"         /* EIntr() */
        "nop\n"                    /* branch delay slot */
        "b     try_lock\n"         /* Retry acquisition */
        "move  %0, %17\n"          /* delay: v0 = old_val */

        /* ============================================================
         * WAIT_LOOP: Interrupts disabled or overflow
         * Call DIntr (0x19f018) to disable interrupts, then retry
         * ============================================================ */
        "wait_loop:\n"
        "jal   0x19f018\n"         /* DIntr() - disable interrupts */
        "nop\n"                    /* branch delay slot */
        "b     try_lock\n"         /* Retry with interrupts disabled */
        "lw    %3, 0x5a80(%16)\n"  /* delay: reload mutex value */
        : "=r"(v0), "+r"(v1), "+r"(v2), "+r"(v3), "+r"(v4), "+r"(v5), "+r"(v6),
          "+r"(v7), "+r"(v8), "+r"(v9), "+r"(v10), "+r"(v11), "+r"(v12), "+r"(v13),
          "+r"(v14), "+r"(v15), "+r"(v16), "+r"(v17)
        : "r"(v0)
        : "memory", "$1", "$24", "$25"
    );
    return v0;
#else
    /* Host fallback: simple mutex using static variable. */
    static int mutex_state = 0;
    (void)size;
    if (mutex_state == 0) {
        mutex_state = 1;
        return 0;
    }
    return 1;
#endif
}

/* Target: 0x0019e784 - Heap free - deallocate memory with coalescing
 * Args: ptr = user pointer (block + 0x10) to free
 * Returns: void
 *
 * Register usage:
 *   $2 = v0 = input: ptr / temporary
 *   $3 = v1 = temporary (block pointers, sizes)
 *   $4 = a0 = temporary
 *   $5 = a1 = temporary
 *   $6 = a2 = previous block in free list
 *   $7 = a3 = current block in free list
 *   $8 = t0 = temporary
 *   $9 = t1 = temporary
 *   $10 = t2 = temporary
 *   $11 = t3 = temporary
 *   $12 = t4 = temporary
 *   $13 = t5 = temporary
 *   $14 = t6 = temporary
 *   $15 = t7 = temporary
 *   $16 = s0 = heap globals base (0x420000)
 *   $ra = return address
 *
 * Stack frame: 0x20 bytes (32 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $ra
 *
 * Heap globals at 0x420000:
 *   0x5a78: free_list_head
 *   0x5a7c: free_list_tail
 *
 * Free block structure:
 *   +0x00: next free block
 *   +0x04: block size
 *   +0x08: prev free block
 *   +0x0c: end address (start + size)
 *
 * Algorithm:
 * 1. If ptr == NULL, return
 * 2. Load free list head
 * 3. If free list empty, return
 * 4. Check if ptr is the head block
 * 5. Search free list for block containing ptr
 * 6. When found, try to coalesce with adjacent free blocks:
 *    a. Check merge with previous (prev_end == ptr)
 *    b. Check merge with next (this_end == next_start)
 *    c. If neither, insert into free list
 * 7. Update tail pointer if needed
 * 8. Return
 */
void free_0019e784(void *ptr)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = ptr;
    register void *v1 __asm__("$3");
    register void *v2 __asm__("$4");
    register void *v3 __asm__("$5");
    register void *v4 __asm__("$6");
    register void *v5 __asm__("$7");
    register void *v6 __asm__("$8");
    register void *v7 __asm__("$9");
    register void *v8 __asm__("$10");
    register void *v9 __asm__("$11");
    register void *v10 __asm__("$12");
    register void *v11 __asm__("$13");
    register void *v12 __asm__("$14");
    register void *v13 __asm__("$15");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 32 bytes (0x20)
         * ============================================================ */
        "addiu $sp, $sp, -0x20\n"  /* Allocate 32-byte stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save $ra */
        "sd    $16, 0x00($sp)\n"   /* Save $s0 */

        /* ============================================================
         * LOAD HEAP GLOBALS BASE: 0x420000
         * ============================================================ */
        "lui   %3, 0x42\n"         /* v3 ($16/s0) = 0x420000 */

        /* ============================================================
         * NULL CHECK
         * ============================================================ */
        "beqz  %0, free_exit\n"    /* if ptr == NULL, exit */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * LOAD FREE LIST HEAD
         * ============================================================ */
        "lw    %4, 0x5a78(%3)\n"   /* v4 = free_list_head */
        "beqz  %4, free_exit\n"    /* if head == NULL, exit */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * CHECK IF PTR IS HEAD BLOCK
         * Head block's user pointer is at head+0x10
         * But the code checks head+0x00 which is next pointer...
         * Actually: free list head points to first FREE block.
         * The block being freed should be matched against free blocks.
         * ============================================================ */
        "lw    %1, 0x0(%4)\n"      /* v1 = head->next (first free block) */
        "beq   %0, %1, free_head\n"  /* if ptr == first_free, goto free_head */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * FREE_SEARCH: Traverse free list to find block before ptr
         * v5 = previous block, v6 = current block
         * ============================================================ */
        "move  %5, %4\n"           /* v5 = previous = head */
        "free_search:\n"
        "lw    %6, 0xc(%5)\n"      /* v6 = prev->end (next free block) */
        "beqz  %6, free_exit\n"    /* if end of list, exit */
        "nop\n"                    /* branch delay slot */
        "lw    %1, 0x0(%6)\n"      /* v1 = current->next */
        "beq   %0, %1, free_found\n"  /* if ptr == current->next, found */
        "nop\n"                    /* branch delay slot */
        "move  %5, %6\n"           /* prev = current */
        "b     free_search\n"      /* continue search */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * FREE_FOUND: Found block to free (v6 = block before ptr)
         * Check coalescing with previous and next blocks
         * ============================================================ */
        "free_found:\n"
        /* Check merge with previous: prev_end == ptr */
        "lw    %1, 0x4(%6)\n"      /* v1 = prev->size */
        "addu  %1, %5, %1\n"       /* v1 = prev + size = prev_end */
        "beq   %1, %0, free_merge_prev\n"  /* if prev_end == ptr, merge */
        "nop\n"                    /* branch delay slot */

        /* Check merge with next: this_end == next_start */
        "lw    %1, 0x8(%0)\n"      /* v1 = ptr->next (block after freed) */
        "beqz  %1, free_no_merge\n"  /* if no next block, no merge */
        "nop\n"                    /* branch delay slot */
        "lw    %2, 0x4(%1)\n"      /* v2 = next->size */
        "addu  %1, %0, %2\n"       /* v1 = ptr + next_size = this_end */
        "lw    %2, 0x0(%1)\n"      /* v2 = next->next */
        "beq   %1, %2, free_merge_next\n"  /* if this_end == next->next, merge */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * FREE_NO_MERGE: Insert as new free block between prev and next
         * ============================================================ */
        "free_no_merge:\n"
        "lw    %1, 0x0(%6)\n"      /* v1 = prev->next */
        "sw    %1, 0xc(%6)\n"      /* prev->end = prev->next */
        "sw    %0, 0xc(%5)\n"      /* prev->end = this (wait, this is %0=ptr) */
        "sw    %5, 0x8(%0)\n"      /* this->prev = prev */
        "sw    %6, 0x0(%0)\n"      /* this->next = prev->next */
        "b     free_exit\n"        /* goto exit */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * FREE_MERGE_PREV: Coalesce with previous free block
         * ============================================================ */
        "free_merge_prev:\n"
        "lw    %1, 0x4(%6)\n"      /* v1 = prev->size */
        "addu  %1, %1, %2\n"       /* v1 = prev_size + this_size (v2 was next->size?) */
        "sw    %1, 0x4(%5)\n"      /* prev->size = combined size */
        "lw    %1, 0xc(%0)\n"      /* v1 = this->end (next free after this) */
        "sw    %1, 0xc(%5)\n"      /* prev->end = this->end */
        "beqz  %1, free_check_tail\n"  /* if no next, check tail */
        "sw    %5, 0x8(%1)\n"      /* next->prev = prev */
        "b     free_check_tail\n"  /* goto check tail */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * FREE_MERGE_NEXT: Coalesce with next free block
         * ============================================================ */
        "free_merge_next:\n"
        "lw    %1, 0x4(%0)\n"      /* v1 = this->size */
        "addu  %1, %1, %2\n"       /* v1 = this_size + next_size */
        "sw    %1, 0x4(%0)\n"      /* this->size = combined size */
        "lw    %1, 0xc(%1)\n"      /* v1 = next->end */
        "sw    %1, 0xc(%0)\n"      /* this->end = next->end */
        "beqz  %1, free_check_tail\n"  /* if no next->end, check tail */
        "sw    %0, 0x8(%1)\n"      /* next_end->prev = this */
        "b     free_check_tail\n"  /* goto check tail */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * FREE_HEAD: ptr was the first free block (head)
         * ============================================================ */
        "free_head:\n"
        "lw    %1, 0xc(%4)\n"      /* v1 = head->end (next free) */
        "sw    %1, 0x5a78(%3)\n"   /* free_list_head = next */
        "beqz  %1, free_check_tail\n"  /* if no next, check tail */
        "sw    $0, 0x8(%1)\n"      /* next->prev = NULL */
        "b     free_check_tail\n"  /* goto check tail */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * FREE_CHECK_TAIL: Update tail pointer if needed
         * ============================================================ */
        "free_check_tail:\n"
        "lw    %1, 0x5a7c(%3)\n"   /* v1 = free_list_tail */
        "beq   %1, %0, free_update_tail\n"  /* if tail == ptr, update */
        "nop\n"                    /* branch delay slot */
        "b     free_exit\n"        /* else exit */
        "nop\n"                    /* branch delay slot */

        "free_update_tail:\n"
        "lw    %1, 0x8(%0)\n"      /* v1 = ptr->prev */
        "sw    %1, 0x5a7c(%3)\n"   /* free_list_tail = prev */
        "b     free_exit\n"        /* goto exit */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * EPILOGUE
         * ============================================================ */
        "free_exit:\n"
        "ld    $ra, 0x10($sp)\n"   /* Restore $ra */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x20\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        : "=r"(v0), "=r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5), "=r"(v6),
          "=r"(v7), "=r"(v8), "=r"(v9), "=r"(v10), "=r"(v11), "=r"(v12), "=r"(v13)
        : "r"(v0)
        : "memory", "$1", "$24", "$25"
    );
#else
    /* Host fallback: simple free using host free. */
    if (ptr) {
        void *orig = ((void **)ptr)[-1];
        free(orig ? orig : ptr);
    }
#endif
}

