/* Historical old EE libkernel __OPTIMIZE_SIZE__ paths.
 * Source: duduclx/PS2DEV@bac0006c6302edcf1bdae253799484497b4e5032
 * ps2sdk/ee/kernel/src/kernel.S
 * Historical `move` pseudo-ops are normalized to EE GAS `daddu` expansions.
 */
    .set noreorder
    .set nomacro
    .text
    .align 2
    .globl memcpy_candidate
    .ent memcpy_candidate
memcpy_candidate:
    .frame $sp,0,$31
    .mask 0,0
    .fmask 0,0
    daddu $8,$4,$zero
    daddu $3,$8,$zero
    addiu $6,$6,-1
    addiu $2,$zero,-1
    beq $6,$2,2f
    daddu $4,$2,$zero
1:  lbu $2,0($5)
    addiu $6,$6,-1
    addiu $5,$5,1
    sb $2,0($3)
    bne $6,$4,1b
    addiu $3,$3,1
2:  jr $31
    daddu $2,$8,$zero
    .end memcpy_candidate
    .size memcpy_candidate,.-memcpy_candidate

    .align 2
    .globl memset_candidate
    .ent memset_candidate
memset_candidate:
    .frame $sp,0,$31
    .mask 0,0
    .fmask 0,0
    daddu $3,$4,$zero
    lui $2,0xffff
    addiu $6,$6,-1
    ori $2,$2,0xffff
    beq $6,$2,4f
    nop
    lui $2,0xffff
    ori $2,$2,0xffff
3:  sb $5,0($3)
    addiu $6,$6,-1
    bne $6,$2,3b
    addiu $3,$3,1
4:  jr $31
    daddu $2,$4,$zero
    .end memset_candidate
    .size memset_candidate,.-memset_candidate

    .p2align 3
    .globl strncpy_candidate
    .ent strncpy_candidate
strncpy_candidate:
    .frame $sp,0,$31
    .mask 0,0
    .fmask 0,0
    daddu $8,$4,$zero
    .p2align 3
5:  beq $6,$zero,8f
    daddu $2,$6,$zero
    lbu $2,0($5)
    addiu $6,$6,-1
    addiu $5,$5,1
    sb $2,0($4)
    sll $2,$2,24
    bne $2,$zero,5b
    addiu $4,$4,1
    daddu $2,$6,$zero
6:  beq $2,$zero,8f
    addiu $6,$6,-1
    .p2align 3
7:  sb $zero,0($4)
    daddu $2,$6,$zero
    addiu $4,$4,1
    bne $2,$zero,7b
    addiu $6,$6,-1
8:  jr $31
    daddu $2,$8,$zero
    .end strncpy_candidate
    .size strncpy_candidate,.-strncpy_candidate

    .align 2
    .globl strlen_candidate
    .ent strlen_candidate
strlen_candidate:
    .frame $sp,0,$31
    .mask 0,0
    .fmask 0,0
    daddu $7,$4,$zero
9:  lb $2,0($4)
    beq $2,$zero,10f
    addiu $4,$4,1
    lb $2,0($4)
    bne $2,$zero,9b
    addiu $4,$4,1
10: addiu $4,$4,-1
    jr $31
    subu $2,$4,$7
    .end strlen_candidate
    .size strlen_candidate,.-strlen_candidate
