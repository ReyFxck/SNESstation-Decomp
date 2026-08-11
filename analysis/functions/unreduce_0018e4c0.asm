  18e4c0: 50 ff bd 27  	addiu	$sp, $sp, -0xb0 <.text+0xffffffffffefff50>
  18e4c4: 42 00 02 3c  	lui	$2, 0x42
  18e4c8: 90 00 be ff  	sd	$fp, 0x90($sp)
  18e4cc: 2d f0 00 00  	move	$fp, $zero
  18e4d0: 70 00 b6 ff  	sd	$22, 0x70($sp)
  18e4d4: 2d b0 00 00  	move	$22, $zero
  18e4d8: 30 00 b2 ff  	sd	$18, 0x30($sp)
  18e4dc: 2d 90 00 00  	move	$18, $zero
  18e4e0: a0 00 bf ff  	sd	$ra, 0xa0($sp)
  18e4e4: 80 00 b7 ff  	sd	$23, 0x80($sp)
  18e4e8: 60 00 b5 ff  	sd	$21, 0x60($sp)
  18e4ec: 50 00 b4 ff  	sd	$20, 0x50($sp)
  18e4f0: 40 00 b3 ff  	sd	$19, 0x40($sp)
  18e4f4: 20 00 b1 ff  	sd	$17, 0x20($sp)
  18e4f8: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18e4fc: 00 00 a0 af  	sw	$zero, 0x0($sp)
  18e500: 54 48 42 8c  	lw	$2, 0x4854($2)
  18e504: 04 00 a0 af  	sw	$zero, 0x4($sp)
  18e508: e0 00 43 8c  	lw	$3, 0xe0($2)
  18e50c: 68 00 44 8c  	lw	$4, 0x68($2)
  18e510: 01 00 02 24  	addiu	$2, $zero, 0x1
  18e514: 88 00 77 dc  	ld	$23, 0x88($3)
  18e518: 08 00 a2 af  	sw	$2, 0x8($sp)
  18e51c: 42 00 02 3c  	lui	$2, 0x42
  18e520: 4b 3a 06 0c  	jal	0x18e92c <.text+0x8e92c>
  18e524: 30 6e 44 ac  	sw	$4, 0x6e30($2)
  18e528: 22 00 e0 1a  	blez	$23, 0x18e5b4 <.text+0x8e5b4>
  18e52c: 00 00 00 00  	nop
  18e530: 45 00 02 3c  	lui	$2, 0x45
  18e534: 10 02 42 24  	addiu	$2, $2, 0x210
  18e538: 21 80 c2 03  	addu	$16, $fp, $2
  18e53c: 00 00 02 92  	lbu	$2, 0x0($16)
  18e540: b6 00 40 14  	bnez	$2, 0x18e81c <.text+0x8e81c>
  18e544: 42 00 14 3c  	lui	$20, 0x42
  18e548: 42 00 10 3c  	lui	$16, 0x42
  18e54c: 2c 6e 02 8e  	lw	$2, 0x6e2c($16)
  18e550: 08 00 42 28  	slti	$2, $2, 0x8
  18e554: ad 00 40 14  	bnez	$2, 0x18e80c <.text+0x8e80c>
  18e558: 00 00 00 00  	nop
  18e55c: 42 00 04 3c  	lui	$4, 0x42
  18e560: 42 00 02 3c  	lui	$2, 0x42
  18e564: e8 43 46 94  	lhu	$6, 0x43e8($2)
  18e568: 20 6e 85 94  	lhu	$5, 0x6e20($4)
  18e56c: 20 6e 82 dc  	ld	$2, 0x6e20($4)
  18e570: 2c 6e 03 8e  	lw	$3, 0x6e2c($16)
  18e574: 24 98 a6 00  	and	$19, $5, $6
  18e578: 3a 12 02 00  	dsrl	$2, $2, 0x8
  18e57c: f8 ff 63 24  	addiu	$3, $3, -0x8 <.text+0xffffffffffeffff8>
  18e580: 20 6e 82 fc  	sd	$2, 0x6e20($4)
  18e584: 2c 6e 03 ae  	sw	$3, 0x6e2c($16)
  18e588: 01 00 02 24  	addiu	$2, $zero, 0x1
  18e58c: 80 00 c2 12  	beq	$22, $2, 0x18e790 <.text+0x8e790>
  18e590: 02 00 c2 2a  	slti	$2, $22, 0x2
  18e594: 6b 00 40 14  	bnez	$2, 0x18e744 <.text+0x8e744>
  18e598: 02 00 02 24  	addiu	$2, $zero, 0x2
  18e59c: 64 00 c2 12  	beq	$22, $2, 0x18e730 <.text+0x8e730>
  18e5a0: 03 00 02 24  	addiu	$2, $zero, 0x3
  18e5a4: 11 00 c2 12  	beq	$22, $2, 0x18e5ec <.text+0x8e5ec>
  18e5a8: 42 00 02 3c  	lui	$2, 0x42
  18e5ac: e0 ff e0 1e  	bgtz	$23, 0x18e530 <.text+0x8e530>
  18e5b0: 2d f0 60 02  	move	$fp, $19
  18e5b4: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18e5b8: 2d 20 40 02  	move	$4, $18
  18e5bc: 10 00 b0 df  	ld	$16, 0x10($sp)
  18e5c0: a0 00 bf df  	ld	$ra, 0xa0($sp)
  18e5c4: 90 00 be df  	ld	$fp, 0x90($sp)
  18e5c8: 80 00 b7 df  	ld	$23, 0x80($sp)
  18e5cc: 70 00 b6 df  	ld	$22, 0x70($sp)
  18e5d0: 60 00 b5 df  	ld	$21, 0x60($sp)
  18e5d4: 50 00 b4 df  	ld	$20, 0x50($sp)
  18e5d8: 40 00 b3 df  	ld	$19, 0x40($sp)
  18e5dc: 30 00 b2 df  	ld	$18, 0x30($sp)
  18e5e0: 20 00 b1 df  	ld	$17, 0x20($sp)
  18e5e4: 08 00 e0 03  	jr	$ra
  18e5e8: b0 00 bd 27  	addiu	$sp, $sp, 0xb0
  18e5ec: 42 00 03 3c  	lui	$3, 0x42
  18e5f0: 30 6e 44 8c  	lw	$4, 0x6e30($2)
  18e5f4: 20 44 63 24  	addiu	$3, $3, 0x4420
  18e5f8: 04 00 a2 8f  	lw	$2, 0x4($sp)
  18e5fc: 80 20 04 00  	sll	$4, $4, 0x2
  18e600: 00 00 a5 8f  	lw	$5, 0x0($sp)
  18e604: 03 00 54 24  	addiu	$20, $2, 0x3
  18e608: 21 18 83 00  	addu	$3, $4, $3
  18e60c: 42 00 02 3c  	lui	$2, 0x42
  18e610: 38 44 42 24  	addiu	$2, $2, 0x4438
  18e614: 21 20 82 00  	addu	$4, $4, $2
  18e618: 00 00 62 8c  	lw	$2, 0x0($3)
  18e61c: 00 00 84 8c  	lw	$4, 0x0($4)
  18e620: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18e624: 07 10 45 00  	srav	$2, $5, $2
  18e628: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  18e62c: 24 10 44 00  	and	$2, $2, $4
  18e630: 2f b8 e3 02  	dsubu	$23, $23, $3
  18e634: 00 12 02 00  	sll	$2, $2, 0x8
  18e638: 21 10 53 00  	addu	$2, $2, $19
  18e63c: 23 10 42 02  	subu	$2, $18, $2
  18e640: ff ff 51 24  	addiu	$17, $2, -0x1 <.text+0xffffffffffefffff>
  18e644: ff 3f 31 32  	andi	$17, $17, 0x3fff
  18e648: 2b 10 51 02  	sltu	$2, $18, $17
  18e64c: 36 00 40 10  	beqz	$2, 0x18e728 <.text+0x8e728>
  18e650: 00 40 02 24  	addiu	$2, $zero, 0x4000
  18e654: 23 80 51 00  	subu	$16, $2, $17
  18e658: 2b 10 90 02  	sltu	$2, $20, $16
  18e65c: 0b 80 82 02  	movn	$16, $20, $2
  18e660: 08 00 a2 8f  	lw	$2, 0x8($sp)
  18e664: 04 00 40 10  	beqz	$2, 0x18e678 <.text+0x8e678>
  18e668: 23 a0 90 02  	subu	$20, $20, $16
  18e66c: 2b 10 32 02  	sltu	$2, $17, $18
  18e670: 24 00 40 10  	beqz	$2, 0x18e704 <.text+0x8e704>
  18e674: 45 00 04 3c  	lui	$4, 0x45
  18e678: 23 10 51 02  	subu	$2, $18, $17
  18e67c: 2b 10 50 00  	sltu	$2, $2, $16
  18e680: 17 00 40 10  	beqz	$2, 0x18e6e0 <.text+0x8e6e0>
  18e684: 45 00 04 3c  	lui	$4, 0x45
  18e688: 45 00 02 3c  	lui	$2, 0x45
  18e68c: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  18e690: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18e694: 21 18 22 02  	addu	$3, $17, $2
  18e698: 01 00 31 26  	addiu	$17, $17, 0x1
  18e69c: 00 00 63 90  	lbu	$3, 0x0($3)
  18e6a0: 21 10 42 02  	addu	$2, $18, $2
  18e6a4: 01 00 52 26  	addiu	$18, $18, 0x1
  18e6a8: f7 ff 00 16  	bnez	$16, 0x18e688 <.text+0x8e688>
  18e6ac: 00 00 43 a0  	sb	$3, 0x0($2)
  18e6b0: 00 40 02 24  	addiu	$2, $zero, 0x4000
  18e6b4: 05 00 42 12  	beq	$18, $2, 0x18e6cc <.text+0x8e6cc>
  18e6b8: 00 40 04 24  	addiu	$4, $zero, 0x4000
  18e6bc: e2 ff 80 56  	bnezl	$20, 0x18e648 <.text+0x8e648>
  18e6c0: ff 3f 31 32  	andi	$17, $17, 0x3fff
  18e6c4: b9 ff 00 10  	b	0x18e5ac <.text+0x8e5ac>
  18e6c8: 2d b0 00 00  	move	$22, $zero
  18e6cc: 08 00 a0 af  	sw	$zero, 0x8($sp)
  18e6d0: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18e6d4: 2d 90 00 00  	move	$18, $zero
  18e6d8: f8 ff 00 10  	b	0x18e6bc <.text+0x8e6bc>
  18e6dc: 00 00 00 00  	nop
  18e6e0: 2d 30 00 02  	move	$6, $16
  18e6e4: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18e6e8: 21 28 24 02  	addu	$5, $17, $4
  18e6ec: 21 88 30 02  	addu	$17, $17, $16
  18e6f0: 21 20 44 02  	addu	$4, $18, $4
  18e6f4: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  18e6f8: 21 90 50 02  	addu	$18, $18, $16
  18e6fc: ed ff 00 10  	b	0x18e6b4 <.text+0x8e6b4>
  18e700: 00 40 02 24  	addiu	$2, $zero, 0x4000
  18e704: 2d 28 00 00  	move	$5, $zero
  18e708: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18e70c: 2d 30 00 02  	move	$6, $16
  18e710: 21 20 44 02  	addu	$4, $18, $4
  18e714: 21 88 30 02  	addu	$17, $17, $16
  18e718: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18e71c: 21 90 50 02  	addu	$18, $18, $16
  18e720: e4 ff 00 10  	b	0x18e6b4 <.text+0x8e6b4>
  18e724: 00 40 02 24  	addiu	$2, $zero, 0x4000
  18e728: cb ff 00 10  	b	0x18e658 <.text+0x8e658>
  18e72c: 23 80 52 00  	subu	$16, $2, $18
  18e730: 04 00 a5 8f  	lw	$5, 0x4($sp)
  18e734: 03 00 16 24  	addiu	$22, $zero, 0x3
  18e738: 21 28 b3 00  	addu	$5, $5, $19
  18e73c: 9b ff 00 10  	b	0x18e5ac <.text+0x8e5ac>
  18e740: 04 00 a5 af  	sw	$5, 0x4($sp)
  18e744: 99 ff c0 16  	bnez	$22, 0x18e5ac <.text+0x8e5ac>
  18e748: 90 00 02 24  	addiu	$2, $zero, 0x90
  18e74c: 0e 00 62 12  	beq	$19, $2, 0x18e788 <.text+0x8e788>
  18e750: 45 00 02 3c  	lui	$2, 0x45
  18e754: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18e758: 21 10 42 02  	addu	$2, $18, $2
  18e75c: 01 00 52 26  	addiu	$18, $18, 0x1
  18e760: 00 00 53 a0  	sb	$19, 0x0($2)
  18e764: 00 40 02 24  	addiu	$2, $zero, 0x4000
  18e768: 90 ff 42 16  	bne	$18, $2, 0x18e5ac <.text+0x8e5ac>
  18e76c: ff ff f7 66  	daddiu	$23, $23, -0x1 <.text+0xffffffffffefffff>
  18e770: 00 40 04 24  	addiu	$4, $zero, 0x4000
  18e774: 08 00 a0 af  	sw	$zero, 0x8($sp)
  18e778: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18e77c: 2d 90 00 00  	move	$18, $zero
  18e780: 8a ff 00 10  	b	0x18e5ac <.text+0x8e5ac>
  18e784: 00 00 00 00  	nop
  18e788: 88 ff 00 10  	b	0x18e5ac <.text+0x8e5ac>
  18e78c: 01 00 16 24  	addiu	$22, $zero, 0x1
  18e790: 10 00 60 12  	beqz	$19, 0x18e7d4 <.text+0x8e7d4>
  18e794: 45 00 02 3c  	lui	$2, 0x45
  18e798: 42 00 02 3c  	lui	$2, 0x42
  18e79c: 02 00 16 24  	addiu	$22, $zero, 0x2
  18e7a0: 30 6e 43 8c  	lw	$3, 0x6e30($2)
  18e7a4: 42 00 02 3c  	lui	$2, 0x42
  18e7a8: 08 44 42 24  	addiu	$2, $2, 0x4408
  18e7ac: 00 00 b3 af  	sw	$19, 0x0($sp)
  18e7b0: 80 18 03 00  	sll	$3, $3, 0x2
  18e7b4: 21 18 62 00  	addu	$3, $3, $2
  18e7b8: 00 00 62 8c  	lw	$2, 0x0($3)
  18e7bc: 24 18 62 02  	and	$3, $19, $2
  18e7c0: 26 10 62 00  	xor	$2, $3, $2
  18e7c4: 04 00 a3 af  	sw	$3, 0x4($sp)
  18e7c8: 03 00 03 24  	addiu	$3, $zero, 0x3
  18e7cc: 77 ff 00 10  	b	0x18e5ac <.text+0x8e5ac>
  18e7d0: 0b b0 62 00  	movn	$22, $3, $2
  18e7d4: 90 ff 03 24  	addiu	$3, $zero, -0x70 <.text+0xffffffffffefff90>
  18e7d8: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18e7dc: 21 10 42 02  	addu	$2, $18, $2
  18e7e0: 01 00 52 26  	addiu	$18, $18, 0x1
  18e7e4: 00 00 43 a0  	sb	$3, 0x0($2)
  18e7e8: 00 40 02 24  	addiu	$2, $zero, 0x4000
  18e7ec: b5 ff 42 16  	bne	$18, $2, 0x18e6c4 <.text+0x8e6c4>
  18e7f0: ff ff f7 66  	daddiu	$23, $23, -0x1 <.text+0xffffffffffefffff>
  18e7f4: 00 40 04 24  	addiu	$4, $zero, 0x4000
  18e7f8: 08 00 a0 af  	sw	$zero, 0x8($sp)
  18e7fc: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18e800: 2d 90 00 00  	move	$18, $zero
  18e804: 69 ff 00 10  	b	0x18e5ac <.text+0x8e5ac>
  18e808: 2d b0 00 00  	move	$22, $zero
  18e80c: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18e810: 00 00 00 00  	nop
  18e814: 52 ff 00 10  	b	0x18e560 <.text+0x8e560>
  18e818: 42 00 04 3c  	lui	$4, 0x42
  18e81c: 2c 6e 82 8e  	lw	$2, 0x6e2c($20)
  18e820: 3e 00 40 18  	blez	$2, 0x18e91c <.text+0x8e91c>
  18e824: 00 00 00 00  	nop
  18e828: 42 00 02 3c  	lui	$2, 0x42
  18e82c: 42 00 11 3c  	lui	$17, 0x42
  18e830: d8 43 55 24  	addiu	$21, $2, 0x43d8
  18e834: 20 6e 25 96  	lhu	$5, 0x6e20($17)
  18e838: 20 6e 22 de  	ld	$2, 0x6e20($17)
  18e83c: 2c 6e 83 8e  	lw	$3, 0x6e2c($20)
  18e840: 02 00 a4 96  	lhu	$4, 0x2($21)
  18e844: 7a 10 02 00  	dsrl	$2, $2, 0x1
  18e848: ff ff 66 24  	addiu	$6, $3, -0x1 <.text+0xffffffffffefffff>
  18e84c: 20 6e 22 fe  	sd	$2, 0x6e20($17)
  18e850: 24 98 a4 00  	and	$19, $5, $4
  18e854: 12 00 60 12  	beqz	$19, 0x18e8a0 <.text+0x8e8a0>
  18e858: 2c 6e 86 ae  	sw	$6, 0x6e2c($20)
  18e85c: 08 00 c2 28  	slti	$2, $6, 0x8
  18e860: 0b 00 40 14  	bnez	$2, 0x18e890 <.text+0x8e890>
  18e864: 00 00 00 00  	nop
  18e868: 20 6e 24 96  	lhu	$4, 0x6e20($17)
  18e86c: 20 6e 22 de  	ld	$2, 0x6e20($17)
  18e870: 2c 6e 83 8e  	lw	$3, 0x6e2c($20)
  18e874: 10 00 a5 96  	lhu	$5, 0x10($21)
  18e878: 3a 12 02 00  	dsrl	$2, $2, 0x8
  18e87c: f8 ff 63 24  	addiu	$3, $3, -0x8 <.text+0xffffffffffeffff8>
  18e880: 20 6e 22 fe  	sd	$2, 0x6e20($17)
  18e884: 24 98 85 00  	and	$19, $4, $5
  18e888: 3f ff 00 10  	b	0x18e588 <.text+0x8e588>
  18e88c: 2c 6e 83 ae  	sw	$3, 0x6e2c($20)
  18e890: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18e894: 00 00 00 00  	nop
  18e898: f4 ff 00 10  	b	0x18e86c <.text+0x8e86c>
  18e89c: 20 6e 24 96  	lhu	$4, 0x6e20($17)
  18e8a0: 00 00 02 92  	lbu	$2, 0x0($16)
  18e8a4: 42 00 03 3c  	lui	$3, 0x42
  18e8a8: 50 44 63 24  	addiu	$3, $3, 0x4450
  18e8ac: 80 10 02 00  	sll	$2, $2, 0x2
  18e8b0: 21 10 43 00  	addu	$2, $2, $3
  18e8b4: 00 00 50 8c  	lw	$16, 0x0($2)
  18e8b8: 2a 10 d0 00  	slt	$2, $6, $16
  18e8bc: 13 00 40 14  	bnez	$2, 0x18e90c <.text+0x8e90c>
  18e8c0: 00 00 00 00  	nop
  18e8c4: 40 10 10 00  	sll	$2, $16, 0x1
  18e8c8: 20 6e 25 de  	ld	$5, 0x6e20($17)
  18e8cc: 21 10 55 00  	addu	$2, $2, $21
  18e8d0: 2c 6e 84 8e  	lw	$4, 0x6e2c($20)
  18e8d4: 00 00 47 94  	lhu	$7, 0x0($2)
  18e8d8: 16 28 05 02  	dsrlv	$5, $5, $16
  18e8dc: 42 00 02 3c  	lui	$2, 0x42
  18e8e0: 20 6e 23 96  	lhu	$3, 0x6e20($17)
  18e8e4: 00 44 46 8c  	lw	$6, 0x4400($2)
  18e8e8: 23 20 90 00  	subu	$4, $4, $16
  18e8ec: 80 11 1e 00  	sll	$2, $fp, 0x6
  18e8f0: 20 6e 25 fe  	sd	$5, 0x6e20($17)
  18e8f4: 2c 6e 84 ae  	sw	$4, 0x6e2c($20)
  18e8f8: 24 18 67 00  	and	$3, $3, $7
  18e8fc: 21 10 46 00  	addu	$2, $2, $6
  18e900: 21 10 43 00  	addu	$2, $2, $3
  18e904: 20 ff 00 10  	b	0x18e588 <.text+0x8e588>
  18e908: 00 00 53 90  	lbu	$19, 0x0($2)
  18e90c: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18e910: 00 00 00 00  	nop
  18e914: ec ff 00 10  	b	0x18e8c8 <.text+0x8e8c8>
  18e918: 40 10 10 00  	sll	$2, $16, 0x1
  18e91c: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18e920: 00 00 00 00  	nop
  18e924: c1 ff 00 10  	b	0x18e82c <.text+0x8e82c>
  18e928: 42 00 02 3c  	lui	$2, 0x42
  18e92c: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  18e930: 30 00 bf ff  	sd	$ra, 0x30($sp)
  18e934: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18e938: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18e93c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18e940: ff 00 12 24  	addiu	$18, $zero, 0xff
  18e944: 42 00 10 3c  	lui	$16, 0x42
  18e948: 2c 6e 02 8e  	lw	$2, 0x6e2c($16)
  18e94c: 06 00 42 28  	slti	$2, $2, 0x6
  18e950: 3d 00 40 14  	bnez	$2, 0x18ea48 <.text+0x8ea48>
  18e954: 00 00 00 00  	nop
  18e958: 42 00 06 3c  	lui	$6, 0x42
  18e95c: 42 00 02 3c  	lui	$2, 0x42
  18e960: 2c 6e 03 8e  	lw	$3, 0x6e2c($16)
  18e964: e4 43 47 90  	lbu	$7, 0x43e4($2)
  18e968: 45 00 02 3c  	lui	$2, 0x45
  18e96c: 20 6e c5 90  	lbu	$5, 0x6e20($6)
  18e970: 10 02 42 24  	addiu	$2, $2, 0x210
  18e974: 20 6e c4 dc  	ld	$4, 0x6e20($6)
  18e978: 21 10 42 02  	addu	$2, $18, $2
  18e97c: 24 28 a7 00  	and	$5, $5, $7
  18e980: fa ff 63 24  	addiu	$3, $3, -0x6 <.text+0xffffffffffeffffa>
  18e984: ba 21 04 00  	dsrl	$4, $4, 0x6
  18e988: 2c 6e 03 ae  	sw	$3, 0x6e2c($16)
  18e98c: 00 00 45 a0  	sb	$5, 0x0($2)
  18e990: 2d 80 00 00  	move	$16, $zero
  18e994: 1f 00 a0 10  	beqz	$5, 0x18ea14 <.text+0x8ea14>
  18e998: 20 6e c4 fc  	sd	$4, 0x6e20($6)
  18e99c: 42 00 11 3c  	lui	$17, 0x42
  18e9a0: 2c 6e 22 8e  	lw	$2, 0x6e2c($17)
  18e9a4: 08 00 42 28  	slti	$2, $2, 0x8
  18e9a8: 23 00 40 14  	bnez	$2, 0x18ea38 <.text+0x8ea38>
  18e9ac: 00 00 00 00  	nop
  18e9b0: 42 00 02 3c  	lui	$2, 0x42
  18e9b4: 42 00 06 3c  	lui	$6, 0x42
  18e9b8: 00 44 44 8c  	lw	$4, 0x4400($2)
  18e9bc: 42 00 02 3c  	lui	$2, 0x42
  18e9c0: e8 43 45 90  	lbu	$5, 0x43e8($2)
  18e9c4: 80 11 12 00  	sll	$2, $18, 0x6
  18e9c8: 20 6e c3 90  	lbu	$3, 0x6e20($6)
  18e9cc: 21 10 44 00  	addu	$2, $2, $4
  18e9d0: 21 10 50 00  	addu	$2, $2, $16
  18e9d4: 01 00 10 26  	addiu	$16, $16, 0x1
  18e9d8: 24 18 65 00  	and	$3, $3, $5
  18e9dc: ff 00 04 32  	andi	$4, $16, 0xff
  18e9e0: 00 00 43 a0  	sb	$3, 0x0($2)
  18e9e4: 45 00 02 3c  	lui	$2, 0x45
  18e9e8: 10 02 42 24  	addiu	$2, $2, 0x210
  18e9ec: 21 10 42 02  	addu	$2, $18, $2
  18e9f0: 2c 6e 23 8e  	lw	$3, 0x6e2c($17)
  18e9f4: 00 00 45 90  	lbu	$5, 0x0($2)
  18e9f8: 20 6e c2 dc  	ld	$2, 0x6e20($6)
  18e9fc: f8 ff 63 24  	addiu	$3, $3, -0x8 <.text+0xffffffffffeffff8>
  18ea00: 2b 20 85 00  	sltu	$4, $4, $5
  18ea04: 2c 6e 23 ae  	sw	$3, 0x6e2c($17)
  18ea08: 3a 12 02 00  	dsrl	$2, $2, 0x8
  18ea0c: e3 ff 80 14  	bnez	$4, 0x18e99c <.text+0x8e99c>
  18ea10: 20 6e c2 fc  	sd	$2, 0x6e20($6)
  18ea14: ff ff 52 26  	addiu	$18, $18, -0x1 <.text+0xffffffffffefffff>
  18ea18: cb ff 41 06  	bgez	$18, 0x18e948 <.text+0x8e948>
  18ea1c: 42 00 10 3c  	lui	$16, 0x42
  18ea20: 30 00 bf df  	ld	$ra, 0x30($sp)
  18ea24: 20 00 b2 df  	ld	$18, 0x20($sp)
  18ea28: 10 00 b1 df  	ld	$17, 0x10($sp)
  18ea2c: 00 00 b0 df  	ld	$16, 0x0($sp)
  18ea30: 08 00 e0 03  	jr	$ra
  18ea34: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  18ea38: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18ea3c: 00 00 00 00  	nop
  18ea40: dc ff 00 10  	b	0x18e9b4 <.text+0x8e9b4>
  18ea44: 42 00 02 3c  	lui	$2, 0x42
  18ea48: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18ea4c: 00 00 00 00  	nop
  18ea50: c2 ff 00 10  	b	0x18e95c <.text+0x8e95c>
  18ea54: 42 00 06 3c  	lui	$6, 0x42
  18ea58: 09 00 03 24  	addiu	$3, $zero, 0x9
  18ea5c: 42 00 02 3c  	lui	$2, 0x42
  18ea60: 3c 6e 43 ac  	sw	$3, 0x6e3c($2)
