/*
 * Canonical C translation of libkernel syscall wrappers at 0x0019FCD0.
 * Five syscall wrappers, each 16 bytes: load syscall number, syscall, jr ra, nop.
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_syscalls_0019fcd0.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 */

#include <stdint.h>

/* Syscall 0x16 (22) */
int syscall_0x16(void)
{
    register int v0 __asm__("$2") = 0x16;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x17 (23) */
int syscall_0x17(void)
{
    register int v0 __asm__("$2") = 0x17;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x42 (66) */
int syscall_0x42(void)
{
    register int v0 __asm__("$2") = 0x42;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x45 (69) */
int syscall_0x45(void)
{
    register int v0 __asm__("$2") = 0x45;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x88 (-120 / 0xFFFFFF88) */
int syscall_0x88(void)
{
    register int v0 __asm__("$2") = 0xFFFFFF88;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}