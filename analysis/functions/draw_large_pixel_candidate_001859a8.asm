
build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  1859a8: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  1859ac: 36 00 02 3c  	lui	$2, 0x36
  1859b0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1859b4: ff 01 83 30  	andi	$3, $4, 0x1ff
  1859b8: 50 d4 51 24  	addiu	$17, $2, -0x2bb0 <.text+0xffffffffffefd450>
  1859bc: 30 00 b3 ff  	sd	$19, 0x30($sp)
  1859c0: 08 00 2a 8e  	lw	$10, 0x8($17)
  1859c4: ff 03 82 30  	andi	$2, $4, 0x3ff
  1859c8: 2d 98 80 00  	move	$19, $4
  1859cc: 00 01 63 2c  	sltiu	$3, $3, 0x100
  1859d0: 0c 00 24 8e  	lw	$4, 0xc($17)
  1859d4: 04 10 42 01  	sllv	$2, $2, $10
  1859d8: 80 00 be ff  	sd	$fp, 0x80($sp)
  1859dc: 2d f0 a0 00  	move	$fp, $5
  1859e0: 70 00 b7 ff  	sd	$23, 0x70($sp)
  1859e4: 21 28 82 00  	addu	$5, $4, $2
  1859e8: 60 00 b6 ff  	sd	$22, 0x60($sp)
  1859ec: 2d b8 e0 00  	move	$23, $7
  1859f0: 50 00 b5 ff  	sd	$21, 0x50($sp)
  1859f4: 2d b0 20 01  	move	$22, $9
  1859f8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1859fc: 2d a8 00 01  	move	$21, $8
  185a00: 90 00 bf ff  	sd	$ra, 0x90($sp)
  185a04: 2d 90 c0 00  	move	$18, $6
  185a08: 40 00 b4 ff  	sd	$20, 0x40($sp)
  185a0c: 03 00 60 14  	bnez	$3, 0x185a1c <.text+0x85a1c>
  185a10: 00 00 b0 ff  	sd	$16, 0x0($sp)
  185a14: 10 00 22 8e  	lw	$2, 0x10($17)
  185a18: 21 28 a2 00  	addu	$5, $5, $2
  185a1c: ff ff a5 30  	andi	$5, $5, 0xffff
  185a20: 28 00 22 8e  	lw	$2, 0x28($17)
  185a24: 06 80 45 01  	srlv	$16, $5, $10
  185a28: 24 00 24 8e  	lw	$4, 0x24($17)
  185a2c: 21 10 50 00  	addu	$2, $2, $16
  185a30: 80 19 10 00  	sll	$3, $16, 0x6
  185a34: 00 00 42 90  	lbu	$2, 0x0($2)
  185a38: ce 00 40 10  	beqz	$2, 0x185d74 <.text+0x85d74>
  185a3c: 21 a0 83 00  	addu	$20, $4, $3
  185a40: 28 00 22 8e  	lw	$2, 0x28($17)
  185a44: 21 10 50 00  	addu	$2, $2, $16
  185a48: 00 00 43 90  	lbu	$3, 0x0($2)
  185a4c: 02 00 02 24  	addiu	$2, $zero, 0x2
  185a50: 3c 00 62 10  	beq	$3, $2, 0x185b44 <.text+0x85b44>
  185a54: 90 00 bf df  	ld	$ra, 0x90($sp)
  185a58: 2c 00 22 92  	lbu	$2, 0x2c($17)
  185a5c: bb 00 40 50  	beqzl	$2, 0x185d4c <.text+0x85d4c>
  185a60: 20 00 22 8e  	lw	$2, 0x20($17)
  185a64: 36 00 02 3c  	lui	$2, 0x36
  185a68: 6f c2 42 90  	lbu	$2, -0x3d91($2)
  185a6c: b3 00 40 14  	bnez	$2, 0x185d3c <.text+0x85d3c>
  185a70: 00 00 00 00  	nop
  185a74: 20 00 22 8e  	lw	$2, 0x20($17)
  185a78: 82 1a 13 00  	srl	$3, $19, 0xa
  185a7c: 24 18 62 00  	and	$3, $3, $2
  185a80: 3f 00 02 3c  	lui	$2, 0x3f
  185a84: 40 1a 03 00  	sll	$3, $3, 0x9
  185a88: 80 2f 42 24  	addiu	$2, $2, 0x2f80
  185a8c: 21 18 62 00  	addu	$3, $3, $2
  185a90: 00 c0 64 32  	andi	$4, $19, 0xc000
  185a94: 36 00 02 3c  	lui	$2, 0x36
  185a98: c4 d4 43 ac  	sw	$3, -0x2b3c($2)
  185a9c: 36 00 02 3c  	lui	$2, 0x36
  185aa0: 80 d4 46 24  	addiu	$6, $2, -0x2b80 <.text+0xffffffffffefd480>
  185aa4: 3c 00 c2 8c  	lw	$2, 0x3c($6)
  185aa8: 40 00 c3 8c  	lw	$3, 0x40($6)
  185aac: 21 40 5e 00  	addu	$8, $2, $fp
  185ab0: 2f 00 80 14  	bnez	$4, 0x185b70 <.text+0x85b70>
  185ab4: 21 28 7e 00  	addu	$5, $3, $fp
  185ab8: 21 10 95 02  	addu	$2, $20, $21
  185abc: 21 10 52 00  	addu	$2, $2, $18
  185ac0: 00 00 44 90  	lbu	$4, 0x0($2)
  185ac4: 1f 00 80 10  	beqz	$4, 0x185b44 <.text+0x85b44>
  185ac8: 90 00 bf df  	ld	$ra, 0x90($sp)
  185acc: 44 00 c3 8c  	lw	$3, 0x44($6)
  185ad0: 40 10 04 00  	sll	$2, $4, 0x1
  185ad4: 2d 48 c0 02  	move	$9, $22
  185ad8: 21 10 43 00  	addu	$2, $2, $3
  185adc: 19 00 c0 12  	beqz	$22, 0x185b44 <.text+0x85b44>
  185ae0: 00 00 44 90  	lbu	$4, 0x0($2)
  185ae4: ff ff e7 26  	addiu	$7, $23, -0x1 <.text+0xffffffffffefffff>
  185ae8: 10 00 e0 04  	bltz	$7, 0x185b2c <.text+0x85b2c>
  185aec: 36 00 02 3c  	lui	$2, 0x36
  185af0: 36 00 02 3c  	lui	$2, 0x36
  185af4: 21 30 a7 00  	addu	$6, $5, $7
  185af8: 80 d4 4b 24  	addiu	$11, $2, -0x2b80 <.text+0xffffffffffefd480>
  185afc: 21 50 07 01  	addu	$10, $8, $7
  185b00: 4c 00 63 91  	lbu	$3, 0x4c($11)
  185b04: 00 00 c2 90  	lbu	$2, 0x0($6)
  185b08: 2b 10 43 00  	sltu	$2, $2, $3
  185b0c: 04 00 40 10  	beqz	$2, 0x185b20 <.text+0x85b20>
  185b10: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  185b14: 00 00 44 a1  	sb	$4, 0x0($10)
  185b18: 4d 00 62 91  	lbu	$2, 0x4d($11)
  185b1c: 00 00 c2 a0  	sb	$2, 0x0($6)
  185b20: f4 ff e1 04  	bgez	$7, 0x185af4 <.text+0x85af4>
  185b24: 36 00 02 3c  	lui	$2, 0x36
  185b28: 36 00 02 3c  	lui	$2, 0x36
  185b2c: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  185b30: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  185b34: 21 28 a2 00  	addu	$5, $5, $2
  185b38: ea ff 20 15  	bnez	$9, 0x185ae4 <.text+0x85ae4>
  185b3c: 21 40 02 01  	addu	$8, $8, $2
  185b40: 90 00 bf df  	ld	$ra, 0x90($sp)
  185b44: 80 00 be df  	ld	$fp, 0x80($sp)
  185b48: 70 00 b7 df  	ld	$23, 0x70($sp)
  185b4c: 60 00 b6 df  	ld	$22, 0x60($sp)
  185b50: 50 00 b5 df  	ld	$21, 0x50($sp)
  185b54: 40 00 b4 df  	ld	$20, 0x40($sp)
  185b58: 30 00 b3 df  	ld	$19, 0x30($sp)
  185b5c: 20 00 b2 df  	ld	$18, 0x20($sp)
  185b60: 10 00 b1 df  	ld	$17, 0x10($sp)
  185b64: 00 00 b0 df  	ld	$16, 0x0($sp)
  185b68: 08 00 e0 03  	jr	$ra
  185b6c: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  185b70: 00 80 62 32  	andi	$2, $19, 0x8000
  185b74: 27 00 40 14  	bnez	$2, 0x185c14 <.text+0x85c14>
  185b78: 00 40 62 32  	andi	$2, $19, 0x4000
  185b7c: 07 00 03 24  	addiu	$3, $zero, 0x7
  185b80: 21 10 95 02  	addu	$2, $20, $21
  185b84: 23 90 72 00  	subu	$18, $3, $18
  185b88: 21 10 52 00  	addu	$2, $2, $18
  185b8c: 00 00 44 90  	lbu	$4, 0x0($2)
  185b90: eb ff 80 10  	beqz	$4, 0x185b40 <.text+0x85b40>
  185b94: 36 00 03 3c  	lui	$3, 0x36
  185b98: 40 10 04 00  	sll	$2, $4, 0x1
  185b9c: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  185ba0: 2d 48 c0 02  	move	$9, $22
  185ba4: 21 10 43 00  	addu	$2, $2, $3
  185ba8: e5 ff c0 12  	beqz	$22, 0x185b40 <.text+0x85b40>
  185bac: 00 00 44 90  	lbu	$4, 0x0($2)
  185bb0: ff ff e7 26  	addiu	$7, $23, -0x1 <.text+0xffffffffffefffff>
  185bb4: 10 00 e0 04  	bltz	$7, 0x185bf8 <.text+0x85bf8>
  185bb8: 36 00 02 3c  	lui	$2, 0x36
  185bbc: 36 00 02 3c  	lui	$2, 0x36
  185bc0: 21 30 a7 00  	addu	$6, $5, $7
  185bc4: 80 d4 4b 24  	addiu	$11, $2, -0x2b80 <.text+0xffffffffffefd480>
  185bc8: 21 50 07 01  	addu	$10, $8, $7
  185bcc: 4c 00 63 91  	lbu	$3, 0x4c($11)
  185bd0: 00 00 c2 90  	lbu	$2, 0x0($6)
  185bd4: 2b 10 43 00  	sltu	$2, $2, $3
  185bd8: 04 00 40 10  	beqz	$2, 0x185bec <.text+0x85bec>
  185bdc: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  185be0: 00 00 44 a1  	sb	$4, 0x0($10)
  185be4: 4d 00 62 91  	lbu	$2, 0x4d($11)
  185be8: 00 00 c2 a0  	sb	$2, 0x0($6)
  185bec: f4 ff e1 04  	bgez	$7, 0x185bc0 <.text+0x85bc0>
  185bf0: 36 00 02 3c  	lui	$2, 0x36
  185bf4: 36 00 02 3c  	lui	$2, 0x36
  185bf8: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  185bfc: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  185c00: 21 28 a2 00  	addu	$5, $5, $2
  185c04: ea ff 20 15  	bnez	$9, 0x185bb0 <.text+0x85bb0>
  185c08: 21 40 02 01  	addu	$8, $8, $2
  185c0c: cd ff 00 10  	b	0x185b44 <.text+0x85b44>
  185c10: 90 00 bf df  	ld	$ra, 0x90($sp)
  185c14: 26 00 40 10  	beqz	$2, 0x185cb0 <.text+0x85cb0>
  185c18: 23 10 95 02  	subu	$2, $20, $21
  185c1c: 07 00 03 24  	addiu	$3, $zero, 0x7
  185c20: 23 90 72 00  	subu	$18, $3, $18
  185c24: 21 10 52 00  	addu	$2, $2, $18
  185c28: 38 00 44 90  	lbu	$4, 0x38($2)
  185c2c: c4 ff 80 10  	beqz	$4, 0x185b40 <.text+0x85b40>
  185c30: 36 00 03 3c  	lui	$3, 0x36
  185c34: 40 10 04 00  	sll	$2, $4, 0x1
  185c38: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  185c3c: 2d 48 c0 02  	move	$9, $22
  185c40: 21 10 43 00  	addu	$2, $2, $3
  185c44: be ff c0 12  	beqz	$22, 0x185b40 <.text+0x85b40>
  185c48: 00 00 44 90  	lbu	$4, 0x0($2)
  185c4c: ff ff e7 26  	addiu	$7, $23, -0x1 <.text+0xffffffffffefffff>
  185c50: 10 00 e0 04  	bltz	$7, 0x185c94 <.text+0x85c94>
  185c54: 36 00 02 3c  	lui	$2, 0x36
  185c58: 36 00 02 3c  	lui	$2, 0x36
  185c5c: 21 30 a7 00  	addu	$6, $5, $7
  185c60: 80 d4 4b 24  	addiu	$11, $2, -0x2b80 <.text+0xffffffffffefd480>
  185c64: 21 50 07 01  	addu	$10, $8, $7
  185c68: 4c 00 63 91  	lbu	$3, 0x4c($11)
  185c6c: 00 00 c2 90  	lbu	$2, 0x0($6)
  185c70: 2b 10 43 00  	sltu	$2, $2, $3
  185c74: 04 00 40 10  	beqz	$2, 0x185c88 <.text+0x85c88>
  185c78: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  185c7c: 00 00 44 a1  	sb	$4, 0x0($10)
  185c80: 4d 00 62 91  	lbu	$2, 0x4d($11)
  185c84: 00 00 c2 a0  	sb	$2, 0x0($6)
  185c88: f4 ff e1 04  	bgez	$7, 0x185c5c <.text+0x85c5c>
  185c8c: 36 00 02 3c  	lui	$2, 0x36
  185c90: 36 00 02 3c  	lui	$2, 0x36
  185c94: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  185c98: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  185c9c: 21 28 a2 00  	addu	$5, $5, $2
  185ca0: ea ff 20 15  	bnez	$9, 0x185c4c <.text+0x85c4c>
  185ca4: 21 40 02 01  	addu	$8, $8, $2
  185ca8: a6 ff 00 10  	b	0x185b44 <.text+0x85b44>
  185cac: 90 00 bf df  	ld	$ra, 0x90($sp)
  185cb0: 21 10 52 00  	addu	$2, $2, $18
  185cb4: 38 00 44 90  	lbu	$4, 0x38($2)
  185cb8: a1 ff 80 10  	beqz	$4, 0x185b40 <.text+0x85b40>
  185cbc: 36 00 03 3c  	lui	$3, 0x36
  185cc0: 40 10 04 00  	sll	$2, $4, 0x1
  185cc4: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  185cc8: 2d 48 c0 02  	move	$9, $22
  185ccc: 21 10 43 00  	addu	$2, $2, $3
  185cd0: 9b ff c0 12  	beqz	$22, 0x185b40 <.text+0x85b40>
  185cd4: 00 00 44 90  	lbu	$4, 0x0($2)
  185cd8: ff ff e7 26  	addiu	$7, $23, -0x1 <.text+0xffffffffffefffff>
  185cdc: 10 00 e0 04  	bltz	$7, 0x185d20 <.text+0x85d20>
  185ce0: 36 00 02 3c  	lui	$2, 0x36
  185ce4: 36 00 02 3c  	lui	$2, 0x36
  185ce8: 21 30 a7 00  	addu	$6, $5, $7
  185cec: 80 d4 4b 24  	addiu	$11, $2, -0x2b80 <.text+0xffffffffffefd480>
  185cf0: 21 50 07 01  	addu	$10, $8, $7
  185cf4: 4c 00 63 91  	lbu	$3, 0x4c($11)
  185cf8: 00 00 c2 90  	lbu	$2, 0x0($6)
  185cfc: 2b 10 43 00  	sltu	$2, $2, $3
  185d00: 04 00 40 10  	beqz	$2, 0x185d14 <.text+0x85d14>
  185d04: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  185d08: 00 00 44 a1  	sb	$4, 0x0($10)
  185d0c: 4d 00 62 91  	lbu	$2, 0x4d($11)
  185d10: 00 00 c2 a0  	sb	$2, 0x0($6)
  185d14: f4 ff e1 04  	bgez	$7, 0x185ce8 <.text+0x85ce8>
  185d18: 36 00 02 3c  	lui	$2, 0x36
  185d1c: 36 00 02 3c  	lui	$2, 0x36
  185d20: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  185d24: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  185d28: 21 28 a2 00  	addu	$5, $5, $2
  185d2c: ea ff 20 15  	bnez	$9, 0x185cd8 <.text+0x85cd8>
  185d30: 21 40 02 01  	addu	$8, $8, $2
  185d34: 83 ff 00 10  	b	0x185b44 <.text+0x85b44>
  185d38: 90 00 bf df  	ld	$ra, 0x90($sp)
  185d3c: 23 0c 05 0c  	jal	0x14308c <.text+0x4308c>
  185d40: 00 00 00 00  	nop
  185d44: 4c ff 00 10  	b	0x185a78 <.text+0x85a78>
  185d48: 20 00 22 8e  	lw	$2, 0x20($17)
  185d4c: 82 1a 13 00  	srl	$3, $19, 0xa
  185d50: 1c 00 24 8e  	lw	$4, 0x1c($17)
  185d54: 24 18 62 00  	and	$3, $3, $2
  185d58: 18 00 22 8e  	lw	$2, 0x18($17)
  185d5c: 04 18 83 00  	sllv	$3, $3, $4
  185d60: 21 18 62 00  	addu	$3, $3, $2
  185d64: 36 00 02 3c  	lui	$2, 0x36
  185d68: 40 18 03 00  	sll	$3, $3, 0x1
  185d6c: 47 ff 00 10  	b	0x185a8c <.text+0x85a8c>
  185d70: b0 ce 42 24  	addiu	$2, $2, -0x3150 <.text+0xffffffffffefceb0>
  185d74: 81 0f 06 0c  	jal	0x183e04 <.text+0x83e04>
  185d78: 2d 20 80 02  	move	$4, $20
  185d7c: 28 00 23 8e  	lw	$3, 0x28($17)
  185d80: 21 18 70 00  	addu	$3, $3, $16
  185d84: 2e ff 00 10  	b	0x185a40 <.text+0x85a40>
  185d88: 00 00 62 a0  	sb	$2, 0x0($3)
