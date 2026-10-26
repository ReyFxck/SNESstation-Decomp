/*
 * Recovered initialization helpers for libkernel RPC clients used by
 * SNES Station v0.23. These are outside the contiguous call-site wrappers
 * but are directly reached by the remaining fio path.
 */
#include <stdint.h>
#include <string.h>

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

/* 0x0019f9e8 - SifInitIopHeap */
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
        /* Prologue */
        "addiu $sp, $sp, -0x20\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x00($sp)\n"

        /* Load global init flag at 0x425AB4 */
        "lui   %7, 0x42\n"
        "lw    %0, 0x5ab4(%7)\n"

        /* If already initialized, return 0 */
        "bnez  %0, iopheap_exit\n"
        "nop\n"

        /* Call SifInitRpc(0) */
        "jal   0x19cc0c\n"
        "move  $4, $0\n"

        /* Set up SifBindRpc parameters */
        "lui   %7, 0x45\n"
        "lui   %8, 0x8000\n"
        "addiu %7, %7, 0xbc0\n"
        "ori   %8, %8, 0x3\n"

        /* Call SifBindRpc */
        "move  $4, %7\n"
        "move  $6, $0\n"
        "jal   0x19c688\n"
        "move  $5, %8\n"

        /* Check result */
        "bltz  %2, iopheap_bind_fail\n"
        "move  $4, %2\n"

        /* Check if server is ready */
        "lw    %0, 0x24(%7)\n"
        "lui   %1, 0xf\n"
        "ori   %1, %1, 0xffff\n"
        "bnez  %0, iopheap_check_server\n"
        "nop\n"

        "iopheap_wait_server:\n"
        "addiu %1, %1, -1\n"
        "addiu %0, $0, -2\n"
        "bne   %1, %0, iopheap_wait_server\n"
        "lui   %7, 0x45\n"

        "iopheap_check_server:\n"
        "addiu %0, $0, 1\n"

        /* Create semaphore */
        "move  $4, $sp\n"
        "sw    %0, 0x8($sp)\n"
        "sw    %0, 0x4($sp)\n"
        "sw    $0, 0x14($sp)\n"
        "jal   0x19ce70\n"
        "nop\n"

        /* Check CreateSema result */
        "lui   %0, 0xffff\n"
        "lui   %1, 0x42\n"
        "sw    %2, 0x6e44(%1)\n"
        "bltz  %2, iopheap_bind_fail\n"
        "ori   %0, %0, 0x29fe\n"

        /* Mark as initialized */
        "lui   %0, 0x42\n"
        "move  $4, $0\n"
        "sw    %7, 0x5ab4(%0)\n"
        "b     iopheap_exit\n"
        "sw    $0, 0x6e48(%0)\n"

        "iopheap_bind_fail:\n"
        "lui   %0, 0xffff\n"
        "ori   %1, %0, 0x29fb\n"
        "move  %3, %1\n"

        "iopheap_exit:\n"
        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x00($sp)\n"
        "addiu $sp, $sp, 0x20\n"
        "jr    $ra\n"
        "move  %2, %3\n"
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

/* 0x0019fd20 - SifLoadFileInit */
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
        /* Prologue */
        "addiu $sp, $sp, -0x20\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x00($sp)\n"

        /* Load global init flag at 0x425AB8 */
        "lui   %7, 0x42\n"
        "lw    %0, 0x5ab8(%7)\n"

        /* If already initialized, return 0 */
        "bnez  %0, loadfile_exit\n"
        "nop\n"

        /* Call SifInitRpc(0) */
        "jal   0x19cc0c\n"
        "move  $4, $0\n"

        /* Set up SifBindRpc parameters */
        "lui   %7, 0x45\n"
        "lui   %8, 0x8000\n"
        "addiu %7, %7, 0xbf0\n"
        "ori   %8, %8, 0x6\n"

        /* Call SifBindRpc */
        "move  $4, %7\n"
        "move  $6, $0\n"
        "jal   0x19c688\n"
        "move  $5, %8\n"

        /* Check result */
        "bltz  %2, loadfile_bind_fail\n"
        "move  $4, %2\n"

        /* Check if server is ready */
        "lw    %0, 0x24(%7)\n"
        "lui   %1, 0xf\n"
        "ori   %1, %1, 0xffff\n"
        "bnez  %0, loadfile_check_server\n"
        "nop\n"

        "loadfile_wait_server:\n"
        "addiu %1, %1, -1\n"
        "addiu %0, $0, -2\n"
        "bne   %1, %0, loadfile_wait_server\n"
        "lui   %7, 0x45\n"

        "loadfile_check_server:\n"
        "addiu %0, $0, 1\n"

        /* Mark as initialized */
        "lui   %7, 0x42\n"
        "sw    %0, 0x5ab8(%7)\n"

        "loadfile_exit:\n"
        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x00($sp)\n"
        "addiu $sp, $sp, 0x20\n"
        "jr    $ra\n"
        "move  %2, $0\n"

        "loadfile_bind_fail:\n"
        "lui   %0, 0xffff\n"
        "ori   %1, %0, 0x29ee\n"
        "move  %3, %1\n"
        "b     loadfile_exit\n"
        "move  %2, %3\n"
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
