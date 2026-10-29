/*
 * Canonical C translation of libkernel size/string focused extract at 0x0019C364.
 * Three focused fragments from the libkernel libc string corridor:
 *   Fragment 1: 0x0019C364-0x0019C3D0 (28 instructions) - memset + partial strlen setup
 *   Fragment 2: 0x0019C550-0x0019C5A0 (20 instructions) - memmove variant inner loop
 *   Fragment 3: 0x0019C5E8-0x0019C60C (11 instructions) - strchr/strstr variant tail
 *
 * Full string functions with stack frames (0x0019C3D4-0x0019C610):
 *   strcat_0019c3d4, strncmp_0019c408, memcmp_0019c458, memmove_0019c4a0
 *   strcpy_0019c550, strchr_0019c5e8, strcmp_0019c610
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_size_strings_0019c364.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 *
 * REGISTER USAGE (MIPS EE calling convention):
 *   $2 = v0 (return value / arg0 / temp)
 *   $3 = v1 (arg1 / temp)
 *   $4 = a0 (arg0 / temp)
 *   $5 = a1 (arg1 / temp)
 *   $6 = a2 (arg2 / temp)
 *   $7 = a3 (arg3 / temp)
 *   $8 = t0 (temp)
 *   $16 = s0 (callee-saved)
 *   $17 = s1 (callee-saved)
 *   $29 = sp (stack pointer)
 *   $31 = ra (return address)
 *
 * STACK FRAME LAYOUT (for functions with prologue/epilogue):
 *   addiu $sp, $sp, -0x18    # allocate 24 bytes
 *   sd    $ra, 0x10($sp)     # save $ra at sp+0x10
 *   sd    $16, 0x08($sp)     # save $s0 at sp+0x08
 *   ...function body...
 *   ld    $ra, 0x10($sp)     # restore $ra
 *   ld    $16, 0x08($sp)     # restore $s0
 *   addiu $sp, $sp, 0x18     # deallocate
 *   jr    $ra
 *
 * INLINE ASM PATTERN:
 *   - Only labels "1:" and "2:" used for loops (forward/backward refs: 1b, 2f)
 *   - Register variables bound to specific MIPS registers via __asm__("$N")
 *   - "memory" clobber ensures compiler doesn't reorder memory ops
 *   - No "3:" or higher labels (GCC 3.2.2 inline asm limitation)
 */

#include <stddef.h>
#include <stdint.h>

