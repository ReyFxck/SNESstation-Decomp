  19b7f0: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19b7f4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19b7f8: 00 00 82 8c  	lw	$2, 0x0($4)
  19b7fc: 3c 00 40 10  	beqz	$2, 0x19b8f0 <.text+0x9b8f0>
  19b800: 2d 60 80 00  	move	$12, $4
  19b804: 3a 00 a0 10  	beqz	$5, 0x19b8f0 <.text+0x9b8f0>
  19b808: 32 00 8b 24  	addiu	$11, $4, 0x32
  19b80c: 04 00 86 ac  	sw	$6, 0x4($4)
  19b810: 08 00 87 ac  	sw	$7, 0x8($4)
  19b814: 20 00 a6 24  	addiu	$6, $5, 0x20
  19b818: 0c 00 88 ac  	sw	$8, 0xc($4)
  19b81c: 25 20 cb 00  	or	$4, $6, $11
  19b820: 10 00 89 ad  	sw	$9, 0x10($12)
  19b824: 07 00 84 30  	andi	$4, $4, 0x7
  19b828: 04 00 a2 8c  	lw	$2, 0x4($5)
  19b82c: 14 00 82 ad  	sw	$2, 0x14($12)
  19b830: 08 00 a3 8c  	lw	$3, 0x8($5)
  19b834: 18 00 83 ad  	sw	$3, 0x18($12)
  19b838: 18 00 a2 8c  	lw	$2, 0x18($5)
  19b83c: 24 00 82 ad  	sw	$2, 0x24($12)
  19b840: 1c 00 a3 8c  	lw	$3, 0x1c($5)
  19b844: 28 00 83 ad  	sw	$3, 0x28($12)
  19b848: 10 00 a2 8c  	lw	$2, 0x10($5)
  19b84c: 1c 00 82 ad  	sw	$2, 0x1c($12)
  19b850: 14 00 a3 8c  	lw	$3, 0x14($5)
  19b854: 20 00 83 ad  	sw	$3, 0x20($12)
  19b858: 0c 00 a2 8c  	lw	$2, 0xc($5)
  19b85c: 28 00 80 10  	beqz	$4, 0x19b900 <.text+0x9b900>
  19b860: 2c 00 82 ad  	sw	$2, 0x2c($12)
  19b864: 20 01 aa 24  	addiu	$10, $5, 0x120
  19b868: 2d 10 40 01  	move	$2, $10
  19b86c: 00 00 00 00  	nop
  19b870: 07 00 c3 68  	ldl	$3, 0x7($6)
  19b874: 00 00 c3 6c  	ldr	$3, 0x0($6)
  19b878: 0f 00 c4 68  	ldl	$4, 0xf($6)
  19b87c: 08 00 c4 6c  	ldr	$4, 0x8($6)
  19b880: 17 00 c5 68  	ldl	$5, 0x17($6)
  19b884: 10 00 c5 6c  	ldr	$5, 0x10($6)
  19b888: 1f 00 c7 68  	ldl	$7, 0x1f($6)
  19b88c: 18 00 c7 6c  	ldr	$7, 0x18($6)
  19b890: 07 00 63 b1  	sdl	$3, 0x7($11)
  19b894: 00 00 63 b5  	sdr	$3, 0x0($11)
  19b898: 0f 00 64 b1  	sdl	$4, 0xf($11)
  19b89c: 08 00 64 b5  	sdr	$4, 0x8($11)
  19b8a0: 17 00 65 b1  	sdl	$5, 0x17($11)
  19b8a4: 10 00 65 b5  	sdr	$5, 0x10($11)
  19b8a8: 1f 00 67 b1  	sdl	$7, 0x1f($11)
  19b8ac: 18 00 67 b5  	sdr	$7, 0x18($11)
  19b8b0: 20 00 c6 24  	addiu	$6, $6, 0x20
  19b8c0: eb ff c2 14  	bne	$6, $2, 0x19b870 <.text+0x9b870>
  19b8c4: 20 00 6b 25  	addiu	$11, $11, 0x20
  19b8c8: 14 00 8b 8d  	lw	$11, 0x14($12)
  19b8cc: 18 00 82 8d  	lw	$2, 0x18($12)
  19b8d0: 00 00 84 8d  	lw	$4, 0x0($12)
  19b8d4: 04 00 85 8d  	lw	$5, 0x4($12)
  19b8d8: 08 00 86 8d  	lw	$6, 0x8($12)
  19b8dc: 0c 00 87 8d  	lw	$7, 0xc($12)
  19b8e0: 10 00 88 8d  	lw	$8, 0x10($12)
  19b8e4: 2c 00 89 8d  	lw	$9, 0x2c($12)
  19b8e8: e2 67 06 0c  	jal	0x199f88 <.text+0x99f88>
  19b8ec: 00 00 a2 af  	sw	$2, 0x0($sp)
  19b8f0: 10 00 bf df  	ld	$ra, 0x10($sp)
  19b8f4: 08 00 e0 03  	jr	$ra
  19b8f8: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19b8fc: 00 00 00 00  	nop
  19b900: 20 01 aa 24  	addiu	$10, $5, 0x120
  19b904: 2d 10 40 01  	move	$2, $10
  19b908: 00 00 c3 dc  	ld	$3, 0x0($6)
  19b90c: 08 00 c4 dc  	ld	$4, 0x8($6)
  19b910: 10 00 c5 dc  	ld	$5, 0x10($6)
  19b914: 18 00 c7 dc  	ld	$7, 0x18($6)
  19b918: 00 00 63 fd  	sd	$3, 0x0($11)
  19b91c: 08 00 64 fd  	sd	$4, 0x8($11)
  19b920: 10 00 65 fd  	sd	$5, 0x10($11)
  19b924: 18 00 67 fd  	sd	$7, 0x18($11)
  19b928: 20 00 c6 24  	addiu	$6, $6, 0x20
  19b938: f3 ff c2 14  	bne	$6, $2, 0x19b908 <.text+0x9b908>
  19b93c: 20 00 6b 25  	addiu	$11, $11, 0x20
  19b940: e2 ff 00 10  	b	0x19b8cc <.text+0x9b8cc>
  19b944: 14 00 8b 8d  	lw	$11, 0x14($12)
  19b948: 40 ff bd 27  	addiu	$sp, $sp, -0xc0 <.text+0xffffffffffefff40>
  19b94c: a0 00 be ff  	sd	$fp, 0xa0($sp)
  19b950: 2d f0 00 01  	move	$fp, $8
  19b954: 90 00 b7 ff  	sd	$23, 0x90($sp)
  19b958: 2d b8 20 01  	move	$23, $9
  19b95c: 70 00 b5 ff  	sd	$21, 0x70($sp)
  19b960: 2d a8 a0 00  	move	$21, $5
  19b964: 60 00 b4 ff  	sd	$20, 0x60($sp)
  19b968: 2d a0 40 01  	move	$20, $10
  19b96c: 50 00 b3 ff  	sd	$19, 0x50($sp)
  19b970: 2d 98 e0 00  	move	$19, $7
  19b974: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19b978: 2d 88 80 00  	move	$17, $4
  19b97c: b0 00 bf ff  	sd	$ra, 0xb0($sp)
  19b980: 80 00 b6 ff  	sd	$22, 0x80($sp)
  19b984: 40 00 b2 ff  	sd	$18, 0x40($sp)
  19b988: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19b98c: 00 00 82 8c  	lw	$2, 0x0($4)
  19b990: 38 00 40 10  	beqz	$2, 0x19ba74 <.text+0x9ba74>
  19b994: 18 00 a6 af  	sw	$6, 0x18($sp)
  19b998: 36 00 60 11  	beqz	$11, 0x19ba74 <.text+0x9ba74>
  19b99c: 2d 90 60 01  	move	$18, $11
  19b9a0: 30 00 80 a0  	sb	$zero, 0x30($4)
  19b9a4: 31 00 80 a0  	sb	$zero, 0x31($4)
  19b9a8: 14 00 84 8c  	lw	$4, 0x14($4)
  19b9ac: 18 00 a2 8f  	lw	$2, 0x18($sp)
  19b9b0: e4 c1 06 0c  	jal	0x1b0790 <.text+0xb0790>
  19b9b4: 23 b0 55 00  	subu	$22, $2, $21
  19b9b8: 18 00 24 8e  	lw	$4, 0x18($17)
  19b9bc: e4 c1 06 0c  	jal	0x1b0790 <.text+0xb0790>
  19b9c0: 2d 80 40 00  	move	$16, $2
  19b9c4: 2c 00 29 8e  	lw	$9, 0x2c($17)
  19b9c8: 00 00 24 8e  	lw	$4, 0x0($17)
  19b9cc: 2d 40 40 00  	move	$8, $2
  19b9d0: 04 00 25 8e  	lw	$5, 0x4($17)
  19b9d4: 2d 58 00 00  	move	$11, $zero
  19b9d8: 08 00 26 8e  	lw	$6, 0x8($17)
  19b9dc: 2d 38 00 02  	move	$7, $16
  19b9e0: 2d 50 00 00  	move	$10, $zero
  19b9e4: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19b9e8: 62 69 06 0c  	jal	0x19a588 <.text+0x9a588>
  19b9ec: 08 00 a0 af  	sw	$zero, 0x8($sp)
  19b9f0: 00 00 42 82  	lb	$2, 0x0($18)
  19b9f4: 1f 00 40 10  	beqz	$2, 0x19ba74 <.text+0x9ba74>
  19b9f8: 03 00 90 2a  	slti	$16, $20, 0x3
  19b9fc: 00 00 00 00  	nop
  19ba00: 2d 28 40 02  	move	$5, $18
  19ba04: 2d 20 20 02  	move	$4, $17
  19ba08: 2d 30 c0 02  	move	$6, $22
  19ba0c: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19ba10: 14 00 a8 27  	addiu	$8, $sp, 0x14
  19ba14: 10 00 a0 af  	sw	$zero, 0x10($sp)
  19ba18: b4 6e 06 0c  	jal	0x19bad0 <.text+0x9bad0>
  19ba1c: 14 00 a0 af  	sw	$zero, 0x14($sp)
  19ba20: 02 00 03 24  	addiu	$3, $zero, 0x2
  19ba24: 24 00 83 12  	beq	$20, $3, 0x19bab8 <.text+0x9bab8>
  19ba28: 10 00 a2 8f  	lw	$2, 0x10($sp)
  19ba2c: 04 00 00 16  	bnez	$16, 0x19ba40 <.text+0x9ba40>
  19ba30: 2d 28 a0 02  	move	$5, $21
  19ba34: 03 00 02 24  	addiu	$2, $zero, 0x3
  19ba38: 1b 00 82 12  	beq	$20, $2, 0x19baa8 <.text+0x9baa8>
  19ba3c: 10 00 a2 8f  	lw	$2, 0x10($sp)
  19ba40: 14 00 a9 8f  	lw	$9, 0x14($sp)
  19ba44: 2d 30 60 02  	move	$6, $19
  19ba48: 2d 50 40 02  	move	$10, $18
  19ba4c: 2d 20 20 02  	move	$4, $17
  19ba50: 2d 38 c0 03  	move	$7, $fp
  19ba54: da 6e 06 0c  	jal	0x19bb68 <.text+0x9bb68>
  19ba58: 2d 40 e0 02  	move	$8, $23
  19ba5c: 14 00 a3 8f  	lw	$3, 0x14($sp)
  19ba60: 28 00 22 8e  	lw	$2, 0x28($17)
  19ba64: 21 90 43 02  	addu	$18, $18, $3
  19ba68: 00 00 44 82  	lb	$4, 0x0($18)
  19ba6c: e4 ff 80 14  	bnez	$4, 0x19ba00 <.text+0x9ba00>
  19ba70: 21 98 62 02  	addu	$19, $19, $2
  19ba74: b0 00 bf df  	ld	$ra, 0xb0($sp)
  19ba78: a0 00 be df  	ld	$fp, 0xa0($sp)
  19ba7c: 90 00 b7 df  	ld	$23, 0x90($sp)
  19ba80: 80 00 b6 df  	ld	$22, 0x80($sp)
  19ba84: 70 00 b5 df  	ld	$21, 0x70($sp)
  19ba88: 60 00 b4 df  	ld	$20, 0x60($sp)
  19ba8c: 50 00 b3 df  	ld	$19, 0x50($sp)
  19ba90: 40 00 b2 df  	ld	$18, 0x40($sp)
  19ba94: 30 00 b1 df  	ld	$17, 0x30($sp)
  19ba98: 20 00 b0 df  	ld	$16, 0x20($sp)
  19ba9c: 08 00 e0 03  	jr	$ra
  19baa0: c0 00 bd 27  	addiu	$sp, $sp, 0xc0
  19baa4: 00 00 00 00  	nop
  19baa8: 18 00 a3 8f  	lw	$3, 0x18($sp)
  19baac: e4 ff 00 10  	b	0x19ba40 <.text+0x9ba40>
  19bab0: 23 28 62 00  	subu	$5, $3, $2
  19bab4: 00 00 00 00  	nop
  19bab8: 23 10 c2 02  	subu	$2, $22, $2
  19babc: c2 1f 02 00  	srl	$3, $2, 0x1f
  19bac0: 21 10 43 00  	addu	$2, $2, $3
  19bac4: 43 10 02 00  	sra	$2, $2, 0x1
  19bac8: dd ff 00 10  	b	0x19ba40 <.text+0x9ba40>
  19bacc: 21 28 a2 02  	addu	$5, $21, $2
  19bad0: 00 00 e0 ac  	sw	$zero, 0x0($7)
  19bad4: 2d 50 80 00  	move	$10, $4
  19bad8: 00 00 00 ad  	sw	$zero, 0x0($8)
  19badc: 00 00 a9 90  	lbu	$9, 0x0($5)
  19bae0: ff 00 23 31  	andi	$3, $9, 0xff
  19bae4: 0e 00 62 2c  	sltiu	$2, $3, 0xe
  19bae8: 10 00 40 10  	beqz	$2, 0x19bb2c <.text+0x9bb2c>
  19baec: 00 16 09 00  	sll	$2, $9, 0x18
  19baf0: 80 10 03 00  	sll	$2, $3, 0x2
  19baf4: 1c 00 03 3c  	lui	$3, 0x1c
  19baf8: 08 a4 63 24  	addiu	$3, $3, -0x5bf8 <.text+0xffffffffffefa408>
  19bafc: 21 10 43 00  	addu	$2, $2, $3
  19bb00: 00 00 44 8c  	lw	$4, 0x0($2)
  19bb04: 08 00 80 00  	jr	$4
  19bb10: 00 00 02 8d  	lw	$2, 0x0($8)
  19bb14: 01 00 42 24  	addiu	$2, $2, 0x1
  19bb18: 08 00 e0 03  	jr	$ra
  19bb1c: 00 00 02 ad  	sw	$2, 0x0($8)
  19bb20: 08 00 e0 03  	jr	$ra
  19bb24: 00 00 00 00  	nop
  19bb28: 00 16 09 00  	sll	$2, $9, 0x18
  19bb2c: 00 00 e4 8c  	lw	$4, 0x0($7)
  19bb30: 03 16 02 00  	sra	$2, $2, 0x18
  19bb34: 21 10 4a 00  	addu	$2, $2, $10
  19bb38: 32 00 43 80  	lb	$3, 0x32($2)
  19bb3c: 21 20 83 00  	addu	$4, $4, $3
  19bb40: 2a 10 c4 00  	slt	$2, $6, $4
  19bb44: f6 ff 40 14  	bnez	$2, 0x19bb20 <.text+0x9bb20>
  19bb48: 00 00 00 00  	nop
  19bb4c: 00 00 e4 ac  	sw	$4, 0x0($7)
  19bb50: 00 00 02 8d  	lw	$2, 0x0($8)
  19bb54: 01 00 a5 24  	addiu	$5, $5, 0x1
  19bb58: 01 00 42 24  	addiu	$2, $2, 0x1
  19bb5c: df ff 00 10  	b	0x19badc <.text+0x9badc>
  19bb60: 00 00 02 ad  	sw	$2, 0x0($8)
  19bb64: 00 00 00 00  	nop
  19bb68: 30 ff bd 27  	addiu	$sp, $sp, -0xd0 <.text+0xffffffffffefff30>
  19bb6c: b0 00 be ff  	sd	$fp, 0xb0($sp)
  19bb70: 2d f0 40 01  	move	$fp, $10
  19bb74: a0 00 b7 ff  	sd	$23, 0xa0($sp)
  19bb78: 2d b8 00 01  	move	$23, $8
  19bb7c: 90 00 b6 ff  	sd	$22, 0x90($sp)
  19bb80: 2d b0 20 01  	move	$22, $9
  19bb84: 80 00 b5 ff  	sd	$21, 0x80($sp)
  19bb88: 2d a8 00 01  	move	$21, $8
  19bb8c: 70 00 b4 ff  	sd	$20, 0x70($sp)
  19bb90: 2d a0 c0 00  	move	$20, $6
  19bb94: 60 00 b3 ff  	sd	$19, 0x60($sp)
  19bb98: 2d 98 a0 00  	move	$19, $5
  19bb9c: 50 00 b2 ff  	sd	$18, 0x50($sp)
  19bba0: 2d 90 00 00  	move	$18, $zero
  19bba4: 30 00 b0 ff  	sd	$16, 0x30($sp)
  19bba8: 2d 80 80 00  	move	$16, $4
  19bbac: c0 00 bf ff  	sd	$ra, 0xc0($sp)
  19bbb0: 40 00 b1 ff  	sd	$17, 0x40($sp)
  19bbb4: 1a 00 20 19  	blez	$9, 0x19bc20 <.text+0x9bc20>
  19bbb8: 20 00 a7 af  	sw	$7, 0x20($sp)
  19bbbc: 21 10 d2 03  	addu	$2, $fp, $18
  19bbc0: 00 00 4f 90  	lbu	$15, 0x0($2)
  19bbc4: f9 ff e3 25  	addiu	$3, $15, -0x7 <.text+0xffffffffffeffff9>
  19bbc8: 07 00 62 2c  	sltiu	$2, $3, 0x7
  19bbcc: 26 00 40 10  	beqz	$2, 0x19bc68 <.text+0x9bc68>
  19bbd0: 80 10 03 00  	sll	$2, $3, 0x2
  19bbd4: 1c 00 03 3c  	lui	$3, 0x1c
  19bbd8: 40 a4 63 24  	addiu	$3, $3, -0x5bc0 <.text+0xffffffffffefa440>
  19bbdc: 21 10 43 00  	addu	$2, $2, $3
  19bbe0: 00 00 44 8c  	lw	$4, 0x0($2)
  19bbe4: 08 00 80 00  	jr	$4
  19bbf0: 30 00 02 92  	lbu	$2, 0x30($16)
  19bbf4: 01 00 42 2c  	sltiu	$2, $2, 0x1
  19bbf8: 15 00 40 10  	beqz	$2, 0x19bc50 <.text+0x9bc50>
  19bbfc: 30 00 02 a2  	sb	$2, 0x30($16)
  19bc00: 00 ff 02 34  	ori	$2, $zero, 0xff00
  19bc04: 38 14 02 00  	dsll	$2, $2, 0x10
  19bc08: 25 a8 e2 02  	or	$21, $23, $2
  19bc0c: 00 00 00 00  	nop
  19bc10: 01 00 52 26  	addiu	$18, $18, 0x1
  19bc14: 2a 10 56 02  	slt	$2, $18, $22
  19bc18: e9 ff 40 14  	bnez	$2, 0x19bbc0 <.text+0x9bbc0>
  19bc1c: 21 10 d2 03  	addu	$2, $fp, $18
  19bc20: c0 00 bf df  	ld	$ra, 0xc0($sp)
  19bc24: b0 00 be df  	ld	$fp, 0xb0($sp)
  19bc28: a0 00 b7 df  	ld	$23, 0xa0($sp)
  19bc2c: 90 00 b6 df  	ld	$22, 0x90($sp)
  19bc30: 80 00 b5 df  	ld	$21, 0x80($sp)
  19bc34: 70 00 b4 df  	ld	$20, 0x70($sp)
  19bc38: 60 00 b3 df  	ld	$19, 0x60($sp)
  19bc3c: 50 00 b2 df  	ld	$18, 0x50($sp)
  19bc40: 40 00 b1 df  	ld	$17, 0x40($sp)
  19bc44: 30 00 b0 df  	ld	$16, 0x30($sp)
  19bc48: 08 00 e0 03  	jr	$ra
  19bc4c: d0 00 bd 27  	addiu	$sp, $sp, 0xd0
  19bc50: ef ff 00 10  	b	0x19bc10 <.text+0x9bc10>
  19bc54: 2d a8 e0 02  	move	$21, $23
  19bc58: 31 00 02 92  	lbu	$2, 0x31($16)
  19bc5c: 01 00 42 2c  	sltiu	$2, $2, 0x1
  19bc60: eb ff 00 10  	b	0x19bc10 <.text+0x9bc10>
  19bc64: 31 00 02 a2  	sb	$2, 0x31($16)
  19bc68: 1c 00 07 8e  	lw	$7, 0x1c($16)
  19bc6c: 21 18 f0 01  	addu	$3, $15, $16
  19bc70: 3c 48 15 00  	dsll32	$9, $21, 0x0
  19bc74: 3f 48 09 00  	dsra32	$9, $9, 0x0
  19bc78: 2d 28 60 02  	move	$5, $19
  19bc7c: 1b 00 e7 01  	divu	$zero, $15, $7
  19bc80: 01 00 e0 50  	beqzl	$7, 0x19bc88 <.text+0x9bc88>
  19bc84: cd 01 00 00  	break	0x0, 0x7
  19bc88: 28 00 02 8e  	lw	$2, 0x28($16)
  19bc8c: 2d 30 80 02  	move	$6, $20
  19bc90: 32 00 6b 80  	lb	$11, 0x32($3)
  19bc94: 20 00 a3 8f  	lw	$3, 0x20($sp)
  19bc98: 21 50 82 02  	addu	$10, $20, $2
  19bc9c: 24 00 0e 8e  	lw	$14, 0x24($16)
  19bca0: 21 88 6b 02  	addu	$17, $19, $11
  19bca4: 10 00 0d 8e  	lw	$13, 0x10($16)
  19bca8: 0c 00 0c 8e  	lw	$12, 0xc($16)
  19bcac: 00 00 04 8e  	lw	$4, 0x0($16)
  19bcb0: 08 00 a3 af  	sw	$3, 0x8($sp)
  19bcb4: 10 00 a9 af  	sw	$9, 0x10($sp)
  19bcb8: ff ff 29 26  	addiu	$9, $17, -0x1 <.text+0xffffffffffefffff>
  19bcbc: 12 40 00 00  	mflo	$8
  19bcc0: 18 38 07 01  	mult	$a3, $t0, $a3  # R5900 3-operand
  19bcc4: 18 18 02 01  	mult	$v1, $t0, $v0  # R5900 3-operand
  19bcc8: 23 38 e7 01  	subu	$7, $15, $7
  19bccc: 21 40 6c 00  	addu	$8, $3, $12
  19bcd0: 18 18 ee 00  	mult	$v1, $a3, $t6  # R5900 3-operand
  19bcd4: 21 10 02 01  	addu	$2, $8, $2
  19bcd8: 00 00 a2 af  	sw	$2, 0x0($sp)
  19bcdc: 21 38 6d 00  	addu	$7, $3, $13
  19bce0: 0a 6c 06 0c  	jal	0x19b028 <.text+0x9b028>
  19bce4: 21 58 eb 00  	addu	$11, $7, $11
  19bce8: 31 00 02 92  	lbu	$2, 0x31($16)
  19bcec: 04 00 40 54  	bnezl	$2, 0x19bd00 <.text+0x9bd00>
  19bcf0: 28 00 06 8e  	lw	$6, 0x28($16)
  19bcf4: c6 ff 00 10  	b	0x19bc10 <.text+0x9bc10>
  19bcf8: 2d 98 20 02  	move	$19, $17
  19bcfc: 00 00 00 00  	nop
  19bd00: 00 7f 0a 3c  	lui	$10, 0x7f00
  19bd04: 25 50 ea 02  	or	$10, $23, $10
  19bd08: 00 00 04 8e  	lw	$4, 0x0($16)
  19bd0c: 21 30 86 02  	addu	$6, $20, $6
  19bd10: 20 00 a9 8f  	lw	$9, 0x20($sp)
  19bd14: 3c 50 0a 00  	dsll32	$10, $10, 0x0
  19bd18: 3f 50 0a 00  	dsra32	$10, $10, 0x0
  19bd1c: 01 00 c8 24  	addiu	$8, $6, 0x1
  19bd20: 2d 28 60 02  	move	$5, $19
  19bd24: 7e 6b 06 0c  	jal	0x19adf8 <.text+0x9adf8>
  19bd28: 2d 38 20 02  	move	$7, $17
  19bd2c: b8 ff 00 10  	b	0x19bc10 <.text+0x9bc10>
  19bd30: 2d 98 20 02  	move	$19, $17
  19bd34: 00 00 00 00  	nop
