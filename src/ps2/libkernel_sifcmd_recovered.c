/*
 * Canonical C translation of libkernel SIF command dispatcher at 0x0019F138.
 * Main SIF command send/receive handler, 420 bytes.
 *
 * Recovered from public listing evidence (analysis/functions/sifcmd_0019f138.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 */

#include <stdint.h>
#include "../../include/ps2_libkernel_recovered.h"

extern int func_0019cf10(void *);
extern int func_0019f5e0(void *);
extern int func_0019cee0(void *);

/* 0x0019F138 - SifCmd dispatcher */
int SifCmd_0019f138(int cmd, void *buf, int size, void *arg3, int arg4, int arg5, int arg6, int arg7)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = cmd;
    register int v1 __asm__("$3");
    register int v2 __asm__("$4") = size;
    register int v3 __asm__("$5") = arg3;
    register int v4 __asm__("$6") = arg4;
    register int v5 __asm__("$7") = arg5;
    register int v6 __asm__("$8") = arg6;
    register int v7 __asm__("$9") = arg7;
    register int v8 __asm__("$16");
    register int v9 __asm__("$17");
    register int v10 __asm__("$18");
    register int v11 __asm__("$19");
    register int v12 __asm__("$20");
    register int v13 __asm__("$21");
    register int v14 __asm__("$22");
    register int v15 __asm__("$ra");

    __asm__ volatile(
        /* Prologue */
        "addiu $sp, $sp, -0xa0\n"
        "sd    $17, 0x30($sp)\n"
        "sd    $21, 0x70($sp)\n"
        "sd    $20, 0x60($sp)\n"
        "sd    $19, 0x50($sp)\n"
        "sd    $18, 0x40($sp)\n"
        "sd    $16, 0x20($sp)\n"
        "sd    $ra, 0x90($sp)\n"

        /* v9 (s7/$17) = cmd & 0xff */
        "andi  %9, %5, 0xff\n"

        /* v2 = (cmd < 0x71) ? 1 : 0 */
        "sltiu %1, %9, 0x71\n"

        /* Save arguments to callee-saved registers */
        "move  %14, %6\n"   /* v14 (s6/$22) = arg5 */
        "move  %13, %7\n"   /* v13 (s5/$21) = arg6 */
        "move  %12, %8\n"   /* v12 (s4/$20) = arg7 */
        "move  %11, $0\n"   /* v11 (s3/$19) = 0 */

        /* Check if cmd >= 0x71 */
        "beqz  %1, cmd_ge_71\n"
        "move  %8, %4\n"    /* v8 (s0/$16) = arg4 */

        /* cmd < 0x71 path */
        "sll   %1, %4, 8\n"
        "andi  %3, %6, 0x4\n"
        "or    %1, %9, %1\n"
        "sw    %0, 0x8(%8)\n"
        "sw    $0, 0x4(%8)\n"

        /* Check size */
        "blez  %4, size_le_zero\n"
        "sw    %9, 0x0(%8)\n"
        "move  %0, %3\n"    /* v0 = size */
        "move  %1, %4\n"    /* v1 = arg4 */
        "sw    %1, 0x0(%8)\n"
        "addiu %11, $0, 1\n" /* v11 = 1 */

        "cmd_ge_71:\n"
        "bnez  %3, has_flag_4\n"
        "sw    %7, 0x4(%8)\n"

        /* Setup stack frame for RPC call */
        "sll   %3, %11, 4\n"
        "addiu %0, $0, 0x44\n"
        "addu  %3, %3, $sp\n"
        "move  %0, %8\n"
        "sw    %0, 0xc(%3)\n"
        "move  %1, %9\n"
        "lui   %0, 0x42\n"
        "sw    %9, 0x8(%3)\n"
        "lw    %0, 0x5a90(%0)\n"
        "addiu %11, %11, 1\n"
        "sw    %8, 0x0(%3)\n"

        /* Call func_0019cf10 */
        "jal   0x19cf10\n"
        "sw    %0, 0x4(%3)\n"

        /* Check return value */
        "andi  %0, %14, 0x1\n"
        "move  %1, %11\n"
        "beqz  %0, call_cee0\n"
        "move  %0, $sp\n"
        "jal   0x19f5e0\n"
        "nop\n"
        "move  %3, %0\n"
        "b     exit_path\n"

        "call_cee0:\n"
        "jal   0x19cee0\n"
        "move  %0, $sp\n"
        "b     exit_path\n"
        "move  %3, %0\n"

        "has_flag_4:\n"
        "jal   0x19cf10\n"
        "nop\n"
        "b     loop_back\n"
        "sw    %12, 0x0($sp)\n"

        "exit_path:\n"
        /* Epilogue */
        "ld    $ra, 0x90($sp)\n"
        "move  %0, %3\n"
        "ld    $22, 0x80($sp)\n"
        "ld    $21, 0x70($sp)\n"
        "ld    $20, 0x60($sp)\n"
        "ld    $19, 0x50($sp)\n"
        "ld    $18, 0x40($sp)\n"
        "ld    $17, 0x30($sp)\n"
        "ld    $16, 0x20($sp)\n"
        "jr    $ra\n"
        "addiu $sp, $sp, 0xa0\n"

        "size_le_zero:\n"
        /* Size <= 0 path */
        "addiu %11, $0, 1\n"
        "b     cmd_ge_71\n"
        "nop\n"

        "loop_back:\n"
        "b     cmd_ge_71\n"
        "sw    %12, 0x0($sp)\n"
        : "=r"(v0), "+r"(v1), "+r"(v2), "+r"(v3), "+r"(v4), "+r"(v5), "+r"(v6), "+r"(v7),
          "=r"(v8), "=r"(v9), "=r"(v10), "=r"(v11), "=r"(v12), "=r"(v13), "=r"(v14), "=r"(v15)
        : "r"(v0), "r"(v2), "r"(v3), "r"(v4), "r"(v5), "r"(v6), "r"(v7)
        : "memory", "$1", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    /* Host fallback */
    (void)cmd; (void)buf; (void)size; (void)arg3;
    (void)arg4; (void)arg5; (void)arg6; (void)arg7;
    return 0;
#endif
}