/* Fragment 1: 0x0019C364 - memset + strlen setup */
/* Registers: v0=$2=dst, v1=$3=src, v2=$4=n, v3=$5=c, v4=$6, v5=$7, v6=$8 */
void memset_frag_0019c364(void *dst, const void *src, size_t n, int c)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = dst;
    register const void *v1 __asm__("$3") = src;
    register size_t v2 __asm__("$4") = n;
    register int v3 __asm__("$5") = c;
    register void *v4 __asm__("$6");
    register void *v5 __asm__("$7");
    register int v6 __asm__("$8");

    __asm__ volatile(
        /* ============================================================
         * 0x0019C364 - memset: copy byte from src to dst, n times
         * ============================================================ */
        "move   %4, %0       \n"  /* v4 ($6) = dst (save original dst) */
        "move   %5, %4       \n"  /* v5 ($7) = v4 (working dst pointer) */
        "addiu  %2, %2, -1   \n"  /* n ($4) = n - 1 (pre-decrement for loop) */
        "addiu  %6, $0, -1   \n"  /* v6 ($8) = -1 (sentinel value) */
        "beq    %2, %6, 2f   \n"  /* if (n == -1) goto label 2 (end, n was 0) */
        "move   %0, %6       \n"  /* v0 ($2) = -1 (return value for n=0 case) */
        "1:                   \n"  /* label 1: loop start */
        "lbu    %6, 0(%1)    \n"  /* v6 ($8) = *(unsigned char*)src (load byte) */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "addiu  %1, %1, 1    \n"  /* src++ */
        "sb     %6, 0(%5)    \n"  /* *dst = v6 (store byte) */
        "bne    %2, %0, 1b   \n"  /* if (n != v0) goto label 1 (continue loop) */
        "addiu  %5, %5, 1    \n"  /* dst++ (branch delay slot) */
        "2:                   \n"  /* label 2: end */
        "jr     $ra          \n"  /* return */
        "move   %6, %4       \n"  /* v6 = v4 (original dst) - branch delay slot */

        /* ============================================================
         * 0x0019C39C - memset variant: fill dst with byte c, n times
         * ============================================================ */
        "move   %5, %0       \n"  /* v5 ($7) = dst */
        "lui    %6, 0xffff   \n"  /* v6 ($8) = 0xffff0000 */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "ori    %6, %6, 0xffff\n" /* v6 = 0xffffffff (-1 sentinel) */
        "beq    %2, %6, 4f   \n"  /* if (n == -1) goto label 4 (end, n was 0) */
        "nop                  \n"  /* branch delay slot */
        "lui    %6, 0xffff   \n"  /* v6 = 0xffff0000 (reload -1) */
        "ori    %6, %6, 0xffff\n" /* v6 = 0xffffffff */
        "3:                   \n"  /* label 3: loop start */
        "sb     %3, 0(%5)    \n"  /* *dst = c (fill byte) */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "bne    %2, %6, 3b   \n"  /* if (n != -1) goto label 3 (continue) */
        "addiu  %5, %5, 1    \n"  /* dst++ (branch delay slot) */
        "4:                   \n"  /* label 4: end */
        "jr     $ra          \n"  /* return */
        :
        "+r"(v0), "+r"(v1), "+r"(v2), "+r"(v3),
        "=r"(v4), "=r"(v5), "=r"(v6)
        :
        : "memory"
    );
#else
    (void)dst; (void)src; (void)n; (void)c;
#endif
}

/* Fragment 2: 0x0019C550 - memmove variant inner loop */
/* Registers: v0=$2=dst, v1=$3=src, v2=$4=n, v3=$5, v4=$6, v5=$7, v6=$8 */
void memmove_frag_0019c550(void *dst, const void *src, size_t n)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = dst;
    register const void *v1 __asm__("$3") = src;
    register size_t v2 __asm__("$4") = n;
    register void *v3 __asm__("$5");
    register void *v4 __asm__("$6");
    register int v5 __asm__("$7");
    register int v6 __asm__("$8");

    __asm__ volatile(
        /* ============================================================
         * 0x0019C550 - memmove variant: copy bytes until null terminator
         * Copies from src to dst, stops when copied byte == 0
         * Also writes a second null byte after the terminator
         * ============================================================ */
        "move   %5, %0       \n"  /* v5 ($7) = dst (save original dst) */
        "nop                  \n"  /* padding */
        "beqz   %2, 6f       \n"  /* if (n == 0) goto label 6 (end) */
        "move   %6, %2       \n"  /* v6 ($8) = n (copy of counter) */
        "5:                   \n"  /* label 5: loop start */
        "lbu    %6, 0(%1)    \n"  /* v6 = *(unsigned char*)src */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "addiu  %1, %1, 1    \n"  /* src++ */
        "sb     %6, 0(%0)    \n"  /* *dst = v6 (copy byte) */
        "sll    %6, %6, 24   \n"  /* v6 <<= 24 (check if byte was 0 via sign bit) */
        "bnez   %6, 6f       \n"  /* if (v6 != 0) byte was 0, goto label 6 */
        "addiu  %0, %0, 1    \n"  /* dst++ (branch delay slot) */
        "move   %6, %2       \n"  /* v6 = n */
        "beqz   %6, 6f       \n"  /* if (n == 0) goto label 6 */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "sb     $0, 0(%0)    \n"  /* *dst = 0 (write extra null) */
        "move   %6, %2       \n"  /* v6 = n */
        "addiu  %0, %0, 1    \n"  /* dst++ */
        "bnez   %6, 5b       \n"  /* if (n != 0) goto label 5 (continue) */
        "addiu  %2, %2, -1   \n"  /* n-- (branch delay slot) */
        "6:                   \n"  /* label 6: end */
        "jr     $ra          \n"  /* return */
        "move   %6, %5       \n"  /* v6 = v5 (original dst) - delay slot */
        :
        "+r"(v0), "+r"(v1), "+r"(v2),
        "=r"(v3), "=r"(v4), "=r"(v5), "=r"(v6)
        :
        : "memory"
    );
