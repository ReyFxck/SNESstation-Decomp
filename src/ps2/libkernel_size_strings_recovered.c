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

/* 0x0019C3D4 - strcat */
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
        /* Prologue */
        "addiu $sp, $sp, -0x18\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x08($sp)\n"

        /* Find end of dest */
        "move  %4, %0\n"
        "1:\n"
        "lb    %5, 0(%4)\n"
        "bnez  %5, 2f\n"
        "addiu %4, %4, 1\n"
        "b     1b\n"
        "2:\n"

        /* Copy src to dest end */
        "3:\n"
        "lb    %5, 0(%1)\n"
        "sb    %5, 0(%4)\n"
        "bnez  %5, 4f\n"
        "addiu %4, %4, 1\n"
        "addiu %1, %1, 1\n"
        "b     3b\n"
        "4:\n"

        /* Return original dest */
        "move  %0, %2\n"

        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x08($sp)\n"
        "addiu $sp, $sp, 0x18\n"
        "jr    $ra\n"
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return strcat(dest, src);
#endif
}

/* 0x0019C408 - strncmp */
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
        /* Prologue */
        "addiu $sp, $sp, -0x18\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x08($sp)\n"

        "beqz  %2, 3f\n"
        "1:\n"
        "lb    %3, 0(%0)\n"
        "lb    %4, 0(%1)\n"
        "subu  %5, %3, %4\n"
        "bnez  %5, 2f\n"
        "addiu %2, %2, -1\n"
        "beqz  %2, 3f\n"
        "beqz  %3, 3f\n"
        "addiu %0, %0, 1\n"
        "addiu %1, %1, 1\n"
        "b     1b\n"
        "2:\n"
        "move  %0, %5\n"
        "b     4f\n"
        "3:\n"
        "move  %0, $0\n"
        "4:\n"

        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x08($sp)\n"
        "addiu $sp, $sp, 0x18\n"
        "jr    $ra\n"
        : "+r"(v0), "+r"(v1), "+r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return strncmp(s1, s2, n);
#endif
}

/* 0x0019C458 - memcmp */
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
        /* Prologue */
        "addiu $sp, $sp, -0x18\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x08($sp)\n"

        "beqz  %2, 3f\n"
        "1:\n"
        "lbu   %3, 0(%0)\n"
        "lbu   %4, 0(%1)\n"
        "subu  %5, %3, %4\n"
        "bnez  %5, 2f\n"
        "addiu %2, %2, -1\n"
        "beqz  %2, 3f\n"
        "addiu %0, %0, 1\n"
        "addiu %1, %1, 1\n"
        "b     1b\n"
        "2:\n"
        "move  %0, %5\n"
        "b     4f\n"
        "3:\n"
        "move  %0, $0\n"
        "4:\n"

        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x08($sp)\n"
        "addiu $sp, $sp, 0x18\n"
        "jr    $ra\n"
        : "+r"(v0), "+r"(v1), "+r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return memcmp(s1, s2, n);
#endif
}

/* 0x0019C4A0 - memmove */
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
        /* Prologue */
        "addiu $sp, $sp, -0x18\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x08($sp)\n"

        "beqz  %2, 3f\n"
        "move  %4, %0\n"

        /* Check for overlap - forward copy if src >= dest or no overlap */
        "subu  %3, %1, %0\n"
        "sltu  %3, %2, %3\n"
        "bnez  %3, 2f\n"
        "nop\n"

        /* Backward copy for overlapping regions */
        "addu  %1, %1, %2\n"
        "addu  %0, %0, %2\n"
        "addiu %1, %1, -1\n"
        "addiu %0, %0, -1\n"
        "1:\n"
        "lbu   %5, 0(%1)\n"
        "sb    %5, 0(%0)\n"
        "addiu %1, %1, -1\n"
        "addiu %0, %0, -1\n"
        "addiu %2, %2, -1\n"
        "bnez  %2, 1b\n"
        "b     3f\n"

        "2:\n"
        /* Forward copy */
        "1:\n"
        "lbu   %5, 0(%1)\n"
        "sb    %5, 0(%0)\n"
        "addiu %1, %1, 1\n"
        "addiu %0, %0, 1\n"
        "addiu %2, %2, -1\n"
        "bnez  %2, 1b\n"

        "3:\n"
        "move  %0, %4\n"

        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x08($sp)\n"
        "addiu $sp, $sp, 0x18\n"
        "jr    $ra\n"
        : "+r"(v0), "+r"(v1), "+r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return memmove(dest, src, n);
#endif
}

/* 0x0019C550 - strcpy */
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
        /* Prologue */
        "addiu $sp, $sp, -0x18\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x08($sp)\n"

        "move  %4, %0\n"
        "1:\n"
        "lb    %5, 0(%1)\n"
        "sb    %5, 0(%0)\n"
        "bnez  %5, 2f\n"
        "addiu %0, %0, 1\n"
        "addiu %1, %1, 1\n"
        "b     1b\n"
        "2:\n"
        "move  %0, %4\n"

        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x08($sp)\n"
        "addiu $sp, $sp, 0x18\n"
        "jr    $ra\n"
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return strcpy(dest, src);
#endif
}

/* 0x0019C5E8 - strchr */
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
        /* Prologue */
        "addiu $sp, $sp, -0x18\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x08($sp)\n"

        "move  %4, %0\n"
        "lb    %1, 0(%0)\n"
        "beq   %1, $0, 2f\n"
        "1:\n"
        "addiu %0, %0, 1\n"
        "lb    %1, 0(%0)\n"
        "bne   %1, %3, 1b\n"
        "move  %0, $0\n"
        "b     3f\n"
        "2:\n"
        "move  %0, %4\n"
        "3:\n"

        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x08($sp)\n"
        "addiu $sp, $sp, 0x18\n"
        "jr    $ra\n"
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return (char *)v0;
#else
    return strchr(s, c);
#endif
}

/* 0x0019C610 - strcmp */
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
        /* Prologue */
        "addiu $sp, $sp, -0x18\n"
        "sd    $ra, 0x10($sp)\n"
        "sd    $16, 0x08($sp)\n"

        "1:\n"
        "lb    %2, 0(%0)\n"
        "lb    %3, 0(%1)\n"
        "subu  %4, %2, %3\n"
        "bnez  %4, 2f\n"
        "beqz  %2, 3f\n"
        "addiu %0, %0, 1\n"
        "addiu %1, %1, 1\n"
        "b     1b\n"
        "2:\n"
        "move  %0, %4\n"
        "b     4f\n"
        "3:\n"
        "move  %0, $0\n"
        "4:\n"

        /* Epilogue */
        "ld    $ra, 0x10($sp)\n"
        "ld    $16, 0x08($sp)\n"
        "addiu $sp, $sp, 0x18\n"
        "jr    $ra\n"
        : "+r"(v0), "+r"(v1), "=r"(v2), "=r"(v3), "=r"(v4), "=r"(v5)
        :
        : "memory"
    );
    return v0;
#else
    return strcmp(s1, s2);
#endif
}