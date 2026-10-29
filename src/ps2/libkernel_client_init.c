/*
 * Recovered initialization helpers for libkernel RPC clients used by
 * SNES Station v0.23. These are outside the contiguous call-site wrappers
 * but are directly reached by the remaining fio path.
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
 * GLOBAL STATE ADDRESSES:
 * ------------------------------------------------------------------------
 * 0x425AB0: fio_init_recovered
 * 0x425AB4: sif_iopheap_init flag
 * 0x425AB8: sif_loadfile_init flag
 * 0x426E44: fio_handle
 * 0x426E48: fio_mode
 * 0x45000BC0: sif_iopheap_cd (SifRpcClientData)
 * 0x45000BF0: sif_loadfile_cd (SifRpcClientData)
 * 0x450003C0: some global value for fio
 *
 * ========================================================================
 * KEY FUNCTIONS (syscalls):
 * ------------------------------------------------------------------------
 * 0x19cc0c: SifInitRpc
 * 0x19c688: SifBindRpc
 * 0x19ce70: CreateSema
 * 0x19f018: DIntr (disable interrupts)
 * 0x19f060: EIntr (enable interrupts)
 * 0x19e2e0: some function (vprintf related)
 * 0x19d244: some function (vprintf related)
 * 0x19fd00: func_0019fd00 (fio related)
 * 0x19fcf0: func_0019fcf0 (fio related)
 * 0x19cea0: func_0019cea0 (fio related)
 */

#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "../../include/ps2_libkernel_recovered.h"

extern void SifInitRpc(int);
extern int SifBindRpc(void *, int, int);
extern int CreateSema(const ee_sema32 *);
extern int iSignalSema(int);

SifRpcClientData32 fio_cd_recovered;
int fio_init_recovered;
int fio_completion_sema_recovered = -1;
int fio_block_mode_recovered;

static void bind_delay_recovered(void)
{
    volatile int i;
    for (i = 0x100000; i >= 0; --i) {
    }
}

/* 0x0019f600 */
int fioInit_0019f600(void)
{
    ee_sema32 sema;
    int res;
    if (fio_init_recovered)
        return 0;

    SifInitRpc(0);
    do {
        res = SifBindRpc(&fio_cd_recovered, (int)0x80000001u, 0);
        if (res < 0)
            return res;
        if (fio_cd_recovered.server == 0)
            bind_delay_recovered();
    } while (fio_cd_recovered.server == 0);

    /* Target writes only offsets +0x04, +0x08 and +0x14 before CreateSema. */
    sema.max_count = 1;
    sema.init_count = 1;
    sema.option = 0;
    fio_completion_sema_recovered = CreateSema(&sema);
    if (fio_completion_sema_recovered < 0)
        return -0xd602;

    fio_init_recovered = 1;
    fio_block_mode_recovered = 0;
    return 0;
}

/* 0x0019f6e8 */
void fio_intr_0019f6e8(void *unused)
{
    (void)unused;
    (void)iSignalSema(fio_completion_sema_recovered);
}

/* Global state for SifInitIopHeap at 0x425AB4. */
#if defined(__mips__)
static int *const sif_iopheap_init_ptr = (int *)0x425AB4;
#else
static int sif_iopheap_init_host = 0;
static int *const sif_iopheap_init_ptr = &sif_iopheap_init_host;
#endif

/* SifRpcClientData for SifInitIopHeap at 0x45000BC0. */
#if defined(__mips__)
static SifRpcClientData32 *const sif_iopheap_cd_ptr = (SifRpcClientData32 *)0x45000BC0;
#else
static SifRpcClientData32 sif_iopheap_cd_host;
static SifRpcClientData32 *const sif_iopheap_cd_ptr = &sif_iopheap_cd_host;
#endif