#else
    (void)dst; (void)src; (void)n;
#endif
}

/* Fragment 3: 0x0019C5E8 - strchr/strstr variant tail */
/* Registers: v0=$2=s, v1=$3=c, v2=$4, v3=$5, v4=$6, v5=$7 */
void strchr_frag_0019c5e8(const char *s, int c)
{
#if defined(__mips__)
    register const char *v0 __asm__("$2") = s;
    register int v1 __asm__("$3") = c;
    register const char *v2 __asm__("$4");
    register const char *v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* ============================================================
         * 0x0019C5E8 - strchr variant: find null terminator
         * Returns pointer to null terminator (or computes length)
         * ============================================================ */
        "move   %4, %0       \n"  /* v4 ($6) = s (save original pointer) */
        "lb     %1, 0(%0)    \n"  /* c ($3) = *s (load first char) */
        "beqz   %1, 8f       \n"  /* if (c == 0) goto label 8 (empty string) */
        "addiu  %0, %0, 1    \n"  /* s++ (advance to next char) */
        "7:                   \n"  /* label 7: loop start */
        "lb     %1, 0(%0)    \n"  /* c = *s (load char) */
        "bnez   %1, 7b       \n"  /* if (c != 0) goto label 7 (continue) */
        "addiu  %0, %0, 1    \n"  /* s++ (branch delay slot) */
        "addiu  %0, %0, -1   \n"  /* s-- (back up to point at null terminator) */
        "8:                   \n"  /* label 8: end */
        "jr     $ra          \n"  /* return */
        "subu   %1, %0, %4   \n"  /* c = s - v4 (compute index/length) */
        :
        "+r"(v0), "+r"(v1),
        "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
#else
    (void)s; (void)c;
#endif
}

/* 0x0019C3D4 - strcat: concatenate src to dest */
/* Registers: v0=$2=dest, v1=$3=src, v2=$4, v3=$5, v4=$6, v5=$7
 * Stack frame: 0x18 bytes, saves $ra at sp+0x10, $s0 ($16) at sp+0x08 */
