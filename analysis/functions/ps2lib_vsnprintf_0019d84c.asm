
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  19d84c: 50 ff bd 27  	addiu	$sp, $sp, -0xb0 <.text+0xffffffffffefff50>
  19d850: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19d854: 90 00 be ff  	sd	$fp, 0x90($sp)
  19d858: 2d f0 e0 00  	move	$fp, $7
  19d85c: 80 00 b7 ff  	sd	$23, 0x80($sp)
  19d860: 2d b8 c0 00  	move	$23, $6
  19d864: 70 00 b6 ff  	sd	$22, 0x70($sp)
  19d868: 2d b0 40 01  	move	$22, $10
  19d86c: 60 00 b5 ff  	sd	$21, 0x60($sp)
  19d870: 2d a8 00 00  	move	$21, $zero
  19d874: 50 00 b4 ff  	sd	$20, 0x50($sp)
  19d878: 2d a0 a0 00  	move	$20, $5
  19d87c: 40 00 b3 ff  	sd	$19, 0x40($sp)
  19d880: 2d 98 00 01  	move	$19, $8
  19d884: 30 00 b2 ff  	sd	$18, 0x30($sp)
  19d888: 2d 90 80 00  	move	$18, $4
  19d88c: 20 00 b1 ff  	sd	$17, 0x20($sp)
  19d890: 2d 88 20 01  	move	$17, $9
  19d894: a0 00 bf ff  	sd	$ra, 0xa0($sp)
  19d898: 10 00 b0 ff  	sd	$16, 0x10($sp)
  19d89c: c0 00 22 11  	beq	$9, $2, 0x19dba0 <.text+0x9dba0>
  19d8a0: 00 00 ab af  	sw	$11, 0x0($sp)
  19d8a4: ef ff 02 24  	addiu	$2, $zero, -0x11 <.text+0xffffffffffefffef>
  19d8a8: 24 b0 42 01  	and	$22, $10, $2
  19d8ac: 04 00 20 16  	bnez	$17, 0x19d8c0 <.text+0x9d8c0>
  19d8b0: 3c 80 17 00  	dsll32	$16, $23, 0x0
  19d8b4: 88 00 80 12  	beqz	$20, 0x19dad8 <.text+0x9dad8>
  19d8b8: 2d 18 00 00  	move	$3, $zero
  19d8bc: 3c 80 17 00  	dsll32	$16, $23, 0x0
  19d8c0: 2d 20 80 02  	move	$4, $20
  19d8c4: 3e 80 10 00  	dsrl32	$16, $16, 0x0
  19d8c8: 01 00 b5 26  	addiu	$21, $21, 0x1
  19d8cc: 1e 8b 06 0c  	jal	0x1a2c78 <.text+0xa2c78>
  19d8d0: 2d 28 00 02  	move	$5, $16
  19d8d4: 14 00 43 8e  	lw	$3, 0x14($18)
  19d8d8: 3c 10 02 00  	dsll32	$2, $2, 0x0
  19d8dc: 3f 10 02 00  	dsra32	$2, $2, 0x0
  19d8e0: 2d 20 40 02  	move	$4, $18
  19d8e4: 21 10 c2 03  	addu	$2, $fp, $2
  19d8e8: 09 f8 60 00  	jalr	$3
  19d8ec: 00 00 45 90  	lbu	$5, 0x0($2)
  19d8f0: 2d 20 80 02  	move	$4, $20
  19d8f4: 85 00 40 14  	bnez	$2, 0x19db0c <.text+0x9db0c>
  19d8f8: 2d 28 00 02  	move	$5, $16
  19d8fc: 6c 89 06 0c  	jal	0x1a25b0 <.text+0xa25b0>
  19d900: 00 00 00 00  	nop
  19d904: ed ff 40 14  	bnez	$2, 0x19d8bc <.text+0x9d8bc>
  19d908: 2d a0 40 00  	move	$20, $2
  19d90c: 23 88 35 02  	subu	$17, $17, $21
  19d910: 2d 10 20 02  	move	$2, $17
  19d914: 0b 00 40 18  	blez	$2, 0x19d944 <.text+0x9d944>
  19d918: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  19d91c: 14 00 42 8e  	lw	$2, 0x14($18)
  19d920: 2d 20 40 02  	move	$4, $18
  19d924: 30 00 05 24  	addiu	$5, $zero, 0x30
  19d928: 09 f8 40 00  	jalr	$2
  19d92c: 01 00 b5 26  	addiu	$21, $21, 0x1
  19d930: 2d 18 20 02  	move	$3, $17
  19d934: 75 00 40 14  	bnez	$2, 0x19db0c <.text+0x9db0c>
  19d938: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  19d93c: f8 ff 60 5c  	bgtzl	$3, 0x19d920 <.text+0x9d920>
  19d940: 14 00 42 8e  	lw	$2, 0x14($18)
  19d944: 08 00 d0 32  	andi	$16, $22, 0x8
  19d948: 07 00 00 12  	beqz	$16, 0x19d968 <.text+0x9d968>
  19d94c: 10 00 c2 32  	andi	$2, $22, 0x10
  19d950: 10 00 02 24  	addiu	$2, $zero, 0x10
  19d954: 8f 00 e2 12  	beq	$23, $2, 0x19db94 <.text+0x9db94>
  19d958: 08 00 02 24  	addiu	$2, $zero, 0x8
  19d95c: 8e 00 e2 12  	beq	$23, $2, 0x19db98 <.text+0x9db98>
  19d960: c2 10 17 00  	srl	$2, $23, 0x3
  19d964: 10 00 c2 32  	andi	$2, $22, 0x10
  19d968: 14 00 40 10  	beqz	$2, 0x19d9bc <.text+0x9d9bc>
  19d96c: 00 00 a2 8f  	lw	$2, 0x0($sp)
  19d970: 04 00 40 14  	bnez	$2, 0x19d984 <.text+0x9d984>
  19d974: 23 98 75 02  	subu	$19, $19, $21
  19d978: 06 00 c2 32  	andi	$2, $22, 0x6
  19d97c: 03 00 40 10  	beqz	$2, 0x19d98c <.text+0x9d98c>
  19d980: 2d 10 60 02  	move	$2, $19
  19d984: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  19d988: 2d 10 60 02  	move	$2, $19
  19d98c: 0b 00 40 18  	blez	$2, 0x19d9bc <.text+0x9d9bc>
  19d990: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  19d994: 14 00 42 8e  	lw	$2, 0x14($18)
  19d998: 2d 20 40 02  	move	$4, $18
  19d99c: 30 00 05 24  	addiu	$5, $zero, 0x30
  19d9a0: 09 f8 40 00  	jalr	$2
  19d9a4: 01 00 b5 26  	addiu	$21, $21, 0x1
  19d9a8: 2d 18 60 02  	move	$3, $19
  19d9ac: 57 00 40 14  	bnez	$2, 0x19db0c <.text+0x9db0c>
  19d9b0: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  19d9b4: f8 ff 60 5c  	bgtzl	$3, 0x19d998 <.text+0x9d998>
  19d9b8: 14 00 42 8e  	lw	$2, 0x14($18)
  19d9bc: 06 00 00 12  	beqz	$16, 0x19d9d8 <.text+0x9d9d8>
  19d9c0: 00 00 a2 8f  	lw	$2, 0x0($sp)
  19d9c4: 10 00 02 24  	addiu	$2, $zero, 0x10
  19d9c8: 68 00 e2 12  	beq	$23, $2, 0x19db6c <.text+0x9db6c>
  19d9cc: 08 00 02 24  	addiu	$2, $zero, 0x8
  19d9d0: 5e 00 e2 12  	beq	$23, $2, 0x19db4c <.text+0x9db4c>
  19d9d4: 00 00 a2 8f  	lw	$2, 0x0($sp)
  19d9d8: 59 00 40 54  	bnezl	$2, 0x19db40 <.text+0x9db40>
  19d9dc: 14 00 42 8e  	lw	$2, 0x14($18)
  19d9e0: 02 00 c2 32  	andi	$2, $22, 0x2
  19d9e4: 53 00 40 54  	bnezl	$2, 0x19db34 <.text+0x9db34>
  19d9e8: 14 00 42 8e  	lw	$2, 0x14($18)
  19d9ec: 04 00 c2 32  	andi	$2, $22, 0x4
  19d9f0: 48 00 40 54  	bnezl	$2, 0x19db14 <.text+0x9db14>
  19d9f4: 14 00 42 8e  	lw	$2, 0x14($18)
  19d9f8: 01 00 d0 32  	andi	$16, $22, 0x1
  19d9fc: 14 00 00 52  	beqzl	$16, 0x19da50 <.text+0x9da50>
  19da00: 23 98 75 02  	subu	$19, $19, $21
  19da04: c2 17 15 00  	srl	$2, $21, 0x1f
  19da08: 21 10 a2 02  	addu	$2, $21, $2
  19da0c: 43 40 02 00  	sra	$8, $2, 0x1
  19da10: 0e 00 00 19  	blez	$8, 0x19da4c <.text+0x9da4c>
  19da14: 2d 38 00 00  	move	$7, $zero
  19da18: 04 00 42 8e  	lw	$2, 0x4($18)
  19da1c: 23 30 f5 00  	subu	$6, $7, $21
  19da20: 21 18 46 00  	addu	$3, $2, $6
  19da24: 23 10 47 00  	subu	$2, $2, $7
  19da28: 00 00 63 90  	lbu	$3, 0x0($3)
  19da2c: 01 00 e7 24  	addiu	$7, $7, 0x1
  19da30: ff ff 44 80  	lb	$4, -0x1($2)
  19da34: 2a 28 e8 00  	slt	$5, $7, $8
  19da38: ff ff 43 a0  	sb	$3, -0x1($2)
  19da3c: 04 00 42 8e  	lw	$2, 0x4($18)
  19da40: 21 10 46 00  	addu	$2, $2, $6
  19da44: f4 ff a0 14  	bnez	$5, 0x19da18 <.text+0x9da18>
  19da48: 00 00 44 a0  	sb	$4, 0x0($2)
  19da4c: 23 98 75 02  	subu	$19, $19, $21
  19da50: 2d 10 60 02  	move	$2, $19
  19da54: 0b 00 40 18  	blez	$2, 0x19da84 <.text+0x9da84>
  19da58: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  19da5c: 14 00 42 8e  	lw	$2, 0x14($18)
  19da60: 2d 20 40 02  	move	$4, $18
  19da64: 20 00 05 24  	addiu	$5, $zero, 0x20
  19da68: 09 f8 40 00  	jalr	$2
  19da6c: 01 00 b5 26  	addiu	$21, $21, 0x1
  19da70: 2d 18 60 02  	move	$3, $19
  19da74: 25 00 40 14  	bnez	$2, 0x19db0c <.text+0x9db0c>
  19da78: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  19da7c: f8 ff 60 5c  	bgtzl	$3, 0x19da60 <.text+0x9da60>
  19da80: 14 00 42 8e  	lw	$2, 0x14($18)
  19da84: 14 00 00 16  	bnez	$16, 0x19dad8 <.text+0x9dad8>
  19da88: 2d 18 00 00  	move	$3, $zero
  19da8c: c2 17 15 00  	srl	$2, $21, 0x1f
  19da90: 21 10 a2 02  	addu	$2, $21, $2
  19da94: 43 40 02 00  	sra	$8, $2, 0x1
  19da98: 0f 00 00 19  	blez	$8, 0x19dad8 <.text+0x9dad8>
  19da9c: 2d 38 00 00  	move	$7, $zero
  19daa0: 04 00 42 8e  	lw	$2, 0x4($18)
  19daa4: 23 30 f5 00  	subu	$6, $7, $21
  19daa8: 21 18 46 00  	addu	$3, $2, $6
  19daac: 23 10 47 00  	subu	$2, $2, $7
  19dab0: 00 00 63 90  	lbu	$3, 0x0($3)
  19dab4: 01 00 e7 24  	addiu	$7, $7, 0x1
  19dab8: ff ff 44 80  	lb	$4, -0x1($2)
  19dabc: 2a 28 e8 00  	slt	$5, $7, $8
  19dac0: ff ff 43 a0  	sb	$3, -0x1($2)
  19dac4: 04 00 42 8e  	lw	$2, 0x4($18)
  19dac8: 21 10 46 00  	addu	$2, $2, $6
  19dacc: f4 ff a0 14  	bnez	$5, 0x19daa0 <.text+0x9daa0>
  19dad0: 00 00 44 a0  	sb	$4, 0x0($2)
  19dad4: 2d 18 00 00  	move	$3, $zero
  19dad8: a0 00 bf df  	ld	$ra, 0xa0($sp)
  19dadc: 2d 10 60 00  	move	$2, $3
  19dae0: 90 00 be df  	ld	$fp, 0x90($sp)
  19dae4: 80 00 b7 df  	ld	$23, 0x80($sp)
  19dae8: 70 00 b6 df  	ld	$22, 0x70($sp)
  19daec: 60 00 b5 df  	ld	$21, 0x60($sp)
  19daf0: 50 00 b4 df  	ld	$20, 0x50($sp)
  19daf4: 40 00 b3 df  	ld	$19, 0x40($sp)
  19daf8: 30 00 b2 df  	ld	$18, 0x30($sp)
  19dafc: 20 00 b1 df  	ld	$17, 0x20($sp)
  19db00: 10 00 b0 df  	ld	$16, 0x10($sp)
  19db04: 08 00 e0 03  	jr	$ra
  19db08: b0 00 bd 27  	addiu	$sp, $sp, 0xb0
  19db0c: f2 ff 00 10  	b	0x19dad8 <.text+0x9dad8>
  19db10: 01 00 03 24  	addiu	$3, $zero, 0x1
  19db14: 2d 20 40 02  	move	$4, $18
  19db18: 20 00 05 24  	addiu	$5, $zero, 0x20
  19db1c: 09 f8 40 00  	jalr	$2
  19db20: 00 00 00 00  	nop
  19db24: ec ff 40 14  	bnez	$2, 0x19dad8 <.text+0x9dad8>
  19db28: 01 00 03 24  	addiu	$3, $zero, 0x1
  19db2c: b2 ff 00 10  	b	0x19d9f8 <.text+0x9d9f8>
  19db30: 01 00 b5 26  	addiu	$21, $21, 0x1
  19db34: 2d 20 40 02  	move	$4, $18
  19db38: f8 ff 00 10  	b	0x19db1c <.text+0x9db1c>
  19db3c: 2b 00 05 24  	addiu	$5, $zero, 0x2b
  19db40: 2d 20 40 02  	move	$4, $18
  19db44: f5 ff 00 10  	b	0x19db1c <.text+0x9db1c>
  19db48: 2d 00 05 24  	addiu	$5, $zero, 0x2d
  19db4c: 14 00 42 8e  	lw	$2, 0x14($18)
  19db50: 2d 20 40 02  	move	$4, $18
  19db54: 09 f8 40 00  	jalr	$2
  19db58: 30 00 05 24  	addiu	$5, $zero, 0x30
  19db5c: de ff 40 14  	bnez	$2, 0x19dad8 <.text+0x9dad8>
  19db60: 01 00 03 24  	addiu	$3, $zero, 0x1
  19db64: 9c ff 00 10  	b	0x19d9d8 <.text+0x9d9d8>
  19db68: 00 00 a2 8f  	lw	$2, 0x0($sp)
  19db6c: 0a 00 c5 93  	lbu	$5, 0xa($fp)
  19db70: 2d 20 40 02  	move	$4, $18
  19db74: 14 00 42 8e  	lw	$2, 0x14($18)
  19db78: 17 00 a5 24  	addiu	$5, $5, 0x17
  19db7c: 09 f8 40 00  	jalr	$2
  19db80: ff 00 a5 30  	andi	$5, $5, 0xff
  19db84: d4 ff 40 14  	bnez	$2, 0x19dad8 <.text+0x9dad8>
  19db88: 01 00 03 24  	addiu	$3, $zero, 0x1
  19db8c: f0 ff 00 10  	b	0x19db50 <.text+0x9db50>
  19db90: 14 00 42 8e  	lw	$2, 0x14($18)
  19db94: c2 10 17 00  	srl	$2, $23, 0x3
  19db98: 72 ff 00 10  	b	0x19d964 <.text+0x9d964>
  19db9c: 21 a8 a2 02  	addu	$21, $21, $2
  19dba0: 42 ff 00 10  	b	0x19d8ac <.text+0x9d8ac>
  19dba4: 01 00 11 24  	addiu	$17, $zero, 0x1
  19dba8: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  19dbac: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19dbb0: 40 00 b4 ff  	sd	$20, 0x40($sp)
  19dbb4: 2d a0 00 01  	move	$20, $8
  19dbb8: 30 00 b3 ff  	sd	$19, 0x30($sp)
  19dbbc: 2d 98 80 00  	move	$19, $4
  19dbc0: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19dbc4: 2d 90 a0 00  	move	$18, $5
  19dbc8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19dbcc: 2d 88 e0 00  	move	$17, $7
  19dbd0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19dbd4: 2d 80 c0 00  	move	$16, $6
  19dbd8: 4f 00 e2 10  	beq	$7, $2, 0x19dd18 <.text+0x9dd18>
  19dbdc: 50 00 bf ff  	sd	$ra, 0x50($sp)
  19dbe0: 23 80 c7 00  	subu	$16, $6, $7
  19dbe4: 01 00 94 32  	andi	$20, $20, 0x1
  19dbe8: 0e 00 80 16  	bnez	$20, 0x19dc24 <.text+0x9dc24>
  19dbec: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19dbf0: 2d 10 00 02  	move	$2, $16
  19dbf4: 0b 00 40 18  	blez	$2, 0x19dc24 <.text+0x9dc24>
  19dbf8: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  19dbfc: 14 00 62 8e  	lw	$2, 0x14($19)
  19dc00: 2d 20 60 02  	move	$4, $19
  19dc04: 09 f8 40 00  	jalr	$2
  19dc08: 20 00 05 24  	addiu	$5, $zero, 0x20
  19dc0c: 2d 18 00 02  	move	$3, $16
  19dc10: 31 00 40 14  	bnez	$2, 0x19dcd8 <.text+0x9dcd8>
  19dc14: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  19dc18: f9 ff 60 5c  	bgtzl	$3, 0x19dc00 <.text+0x9dc00>
  19dc1c: 14 00 62 8e  	lw	$2, 0x14($19)
  19dc20: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19dc24: 2e 00 23 52  	beql	$17, $3, 0x19dce0 <.text+0x9dce0>
  19dc28: 00 00 45 92  	lbu	$5, 0x0($18)
  19dc2c: 00 00 42 92  	lbu	$2, 0x0($18)
  19dc30: 11 00 40 10  	beqz	$2, 0x19dc78 <.text+0x9dc78>
  19dc34: 00 00 00 00  	nop
  19dc38: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  19dc3c: 0e 00 23 12  	beq	$17, $3, 0x19dc78 <.text+0x9dc78>
  19dc40: 00 00 00 00  	nop
  19dc44: 00 00 45 92  	lbu	$5, 0x0($18)
  19dc48: 2d 20 60 02  	move	$4, $19
  19dc4c: 14 00 62 8e  	lw	$2, 0x14($19)
  19dc50: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  19dc54: 09 f8 40 00  	jalr	$2
  19dc58: 01 00 52 26  	addiu	$18, $18, 0x1
  19dc5c: 15 00 40 14  	bnez	$2, 0x19dcb4 <.text+0x9dcb4>
  19dc60: 01 00 03 24  	addiu	$3, $zero, 0x1
  19dc64: 00 00 42 92  	lbu	$2, 0x0($18)
  19dc68: 03 00 40 10  	beqz	$2, 0x19dc78 <.text+0x9dc78>
  19dc6c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19dc70: f5 ff 22 56  	bnel	$17, $2, 0x19dc48 <.text+0x9dc48>
  19dc74: 00 00 45 92  	lbu	$5, 0x0($18)
  19dc78: 0e 00 80 12  	beqz	$20, 0x19dcb4 <.text+0x9dcb4>
  19dc7c: 2d 18 00 00  	move	$3, $zero
  19dc80: 2d 10 00 02  	move	$2, $16
  19dc84: 0b 00 40 18  	blez	$2, 0x19dcb4 <.text+0x9dcb4>
  19dc88: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  19dc8c: 14 00 62 8e  	lw	$2, 0x14($19)
  19dc90: 2d 20 60 02  	move	$4, $19
  19dc94: 09 f8 40 00  	jalr	$2
  19dc98: 20 00 05 24  	addiu	$5, $zero, 0x20
  19dc9c: 2d 18 00 02  	move	$3, $16
  19dca0: 0d 00 40 14  	bnez	$2, 0x19dcd8 <.text+0x9dcd8>
  19dca4: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  19dca8: f9 ff 60 5c  	bgtzl	$3, 0x19dc90 <.text+0x9dc90>
  19dcac: 14 00 62 8e  	lw	$2, 0x14($19)
  19dcb0: 2d 18 00 00  	move	$3, $zero
  19dcb4: 50 00 bf df  	ld	$ra, 0x50($sp)
  19dcb8: 2d 10 60 00  	move	$2, $3
  19dcbc: 40 00 b4 df  	ld	$20, 0x40($sp)
  19dcc0: 30 00 b3 df  	ld	$19, 0x30($sp)
  19dcc4: 20 00 b2 df  	ld	$18, 0x20($sp)
  19dcc8: 10 00 b1 df  	ld	$17, 0x10($sp)
  19dccc: 00 00 b0 df  	ld	$16, 0x0($sp)
  19dcd0: 08 00 e0 03  	jr	$ra
  19dcd4: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  19dcd8: f6 ff 00 10  	b	0x19dcb4 <.text+0x9dcb4>
  19dcdc: 01 00 03 24  	addiu	$3, $zero, 0x1
  19dce0: e5 ff a0 10  	beqz	$5, 0x19dc78 <.text+0x9dc78>
  19dce4: 00 00 00 00  	nop
  19dce8: 14 00 62 8e  	lw	$2, 0x14($19)
  19dcec: ff 00 a5 30  	andi	$5, $5, 0xff
  19dcf0: 2d 20 60 02  	move	$4, $19
  19dcf4: 09 f8 40 00  	jalr	$2
  19dcf8: 01 00 52 26  	addiu	$18, $18, 0x1
  19dcfc: ed ff 40 14  	bnez	$2, 0x19dcb4 <.text+0x9dcb4>
  19dd00: 01 00 03 24  	addiu	$3, $zero, 0x1
  19dd04: 00 00 45 92  	lbu	$5, 0x0($18)
  19dd08: f8 ff a0 54  	bnezl	$5, 0x19dcec <.text+0x9dcec>
  19dd0c: 14 00 62 8e  	lw	$2, 0x14($19)
  19dd10: d9 ff 00 10  	b	0x19dc78 <.text+0x9dc78>
  19dd14: 00 00 00 00  	nop
  19dd18: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  19dd1c: 2d 20 a0 00  	move	$4, $5
  19dd20: b0 ff 00 10  	b	0x19dbe4 <.text+0x9dbe4>
  19dd24: 23 80 02 02  	subu	$16, $16, $2
  19dd28: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  19dd2c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19dd30: 01 00 f2 30  	andi	$18, $7, 0x1
  19dd34: 40 00 b4 ff  	sd	$20, 0x40($sp)
  19dd38: 2d a0 e0 00  	move	$20, $7
  19dd3c: 30 00 b3 ff  	sd	$19, 0x30($sp)
  19dd40: ff 00 b3 30  	andi	$19, $5, 0xff
  19dd44: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19dd48: 2d 88 80 00  	move	$17, $4
  19dd4c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19dd50: 2d 80 c0 00  	move	$16, $6
  19dd54: 0e 00 40 16  	bnez	$18, 0x19dd90 <.text+0x9dd90>
  19dd58: 50 00 bf ff  	sd	$ra, 0x50($sp)
  19dd5c: ff ff d0 24  	addiu	$16, $6, -0x1 <.text+0xffffffffffefffff>
  19dd60: 0c 00 00 1a  	blez	$16, 0x19dd94 <.text+0x9dd94>
  19dd64: 14 00 22 8e  	lw	$2, 0x14($17)
  19dd68: 2d 20 20 02  	move	$4, $17
  19dd6c: 09 f8 40 00  	jalr	$2
  19dd70: 20 00 05 24  	addiu	$5, $zero, 0x20
  19dd74: 1d 00 40 14  	bnez	$2, 0x19ddec <.text+0x9ddec>
  19dd78: 01 00 03 24  	addiu	$3, $zero, 0x1
  19dd7c: 05 00 40 56  	bnezl	$18, 0x19dd94 <.text+0x9dd94>
  19dd80: 14 00 22 8e  	lw	$2, 0x14($17)
  19dd84: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  19dd88: f7 ff 00 5e  	bgtzl	$16, 0x19dd68 <.text+0x9dd68>
  19dd8c: 14 00 22 8e  	lw	$2, 0x14($17)
  19dd90: 14 00 22 8e  	lw	$2, 0x14($17)
  19dd94: 2d 28 60 02  	move	$5, $19
  19dd98: 09 f8 40 00  	jalr	$2
  19dd9c: 2d 20 20 02  	move	$4, $17
  19dda0: 12 00 40 14  	bnez	$2, 0x19ddec <.text+0x9ddec>
  19dda4: 01 00 03 24  	addiu	$3, $zero, 0x1
  19dda8: 01 00 92 32  	andi	$18, $20, 0x1
  19ddac: 0e 00 40 12  	beqz	$18, 0x19dde8 <.text+0x9dde8>
  19ddb0: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  19ddb4: 0d 00 00 1a  	blez	$16, 0x19ddec <.text+0x9ddec>
  19ddb8: 2d 18 00 00  	move	$3, $zero
  19ddbc: 14 00 22 8e  	lw	$2, 0x14($17)
  19ddc0: 2d 20 20 02  	move	$4, $17
  19ddc4: 20 00 05 24  	addiu	$5, $zero, 0x20
  19ddc8: 09 f8 40 00  	jalr	$2
  19ddcc: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  19ddd0: 06 00 40 14  	bnez	$2, 0x19ddec <.text+0x9ddec>
  19ddd4: 01 00 03 24  	addiu	$3, $zero, 0x1
  19ddd8: 04 00 40 52  	beqzl	$18, 0x19ddec <.text+0x9ddec>
  19dddc: 2d 18 00 00  	move	$3, $zero
  19dde0: f7 ff 00 5e  	bgtzl	$16, 0x19ddc0 <.text+0x9ddc0>
  19dde4: 14 00 22 8e  	lw	$2, 0x14($17)
  19dde8: 2d 18 00 00  	move	$3, $zero
  19ddec: 50 00 bf df  	ld	$ra, 0x50($sp)
  19ddf0: 2d 10 60 00  	move	$2, $3
  19ddf4: 40 00 b4 df  	ld	$20, 0x40($sp)
  19ddf8: 30 00 b3 df  	ld	$19, 0x30($sp)
  19ddfc: 20 00 b2 df  	ld	$18, 0x20($sp)
  19de00: 10 00 b1 df  	ld	$17, 0x10($sp)
  19de04: 00 00 b0 df  	ld	$16, 0x0($sp)
  19de08: 08 00 e0 03  	jr	$ra
  19de0c: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  19de10: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  19de14: 30 00 b3 ff  	sd	$19, 0x30($sp)
  19de18: 2d 98 80 00  	move	$19, $4
  19de1c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19de20: 01 00 b2 24  	addiu	$18, $5, 0x1
  19de24: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19de28: 40 00 bf ff  	sd	$ra, 0x40($sp)
  19de2c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19de30: 00 00 b0 90  	lbu	$16, 0x0($5)
  19de34: 0d 00 00 12  	beqz	$16, 0x19de6c <.text+0x9de6c>
  19de38: 2d 88 c0 00  	move	$17, $6
  19de3c: 25 00 02 24  	addiu	$2, $zero, 0x25
  19de40: 13 00 02 52  	beql	$16, $2, 0x19de90 <.text+0x9de90>
  19de44: 00 00 50 92  	lbu	$16, 0x0($18)
  19de48: 14 00 62 8e  	lw	$2, 0x14($19)
  19de4c: 2d 28 00 02  	move	$5, $16
  19de50: 09 f8 40 00  	jalr	$2
  19de54: 2d 20 60 02  	move	$4, $19
  19de58: 05 00 40 14  	bnez	$2, 0x19de70 <.text+0x9de70>
  19de5c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19de60: 00 00 50 92  	lbu	$16, 0x0($18)
  19de64: f5 ff 00 16  	bnez	$16, 0x19de3c <.text+0x9de3c>
  19de68: 01 00 52 26  	addiu	$18, $18, 0x1
  19de6c: 2d 18 00 00  	move	$3, $zero
  19de70: 40 00 bf df  	ld	$ra, 0x40($sp)
  19de74: 2d 10 60 00  	move	$2, $3
  19de78: 30 00 b3 df  	ld	$19, 0x30($sp)
  19de7c: 20 00 b2 df  	ld	$18, 0x20($sp)
  19de80: 10 00 b1 df  	ld	$17, 0x10($sp)
  19de84: 00 00 b0 df  	ld	$16, 0x0($sp)
  19de88: 08 00 e0 03  	jr	$ra
  19de8c: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  19de90: 2d 50 00 00  	move	$10, $zero
  19de94: 2d 30 00 00  	move	$6, $zero
  19de98: ff ff 09 24  	addiu	$9, $zero, -0x1 <.text+0xffffffffffefffff>
  19de9c: 2d 28 00 00  	move	$5, $zero
  19dea0: 2d 38 00 00  	move	$7, $zero
  19dea4: 10 00 00 12  	beqz	$16, 0x19dee8 <.text+0x9dee8>
  19dea8: 01 00 52 26  	addiu	$18, $18, 0x1
  19deac: 2d 00 02 24  	addiu	$2, $zero, 0x2d
  19deb0: ee 00 02 12  	beq	$16, $2, 0x19e26c <.text+0x9e26c>
  19deb4: 2b 00 02 24  	addiu	$2, $zero, 0x2b
  19deb8: ea 00 02 12  	beq	$16, $2, 0x19e264 <.text+0x9e264>
  19debc: 20 00 02 24  	addiu	$2, $zero, 0x20
  19dec0: e6 00 02 12  	beq	$16, $2, 0x19e25c <.text+0x9e25c>
  19dec4: 23 00 02 24  	addiu	$2, $zero, 0x23
  19dec8: e2 00 02 12  	beq	$16, $2, 0x19e254 <.text+0x9e254>
  19decc: 30 00 02 24  	addiu	$2, $zero, 0x30
  19ded0: 06 00 02 16  	bne	$16, $2, 0x19deec <.text+0x9deec>
  19ded4: 06 00 42 31  	andi	$2, $10, 0x6
  19ded8: 10 00 4a 35  	ori	$10, $10, 0x10
  19dedc: 00 00 50 92  	lbu	$16, 0x0($18)
  19dee0: f2 ff 00 16  	bnez	$16, 0x19deac <.text+0x9deac>
  19dee4: 01 00 52 26  	addiu	$18, $18, 0x1
  19dee8: 06 00 42 31  	andi	$2, $10, 0x6
  19deec: 04 00 44 39  	xori	$4, $10, 0x4
  19def0: 06 00 42 38  	xori	$2, $2, 0x6
  19def4: d0 ff 03 26  	addiu	$3, $16, -0x30 <.text+0xffffffffffefffd0>
  19def8: 0a 50 82 00  	movz	$10, $4, $2
  19defc: 0a 00 63 2c  	sltiu	$3, $3, 0xa
  19df00: 11 00 42 31  	andi	$2, $10, 0x11
  19df04: 10 00 44 39  	xori	$4, $10, 0x10
  19df08: 11 00 42 38  	xori	$2, $2, 0x11
  19df0c: c9 00 60 10  	beqz	$3, 0x19e234 <.text+0x9e234>
  19df10: 0a 50 82 00  	movz	$10, $4, $2
  19df14: 0a 00 03 24  	addiu	$3, $zero, 0xa
  19df18: 18 10 c3 00  	<unknown>
  19df1c: 21 18 50 00  	addu	$3, $2, $16
  19df20: 00 00 50 92  	lbu	$16, 0x0($18)
  19df24: d0 ff 66 24  	addiu	$6, $3, -0x30 <.text+0xffffffffffefffd0>
  19df28: d0 ff 02 26  	addiu	$2, $16, -0x30 <.text+0xffffffffffefffd0>
  19df2c: 0a 00 42 2c  	sltiu	$2, $2, 0xa
  19df30: f8 ff 40 14  	bnez	$2, 0x19df14 <.text+0x9df14>
  19df34: 01 00 52 26  	addiu	$18, $18, 0x1
  19df38: 2e 00 02 24  	addiu	$2, $zero, 0x2e
  19df3c: a5 00 02 52  	beql	$16, $2, 0x19e1d4 <.text+0x9e1d4>
  19df40: 00 00 50 92  	lbu	$16, 0x0($18)
  19df44: 68 00 02 24  	addiu	$2, $zero, 0x68
  19df48: 9f 00 02 12  	beq	$16, $2, 0x19e1c8 <.text+0x9e1c8>
  19df4c: 6c 00 02 24  	addiu	$2, $zero, 0x6c
  19df50: 9a 00 02 52  	beql	$16, $2, 0x19e1bc <.text+0x9e1bc>
  19df54: 00 00 50 92  	lbu	$16, 0x0($18)
  19df58: 79 00 02 2e  	sltiu	$2, $16, 0x79
  19df5c: 7a 00 40 10  	beqz	$2, 0x19e148 <.text+0x9e148>
  19df60: 1c 00 03 3c  	lui	$3, 0x1c
  19df64: 80 10 10 00  	sll	$2, $16, 0x2
  19df68: c4 a4 63 24  	addiu	$3, $3, -0x5b3c <.text+0xffffffffffefa4c4>
  19df6c: 21 10 43 00  	addu	$2, $2, $3
  19df70: 00 00 42 8c  	lw	$2, 0x0($2)
  19df74: 08 00 40 00  	jr	$2
  19df78: 00 00 00 00  	nop
  19df7c: 00 00 25 92  	lbu	$5, 0x0($17)
  19df80: 2d 38 40 01  	move	$7, $10
  19df84: 2d 20 60 02  	move	$4, $19
  19df88: 4a 77 06 0c  	jal	0x19dd28 <.text+0x9dd28>
  19df8c: 08 00 31 26  	addiu	$17, $17, 0x8
  19df90: b1 ff 00 10  	b	0x19de58 <.text+0x9de58>
  19df94: 00 00 00 00  	nop
  19df98: ab ff 00 10  	b	0x19de48 <.text+0x9de48>
  19df9c: ff ff 52 26  	addiu	$18, $18, -0x1 <.text+0xffffffffffefffff>
  19dfa0: 0d 00 a0 10  	beqz	$5, 0x19dfd8 <.text+0x9dfd8>
  19dfa4: 2d 10 20 02  	move	$2, $17
  19dfa8: 08 00 31 26  	addiu	$17, $17, 0x8
  19dfac: 00 00 45 dc  	ld	$5, 0x0($2)
  19dfb0: 1c 00 07 3c  	lui	$7, 0x1c
  19dfb4: 2d 40 c0 00  	move	$8, $6
  19dfb8: 78 a4 e7 24  	addiu	$7, $7, -0x5b88 <.text+0xffffffffffefa478>
  19dfbc: 2d 20 60 02  	move	$4, $19
  19dfc0: 10 00 06 24  	addiu	$6, $zero, 0x10
  19dfc4: 2d 58 00 00  	move	$11, $zero
  19dfc8: 13 76 06 0c  	jal	0x19d84c <.text+0x9d84c>
  19dfcc: 00 00 00 00  	nop
  19dfd0: a1 ff 00 10  	b	0x19de58 <.text+0x9de58>
  19dfd4: 00 00 00 00  	nop
  19dfd8: 04 00 e0 10  	beqz	$7, 0x19dfec <.text+0x9dfec>
  19dfdc: 00 00 00 00  	nop
  19dfe0: 08 00 31 26  	addiu	$17, $17, 0x8
  19dfe4: f2 ff 00 10  	b	0x19dfb0 <.text+0x9dfb0>
  19dfe8: 00 00 45 94  	lhu	$5, 0x0($2)
  19dfec: 08 00 31 26  	addiu	$17, $17, 0x8
  19dff0: ef ff 00 10  	b	0x19dfb0 <.text+0x9dfb0>
  19dff4: 00 00 45 9c  	lwu	$5, 0x0($2)
  19dff8: 0e 00 a0 10  	beqz	$5, 0x19e034 <.text+0x9e034>
  19dffc: 2d 58 00 00  	move	$11, $zero
  19e000: 2d 10 20 02  	move	$2, $17
  19e004: 08 00 31 26  	addiu	$17, $17, 0x8
  19e008: 00 00 45 dc  	ld	$5, 0x0($2)
  19e00c: 07 00 a2 04  	bltzl	$5, 0x19e02c <.text+0x9e02c>
  19e010: 2f 28 05 00  	dnegu	$5, $5
  19e014: 1c 00 07 3c  	lui	$7, 0x1c
  19e018: 2d 40 c0 00  	move	$8, $6
  19e01c: 90 a4 e7 24  	addiu	$7, $7, -0x5b70 <.text+0xffffffffffefa490>
  19e020: 2d 20 60 02  	move	$4, $19
  19e024: e8 ff 00 10  	b	0x19dfc8 <.text+0x9dfc8>
  19e028: 0a 00 06 24  	addiu	$6, $zero, 0xa
  19e02c: f9 ff 00 10  	b	0x19e014 <.text+0x9e014>
  19e030: 01 00 0b 24  	addiu	$11, $zero, 0x1
  19e034: 04 00 e0 10  	beqz	$7, 0x19e048 <.text+0x9e048>
  19e038: 2d 10 20 02  	move	$2, $17
  19e03c: 08 00 31 26  	addiu	$17, $17, 0x8
  19e040: f2 ff 00 10  	b	0x19e00c <.text+0x9e00c>
  19e044: 00 00 45 84  	lh	$5, 0x0($2)
  19e048: 08 00 31 26  	addiu	$17, $17, 0x8
  19e04c: ef ff 00 10  	b	0x19e00c <.text+0x9e00c>
  19e050: 00 00 45 8c  	lw	$5, 0x0($2)
  19e054: 2d 18 20 02  	move	$3, $17
  19e058: 04 00 62 8e  	lw	$2, 0x4($19)
  19e05c: 00 00 64 8e  	lw	$4, 0x0($19)
  19e060: 08 00 31 26  	addiu	$17, $17, 0x8
  19e064: 00 00 63 8c  	lw	$3, 0x0($3)
  19e068: 23 10 44 00  	subu	$2, $2, $4
  19e06c: 7c ff 00 10  	b	0x19de60 <.text+0x9de60>
  19e070: 00 00 62 ac  	sw	$2, 0x0($3)
  19e074: 09 00 a0 10  	beqz	$5, 0x19e09c <.text+0x9e09c>
  19e078: 2d 10 20 02  	move	$2, $17
  19e07c: 08 00 31 26  	addiu	$17, $17, 0x8
  19e080: 00 00 45 dc  	ld	$5, 0x0($2)
  19e084: 1c 00 07 3c  	lui	$7, 0x1c
  19e088: 2d 40 c0 00  	move	$8, $6
  19e08c: a0 a4 e7 24  	addiu	$7, $7, -0x5b60 <.text+0xffffffffffefa4a0>
  19e090: 2d 20 60 02  	move	$4, $19
  19e094: cb ff 00 10  	b	0x19dfc4 <.text+0x9dfc4>
  19e098: 08 00 06 24  	addiu	$6, $zero, 0x8
  19e09c: 04 00 e0 10  	beqz	$7, 0x19e0b0 <.text+0x9e0b0>
  19e0a0: 00 00 00 00  	nop
  19e0a4: 08 00 31 26  	addiu	$17, $17, 0x8
  19e0a8: f6 ff 00 10  	b	0x19e084 <.text+0x9e084>
  19e0ac: 00 00 45 94  	lhu	$5, 0x0($2)
  19e0b0: 08 00 31 26  	addiu	$17, $17, 0x8
  19e0b4: f3 ff 00 10  	b	0x19e084 <.text+0x9e084>
  19e0b8: 00 00 45 9c  	lwu	$5, 0x0($2)
  19e0bc: 1c 00 07 3c  	lui	$7, 0x1c
  19e0c0: 00 00 25 9e  	lwu	$5, 0x0($17)
  19e0c4: 2d 40 c0 00  	move	$8, $6
  19e0c8: 78 a4 e7 24  	addiu	$7, $7, -0x5b88 <.text+0xffffffffffefa478>
  19e0cc: 2d 20 60 02  	move	$4, $19
  19e0d0: 10 00 06 24  	addiu	$6, $zero, 0x10
  19e0d4: 13 76 06 0c  	jal	0x19d84c <.text+0x9d84c>
  19e0d8: 2d 58 00 00  	move	$11, $zero
  19e0dc: 5e ff 00 10  	b	0x19de58 <.text+0x9de58>
  19e0e0: 08 00 31 26  	addiu	$17, $17, 0x8
  19e0e4: 00 00 25 8e  	lw	$5, 0x0($17)
  19e0e8: 2d 38 20 01  	move	$7, $9
  19e0ec: 2d 40 40 01  	move	$8, $10
  19e0f0: ea 76 06 0c  	jal	0x19dba8 <.text+0x9dba8>
  19e0f4: 2d 20 60 02  	move	$4, $19
  19e0f8: 57 ff 00 10  	b	0x19de58 <.text+0x9de58>
  19e0fc: 08 00 31 26  	addiu	$17, $17, 0x8
  19e100: 09 00 a0 10  	beqz	$5, 0x19e128 <.text+0x9e128>
  19e104: 2d 10 20 02  	move	$2, $17
  19e108: 08 00 31 26  	addiu	$17, $17, 0x8
  19e10c: 00 00 45 dc  	ld	$5, 0x0($2)
  19e110: 1c 00 07 3c  	lui	$7, 0x1c
  19e114: 2d 40 c0 00  	move	$8, $6
  19e118: 90 a4 e7 24  	addiu	$7, $7, -0x5b70 <.text+0xffffffffffefa490>
  19e11c: 2d 20 60 02  	move	$4, $19
  19e120: a8 ff 00 10  	b	0x19dfc4 <.text+0x9dfc4>
  19e124: 0a 00 06 24  	addiu	$6, $zero, 0xa
  19e128: 04 00 e0 10  	beqz	$7, 0x19e13c <.text+0x9e13c>
  19e12c: 00 00 00 00  	nop
  19e130: 08 00 31 26  	addiu	$17, $17, 0x8
  19e134: f6 ff 00 10  	b	0x19e110 <.text+0x9e110>
  19e138: 00 00 45 94  	lhu	$5, 0x0($2)
  19e13c: 08 00 31 26  	addiu	$17, $17, 0x8
  19e140: f3 ff 00 10  	b	0x19e110 <.text+0x9e110>
  19e144: 00 00 45 9c  	lwu	$5, 0x0($2)
  19e148: 14 00 62 8e  	lw	$2, 0x14($19)
  19e14c: 2d 20 60 02  	move	$4, $19
  19e150: 09 f8 40 00  	jalr	$2
  19e154: 25 00 05 24  	addiu	$5, $zero, 0x25
  19e158: 06 00 40 14  	bnez	$2, 0x19e174 <.text+0x9e174>
  19e15c: 2d 28 00 02  	move	$5, $16
  19e160: 14 00 62 8e  	lw	$2, 0x14($19)
  19e164: 09 f8 40 00  	jalr	$2
  19e168: 2d 20 60 02  	move	$4, $19
  19e16c: 3d ff 40 50  	beqzl	$2, 0x19de64 <.text+0x9de64>
  19e170: 00 00 50 92  	lbu	$16, 0x0($18)
  19e174: 3e ff 00 10  	b	0x19de70 <.text+0x9de70>
  19e178: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19e17c: 07 00 a0 10  	beqz	$5, 0x19e19c <.text+0x9e19c>
  19e180: 2d 10 20 02  	move	$2, $17
  19e184: 08 00 31 26  	addiu	$17, $17, 0x8
  19e188: 00 00 45 dc  	ld	$5, 0x0($2)
  19e18c: 1c 00 07 3c  	lui	$7, 0x1c
  19e190: 2d 40 c0 00  	move	$8, $6
  19e194: 89 ff 00 10  	b	0x19dfbc <.text+0x9dfbc>
  19e198: b0 a4 e7 24  	addiu	$7, $7, -0x5b50 <.text+0xffffffffffefa4b0>
  19e19c: 04 00 e0 10  	beqz	$7, 0x19e1b0 <.text+0x9e1b0>
  19e1a0: 00 00 00 00  	nop
  19e1a4: 08 00 31 26  	addiu	$17, $17, 0x8
  19e1a8: f8 ff 00 10  	b	0x19e18c <.text+0x9e18c>
  19e1ac: 00 00 45 94  	lhu	$5, 0x0($2)
  19e1b0: 08 00 31 26  	addiu	$17, $17, 0x8
  19e1b4: f5 ff 00 10  	b	0x19e18c <.text+0x9e18c>
  19e1b8: 00 00 45 9c  	lwu	$5, 0x0($2)
  19e1bc: 01 00 05 24  	addiu	$5, $zero, 0x1
  19e1c0: 65 ff 00 10  	b	0x19df58 <.text+0x9df58>
  19e1c4: 01 00 52 26  	addiu	$18, $18, 0x1
  19e1c8: 00 00 50 92  	lbu	$16, 0x0($18)
  19e1cc: fc ff 00 10  	b	0x19e1c0 <.text+0x9e1c0>
  19e1d0: 01 00 07 24  	addiu	$7, $zero, 0x1
  19e1d4: 2d 48 00 00  	move	$9, $zero
  19e1d8: d0 ff 02 26  	addiu	$2, $16, -0x30 <.text+0xffffffffffefffd0>
  19e1dc: 0a 00 42 2c  	sltiu	$2, $2, 0xa
  19e1e0: 0c 00 40 10  	beqz	$2, 0x19e214 <.text+0x9e214>
  19e1e4: 01 00 52 26  	addiu	$18, $18, 0x1
  19e1e8: 0a 00 03 24  	addiu	$3, $zero, 0xa
  19e1ec: 18 10 23 01  	<unknown>
  19e1f0: 21 18 50 00  	addu	$3, $2, $16
  19e1f4: 00 00 50 92  	lbu	$16, 0x0($18)
  19e1f8: d0 ff 69 24  	addiu	$9, $3, -0x30 <.text+0xffffffffffefffd0>
  19e1fc: d0 ff 02 26  	addiu	$2, $16, -0x30 <.text+0xffffffffffefffd0>
  19e200: 0a 00 42 2c  	sltiu	$2, $2, 0xa
  19e204: f8 ff 40 14  	bnez	$2, 0x19e1e8 <.text+0x9e1e8>
  19e208: 01 00 52 26  	addiu	$18, $18, 0x1
  19e20c: 4e ff 00 10  	b	0x19df48 <.text+0x9df48>
  19e210: 68 00 02 24  	addiu	$2, $zero, 0x68
  19e214: 2a 00 02 24  	addiu	$2, $zero, 0x2a
  19e218: 4b ff 02 16  	bne	$16, $2, 0x19df48 <.text+0x9df48>
  19e21c: 68 00 02 24  	addiu	$2, $zero, 0x68
  19e220: 00 00 50 92  	lbu	$16, 0x0($18)
  19e224: 01 00 52 26  	addiu	$18, $18, 0x1
  19e228: 00 00 29 8e  	lw	$9, 0x0($17)
  19e22c: 46 ff 00 10  	b	0x19df48 <.text+0x9df48>
  19e230: 08 00 31 26  	addiu	$17, $17, 0x8
  19e234: 2a 00 02 24  	addiu	$2, $zero, 0x2a
  19e238: 40 ff 02 16  	bne	$16, $2, 0x19df3c <.text+0x9df3c>
  19e23c: 2e 00 02 24  	addiu	$2, $zero, 0x2e
  19e240: 00 00 50 92  	lbu	$16, 0x0($18)
  19e244: 01 00 52 26  	addiu	$18, $18, 0x1
  19e248: 00 00 26 8e  	lw	$6, 0x0($17)
  19e24c: 3b ff 00 10  	b	0x19df3c <.text+0x9df3c>
  19e250: 08 00 31 26  	addiu	$17, $17, 0x8
  19e254: 21 ff 00 10  	b	0x19dedc <.text+0x9dedc>
  19e258: 08 00 4a 35  	ori	$10, $10, 0x8
  19e25c: 1f ff 00 10  	b	0x19dedc <.text+0x9dedc>
  19e260: 04 00 4a 35  	ori	$10, $10, 0x4
  19e264: 1d ff 00 10  	b	0x19dedc <.text+0x9dedc>
  19e268: 02 00 4a 35  	ori	$10, $10, 0x2
  19e26c: 1b ff 00 10  	b	0x19dedc <.text+0x9dedc>
  19e270: 01 00 4a 35  	ori	$10, $10, 0x1
  19e274: 04 00 83 8c  	lw	$3, 0x4($4)
  19e278: 08 00 82 8c  	lw	$2, 0x8($4)
  19e27c: 21 18 65 00  	addu	$3, $3, $5
  19e280: 08 00 e0 03  	jr	$ra
  19e284: 2b 10 43 00  	sltu	$2, $2, $3
  19e288: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19e28c: 2d 10 a0 00  	move	$2, $5
  19e290: 01 00 05 24  	addiu	$5, $zero, 0x1
  19e294: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19e298: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19e29c: ff 00 51 30  	andi	$17, $2, 0xff
  19e2a0: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19e2a4: 9d 78 06 0c  	jal	0x19e274 <.text+0x9e274>
  19e2a8: 2d 80 80 00  	move	$16, $4
  19e2ac: 06 00 40 14  	bnez	$2, 0x19e2c8 <.text+0x9e2c8>
  19e2b0: 01 00 03 24  	addiu	$3, $zero, 0x1
  19e2b4: 04 00 02 8e  	lw	$2, 0x4($16)
  19e2b8: 2d 18 00 00  	move	$3, $zero
  19e2bc: 00 00 51 a0  	sb	$17, 0x0($2)
  19e2c0: 01 00 42 24  	addiu	$2, $2, 0x1
  19e2c4: 04 00 02 ae  	sw	$2, 0x4($16)
  19e2c8: 20 00 bf df  	ld	$ra, 0x20($sp)
  19e2cc: 2d 10 60 00  	move	$2, $3
  19e2d0: 10 00 b1 df  	ld	$17, 0x10($sp)
  19e2d4: 00 00 b0 df  	ld	$16, 0x0($sp)
  19e2d8: 08 00 e0 03  	jr	$ra
  19e2dc: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19e2e0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19e2e4: 2d 18 80 00  	move	$3, $4
  19e2e8: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19e2ec: 2d 80 a0 00  	move	$16, $5
  19e2f0: 21 10 90 00  	addu	$2, $4, $16
  19e2f4: 2d 28 c0 00  	move	$5, $6
  19e2f8: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  19e2fc: 2d 20 a0 03  	move	$4, $sp
  19e300: 08 00 a2 af  	sw	$2, 0x8($sp)
  19e304: 2d 30 e0 00  	move	$6, $7
  19e308: 1a 00 02 3c  	lui	$2, 0x1a
  19e30c: 00 00 a3 af  	sw	$3, 0x0($sp)
  19e310: 88 e2 42 24  	addiu	$2, $2, -0x1d78 <.text+0xffffffffffefe288>
  19e314: 04 00 a3 af  	sw	$3, 0x4($sp)
  19e318: 14 00 a2 af  	sw	$2, 0x14($sp)
  19e31c: 1a 00 02 3c  	lui	$2, 0x1a
  19e320: 74 e2 42 24  	addiu	$2, $2, -0x1d8c <.text+0xffffffffffefe274>
  19e324: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19e328: 18 00 a2 af  	sw	$2, 0x18($sp)
  19e32c: 10 00 a0 af  	sw	$zero, 0x10($sp)
  19e330: 84 77 06 0c  	jal	0x19de10 <.text+0x9de10>
  19e334: 0c 00 b0 af  	sw	$16, 0xc($sp)
  19e338: 04 00 a3 8f  	lw	$3, 0x4($sp)
  19e33c: 04 00 40 14  	bnez	$2, 0x19e350 <.text+0x9e350>
  19e340: 00 00 60 a0  	sb	$zero, 0x0($3)
  19e344: 04 00 a3 8f  	lw	$3, 0x4($sp)
  19e348: 00 00 a2 8f  	lw	$2, 0x0($sp)
  19e34c: 23 80 62 00  	subu	$16, $3, $2
  19e350: 2d 10 00 02  	move	$2, $16
  19e354: 30 00 bf df  	ld	$ra, 0x30($sp)
  19e358: 20 00 b0 df  	ld	$16, 0x20($sp)
  19e35c: 08 00 e0 03  	jr	$ra
  19e360: 40 00 bd 27  	addiu	$sp, $sp, 0x40
