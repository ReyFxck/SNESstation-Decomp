# Focused R5900 extract: 0x0019fb00..0x0019fd20

  19fb00: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19fb04: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19fb08: 2d 88 80 00  	move	$17, $4
  19fb0c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19fb10: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19fb14: 00 60 10 40  	mfc0	$16, $12, 0x0
  19fb18: 01 00 02 3c  	lui	$2, 0x1
  19fb1c: 24 80 02 02  	and	$16, $16, $2
  19fb20: 11 00 00 16  	bnez	$16, 0x19fb68 <.text+0x9fb68>
  19fb24: 00 00 00 00  	nop
  19fb28: 34 7f 06 0c  	jal	0x19fcd0 <.text+0x9fcd0>
  19fb2c: 2d 20 20 02  	move	$4, $17
  19fb30: 2d 88 40 00  	move	$17, $2
  19fb34: 0f 00 00 00  	sync
  19fb38: 07 00 00 16  	bnez	$16, 0x19fb58 <.text+0x9fb58>
  19fb3c: 00 00 00 00  	nop
  19fb40: 2d 10 20 02  	move	$2, $17
  19fb44: 20 00 bf df  	ld	$ra, 0x20($sp)
  19fb48: 10 00 b1 df  	ld	$17, 0x10($sp)
  19fb4c: 00 00 b0 df  	ld	$16, 0x0($sp)
  19fb50: 08 00 e0 03  	jr	$ra
  19fb54: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19fb58: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19fb5c: 00 00 00 00  	nop
  19fb60: f8 ff 00 10  	b	0x19fb44 <.text+0x9fb44>
  19fb64: 2d 10 20 02  	move	$2, $17
  19fb68: 06 7c 06 0c  	jal	0x19f018 <.text+0x9f018>
  19fb6c: 00 00 00 00  	nop
  19fb70: ed ff 00 10  	b	0x19fb28 <.text+0x9fb28>
  19fb74: 00 00 00 00  	nop
  19fb78: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19fb7c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19fb80: 2d 88 80 00  	move	$17, $4
  19fb84: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19fb88: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19fb8c: 00 60 10 40  	mfc0	$16, $12, 0x0
  19fb90: 01 00 02 3c  	lui	$2, 0x1
  19fb94: 24 80 02 02  	and	$16, $16, $2
  19fb98: 11 00 00 16  	bnez	$16, 0x19fbe0 <.text+0x9fbe0>
  19fb9c: 00 00 00 00  	nop
  19fba0: 38 7f 06 0c  	jal	0x19fce0 <.text+0x9fce0>
  19fba4: 2d 20 20 02  	move	$4, $17
  19fba8: 2d 88 40 00  	move	$17, $2
  19fbac: 0f 00 00 00  	sync
  19fbb0: 07 00 00 16  	bnez	$16, 0x19fbd0 <.text+0x9fbd0>
  19fbb4: 00 00 00 00  	nop
  19fbb8: 2d 10 20 02  	move	$2, $17
  19fbbc: 20 00 bf df  	ld	$ra, 0x20($sp)
  19fbc0: 10 00 b1 df  	ld	$17, 0x10($sp)
  19fbc4: 00 00 b0 df  	ld	$16, 0x0($sp)
  19fbc8: 08 00 e0 03  	jr	$ra
  19fbcc: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19fbd0: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19fbd4: 00 00 00 00  	nop
  19fbd8: f8 ff 00 10  	b	0x19fbbc <.text+0x9fbbc>
  19fbdc: 2d 10 20 02  	move	$2, $17
  19fbe0: 06 7c 06 0c  	jal	0x19f018 <.text+0x9f018>
  19fbe4: 00 00 00 00  	nop
  19fbe8: ed ff 00 10  	b	0x19fba0 <.text+0x9fba0>
  19fbec: 00 00 00 00  	nop
  19fbf0: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  19fbf4: 80 00 b0 ff  	sd	$16, 0x80($sp)
  19fbf8: 90 00 bf ff  	sd	$ra, 0x90($sp)
  19fbfc: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19fc00: 42 00 10 3c  	lui	$16, 0x42
  19fc04: 88 5a 05 8e  	lw	$5, 0x5a88($16)
  19fc08: 2d 30 00 00  	move	$6, $zero
  19fc0c: 00 00 a2 90  	lbu	$2, 0x0($5)
  19fc10: 23 00 40 10  	beqz	$2, 0x19fca0 <.text+0x9fca0>
  19fc14: 88 5a 10 26  	addiu	$16, $16, 0x5a88
  19fc18: 1e 00 42 24  	addiu	$2, $2, 0x1e
  19fc1c: 03 21 02 00  	sra	$4, $2, 0x4
  19fc20: 0e 00 80 10  	beqz	$4, 0x19fc5c <.text+0x9fc5c>
  19fc24: 00 00 a0 ac  	sw	$zero, 0x0($5)
  19fc28: ff ff 84 24  	addiu	$4, $4, -0x1 <.text+0xffffffffffefffff>
  19fc2c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19fc30: 0a 00 82 10  	beq	$4, $2, 0x19fc5c <.text+0x9fc5c>
  19fc34: 00 00 00 00  	nop
  19fc38: 00 19 06 00  	sll	$3, $6, 0x4
  19fc3c: ff ff 84 24  	addiu	$4, $4, -0x1 <.text+0xffffffffffefffff>
  19fc40: 21 10 65 00  	addu	$2, $3, $5
  19fc44: 00 00 42 78  	lq	$v0, 0x0($v0)
  19fc48: 21 18 7d 00  	addu	$3, $3, $sp
  19fc4c: 00 00 62 7c  	ext	$2, $3, 0x0, 0x1
  19fc50: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19fc54: f8 ff 82 14  	bne	$4, $2, 0x19fc38 <.text+0x9fc38>
  19fc58: 01 00 c6 24  	addiu	$6, $6, 0x1
  19fc5c: 44 7f 06 0c  	jal	0x19fd10 <.text+0x9fd10>
  19fc60: 00 00 00 00  	nop
  19fc64: 08 00 a3 8f  	lw	$3, 0x8($sp)
  19fc68: ff 7f 02 3c  	lui	$2, 0x7fff
  19fc6c: ff ff 42 34  	ori	$2, $2, 0xffff
  19fc70: 24 20 62 00  	and	$4, $3, $2
  19fc74: 20 00 82 28  	slti	$2, $4, 0x20
  19fc78: 09 00 40 10  	beqz	$2, 0x19fca0 <.text+0x9fca0>
  19fc7c: 00 00 00 00  	nop
  19fc80: 02 00 63 04  	bgezl	$3, 0x19fc8c <.text+0x9fc8c>
  19fc84: 14 00 10 8e  	lw	$16, 0x14($16)
  19fc88: 0c 00 10 8e  	lw	$16, 0xc($16)
  19fc8c: c0 10 04 00  	sll	$2, $4, 0x3
  19fc90: 21 10 50 00  	addu	$2, $2, $16
  19fc94: 00 00 43 8c  	lw	$3, 0x0($2)
  19fc98: 09 00 60 54  	bnezl	$3, 0x19fcc0 <.text+0x9fcc0>
  19fc9c: 04 00 45 8c  	lw	$5, 0x4($2)
  19fca0: 0f 00 00 00  	sync
  19fca4: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19fca8: 00 00 00 00  	nop
  19fcac: 80 00 b0 df  	ld	$16, 0x80($sp)
  19fcb0: 90 00 bf df  	ld	$ra, 0x90($sp)
  19fcb4: 2d 10 00 00  	move	$2, $zero
  19fcb8: 08 00 e0 03  	jr	$ra
  19fcbc: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  19fcc0: 09 f8 60 00  	jalr	$3
  19fcc4: 2d 20 a0 03  	move	$4, $sp
  19fcc8: f5 ff 00 10  	b	0x19fca0 <.text+0x9fca0>
  19fccc: 00 00 00 00  	nop
  19fcd0: 16 00 03 24  	addiu	$3, $zero, 0x16
  19fcd4: 0c 00 00 00  	syscall
  19fcd8: 08 00 e0 03  	jr	$ra
  19fcdc: 00 00 00 00  	nop
  19fce0: 17 00 03 24  	addiu	$3, $zero, 0x17
  19fce4: 0c 00 00 00  	syscall
  19fce8: 08 00 e0 03  	jr	$ra
  19fcec: 00 00 00 00  	nop
  19fcf0: 42 00 03 24  	addiu	$3, $zero, 0x42
  19fcf4: 0c 00 00 00  	syscall
  19fcf8: 08 00 e0 03  	jr	$ra
  19fcfc: 00 00 00 00  	nop
  19fd00: 45 00 03 24  	addiu	$3, $zero, 0x45
  19fd04: 0c 00 00 00  	syscall
  19fd08: 08 00 e0 03  	jr	$ra
  19fd0c: 00 00 00 00  	nop
  19fd10: 88 ff 03 24  	addiu	$3, $zero, -0x78 <.text+0xffffffffffefff88>
  19fd14: 0c 00 00 00  	syscall
  19fd18: 08 00 e0 03  	jr	$ra
  19fd1c: 00 00 00 00  	nop