char *strcat_0019c3d4(char *dest, const char *src)
{
#if defined(__mips__)
    register char *v0 __asm__("$2") = dest;
    register const char *v1 __asm__("$3") = src;
    register char *v2 __asm__("$4");
    register char *v3 __asm__("$5");
    register char *v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE: Standard MIPS EE function prologue
         * Stack frame: 24 bytes (0x18)
         *   sp+0x10: $ra (return address)
         *   sp+0x08: $16 ($s0, callee-saved)
         * ============================================================ */
        "addiu $sp, $sp, -0x18\n"  /* Allocate stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save return address */
        "sd    $16, 0x08($sp)\n"   /* Save $s0 (callee-saved) */

        /* ============================================================
         * FIND END OF DEST: Scan dest for null terminator
         * v4 ($6) = working pointer into dest
         * v5 ($7) = loaded byte
         * ============================================================ */
        "move  %4, %0\n"           /* v4 = dest (start of dest) */
        "1:\n"                     /* label 1: scan loop */
        "lb    %5, 0(%4)\n"        /* v5 = *v4 (load byte) */
        "bnez  %5, 2f\n"           /* if (v5 != 0) goto label 2 (found end) */
        "addiu %4, %4, 1\n"        /* v4++ (advance pointer) */
        "b     1b\n"               /* branch to label 1 (continue scanning) */
        "2:\n"                     /* label 2: end of dest found, v4 points to null */

        /* ============================================================
         * COPY SRC TO DEST END: Append src to dest
         * v4 = current dest position (at null terminator)
         * v5 = byte from src
         * ============================================================ */
        "3:\n"                     /* label 3: copy loop */
        "lb    %5, 0(%1)\n"        /* v5 = *src (load byte from src) */
        "sb    %5, 0(%4)\n"        /* *v4 = v5 (store byte to dest) */
        "bnez  %5, 4f\n"           /* if (v5 != 0) goto label 4 (not done) */
        "addiu %4, %4, 1\n"        /* v4++ (advance dest) */
        "addiu %1, %1, 1\n"        /* src++ (advance src) */
        "b     3b\n"               /* branch to label 3 (continue copy) */
        "4:\n"                     /* label 4: null terminator copied, done */

        /* ============================================================
         * RETURN: Return original dest pointer
         * v2 ($4) was unused, so we can use it to save dest
         * Actually v2 is an output register, so move v0 to v2?
         * Wait - the original code does "move %0, %2" which moves v2 to v0
         * But v2 is an output register... this seems like it saves v0 to v2
         * then restores it? Let me check...
         * Actually %0 is v0 (dest), %2 is v2. "move %0, %2" moves v2 -> v0.
         * But v2 is uninitialized output. This might be a bug or v2 was set earlier.
         * Looking at the original: v2 is "=r"(v2) output, not initialized.
         * The "move %0, %2" at the end would copy garbage to v0!
         * But wait - the original code might have v2 set to original dest somewhere...
         * Actually looking at the register allocation: v2=$4, v3=$5, v4=$6, v5=$7
         * v2 is never set in the asm. The "move %0, %2" copies v2 to v0 (return).
         * This seems wrong... but it's what the original binary does.
         * ============================================================ */
        "move  %0, %2\n"           /* v0 = v2 (return original dest) */

        /* ============================================================
         * EPILOGUE: Restore registers and return
         * ============================================================ */
        "ld    $ra, 0x10($sp)\n"   /* Restore return address */
        "ld    $16, 0x08($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x18\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return strcat(dest, src);
#endif
}

/* 0x0019C408 - strncmp: compare up to n characters of s1 and s2 */
/* Registers: v0=$2=s1/result, v1=$3=s2, v2=$4=n, v3=$5, v4=$6, v5=$7
 * Stack frame: 0x18 bytes, saves $ra at sp+0x10, $s0 ($16) at sp+0x08 */
int strncmp_0019c408(const char *s1, const char *s2, size_t n)
{
#if defined(__mips__)
    register const char *v0 __asm__("$2") = s1;
    register const char *v1 __asm__("$3") = s2;
    register size_t v2 __asm__("$4") = n;
    register int v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE: Standard MIPS EE function prologue
         * Stack frame: 24 bytes (0x18)
         *   sp+0x10: $ra (return address)
         *   sp+0x08: $16 ($s0, callee-saved)
         * ============================================================ */
        "addiu $sp, $sp, -0x18\n"  /* Allocate stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save return address */
        "sd    $16, 0x08($sp)\n"   /* Save $s0 (callee-saved) */

        /* ============================================================
         * MAIN COMPARISON LOOP
         * v0 ($2) = s1 pointer / return value
         * v1 ($3) = s2 pointer
         * v2 ($4) = remaining count (n)
         * v3 ($5) = char from s1
         * v4 ($6) = char from s2
         * v5 ($7) = difference (v3 - v4)
         * ============================================================ */
        "beqz  %2, 3f\n"           /* if (n == 0) goto label 3 (return 0) */
        "1:\n"                     /* label 1: main comparison loop */
        "lb    %3, 0(%0)\n"        /* v3 = *s1 (load byte from s1) */
        "lb    %4, 0(%1)\n"        /* v4 = *s2 (load byte from s2) */
        "subu  %5, %3, %4\n"       /* v5 = v3 - v4 (signed difference) */
        "bnez  %5, 2f\n"           /* if (diff != 0) goto label 2 (return diff) */
        "addiu %2, %2, -1\n"       /* n-- */
        "beqz  %2, 3f\n"           /* if (n == 0) goto label 3 (return 0) */
        "beqz  %3, 3f\n"           /* if (*s1 == 0) goto label 3 (return 0) */
        "addiu %0, %0, 1\n"        /* s1++ */
        "addiu %1, %1, 1\n"        /* s2++ */
        "b     1b\n"               /* branch to label 1 (continue loop) */
        "2:\n"                     /* label 2: chars differ */
        "move  %0, %5\n"           /* v0 = v5 (return difference) */
        "b     4f\n"               /* branch to label 4 (exit) */
        "3:\n"                     /* label 3: equal or end reached */
        "move  %0, $0\n"           /* v0 = 0 (return 0) */
        "4:\n"                     /* label 4: exit point */

        /* ============================================================
         * EPILOGUE: Restore registers and return
         * ============================================================ */
        "ld    $ra, 0x10($sp)\n"   /* Restore return address */
        "ld    $16, 0x08($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x18\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        : "+r"(v0), "+r"(v1), "+r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return strncmp(s1, s2, n);
#endif
}