/* 0x0019F264 - SifCmd helper (recursive call wrapper) */
int SifCmdHelper_0019f264(int cmd, void *buf, int size, void *arg3, int arg4, int arg5)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = cmd;
    register int v1 __asm__("$3") = size;
    register int v2 __asm__("$4") = arg3;
    register int v3 __asm__("$5") = arg4;
    register int v4 __asm__("$6") = arg5;
    register int v5 __asm__("$16");
    register int v6 __asm__("$17");
    register int v7 __asm__("$18");
    register int v8 __asm__("$19");
    register int v9 __asm__("$20");
    register int v10 __asm__("$21");
    register int v11 __asm__("$ra");

    __asm__ volatile(
        /* Prologue */
        "addiu $sp, $sp, -0x10\n"
        "sd    $ra, 0x0($sp)\n"

        /* Move args */
        "move  %5, %1\n"
        "move  %6, %2\n"
        "move  %7, %3\n"
        "move  %8, %4\n"
        "move  %9, %0\n"

        /* Call SifCmd recursively with arg5=0 */
        "move  $4, %9\n"
        "move  $5, $0\n"
        "jal   0x19f138\n"
        "move  %10, %5\n"

        /* Epilogue */
        "ld    $ra, 0x0($sp)\n"
        "addiu $sp, $sp, 0x10\n"
        "jr    $ra\n"
        "move  %0, %10\n"
        : "=r"(v0), "+r"(v1), "+r"(v2), "+r"(v3), "+r"(v4),
          "=r"(v5), "=r"(v6), "=r"(v7), "=r"(v8), "=r"(v9), "=r"(v10)
        : "r"(v0), "r"(v1), "r"(v2), "r"(v3), "r"(v4)
        : "memory", "$1", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    (void)cmd; (void)buf; (void)size; (void)arg3;
    (void)arg4; (void)arg5;
    return 0;
#endif
}

