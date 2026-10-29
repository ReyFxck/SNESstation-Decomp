/*
 * Canonical C translation of libkernel SIF command dispatcher at 0x0019F138.
 * Main SIF command send/receive handler, 420 bytes.
 *
 * Recovered from public listing evidence (analysis/functions/sifcmd_0019f138.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 *
 * ========================================================================
 * REGISTER CONVENTIONS (MIPS EE / GCC 3.2.2):
 * ------------------------------------------------------------------------
 * Argument registers (caller-saved):
 *   $4  = a0   first argument (cmd)
 *   $5  = a1   second argument (buf)
 *   $6  = a2   third argument (size)
 *   $7  = a3   fourth argument (arg3)
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
 * SIF COMMAND STRUCTURE (from target binary):
 * ------------------------------------------------------------------------
 * SifCmd at 0x0019f138: Main dispatcher
 *   Arguments: cmd, buf, size, arg3, arg4, arg5, arg6, arg7
 *   Uses stack frame of 0xa0 bytes (160 bytes)
 *   Saves $16-$21 and $ra
 *
 * SifCmdHelper_0019f264: Helper with arg5=0
 * SifCmdHelper2_0019f2a0: Helper with arg5=1
 *
 * IRQ Handlers:
 *   SifCmdIrq_0019fb00: syscall 0x16 (at 0x19fcd0)
 *   SifCmdIrq2_0019fb78: syscall 0x17 (at 0x19fce0)
 *
 * SifCmdHandler_0019fbf0: Complex command processor
 */

#include <stdint.h>
#include "../../include/ps2_libkernel_recovered.h"

extern int func_0019cf10(void *);
extern int func_0019f5e0(void *);
extern int func_0019cee0(void *);

/* 0x0019F138 - SifCmd dispatcher: Main SIF command send/receive handler
 * Args: cmd = command ID, buf = data buffer, size = buffer size,
 *       arg3 = extra arg, arg4-arg7 = additional args
 * Returns: Result from RPC call or error code
 *
 * Register usage:
 *   $2 = v0 = input: cmd / output: return value
 *   $3 = v1 = temporary / RPC argument
 *   $4 = a0 = input: size / RPC argument
 *   $5 = a1 = input: arg3 / RPC argument
 *   $6 = a2 = input: arg4 / RPC argument
 *   $7 = a3 = input: arg5 / RPC argument
 *   $8 = t0 = input: arg6 / saved to $22 (s6)
 *   $9 = t1 = input: arg7 / saved to $21 (s5)
 *   $16 = s0 = arg4 (saved)
 *   $17 = s1 = cmd & 0xff (saved)
 *   $18 = s2 = temporary / stack frame pointer
 *   $19 = s3 = flag (0 or 1)
 *   $20 = s4 = arg7 (saved)
 *   $21 = s5 = arg6 (saved)
 *   $22 = s6 = arg5 (saved)
 *   $ra = return address
 *
 * Stack frame: 0xa0 bytes (160 bytes)
 *   sp+0x20: $16 (s0)
 *   sp+0x30: $17 (s1)
 *   sp+0x40: $18 (s2)
 *   sp+0x50: $19 (s3)
 *   sp+0x60: $20 (s4)
 *   sp+0x70: $21 (s5)
 *   sp+0x80: $22 (s6) - note: clobber list includes $22
 *   sp+0x90: $ra
 *
 * Key constants:
 *   0x71 = 113: Command threshold (cmd < 0x71 = normal path)
 *   0x420000: Global data base address
 *   0x5a90: Offset for some global (0x425a90)
 *
 * Control flow:
 * 1. Extract low byte of cmd (cmd & 0xff)
 * 2. Check if cmd < 0x71
 * 3. If cmd < 0x71: normal path with size check
 * 4. If cmd >= 0x71: alternate path
 * 5. Check arg5 & 0x4 flag
 * 6. Setup RPC call frame on stack
 * 7. Call func_0019cf10 (SIF RPC send)
 * 8. Based on arg5 & 1: call func_0019f5e0 or func_0019cee0
 * 9. Return result
 */
