
/mnt/data/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  1a0740: 44 00 02 3c  	lui	$2, 0x44
  1a0744: 00 6d 43 8c  	lw	$3, 0x6d00($2)
  1a0748: 00 20 02 3c  	lui	$2, 0x2000
  1a074c: 03 00 60 10  	beqz	$3, 0x1a075c <.text+0xa075c>
  1a0750: 25 20 82 00  	or	$4, $4, $2
  1a0754: 00 00 82 8c  	lw	$2, 0x0($4)
  1a0758: 00 00 62 ac  	sw	$2, 0x0($3)
  1a075c: 44 00 02 3c  	lui	$2, 0x44
  1a0760: 04 6d 43 8c  	lw	$3, 0x6d04($2)
  1a0764: 04 00 60 10  	beqz	$3, 0x1a0778 <.text+0xa0778>
  1a0768: 44 00 02 3c  	lui	$2, 0x44
  1a076c: 04 00 82 8c  	lw	$2, 0x4($4)
  1a0770: 00 00 62 ac  	sw	$2, 0x0($3)
  1a0774: 44 00 02 3c  	lui	$2, 0x44
  1a0778: 08 6d 45 8c  	lw	$5, 0x6d08($2)
  1a077c: 0d 00 a0 10  	beqz	$5, 0x1a07b4 <.text+0xa07b4>
  1a0780: 42 00 02 3c  	lui	$2, 0x42
  1a0784: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0788: 08 00 60 14  	bnez	$3, 0x1a07ac <.text+0xa07ac>
  1a078c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a0790: 00 00 82 8c  	lw	$2, 0x0($4)
  1a0794: 03 00 40 54  	bnezl	$2, 0x1a07a4 <.text+0xa07a4>
  1a0798: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a079c: 08 00 e0 03  	jr	$ra
  1a07a0: 00 00 a0 ac  	sw	$zero, 0x0($5)
  1a07a4: 08 00 e0 03  	jr	$ra
  1a07a8: 00 00 a2 ac  	sw	$2, 0x0($5)
  1a07ac: 03 00 62 10  	beq	$3, $2, 0x1a07bc <.text+0xa07bc>
  1a07b0: 00 00 00 00  	nop
  1a07b4: 08 00 e0 03  	jr	$ra
  1a07b8: 00 00 00 00  	nop
  1a07bc: 90 00 82 8c  	lw	$2, 0x90($4)
  1a07c0: 08 00 e0 03  	jr	$ra
  1a07c4: 00 00 a2 ac  	sw	$2, 0x0($5)
  1a07c8: 00 20 02 3c  	lui	$2, 0x2000
  1a07cc: 25 30 82 00  	or	$6, $4, $2
  1a07d0: 08 00 c7 8c  	lw	$7, 0x8($6)
  1a07d4: 0d 00 e0 10  	beqz	$7, 0x1a080c <.text+0xa080c>
  1a07d8: 10 00 c8 24  	addiu	$8, $6, 0x10
  1a07dc: 00 00 c2 8c  	lw	$2, 0x0($6)
  1a07e0: 0a 00 40 18  	blez	$2, 0x1a080c <.text+0xa080c>
  1a07e4: 2d 20 00 00  	move	$4, $zero
  1a07e8: 21 10 04 01  	addu	$2, $8, $4
  1a07ec: 21 18 e4 00  	addu	$3, $7, $4
  1a07f0: 00 00 42 90  	lbu	$2, 0x0($2)
  1a07f4: 01 00 84 24  	addiu	$4, $4, 0x1
  1a07f8: 00 00 62 a0  	sb	$2, 0x0($3)
  1a07fc: 00 00 c2 8c  	lw	$2, 0x0($6)
  1a0800: 2a 10 82 00  	slt	$2, $4, $2
  1a0804: f9 ff 40 14  	bnez	$2, 0x1a07ec <.text+0xa07ec>
  1a0808: 21 10 04 01  	addu	$2, $8, $4
  1a080c: 42 00 02 3c  	lui	$2, 0x42
  1a0810: 50 00 c4 24  	addiu	$4, $6, 0x50
  1a0814: d0 5a 42 8c  	lw	$2, 0x5ad0($2)
  1a0818: 20 00 c5 24  	addiu	$5, $6, 0x20
  1a081c: 0c 00 c7 8c  	lw	$7, 0xc($6)
  1a0820: 01 00 43 38  	xori	$3, $2, 0x1
  1a0824: 00 00 42 38  	xori	$2, $2, 0x0
  1a0828: 0a 40 83 00  	movz	$8, $4, $3
  1a082c: 0d 00 e0 10  	beqz	$7, 0x1a0864 <.text+0xa0864>
  1a0830: 0a 40 a2 00  	movz	$8, $5, $2
  1a0834: 04 00 c2 8c  	lw	$2, 0x4($6)
  1a0838: 0a 00 40 18  	blez	$2, 0x1a0864 <.text+0xa0864>
  1a083c: 2d 20 00 00  	move	$4, $zero
  1a0840: 21 10 04 01  	addu	$2, $8, $4
  1a0844: 21 18 e4 00  	addu	$3, $7, $4
  1a0848: 00 00 42 90  	lbu	$2, 0x0($2)
  1a084c: 01 00 84 24  	addiu	$4, $4, 0x1
  1a0850: 00 00 62 a0  	sb	$2, 0x0($3)
  1a0854: 04 00 c2 8c  	lw	$2, 0x4($6)
  1a0858: 2a 10 82 00  	slt	$2, $4, $2
  1a085c: f9 ff 40 14  	bnez	$2, 0x1a0844 <.text+0xa0844>
  1a0860: 21 10 04 01  	addu	$2, $8, $4
  1a0864: 08 00 e0 03  	jr	$ra
  1a0868: 00 00 00 00  	nop
  1a086c: 44 00 02 3c  	lui	$2, 0x44
  1a0870: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a0874: d0 6d 42 24  	addiu	$2, $2, 0x6dd0
  1a0878: 00 20 03 3c  	lui	$3, 0x2000
  1a087c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a0880: 25 88 43 00  	or	$17, $2, $3
  1a0884: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a0888: 2d 90 80 00  	move	$18, $4
  1a088c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a0890: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a0894: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  1a0898: 2d 20 20 02  	move	$4, $17
  1a089c: 2d 80 40 00  	move	$16, $2
  1a08a0: 00 04 42 28  	slti	$2, $2, 0x400
  1a08a4: 0d 00 40 10  	beqz	$2, 0x1a08dc <.text+0xa08dc>
  1a08a8: ff 03 24 26  	addiu	$4, $17, 0x3ff
  1a08ac: 2d 20 40 02  	move	$4, $18
  1a08b0: 2d 28 20 02  	move	$5, $17
  1a08b4: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  1a08b8: 2d 30 00 02  	move	$6, $16
  1a08bc: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a08c0: 21 10 30 02  	addu	$2, $17, $16
  1a08c4: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a08c8: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a08cc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a08d0: 00 00 40 a0  	sb	$zero, 0x0($2)
  1a08d4: 08 00 e0 03  	jr	$ra
  1a08d8: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a08dc: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  1a08e0: 00 00 00 00  	nop
  1a08e4: f1 ff 00 10  	b	0x1a08ac <.text+0xa08ac>
  1a08e8: 2d 80 40 00  	move	$16, $2
  1a08ec: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a08f0: 42 00 02 3c  	lui	$2, 0x42
  1a08f4: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a08f8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a08fc: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a0900: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a0904: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0908: 07 00 40 10  	beqz	$2, 0x1a0928 <.text+0xa0928>
  1a090c: 2d 80 80 00  	move	$16, $4
  1a0910: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a0914: 2d 10 60 00  	move	$2, $3
  1a0918: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a091c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0920: 08 00 e0 03  	jr	$ra
  1a0924: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a0928: 03 73 06 0c  	jal	0x19cc0c <.text+0x9cc0c>
  1a092c: 2d 20 00 00  	move	$4, $zero
  1a0930: 42 00 02 3c  	lui	$2, 0x42
  1a0934: d0 5a 50 ac  	sw	$16, 0x5ad0($2)
  1a0938: 44 00 02 3c  	lui	$2, 0x44
  1a093c: 00 80 05 3c  	lui	$5, 0x8000
  1a0940: c0 64 51 24  	addiu	$17, $2, 0x64c0
  1a0944: 00 04 a5 34  	ori	$5, $5, 0x400
  1a0948: 2d 20 20 02  	move	$4, $17
  1a094c: a2 71 06 0c  	jal	0x19c688 <.text+0x9c688>
  1a0950: 2d 30 00 00  	move	$6, $zero
  1a0954: 2d 48 40 00  	move	$9, $2
  1a0958: ed ff 40 04  	bltz	$2, 0x1a0910 <.text+0xa0910>
  1a095c: 2d 18 40 00  	move	$3, $2
  1a0960: 24 00 22 8e  	lw	$2, 0x24($17)
  1a0964: 0f 00 03 3c  	lui	$3, 0xf
  1a0968: 0f 00 40 14  	bnez	$2, 0x1a09a8 <.text+0xa09a8>
  1a096c: ff ff 63 34  	ori	$3, $3, 0xffff
		...
  1a0984: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  1a0988: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
		...
  1a0998: f5 ff 62 14  	bne	$3, $2, 0x1a0970 <.text+0xa0970>
  1a099c: 44 00 02 3c  	lui	$2, 0x44
  1a09a0: e7 ff 00 10  	b	0x1a0940 <.text+0xa0940>
  1a09a4: 00 80 05 3c  	lui	$5, 0x8000
  1a09a8: 42 00 02 3c  	lui	$2, 0x42
  1a09ac: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a09b0: 04 00 60 10  	beqz	$3, 0x1a09c4 <.text+0xa09c4>
  1a09b4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a09b8: 08 00 62 10  	beq	$3, $2, 0x1a09dc <.text+0xa09dc>
  1a09bc: 44 00 02 3c  	lui	$2, 0x44
  1a09c0: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a09c4: 42 00 03 3c  	lui	$3, 0x42
  1a09c8: c8 5a 62 ac  	sw	$2, 0x5ac8($3)
  1a09cc: 2d 18 20 01  	move	$3, $9
  1a09d0: 42 00 02 3c  	lui	$2, 0x42
  1a09d4: ce ff 00 10  	b	0x1a0910 <.text+0xa0910>
  1a09d8: cc 5a 40 ac  	sw	$zero, 0x5acc($2)
  1a09dc: 44 00 07 3c  	lui	$7, 0x44
  1a09e0: 00 65 50 24  	addiu	$16, $2, 0x6500
  1a09e4: 2d 20 20 02  	move	$4, $17
  1a09e8: 1c 00 02 3c  	lui	$2, 0x1c
  1a09ec: 40 72 e7 24  	addiu	$7, $7, 0x7240
  1a09f0: 9c a7 45 8c  	lw	$5, -0x5864($2)
  1a09f4: 2d 30 00 00  	move	$6, $zero
  1a09f8: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a09fc: 2d 48 00 02  	move	$9, $16
  1a0a00: 0c 00 0a 24  	addiu	$10, $zero, 0xc
  1a0a04: 2d 58 00 00  	move	$11, $zero
  1a0a08: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0a0c: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0a10: 09 00 40 14  	bnez	$2, 0x1a0a38 <.text+0xa0a38>
  1a0a14: 9c ff 43 24  	addiu	$3, $2, -0x64 <.text+0xffffffffffefff9c>
  1a0a18: 04 00 02 26  	addiu	$2, $16, 0x4
  1a0a1c: 00 20 03 3c  	lui	$3, 0x2000
  1a0a20: 25 10 43 00  	or	$2, $2, $3
  1a0a24: 00 00 49 8c  	lw	$9, 0x0($2)
  1a0a28: 05 02 22 29  	slti	$2, $9, 0x205
  1a0a2c: 05 00 40 10  	beqz	$2, 0x1a0a44 <.text+0xa0a44>
  1a0a30: 08 00 02 26  	addiu	$2, $16, 0x8
  1a0a34: 88 ff 03 24  	addiu	$3, $zero, -0x78 <.text+0xffffffffffefff88>
  1a0a38: 42 00 02 3c  	lui	$2, 0x42
  1a0a3c: b4 ff 00 10  	b	0x1a0910 <.text+0xa0910>
  1a0a40: c8 5a 40 ac  	sw	$zero, 0x5ac8($2)
  1a0a44: 25 10 62 00  	or	$2, $3, $2
  1a0a48: 00 00 49 8c  	lw	$9, 0x0($2)
  1a0a4c: 06 02 22 29  	slti	$2, $9, 0x206
  1a0a50: 03 00 40 10  	beqz	$2, 0x1a0a60 <.text+0xa0a60>
  1a0a54: 25 10 03 02  	or	$2, $16, $3
  1a0a58: f7 ff 00 10  	b	0x1a0a38 <.text+0xa0a38>
  1a0a5c: 87 ff 03 24  	addiu	$3, $zero, -0x79 <.text+0xffffffffffefff87>
  1a0a60: d7 ff 00 10  	b	0x1a09c0 <.text+0xa09c0>
  1a0a64: 00 00 49 8c  	lw	$9, 0x0($2)
  1a0a68: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a0a6c: 42 00 02 3c  	lui	$2, 0x42
  1a0a70: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a0a74: 2d 50 80 00  	move	$10, $4
  1a0a78: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0a7c: 2d 58 a0 00  	move	$11, $5
  1a0a80: 2d 48 c0 00  	move	$9, $6
  1a0a84: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0a88: 3b 00 40 10  	beqz	$2, 0x1a0b78 <.text+0xa0b78>
  1a0a8c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0a90: 42 00 02 3c  	lui	$2, 0x42
  1a0a94: cc 5a 42 8c  	lw	$2, 0x5acc($2)
  1a0a98: 37 00 40 14  	bnez	$2, 0x1a0b78 <.text+0xa0b78>
  1a0a9c: 2d 18 40 00  	move	$3, $2
  1a0aa0: 42 00 02 3c  	lui	$2, 0x42
  1a0aa4: d0 5a 42 8c  	lw	$2, 0x5ad0($2)
  1a0aa8: 38 00 40 14  	bnez	$2, 0x1a0b8c <.text+0xa0b8c>
  1a0aac: 44 00 03 3c  	lui	$3, 0x44
  1a0ab0: 2b 20 06 00  	sltu	$4, $zero, $6
  1a0ab4: c0 76 63 24  	addiu	$3, $3, 0x76c0
  1a0ab8: 2b 28 07 00  	sltu	$5, $zero, $7
  1a0abc: 2b 30 08 00  	sltu	$6, $zero, $8
  1a0ac0: 44 00 02 3c  	lui	$2, 0x44
  1a0ac4: 44 00 10 3c  	lui	$16, 0x44
  1a0ac8: 10 6d 42 24  	addiu	$2, $2, 0x6d10
  1a0acc: 04 00 6a ac  	sw	$10, 0x4($3)
  1a0ad0: 1c 00 62 ac  	sw	$2, 0x1c($3)
  1a0ad4: 10 6d 10 26  	addiu	$16, $16, 0x6d10
  1a0ad8: 44 00 02 3c  	lui	$2, 0x44
  1a0adc: 08 00 6b ac  	sw	$11, 0x8($3)
  1a0ae0: 0c 00 64 ac  	sw	$4, 0xc($3)
  1a0ae4: 2d 20 00 02  	move	$4, $16
  1a0ae8: 10 00 65 ac  	sw	$5, 0x10($3)
  1a0aec: c0 00 05 24  	addiu	$5, $zero, 0xc0
  1a0af0: 14 00 66 ac  	sw	$6, 0x14($3)
  1a0af4: 00 6d 49 ac  	sw	$9, 0x6d00($2)
  1a0af8: 44 00 02 3c  	lui	$2, 0x44
  1a0afc: 04 6d 47 ac  	sw	$7, 0x6d04($2)
  1a0b00: 44 00 02 3c  	lui	$2, 0x44
  1a0b04: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  1a0b08: 08 6d 48 ac  	sw	$8, 0x6d08($2)
  1a0b0c: 42 00 02 3c  	lui	$2, 0x42
  1a0b10: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0b14: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0b18: 44 00 07 3c  	lui	$7, 0x44
  1a0b1c: 1c 00 02 3c  	lui	$2, 0x1c
  1a0b20: 44 00 09 3c  	lui	$9, 0x44
  1a0b24: 18 28 64 00  	<unknown>
  1a0b28: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0b2c: 44 00 04 3c  	lui	$4, 0x44
  1a0b30: 1a 00 0b 3c  	lui	$11, 0x1a
  1a0b34: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0b38: c0 76 e7 24  	addiu	$7, $7, 0x76c0
  1a0b3c: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0b40: 40 07 6b 25  	addiu	$11, $11, 0x740
  1a0b44: 21 18 a2 00  	addu	$3, $5, $2
  1a0b48: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0b4c: 04 00 65 8c  	lw	$5, 0x4($3)
  1a0b50: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0b54: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0b58: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0b5c: 00 00 b0 af  	sw	$16, 0x0($sp)
  1a0b60: 05 00 40 14  	bnez	$2, 0x1a0b78 <.text+0xa0b78>
  1a0b64: 2d 18 40 00  	move	$3, $2
  1a0b68: 01 00 03 24  	addiu	$3, $zero, 0x1
  1a0b6c: 42 00 02 3c  	lui	$2, 0x42
  1a0b70: cc 5a 43 ac  	sw	$3, 0x5acc($2)
  1a0b74: 2d 18 00 00  	move	$3, $zero
  1a0b78: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a0b7c: 2d 10 60 00  	move	$2, $3
  1a0b80: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0b84: 08 00 e0 03  	jr	$ra
  1a0b88: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a0b8c: 2b 20 08 00  	sltu	$4, $zero, $8
  1a0b90: c0 76 63 24  	addiu	$3, $3, 0x76c0
  1a0b94: 2b 28 07 00  	sltu	$5, $zero, $7
  1a0b98: c9 ff 00 10  	b	0x1a0ac0 <.text+0xa0ac0>
  1a0b9c: 2b 30 06 00  	sltu	$6, $zero, $6
  1a0ba0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a0ba4: 42 00 02 3c  	lui	$2, 0x42
  1a0ba8: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a0bac: 2d 48 a0 00  	move	$9, $5
  1a0bb0: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a0bb4: 2d 40 80 00  	move	$8, $4
  1a0bb8: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0bbc: 2d 28 c0 00  	move	$5, $6
  1a0bc0: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0bc4: 05 00 40 10  	beqz	$2, 0x1a0bdc <.text+0xa0bdc>
  1a0bc8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0bcc: 42 00 11 3c  	lui	$17, 0x42
  1a0bd0: cc 5a 22 8e  	lw	$2, 0x5acc($17)
  1a0bd4: 07 00 40 10  	beqz	$2, 0x1a0bf4 <.text+0xa0bf4>
  1a0bd8: 2d 18 40 00  	move	$3, $2
  1a0bdc: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a0be0: 2d 10 60 00  	move	$2, $3
  1a0be4: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a0be8: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0bec: 08 00 e0 03  	jr	$ra
  1a0bf0: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a0bf4: 44 00 02 3c  	lui	$2, 0x44
  1a0bf8: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  1a0bfc: 40 72 50 24  	addiu	$16, $2, 0x7240
  1a0c00: 40 72 48 ac  	sw	$8, 0x7240($2)
  1a0c04: 14 00 04 26  	addiu	$4, $16, 0x14
  1a0c08: 04 00 09 ae  	sw	$9, 0x4($16)
  1a0c0c: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a0c10: 08 00 07 ae  	sw	$7, 0x8($16)
  1a0c14: 42 00 02 3c  	lui	$2, 0x42
  1a0c18: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0c1c: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0c20: 44 00 09 3c  	lui	$9, 0x44
  1a0c24: 1c 00 02 3c  	lui	$2, 0x1c
  1a0c28: 2d 38 00 02  	move	$7, $16
  1a0c2c: 18 28 64 00  	<unknown>
  1a0c30: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0c34: 44 00 04 3c  	lui	$4, 0x44
  1a0c38: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0c3c: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0c40: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0c44: 14 04 08 24  	addiu	$8, $zero, 0x414
  1a0c48: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0c4c: 21 18 a2 00  	addu	$3, $5, $2
  1a0c50: 2d 58 00 00  	move	$11, $zero
  1a0c54: 08 00 65 8c  	lw	$5, 0x8($3)
  1a0c58: 13 04 00 a2  	sb	$zero, 0x413($16)
  1a0c5c: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0c60: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0c64: dd ff 40 14  	bnez	$2, 0x1a0bdc <.text+0xa0bdc>
  1a0c68: 2d 18 40 00  	move	$3, $2
  1a0c6c: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a0c70: 2d 18 00 00  	move	$3, $zero
  1a0c74: d9 ff 00 10  	b	0x1a0bdc <.text+0xa0bdc>
  1a0c78: cc 5a 22 ae  	sw	$2, 0x5acc($17)
  1a0c7c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a0c80: 42 00 02 3c  	lui	$2, 0x42
  1a0c84: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a0c88: 2d 68 80 00  	move	$13, $4
  1a0c8c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0c90: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0c94: 05 00 40 10  	beqz	$2, 0x1a0cac <.text+0xa0cac>
  1a0c98: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0c9c: 42 00 10 3c  	lui	$16, 0x42
  1a0ca0: cc 5a 02 8e  	lw	$2, 0x5acc($16)
  1a0ca4: 06 00 40 10  	beqz	$2, 0x1a0cc0 <.text+0xa0cc0>
  1a0ca8: 2d 18 40 00  	move	$3, $2
  1a0cac: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a0cb0: 2d 10 60 00  	move	$2, $3
  1a0cb4: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0cb8: 08 00 e0 03  	jr	$ra
  1a0cbc: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a0cc0: 42 00 02 3c  	lui	$2, 0x42
  1a0cc4: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0cc8: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0ccc: 44 00 0c 3c  	lui	$12, 0x44
  1a0cd0: 1c 00 02 3c  	lui	$2, 0x1c
  1a0cd4: 44 00 09 3c  	lui	$9, 0x44
  1a0cd8: 18 28 64 00  	<unknown>
  1a0cdc: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0ce0: 44 00 04 3c  	lui	$4, 0x44
  1a0ce4: 80 76 87 25  	addiu	$7, $12, 0x7680
  1a0ce8: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0cec: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0cf0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0cf4: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0cf8: 21 18 a2 00  	addu	$3, $5, $2
  1a0cfc: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0d00: 0c 00 65 8c  	lw	$5, 0xc($3)
  1a0d04: 2d 58 00 00  	move	$11, $zero
  1a0d08: 80 76 8d ad  	sw	$13, 0x7680($12)
  1a0d0c: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0d10: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0d14: e5 ff 40 14  	bnez	$2, 0x1a0cac <.text+0xa0cac>
  1a0d18: 2d 18 40 00  	move	$3, $2
  1a0d1c: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a0d20: 2d 18 00 00  	move	$3, $zero
  1a0d24: e1 ff 00 10  	b	0x1a0cac <.text+0xa0cac>
  1a0d28: cc 5a 02 ae  	sw	$2, 0x5acc($16)
  1a0d2c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a0d30: 42 00 02 3c  	lui	$2, 0x42
  1a0d34: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a0d38: 2d 78 80 00  	move	$15, $4
  1a0d3c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0d40: 2d 68 a0 00  	move	$13, $5
  1a0d44: 2d 70 c0 00  	move	$14, $6
  1a0d48: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0d4c: 05 00 40 10  	beqz	$2, 0x1a0d64 <.text+0xa0d64>
  1a0d50: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0d54: 42 00 10 3c  	lui	$16, 0x42
  1a0d58: cc 5a 02 8e  	lw	$2, 0x5acc($16)
  1a0d5c: 06 00 40 10  	beqz	$2, 0x1a0d78 <.text+0xa0d78>
  1a0d60: 2d 18 40 00  	move	$3, $2
  1a0d64: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a0d68: 2d 10 60 00  	move	$2, $3
  1a0d6c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0d70: 08 00 e0 03  	jr	$ra
  1a0d74: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a0d78: 42 00 02 3c  	lui	$2, 0x42
  1a0d7c: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0d80: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0d84: 44 00 0c 3c  	lui	$12, 0x44
  1a0d88: 1c 00 02 3c  	lui	$2, 0x1c
  1a0d8c: 44 00 09 3c  	lui	$9, 0x44
  1a0d90: 18 28 64 00  	<unknown>
  1a0d94: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0d98: 44 00 04 3c  	lui	$4, 0x44
  1a0d9c: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0da0: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0da4: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0da8: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0dac: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0db0: 21 18 a2 00  	addu	$3, $5, $2
  1a0db4: 2d 58 00 00  	move	$11, $zero
  1a0db8: 80 76 82 25  	addiu	$2, $12, 0x7680
  1a0dbc: 10 00 65 8c  	lw	$5, 0x10($3)
  1a0dc0: 10 00 4d ac  	sw	$13, 0x10($2)
  1a0dc4: 2d 38 40 00  	move	$7, $2
  1a0dc8: 14 00 4e ac  	sw	$14, 0x14($2)
  1a0dcc: 80 76 8f ad  	sw	$15, 0x7680($12)
  1a0dd0: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0dd4: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0dd8: e2 ff 40 14  	bnez	$2, 0x1a0d64 <.text+0xa0d64>
  1a0ddc: 2d 18 40 00  	move	$3, $2
  1a0de0: 04 00 02 24  	addiu	$2, $zero, 0x4
  1a0de4: 2d 18 00 00  	move	$3, $zero
  1a0de8: de ff 00 10  	b	0x1a0d64 <.text+0xa0d64>
  1a0dec: cc 5a 02 ae  	sw	$2, 0x5acc($16)
  1a0df0: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a0df4: 42 00 02 3c  	lui	$2, 0x42
  1a0df8: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1a0dfc: 2d 40 80 00  	move	$8, $4
  1a0e00: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a0e04: 2d 38 a0 00  	move	$7, $5
  1a0e08: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a0e0c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0e10: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0e14: 05 00 40 10  	beqz	$2, 0x1a0e2c <.text+0xa0e2c>
  1a0e18: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0e1c: 42 00 12 3c  	lui	$18, 0x42
  1a0e20: cc 5a 42 8e  	lw	$2, 0x5acc($18)
  1a0e24: 08 00 40 10  	beqz	$2, 0x1a0e48 <.text+0xa0e48>
  1a0e28: 2d 18 40 00  	move	$3, $2
  1a0e2c: 40 00 bf df  	ld	$ra, 0x40($sp)
  1a0e30: 2d 10 60 00  	move	$2, $3
  1a0e34: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a0e38: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a0e3c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0e40: 08 00 e0 03  	jr	$ra
  1a0e44: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a0e48: 44 00 02 3c  	lui	$2, 0x44
  1a0e4c: 44 00 11 3c  	lui	$17, 0x44
  1a0e50: 80 76 50 24  	addiu	$16, $2, 0x7680
  1a0e54: 2d 28 c0 00  	move	$5, $6
  1a0e58: 2d 20 e0 00  	move	$4, $7
  1a0e5c: 10 6d 31 26  	addiu	$17, $17, 0x6d10
  1a0e60: 80 76 48 ac  	sw	$8, 0x7680($2)
  1a0e64: 0c 00 06 ae  	sw	$6, 0xc($16)
  1a0e68: 18 00 07 ae  	sw	$7, 0x18($16)
  1a0e6c: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  1a0e70: 1c 00 11 ae  	sw	$17, 0x1c($16)
  1a0e74: 2d 20 20 02  	move	$4, $17
  1a0e78: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  1a0e7c: c0 00 05 24  	addiu	$5, $zero, 0xc0
  1a0e80: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0e84: 42 00 02 3c  	lui	$2, 0x42
  1a0e88: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0e8c: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0e90: 44 00 09 3c  	lui	$9, 0x44
  1a0e94: 1c 00 02 3c  	lui	$2, 0x1c
  1a0e98: 1a 00 0b 3c  	lui	$11, 0x1a
  1a0e9c: 18 28 64 00  	<unknown>
  1a0ea0: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0ea4: 44 00 04 3c  	lui	$4, 0x44
  1a0ea8: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0eac: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0eb0: 2d 38 00 02  	move	$7, $16
  1a0eb4: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0eb8: c8 07 6b 25  	addiu	$11, $11, 0x7c8
  1a0ebc: 21 18 a2 00  	addu	$3, $5, $2
  1a0ec0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0ec4: 14 00 65 8c  	lw	$5, 0x14($3)
  1a0ec8: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0ecc: 00 00 b1 af  	sw	$17, 0x0($sp)
  1a0ed0: d6 ff 40 14  	bnez	$2, 0x1a0e2c <.text+0xa0e2c>
  1a0ed4: 2d 18 40 00  	move	$3, $2
  1a0ed8: 05 00 02 24  	addiu	$2, $zero, 0x5
  1a0edc: 2d 18 00 00  	move	$3, $zero
  1a0ee0: d2 ff 00 10  	b	0x1a0e2c <.text+0xa0e2c>
  1a0ee4: cc 5a 42 ae  	sw	$2, 0x5acc($18)
  1a0ee8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a0eec: 42 00 02 3c  	lui	$2, 0x42
  1a0ef0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a0ef4: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0ef8: 37 00 40 10  	beqz	$2, 0x1a0fd8 <.text+0xa0fd8>
  1a0efc: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0f00: 42 00 02 3c  	lui	$2, 0x42
  1a0f04: cc 5a 42 8c  	lw	$2, 0x5acc($2)
  1a0f08: 33 00 40 14  	bnez	$2, 0x1a0fd8 <.text+0xa0fd8>
  1a0f0c: 2d 18 40 00  	move	$3, $2
  1a0f10: 44 00 02 3c  	lui	$2, 0x44
  1a0f14: 11 00 c3 28  	slti	$3, $6, 0x11
  1a0f18: 80 76 44 ac  	sw	$4, 0x7680($2)
  1a0f1c: 32 00 60 10  	beqz	$3, 0x1a0fe8 <.text+0xa0fe8>
  1a0f20: 80 76 47 24  	addiu	$7, $2, 0x7680
  1a0f24: 14 00 e6 ac  	sw	$6, 0x14($7)
  1a0f28: 18 00 e0 ac  	sw	$zero, 0x18($7)
  1a0f2c: 0c 00 e0 ac  	sw	$zero, 0xc($7)
  1a0f30: 44 00 07 3c  	lui	$7, 0x44
  1a0f34: 80 76 e2 24  	addiu	$2, $7, 0x7680
  1a0f38: 14 00 42 8c  	lw	$2, 0x14($2)
  1a0f3c: 0a 00 40 18  	blez	$2, 0x1a0f68 <.text+0xa0f68>
  1a0f40: 2d 30 00 00  	move	$6, $zero
  1a0f44: 80 76 e2 24  	addiu	$2, $7, 0x7680
  1a0f48: 21 20 a6 00  	addu	$4, $5, $6
  1a0f4c: 14 00 43 8c  	lw	$3, 0x14($2)
  1a0f50: 21 10 c2 00  	addu	$2, $6, $2
  1a0f54: 00 00 84 90  	lbu	$4, 0x0($4)
  1a0f58: 01 00 c6 24  	addiu	$6, $6, 0x1
  1a0f5c: 2a 18 c3 00  	slt	$3, $6, $3
  1a0f60: f8 ff 60 14  	bnez	$3, 0x1a0f44 <.text+0xa0f44>
  1a0f64: 20 00 44 a0  	sb	$4, 0x20($2)
  1a0f68: ac 73 06 0c  	jal	0x19ceb0 <.text+0x9ceb0>
  1a0f6c: 2d 20 00 00  	move	$4, $zero
  1a0f70: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0f74: 42 00 02 3c  	lui	$2, 0x42
  1a0f78: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0f7c: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0f80: 44 00 07 3c  	lui	$7, 0x44
  1a0f84: 1c 00 02 3c  	lui	$2, 0x1c
  1a0f88: 44 00 09 3c  	lui	$9, 0x44
  1a0f8c: 18 28 64 00  	<unknown>
  1a0f90: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0f94: 44 00 04 3c  	lui	$4, 0x44
  1a0f98: 2d 58 00 00  	move	$11, $zero
  1a0f9c: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0fa0: 80 76 e7 24  	addiu	$7, $7, 0x7680
  1a0fa4: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0fa8: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0fac: 21 18 a2 00  	addu	$3, $5, $2
  1a0fb0: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0fb4: 18 00 65 8c  	lw	$5, 0x18($3)
  1a0fb8: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0fbc: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0fc0: 05 00 40 14  	bnez	$2, 0x1a0fd8 <.text+0xa0fd8>
  1a0fc4: 2d 18 40 00  	move	$3, $2
  1a0fc8: 06 00 03 24  	addiu	$3, $zero, 0x6
  1a0fcc: 42 00 02 3c  	lui	$2, 0x42
  1a0fd0: cc 5a 43 ac  	sw	$3, 0x5acc($2)
  1a0fd4: 2d 18 00 00  	move	$3, $zero
  1a0fd8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a0fdc: 2d 10 60 00  	move	$2, $3
  1a0fe0: 08 00 e0 03  	jr	$ra
  1a0fe4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a0fe8: f0 ff 03 24  	addiu	$3, $zero, -0x10 <.text+0xffffffffffeffff0>
  1a0fec: ff ff a2 24  	addiu	$2, $5, -0x1 <.text+0xffffffffffefffff>
  1a0ff0: 24 10 43 00  	and	$2, $2, $3
  1a0ff4: 23 20 45 00  	subu	$4, $2, $5
  1a0ff8: 23 10 a2 00  	subu	$2, $5, $2
  1a0ffc: 21 18 a4 00  	addu	$3, $5, $4
  1a1000: 21 10 c2 00  	addu	$2, $6, $2
  1a1004: f0 ff 42 24  	addiu	$2, $2, -0x10 <.text+0xffffffffffeffff0>
  1a1008: 10 00 63 24  	addiu	$3, $3, 0x10
  1a100c: 10 00 84 24  	addiu	$4, $4, 0x10
  1a1010: 18 00 e3 ac  	sw	$3, 0x18($7)
  1a1014: 0c 00 e2 ac  	sw	$2, 0xc($7)
  1a1018: c5 ff 00 10  	b	0x1a0f30 <.text+0xa0f30>
  1a101c: 14 00 e4 ac  	sw	$4, 0x14($7)
  1a1020: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a1024: 42 00 02 3c  	lui	$2, 0x42
  1a1028: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a102c: 2d 68 80 00  	move	$13, $4
  1a1030: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a1034: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a1038: 05 00 40 10  	beqz	$2, 0x1a1050 <.text+0xa1050>
  1a103c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1040: 42 00 10 3c  	lui	$16, 0x42
  1a1044: cc 5a 02 8e  	lw	$2, 0x5acc($16)
  1a1048: 06 00 40 10  	beqz	$2, 0x1a1064 <.text+0xa1064>
  1a104c: 2d 18 40 00  	move	$3, $2
  1a1050: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a1054: 2d 10 60 00  	move	$2, $3
  1a1058: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a105c: 08 00 e0 03  	jr	$ra
  1a1060: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a1064: 42 00 02 3c  	lui	$2, 0x42
  1a1068: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a106c: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a1070: 44 00 0c 3c  	lui	$12, 0x44
  1a1074: 1c 00 02 3c  	lui	$2, 0x1c
  1a1078: 44 00 09 3c  	lui	$9, 0x44
  1a107c: 18 28 64 00  	<unknown>
  1a1080: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a1084: 44 00 04 3c  	lui	$4, 0x44
  1a1088: 80 76 87 25  	addiu	$7, $12, 0x7680
  1a108c: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a1090: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a1094: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a1098: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a109c: 21 18 a2 00  	addu	$3, $5, $2
  1a10a0: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a10a4: 1c 00 65 8c  	lw	$5, 0x1c($3)
  1a10a8: 2d 58 00 00  	move	$11, $zero
  1a10ac: 80 76 8d ad  	sw	$13, 0x7680($12)
  1a10b0: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a10b4: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a10b8: e5 ff 40 14  	bnez	$2, 0x1a1050 <.text+0xa1050>
  1a10bc: 2d 18 40 00  	move	$3, $2
  1a10c0: 0a 00 02 24  	addiu	$2, $zero, 0xa
  1a10c4: 2d 18 00 00  	move	$3, $zero
  1a10c8: e1 ff 00 10  	b	0x1a1050 <.text+0xa1050>
  1a10cc: cc 5a 02 ae  	sw	$2, 0x5acc($16)
  1a10d0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a10d4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a10d8: e8 82 06 0c  	jal	0x1a0ba0 <.text+0xa0ba0>
  1a10dc: 40 00 07 24  	addiu	$7, $zero, 0x40
  1a10e0: 03 00 40 10  	beqz	$2, 0x1a10f0 <.text+0xa10f0>
  1a10e4: 0b 00 04 24  	addiu	$4, $zero, 0xb
  1a10e8: 42 00 03 3c  	lui	$3, 0x42
  1a10ec: cc 5a 64 ac  	sw	$4, 0x5acc($3)
  1a10f0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a10f4: 08 00 e0 03  	jr	$ra
  1a10f8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a10fc: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  1a1100: 42 00 02 3c  	lui	$2, 0x42
  1a1104: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a1108: 2d 40 a0 00  	move	$8, $5
  1a110c: 50 00 bf ff  	sd	$ra, 0x50($sp)
  1a1110: 2d 48 80 00  	move	$9, $4
  1a1114: 40 00 b3 ff  	sd	$19, 0x40($sp)
  1a1118: 2d 28 c0 00  	move	$5, $6
  1a111c: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a1120: 2d 90 e0 00  	move	$18, $7
  1a1124: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a1128: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a112c: 05 00 40 10  	beqz	$2, 0x1a1144 <.text+0xa1144>
  1a1130: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1134: 42 00 13 3c  	lui	$19, 0x42
  1a1138: cc 5a 62 8e  	lw	$2, 0x5acc($19)
  1a113c: 09 00 40 10  	beqz	$2, 0x1a1164 <.text+0xa1164>
  1a1140: 2d 18 40 00  	move	$3, $2
  1a1144: 50 00 bf df  	ld	$ra, 0x50($sp)
  1a1148: 2d 10 60 00  	move	$2, $3
  1a114c: 40 00 b3 df  	ld	$19, 0x40($sp)
  1a1150: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a1154: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a1158: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a115c: 08 00 e0 03  	jr	$ra
  1a1160: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  1a1164: 44 00 02 3c  	lui	$2, 0x44
  1a1168: 44 00 10 3c  	lui	$16, 0x44
  1a116c: 40 72 51 24  	addiu	$17, $2, 0x7240
  1a1170: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  1a1174: d0 6d 10 26  	addiu	$16, $16, 0x6dd0
  1a1178: 14 00 24 26  	addiu	$4, $17, 0x14
  1a117c: 40 72 49 ac  	sw	$9, 0x7240($2)
  1a1180: 04 00 28 ae  	sw	$8, 0x4($17)
  1a1184: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a1188: 10 00 30 ae  	sw	$16, 0x10($17)
  1a118c: 2d 20 00 02  	move	$4, $16
  1a1190: 00 04 05 24  	addiu	$5, $zero, 0x400
  1a1194: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  1a1198: 13 04 20 a2  	sb	$zero, 0x413($17)
  1a119c: 42 00 02 3c  	lui	$2, 0x42
  1a11a0: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a11a4: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a11a8: 44 00 09 3c  	lui	$9, 0x44
  1a11ac: 1c 00 02 3c  	lui	$2, 0x1c
  1a11b0: 1a 00 0b 3c  	lui	$11, 0x1a
  1a11b4: 18 28 64 00  	<unknown>
  1a11b8: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a11bc: 44 00 04 3c  	lui	$4, 0x44
  1a11c0: 2d 38 20 02  	move	$7, $17
  1a11c4: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a11c8: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a11cc: 6c 08 6b 25  	addiu	$11, $11, 0x86c
  1a11d0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a11d4: 21 18 a2 00  	addu	$3, $5, $2
  1a11d8: 14 04 08 24  	addiu	$8, $zero, 0x414
  1a11dc: 20 00 65 8c  	lw	$5, 0x20($3)
  1a11e0: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a11e4: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a11e8: 00 00 b2 af  	sw	$18, 0x0($sp)
  1a11ec: d5 ff 40 14  	bnez	$2, 0x1a1144 <.text+0xa1144>
  1a11f0: 2d 18 40 00  	move	$3, $2
  1a11f4: 0c 00 02 24  	addiu	$2, $zero, 0xc
  1a11f8: 2d 18 00 00  	move	$3, $zero
  1a11fc: d1 ff 00 10  	b	0x1a1144 <.text+0xa1144>
  1a1200: cc 5a 62 ae  	sw	$2, 0x5acc($19)
  1a1204: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  1a1208: 42 00 02 3c  	lui	$2, 0x42
  1a120c: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a1210: 2d 58 a0 00  	move	$11, $5
  1a1214: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a1218: 2d 50 80 00  	move	$10, $4
  1a121c: 50 00 bf ff  	sd	$ra, 0x50($sp)
  1a1220: 2d 28 c0 00  	move	$5, $6
  1a1224: 40 00 b3 ff  	sd	$19, 0x40($sp)
  1a1228: 2d 88 00 01  	move	$17, $8
  1a122c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a1230: 2d 90 20 01  	move	$18, $9
  1a1234: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a1238: 05 00 40 10  	beqz	$2, 0x1a1250 <.text+0xa1250>
  1a123c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1240: 42 00 13 3c  	lui	$19, 0x42
  1a1244: cc 5a 62 8e  	lw	$2, 0x5acc($19)
  1a1248: 09 00 40 10  	beqz	$2, 0x1a1270 <.text+0xa1270>
  1a124c: 2d 18 40 00  	move	$3, $2
  1a1250: 50 00 bf df  	ld	$ra, 0x50($sp)
  1a1254: 2d 10 60 00  	move	$2, $3
  1a1258: 40 00 b3 df  	ld	$19, 0x40($sp)
  1a125c: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a1260: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a1264: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a1268: 08 00 e0 03  	jr	$ra
  1a126c: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  1a1270: 44 00 02 3c  	lui	$2, 0x44
  1a1274: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  1a1278: 40 72 50 24  	addiu	$16, $2, 0x7240
  1a127c: 40 72 4a ac  	sw	$10, 0x7240($2)
  1a1280: 14 00 04 26  	addiu	$4, $16, 0x14
  1a1284: 04 00 0b ae  	sw	$11, 0x4($16)
  1a1288: 08 00 07 ae  	sw	$7, 0x8($16)
  1a128c: 0c 00 08 ae  	sw	$8, 0xc($16)
  1a1290: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a1294: 10 00 09 ae  	sw	$9, 0x10($16)
  1a1298: 80 29 11 00  	sll	$5, $17, 0x6
  1a129c: 2d 20 40 02  	move	$4, $18
  1a12a0: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  1a12a4: 13 04 00 a2  	sb	$zero, 0x413($16)
  1a12a8: 42 00 02 3c  	lui	$2, 0x42
  1a12ac: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a12b0: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a12b4: 44 00 09 3c  	lui	$9, 0x44
  1a12b8: 1c 00 02 3c  	lui	$2, 0x1c
  1a12bc: 2d 58 00 00  	move	$11, $zero
  1a12c0: 18 28 64 00  	<unknown>
  1a12c4: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a12c8: 44 00 04 3c  	lui	$4, 0x44
  1a12cc: 2d 38 00 02  	move	$7, $16
  1a12d0: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a12d4: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a12d8: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a12dc: 14 04 08 24  	addiu	$8, $zero, 0x414
  1a12e0: 21 18 a2 00  	addu	$3, $5, $2
  1a12e4: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a12e8: 24 00 65 8c  	lw	$5, 0x24($3)
  1a12ec: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a12f0: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a12f4: d6 ff 40 14  	bnez	$2, 0x1a1250 <.text+0xa1250>
  1a12f8: 2d 18 40 00  	move	$3, $2
  1a12fc: 0d 00 02 24  	addiu	$2, $zero, 0xd
  1a1300: 2d 18 00 00  	move	$3, $zero
  1a1304: d2 ff 00 10  	b	0x1a1250 <.text+0xa1250>
  1a1308: cc 5a 62 ae  	sw	$2, 0x5acc($19)
  1a130c: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a1310: 42 00 02 3c  	lui	$2, 0x42
  1a1314: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a1318: 2d 48 e0 00  	move	$9, $7
  1a131c: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a1320: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a1324: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a1328: 05 00 40 10  	beqz	$2, 0x1a1340 <.text+0xa1340>
  1a132c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1330: 42 00 11 3c  	lui	$17, 0x42
  1a1334: cc 5a 22 8e  	lw	$2, 0x5acc($17)
  1a1338: 07 00 40 10  	beqz	$2, 0x1a1358 <.text+0xa1358>
  1a133c: 2d 18 40 00  	move	$3, $2
  1a1340: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a1344: 2d 10 60 00  	move	$2, $3
  1a1348: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a134c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a1350: 08 00 e0 03  	jr	$ra
  1a1354: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a1358: 44 00 02 3c  	lui	$2, 0x44
  1a135c: 44 00 07 3c  	lui	$7, 0x44
  1a1360: 40 72 50 24  	addiu	$16, $2, 0x7240
  1a1364: d0 71 e3 24  	addiu	$3, $7, 0x71d0
  1a1368: 08 00 08 ae  	sw	$8, 0x8($16)
  1a136c: 2d 38 60 00  	move	$7, $3
  1a1370: 10 00 03 ae  	sw	$3, 0x10($16)
  1a1374: 40 72 44 ac  	sw	$4, 0x7240($2)
  1a1378: 14 00 04 26  	addiu	$4, $16, 0x14
  1a137c: 04 00 05 ae  	sw	$5, 0x4($16)
  1a1380: 2d 28 c0 00  	move	$5, $6
  1a1384: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  1a1388: 07 00 22 69  	ldl	$2, 0x7($9)
  1a138c: 00 00 22 6d  	ldr	$2, 0x0($9)
  1a1390: 0f 00 23 69  	ldl	$3, 0xf($9)
  1a1394: 08 00 23 6d  	ldr	$3, 0x8($9)
  1a1398: 17 00 28 69  	ldl	$8, 0x17($9)
  1a139c: 10 00 28 6d  	ldr	$8, 0x10($9)
  1a13a0: 1f 00 2a 69  	ldl	$10, 0x1f($9)
  1a13a4: 18 00 2a 6d  	ldr	$10, 0x18($9)
  1a13a8: 07 00 e2 b0  	sdl	$2, 0x7($7)
  1a13ac: 00 00 e2 b4  	sdr	$2, 0x0($7)
  1a13b0: 0f 00 e3 b0  	sdl	$3, 0xf($7)
  1a13b4: 08 00 e3 b4  	sdr	$3, 0x8($7)
  1a13b8: 17 00 e8 b0  	sdl	$8, 0x17($7)
  1a13bc: 10 00 e8 b4  	sdr	$8, 0x10($7)
  1a13c0: 1f 00 ea b0  	sdl	$10, 0x1f($7)
  1a13c4: 18 00 ea b4  	sdr	$10, 0x18($7)
  1a13c8: 27 00 22 69  	ldl	$2, 0x27($9)
  1a13cc: 20 00 22 6d  	ldr	$2, 0x20($9)
  1a13d0: 2f 00 23 69  	ldl	$3, 0x2f($9)
  1a13d4: 28 00 23 6d  	ldr	$3, 0x28($9)
  1a13d8: 37 00 28 69  	ldl	$8, 0x37($9)
  1a13dc: 30 00 28 6d  	ldr	$8, 0x30($9)
  1a13e0: 3f 00 2a 69  	ldl	$10, 0x3f($9)
  1a13e4: 38 00 2a 6d  	ldr	$10, 0x38($9)
  1a13e8: 27 00 e2 b0  	sdl	$2, 0x27($7)
  1a13ec: 20 00 e2 b4  	sdr	$2, 0x20($7)
  1a13f0: 2f 00 e3 b0  	sdl	$3, 0x2f($7)
  1a13f4: 28 00 e3 b4  	sdr	$3, 0x28($7)
  1a13f8: 37 00 e8 b0  	sdl	$8, 0x37($7)
  1a13fc: 30 00 e8 b4  	sdr	$8, 0x30($7)
  1a1400: 3f 00 ea b0  	sdl	$10, 0x3f($7)
  1a1404: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a1408: 38 00 ea b4  	sdr	$10, 0x38($7)
  1a140c: 13 04 00 a2  	sb	$zero, 0x413($16)
  1a1410: ac 73 06 0c  	jal	0x19ceb0 <.text+0x9ceb0>
  1a1414: 2d 20 00 00  	move	$4, $zero
  1a1418: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a141c: 42 00 02 3c  	lui	$2, 0x42
  1a1420: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a1424: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a1428: 44 00 09 3c  	lui	$9, 0x44
  1a142c: 1c 00 02 3c  	lui	$2, 0x1c
  1a1430: 2d 58 00 00  	move	$11, $zero
  1a1434: 18 28 64 00  	<unknown>
  1a1438: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a143c: 44 00 04 3c  	lui	$4, 0x44
  1a1440: 2d 38 00 02  	move	$7, $16
  1a1444: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a1448: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a144c: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a1450: 14 04 08 24  	addiu	$8, $zero, 0x414
  1a1454: 21 18 a2 00  	addu	$3, $5, $2
  1a1458: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a145c: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a1460: 28 00 65 8c  	lw	$5, 0x28($3)
  1a1464: b6 ff 40 14  	bnez	$2, 0x1a1340 <.text+0xa1340>
  1a1468: 2d 18 40 00  	move	$3, $2
  1a146c: 0e 00 02 24  	addiu	$2, $zero, 0xe
  1a1470: 2d 18 00 00  	move	$3, $zero
  1a1474: b2 ff 00 10  	b	0x1a1340 <.text+0xa1340>
  1a1478: cc 5a 22 ae  	sw	$2, 0x5acc($17)
  1a147c: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a1480: 42 00 02 3c  	lui	$2, 0x42
  1a1484: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a1488: 2d 40 a0 00  	move	$8, $5
  1a148c: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a1490: 2d 38 80 00  	move	$7, $4
  1a1494: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a1498: 2d 28 c0 00  	move	$5, $6
  1a149c: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a14a0: 05 00 40 10  	beqz	$2, 0x1a14b8 <.text+0xa14b8>
  1a14a4: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a14a8: 42 00 11 3c  	lui	$17, 0x42
  1a14ac: cc 5a 22 8e  	lw	$2, 0x5acc($17)
  1a14b0: 07 00 40 10  	beqz	$2, 0x1a14d0 <.text+0xa14d0>
  1a14b4: 2d 18 40 00  	move	$3, $2
  1a14b8: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a14bc: 2d 10 60 00  	move	$2, $3
  1a14c0: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a14c4: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a14c8: 08 00 e0 03  	jr	$ra
  1a14cc: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a14d0: 44 00 02 3c  	lui	$2, 0x44
  1a14d4: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  1a14d8: 40 72 50 24  	addiu	$16, $2, 0x7240
  1a14dc: 40 72 47 ac  	sw	$7, 0x7240($2)
  1a14e0: 14 00 04 26  	addiu	$4, $16, 0x14
  1a14e4: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a14e8: 04 00 08 ae  	sw	$8, 0x4($16)
  1a14ec: 13 04 00 a2  	sb	$zero, 0x413($16)
  1a14f0: 42 00 02 3c  	lui	$2, 0x42
  1a14f4: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a14f8: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a14fc: 44 00 09 3c  	lui	$9, 0x44
  1a1500: 1c 00 02 3c  	lui	$2, 0x1c
  1a1504: 2d 58 00 00  	move	$11, $zero
  1a1508: 18 28 64 00  	<unknown>
  1a150c: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a1510: 44 00 04 3c  	lui	$4, 0x44
  1a1514: 2d 38 00 02  	move	$7, $16
  1a1518: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a151c: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a1520: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a1524: 14 04 08 24  	addiu	$8, $zero, 0x414
  1a1528: 21 18 a2 00  	addu	$3, $5, $2
  1a152c: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a1530: 2c 00 65 8c  	lw	$5, 0x2c($3)
  1a1534: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a1538: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a153c: de ff 40 14  	bnez	$2, 0x1a14b8 <.text+0xa14b8>
  1a1540: 2d 18 40 00  	move	$3, $2
  1a1544: 0f 00 02 24  	addiu	$2, $zero, 0xf
  1a1548: 2d 18 00 00  	move	$3, $zero
  1a154c: da ff 00 10  	b	0x1a14b8 <.text+0xa14b8>
  1a1550: cc 5a 22 ae  	sw	$2, 0x5acc($17)
  1a1554: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a1558: 42 00 02 3c  	lui	$2, 0x42
  1a155c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a1560: 2d 70 80 00  	move	$14, $4
  1a1564: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a1568: 2d 68 a0 00  	move	$13, $5
  1a156c: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a1570: 05 00 40 10  	beqz	$2, 0x1a1588 <.text+0xa1588>
  1a1574: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1578: 42 00 10 3c  	lui	$16, 0x42
  1a157c: cc 5a 02 8e  	lw	$2, 0x5acc($16)
  1a1580: 06 00 40 10  	beqz	$2, 0x1a159c <.text+0xa159c>
  1a1584: 2d 18 40 00  	move	$3, $2
  1a1588: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a158c: 2d 10 60 00  	move	$2, $3
  1a1590: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a1594: 08 00 e0 03  	jr	$ra
  1a1598: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a159c: 42 00 02 3c  	lui	$2, 0x42
  1a15a0: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a15a4: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a15a8: 44 00 0c 3c  	lui	$12, 0x44
  1a15ac: 1c 00 02 3c  	lui	$2, 0x1c
  1a15b0: 44 00 09 3c  	lui	$9, 0x44
  1a15b4: 18 28 64 00  	<unknown>
  1a15b8: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a15bc: 44 00 04 3c  	lui	$4, 0x44
  1a15c0: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a15c4: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a15c8: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a15cc: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a15d0: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a15d4: 21 18 a2 00  	addu	$3, $5, $2
  1a15d8: 2d 58 00 00  	move	$11, $zero
  1a15dc: 40 72 82 25  	addiu	$2, $12, 0x7240
  1a15e0: 30 00 65 8c  	lw	$5, 0x30($3)
  1a15e4: 04 00 4d ac  	sw	$13, 0x4($2)
  1a15e8: 2d 38 40 00  	move	$7, $2
  1a15ec: 40 72 8e ad  	sw	$14, 0x7240($12)
  1a15f0: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a15f4: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a15f8: e3 ff 40 14  	bnez	$2, 0x1a1588 <.text+0xa1588>
  1a15fc: 2d 18 40 00  	move	$3, $2
  1a1600: 10 00 02 24  	addiu	$2, $zero, 0x10
  1a1604: 2d 18 00 00  	move	$3, $zero
  1a1608: df ff 00 10  	b	0x1a1588 <.text+0xa1588>
  1a160c: cc 5a 02 ae  	sw	$2, 0x5acc($16)
  1a1610: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a1614: 42 00 02 3c  	lui	$2, 0x42
  1a1618: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a161c: 2d 70 80 00  	move	$14, $4
  1a1620: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a1624: 2d 68 a0 00  	move	$13, $5
  1a1628: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a162c: 05 00 40 10  	beqz	$2, 0x1a1644 <.text+0xa1644>
  1a1630: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1634: 42 00 10 3c  	lui	$16, 0x42
  1a1638: cc 5a 02 8e  	lw	$2, 0x5acc($16)
  1a163c: 06 00 40 10  	beqz	$2, 0x1a1658 <.text+0xa1658>
  1a1640: 2d 18 40 00  	move	$3, $2
  1a1644: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a1648: 2d 10 60 00  	move	$2, $3
  1a164c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a1650: 08 00 e0 03  	jr	$ra
  1a1654: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a1658: 42 00 02 3c  	lui	$2, 0x42
  1a165c: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a1660: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a1664: 44 00 0c 3c  	lui	$12, 0x44
  1a1668: 1c 00 02 3c  	lui	$2, 0x1c
  1a166c: 44 00 09 3c  	lui	$9, 0x44
  1a1670: 18 28 64 00  	<unknown>
  1a1674: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a1678: 44 00 04 3c  	lui	$4, 0x44
  1a167c: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a1680: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a1684: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a1688: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a168c: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a1690: 21 18 a2 00  	addu	$3, $5, $2
  1a1694: 2d 58 00 00  	move	$11, $zero
  1a1698: 40 72 82 25  	addiu	$2, $12, 0x7240
  1a169c: 34 00 65 8c  	lw	$5, 0x34($3)
  1a16a0: 04 00 4d ac  	sw	$13, 0x4($2)
  1a16a4: 2d 38 40 00  	move	$7, $2
  1a16a8: 40 72 8e ad  	sw	$14, 0x7240($12)
  1a16ac: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a16b0: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a16b4: e3 ff 40 14  	bnez	$2, 0x1a1644 <.text+0xa1644>
  1a16b8: 2d 18 40 00  	move	$3, $2
  1a16bc: 11 00 02 24  	addiu	$2, $zero, 0x11
  1a16c0: 2d 18 00 00  	move	$3, $zero
  1a16c4: df ff 00 10  	b	0x1a1644 <.text+0xa1644>
  1a16c8: cc 5a 02 ae  	sw	$2, 0x5acc($16)
  1a16cc: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a16d0: 42 00 02 3c  	lui	$2, 0x42
  1a16d4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a16d8: 2d 40 a0 00  	move	$8, $5
  1a16dc: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a16e0: 2d 38 80 00  	move	$7, $4
  1a16e4: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a16e8: 2d 28 c0 00  	move	$5, $6
  1a16ec: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a16f0: 05 00 40 10  	beqz	$2, 0x1a1708 <.text+0xa1708>
  1a16f4: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a16f8: 42 00 11 3c  	lui	$17, 0x42
  1a16fc: cc 5a 22 8e  	lw	$2, 0x5acc($17)
  1a1700: 07 00 40 10  	beqz	$2, 0x1a1720 <.text+0xa1720>
  1a1704: 2d 18 40 00  	move	$3, $2
  1a1708: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a170c: 2d 10 60 00  	move	$2, $3
  1a1710: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a1714: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a1718: 08 00 e0 03  	jr	$ra
  1a171c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a1720: 44 00 02 3c  	lui	$2, 0x44
  1a1724: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  1a1728: 40 72 50 24  	addiu	$16, $2, 0x7240
  1a172c: 40 72 47 ac  	sw	$7, 0x7240($2)
  1a1730: 14 00 04 26  	addiu	$4, $16, 0x14
  1a1734: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a1738: 04 00 08 ae  	sw	$8, 0x4($16)
  1a173c: 13 04 00 a2  	sb	$zero, 0x413($16)
  1a1740: 42 00 02 3c  	lui	$2, 0x42
  1a1744: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a1748: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a174c: 44 00 09 3c  	lui	$9, 0x44
  1a1750: 1c 00 02 3c  	lui	$2, 0x1c
  1a1754: 2d 58 00 00  	move	$11, $zero
  1a1758: 18 28 64 00  	<unknown>
  1a175c: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a1760: 44 00 04 3c  	lui	$4, 0x44
  1a1764: 2d 38 00 02  	move	$7, $16
  1a1768: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a176c: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a1770: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a1774: 14 04 08 24  	addiu	$8, $zero, 0x414
  1a1778: 21 18 a2 00  	addu	$3, $5, $2
  1a177c: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a1780: 48 00 65 8c  	lw	$5, 0x48($3)
  1a1784: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a1788: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a178c: de ff 40 14  	bnez	$2, 0x1a1708 <.text+0xa1708>
  1a1790: 2d 18 40 00  	move	$3, $2
  1a1794: 12 00 02 24  	addiu	$2, $zero, 0x12
  1a1798: 2d 18 00 00  	move	$3, $zero
  1a179c: da ff 00 10  	b	0x1a1708 <.text+0xa1708>
  1a17a0: cc 5a 22 ae  	sw	$2, 0x5acc($17)
  1a17a4: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  1a17a8: 42 00 02 3c  	lui	$2, 0x42
  1a17ac: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a17b0: 2d 40 a0 00  	move	$8, $5
  1a17b4: 50 00 bf ff  	sd	$ra, 0x50($sp)
  1a17b8: 2d 48 80 00  	move	$9, $4
  1a17bc: 40 00 b3 ff  	sd	$19, 0x40($sp)
  1a17c0: 2d 28 c0 00  	move	$5, $6
  1a17c4: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a17c8: 2d 90 e0 00  	move	$18, $7
  1a17cc: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a17d0: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a17d4: 05 00 40 10  	beqz	$2, 0x1a17ec <.text+0xa17ec>
  1a17d8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a17dc: 42 00 13 3c  	lui	$19, 0x42
  1a17e0: cc 5a 62 8e  	lw	$2, 0x5acc($19)
  1a17e4: 09 00 40 10  	beqz	$2, 0x1a180c <.text+0xa180c>
  1a17e8: 2d 18 40 00  	move	$3, $2
  1a17ec: 50 00 bf df  	ld	$ra, 0x50($sp)
  1a17f0: 2d 10 60 00  	move	$2, $3
  1a17f4: 40 00 b3 df  	ld	$19, 0x40($sp)
  1a17f8: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a17fc: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a1800: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a1804: 08 00 e0 03  	jr	$ra
  1a1808: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  1a180c: 44 00 02 3c  	lui	$2, 0x44
  1a1810: 44 00 10 3c  	lui	$16, 0x44
  1a1814: 40 72 51 24  	addiu	$17, $2, 0x7240
  1a1818: 40 72 49 ac  	sw	$9, 0x7240($2)
  1a181c: d0 71 10 26  	addiu	$16, $16, 0x71d0
  1a1820: 14 00 24 26  	addiu	$4, $17, 0x14
  1a1824: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  1a1828: 10 00 02 24  	addiu	$2, $zero, 0x10
  1a182c: 04 00 28 ae  	sw	$8, 0x4($17)
  1a1830: 08 00 22 ae  	sw	$2, 0x8($17)
  1a1834: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a1838: 10 00 30 ae  	sw	$16, 0x10($17)
  1a183c: 2d 28 40 02  	move	$5, $18
  1a1840: 20 00 06 24  	addiu	$6, $zero, 0x20
  1a1844: 20 00 04 26  	addiu	$4, $16, 0x20
  1a1848: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a184c: 13 04 20 a2  	sb	$zero, 0x413($17)
  1a1850: 3f 00 00 a2  	sb	$zero, 0x3f($16)
  1a1854: ac 73 06 0c  	jal	0x19ceb0 <.text+0x9ceb0>
  1a1858: 2d 20 00 00  	move	$4, $zero
  1a185c: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a1860: 42 00 02 3c  	lui	$2, 0x42
  1a1864: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a1868: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a186c: 44 00 09 3c  	lui	$9, 0x44
  1a1870: 1c 00 02 3c  	lui	$2, 0x1c
  1a1874: 2d 58 00 00  	move	$11, $zero
  1a1878: 18 28 64 00  	<unknown>
  1a187c: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a1880: 44 00 04 3c  	lui	$4, 0x44
  1a1884: 2d 38 20 02  	move	$7, $17
  1a1888: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a188c: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a1890: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a1894: 14 04 08 24  	addiu	$8, $zero, 0x414
  1a1898: 21 18 a2 00  	addu	$3, $5, $2
  1a189c: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a18a0: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a18a4: 38 00 65 8c  	lw	$5, 0x38($3)
  1a18a8: d0 ff 40 14  	bnez	$2, 0x1a17ec <.text+0xa17ec>
  1a18ac: 2d 18 40 00  	move	$3, $2
  1a18b0: 13 00 02 24  	addiu	$2, $zero, 0x13
  1a18b4: 2d 18 00 00  	move	$3, $zero
  1a18b8: cc ff 00 10  	b	0x1a17ec <.text+0xa17ec>
  1a18bc: cc 5a 62 ae  	sw	$2, 0x5acc($19)
  1a18c0: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a18c4: 42 00 02 3c  	lui	$2, 0x42
  1a18c8: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a18cc: 2d 68 80 00  	move	$13, $4
  1a18d0: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a18d4: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a18d8: 05 00 40 10  	beqz	$2, 0x1a18f0 <.text+0xa18f0>
  1a18dc: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a18e0: 42 00 10 3c  	lui	$16, 0x42
  1a18e4: cc 5a 02 8e  	lw	$2, 0x5acc($16)
  1a18e8: 06 00 40 10  	beqz	$2, 0x1a1904 <.text+0xa1904>
  1a18ec: 2d 18 40 00  	move	$3, $2
  1a18f0: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a18f4: 2d 10 60 00  	move	$2, $3
  1a18f8: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a18fc: 08 00 e0 03  	jr	$ra
  1a1900: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a1904: 42 00 02 3c  	lui	$2, 0x42
  1a1908: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a190c: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a1910: 44 00 0c 3c  	lui	$12, 0x44
  1a1914: 1c 00 02 3c  	lui	$2, 0x1c
  1a1918: 54 72 87 25  	addiu	$7, $12, 0x7254
  1a191c: 18 28 64 00  	<unknown>
  1a1920: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a1924: 44 00 04 3c  	lui	$4, 0x44
  1a1928: 44 00 09 3c  	lui	$9, 0x44
  1a192c: ec ff e7 24  	addiu	$7, $7, -0x14 <.text+0xffffffffffefffec>
  1a1930: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a1934: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a1938: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a193c: 21 18 a2 00  	addu	$3, $5, $2
  1a1940: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a1944: 50 00 65 8c  	lw	$5, 0x50($3)
  1a1948: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a194c: 2d 58 00 00  	move	$11, $zero
  1a1950: 54 72 8d ad  	sw	$13, 0x7254($12)
  1a1954: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a1958: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a195c: e4 ff 40 14  	bnez	$2, 0x1a18f0 <.text+0xa18f0>
  1a1960: 2d 18 40 00  	move	$3, $2
  1a1964: 14 00 02 24  	addiu	$2, $zero, 0x14
  1a1968: 2d 18 00 00  	move	$3, $zero
  1a196c: e0 ff 00 10  	b	0x1a18f0 <.text+0xa18f0>
  1a1970: cc 5a 02 ae  	sw	$2, 0x5acc($16)
  1a1974: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a1978: 42 00 02 3c  	lui	$2, 0x42
  1a197c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a1980: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1984: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a1988: 2d 90 c0 00  	move	$18, $6
  1a198c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1990: 2d 88 a0 00  	move	$17, $5
  1a1994: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a1998: cc 5a 42 8c  	lw	$2, 0x5acc($2)
  1a199c: 08 00 40 14  	bnez	$2, 0x1a19c0 <.text+0xa19c0>
  1a19a0: 2d 80 80 00  	move	$16, $4
  1a19a4: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a19a8: 2d 10 60 00  	move	$2, $3
  1a19ac: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a19b0: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a19b4: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a19b8: 08 00 e0 03  	jr	$ra
  1a19bc: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a19c0: 44 00 04 3c  	lui	$4, 0x44
  1a19c4: bd 86 06 0c  	jal	0x1a1af4 <.text+0xa1af4>
  1a19c8: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a19cc: 12 00 00 16  	bnez	$16, 0x1a1a18 <.text+0xa1a18>
  1a19d0: 2d 20 40 00  	move	$4, $2
  1a19d4: 44 00 04 3c  	lui	$4, 0x44
  1a19d8: bd 86 06 0c  	jal	0x1a1af4 <.text+0xa1af4>
  1a19dc: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a19e0: 0c 00 40 10  	beqz	$2, 0x1a1a14 <.text+0xa1a14>
  1a19e4: 2d 18 00 00  	move	$3, $zero
  1a19e8: 01 00 02 3c  	lui	$2, 0x1
  1a19ec: 01 00 63 24  	addiu	$3, $3, 0x1
  1a19f0: 9f 86 42 34  	ori	$2, $2, 0x869f
  1a19f4: 2a 10 43 00  	slt	$2, $2, $3
		...
  1a1a04: f9 ff 40 10  	beqz	$2, 0x1a19ec <.text+0xa19ec>
  1a1a08: 01 00 02 3c  	lui	$2, 0x1
  1a1a0c: f2 ff 00 10  	b	0x1a19d8 <.text+0xa19d8>
  1a1a10: 44 00 04 3c  	lui	$4, 0x44
  1a1a14: 2d 20 00 00  	move	$4, $zero
  1a1a18: 03 00 20 12  	beqz	$17, 0x1a1a28 <.text+0xa1a28>
  1a1a1c: 42 00 02 3c  	lui	$2, 0x42
  1a1a20: cc 5a 42 8c  	lw	$2, 0x5acc($2)
  1a1a24: 00 00 22 ae  	sw	$2, 0x0($17)
  1a1a28: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a1a2c: dd ff 82 10  	beq	$4, $2, 0x1a19a4 <.text+0xa19a4>
  1a1a30: 2d 18 00 00  	move	$3, $zero
  1a1a34: 42 00 02 3c  	lui	$2, 0x42
  1a1a38: 04 00 40 12  	beqz	$18, 0x1a1a4c <.text+0xa1a4c>
  1a1a3c: cc 5a 40 ac  	sw	$zero, 0x5acc($2)
  1a1a40: 44 00 02 3c  	lui	$2, 0x44
  1a1a44: 00 65 42 8c  	lw	$2, 0x6500($2)
  1a1a48: 00 00 42 ae  	sw	$2, 0x0($18)
  1a1a4c: d5 ff 00 10  	b	0x1a19a4 <.text+0xa19a4>
  1a1a50: 01 00 03 24  	addiu	$3, $zero, 0x1
  1a1a54: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a1a58: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a1a5c: 2d 88 80 00  	move	$17, $4
  1a1a60: 2d 20 a0 00  	move	$4, $5
  1a1a64: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1a68: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a1a6c: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  1a1a70: 2d 80 a0 00  	move	$16, $5
  1a1a74: 2d 38 00 00  	move	$7, $zero
  1a1a78: 15 00 40 18  	blez	$2, 0x1a1ad0 <.text+0xa1ad0>
  1a1a7c: 2d 30 40 00  	move	$6, $2
  1a1a80: 2d 28 20 02  	move	$5, $17
  1a1a84: 21 10 07 02  	addu	$2, $16, $7
  1a1a88: 00 00 44 80  	lb	$4, 0x0($2)
  1a1a8c: 1f 00 82 24  	addiu	$2, $4, 0x1f
  1a1a90: 20 00 83 24  	addiu	$3, $4, 0x20
  1a1a94: 00 12 02 00  	sll	$2, $2, 0x8
  1a1a98: 00 1a 03 00  	sll	$3, $3, 0x8
  1a1a9c: 82 00 42 34  	ori	$2, $2, 0x82
  1a1aa0: 82 00 63 34  	ori	$3, $3, 0x82
  1a1aa4: 00 14 02 00  	sll	$2, $2, 0x10
  1a1aa8: 61 00 84 28  	slti	$4, $4, 0x61
  1a1aac: 00 1c 03 00  	sll	$3, $3, 0x10
  1a1ab0: 02 00 80 14  	bnez	$4, 0x1a1abc <.text+0xa1abc>
  1a1ab4: 03 14 02 00  	sra	$2, $2, 0x10
  1a1ab8: 03 14 03 00  	sra	$2, $3, 0x10
  1a1abc: 01 00 e7 24  	addiu	$7, $7, 0x1
  1a1ac0: 00 00 a2 a4  	sh	$2, 0x0($5)
  1a1ac4: 2a 10 e6 00  	slt	$2, $7, $6
  1a1ac8: ee ff 40 14  	bnez	$2, 0x1a1a84 <.text+0xa1a84>
  1a1acc: 02 00 a5 24  	addiu	$5, $5, 0x2
  1a1ad0: 40 18 07 00  	sll	$3, $7, 0x1
  1a1ad4: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a1ad8: 21 18 71 00  	addu	$3, $3, $17
  1a1adc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a1ae0: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a1ae4: 2d 10 c0 00  	move	$2, $6
  1a1ae8: 02 00 60 a4  	sh	$zero, 0x2($3)
  1a1aec: 08 00 e0 03  	jr	$ra
  1a1af0: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a1af4: 00 00 85 8c  	lw	$5, 0x0($4)
  1a1af8: 05 00 a0 10  	beqz	$5, 0x1a1b10 <.text+0xa1b10>
  1a1afc: 2d 30 00 00  	move	$6, $zero
  1a1b00: 04 00 83 8c  	lw	$3, 0x4($4)
  1a1b04: 18 00 a2 8c  	lw	$2, 0x18($5)
  1a1b08: 03 00 62 50  	beql	$3, $2, 0x1a1b18 <.text+0xa1b18>
  1a1b0c: 10 00 a2 8c  	lw	$2, 0x10($5)
  1a1b10: 08 00 e0 03  	jr	$ra
  1a1b14: 2d 10 c0 00  	move	$2, $6
  1a1b18: fd ff 00 10  	b	0x1a1b10 <.text+0xa1b10>
  1a1b1c: 01 00 46 30  	andi	$6, $2, 0x1
  1a1b20: 3c 10 05 00  	dsll32	$2, $5, 0x0
  1a1b24: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1b28: 3f 30 04 00  	dsra32	$6, $4, 0x0
  1a1b2c: 18 30 c2 00  	<unknown>
  1a1b30: 3c 20 04 00  	dsll32	$4, $4, 0x0
  1a1b34: 3f 20 04 00  	dsra32	$4, $4, 0x0
  1a1b38: 19 00 82 00  	multu	$4, $2
  1a1b3c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1b40: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a1b44: 12 10 00 00  	mflo	$2
  1a1b48: 3f 28 05 00  	dsra32	$5, $5, 0x0
  1a1b4c: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1b50: 10 38 00 00  	mfhi	$7
  1a1b54: 24 40 03 01  	and	$8, $8, $3
  1a1b58: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1b5c: 18 18 85 00  	<unknown>
  1a1b60: 25 40 02 01  	or	$8, $8, $2
  1a1b64: ff ff 02 3c  	lui	$2, 0xffff
  1a1b68: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1b6c: 3c 38 07 00  	dsll32	$7, $7, 0x0
  1a1b70: 24 40 02 01  	and	$8, $8, $2
  1a1b74: 25 40 07 01  	or	$8, $8, $7
  1a1b78: 21 20 66 00  	addu	$4, $3, $6
  1a1b7c: 24 10 02 01  	and	$2, $8, $2
  1a1b80: 3f 18 08 00  	dsra32	$3, $8, 0x0
  1a1b84: 21 18 64 00  	addu	$3, $3, $4
  1a1b88: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a1b8c: 08 00 e0 03  	jr	$ra
  1a1b90: 25 10 43 00  	or	$2, $2, $3
  1a1b94: 00 00 00 00  	nop
  1a1b98: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1b9c: fa 1a 03 00  	dsrl	$3, $3, 0xb
  1a1ba0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1ba4: f8 12 02 00  	dsll	$2, $2, 0xb
  1a1ba8: ba 12 02 00  	dsrl	$2, $2, 0xa
  1a1bac: 2d 18 83 00  	daddu	$3, $4, $3
  1a1bb0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a1bb4: 2b 10 43 00  	sltu	$2, $2, $3
  1a1bb8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a1bbc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1bc0: 2d 90 80 00  	move	$18, $4
  1a1bc4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a1bc8: ff 07 84 30  	andi	$4, $4, 0x7ff
  1a1bcc: e0 81 10 34  	ori	$16, $zero, 0x81e0
  1a1bd0: fc 83 10 00  	dsll32	$16, $16, 0xf
  1a1bd4: 06 00 40 10  	beqz	$2, 0x1a1bf0 <.text+0xa1bf0>
  1a1bd8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a1bdc: 04 00 80 10  	beqz	$4, 0x1a1bf0 <.text+0xa1bf0>
  1a1be0: 00 f8 02 24  	addiu	$2, $zero, -0x800 <.text+0xffffffffffeff800>
  1a1be4: 00 08 03 24  	addiu	$3, $zero, 0x800
  1a1be8: 24 90 42 02  	and	$18, $18, $2
  1a1bec: 25 90 43 02  	or	$18, $18, $3
  1a1bf0: 3f 20 12 00  	dsra32	$4, $18, 0x0
  1a1bf4: cc 8e 06 0c  	jal	0x1a3b30 <.text+0xa3b30>
  1a1bf8: 00 00 00 00  	nop
  1a1bfc: 2d 28 00 02  	move	$5, $16
  1a1c00: 2d 20 40 00  	move	$4, $2
  1a1c04: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a1c08: 00 00 00 00  	nop
  1a1c0c: 2d 28 00 02  	move	$5, $16
  1a1c10: ff ff 10 3c  	lui	$16, 0xffff
  1a1c14: 3e 80 10 00  	dsrl32	$16, $16, 0x0
  1a1c18: 2d 20 40 00  	move	$4, $2
  1a1c1c: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a1c20: 24 80 50 02  	and	$16, $18, $16
  1a1c24: 3c 80 10 00  	dsll32	$16, $16, 0x0
  1a1c28: 3f 80 10 00  	dsra32	$16, $16, 0x0
  1a1c2c: 2d 88 40 00  	move	$17, $2
  1a1c30: cc 8e 06 0c  	jal	0x1a3b30 <.text+0xa3b30>
  1a1c34: 2d 20 00 02  	move	$4, $16
  1a1c38: 0f 00 00 06  	bltz	$16, 0x1a1c78 <.text+0xa1c78>
  1a1c3c: 00 00 00 00  	nop
  1a1c40: 2d 28 40 00  	move	$5, $2
  1a1c44: 2d 20 20 02  	move	$4, $17
  1a1c48: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a1c4c: 00 00 00 00  	nop
  1a1c50: 2d 20 40 00  	move	$4, $2
  1a1c54: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  1a1c58: 00 00 00 00  	nop
  1a1c5c: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a1c60: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a1c64: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a1c68: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a1c6c: 08 00 e0 03  	jr	$ra
  1a1c70: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a1c74: 00 00 00 00  	nop
  1a1c78: e0 83 05 34  	ori	$5, $zero, 0x83e0
  1a1c7c: fc 2b 05 00  	dsll32	$5, $5, 0xf
  1a1c80: 2d 20 40 00  	move	$4, $2
  1a1c84: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a1c88: 00 00 00 00  	nop
  1a1c8c: ec ff 00 10  	b	0x1a1c40 <.text+0xa1c40>
		...
  1a1c98: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a1c9c: 2d 28 00 00  	move	$5, $zero
  1a1ca0: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a1ca4: 2d 90 80 00  	move	$18, $4
  1a1ca8: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a1cac: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a1cb0: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a1cb4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1cb8: 26 00 40 04  	bltz	$2, 0x1a1d54 <.text+0xa1d54>
  1a1cbc: 2d 20 00 00  	move	$4, $zero
  1a1cc0: c0 f7 05 34  	ori	$5, $zero, 0xf7c0
  1a1cc4: bc 2b 05 00  	dsll32	$5, $5, 0xe
  1a1cc8: 2d 20 40 02  	move	$4, $18
  1a1ccc: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a1cd0: 00 00 00 00  	nop
  1a1cd4: 2d 20 40 00  	move	$4, $2
  1a1cd8: 46 8f 06 0c  	jal	0x1a3d18 <.text+0xa3d18>
  1a1cdc: 00 00 00 00  	nop
  1a1ce0: 3c 88 02 00  	dsll32	$17, $2, 0x0
  1a1ce4: 7a 10 11 00  	dsrl	$2, $17, 0x1
  1a1ce8: 01 00 23 32  	andi	$3, $17, 0x1
  1a1cec: 25 18 62 00  	or	$3, $3, $2
  1a1cf0: 29 00 20 06  	bltz	$17, 0x1a1d98 <.text+0xa1d98>
  1a1cf4: 2d 20 20 02  	move	$4, $17
  1a1cf8: 60 9d 06 0c  	jal	0x1a7580 <.text+0xa7580>
  1a1cfc: 00 00 00 00  	nop
  1a1d00: 2d 20 40 02  	move	$4, $18
  1a1d04: 2d 28 40 00  	move	$5, $2
  1a1d08: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a1d0c: 00 00 00 00  	nop
  1a1d10: 2d 80 00 00  	move	$16, $zero
  1a1d14: 2d 28 00 02  	move	$5, $16
  1a1d18: 2d 20 40 00  	move	$4, $2
  1a1d1c: 2d 90 40 00  	move	$18, $2
  1a1d20: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a1d24: 00 00 00 00  	nop
  1a1d28: 2d 28 40 02  	move	$5, $18
  1a1d2c: 2d 20 00 02  	move	$4, $16
  1a1d30: 0f 00 40 04  	bltz	$2, 0x1a1d70 <.text+0xa1d70>
  1a1d34: 00 00 00 00  	nop
  1a1d38: 2d 20 40 02  	move	$4, $18
  1a1d3c: 46 8f 06 0c  	jal	0x1a3d18 <.text+0xa3d18>
  1a1d40: 00 00 00 00  	nop
  1a1d44: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1d48: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1d4c: 2d 88 22 02  	daddu	$17, $17, $2
  1a1d50: 2d 20 20 02  	move	$4, $17
  1a1d54: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a1d58: 2d 10 80 00  	move	$2, $4
  1a1d5c: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a1d60: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a1d64: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a1d68: 08 00 e0 03  	jr	$ra
  1a1d6c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a1d70: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a1d74: 00 00 00 00  	nop
  1a1d78: 2d 20 40 00  	move	$4, $2
  1a1d7c: 46 8f 06 0c  	jal	0x1a3d18 <.text+0xa3d18>
  1a1d80: 00 00 00 00  	nop
  1a1d84: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1d88: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1d8c: f0 ff 00 10  	b	0x1a1d50 <.text+0xa1d50>
  1a1d90: 2f 88 22 02  	dsubu	$17, $17, $2
  1a1d94: 00 00 00 00  	nop
  1a1d98: 60 9d 06 0c  	jal	0x1a7580 <.text+0xa7580>
  1a1d9c: 2d 20 60 00  	move	$4, $3
  1a1da0: 2d 20 40 00  	move	$4, $2
  1a1da4: 2d 28 40 00  	move	$5, $2
  1a1da8: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a1dac: 00 00 00 00  	nop
  1a1db0: d3 ff 00 10  	b	0x1a1d00 <.text+0xa1d00>
  1a1db4: 00 00 00 00  	nop
  1a1db8: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1dbc: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1dc0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1dc4: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a1dc8: 24 40 02 01  	and	$8, $8, $2
  1a1dcc: 24 48 23 01  	and	$9, $9, $3
  1a1dd0: 3c 10 04 00  	dsll32	$2, $4, 0x0
  1a1dd4: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1dd8: 3f 38 04 00  	dsra32	$7, $4, 0x0
  1a1ddc: 23 10 02 00  	negu	$2, $2
  1a1de0: 2d 50 80 00  	move	$10, $4
  1a1de4: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1de8: 23 20 07 00  	negu	$4, $7
  1a1dec: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1df0: 3f 58 05 00  	dsra32	$11, $5, 0x0
  1a1df4: 25 40 02 01  	or	$8, $8, $2
  1a1df8: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a1dfc: 3c 10 05 00  	dsll32	$2, $5, 0x0
  1a1e00: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1e04: 3c 18 08 00  	dsll32	$3, $8, 0x0
  1a1e08: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1a1e0c: 23 10 02 00  	negu	$2, $2
  1a1e10: 2b 18 03 00  	sltu	$3, $zero, $3
  1a1e14: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1e18: 23 20 83 00  	subu	$4, $4, $3
  1a1e1c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1e20: 3c 60 04 00  	dsll32	$12, $4, 0x0
  1a1e24: 25 48 22 01  	or	$9, $9, $2
  1a1e28: 23 20 0b 00  	negu	$4, $11
  1a1e2c: 3c 10 09 00  	dsll32	$2, $9, 0x0
  1a1e30: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1e34: ff ff 03 3c  	lui	$3, 0xffff
  1a1e38: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a1e3c: 2b 10 02 00  	sltu	$2, $zero, $2
  1a1e40: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1e44: 23 20 82 00  	subu	$4, $4, $2
  1a1e48: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a1e4c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a1e50: 2d 30 00 00  	move	$6, $zero
  1a1e54: 24 40 03 01  	and	$8, $8, $3
  1a1e58: 24 48 23 01  	and	$9, $9, $3
  1a1e5c: 3c 10 04 00  	dsll32	$2, $4, 0x0
  1a1e60: 25 00 e0 04  	bltz	$7, 0x1a1ef8 <.text+0xa1ef8>
  1a1e64: 2d 80 00 00  	move	$16, $zero
  1a1e68: 1f 00 60 05  	bltz	$11, 0x1a1ee8 <.text+0xa1ee8>
  1a1e6c: 2d 20 40 01  	move	$4, $10
  1a1e70: c2 87 06 0c  	jal	0x1a1f08 <.text+0xa1f08>
  1a1e74: 00 00 00 00  	nop
  1a1e78: 15 00 00 12  	beqz	$16, 0x1a1ed0 <.text+0xa1ed0>
  1a1e7c: 2d 20 40 00  	move	$4, $2
  1a1e80: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1e84: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1e88: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1e8c: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a1e90: 23 10 02 00  	negu	$2, $2
  1a1e94: 24 88 23 02  	and	$17, $17, $3
  1a1e98: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1e9c: 3f 20 04 00  	dsra32	$4, $4, 0x0
  1a1ea0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1ea4: 23 20 04 00  	negu	$4, $4
  1a1ea8: 25 88 22 02  	or	$17, $17, $2
  1a1eac: ff ff 02 3c  	lui	$2, 0xffff
  1a1eb0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1eb4: 3c 18 11 00  	dsll32	$3, $17, 0x0
  1a1eb8: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1a1ebc: 24 88 22 02  	and	$17, $17, $2
  1a1ec0: 2b 18 03 00  	sltu	$3, $zero, $3
  1a1ec4: 23 20 83 00  	subu	$4, $4, $3
  1a1ec8: 3c 20 04 00  	dsll32	$4, $4, 0x0
  1a1ecc: 25 20 24 02  	or	$4, $17, $4
  1a1ed0: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a1ed4: 2d 10 80 00  	move	$2, $4
  1a1ed8: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a1edc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a1ee0: 08 00 e0 03  	jr	$ra
  1a1ee4: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a1ee8: 25 28 22 01  	or	$5, $9, $2
  1a1eec: e0 ff 00 10  	b	0x1a1e70 <.text+0xa1e70>
  1a1ef0: 27 80 10 00  	nor	$16, $zero, $16
  1a1ef4: 00 00 00 00  	nop
  1a1ef8: 25 50 0c 01  	or	$10, $8, $12
  1a1efc: da ff 00 10  	b	0x1a1e68 <.text+0xa1e68>
  1a1f00: ff ff 10 24  	addiu	$16, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1f04: 00 00 00 00  	nop
  1a1f08: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a1f0c: 3f 38 05 00  	dsra32	$7, $5, 0x0
  1a1f10: 3f 40 04 00  	dsra32	$8, $4, 0x0
  1a1f14: 3c 48 05 00  	dsll32	$9, $5, 0x0
  1a1f18: 3f 48 09 00  	dsra32	$9, $9, 0x0
  1a1f1c: 3c 68 04 00  	dsll32	$13, $4, 0x0
  1a1f20: 3f 68 0d 00  	dsra32	$13, $13, 0x0
  1a1f24: 08 01 e0 14  	bnez	$7, 0x1a2348 <.text+0xa2348>
  1a1f28: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1f2c: 2b 10 09 01  	sltu	$2, $8, $9
  1a1f30: 71 00 40 10  	beqz	$2, 0x1a20f8 <.text+0xa20f8>
  1a1f34: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a1f38: 2b 10 49 00  	sltu	$2, $2, $9
  1a1f3c: 66 00 40 14  	bnez	$2, 0x1a20d8 <.text+0xa20d8>
  1a1f40: ff 00 02 3c  	lui	$2, 0xff
  1a1f44: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a1f48: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a1f4c: 2d 28 40 00  	move	$5, $2
  1a1f50: 0b 28 03 00  	movn	$5, $zero, $3
  1a1f54: 1c 00 03 3c  	lui	$3, 0x1c
  1a1f58: 06 10 a9 00  	srlv	$2, $9, $5
  1a1f5c: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a1f60: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a1f64: 21 10 43 00  	addu	$2, $2, $3
  1a1f68: 00 00 44 90  	lbu	$4, 0x0($2)
  1a1f6c: 21 20 85 00  	addu	$4, $4, $5
  1a1f70: 23 78 e4 00  	subu	$15, $7, $4
  1a1f74: 08 00 e0 51  	beqzl	$15, 0x1a1f98 <.text+0xa1f98>
  1a1f78: 02 3c 09 00  	srl	$7, $9, 0x10
  1a1f7c: 23 10 ef 00  	subu	$2, $7, $15
  1a1f80: 04 18 e8 01  	sllv	$3, $8, $15
  1a1f84: 06 10 4d 00  	srlv	$2, $13, $2
  1a1f88: 04 48 e9 01  	sllv	$9, $9, $15
  1a1f8c: 25 40 62 00  	or	$8, $3, $2
  1a1f90: 04 68 ed 01  	sllv	$13, $13, $15
  1a1f94: 02 3c 09 00  	srl	$7, $9, 0x10
  1a1f98: ff ff 2a 31  	andi	$10, $9, 0xffff
  1a1f9c: 1b 00 07 01  	divu	$zero, $8, $7
  1a1fa0: 02 24 0d 00  	srl	$4, $13, 0x10
  1a1fa4: 01 00 e0 50  	beqzl	$7, 0x1a1fac <.text+0xa1fac>
  1a1fa8: cd 01 00 00  	break	0x0, 0x7
  1a1fac: 12 10 00 00  	mflo	$2
  1a1fb0: 10 18 00 00  	mfhi	$3
  1a1fb4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a1fb8: 25 18 64 00  	or	$3, $3, $4
  1a1fbc: 12 28 00 00  	mflo	$5
  1a1fc0: 18 40 4a 00  	<unknown>
  1a1fc4: 2b 10 68 00  	sltu	$2, $3, $8
  1a1fc8: 0c 00 40 50  	beqzl	$2, 0x1a1ffc <.text+0xa1ffc>
  1a1fcc: 23 18 68 00  	subu	$3, $3, $8
  1a1fd0: 21 18 69 00  	addu	$3, $3, $9
  1a1fd4: 2b 10 69 00  	sltu	$2, $3, $9
  1a1fd8: 07 00 40 14  	bnez	$2, 0x1a1ff8 <.text+0xa1ff8>
  1a1fdc: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a1fe0: 2b 10 68 00  	sltu	$2, $3, $8
  1a1fe4: 05 00 40 50  	beqzl	$2, 0x1a1ffc <.text+0xa1ffc>
  1a1fe8: 23 18 68 00  	subu	$3, $3, $8
  1a1fec: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a1ff0: 21 18 69 00  	addu	$3, $3, $9
  1a1ff4: 00 00 00 00  	nop
  1a1ff8: 23 18 68 00  	subu	$3, $3, $8
  1a1ffc: 01 00 e0 50  	beqzl	$7, 0x1a2004 <.text+0xa2004>
  1a2000: cd 01 00 00  	break	0x0, 0x7
  1a2004: 1b 00 67 00  	divu	$zero, $3, $7
  1a2008: ff ff a4 31  	andi	$4, $13, 0xffff
  1a200c: 12 10 00 00  	mflo	$2
  1a2010: 10 18 00 00  	mfhi	$3
  1a2014: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2018: 25 18 64 00  	or	$3, $3, $4
  1a201c: 12 38 00 00  	mflo	$7
  1a2020: 18 40 4a 00  	<unknown>
  1a2024: 2b 10 68 00  	sltu	$2, $3, $8
  1a2028: 0c 00 40 10  	beqz	$2, 0x1a205c <.text+0xa205c>
  1a202c: 00 14 05 00  	sll	$2, $5, 0x10
  1a2030: 21 18 69 00  	addu	$3, $3, $9
  1a2034: 2b 10 69 00  	sltu	$2, $3, $9
  1a2038: 07 00 40 14  	bnez	$2, 0x1a2058 <.text+0xa2058>
  1a203c: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2040: 2b 10 68 00  	sltu	$2, $3, $8
  1a2044: 05 00 40 10  	beqz	$2, 0x1a205c <.text+0xa205c>
  1a2048: 00 14 05 00  	sll	$2, $5, 0x10
  1a204c: 21 18 69 00  	addu	$3, $3, $9
  1a2050: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2054: 00 00 00 00  	nop
  1a2058: 00 14 05 00  	sll	$2, $5, 0x10
  1a205c: 23 68 68 00  	subu	$13, $3, $8
  1a2060: 25 50 47 00  	or	$10, $2, $7
  1a2064: 2d 80 00 00  	move	$16, $zero
  1a2068: 0b 00 c0 10  	beqz	$6, 0x1a2098 <.text+0xa2098>
  1a206c: 06 10 ed 01  	srlv	$2, $13, $15
  1a2070: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2074: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2078: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a207c: 24 60 83 01  	and	$12, $12, $3
  1a2080: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2084: ff ff 03 3c  	lui	$3, 0xffff
  1a2088: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a208c: 25 60 82 01  	or	$12, $12, $2
  1a2090: 24 60 83 01  	and	$12, $12, $3
  1a2094: 00 00 cc fc  	sd	$12, 0x0($6)
  1a2098: 3c 10 0a 00  	dsll32	$2, $10, 0x0
  1a209c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a20a0: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a20a4: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a20a8: 24 70 c3 01  	and	$14, $14, $3
  1a20ac: 25 70 c2 01  	or	$14, $14, $2
  1a20b0: 3c 18 10 00  	dsll32	$3, $16, 0x0
  1a20b4: ff ff 02 3c  	lui	$2, 0xffff
  1a20b8: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a20bc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a20c0: 24 70 c2 01  	and	$14, $14, $2
  1a20c4: 25 70 c3 01  	or	$14, $14, $3
  1a20c8: 2d 10 c0 01  	move	$2, $14
  1a20cc: 08 00 e0 03  	jr	$ra
  1a20d0: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a20d4: 00 00 00 00  	nop
  1a20d8: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a20dc: ff ff 42 34  	ori	$2, $2, 0xffff
  1a20e0: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a20e4: 2b 10 49 00  	sltu	$2, $2, $9
  1a20e8: 2d 28 60 00  	move	$5, $3
  1a20ec: 99 ff 00 10  	b	0x1a1f54 <.text+0xa1f54>
  1a20f0: 0b 28 82 00  	movn	$5, $4, $2
  1a20f4: 00 00 00 00  	nop
  1a20f8: 08 00 20 15  	bnez	$9, 0x1a211c <.text+0xa211c>
  1a20fc: 2b 10 49 00  	sltu	$2, $2, $9
  1a2100: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a2104: 01 00 20 51  	beqzl	$9, 0x1a210c <.text+0xa210c>
  1a2108: cd 01 00 00  	break	0x0, 0x7
  1a210c: 1b 00 47 00  	divu	$zero, $2, $7
  1a2110: 12 48 00 00  	mflo	$9
  1a2114: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2118: 2b 10 49 00  	sltu	$2, $2, $9
  1a211c: 82 00 40 14  	bnez	$2, 0x1a2328 <.text+0xa2328>
  1a2120: ff 00 02 3c  	lui	$2, 0xff
  1a2124: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2128: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a212c: 2d 28 40 00  	move	$5, $2
  1a2130: 0b 28 03 00  	movn	$5, $zero, $3
  1a2134: 1c 00 03 3c  	lui	$3, 0x1c
  1a2138: 06 10 a9 00  	srlv	$2, $9, $5
  1a213c: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2140: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a2144: 21 10 43 00  	addu	$2, $2, $3
  1a2148: 00 00 44 90  	lbu	$4, 0x0($2)
  1a214c: 21 20 85 00  	addu	$4, $4, $5
  1a2150: 23 78 e4 00  	subu	$15, $7, $4
  1a2154: 38 00 e0 15  	bnez	$15, 0x1a2238 <.text+0xa2238>
  1a2158: 23 c0 ef 00  	subu	$24, $7, $15
  1a215c: 23 40 09 01  	subu	$8, $8, $9
  1a2160: 01 00 10 24  	addiu	$16, $zero, 0x1
  1a2164: 02 54 09 00  	srl	$10, $9, 0x10
  1a2168: ff ff 39 31  	andi	$25, $9, 0xffff
  1a216c: 1b 00 0a 01  	divu	$zero, $8, $10
  1a2170: 02 2c 0d 00  	srl	$5, $13, 0x10
  1a2174: 01 00 40 51  	beqzl	$10, 0x1a217c <.text+0xa217c>
  1a2178: cd 01 00 00  	break	0x0, 0x7
  1a217c: 12 20 00 00  	mflo	$4
  1a2180: 10 18 00 00  	mfhi	$3
  1a2184: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2188: 25 18 65 00  	or	$3, $3, $5
  1a218c: 12 58 00 00  	mflo	$11
  1a2190: 18 38 99 00  	<unknown>
  1a2194: 2b 10 67 00  	sltu	$2, $3, $7
  1a2198: 0c 00 40 50  	beqzl	$2, 0x1a21cc <.text+0xa21cc>
  1a219c: 23 18 67 00  	subu	$3, $3, $7
  1a21a0: 21 18 69 00  	addu	$3, $3, $9
  1a21a4: 2b 10 69 00  	sltu	$2, $3, $9
  1a21a8: 07 00 40 14  	bnez	$2, 0x1a21c8 <.text+0xa21c8>
  1a21ac: ff ff 8b 24  	addiu	$11, $4, -0x1 <.text+0xffffffffffefffff>
  1a21b0: 2b 10 67 00  	sltu	$2, $3, $7
  1a21b4: 05 00 40 50  	beqzl	$2, 0x1a21cc <.text+0xa21cc>
  1a21b8: 23 18 67 00  	subu	$3, $3, $7
  1a21bc: ff ff 6b 25  	addiu	$11, $11, -0x1 <.text+0xffffffffffefffff>
  1a21c0: 21 18 69 00  	addu	$3, $3, $9
  1a21c4: 00 00 00 00  	nop
  1a21c8: 23 18 67 00  	subu	$3, $3, $7
  1a21cc: ff ff a4 31  	andi	$4, $13, 0xffff
  1a21d0: 1b 00 6a 00  	divu	$zero, $3, $10
  1a21d4: 01 00 40 51  	beqzl	$10, 0x1a21dc <.text+0xa21dc>
  1a21d8: cd 01 00 00  	break	0x0, 0x7
  1a21dc: 12 10 00 00  	mflo	$2
  1a21e0: 10 18 00 00  	mfhi	$3
  1a21e4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a21e8: 25 18 64 00  	or	$3, $3, $4
  1a21ec: 12 40 00 00  	mflo	$8
  1a21f0: 18 38 59 00  	<unknown>
  1a21f4: 2b 10 67 00  	sltu	$2, $3, $7
  1a21f8: 0c 00 40 10  	beqz	$2, 0x1a222c <.text+0xa222c>
  1a21fc: 00 14 0b 00  	sll	$2, $11, 0x10
  1a2200: 21 18 69 00  	addu	$3, $3, $9
  1a2204: 2b 10 69 00  	sltu	$2, $3, $9
  1a2208: 07 00 40 14  	bnez	$2, 0x1a2228 <.text+0xa2228>
  1a220c: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2210: 2b 10 67 00  	sltu	$2, $3, $7
  1a2214: 05 00 40 10  	beqz	$2, 0x1a222c <.text+0xa222c>
  1a2218: 00 14 0b 00  	sll	$2, $11, 0x10
  1a221c: 21 18 69 00  	addu	$3, $3, $9
  1a2220: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2224: 00 00 00 00  	nop
  1a2228: 00 14 0b 00  	sll	$2, $11, 0x10
  1a222c: 23 68 67 00  	subu	$13, $3, $7
  1a2230: 8d ff 00 10  	b	0x1a2068 <.text+0xa2068>
  1a2234: 25 50 48 00  	or	$10, $2, $8
  1a2238: 04 48 e9 01  	sllv	$9, $9, $15
  1a223c: 06 28 08 03  	srlv	$5, $8, $24
  1a2240: 02 54 09 00  	srl	$10, $9, 0x10
  1a2244: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2248: ff ff 39 31  	andi	$25, $9, 0xffff
  1a224c: 06 18 0d 03  	srlv	$3, $13, $24
  1a2250: 04 10 e8 01  	sllv	$2, $8, $15
  1a2254: 25 40 43 00  	or	$8, $2, $3
  1a2258: 01 00 40 51  	beqzl	$10, 0x1a2260 <.text+0xa2260>
  1a225c: cd 01 00 00  	break	0x0, 0x7
  1a2260: 02 1c 08 00  	srl	$3, $8, 0x10
  1a2264: 2d 58 40 01  	move	$11, $10
  1a2268: 04 68 ed 01  	sllv	$13, $13, $15
  1a226c: 12 28 00 00  	mflo	$5
  1a2270: 10 20 00 00  	mfhi	$4
  1a2274: 00 24 04 00  	sll	$4, $4, 0x10
  1a2278: 25 18 83 00  	or	$3, $4, $3
  1a227c: 12 c0 00 00  	mflo	$24
  1a2280: 18 38 b9 00  	<unknown>
  1a2284: 2b 10 67 00  	sltu	$2, $3, $7
  1a2288: 0b 00 40 10  	beqz	$2, 0x1a22b8 <.text+0xa22b8>
  1a228c: 2d 80 20 03  	move	$16, $25
  1a2290: 21 18 69 00  	addu	$3, $3, $9
  1a2294: 2b 10 69 00  	sltu	$2, $3, $9
  1a2298: 07 00 40 14  	bnez	$2, 0x1a22b8 <.text+0xa22b8>
  1a229c: ff ff b8 24  	addiu	$24, $5, -0x1 <.text+0xffffffffffefffff>
  1a22a0: 2b 10 67 00  	sltu	$2, $3, $7
  1a22a4: 05 00 40 50  	beqzl	$2, 0x1a22bc <.text+0xa22bc>
  1a22a8: 23 18 67 00  	subu	$3, $3, $7
  1a22ac: ff ff 18 27  	addiu	$24, $24, -0x1 <.text+0xffffffffffefffff>
  1a22b0: 21 18 69 00  	addu	$3, $3, $9
  1a22b4: 00 00 00 00  	nop
  1a22b8: 23 18 67 00  	subu	$3, $3, $7
  1a22bc: ff ff 04 31  	andi	$4, $8, 0xffff
  1a22c0: 1b 00 6b 00  	divu	$zero, $3, $11
  1a22c4: 01 00 60 51  	beqzl	$11, 0x1a22cc <.text+0xa22cc>
  1a22c8: cd 01 00 00  	break	0x0, 0x7
  1a22cc: 12 10 00 00  	mflo	$2
  1a22d0: 10 18 00 00  	mfhi	$3
  1a22d4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a22d8: 25 18 64 00  	or	$3, $3, $4
  1a22dc: 12 28 00 00  	mflo	$5
  1a22e0: 18 38 50 00  	<unknown>
  1a22e4: 2b 10 67 00  	sltu	$2, $3, $7
  1a22e8: 0c 00 40 10  	beqz	$2, 0x1a231c <.text+0xa231c>
  1a22ec: 00 14 18 00  	sll	$2, $24, 0x10
  1a22f0: 21 18 69 00  	addu	$3, $3, $9
  1a22f4: 2b 10 69 00  	sltu	$2, $3, $9
  1a22f8: 07 00 40 14  	bnez	$2, 0x1a2318 <.text+0xa2318>
  1a22fc: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a2300: 2b 10 67 00  	sltu	$2, $3, $7
  1a2304: 05 00 40 10  	beqz	$2, 0x1a231c <.text+0xa231c>
  1a2308: 00 14 18 00  	sll	$2, $24, 0x10
  1a230c: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a2310: 21 18 69 00  	addu	$3, $3, $9
  1a2314: 00 00 00 00  	nop
  1a2318: 00 14 18 00  	sll	$2, $24, 0x10
  1a231c: 23 40 67 00  	subu	$8, $3, $7
  1a2320: 92 ff 00 10  	b	0x1a216c <.text+0xa216c>
  1a2324: 25 80 45 00  	or	$16, $2, $5
  1a2328: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a232c: ff ff 42 34  	ori	$2, $2, 0xffff
  1a2330: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a2334: 2b 10 49 00  	sltu	$2, $2, $9
  1a2338: 2d 28 60 00  	move	$5, $3
  1a233c: 7d ff 00 10  	b	0x1a2134 <.text+0xa2134>
  1a2340: 0b 28 82 00  	movn	$5, $4, $2
  1a2344: 00 00 00 00  	nop
  1a2348: 2b 10 07 01  	sltu	$2, $8, $7
  1a234c: 1f 00 40 14  	bnez	$2, 0x1a23cc <.text+0xa23cc>
  1a2350: 2d 50 00 00  	move	$10, $zero
  1a2354: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2358: 2b 10 47 00  	sltu	$2, $2, $7
  1a235c: 8c 00 40 14  	bnez	$2, 0x1a2590 <.text+0xa2590>
  1a2360: ff 00 02 3c  	lui	$2, 0xff
  1a2364: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2368: 00 01 e3 2c  	sltiu	$3, $7, 0x100
  1a236c: 2d 28 40 00  	move	$5, $2
  1a2370: 0b 28 03 00  	movn	$5, $zero, $3
  1a2374: 1c 00 03 3c  	lui	$3, 0x1c
  1a2378: 06 10 a7 00  	srlv	$2, $7, $5
  1a237c: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2380: 20 00 0a 24  	addiu	$10, $zero, 0x20
  1a2384: 21 10 43 00  	addu	$2, $2, $3
  1a2388: 00 00 44 90  	lbu	$4, 0x0($2)
  1a238c: 21 20 85 00  	addu	$4, $4, $5
  1a2390: 23 78 44 01  	subu	$15, $10, $4
  1a2394: 1c 00 e0 15  	bnez	$15, 0x1a2408 <.text+0xa2408>
  1a2398: 23 c0 4f 01  	subu	$24, $10, $15
  1a239c: 2b 10 e8 00  	sltu	$2, $7, $8
  1a23a0: 05 00 40 14  	bnez	$2, 0x1a23b8 <.text+0xa23b8>
  1a23a4: 23 20 a9 01  	subu	$4, $13, $9
  1a23a8: 2b 10 a9 01  	sltu	$2, $13, $9
  1a23ac: 07 00 40 14  	bnez	$2, 0x1a23cc <.text+0xa23cc>
  1a23b0: 2d 50 00 00  	move	$10, $zero
  1a23b4: 23 20 a9 01  	subu	$4, $13, $9
  1a23b8: 23 18 07 01  	subu	$3, $8, $7
  1a23bc: 2b 10 a4 01  	sltu	$2, $13, $4
  1a23c0: 01 00 0a 24  	addiu	$10, $zero, 0x1
  1a23c4: 23 40 62 00  	subu	$8, $3, $2
  1a23c8: 2d 68 80 00  	move	$13, $4
  1a23cc: 32 ff c0 10  	beqz	$6, 0x1a2098 <.text+0xa2098>
  1a23d0: 2d 80 00 00  	move	$16, $zero
  1a23d4: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a23d8: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a23dc: 3c 10 0d 00  	dsll32	$2, $13, 0x0
  1a23e0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a23e4: 24 60 83 01  	and	$12, $12, $3
  1a23e8: 25 60 82 01  	or	$12, $12, $2
  1a23ec: 3c 18 08 00  	dsll32	$3, $8, 0x0
  1a23f0: ff ff 02 3c  	lui	$2, 0xffff
  1a23f4: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a23f8: 24 60 82 01  	and	$12, $12, $2
  1a23fc: 25 ff 00 10  	b	0x1a2094 <.text+0xa2094>
  1a2400: 25 60 83 01  	or	$12, $12, $3
  1a2404: 00 00 00 00  	nop
  1a2408: 04 18 e7 01  	sllv	$3, $7, $15
  1a240c: 06 10 09 03  	srlv	$2, $9, $24
  1a2410: 06 28 08 03  	srlv	$5, $8, $24
  1a2414: 25 38 62 00  	or	$7, $3, $2
  1a2418: 04 20 e8 01  	sllv	$4, $8, $15
  1a241c: 02 54 07 00  	srl	$10, $7, 0x10
  1a2420: ff ff f0 30  	andi	$16, $7, 0xffff
  1a2424: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2428: 06 10 0d 03  	srlv	$2, $13, $24
  1a242c: 25 40 82 00  	or	$8, $4, $2
  1a2430: 01 00 40 51  	beqzl	$10, 0x1a2438 <.text+0xa2438>
  1a2434: cd 01 00 00  	break	0x0, 0x7
  1a2438: 02 24 08 00  	srl	$4, $8, 0x10
  1a243c: 04 48 e9 01  	sllv	$9, $9, $15
  1a2440: 12 28 00 00  	mflo	$5
  1a2444: 10 18 00 00  	mfhi	$3
  1a2448: 00 1c 03 00  	sll	$3, $3, 0x10
  1a244c: 25 18 64 00  	or	$3, $3, $4
  1a2450: 12 c8 00 00  	mflo	$25
  1a2454: 18 58 b0 00  	<unknown>
  1a2458: 2b 10 6b 00  	sltu	$2, $3, $11
  1a245c: 0a 00 40 10  	beqz	$2, 0x1a2488 <.text+0xa2488>
  1a2460: 04 68 ed 01  	sllv	$13, $13, $15
  1a2464: 21 18 67 00  	addu	$3, $3, $7
  1a2468: 2b 10 67 00  	sltu	$2, $3, $7
  1a246c: 06 00 40 14  	bnez	$2, 0x1a2488 <.text+0xa2488>
  1a2470: ff ff b9 24  	addiu	$25, $5, -0x1 <.text+0xffffffffffefffff>
  1a2474: 2b 10 6b 00  	sltu	$2, $3, $11
  1a2478: 04 00 40 50  	beqzl	$2, 0x1a248c <.text+0xa248c>
  1a247c: 23 18 6b 00  	subu	$3, $3, $11
  1a2480: ff ff 39 27  	addiu	$25, $25, -0x1 <.text+0xffffffffffefffff>
  1a2484: 21 18 67 00  	addu	$3, $3, $7
  1a2488: 23 18 6b 00  	subu	$3, $3, $11
  1a248c: ff ff 04 31  	andi	$4, $8, 0xffff
  1a2490: 1b 00 6a 00  	divu	$zero, $3, $10
  1a2494: 01 00 40 51  	beqzl	$10, 0x1a249c <.text+0xa249c>
  1a2498: cd 01 00 00  	break	0x0, 0x7
  1a249c: 12 10 00 00  	mflo	$2
  1a24a0: 10 18 00 00  	mfhi	$3
  1a24a4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a24a8: 25 28 64 00  	or	$5, $3, $4
  1a24ac: 12 40 00 00  	mflo	$8
  1a24b0: 18 58 50 00  	<unknown>
  1a24b4: 2b 10 ab 00  	sltu	$2, $5, $11
  1a24b8: 0c 00 40 10  	beqz	$2, 0x1a24ec <.text+0xa24ec>
  1a24bc: 00 14 19 00  	sll	$2, $25, 0x10
  1a24c0: 21 28 a7 00  	addu	$5, $5, $7
  1a24c4: 2b 10 a7 00  	sltu	$2, $5, $7
  1a24c8: 07 00 40 14  	bnez	$2, 0x1a24e8 <.text+0xa24e8>
  1a24cc: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a24d0: 2b 10 ab 00  	sltu	$2, $5, $11
  1a24d4: 05 00 40 10  	beqz	$2, 0x1a24ec <.text+0xa24ec>
  1a24d8: 00 14 19 00  	sll	$2, $25, 0x10
  1a24dc: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a24e0: 21 28 a7 00  	addu	$5, $5, $7
  1a24e4: 00 00 00 00  	nop
  1a24e8: 00 14 19 00  	sll	$2, $25, 0x10
  1a24ec: 23 28 ab 00  	subu	$5, $5, $11
  1a24f0: 25 50 48 00  	or	$10, $2, $8
  1a24f4: 19 00 49 01  	multu	$10, $9
  1a24f8: 10 18 00 00  	mfhi	$3
  1a24fc: 12 58 00 00  	mflo	$11
  1a2500: 2b 10 a3 00  	sltu	$2, $5, $3
  1a2504: 1b 00 40 14  	bnez	$2, 0x1a2574 <.text+0xa2574>
  1a2508: 23 20 69 01  	subu	$4, $11, $9
  1a250c: 17 00 65 10  	beq	$3, $5, 0x1a256c <.text+0xa256c>
  1a2510: 2b 10 ab 01  	sltu	$2, $13, $11
  1a2514: e0 fe c0 10  	beqz	$6, 0x1a2098 <.text+0xa2098>
  1a2518: 2d 80 00 00  	move	$16, $zero
  1a251c: 23 20 ab 01  	subu	$4, $13, $11
  1a2520: 23 28 a3 00  	subu	$5, $5, $3
  1a2524: 2b 18 a4 01  	sltu	$3, $13, $4
  1a2528: 06 20 e4 01  	srlv	$4, $4, $15
  1a252c: 23 40 a3 00  	subu	$8, $5, $3
  1a2530: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2534: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2538: 04 10 08 03  	sllv	$2, $8, $24
  1a253c: 24 60 83 01  	and	$12, $12, $3
  1a2540: 25 10 44 00  	or	$2, $2, $4
  1a2544: 06 28 e8 01  	srlv	$5, $8, $15
  1a2548: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a254c: ff ff 03 3c  	lui	$3, 0xffff
  1a2550: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a2554: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2558: 3c 28 05 00  	dsll32	$5, $5, 0x0
  1a255c: 25 60 82 01  	or	$12, $12, $2
  1a2560: 24 60 83 01  	and	$12, $12, $3
  1a2564: cb fe 00 10  	b	0x1a2094 <.text+0xa2094>
  1a2568: 25 60 85 01  	or	$12, $12, $5
  1a256c: e9 ff 40 10  	beqz	$2, 0x1a2514 <.text+0xa2514>
  1a2570: 23 20 69 01  	subu	$4, $11, $9
  1a2574: 23 18 67 00  	subu	$3, $3, $7
  1a2578: 2b 10 64 01  	sltu	$2, $11, $4
  1a257c: ff ff 4a 25  	addiu	$10, $10, -0x1 <.text+0xffffffffffefffff>
  1a2580: 23 18 62 00  	subu	$3, $3, $2
  1a2584: e3 ff 00 10  	b	0x1a2514 <.text+0xa2514>
  1a2588: 2d 58 80 00  	move	$11, $4
  1a258c: 00 00 00 00  	nop
  1a2590: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a2594: ff ff 42 34  	ori	$2, $2, 0xffff
  1a2598: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a259c: 2b 10 47 00  	sltu	$2, $2, $7
  1a25a0: 2d 28 60 00  	move	$5, $3
  1a25a4: 73 ff 00 10  	b	0x1a2374 <.text+0xa2374>
  1a25a8: 0b 28 82 00  	movn	$5, $4, $2
  1a25ac: 00 00 00 00  	nop
  1a25b0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a25b4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a25b8: 74 89 06 0c  	jal	0x1a25d0 <.text+0xa25d0>
  1a25bc: 2d 30 00 00  	move	$6, $zero
  1a25c0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a25c4: 08 00 e0 03  	jr	$ra
  1a25c8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a25cc: 00 00 00 00  	nop
  1a25d0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a25d4: 3f 38 05 00  	dsra32	$7, $5, 0x0
  1a25d8: 3f 40 04 00  	dsra32	$8, $4, 0x0
  1a25dc: 3c 48 05 00  	dsll32	$9, $5, 0x0
  1a25e0: 3f 48 09 00  	dsra32	$9, $9, 0x0
  1a25e4: 3c 68 04 00  	dsll32	$13, $4, 0x0
  1a25e8: 3f 68 0d 00  	dsra32	$13, $13, 0x0
  1a25ec: 08 01 e0 14  	bnez	$7, 0x1a2a10 <.text+0xa2a10>
  1a25f0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a25f4: 2b 10 09 01  	sltu	$2, $8, $9
  1a25f8: 71 00 40 10  	beqz	$2, 0x1a27c0 <.text+0xa27c0>
  1a25fc: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2600: 2b 10 49 00  	sltu	$2, $2, $9
  1a2604: 66 00 40 14  	bnez	$2, 0x1a27a0 <.text+0xa27a0>
  1a2608: ff 00 02 3c  	lui	$2, 0xff
  1a260c: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2610: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a2614: 2d 28 40 00  	move	$5, $2
  1a2618: 0b 28 03 00  	movn	$5, $zero, $3
  1a261c: 1c 00 03 3c  	lui	$3, 0x1c
  1a2620: 06 10 a9 00  	srlv	$2, $9, $5
  1a2624: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2628: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a262c: 21 10 43 00  	addu	$2, $2, $3
  1a2630: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2634: 21 20 85 00  	addu	$4, $4, $5
  1a2638: 23 78 e4 00  	subu	$15, $7, $4
  1a263c: 08 00 e0 51  	beqzl	$15, 0x1a2660 <.text+0xa2660>
  1a2640: 02 3c 09 00  	srl	$7, $9, 0x10
  1a2644: 23 10 ef 00  	subu	$2, $7, $15
  1a2648: 04 18 e8 01  	sllv	$3, $8, $15
  1a264c: 06 10 4d 00  	srlv	$2, $13, $2
  1a2650: 04 48 e9 01  	sllv	$9, $9, $15
  1a2654: 25 40 62 00  	or	$8, $3, $2
  1a2658: 04 68 ed 01  	sllv	$13, $13, $15
  1a265c: 02 3c 09 00  	srl	$7, $9, 0x10
  1a2660: ff ff 2a 31  	andi	$10, $9, 0xffff
  1a2664: 1b 00 07 01  	divu	$zero, $8, $7
  1a2668: 02 24 0d 00  	srl	$4, $13, 0x10
  1a266c: 01 00 e0 50  	beqzl	$7, 0x1a2674 <.text+0xa2674>
  1a2670: cd 01 00 00  	break	0x0, 0x7
  1a2674: 12 10 00 00  	mflo	$2
  1a2678: 10 18 00 00  	mfhi	$3
  1a267c: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2680: 25 18 64 00  	or	$3, $3, $4
  1a2684: 12 28 00 00  	mflo	$5
  1a2688: 18 40 4a 00  	<unknown>
  1a268c: 2b 10 68 00  	sltu	$2, $3, $8
  1a2690: 0c 00 40 50  	beqzl	$2, 0x1a26c4 <.text+0xa26c4>
  1a2694: 23 18 68 00  	subu	$3, $3, $8
  1a2698: 21 18 69 00  	addu	$3, $3, $9
  1a269c: 2b 10 69 00  	sltu	$2, $3, $9
  1a26a0: 07 00 40 14  	bnez	$2, 0x1a26c0 <.text+0xa26c0>
  1a26a4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a26a8: 2b 10 68 00  	sltu	$2, $3, $8
  1a26ac: 05 00 40 50  	beqzl	$2, 0x1a26c4 <.text+0xa26c4>
  1a26b0: 23 18 68 00  	subu	$3, $3, $8
  1a26b4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a26b8: 21 18 69 00  	addu	$3, $3, $9
  1a26bc: 00 00 00 00  	nop
  1a26c0: 23 18 68 00  	subu	$3, $3, $8
  1a26c4: 01 00 e0 50  	beqzl	$7, 0x1a26cc <.text+0xa26cc>
  1a26c8: cd 01 00 00  	break	0x0, 0x7
  1a26cc: 1b 00 67 00  	divu	$zero, $3, $7
  1a26d0: ff ff a4 31  	andi	$4, $13, 0xffff
  1a26d4: 12 10 00 00  	mflo	$2
  1a26d8: 10 18 00 00  	mfhi	$3
  1a26dc: 00 1c 03 00  	sll	$3, $3, 0x10
  1a26e0: 25 18 64 00  	or	$3, $3, $4
  1a26e4: 12 38 00 00  	mflo	$7
  1a26e8: 18 40 4a 00  	<unknown>
  1a26ec: 2b 10 68 00  	sltu	$2, $3, $8
  1a26f0: 0c 00 40 10  	beqz	$2, 0x1a2724 <.text+0xa2724>
  1a26f4: 00 14 05 00  	sll	$2, $5, 0x10
  1a26f8: 21 18 69 00  	addu	$3, $3, $9
  1a26fc: 2b 10 69 00  	sltu	$2, $3, $9
  1a2700: 07 00 40 14  	bnez	$2, 0x1a2720 <.text+0xa2720>
  1a2704: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2708: 2b 10 68 00  	sltu	$2, $3, $8
  1a270c: 05 00 40 10  	beqz	$2, 0x1a2724 <.text+0xa2724>
  1a2710: 00 14 05 00  	sll	$2, $5, 0x10
  1a2714: 21 18 69 00  	addu	$3, $3, $9
  1a2718: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a271c: 00 00 00 00  	nop
  1a2720: 00 14 05 00  	sll	$2, $5, 0x10
  1a2724: 23 68 68 00  	subu	$13, $3, $8
  1a2728: 25 50 47 00  	or	$10, $2, $7
  1a272c: 2d 80 00 00  	move	$16, $zero
  1a2730: 0b 00 c0 10  	beqz	$6, 0x1a2760 <.text+0xa2760>
  1a2734: 06 10 ed 01  	srlv	$2, $13, $15
  1a2738: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a273c: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2740: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a2744: 24 60 83 01  	and	$12, $12, $3
  1a2748: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a274c: ff ff 03 3c  	lui	$3, 0xffff
  1a2750: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a2754: 25 60 82 01  	or	$12, $12, $2
  1a2758: 24 60 83 01  	and	$12, $12, $3
  1a275c: 00 00 cc fc  	sd	$12, 0x0($6)
  1a2760: 3c 10 0a 00  	dsll32	$2, $10, 0x0
  1a2764: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2768: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a276c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2770: 24 70 c3 01  	and	$14, $14, $3
  1a2774: 25 70 c2 01  	or	$14, $14, $2
  1a2778: 3c 18 10 00  	dsll32	$3, $16, 0x0
  1a277c: ff ff 02 3c  	lui	$2, 0xffff
  1a2780: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2784: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a2788: 24 70 c2 01  	and	$14, $14, $2
  1a278c: 25 70 c3 01  	or	$14, $14, $3
  1a2790: 2d 10 c0 01  	move	$2, $14
  1a2794: 08 00 e0 03  	jr	$ra
  1a2798: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a279c: 00 00 00 00  	nop
  1a27a0: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a27a4: ff ff 42 34  	ori	$2, $2, 0xffff
  1a27a8: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a27ac: 2b 10 49 00  	sltu	$2, $2, $9
  1a27b0: 2d 28 60 00  	move	$5, $3
  1a27b4: 99 ff 00 10  	b	0x1a261c <.text+0xa261c>
  1a27b8: 0b 28 82 00  	movn	$5, $4, $2
  1a27bc: 00 00 00 00  	nop
  1a27c0: 08 00 20 15  	bnez	$9, 0x1a27e4 <.text+0xa27e4>
  1a27c4: 2b 10 49 00  	sltu	$2, $2, $9
  1a27c8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a27cc: 01 00 20 51  	beqzl	$9, 0x1a27d4 <.text+0xa27d4>
  1a27d0: cd 01 00 00  	break	0x0, 0x7
  1a27d4: 1b 00 47 00  	divu	$zero, $2, $7
  1a27d8: 12 48 00 00  	mflo	$9
  1a27dc: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a27e0: 2b 10 49 00  	sltu	$2, $2, $9
  1a27e4: 82 00 40 14  	bnez	$2, 0x1a29f0 <.text+0xa29f0>
  1a27e8: ff 00 02 3c  	lui	$2, 0xff
  1a27ec: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a27f0: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a27f4: 2d 28 40 00  	move	$5, $2
  1a27f8: 0b 28 03 00  	movn	$5, $zero, $3
  1a27fc: 1c 00 03 3c  	lui	$3, 0x1c
  1a2800: 06 10 a9 00  	srlv	$2, $9, $5
  1a2804: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2808: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a280c: 21 10 43 00  	addu	$2, $2, $3
  1a2810: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2814: 21 20 85 00  	addu	$4, $4, $5
  1a2818: 23 78 e4 00  	subu	$15, $7, $4
  1a281c: 38 00 e0 15  	bnez	$15, 0x1a2900 <.text+0xa2900>
  1a2820: 23 c0 ef 00  	subu	$24, $7, $15
  1a2824: 23 40 09 01  	subu	$8, $8, $9
  1a2828: 01 00 10 24  	addiu	$16, $zero, 0x1
  1a282c: 02 54 09 00  	srl	$10, $9, 0x10
  1a2830: ff ff 39 31  	andi	$25, $9, 0xffff
  1a2834: 1b 00 0a 01  	divu	$zero, $8, $10
  1a2838: 02 2c 0d 00  	srl	$5, $13, 0x10
  1a283c: 01 00 40 51  	beqzl	$10, 0x1a2844 <.text+0xa2844>
  1a2840: cd 01 00 00  	break	0x0, 0x7
  1a2844: 12 20 00 00  	mflo	$4
  1a2848: 10 18 00 00  	mfhi	$3
  1a284c: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2850: 25 18 65 00  	or	$3, $3, $5
  1a2854: 12 58 00 00  	mflo	$11
  1a2858: 18 38 99 00  	<unknown>
  1a285c: 2b 10 67 00  	sltu	$2, $3, $7
  1a2860: 0c 00 40 50  	beqzl	$2, 0x1a2894 <.text+0xa2894>
  1a2864: 23 18 67 00  	subu	$3, $3, $7
  1a2868: 21 18 69 00  	addu	$3, $3, $9
  1a286c: 2b 10 69 00  	sltu	$2, $3, $9
  1a2870: 07 00 40 14  	bnez	$2, 0x1a2890 <.text+0xa2890>
  1a2874: ff ff 8b 24  	addiu	$11, $4, -0x1 <.text+0xffffffffffefffff>
  1a2878: 2b 10 67 00  	sltu	$2, $3, $7
  1a287c: 05 00 40 50  	beqzl	$2, 0x1a2894 <.text+0xa2894>
  1a2880: 23 18 67 00  	subu	$3, $3, $7
  1a2884: ff ff 6b 25  	addiu	$11, $11, -0x1 <.text+0xffffffffffefffff>
  1a2888: 21 18 69 00  	addu	$3, $3, $9
  1a288c: 00 00 00 00  	nop
  1a2890: 23 18 67 00  	subu	$3, $3, $7
  1a2894: ff ff a4 31  	andi	$4, $13, 0xffff
  1a2898: 1b 00 6a 00  	divu	$zero, $3, $10
  1a289c: 01 00 40 51  	beqzl	$10, 0x1a28a4 <.text+0xa28a4>
  1a28a0: cd 01 00 00  	break	0x0, 0x7
  1a28a4: 12 10 00 00  	mflo	$2
  1a28a8: 10 18 00 00  	mfhi	$3
  1a28ac: 00 1c 03 00  	sll	$3, $3, 0x10
  1a28b0: 25 18 64 00  	or	$3, $3, $4
  1a28b4: 12 40 00 00  	mflo	$8
  1a28b8: 18 38 59 00  	<unknown>
  1a28bc: 2b 10 67 00  	sltu	$2, $3, $7
  1a28c0: 0c 00 40 10  	beqz	$2, 0x1a28f4 <.text+0xa28f4>
  1a28c4: 00 14 0b 00  	sll	$2, $11, 0x10
  1a28c8: 21 18 69 00  	addu	$3, $3, $9
  1a28cc: 2b 10 69 00  	sltu	$2, $3, $9
  1a28d0: 07 00 40 14  	bnez	$2, 0x1a28f0 <.text+0xa28f0>
  1a28d4: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a28d8: 2b 10 67 00  	sltu	$2, $3, $7
  1a28dc: 05 00 40 10  	beqz	$2, 0x1a28f4 <.text+0xa28f4>
  1a28e0: 00 14 0b 00  	sll	$2, $11, 0x10
  1a28e4: 21 18 69 00  	addu	$3, $3, $9
  1a28e8: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a28ec: 00 00 00 00  	nop
  1a28f0: 00 14 0b 00  	sll	$2, $11, 0x10
  1a28f4: 23 68 67 00  	subu	$13, $3, $7
  1a28f8: 8d ff 00 10  	b	0x1a2730 <.text+0xa2730>
  1a28fc: 25 50 48 00  	or	$10, $2, $8
  1a2900: 04 48 e9 01  	sllv	$9, $9, $15
  1a2904: 06 28 08 03  	srlv	$5, $8, $24
  1a2908: 02 54 09 00  	srl	$10, $9, 0x10
  1a290c: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2910: ff ff 39 31  	andi	$25, $9, 0xffff
  1a2914: 06 18 0d 03  	srlv	$3, $13, $24
  1a2918: 04 10 e8 01  	sllv	$2, $8, $15
  1a291c: 25 40 43 00  	or	$8, $2, $3
  1a2920: 01 00 40 51  	beqzl	$10, 0x1a2928 <.text+0xa2928>
  1a2924: cd 01 00 00  	break	0x0, 0x7
  1a2928: 02 1c 08 00  	srl	$3, $8, 0x10
  1a292c: 2d 58 40 01  	move	$11, $10
  1a2930: 04 68 ed 01  	sllv	$13, $13, $15
  1a2934: 12 28 00 00  	mflo	$5
  1a2938: 10 20 00 00  	mfhi	$4
  1a293c: 00 24 04 00  	sll	$4, $4, 0x10
  1a2940: 25 18 83 00  	or	$3, $4, $3
  1a2944: 12 c0 00 00  	mflo	$24
  1a2948: 18 38 b9 00  	<unknown>
  1a294c: 2b 10 67 00  	sltu	$2, $3, $7
  1a2950: 0b 00 40 10  	beqz	$2, 0x1a2980 <.text+0xa2980>
  1a2954: 2d 80 20 03  	move	$16, $25
  1a2958: 21 18 69 00  	addu	$3, $3, $9
  1a295c: 2b 10 69 00  	sltu	$2, $3, $9
  1a2960: 07 00 40 14  	bnez	$2, 0x1a2980 <.text+0xa2980>
  1a2964: ff ff b8 24  	addiu	$24, $5, -0x1 <.text+0xffffffffffefffff>
  1a2968: 2b 10 67 00  	sltu	$2, $3, $7
  1a296c: 05 00 40 50  	beqzl	$2, 0x1a2984 <.text+0xa2984>
  1a2970: 23 18 67 00  	subu	$3, $3, $7
  1a2974: ff ff 18 27  	addiu	$24, $24, -0x1 <.text+0xffffffffffefffff>
  1a2978: 21 18 69 00  	addu	$3, $3, $9
  1a297c: 00 00 00 00  	nop
  1a2980: 23 18 67 00  	subu	$3, $3, $7
  1a2984: ff ff 04 31  	andi	$4, $8, 0xffff
  1a2988: 1b 00 6b 00  	divu	$zero, $3, $11
  1a298c: 01 00 60 51  	beqzl	$11, 0x1a2994 <.text+0xa2994>
  1a2990: cd 01 00 00  	break	0x0, 0x7
  1a2994: 12 10 00 00  	mflo	$2
  1a2998: 10 18 00 00  	mfhi	$3
  1a299c: 00 1c 03 00  	sll	$3, $3, 0x10
  1a29a0: 25 18 64 00  	or	$3, $3, $4
  1a29a4: 12 28 00 00  	mflo	$5
  1a29a8: 18 38 50 00  	<unknown>
  1a29ac: 2b 10 67 00  	sltu	$2, $3, $7
  1a29b0: 0c 00 40 10  	beqz	$2, 0x1a29e4 <.text+0xa29e4>
  1a29b4: 00 14 18 00  	sll	$2, $24, 0x10
  1a29b8: 21 18 69 00  	addu	$3, $3, $9
  1a29bc: 2b 10 69 00  	sltu	$2, $3, $9
  1a29c0: 07 00 40 14  	bnez	$2, 0x1a29e0 <.text+0xa29e0>
  1a29c4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a29c8: 2b 10 67 00  	sltu	$2, $3, $7
  1a29cc: 05 00 40 10  	beqz	$2, 0x1a29e4 <.text+0xa29e4>
  1a29d0: 00 14 18 00  	sll	$2, $24, 0x10
  1a29d4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a29d8: 21 18 69 00  	addu	$3, $3, $9
  1a29dc: 00 00 00 00  	nop
  1a29e0: 00 14 18 00  	sll	$2, $24, 0x10
  1a29e4: 23 40 67 00  	subu	$8, $3, $7
  1a29e8: 92 ff 00 10  	b	0x1a2834 <.text+0xa2834>
  1a29ec: 25 80 45 00  	or	$16, $2, $5
  1a29f0: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a29f4: ff ff 42 34  	ori	$2, $2, 0xffff
  1a29f8: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a29fc: 2b 10 49 00  	sltu	$2, $2, $9
  1a2a00: 2d 28 60 00  	move	$5, $3
  1a2a04: 7d ff 00 10  	b	0x1a27fc <.text+0xa27fc>
  1a2a08: 0b 28 82 00  	movn	$5, $4, $2
  1a2a0c: 00 00 00 00  	nop
  1a2a10: 2b 10 07 01  	sltu	$2, $8, $7
  1a2a14: 1f 00 40 14  	bnez	$2, 0x1a2a94 <.text+0xa2a94>
  1a2a18: 2d 50 00 00  	move	$10, $zero
  1a2a1c: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2a20: 2b 10 47 00  	sltu	$2, $2, $7
  1a2a24: 8c 00 40 14  	bnez	$2, 0x1a2c58 <.text+0xa2c58>
  1a2a28: ff 00 02 3c  	lui	$2, 0xff
  1a2a2c: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2a30: 00 01 e3 2c  	sltiu	$3, $7, 0x100
  1a2a34: 2d 28 40 00  	move	$5, $2
  1a2a38: 0b 28 03 00  	movn	$5, $zero, $3
  1a2a3c: 1c 00 03 3c  	lui	$3, 0x1c
  1a2a40: 06 10 a7 00  	srlv	$2, $7, $5
  1a2a44: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2a48: 20 00 0a 24  	addiu	$10, $zero, 0x20
  1a2a4c: 21 10 43 00  	addu	$2, $2, $3
  1a2a50: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2a54: 21 20 85 00  	addu	$4, $4, $5
  1a2a58: 23 78 44 01  	subu	$15, $10, $4
  1a2a5c: 1c 00 e0 15  	bnez	$15, 0x1a2ad0 <.text+0xa2ad0>
  1a2a60: 23 c0 4f 01  	subu	$24, $10, $15
  1a2a64: 2b 10 e8 00  	sltu	$2, $7, $8
  1a2a68: 05 00 40 14  	bnez	$2, 0x1a2a80 <.text+0xa2a80>
  1a2a6c: 23 20 a9 01  	subu	$4, $13, $9
  1a2a70: 2b 10 a9 01  	sltu	$2, $13, $9
  1a2a74: 07 00 40 14  	bnez	$2, 0x1a2a94 <.text+0xa2a94>
  1a2a78: 2d 50 00 00  	move	$10, $zero
  1a2a7c: 23 20 a9 01  	subu	$4, $13, $9
  1a2a80: 23 18 07 01  	subu	$3, $8, $7
  1a2a84: 2b 10 a4 01  	sltu	$2, $13, $4
  1a2a88: 01 00 0a 24  	addiu	$10, $zero, 0x1
  1a2a8c: 23 40 62 00  	subu	$8, $3, $2
  1a2a90: 2d 68 80 00  	move	$13, $4
  1a2a94: 32 ff c0 10  	beqz	$6, 0x1a2760 <.text+0xa2760>
  1a2a98: 2d 80 00 00  	move	$16, $zero
  1a2a9c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2aa0: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2aa4: 3c 10 0d 00  	dsll32	$2, $13, 0x0
  1a2aa8: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2aac: 24 60 83 01  	and	$12, $12, $3
  1a2ab0: 25 60 82 01  	or	$12, $12, $2
  1a2ab4: 3c 18 08 00  	dsll32	$3, $8, 0x0
  1a2ab8: ff ff 02 3c  	lui	$2, 0xffff
  1a2abc: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2ac0: 24 60 82 01  	and	$12, $12, $2
  1a2ac4: 25 ff 00 10  	b	0x1a275c <.text+0xa275c>
  1a2ac8: 25 60 83 01  	or	$12, $12, $3
  1a2acc: 00 00 00 00  	nop
  1a2ad0: 04 18 e7 01  	sllv	$3, $7, $15
  1a2ad4: 06 10 09 03  	srlv	$2, $9, $24
  1a2ad8: 06 28 08 03  	srlv	$5, $8, $24
  1a2adc: 25 38 62 00  	or	$7, $3, $2
  1a2ae0: 04 20 e8 01  	sllv	$4, $8, $15
  1a2ae4: 02 54 07 00  	srl	$10, $7, 0x10
  1a2ae8: ff ff f0 30  	andi	$16, $7, 0xffff
  1a2aec: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2af0: 06 10 0d 03  	srlv	$2, $13, $24
  1a2af4: 25 40 82 00  	or	$8, $4, $2
  1a2af8: 01 00 40 51  	beqzl	$10, 0x1a2b00 <.text+0xa2b00>
  1a2afc: cd 01 00 00  	break	0x0, 0x7
  1a2b00: 02 24 08 00  	srl	$4, $8, 0x10
  1a2b04: 04 48 e9 01  	sllv	$9, $9, $15
  1a2b08: 12 28 00 00  	mflo	$5
  1a2b0c: 10 18 00 00  	mfhi	$3
  1a2b10: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2b14: 25 18 64 00  	or	$3, $3, $4
  1a2b18: 12 c8 00 00  	mflo	$25
  1a2b1c: 18 58 b0 00  	<unknown>
  1a2b20: 2b 10 6b 00  	sltu	$2, $3, $11
  1a2b24: 0a 00 40 10  	beqz	$2, 0x1a2b50 <.text+0xa2b50>
  1a2b28: 04 68 ed 01  	sllv	$13, $13, $15
  1a2b2c: 21 18 67 00  	addu	$3, $3, $7
  1a2b30: 2b 10 67 00  	sltu	$2, $3, $7
  1a2b34: 06 00 40 14  	bnez	$2, 0x1a2b50 <.text+0xa2b50>
  1a2b38: ff ff b9 24  	addiu	$25, $5, -0x1 <.text+0xffffffffffefffff>
  1a2b3c: 2b 10 6b 00  	sltu	$2, $3, $11
  1a2b40: 04 00 40 50  	beqzl	$2, 0x1a2b54 <.text+0xa2b54>
  1a2b44: 23 18 6b 00  	subu	$3, $3, $11
  1a2b48: ff ff 39 27  	addiu	$25, $25, -0x1 <.text+0xffffffffffefffff>
  1a2b4c: 21 18 67 00  	addu	$3, $3, $7
  1a2b50: 23 18 6b 00  	subu	$3, $3, $11
  1a2b54: ff ff 04 31  	andi	$4, $8, 0xffff
  1a2b58: 1b 00 6a 00  	divu	$zero, $3, $10
  1a2b5c: 01 00 40 51  	beqzl	$10, 0x1a2b64 <.text+0xa2b64>
  1a2b60: cd 01 00 00  	break	0x0, 0x7
  1a2b64: 12 10 00 00  	mflo	$2
  1a2b68: 10 18 00 00  	mfhi	$3
  1a2b6c: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2b70: 25 28 64 00  	or	$5, $3, $4
  1a2b74: 12 40 00 00  	mflo	$8
  1a2b78: 18 58 50 00  	<unknown>
  1a2b7c: 2b 10 ab 00  	sltu	$2, $5, $11
  1a2b80: 0c 00 40 10  	beqz	$2, 0x1a2bb4 <.text+0xa2bb4>
  1a2b84: 00 14 19 00  	sll	$2, $25, 0x10
  1a2b88: 21 28 a7 00  	addu	$5, $5, $7
  1a2b8c: 2b 10 a7 00  	sltu	$2, $5, $7
  1a2b90: 07 00 40 14  	bnez	$2, 0x1a2bb0 <.text+0xa2bb0>
  1a2b94: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2b98: 2b 10 ab 00  	sltu	$2, $5, $11
  1a2b9c: 05 00 40 10  	beqz	$2, 0x1a2bb4 <.text+0xa2bb4>
  1a2ba0: 00 14 19 00  	sll	$2, $25, 0x10
  1a2ba4: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2ba8: 21 28 a7 00  	addu	$5, $5, $7
  1a2bac: 00 00 00 00  	nop
  1a2bb0: 00 14 19 00  	sll	$2, $25, 0x10
  1a2bb4: 23 28 ab 00  	subu	$5, $5, $11
  1a2bb8: 25 50 48 00  	or	$10, $2, $8
  1a2bbc: 19 00 49 01  	multu	$10, $9
  1a2bc0: 10 18 00 00  	mfhi	$3
  1a2bc4: 12 58 00 00  	mflo	$11
  1a2bc8: 2b 10 a3 00  	sltu	$2, $5, $3
  1a2bcc: 1b 00 40 14  	bnez	$2, 0x1a2c3c <.text+0xa2c3c>
  1a2bd0: 23 20 69 01  	subu	$4, $11, $9
  1a2bd4: 17 00 65 10  	beq	$3, $5, 0x1a2c34 <.text+0xa2c34>
  1a2bd8: 2b 10 ab 01  	sltu	$2, $13, $11
  1a2bdc: e0 fe c0 10  	beqz	$6, 0x1a2760 <.text+0xa2760>
  1a2be0: 2d 80 00 00  	move	$16, $zero
  1a2be4: 23 20 ab 01  	subu	$4, $13, $11
  1a2be8: 23 28 a3 00  	subu	$5, $5, $3
  1a2bec: 2b 18 a4 01  	sltu	$3, $13, $4
  1a2bf0: 06 20 e4 01  	srlv	$4, $4, $15
  1a2bf4: 23 40 a3 00  	subu	$8, $5, $3
  1a2bf8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2bfc: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2c00: 04 10 08 03  	sllv	$2, $8, $24
  1a2c04: 24 60 83 01  	and	$12, $12, $3
  1a2c08: 25 10 44 00  	or	$2, $2, $4
  1a2c0c: 06 28 e8 01  	srlv	$5, $8, $15
  1a2c10: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a2c14: ff ff 03 3c  	lui	$3, 0xffff
  1a2c18: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a2c1c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2c20: 3c 28 05 00  	dsll32	$5, $5, 0x0
  1a2c24: 25 60 82 01  	or	$12, $12, $2
  1a2c28: 24 60 83 01  	and	$12, $12, $3
  1a2c2c: cb fe 00 10  	b	0x1a275c <.text+0xa275c>
  1a2c30: 25 60 85 01  	or	$12, $12, $5
  1a2c34: e9 ff 40 10  	beqz	$2, 0x1a2bdc <.text+0xa2bdc>
  1a2c38: 23 20 69 01  	subu	$4, $11, $9
  1a2c3c: 23 18 67 00  	subu	$3, $3, $7
  1a2c40: 2b 10 64 01  	sltu	$2, $11, $4
  1a2c44: ff ff 4a 25  	addiu	$10, $10, -0x1 <.text+0xffffffffffefffff>
  1a2c48: 23 18 62 00  	subu	$3, $3, $2
  1a2c4c: e3 ff 00 10  	b	0x1a2bdc <.text+0xa2bdc>
  1a2c50: 2d 58 80 00  	move	$11, $4
  1a2c54: 00 00 00 00  	nop
  1a2c58: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a2c5c: ff ff 42 34  	ori	$2, $2, 0xffff
  1a2c60: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a2c64: 2b 10 47 00  	sltu	$2, $2, $7
  1a2c68: 2d 28 60 00  	move	$5, $3
  1a2c6c: 73 ff 00 10  	b	0x1a2a3c <.text+0xa2a3c>
  1a2c70: 0b 28 82 00  	movn	$5, $4, $2
  1a2c74: 00 00 00 00  	nop
  1a2c78: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a2c7c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a2c80: 26 8b 06 0c  	jal	0x1a2c98 <.text+0xa2c98>
  1a2c84: 2d 30 a0 03  	move	$6, $sp
  1a2c88: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a2c8c: 00 00 a2 df  	ld	$2, 0x0($sp)
  1a2c90: 08 00 e0 03  	jr	$ra
  1a2c94: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a2c98: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a2c9c: 3f 38 05 00  	dsra32	$7, $5, 0x0
  1a2ca0: 3f 40 04 00  	dsra32	$8, $4, 0x0
  1a2ca4: 3c 48 05 00  	dsll32	$9, $5, 0x0
  1a2ca8: 3f 48 09 00  	dsra32	$9, $9, 0x0
  1a2cac: 3c 68 04 00  	dsll32	$13, $4, 0x0
  1a2cb0: 3f 68 0d 00  	dsra32	$13, $13, 0x0
  1a2cb4: 08 01 e0 14  	bnez	$7, 0x1a30d8 <.text+0xa30d8>
  1a2cb8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a2cbc: 2b 10 09 01  	sltu	$2, $8, $9
  1a2cc0: 71 00 40 10  	beqz	$2, 0x1a2e88 <.text+0xa2e88>
  1a2cc4: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2cc8: 2b 10 49 00  	sltu	$2, $2, $9
  1a2ccc: 66 00 40 14  	bnez	$2, 0x1a2e68 <.text+0xa2e68>
  1a2cd0: ff 00 02 3c  	lui	$2, 0xff
  1a2cd4: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2cd8: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a2cdc: 2d 28 40 00  	move	$5, $2
  1a2ce0: 0b 28 03 00  	movn	$5, $zero, $3
  1a2ce4: 1c 00 03 3c  	lui	$3, 0x1c
  1a2ce8: 06 10 a9 00  	srlv	$2, $9, $5
  1a2cec: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2cf0: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a2cf4: 21 10 43 00  	addu	$2, $2, $3
  1a2cf8: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2cfc: 21 20 85 00  	addu	$4, $4, $5
  1a2d00: 23 78 e4 00  	subu	$15, $7, $4
  1a2d04: 08 00 e0 51  	beqzl	$15, 0x1a2d28 <.text+0xa2d28>
  1a2d08: 02 3c 09 00  	srl	$7, $9, 0x10
  1a2d0c: 23 10 ef 00  	subu	$2, $7, $15
  1a2d10: 04 18 e8 01  	sllv	$3, $8, $15
  1a2d14: 06 10 4d 00  	srlv	$2, $13, $2
  1a2d18: 04 48 e9 01  	sllv	$9, $9, $15
  1a2d1c: 25 40 62 00  	or	$8, $3, $2
  1a2d20: 04 68 ed 01  	sllv	$13, $13, $15
  1a2d24: 02 3c 09 00  	srl	$7, $9, 0x10
  1a2d28: ff ff 2a 31  	andi	$10, $9, 0xffff
  1a2d2c: 1b 00 07 01  	divu	$zero, $8, $7
  1a2d30: 02 24 0d 00  	srl	$4, $13, 0x10
  1a2d34: 01 00 e0 50  	beqzl	$7, 0x1a2d3c <.text+0xa2d3c>
  1a2d38: cd 01 00 00  	break	0x0, 0x7
  1a2d3c: 12 10 00 00  	mflo	$2
  1a2d40: 10 18 00 00  	mfhi	$3
  1a2d44: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2d48: 25 18 64 00  	or	$3, $3, $4
  1a2d4c: 12 28 00 00  	mflo	$5
  1a2d50: 18 40 4a 00  	<unknown>
  1a2d54: 2b 10 68 00  	sltu	$2, $3, $8
  1a2d58: 0c 00 40 50  	beqzl	$2, 0x1a2d8c <.text+0xa2d8c>
  1a2d5c: 23 18 68 00  	subu	$3, $3, $8
  1a2d60: 21 18 69 00  	addu	$3, $3, $9
  1a2d64: 2b 10 69 00  	sltu	$2, $3, $9
  1a2d68: 07 00 40 14  	bnez	$2, 0x1a2d88 <.text+0xa2d88>
  1a2d6c: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a2d70: 2b 10 68 00  	sltu	$2, $3, $8
  1a2d74: 05 00 40 50  	beqzl	$2, 0x1a2d8c <.text+0xa2d8c>
  1a2d78: 23 18 68 00  	subu	$3, $3, $8
  1a2d7c: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a2d80: 21 18 69 00  	addu	$3, $3, $9
  1a2d84: 00 00 00 00  	nop
  1a2d88: 23 18 68 00  	subu	$3, $3, $8
  1a2d8c: 01 00 e0 50  	beqzl	$7, 0x1a2d94 <.text+0xa2d94>
  1a2d90: cd 01 00 00  	break	0x0, 0x7
  1a2d94: 1b 00 67 00  	divu	$zero, $3, $7
  1a2d98: ff ff a4 31  	andi	$4, $13, 0xffff
  1a2d9c: 12 10 00 00  	mflo	$2
  1a2da0: 10 18 00 00  	mfhi	$3
  1a2da4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2da8: 25 18 64 00  	or	$3, $3, $4
  1a2dac: 12 38 00 00  	mflo	$7
  1a2db0: 18 40 4a 00  	<unknown>
  1a2db4: 2b 10 68 00  	sltu	$2, $3, $8
  1a2db8: 0c 00 40 10  	beqz	$2, 0x1a2dec <.text+0xa2dec>
  1a2dbc: 00 14 05 00  	sll	$2, $5, 0x10
  1a2dc0: 21 18 69 00  	addu	$3, $3, $9
  1a2dc4: 2b 10 69 00  	sltu	$2, $3, $9
  1a2dc8: 07 00 40 14  	bnez	$2, 0x1a2de8 <.text+0xa2de8>
  1a2dcc: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2dd0: 2b 10 68 00  	sltu	$2, $3, $8
  1a2dd4: 05 00 40 10  	beqz	$2, 0x1a2dec <.text+0xa2dec>
  1a2dd8: 00 14 05 00  	sll	$2, $5, 0x10
  1a2ddc: 21 18 69 00  	addu	$3, $3, $9
  1a2de0: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2de4: 00 00 00 00  	nop
  1a2de8: 00 14 05 00  	sll	$2, $5, 0x10
  1a2dec: 23 68 68 00  	subu	$13, $3, $8
  1a2df0: 25 50 47 00  	or	$10, $2, $7
  1a2df4: 2d 80 00 00  	move	$16, $zero
  1a2df8: 0b 00 c0 10  	beqz	$6, 0x1a2e28 <.text+0xa2e28>
  1a2dfc: 06 10 ed 01  	srlv	$2, $13, $15
  1a2e00: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2e04: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2e08: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a2e0c: 24 60 83 01  	and	$12, $12, $3
  1a2e10: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2e14: ff ff 03 3c  	lui	$3, 0xffff
  1a2e18: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a2e1c: 25 60 82 01  	or	$12, $12, $2
  1a2e20: 24 60 83 01  	and	$12, $12, $3
  1a2e24: 00 00 cc fc  	sd	$12, 0x0($6)
  1a2e28: 3c 10 0a 00  	dsll32	$2, $10, 0x0
  1a2e2c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2e30: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2e34: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2e38: 24 70 c3 01  	and	$14, $14, $3
  1a2e3c: 25 70 c2 01  	or	$14, $14, $2
  1a2e40: 3c 18 10 00  	dsll32	$3, $16, 0x0
  1a2e44: ff ff 02 3c  	lui	$2, 0xffff
  1a2e48: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2e4c: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a2e50: 24 70 c2 01  	and	$14, $14, $2
  1a2e54: 25 70 c3 01  	or	$14, $14, $3
  1a2e58: 2d 10 c0 01  	move	$2, $14
  1a2e5c: 08 00 e0 03  	jr	$ra
  1a2e60: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a2e64: 00 00 00 00  	nop
  1a2e68: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a2e6c: ff ff 42 34  	ori	$2, $2, 0xffff
  1a2e70: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a2e74: 2b 10 49 00  	sltu	$2, $2, $9
  1a2e78: 2d 28 60 00  	move	$5, $3
  1a2e7c: 99 ff 00 10  	b	0x1a2ce4 <.text+0xa2ce4>
  1a2e80: 0b 28 82 00  	movn	$5, $4, $2
  1a2e84: 00 00 00 00  	nop
  1a2e88: 08 00 20 15  	bnez	$9, 0x1a2eac <.text+0xa2eac>
  1a2e8c: 2b 10 49 00  	sltu	$2, $2, $9
  1a2e90: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a2e94: 01 00 20 51  	beqzl	$9, 0x1a2e9c <.text+0xa2e9c>
  1a2e98: cd 01 00 00  	break	0x0, 0x7
  1a2e9c: 1b 00 47 00  	divu	$zero, $2, $7
  1a2ea0: 12 48 00 00  	mflo	$9
  1a2ea4: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2ea8: 2b 10 49 00  	sltu	$2, $2, $9
  1a2eac: 82 00 40 14  	bnez	$2, 0x1a30b8 <.text+0xa30b8>
  1a2eb0: ff 00 02 3c  	lui	$2, 0xff
  1a2eb4: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2eb8: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a2ebc: 2d 28 40 00  	move	$5, $2
  1a2ec0: 0b 28 03 00  	movn	$5, $zero, $3
  1a2ec4: 1c 00 03 3c  	lui	$3, 0x1c
  1a2ec8: 06 10 a9 00  	srlv	$2, $9, $5
  1a2ecc: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2ed0: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a2ed4: 21 10 43 00  	addu	$2, $2, $3
  1a2ed8: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2edc: 21 20 85 00  	addu	$4, $4, $5
  1a2ee0: 23 78 e4 00  	subu	$15, $7, $4
  1a2ee4: 38 00 e0 15  	bnez	$15, 0x1a2fc8 <.text+0xa2fc8>
  1a2ee8: 23 c0 ef 00  	subu	$24, $7, $15
  1a2eec: 23 40 09 01  	subu	$8, $8, $9
  1a2ef0: 01 00 10 24  	addiu	$16, $zero, 0x1
  1a2ef4: 02 54 09 00  	srl	$10, $9, 0x10
  1a2ef8: ff ff 39 31  	andi	$25, $9, 0xffff
  1a2efc: 1b 00 0a 01  	divu	$zero, $8, $10
  1a2f00: 02 2c 0d 00  	srl	$5, $13, 0x10
  1a2f04: 01 00 40 51  	beqzl	$10, 0x1a2f0c <.text+0xa2f0c>
  1a2f08: cd 01 00 00  	break	0x0, 0x7
  1a2f0c: 12 20 00 00  	mflo	$4
  1a2f10: 10 18 00 00  	mfhi	$3
  1a2f14: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2f18: 25 18 65 00  	or	$3, $3, $5
  1a2f1c: 12 58 00 00  	mflo	$11
  1a2f20: 18 38 99 00  	<unknown>
  1a2f24: 2b 10 67 00  	sltu	$2, $3, $7
  1a2f28: 0c 00 40 50  	beqzl	$2, 0x1a2f5c <.text+0xa2f5c>
  1a2f2c: 23 18 67 00  	subu	$3, $3, $7
  1a2f30: 21 18 69 00  	addu	$3, $3, $9
  1a2f34: 2b 10 69 00  	sltu	$2, $3, $9
  1a2f38: 07 00 40 14  	bnez	$2, 0x1a2f58 <.text+0xa2f58>
  1a2f3c: ff ff 8b 24  	addiu	$11, $4, -0x1 <.text+0xffffffffffefffff>
  1a2f40: 2b 10 67 00  	sltu	$2, $3, $7
  1a2f44: 05 00 40 50  	beqzl	$2, 0x1a2f5c <.text+0xa2f5c>
  1a2f48: 23 18 67 00  	subu	$3, $3, $7
  1a2f4c: ff ff 6b 25  	addiu	$11, $11, -0x1 <.text+0xffffffffffefffff>
  1a2f50: 21 18 69 00  	addu	$3, $3, $9
  1a2f54: 00 00 00 00  	nop
  1a2f58: 23 18 67 00  	subu	$3, $3, $7
  1a2f5c: ff ff a4 31  	andi	$4, $13, 0xffff
  1a2f60: 1b 00 6a 00  	divu	$zero, $3, $10
  1a2f64: 01 00 40 51  	beqzl	$10, 0x1a2f6c <.text+0xa2f6c>
  1a2f68: cd 01 00 00  	break	0x0, 0x7
  1a2f6c: 12 10 00 00  	mflo	$2
  1a2f70: 10 18 00 00  	mfhi	$3
  1a2f74: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2f78: 25 18 64 00  	or	$3, $3, $4
  1a2f7c: 12 40 00 00  	mflo	$8
  1a2f80: 18 38 59 00  	<unknown>
  1a2f84: 2b 10 67 00  	sltu	$2, $3, $7
  1a2f88: 0c 00 40 10  	beqz	$2, 0x1a2fbc <.text+0xa2fbc>
  1a2f8c: 00 14 0b 00  	sll	$2, $11, 0x10
  1a2f90: 21 18 69 00  	addu	$3, $3, $9
  1a2f94: 2b 10 69 00  	sltu	$2, $3, $9
  1a2f98: 07 00 40 14  	bnez	$2, 0x1a2fb8 <.text+0xa2fb8>
  1a2f9c: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2fa0: 2b 10 67 00  	sltu	$2, $3, $7
  1a2fa4: 05 00 40 10  	beqz	$2, 0x1a2fbc <.text+0xa2fbc>
  1a2fa8: 00 14 0b 00  	sll	$2, $11, 0x10
  1a2fac: 21 18 69 00  	addu	$3, $3, $9
  1a2fb0: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2fb4: 00 00 00 00  	nop
  1a2fb8: 00 14 0b 00  	sll	$2, $11, 0x10
  1a2fbc: 23 68 67 00  	subu	$13, $3, $7
  1a2fc0: 8d ff 00 10  	b	0x1a2df8 <.text+0xa2df8>
  1a2fc4: 25 50 48 00  	or	$10, $2, $8
  1a2fc8: 04 48 e9 01  	sllv	$9, $9, $15
  1a2fcc: 06 28 08 03  	srlv	$5, $8, $24
  1a2fd0: 02 54 09 00  	srl	$10, $9, 0x10
  1a2fd4: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2fd8: ff ff 39 31  	andi	$25, $9, 0xffff
  1a2fdc: 06 18 0d 03  	srlv	$3, $13, $24
  1a2fe0: 04 10 e8 01  	sllv	$2, $8, $15
  1a2fe4: 25 40 43 00  	or	$8, $2, $3
  1a2fe8: 01 00 40 51  	beqzl	$10, 0x1a2ff0 <.text+0xa2ff0>
  1a2fec: cd 01 00 00  	break	0x0, 0x7
  1a2ff0: 02 1c 08 00  	srl	$3, $8, 0x10
  1a2ff4: 2d 58 40 01  	move	$11, $10
  1a2ff8: 04 68 ed 01  	sllv	$13, $13, $15
  1a2ffc: 12 28 00 00  	mflo	$5
