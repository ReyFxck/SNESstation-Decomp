  18ea64: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  18ea68: ff 01 03 24  	addiu	$3, $zero, 0x1ff
  18ea6c: 42 00 02 3c  	lui	$2, 0x42
  18ea70: 40 6e 43 ac  	sw	$3, 0x6e40($2)
  18ea74: 00 20 06 24  	addiu	$6, $zero, 0x2000
  18ea78: 00 20 03 24  	addiu	$3, $zero, 0x2000
  18ea7c: 42 00 02 3c  	lui	$2, 0x42
  18ea80: 34 6e 43 ac  	sw	$3, 0x6e34($2)
  18ea84: 42 00 02 3c  	lui	$2, 0x42
  18ea88: 01 01 03 24  	addiu	$3, $zero, 0x101
  18ea8c: 50 00 bf ff  	sd	$ra, 0x50($sp)
  18ea90: 40 00 b4 ff  	sd	$20, 0x40($sp)
  18ea94: 30 00 b3 ff  	sd	$19, 0x30($sp)
  18ea98: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18ea9c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18eaa0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18eaa4: 38 6e 43 ac  	sw	$3, 0x6e38($2)
  18eaa8: 45 00 02 3c  	lui	$2, 0x45
  18eaac: 40 18 06 00  	sll	$3, $6, 0x1
  18eab0: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18eab4: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  18eab8: 21 18 62 00  	addu	$3, $3, $2
  18eabc: 00 01 c4 28  	slti	$4, $6, 0x100
  18eac0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18eac4: f8 ff 80 10  	beqz	$4, 0x18eaa8 <.text+0x8eaa8>
  18eac8: 00 00 62 a4  	sh	$2, 0x0($3)
  18eacc: ff 00 06 24  	addiu	$6, $zero, 0xff
  18ead0: 45 00 03 3c  	lui	$3, 0x45
  18ead4: 40 10 06 00  	sll	$2, $6, 0x1
  18ead8: 00 82 72 24  	addiu	$18, $3, -0x7e00 <.text+0xffffffffffef8200>
  18eadc: 21 18 d2 00  	addu	$3, $6, $18
  18eae0: 21 10 52 00  	addu	$2, $2, $18
  18eae4: 04 40 66 a0  	sb	$6, 0x4004($3)
  18eae8: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  18eaec: f8 ff c1 04  	bgez	$6, 0x18ead0 <.text+0x8ead0>
  18eaf0: 00 00 40 a4  	sh	$zero, 0x0($2)
  18eaf4: 42 00 10 3c  	lui	$16, 0x42
  18eaf8: 42 00 11 3c  	lui	$17, 0x42
  18eafc: 3c 6e 03 8e  	lw	$3, 0x6e3c($16)
  18eb00: 2c 6e 22 8e  	lw	$2, 0x6e2c($17)
  18eb04: 2a 10 43 00  	slt	$2, $2, $3
  18eb08: e6 00 40 14  	bnez	$2, 0x18eea4 <.text+0x8eea4>
  18eb0c: 00 00 00 00  	nop
  18eb10: 3c 6e 05 8e  	lw	$5, 0x6e3c($16)
  18eb14: 42 00 02 3c  	lui	$2, 0x42
  18eb18: 42 00 06 3c  	lui	$6, 0x42
  18eb1c: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18eb20: 40 18 05 00  	sll	$3, $5, 0x1
  18eb24: 20 6e c4 dc  	ld	$4, 0x6e20($6)
  18eb28: 21 18 62 00  	addu	$3, $3, $2
  18eb2c: 42 00 10 3c  	lui	$16, 0x42
  18eb30: 2c 6e 22 8e  	lw	$2, 0x6e2c($17)
  18eb34: 16 20 a4 00  	dsrlv	$4, $4, $5
  18eb38: 00 00 68 94  	lhu	$8, 0x0($3)
  18eb3c: 2a 6e 07 82  	lb	$7, 0x6e2a($16)
  18eb40: 23 10 45 00  	subu	$2, $2, $5
  18eb44: 20 6e c3 94  	lhu	$3, 0x6e20($6)
  18eb48: 2c 6e 22 ae  	sw	$2, 0x6e2c($17)
  18eb4c: 20 6e c4 fc  	sd	$4, 0x6e20($6)
  18eb50: 09 00 e0 10  	beqz	$7, 0x18eb78 <.text+0x8eb78>
  18eb54: 24 98 68 00  	and	$19, $3, $8
  18eb58: 50 00 bf df  	ld	$ra, 0x50($sp)
  18eb5c: 40 00 b4 df  	ld	$20, 0x40($sp)
  18eb60: 30 00 b3 df  	ld	$19, 0x30($sp)
  18eb64: 20 00 b2 df  	ld	$18, 0x20($sp)
  18eb68: 10 00 b1 df  	ld	$17, 0x10($sp)
  18eb6c: 00 00 b0 df  	ld	$16, 0x0($sp)
  18eb70: 08 00 e0 03  	jr	$ra
  18eb74: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  18eb78: 06 60 53 a2  	sb	$19, 0x6006($18)
  18eb7c: 01 00 04 24  	addiu	$4, $zero, 0x1
  18eb80: 2d a0 60 02  	move	$20, $19
  18eb84: eb 38 06 0c  	jal	0x18e3ac <.text+0x8e3ac>
  18eb88: 00 20 12 24  	addiu	$18, $zero, 0x2000
  18eb8c: 2a 6e 02 82  	lb	$2, 0x6e2a($16)
  18eb90: f2 ff 40 14  	bnez	$2, 0x18eb5c <.text+0x8eb5c>
  18eb94: 50 00 bf df  	ld	$ra, 0x50($sp)
  18eb98: 42 00 10 3c  	lui	$16, 0x42
  18eb9c: 42 00 11 3c  	lui	$17, 0x42
  18eba0: 3c 6e 03 8e  	lw	$3, 0x6e3c($16)
  18eba4: 2c 6e 22 8e  	lw	$2, 0x6e2c($17)
  18eba8: 2a 10 43 00  	slt	$2, $2, $3
  18ebac: b9 00 40 14  	bnez	$2, 0x18ee94 <.text+0x8ee94>
  18ebb0: 00 00 00 00  	nop
  18ebb4: 3c 6e 06 8e  	lw	$6, 0x6e3c($16)
  18ebb8: 42 00 02 3c  	lui	$2, 0x42
  18ebbc: 42 00 07 3c  	lui	$7, 0x42
  18ebc0: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18ebc4: 40 18 06 00  	sll	$3, $6, 0x1
  18ebc8: 20 6e e5 dc  	ld	$5, 0x6e20($7)
  18ebcc: 21 18 62 00  	addu	$3, $3, $2
  18ebd0: 2c 6e 24 8e  	lw	$4, 0x6e2c($17)
  18ebd4: 42 00 02 3c  	lui	$2, 0x42
  18ebd8: 00 00 68 94  	lhu	$8, 0x0($3)
  18ebdc: 2a 6e 43 80  	lb	$3, 0x6e2a($2)
  18ebe0: 23 20 86 00  	subu	$4, $4, $6
  18ebe4: 20 6e e2 94  	lhu	$2, 0x6e20($7)
  18ebe8: 16 28 c5 00  	dsrlv	$5, $5, $6
  18ebec: 20 6e e5 fc  	sd	$5, 0x6e20($7)
  18ebf0: 2c 6e 24 ae  	sw	$4, 0x6e2c($17)
  18ebf4: d8 ff 60 14  	bnez	$3, 0x18eb58 <.text+0x8eb58>
  18ebf8: 24 30 48 00  	and	$6, $2, $8
  18ebfc: 00 01 02 24  	addiu	$2, $zero, 0x100
  18ec00: 51 00 c2 10  	beq	$6, $2, 0x18ed48 <.text+0x8ed48>
  18ec04: 45 00 02 3c  	lui	$2, 0x45
  18ec08: 00 82 44 24  	addiu	$4, $2, -0x7e00 <.text+0xffffffffffef8200>
  18ec0c: 40 10 06 00  	sll	$2, $6, 0x1
  18ec10: 21 10 44 00  	addu	$2, $2, $4
  18ec14: 00 00 43 84  	lh	$3, 0x0($2)
  18ec18: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18ec1c: 45 00 62 10  	beq	$3, $2, 0x18ed34 <.text+0x8ed34>
  18ec20: 2d 88 c0 00  	move	$17, $6
  18ec24: 01 01 c2 28  	slti	$2, $6, 0x101
  18ec28: 12 00 40 14  	bnez	$2, 0x18ec74 <.text+0x8ec74>
  18ec2c: 06 60 82 24  	addiu	$2, $4, 0x6006
  18ec30: 21 28 42 02  	addu	$5, $18, $2
  18ec34: 45 00 03 3c  	lui	$3, 0x45
  18ec38: 40 10 06 00  	sll	$2, $6, 0x1
  18ec3c: 00 82 63 24  	addiu	$3, $3, -0x7e00 <.text+0xffffffffffef8200>
  18ec40: 21 10 43 00  	addu	$2, $2, $3
  18ec44: 21 20 c3 00  	addu	$4, $6, $3
  18ec48: 00 00 43 84  	lh	$3, 0x0($2)
  18ec4c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18ec50: 34 00 62 10  	beq	$3, $2, 0x18ed24 <.text+0x8ed24>
  18ec54: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  18ec58: 04 40 82 90  	lbu	$2, 0x4004($4)
  18ec5c: ff ff 52 26  	addiu	$18, $18, -0x1 <.text+0xffffffffffefffff>
  18ec60: 2d 30 60 00  	move	$6, $3
  18ec64: 00 00 a2 a0  	sb	$2, 0x0($5)
  18ec68: 01 01 c2 28  	slti	$2, $6, 0x101
  18ec6c: f2 ff 40 10  	beqz	$2, 0x18ec38 <.text+0x8ec38>
  18ec70: 45 00 03 3c  	lui	$3, 0x45
  18ec74: 45 00 02 3c  	lui	$2, 0x45
  18ec78: ff ff 52 26  	addiu	$18, $18, -0x1 <.text+0xffffffffffefffff>
  18ec7c: 00 82 50 24  	addiu	$16, $2, -0x7e00 <.text+0xffffffffffef8200>
  18ec80: 00 20 04 24  	addiu	$4, $zero, 0x2000
  18ec84: 21 10 d0 00  	addu	$2, $6, $16
  18ec88: 23 20 92 00  	subu	$4, $4, $18
  18ec8c: 04 40 54 90  	lbu	$20, 0x4004($2)
  18ec90: 21 10 50 02  	addu	$2, $18, $16
  18ec94: 00 20 12 24  	addiu	$18, $zero, 0x2000
  18ec98: eb 38 06 0c  	jal	0x18e3ac <.text+0x8e3ac>
  18ec9c: 06 60 54 a0  	sb	$20, 0x6006($2)
  18eca0: 42 00 02 3c  	lui	$2, 0x42
  18eca4: 38 6e 46 8c  	lw	$6, 0x6e38($2)
  18eca8: 42 00 02 3c  	lui	$2, 0x42
  18ecac: 34 6e 42 8c  	lw	$2, 0x6e34($2)
  18ecb0: 2a 10 c2 00  	slt	$2, $6, $2
  18ecb4: 16 00 40 10  	beqz	$2, 0x18ed10 <.text+0x8ed10>
  18ecb8: 42 00 02 3c  	lui	$2, 0x42
  18ecbc: 40 10 06 00  	sll	$2, $6, 0x1
  18ecc0: 21 18 d0 00  	addu	$3, $6, $16
  18ecc4: 21 10 50 00  	addu	$2, $2, $16
  18ecc8: 04 40 74 a0  	sb	$20, 0x4004($3)
  18eccc: 00 00 53 a4  	sh	$19, 0x0($2)
  18ecd0: 42 00 02 3c  	lui	$2, 0x42
  18ecd4: 01 00 c6 24  	addiu	$6, $6, 0x1
  18ecd8: 34 6e 43 8c  	lw	$3, 0x6e34($2)
  18ecdc: 40 20 06 00  	sll	$4, $6, 0x1
  18ece0: 45 00 02 3c  	lui	$2, 0x45
  18ece4: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18ece8: 2a 18 c3 00  	slt	$3, $6, $3
  18ecec: 05 00 60 10  	beqz	$3, 0x18ed04 <.text+0x8ed04>
  18ecf0: 21 20 82 00  	addu	$4, $4, $2
  18ecf4: 00 00 83 84  	lh	$3, 0x0($4)
  18ecf8: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18ecfc: f5 ff 62 14  	bne	$3, $2, 0x18ecd4 <.text+0x8ecd4>
  18ed00: 42 00 02 3c  	lui	$2, 0x42
  18ed04: 42 00 02 3c  	lui	$2, 0x42
  18ed08: 38 6e 46 ac  	sw	$6, 0x6e38($2)
  18ed0c: 42 00 02 3c  	lui	$2, 0x42
  18ed10: 2a 6e 42 80  	lb	$2, 0x6e2a($2)
  18ed14: a0 ff 40 10  	beqz	$2, 0x18eb98 <.text+0x8eb98>
  18ed18: 2d 98 20 02  	move	$19, $17
  18ed1c: 8f ff 00 10  	b	0x18eb5c <.text+0x8eb5c>
  18ed20: 50 00 bf df  	ld	$ra, 0x50($sp)
  18ed24: ff ff 52 26  	addiu	$18, $18, -0x1 <.text+0xffffffffffefffff>
  18ed28: 2d 30 60 02  	move	$6, $19
  18ed2c: ce ff 00 10  	b	0x18ec68 <.text+0x8ec68>
  18ed30: 00 00 b4 a0  	sb	$20, 0x0($5)
  18ed34: ff 7f 82 24  	addiu	$2, $4, 0x7fff
  18ed38: ff 1f 12 24  	addiu	$18, $zero, 0x1fff
  18ed3c: 06 00 54 a0  	sb	$20, 0x6($2)
  18ed40: b8 ff 00 10  	b	0x18ec24 <.text+0x8ec24>
  18ed44: 2d 30 60 02  	move	$6, $19
  18ed48: 42 00 11 3c  	lui	$17, 0x42
  18ed4c: 42 00 10 3c  	lui	$16, 0x42
  18ed50: 3c 6e 23 8e  	lw	$3, 0x6e3c($17)
  18ed54: 2c 6e 02 8e  	lw	$2, 0x6e2c($16)
  18ed58: 2a 10 43 00  	slt	$2, $2, $3
  18ed5c: 49 00 40 14  	bnez	$2, 0x18ee84 <.text+0x8ee84>
  18ed60: 00 00 00 00  	nop
  18ed64: 3c 6e 27 8e  	lw	$7, 0x6e3c($17)
  18ed68: 42 00 02 3c  	lui	$2, 0x42
  18ed6c: 42 00 04 3c  	lui	$4, 0x42
  18ed70: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18ed74: 40 18 07 00  	sll	$3, $7, 0x1
  18ed78: 20 6e 86 94  	lhu	$6, 0x6e20($4)
  18ed7c: 21 18 62 00  	addu	$3, $3, $2
  18ed80: 20 6e 82 dc  	ld	$2, 0x6e20($4)
  18ed84: 00 00 65 94  	lhu	$5, 0x0($3)
  18ed88: 2c 6e 03 8e  	lw	$3, 0x6e2c($16)
  18ed8c: 16 10 e2 00  	dsrlv	$2, $2, $7
  18ed90: 20 6e 82 fc  	sd	$2, 0x6e20($4)
  18ed94: 24 30 c5 00  	and	$6, $6, $5
  18ed98: 23 18 67 00  	subu	$3, $3, $7
  18ed9c: 01 00 02 24  	addiu	$2, $zero, 0x1
  18eda0: 2a 00 c2 10  	beq	$6, $2, 0x18ee4c <.text+0x8ee4c>
  18eda4: 2c 6e 03 ae  	sw	$3, 0x6e2c($16)
  18eda8: 02 00 02 24  	addiu	$2, $zero, 0x2
  18edac: 23 00 c2 10  	beq	$6, $2, 0x18ee3c <.text+0x8ee3c>
  18edb0: 00 00 00 00  	nop
  18edb4: 42 00 10 3c  	lui	$16, 0x42
  18edb8: 42 00 11 3c  	lui	$17, 0x42
  18edbc: 3c 6e 03 8e  	lw	$3, 0x6e3c($16)
  18edc0: 2c 6e 22 8e  	lw	$2, 0x6e2c($17)
  18edc4: 2a 10 43 00  	slt	$2, $2, $3
  18edc8: 18 00 40 14  	bnez	$2, 0x18ee2c <.text+0x8ee2c>
  18edcc: 00 00 00 00  	nop
  18edd0: 3c 6e 06 8e  	lw	$6, 0x6e3c($16)
  18edd4: 42 00 02 3c  	lui	$2, 0x42
  18edd8: 42 00 07 3c  	lui	$7, 0x42
  18eddc: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18ede0: 40 18 06 00  	sll	$3, $6, 0x1
  18ede4: 20 6e e5 dc  	ld	$5, 0x6e20($7)
  18ede8: 21 18 62 00  	addu	$3, $3, $2
  18edec: 2c 6e 24 8e  	lw	$4, 0x6e2c($17)
  18edf0: 42 00 02 3c  	lui	$2, 0x42
  18edf4: 00 00 68 94  	lhu	$8, 0x0($3)
  18edf8: 2a 6e 43 80  	lb	$3, 0x6e2a($2)
  18edfc: 23 20 86 00  	subu	$4, $4, $6
  18ee00: 20 6e e2 94  	lhu	$2, 0x6e20($7)
  18ee04: 16 28 c5 00  	dsrlv	$5, $5, $6
  18ee08: 20 6e e5 fc  	sd	$5, 0x6e20($7)
  18ee0c: 2c 6e 24 ae  	sw	$4, 0x6e2c($17)
  18ee10: 51 ff 60 14  	bnez	$3, 0x18eb58 <.text+0x8eb58>
  18ee14: 24 30 48 00  	and	$6, $2, $8
  18ee18: 00 01 02 24  	addiu	$2, $zero, 0x100
  18ee1c: cb ff c2 10  	beq	$6, $2, 0x18ed4c <.text+0x8ed4c>
  18ee20: 42 00 11 3c  	lui	$17, 0x42
  18ee24: 78 ff 00 10  	b	0x18ec08 <.text+0x8ec08>
  18ee28: 45 00 02 3c  	lui	$2, 0x45
  18ee2c: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18ee30: 00 00 00 00  	nop
  18ee34: e7 ff 00 10  	b	0x18edd4 <.text+0x8edd4>
  18ee38: 3c 6e 06 8e  	lw	$6, 0x6e3c($16)
  18ee3c: ad 3b 06 0c  	jal	0x18eeb4 <.text+0x8eeb4>
  18ee40: 42 00 10 3c  	lui	$16, 0x42
  18ee44: dd ff 00 10  	b	0x18edbc <.text+0x8edbc>
  18ee48: 42 00 11 3c  	lui	$17, 0x42
  18ee4c: 01 00 e3 24  	addiu	$3, $7, 0x1
  18ee50: 0d 00 02 24  	addiu	$2, $zero, 0xd
  18ee54: 06 00 62 10  	beq	$3, $2, 0x18ee70 <.text+0x8ee70>
  18ee58: 3c 6e 23 ae  	sw	$3, 0x6e3c($17)
  18ee5c: 04 10 66 00  	sllv	$2, $6, $3
  18ee60: 42 00 03 3c  	lui	$3, 0x42
  18ee64: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  18ee68: d2 ff 00 10  	b	0x18edb4 <.text+0x8edb4>
  18ee6c: 40 6e 62 ac  	sw	$2, 0x6e40($3)
  18ee70: 42 00 02 3c  	lui	$2, 0x42
  18ee74: 34 6e 43 8c  	lw	$3, 0x6e34($2)
  18ee78: 42 00 02 3c  	lui	$2, 0x42
  18ee7c: cd ff 00 10  	b	0x18edb4 <.text+0x8edb4>
  18ee80: 40 6e 43 ac  	sw	$3, 0x6e40($2)
  18ee84: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18ee88: 00 00 00 00  	nop
  18ee8c: b6 ff 00 10  	b	0x18ed68 <.text+0x8ed68>
  18ee90: 3c 6e 27 8e  	lw	$7, 0x6e3c($17)
  18ee94: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18ee98: 00 00 00 00  	nop
  18ee9c: 46 ff 00 10  	b	0x18ebb8 <.text+0x8ebb8>
  18eea0: 3c 6e 06 8e  	lw	$6, 0x6e3c($16)
  18eea4: 10 39 06 0c  	jal	0x18e440 <.text+0x8e440>
  18eea8: 00 00 00 00  	nop
  18eeac: 19 ff 00 10  	b	0x18eb14 <.text+0x8eb14>
  18eeb0: 3c 6e 05 8e  	lw	$5, 0x6e3c($16)
  18eeb4: 42 00 02 3c  	lui	$2, 0x42
  18eeb8: 01 01 06 24  	addiu	$6, $zero, 0x101
  18eebc: 38 6e 47 8c  	lw	$7, 0x6e38($2)
  18eec0: 2a 10 c7 00  	slt	$2, $6, $7
  18eec4: 0e 00 40 10  	beqz	$2, 0x18ef00 <.text+0x8ef00>
  18eec8: 42 00 02 3c  	lui	$2, 0x42
  18eecc: 45 00 02 3c  	lui	$2, 0x45
  18eed0: 40 20 06 00  	sll	$4, $6, 0x1
  18eed4: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18eed8: 00 80 03 24  	addiu	$3, $zero, -0x8000 <.text+0xffffffffffef8000>
  18eedc: 21 20 82 00  	addu	$4, $4, $2
  18eee0: 01 00 c6 24  	addiu	$6, $6, 0x1
  18eee4: 00 00 82 94  	lhu	$2, 0x0($4)
  18eee8: 2a 28 c7 00  	slt	$5, $6, $7
  18eeec: 25 10 43 00  	or	$2, $2, $3
  18eef0: f6 ff a0 14  	bnez	$5, 0x18eecc <.text+0x8eecc>
  18eef4: 00 00 82 a4  	sh	$2, 0x0($4)
  18eef8: 42 00 02 3c  	lui	$2, 0x42
  18eefc: 01 01 06 24  	addiu	$6, $zero, 0x101
  18ef00: 38 6e 42 8c  	lw	$2, 0x6e38($2)
  18ef04: 2a 10 c2 00  	slt	$2, $6, $2
  18ef08: 15 00 40 10  	beqz	$2, 0x18ef60 <.text+0x8ef60>
  18ef0c: 42 00 02 3c  	lui	$2, 0x42
  18ef10: 45 00 04 3c  	lui	$4, 0x45
  18ef14: 40 18 06 00  	sll	$3, $6, 0x1
  18ef18: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18ef1c: 42 00 02 3c  	lui	$2, 0x42
  18ef20: 21 18 64 00  	addu	$3, $3, $4
  18ef24: 38 6e 45 8c  	lw	$5, 0x6e38($2)
  18ef28: 00 00 62 94  	lhu	$2, 0x0($3)
  18ef2c: 01 00 c6 24  	addiu	$6, $6, 0x1
  18ef30: 2a 28 c5 00  	slt	$5, $6, $5
  18ef34: ff 7f 42 30  	andi	$2, $2, 0x7fff
  18ef38: 40 18 02 00  	sll	$3, $2, 0x1
  18ef3c: 01 01 42 28  	slti	$2, $2, 0x101
  18ef40: 04 00 40 14  	bnez	$2, 0x18ef54 <.text+0x8ef54>
  18ef44: 21 18 64 00  	addu	$3, $3, $4
  18ef48: 00 00 62 94  	lhu	$2, 0x0($3)
  18ef4c: ff 7f 42 30  	andi	$2, $2, 0x7fff
  18ef50: 00 00 62 a4  	sh	$2, 0x0($3)
  18ef54: ef ff a0 54  	bnezl	$5, 0x18ef14 <.text+0x8ef14>
  18ef58: 45 00 04 3c  	lui	$4, 0x45
  18ef5c: 42 00 02 3c  	lui	$2, 0x42
  18ef60: 01 01 06 24  	addiu	$6, $zero, 0x101
  18ef64: 38 6e 42 8c  	lw	$2, 0x6e38($2)
  18ef68: 2a 10 c2 00  	slt	$2, $6, $2
  18ef6c: 12 00 40 10  	beqz	$2, 0x18efb8 <.text+0x8efb8>
  18ef70: 42 00 02 3c  	lui	$2, 0x42
  18ef74: 45 00 02 3c  	lui	$2, 0x45
  18ef78: 40 18 06 00  	sll	$3, $6, 0x1
  18ef7c: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18ef80: 01 00 c6 24  	addiu	$6, $6, 0x1
  18ef84: 21 20 62 00  	addu	$4, $3, $2
  18ef88: 42 00 02 3c  	lui	$2, 0x42
  18ef8c: 38 6e 43 8c  	lw	$3, 0x6e38($2)
  18ef90: 00 00 82 84  	lh	$2, 0x0($4)
  18ef94: 00 80 42 30  	andi	$2, $2, 0x8000
  18ef98: 03 00 40 10  	beqz	$2, 0x18efa8 <.text+0x8efa8>
  18ef9c: 2a 18 c3 00  	slt	$3, $6, $3
  18efa0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18efa4: 00 00 82 a4  	sh	$2, 0x0($4)
  18efa8: f3 ff 60 14  	bnez	$3, 0x18ef78 <.text+0x8ef78>
  18efac: 45 00 02 3c  	lui	$2, 0x45
  18efb0: 42 00 02 3c  	lui	$2, 0x42
  18efb4: 01 01 06 24  	addiu	$6, $zero, 0x101
  18efb8: 34 6e 45 8c  	lw	$5, 0x6e34($2)
  18efbc: 2a 10 c5 00  	slt	$2, $6, $5
  18efc0: 11 00 40 10  	beqz	$2, 0x18f008 <.text+0x8f008>
  18efc4: 42 00 02 3c  	lui	$2, 0x42
  18efc8: 45 00 02 3c  	lui	$2, 0x45
  18efcc: 00 82 44 24  	addiu	$4, $2, -0x7e00 <.text+0xffffffffffef8200>
  18efd0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18efd4: 02 02 83 84  	lh	$3, 0x202($4)
  18efd8: 0b 00 62 10  	beq	$3, $2, 0x18f008 <.text+0x8f008>
  18efdc: 42 00 02 3c  	lui	$2, 0x42
  18efe0: 02 02 84 24  	addiu	$4, $4, 0x202
  18efe4: 01 00 c6 24  	addiu	$6, $6, 0x1
  18efe8: 2a 10 c5 00  	slt	$2, $6, $5
  18efec: 05 00 40 10  	beqz	$2, 0x18f004 <.text+0x8f004>
  18eff0: 02 00 84 24  	addiu	$4, $4, 0x2
  18eff4: 00 00 83 84  	lh	$3, 0x0($4)
  18eff8: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18effc: fa ff 62 54  	bnel	$3, $2, 0x18efe8 <.text+0x8efe8>
  18f000: 01 00 c6 24  	addiu	$6, $6, 0x1
  18f004: 42 00 02 3c  	lui	$2, 0x42
  18f008: 08 00 e0 03  	jr	$ra
  18f00c: 38 6e 46 ac  	sw	$6, 0x6e38($2)