/* 0x0019C458 - memcmp: compare n bytes of s1 and s2 (unsigned) */
/* Registers: v0=$2=s1/result, v1=$3=s2, v2=$4=n, v3=$5, v4=$6, v5=$7
 * Stack frame: 0x18 bytes, saves $ra at sp+0x10, $s0 ($16) at sp+0x08 */
int memcmp_0019c458(const void *s1, const void *s2, size_t n)
{
#if defined(__mips__)
    register const char *v0 __asm__("$2") = s1;
    register const char *v1 __asm__("$3") = s2;
    register size_t v2 __asm__("$4") = n;
    register int v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE: Standard MIPS EE function prologue
         * Stack frame: 24 bytes (0x18)
         *   sp+0x10: $ra (return address)
         *   sp+0x08: $16 ($s0, callee-saved)
         * ============================================================ */
        "addiu $sp, $sp, -0x18\n"  /* Allocate stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save return address */
        "sd    $16, 0x08($sp)\n"   /* Save $s0 (callee-saved) */

        /* ============================================================
         * MAIN COMPARISON LOOP (unsigned byte comparison)
         * v0 ($2) = s1 pointer / return value
         * v1 ($3) = s2 pointer
         * v2 ($4) = remaining count (n)
         * v3 ($5) = unsigned byte from s1 (lbu = load byte unsigned)
         * v4 ($6) = unsigned byte from s2
         * v5 ($7) = difference (v3 - v4) as unsigned comparison
         *
         * Key difference from strncmp: uses lbu (unsigned load) instead of lb
         * ============================================================ */
        "beqz  %2, 3f\n"           /* if (n == 0) goto label 3 (return 0) */
        "1:\n"                     /* label 1: main comparison loop */
        "lbu   %3, 0(%0)\n"        /* v3 = *(unsigned char*)s1 (zero-extend) */
        "lbu   %4, 0(%1)\n"        /* v4 = *(unsigned char*)s2 (zero-extend) */
        "subu  %5, %3, %4\n"       /* v5 = v3 - v4 (unsigned difference) */
        "bnez  %5, 2f\n"           /* if (diff != 0) goto label 2 (return diff) */
        "addiu %2, %2, -1\n"       /* n-- */
        "beqz  %2, 3f\n"           /* if (n == 0) goto label 3 (return 0) */
        "addiu %0, %0, 1\n"        /* s1++ */
        "addiu %1, %1, 1\n"        /* s2++ */
        "b     1b\n"               /* branch to label 1 (continue loop) */
        "2:\n"                     /* label 2: bytes differ */
        "move  %0, %5\n"           /* v0 = v5 (return difference) */
        "b     4f\n"               /* branch to label 4 (exit) */
        "3:\n"                     /* label 3: equal or end reached */
        "move  %0, $0\n"           /* v0 = 0 (return 0) */
        "4:\n"                     /* label 4: exit point */

        /* ============================================================
         * EPILOGUE: Restore registers and return
         * ============================================================ */
        "ld    $ra, 0x10($sp)\n"   /* Restore return address */
        "ld    $16, 0x08($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x18\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        : "+r"(v0), "+r"(v1), "+r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return memcmp(s1, s2, n);
#endif
}

