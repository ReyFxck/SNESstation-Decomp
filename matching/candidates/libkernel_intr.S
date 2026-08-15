/*
 * Byte-exact EE reconstruction of DIntr/EIntr.
 *
 * Historical 2003 PS2DEV glue.c from duduclx/PS2DEV bac0006c
 * establishes the source behavior: read COP0 Status bit 0x10000, disable or
 * enable interrupts, and return the previous state.  SNES Station's exact
 * compiler scheduling/register allocation is recorded here because the
 * neighboring SNESticle libkernel map demonstrates a close but not globally
 * identical historical build.
 *
 * This is a matching reconstruction, not a claim to be Hiryu's original
 * assembly source.  The readable recovered C remains in src/ps2/.
 */

    .set noreorder
    .text
    .align 2

    .globl DIntr_candidate
    .type DIntr_candidate,@function
    .ent DIntr_candidate
DIntr_candidate:
    mfc0    $3,$12
    lui     $2,1
    and     $3,$3,$2
    daddu   $4,$zero,$zero
    beq     $3,$zero,2f
    sltu    $5,$zero,$3
1:
    /* R5900 DI and sync.p encodings; .word keeps old assemblers reproducible. */
    .word   0x42000039
    .word   0x0000040f
    mfc0    $3,$12
    lui     $2,1
    and     $3,$3,$2
    nop
    bne     $3,$zero,1b
    nop
    daddu   $4,$5,$zero
2:
    jr      $31
    daddu   $2,$4,$zero
    nop
    .end DIntr_candidate
    .size DIntr_candidate,.-DIntr_candidate

    .globl EIntr_candidate
    .type EIntr_candidate,@function
    .ent EIntr_candidate
EIntr_candidate:
    mfc0    $2,$12
    lui     $3,1
    and     $2,$2,$3
    /* R5900 EI encoding. */
    .word   0x42000038
    jr      $31
    sltu    $2,$zero,$2
    .end EIntr_candidate
    .size EIntr_candidate,.-EIntr_candidate