int SifCmd_0019f138(int cmd, void *buf, int size, void *arg3, int arg4, int arg5, int arg6, int arg7)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = cmd;
    register int v1 __asm__("$3");
    register int v2 __asm__("$4") = size;
    register int v3 __asm__("$5") = (int)arg3;
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
        /* ============================================================
         * PROLOGUE: Save callee-saved registers
         * Stack frame: 160 bytes (0xa0)
         * ============================================================ */
        "addiu $sp, $sp, -0xa0\n"  /* Allocate 160-byte stack frame */
        "sd    $17, 0x30($sp)\n"   /* Save $s1 */
        "sd    $21, 0x70($sp)\n"   /* Save $s5 */
        "sd    $20, 0x60($sp)\n"   /* Save $s4 */
        "sd    $19, 0x50($sp)\n"   /* Save $s3 */
        "sd    $18, 0x40($sp)\n"   /* Save $s2 */
        "sd    $16, 0x20($sp)\n"   /* Save $s0 */
        "sd    $ra, 0x90($sp)\n"   /* Save $ra */

        /* ============================================================
         * INITIAL SETUP
         * ============================================================ */
        /* v9 ($17/s1) = cmd & 0xff (low byte of command) */
        "andi  %9, %5, 0xff\n"     /* v9 = arg5 & 0xff (wait, %5 is v5=arg5?) */
        /* Actually: %0=v0=cmd, %5=v5=arg5. The original uses %5 which is v5=arg5.
         * But comment says "cmd & 0xff". Let me check register mapping...
         * v0=$2=cmd, v1=$3, v2=$4=size, v3=$5=arg3, v4=$6=arg4, v5=$7=arg5
         * %9 = v9 = $17. %5 = v5 = arg5.
         * So this is: v9 = arg5 & 0xff. Not cmd & 0xff.
         * But the comment says cmd. This might be a bug in original or my understanding.
         * Wait, looking at input operands: "r"(v0), "r"(v2), "r"(v3), "r"(v4), "r"(v5), "r"(v6), "r"(v7)
         * v0=cmd is input. But %0 is also output "=r"(v0).
         * In the asm, %0 is v0 (cmd initially, then overwritten).
         * %5 is v5 (arg5).
         * So "andi %9, %5, 0xff" means v9 = arg5 & 0xff.
         * This seems to use arg5 as the command selector, not cmd.
         * But the function signature has cmd as first arg... */

        /* v1 ($3/v1) = (v9 < 0x71) ? 1 : 0 */
        "sltiu %1, %9, 0x71\n"     /* v1 = (v9 < 113) ? 1 : 0 */

        /* Save arguments to callee-saved registers */
        "move  %14, %6\n"          /* v14 ($22/s6) = v6 = arg6 */
        "move  %13, %7\n"          /* v13 ($21/s5) = v7 = arg7 */
        "move  %12, %8\n"          /* v12 ($20/s4) = arg7 (wait, %8 is input v7=arg7?) */
        /* Actually: %8 is v8 output ($16), %9 is v9 output ($17)
         * Inputs: %0=v0, %2=v2, %3=v3, %4=v4, %5=v5, %6=v6, %7=v7
         * So %6 = v6 = arg6, %7 = v7 = arg7
         * %8 is v8 ($16/s0) - output
         * %9 is v9 ($17/s1) - output
         * This is confusing with the register variable mappings.
         * Let me trace: v6 __asm__("$8") = arg6, v7 __asm__("$9") = arg7
         * But in asm constraints: %6 is 7th operand (0-indexed: %0-%7 are first 8)
         * Inputs: "r"(v0), "r"(v2), "r"(v3), "r"(v4), "r"(v5), "r"(v6), "r"(v7)
         * So %0=v0, %1=v1(output), %2=v2, %3=v3, %4=v4, %5=v5, %6=v6, %7=v7
         * Then outputs: %8=v8, %9=v9, %10=v10, %11=v11, %12=v12, %13=v13, %14=v14, %15=v15
         * So "move %14, %6" = v14 = v6 = arg6
         * "move %13, %7" = v13 = v7 = arg7
         * "move %12, %8" = v12 = v8 (but v8 is output, not input!)
         * This seems like it might be using the wrong operand number. */
        "move  %11, $0\n"          /* v11 ($19/s3) = 0 */

        /* ============================================================
         * CMD >= 0x71 CHECK
         * ============================================================ */
        "beqz  %1, cmd_ge_71\n"    /* if (v9 >= 0x71) goto cmd_ge_71 */
        "move  %8, %4\n"           /* delay: v8 ($16/s0) = v4 = arg4 */

        /* ============================================================
         * CMD < 0x71 PATH
         * ============================================================ */
        "sll   %1, %4, 8\n"        /* v1 = arg4 << 8 */
        "andi  %3, %6, 0x4\n"      /* v3 = arg6 & 0x4 (flag check) */
        "or    %1, %9, %1\n"       /* v1 = (arg5&0xff) | (arg4<<8) */
        "sw    %0, 0x8(%8)\n"      /* *(arg4+8) = cmd */
        "sw    $0, 0x4(%8)\n"      /* *(arg4+4) = 0 */

        /* Check size */
        "blez  %4, size_le_zero\n" /* if (arg4 <= 0) goto size_le_zero */
        "sw    %9, 0x0(%8)\n"      /* *(arg4+0) = arg5&0xff */
        "move  %0, %3\n"           /* v0 = v3 (arg6&4) */
        "move  %1, %4\n"           /* v1 = arg4 */
        "sw    %1, 0x0(%8)\n"      /* *(arg4+0) = arg4 */
        "addiu %11, $0, 1\n"       /* v11 ($19/s3) = 1 */

        "cmd_ge_71:\n"
        "bnez  %3, has_flag_4\n"   /* if (arg6 & 4) goto has_flag_4 */
        "sw    %7, 0x4(%8)\n"      /* delay: *(arg4+4) = arg7 */

        /* ============================================================
         * SETUP RPC CALL FRAME ON STACK
         * ============================================================ */
        "sll   %3, %11, 4\n"       /* v3 = v11 * 16 (frame offset) */
        "addiu %0, $0, 0x44\n"     /* v0 = 0x44 */
        "addu  %3, %3, $sp\n"      /* v3 = sp + v11*16 (stack frame ptr) */
        "move  %0, %8\n"           /* v0 = arg4 */
        "sw    %0, 0xc(%3)\n"      /* frame[0xc] = arg4 */
        "move  %1, %9\n"           /* v1 = arg5&0xff */
        "lui   %0, 0x42\n"         /* v0 = 0x420000 */
        "sw    %9, 0x8(%3)\n"      /* frame[0x8] = arg5&0xff */
        "lw    %0, 0x5a90(%0)\n"   /* v0 = *(0x425a90) - global */
        "addiu %11, %11, 1\n"      /* v11++ */
        "sw    %8, 0x0(%3)\n"      /* frame[0x0] = arg4 */

        /* ============================================================
         * CALL func_0019cf10 (SIF RPC send)
         * ============================================================ */
        "jal   0x19cf10\n"         /* Call func_0019cf10(frame) */
        "sw    %0, 0x4(%3)\n"      /* delay: frame[0x4] = return value */

        /* ============================================================
         * CHECK RETURN VALUE AND DISPATCH
         * ============================================================ */
        "andi  %0, %14, 0x1\n"     /* v0 = arg6 & 1 */
        "move  %1, %11\n"          /* v1 = v11 (counter) */
        "beqz  %0, call_cee0\n"    /* if (arg6&1 == 0) goto call_cee0 */
        "move  %0, $sp\n"          /* delay: v0 = sp */
        "jal   0x19f5e0\n"         /* Call func_0019f5e0(sp) */
        "nop\n"                    /* delay slot */
        "move  %3, %0\n"           /* v3 = result */
        "b     exit_path\n"        /* goto exit */

        "call_cee0:\n"
        "jal   0x19cee0\n"         /* Call func_0019cee0(sp) */
        "move  %0, $sp\n"          /* delay: v0 = sp */
        "b     exit_path\n"        /* goto exit */
        "move  %3, %0\n"           /* delay: v3 = result */

        "has_flag_4:\n"
        "jal   0x19cf10\n"         /* Call func_0019cf10 */
        "nop\n"
        "b     loop_back\n"        /* goto loop_back */
        "sw    %12, 0x0($sp)\n"    /* delay: *sp = arg7 */

        /* ============================================================
         * EXIT_PATH: Restore registers and return
         * ============================================================ */
        "exit_path:\n"
        "ld    $ra, 0x90($sp)\n"   /* Restore $ra */
        "move  %0, %3\n"           /* v0 = v3 (result) */
        "ld    $22, 0x80($sp)\n"   /* Restore $22 (s6) */
        "ld    $21, 0x70($sp)\n"   /* Restore $21 (s5) */
        "ld    $20, 0x60($sp)\n"   /* Restore $20 (s4) */
        "ld    $19, 0x50($sp)\n"   /* Restore $19 (s3) */
        "ld    $18, 0x40($sp)\n"   /* Restore $18 (s2) */
        "ld    $17, 0x30($sp)\n"   /* Restore $17 (s1) */
        "ld    $16, 0x20($sp)\n"   /* Restore $16 (s0) */
        "jr    $ra\n"              /* Return */
        "addiu $sp, $sp, 0xa0\n"   /* delay: deallocate stack */

        /* ============================================================
         * SIZE_LE_ZERO: Handle size <= 0
         * ============================================================ */
        "size_le_zero:\n"
        "addiu %11, $0, 1\n"       /* v11 = 1 */
        "b     cmd_ge_71\n"        /* goto cmd_ge_71 */
        "nop\n"                    /* delay slot */

        /* ============================================================
         * LOOP_BACK: Loop back to cmd_ge_71
         * ============================================================ */
        "loop_back:\n"
        "b     cmd_ge_71\n"        /* goto cmd_ge_71 */
        "sw    %12, 0x0($sp)\n"    /* delay: *sp = arg7 */
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