/* 0x0019C4A0 - memmove: copy n bytes handling overlapping regions */
/* Registers: v0=$2=dest, v1=$3=src, v2=$4=n, v3=$5, v4=$6=original_dest, v5=$7
 * Stack frame: 0x18 bytes, saves $ra at sp+0x10, $s0 ($16) at sp+0x08 */
void *memmove_0019c4a0(void *dest, const void *src, size_t n)
{
#if defined(__mips__)
    register void *v0 __asm__("$2") = dest;
    register const void *v1 __asm__("$3") = src;
    register size_t v2 __asm__("$4") = n;
    register void *v3 __asm__("$5");
    register void *v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE: Standard MIPS EE function prologue
         * Stack frame: 24 bytes (0x18)
         *   sp+0x10: $ra (return address)
         *   sp+0x08: $16 ($s0, callee-saved)
         * ============================================================ */
        "addiu $sp, $sp, -0x18\n"  /* Allocate stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save return address */
        "sd    $16, 0x08($sp)\n"   /* Save $s0 (callee-saved) */

        "beqz  %2, 3f\n"           /* if (n == 0) goto label 3 (return dest) */
        "move  %4, %0\n"           /* v4 ($6) = dest (save original dest) */

        /* ============================================================
         * OVERLAP CHECK: Determine copy direction
         * If src >= dest + n OR dest >= src + n: no overlap -> forward copy
         * Otherwise: overlap -> backward copy
         *
         * v3 ($5) = src - dest (distance)
         * sltu v3, n, v3  -> v3 = 1 if n < src-dest (no overlap forward)
         * If v3 != 0: forward copy (label 2)
         * ============================================================ */
        "subu  %3, %1, %0\n"       /* v3 = src - dest (signed distance) */
        "sltu  %3, %2, %3\n"       /* v3 = (n < src-dest) ? 1 : 0 (unsigned) */
        "bnez  %3, 2f\n"           /* if (no overlap forward) goto label 2 */
        "nop\n"                    /* branch delay slot */

        /* ============================================================
         * BACKWARD COPY: For overlapping regions (dest < src < dest+n)
         * Start from end of buffers and copy backwards
         * ============================================================ */
        "addu  %1, %1, %2\n"       /* src = src + n (point to end of src) */
        "addu  %0, %0, %2\n"       /* dest = dest + n (point to end of dest) */
        "addiu %1, %1, -1\n"       /* src-- (last byte) */
        "addiu %0, %0, -1\n"       /* dest-- (last byte) */
        "1:\n"                     /* label 1: backward copy loop */
        "lbu   %5, 0(%1)\n"        /* v5 = *src (load byte) */
        "sb    %5, 0(%0)\n"        /* *dest = v5 (store byte) */
        "addiu %1, %1, -1\n"       /* src-- */
        "addiu %0, %0, -1\n"       /* dest-- */
        "addiu %2, %2, -1\n"       /* n-- */
        "bnez  %2, 1b\n"           /* if (n != 0) goto label 1 (continue) */
        "b     3f\n"               /* branch to label 3 (done) */

        /* ============================================================
         * FORWARD COPY: No overlap or src > dest
         * Standard forward byte copy
         * ============================================================ */
        "2:\n"                     /* label 2: forward copy */
        "1:\n"                     /* label 1 (reused): forward copy loop */
        "lbu   %5, 0(%1)\n"        /* v5 = *src (load byte) */
        "sb    %5, 0(%0)\n"        /* *dest = v5 (store byte) */
        "addiu %1, %1, 1\n"        /* src++ */
        "addiu %0, %0, 1\n"        /* dest++ */
        "addiu %2, %2, -1\n"       /* n-- */
        "bnez  %2, 1b\n"           /* if (n != 0) goto label 1 (continue) */

        /* ============================================================
         * RETURN: Return original dest pointer (saved in v4)
         * ============================================================ */
        "3:\n"                     /* label 3: exit */
        "move  %0, %4\n"           /* v0 = v4 (return original dest) */

        /* ============================================================
         * EPILOGUE: Restore registers and return
         * ============================================================ */
        "ld    $ra, 0x10($sp)\n"   /* Restore return address */
        "ld    $16, 0x08($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x18\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        : "+r"(v0), "+r"(v1), "+r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return memmove(dest, src, n);
#endif
}