/* 0x0019f9e8 - SifInitIopHeap: Initialize IOP heap RPC client
 * Args: none
 * Returns: 0 on success, negative error code on failure
 *
 * Register usage:
 *   $2 = v0 = temporary / return value
 *   $3 = v1 = temporary (counter, error codes)
 *   $4 = a0 = function argument
 *   $5 = a1 = function argument
 *   $6 = a2 = function argument
 *   $7 = a3 = temporary
 *   $8 = t0 = temporary
 *   $16 = s0 = global base (0x420000) / SifRpcClientData pointer
 *   $17 = s1 = temporary (0x80000003)
 *   $18 = s2 = temporary
 *   $19 = s3 = temporary
 *   $ra = return address
 *
 * Stack frame: 0x20 bytes (32 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $ra
 *
 * Global state:
 *   0x425AB4: sif_iopheap_init flag (0 = not initialized)
 *   0x45000BC0: sif_iopheap_cd (SifRpcClientData for bind)
 *   0x426E44: fio_handle (stores semaphore ID)
 *   0x426E48: fio_mode (stores error code on failure)
 *
 * Algorithm:
 * 1. Check init flag at 0x425AB4 - if set, return 0
 * 2. Call SifInitRpc(0)
 * 3. Set up SifRpcClientData at 0x45000BC0
 * 4. Call SifBindRpc with server=0x80000003
 * 5. Wait for server ready (poll with timeout 0xffff)
 * 6. Create semaphore (max=1, init=1, option=0)
 * 7. Store semaphore ID at 0x426E44
 * 8. Set init flag, return 0
 * 9. On bind fail: store error 0xFFFF29FB at 0x426E48, return error
 */