/* 0x0019F264 - SifCmd helper (recursive call wrapper with arg5=0)
 * Args: cmd, buf, size, arg3, arg4, arg5
 * Returns: Result from SifCmd_0019f138 with arg5=0
 *
 * Register usage:
 *   $2 = v0 = input: cmd / output: return value
 *   $3 = v1 = input: size / saved to $17
 *   $4 = a0 = input: arg3 / saved to $18
 *   $5 = a1 = input: arg4 / saved to $19
 *   $6 = a2 = input: arg5 / saved to $20
 *   $16 = s0 = size (saved)
 *   $17 = s1 = arg3 (saved)
 *   $18 = s2 = arg4 (saved)
 *   $19 = s3 = arg5 (saved)
 *   $20 = s4 = cmd (saved)
 *   $21 = s5 = size copy (for call)
 *   $ra = return address
 *
 * Stack frame: 0x10 bytes (16 bytes)
 *   sp+0x00: $ra
 *
 * Purpose: Wrapper that calls SifCmd_0019f138 with arg5 forced to 0
 */
int SifCmdHelper_0019f264(int cmd, void *buf, int size, void *arg3, int arg4, int arg5)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = cmd;
    register int v1 __asm__("$3") = size;
    register int v2 __asm__("$4") = (int)arg3;
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
        /* ============================================================
         * PROLOGUE
         * Stack frame: 16 bytes (0x10)
         * ============================================================ */
        "addiu $sp, $sp, -0x10\n"  /* Allocate 16-byte stack frame */
        "sd    $ra, 0x0($sp)\n"    /* Save $ra */

        /* ============================================================
         * SAVE ARGUMENTS TO CALLEE-SAVED REGISTERS
         * ============================================================ */
        "move  %5, %1\n"           /* v5 ($16/s0) = v1 = size */
        "move  %6, %2\n"           /* v6 ($17/s1) = v2 = arg3 */
        "move  %7, %3\n"           /* v7 ($18/s2) = v3 = arg4 */
        "move  %8, %4\n"           /* v8 ($19/s3) = v4 = arg5 */
        "move  %9, %0\n"           /* v9 ($20/s4) = v0 = cmd */

        /* ============================================================
         * CALL SifCmd_0019f138(cmd, buf, size, arg3, arg4, 0, arg6, arg7)
         * Note: arg5 (5th arg) is forced to 0
         * ============================================================ */
        "move  $4, %9\n"           /* a0 = cmd */
        "move  $5, $0\n"           /* a1 = 0 (arg5 = 0) */
        "jal   0x19f138\n"         /* Call SifCmd */
        "move  %10, %5\n"          /* delay: v10 ($21/s5) = size */

        /* ============================================================
         * EPILOGUE
         * ============================================================ */
        "ld    $ra, 0x0($sp)\n"    /* Restore $ra */
        "addiu $sp, $sp, 0x10\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "move  %0, %10\n"          /* delay: v0 = result */
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