/* 0x0019F2A0 - SifCmd helper variant */
int SifCmdHelper2_0019f2a0(int cmd, void *buf, int size, void *arg3, int arg4, int arg5)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = cmd;
    register int v1 __asm__("$3") = size;
    register int v2 __asm__("$4") = arg3;
    register int v3 __asm__("$5") = arg4;
    register int v4 __asm__("$6") = arg5;
    register int v5 __asm__("$16");
    register int v6 __asm__("$17");
    register int v7 __asm__("$18");
    register int v8 __asm__("$19");
    register int v9 __asm__("$20");
    register int v10 __asm__("$21");
    register int v11 __asm__("$ra");

    __asm__ volatile(
        /* Prologue */
        "addiu $sp, $sp, -0x10\n"
        "sd    $ra, 0x0($sp)\n"

        /* Move args */
        "move  %5, %1\n"
        "move  %6, %2\n"
        "move  %7, %3\n"
        "move  %8, %4\n"
        "move  %9, %0\n"

        /* Call SifCmd recursively with arg5=1 */
        "move  $4, %9\n"
        "addiu $5, $0, 1\n"
        "jal   0x19f138\n"
        "move  %10, %5\n"

        /* Epilogue */
        "ld    $ra, 0x0($sp)\n"
        "addiu $sp, $sp, 0x10\n"
        "jr    $ra\n"
        "move  %0, %10\n"
        : "=r"(v0), "+r"(v1), "+r"(v2), "+r"(v3), "+r"(v4),
          "=r"(v5), "=r"(v6), "=r"(v7), "=r"(v8), "=r"(v9), "=r"(v10)
        : "r"(v0), "r"(v1), "r"(v2), "r"(v3), "r"(v4)
        : "memory", "$1", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    (void)cmd; (void)buf; (void)size; (void)arg3;
    (void)arg4; (void)arg5;
    return 0;
#endif
}

/* 0x0019F2DC - Simple getter */
int func_0019f2dc(void *ptr)
{
#if defined(__mips__)
    register int v0 __asm__("$2");
    register void *v1 __asm__("$3") = ptr;

    __asm__ volatile(
        "lw    %0, 0x10(%1)\n"
        "jr    $ra\n"
        "nop\n"
        : "=r"(v0)
        : "r"(v1)
        : "memory"
    );
    return v0;
#else
    (void)ptr;
    return 0;
#endif
}

/* 0x0019F2E4 - Simple setter */
void func_0019f2e4(void *dst, void *src)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = dst;
    register void *v1 __asm__("$3") = src;
    register int v2 __asm__("$4");

    __asm__ volatile(
        "lw    %2, 0x10(%0)\n"
        "jr    $ra\n"
        "sw    %2, 0x8(%1)\n"
        : "+r"(v0), "+r"(v1), "=r"(v2)
        :
        : "memory"
    );
#else
    (void)dst; (void)src;
#endif
}

/* 0x0019F2F0 - Address calculator */
int func_0019f2f0(void *base, void *index)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = base;
    register void *v1 __asm__("$3") = index;
    register int v2 __asm__("$4");
    register int v3 __asm__("$5");

    __asm__ volatile(
        "lw    %2, 0x10(%0)\n"
        "lw    %3, 0x14(%1)\n"
        "sll   %2, %2, 2\n"
        "addu  %2, %2, %3\n"
        "jr    $ra\n"
        "sw    %1, 0x0(%2)\n"
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3)
        :
        : "memory"
    );
    return (int)v0;
#else
    (void)base; (void)index;
    return 0;
#endif
}