int SifInitIopHeap_0019f9e8(void)
{
#if defined(__mips__)
    register int v0 __asm__("$2");
    register int v1 __asm__("$3");
    register int v2 __asm__("$4");
    register int v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");
    register int v6 __asm__("$8");
    register int v7 __asm__("$16");
    register int v8 __asm__("$17");
    register int v9 __asm__("$18");
    register int v10 __asm__("$19");
    register int v11 __asm__("$ra");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 32 bytes (0x20)
         * ============================================================ */
        "addiu $sp, $sp, -0x20\n"  /* Allocate 32-byte stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save $ra */
        "sd    $16, 0x00($sp)\n"   /* Save $s0 */

        /* ============================================================
         * CHECK INIT FLAG
         * ============================================================ */
        "lui   %7, 0x42\n"         /* v7 ($16/s0) = 0x420000 */
        "lw    %0, 0x5ab4(%7)\n"   /* v0 = *(0x425ab4) - init flag */

        /* If already initialized, return 0 */
        "bnez  %0, iopheap_exit\n" /* if flag != 0, goto exit */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * CALL SifInitRpc(0)
         * ============================================================ */
        "jal   0x19cc0c\n"         /* Call SifInitRpc */
        "move  $4, $0\n"           /* delay: a0 = 0 */

        /* ============================================================
         * SET UP SifBindRpc PARAMETERS
         * SifRpcClientData at 0x45000BC0
         * Server = 0x80000003
         * ============================================================ */
        "lui   %7, 0x45\n"         /* v7 = 0x45000000 */
        "lui   %8, 0x8000\n"       /* v8 = 0x80000000 */
        "addiu %7, %7, 0xbc0\n"    /* v7 = 0x45000BC0 (client data) */
        "ori   %8, %8, 0x3\n"      /* v8 = 0x80000003 (server) */

        /* ============================================================
         * CALL SifBindRpc(client_data, server, 0)
         * ============================================================ */
        "move  $4, %7\n"           /* a0 = client_data */
        "move  $6, $0\n"           /* a2 = 0 */
        "jal   0x19c688\n"         /* Call SifBindRpc */
        "move  $5, %8\n"           /* delay: a1 = server */

        /* ============================================================
         * CHECK BIND RESULT
         * ============================================================ */
        "bltz  %2, iopheap_bind_fail\n"  /* if result < 0, goto bind_fail */
        "move  $4, %2\n"           /* delay: a0 = result */

        /* ============================================================
         * WAIT FOR SERVER READY
         * Poll client_data+0x24 (server field) until non-zero
         * Timeout: 0xffff iterations
         * ============================================================ */
        "lw    %0, 0x24(%7)\n"     /* v0 = client_data->server */
        "lui   %1, 0xf\n"          /* v1 = 0xf0000 */
        "ori   %1, %1, 0xffff\n"   /* v1 = 0xffffffff (timeout counter) */
        "bnez  %0, iopheap_check_server\n"  /* if server ready, skip wait */
        "nop\n"                    /* branch delay slot */

        "iopheap_wait_server:\n"
        "addiu %1, %1, -1\n"       /* v1-- (decrement timeout) */
        "addiu %0, $0, -2\n"       /* v0 = -2 */
        "bne   %1, %0, iopheap_wait_server\n"  /* if timeout not expired, loop */
        "lui   %7, 0x45\n"         /* delay: reload v7 = 0x45000000 */

        "iopheap_check_server:\n"
        "addiu %0, $0, 1\n"        /* v0 = 1 */

        /* ============================================================
         * CREATE SEMAPHORE
         * ee_sema32 on stack: max_count=1, init_count=1, option=0
         * ============================================================ */
        "move  $4, $sp\n"          /* a0 = &sema (on stack) */
        "sw    %0, 0x8($sp)\n"     /* sema.max_count = 1 */
        "sw    %0, 0x4($sp)\n"     /* sema.init_count = 1 */
        "sw    $0, 0x14($sp)\n"    /* sema.option = 0 */
        "jal   0x19ce70\n"         /* Call CreateSema */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * CHECK CreateSema RESULT
         * ============================================================ */
        "lui   %0, 0xffff\n"       /* v0 = 0xffff0000 */
        "lui   %1, 0x42\n"         /* v1 = 0x420000 */
        "sw    %2, 0x6e44(%1)\n"   /* *(0x426e44) = sema_id (fio_handle) */
        "bltz  %2, iopheap_bind_fail\n"  /* if sema_id < 0, goto bind_fail */
        "ori   %0, %0, 0x29fe\n"   /* v0 = 0xffff29fe (error code?) */

        /* ============================================================
         * MARK INITIALIZED
         * ============================================================ */
        "lui   %0, 0x42\n"         /* v0 = 0x420000 */
        "move  $4, $0\n"           /* a0 = 0 */
        "sw    %7, 0x5ab4(%0)\n"   /* *(0x425ab4) = 0x45000bc0 (mark init) */
        "b     iopheap_exit\n"     /* goto exit */
        "sw    $0, 0x6e48(%0)\n"   /* delay: *(0x426e48) = 0 (fio_mode = 0) */

        /* ============================================================
         * BIND FAIL HANDLING
         * ============================================================ */
        "iopheap_bind_fail:\n"
        "lui   %0, 0xffff\n"       /* v0 = 0xffff0000 */
        "ori   %1, %0, 0x29fb\n"   /* v1 = 0xffff29fb (error code) */
        "move  %3, %1\n"           /* v3 = error code */

        /* ============================================================
         * EXIT
         * ============================================================ */
        "iopheap_exit:\n"
        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"   /* Restore $ra */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x20\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "move  %2, %3\n"           /* delay: v2 = v3 (return error or 0) */
        : "=r"(v0), "=r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5), "=r"(v6),
          "=r"(v7), "=r"(v8), "=r"(v9), "=r"(v10), "=r"(v11)
        :
        : "memory", "$1", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v2;
#else
    /* Host fallback */
    if (*sif_iopheap_init_ptr)
        return 0;

    SifInitRpc(0);
    int res = SifBindRpc(sif_iopheap_cd_ptr, 0x80000003, 0);
    if (res < 0)
        return res;

    ee_sema32 sema = { .max_count = 1, .init_count = 1, .option = 0 };
    int sema_id = CreateSema(&sema);
    if (sema_id < 0)
        return -0xd602;

    *sif_iopheap_init_ptr = 1;
    return 0;
#endif
}

/* Global state for SifLoadFileInit at 0x425AB8. */
#if defined(__mips__)
static int *const sif_loadfile_init_ptr = (int *)0x425AB8;
#else
static int sif_loadfile_init_host = 0;
static int *const sif_loadfile_init_ptr = &sif_loadfile_init_host;
#endif