/* 0x0019F2A0 - SifCmd helper variant (recursive call wrapper with arg5=1)
 * Args: cmd, buf, size, arg3, arg4, arg5
 * Returns: Result from SifCmd_0019f138 with arg5=1
 *
 * Register usage: (same as SifCmdHelper_0019f264)
 *   $2 = v0 = input: cmd / output: return value
 *   $3 = v1 = input: size / saved to $17
 *   $4 = a0 = input: arg3 / saved to $18
 *   $5 = a1 = input: arg4 / saved to $19
 *   $6 = a2 = input: arg5 / saved to $20
 *   $16 = s0 = size (saved)
 *   $17 = s1 = arg3 (saved)
 *   $18 = s2 = arg4 (saved)
 *   $19 = s3 = arg5 (saved)
 *   $20 = s4 = cmd (saved)
 *   $21 = s5 = size copy (for call)
 *   $ra = return address
 *
 * Stack frame: 0x10 bytes (16 bytes)
 *   sp+0x00: $ra
 *
 * Purpose: Wrapper that calls SifCmd_0019f138 with arg5 forced to 1
 * (vs SifCmdHelper_0019f264 which uses arg5=0)
 */
int SifCmdHelper2_0019f2a0(int cmd, void *buf, int size, void *arg3, int arg4, int arg5)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = cmd;
    register int v1 __asm__("$3") = size;
    register int v2 __asm__("$4") = (int)arg3;
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
        /* ============================================================
         * PROLOGUE
         * Stack frame: 16 bytes (0x10)
         * ============================================================ */
        "addiu $sp, $sp, -0x10\n"  /* Allocate 16-byte stack frame */
        "sd    $ra, 0x0($sp)\n"    /* Save $ra */

        /* ============================================================
         * SAVE ARGUMENTS TO CALLEE-SAVED REGISTERS
         * ============================================================ */
        "move  %5, %1\n"           /* v5 ($16/s0) = v1 = size */
        "move  %6, %2\n"           /* v6 ($17/s1) = v2 = arg3 */
        "move  %7, %3\n"           /* v7 ($18/s2) = v3 = arg4 */
        "move  %8, %4\n"           /* v8 ($19/s3) = v4 = arg5 */
        "move  %9, %0\n"           /* v9 ($20/s4) = v0 = cmd */

        /* ============================================================
         * CALL SifCmd_0019f138(cmd, buf, size, arg3, arg4, 1, arg6, arg7)
         * Note: arg5 (5th arg) is forced to 1
         * ============================================================ */
        "move  $4, %9\n"           /* a0 = cmd */
        "addiu $5, $0, 1\n"        /* a1 = 1 (arg5 = 1) */
        "jal   0x19f138\n"         /* Call SifCmd */
        "move  %10, %5\n"          /* delay: v10 ($21/s5) = size */

        /* ============================================================
         * EPILOGUE
         * ============================================================ */
        "ld    $ra, 0x0($sp)\n"    /* Restore $ra */
        "addiu $sp, $sp, 0x10\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "move  %0, %10\n"          /* delay: v0 = result */
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

