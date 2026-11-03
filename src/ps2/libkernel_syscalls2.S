/*
 * Canonical assembly translation of libkernel syscall wrappers at 0x0019CE60.
 * Eleven syscall wrappers, each 16 bytes: li v0, num; syscall; jr ra; nop.
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_syscalls2_0019ce60.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 */

/* Syscall 0xCC (-52 / 0xFFFFFFCC) */
    .type   syscall_0xCC, @function
    .global syscall_0xCC
    .ent    syscall_0xCC
syscall_0xCC:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0xFFFFFFCC
    syscall
    jr      $ra
    nop
    .end    syscall_0xCC
    .size   syscall_0xCC, .-syscall_0xCC

/* Syscall 0x40 (64) */
    .type   syscall_0x40, @function
    .global syscall_0x40
    .ent    syscall_0x40
syscall_0x40:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x40
    syscall
    jr      $ra
    nop
    .end    syscall_0x40
    .size   syscall_0x40, .-syscall_0x40

/* Syscall 0x41 (65) */
    .type   syscall_0x41, @function
    .global syscall_0x41
    .ent    syscall_0x41
syscall_0x41:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x41
    syscall
    jr      $ra
    nop
    .end    syscall_0x41
    .size   syscall_0x41, .-syscall_0x41

/* Syscall 0xBD (-67 / 0xFFFFFFBD) */
    .type   syscall_0xBD, @function
    .global syscall_0xBD
    .ent    syscall_0xBD
syscall_0xBD:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0xFFFFFFBD
    syscall
    jr      $ra
    nop
    .end    syscall_0xBD
    .size   syscall_0xBD, .-syscall_0xBD

/* Syscall 0x44 (68) */
    .type   syscall_0x44, @function
    .global syscall_0x44
    .ent    syscall_0x44
syscall_0x44:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x44
    syscall
    jr      $ra
    nop
    .end    syscall_0x44
    .size   syscall_0x44, .-syscall_0x44

/* Syscall 0x64 (100) */
    .type   syscall_0x64, @function
    .global syscall_0x64
    .ent    syscall_0x64
syscall_0x64:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x64
    syscall
    jr      $ra
    nop
    .end    syscall_0x64
    .size   syscall_0x64, .-syscall_0x64

/* Syscall 0x98 (-104 / 0xFFFFFF98) */
    .type   syscall_0x98, @function
    .global syscall_0x98
    .ent    syscall_0x98
syscall_0x98:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0xFFFFFF98
    syscall
    jr      $ra
    nop
    .end    syscall_0x98
    .size   syscall_0x98, .-syscall_0x98

/* Syscall 0x76 (118) */
    .type   syscall_0x76, @function
    .global syscall_0x76
    .ent    syscall_0x76
syscall_0x76:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x76
    syscall
    jr      $ra
    nop
    .end    syscall_0x76
    .size   syscall_0x76, .-syscall_0x76

/* Syscall 0x77 (119) */
    .type   syscall_0x77, @function
    .global syscall_0x77
    .ent    syscall_0x77
syscall_0x77:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x77
    syscall
    jr      $ra
    nop
    .end    syscall_0x77
    .size   syscall_0x77, .-syscall_0x77

/* Syscall 0x79 (121) */
    .type   syscall_0x79, @function
    .global syscall_0x79
    .ent    syscall_0x79
syscall_0x79:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x79
    syscall
    jr      $ra
    nop
    .end    syscall_0x79
    .size   syscall_0x79, .-syscall_0x79

/* Syscall 0x7A (122) */
    .type   syscall_0x7A, @function
    .global syscall_0x7A
    .ent    syscall_0x7A
syscall_0x7A:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, 0x7A
    syscall
    jr      $ra
    nop
    .end    syscall_0x7A
    .size   syscall_0x7A, .-syscall_0x7A