/* SifRpcClientData for SifLoadFileInit at 0x45000BF0. */
#if defined(__mips__)
static SifRpcClientData32 *const sif_loadfile_cd_ptr = (SifRpcClientData32 *)0x45000BF0;
#else
static SifRpcClientData32 sif_loadfile_cd_host;
static SifRpcClientData32 *const sif_loadfile_cd_ptr = &sif_loadfile_cd_host;
#endif

/* 0x0019fd20 - SifLoadFileInit: Initialize LoadFile RPC client
 * Args: none
 * Returns: 0 on success, negative error code on failure
 *
 * Register usage: (similar to SifInitIopHeap)
 *   $2 = v0 = temporary / return value
 *   $3 = v1 = temporary (counter, error codes)
 *   $4 = a0 = function argument
 *   $5 = a1 = function argument
 *   $6 = a2 = function argument
 *   $7 = a3 = temporary
 *   $8 = t0 = temporary
 *   $16 = s0 = global base (0x420000) / SifRpcClientData pointer
 *   $17 = s1 = temporary (0x80000006)
 *   $18 = s2 = temporary
 *   $19 = s3 = temporary
 *   $ra = return address
 *
 * Stack frame: 0x20 bytes (32 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $ra
 *
 * Global state:
 *   0x425AB8: sif_loadfile_init flag (0 = not initialized)
 *   0x45000BF0: sif_loadfile_cd (SifRpcClientData for bind)
 *
 * Algorithm:
 * 1. Check init flag at 0x425AB8 - if set, return 0
 * 2. Call SifInitRpc(0)
 * 3. Set up SifRpcClientData at 0x45000BF0
 * 4. Call SifBindRpc with server=0x80000006
 * 5. Wait for server ready (poll with timeout 0xffff)
 * 6. Set init flag, return 0
 * 7. On bind fail: store error 0xFFFF29EE, return error
 *
 * Difference from SifInitIopHeap:
 * - Uses server=0x80000006 (vs 0x80000003)
 * - Client data at 0x45000BF0 (vs 0x45000BC0)
 * - Init flag at 0x425AB8 (vs 0x425AB4)
 * - No semaphore creation
 * - Different error code on failure (0x29EE vs 0x29FB)
 */