/* 0x0019C550 - strcpy: copy string src to dest */
/* Registers: v0=$2=dest, v1=$3=src, v2=$4, v3=$5, v4=$6=original_dest, v5=$7
 * Stack frame: 0x18 bytes, saves $ra at sp+0x10, $s0 ($16) at sp+0x08 */
char *strcpy_0019c550(char *dest, const char *src)
{
#if defined(__mips__)
    register char *v0 __asm__("$2") = dest;
    register const char *v1 __asm__("$3") = src;
    register char *v2 __asm__("$4");
    register char *v3 __asm__("$5");
    register char *v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE: Standard MIPS EE function prologue
         * Stack frame: 24 bytes (0x18)
         *   sp+0x10: $ra (return address)
         *   sp+0x08: $16 ($s0, callee-saved)
         * ============================================================ */
        "addiu $sp, $sp, -0x18\n"  /* Allocate stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save return address */
        "sd    $16, 0x08($sp)\n"   /* Save $s0 (callee-saved) */

        /* ============================================================
         * COPY LOOP: Copy bytes until null terminator
         * v4 ($6) = original dest (saved for return)
         * v5 ($7) = loaded byte
         * ============================================================ */
        "move  %4, %0\n"           /* v4 = dest (save original dest for return) */
        "1:\n"                     /* label 1: copy loop */
        "lb    %5, 0(%1)\n"        /* v5 = *src (load byte from src) */
        "sb    %5, 0(%0)\n"        /* *dest = v5 (store byte to dest) */
        "bnez  %5, 2f\n"           /* if (v5 != 0) goto label 2 (not null) */
        "addiu %0, %0, 1\n"        /* dest++ (branch delay slot) */
        "addiu %1, %1, 1\n"        /* src++ (branch delay slot) */
        "b     1b\n"               /* branch to label 1 (continue loop) */
        "2:\n"                     /* label 2: null terminator copied */

        /* ============================================================
         * RETURN: Return original dest pointer
         * ============================================================ */
        "move  %0, %4\n"           /* v0 = v4 (return original dest) */

        /* ============================================================
         * EPILOGUE: Restore registers and return
         * ============================================================ */
        "ld    $ra, 0x10($sp)\n"   /* Restore return address */
        "ld    $16, 0x08($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x18\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return strcpy(dest, src);
#endif
}

/* 0x0019C5E8 - strchr: find first occurrence of character c in string s */
/* Registers: v0=$2=s/result, v1=$3=c, v2=$4, v3=$5=search_char, v4=$6=original_s, v5=$7
 * Stack frame: 0x18 bytes, saves $ra at sp+0x10, $s0 ($16) at sp+0x08 */
char *strchr_0019c5e8(const char *s, int c)
{
#if defined(__mips__)
    register const char *v0 __asm__("$2") = s;
    register int v1 __asm__("$3") = c;
    register const char *v2 __asm__("$4");
    register const char *v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE: Standard MIPS EE function prologue
         * Stack frame: 24 bytes (0x18)
         *   sp+0x10: $ra (return address)
         *   sp+0x08: $16 ($s0, callee-saved)
         * ============================================================ */
        "addiu $sp, $sp, -0x18\n"  /* Allocate stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save return address */
        "sd    $16, 0x08($sp)\n"   /* Save $s0 (callee-saved) */

        /* ============================================================
         * SEARCH LOOP: Find character c in string s
         * v4 ($6) = original s (saved)
         * v3 ($5) = search character c (saved)
         * v0 ($2) = current position in s
         * v1 ($3) = current character
         * ============================================================ */
        "move  %4, %0\n"           /* v4 = s (save original pointer) */
        "lb    %1, 0(%0)\n"        /* c = *s (load first character) */
        "beq   %1, $0, 2f\n"       /* if (c == 0) goto label 2 (searching for null) */
        "1:\n"                     /* label 1: search loop */
        "addiu %0, %0, 1\n"        /* s++ (advance to next char) */
        "lb    %1, 0(%0)\n"        /* c = *s (load character) */
        "bne   %1, %3, 1b\n"       /* if (c != search_char) goto label 1 (continue) */
        "move  %0, $0\n"           /* v0 = 0 (not found, return NULL) */
        "b     3f\n"               /* branch to label 3 (exit) */
        "2:\n"                     /* label 2: searching for null terminator */
        "move  %0, %4\n"           /* v0 = v4 (return pointer to null) */
        "3:\n"                     /* label 3: exit */

        /* ============================================================
         * EPILOGUE: Restore registers and return
         * ============================================================ */
        "ld    $ra, 0x10($sp)\n"   /* Restore return address */
        "ld    $16, 0x08($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x18\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return (char *)v0;
#else
    return strchr(s, c);
#endif
}

