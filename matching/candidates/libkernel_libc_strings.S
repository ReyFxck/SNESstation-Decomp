/*
 * Historical old PS2DEV/PS2LIB libc string/memory assembly.
 *
 * Source lineage: duduclx/PS2DEV @ bac0006c6302edcf1bdae253799484497b4e5032,
 * ps2sdk/ee/libc/src/{strcat,strncmp,memcmp,memmove,strcpy,strchr,strcmp}.S.
 * These are the size-oriented historical assembly paths, renamed only with a
 * _candidate suffix so the isolated matcher can coexist with the recovered tree.
 */
    .set noreorder
    .set nomacro
    .text

    .align 2
    .globl strcat_candidate
    .type strcat_candidate,@function
    .ent strcat_candidate
strcat_candidate:
    addiu   $sp,$sp,-32
    sq      $16,0($sp)
    daddu   $16,$4,$zero
    sq      $31,16($sp)
1:
    lb      $2,0($4)
    bnel    $2,$zero,1b
    addiu   $4,$4,1
    jal     strcpy
    nop
    daddu   $2,$16,$zero
    lq      $31,16($sp)
    lq      $16,0($sp)
    jr      $31
    addiu   $sp,$sp,32
    .end strcat_candidate
    .size strcat_candidate,.-strcat_candidate

    .p2align 3
    .globl strncmp_candidate
    .type strncmp_candidate,@function
    .ent strncmp_candidate
strncmp_candidate:
    bne     $6,$zero,4f
    lb      $3,0($4)
2:
    jr      $31
    daddu   $2,$zero,$zero
    .p2align 2
3:
    beq     $6,$zero,2b
    nop
    beq     $7,$zero,2b
    addiu   $4,$4,1
    addiu   $5,$5,1
    lb      $3,0($4)
4:
    addiu   $6,$6,-1
    lb      $2,0($5)
    beq     $3,$2,3b
    lbu     $7,0($4)
    lbu     $2,0($5)
    lbu     $3,0($4)
    jr      $31
    subu    $2,$3,$2
    .end strncmp_candidate
    .size strncmp_candidate,.-strncmp_candidate

    .align 2
    .globl memcmp_candidate
    .type memcmp_candidate,@function
    .ent memcmp_candidate
memcmp_candidate:
    lui     $2,0xffff
    addiu   $6,$6,-1
    ori     $2,$2,0xffff
    beq     $6,$2,8f
    nop
    lui     $7,0xffff
    ori     $7,$7,0xffff
6:
    lbu     $3,0($4)
    lbu     $2,0($5)
    beq     $3,$2,7f
    addiu   $4,$4,1
    jr      $31
    subu    $2,$3,$2
7:
    addiu   $6,$6,-1
    bne     $6,$7,6b
    addiu   $5,$5,1
8:
    jr      $31
    daddu   $2,$zero,$zero
    .end memcmp_candidate
    .size memcmp_candidate,.-memcmp_candidate

    .align 2
    .globl memmove_candidate
    .type memmove_candidate,@function
    .ent memmove_candidate
memmove_candidate:
    daddu   $8,$4,$zero
    sltu    $2,$5,$8
    beq     $2,$zero,12f
    daddu   $3,$8,$zero
    addu    $7,$5,$6
    sltu    $2,$8,$7
    beq     $2,$zero,12f
    addiu   $2,$zero,-1
    addu    $3,$8,$6
    addiu   $6,$6,-1
    beq     $6,$2,14f
    daddu   $5,$7,$zero
    daddu   $4,$2,$zero
10:
    addiu   $5,$5,-1
    addiu   $3,$3,-1
    lbu     $2,0($5)
    addiu   $6,$6,-1
    bne     $6,$4,10b
    sb      $2,0($3)
    jr      $31
    daddu   $2,$8,$zero
12:
    daddu   $3,$4,$zero
    addiu   $6,$6,-1
    addiu   $2,$zero,-1
    beq     $6,$2,14f
    daddu   $4,$2,$zero
13:
    lbu     $2,0($5)
    addiu   $6,$6,-1
    addiu   $5,$5,1
    sb      $2,0($3)
    bne     $6,$4,13b
    addiu   $3,$3,1
14:
    jr      $31
    daddu   $2,$8,$zero
    .end memmove_candidate
    .size memmove_candidate,.-memmove_candidate

    .align 2
    .globl strcpy_candidate
    .type strcpy_candidate,@function
    .ent strcpy_candidate
strcpy_candidate:
    daddu   $7,$4,$zero
    daddu   $3,$4,$zero
15:
    lbu     $2,0($5)
    addiu   $5,$5,1
    sb      $2,0($3)
    sll     $2,$2,24
    bne     $2,$zero,15b
    addiu   $3,$3,1
    jr      $31
    daddu   $2,$7,$zero
    .end strcpy_candidate
    .size strcpy_candidate,.-strcpy_candidate

    .align 2
    .globl strchr_candidate
    .type strchr_candidate,@function
    .ent strchr_candidate
strchr_candidate:
    andi    $5,$5,0xff
    b       18f
    lbu     $2,0($4)
17:
    beql    $2,$5,19f
    lbu     $3,0($4)
    addiu   $4,$4,1
    lbu     $2,0($4)
18:
    bne     $2,$zero,17b
    nop
    lbu     $3,0($4)
19:
    daddu   $2,$zero,$zero
    xor     $3,$3,$5
    jr      $31
    movz    $2,$4,$3
    .end strchr_candidate
    .size strchr_candidate,.-strchr_candidate

    .align 2
    .globl strcmp_candidate
    .type strcmp_candidate,@function
    .ent strcmp_candidate
strcmp_candidate:
    lb      $2,0($4)
    beq     $2,$zero,22f
    lbu     $3,0($4)
20:
    sll     $2,$3,24
    lb      $3,0($5)
    sra     $2,$2,24
    bnel    $2,$3,22f
    lbu     $3,0($4)
    addiu   $4,$4,1
    addiu   $5,$5,1
    lb      $2,0($4)
    bne     $2,$zero,20b
    lbu     $3,0($4)
22:
    lbu     $2,0($5)
    jr      $31
    subu    $2,$3,$2
    .end strcmp_candidate
    .size strcmp_candidate,.-strcmp_candidate