int SifLoadFileInit_0019fd20(void)
{
#if defined(__mips__)
    register int v0 __asm__("$2");
    register int v1 __asm__("$3");
    register int v2 __asm__("$4");
    register int v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");
    register int v6 __asm__("$8");
    register int v7 __asm__("$16");
    register int v8 __asm__("$17");
    register int v9 __asm__("$18");
    register int v10 __asm__("$19");
    register int v11 __asm__("$ra");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 32 bytes (0x20)
         * ============================================================ */
        "addiu $sp, $sp, -0x20\n"  /* Allocate 32-byte stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save $ra */
        "sd    $16, 0x00($sp)\n"   /* Save $s0 */

        /* ============================================================
         * CHECK INIT FLAG
         * ============================================================ */
        "lui   %7, 0x42\n"         /* v7 ($16/s0) = 0x420000 */
        "lw    %0, 0x5ab8(%7)\n"   /* v0 = *(0x425ab8) - init flag */

        /* If already initialized, return 0 */
        "bnez  %0, loadfile_exit\n"  /* if flag != 0, goto exit */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * CALL SifInitRpc(0)
         * ============================================================ */
        "jal   0x19cc0c\n"         /* Call SifInitRpc */
        "move  $4, $0\n"           /* delay: a0 = 0 */

        /* ============================================================
         * SET UP SifBindRpc PARAMETERS
         * SifRpcClientData at 0x45000BF0
         * Server = 0x80000006
         * ============================================================ */
        "lui   %7, 0x45\n"         /* v7 = 0x45000000 */
        "lui   %8, 0x8000\n"       /* v8 = 0x80000000 */
        "addiu %7, %7, 0xbf0\n"    /* v7 = 0x45000BF0 (client data) */
        "ori   %8, %8, 0x6\n"      /* v8 = 0x80000006 (server) */

        /* ============================================================
         * CALL SifBindRpc(client_data, server, 0)
         * ============================================================ */
        "move  $4, %7\n"           /* a0 = client_data */
        "move  $6, $0\n"           /* a2 = 0 */
        "jal   0x19c688\n"         /* Call SifBindRpc */
        "move  $5, %8\n"           /* delay: a1 = server */

        /* ============================================================
         * CHECK BIND RESULT
         * ============================================================ */
        "bltz  %2, loadfile_bind_fail\n"  /* if result < 0, goto bind_fail */
        "move  $4, %2\n"           /* delay: a0 = result */

        /* ============================================================
         * WAIT FOR SERVER READY
         * Poll client_data+0x24 (server field) until non-zero
         * Timeout: 0xffff iterations
         * ============================================================ */
        "lw    %0, 0x24(%7)\n"     /* v0 = client_data->server */
        "lui   %1, 0xf\n"          /* v1 = 0xf0000 */
        "ori   %1, %1, 0xffff\n"   /* v1 = 0xffffffff (timeout counter) */
        "bnez  %0, loadfile_check_server\n"  /* if server ready, skip wait */
        "nop\n"                    /* branch delay slot */

        "loadfile_wait_server:\n"
        "addiu %1, %1, -1\n"       /* v1-- (decrement timeout) */
        "addiu %0, $0, -2\n"       /* v0 = -2 */
        "bne   %1, %0, loadfile_wait_server\n"  /* if timeout not expired, loop */
        "lui   %7, 0x45\n"         /* delay: reload v7 = 0x45000000 */

        "loadfile_check_server:\n"
        "addiu %0, $0, 1\n"        /* v0 = 1 */

        /* ============================================================
         * MARK INITIALIZED (no semaphore needed for LoadFile)
         * ============================================================ */
        "lui   %7, 0x42\n"         /* v7 = 0x420000 */
        "sw    %0, 0x5ab8(%7)\n"   /* *(0x425ab8) = 1 (mark init) */

        "loadfile_exit:\n"
        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"   /* Restore $ra */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x20\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "move  %2, $0\n"           /* delay: v2 = 0 (success) */

        /* ============================================================
         * BIND FAIL HANDLING
         * ============================================================ */
        "loadfile_bind_fail:\n"
        "lui   %0, 0xffff\n"       /* v0 = 0xffff0000 */
        "ori   %1, %0, 0x29ee\n"   /* v1 = 0xffff29ee (error code) */
        "move  %3, %1\n"           /* v3 = error code */
        "b     loadfile_exit\n"    /* goto exit */
        "move  %2, %3\n"           /* delay: v2 = error code */
        : "=r"(v0), "=r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5), "=r"(v6),
          "=r"(v7), "=r"(v8), "=r"(v9), "=r"(v10), "=r"(v11)
        :
        : "memory", "$1", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v2;
#else
    /* Host fallback */
    if (*sif_loadfile_init_ptr)
        return 0;

    SifInitRpc(0);
    int res = SifBindRpc(sif_loadfile_cd_ptr, 0x80000006, 0);
    if (res < 0)
        return res;

    *sif_loadfile_init_ptr = 1;
    return 0;
#endif
}

/* 0x0019FAA8 - vprintf: formatted output to stdout
 * Args: fmt = format string, ap = va_list arguments
 * Returns: Number of characters written (or error)
 *
 * Register usage:
 *   $2 = v0 = input: fmt / output: return value (chars written)
 *   $3 = v1 = temporary / second function result
 *   $4 = a0 = input: ap (va_list) / function arguments
 *   $5 = a1 = temporary
 *   $16 = s0 = saved ap / first function result
 *   $17 = s1 = 0x4454C0 (some global / file descriptor?)
 *   $ra = return address
 *
 * Stack frame: 0x30 bytes (48 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $17 (s1)
 *   sp+0x20: $ra
 *
 * Function calls:
 *   0x19e2e0: Some setup function (a0=0x4454C0, a1=1, a2=ap)
 *   0x19d244: Actual output function (a0=0x4454C0, a1=0x4454C0, a2=fmt, a3=result)
 *
 * Algorithm:
 * 1. Save ap to $s0
 * 2. Load global 0x4454C0 to $s1
 * 3. Call 0x19e2e0(0x4454C0, 1, ap) - setup
 * 4. Call 0x19d244(0x4454C0, 0x4454C0, fmt, setup_result) - output
 * 5. Return result from 0x19d244
 */
