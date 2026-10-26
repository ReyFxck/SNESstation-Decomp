/*
 * Canonical C translation of libkernel syscall wrappers at 0x0019CE60.
 * Eleven syscall wrappers, each 16 bytes: load syscall number, syscall, jr ra, nop.
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_syscalls_0019ce60.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 */

#include <stdint.h>

/* Syscall 0xCC (-52 / 0xFFFFFFCC) */
int syscall_0xCC(void)
{
    register int v0 __asm__("$2") = 0xFFFFFFCC;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x40 (64) */
int syscall_0x40(void)
{
    register int v0 __asm__("$2") = 0x40;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x41 (65) */
int syscall_0x41(void)
{
    register int v0 __asm__("$2") = 0x41;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0xBD (-67 / 0xFFFFFFBD) */
int syscall_0xBD(void)
{
    register int v0 __asm__("$2") = 0xFFFFFFBD;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x44 (68) */
int syscall_0x44(void)
{
    register int v0 __asm__("$2") = 0x44;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x64 (100) */
int syscall_0x64(void)
{
    register int v0 __asm__("$2") = 0x64;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x98 (-104 / 0xFFFFFF98) */
int syscall_0x98(void)
{
    register int v0 __asm__("$2") = 0xFFFFFF98;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x76 (118) */
int syscall_0x76(void)
{
    register int v0 __asm__("$2") = 0x76;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x77 (119) */
int syscall_0x77(void)
{
    register int v0 __asm__("$2") = 0x77;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x79 (121) */
int syscall_0x79(void)
{
    register int v0 __asm__("$2") = 0x79;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x7A (122) */
int syscall_0x7A(void)
{
    register int v0 __asm__("$2") = 0x7A;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}