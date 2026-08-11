# SIF RPC core and leaf wrappers: 0x0019c688..0x0019cfbf
  19c688: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  19c68c: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19c690: 2d 88 80 00  	move	$17, $4
  19c694: 42 00 04 3c  	lui	$4, 0x42
  19c698: 50 00 b3 ff  	sd	$19, 0x50($sp)
  19c69c: 40 5a 84 24  	addiu	$4, $4, 0x5a40
  19c6a0: 40 00 b2 ff  	sd	$18, 0x40($sp)
  19c6a4: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19c6a8: 2d 98 a0 00  	move	$19, $5
  19c6ac: 60 00 bf ff  	sd	$ra, 0x60($sp)
  19c6b0: 5c 73 06 0c  	jal	0x19cd70 <.text+0x9cd70>
  19c6b4: 2d 90 c0 00  	move	$18, $6
  19c6b8: ff ff 03 3c  	lui	$3, 0xffff
  19c6bc: 2d 80 40 00  	move	$16, $2
  19c6c0: 19 00 40 10  	beqz	$2, 0x19c728 <.text+0x9c728>
  19c6c4: f0 29 63 34  	ori	$3, $3, 0x29f0
  19c6c8: 10 00 20 ae  	sw	$zero, 0x10($17)
  19c6cc: 2d 28 40 00  	move	$5, $2
  19c6d0: 00 80 04 3c  	lui	$4, 0x8000
  19c6d4: 01 00 43 32  	andi	$3, $18, 0x1
  19c6d8: 18 00 42 8c  	lw	$2, 0x18($2)
  19c6dc: 09 00 84 34  	ori	$4, $4, 0x9
  19c6e0: 00 00 30 ae  	sw	$16, 0x0($17)
  19c6e4: 40 00 06 24  	addiu	$6, $zero, 0x40
  19c6e8: 04 00 22 ae  	sw	$2, 0x4($17)
  19c6ec: 2d 38 00 00  	move	$7, $zero
  19c6f0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19c6f4: 2d 40 00 00  	move	$8, $zero
  19c6f8: 08 00 22 ae  	sw	$2, 0x8($17)
  19c6fc: 2d 48 00 00  	move	$9, $zero
  19c700: 20 00 13 ae  	sw	$19, 0x20($16)
  19c704: 24 00 20 ae  	sw	$zero, 0x24($17)
  19c708: 14 00 10 ae  	sw	$16, 0x14($16)
  19c70c: 0e 00 60 10  	beqz	$3, 0x19c748 <.text+0x9c748>
  19c710: 1c 00 11 ae  	sw	$17, 0x1c($16)
  19c714: 99 7c 06 0c  	jal	0x19f264 <.text+0x9f264>
  19c718: 00 00 00 00  	nop
  19c71c: ff ff 03 3c  	lui	$3, 0xffff
  19c720: ef 29 63 34  	ori	$3, $3, 0x29ef
  19c724: 0b 18 02 00  	movn	$3, $zero, $2
  19c728: 60 00 bf df  	ld	$ra, 0x60($sp)
  19c72c: 2d 10 60 00  	move	$2, $3
  19c730: 50 00 b3 df  	ld	$19, 0x50($sp)
  19c734: 40 00 b2 df  	ld	$18, 0x40($sp)
  19c738: 30 00 b1 df  	ld	$17, 0x30($sp)
  19c73c: 20 00 b0 df  	ld	$16, 0x20($sp)
  19c740: 08 00 e0 03  	jr	$ra
  19c744: 70 00 bd 27  	addiu	$sp, $sp, 0x70
  19c748: 01 00 02 24  	addiu	$2, $zero, 0x1
  19c74c: 2d 20 a0 03  	move	$4, $sp
  19c750: 04 00 a2 af  	sw	$2, 0x4($sp)
  19c754: 9c 73 06 0c  	jal	0x19ce70 <.text+0x9ce70>
  19c758: 08 00 a0 af  	sw	$zero, 0x8($sp)
  19c75c: ff ff 03 3c  	lui	$3, 0xffff
  19c760: 08 00 22 ae  	sw	$2, 0x8($17)
  19c764: f0 ff 40 04  	bltz	$2, 0x19c728 <.text+0x9c728>
  19c768: fe 29 63 34  	ori	$3, $3, 0x29fe
  19c76c: 00 80 04 3c  	lui	$4, 0x8000
  19c770: 2d 28 00 02  	move	$5, $16
  19c774: 09 00 84 34  	ori	$4, $4, 0x9
  19c778: 40 00 06 24  	addiu	$6, $zero, 0x40
  19c77c: 2d 38 00 00  	move	$7, $zero
  19c780: 2d 40 00 00  	move	$8, $zero
  19c784: 99 7c 06 0c  	jal	0x19f264 <.text+0x9f264>
  19c788: 2d 48 00 00  	move	$9, $zero
  19c78c: ff ff 03 3c  	lui	$3, 0xffff
  19c790: e5 ff 40 10  	beqz	$2, 0x19c728 <.text+0x9c728>
  19c794: ef 29 63 34  	ori	$3, $3, 0x29ef
  19c798: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19c79c: 08 00 24 8e  	lw	$4, 0x8($17)
  19c7a0: a0 73 06 0c  	jal	0x19ce80 <.text+0x9ce80>
  19c7a4: 08 00 24 8e  	lw	$4, 0x8($17)
  19c7a8: df ff 00 10  	b	0x19c728 <.text+0x9c728>
  19c7ac: 2d 18 00 00  	move	$3, $zero
  19c7b0: 40 ff bd 27  	addiu	$sp, $sp, -0xc0 <.text+0xffffffffffefff40>
  19c7b4: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19c7b8: 2d 88 80 00  	move	$17, $4
  19c7bc: 42 00 04 3c  	lui	$4, 0x42
  19c7c0: a0 00 be ff  	sd	$fp, 0xa0($sp)
  19c7c4: 40 5a 84 24  	addiu	$4, $4, 0x5a40
  19c7c8: 90 00 b7 ff  	sd	$23, 0x90($sp)
  19c7cc: 80 00 b6 ff  	sd	$22, 0x80($sp)
  19c7d0: 2d f0 c0 00  	move	$fp, $6
  19c7d4: 70 00 b5 ff  	sd	$21, 0x70($sp)
  19c7d8: 2d b0 a0 00  	move	$22, $5
  19c7dc: 60 00 b4 ff  	sd	$20, 0x60($sp)
  19c7e0: 2d a8 20 01  	move	$21, $9
  19c7e4: 50 00 b3 ff  	sd	$19, 0x50($sp)
  19c7e8: 2d a0 e0 00  	move	$20, $7
  19c7ec: 40 00 b2 ff  	sd	$18, 0x40($sp)
  19c7f0: 2d 98 00 01  	move	$19, $8
  19c7f4: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19c7f8: 2d 90 40 01  	move	$18, $10
  19c7fc: b0 00 bf ff  	sd	$ra, 0xb0($sp)
  19c800: 5c 73 06 0c  	jal	0x19cd70 <.text+0x9cd70>
  19c804: 2d b8 60 01  	move	$23, $11
  19c808: ff ff 03 3c  	lui	$3, 0xffff
  19c80c: 2d 80 40 00  	move	$16, $2
  19c810: 2d 00 40 10  	beqz	$2, 0x19c8c8 <.text+0x9c8c8>
  19c814: f0 29 63 34  	ori	$3, $3, 0x29f0
  19c818: 18 00 42 8c  	lw	$2, 0x18($2)
  19c81c: 02 00 c4 33  	andi	$4, $fp, 0x2
  19c820: 24 00 23 8e  	lw	$3, 0x24($17)
  19c824: 04 00 22 ae  	sw	$2, 0x4($17)
  19c828: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19c82c: 08 00 22 ae  	sw	$2, 0x8($17)
  19c830: c0 00 a2 8f  	lw	$2, 0xc0($sp)
  19c834: 20 00 16 ae  	sw	$22, 0x20($16)
  19c838: 20 00 22 ae  	sw	$2, 0x20($17)
  19c83c: 01 00 02 24  	addiu	$2, $zero, 0x1
  19c840: 00 00 30 ae  	sw	$16, 0x0($17)
  19c844: 30 00 02 ae  	sw	$2, 0x30($16)
  19c848: 34 00 03 ae  	sw	$3, 0x34($16)
  19c84c: 1c 00 37 ae  	sw	$23, 0x1c($17)
  19c850: 24 00 13 ae  	sw	$19, 0x24($16)
  19c854: 28 00 15 ae  	sw	$21, 0x28($16)
  19c858: 2c 00 12 ae  	sw	$18, 0x2c($16)
  19c85c: 14 00 10 ae  	sw	$16, 0x14($16)
  19c860: 09 00 80 14  	bnez	$4, 0x19c888 <.text+0x9c888>
  19c864: 1c 00 11 ae  	sw	$17, 0x1c($16)
  19c868: 03 00 60 1a  	blez	$19, 0x19c878 <.text+0x9c878>
  19c86c: 2d 20 80 02  	move	$4, $20
  19c870: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19c874: 2d 28 60 02  	move	$5, $19
  19c878: 03 00 40 1a  	blez	$18, 0x19c888 <.text+0x9c888>
  19c87c: 2d 20 a0 02  	move	$4, $21
  19c880: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19c884: 2d 28 40 02  	move	$5, $18
  19c888: 01 00 c2 33  	andi	$2, $fp, 0x1
  19c88c: 1b 00 40 10  	beqz	$2, 0x19c8fc <.text+0x9c8fc>
  19c890: 01 00 02 24  	addiu	$2, $zero, 0x1
  19c894: 01 00 e0 52  	beqzl	$23, 0x19c89c <.text+0x9c89c>
  19c898: 30 00 00 ae  	sw	$zero, 0x30($16)
  19c89c: 14 00 28 8e  	lw	$8, 0x14($17)
  19c8a0: 00 80 04 3c  	lui	$4, 0x8000
  19c8a4: 2d 28 00 02  	move	$5, $16
  19c8a8: 2d 38 80 02  	move	$7, $20
  19c8ac: 2d 48 60 02  	move	$9, $19
  19c8b0: 0a 00 84 34  	ori	$4, $4, 0xa
  19c8b4: 99 7c 06 0c  	jal	0x19f264 <.text+0x9f264>
  19c8b8: 40 00 06 24  	addiu	$6, $zero, 0x40
  19c8bc: ff ff 03 3c  	lui	$3, 0xffff
  19c8c0: ef 29 63 34  	ori	$3, $3, 0x29ef
  19c8c4: 0b 18 02 00  	movn	$3, $zero, $2
  19c8c8: b0 00 bf df  	ld	$ra, 0xb0($sp)
  19c8cc: 2d 10 60 00  	move	$2, $3
  19c8d0: a0 00 be df  	ld	$fp, 0xa0($sp)
  19c8d4: 90 00 b7 df  	ld	$23, 0x90($sp)
  19c8d8: 80 00 b6 df  	ld	$22, 0x80($sp)
  19c8dc: 70 00 b5 df  	ld	$21, 0x70($sp)
  19c8e0: 60 00 b4 df  	ld	$20, 0x60($sp)
  19c8e4: 50 00 b3 df  	ld	$19, 0x50($sp)
  19c8e8: 40 00 b2 df  	ld	$18, 0x40($sp)
  19c8ec: 30 00 b1 df  	ld	$17, 0x30($sp)
  19c8f0: 20 00 b0 df  	ld	$16, 0x20($sp)
  19c8f4: 08 00 e0 03  	jr	$ra
  19c8f8: c0 00 bd 27  	addiu	$sp, $sp, 0xc0
  19c8fc: 2d 20 a0 03  	move	$4, $sp
  19c900: 04 00 a2 af  	sw	$2, 0x4($sp)
  19c904: 9c 73 06 0c  	jal	0x19ce70 <.text+0x9ce70>
  19c908: 08 00 a0 af  	sw	$zero, 0x8($sp)
  19c90c: ff ff 03 3c  	lui	$3, 0xffff
  19c910: 08 00 22 ae  	sw	$2, 0x8($17)
  19c914: ec ff 40 04  	bltz	$2, 0x19c8c8 <.text+0x9c8c8>
  19c918: fe 29 63 34  	ori	$3, $3, 0x29fe
  19c91c: 00 80 04 3c  	lui	$4, 0x8000
  19c920: 14 00 28 8e  	lw	$8, 0x14($17)
  19c924: 2d 28 00 02  	move	$5, $16
  19c928: 2d 38 80 02  	move	$7, $20
  19c92c: 2d 48 60 02  	move	$9, $19
  19c930: 0a 00 84 34  	ori	$4, $4, 0xa
  19c934: 99 7c 06 0c  	jal	0x19f264 <.text+0x9f264>
  19c938: 40 00 06 24  	addiu	$6, $zero, 0x40
  19c93c: ff ff 03 3c  	lui	$3, 0xffff
  19c940: e1 ff 40 10  	beqz	$2, 0x19c8c8 <.text+0x9c8c8>
  19c944: ef 29 63 34  	ori	$3, $3, 0x29ef
  19c948: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19c94c: 08 00 24 8e  	lw	$4, 0x8($17)
  19c950: a0 73 06 0c  	jal	0x19ce80 <.text+0x9ce80>
  19c954: 08 00 24 8e  	lw	$4, 0x8($17)
  19c958: db ff 00 10  	b	0x19c8c8 <.text+0x9c8c8>
  19c95c: 2d 18 00 00  	move	$3, $zero
  19c960: 10 00 82 8c  	lw	$2, 0x10($4)
  19c964: fe ff 03 24  	addiu	$3, $zero, -0x2 <.text+0xffffffffffeffffe>
  19c968: 18 00 80 ac  	sw	$zero, 0x18($4)
  19c96c: 24 10 43 00  	and	$2, $2, $3
  19c970: 08 00 e0 03  	jr	$ra
  19c974: 10 00 82 ac  	sw	$2, 0x10($4)
  19c978: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19c97c: 00 80 02 3c  	lui	$2, 0x8000
  19c980: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19c984: 00 80 05 3c  	lui	$5, 0x8000
  19c988: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19c98c: 0a 00 42 34  	ori	$2, $2, 0xa
  19c990: 09 00 a5 34  	ori	$5, $5, 0x9
  19c994: 20 00 83 8c  	lw	$3, 0x20($4)
  19c998: 15 00 62 10  	beq	$3, $2, 0x19c9f0 <.text+0x9c9f0>
  19c99c: 1c 00 90 8c  	lw	$16, 0x1c($4)
  19c9a0: 0d 00 65 50  	beql	$3, $5, 0x19c9d8 <.text+0x9c9d8>
  19c9a4: 28 00 82 8c  	lw	$2, 0x28($4)
  19c9a8: 08 00 02 8e  	lw	$2, 0x8($16)
  19c9ac: 03 00 40 04  	bltz	$2, 0x19c9bc <.text+0x9c9bc>
  19c9b0: 2d 20 40 00  	move	$4, $2
  19c9b4: a4 73 06 0c  	jal	0x19ce90 <.text+0x9ce90>
  19c9b8: 00 00 00 00  	nop
  19c9bc: 58 72 06 0c  	jal	0x19c960 <.text+0x9c960>
  19c9c0: 00 00 04 8e  	lw	$4, 0x0($16)
  19c9c4: 10 00 bf df  	ld	$ra, 0x10($sp)
  19c9c8: 00 00 00 ae  	sw	$zero, 0x0($16)
  19c9cc: 00 00 b0 df  	ld	$16, 0x0($sp)
  19c9d0: 08 00 e0 03  	jr	$ra
  19c9d4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19c9d8: 24 00 83 8c  	lw	$3, 0x24($4)
  19c9dc: 14 00 02 ae  	sw	$2, 0x14($16)
  19c9e0: 24 00 03 ae  	sw	$3, 0x24($16)
  19c9e4: 2c 00 82 8c  	lw	$2, 0x2c($4)
  19c9e8: ef ff 00 10  	b	0x19c9a8 <.text+0x9c9a8>
  19c9ec: 18 00 02 ae  	sw	$2, 0x18($16)
  19c9f0: 1c 00 02 8e  	lw	$2, 0x1c($16)
  19c9f4: ed ff 40 50  	beqzl	$2, 0x19c9ac <.text+0x9c9ac>
  19c9f8: 08 00 02 8e  	lw	$2, 0x8($16)
  19c9fc: 09 f8 40 00  	jalr	$2
  19ca00: 20 00 04 8e  	lw	$4, 0x20($16)
  19ca04: e9 ff 00 10  	b	0x19c9ac <.text+0x9c9ac>
  19ca08: 08 00 02 8e  	lw	$2, 0x8($16)
  19ca0c: 28 00 a5 8c  	lw	$5, 0x28($5)
  19ca10: 0e 00 a0 10  	beqz	$5, 0x19ca4c <.text+0x9ca4c>
  19ca14: 2d 30 00 00  	move	$6, $zero
  19ca18: 08 00 a3 8c  	lw	$3, 0x8($5)
  19ca1c: 08 00 60 50  	beqzl	$3, 0x19ca40 <.text+0x9ca40>
  19ca20: 14 00 a5 8c  	lw	$5, 0x14($5)
  19ca24: 00 00 62 8c  	lw	$2, 0x0($3)
  19ca28: 08 00 44 10  	beq	$2, $4, 0x19ca4c <.text+0x9ca4c>
  19ca2c: 2d 30 60 00  	move	$6, $3
  19ca30: 38 00 63 8c  	lw	$3, 0x38($3)
  19ca34: fc ff 60 54  	bnezl	$3, 0x19ca28 <.text+0x9ca28>
  19ca38: 00 00 62 8c  	lw	$2, 0x0($3)
  19ca3c: 14 00 a5 8c  	lw	$5, 0x14($5)
  19ca40: f6 ff a0 54  	bnezl	$5, 0x19ca1c <.text+0x9ca1c>
  19ca44: 08 00 a3 8c  	lw	$3, 0x8($5)
  19ca48: 2d 30 00 00  	move	$6, $zero
  19ca4c: 08 00 e0 03  	jr	$ra
  19ca50: 2d 10 c0 00  	move	$2, $6
  19ca54: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19ca58: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19ca5c: 2d 80 80 00  	move	$16, $4
  19ca60: 2d 20 a0 00  	move	$4, $5
  19ca64: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19ca68: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19ca6c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19ca70: 8b 73 06 0c  	jal	0x19ce2c <.text+0x9ce2c>
  19ca74: 2d 88 a0 00  	move	$17, $5
  19ca78: 1c 00 03 8e  	lw	$3, 0x1c($16)
  19ca7c: 2d 90 40 00  	move	$18, $2
  19ca80: 14 00 04 8e  	lw	$4, 0x14($16)
  19ca84: 00 80 02 3c  	lui	$2, 0x8000
  19ca88: 1c 00 43 ae  	sw	$3, 0x1c($18)
  19ca8c: 09 00 42 34  	ori	$2, $2, 0x9
  19ca90: 14 00 44 ae  	sw	$4, 0x14($18)
  19ca94: 20 00 42 ae  	sw	$2, 0x20($18)
  19ca98: 2d 28 20 02  	move	$5, $17
  19ca9c: 83 72 06 0c  	jal	0x19ca0c <.text+0x9ca0c>
  19caa0: 20 00 04 8e  	lw	$4, 0x20($16)
  19caa4: 14 00 40 10  	beqz	$2, 0x19caf8 <.text+0x9caf8>
  19caa8: 2d 18 40 00  	move	$3, $2
  19caac: 08 00 42 8c  	lw	$2, 0x8($2)
  19cab0: 24 00 43 ae  	sw	$3, 0x24($18)
  19cab4: 28 00 42 ae  	sw	$2, 0x28($18)
  19cab8: 14 00 62 8c  	lw	$2, 0x14($3)
  19cabc: 2c 00 42 ae  	sw	$2, 0x2c($18)
  19cac0: 00 80 04 3c  	lui	$4, 0x8000
  19cac4: 2d 28 40 02  	move	$5, $18
  19cac8: 08 00 84 34  	ori	$4, $4, 0x8
  19cacc: 40 00 06 24  	addiu	$6, $zero, 0x40
  19cad0: 2d 38 00 00  	move	$7, $zero
  19cad4: 2d 40 00 00  	move	$8, $zero
  19cad8: a8 7c 06 0c  	jal	0x19f2a0 <.text+0x9f2a0>
  19cadc: 2d 48 00 00  	move	$9, $zero
  19cae0: 00 00 b0 df  	ld	$16, 0x0($sp)
  19cae4: 30 00 bf df  	ld	$ra, 0x30($sp)
  19cae8: 20 00 b2 df  	ld	$18, 0x20($sp)
  19caec: 10 00 b1 df  	ld	$17, 0x10($sp)
  19caf0: 08 00 e0 03  	jr	$ra
  19caf4: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19caf8: 24 00 40 ae  	sw	$zero, 0x24($18)
  19cafc: 28 00 40 ae  	sw	$zero, 0x28($18)
  19cb00: ef ff 00 10  	b	0x19cac0 <.text+0x9cac0>
  19cb04: 2c 00 40 ae  	sw	$zero, 0x2c($18)
  19cb08: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19cb0c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19cb10: 34 00 85 8c  	lw	$5, 0x34($4)
  19cb14: 40 00 a7 8c  	lw	$7, 0x40($5)
  19cb18: 0c 00 e2 8c  	lw	$2, 0xc($7)
  19cb1c: 20 00 40 10  	beqz	$2, 0x19cba0 <.text+0x9cba0>
  19cb20: 2d 30 80 00  	move	$6, $4
  19cb24: 10 00 e2 8c  	lw	$2, 0x10($7)
  19cb28: 38 00 45 ac  	sw	$5, 0x38($2)
  19cb2c: 20 00 c2 8c  	lw	$2, 0x20($6)
  19cb30: 14 00 c3 8c  	lw	$3, 0x14($6)
  19cb34: 24 00 a2 ac  	sw	$2, 0x24($5)
  19cb38: 1c 00 c4 8c  	lw	$4, 0x1c($6)
  19cb3c: 24 00 c2 8c  	lw	$2, 0x24($6)
  19cb40: 20 00 a3 ac  	sw	$3, 0x20($5)
  19cb44: 0c 00 a2 ac  	sw	$2, 0xc($5)
  19cb48: 28 00 c3 8c  	lw	$3, 0x28($6)
  19cb4c: 2c 00 c2 8c  	lw	$2, 0x2c($6)
  19cb50: 1c 00 a4 ac  	sw	$4, 0x1c($5)
  19cb54: 2c 00 a2 ac  	sw	$2, 0x2c($5)
  19cb58: 10 00 e5 ac  	sw	$5, 0x10($7)
  19cb5c: 30 00 c2 8c  	lw	$2, 0x30($6)
  19cb60: 28 00 a3 ac  	sw	$3, 0x28($5)
  19cb64: 30 00 a2 ac  	sw	$2, 0x30($5)
  19cb68: 10 00 c2 8c  	lw	$2, 0x10($6)
  19cb6c: 34 00 a2 ac  	sw	$2, 0x34($5)
  19cb70: 00 00 e4 8c  	lw	$4, 0x0($7)
  19cb74: 04 00 80 04  	bltz	$4, 0x19cb88 <.text+0x9cb88>
  19cb78: 00 00 bf df  	ld	$ra, 0x0($sp)
  19cb7c: 04 00 e2 8c  	lw	$2, 0x4($7)
  19cb80: 03 00 40 14  	bnez	$2, 0x19cb90 <.text+0x9cb90>
  19cb84: 00 00 00 00  	nop
  19cb88: 08 00 e0 03  	jr	$ra
  19cb8c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19cb90: 98 73 06 0c  	jal	0x19ce60 <.text+0x9ce60>
  19cb94: 00 00 00 00  	nop
  19cb98: fb ff 00 10  	b	0x19cb88 <.text+0x9cb88>
  19cb9c: 00 00 bf df  	ld	$ra, 0x0($sp)
  19cba0: e2 ff 00 10  	b	0x19cb2c <.text+0x9cb2c>
  19cba4: 0c 00 e5 ac  	sw	$5, 0xc($7)
  19cba8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19cbac: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19cbb0: 2d 80 80 00  	move	$16, $4
  19cbb4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19cbb8: 8b 73 06 0c  	jal	0x19ce2c <.text+0x9ce2c>
  19cbbc: 2d 20 a0 00  	move	$4, $5
  19cbc0: 1c 00 07 8e  	lw	$7, 0x1c($16)
  19cbc4: 14 00 04 8e  	lw	$4, 0x14($16)
  19cbc8: 00 80 03 3c  	lui	$3, 0x8000
  19cbcc: 0c 00 63 34  	ori	$3, $3, 0xc
  19cbd0: 1c 00 47 ac  	sw	$7, 0x1c($2)
  19cbd4: 14 00 44 ac  	sw	$4, 0x14($2)
  19cbd8: 40 00 06 24  	addiu	$6, $zero, 0x40
  19cbdc: 20 00 43 ac  	sw	$3, 0x20($2)
  19cbe0: 00 80 04 3c  	lui	$4, 0x8000
  19cbe4: 20 00 07 8e  	lw	$7, 0x20($16)
  19cbe8: 2d 28 40 00  	move	$5, $2
  19cbec: 28 00 09 8e  	lw	$9, 0x28($16)
  19cbf0: 08 00 84 34  	ori	$4, $4, 0x8
  19cbf4: a8 7c 06 0c  	jal	0x19f2a0 <.text+0x9f2a0>
  19cbf8: 24 00 08 8e  	lw	$8, 0x24($16)
  19cbfc: 00 00 b0 df  	ld	$16, 0x0($sp)
  19cc00: 10 00 bf df  	ld	$ra, 0x10($sp)
  19cc04: 08 00 e0 03  	jr	$ra
  19cc08: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19cc0c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19cc10: 42 00 03 3c  	lui	$3, 0x42
  19cc14: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19cc18: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19cc1c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19cc20: 6c 5a 62 8c  	lw	$2, 0x5a6c($3)
  19cc24: 06 00 40 10  	beqz	$2, 0x19cc40 <.text+0x9cc40>
  19cc28: 42 00 10 3c  	lui	$16, 0x42
  19cc2c: 20 00 bf df  	ld	$ra, 0x20($sp)
  19cc30: 10 00 b1 df  	ld	$17, 0x10($sp)
  19cc34: 00 00 b0 df  	ld	$16, 0x0($sp)
  19cc38: 08 00 e0 03  	jr	$ra
  19cc3c: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19cc40: 01 00 11 24  	addiu	$17, $zero, 0x1
  19cc44: 40 5a 10 26  	addiu	$16, $16, 0x5a40
  19cc48: c1 7c 06 0c  	jal	0x19f304 <.text+0x9f304>
  19cc4c: 6c 5a 71 ac  	sw	$17, 0x5a6c($3)
  19cc50: 06 7c 06 0c  	jal	0x19f018 <.text+0x9f018>
  19cc54: 00 00 00 00  	nop
  19cc58: 2d 30 00 02  	move	$6, $16
  19cc5c: 04 00 08 8e  	lw	$8, 0x4($16)
  19cc60: 00 20 02 3c  	lui	$2, 0x2000
  19cc64: 14 00 07 8e  	lw	$7, 0x14($16)
  19cc68: 00 80 04 3c  	lui	$4, 0x8000
  19cc6c: 1c 00 03 8e  	lw	$3, 0x1c($16)
  19cc70: 25 40 02 01  	or	$8, $8, $2
  19cc74: 25 38 e2 00  	or	$7, $7, $2
  19cc78: 04 00 08 ae  	sw	$8, 0x4($16)
  19cc7c: 25 18 62 00  	or	$3, $3, $2
  19cc80: 14 00 07 ae  	sw	$7, 0x14($16)
  19cc84: 1c 00 03 ae  	sw	$3, 0x1c($16)
  19cc88: 1a 00 05 3c  	lui	$5, 0x1a
  19cc8c: 78 c9 a5 24  	addiu	$5, $5, -0x3688 <.text+0xffffffffffefc978>
  19cc90: 51 7d 06 0c  	jal	0x19f544 <.text+0x9f544>
  19cc94: 08 00 84 34  	ori	$4, $4, 0x8
  19cc98: 2d 30 00 02  	move	$6, $16
  19cc9c: 00 80 04 3c  	lui	$4, 0x8000
  19cca0: 1a 00 05 3c  	lui	$5, 0x1a
  19cca4: 09 00 84 34  	ori	$4, $4, 0x9
  19cca8: 51 7d 06 0c  	jal	0x19f544 <.text+0x9f544>
  19ccac: 54 ca a5 24  	addiu	$5, $5, -0x35ac <.text+0xffffffffffefca54>
  19ccb0: 1a 00 05 3c  	lui	$5, 0x1a
  19ccb4: 00 80 04 3c  	lui	$4, 0x8000
  19ccb8: 2d 30 00 02  	move	$6, $16
  19ccbc: 0a 00 84 34  	ori	$4, $4, 0xa
  19ccc0: 51 7d 06 0c  	jal	0x19f544 <.text+0x9f544>
  19ccc4: 08 cb a5 24  	addiu	$5, $5, -0x34f8 <.text+0xffffffffffefcb08>
  19ccc8: 1a 00 05 3c  	lui	$5, 0x1a
  19cccc: 00 80 04 3c  	lui	$4, 0x8000
  19ccd0: 2d 30 00 02  	move	$6, $16
  19ccd4: 0c 00 84 34  	ori	$4, $4, 0xc
  19ccd8: 51 7d 06 0c  	jal	0x19f544 <.text+0x9f544>
  19ccdc: a8 cb a5 24  	addiu	$5, $5, -0x3458 <.text+0xffffffffffefcba8>
  19cce0: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19cce4: 00 00 00 00  	nop
  19cce8: 00 80 04 3c  	lui	$4, 0x8000
  19ccec: c0 73 06 0c  	jal	0x19cf00 <.text+0x9cf00>
  19ccf0: 02 00 84 34  	ori	$4, $4, 0x2
  19ccf4: ce ff 40 14  	bnez	$2, 0x19cc30 <.text+0x9cc30>
  19ccf8: 20 00 bf df  	ld	$ra, 0x20($sp)
  19ccfc: 44 00 02 3c  	lui	$2, 0x44
  19cd00: 00 80 04 3c  	lui	$4, 0x8000
  19cd04: 80 39 42 24  	addiu	$2, $2, 0x3980
  19cd08: 02 00 84 34  	ori	$4, $4, 0x2
  19cd0c: 0c 00 51 ac  	sw	$17, 0xc($2)
  19cd10: 2d 28 40 00  	move	$5, $2
  19cd14: 10 00 06 24  	addiu	$6, $zero, 0x10
  19cd18: 2d 38 00 00  	move	$7, $zero
  19cd1c: 2d 40 00 00  	move	$8, $zero
  19cd20: 99 7c 06 0c  	jal	0x19f264 <.text+0x9f264>
  19cd24: 2d 48 00 00  	move	$9, $zero
  19cd28: 5f 7d 06 0c  	jal	0x19f57c <.text+0x9f57c>
  19cd2c: 2d 20 00 00  	move	$4, $zero
  19cd30: fd ff 40 10  	beqz	$2, 0x19cd28 <.text+0x9cd28>
  19cd34: 00 80 04 3c  	lui	$4, 0x8000
  19cd38: 01 00 05 24  	addiu	$5, $zero, 0x1
  19cd3c: bc 73 06 0c  	jal	0x19cef0 <.text+0x9cef0>
  19cd40: 02 00 84 34  	ori	$4, $4, 0x2
  19cd44: ba ff 00 10  	b	0x19cc30 <.text+0x9cc30>
  19cd48: 20 00 bf df  	ld	$ra, 0x20($sp)
  19cd4c: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19cd50: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19cd54: 44 7d 06 0c  	jal	0x19f510 <.text+0x9f510>
  19cd58: 00 00 00 00  	nop
  19cd5c: 00 00 bf df  	ld	$ra, 0x0($sp)
  19cd60: 42 00 02 3c  	lui	$2, 0x42
  19cd64: 6c 5a 40 ac  	sw	$zero, 0x5a6c($2)
  19cd68: 08 00 e0 03  	jr	$ra
  19cd6c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19cd70: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19cd74: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19cd78: 2d 88 80 00  	move	$17, $4
  19cd7c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19cd80: 2d 90 00 00  	move	$18, $zero
  19cd84: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19cd88: 06 7c 06 0c  	jal	0x19f018 <.text+0x9f018>
  19cd8c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19cd90: 08 00 23 8e  	lw	$3, 0x8($17)
  19cd94: 21 00 60 18  	blez	$3, 0x19ce1c <.text+0x9ce1c>
  19cd98: 00 00 00 00  	nop
  19cd9c: 04 00 30 8e  	lw	$16, 0x4($17)
  19cda0: 10 00 02 8e  	lw	$2, 0x10($16)
  19cda4: 01 00 42 30  	andi	$2, $2, 0x1
  19cda8: 05 00 40 10  	beqz	$2, 0x19cdc0 <.text+0x9cdc0>
  19cdac: 00 00 00 00  	nop
  19cdb0: 01 00 52 26  	addiu	$18, $18, 0x1
  19cdb4: 2a 10 43 02  	slt	$2, $18, $3
  19cdb8: f9 ff 40 14  	bnez	$2, 0x19cda0 <.text+0x9cda0>
  19cdbc: 40 00 10 26  	addiu	$16, $16, 0x40
  19cdc0: 16 00 43 12  	beq	$18, $3, 0x19ce1c <.text+0x9ce1c>
  19cdc4: 00 00 00 00  	nop
  19cdc8: 00 00 23 8e  	lw	$3, 0x0($17)
  19cdcc: 10 00 60 10  	beqz	$3, 0x19ce10 <.text+0x9ce10>
  19cdd0: 02 00 02 24  	addiu	$2, $zero, 0x2
  19cdd4: 01 00 63 24  	addiu	$3, $3, 0x1
  19cdd8: 00 00 23 ae  	sw	$3, 0x0($17)
  19cddc: 00 14 12 00  	sll	$2, $18, 0x10
  19cde0: 18 00 03 ae  	sw	$3, 0x18($16)
  19cde4: 05 00 42 34  	ori	$2, $2, 0x5
  19cde8: 14 00 10 ae  	sw	$16, 0x14($16)
  19cdec: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19cdf0: 10 00 02 ae  	sw	$2, 0x10($16)
  19cdf4: 2d 10 00 02  	move	$2, $16
  19cdf8: 30 00 bf df  	ld	$ra, 0x30($sp)
  19cdfc: 20 00 b2 df  	ld	$18, 0x20($sp)
  19ce00: 10 00 b1 df  	ld	$17, 0x10($sp)
  19ce04: 00 00 b0 df  	ld	$16, 0x0($sp)
  19ce08: 08 00 e0 03  	jr	$ra
  19ce0c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19ce10: 01 00 03 24  	addiu	$3, $zero, 0x1
  19ce14: f1 ff 00 10  	b	0x19cddc <.text+0x9cddc>
  19ce18: 00 00 22 ae  	sw	$2, 0x0($17)
  19ce1c: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19ce20: 00 00 00 00  	nop
  19ce24: f4 ff 00 10  	b	0x19cdf8 <.text+0x9cdf8>
  19ce28: 2d 10 00 00  	move	$2, $zero
  19ce2c: 24 00 83 8c  	lw	$3, 0x24($4)
  19ce30: 18 00 82 8c  	lw	$2, 0x18($4)
  19ce34: 1a 00 62 00  	div	$zero, $3, $2
  19ce38: 01 00 40 50  	beqzl	$2, 0x19ce40 <.text+0x9ce40>
  19ce3c: cd 01 00 00  	break	0x0, 0x7
  19ce40: 14 00 82 8c  	lw	$2, 0x14($4)
  19ce44: 10 18 00 00  	mfhi	$3
  19ce48: 80 29 03 00  	sll	$5, $3, 0x6
  19ce4c: 01 00 63 24  	addiu	$3, $3, 0x1
  19ce50: 21 10 45 00  	addu	$2, $2, $5
  19ce54: 08 00 e0 03  	jr	$ra
  19ce58: 24 00 83 ac  	sw	$3, 0x24($4)
  19ce5c: 00 00 00 00  	nop
  19ce60: cc ff 03 24  	addiu	$3, $zero, -0x34 <.text+0xffffffffffefffcc>
  19ce64: 0c 00 00 00  	syscall
  19ce68: 08 00 e0 03  	jr	$ra
  19ce6c: 00 00 00 00  	nop
  19ce70: 40 00 03 24  	addiu	$3, $zero, 0x40
  19ce74: 0c 00 00 00  	syscall
  19ce78: 08 00 e0 03  	jr	$ra
  19ce7c: 00 00 00 00  	nop
  19ce80: 41 00 03 24  	addiu	$3, $zero, 0x41
  19ce84: 0c 00 00 00  	syscall
  19ce88: 08 00 e0 03  	jr	$ra
  19ce8c: 00 00 00 00  	nop
  19ce90: bd ff 03 24  	addiu	$3, $zero, -0x43 <.text+0xffffffffffefffbd>
  19ce94: 0c 00 00 00  	syscall
  19ce98: 08 00 e0 03  	jr	$ra
  19ce9c: 00 00 00 00  	nop
  19cea0: 44 00 03 24  	addiu	$3, $zero, 0x44
  19cea4: 0c 00 00 00  	syscall
  19cea8: 08 00 e0 03  	jr	$ra
  19ceac: 00 00 00 00  	nop
  19ceb0: 64 00 03 24  	addiu	$3, $zero, 0x64
  19ceb4: 0c 00 00 00  	syscall
  19ceb8: 08 00 e0 03  	jr	$ra
  19cebc: 00 00 00 00  	nop
  19cec0: 98 ff 03 24  	addiu	$3, $zero, -0x68 <.text+0xffffffffffefff98>
  19cec4: 0c 00 00 00  	syscall
  19cec8: 08 00 e0 03  	jr	$ra
  19cecc: 00 00 00 00  	nop
  19ced0: 76 00 03 24  	addiu	$3, $zero, 0x76
  19ced4: 0c 00 00 00  	syscall
  19ced8: 08 00 e0 03  	jr	$ra
  19cedc: 00 00 00 00  	nop
  19cee0: 77 00 03 24  	addiu	$3, $zero, 0x77
  19cee4: 0c 00 00 00  	syscall
  19cee8: 08 00 e0 03  	jr	$ra
  19ceec: 00 00 00 00  	nop
  19cef0: 79 00 03 24  	addiu	$3, $zero, 0x79
  19cef4: 0c 00 00 00  	syscall
  19cef8: 08 00 e0 03  	jr	$ra
  19cefc: 00 00 00 00  	nop
  19cf00: 7a 00 03 24  	addiu	$3, $zero, 0x7a
  19cf04: 0c 00 00 00  	syscall
  19cf08: 08 00 e0 03  	jr	$ra
  19cf0c: 00 00 00 00  	nop
  19cf10: ff ff 19 3c  	lui	$25, 0xffff
  19cf14: c0 ff 39 37  	ori	$25, $25, 0xffc0
  19cf18: 26 00 a0 18  	blez	$5, 0x19cfb4 <.text+0x9cfb4>
  19cf1c: 21 50 85 00  	addu	$10, $4, $5
  19cf20: 24 40 99 00  	and	$8, $4, $25
  19cf24: ff ff 4a 25  	addiu	$10, $10, -0x1 <.text+0xffffffffffefffff>
  19cf28: 24 48 59 01  	and	$9, $10, $25
  19cf2c: 23 50 28 01  	subu	$10, $9, $8
  19cf30: 82 59 0a 00  	srl	$11, $10, 0x6
  19cf34: 01 00 6b 25  	addiu	$11, $11, 0x1
  19cf38: 07 00 69 31  	andi	$9, $11, 0x7
  19cf3c: 08 00 20 11  	beqz	$9, 0x19cf60 <.text+0x9cf60>
  19cf40: c2 50 0b 00  	srl	$10, $11, 0x3
  19cf44: 0f 00 00 00  	sync
  19cf48: 00 00 18 bd  	cache	0x18, 0x0($8)
  19cf4c: 0f 00 00 00  	sync
  19cf50: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  19cf54: 00 00 00 00  	nop
  19cf58: fa ff 20 1d  	bgtz	$9, 0x19cf44 <.text+0x9cf44>
  19cf5c: 40 00 08 25  	addiu	$8, $8, 0x40
  19cf60: 14 00 40 11  	beqz	$10, 0x19cfb4 <.text+0x9cfb4>
  19cf64: ff ff 4a 25  	addiu	$10, $10, -0x1 <.text+0xffffffffffefffff>
  19cf68: 0f 00 00 00  	sync
  19cf6c: 00 00 18 bd  	cache	0x18, 0x0($8)
  19cf70: 0f 00 00 00  	sync
  19cf74: 40 00 18 bd  	cache	0x18, 0x40($8)
  19cf78: 0f 00 00 00  	sync
  19cf7c: 80 00 18 bd  	cache	0x18, 0x80($8)
  19cf80: 0f 00 00 00  	sync
  19cf84: c0 00 18 bd  	cache	0x18, 0xc0($8)
  19cf88: 0f 00 00 00  	sync
  19cf8c: 00 01 18 bd  	cache	0x18, 0x100($8)
  19cf90: 0f 00 00 00  	sync
  19cf94: 40 01 18 bd  	cache	0x18, 0x140($8)
  19cf98: 0f 00 00 00  	sync
  19cf9c: 80 01 18 bd  	cache	0x18, 0x180($8)
  19cfa0: 0f 00 00 00  	sync
  19cfa4: c0 01 18 bd  	cache	0x18, 0x1c0($8)
  19cfa8: 0f 00 00 00  	sync
  19cfac: ed ff 40 1d  	bgtz	$10, 0x19cf64 <.text+0x9cf64>
  19cfb0: 00 02 08 25  	addiu	$8, $8, 0x200
  19cfb4: 08 00 e0 03  	jr	$ra
		...

