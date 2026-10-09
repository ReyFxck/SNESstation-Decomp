/*
 * Byte-exact EE reconstruction of get_tree @ 0x0018c124.
 *
 * The readable source model remains matching/candidates/get_tree.c and tracks
 * iaddis/SNESticle commit 9590ebf3bf768424ebd6cb018f322e724a7aade3,
 * Gep/Source/common/unzip/explode.c.
 *
 * The surviving SNESticle release_EE3.2.2-b1/explode.lst compiles that same
 * historical C shape to 208 bytes.  The SNES Station target is 212 bytes and
 * differs in register allocation: the target reloads %hi(bytebuf) in the outer
 * loop instead of keeping a second base register live.  This file records the
 * 212 target bytes while preserving relocations for bytebuf and ReadByte.
 *
 * It is a matching reconstruction, not a claim to be Hiryu's original source.
 */

    .set noreorder
    .set nomacro
    .text
    .align 2
    .globl get_tree_candidate
    .ent get_tree_candidate
get_tree_candidate:
    .frame $sp,96,$31
    .mask 0x801f0000,-16
    .fmask 0x00000000,0

    addiu   $sp,$sp,-96
    sd      $16,0($sp)
    lui     $16,%hi(bytebuf)
    sd      $20,64($sp)
    daddu   $20,$4,$zero
    addiu   $4,$16,%lo(bytebuf)
    sd      $19,48($sp)
    sd      $18,32($sp)
    daddu   $19,$5,$zero
    sd      $17,16($sp)
    sd      $31,80($sp)
    jal     ReadByte
    daddu   $18,$zero,$zero
    lhu     $2,%lo(bytebuf)($16)
    addiu   $17,$2,1
    lui     $16,%hi(bytebuf)
1:
    jal     ReadByte
    addiu   $4,$16,%lo(bytebuf)
    lhu     $3,%lo(bytebuf)($16)
    andi    $2,$3,0xf0
    andi    $3,$3,0x0f
    srl     $2,$2,4
    addiu   $4,$3,1
    addiu   $3,$2,1
    addu    $2,$18,$3
    sltu    $2,$19,$2
    bne     $2,$zero,3f
    addiu   $5,$zero,4
    sll     $2,$18,2
    addu    $2,$2,$20
2:
    addiu   $3,$3,-1
    sw      $4,0($2)
    addiu   $18,$18,1
    .word   0
    .word   0
    .word   0
    bne     $3,$zero,2b
    addiu   $2,$2,4
    addiu   $17,$17,-1
    bne     $17,$zero,1b
    lui     $16,%hi(bytebuf)
    xor     $2,$18,$19
    addiu   $5,$zero,4
    movz    $5,$zero,$2
3:
    ld      $31,80($sp)
    daddu   $2,$5,$zero
    ld      $20,64($sp)
    ld      $19,48($sp)
    ld      $18,32($sp)
    ld      $17,16($sp)
    ld      $16,0($sp)
    jr      $31
    addiu   $sp,$sp,96

    .end get_tree_candidate
    .size get_tree_candidate,.-get_tree_candidate
