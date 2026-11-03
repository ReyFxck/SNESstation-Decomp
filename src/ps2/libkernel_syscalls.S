/*
 * Canonical assembly translation of libkernel syscall wrappers at 0x0019FCD0.
 * Five syscall wrappers, each 16 bytes: li v0, num; syscall; jr ra; nop.
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_syscalls_0019fcd0.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 */

/* Syscall 0x16 (22) */
    .type   syscall_0x16, @function
    .global syscall_0x16
    .ent    syscall_0x16
syscall_0x16:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x16
    syscall
    jr      $ra
    nop
    .end    syscall_0x16
    .size   syscall_0x16, .-syscall_0x16

/* Syscall 0x17 (23) */
    .type   syscall_0x17, @function
    .global syscall_0x17
    .ent    syscall_0x17
syscall_0x17:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x17
    syscall
    jr      $ra
    nop
    .end    syscall_0x17
    .size   syscall_0x17, .-syscall_0x17

/* Syscall 0x42 (66) */
    .type   syscall_0x42, @function
    .global syscall_0x42
    .ent    syscall_0x42
syscall_0x42:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x42
    syscall
    jr      $ra
    nop
    .end    syscall_0x42
    .size   syscall_0x42, .-syscall_0x42

/* Syscall 0x45 (69) */
    .type   syscall_0x45, @function
    .global syscall_0x45
    .ent    syscall_0x45
syscall_0x45:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x45
    syscall
    jr      $ra
    nop
    .end    syscall_0x45
    .size   syscall_0x45, .-syscall_0x45

/* Syscall 0xFFFFFF88 (-120) */
    .type   syscall_0xFFFFFF88, @function
    .global syscall_0xFFFFFF88
    .ent    syscall_0xFFFFFF88
syscall_0xFFFFFF88:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0xFFFFFF88
    syscall
    jr      $ra
    nop
    .end    syscall_0xFFFFFF88
    .size   syscall_0xFFFFFF88, .-syscall_0xFFFFFF88