int vprintf_0019faa8(const char *fmt, va_list ap)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = (int)fmt;
    register int v1 __asm__("$3");
    register int v2 __asm__("$4") = (int)ap;
    register int v3 __asm__("$5");
    register int v4 __asm__("$16");
    register int v5 __asm__("$17");
    register int v6 __asm__("$ra");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 48 bytes (0x30)
         * ============================================================ */
        "addiu $sp, $sp, -0x30\n"  /* Allocate 48-byte stack frame */
        "sd    $16, 0x00($sp)\n"   /* Save $s0 */
        "sd    $ra, 0x20($sp)\n"   /* Save $ra */
        "move  %4, %2\n"           /* v4 ($16/s0) = v2 = ap (save va_list) */
        "lui   %5, 0x44\n"         /* v5 ($17/s1) = 0x440000 */
        "addiu %5, %5, 0x54c0\n"   /* v5 = 0x4454C0 (global/file descriptor) */
        "move  $4, %5\n"           /* a0 = 0x4454C0 */
        "sd    $17, 0x10($sp)\n"   /* Save $s1 */
        "addiu $4, $0, 0x1\n"      /* a1 = 1 */
        "jal   0x19e2e0\n"         /* Call 0x19e2e0(0x4454C0, 1, ap) */
        "move  %3, %4\n"           /* delay: a2 = ap */
        "move  %3, %4\n"           /* v3 = v4 = ap */

        /* ============================================================
         * SECOND CALL: 0x19d244
         * ============================================================ */
        "move  $4, %5\n"           /* a0 = 0x4454C0 */
        "move  $5, %5\n"           /* a1 = 0x4454C0 */
        "move  $6, %1\n"           /* a2 = v1 (fmt? wait, v1 is uninitialized?) */
        "jal   0x19d244\n"         /* Call 0x19d244(0x4454C0, 0x4454C0, fmt, v3) */
        "move  %1, %3\n"           /* delay: a3 = v3 = ap */

        /* ============================================================
         * EPILOGUE
         * ============================================================ */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "move  %0, %1\n"           /* v0 = v1 (result from 0x19d244) */
        "ld    $ra, 0x20($sp)\n"   /* Restore $ra */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "addiu $sp, $sp, 0x30\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "nop\n"                    /* delay slot */
        : "=r"(v0), "=r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5), "=r"(v6)
        : "r"(v0), "r"(v2)
        : "memory", "$1", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    (void)fmt; (void)ap;
    return 0;
#endif
}

/* Global state for fio at 0x425AB0. */
#if defined(__mips__)
static int *const fio_init_ptr = (int *)0x425AB0;
static int *const fio_mode_ptr = (int *)0x426E48;
static int *const fio_handle_ptr = (int *)0x426E44;
#else
static int fio_init_host = 0;
static int fio_mode_host = 0;
static int fio_handle_host = 0;
static int *const fio_init_ptr = &fio_init_host;
static int *const fio_mode_ptr = &fio_mode_host;
static int *const fio_handle_ptr = &fio_handle_host;
#endif

extern int fioInit_0019f600(void);
extern int func_0019fd00(int);
extern int func_0019fcf0(int);
extern int func_0019cea0(int);