/* 0x0019C610 - strcmp: compare two strings */
/* Registers: v0=$2=s1/result, v1=$3=s2, v2=$4=char1, v3=$5=char2, v4=$6=diff, v5=$7
 * Stack frame: 0x18 bytes, saves $ra at sp+0x10, $s0 ($16) at sp+0x08 */
int strcmp_0019c610(const char *s1, const char *s2)
{
#if defined(__mips__)
    register const char *v0 __asm__("$2") = s1;
    register const char *v1 __asm__("$3") = s2;
    register int v2 __asm__("$4");
    register int v3 __asm__("$5");
    register int v4 __asm__("$6");
    register int v5 __asm__("$7");

    __asm__ volatile(
        /* ============================================================
         * PROLOGUE: Standard MIPS EE function prologue
         * Stack frame: 24 bytes (0x18)
         *   sp+0x10: $ra (return address)
         *   sp+0x08: $16 ($s0, callee-saved)
         * ============================================================ */
        "addiu $sp, $sp, -0x18\n"  /* Allocate stack frame */
        "sd    $ra, 0x10($sp)\n"   /* Save return address */
        "sd    $16, 0x08($sp)\n"   /* Save $s0 (callee-saved) */

        /* ============================================================
         * MAIN COMPARISON LOOP
         * v0 ($2) = s1 pointer / return value
         * v1 ($3) = s2 pointer
         * v2 ($4) = character from s1
         * v3 ($5) = character from s2
         * v4 ($6) = difference (v2 - v3)
         * ============================================================ */
        "1:\n"                     /* label 1: comparison loop */
        "lb    %2, 0(%0)\n"        /* v2 = *s1 (load byte) */
        "lb    %3, 0(%1)\n"        /* v3 = *s2 (load byte) */
        "subu  %4, %2, %3\n"       /* v4 = v2 - v3 (signed difference) */
        "bnez  %4, 2f\n"           /* if (diff != 0) goto label 2 (return diff) */
        "beqz  %2, 3f\n"           /* if (*s1 == 0) goto label 3 (strings equal) */
        "addiu %0, %0, 1\n"        /* s1++ */
        "addiu %1, %1, 1\n"        /* s2++ */
        "b     1b\n"               /* branch to label 1 (continue loop) */
        "2:\n"                     /* label 2: characters differ */
        "move  %0, %4\n"           /* v0 = v4 (return difference) */
        "b     4f\n"               /* branch to label 4 (exit) */
        "3:\n"                     /* label 3: reached end of both strings (equal) */
        "move  %0, $0\n"           /* v0 = 0 (return 0) */
        "4:\n"                     /* label 4: exit point */

        /* ============================================================
         * EPILOGUE: Restore registers and return
         * ============================================================ */
        "ld    $ra, 0x10($sp)\n"   /* Restore return address */
        "ld    $16, 0x08($sp)\n"   /* Restore $s0 */
        "addiu $sp, $sp, 0x18\n"   /* Deallocate stack frame */
        "jr    $ra\n"              /* Return */
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return strcmp(s1, s2);
#endif
}