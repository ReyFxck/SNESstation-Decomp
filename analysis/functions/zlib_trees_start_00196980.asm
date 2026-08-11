  196980: 08 00 e0 03  	jr	$ra
  196984: 00 00 00 00  	nop
  196988: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19698c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  196990: 10 00 bf ff  	sd	$ra, 0x10($sp)
  196994: 60 5a 06 0c  	jal	0x196980 <.text+0x96980>
  196998: 2d 80 80 00  	move	$16, $4
  19699c: a4 00 02 26  	addiu	$2, $16, 0xa4
  1969a0: 98 09 03 26  	addiu	$3, $16, 0x998
  1969a4: 28 0b 02 ae  	sw	$2, 0xb28($16)
  1969a8: 8c 0a 05 26  	addiu	$5, $16, 0xa8c
  1969ac: 42 00 02 3c  	lui	$2, 0x42
  1969b0: 34 0b 03 ae  	sw	$3, 0xb34($16)
  1969b4: b8 59 42 24  	addiu	$2, $2, 0x59b8
  1969b8: 40 0b 05 ae  	sw	$5, 0xb40($16)
  1969bc: 30 0b 02 ae  	sw	$2, 0xb30($16)
  1969c0: 2d 20 00 02  	move	$4, $16
  1969c4: 42 00 02 3c  	lui	$2, 0x42
  1969c8: d0 16 00 a6  	sh	$zero, 0x16d0($16)
  1969cc: d0 59 42 24  	addiu	$2, $2, 0x59d0
  1969d0: d4 16 00 ae  	sw	$zero, 0x16d4($16)
  1969d4: 3c 0b 02 ae  	sw	$2, 0xb3c($16)
  1969d8: 42 00 02 3c  	lui	$2, 0x42
  1969dc: e8 59 42 24  	addiu	$2, $2, 0x59e8
  1969e0: 48 0b 02 ae  	sw	$2, 0xb48($16)
  1969e4: 08 00 02 24  	addiu	$2, $zero, 0x8
  1969e8: 80 5a 06 0c  	jal	0x196a00 <.text+0x96a00>
  1969ec: cc 16 02 ae  	sw	$2, 0x16cc($16)
  1969f0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1969f4: 10 00 bf df  	ld	$ra, 0x10($sp)
  1969f8: 08 00 e0 03  	jr	$ra
  1969fc: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  196a00: 1d 01 03 24  	addiu	$3, $zero, 0x11d
  196a04: a4 00 82 24  	addiu	$2, $4, 0xa4
  196a08: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  196a0c: 00 00 40 a4  	sh	$zero, 0x0($2)
  196a20: f9 ff 61 04  	bgez	$3, 0x196a08 <.text+0x96a08>
  196a24: 04 00 42 24  	addiu	$2, $2, 0x4
  196a28: 98 09 82 24  	addiu	$2, $4, 0x998
  196a2c: 1d 00 03 24  	addiu	$3, $zero, 0x1d
  196a30: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  196a34: 00 00 40 a4  	sh	$zero, 0x0($2)
  196a48: f9 ff 61 04  	bgez	$3, 0x196a30 <.text+0x96a30>
  196a4c: 04 00 42 24  	addiu	$2, $2, 0x4
  196a50: 8c 0a 82 24  	addiu	$2, $4, 0xa8c
  196a54: 12 00 03 24  	addiu	$3, $zero, 0x12
  196a58: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  196a5c: 00 00 40 a4  	sh	$zero, 0x0($2)
  196a70: f9 ff 61 04  	bgez	$3, 0x196a58 <.text+0x96a58>
  196a74: 04 00 42 24  	addiu	$2, $2, 0x4
  196a78: 01 00 02 24  	addiu	$2, $zero, 0x1
  196a7c: b0 16 80 ac  	sw	$zero, 0x16b0($4)
  196a80: a4 04 82 a4  	sh	$2, 0x4a4($4)
  196a84: c0 16 80 fc  	sd	$zero, 0x16c0($4)
  196a88: b8 16 80 fc  	sd	$zero, 0x16b8($4)
  196a8c: 08 00 e0 03  	jr	$ra
  196a90: c8 16 80 ac  	sw	$zero, 0x16c8($4)
  196a94: 60 14 82 8c  	lw	$2, 0x1460($4)
  196a98: 80 18 06 00  	sll	$3, $6, 0x2
  196a9c: 40 48 06 00  	sll	$9, $6, 0x1
  196aa0: 21 18 64 00  	addu	$3, $3, $4
  196aa4: 2d 68 40 00  	move	$13, $2
  196aa8: 2a 10 49 00  	slt	$2, $2, $9
  196aac: 29 00 40 14  	bnez	$2, 0x196b54 <.text+0x96b54>
  196ab0: 6c 0b 6c 8c  	lw	$12, 0xb6c($3)
  196ab4: 80 10 09 00  	sll	$2, $9, 0x2
  196ab8: 80 18 0c 00  	sll	$3, $12, 0x2
  196abc: 21 38 44 00  	addu	$7, $2, $4
  196ac0: 21 70 65 00  	addu	$14, $3, $5
  196ac4: 80 10 06 00  	sll	$2, $6, 0x2
  196ac8: 21 c0 44 00  	addu	$24, $2, $4
  196acc: 2a 10 2d 01  	slt	$2, $9, $13
  196ad0: 0f 00 40 10  	beqz	$2, 0x196b10 <.text+0x96b10>
  196ad4: 21 78 84 01  	addu	$15, $12, $4
  196ad8: 6c 0b eb 8c  	lw	$11, 0xb6c($7)
  196adc: 70 0b e8 8c  	lw	$8, 0xb70($7)
  196ae0: 80 10 0b 00  	sll	$2, $11, 0x2
  196ae4: 80 18 08 00  	sll	$3, $8, 0x2
  196ae8: 21 10 45 00  	addu	$2, $2, $5
  196aec: 21 18 65 00  	addu	$3, $3, $5
  196af0: 00 00 4a 94  	lhu	$10, 0x0($2)
  196af4: 00 00 67 94  	lhu	$7, 0x0($3)
  196af8: 2b 10 ea 00  	sltu	$2, $7, $10
  196afc: 04 00 40 54  	bnezl	$2, 0x196b10 <.text+0x96b10>
  196b00: 01 00 29 25  	addiu	$9, $9, 0x1
  196b04: 21 10 04 01  	addu	$2, $8, $4
  196b08: 1c 00 ea 10  	beq	$7, $10, 0x196b7c <.text+0x96b7c>
  196b0c: 21 40 64 01  	addu	$8, $11, $4
  196b10: 80 10 09 00  	sll	$2, $9, 0x2
  196b14: 00 00 c3 95  	lhu	$3, 0x0($14)
  196b18: 21 10 44 00  	addu	$2, $2, $4
  196b1c: 6c 0b 48 8c  	lw	$8, 0xb6c($2)
  196b20: 80 10 08 00  	sll	$2, $8, 0x2
  196b24: 21 10 45 00  	addu	$2, $2, $5
  196b28: 00 00 47 94  	lhu	$7, 0x0($2)
  196b2c: 2b 10 67 00  	sltu	$2, $3, $7
  196b30: 08 00 40 14  	bnez	$2, 0x196b54 <.text+0x96b54>
  196b34: 21 50 04 01  	addu	$10, $8, $4
  196b38: 0a 00 67 50  	beql	$3, $7, 0x196b64 <.text+0x96b64>
  196b3c: 68 14 e3 91  	lbu	$3, 0x1468($15)
  196b40: 2d 30 20 01  	move	$6, $9
  196b44: 40 48 09 00  	sll	$9, $9, 0x1
  196b48: 2a 10 a9 01  	slt	$2, $13, $9
  196b4c: d9 ff 40 10  	beqz	$2, 0x196ab4 <.text+0x96ab4>
  196b50: 6c 0b 08 af  	sw	$8, 0xb6c($24)
  196b54: 80 10 06 00  	sll	$2, $6, 0x2
  196b58: 21 10 44 00  	addu	$2, $2, $4
  196b5c: 08 00 e0 03  	jr	$ra
  196b60: 6c 0b 4c ac  	sw	$12, 0xb6c($2)
  196b64: 68 14 42 91  	lbu	$2, 0x1468($10)
  196b68: 2b 10 43 00  	sltu	$2, $2, $3
  196b6c: f5 ff 40 54  	bnezl	$2, 0x196b44 <.text+0x96b44>
  196b70: 2d 30 20 01  	move	$6, $9
  196b74: f8 ff 00 10  	b	0x196b58 <.text+0x96b58>
  196b78: 80 10 06 00  	sll	$2, $6, 0x2
  196b7c: 68 14 43 90  	lbu	$3, 0x1468($2)
  196b80: 68 14 02 91  	lbu	$2, 0x1468($8)
  196b84: 2b 10 43 00  	sltu	$2, $2, $3
  196b88: e2 ff 40 14  	bnez	$2, 0x196b14 <.text+0x96b14>
  196b8c: 80 10 09 00  	sll	$2, $9, 0x2
  196b90: df ff 00 10  	b	0x196b10 <.text+0x96b10>
  196b94: 01 00 29 25  	addiu	$9, $9, 0x1
  196b98: 40 ff bd 27  	addiu	$sp, $sp, -0xc0 <.text+0xffffffffffefff40>
  196b9c: 4c 0b 83 24  	addiu	$3, $4, 0xb4c
  196ba0: 50 00 b3 ff  	sd	$19, 0x50($sp)
  196ba4: 2d 98 80 00  	move	$19, $4
  196ba8: 30 00 b1 ff  	sd	$17, 0x30($sp)
  196bac: 0f 00 11 24  	addiu	$17, $zero, 0xf
  196bb0: b0 00 bf ff  	sd	$ra, 0xb0($sp)
  196bb4: a0 00 be ff  	sd	$fp, 0xa0($sp)
  196bb8: 90 00 b7 ff  	sd	$23, 0x90($sp)
  196bbc: 80 00 b6 ff  	sd	$22, 0x80($sp)
  196bc0: 70 00 b5 ff  	sd	$21, 0x70($sp)
  196bc4: 60 00 b4 ff  	sd	$20, 0x60($sp)
  196bc8: 40 00 b2 ff  	sd	$18, 0x40($sp)
  196bcc: 20 00 b0 ff  	sd	$16, 0x20($sp)
  196bd0: 10 00 a0 af  	sw	$zero, 0x10($sp)
  196bd4: 08 00 a2 8c  	lw	$2, 0x8($5)
  196bd8: 00 00 be 8c  	lw	$fp, 0x0($5)
  196bdc: 00 00 44 8c  	lw	$4, 0x0($2)
  196be0: 04 00 47 8c  	lw	$7, 0x4($2)
  196be4: 10 00 55 8c  	lw	$21, 0x10($2)
  196be8: 04 00 a5 8c  	lw	$5, 0x4($5)
  196bec: 08 00 42 8c  	lw	$2, 0x8($2)
  196bf0: 00 00 a5 af  	sw	$5, 0x0($sp)
  196bf4: 04 00 a4 af  	sw	$4, 0x4($sp)
  196bf8: 08 00 a7 af  	sw	$7, 0x8($sp)
  196bfc: 0c 00 a2 af  	sw	$2, 0xc($sp)
  196c00: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  196c04: 00 00 60 a4  	sh	$zero, 0x0($3)
  196c18: f9 ff 21 06  	bgez	$17, 0x196c00 <.text+0x96c00>
  196c1c: 02 00 63 24  	addiu	$3, $3, 0x2
  196c20: 64 14 62 8e  	lw	$2, 0x1464($19)
  196c24: 80 10 02 00  	sll	$2, $2, 0x2
  196c28: 21 10 53 00  	addu	$2, $2, $19
  196c2c: 6c 0b 42 8c  	lw	$2, 0xb6c($2)
  196c30: 80 10 02 00  	sll	$2, $2, 0x2
  196c34: 21 10 5e 00  	addu	$2, $2, $fp
  196c38: 02 00 40 a4  	sh	$zero, 0x2($2)
  196c3c: 64 14 62 8e  	lw	$2, 0x1464($19)
  196c40: 01 00 52 24  	addiu	$18, $2, 0x1
  196c44: 3d 02 42 2a  	slti	$2, $18, 0x23d
  196c48: 3c 00 40 10  	beqz	$2, 0x196d3c <.text+0x96d3c>
  196c4c: 80 10 12 00  	sll	$2, $18, 0x2
  196c50: 21 10 53 00  	addu	$2, $2, $19
  196c54: 6c 0b 54 24  	addiu	$20, $2, 0xb6c
  196c58: 00 00 90 8e  	lw	$16, 0x0($20)
  196c5c: 01 00 52 26  	addiu	$18, $18, 0x1
  196c60: 00 00 a2 8f  	lw	$2, 0x0($sp)
  196c64: 04 00 94 26  	addiu	$20, $20, 0x4
  196c68: 80 b8 10 00  	sll	$23, $16, 0x2
  196c6c: 0c 00 a4 8f  	lw	$4, 0xc($sp)
  196c70: 21 28 fe 02  	addu	$5, $23, $fp
  196c74: 2a 18 50 00  	slt	$3, $2, $16
  196c78: 02 00 a2 94  	lhu	$2, 0x2($5)
  196c7c: 2d b0 00 00  	move	$22, $zero
  196c80: 80 10 02 00  	sll	$2, $2, 0x2
  196c84: 21 10 5e 00  	addu	$2, $2, $fp
  196c88: 02 00 42 94  	lhu	$2, 0x2($2)
  196c8c: 01 00 51 24  	addiu	$17, $2, 0x1
  196c90: 2a 10 b1 02  	slt	$2, $21, $17
  196c94: 05 00 40 10  	beqz	$2, 0x196cac <.text+0x96cac>
  196c98: 2a 30 04 02  	slt	$6, $16, $4
  196c9c: 10 00 a7 8f  	lw	$7, 0x10($sp)
  196ca0: 2d 88 a0 02  	move	$17, $21
  196ca4: 01 00 e7 24  	addiu	$7, $7, 0x1
  196ca8: 10 00 a7 af  	sw	$7, 0x10($sp)
  196cac: 40 10 11 00  	sll	$2, $17, 0x1
  196cb0: 02 00 b1 a4  	sh	$17, 0x2($5)
  196cb4: 21 10 53 00  	addu	$2, $2, $19
  196cb8: 1d 00 60 14  	bnez	$3, 0x196d30 <.text+0x96d30>
  196cbc: 40 0b 44 24  	addiu	$4, $2, 0xb40
  196cc0: 0c 00 a3 8f  	lw	$3, 0xc($sp)
  196cc4: 08 00 a7 8f  	lw	$7, 0x8($sp)
  196cc8: 23 10 03 02  	subu	$2, $16, $3
  196ccc: 0c 00 83 94  	lhu	$3, 0xc($4)
  196cd0: 80 10 02 00  	sll	$2, $2, 0x2
  196cd4: 01 00 63 24  	addiu	$3, $3, 0x1
  196cd8: 21 10 47 00  	addu	$2, $2, $7
  196cdc: 02 00 c0 14  	bnez	$6, 0x196ce8 <.text+0x96ce8>
  196ce0: 0c 00 83 a4  	sh	$3, 0xc($4)
  196ce4: 00 00 56 8c  	lw	$22, 0x0($2)
  196ce8: 00 00 b0 94  	lhu	$16, 0x0($5)
  196cec: 21 28 36 02  	addu	$5, $17, $22
  196cf0: c8 86 06 0c  	jal	0x1a1b20 <.text+0xa1b20>
  196cf4: 2d 20 00 02  	move	$4, $16
  196cf8: 04 00 a7 8f  	lw	$7, 0x4($sp)
  196cfc: b8 16 63 de  	ld	$3, 0x16b8($19)
  196d00: 04 00 a4 8f  	lw	$4, 0x4($sp)
  196d04: 2d 18 62 00  	daddu	$3, $3, $2
  196d08: 21 28 e4 02  	addu	$5, $23, $4
  196d0c: b8 16 63 fe  	sd	$3, 0x16b8($19)
  196d10: 07 00 e0 10  	beqz	$7, 0x196d30 <.text+0x96d30>
  196d14: 2d 20 00 02  	move	$4, $16
  196d18: 02 00 a5 94  	lhu	$5, 0x2($5)
  196d1c: c8 86 06 0c  	jal	0x1a1b20 <.text+0xa1b20>
  196d20: 21 28 b6 00  	addu	$5, $5, $22
  196d24: c0 16 63 de  	ld	$3, 0x16c0($19)
  196d28: 2d 18 62 00  	daddu	$3, $3, $2
  196d2c: c0 16 63 fe  	sd	$3, 0x16c0($19)
  196d30: 3d 02 42 2a  	slti	$2, $18, 0x23d
  196d34: c9 ff 40 54  	bnezl	$2, 0x196c5c <.text+0x96c5c>
  196d38: 00 00 90 8e  	lw	$16, 0x0($20)
  196d3c: 10 00 a2 8f  	lw	$2, 0x10($sp)
  196d40: 44 00 40 10  	beqz	$2, 0x196e54 <.text+0x96e54>
  196d44: b0 00 bf df  	ld	$ra, 0xb0($sp)
  196d48: ff ff b1 26  	addiu	$17, $21, -0x1 <.text+0xffffffffffefffff>
  196d4c: 40 10 11 00  	sll	$2, $17, 0x1
  196d50: 21 18 53 00  	addu	$3, $2, $19
  196d54: 4c 0b 62 94  	lhu	$2, 0xb4c($3)
  196d58: 09 00 40 14  	bnez	$2, 0x196d80 <.text+0x96d80>
  196d5c: 4c 0b 63 24  	addiu	$3, $3, 0xb4c
  196d60: fe ff 63 24  	addiu	$3, $3, -0x2 <.text+0xffffffffffeffffe>
  196d64: 00 00 62 94  	lhu	$2, 0x0($3)
  196d78: f9 ff 40 10  	beqz	$2, 0x196d60 <.text+0x96d60>
  196d7c: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  196d80: 40 18 11 00  	sll	$3, $17, 0x1
  196d84: 40 10 15 00  	sll	$2, $21, 0x1
  196d88: 21 18 73 00  	addu	$3, $3, $19
  196d8c: 21 30 53 00  	addu	$6, $2, $19
  196d90: 40 0b 64 24  	addiu	$4, $3, 0xb40
  196d94: 40 0b c5 24  	addiu	$5, $6, 0xb40
  196d98: 0c 00 82 94  	lhu	$2, 0xc($4)
  196d9c: 42 0b 63 24  	addiu	$3, $3, 0xb42
  196da0: 10 00 a7 8f  	lw	$7, 0x10($sp)
  196da4: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  196da8: 0c 00 82 a4  	sh	$2, 0xc($4)
  196dac: fe ff e7 24  	addiu	$7, $7, -0x2 <.text+0xffffffffffeffffe>
  196db0: 10 00 a7 af  	sw	$7, 0x10($sp)
  196db4: 0c 00 62 94  	lhu	$2, 0xc($3)
  196db8: 02 00 42 24  	addiu	$2, $2, 0x2
  196dbc: 0c 00 62 a4  	sh	$2, 0xc($3)
  196dc0: 0c 00 a2 94  	lhu	$2, 0xc($5)
  196dc4: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  196dc8: df ff e0 1c  	bgtz	$7, 0x196d48 <.text+0x96d48>
  196dcc: 0c 00 a2 a4  	sh	$2, 0xc($5)
  196dd0: 1f 00 a0 12  	beqz	$21, 0x196e50 <.text+0x96e50>
  196dd4: 2d 88 a0 02  	move	$17, $21
  196dd8: 80 10 12 00  	sll	$2, $18, 0x2
  196ddc: 4c 0b d6 24  	addiu	$22, $6, 0xb4c
  196de0: 21 a8 53 00  	addu	$21, $2, $19
  196de4: 00 00 d0 96  	lhu	$16, 0x0($22)
  196de8: 17 00 00 52  	beqzl	$16, 0x196e48 <.text+0x96e48>
  196dec: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  196df0: 6c 0b b4 26  	addiu	$20, $21, 0xb6c
  196df4: fc ff 94 26  	addiu	$20, $20, -0x4 <.text+0xffffffffffeffffc>
  196df8: 00 00 a4 8f  	lw	$4, 0x0($sp)
  196dfc: 00 00 82 8e  	lw	$2, 0x0($20)
  196e00: fc ff b5 26  	addiu	$21, $21, -0x4 <.text+0xffffffffffeffffc>
  196e04: 80 18 02 00  	sll	$3, $2, 0x2
  196e08: 2a 10 82 00  	slt	$2, $4, $2
  196e0c: 0b 00 40 14  	bnez	$2, 0x196e3c <.text+0x96e3c>
  196e10: 21 90 7e 00  	addu	$18, $3, $fp
  196e14: 02 00 42 96  	lhu	$2, 0x2($18)
  196e18: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  196e1c: 07 00 22 12  	beq	$17, $2, 0x196e3c <.text+0x96e3c>
  196e20: 2f 20 22 02  	dsubu	$4, $17, $2
  196e24: c8 86 06 0c  	jal	0x1a1b20 <.text+0xa1b20>
  196e28: 00 00 45 96  	lhu	$5, 0x0($18)
  196e2c: b8 16 63 de  	ld	$3, 0x16b8($19)
  196e30: 2d 18 62 00  	daddu	$3, $3, $2
  196e34: b8 16 63 fe  	sd	$3, 0x16b8($19)
  196e38: 02 00 51 a6  	sh	$17, 0x2($18)
  196e3c: ee ff 00 56  	bnezl	$16, 0x196df8 <.text+0x96df8>
  196e40: fc ff 94 26  	addiu	$20, $20, -0x4 <.text+0xffffffffffeffffc>
  196e44: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  196e48: e6 ff 20 16  	bnez	$17, 0x196de4 <.text+0x96de4>
  196e4c: fe ff d6 26  	addiu	$22, $22, -0x2 <.text+0xffffffffffeffffe>
  196e50: b0 00 bf df  	ld	$ra, 0xb0($sp)
  196e54: a0 00 be df  	ld	$fp, 0xa0($sp)
  196e58: 90 00 b7 df  	ld	$23, 0x90($sp)
  196e5c: 80 00 b6 df  	ld	$22, 0x80($sp)
  196e60: 70 00 b5 df  	ld	$21, 0x70($sp)
  196e64: 60 00 b4 df  	ld	$20, 0x60($sp)
  196e68: 50 00 b3 df  	ld	$19, 0x50($sp)
  196e6c: 40 00 b2 df  	ld	$18, 0x40($sp)
  196e70: 30 00 b1 df  	ld	$17, 0x30($sp)
  196e74: 20 00 b0 df  	ld	$16, 0x20($sp)
  196e78: 08 00 e0 03  	jr	$ra
  196e7c: c0 00 bd 27  	addiu	$sp, $sp, 0xc0
  196e80: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  196e84: 2d 48 80 00  	move	$9, $4
  196e88: 40 00 bf ff  	sd	$ra, 0x40($sp)
  196e8c: 2d 40 00 00  	move	$8, $zero
  196e90: 30 00 b1 ff  	sd	$17, 0x30($sp)
  196e94: 01 00 07 24  	addiu	$7, $zero, 0x1
  196e98: 20 00 b0 ff  	sd	$16, 0x20($sp)
  196e9c: 40 18 07 00  	sll	$3, $7, 0x1
  196ea0: 01 00 e7 24  	addiu	$7, $7, 0x1
  196ea4: 21 10 c3 00  	addu	$2, $6, $3
  196ea8: 10 00 e4 28  	slti	$4, $7, 0x10
  196eac: fe ff 42 94  	lhu	$2, -0x2($2)
  196eb0: 21 18 7d 00  	addu	$3, $3, $sp
  196eb4: 21 10 02 01  	addu	$2, $8, $2
  196eb8: 40 10 02 00  	sll	$2, $2, 0x1
  196ebc: 00 00 62 a4  	sh	$2, 0x0($3)
  196ec0: f6 ff 80 14  	bnez	$4, 0x196e9c <.text+0x96e9c>
  196ec4: ff ff 48 30  	andi	$8, $2, 0xffff
  196ec8: 0a 00 a0 04  	bltz	$5, 0x196ef4 <.text+0x96ef4>
  196ecc: 2d 80 20 01  	move	$16, $9
  196ed0: 01 00 b1 24  	addiu	$17, $5, 0x1
  196ed4: 02 00 02 96  	lhu	$2, 0x2($16)
  196ed8: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  196edc: 40 18 02 00  	sll	$3, $2, 0x1
  196ee0: 2d 28 40 00  	move	$5, $2
  196ee4: 08 00 40 14  	bnez	$2, 0x196f08 <.text+0x96f08>
  196ee8: 21 18 7d 00  	addu	$3, $3, $sp
  196eec: f9 ff 20 16  	bnez	$17, 0x196ed4 <.text+0x96ed4>
  196ef0: 04 00 10 26  	addiu	$16, $16, 0x4
  196ef4: 40 00 bf df  	ld	$ra, 0x40($sp)
  196ef8: 30 00 b1 df  	ld	$17, 0x30($sp)
  196efc: 20 00 b0 df  	ld	$16, 0x20($sp)
  196f00: 08 00 e0 03  	jr	$ra
  196f04: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  196f08: 00 00 62 94  	lhu	$2, 0x0($3)
  196f0c: 2d 20 40 00  	move	$4, $2
  196f10: 01 00 42 24  	addiu	$2, $2, 0x1
  196f14: 10 62 06 0c  	jal	0x198840 <.text+0x98840>
  196f18: 00 00 62 a4  	sh	$2, 0x0($3)
  196f1c: f3 ff 00 10  	b	0x196eec <.text+0x96eec>
  196f20: 00 00 02 a6  	sh	$2, 0x0($16)
  196f24: 70 ff bd 27  	addiu	$sp, $sp, -0x90 <.text+0xffffffffffefff70>
  196f28: 3d 02 02 24  	addiu	$2, $zero, 0x23d
  196f2c: 70 00 b7 ff  	sd	$23, 0x70($sp)
  196f30: 2d b8 a0 00  	move	$23, $5
  196f34: 60 00 b6 ff  	sd	$22, 0x60($sp)
  196f38: ff ff 16 24  	addiu	$22, $zero, -0x1 <.text+0xffffffffffefffff>
  196f3c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  196f40: 2d 90 00 00  	move	$18, $zero
  196f44: 00 00 b0 ff  	sd	$16, 0x0($sp)
  196f48: 2d 80 80 00  	move	$16, $4
  196f4c: 80 00 bf ff  	sd	$ra, 0x80($sp)
  196f50: 50 00 b5 ff  	sd	$21, 0x50($sp)
  196f54: 40 00 b4 ff  	sd	$20, 0x40($sp)
  196f58: 30 00 b3 ff  	sd	$19, 0x30($sp)
  196f5c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  196f60: 08 00 a3 8c  	lw	$3, 0x8($5)
  196f64: 00 00 b3 8c  	lw	$19, 0x0($5)
  196f68: 0c 00 74 8c  	lw	$20, 0xc($3)
  196f6c: 00 00 66 8c  	lw	$6, 0x0($3)
  196f70: 64 14 82 ac  	sw	$2, 0x1464($4)
  196f74: 13 00 80 1a  	blez	$20, 0x196fc4 <.text+0x96fc4>
  196f78: 60 14 80 ac  	sw	$zero, 0x1460($4)
  196f7c: 68 14 85 24  	addiu	$5, $4, 0x1468
  196f80: 2d 20 60 02  	move	$4, $19
  196f84: 00 00 82 94  	lhu	$2, 0x0($4)
  196f88: 09 00 40 50  	beqzl	$2, 0x196fb0 <.text+0x96fb0>
  196f8c: 02 00 80 a4  	sh	$zero, 0x2($4)
  196f90: 60 14 03 8e  	lw	$3, 0x1460($16)
  196f94: 2d b0 40 02  	move	$22, $18
  196f98: 00 00 a0 a0  	sb	$zero, 0x0($5)
  196f9c: 01 00 63 24  	addiu	$3, $3, 0x1
  196fa0: 80 10 03 00  	sll	$2, $3, 0x2
  196fa4: 60 14 03 ae  	sw	$3, 0x1460($16)
  196fa8: 21 10 50 00  	addu	$2, $2, $16
  196fac: 6c 0b 52 ac  	sw	$18, 0xb6c($2)
  196fb0: 01 00 52 26  	addiu	$18, $18, 0x1
  196fb4: 01 00 a5 24  	addiu	$5, $5, 0x1
  196fb8: 2a 10 54 02  	slt	$2, $18, $20
  196fbc: f1 ff 40 14  	bnez	$2, 0x196f84 <.text+0x96f84>
  196fc0: 04 00 84 24  	addiu	$4, $4, 0x4
  196fc4: 60 14 03 8e  	lw	$3, 0x1460($16)
  196fc8: 02 00 62 28  	slti	$2, $3, 0x2
  196fcc: 66 00 40 14  	bnez	$2, 0x197168 <.text+0x97168>
  196fd0: 01 00 62 24  	addiu	$2, $3, 0x1
  196fd4: 04 00 f6 ae  	sw	$22, 0x4($23)
  196fd8: 60 14 02 8e  	lw	$2, 0x1460($16)
  196fdc: c2 1f 02 00  	srl	$3, $2, 0x1f
  196fe0: 21 10 43 00  	addu	$2, $2, $3
  196fe4: 43 90 02 00  	sra	$18, $2, 0x1
  196fe8: 09 00 40 1a  	blez	$18, 0x197010 <.text+0x97010>
  196fec: 21 10 90 02  	addu	$2, $20, $16
  196ff0: 2d 30 40 02  	move	$6, $18
  196ff4: 2d 20 00 02  	move	$4, $16
  196ff8: ff ff 52 26  	addiu	$18, $18, -0x1 <.text+0xffffffffffefffff>
  196ffc: a5 5a 06 0c  	jal	0x196a94 <.text+0x96a94>