/* 0x0019f708 - fio I/O function (read/write)
 * Args: buf = buffer, mode = operation mode (1=read?), user_ptr = user pointer, size = size
 * Returns: Result code (1=success, negative=error, or error code in v2)
 *
 * Register usage:
 *   $2 = v0 = input: buf / temporary / return value
 *   $3 = v1 = input: mode / temporary
 *   $4 = a0 = input: user_ptr / temporary
 *   $5 = a1 = input: size / saved to $17
 *   $6 = a2 = temporary
 *   $7 = a3 = temporary
 *   $8 = t0 = temporary
 *   $16 = s0 = buf (saved)
 *   $17 = s1 = size (saved)
 *   $ra = return address
 *
 * Stack frame: 0x30 bytes (48 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $17 (s1)
 *   sp+0x20: $ra
 *
 * Global state:
 *   0x425AB0: fio_init_recovered (init flag)
 *   0x426E44: fio_handle (RPC client handle)
 *   0x426E48: fio_mode (mode / error code)
 *   0x450003C0: some global value
 *
 * Function calls:
 *   fioInit_0019f600: Initialize fio
 *   func_0019fd00: Some RPC call
 *   func_0019fcf0: Another RPC call
 *   func_0019cea0: Cleanup RPC call
 *
 * Algorithm:
 * 1. Save size to $s1
 * 2. Check fio_init flag - if not init, call fioInit
 * 3. Check fio_mode == 1 - if not, return error 0xFFFF29FB
 * 4. Call func_0019fd00(fio_handle)
 * 5. If success, call func_0019fcf0(fio_handle)
 * 6. If user_ptr != NULL, copy value from 0x450003C0 to user_ptr
 * 7. Return success (1)
 * 8. Call func_0019cea0(fio_handle)
 * 9. Loop back to step 4
 * 10. On error: store error code, return
 */