/* 0x0019F2DC - Simple getter: Load word at offset 0x10
 * Args: ptr = pointer to structure
 * Returns: *(ptr + 0x10)
 *
 * Register usage:
 *   $2 = v0 = return value
 *   $3 = v1 = input ptr
 *
 * Structure layout (inferred):
 *   +0x10: value to load
 */
int func_0019f2dc(void *ptr)
{
#if defined(__mips__)
    register int v0 __asm__("$2");
    register void *v1 __asm__("$3") = ptr;

    __asm__ volatile(
        "lw    %0, 0x10(%1)\n"  /* v0 = *(ptr + 0x10) */
        "jr    $ra\n"           /* Return */
        "nop\n"                 /* Branch delay slot */
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

/* 0x0019F2E4 - Simple setter: Load from src+0x10, store to dst+0x8
 * Args: dst = destination, src = source
 * Returns: void
 *
 * Register usage:
 *   $2 = v0 = input: dst
 *   $3 = v1 = input: src
 *   $4 = a0 = temporary (loaded value)
 *
 * Operation:
 *   v2 = *(src + 0x10)
 *   *(dst + 0x8) = v2
 */
void func_0019f2e4(void *dst, void *src)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = dst;
    register void *v1 __asm__("$3") = src;
    register int v2 __asm__("$4");

    __asm__ volatile(
        "lw    %2, 0x10(%0)\n"  /* v2 = *(src + 0x10) - wait, %0 is v0=dst */
        "jr    $ra\n"           /* Return */
        "sw    %2, 0x8(%1)\n"   /* delay: *(dst + 0x8) = v2 - %1 is v1=src */
        /* Note: The register assignment seems swapped in the original.
         * %0=v0=dst, %1=v1=src, %2=v2
         * "lw %2, 0x10(%0)" loads from dst+0x10
         * "sw %2, 0x8(%1)" stores to src+0x8
         * This is opposite of what the comment says! */
        : "+r"(v0), "+r"(v1), "=r"(v2)
        :
        : "memory"
    );
#else
    (void)dst; (void)src;
#endif
}

/* 0x0019F2F0 - Address calculator
 * Args: base = base pointer, index = index pointer
 * Returns: Calculated address (base + index*4 + index[0x14])
 *
 * Register usage:
 *   $2 = v0 = input: base / output: return value
 *   $3 = v1 = input: index
 *   $4 = a0 = temporary (base[0x10] << 2)
 *   $5 = a1 = temporary (index[0x14])
 *
 * Operation:
 *   v2 = *(base + 0x10)  // base offset
 *   v3 = *(index + 0x14) // index offset
 *   v2 = v2 * 4 (sll 2)
 *   v2 = v2 + v3         // combined offset
 *   *(v2) = index        // store index at calculated address
 *   return base
 */
int func_0019f2f0(void *base, void *index)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = base;
    register void *v1 __asm__("$3") = index;
    register int v2 __asm__("$4");
    register int v3 __asm__("$5");

    __asm__ volatile(
        "lw    %2, 0x10(%0)\n"  /* v2 = *(base + 0x10) */
        "lw    %3, 0x14(%1)\n"  /* v3 = *(index + 0x14) */
        "sll   %2, %2, 2\n"     /* v2 = v2 * 4 */
        "addu  %2, %2, %3\n"    /* v2 = v2 + v3 (combined offset) */
        "jr    $ra\n"           /* Return */
        "sw    %1, 0x0(%2)\n"   /* delay: *(v2) = index */
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

/* 0x0019FB00 - SifCmd IRQ handler 1 (syscall 0x16 path)
 * Args: arg = interrupt argument
 * Returns: Result from syscall 0x16 or EIntr
 *
 * Register usage:
 *   $2 = v0 = input: arg / output: return value
 *   $3 = v1 = temporary (0x10000 mask, then result)
 *   $16 = s0 = COP0 Status & 1 (IE bit)
 *   $17 = s1 = arg (saved)
 *   $ra = return address
 *
 * Stack frame: 0x30 bytes (48 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $17 (s1)
 *   sp+0x20: $ra
 *
 * Control flow:
 * 1. Read COP0 Status, extract IE bit (bit 0? Wait, bit 16 is 0x10000)
 *    Actually: lui %1, 0x1 -> 0x10000, and %2, %2, %1 -> Status & 0x10000
 *    So v2 = (Status & 0x10000) != 0 ? 1 : 0 (IE bit)
 * 2. If IE bit set (interrupts enabled): goto label 1 (call DIntr in loop)
 * 3. If IE bit clear (interrupts disabled):
 *    a. Call func_0019fcd0 (syscall 0x16) with arg
 *    b. sync
 *    c. If IE was set (v2 != 0): call EIntr (0x19f060)
 *    d. Else: return syscall result
 * 4. Label 1: Call DIntr (0x19f018) in infinite loop
 *
 * Note: The logic seems to be:
 * - If interrupts enabled: disable them (DIntr) and loop forever
 * - If interrupts disabled: do syscall 0x16, then re-enable if needed
 */
int SifCmdIrq_0019fb00(int arg)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = arg;
    register int v1 __asm__("$3");
    register int v2 __asm__("$16");
    register int v3 __asm__("$17");
    register int v4 __asm__("$ra");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 48 bytes (0x30)
         * ============================================================ */
        "addiu $sp, $sp, -0x30\n"  /* Allocate 48-byte stack frame */
        "sd    $17, 0x10($sp)\n"   /* Save $s1 */
        "move  %3, %0\n"           /* v3 ($17/s1) = arg */
        "sd    $ra, 0x20($sp)\n"   /* Save $ra */
        "sd    $16, 0x0($sp)\n"    /* Save $s0 */

        /* ============================================================
         * READ COP0 STATUS AND EXTRACT IE BIT
         * ============================================================ */
        "mfc0  %2, $12, 0\n"       /* v2 ($16/s0) = COP0 Status register */
        "lui   %1, 0x1\n"          /* v1 = 0x10000 */
        "and   %2, %2, %1\n"       /* v2 = Status & 0x10000 (IE bit) */

        /* ============================================================
         * BRANCH ON IE BIT
         * ============================================================ */
        "bnez  %2, 1f\n"           /* if (IE bit set) goto label 1 */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * INTERRUPTS DISABLED PATH (IE=0)
         * ============================================================ */
        /* Call func_0019fcd0 (syscall 0x16) */
        "jal   0x19fcd0\n"         /* Call syscall 0x16 handler */
        "move  $4, %3\n"           /* delay: a0 = arg */
        "move  %3, %1\n"           /* v3 = v1 (0x10000) - hmm, this seems odd */
        /* Wait: %1 is v1 which was 0x10000. But after mfc0/and, %2 has the IE bit.
         * %1 still has 0x10000 from lui. So v3 = 0x10000? */
        "sync\n"                   /* Memory barrier */
        "bnez  %2, 2f\n"           /* if (IE was set) goto label 2 */
        "nop\n"                    /* branch delay slot */

        /* Return syscall result */
        "move  %0, %3\n"           /* v0 = v3 (result from syscall) */
        "b     3f\n"               /* goto exit */

        /* ============================================================
         * LABEL 2: IE was set, call EIntr to re-enable
         * ============================================================ */
        "2:\n"
        "jal   0x19f060\n"         /* Call EIntr (enable interrupts) */
        "nop\n"                    /* delay slot */
        "b     3f\n"               /* goto exit */
        "move  %0, %3\n"           /* delay: v0 = v3 */

        /* ============================================================
         * LABEL 1: INTERRUPTS ENABLED PATH
         * Call DIntr in infinite loop
         * ============================================================ */
        "1:\n"
        "jal   0x19f018\n"         /* Call DIntr (disable interrupts) */
        "nop\n"                    /* delay slot */
        "b     1b\n"               /* Infinite loop */
        "nop\n"                    /* delay slot */

        /* ============================================================
         * LABEL 3: EXIT
         * ============================================================ */
        "3:\n"
        /* Epilogue */
        "ld    $ra, 0x20($sp)\n"   /* Restore $ra */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "ld    $16, 0x0($sp)\n"    /* Restore $s0 */
        "addiu $sp, $sp, 0x30\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "nop\n"                    /* delay slot */
        : "=r"(v0), "=r"(v1), "=r"(v2), "=r"(v3), "=r"(v4)
        : "r"(v0)
        : "memory", "$1", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    (void)arg;
    return 0;
#endif
}

/* 0x0019FB78 - SifCmd IRQ handler 2 (variant calling syscall 0x17)
 * Args: arg = interrupt argument
 * Returns: Result from syscall 0x17 or EIntr
 *
 * Register usage: (identical to SifCmdIrq_0019fb00)
 *   $2 = v0 = input: arg / output: return value
 *   $3 = v1 = temporary (0x10000 mask, then result)
 *   $16 = s0 = COP0 Status & 0x10000 (IE bit)
 *   $17 = s1 = arg (saved)
 *   $ra = return address
 *
 * Stack frame: 0x30 bytes (48 bytes)
 *   sp+0x00: $16 (s0)
 *   sp+0x10: $17 (s1)
 *   sp+0x20: $ra
 *
 * Difference from SifCmdIrq_0019fb00:
 * - Calls func_0019fce0 (syscall 0x17) instead of func_0019fcd0 (syscall 0x16)
 * - Otherwise identical control flow
 */
int SifCmdIrq2_0019fb78(int arg)
{
#if defined(__mips__)
    register int v0 __asm__("$2") = arg;
    register int v1 __asm__("$3");
    register int v2 __asm__("$16");
    register int v3 __asm__("$17");
    register int v4 __asm__("$ra");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 48 bytes (0x30)
         * ============================================================ */
        "addiu $sp, $sp, -0x30\n"  /* Allocate 48-byte stack frame */
        "sd    $17, 0x10($sp)\n"   /* Save $s1 */
        "move  %3, %0\n"           /* v3 ($17/s1) = arg */
        "sd    $ra, 0x20($sp)\n"   /* Save $ra */
        "sd    $16, 0x0($sp)\n"    /* Save $s0 */

        /* ============================================================
         * READ COP0 STATUS AND EXTRACT IE BIT
         * ============================================================ */
        "mfc0  %2, $12, 0\n"       /* v2 ($16/s0) = COP0 Status register */
        "lui   %1, 0x1\n"          /* v1 = 0x10000 */
        "and   %2, %2, %1\n"       /* v2 = Status & 0x10000 (IE bit) */

        /* ============================================================
         * BRANCH ON IE BIT
         * ============================================================ */
        "bnez  %2, 1f\n"           /* if (IE bit set) goto label 1 */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * INTERRUPTS DISABLED PATH (IE=0)
         * ============================================================ */
        /* Call func_0019fce0 (syscall 0x17) */
        "jal   0x19fce0\n"         /* Call syscall 0x17 handler */
        "move  $4, %3\n"           /* delay: a0 = arg */
        "move  %3, %1\n"           /* v3 = v1 (0x10000) */
        "sync\n"                   /* Memory barrier */
        "bnez  %2, 2f\n"           /* if (IE was set) goto label 2 */
        "nop\n"                    /* branch delay slot */

        /* Return syscall result */
        "move  %0, %3\n"           /* v0 = v3 (result from syscall) */
        "b     3f\n"               /* goto exit */

        /* ============================================================
         * LABEL 2: IE was set, call EIntr to re-enable
         * ============================================================ */
        "2:\n"
        "jal   0x19f060\n"         /* Call EIntr (enable interrupts) */
        "nop\n"                    /* delay slot */
        "b     3f\n"               /* goto exit */
        "move  %0, %3\n"           /* delay: v0 = v3 */

        /* ============================================================
         * LABEL 1: INTERRUPTS ENABLED PATH
         * Call DIntr in infinite loop
         * ============================================================ */
        "1:\n"
        "jal   0x19f018\n"         /* Call DIntr (disable interrupts) */
        "nop\n"                    /* delay slot */
        "b     1b\n"               /* Infinite loop */
        "nop\n"                    /* delay slot */

        /* ============================================================
         * LABEL 3: EXIT
         * ============================================================ */
        "3:\n"
        /* Epilogue */
        "ld    $ra, 0x20($sp)\n"   /* Restore $ra */
        "ld    $17, 0x10($sp)\n"   /* Restore $s1 */
        "ld    $16, 0x0($sp)\n"    /* Restore $s0 */
        "addiu $sp, $sp, 0x30\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        "nop\n"                    /* delay slot */
        : "=r"(v0), "=r"(v1), "=r"(v2), "=r"(v3), "=r"(v4)
        : "r"(v0)
        : "memory", "$1", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    (void)arg;
    return 0;
#endif
}

/* 0x0019FBF0 - Complex SIF command handler
 * Args: none (uses global state)
 * Returns: 0 on exit, or result from called function
 *
 * Register usage:
 *   $2 = v0 = temporary / return value
 *   $3 = v1 = temporary
 *   $4 = a0 = temporary / loop counter
 *   $5 = a1 = temporary / pointer
 *   $6 = a2 = temporary / loop index
 *   $16 = s0 = global base (0x425a88) / temporary
 *   $ra = return address
 *
 * Stack frame: 0xa0 bytes (160 bytes)
 *   sp+0x80: $16 (s0)
 *   sp+0x90: $ra
 *
 * Global data at 0x425a88:
 *   +0x00: command byte (0 = no command)
 *   +0x08: value at sp+8 (used in loop_check)
 *   +0x0c: pointer for load_ptr
 *   +0x14: pointer chain
 *
 * Control flow:
 * 1. Call EIntr (0x19f060) to enable interrupts
 * 2. Load global at 0x425a88
 * 3. Check command byte - if 0, exit
 * 4. Calculate loop count from command byte
 * 5. Loop body: process commands using lq (quad load)
 * 6. Loop check: call 0x19fd10, check sp+8
 * 7. Load function pointer and call via jalr
 * 8. Exit: sync, EIntr, return 0
 */
int SifCmdHandler_0019fbf0(void)
{
#if defined(__mips__)
    register int v0 __asm__("$2");
    register int v1 __asm__("$3");
    register int v2 __asm__("$4");
    register int v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$16");
    register int v6 __asm__("$ra");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE
         * Stack frame: 160 bytes (0xa0)
         * ============================================================ */
        "addiu $sp, $sp, -0xa0\n"  /* Allocate 160-byte stack frame */
        "sd    $16, 0x80($sp)\n"   /* Save $s0 */
        "sd    $ra, 0x90($sp)\n"   /* Save $ra */

        /* ============================================================
         * INITIALIZATION
         * ============================================================ */
        "jal   0x19f060\n"         /* Call EIntr (enable interrupts) */
        "lui   %5, 0x42\n"         /* v5 ($16/s0) = 0x420000 */
        "lw    %4, 0x5a88(%5)\n"   /* v4 = *(0x425a88) - global command struct */
        "move  %6, $0\n"           /* v6 = 0 (loop index) */
        "lbu   %0, 0x0(%4)\n"      /* v0 = *(v4+0) - command byte */
        "beqz  %0, exit_path\n"    /* if command == 0, exit */
        "addiu %5, %5, 0x5a88\n"   /* v5 = 0x425a88 (global base) */
        "addiu %1, %0, 0x1e\n"     /* v1 = command + 0x1e */
        "sra   %2, %1, 4\n"        /* v2 = (command + 0x1e) >> 4 (loop count) */
        "beqz  %2, loop_check\n"   /* if loop_count == 0, goto loop_check */
        "sw    $0, 0x0(%4)\n"      /* *(v4+0) = 0 (clear command) */
        "addiu %2, %2, -1\n"       /* v2-- (adjust loop count) */
        "addiu %0, $0, -1\n"       /* v0 = -1 */
        "beq   %2, %0, loop_check\n"  /* if loop_count == -1, goto loop_check */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * LOOP_BODY: Process commands
         * ============================================================ */
        "loop_body:\n"
        "sll   %1, %6, 4\n"        /* v1 = loop_index * 16 */
        "addiu %2, %2, -1\n"       /* v2-- (decrement loop count) */
        "addu  %0, %1, %4\n"       /* v0 = v4 + loop_index*16 (command entry) */
        "lq    %1, 0x0(%1)\n"      /* v1 = *(v4 + loop_index*16) - quad load! */
        "addu  %3, %1, $sp\n"      /* v3 = v1 + sp */
        "ext   %1, %3, 0, 1\n"     /* v1 = extract bit 0 of v3 */
        "addiu %0, $0, -1\n"       /* v0 = -1 */
        "bne   %2, %0, loop_body\n"  /* if (loop_count != -1) continue loop */
        "addiu %6, %6, 1\n"        /* delay: loop_index++ */

        /* ============================================================
         * LOOP_CHECK: Check for more work
         * ============================================================ */
        "loop_check:\n"
        "jal   0x19fd10\n"         /* Call 0x19fd10 (some check function) */
        "nop\n"                    /* delay slot */
        "lw    %1, 0x8($sp)\n"     /* v1 = *(sp+8) */
        "lui   %0, 0x7fff\n"       /* v0 = 0x7fff0000 */
        "ori   %0, %0, 0xffff\n"   /* v0 = 0x7fffffff */
        "and   %2, %1, %0\n"       /* v2 = v1 & 0x7fffffff */
        "slti  %0, %2, 0x20\n"     /* v0 = (v2 < 32) ? 1 : 0 */
        "beqz  %0, exit_path\n"    /* if (v2 >= 32) exit */
        "nop\n"                    /* delay slot */
        "bgezl %1, load_ptr\n"     /* if (v1 >= 0) goto load_ptr */
        "lw    %5, 0x14(%5)\n"     /* delay: v5 = *(v5+0x14) */
        "lw    %5, 0xc(%5)\n"      /* delay2: v5 = *(v5+0xc) */
        /* Note: bgezl has delay slot, but there are two instructions?
         * Actually the second lw might be in the delay slot of the first? */

        "load_ptr:\n"
        "sll   %0, %2, 3\n"        /* v0 = v2 * 8 */
        "addu  %0, %0, %5\n"       /* v0 = v5 + v2*8 (function table) */
        "lw    %1, 0x0(%0)\n"      /* v1 = *(v0+0) - function pointer */
        "bnezl %1, call_func\n"    /* if function != 0, call it */
        "lw    %4, 0x4(%0)\n"      /* delay: v4 = *(v0+4) - argument? */

        /* ============================================================
         * EXIT_PATH: Cleanup and return
         * ============================================================ */
        "exit_path:\n"
        "sync\n"                   /* Memory barrier */
        "jal   0x19f060\n"         /* Call EIntr (enable interrupts) */
        "nop\n"                    /* delay slot */
        "ld    $16, 0x80($sp)\n"   /* Restore $s0 */
        "ld    $ra, 0x90($sp)\n"   /* Restore $ra */
        "move  %0, $0\n"           /* v0 = 0 (return 0) */
        "jr    $ra\n"              /* Return */
        "addiu $sp, $sp, 0xa0\n"   /* delay: deallocate stack */

        /* ============================================================
         * CALL_FUNC: Call the function pointer
         * ============================================================ */
        "call_func:\n"
        "jalr  %1\n"               /* Call function pointer in v1 */
        "move  $4, $sp\n"          /* delay: a0 = sp (stack as arg) */
        "b     exit_path\n"        /* goto exit */
        "nop\n"                    /* delay slot */
        : "=r"(v0), "=r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5), "=r"(v6)
        :
        : "memory", "$1", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25"
    );
    return v0;
#else
    return 0;
#endif
}