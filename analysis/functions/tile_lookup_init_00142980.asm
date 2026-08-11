
build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  142980: 38 2a b0 24  	addiu	$16, $5, 0x2a38
  142984: df ff 00 10  	b	0x142904 <.text+0x42904>
  142988: 30 00 bf df  	ld	$ra, 0x30($sp)
  14298c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  142990: da ff 00 10  	b	0x1428fc <.text+0x428fc>
  142994: 84 00 02 ae  	sw	$2, 0x84($16)
  142998: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  14299c: 34 00 05 3c  	lui	$5, 0x34
  1429a0: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1429a4: 38 2a a2 24  	addiu	$2, $5, 0x2a38
  1429a8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1429ac: 2d 90 80 00  	move	$18, $4
  1429b0: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1429b4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1429b8: 48 00 42 8c  	lw	$2, 0x48($2)
  1429bc: 20 00 42 30  	andi	$2, $2, 0x20
  1429c0: 03 00 40 10  	beqz	$2, 0x1429d0 <.text+0x429d0>
  1429c4: 2d 88 00 00  	move	$17, $zero
  1429c8: 08 00 80 14  	bnez	$4, 0x1429ec <.text+0x429ec>
  1429cc: 38 2a b0 24  	addiu	$16, $5, 0x2a38
  1429d0: 2d 10 20 02  	move	$2, $17
  1429d4: 30 00 bf df  	ld	$ra, 0x30($sp)
  1429d8: 20 00 b2 df  	ld	$18, 0x20($sp)
  1429dc: 10 00 b1 df  	ld	$17, 0x10($sp)
  1429e0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1429e4: 08 00 e0 03  	jr	$ra
  1429e8: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1429ec: 3c 00 04 96  	lhu	$4, 0x3c($16)
  1429f0: d8 01 03 8e  	lw	$3, 0x1d8($16)
  1429f4: 48 00 02 8e  	lw	$2, 0x48($16)
  1429f8: 21 18 64 00  	addu	$3, $3, $4
  1429fc: 6d 00 04 92  	lbu	$4, 0x6d($16)
  142a00: 00 00 65 90  	lbu	$5, 0x0($3)
  142a04: 00 03 42 30  	andi	$2, $2, 0x300
  142a08: 34 00 03 3c  	lui	$3, 0x34
  142a0c: 25 10 44 00  	or	$2, $2, $4
  142a10: 3c 32 63 8c  	lw	$3, 0x323c($3)
  142a14: 80 10 02 00  	sll	$2, $2, 0x2
  142a18: 6d 00 05 a2  	sb	$5, 0x6d($16)
  142a1c: 21 10 43 00  	addu	$2, $2, $3
  142a20: 00 00 42 8c  	lw	$2, 0x0($2)
  142a24: 09 f8 40 00  	jalr	$2
  142a28: 01 00 31 26  	addiu	$17, $17, 0x1
  142a2c: 34 00 05 3c  	lui	$5, 0x34
  142a30: 90 00 02 8e  	lw	$2, 0x90($16)
  142a34: 3c 00 03 96  	lhu	$3, 0x3c($16)
  142a38: 0c 00 62 10  	beq	$3, $2, 0x142a6c <.text+0x42a6c>
  142a3c: 2b 20 32 02  	sltu	$4, $17, $18
  142a40: 94 00 02 8e  	lw	$2, 0x94($16)
  142a44: e3 ff 62 10  	beq	$3, $2, 0x1429d4 <.text+0x429d4>
  142a48: 2d 10 20 02  	move	$2, $17
  142a4c: 48 00 02 8e  	lw	$2, 0x48($16)
  142a50: 20 00 42 30  	andi	$2, $2, 0x20
  142a54: df ff 40 10  	beqz	$2, 0x1429d4 <.text+0x429d4>
  142a58: 2d 10 20 02  	move	$2, $17
  142a5c: e3 ff 80 14  	bnez	$4, 0x1429ec <.text+0x429ec>
  142a60: 38 2a b0 24  	addiu	$16, $5, 0x2a38
  142a64: dc ff 00 10  	b	0x1429d8 <.text+0x429d8>
  142a68: 30 00 bf df  	ld	$ra, 0x30($sp)
  142a6c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  142a70: d7 ff 00 10  	b	0x1429d0 <.text+0x429d0>
  142a74: 84 00 02 ae  	sw	$2, 0x84($16)
  142a78: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  142a7c: 01 00 0b 24  	addiu	$11, $zero, 0x1
  142a80: 20 00 bf ff  	sd	$ra, 0x20($sp)
  142a84: 02 00 0a 24  	addiu	$10, $zero, 0x2
  142a88: 10 00 b1 ff  	sd	$17, 0x10($sp)
  142a8c: 2d 68 00 00  	move	$13, $zero
  142a90: 00 00 b0 ff  	sd	$16, 0x0($sp)
  142a94: 2d 48 00 00  	move	$9, $zero
  142a98: 08 00 2c 31  	andi	$12, $9, 0x8
  142a9c: 2d 30 00 00  	move	$6, $zero
  142aa0: 0b 30 6c 01  	movn	$6, $11, $12
  142aa4: 04 00 28 31  	andi	$8, $9, 0x4
  142aa8: 03 00 00 11  	beqz	$8, 0x142ab8 <.text+0x42ab8>
  142aac: 2d 20 c0 00  	move	$4, $6
  142ab0: 00 12 0b 00  	sll	$2, $11, 0x8
  142ab4: 25 30 c2 00  	or	$6, $6, $2
  142ab8: 02 00 27 31  	andi	$7, $9, 0x2
  142abc: 03 00 e0 10  	beqz	$7, 0x142acc <.text+0x42acc>
  142ac0: 01 00 23 31  	andi	$3, $9, 0x1
  142ac4: 00 14 0b 00  	sll	$2, $11, 0x10
  142ac8: 25 30 c2 00  	or	$6, $6, $2
  142acc: 02 00 60 10  	beqz	$3, 0x142ad8 <.text+0x42ad8>
  142ad0: 00 16 0b 00  	sll	$2, $11, 0x18
  142ad4: 25 30 c2 00  	or	$6, $6, $2
  142ad8: 03 00 00 11  	beqz	$8, 0x142ae8 <.text+0x42ae8>
  142adc: 2d 28 80 00  	move	$5, $4
  142ae0: 00 12 0b 00  	sll	$2, $11, 0x8
  142ae4: 25 28 82 00  	or	$5, $4, $2
  142ae8: 02 00 e0 10  	beqz	$7, 0x142af4 <.text+0x42af4>
  142aec: 00 14 0b 00  	sll	$2, $11, 0x10
  142af0: 25 28 a2 00  	or	$5, $5, $2
  142af4: 04 00 60 10  	beqz	$3, 0x142b08 <.text+0x42b08>
  142af8: 00 11 0d 00  	sll	$2, $13, 0x4
  142afc: 00 16 0b 00  	sll	$2, $11, 0x18
  142b00: 25 28 a2 00  	or	$5, $5, $2
  142b04: 00 11 0d 00  	sll	$2, $13, 0x4
  142b08: 21 10 49 00  	addu	$2, $2, $9
  142b0c: 80 20 02 00  	sll	$4, $2, 0x2
  142b10: 36 00 02 3c  	lui	$2, 0x36
  142b14: a0 f9 42 24  	addiu	$2, $2, -0x660 <.text+0xffffffffffeff9a0>
  142b18: 21 10 82 00  	addu	$2, $4, $2
  142b1c: 00 00 46 ac  	sw	$6, 0x0($2)
  142b20: 2d 30 00 00  	move	$6, $zero
  142b24: 36 00 02 3c  	lui	$2, 0x36
  142b28: 0b 30 4c 01  	movn	$6, $10, $12
  142b2c: a0 fa 42 24  	addiu	$2, $2, -0x560 <.text+0xffffffffffeffaa0>
  142b30: 21 10 82 00  	addu	$2, $4, $2
  142b34: 00 00 45 ac  	sw	$5, 0x0($2)
  142b38: 03 00 00 11  	beqz	$8, 0x142b48 <.text+0x42b48>
  142b3c: 2d 28 c0 00  	move	$5, $6
  142b40: 00 12 0a 00  	sll	$2, $10, 0x8
  142b44: 25 30 c2 00  	or	$6, $6, $2
  142b48: 02 00 e0 10  	beqz	$7, 0x142b54 <.text+0x42b54>
  142b4c: 00 14 0a 00  	sll	$2, $10, 0x10
  142b50: 25 30 c2 00  	or	$6, $6, $2
  142b54: 02 00 60 10  	beqz	$3, 0x142b60 <.text+0x42b60>
  142b58: 00 16 0a 00  	sll	$2, $10, 0x18
  142b5c: 25 30 c2 00  	or	$6, $6, $2
  142b60: 02 00 00 11  	beqz	$8, 0x142b6c <.text+0x42b6c>
  142b64: 00 12 0a 00  	sll	$2, $10, 0x8
  142b68: 25 28 a2 00  	or	$5, $5, $2
  142b6c: 02 00 e0 10  	beqz	$7, 0x142b78 <.text+0x42b78>
  142b70: 00 14 0a 00  	sll	$2, $10, 0x10
  142b74: 25 28 a2 00  	or	$5, $5, $2
  142b78: 04 00 60 10  	beqz	$3, 0x142b8c <.text+0x42b8c>
  142b7c: 01 00 22 25  	addiu	$2, $9, 0x1
  142b80: 00 16 0a 00  	sll	$2, $10, 0x18
  142b84: 25 28 a2 00  	or	$5, $5, $2
  142b88: 01 00 22 25  	addiu	$2, $9, 0x1
  142b8c: 36 00 03 3c  	lui	$3, 0x36
  142b90: 00 16 02 00  	sll	$2, $2, 0x18
  142b94: a0 fc 63 24  	addiu	$3, $3, -0x360 <.text+0xffffffffffeffca0>
  142b98: 03 4e 02 00  	sra	$9, $2, 0x18
  142b9c: 21 18 83 00  	addu	$3, $4, $3
  142ba0: 36 00 02 3c  	lui	$2, 0x36
  142ba4: 00 00 65 ac  	sw	$5, 0x0($3)
  142ba8: a0 fb 42 24  	addiu	$2, $2, -0x460 <.text+0xffffffffffeffba0>
  142bac: 21 10 82 00  	addu	$2, $4, $2
  142bb0: 10 00 24 29  	slti	$4, $9, 0x10
  142bb4: b8 ff 80 14  	bnez	$4, 0x142a98 <.text+0x42a98>
  142bb8: 00 00 46 ac  	sw	$6, 0x0($2)
  142bbc: 01 00 a2 25  	addiu	$2, $13, 0x1
  142bc0: 80 50 0a 00  	sll	$10, $10, 0x2
  142bc4: ff 00 4d 30  	andi	$13, $2, 0xff
  142bc8: 04 00 a2 2d  	sltiu	$2, $13, 0x4
  142bcc: b1 ff 40 14  	bnez	$2, 0x142a94 <.text+0x42a94>
  142bd0: 80 58 0b 00  	sll	$11, $11, 0x2
  142bd4: 34 00 02 3c  	lui	$2, 0x34
  142bd8: 36 00 03 3c  	lui	$3, 0x36
  142bdc: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  142be0: e0 54 46 24  	addiu	$6, $2, 0x54e0
  142be4: 10 00 e8 8c  	lw	$8, 0x10($7)
  142be8: 83 00 c2 90  	lbu	$2, 0x83($6)
  142bec: 28 00 e8 ac  	sw	$8, 0x28($7)
  142bf0: 24 00 e8 ac  	sw	$8, 0x24($7)
  142bf4: 03 00 40 10  	beqz	$2, 0x142c04 <.text+0x42c04>
  142bf8: 2c 00 e8 ac  	sw	$8, 0x2c($7)
  142bfc: 42 10 08 00  	srl	$2, $8, 0x1
  142c00: 2c 00 e2 ac  	sw	$2, 0x2c($7)
  142c04: 36 00 02 3c  	lui	$2, 0x36
  142c08: 80 d4 65 8c  	lw	$5, -0x2b80($3)
  142c0c: 68 c2 49 24  	addiu	$9, $2, -0x3d98 <.text+0xffffffffffefc268>
  142c10: 08 00 e4 8c  	lw	$4, 0x8($7)
  142c14: 04 00 e3 8c  	lw	$3, 0x4($7)
  142c18: 0c 00 e2 8c  	lw	$2, 0xc($7)
  142c1c: 23 18 65 00  	subu	$3, $3, $5
  142c20: 84 00 c5 90  	lbu	$5, 0x84($6)
  142c24: 23 10 44 00  	subu	$2, $2, $4
  142c28: 43 18 03 00  	sra	$3, $3, 0x1
  142c2c: 01 00 04 24  	addiu	$4, $zero, 0x1
  142c30: 48 00 e2 ac  	sw	$2, 0x48($7)
  142c34: 36 00 02 3c  	lui	$2, 0x36
  142c38: 14 00 e3 ac  	sw	$3, 0x14($7)
  142c3c: f8 bf 40 a0  	sb	$zero, -0x4008($2)
