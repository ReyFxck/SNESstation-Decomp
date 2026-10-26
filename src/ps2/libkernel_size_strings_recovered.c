/*
 * Canonical C translation of libkernel size/string focused extract at 0x0019C364.
 * Three focused fragments from the libkernel libc string corridor:
 *   Fragment 1: 0x0019C364-0x0019C3D0 (28 instructions) - memset + partial strlen setup
 *   Fragment 2: 0x0019C550-0x0019C5A0 (20 instructions) - memmove variant inner loop
 *   Fragment 3: 0x0019C5E8-0x0019C60C (11 instructions) - strchr/strstr variant tail
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_size_strings_0019c364.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 */

#include <stddef.h>
#include <stdint.h>

/* Fragment 1: 0x0019C364 - memset + strlen setup */
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
        /* 0x0019C364 - memset */
        "move   %4, %0       \n"  /* v4 = dst */
        "move   %5, %4       \n"  /* v5 = v4 */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "addiu  %6, $0, -1   \n"  /* v6 = -1 */
        "beq    %2, %6, 2f   \n"  /* if (n == -1) goto end (2) */
        "move   %0, %6       \n"  /* dst = -1 */
        "1:                   \n"  /* loop start (1) */
        "lbu    %6, 0(%1)    \n"  /* v6 = *src */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "addiu  %1, %1, 1    \n"  /* src++ */
        "sb     %6, 0(%5)    \n"  /* *v5 = v6 */
        "bne    %2, %0, 1b   \n"  /* if (n != dst) goto loop (1) */
        "addiu  %5, %5, 1    \n"  /* v5++ */
        "2:                   \n"  /* end (2) */
        "jr     $ra          \n"  /* return */
        "move   %6, %4       \n"  /* v6 = v4 (dst) */

        /* 0x0019C39C - strlen setup */
        "move   %5, %0       \n"  /* v5 = dst */
        "lui    %6, 0xffff   \n"  /* v6 = 0xffff0000 */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "ori    %6, %6, 0xffff\n" /* v6 = 0xffffffff */
        "beq    %2, %6, 4f   \n"  /* if (n == -1) goto end (4) */
        "nop                  \n"
        "lui    %6, 0xffff   \n"  /* v6 = 0xffff0000 */
        "ori    %6, %6, 0xffff\n" /* v6 = 0xffffffff */
        "3:                   \n"  /* loop start (3) */
        "sb     %3, 0(%5)    \n"  /* *v5 = c */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "bne    %2, %6, 3b   \n"  /* if (n != -1) goto loop (3) */
        "addiu  %5, %5, 1    \n"  /* v5++ */
        "4:                   \n"  /* end (4) */
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
        /* 0x0019C550 */
        "move   %5, %0       \n"  /* v5 = dst */
        "nop                  \n"
        "beqz   %2, 6f       \n"  /* if (n == 0) goto end (6) */
        "move   %6, %2       \n"  /* v6 = n */
        "5:                   \n"  /* loop start (5) */
        "lbu    %6, 0(%1)    \n"  /* v6 = *src */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "addiu  %1, %1, 1    \n"  /* src++ */
        "sb     %6, 0(%0)    \n"  /* *dst = v6 */
        "sll    %6, %6, 24   \n"  /* v6 <<= 24 */
        "bnez   %6, 6f       \n"  /* if (v6 != 0) goto end (6) */
        "addiu  %0, %0, 1    \n"  /* dst++ */
        "move   %6, %2       \n"  /* v6 = n */
        "beqz   %6, 6f       \n"  /* if (n == 0) goto end (6) */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "sb     $0, 0(%0)    \n"  /* *dst = 0 */
        "move   %6, %2       \n"  /* v6 = n */
        "addiu  %0, %0, 1    \n"  /* dst++ */
        "bnez   %6, 5b       \n"  /* if (n != 0) goto loop (5) */
        "addiu  %2, %2, -1   \n"  /* n-- */
        "6:                   \n"  /* end (6) */
        "jr     $ra          \n"  /* return */
        "move   %6, %5       \n"  /* v6 = v5 (original dst) */
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
        /* 0x0019C5E8 */
        "move   %4, %0       \n"  /* v4 = s */
        "lb     %1, 0(%0)    \n"  /* c = *s */
        "beqz   %1, 8f       \n"  /* if (c == 0) goto end (8) */
        "addiu  %0, %0, 1    \n"  /* s++ */
        "7:                   \n"  /* loop start (7) */
        "lb     %1, 0(%0)    \n"  /* c = *s */
        "bnez   %1, 7b       \n"  /* if (c != 0) goto loop (7) */
        "addiu  %0, %0, 1    \n"  /* s++ */
        "addiu  %0, %0, -1   \n"  /* s-- */
        "8:                   \n"  /* end (8) */
        "jr     $ra          \n"  /* return */
        "subu   %1, %0, %4   \n"  /* c = s - v4 */
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