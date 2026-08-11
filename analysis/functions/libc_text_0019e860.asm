
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  19e860: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19e864: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19e868: 2d 90 a0 00  	move	$18, $5
  19e86c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19e870: 2d 88 80 00  	move	$17, $4
  19e874: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19e878: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19e87c: 00 00 22 82  	lb	$2, 0x0($17)
  19e880: 0b 00 40 10  	beqz	$2, 0x19e8b0 <.text+0x9e8b0>
  19e884: 2d 20 40 00  	move	$4, $2
  19e888: 6b 7b 06 0c  	jal	0x19edac <.text+0x9edac>
  19e88c: 00 00 00 00  	nop
  19e890: 00 00 44 82  	lb	$4, 0x0($18)
  19e894: 6b 7b 06 0c  	jal	0x19edac <.text+0x9edac>
  19e898: 2d 80 40 00  	move	$16, $2
  19e89c: 04 00 02 16  	bne	$16, $2, 0x19e8b0 <.text+0x9e8b0>
  19e8a0: 00 00 00 00  	nop
  19e8a4: 01 00 31 26  	addiu	$17, $17, 0x1
  19e8a8: f4 ff 00 10  	b	0x19e87c <.text+0x9e87c>
  19e8ac: 01 00 52 26  	addiu	$18, $18, 0x1
  19e8b0: 6b 7b 06 0c  	jal	0x19edac <.text+0x9edac>
  19e8b4: 00 00 24 92  	lbu	$4, 0x0($17)
  19e8b8: 00 00 44 92  	lbu	$4, 0x0($18)
  19e8bc: 6b 7b 06 0c  	jal	0x19edac <.text+0x9edac>
  19e8c0: 2d 80 40 00  	move	$16, $2
  19e8c4: 10 00 b1 df  	ld	$17, 0x10($sp)
  19e8c8: 23 80 02 02  	subu	$16, $16, $2
  19e8cc: 30 00 bf df  	ld	$ra, 0x30($sp)
  19e8d0: 2d 10 00 02  	move	$2, $16
  19e8d4: 20 00 b2 df  	ld	$18, 0x20($sp)
  19e8d8: 00 00 b0 df  	ld	$16, 0x0($sp)
  19e8dc: 08 00 e0 03  	jr	$ra
  19e8e0: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19e8e4: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  19e8e8: 2d 10 00 00  	move	$2, $zero
  19e8ec: 30 00 b3 ff  	sd	$19, 0x30($sp)
  19e8f0: 2d 98 a0 00  	move	$19, $5
  19e8f4: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19e8f8: 2d 90 c0 00  	move	$18, $6
  19e8fc: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19e900: 2d 88 80 00  	move	$17, $4
  19e904: 40 00 bf ff  	sd	$ra, 0x40($sp)
  19e908: 1d 00 c0 10  	beqz	$6, 0x19e980 <.text+0x9e980>
  19e90c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19e910: ff ff 52 26  	addiu	$18, $18, -0x1 <.text+0xffffffffffefffff>
  19e914: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19e918: 13 00 42 12  	beq	$18, $2, 0x19e968 <.text+0x9e968>
  19e91c: 00 00 00 00  	nop
  19e920: 6b 7b 06 0c  	jal	0x19edac <.text+0x9edac>
  19e924: 00 00 24 82  	lb	$4, 0x0($17)
  19e928: 00 00 64 82  	lb	$4, 0x0($19)
  19e92c: 6b 7b 06 0c  	jal	0x19edac <.text+0x9edac>
  19e930: 2d 80 40 00  	move	$16, $2
  19e934: 0c 00 02 16  	bne	$16, $2, 0x19e968 <.text+0x9e968>
  19e938: 00 00 00 00  	nop
  19e93c: 0a 00 40 12  	beqz	$18, 0x19e968 <.text+0x9e968>
  19e940: 00 00 00 00  	nop
  19e944: 00 00 22 82  	lb	$2, 0x0($17)
  19e948: 07 00 40 10  	beqz	$2, 0x19e968 <.text+0x9e968>
  19e94c: 00 00 00 00  	nop
  19e950: 00 00 62 82  	lb	$2, 0x0($19)
  19e954: 04 00 40 10  	beqz	$2, 0x19e968 <.text+0x9e968>
  19e958: 00 00 00 00  	nop
  19e95c: 01 00 31 26  	addiu	$17, $17, 0x1
  19e960: eb ff 00 10  	b	0x19e910 <.text+0x9e910>
  19e964: 01 00 73 26  	addiu	$19, $19, 0x1
  19e968: 6b 7b 06 0c  	jal	0x19edac <.text+0x9edac>
  19e96c: 00 00 24 92  	lbu	$4, 0x0($17)
  19e970: 00 00 64 92  	lbu	$4, 0x0($19)
  19e974: 6b 7b 06 0c  	jal	0x19edac <.text+0x9edac>
  19e978: 2d 80 40 00  	move	$16, $2
  19e97c: 23 10 02 02  	subu	$2, $16, $2
  19e980: 40 00 bf df  	ld	$ra, 0x40($sp)
  19e984: 30 00 b3 df  	ld	$19, 0x30($sp)
  19e988: 20 00 b2 df  	ld	$18, 0x20($sp)
  19e98c: 10 00 b1 df  	ld	$17, 0x10($sp)
  19e990: 00 00 b0 df  	ld	$16, 0x0($sp)
  19e994: 08 00 e0 03  	jr	$ra
  19e998: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  19e99c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19e9a0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19e9a4: 2d 88 a0 00  	move	$17, $5
  19e9a8: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19e9ac: 35 00 80 10  	beqz	$4, 0x19ea84 <.text+0x9ea84>
  19e9b0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19e9b4: 44 00 02 3c  	lui	$2, 0x44
  19e9b8: 40 51 44 ac  	sw	$4, 0x5140($2)
  19e9bc: 44 00 10 3c  	lui	$16, 0x44
  19e9c0: 2d 20 20 02  	move	$4, $17
  19e9c4: 40 51 02 8e  	lw	$2, 0x5140($16)
  19e9c8: 84 71 06 0c  	jal	0x19c610 <.text+0x9c610>
  19e9cc: 00 00 45 80  	lb	$5, 0x0($2)
  19e9d0: 0d 00 40 10  	beqz	$2, 0x19ea08 <.text+0x9ea08>
  19e9d4: 40 51 02 8e  	lw	$2, 0x5140($16)
  19e9d8: 01 00 43 24  	addiu	$3, $2, 0x1
  19e9dc: 40 51 03 ae  	sw	$3, 0x5140($16)
  19e9e0: 01 00 42 80  	lb	$2, 0x1($2)
  19e9e4: f6 ff 40 14  	bnez	$2, 0x19e9c0 <.text+0x9e9c0>
  19e9e8: 44 00 10 3c  	lui	$16, 0x44
  19e9ec: 2d 20 00 00  	move	$4, $zero
  19e9f0: 20 00 bf df  	ld	$ra, 0x20($sp)
  19e9f4: 2d 10 80 00  	move	$2, $4
  19e9f8: 10 00 b1 df  	ld	$17, 0x10($sp)
  19e9fc: 00 00 b0 df  	ld	$16, 0x0($sp)
  19ea00: 08 00 e0 03  	jr	$ra
  19ea04: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19ea08: 40 51 03 8e  	lw	$3, 0x5140($16)
  19ea0c: 00 00 62 80  	lb	$2, 0x0($3)
  19ea10: f7 ff 40 10  	beqz	$2, 0x19e9f0 <.text+0x9e9f0>
  19ea14: 2d 20 00 00  	move	$4, $zero
  19ea18: 44 00 10 3c  	lui	$16, 0x44
  19ea1c: 44 51 03 ae  	sw	$3, 0x5144($16)
  19ea20: 00 00 62 80  	lb	$2, 0x0($3)
  19ea24: 03 00 40 14  	bnez	$2, 0x19ea34 <.text+0x9ea34>
  19ea28: 44 00 02 3c  	lui	$2, 0x44
  19ea2c: f0 ff 00 10  	b	0x19e9f0 <.text+0x9e9f0>
  19ea30: 40 51 44 8c  	lw	$4, 0x5140($2)
  19ea34: 44 51 02 8e  	lw	$2, 0x5144($16)
  19ea38: 2d 20 20 02  	move	$4, $17
  19ea3c: 84 71 06 0c  	jal	0x19c610 <.text+0x9c610>
  19ea40: 00 00 45 80  	lb	$5, 0x0($2)
  19ea44: 08 00 40 14  	bnez	$2, 0x19ea68 <.text+0x9ea68>
  19ea48: 44 51 02 8e  	lw	$2, 0x5144($16)
  19ea4c: 01 00 43 24  	addiu	$3, $2, 0x1
  19ea50: 44 51 03 ae  	sw	$3, 0x5144($16)
  19ea54: 01 00 42 80  	lb	$2, 0x1($2)
  19ea58: f7 ff 40 54  	bnezl	$2, 0x19ea38 <.text+0x9ea38>
  19ea5c: 44 51 02 8e  	lw	$2, 0x5144($16)
  19ea60: f2 ff 00 10  	b	0x19ea2c <.text+0x9ea2c>
  19ea64: 44 00 02 3c  	lui	$2, 0x44
  19ea68: 44 00 03 3c  	lui	$3, 0x44
  19ea6c: 00 00 40 a0  	sb	$zero, 0x0($2)
  19ea70: 44 51 02 8e  	lw	$2, 0x5144($16)
  19ea74: 40 51 64 8c  	lw	$4, 0x5140($3)
  19ea78: 01 00 42 24  	addiu	$2, $2, 0x1
  19ea7c: dc ff 00 10  	b	0x19e9f0 <.text+0x9e9f0>
  19ea80: 44 51 02 ae  	sw	$2, 0x5144($16)
  19ea84: 44 00 02 3c  	lui	$2, 0x44
  19ea88: 44 51 43 8c  	lw	$3, 0x5144($2)
  19ea8c: 00 00 62 80  	lb	$2, 0x0($3)
  19ea90: d7 ff 40 10  	beqz	$2, 0x19e9f0 <.text+0x9e9f0>
  19ea94: 2d 20 00 00  	move	$4, $zero
  19ea98: 44 00 02 3c  	lui	$2, 0x44
  19ea9c: c7 ff 00 10  	b	0x19e9bc <.text+0x9e9bc>
  19eaa0: 40 51 43 ac  	sw	$3, 0x5140($2)
  19eaa4: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19eaa8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19eaac: 2d 88 a0 00  	move	$17, $5
  19eab0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19eab4: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19eab8: 84 71 06 0c  	jal	0x19c610 <.text+0x9c610>
  19eabc: 2d 80 00 00  	move	$16, $zero
  19eac0: 08 00 40 50  	beqzl	$2, 0x19eae4 <.text+0x9eae4>
  19eac4: 2d 10 00 02  	move	$2, $16
  19eac8: 2d 28 20 02  	move	$5, $17
  19eacc: 01 00 44 24  	addiu	$4, $2, 0x1
  19ead0: 84 71 06 0c  	jal	0x19c610 <.text+0x9c610>
  19ead4: 2d 80 40 00  	move	$16, $2
  19ead8: fc ff 40 14  	bnez	$2, 0x19eacc <.text+0x9eacc>
  19eadc: 2d 28 20 02  	move	$5, $17
  19eae0: 2d 10 00 02  	move	$2, $16
  19eae4: 20 00 bf df  	ld	$ra, 0x20($sp)
  19eae8: 10 00 b1 df  	ld	$17, 0x10($sp)
  19eaec: 00 00 b0 df  	ld	$16, 0x0($sp)
  19eaf0: 08 00 e0 03  	jr	$ra
  19eaf4: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19eaf8: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19eafc: 2d 18 00 00  	move	$3, $zero
  19eb00: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19eb04: 2d 88 a0 00  	move	$17, $5
  19eb08: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19eb0c: 08 00 80 10  	beqz	$4, 0x19eb30 <.text+0x9eb30>
  19eb10: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19eb14: 00 00 a2 80  	lb	$2, 0x0($5)
  19eb18: 05 00 40 10  	beqz	$2, 0x19eb30 <.text+0x9eb30>
  19eb1c: 2d 18 80 00  	move	$3, $4
  19eb20: 00 00 82 80  	lb	$2, 0x0($4)
  19eb24: 08 00 40 14  	bnez	$2, 0x19eb48 <.text+0x9eb48>
  19eb28: 2d 80 80 00  	move	$16, $4
  19eb2c: 2d 18 00 00  	move	$3, $zero
  19eb30: 20 00 bf df  	ld	$ra, 0x20($sp)
  19eb34: 2d 10 60 00  	move	$2, $3
  19eb38: 10 00 b1 df  	ld	$17, 0x10($sp)
  19eb3c: 00 00 b0 df  	ld	$16, 0x0($sp)
  19eb40: 08 00 e0 03  	jr	$ra
  19eb44: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19eb48: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  19eb4c: 2d 20 20 02  	move	$4, $17
  19eb50: 2d 28 20 02  	move	$5, $17
  19eb54: 2d 20 00 02  	move	$4, $16
  19eb58: 04 71 06 0c  	jal	0x19c410 <.text+0x9c410>
  19eb5c: 2d 30 40 00  	move	$6, $2
  19eb60: f3 ff 40 10  	beqz	$2, 0x19eb30 <.text+0x9eb30>
  19eb64: 2d 18 00 02  	move	$3, $16
  19eb68: 01 00 10 26  	addiu	$16, $16, 0x1
  19eb6c: 00 00 02 82  	lb	$2, 0x0($16)
  19eb70: f5 ff 40 14  	bnez	$2, 0x19eb48 <.text+0x9eb48>
  19eb74: 2d 18 00 00  	move	$3, $zero
  19eb78: ee ff 00 10  	b	0x19eb34 <.text+0x9eb34>
  19eb7c: 20 00 bf df  	ld	$ra, 0x20($sp)
  19eb80: 50 ff bd 27  	addiu	$sp, $sp, -0xb0 <.text+0xffffffffffefff50>
  19eb84: a0 00 bf ff  	sd	$ra, 0xa0($sp)
  19eb88: 70 00 b6 ff  	sd	$22, 0x70($sp)
  19eb8c: 60 00 b5 ff  	sd	$21, 0x60($sp)
  19eb90: 50 00 b4 ff  	sd	$20, 0x50($sp)
  19eb94: 30 00 b2 ff  	sd	$18, 0x30($sp)
  19eb98: 10 00 b0 ff  	sd	$16, 0x10($sp)
  19eb9c: 00 00 a5 af  	sw	$5, 0x0($sp)
  19eba0: 90 00 be ff  	sd	$fp, 0x90($sp)
  19eba4: 2d f0 00 00  	move	$fp, $zero
  19eba8: 80 00 b7 ff  	sd	$23, 0x80($sp)
  19ebac: 2d b8 80 00  	move	$23, $4
  19ebb0: 40 00 b3 ff  	sd	$19, 0x40($sp)
  19ebb4: 2d 98 c0 00  	move	$19, $6
  19ebb8: 20 00 b1 ff  	sd	$17, 0x20($sp)
  19ebbc: 2d 88 80 00  	move	$17, $4
  19ebc0: 00 00 30 82  	lb	$16, 0x0($17)
  19ebc4: 01 00 31 26  	addiu	$17, $17, 0x1
  19ebc8: eb 7b 06 0c  	jal	0x19efac <.text+0x9efac>
  19ebcc: 2d 20 00 02  	move	$4, $16
  19ebd0: fc ff 40 54  	bnezl	$2, 0x19ebc4 <.text+0x9ebc4>
  19ebd4: 00 00 30 82  	lb	$16, 0x0($17)
  19ebd8: 2d 00 02 24  	addiu	$2, $zero, 0x2d
  19ebdc: 70 00 02 12  	beq	$16, $2, 0x19eda0 <.text+0x9eda0>
  19ebe0: 2b 00 02 24  	addiu	$2, $zero, 0x2b
  19ebe4: 6c 00 02 52  	beql	$16, $2, 0x19ed98 <.text+0x9ed98>
  19ebe8: 00 00 30 82  	lb	$16, 0x0($17)
  19ebec: 5f 00 60 12  	beqz	$19, 0x19ed6c <.text+0x9ed6c>
  19ebf0: 30 00 02 24  	addiu	$2, $zero, 0x30
  19ebf4: 10 00 02 24  	addiu	$2, $zero, 0x10
  19ebf8: 5c 00 62 12  	beq	$19, $2, 0x19ed6c <.text+0x9ed6c>
  19ebfc: 30 00 02 24  	addiu	$2, $zero, 0x30
  19ec00: 04 00 60 16  	bnez	$19, 0x19ec14 <.text+0x9ec14>
  19ec04: 30 00 03 3a  	xori	$3, $16, 0x30
  19ec08: 08 00 13 24  	addiu	$19, $zero, 0x8
  19ec0c: 0a 00 02 24  	addiu	$2, $zero, 0xa
  19ec10: 0b 98 43 00  	movn	$19, $2, $3
  19ec14: ff 7f 02 3c  	lui	$2, 0x7fff
  19ec18: ff ff 42 34  	ori	$2, $2, 0xffff
  19ec1c: 00 80 14 34  	ori	$20, $zero, 0x8000
  19ec20: 38 a4 14 00  	dsll	$20, $20, 0x10
  19ec24: 0a a0 5e 00  	movz	$20, $2, $fp
  19ec28: 2d a8 60 02  	move	$21, $19
  19ec2c: 2d 20 80 02  	move	$4, $20
  19ec30: 1e 8b 06 0c  	jal	0x1a2c78 <.text+0xa2c78>
  19ec34: 2d 28 a0 02  	move	$5, $21
  19ec38: 2d 90 00 00  	move	$18, $zero
  19ec3c: 2d 20 80 02  	move	$4, $20
  19ec40: 3c 10 02 00  	dsll32	$2, $2, 0x0
  19ec44: 3f 10 02 00  	dsra32	$2, $2, 0x0
  19ec48: 2d 28 a0 02  	move	$5, $21
  19ec4c: 6c 89 06 0c  	jal	0x1a25b0 <.text+0xa25b0>
  19ec50: 04 00 a2 af  	sw	$2, 0x4($sp)
  19ec54: 2d b0 00 00  	move	$22, $zero
  19ec58: 2d a0 40 00  	move	$20, $2
  19ec5c: a0 7b 06 0c  	jal	0x19ee80 <.text+0x9ee80>
  19ec60: 2d 20 00 02  	move	$4, $16
  19ec64: 36 00 40 10  	beqz	$2, 0x19ed40 <.text+0x9ed40>
  19ec68: 00 00 00 00  	nop
  19ec6c: d0 ff 10 26  	addiu	$16, $16, -0x30 <.text+0xffffffffffefffd0>
  19ec70: 2a 10 13 02  	slt	$2, $16, $19
  19ec74: 14 00 40 10  	beqz	$2, 0x19ecc8 <.text+0x9ecc8>
  19ec78: 00 00 00 00  	nop
  19ec7c: 10 00 c0 06  	bltz	$22, 0x19ecc0 <.text+0x9ecc0>
  19ec80: 2b 10 92 02  	sltu	$2, $20, $18
  19ec84: 08 00 40 14  	bnez	$2, 0x19eca8 <.text+0x9eca8>
  19ec88: ff ff 16 24  	addiu	$22, $zero, -0x1 <.text+0xffffffffffefffff>
  19ec8c: 09 00 54 12  	beq	$18, $20, 0x19ecb4 <.text+0x9ecb4>
  19ec90: 04 00 a3 8f  	lw	$3, 0x4($sp)
  19ec94: 2d 20 40 02  	move	$4, $18
  19ec98: c8 86 06 0c  	jal	0x1a1b20 <.text+0xa1b20>
  19ec9c: 2d 28 a0 02  	move	$5, $21
  19eca0: 01 00 16 24  	addiu	$22, $zero, 0x1
  19eca4: 2d 90 02 02  	daddu	$18, $16, $2
  19eca8: 00 00 30 82  	lb	$16, 0x0($17)
  19ecac: eb ff 00 10  	b	0x19ec5c <.text+0x9ec5c>
  19ecb0: 01 00 31 26  	addiu	$17, $17, 0x1
  19ecb4: 2a 10 70 00  	slt	$2, $3, $16
  19ecb8: f7 ff 40 10  	beqz	$2, 0x19ec98 <.text+0x9ec98>
  19ecbc: 2d 20 40 02  	move	$4, $18
  19ecc0: f9 ff 00 10  	b	0x19eca8 <.text+0x9eca8>
  19ecc4: ff ff 16 24  	addiu	$22, $zero, -0x1 <.text+0xffffffffffefffff>
  19ecc8: 15 00 c0 06  	bltz	$22, 0x19ed20 <.text+0x9ed20>
  19eccc: 2f 10 12 00  	dnegu	$2, $18
  19ecd0: 0b 90 5e 00  	movn	$18, $2, $fp
  19ecd4: 00 00 a2 8f  	lw	$2, 0x0($sp)
  19ecd8: 04 00 40 10  	beqz	$2, 0x19ecec <.text+0x9ecec>
  19ecdc: ff ff 22 26  	addiu	$2, $17, -0x1 <.text+0xffffffffffefffff>
  19ece0: 00 00 a3 8f  	lw	$3, 0x0($sp)
  19ece4: 0b b8 56 00  	movn	$23, $2, $22
  19ece8: 00 00 77 ac  	sw	$23, 0x0($3)
  19ecec: 2d 10 40 02  	move	$2, $18
  19ecf0: a0 00 bf df  	ld	$ra, 0xa0($sp)
  19ecf4: 90 00 be df  	ld	$fp, 0x90($sp)
  19ecf8: 80 00 b7 df  	ld	$23, 0x80($sp)
  19ecfc: 70 00 b6 df  	ld	$22, 0x70($sp)
  19ed00: 60 00 b5 df  	ld	$21, 0x60($sp)
  19ed04: 50 00 b4 df  	ld	$20, 0x50($sp)
  19ed08: 40 00 b3 df  	ld	$19, 0x40($sp)
  19ed0c: 30 00 b2 df  	ld	$18, 0x30($sp)
  19ed10: 20 00 b1 df  	ld	$17, 0x20($sp)
  19ed14: 10 00 b0 df  	ld	$16, 0x10($sp)
  19ed18: 08 00 e0 03  	jr	$ra
  19ed1c: b0 00 bd 27  	addiu	$sp, $sp, 0xb0
  19ed20: ff 7f 02 3c  	lui	$2, 0x7fff
  19ed24: ff ff 42 34  	ori	$2, $2, 0xffff
  19ed28: 00 80 12 3c  	lui	$18, 0x8000
  19ed2c: 0a 90 5e 00  	movz	$18, $2, $fp
  19ed30: 22 00 03 24  	addiu	$3, $zero, 0x22
  19ed34: 42 00 02 3c  	lui	$2, 0x42
  19ed38: e6 ff 00 10  	b	0x19ecd4 <.text+0x9ecd4>
  19ed3c: 70 5a 43 ac  	sw	$3, 0x5a70($2)
  19ed40: 8d 7b 06 0c  	jal	0x19ee34 <.text+0x9ee34>
  19ed44: 2d 20 00 02  	move	$4, $16
  19ed48: df ff 40 10  	beqz	$2, 0x19ecc8 <.text+0x9ecc8>
  19ed4c: 00 00 00 00  	nop
  19ed50: 83 7b 06 0c  	jal	0x19ee0c <.text+0x9ee0c>
  19ed54: 2d 20 00 02  	move	$4, $16
  19ed58: c9 ff 03 26  	addiu	$3, $16, -0x37 <.text+0xffffffffffefffc9>
  19ed5c: a9 ff 04 26  	addiu	$4, $16, -0x57 <.text+0xffffffffffefffa9>
  19ed60: 2d 80 60 00  	move	$16, $3
  19ed64: c2 ff 00 10  	b	0x19ec70 <.text+0x9ec70>
  19ed68: 0a 80 82 00  	movz	$16, $4, $2
  19ed6c: a4 ff 02 16  	bne	$16, $2, 0x19ec00 <.text+0x9ec00>
  19ed70: 78 00 02 24  	addiu	$2, $zero, 0x78
  19ed74: 00 00 23 82  	lb	$3, 0x0($17)
  19ed78: 03 00 62 10  	beq	$3, $2, 0x19ed88 <.text+0x9ed88>
  19ed7c: 58 00 02 24  	addiu	$2, $zero, 0x58
  19ed80: 9f ff 62 14  	bne	$3, $2, 0x19ec00 <.text+0x9ec00>
  19ed84: 00 00 00 00  	nop
  19ed88: 01 00 30 82  	lb	$16, 0x1($17)
  19ed8c: 10 00 13 24  	addiu	$19, $zero, 0x10
  19ed90: 9b ff 00 10  	b	0x19ec00 <.text+0x9ec00>
  19ed94: 02 00 31 26  	addiu	$17, $17, 0x2
  19ed98: 94 ff 00 10  	b	0x19ebec <.text+0x9ebec>
  19ed9c: 01 00 31 26  	addiu	$17, $17, 0x1
  19eda0: 00 00 30 82  	lb	$16, 0x0($17)
  19eda4: fc ff 00 10  	b	0x19ed98 <.text+0x9ed98>
  19eda8: 01 00 1e 24  	addiu	$fp, $zero, 0x1
  19edac: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19edb0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19edb4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19edb8: 83 7b 06 0c  	jal	0x19ee0c <.text+0x9ee0c>
  19edbc: 2d 80 80 00  	move	$16, $4
  19edc0: 20 00 03 26  	addiu	$3, $16, 0x20
  19edc4: 10 00 bf df  	ld	$ra, 0x10($sp)
  19edc8: 0b 80 62 00  	movn	$16, $3, $2
  19edcc: 2d 10 00 02  	move	$2, $16
  19edd0: 00 00 b0 df  	ld	$16, 0x0($sp)
  19edd4: 08 00 e0 03  	jr	$ra
  19edd8: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19eddc: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19ede0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19ede4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19ede8: 88 7b 06 0c  	jal	0x19ee20 <.text+0x9ee20>
  19edec: 2d 80 80 00  	move	$16, $4
  19edf0: e0 ff 03 26  	addiu	$3, $16, -0x20 <.text+0xffffffffffefffe0>
  19edf4: 10 00 bf df  	ld	$ra, 0x10($sp)
  19edf8: 0b 80 62 00  	movn	$16, $3, $2
  19edfc: 2d 10 00 02  	move	$2, $16
  19ee00: 00 00 b0 df  	ld	$16, 0x0($sp)
  19ee04: 08 00 e0 03  	jr	$ra
  19ee08: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19ee0c: 41 00 83 28  	slti	$3, $4, 0x41
  19ee10: 5b 00 82 28  	slti	$2, $4, 0x5b
  19ee14: 00 00 63 38  	xori	$3, $3, 0x0
  19ee18: 08 00 e0 03  	jr	$ra
  19ee1c: 0b 10 03 00  	movn	$2, $zero, $3
  19ee20: 61 00 83 28  	slti	$3, $4, 0x61
  19ee24: 7b 00 82 28  	slti	$2, $4, 0x7b
  19ee28: 00 00 63 38  	xori	$3, $3, 0x0
  19ee2c: 08 00 e0 03  	jr	$ra
  19ee30: 0b 10 03 00  	movn	$2, $zero, $3
  19ee34: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19ee38: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19ee3c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19ee40: 88 7b 06 0c  	jal	0x19ee20 <.text+0x9ee20>
  19ee44: 2d 80 80 00  	move	$16, $4
  19ee48: 07 00 40 10  	beqz	$2, 0x19ee68 <.text+0x9ee68>
  19ee4c: 2d 20 00 02  	move	$4, $16
  19ee50: 01 00 03 24  	addiu	$3, $zero, 0x1
  19ee54: 10 00 bf df  	ld	$ra, 0x10($sp)
  19ee58: 2d 10 60 00  	move	$2, $3
  19ee5c: 00 00 b0 df  	ld	$16, 0x0($sp)
  19ee60: 08 00 e0 03  	jr	$ra
  19ee64: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19ee68: 83 7b 06 0c  	jal	0x19ee0c <.text+0x9ee0c>
  19ee6c: 00 00 00 00  	nop
  19ee70: f8 ff 40 10  	beqz	$2, 0x19ee54 <.text+0x9ee54>
  19ee74: 2d 18 00 00  	move	$3, $zero
  19ee78: f6 ff 00 10  	b	0x19ee54 <.text+0x9ee54>
  19ee7c: 01 00 03 24  	addiu	$3, $zero, 0x1
  19ee80: 30 00 83 28  	slti	$3, $4, 0x30
  19ee84: 3a 00 82 28  	slti	$2, $4, 0x3a
  19ee88: 00 00 63 38  	xori	$3, $3, 0x0
  19ee8c: 08 00 e0 03  	jr	$ra
  19ee90: 0b 10 03 00  	movn	$2, $zero, $3
  19ee94: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19ee98: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19ee9c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19eea0: 8d 7b 06 0c  	jal	0x19ee34 <.text+0x9ee34>
  19eea4: 2d 80 80 00  	move	$16, $4
  19eea8: 07 00 40 10  	beqz	$2, 0x19eec8 <.text+0x9eec8>
  19eeac: 2d 20 00 02  	move	$4, $16
  19eeb0: 01 00 03 24  	addiu	$3, $zero, 0x1
  19eeb4: 10 00 bf df  	ld	$ra, 0x10($sp)
  19eeb8: 2d 10 60 00  	move	$2, $3
  19eebc: 00 00 b0 df  	ld	$16, 0x0($sp)
  19eec0: 08 00 e0 03  	jr	$ra
  19eec4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19eec8: a0 7b 06 0c  	jal	0x19ee80 <.text+0x9ee80>
  19eecc: 00 00 00 00  	nop
  19eed0: f8 ff 40 10  	beqz	$2, 0x19eeb4 <.text+0x9eeb4>
  19eed4: 2d 18 00 00  	move	$3, $zero
  19eed8: f6 ff 00 10  	b	0x19eeb4 <.text+0x9eeb4>
  19eedc: 01 00 03 24  	addiu	$3, $zero, 0x1
  19eee0: 20 00 82 28  	slti	$2, $4, 0x20
  19eee4: 03 00 40 14  	bnez	$2, 0x19eef4 <.text+0x9eef4>
  19eee8: 01 00 03 24  	addiu	$3, $zero, 0x1
  19eeec: 7f 00 82 38  	xori	$2, $4, 0x7f
  19eef0: 01 00 43 2c  	sltiu	$3, $2, 0x1
  19eef4: 08 00 e0 03  	jr	$ra
  19eef8: 2d 10 60 00  	move	$2, $3
  19eefc: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19ef00: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19ef04: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19ef08: b8 7b 06 0c  	jal	0x19eee0 <.text+0x9eee0>
  19ef0c: 2d 80 80 00  	move	$16, $4
  19ef10: 06 00 40 10  	beqz	$2, 0x19ef2c <.text+0x9ef2c>
  19ef14: 2d 18 00 00  	move	$3, $zero
  19ef18: 10 00 bf df  	ld	$ra, 0x10($sp)
  19ef1c: 2d 10 60 00  	move	$2, $3
  19ef20: 00 00 b0 df  	ld	$16, 0x0($sp)
  19ef24: 08 00 e0 03  	jr	$ra
  19ef28: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19ef2c: eb 7b 06 0c  	jal	0x19efac <.text+0x9efac>
  19ef30: 2d 20 00 02  	move	$4, $16
  19ef34: f8 ff 00 10  	b	0x19ef18 <.text+0x9ef18>
  19ef38: 01 00 43 2c  	sltiu	$3, $2, 0x1
  19ef3c: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19ef40: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19ef44: b8 7b 06 0c  	jal	0x19eee0 <.text+0x9eee0>
  19ef48: 00 00 00 00  	nop
  19ef4c: 00 00 bf df  	ld	$ra, 0x0($sp)
  19ef50: 01 00 42 2c  	sltiu	$2, $2, 0x1
  19ef54: 08 00 e0 03  	jr	$ra
  19ef58: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19ef5c: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19ef60: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19ef64: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19ef68: b8 7b 06 0c  	jal	0x19eee0 <.text+0x9eee0>
  19ef6c: 2d 80 80 00  	move	$16, $4
  19ef70: 06 00 40 10  	beqz	$2, 0x19ef8c <.text+0x9ef8c>
  19ef74: 2d 18 00 00  	move	$3, $zero
  19ef78: 10 00 bf df  	ld	$ra, 0x10($sp)
  19ef7c: 2d 10 60 00  	move	$2, $3
  19ef80: 00 00 b0 df  	ld	$16, 0x0($sp)
  19ef84: 08 00 e0 03  	jr	$ra
  19ef88: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19ef8c: a5 7b 06 0c  	jal	0x19ee94 <.text+0x9ee94>
  19ef90: 2d 20 00 02  	move	$4, $16
  19ef94: f8 ff 40 14  	bnez	$2, 0x19ef78 <.text+0x9ef78>
  19ef98: 2d 18 00 00  	move	$3, $zero
  19ef9c: eb 7b 06 0c  	jal	0x19efac <.text+0x9efac>
  19efa0: 2d 20 00 02  	move	$4, $16
  19efa4: f4 ff 00 10  	b	0x19ef78 <.text+0x9ef78>
  19efa8: 01 00 43 2c  	sltiu	$3, $2, 0x1
  19efac: f7 ff 82 24  	addiu	$2, $4, -0x9 <.text+0xffffffffffeffff7>
  19efb0: 05 00 42 2c  	sltiu	$2, $2, 0x5
  19efb4: 03 00 40 14  	bnez	$2, 0x19efc4 <.text+0x9efc4>
  19efb8: 01 00 03 24  	addiu	$3, $zero, 0x1
  19efbc: 20 00 82 38  	xori	$2, $4, 0x20
  19efc0: 01 00 43 2c  	sltiu	$3, $2, 0x1
  19efc4: 08 00 e0 03  	jr	$ra
  19efc8: 2d 10 60 00  	move	$2, $3
  19efcc: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19efd0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19efd4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19efd8: a0 7b 06 0c  	jal	0x19ee80 <.text+0x9ee80>
  19efdc: 2d 80 80 00  	move	$16, $4
  19efe0: 07 00 40 14  	bnez	$2, 0x19f000 <.text+0x9f000>
  19efe4: 01 00 04 24  	addiu	$4, $zero, 0x1
  19efe8: 9f ff 02 26  	addiu	$2, $16, -0x61 <.text+0xffffffffffefff9f>
  19efec: 06 00 42 2c  	sltiu	$2, $2, 0x6
  19eff0: 04 00 40 14  	bnez	$2, 0x19f004 <.text+0x9f004>
  19eff4: 10 00 bf df  	ld	$ra, 0x10($sp)
  19eff8: bf ff 02 26  	addiu	$2, $16, -0x41 <.text+0xffffffffffefffbf>
  19effc: 06 00 44 2c  	sltiu	$4, $2, 0x6
  19f000: 10 00 bf df  	ld	$ra, 0x10($sp)
  19f004: 2d 10 80 00  	move	$2, $4
  19f008: 00 00 b0 df  	ld	$16, 0x0($sp)
  19f00c: 08 00 e0 03  	jr	$ra
  19f010: 20 00 bd 27  	addiu	$sp, $sp, 0x20