int fio_io_0019f708(void *buf, int mode, void *user_ptr, int size)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = (int)buf;
    register int v1 __asm__("$3") = mode;
    register int v2 __asm__("$4") = (int)user_ptr;
    register int v3 __asm__("$5") = size;
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");
    register int v6 __asm__("$8");
    register int v7 __asm__("$16");
    register int v8 __asm__("$17");
    register int v9 __asm__("$ra");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 48 bytes (0x30)
         * ============================================================ */
        "addiu $sp, $sp, -0x30\n"  /* Allocate 48-byte stack frame */
        "sd    $17, 0x10($sp)\n"   /* Save $s1 */
        "sd    $16, 0x00($sp)\n"   /* Save $s0 */
        "sd    $ra, 0x20($sp)\n"   /* Save $ra */

        /* ============================================================
         * SAVE SIZE TO $s1
         * ============================================================ */
        "move  %7, %3\n"           /* v7 ($17/s1) = v3 = size */

        /* ============================================================
         * CHECK FIO INIT FLAG
         * ============================================================ */
        "lui   %8, 0x42\n"         /* v8 = 0x420000 */
        "lw    %0, 0x5ab0(%8)\n"   /* v0 = *(0x425ab0) - fio_init flag */

        /* If fio not initialized, call fioInit */
        "bnez  %0, fio_check_mode\n"  /* if init, goto check_mode */
        "nop\n"                    /* branch delay slot */

        /* Call fioInit */
        "jal   fioInit_0019f600\n" /* Call fioInit */
        "nop\n"                    /* delay slot */

        /* Check fioInit result */
        "bgez  %2, fio_check_mode\n"  /* if result >= 0, goto check_mode */
        "move  %6, %2\n"           /* delay: v6 = result (error) */

        /* fioInit failed - return error */
        "ld    $ra, 0x20($sp)\n"   /* Restore $ra */
        "lui   %8, 0x42\n"         /* v8 = 0x420000 */
        "jr    $ra\n"              /* Return */
        "sw    %6, 0x6e48(%8)\n"   /* delay: *(0x426e48) = error (fio_mode) */

        "fio_check_mode:\n"
        /* ============================================================
         * SAVE BUF TO $s0
         * ============================================================ */
        "move  %8, %0\n"           /* v8 ($16/s0) = v0 = buf */

        /* ============================================================
         * LOAD FIO_MODE AND CHECK
         * ============================================================ */
        "lui   %0, 0x42\n"         /* v0 = 0x420000 */
        "lw    %4, 0x6e48(%0)\n"   /* v4 = *(0x426e48) - fio_mode */

        /* Check if mode == 1 */
        "addiu %1, $0, 1\n"        /* v1 = 1 */
        "beq   %4, %1, fio_mode_ok\n"  /* if mode == 1, ok */
        "nop\n"                    /* branch delay slot */

        /* Mode not 1 - return error 0xFFFF29FB */
        "lui   %3, 0xffff\n"       /* v3 = 0xffff0000 */
        "ori   %6, %3, 0x29fb\n"   /* v6 = 0xffff29fb (error code) */
        "ld    $ra, 0x20($sp)\n"   /* Restore $ra */
        "move  %2, %6\n"           /* v2 = error code */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x30\n"   /* Deallocate stack */
        "jr    $ra\n"              /* Return */

        "fio_mode_ok:\n"
        /* ============================================================
         * CALL func_0019fd00(fio_handle)
         * ============================================================ */
        "lui   %8, 0x42\n"         /* v8 = 0x420000 */
        "lw    %4, 0x6e44(%8)\n"   /* v4 = *(0x426e44) - fio_handle */
        "jal   func_0019fd00\n"    /* Call func_0019fd00 */
        "nop\n"                    /* delay slot */

        /* ============================================================
         * CHECK RESULT
         * ============================================================ */
        "bltz  %2, fio_error_exit\n"  /* if result < 0, goto error_exit */
        "move  %0, $0\n"           /* delay: v0 = 0 */

        /* ============================================================
         * CALL func_0019fcf0(fio_handle)
         * ============================================================ */
        "lw    %4, 0x6e44(%8)\n"   /* v4 = fio_handle */
        "jal   func_0019fcf0\n"    /* Call func_0019fcf0 */
        "nop\n"                    /* delay slot */

        /* ============================================================
         * CHECK USER_PTR
         * ============================================================ */
        "beqz  %7, fio_skip_user\n"  /* if user_ptr == 0, skip */
        "nop\n"                    /* delay slot */

        /* Load value from 0x450003C0 and store to user_ptr */
        "lui   %4, 0x45\n"         /* v4 = 0x45000000 */
        "lui   %5, 0x2000\n"       /* v5 = 0x20000000 */
        "addiu %4, %4, 0x3c0\n"    /* v4 = 0x450003C0 */
        "or    %4, %4, %5\n"       /* v4 = 0x450003C0 | 0x20000000 = 0x650003C0? */
        /* Wait: 0x45000000 | 0x20000000 = 0x65000000, +0x3c0 = 0x650003C0 */
        "lw    %5, 0x0(%4)\n"      /* v5 = *(0x650003C0) */
        "sw    %5, 0x0(%7)\n"      /* *user_ptr = v5 */

        "fio_skip_user:\n"
        /* Return success (1) */
        "addiu %6, $0, 1\n"        /* v6 = 1 */
        "b     fio_error_exit\n"   /* goto exit (with success) */
        "move  %0, %6\n"           /* delay: v0 = 1 */

        /* ============================================================
         * CALL func_0019cea0(fio_handle)
         * ============================================================ */
        "lw    %4, 0x6e44(%8)\n"   /* v4 = fio_handle */
        "jal   func_0019cea0\n"    /* Call func_0019cea0 */
        "nop\n"                    /* delay slot */

        /* ============================================================
         * LOOP BACK
         * ============================================================ */
        "b     fio_mode_ok\n"      /* Loop back to process more */
        "nop\n"                    /* delay slot */

        "fio_error_exit:\n"
        /* ============================================================
         * EPILOGUE
         * ============================================================ */
        "ld    $ra, 0x20($sp)\n"   /* Restore $ra */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "ld    $16, 0x00($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x30\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "move  %2, %6\n"           /* delay: v2 = v6 (return value) */
        : "=r"(v0), "=r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5), "=r"(v6),
          "=r"(v7), "=r"(v8), "=r"(v9)
        : "r"(v0), "r"(v1), "r"(v2), "r"(v3)
        : "memory", "$1", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v2;
#else
    /* Host fallback */
    if (!*fio_init_ptr) {
        int res = fioInit_0019f600();
        if (res < 0)
            return res;
    }
    (void)buf; (void)mode; (void)user_ptr; (void)size;
    return 0;
#endif
}
