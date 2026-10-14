/*
 * Byte-exact EE reconstruction of numtestf @ 0x001a06c0.
 *
 * Newlib 1.10.0 establishes the behavior, but the target leaf uses an older
 * instruction-selection shape that the surviving GCC 3.2.2 BETA 3 backend
 * does not reproduce from C: signed ADDI returns, $f11 for the zero compare,
 * and separate return blocks.  mathfp_numtest.c remains the readable C model;
 * this file records the observed target instructions without claiming to be
 * Hiryu's original source.
 */

    .set noreorder
    .set nomacro
    .text
    .align 2
    .globl numtestf_candidate
    .ent numtestf_candidate
numtestf_candidate:
    .frame $sp,0,$31
    .mask 0x00000000,0
    .fmask 0x00000000,0

    mtc1 $zero,$f11
    .word 0
    c.eq.s $f12,$f11
    .word 0
    bc1f 1f
    .word 0
    .word 0
    jr $ra
    addi $v0,$zero,0
    .word 0

1:
    mfc1 $a0,$f12
    .word 0
    dsrl $a1,$a0,23
    addi $a2,$zero,0x7f8
    and $a1,$a1,$a2
    beq $a1,$a2,2f
    .word 0
    .word 0
    jr $ra
    addi $v0,$zero,3
    .word 0

2:
    lui $a1,0x7f
    ori $a1,$a1,0xffff
    and $a0,$a0,$a1
    beq $a0,$zero,3f
    .word 0
    jr $ra
    addi $v0,$zero,2
    .word 0

3:
    jr $ra
    addi $v0,$zero,1
    .word 0

    .end numtestf_candidate
    .size numtestf_candidate,.-numtestf_candidate
