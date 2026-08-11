# CDVD EE RPC corridor: 0x0019be70..0x0019c363
  19be70: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19be74: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19be78: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19be7c: 44 00 02 3c  	lui	$2, 0x44
  19be80: 00 0b 05 3c  	lui	$5, 0xb00
  19be84: 00 39 50 24  	addiu	$16, $2, 0x3900
  19be88: 37 13 a5 34  	ori	$5, $5, 0x1337
  19be8c: 2d 20 00 02  	move	$4, $16
  19be90: a2 71 06 0c  	jal	0x19c688 <.text+0x9c688>
  19be94: 2d 30 00 00  	move	$6, $zero
  19be98: 14 00 40 04  	bltz	$2, 0x19beec <.text+0x9beec>
  19be9c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19bea0: 24 00 02 8e  	lw	$2, 0x24($16)
  19bea4: 0d 00 40 14  	bnez	$2, 0x19bedc <.text+0x9bedc>
  19bea8: 01 00 03 3c  	lui	$3, 0x1
  19beac: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  19beb0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
		...
  19becc: f9 ff 62 14  	bne	$3, $2, 0x19beb4 <.text+0x9beb4>
  19bed0: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  19bed4: ea ff 00 10  	b	0x19be80 <.text+0x9be80>
  19bed8: 44 00 02 3c  	lui	$2, 0x44
  19bedc: 01 00 03 24  	addiu	$3, $zero, 0x1
  19bee0: 42 00 02 3c  	lui	$2, 0x42
  19bee4: 38 5a 43 ac  	sw	$3, 0x5a38($2)
  19bee8: 2d 18 00 00  	move	$3, $zero
  19beec: 10 00 bf df  	ld	$ra, 0x10($sp)
  19bef0: 2d 10 60 00  	move	$2, $3
  19bef4: 00 00 b0 df  	ld	$16, 0x0($sp)
  19bef8: 08 00 e0 03  	jr	$ra
  19befc: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19bf00: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19bf04: 42 00 02 3c  	lui	$2, 0x42
  19bf08: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19bf0c: 2d 60 80 00  	move	$12, $4
  19bf10: 10 00 b0 ff  	sd	$16, 0x10($sp)
  19bf14: 38 5a 42 8c  	lw	$2, 0x5a38($2)
  19bf18: 06 00 40 14  	bnez	$2, 0x19bf34 <.text+0x9bf34>
  19bf1c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19bf20: 20 00 bf df  	ld	$ra, 0x20($sp)
  19bf24: 2d 10 60 00  	move	$2, $3
  19bf28: 10 00 b0 df  	ld	$16, 0x10($sp)
  19bf2c: 08 00 e0 03  	jr	$ra
  19bf30: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19bf34: 44 00 10 3c  	lui	$16, 0x44
  19bf38: 44 00 04 3c  	lui	$4, 0x44
  19bf3c: 00 ed 07 26  	addiu	$7, $16, -0x1300 <.text+0xffffffffffefed00>
  19bf40: 00 39 84 24  	addiu	$4, $4, 0x3900
  19bf44: 06 00 05 24  	addiu	$5, $zero, 0x6
  19bf48: 2d 30 00 00  	move	$6, $zero
  19bf4c: 04 00 08 24  	addiu	$8, $zero, 0x4
  19bf50: 2d 48 e0 00  	move	$9, $7
  19bf54: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19bf58: 2d 58 00 00  	move	$11, $zero
  19bf5c: 00 ed 0c ae  	sw	$12, -0x1300($16)
  19bf60: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19bf64: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19bf68: ed ff 00 10  	b	0x19bf20 <.text+0x9bf20>
  19bf6c: 00 ed 03 8e  	lw	$3, -0x1300($16)
  19bf70: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19bf74: 42 00 02 3c  	lui	$2, 0x42
  19bf78: 20 00 b1 ff  	sd	$17, 0x20($sp)
  19bf7c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19bf80: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19bf84: 10 00 b0 ff  	sd	$16, 0x10($sp)
  19bf88: 38 5a 42 8c  	lw	$2, 0x5a38($2)
  19bf8c: 07 00 40 14  	bnez	$2, 0x19bfac <.text+0x9bfac>
  19bf90: 2d 88 a0 00  	move	$17, $5
  19bf94: 30 00 bf df  	ld	$ra, 0x30($sp)
  19bf98: 2d 10 60 00  	move	$2, $3
  19bf9c: 20 00 b1 df  	ld	$17, 0x20($sp)
  19bfa0: 10 00 b0 df  	ld	$16, 0x10($sp)
  19bfa4: 08 00 e0 03  	jr	$ra
  19bfa8: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19bfac: 44 00 02 3c  	lui	$2, 0x44
  19bfb0: 2d 28 80 00  	move	$5, $4
  19bfb4: 00 ed 50 24  	addiu	$16, $2, -0x1300 <.text+0xffffffffffefed00>
  19bfb8: 00 04 06 24  	addiu	$6, $zero, 0x400
  19bfbc: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  19bfc0: 2d 20 00 02  	move	$4, $16
  19bfc4: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19bfc8: 44 00 04 3c  	lui	$4, 0x44
  19bfcc: 01 00 05 24  	addiu	$5, $zero, 0x1
  19bfd0: 00 39 84 24  	addiu	$4, $4, 0x3900
  19bfd4: 2d 58 00 00  	move	$11, $zero
  19bfd8: 2d 30 00 00  	move	$6, $zero
  19bfdc: 2d 38 00 02  	move	$7, $16
  19bfe0: 00 04 08 24  	addiu	$8, $zero, 0x400
  19bfe4: 2d 48 00 02  	move	$9, $16
  19bfe8: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19bfec: 90 04 0a 24  	addiu	$10, $zero, 0x490
  19bff0: 00 04 03 26  	addiu	$3, $16, 0x400
  19bff4: 25 10 23 02  	or	$2, $17, $3
  19bff8: 80 04 04 26  	addiu	$4, $16, 0x480
  19bffc: 07 00 42 30  	andi	$2, $2, 0x7
  19c000: 23 00 40 10  	beqz	$2, 0x19c090 <.text+0x9c090>
  19c004: 2d 28 20 02  	move	$5, $17
  19c008: 2d 10 80 00  	move	$2, $4
  19c00c: 07 00 64 68  	ldl	$4, 0x7($3)
  19c010: 00 00 64 6c  	ldr	$4, 0x0($3)
  19c014: 0f 00 66 68  	ldl	$6, 0xf($3)
  19c018: 08 00 66 6c  	ldr	$6, 0x8($3)
  19c01c: 17 00 67 68  	ldl	$7, 0x17($3)
  19c020: 10 00 67 6c  	ldr	$7, 0x10($3)
  19c024: 1f 00 68 68  	ldl	$8, 0x1f($3)
  19c028: 18 00 68 6c  	ldr	$8, 0x18($3)
  19c02c: 07 00 a4 b0  	sdl	$4, 0x7($5)
  19c030: 00 00 a4 b4  	sdr	$4, 0x0($5)
  19c034: 0f 00 a6 b0  	sdl	$6, 0xf($5)
  19c038: 08 00 a6 b4  	sdr	$6, 0x8($5)
  19c03c: 17 00 a7 b0  	sdl	$7, 0x17($5)
  19c040: 10 00 a7 b4  	sdr	$7, 0x10($5)
  19c044: 1f 00 a8 b0  	sdl	$8, 0x1f($5)
  19c048: 18 00 a8 b4  	sdr	$8, 0x18($5)
  19c04c: 20 00 63 24  	addiu	$3, $3, 0x20
		...
  19c05c: eb ff 62 14  	bne	$3, $2, 0x19c00c <.text+0x9c00c>
  19c060: 20 00 a5 24  	addiu	$5, $5, 0x20
  19c064: 07 00 62 68  	ldl	$2, 0x7($3)
  19c068: 00 00 62 6c  	ldr	$2, 0x0($3)
  19c06c: 0f 00 64 68  	ldl	$4, 0xf($3)
  19c070: 08 00 64 6c  	ldr	$4, 0x8($3)
  19c074: 07 00 a2 b0  	sdl	$2, 0x7($5)
  19c078: 00 00 a2 b4  	sdr	$2, 0x0($5)
  19c07c: 0f 00 a4 b0  	sdl	$4, 0xf($5)
  19c080: 08 00 a4 b4  	sdr	$4, 0x8($5)
  19c084: 44 00 02 3c  	lui	$2, 0x44
  19c088: c2 ff 00 10  	b	0x19bf94 <.text+0x9bf94>
  19c08c: 00 ed 43 8c  	lw	$3, -0x1300($2)
  19c090: 00 00 62 dc  	ld	$2, 0x0($3)
  19c094: 08 00 66 dc  	ld	$6, 0x8($3)
  19c098: 10 00 67 dc  	ld	$7, 0x10($3)
  19c09c: 18 00 68 dc  	ld	$8, 0x18($3)
  19c0a0: 00 00 a2 fc  	sd	$2, 0x0($5)
  19c0a4: 08 00 a6 fc  	sd	$6, 0x8($5)
  19c0a8: 10 00 a7 fc  	sd	$7, 0x10($5)
  19c0ac: 18 00 a8 fc  	sd	$8, 0x18($5)
  19c0b0: 20 00 63 24  	addiu	$3, $3, 0x20
		...
  19c0c0: f3 ff 64 14  	bne	$3, $4, 0x19c090 <.text+0x9c090>
  19c0c4: 20 00 a5 24  	addiu	$5, $5, 0x20
  19c0c8: e6 ff 00 10  	b	0x19c064 <.text+0x9c064>
  19c0cc: 00 00 00 00  	nop
  19c0d0: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19c0d4: 42 00 02 3c  	lui	$2, 0x42
  19c0d8: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19c0dc: 44 00 07 3c  	lui	$7, 0x44
  19c0e0: 00 ed e7 24  	addiu	$7, $7, -0x1300 <.text+0xffffffffffefed00>
  19c0e4: 44 00 04 3c  	lui	$4, 0x44
  19c0e8: 38 5a 42 8c  	lw	$2, 0x5a38($2)
  19c0ec: 00 39 84 24  	addiu	$4, $4, 0x3900
  19c0f0: 04 00 05 24  	addiu	$5, $zero, 0x4
  19c0f4: 2d 30 00 00  	move	$6, $zero
  19c0f8: 2d 40 00 00  	move	$8, $zero
  19c0fc: 2d 48 e0 00  	move	$9, $7
  19c100: 2d 50 00 00  	move	$10, $zero
  19c104: 04 00 40 14  	bnez	$2, 0x19c118 <.text+0x9c118>
  19c108: 2d 58 00 00  	move	$11, $zero
  19c10c: 10 00 bf df  	ld	$ra, 0x10($sp)
  19c110: 08 00 e0 03  	jr	$ra
  19c114: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19c118: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19c11c: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19c120: fb ff 00 10  	b	0x19c110 <.text+0x9c110>
  19c124: 10 00 bf df  	ld	$ra, 0x10($sp)
  19c128: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19c12c: 42 00 02 3c  	lui	$2, 0x42
  19c130: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19c134: 10 00 b0 ff  	sd	$16, 0x10($sp)
  19c138: 38 5a 42 8c  	lw	$2, 0x5a38($2)
  19c13c: 06 00 40 14  	bnez	$2, 0x19c158 <.text+0x9c158>
  19c140: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19c144: 20 00 bf df  	ld	$ra, 0x20($sp)
  19c148: 2d 10 60 00  	move	$2, $3
  19c14c: 10 00 b0 df  	ld	$16, 0x10($sp)
  19c150: 08 00 e0 03  	jr	$ra
  19c154: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19c158: 44 00 10 3c  	lui	$16, 0x44
  19c15c: 44 00 04 3c  	lui	$4, 0x44
  19c160: 00 ed 07 26  	addiu	$7, $16, -0x1300 <.text+0xffffffffffefed00>
  19c164: 00 39 84 24  	addiu	$4, $4, 0x3900
  19c168: 05 00 05 24  	addiu	$5, $zero, 0x5
  19c16c: 2d 30 00 00  	move	$6, $zero
  19c170: 04 00 08 24  	addiu	$8, $zero, 0x4
  19c174: 2d 48 e0 00  	move	$9, $7
  19c178: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19c17c: 2d 58 00 00  	move	$11, $zero
  19c180: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19c184: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19c188: ee ff 00 10  	b	0x19c144 <.text+0x9c144>
  19c18c: 00 ed 03 8e  	lw	$3, -0x1300($16)
  19c190: 80 ff bd 27  	addiu	$sp, $sp, -0x80 <.text+0xffffffffffefff80>
  19c194: 42 00 02 3c  	lui	$2, 0x42
  19c198: 60 00 b5 ff  	sd	$21, 0x60($sp)
  19c19c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19c1a0: 50 00 b4 ff  	sd	$20, 0x50($sp)
  19c1a4: 2d a8 20 01  	move	$21, $9
  19c1a8: 40 00 b3 ff  	sd	$19, 0x40($sp)
  19c1ac: 2d a0 c0 00  	move	$20, $6
  19c1b0: 30 00 b2 ff  	sd	$18, 0x30($sp)
  19c1b4: 2d 98 00 01  	move	$19, $8
  19c1b8: 20 00 b1 ff  	sd	$17, 0x20($sp)
  19c1bc: 2d 90 e0 00  	move	$18, $7
  19c1c0: 70 00 bf ff  	sd	$ra, 0x70($sp)
  19c1c4: 10 00 b0 ff  	sd	$16, 0x10($sp)
  19c1c8: 38 5a 42 8c  	lw	$2, 0x5a38($2)
  19c1cc: 0b 00 40 14  	bnez	$2, 0x19c1fc <.text+0x9c1fc>
  19c1d0: 2d 88 a0 00  	move	$17, $5
  19c1d4: 70 00 bf df  	ld	$ra, 0x70($sp)
  19c1d8: 2d 10 60 00  	move	$2, $3
  19c1dc: 60 00 b5 df  	ld	$21, 0x60($sp)
  19c1e0: 50 00 b4 df  	ld	$20, 0x50($sp)
  19c1e4: 40 00 b3 df  	ld	$19, 0x40($sp)
  19c1e8: 30 00 b2 df  	ld	$18, 0x30($sp)
  19c1ec: 20 00 b1 df  	ld	$17, 0x20($sp)
  19c1f0: 10 00 b0 df  	ld	$16, 0x10($sp)
  19c1f4: 08 00 e0 03  	jr	$ra
  19c1f8: 80 00 bd 27  	addiu	$sp, $sp, 0x80
  19c1fc: 44 00 02 3c  	lui	$2, 0x44
  19c200: 2d 28 80 00  	move	$5, $4
  19c204: 00 ed 50 24  	addiu	$16, $2, -0x1300 <.text+0xffffffffffefed00>
  19c208: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  19c20c: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  19c210: 2d 20 00 02  	move	$4, $16
  19c214: 2d 28 20 02  	move	$5, $17
  19c218: 7f 00 06 24  	addiu	$6, $zero, 0x7f
  19c21c: 21 00 20 12  	beqz	$17, 0x19c2a4 <.text+0x9c2a4>
  19c220: 00 04 04 26  	addiu	$4, $16, 0x400
  19c224: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  19c228: 00 00 00 00  	nop
  19c22c: 90 00 05 24  	addiu	$5, $zero, 0x90
  19c230: 44 00 11 3c  	lui	$17, 0x44
  19c234: 18 28 65 02  	mult	$a1, $s3, $a1  # R5900 3-operand
  19c238: 00 ed 30 26  	addiu	$16, $17, -0x1300 <.text+0xffffffffffefed00>
  19c23c: 80 04 14 ae  	sw	$20, 0x480($16)
  19c240: 2d 20 40 02  	move	$4, $18
  19c244: 84 04 12 ae  	sw	$18, 0x484($16)
  19c248: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19c24c: 88 04 13 ae  	sw	$19, 0x488($16)
  19c250: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19c254: 44 00 04 3c  	lui	$4, 0x44
  19c258: 02 00 05 24  	addiu	$5, $zero, 0x2
  19c25c: 2d 30 00 00  	move	$6, $zero
  19c260: 00 39 84 24  	addiu	$4, $4, 0x3900
  19c264: 2d 38 00 02  	move	$7, $16
  19c268: 2d 48 00 02  	move	$9, $16
  19c26c: 2d 58 00 00  	move	$11, $zero
  19c270: 8c 04 08 24  	addiu	$8, $zero, 0x48c
  19c274: 04 04 0a 24  	addiu	$10, $zero, 0x404
  19c278: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19c27c: 04 00 10 26  	addiu	$16, $16, 0x4
  19c280: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  19c284: 00 ed 31 8e  	lw	$17, -0x1300($17)
  19c288: 2d 20 a0 02  	move	$4, $21
  19c28c: 03 00 a0 12  	beqz	$21, 0x19c29c <.text+0x9c29c>
  19c290: 2d 28 00 02  	move	$5, $16
  19c294: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  19c298: 00 00 00 00  	nop
  19c29c: cd ff 00 10  	b	0x19c1d4 <.text+0x9c1d4>
  19c2a0: 2d 18 20 02  	move	$3, $17
  19c2a4: e1 ff 00 10  	b	0x19c22c <.text+0x9c22c>
  19c2a8: 00 04 00 ae  	sw	$zero, 0x400($16)
  19c2ac: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19c2b0: 42 00 02 3c  	lui	$2, 0x42
  19c2b4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19c2b8: 44 00 07 3c  	lui	$7, 0x44
  19c2bc: 00 ed e7 24  	addiu	$7, $7, -0x1300 <.text+0xffffffffffefed00>
  19c2c0: 44 00 04 3c  	lui	$4, 0x44
  19c2c4: 38 5a 42 8c  	lw	$2, 0x5a38($2)
  19c2c8: 00 39 84 24  	addiu	$4, $4, 0x3900
  19c2cc: 07 00 05 24  	addiu	$5, $zero, 0x7
  19c2d0: 2d 30 00 00  	move	$6, $zero
  19c2d4: 2d 40 00 00  	move	$8, $zero
  19c2d8: 2d 48 e0 00  	move	$9, $7
  19c2dc: 2d 50 00 00  	move	$10, $zero
  19c2e0: 04 00 40 14  	bnez	$2, 0x19c2f4 <.text+0x9c2f4>
  19c2e4: 2d 58 00 00  	move	$11, $zero
  19c2e8: 10 00 bf df  	ld	$ra, 0x10($sp)
  19c2ec: 08 00 e0 03  	jr	$ra
  19c2f0: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19c2f4: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19c2f8: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19c2fc: fb ff 00 10  	b	0x19c2ec <.text+0x9c2ec>
  19c300: 10 00 bf df  	ld	$ra, 0x10($sp)
  19c304: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19c308: 42 00 02 3c  	lui	$2, 0x42
  19c30c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  19c310: 44 00 04 3c  	lui	$4, 0x44
  19c314: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19c318: 44 00 10 3c  	lui	$16, 0x44
  19c31c: 00 ed 07 26  	addiu	$7, $16, -0x1300 <.text+0xffffffffffefed00>
  19c320: 00 39 84 24  	addiu	$4, $4, 0x3900
  19c324: 38 5a 42 8c  	lw	$2, 0x5a38($2)
  19c328: 08 00 05 24  	addiu	$5, $zero, 0x8
  19c32c: 2d 30 00 00  	move	$6, $zero
  19c330: 2d 40 00 00  	move	$8, $zero
  19c334: 2d 48 e0 00  	move	$9, $7
  19c338: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19c33c: 05 00 40 14  	bnez	$2, 0x19c354 <.text+0x9c354>
  19c340: 2d 58 00 00  	move	$11, $zero
  19c344: 20 00 bf df  	ld	$ra, 0x20($sp)
  19c348: 10 00 b0 df  	ld	$16, 0x10($sp)
  19c34c: 08 00 e0 03  	jr	$ra
  19c350: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19c354: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19c358: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19c35c: f9 ff 00 10  	b	0x19c344 <.text+0x9c344>
  19c360: 00 ed 02 8e  	lw	$2, -0x1300($16)
