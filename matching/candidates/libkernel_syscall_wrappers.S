/*
 * Historical PS2 EE libkernel syscall-wrapper family.
 *
 * The wrapper shape follows the old PS2DEV snapshot kernel.S (duduclx/PS2DEV bac0006c) SYSCALL macro:
 * load the syscall number into $v1 ($3), execute syscall, return via $ra,
 * and keep a nop in the return delay slot.
 *
 * SNES Station target bytes are compared in two committed-listing gates:
 *   0x0019ce60..0x0019cf10 (11 functions)
 *   0x0019f5d0..0x0019f600 (3 functions)
 *
 * This file preserves the historical assembly source form rather than
 * rewriting these leaves as invented C.
 */

    .set noreorder
    .text

    .macro EE_SYSCALL_WRAPPER name, number
    .align 4
    .globl \name\()_candidate
    .type \name\()_candidate,@function
    .ent \name\()_candidate
\name\()_candidate:
    addiu   $3,$zero,\number
    syscall
    jr      $31
    nop
    .end \name\()_candidate
    .size \name\()_candidate,.-\name\()_candidate
    .endm

    EE_SYSCALL_WRAPPER iWakeupThread,-0x34
    EE_SYSCALL_WRAPPER CreateSema,0x40
    EE_SYSCALL_WRAPPER DeleteSema,0x41
    EE_SYSCALL_WRAPPER iSignalSema,-0x43
    EE_SYSCALL_WRAPPER WaitSema,0x44
    EE_SYSCALL_WRAPPER FlushCache,0x64
    EE_SYSCALL_WRAPPER iFlushCache,-0x68
    EE_SYSCALL_WRAPPER SifDmaStat,0x76
    EE_SYSCALL_WRAPPER SifSetDma,0x77
    EE_SYSCALL_WRAPPER SifSetReg,0x79
    EE_SYSCALL_WRAPPER SifGetReg,0x7a

    EE_SYSCALL_WRAPPER SifStopDma,0x6b
    EE_SYSCALL_WRAPPER iSifSetDma,-0x77
    EE_SYSCALL_WRAPPER SifSetDChain,0x78

    EE_SYSCALL_WRAPPER _EnableDmac,0x16
    EE_SYSCALL_WRAPPER _DisableDmac,0x17
    EE_SYSCALL_WRAPPER SignalSema,0x42
    EE_SYSCALL_WRAPPER PollSema,0x45
    EE_SYSCALL_WRAPPER iSifSetDChain,-0x78
