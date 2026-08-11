
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  19e474: 0d 00 80 10  	beqz	$4, 0x19e4ac <.text+0x9e4ac>
  19e478: 00 00 00 00  	nop
  19e47c: 0c 00 86 8c  	lw	$6, 0xc($4)
  19e480: 08 00 c0 10  	beqz	$6, 0x19e4a4 <.text+0x9e4a4>
  19e484: 00 00 00 00  	nop
  19e488: 00 00 82 8c  	lw	$2, 0x0($4)
  19e48c: 04 00 83 8c  	lw	$3, 0x4($4)
  19e490: 21 10 43 00  	addu	$2, $2, $3
  19e494: 23 10 c2 00  	subu	$2, $6, $2
  19e498: 2b 10 45 00  	sltu	$2, $2, $5
  19e49c: 03 00 40 10  	beqz	$2, 0x19e4ac <.text+0x9e4ac>
  19e4a0: 00 00 00 00  	nop
  19e4a4: f5 ff c0 14  	bnez	$6, 0x19e47c <.text+0x9e47c>
  19e4a8: 2d 20 c0 00  	move	$4, $6
  19e4ac: 08 00 e0 03  	jr	$ra
  19e4b0: 2d 10 80 00  	move	$2, $4
  19e4b4: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19e4b8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19e4bc: 10 00 90 24  	addiu	$16, $4, 0x10
  19e4c0: 0f 00 02 32  	andi	$2, $16, 0xf
  19e4c4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19e4c8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19e4cc: 04 00 40 10  	beqz	$2, 0x19e4e0 <.text+0x9e4e0>
  19e4d0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19e4d4: 1f 00 83 24  	addiu	$3, $4, 0x1f
  19e4d8: f0 ff 02 24  	addiu	$2, $zero, -0x10 <.text+0xffffffffffeffff0>
  19e4dc: 24 80 62 00  	and	$16, $3, $2
  19e4e0: 42 00 12 3c  	lui	$18, 0x42
  19e4e4: 78 5a 44 8e  	lw	$4, 0x5a78($18)
  19e4e8: 25 00 80 14  	bnez	$4, 0x19e580 <.text+0x9e580>
  19e4ec: 42 00 02 3c  	lui	$2, 0x42
  19e4f0: 42 00 11 3c  	lui	$17, 0x42
  19e4f4: 74 5a 22 8e  	lw	$2, 0x5a74($17)
  19e4f8: 18 00 40 10  	beqz	$2, 0x19e55c <.text+0x9e55c>
  19e4fc: 00 00 00 00  	nop
  19e500: 1e 7c 06 0c  	jal	0x19f078 <.text+0x9f078>
  19e504: 2d 20 00 02  	move	$4, $16
  19e508: 2d 38 40 00  	move	$7, $2
  19e50c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19e510: 0b 00 e2 10  	beq	$7, $2, 0x19e540 <.text+0x9e540>
  19e514: 2d 28 00 00  	move	$5, $zero
  19e518: 10 00 e8 24  	addiu	$8, $7, 0x10
  19e51c: f0 ff 02 26  	addiu	$2, $16, -0x10 <.text+0xffffffffffeffff0>
  19e520: 04 00 e2 ac  	sw	$2, 0x4($7)
  19e524: 2d 28 00 01  	move	$5, $8
  19e528: 42 00 02 3c  	lui	$2, 0x42
  19e52c: 78 5a 47 ae  	sw	$7, 0x5a78($18)
  19e530: 00 00 e8 ac  	sw	$8, 0x0($7)
  19e534: 08 00 e0 ac  	sw	$zero, 0x8($7)
  19e538: 0c 00 e0 ac  	sw	$zero, 0xc($7)
  19e53c: 7c 5a 47 ac  	sw	$7, 0x5a7c($2)
  19e540: 30 00 bf df  	ld	$ra, 0x30($sp)
  19e544: 2d 10 a0 00  	move	$2, $5
  19e548: 20 00 b2 df  	ld	$18, 0x20($sp)
  19e54c: 10 00 b1 df  	ld	$17, 0x10($sp)
  19e550: 00 00 b0 df  	ld	$16, 0x0($sp)
  19e554: 08 00 e0 03  	jr	$ra
  19e558: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19e55c: 1e 7c 06 0c  	jal	0x19f078 <.text+0x9f078>
  19e560: 2d 20 00 00  	move	$4, $zero
  19e564: 0f 00 42 30  	andi	$2, $2, 0xf
  19e568: 1e 7c 06 0c  	jal	0x19f078 <.text+0x9f078>
  19e56c: 2d 20 40 00  	move	$4, $2
  19e570: 1e 7c 06 0c  	jal	0x19f078 <.text+0x9f078>
  19e574: 2d 20 00 00  	move	$4, $zero
  19e578: e1 ff 00 10  	b	0x19e500 <.text+0x9e500>
  19e57c: 74 5a 22 ae  	sw	$2, 0x5a74($17)
  19e580: 74 5a 43 8c  	lw	$3, 0x5a74($2)
  19e584: 21 10 70 00  	addu	$2, $3, $16
  19e588: 2b 10 44 00  	sltu	$2, $2, $4
  19e58c: 0b 00 40 10  	beqz	$2, 0x19e5bc <.text+0x9e5bc>
  19e590: 10 00 68 24  	addiu	$8, $3, 0x10
  19e594: f0 ff 02 26  	addiu	$2, $16, -0x10 <.text+0xffffffffffeffff0>
  19e598: 2d 30 60 00  	move	$6, $3
  19e59c: 2d 28 00 01  	move	$5, $8
  19e5a0: 08 00 60 ac  	sw	$zero, 0x8($3)
  19e5a4: 0c 00 64 ac  	sw	$4, 0xc($3)
  19e5a8: 08 00 83 ac  	sw	$3, 0x8($4)
  19e5ac: 04 00 62 ac  	sw	$2, 0x4($3)
  19e5b0: 78 5a 43 ae  	sw	$3, 0x5a78($18)
  19e5b4: e2 ff 00 10  	b	0x19e540 <.text+0x9e540>
  19e5b8: 00 00 c8 ac  	sw	$8, 0x0($6)
  19e5bc: 1d 79 06 0c  	jal	0x19e474 <.text+0x9e474>
  19e5c0: 2d 28 00 02  	move	$5, $16
  19e5c4: 0e 00 40 10  	beqz	$2, 0x19e600 <.text+0x9e600>
  19e5c8: 2d 38 40 00  	move	$7, $2
  19e5cc: 00 00 43 8c  	lw	$3, 0x0($2)
  19e5d0: f0 ff 05 26  	addiu	$5, $16, -0x10 <.text+0xffffffffffeffff0>
  19e5d4: 04 00 42 8c  	lw	$2, 0x4($2)
  19e5d8: 0c 00 e4 8c  	lw	$4, 0xc($7)
  19e5dc: 21 30 62 00  	addu	$6, $3, $2
  19e5e0: 10 00 c8 24  	addiu	$8, $6, 0x10
  19e5e4: 04 00 c5 ac  	sw	$5, 0x4($6)
  19e5e8: 2d 28 00 01  	move	$5, $8
  19e5ec: 08 00 c7 ac  	sw	$7, 0x8($6)
  19e5f0: 0c 00 c4 ac  	sw	$4, 0xc($6)
  19e5f4: 08 00 86 ac  	sw	$6, 0x8($4)
  19e5f8: ee ff 00 10  	b	0x19e5b4 <.text+0x9e5b4>
  19e5fc: 0c 00 e6 ac  	sw	$6, 0xc($7)
  19e600: 1e 7c 06 0c  	jal	0x19f078 <.text+0x9f078>
  19e604: 2d 20 00 02  	move	$4, $16
  19e608: 2d 38 40 00  	move	$7, $2
  19e60c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19e610: cb ff e2 10  	beq	$7, $2, 0x19e540 <.text+0x9e540>
  19e614: 2d 28 00 00  	move	$5, $zero
  19e618: 42 00 04 3c  	lui	$4, 0x42
  19e61c: f0 ff 02 26  	addiu	$2, $16, -0x10 <.text+0xffffffffffeffff0>
  19e620: 7c 5a 83 8c  	lw	$3, 0x5a7c($4)
  19e624: 10 00 e8 24  	addiu	$8, $7, 0x10
  19e628: 0c 00 e0 ac  	sw	$zero, 0xc($7)
  19e62c: 2d 30 e0 00  	move	$6, $7
  19e630: 0c 00 67 ac  	sw	$7, 0xc($3)
  19e634: 2d 28 00 01  	move	$5, $8
  19e638: 08 00 e3 ac  	sw	$3, 0x8($7)
  19e63c: 04 00 e2 ac  	sw	$2, 0x4($7)
  19e640: dc ff 00 10  	b	0x19e5b4 <.text+0x9e5b4>
  19e644: 7c 5a 87 ac  	sw	$7, 0x5a7c($4)
  19e648: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19e64c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19e650: 18 88 85 00  	<unknown>
  19e654: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19e658: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19e65c: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  19e660: 2d 20 20 02  	move	$4, $17
  19e664: 2d 80 40 00  	move	$16, $2
  19e668: 06 00 00 12  	beqz	$16, 0x19e684 <.text+0x9e684>
  19e66c: 2d 10 00 00  	move	$2, $zero
  19e670: 2d 30 20 02  	move	$6, $17
  19e674: 2d 20 00 02  	move	$4, $16
  19e678: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  19e67c: 2d 28 00 00  	move	$5, $zero
  19e680: 2d 10 00 02  	move	$2, $16
  19e684: 20 00 bf df  	ld	$ra, 0x20($sp)
  19e688: 10 00 b1 df  	ld	$17, 0x10($sp)
  19e68c: 00 00 b0 df  	ld	$16, 0x0($sp)
  19e690: 08 00 e0 03  	jr	$ra
  19e694: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19e698: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19e69c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  19e6a0: 2d 80 80 00  	move	$16, $4
  19e6a4: 11 00 02 2e  	sltiu	$2, $16, 0x11
  19e6a8: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19e6ac: 09 00 40 10  	beqz	$2, 0x19e6d4 <.text+0x9e6d4>
  19e6b0: 2d 20 a0 00  	move	$4, $5
  19e6b4: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  19e6b8: 00 00 00 00  	nop
  19e6bc: 2d 30 40 00  	move	$6, $2
  19e6c0: 20 00 bf df  	ld	$ra, 0x20($sp)
  19e6c4: 2d 10 c0 00  	move	$2, $6
  19e6c8: 10 00 b0 df  	ld	$16, 0x10($sp)
  19e6cc: 08 00 e0 03  	jr	$ra
  19e6d0: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19e6d4: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  19e6d8: 21 20 b0 00  	addu	$4, $5, $16
  19e6dc: 2d 30 00 00  	move	$6, $zero
  19e6e0: 2d 20 40 00  	move	$4, $2
  19e6e4: f6 ff 40 10  	beqz	$2, 0x19e6c0 <.text+0x9e6c0>
  19e6e8: 2d 40 40 00  	move	$8, $2
  19e6ec: f0 ff 45 24  	addiu	$5, $2, -0x10 <.text+0xffffffffffeffff0>
  19e6f0: ff ff 07 26  	addiu	$7, $16, -0x1 <.text+0xffffffffffefffff>
  19e6f4: 04 00 a2 8c  	lw	$2, 0x4($5)
  19e6f8: 24 18 87 00  	and	$3, $4, $7
  19e6fc: 2d 30 80 00  	move	$6, $4
  19e700: 23 10 50 00  	subu	$2, $2, $16
  19e704: ee ff 60 10  	beqz	$3, 0x19e6c0 <.text+0x9e6c0>
  19e708: 04 00 a2 ac  	sw	$2, 0x4($5)
  19e70c: 21 10 90 00  	addu	$2, $4, $16
  19e710: f7 ff 03 69  	ldl	$3, -0x9($8)
  19e714: f0 ff 03 6d  	ldr	$3, -0x10($8)
  19e718: ff ff 04 69  	ldl	$4, -0x1($8)
  19e71c: f8 ff 04 6d  	ldr	$4, -0x8($8)
  19e720: 07 00 a3 b3  	sdl	$3, 0x7($sp)
  19e724: 00 00 a3 b7  	sdr	$3, 0x0($sp)
  19e728: 0f 00 a4 b3  	sdl	$4, 0xf($sp)
  19e72c: 08 00 a4 b7  	sdr	$4, 0x8($sp)
  19e730: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  19e734: 27 18 07 00  	nor	$3, $zero, $7
  19e738: 24 20 43 00  	and	$4, $2, $3
  19e73c: 07 00 a2 6b  	ldl	$2, 0x7($sp)
  19e740: 00 00 a2 6f  	ldr	$2, 0x0($sp)
  19e744: 0f 00 a3 6b  	ldl	$3, 0xf($sp)
  19e748: 08 00 a3 6f  	ldr	$3, 0x8($sp)
  19e74c: f7 ff 82 b0  	sdl	$2, -0x9($4)
  19e750: f0 ff 82 b4  	sdr	$2, -0x10($4)
  19e754: ff ff 83 b0  	sdl	$3, -0x1($4)
  19e758: f8 ff 83 b4  	sdr	$3, -0x8($4)
  19e75c: f0 ff 85 24  	addiu	$5, $4, -0x10 <.text+0xffffffffffeffff0>
  19e760: 08 00 a2 8c  	lw	$2, 0x8($5)
  19e764: 01 00 40 54  	bnezl	$2, 0x19e76c <.text+0x9e76c>
  19e768: 0c 00 45 ac  	sw	$5, 0xc($2)
  19e76c: 0c 00 a2 8c  	lw	$2, 0xc($5)
  19e770: 01 00 40 54  	bnezl	$2, 0x19e778 <.text+0x9e778>
  19e774: 08 00 45 ac  	sw	$5, 0x8($2)
  19e778: f0 ff 84 ac  	sw	$4, -0x10($4)
  19e77c: d0 ff 00 10  	b	0x19e6c0 <.text+0x9e6c0>
  19e780: 2d 30 80 00  	move	$6, $4
  19e784: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19e788: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19e78c: 16 00 80 10  	beqz	$4, 0x19e7e8 <.text+0x9e7e8>
  19e790: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19e794: 42 00 07 3c  	lui	$7, 0x42
  19e798: 78 5a e5 8c  	lw	$5, 0x5a78($7)
  19e79c: 13 00 a0 50  	beqzl	$5, 0x19e7ec <.text+0x9e7ec>
  19e7a0: 10 00 bf df  	ld	$ra, 0x10($sp)
  19e7a4: 00 00 a2 8c  	lw	$2, 0x0($5)
  19e7a8: 1f 00 82 10  	beq	$4, $2, 0x19e828 <.text+0x9e828>
  19e7ac: 2d 18 a0 00  	move	$3, $5
  19e7b0: 0c 00 70 8c  	lw	$16, 0xc($3)
  19e7b4: 0c 00 00 12  	beqz	$16, 0x19e7e8 <.text+0x9e7e8>
  19e7b8: 2d 18 00 02  	move	$3, $16
  19e7bc: 00 00 02 8e  	lw	$2, 0x0($16)
  19e7c0: fc ff 82 54  	bnel	$4, $2, 0x19e7b4 <.text+0x9e7b4>
  19e7c4: 0c 00 70 8c  	lw	$16, 0xc($3)
  19e7c8: 0c 00 03 8e  	lw	$3, 0xc($16)
  19e7cc: 0a 00 60 50  	beqzl	$3, 0x19e7f8 <.text+0x9e7f8>
  19e7d0: 08 00 03 8e  	lw	$3, 0x8($16)
  19e7d4: 08 00 02 8e  	lw	$2, 0x8($16)
  19e7d8: 08 00 62 ac  	sw	$2, 0x8($3)
  19e7dc: 0c 00 03 8e  	lw	$3, 0xc($16)
  19e7e0: 08 00 02 8e  	lw	$2, 0x8($16)
  19e7e4: 0c 00 43 ac  	sw	$3, 0xc($2)
  19e7e8: 10 00 bf df  	ld	$ra, 0x10($sp)
  19e7ec: 00 00 b0 df  	ld	$16, 0x0($sp)
  19e7f0: 08 00 e0 03  	jr	$ra
  19e7f4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19e7f8: 2d 20 00 00  	move	$4, $zero
  19e7fc: 42 00 02 3c  	lui	$2, 0x42
  19e800: 1e 7c 06 0c  	jal	0x19f078 <.text+0x9f078>
  19e804: 7c 5a 43 ac  	sw	$3, 0x5a7c($2)
  19e808: 08 00 03 8e  	lw	$3, 0x8($16)
  19e80c: 04 00 65 8c  	lw	$5, 0x4($3)
  19e810: 00 00 64 8c  	lw	$4, 0x0($3)
  19e814: 21 20 85 00  	addu	$4, $4, $5
  19e818: 1e 7c 06 0c  	jal	0x19f078 <.text+0x9f078>
  19e81c: 23 20 82 00  	subu	$4, $4, $2
  19e820: ef ff 00 10  	b	0x19e7e0 <.text+0x9e7e0>
  19e824: 0c 00 03 8e  	lw	$3, 0xc($16)
  19e828: 0c 00 a6 8c  	lw	$6, 0xc($5)
  19e82c: 23 18 85 00  	subu	$3, $4, $5
  19e830: 04 00 a2 8c  	lw	$2, 0x4($5)
  19e834: 78 5a e6 ac  	sw	$6, 0x5a78($7)
  19e838: 03 00 c0 10  	beqz	$6, 0x19e848 <.text+0x9e848>
  19e83c: 21 10 43 00  	addu	$2, $2, $3
  19e840: e9 ff 00 10  	b	0x19e7e8 <.text+0x9e7e8>
  19e844: 08 00 c0 ac  	sw	$zero, 0x8($6)
  19e848: 23 20 02 00  	negu	$4, $2
  19e84c: 42 00 02 3c  	lui	$2, 0x42
  19e850: 1e 7c 06 0c  	jal	0x19f078 <.text+0x9f078>
  19e854: 7c 5a 40 ac  	sw	$zero, 0x5a7c($2)
  19e858: e4 ff 00 10  	b	0x19e7ec <.text+0x9e7ec>
  19e85c: 10 00 bf df  	ld	$ra, 0x10($sp)
