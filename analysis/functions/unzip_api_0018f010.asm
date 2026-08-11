# SNES Station v0.23 WIP — unzip.c API block
# Target VA: 0x0018f010..0x001906ff
# Extracted from the independently unpacked target image.

  18f010: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  18f014: 01 00 06 24  	addiu	$6, $zero, 0x1
  18f018: 20 00 b1 ff  	sd	$17, 0x20($sp)
  18f01c: 2d 88 a0 00  	move	$17, $5
  18f020: 2d 28 a0 03  	move	$5, $sp
  18f024: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18f028: 30 00 bf ff  	sd	$ra, 0x30($sp)
  18f02c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18f030: 2d 80 80 00  	move	$16, $4
  18f034: 00 00 10 2a  	slti	$16, $16, 0x0
  18f038: 01 00 03 24  	addiu	$3, $zero, 0x1
  18f03c: 09 00 43 10  	beq	$2, $3, 0x18f064 <.text+0x8f064>
  18f040: 2d 20 00 00  	move	$4, $zero
  18f044: ff ff 04 24  	addiu	$4, $zero, -0x1 <.text+0xffffffffffefffff>
  18f048: 0a 20 10 00  	movz	$4, $zero, $16
  18f04c: 30 00 bf df  	ld	$ra, 0x30($sp)
  18f050: 2d 10 80 00  	move	$2, $4
  18f054: 20 00 b1 df  	ld	$17, 0x20($sp)
  18f058: 10 00 b0 df  	ld	$16, 0x10($sp)
  18f05c: 08 00 e0 03  	jr	$ra
  18f060: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  18f064: 00 00 a2 93  	lbu	$2, 0x0($sp)
  18f068: f8 ff 00 10  	b	0x18f04c <.text+0x8f04c>
  18f06c: 00 00 22 ae  	sw	$2, 0x0($17)
  18f070: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  18f074: 30 00 b2 ff  	sd	$18, 0x30($sp)
  18f078: 2d 90 a0 00  	move	$18, $5
  18f07c: 2d 28 a0 03  	move	$5, $sp
  18f080: 20 00 b1 ff  	sd	$17, 0x20($sp)
  18f084: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18f088: 40 00 bf ff  	sd	$ra, 0x40($sp)
  18f08c: 04 3c 06 0c  	jal	0x18f010 <.text+0x8f010>
  18f090: 2d 80 80 00  	move	$16, $4
  18f094: 2d 30 40 00  	move	$6, $2
  18f098: 00 00 b1 8f  	lw	$17, 0x0($sp)
  18f09c: 2d 20 00 02  	move	$4, $16
  18f0a0: 0e 00 40 10  	beqz	$2, 0x18f0dc <.text+0x8f0dc>
  18f0a4: 2d 28 a0 03  	move	$5, $sp
  18f0a8: 00 00 a3 8f  	lw	$3, 0x0($sp)
  18f0ac: 2d 10 c0 00  	move	$2, $6
  18f0b0: 40 00 bf df  	ld	$ra, 0x40($sp)
  18f0b4: 38 1a 03 00  	dsll	$3, $3, 0x8
  18f0b8: 10 00 b0 df  	ld	$16, 0x10($sp)
  18f0bc: 2d 88 23 02  	daddu	$17, $17, $3
  18f0c0: 2d 18 00 00  	move	$3, $zero
  18f0c4: 0a 18 26 02  	movz	$3, $17, $6
  18f0c8: 20 00 b1 df  	ld	$17, 0x20($sp)
  18f0cc: 00 00 43 fe  	sd	$3, 0x0($18)
  18f0d0: 30 00 b2 df  	ld	$18, 0x30($sp)
  18f0d4: 08 00 e0 03  	jr	$ra
  18f0d8: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  18f0dc: 04 3c 06 0c  	jal	0x18f010 <.text+0x8f010>
  18f0e0: 00 00 00 00  	nop
  18f0e4: f0 ff 00 10  	b	0x18f0a8 <.text+0x8f0a8>
  18f0e8: 2d 30 40 00  	move	$6, $2
  18f0ec: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  18f0f0: 30 00 b2 ff  	sd	$18, 0x30($sp)
  18f0f4: 2d 90 a0 00  	move	$18, $5
  18f0f8: 2d 28 a0 03  	move	$5, $sp
  18f0fc: 20 00 b1 ff  	sd	$17, 0x20($sp)
  18f100: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18f104: 40 00 bf ff  	sd	$ra, 0x40($sp)
  18f108: 04 3c 06 0c  	jal	0x18f010 <.text+0x8f010>
  18f10c: 2d 88 80 00  	move	$17, $4
  18f110: 2d 30 40 00  	move	$6, $2
  18f114: 00 00 b0 8f  	lw	$16, 0x0($sp)
  18f118: 2d 20 20 02  	move	$4, $17
  18f11c: 22 00 40 10  	beqz	$2, 0x18f1a8 <.text+0x8f1a8>
  18f120: 2d 28 a0 03  	move	$5, $sp
  18f124: 00 00 a2 8f  	lw	$2, 0x0($sp)
  18f128: 2d 20 20 02  	move	$4, $17
  18f12c: 2d 28 a0 03  	move	$5, $sp
  18f130: 38 12 02 00  	dsll	$2, $2, 0x8
  18f134: 18 00 c0 10  	beqz	$6, 0x18f198 <.text+0x8f198>
  18f138: 2d 80 02 02  	daddu	$16, $16, $2
  18f13c: 00 00 a2 8f  	lw	$2, 0x0($sp)
  18f140: 2d 20 20 02  	move	$4, $17
  18f144: 2d 28 a0 03  	move	$5, $sp
  18f148: 38 14 02 00  	dsll	$2, $2, 0x10
  18f14c: 0e 00 c0 10  	beqz	$6, 0x18f188 <.text+0x8f188>
  18f150: 2d 80 02 02  	daddu	$16, $16, $2
  18f154: 00 00 a3 8f  	lw	$3, 0x0($sp)
  18f158: 2d 10 c0 00  	move	$2, $6
  18f15c: 40 00 bf df  	ld	$ra, 0x40($sp)
  18f160: 38 1e 03 00  	dsll	$3, $3, 0x18
  18f164: 20 00 b1 df  	ld	$17, 0x20($sp)
  18f168: 2d 80 03 02  	daddu	$16, $16, $3
  18f16c: 2d 18 00 00  	move	$3, $zero
  18f170: 0a 18 06 02  	movz	$3, $16, $6
  18f174: 10 00 b0 df  	ld	$16, 0x10($sp)
  18f178: 00 00 43 fe  	sd	$3, 0x0($18)
  18f17c: 30 00 b2 df  	ld	$18, 0x30($sp)
  18f180: 08 00 e0 03  	jr	$ra
  18f184: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  18f188: 04 3c 06 0c  	jal	0x18f010 <.text+0x8f010>
  18f18c: 00 00 00 00  	nop
  18f190: f0 ff 00 10  	b	0x18f154 <.text+0x8f154>
  18f194: 2d 30 40 00  	move	$6, $2
  18f198: 04 3c 06 0c  	jal	0x18f010 <.text+0x8f010>
  18f19c: 00 00 00 00  	nop
  18f1a0: e6 ff 00 10  	b	0x18f13c <.text+0x8f13c>
  18f1a4: 2d 30 40 00  	move	$6, $2
  18f1a8: 04 3c 06 0c  	jal	0x18f010 <.text+0x8f010>
  18f1ac: 00 00 00 00  	nop
  18f1b0: dc ff 00 10  	b	0x18f124 <.text+0x8f124>
  18f1b4: 2d 30 40 00  	move	$6, $2
  18f1b8: 2d 48 80 00  	move	$9, $4
  18f1bc: 2d 40 a0 00  	move	$8, $5
  18f1c0: 00 00 27 81  	lb	$7, 0x0($9)
  18f1c4: 01 00 0a 24  	addiu	$10, $zero, 0x1
  18f1c8: 00 00 06 81  	lb	$6, 0x0($8)
  18f1cc: 01 00 29 25  	addiu	$9, $9, 0x1
  18f1d0: 9f ff e2 24  	addiu	$2, $7, -0x61 <.text+0xffffffffffefff9f>
  18f1d4: e0 ff e5 24  	addiu	$5, $7, -0x20 <.text+0xffffffffffefffe0>
  18f1d8: e0 ff c4 24  	addiu	$4, $6, -0x20 <.text+0xffffffffffefffe0>
  18f1dc: 9f ff c3 24  	addiu	$3, $6, -0x61 <.text+0xffffffffffefff9f>
  18f1e0: 1a 00 42 2c  	sltiu	$2, $2, 0x1a
  18f1e4: 1a 00 63 2c  	sltiu	$3, $3, 0x1a
  18f1e8: 00 2e 05 00  	sll	$5, $5, 0x18
  18f1ec: 00 26 04 00  	sll	$4, $4, 0x18
  18f1f0: 02 00 40 10  	beqz	$2, 0x18f1fc <.text+0x8f1fc>
  18f1f4: 01 00 08 25  	addiu	$8, $8, 0x1
  18f1f8: 03 3e 05 00  	sra	$7, $5, 0x18
  18f1fc: 01 00 60 54  	bnezl	$3, 0x18f204 <.text+0x8f204>
  18f200: 03 36 04 00  	sra	$6, $4, 0x18
  18f204: 0c 00 e0 50  	beqzl	$7, 0x18f238 <.text+0x8f238>
  18f208: ff ff 0a 24  	addiu	$10, $zero, -0x1 <.text+0xffffffffffefffff>
  18f20c: 08 00 c0 10  	beqz	$6, 0x18f230 <.text+0x8f230>
  18f210: 00 00 00 00  	nop
  18f214: 2a 10 e6 00  	slt	$2, $7, $6
  18f218: 05 00 40 14  	bnez	$2, 0x18f230 <.text+0x8f230>
  18f21c: ff ff 0a 24  	addiu	$10, $zero, -0x1 <.text+0xffffffffffefffff>
  18f220: 2a 10 c7 00  	slt	$2, $6, $7
  18f224: e7 ff 40 50  	beqzl	$2, 0x18f1c4 <.text+0x8f1c4>
  18f228: 00 00 27 81  	lb	$7, 0x0($9)
  18f22c: 01 00 0a 24  	addiu	$10, $zero, 0x1
  18f230: 08 00 e0 03  	jr	$ra
  18f234: 2d 10 40 01  	move	$2, $10
  18f238: fd ff 00 10  	b	0x18f230 <.text+0x8f230>
  18f23c: 0a 50 06 00  	movz	$10, $zero, $6
  18f240: 02 00 02 24  	addiu	$2, $zero, 0x2
  18f244: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  18f248: 0a 30 46 00  	movz	$6, $2, $6
  18f24c: 01 00 02 24  	addiu	$2, $zero, 0x1
  18f250: 06 00 c2 10  	beq	$6, $2, 0x18f26c <.text+0x8f26c>
  18f254: 00 00 bf ff  	sd	$ra, 0x0($sp)
  18f258: 6e 3c 06 0c  	jal	0x18f1b8 <.text+0x8f1b8>
  18f25c: 00 00 00 00  	nop
  18f260: 00 00 bf df  	ld	$ra, 0x0($sp)
  18f264: 08 00 e0 03  	jr	$ra
  18f268: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  18f26c: 92 71 06 0c  	jal	0x19c648 <.text+0x9c648>
  18f270: 00 00 00 00  	nop
  18f274: fb ff 00 10  	b	0x18f264 <.text+0x8f264>
  18f278: 00 00 bf df  	ld	$ra, 0x0($sp)
  18f27c: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  18f280: 2d 28 00 00  	move	$5, $zero
  18f284: 02 00 06 24  	addiu	$6, $zero, 0x2
  18f288: 90 00 bf ff  	sd	$ra, 0x90($sp)
  18f28c: 80 00 be ff  	sd	$fp, 0x80($sp)
  18f290: 2d f0 80 00  	move	$fp, $4
  18f294: 70 00 b7 ff  	sd	$23, 0x70($sp)
  18f298: 2d b8 00 00  	move	$23, $zero
  18f29c: 50 00 b5 ff  	sd	$21, 0x50($sp)
  18f2a0: 40 00 b4 ff  	sd	$20, 0x40($sp)
  18f2a4: ff ff 14 34  	ori	$20, $zero, 0xffff
  18f2a8: 30 00 b3 ff  	sd	$19, 0x30($sp)
  18f2ac: 60 00 b6 ff  	sd	$22, 0x60($sp)
  18f2b0: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18f2b4: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18f2b8: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  18f2bc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18f2c0: 2d a8 40 00  	move	$21, $2
  18f2c4: 04 04 04 24  	addiu	$4, $zero, 0x404
  18f2c8: 2b 10 b4 02  	sltu	$2, $21, $20
  18f2cc: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  18f2d0: 0b a0 a2 02  	movn	$20, $21, $2
  18f2d4: 2d 98 40 00  	move	$19, $2
  18f2d8: 0a 00 60 12  	beqz	$19, 0x18f304 <.text+0x8f304>
  18f2dc: 2d 10 00 00  	move	$2, $zero
  18f2e0: 04 00 16 24  	addiu	$22, $zero, 0x4
  18f2e4: 2b 10 d4 02  	sltu	$2, $22, $20
  18f2e8: 12 00 40 14  	bnez	$2, 0x18f334 <.text+0x8f334>
  18f2ec: 00 04 c2 66  	daddiu	$2, $22, 0x400
  18f2f0: 04 00 60 12  	beqz	$19, 0x18f304 <.text+0x8f304>
  18f2f4: 2d 10 e0 02  	move	$2, $23
  18f2f8: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  18f2fc: 2d 20 60 02  	move	$4, $19
  18f300: 2d 10 e0 02  	move	$2, $23
  18f304: 90 00 bf df  	ld	$ra, 0x90($sp)
  18f308: 80 00 be df  	ld	$fp, 0x80($sp)
  18f30c: 70 00 b7 df  	ld	$23, 0x70($sp)
  18f310: 60 00 b6 df  	ld	$22, 0x60($sp)
  18f314: 50 00 b5 df  	ld	$21, 0x50($sp)
  18f318: 40 00 b4 df  	ld	$20, 0x40($sp)
  18f31c: 30 00 b3 df  	ld	$19, 0x30($sp)
  18f320: 20 00 b2 df  	ld	$18, 0x20($sp)
  18f324: 10 00 b1 df  	ld	$17, 0x10($sp)
  18f328: 00 00 b0 df  	ld	$16, 0x0($sp)
  18f32c: 08 00 e0 03  	jr	$ra
  18f330: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  18f334: 2d 20 c0 03  	move	$4, $fp
  18f338: 2b 18 82 02  	sltu	$3, $20, $2
  18f33c: 2d b0 80 02  	move	$22, $20
  18f340: 0a b0 43 00  	movz	$22, $2, $3
  18f344: 2d 30 00 00  	move	$6, $zero
  18f348: 2f 90 b6 02  	dsubu	$18, $21, $22
  18f34c: 04 04 02 24  	addiu	$2, $zero, 0x404
  18f350: 2f 80 b2 02  	dsubu	$16, $21, $18
  18f354: 3c 28 12 00  	dsll32	$5, $18, 0x0
  18f358: 3f 28 05 00  	dsra32	$5, $5, 0x0
  18f35c: 05 04 03 2e  	sltiu	$3, $16, 0x405
  18f360: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  18f364: 0a 80 43 00  	movz	$16, $2, $3
  18f368: e1 ff 40 04  	bltz	$2, 0x18f2f0 <.text+0x8f2f0>
  18f36c: 2d 20 c0 03  	move	$4, $fp
  18f370: 3c 88 10 00  	dsll32	$17, $16, 0x0
  18f374: 3f 88 11 00  	dsra32	$17, $17, 0x0
  18f378: 2d 28 60 02  	move	$5, $19
  18f37c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18f380: 2d 30 20 02  	move	$6, $17
  18f384: da ff 50 14  	bne	$2, $16, 0x18f2f0 <.text+0x8f2f0>
  18f388: fd ff 22 26  	addiu	$2, $17, -0x3 <.text+0xffffffffffeffffd>
  18f38c: 0a 00 40 18  	blez	$2, 0x18f3b8 <.text+0x8f3b8>
  18f390: 2d 28 00 00  	move	$5, $zero
  18f394: 21 20 65 02  	addu	$4, $19, $5
  18f398: 50 00 02 24  	addiu	$2, $zero, 0x50
  18f39c: 00 00 83 90  	lbu	$3, 0x0($4)
  18f3a0: 0b 00 62 10  	beq	$3, $2, 0x18f3d0 <.text+0x8f3d0>
  18f3a4: fd ff 26 26  	addiu	$6, $17, -0x3 <.text+0xffffffffffeffffd>
  18f3a8: 01 00 a5 24  	addiu	$5, $5, 0x1
  18f3ac: 2a 10 a6 00  	slt	$2, $5, $6
  18f3b0: f9 ff 40 54  	bnezl	$2, 0x18f398 <.text+0x8f398>
  18f3b4: 21 20 65 02  	addu	$4, $19, $5
  18f3b8: cd ff e0 16  	bnez	$23, 0x18f2f0 <.text+0x8f2f0>
  18f3bc: 2b 10 d4 02  	sltu	$2, $22, $20
  18f3c0: dc ff 40 14  	bnez	$2, 0x18f334 <.text+0x8f334>
  18f3c4: 00 04 c2 66  	daddiu	$2, $22, 0x400
  18f3c8: c9 ff 00 10  	b	0x18f2f0 <.text+0x8f2f0>
  18f3cc: 00 00 00 00  	nop
  18f3d0: 01 00 83 90  	lbu	$3, 0x1($4)
  18f3d4: 4b 00 02 24  	addiu	$2, $zero, 0x4b
  18f3d8: f4 ff 62 54  	bnel	$3, $2, 0x18f3ac <.text+0x8f3ac>
  18f3dc: 01 00 a5 24  	addiu	$5, $5, 0x1
  18f3e0: 02 00 83 90  	lbu	$3, 0x2($4)
  18f3e4: 05 00 02 24  	addiu	$2, $zero, 0x5
  18f3e8: f0 ff 62 54  	bnel	$3, $2, 0x18f3ac <.text+0x8f3ac>
  18f3ec: 01 00 a5 24  	addiu	$5, $5, 0x1
  18f3f0: 03 00 83 90  	lbu	$3, 0x3($4)
  18f3f4: 06 00 02 24  	addiu	$2, $zero, 0x6
  18f3f8: ec ff 62 54  	bnel	$3, $2, 0x18f3ac <.text+0x8f3ac>
  18f3fc: 01 00 a5 24  	addiu	$5, $5, 0x1
  18f400: ed ff 00 10  	b	0x18f3b8 <.text+0x8f3b8>
  18f404: 2d b8 b2 00  	daddu	$23, $5, $18
  18f408: a0 fe bd 27  	addiu	$sp, $sp, -0x160 <.text+0xffffffffffeffea0>
  18f40c: 01 00 05 24  	addiu	$5, $zero, 0x1
  18f410: 30 01 b2 ff  	sd	$18, 0x130($sp)
  18f414: 50 01 bf ff  	sd	$ra, 0x150($sp)
  18f418: 40 01 b3 ff  	sd	$19, 0x140($sp)
  18f41c: 20 01 b1 ff  	sd	$17, 0x120($sp)
  18f420: f0 73 06 0c  	jal	0x19cfc0 <.text+0x9cfc0>
  18f424: 10 01 b0 ff  	sd	$16, 0x110($sp)
  18f428: 2d 90 40 00  	move	$18, $2
  18f42c: 41 00 40 06  	bltz	$18, 0x18f534 <.text+0x8f534>
  18f430: 2d 10 00 00  	move	$2, $zero
  18f434: 2d 20 40 02  	move	$4, $18
  18f438: 9f 3c 06 0c  	jal	0x18f27c <.text+0x8f27c>
  18f43c: ff ff 10 24  	addiu	$16, $zero, -0x1 <.text+0xffffffffffefffff>
  18f440: 2d 88 00 02  	move	$17, $16
  18f444: 2d 30 00 00  	move	$6, $zero
  18f448: 3c 28 02 00  	dsll32	$5, $2, 0x0
  18f44c: 3f 28 05 00  	dsra32	$5, $5, 0x0
  18f450: 2d 20 40 02  	move	$4, $18
  18f454: 0b 88 02 00  	movn	$17, $zero, $2
  18f458: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  18f45c: 2d 98 40 00  	move	$19, $2
  18f460: 00 00 42 28  	slti	$2, $2, 0x0
  18f464: f0 00 a5 27  	addiu	$5, $sp, 0xf0
  18f468: 2d 20 40 02  	move	$4, $18
  18f46c: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f470: 0b 88 02 02  	movn	$17, $16, $2
  18f474: f8 00 a5 27  	addiu	$5, $sp, 0xf8
  18f478: 2d 20 40 02  	move	$4, $18
  18f47c: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f480: 0b 88 02 02  	movn	$17, $16, $2
  18f484: 00 01 a5 27  	addiu	$5, $sp, 0x100
  18f488: 2d 20 40 02  	move	$4, $18
  18f48c: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f490: 0b 88 02 02  	movn	$17, $16, $2
  18f494: 08 00 a5 27  	addiu	$5, $sp, 0x8
  18f498: 2d 20 40 02  	move	$4, $18
  18f49c: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f4a0: 0b 88 02 02  	movn	$17, $16, $2
  18f4a4: 2d 20 40 02  	move	$4, $18
  18f4a8: 08 01 a5 27  	addiu	$5, $sp, 0x108
  18f4ac: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f4b0: 0b 88 02 02  	movn	$17, $16, $2
  18f4b4: 08 01 a3 df  	ld	$3, 0x108($sp)
  18f4b8: 0b 88 02 02  	movn	$17, $16, $2
  18f4bc: 08 00 a2 df  	ld	$2, 0x8($sp)
  18f4c0: 40 00 62 10  	beq	$3, $2, 0x18f5c4 <.text+0x8f5c4>
  18f4c4: 00 01 a2 df  	ld	$2, 0x100($sp)
  18f4c8: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18f4cc: 2d 20 40 02  	move	$4, $18
  18f4d0: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f4d4: 40 00 a5 27  	addiu	$5, $sp, 0x40
  18f4d8: ff ff 10 24  	addiu	$16, $zero, -0x1 <.text+0xffffffffffefffff>
  18f4dc: 2d 20 40 02  	move	$4, $18
  18f4e0: 48 00 a5 27  	addiu	$5, $sp, 0x48
  18f4e4: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f4e8: 0b 88 02 02  	movn	$17, $16, $2
  18f4ec: 2d 20 40 02  	move	$4, $18
  18f4f0: 10 00 a5 27  	addiu	$5, $sp, 0x10
  18f4f4: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f4f8: 0b 88 02 02  	movn	$17, $16, $2
  18f4fc: 0b 88 02 02  	movn	$17, $16, $2
  18f500: 48 00 a3 df  	ld	$3, 0x48($sp)
  18f504: 40 00 a2 df  	ld	$2, 0x40($sp)
  18f508: 2d 18 62 00  	daddu	$3, $3, $2
  18f50c: 2b 10 63 02  	sltu	$2, $19, $3
  18f510: 03 00 40 10  	beqz	$2, 0x18f520 <.text+0x8f520>
  18f514: 00 00 00 00  	nop
  18f518: 03 00 20 16  	bnez	$17, 0x18f528 <.text+0x8f528>
  18f51c: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18f520: 0b 00 20 12  	beqz	$17, 0x18f550 <.text+0x8f550>
  18f524: 2f 10 63 02  	dsubu	$2, $19, $3
  18f528: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  18f52c: 2d 20 40 02  	move	$4, $18
  18f530: 2d 10 00 00  	move	$2, $zero
  18f534: 50 01 bf df  	ld	$ra, 0x150($sp)
  18f538: 40 01 b3 df  	ld	$19, 0x140($sp)
  18f53c: 30 01 b2 df  	ld	$18, 0x130($sp)
  18f540: 20 01 b1 df  	ld	$17, 0x120($sp)
  18f544: 10 01 b0 df  	ld	$16, 0x110($sp)
  18f548: 08 00 e0 03  	jr	$ra
  18f54c: 60 01 bd 27  	addiu	$sp, $sp, 0x160
  18f550: e8 00 04 24  	addiu	$4, $zero, 0xe8
  18f554: 00 00 b2 af  	sw	$18, 0x0($sp)
  18f558: 18 00 a2 ff  	sd	$2, 0x18($sp)
  18f55c: 38 00 b3 ff  	sd	$19, 0x38($sp)
  18f560: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  18f564: e0 00 a0 af  	sw	$zero, 0xe0($sp)
  18f568: 2d 80 40 00  	move	$16, $2
  18f56c: 2d 18 a0 03  	move	$3, $sp
  18f570: e0 00 a4 27  	addiu	$4, $sp, 0xe0
  18f574: 00 00 65 dc  	ld	$5, 0x0($3)
  18f578: 08 00 66 dc  	ld	$6, 0x8($3)
  18f57c: 10 00 67 dc  	ld	$7, 0x10($3)
  18f580: 18 00 68 dc  	ld	$8, 0x18($3)
  18f584: 00 00 45 fc  	sd	$5, 0x0($2)
  18f588: 08 00 46 fc  	sd	$6, 0x8($2)
  18f58c: 10 00 47 fc  	sd	$7, 0x10($2)
  18f590: 18 00 48 fc  	sd	$8, 0x18($2)
  18f594: 20 00 63 24  	addiu	$3, $3, 0x20
		...
  18f5a4: f3 ff 64 14  	bne	$3, $4, 0x18f574 <.text+0x8f574>
  18f5a8: 20 00 42 24  	addiu	$2, $2, 0x20
  18f5ac: 00 00 65 dc  	ld	$5, 0x0($3)
  18f5b0: 2d 20 00 02  	move	$4, $16
  18f5b4: bb 3e 06 0c  	jal	0x18faec <.text+0x8faec>
  18f5b8: 00 00 45 fc  	sd	$5, 0x0($2)
  18f5bc: dd ff 00 10  	b	0x18f534 <.text+0x8f534>
  18f5c0: 2d 10 00 02  	move	$2, $16
  18f5c4: c1 ff 40 54  	bnezl	$2, 0x18f4cc <.text+0x8f4cc>
  18f5c8: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18f5cc: f8 00 a2 df  	ld	$2, 0xf8($sp)
  18f5d0: bf ff 40 10  	beqz	$2, 0x18f4d0 <.text+0x8f4d0>
  18f5d4: 2d 20 40 02  	move	$4, $18
  18f5d8: bd ff 00 10  	b	0x18f4d0 <.text+0x8f4d0>
  18f5dc: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18f5e0: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  18f5e4: 9a ff 02 24  	addiu	$2, $zero, -0x66 <.text+0xffffffffffefff9a>
  18f5e8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18f5ec: 2d 80 80 00  	move	$16, $4
  18f5f0: 09 00 80 10  	beqz	$4, 0x18f618 <.text+0x8f618>
  18f5f4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  18f5f8: e0 00 82 8c  	lw	$2, 0xe0($4)
  18f5fc: 0a 00 40 14  	bnez	$2, 0x18f628 <.text+0x8f628>
  18f600: 00 00 00 00  	nop
  18f604: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  18f608: 00 00 04 8e  	lw	$4, 0x0($16)
  18f60c: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  18f610: 2d 20 00 02  	move	$4, $16
  18f614: 2d 10 00 00  	move	$2, $zero
  18f618: 10 00 bf df  	ld	$ra, 0x10($sp)
  18f61c: 00 00 b0 df  	ld	$16, 0x0($sp)
  18f620: 08 00 e0 03  	jr	$ra
  18f624: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  18f628: 5e 41 06 0c  	jal	0x190578 <.text+0x90578>
  18f62c: 00 00 00 00  	nop
  18f630: f4 ff 00 10  	b	0x18f604 <.text+0x8f604>
  18f634: 00 00 00 00  	nop
  18f638: 04 00 80 10  	beqz	$4, 0x18f64c <.text+0x8f64c>
  18f63c: 9a ff 03 24  	addiu	$3, $zero, -0x66 <.text+0xffffffffffefff9a>
  18f640: 08 00 82 78  	<unknown>
  18f644: 2d 18 00 00  	move	$3, $zero
  18f648: 00 00 a2 7c  	ext	$2, $5, 0x0, 0x1
  18f64c: 08 00 e0 03  	jr	$ra
  18f650: 2d 10 60 00  	move	$2, $3
  18f654: 3a 1c 04 00  	dsrl	$3, $4, 0x10
  18f658: 1f 00 88 30  	andi	$8, $4, 0x1f
  18f65c: 00 fe 66 30  	andi	$6, $3, 0xfe00
  18f660: e0 01 62 30  	andi	$2, $3, 0x1e0
  18f664: 7a 11 02 00  	dsrl	$2, $2, 0x5
  18f668: 7a 32 06 00  	dsrl	$6, $6, 0x9
  18f66c: 00 f8 87 30  	andi	$7, $4, 0xf800
  18f670: ff ff 42 64  	daddiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  18f674: bc 07 c6 64  	daddiu	$6, $6, 0x7bc
  18f678: 1f 00 63 30  	andi	$3, $3, 0x1f
  18f67c: e0 07 84 30  	andi	$4, $4, 0x7e0
  18f680: 3c 18 03 00  	dsll32	$3, $3, 0x0
  18f684: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18f688: 3c 10 02 00  	dsll32	$2, $2, 0x0
  18f68c: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18f690: 3c 30 06 00  	dsll32	$6, $6, 0x0
  18f694: 3f 30 06 00  	dsra32	$6, $6, 0x0
  18f698: 78 3d 07 00  	dsll	$7, $7, 0x15
  18f69c: 3f 38 07 00  	dsra32	$7, $7, 0x0
  18f6a0: f8 26 04 00  	dsll	$4, $4, 0x1b
  18f6a4: 3f 20 04 00  	dsra32	$4, $4, 0x0
  18f6a8: 7c 40 08 00  	dsll32	$8, $8, 0x1
  18f6ac: 3f 40 08 00  	dsra32	$8, $8, 0x0
  18f6b0: 0c 00 a3 ac  	sw	$3, 0xc($5)
  18f6b4: 00 00 a8 ac  	sw	$8, 0x0($5)
  18f6b8: 10 00 a2 ac  	sw	$2, 0x10($5)
  18f6bc: 14 00 a6 ac  	sw	$6, 0x14($5)
  18f6c0: 08 00 a7 ac  	sw	$7, 0x8($5)
  18f6c4: 08 00 e0 03  	jr	$ra
  18f6c8: 04 00 a4 ac  	sw	$4, 0x4($5)
  18f6cc: b0 fe bd 27  	addiu	$sp, $sp, -0x150 <.text+0xffffffffffeffeb0>
  18f6d0: 9a ff 02 24  	addiu	$2, $zero, -0x66 <.text+0xffffffffffefff9a>
  18f6d4: 30 01 be ff  	sd	$fp, 0x130($sp)
  18f6d8: 2d f0 60 01  	move	$fp, $11
  18f6dc: 20 01 b7 ff  	sd	$23, 0x120($sp)
  18f6e0: 2d b8 40 01  	move	$23, $10
  18f6e4: 10 01 b6 ff  	sd	$22, 0x110($sp)
  18f6e8: 2d b0 e0 00  	move	$22, $7
  18f6ec: 00 01 b5 ff  	sd	$21, 0x100($sp)
  18f6f0: 2d a8 00 01  	move	$21, $8
  18f6f4: e0 00 b3 ff  	sd	$19, 0xe0($sp)
  18f6f8: 2d 98 80 00  	move	$19, $4
  18f6fc: 40 01 bf ff  	sd	$ra, 0x140($sp)
  18f700: f0 00 b4 ff  	sd	$20, 0xf0($sp)
  18f704: d0 00 b2 ff  	sd	$18, 0xd0($sp)
  18f708: c0 00 b1 ff  	sd	$17, 0xc0($sp)
  18f70c: b0 00 b0 ff  	sd	$16, 0xb0($sp)
  18f710: a0 00 a5 af  	sw	$5, 0xa0($sp)
  18f714: a4 00 a6 af  	sw	$6, 0xa4($sp)
  18f718: 9f 00 80 10  	beqz	$4, 0x18f998 <.text+0x8f998>
  18f71c: a8 00 a9 af  	sw	$9, 0xa8($sp)
  18f720: 28 00 85 dc  	ld	$5, 0x28($4)
  18f724: 2d 30 00 00  	move	$6, $zero
  18f728: 18 00 82 dc  	ld	$2, 0x18($4)
  18f72c: ff ff 12 24  	addiu	$18, $zero, -0x1 <.text+0xffffffffffefffff>
  18f730: 2d 28 a2 00  	daddu	$5, $5, $2
  18f734: 3c 28 05 00  	dsll32	$5, $5, 0x0
  18f738: 3f 28 05 00  	dsra32	$5, $5, 0x0
  18f73c: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  18f740: 00 00 84 8c  	lw	$4, 0x0($4)
  18f744: 00 00 42 28  	slti	$2, $2, 0x0
  18f748: 0a 90 02 00  	movz	$18, $zero, $2
  18f74c: c9 00 40 52  	beqzl	$18, 0x18fa74 <.text+0x8fa74>
  18f750: 00 00 64 8e  	lw	$4, 0x0($19)
  18f754: 00 00 64 8e  	lw	$4, 0x0($19)
  18f758: 2d 28 a0 03  	move	$5, $sp
  18f75c: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f760: ff ff 10 24  	addiu	$16, $zero, -0x1 <.text+0xffffffffffefffff>
  18f764: 08 00 a5 27  	addiu	$5, $sp, 0x8
  18f768: 00 00 64 8e  	lw	$4, 0x0($19)
  18f76c: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f770: 0b 90 02 02  	movn	$18, $16, $2
  18f774: 10 00 a5 27  	addiu	$5, $sp, 0x10
  18f778: 00 00 64 8e  	lw	$4, 0x0($19)
  18f77c: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f780: 0b 90 02 02  	movn	$18, $16, $2
  18f784: 18 00 a5 27  	addiu	$5, $sp, 0x18
  18f788: 00 00 64 8e  	lw	$4, 0x0($19)
  18f78c: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f790: 0b 90 02 02  	movn	$18, $16, $2
  18f794: 20 00 a5 27  	addiu	$5, $sp, 0x20
  18f798: 00 00 64 8e  	lw	$4, 0x0($19)
  18f79c: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f7a0: 0b 90 02 02  	movn	$18, $16, $2
  18f7a4: 70 00 a5 27  	addiu	$5, $sp, 0x70
  18f7a8: 20 00 a4 df  	ld	$4, 0x20($sp)
  18f7ac: 95 3d 06 0c  	jal	0x18f654 <.text+0x8f654>
  18f7b0: 0b 90 02 02  	movn	$18, $16, $2
  18f7b4: 28 00 a5 27  	addiu	$5, $sp, 0x28
  18f7b8: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f7bc: 00 00 64 8e  	lw	$4, 0x0($19)
  18f7c0: 30 00 a5 27  	addiu	$5, $sp, 0x30
  18f7c4: 00 00 64 8e  	lw	$4, 0x0($19)
  18f7c8: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f7cc: 0b 90 02 02  	movn	$18, $16, $2
  18f7d0: 38 00 a5 27  	addiu	$5, $sp, 0x38
  18f7d4: 00 00 64 8e  	lw	$4, 0x0($19)
  18f7d8: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f7dc: 0b 90 02 02  	movn	$18, $16, $2
  18f7e0: 40 00 a5 27  	addiu	$5, $sp, 0x40
  18f7e4: 00 00 64 8e  	lw	$4, 0x0($19)
  18f7e8: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f7ec: 0b 90 02 02  	movn	$18, $16, $2
  18f7f0: 48 00 a5 27  	addiu	$5, $sp, 0x48
  18f7f4: 00 00 64 8e  	lw	$4, 0x0($19)
  18f7f8: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f7fc: 0b 90 02 02  	movn	$18, $16, $2
  18f800: 50 00 a5 27  	addiu	$5, $sp, 0x50
  18f804: 00 00 64 8e  	lw	$4, 0x0($19)
  18f808: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f80c: 0b 90 02 02  	movn	$18, $16, $2
  18f810: 58 00 a5 27  	addiu	$5, $sp, 0x58
  18f814: 00 00 64 8e  	lw	$4, 0x0($19)
  18f818: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f81c: 0b 90 02 02  	movn	$18, $16, $2
  18f820: 60 00 a5 27  	addiu	$5, $sp, 0x60
  18f824: 00 00 64 8e  	lw	$4, 0x0($19)
  18f828: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18f82c: 0b 90 02 02  	movn	$18, $16, $2
  18f830: 68 00 a5 27  	addiu	$5, $sp, 0x68
  18f834: 00 00 64 8e  	lw	$4, 0x0($19)
  18f838: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f83c: 0b 90 02 02  	movn	$18, $16, $2
  18f840: 00 00 64 8e  	lw	$4, 0x0($19)
  18f844: 98 00 a5 27  	addiu	$5, $sp, 0x98
  18f848: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18f84c: 0b 90 02 02  	movn	$18, $16, $2
  18f850: 40 00 b1 df  	ld	$17, 0x40($sp)
  18f854: 0b 90 02 02  	movn	$18, $16, $2
  18f858: 7c 00 40 16  	bnez	$18, 0x18fa4c <.text+0x8fa4c>
  18f85c: 2d a0 20 02  	move	$20, $17
  18f860: 0c 00 c0 12  	beqz	$22, 0x18f894 <.text+0x8f894>
  18f864: 2b 10 35 02  	sltu	$2, $17, $21
  18f868: 05 00 40 10  	beqz	$2, 0x18f880 <.text+0x8f880>
  18f86c: 2d 80 a0 02  	move	$16, $21
  18f870: 40 00 a2 8f  	lw	$2, 0x40($sp)
  18f874: 2d 80 20 02  	move	$16, $17
  18f878: 21 10 c2 02  	addu	$2, $22, $2
  18f87c: 00 00 40 a0  	sb	$zero, 0x0($2)
  18f880: 04 00 80 12  	beqz	$20, 0x18f894 <.text+0x8f894>
  18f884: 2f 88 90 02  	dsubu	$17, $20, $16
  18f888: 72 00 a0 56  	bnezl	$21, 0x18fa54 <.text+0x8fa54>
  18f88c: 00 00 64 8e  	lw	$4, 0x0($19)
  18f890: 2f 88 90 02  	dsubu	$17, $20, $16
  18f894: 0f 00 40 16  	bnez	$18, 0x18f8d4 <.text+0x8f8d4>
  18f898: 48 00 a2 df  	ld	$2, 0x48($sp)
  18f89c: a8 00 a2 8f  	lw	$2, 0xa8($sp)
  18f8a0: 6a 00 40 10  	beqz	$2, 0x18fa4c <.text+0x8fa4c>
  18f8a4: 2d 80 e0 02  	move	$16, $23
  18f8a8: 48 00 a3 df  	ld	$3, 0x48($sp)
  18f8ac: 2b 10 77 00  	sltu	$2, $3, $23
  18f8b0: 5d 00 20 16  	bnez	$17, 0x18fa28 <.text+0x8fa28>
  18f8b4: 0b 80 62 00  	movn	$16, $3, $2
  18f8b8: 48 00 a2 df  	ld	$2, 0x48($sp)
  18f8bc: 05 00 40 10  	beqz	$2, 0x18f8d4 <.text+0x8f8d4>
  18f8c0: 2f 10 50 00  	dsubu	$2, $2, $16
  18f8c4: 50 00 e0 56  	bnezl	$23, 0x18fa08 <.text+0x8fa08>
  18f8c8: 00 00 64 8e  	lw	$4, 0x0($19)
  18f8cc: 48 00 a2 df  	ld	$2, 0x48($sp)
  18f8d0: 2f 10 50 00  	dsubu	$2, $2, $16
  18f8d4: 12 00 40 16  	bnez	$18, 0x18f920 <.text+0x8f920>
  18f8d8: 2d 88 22 02  	daddu	$17, $17, $2
  18f8dc: 10 00 c0 13  	beqz	$fp, 0x18f920 <.text+0x8f920>
  18f8e0: 50 00 a6 df  	ld	$6, 0x50($sp)
  18f8e4: 50 01 a3 df  	ld	$3, 0x150($sp)
  18f8e8: 2b 10 c3 00  	sltu	$2, $6, $3
  18f8ec: 05 00 40 10  	beqz	$2, 0x18f904 <.text+0x8f904>
  18f8f0: 50 01 b0 df  	ld	$16, 0x150($sp)
  18f8f4: 50 00 a2 8f  	lw	$2, 0x50($sp)
  18f8f8: 2d 80 c0 00  	move	$16, $6
  18f8fc: 21 10 c2 03  	addu	$2, $fp, $2
  18f900: 00 00 40 a0  	sb	$zero, 0x0($2)
  18f904: 38 00 20 56  	bnezl	$17, 0x18f9e8 <.text+0x8f9e8>
  18f908: 00 00 64 8e  	lw	$4, 0x0($19)
  18f90c: 50 00 a2 df  	ld	$2, 0x50($sp)
  18f910: 03 00 40 10  	beqz	$2, 0x18f920 <.text+0x8f920>
  18f914: 50 01 a4 df  	ld	$4, 0x150($sp)
  18f918: 2b 00 80 54  	bnezl	$4, 0x18f9c8 <.text+0x8f9c8>
  18f91c: 00 00 64 8e  	lw	$4, 0x0($19)
  18f920: 1d 00 40 16  	bnez	$18, 0x18f998 <.text+0x8f998>
  18f924: 2d 10 40 02  	move	$2, $18
  18f928: a0 00 a6 8f  	lw	$6, 0xa0($sp)
  18f92c: 13 00 c0 10  	beqz	$6, 0x18f97c <.text+0x8f97c>
  18f930: a0 00 a5 8f  	lw	$5, 0xa0($sp)
  18f934: 2d 10 a0 03  	move	$2, $sp
  18f938: 80 00 a3 27  	addiu	$3, $sp, 0x80
  18f93c: 00 00 47 dc  	ld	$7, 0x0($2)
  18f940: 08 00 48 dc  	ld	$8, 0x8($2)
  18f944: 10 00 44 dc  	ld	$4, 0x10($2)
  18f948: 18 00 46 dc  	ld	$6, 0x18($2)
  18f94c: 00 00 a7 fc  	sd	$7, 0x0($5)
  18f950: 08 00 a8 fc  	sd	$8, 0x8($5)
  18f954: 10 00 a4 fc  	sd	$4, 0x10($5)
  18f958: 18 00 a6 fc  	sd	$6, 0x18($5)
  18f95c: 20 00 42 24  	addiu	$2, $2, 0x20
		...
  18f96c: f3 ff 43 14  	bne	$2, $3, 0x18f93c <.text+0x8f93c>
  18f970: 20 00 a5 24  	addiu	$5, $5, 0x20
  18f974: 00 00 47 dc  	ld	$7, 0x0($2)
  18f978: 00 00 a7 fc  	sd	$7, 0x0($5)
  18f97c: 06 00 40 16  	bnez	$18, 0x18f998 <.text+0x8f998>
  18f980: 2d 10 40 02  	move	$2, $18
  18f984: a4 00 a6 8f  	lw	$6, 0xa4($sp)
  18f988: 02 00 c0 10  	beqz	$6, 0x18f994 <.text+0x8f994>
  18f98c: 98 00 a2 df  	ld	$2, 0x98($sp)
  18f990: 00 00 c2 fc  	sd	$2, 0x0($6)
  18f994: 2d 10 40 02  	move	$2, $18
  18f998: 40 01 bf df  	ld	$ra, 0x140($sp)
  18f99c: 30 01 be df  	ld	$fp, 0x130($sp)
  18f9a0: 20 01 b7 df  	ld	$23, 0x120($sp)
  18f9a4: 10 01 b6 df  	ld	$22, 0x110($sp)
  18f9a8: 00 01 b5 df  	ld	$21, 0x100($sp)
  18f9ac: f0 00 b4 df  	ld	$20, 0xf0($sp)
  18f9b0: e0 00 b3 df  	ld	$19, 0xe0($sp)
  18f9b4: d0 00 b2 df  	ld	$18, 0xd0($sp)
  18f9b8: c0 00 b1 df  	ld	$17, 0xc0($sp)
  18f9bc: b0 00 b0 df  	ld	$16, 0xb0($sp)
  18f9c0: 08 00 e0 03  	jr	$ra
  18f9c4: 50 01 bd 27  	addiu	$sp, $sp, 0x150
  18f9c8: 3c 30 10 00  	dsll32	$6, $16, 0x0
  18f9cc: 3f 30 06 00  	dsra32	$6, $6, 0x0
  18f9d0: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18f9d4: 2d 28 c0 03  	move	$5, $fp
  18f9d8: 26 18 50 00  	xor	$3, $2, $16
  18f9dc: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18f9e0: cf ff 00 10  	b	0x18f920 <.text+0x8f920>
  18f9e4: 0b 90 43 00  	movn	$18, $2, $3
  18f9e8: 3c 28 11 00  	dsll32	$5, $17, 0x0
  18f9ec: 3f 28 05 00  	dsra32	$5, $5, 0x0
  18f9f0: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  18f9f4: 01 00 06 24  	addiu	$6, $zero, 0x1
  18f9f8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  18f9fc: 00 00 42 28  	slti	$2, $2, 0x0
  18fa00: c2 ff 00 10  	b	0x18f90c <.text+0x8f90c>
  18fa04: 0b 90 62 00  	movn	$18, $3, $2
  18fa08: 3c 30 10 00  	dsll32	$6, $16, 0x0
  18fa0c: 3f 30 06 00  	dsra32	$6, $6, 0x0
  18fa10: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18fa14: a8 00 a5 8f  	lw	$5, 0xa8($sp)
  18fa18: 26 18 50 00  	xor	$3, $2, $16
  18fa1c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18fa20: aa ff 00 10  	b	0x18f8cc <.text+0x8f8cc>
  18fa24: 0b 90 43 00  	movn	$18, $2, $3
  18fa28: 00 00 64 8e  	lw	$4, 0x0($19)
  18fa2c: 3c 28 11 00  	dsll32	$5, $17, 0x0
  18fa30: 3f 28 05 00  	dsra32	$5, $5, 0x0
  18fa34: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  18fa38: 01 00 06 24  	addiu	$6, $zero, 0x1
  18fa3c: 9e ff 42 04  	bltzl	$2, 0x18f8b8 <.text+0x8f8b8>
  18fa40: ff ff 12 24  	addiu	$18, $zero, -0x1 <.text+0xffffffffffefffff>
  18fa44: 9c ff 00 10  	b	0x18f8b8 <.text+0x8f8b8>
  18fa48: 2d 88 00 00  	move	$17, $zero
  18fa4c: a1 ff 00 10  	b	0x18f8d4 <.text+0x8f8d4>
  18fa50: 48 00 a2 df  	ld	$2, 0x48($sp)
  18fa54: 3c 30 10 00  	dsll32	$6, $16, 0x0
  18fa58: 3f 30 06 00  	dsra32	$6, $6, 0x0
  18fa5c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18fa60: 2d 28 c0 02  	move	$5, $22
  18fa64: 26 18 50 00  	xor	$3, $2, $16
  18fa68: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18fa6c: 88 ff 00 10  	b	0x18f890 <.text+0x8f890>
  18fa70: 0b 90 43 00  	movn	$18, $2, $3
  18fa74: 90 00 a5 27  	addiu	$5, $sp, 0x90
  18fa78: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18fa7c: ff ff 12 24  	addiu	$18, $zero, -0x1 <.text+0xffffffffffefffff>
  18fa80: 35 ff 40 54  	bnezl	$2, 0x18f758 <.text+0x8f758>
  18fa84: 00 00 64 8e  	lw	$4, 0x0($19)
  18fa88: 90 00 a2 df  	ld	$2, 0x90($sp)
  18fa8c: 01 02 03 3c  	lui	$3, 0x201
  18fa90: 50 4b 63 34  	ori	$3, $3, 0x4b50
  18fa94: 99 ff 12 24  	addiu	$18, $zero, -0x67 <.text+0xffffffffffefff99>
  18fa98: 26 10 43 00  	xor	$2, $2, $3
  18fa9c: 2d ff 00 10  	b	0x18f754 <.text+0x8f754>
  18faa0: 0a 90 02 00  	movz	$18, $zero, $2
  18faa4: 2d 18 00 01  	move	$3, $8
  18faa8: 2d 60 20 01  	move	$12, $9
  18faac: 2d 68 40 01  	move	$13, $10
  18fab0: 2d 10 e0 00  	move	$2, $7
  18fab4: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  18fab8: 2d 70 60 01  	move	$14, $11
  18fabc: 2d 38 c0 00  	move	$7, $6
  18fac0: 2d 40 40 00  	move	$8, $2
  18fac4: 2d 48 60 00  	move	$9, $3
  18fac8: 2d 50 80 01  	move	$10, $12
  18facc: 2d 58 a0 01  	move	$11, $13
  18fad0: 2d 30 00 00  	move	$6, $zero
  18fad4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  18fad8: b3 3d 06 0c  	jal	0x18f6cc <.text+0x8f6cc>
  18fadc: 00 00 ae ff  	sd	$14, 0x0($sp)
  18fae0: 10 00 bf df  	ld	$ra, 0x10($sp)
  18fae4: 08 00 e0 03  	jr	$ra
  18fae8: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  18faec: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  18faf0: 9a ff 02 24  	addiu	$2, $zero, -0x66 <.text+0xffffffffffefff9a>
  18faf4: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18faf8: 2d 80 80 00  	move	$16, $4
  18fafc: 11 00 80 10  	beqz	$4, 0x18fb44 <.text+0x8fb44>
  18fb00: 20 00 bf ff  	sd	$ra, 0x20($sp)
  18fb04: 48 00 82 dc  	ld	$2, 0x48($4)
  18fb08: 50 00 85 24  	addiu	$5, $4, 0x50
  18fb0c: 20 00 80 fc  	sd	$zero, 0x20($4)
  18fb10: d8 00 86 24  	addiu	$6, $4, 0xd8
  18fb14: 28 00 82 fc  	sd	$2, 0x28($4)
  18fb18: 2d 38 00 00  	move	$7, $zero
  18fb1c: 2d 40 00 00  	move	$8, $zero
  18fb20: 2d 48 00 00  	move	$9, $zero
  18fb24: 2d 50 00 00  	move	$10, $zero
  18fb28: 2d 58 00 00  	move	$11, $zero
  18fb2c: b3 3d 06 0c  	jal	0x18f6cc <.text+0x8f6cc>
  18fb30: 00 00 a0 ff  	sd	$zero, 0x0($sp)
  18fb34: 01 00 43 2c  	sltiu	$3, $2, 0x1
  18fb38: 3c 18 03 00  	dsll32	$3, $3, 0x0
  18fb3c: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  18fb40: 30 00 03 fe  	sd	$3, 0x30($16)
  18fb44: 20 00 bf df  	ld	$ra, 0x20($sp)
  18fb48: 10 00 b0 df  	ld	$16, 0x10($sp)
  18fb4c: 08 00 e0 03  	jr	$ra
  18fb50: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  18fb54: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  18fb58: 9a ff 05 24  	addiu	$5, $zero, -0x66 <.text+0xffffffffffefff9a>
  18fb5c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18fb60: 2d 80 80 00  	move	$16, $4
  18fb64: 20 00 80 10  	beqz	$4, 0x18fbe8 <.text+0x8fbe8>
  18fb68: 20 00 bf ff  	sd	$ra, 0x20($sp)
  18fb6c: 30 00 82 dc  	ld	$2, 0x30($4)
  18fb70: 1d 00 40 10  	beqz	$2, 0x18fbe8 <.text+0x8fbe8>
  18fb74: 9c ff 05 24  	addiu	$5, $zero, -0x64 <.text+0xffffffffffefff9c>
  18fb78: 20 00 83 dc  	ld	$3, 0x20($4)
  18fb7c: 08 00 82 dc  	ld	$2, 0x8($4)
  18fb80: 01 00 68 64  	daddiu	$8, $3, 0x1
  18fb84: 18 00 02 11  	beq	$8, $2, 0x18fbe8 <.text+0x8fbe8>
  18fb88: d8 00 86 24  	addiu	$6, $4, 0xd8
  18fb8c: 98 00 82 dc  	ld	$2, 0x98($4)
  18fb90: 50 00 85 24  	addiu	$5, $4, 0x50
  18fb94: 90 00 83 dc  	ld	$3, 0x90($4)
  18fb98: a0 00 87 dc  	ld	$7, 0xa0($4)
  18fb9c: 2d 48 00 00  	move	$9, $zero
  18fba0: 2d 18 62 00  	daddu	$3, $3, $2
  18fba4: 20 00 88 fc  	sd	$8, 0x20($4)
  18fba8: 28 00 82 dc  	ld	$2, 0x28($4)
  18fbac: 2d 18 67 00  	daddu	$3, $3, $7
  18fbb0: 2d 40 00 00  	move	$8, $zero
  18fbb4: 2d 50 00 00  	move	$10, $zero
  18fbb8: 2d 10 43 00  	daddu	$2, $2, $3
  18fbbc: 2d 38 00 00  	move	$7, $zero
  18fbc0: 2e 00 42 64  	daddiu	$2, $2, 0x2e
  18fbc4: 2d 58 00 00  	move	$11, $zero
  18fbc8: 28 00 82 fc  	sd	$2, 0x28($4)
  18fbcc: b3 3d 06 0c  	jal	0x18f6cc <.text+0x8f6cc>
  18fbd0: 00 00 a0 ff  	sd	$zero, 0x0($sp)
  18fbd4: 01 00 43 2c  	sltiu	$3, $2, 0x1
  18fbd8: 2d 28 40 00  	move	$5, $2
  18fbdc: 3c 18 03 00  	dsll32	$3, $3, 0x0
  18fbe0: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  18fbe4: 30 00 03 fe  	sd	$3, 0x30($16)
  18fbe8: 20 00 bf df  	ld	$ra, 0x20($sp)
  18fbec: 2d 10 a0 00  	move	$2, $5
  18fbf0: 10 00 b0 df  	ld	$16, 0x10($sp)
  18fbf4: 08 00 e0 03  	jr	$ra
  18fbf8: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  18fbfc: 90 fe bd 27  	addiu	$sp, $sp, -0x170 <.text+0xffffffffffeffe90>
  18fc00: 9a ff 03 24  	addiu	$3, $zero, -0x66 <.text+0xffffffffffefff9a>
  18fc04: 50 01 b4 ff  	sd	$20, 0x150($sp)
  18fc08: 2d a0 c0 00  	move	$20, $6
  18fc0c: 40 01 b3 ff  	sd	$19, 0x140($sp)
  18fc10: 2d 98 a0 00  	move	$19, $5
  18fc14: 10 01 b0 ff  	sd	$16, 0x110($sp)
  18fc18: 2d 80 80 00  	move	$16, $4
  18fc1c: 60 01 bf ff  	sd	$ra, 0x160($sp)
  18fc20: 30 01 b2 ff  	sd	$18, 0x130($sp)
  18fc24: 09 00 80 10  	beqz	$4, 0x18fc4c <.text+0x8fc4c>
  18fc28: 20 01 b1 ff  	sd	$17, 0x120($sp)
  18fc2c: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  18fc30: 2d 20 a0 00  	move	$4, $5
  18fc34: 00 01 42 2c  	sltiu	$2, $2, 0x100
  18fc38: 04 00 40 10  	beqz	$2, 0x18fc4c <.text+0x8fc4c>
  18fc3c: 9a ff 03 24  	addiu	$3, $zero, -0x66 <.text+0xffffffffffefff9a>
  18fc40: 30 00 02 de  	ld	$2, 0x30($16)
  18fc44: 0a 00 40 14  	bnez	$2, 0x18fc70 <.text+0x8fc70>
  18fc48: 9c ff 03 24  	addiu	$3, $zero, -0x64 <.text+0xffffffffffefff9c>
  18fc4c: 60 01 bf df  	ld	$ra, 0x160($sp)
  18fc50: 2d 10 60 00  	move	$2, $3
  18fc54: 50 01 b4 df  	ld	$20, 0x150($sp)
  18fc58: 40 01 b3 df  	ld	$19, 0x140($sp)
  18fc5c: 30 01 b2 df  	ld	$18, 0x130($sp)
  18fc60: 20 01 b1 df  	ld	$17, 0x120($sp)
  18fc64: 10 01 b0 df  	ld	$16, 0x110($sp)
  18fc68: 08 00 e0 03  	jr	$ra
  18fc6c: 70 01 bd 27  	addiu	$sp, $sp, 0x170
  18fc70: 2d 20 00 02  	move	$4, $16
  18fc74: 20 00 12 de  	ld	$18, 0x20($16)
  18fc78: bb 3e 06 0c  	jal	0x18faec <.text+0x8faec>
  18fc7c: 28 00 11 de  	ld	$17, 0x28($16)
  18fc80: 05 00 40 10  	beqz	$2, 0x18fc98 <.text+0x8fc98>
  18fc84: 00 01 07 24  	addiu	$7, $zero, 0x100
  18fc88: 28 00 11 fe  	sd	$17, 0x28($16)
  18fc8c: 2d 18 40 00  	move	$3, $2
  18fc90: ee ff 00 10  	b	0x18fc4c <.text+0x8fc4c>
  18fc94: 20 00 12 fe  	sd	$18, 0x20($16)
  18fc98: 2d 40 00 00  	move	$8, $zero
  18fc9c: 2d 48 00 00  	move	$9, $zero
  18fca0: 2d 50 00 00  	move	$10, $zero
  18fca4: 2d 58 00 00  	move	$11, $zero
  18fca8: 2d 20 00 02  	move	$4, $16
  18fcac: 2d 28 00 00  	move	$5, $zero
  18fcb0: a9 3e 06 0c  	jal	0x18faa4 <.text+0x8faa4>
  18fcb4: 2d 30 a0 03  	move	$6, $sp
  18fcb8: 2d 30 80 02  	move	$6, $20
  18fcbc: 2d 20 a0 03  	move	$4, $sp
  18fcc0: 90 3c 06 0c  	jal	0x18f240 <.text+0x8f240>
  18fcc4: 2d 28 60 02  	move	$5, $19
  18fcc8: 2d 18 00 00  	move	$3, $zero
  18fccc: df ff 40 10  	beqz	$2, 0x18fc4c <.text+0x8fc4c>
  18fcd0: 2d 20 00 02  	move	$4, $16
  18fcd4: d5 3e 06 0c  	jal	0x18fb54 <.text+0x8fb54>
  18fcd8: 00 00 00 00  	nop
  18fcdc: ee ff 40 10  	beqz	$2, 0x18fc98 <.text+0x8fc98>
  18fce0: 00 01 07 24  	addiu	$7, $zero, 0x100
  18fce4: e9 ff 00 10  	b	0x18fc8c <.text+0x8fc8c>
  18fce8: 28 00 11 fe  	sd	$17, 0x28($16)
  18fcec: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  18fcf0: 00 00 c0 fc  	sd	$zero, 0x0($6)
  18fcf4: 80 00 b5 ff  	sd	$21, 0x80($sp)
  18fcf8: 2d a8 c0 00  	move	$21, $6
  18fcfc: 70 00 b4 ff  	sd	$20, 0x70($sp)
  18fd00: 2d 30 00 00  	move	$6, $zero
  18fd04: 60 00 b3 ff  	sd	$19, 0x60($sp)
  18fd08: 2d a0 e0 00  	move	$20, $7
  18fd0c: 50 00 b2 ff  	sd	$18, 0x50($sp)
  18fd10: 2d 98 a0 00  	move	$19, $5
  18fd14: 90 00 bf ff  	sd	$ra, 0x90($sp)
  18fd18: 2d 90 80 00  	move	$18, $4
  18fd1c: 40 00 b1 ff  	sd	$17, 0x40($sp)
  18fd20: 30 00 b0 ff  	sd	$16, 0x30($sp)
  18fd24: 00 00 a0 ac  	sw	$zero, 0x0($5)
  18fd28: 18 00 82 dc  	ld	$2, 0x18($4)
  18fd2c: d8 00 85 dc  	ld	$5, 0xd8($4)
  18fd30: 00 00 e0 ac  	sw	$zero, 0x0($7)
  18fd34: 2d 28 a2 00  	daddu	$5, $5, $2
  18fd38: 3c 28 05 00  	dsll32	$5, $5, 0x0
  18fd3c: 3f 28 05 00  	dsra32	$5, $5, 0x0
  18fd40: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  18fd44: 00 00 84 8c  	lw	$4, 0x0($4)
  18fd48: 50 00 40 04  	bltz	$2, 0x18fe8c <.text+0x8fe8c>
  18fd4c: ff ff 06 24  	addiu	$6, $zero, -0x1 <.text+0xffffffffffefffff>
  18fd50: 00 00 44 8e  	lw	$4, 0x0($18)
  18fd54: 2d 28 a0 03  	move	$5, $sp
  18fd58: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18fd5c: ff ff 11 24  	addiu	$17, $zero, -0x1 <.text+0xffffffffffefffff>
  18fd60: 07 00 40 14  	bnez	$2, 0x18fd80 <.text+0x8fd80>
  18fd64: 00 00 44 8e  	lw	$4, 0x0($18)
  18fd68: 00 00 a2 df  	ld	$2, 0x0($sp)
  18fd6c: 03 04 03 3c  	lui	$3, 0x403
  18fd70: 50 4b 63 34  	ori	$3, $3, 0x4b50
  18fd74: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18fd78: 26 10 43 00  	xor	$2, $2, $3
  18fd7c: 0a 88 02 00  	movz	$17, $zero, $2
  18fd80: 08 00 a5 27  	addiu	$5, $sp, 0x8
  18fd84: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18fd88: ff ff 10 24  	addiu	$16, $zero, -0x1 <.text+0xffffffffffefffff>
  18fd8c: 00 00 44 8e  	lw	$4, 0x0($18)
  18fd90: 10 00 a5 27  	addiu	$5, $sp, 0x10
  18fd94: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18fd98: 0b 88 02 02  	movn	$17, $16, $2
  18fd9c: 00 00 44 8e  	lw	$4, 0x0($18)
  18fda0: 08 00 a5 27  	addiu	$5, $sp, 0x8
  18fda4: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18fda8: 0b 88 02 02  	movn	$17, $16, $2
  18fdac: 67 00 40 10  	beqz	$2, 0x18ff4c <.text+0x8ff4c>
  18fdb0: 00 00 00 00  	nop
  18fdb4: ff ff 11 24  	addiu	$17, $zero, -0x1 <.text+0xffffffffffefffff>
  18fdb8: 05 00 20 16  	bnez	$17, 0x18fdd0 <.text+0x8fdd0>
  18fdbc: 00 00 44 8e  	lw	$4, 0x0($18)
  18fdc0: 68 00 42 de  	ld	$2, 0x68($18)
  18fdc4: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18fdc8: 09 00 42 2c  	sltiu	$2, $2, 0x9
  18fdcc: 0b 88 02 00  	movn	$17, $zero, $2
  18fdd0: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18fdd4: 08 00 a5 27  	addiu	$5, $sp, 0x8
  18fdd8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  18fddc: 00 00 44 8e  	lw	$4, 0x0($18)
  18fde0: 08 00 a5 27  	addiu	$5, $sp, 0x8
  18fde4: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18fde8: 0b 88 62 00  	movn	$17, $3, $2
  18fdec: 4d 00 40 10  	beqz	$2, 0x18ff24 <.text+0x8ff24>
  18fdf0: 00 00 00 00  	nop
  18fdf4: ff ff 11 24  	addiu	$17, $zero, -0x1 <.text+0xffffffffffefffff>
  18fdf8: 00 00 44 8e  	lw	$4, 0x0($18)
  18fdfc: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18fe00: 08 00 a5 27  	addiu	$5, $sp, 0x8
  18fe04: 3d 00 40 10  	beqz	$2, 0x18fefc <.text+0x8fefc>
  18fe08: 00 00 00 00  	nop
  18fe0c: ff ff 11 24  	addiu	$17, $zero, -0x1 <.text+0xffffffffffefffff>
  18fe10: 00 00 44 8e  	lw	$4, 0x0($18)
  18fe14: 3b 3c 06 0c  	jal	0x18f0ec <.text+0x8f0ec>
  18fe18: 08 00 a5 27  	addiu	$5, $sp, 0x8
  18fe1c: 2d 00 40 10  	beqz	$2, 0x18fed4 <.text+0x8fed4>
  18fe20: 00 00 00 00  	nop
  18fe24: ff ff 11 24  	addiu	$17, $zero, -0x1 <.text+0xffffffffffefffff>
  18fe28: 00 00 44 8e  	lw	$4, 0x0($18)
  18fe2c: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18fe30: 18 00 a5 27  	addiu	$5, $sp, 0x18
  18fe34: 1f 00 40 10  	beqz	$2, 0x18feb4 <.text+0x8feb4>
  18fe38: 00 00 00 00  	nop
  18fe3c: ff ff 11 24  	addiu	$17, $zero, -0x1 <.text+0xffffffffffefffff>
  18fe40: 00 00 62 8e  	lw	$2, 0x0($19)
  18fe44: 20 00 a5 27  	addiu	$5, $sp, 0x20
  18fe48: 18 00 a3 8f  	lw	$3, 0x18($sp)
  18fe4c: 21 10 43 00  	addu	$2, $2, $3
  18fe50: 00 00 62 ae  	sw	$2, 0x0($19)
  18fe54: 1c 3c 06 0c  	jal	0x18f070 <.text+0x8f070>
  18fe58: 00 00 44 8e  	lw	$4, 0x0($18)
  18fe5c: ff ff 06 24  	addiu	$6, $zero, -0x1 <.text+0xffffffffffefffff>
  18fe60: 20 00 a5 8f  	lw	$5, 0x20($sp)
  18fe64: 0a 30 22 02  	movz	$6, $17, $2
  18fe68: d8 00 43 de  	ld	$3, 0xd8($18)
  18fe6c: 00 00 85 ae  	sw	$5, 0x0($20)
  18fe70: 18 00 a2 df  	ld	$2, 0x18($sp)
  18fe74: 00 00 64 8e  	lw	$4, 0x0($19)
  18fe78: 2d 18 62 00  	daddu	$3, $3, $2
  18fe7c: 21 20 85 00  	addu	$4, $4, $5
  18fe80: 1e 00 63 64  	daddiu	$3, $3, 0x1e
  18fe84: 00 00 a3 fe  	sd	$3, 0x0($21)
  18fe88: 00 00 64 ae  	sw	$4, 0x0($19)
  18fe8c: 90 00 bf df  	ld	$ra, 0x90($sp)
  18fe90: 2d 10 c0 00  	move	$2, $6
  18fe94: 80 00 b5 df  	ld	$21, 0x80($sp)
  18fe98: 70 00 b4 df  	ld	$20, 0x70($sp)
  18fe9c: 60 00 b3 df  	ld	$19, 0x60($sp)
  18fea0: 50 00 b2 df  	ld	$18, 0x50($sp)
  18fea4: 40 00 b1 df  	ld	$17, 0x40($sp)
  18fea8: 30 00 b0 df  	ld	$16, 0x30($sp)
  18feac: 08 00 e0 03  	jr	$ra
  18feb0: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  18feb4: e3 ff 20 56  	bnezl	$17, 0x18fe44 <.text+0x8fe44>
  18feb8: 00 00 62 8e  	lw	$2, 0x0($19)
  18febc: 90 00 43 de  	ld	$3, 0x90($18)
  18fec0: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18fec4: 18 00 a2 df  	ld	$2, 0x18($sp)
  18fec8: 26 10 43 00  	xor	$2, $2, $3
  18fecc: dc ff 00 10  	b	0x18fe40 <.text+0x8fe40>
  18fed0: 0a 88 02 00  	movz	$17, $zero, $2
  18fed4: d5 ff 20 56  	bnezl	$17, 0x18fe2c <.text+0x8fe2c>
  18fed8: 00 00 44 8e  	lw	$4, 0x0($18)
  18fedc: 88 00 43 de  	ld	$3, 0x88($18)
  18fee0: 08 00 a2 df  	ld	$2, 0x8($sp)
  18fee4: d0 ff 43 10  	beq	$2, $3, 0x18fe28 <.text+0x8fe28>
  18fee8: 10 00 a2 df  	ld	$2, 0x10($sp)
  18feec: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18fef0: 08 00 42 30  	andi	$2, $2, 0x8
  18fef4: cc ff 00 10  	b	0x18fe28 <.text+0x8fe28>
  18fef8: 0b 88 02 00  	movn	$17, $zero, $2
  18fefc: c5 ff 20 56  	bnezl	$17, 0x18fe14 <.text+0x8fe14>
  18ff00: 00 00 44 8e  	lw	$4, 0x0($18)
  18ff04: 80 00 43 de  	ld	$3, 0x80($18)
  18ff08: 08 00 a2 df  	ld	$2, 0x8($sp)
  18ff0c: c0 ff 43 10  	beq	$2, $3, 0x18fe10 <.text+0x8fe10>
  18ff10: 10 00 a2 df  	ld	$2, 0x10($sp)
  18ff14: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18ff18: 08 00 42 30  	andi	$2, $2, 0x8
  18ff1c: bc ff 00 10  	b	0x18fe10 <.text+0x8fe10>
  18ff20: 0b 88 02 00  	movn	$17, $zero, $2
  18ff24: b5 ff 20 56  	bnezl	$17, 0x18fdfc <.text+0x8fdfc>
  18ff28: 00 00 44 8e  	lw	$4, 0x0($18)
  18ff2c: 78 00 43 de  	ld	$3, 0x78($18)
  18ff30: 08 00 a2 df  	ld	$2, 0x8($sp)
  18ff34: b0 ff 43 10  	beq	$2, $3, 0x18fdf8 <.text+0x8fdf8>
  18ff38: 10 00 a2 df  	ld	$2, 0x10($sp)
  18ff3c: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18ff40: 08 00 42 30  	andi	$2, $2, 0x8
  18ff44: ac ff 00 10  	b	0x18fdf8 <.text+0x8fdf8>
  18ff48: 0b 88 02 00  	movn	$17, $zero, $2
  18ff4c: a0 ff 20 56  	bnezl	$17, 0x18fdd0 <.text+0x8fdd0>
  18ff50: 00 00 44 8e  	lw	$4, 0x0($18)
  18ff54: 68 00 43 de  	ld	$3, 0x68($18)
  18ff58: 99 ff 11 24  	addiu	$17, $zero, -0x67 <.text+0xffffffffffefff99>
  18ff5c: 08 00 a2 df  	ld	$2, 0x8($sp)
  18ff60: 26 10 43 00  	xor	$2, $2, $3
  18ff64: 94 ff 00 10  	b	0x18fdb8 <.text+0x8fdb8>
  18ff68: 0a 88 02 00  	movz	$17, $zero, $2
  18ff6c: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  18ff70: 9a ff 05 24  	addiu	$5, $zero, -0x66 <.text+0xffffffffffefff9a>
  18ff74: 30 00 b1 ff  	sd	$17, 0x30($sp)
  18ff78: 2d 88 80 00  	move	$17, $4
  18ff7c: 40 00 bf ff  	sd	$ra, 0x40($sp)
  18ff80: 0e 00 80 10  	beqz	$4, 0x18ffbc <.text+0x8ffbc>
  18ff84: 20 00 b0 ff  	sd	$16, 0x20($sp)
  18ff88: 30 00 82 dc  	ld	$2, 0x30($4)
  18ff8c: 0c 00 40 10  	beqz	$2, 0x18ffc0 <.text+0x8ffc0>
  18ff90: 40 00 bf df  	ld	$ra, 0x40($sp)
  18ff94: e0 00 82 8c  	lw	$2, 0xe0($4)
  18ff98: 4a 00 40 14  	bnez	$2, 0x1900c4 <.text+0x900c4>
  18ff9c: 00 00 00 00  	nop
  18ffa0: 2d 28 a0 03  	move	$5, $sp
  18ffa4: 2d 20 20 02  	move	$4, $17
  18ffa8: 08 00 a6 27  	addiu	$6, $sp, 0x8
  18ffac: 3b 3f 06 0c  	jal	0x18fcec <.text+0x8fcec>
  18ffb0: 10 00 a7 27  	addiu	$7, $sp, 0x10
  18ffb4: 07 00 40 10  	beqz	$2, 0x18ffd4 <.text+0x8ffd4>
  18ffb8: 99 ff 05 24  	addiu	$5, $zero, -0x67 <.text+0xffffffffffefff99>
  18ffbc: 40 00 bf df  	ld	$ra, 0x40($sp)
  18ffc0: 2d 10 a0 00  	move	$2, $5
  18ffc4: 30 00 b1 df  	ld	$17, 0x30($sp)
  18ffc8: 20 00 b0 df  	ld	$16, 0x20($sp)
  18ffcc: 08 00 e0 03  	jr	$ra
  18ffd0: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  18ffd4: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  18ffd8: b0 00 04 24  	addiu	$4, $zero, 0xb0
  18ffdc: 98 ff 05 24  	addiu	$5, $zero, -0x68 <.text+0xffffffffffefff98>
  18ffe0: f6 ff 40 10  	beqz	$2, 0x18ffbc <.text+0x8ffbc>
  18ffe4: 2d 80 40 00  	move	$16, $2
  18ffe8: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  18ffec: 00 40 04 24  	addiu	$4, $zero, 0x4000
  18fff0: 08 00 a3 df  	ld	$3, 0x8($sp)
  18fff4: 70 00 00 fe  	sd	$zero, 0x70($16)
  18fff8: 60 00 03 fe  	sd	$3, 0x60($16)
  18fffc: 10 00 a3 8f  	lw	$3, 0x10($sp)
  190000: 00 00 02 ae  	sw	$2, 0x0($16)
  190004: 2b 00 40 10  	beqz	$2, 0x1900b4 <.text+0x900b4>
  190008: 68 00 03 ae  	sw	$3, 0x68($16)
  19000c: 68 00 26 de  	ld	$6, 0x68($17)
  190010: 78 00 23 de  	ld	$3, 0x78($17)
  190014: 18 00 22 de  	ld	$2, 0x18($17)
  190018: 07 00 c5 2c  	sltiu	$5, $6, 0x7
  19001c: 00 00 24 8e  	lw	$4, 0x0($17)
  190020: 80 00 03 fe  	sd	$3, 0x80($16)
  190024: 98 00 04 ae  	sw	$4, 0x98($16)
  190028: a8 00 02 fe  	sd	$2, 0xa8($16)
  19002c: 58 00 00 fe  	sd	$zero, 0x58($16)
  190030: 78 00 00 fe  	sd	$zero, 0x78($16)
  190034: a0 00 06 fe  	sd	$6, 0xa0($16)
  190038: 11 00 a0 14  	bnez	$5, 0x190080 <.text+0x90080>
  19003c: 20 00 00 fe  	sd	$zero, 0x20($16)
  190040: 08 00 02 24  	addiu	$2, $zero, 0x8
  190044: dd ff c2 14  	bne	$6, $2, 0x18ffbc <.text+0x8ffbc>
  190048: 98 ff 05 24  	addiu	$5, $zero, -0x68 <.text+0xffffffffffefff98>
  19004c: 1c 00 06 3c  	lui	$6, 0x1c
  190050: 08 00 04 26  	addiu	$4, $16, 0x8
  190054: d0 88 c6 24  	addiu	$6, $6, -0x7730 <.text+0xffffffffffef88d0>
  190058: f1 ff 05 24  	addiu	$5, $zero, -0xf <.text+0xffffffffffeffff1>
  19005c: 48 00 07 24  	addiu	$7, $zero, 0x48
  190060: 30 00 00 ae  	sw	$zero, 0x30($16)
  190064: 34 00 00 ae  	sw	$zero, 0x34($16)
  190068: 00 4a 06 0c  	jal	0x192800 <.text+0x92800>
  19006c: 38 00 00 ae  	sw	$zero, 0x38($16)
  190070: 04 00 40 54  	bnezl	$2, 0x190084 <.text+0x90084>
  190074: d8 00 22 de  	ld	$2, 0xd8($17)
  190078: 01 00 02 24  	addiu	$2, $zero, 0x1
  19007c: 58 00 02 fe  	sd	$2, 0x58($16)
  190080: d8 00 22 de  	ld	$2, 0xd8($17)
  190084: 2d 28 00 00  	move	$5, $zero
  190088: 00 00 a3 9f  	lwu	$3, 0x0($sp)
  19008c: 80 00 24 de  	ld	$4, 0x80($17)
  190090: 2d 10 43 00  	daddu	$2, $2, $3
  190094: e0 00 30 ae  	sw	$16, 0xe0($17)
  190098: 88 00 23 de  	ld	$3, 0x88($17)
  19009c: 1e 00 42 64  	daddiu	$2, $2, 0x1e
  1900a0: 88 00 04 fe  	sd	$4, 0x88($16)
  1900a4: 90 00 03 fe  	sd	$3, 0x90($16)
  1900a8: 50 00 02 fe  	sd	$2, 0x50($16)
  1900ac: c3 ff 00 10  	b	0x18ffbc <.text+0x8ffbc>
  1900b0: 0c 00 00 ae  	sw	$zero, 0xc($16)
  1900b4: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1900b8: 2d 20 00 02  	move	$4, $16
  1900bc: bf ff 00 10  	b	0x18ffbc <.text+0x8ffbc>
  1900c0: 98 ff 05 24  	addiu	$5, $zero, -0x68 <.text+0xffffffffffefff98>
  1900c4: 5e 41 06 0c  	jal	0x190578 <.text+0x90578>
  1900c8: 00 00 00 00  	nop
  1900cc: b5 ff 00 10  	b	0x18ffa4 <.text+0x8ffa4>
  1900d0: 2d 28 a0 03  	move	$5, $sp
  1900d4: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  1900d8: 9a ff 08 24  	addiu	$8, $zero, -0x66 <.text+0xffffffffffefff9a>
  1900dc: 50 00 b5 ff  	sd	$21, 0x50($sp)
  1900e0: 2d a8 00 00  	move	$21, $zero
  1900e4: 40 00 b4 ff  	sd	$20, 0x40($sp)
  1900e8: 2d a0 00 00  	move	$20, $zero
  1900ec: 60 00 bf ff  	sd	$ra, 0x60($sp)
  1900f0: 30 00 b3 ff  	sd	$19, 0x30($sp)
  1900f4: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1900f8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1900fc: 68 00 80 10  	beqz	$4, 0x1902a0 <.text+0x902a0>
  190100: 00 00 b0 ff  	sd	$16, 0x0($sp)
  190104: e0 00 87 8c  	lw	$7, 0xe0($4)
  190108: 42 00 09 3c  	lui	$9, 0x42
  19010c: 42 00 02 3c  	lui	$2, 0x42
  190110: 9a ff 08 24  	addiu	$8, $zero, -0x66 <.text+0xffffffffffefff9a>
  190114: 54 48 44 ac  	sw	$4, 0x4854($2)
  190118: 61 00 e0 10  	beqz	$7, 0x1902a0 <.text+0x902a0>
  19011c: 50 48 27 ad  	sw	$7, 0x4850($9)
  190120: 00 00 e2 8c  	lw	$2, 0x0($7)
  190124: 5e 00 40 10  	beqz	$2, 0x1902a0 <.text+0x902a0>
  190128: 9c ff 08 24  	addiu	$8, $zero, -0x64 <.text+0xffffffffffefff9c>
  19012c: 5c 00 c0 10  	beqz	$6, 0x1902a0 <.text+0x902a0>
  190130: 2d 40 00 00  	move	$8, $zero
  190134: 3c 10 06 00  	dsll32	$2, $6, 0x0
  190138: 90 00 e3 dc  	ld	$3, 0x90($7)
  19013c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  190140: 18 00 e5 ac  	sw	$5, 0x18($7)
  190144: 2b 18 62 00  	sltu	$3, $3, $2
  190148: 03 00 60 10  	beqz	$3, 0x190158 <.text+0x90158>
  19014c: 1c 00 e6 ac  	sw	$6, 0x1c($7)
  190150: 90 00 e2 8c  	lw	$2, 0x90($7)
  190154: 1c 00 e2 ac  	sw	$2, 0x1c($7)
  190158: 50 48 22 8d  	lw	$2, 0x4850($9)
  19015c: 1c 00 42 8c  	lw	$2, 0x1c($2)
  190160: 59 00 40 10  	beqz	$2, 0x1902c8 <.text+0x902c8>
  190164: 42 00 11 3c  	lui	$17, 0x42
  190168: 50 48 24 8e  	lw	$4, 0x4850($17)
  19016c: a0 00 83 dc  	ld	$3, 0xa0($4)
  190170: 8f 00 60 50  	beqzl	$3, 0x1903b0 <.text+0x903b0>
  190174: 0c 00 82 8c  	lw	$2, 0xc($4)
  190178: 08 00 02 24  	addiu	$2, $zero, 0x8
  19017c: 8c 00 62 50  	beql	$3, $2, 0x1903b0 <.text+0x903b0>
  190180: 0c 00 82 8c  	lw	$2, 0xc($4)
  190184: 42 00 02 3c  	lui	$2, 0x42
  190188: 50 48 43 8c  	lw	$3, 0x4850($2)
  19018c: a0 00 62 dc  	ld	$2, 0xa0($3)
  190190: 09 00 42 2c  	sltiu	$2, $2, 0x9
  190194: 42 00 40 50  	beqzl	$2, 0x1902a0 <.text+0x902a0>
  190198: 2d 40 00 00  	move	$8, $zero
  19019c: a0 00 62 8c  	lw	$2, 0xa0($3)
  1901a0: 1c 00 03 3c  	lui	$3, 0x1c
  1901a4: d8 88 63 24  	addiu	$3, $3, -0x7728 <.text+0xffffffffffef88d8>
  1901a8: 80 10 02 00  	sll	$2, $2, 0x2
  1901ac: 21 10 43 00  	addu	$2, $2, $3
  1901b0: 00 00 42 8c  	lw	$2, 0x0($2)
  1901b4: 08 00 40 00  	jr	$2
  1901b8: 00 00 00 00  	nop
  1901bc: 42 00 02 3c  	lui	$2, 0x42
  1901c0: 50 48 42 8c  	lw	$2, 0x4850($2)
  1901c4: 0c 00 44 8c  	lw	$4, 0xc($2)
  1901c8: 1c 00 43 8c  	lw	$3, 0x1c($2)
  1901cc: 2b 10 64 00  	sltu	$2, $3, $4
  1901d0: 2d 90 60 00  	move	$18, $3
  1901d4: 0a 90 82 00  	movz	$18, $4, $2
  1901d8: 0c 00 40 12  	beqz	$18, 0x19020c <.text+0x9020c>
  1901dc: 2d 28 00 00  	move	$5, $zero
  1901e0: 42 00 02 3c  	lui	$2, 0x42
  1901e4: 50 48 43 8c  	lw	$3, 0x4850($2)
  1901e8: 08 00 62 8c  	lw	$2, 0x8($3)
  1901ec: 18 00 63 8c  	lw	$3, 0x18($3)
  1901f0: 21 10 45 00  	addu	$2, $2, $5
  1901f4: 21 18 65 00  	addu	$3, $3, $5
  1901f8: 00 00 44 90  	lbu	$4, 0x0($2)
  1901fc: 01 00 a5 24  	addiu	$5, $5, 0x1
  190200: 2b 10 b2 00  	sltu	$2, $5, $18
  190204: f6 ff 40 14  	bnez	$2, 0x1901e0 <.text+0x901e0>
  190208: 00 00 64 a0  	sb	$4, 0x0($3)
  19020c: 42 00 11 3c  	lui	$17, 0x42
  190210: 2d 30 40 02  	move	$6, $18
  190214: 50 48 30 8e  	lw	$16, 0x4850($17)
  190218: 21 a0 92 02  	addu	$20, $20, $18
  19021c: 78 00 04 de  	ld	$4, 0x78($16)
  190220: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  190224: 18 00 05 8e  	lw	$5, 0x18($16)
  190228: 50 48 23 8e  	lw	$3, 0x4850($17)
  19022c: 3c 40 12 00  	dsll32	$8, $18, 0x0
  190230: 78 00 02 fe  	sd	$2, 0x78($16)
  190234: 90 00 64 dc  	ld	$4, 0x90($3)
  190238: 3e 40 08 00  	dsrl32	$8, $8, 0x0
  19023c: 0c 00 66 8c  	lw	$6, 0xc($3)
  190240: 1c 00 69 8c  	lw	$9, 0x1c($3)
  190244: 2f 20 88 00  	dsubu	$4, $4, $8
  190248: 18 00 67 8c  	lw	$7, 0x18($3)
  19024c: 23 30 d2 00  	subu	$6, $6, $18
  190250: 08 00 65 8c  	lw	$5, 0x8($3)
  190254: 23 48 32 01  	subu	$9, $9, $18
  190258: 20 00 62 dc  	ld	$2, 0x20($3)
  19025c: 21 38 f2 00  	addu	$7, $7, $18
  190260: 21 28 b2 00  	addu	$5, $5, $18
  190264: 90 00 64 fc  	sd	$4, 0x90($3)
  190268: 2d 10 48 00  	daddu	$2, $2, $8
  19026c: 0c 00 66 ac  	sw	$6, 0xc($3)
  190270: 20 00 62 fc  	sd	$2, 0x20($3)
  190274: 1c 00 69 ac  	sw	$9, 0x1c($3)
  190278: 18 00 67 ac  	sw	$7, 0x18($3)
  19027c: 08 00 65 ac  	sw	$5, 0x8($3)
  190280: 42 00 02 3c  	lui	$2, 0x42
  190284: 50 48 42 8c  	lw	$2, 0x4850($2)
  190288: 1c 00 42 8c  	lw	$2, 0x1c($2)
  19028c: 0e 00 40 10  	beqz	$2, 0x1902c8 <.text+0x902c8>
  190290: 00 00 00 00  	nop
  190294: b4 ff a0 12  	beqz	$21, 0x190168 <.text+0x90168>
  190298: 42 00 11 3c  	lui	$17, 0x42
  19029c: 2d 40 a0 02  	move	$8, $21
  1902a0: 60 00 bf df  	ld	$ra, 0x60($sp)
  1902a4: 2d 10 00 01  	move	$2, $8
  1902a8: 50 00 b5 df  	ld	$21, 0x50($sp)
  1902ac: 40 00 b4 df  	ld	$20, 0x40($sp)
  1902b0: 30 00 b3 df  	ld	$19, 0x30($sp)
  1902b4: 20 00 b2 df  	ld	$18, 0x20($sp)
  1902b8: 10 00 b1 df  	ld	$17, 0x10($sp)
  1902bc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1902c0: 08 00 e0 03  	jr	$ra
  1902c4: 70 00 bd 27  	addiu	$sp, $sp, 0x70
  1902c8: f5 ff a0 12  	beqz	$21, 0x1902a0 <.text+0x902a0>
  1902cc: 2d 40 80 02  	move	$8, $20
  1902d0: f3 ff 00 10  	b	0x1902a0 <.text+0x902a0>
  1902d4: 2d 40 a0 02  	move	$8, $21
  1902d8: 42 00 02 3c  	lui	$2, 0x42
  1902dc: 50 48 42 8c  	lw	$2, 0x4850($2)
  1902e0: 90 00 42 dc  	ld	$2, 0x90($2)
  1902e4: 3c a0 02 00  	dsll32	$20, $2, 0x0
  1902e8: 96 3a 06 0c  	jal	0x18ea58 <.text+0x8ea58>
  1902ec: 3f a0 14 00  	dsra32	$20, $20, 0x0
  1902f0: e4 ff 00 10  	b	0x190284 <.text+0x90284>
  1902f4: 42 00 02 3c  	lui	$2, 0x42
  1902f8: 42 00 02 3c  	lui	$2, 0x42
  1902fc: 50 48 42 8c  	lw	$2, 0x4850($2)
  190300: 90 00 42 dc  	ld	$2, 0x90($2)
  190304: 3c a0 02 00  	dsll32	$20, $2, 0x0
  190308: 30 39 06 0c  	jal	0x18e4c0 <.text+0x8e4c0>
  19030c: 3f a0 14 00  	dsra32	$20, $20, 0x0
  190310: dc ff 00 10  	b	0x190284 <.text+0x90284>
  190314: 42 00 02 3c  	lui	$2, 0x42
  190318: 42 00 02 3c  	lui	$2, 0x42
  19031c: 50 48 42 8c  	lw	$2, 0x4850($2)
  190320: 90 00 42 dc  	ld	$2, 0x90($2)
  190324: 3c a0 02 00  	dsll32	$20, $2, 0x0
  190328: 46 36 06 0c  	jal	0x18d918 <.text+0x8d918>
  19032c: 3f a0 14 00  	dsra32	$20, $20, 0x0
  190330: d3 ff 00 10  	b	0x190280 <.text+0x90280>
  190334: 2d a8 40 00  	move	$21, $2
  190338: d9 ff 00 10  	b	0x1902a0 <.text+0x902a0>
  19033c: 2d 40 00 00  	move	$8, $zero
  190340: 42 00 12 3c  	lui	$18, 0x42
  190344: 02 00 05 24  	addiu	$5, $zero, 0x2
  190348: 50 48 42 8e  	lw	$2, 0x4850($18)
  19034c: 08 00 44 24  	addiu	$4, $2, 0x8
  190350: 18 00 50 8c  	lw	$16, 0x18($2)
  190354: 5b 4a 06 0c  	jal	0x19296c <.text+0x9296c>
  190358: 20 00 53 dc  	ld	$19, 0x20($2)
  19035c: 50 48 51 8e  	lw	$17, 0x4850($18)
  190360: 2d 28 00 02  	move	$5, $16
  190364: 2d a8 40 00  	move	$21, $2
  190368: 20 00 30 de  	ld	$16, 0x20($17)
  19036c: 78 00 24 de  	ld	$4, 0x78($17)
  190370: 2f 80 13 02  	dsubu	$16, $16, $19
  190374: 3c 10 10 00  	dsll32	$2, $16, 0x0
  190378: 3f 10 02 00  	dsra32	$2, $2, 0x0
  19037c: 2d 30 40 00  	move	$6, $2
  190380: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  190384: 21 a0 82 02  	addu	$20, $20, $2
  190388: 50 48 43 8e  	lw	$3, 0x4850($18)
  19038c: 78 00 22 fe  	sd	$2, 0x78($17)
  190390: 90 00 62 dc  	ld	$2, 0x90($3)
  190394: 2f 10 50 00  	dsubu	$2, $2, $16
  190398: 90 00 62 fc  	sd	$2, 0x90($3)
  19039c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1903a0: b7 ff a2 16  	bne	$21, $2, 0x190280 <.text+0x90280>
  1903a4: 2d 40 80 02  	move	$8, $20
  1903a8: be ff 00 10  	b	0x1902a4 <.text+0x902a4>
  1903ac: 60 00 bf df  	ld	$ra, 0x60($sp)
  1903b0: 75 ff 40 14  	bnez	$2, 0x190188 <.text+0x90188>
  1903b4: 42 00 02 3c  	lui	$2, 0x42
  1903b8: 88 00 82 dc  	ld	$2, 0x88($4)
  1903bc: 72 ff 40 50  	beqzl	$2, 0x190188 <.text+0x90188>
  1903c0: 42 00 02 3c  	lui	$2, 0x42
  1903c4: 3c 18 02 00  	dsll32	$3, $2, 0x0
  1903c8: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1903cc: 00 40 10 24  	addiu	$16, $zero, 0x4000
  1903d0: 00 40 42 2c  	sltiu	$2, $2, 0x4000
  1903d4: 0b 80 62 00  	movn	$16, $3, $2
  1903d8: d7 ff 00 12  	beqz	$16, 0x190338 <.text+0x90338>
  1903dc: 2d 30 00 00  	move	$6, $zero
  1903e0: 50 00 85 dc  	ld	$5, 0x50($4)
  1903e4: a8 00 82 dc  	ld	$2, 0xa8($4)
  1903e8: 2d 28 a2 00  	daddu	$5, $5, $2
  1903ec: 3c 28 05 00  	dsll32	$5, $5, 0x0
  1903f0: 3f 28 05 00  	dsra32	$5, $5, 0x0
  1903f4: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  1903f8: 98 00 84 8c  	lw	$4, 0x98($4)
  1903fc: 14 00 40 04  	bltz	$2, 0x190450 <.text+0x90450>
  190400: 50 48 22 8e  	lw	$2, 0x4850($17)
  190404: 2d 30 00 02  	move	$6, $16
  190408: 00 00 45 8c  	lw	$5, 0x0($2)
  19040c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  190410: 98 00 44 8c  	lw	$4, 0x98($2)
  190414: 0e 00 50 14  	bne	$2, $16, 0x190450 <.text+0x90450>
  190418: 2d 38 40 00  	move	$7, $2
  19041c: 50 48 22 8e  	lw	$2, 0x4850($17)
  190420: 3c 18 07 00  	dsll32	$3, $7, 0x0
  190424: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  190428: 50 00 44 dc  	ld	$4, 0x50($2)
  19042c: 88 00 45 dc  	ld	$5, 0x88($2)
  190430: 00 00 46 8c  	lw	$6, 0x0($2)
  190434: 2d 20 83 00  	daddu	$4, $4, $3
  190438: 2f 28 a3 00  	dsubu	$5, $5, $3
  19043c: 0c 00 47 ac  	sw	$7, 0xc($2)
  190440: 50 00 44 fc  	sd	$4, 0x50($2)
  190444: 88 00 45 fc  	sd	$5, 0x88($2)
  190448: 4e ff 00 10  	b	0x190184 <.text+0x90184>
  19044c: 08 00 46 ac  	sw	$6, 0x8($2)
  190450: 93 ff 00 10  	b	0x1902a0 <.text+0x902a0>
  190454: ff ff 08 24  	addiu	$8, $zero, -0x1 <.text+0xffffffffffefffff>
  190458: 04 00 80 10  	beqz	$4, 0x19046c <.text+0x9046c>
  19045c: 9a ff 02 24  	addiu	$2, $zero, -0x66 <.text+0xffffffffffefff9a>
  190460: e0 00 84 8c  	lw	$4, 0xe0($4)
  190464: 01 00 80 54  	bnezl	$4, 0x19046c <.text+0x9046c>
  190468: 20 00 82 dc  	ld	$2, 0x20($4)
  19046c: 08 00 e0 03  	jr	$ra
  190470: 00 00 00 00  	nop
  190474: 08 00 80 10  	beqz	$4, 0x190498 <.text+0x90498>
  190478: 9a ff 02 24  	addiu	$2, $zero, -0x66 <.text+0xffffffffffefff9a>
  19047c: e0 00 84 8c  	lw	$4, 0xe0($4)
  190480: 05 00 80 10  	beqz	$4, 0x190498 <.text+0x90498>
  190484: 00 00 00 00  	nop
  190488: 90 00 82 dc  	ld	$2, 0x90($4)
  19048c: 01 00 42 2c  	sltiu	$2, $2, 0x1
  190490: 3c 10 02 00  	dsll32	$2, $2, 0x0
  190494: 3f 10 02 00  	dsra32	$2, $2, 0x0
  190498: 08 00 e0 03  	jr	$ra
  19049c: 00 00 00 00  	nop
  1904a0: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  1904a4: 9a ff 03 24  	addiu	$3, $zero, -0x66 <.text+0xffffffffffefff9a>
  1904a8: 40 00 b4 ff  	sd	$20, 0x40($sp)
  1904ac: 2d a0 a0 00  	move	$20, $5
  1904b0: 50 00 bf ff  	sd	$ra, 0x50($sp)
  1904b4: 30 00 b3 ff  	sd	$19, 0x30($sp)
  1904b8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1904bc: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1904c0: 13 00 80 10  	beqz	$4, 0x190510 <.text+0x90510>
  1904c4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1904c8: e0 00 90 8c  	lw	$16, 0xe0($4)
  1904cc: 11 00 00 52  	beqzl	$16, 0x190514 <.text+0x90514>
  1904d0: 50 00 bf df  	ld	$ra, 0x50($sp)
  1904d4: 68 00 02 9e  	lwu	$2, 0x68($16)
  1904d8: 70 00 07 de  	ld	$7, 0x70($16)
  1904dc: 2f 88 47 00  	dsubu	$17, $2, $7
  1904e0: 3c 18 11 00  	dsll32	$3, $17, 0x0
  1904e4: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1904e8: 09 00 a0 10  	beqz	$5, 0x190510 <.text+0x90510>
  1904ec: 3c 10 06 00  	dsll32	$2, $6, 0x0
  1904f0: 3c 90 11 00  	dsll32	$18, $17, 0x0
  1904f4: 3f 90 12 00  	dsra32	$18, $18, 0x0
  1904f8: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1904fc: 2d 98 c0 00  	move	$19, $6
  190500: 2b 10 22 02  	sltu	$2, $17, $2
  190504: 0b 98 42 02  	movn	$19, $18, $2
  190508: 0a 00 60 16  	bnez	$19, 0x190534 <.text+0x90534>
  19050c: 2d 18 00 00  	move	$3, $zero
  190510: 50 00 bf df  	ld	$ra, 0x50($sp)
  190514: 2d 10 60 00  	move	$2, $3
  190518: 40 00 b4 df  	ld	$20, 0x40($sp)
  19051c: 30 00 b3 df  	ld	$19, 0x30($sp)
  190520: 20 00 b2 df  	ld	$18, 0x20($sp)
  190524: 10 00 b1 df  	ld	$17, 0x10($sp)
  190528: 00 00 b0 df  	ld	$16, 0x0($sp)
  19052c: 08 00 e0 03  	jr	$ra
  190530: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  190534: 60 00 05 de  	ld	$5, 0x60($16)
  190538: 2d 30 00 00  	move	$6, $zero
  19053c: 2d 28 a7 00  	daddu	$5, $5, $7
  190540: 3c 28 05 00  	dsll32	$5, $5, 0x0
  190544: 3f 28 05 00  	dsra32	$5, $5, 0x0
  190548: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  19054c: 98 00 04 8e  	lw	$4, 0x98($16)
  190550: ef ff 40 04  	bltz	$2, 0x190510 <.text+0x90510>
  190554: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  190558: 98 00 04 8e  	lw	$4, 0x98($16)
  19055c: 2d 28 80 02  	move	$5, $20
  190560: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  190564: 2d 30 40 02  	move	$6, $18
  190568: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19056c: 26 10 51 00  	xor	$2, $2, $17
  190570: e7 ff 00 10  	b	0x190510 <.text+0x90510>
  190574: 0a 18 62 02  	movz	$3, $19, $2
  190578: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19057c: 9a ff 02 24  	addiu	$2, $zero, -0x66 <.text+0xffffffffffefff9a>
  190580: 20 00 b2 ff  	sd	$18, 0x20($sp)
  190584: 2d 90 00 00  	move	$18, $zero
  190588: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19058c: 2d 88 80 00  	move	$17, $4
  190590: 30 00 bf ff  	sd	$ra, 0x30($sp)
  190594: 16 00 80 10  	beqz	$4, 0x1905f0 <.text+0x905f0>
  190598: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19059c: e0 00 90 8c  	lw	$16, 0xe0($4)
  1905a0: 14 00 00 52  	beqzl	$16, 0x1905f4 <.text+0x905f4>
  1905a4: 30 00 bf df  	ld	$ra, 0x30($sp)
  1905a8: 90 00 02 de  	ld	$2, 0x90($16)
  1905ac: 06 00 40 14  	bnez	$2, 0x1905c8 <.text+0x905c8>
  1905b0: 00 00 04 8e  	lw	$4, 0x0($16)
  1905b4: 78 00 02 de  	ld	$2, 0x78($16)
  1905b8: 97 ff 12 24  	addiu	$18, $zero, -0x69 <.text+0xffffffffffefff97>
  1905bc: 80 00 03 de  	ld	$3, 0x80($16)
  1905c0: 26 10 43 00  	xor	$2, $2, $3
  1905c4: 0a 90 02 00  	movz	$18, $zero, $2
  1905c8: 13 00 80 14  	bnez	$4, 0x190618 <.text+0x90618>
  1905cc: 00 00 00 00  	nop
  1905d0: 58 00 02 de  	ld	$2, 0x58($16)
  1905d4: 0c 00 40 14  	bnez	$2, 0x190608 <.text+0x90608>
  1905d8: 00 00 00 ae  	sw	$zero, 0x0($16)
  1905dc: 58 00 00 fe  	sd	$zero, 0x58($16)
  1905e0: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1905e4: 2d 20 00 02  	move	$4, $16
  1905e8: e0 00 20 ae  	sw	$zero, 0xe0($17)
  1905ec: 2d 10 40 02  	move	$2, $18
  1905f0: 30 00 bf df  	ld	$ra, 0x30($sp)
  1905f4: 20 00 b2 df  	ld	$18, 0x20($sp)
  1905f8: 10 00 b1 df  	ld	$17, 0x10($sp)
  1905fc: 00 00 b0 df  	ld	$16, 0x0($sp)
  190600: 08 00 e0 03  	jr	$ra
  190604: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  190608: e1 49 06 0c  	jal	0x192784 <.text+0x92784>
  19060c: 08 00 04 26  	addiu	$4, $16, 0x8
  190610: f3 ff 00 10  	b	0x1905e0 <.text+0x905e0>
  190614: 58 00 00 fe  	sd	$zero, 0x58($16)
  190618: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  19061c: 00 00 00 00  	nop
  190620: ec ff 00 10  	b	0x1905d4 <.text+0x905d4>
  190624: 58 00 02 de  	ld	$2, 0x58($16)
  190628: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  19062c: 9a ff 03 24  	addiu	$3, $zero, -0x66 <.text+0xffffffffffefff9a>
  190630: 30 00 b3 ff  	sd	$19, 0x30($sp)
  190634: 2d 98 c0 00  	move	$19, $6
  190638: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19063c: 2d 90 a0 00  	move	$18, $5
  190640: 10 00 b1 ff  	sd	$17, 0x10($sp)
  190644: 2d 88 80 00  	move	$17, $4
  190648: 40 00 bf ff  	sd	$ra, 0x40($sp)
  19064c: 1b 00 80 10  	beqz	$4, 0x1906bc <.text+0x906bc>
  190650: 00 00 b0 ff  	sd	$16, 0x0($sp)
  190654: 38 00 85 dc  	ld	$5, 0x38($4)
  190658: 2d 80 c0 00  	move	$16, $6
  19065c: 10 00 83 dc  	ld	$3, 0x10($4)
  190660: 2d 30 00 00  	move	$6, $zero
  190664: 16 00 a5 64  	daddiu	$5, $5, 0x16
  190668: 00 00 84 8c  	lw	$4, 0x0($4)
  19066c: 2b 10 73 00  	sltu	$2, $3, $19
  190670: 3c 28 05 00  	dsll32	$5, $5, 0x0
  190674: 3f 28 05 00  	dsra32	$5, $5, 0x0
  190678: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  19067c: 0b 80 62 00  	movn	$16, $3, $2
  190680: 0e 00 40 04  	bltz	$2, 0x1906bc <.text+0x906bc>
  190684: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  190688: 14 00 00 56  	bnezl	$16, 0x1906dc <.text+0x906dc>
  19068c: 00 00 40 a2  	sb	$zero, 0x0($18)
  190690: 08 00 40 12  	beqz	$18, 0x1906b4 <.text+0x906b4>
  190694: 00 00 00 00  	nop
  190698: 10 00 22 de  	ld	$2, 0x10($17)
  19069c: 2b 10 53 00  	sltu	$2, $2, $19
  1906a0: 04 00 40 10  	beqz	$2, 0x1906b4 <.text+0x906b4>
  1906a4: 00 00 00 00  	nop
  1906a8: 10 00 22 8e  	lw	$2, 0x10($17)
  1906ac: 21 10 42 02  	addu	$2, $18, $2
  1906b0: 00 00 40 a0  	sb	$zero, 0x0($2)
  1906b4: 3c 18 10 00  	dsll32	$3, $16, 0x0
  1906b8: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1906bc: 40 00 bf df  	ld	$ra, 0x40($sp)
  1906c0: 2d 10 60 00  	move	$2, $3
  1906c4: 30 00 b3 df  	ld	$19, 0x30($sp)
  1906c8: 20 00 b2 df  	ld	$18, 0x20($sp)
  1906cc: 10 00 b1 df  	ld	$17, 0x10($sp)
  1906d0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1906d4: 08 00 e0 03  	jr	$ra
  1906d8: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1906dc: 3c 30 10 00  	dsll32	$6, $16, 0x0
  1906e0: 3f 30 06 00  	dsra32	$6, $6, 0x0
  1906e4: 2d 28 40 02  	move	$5, $18
  1906e8: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1906ec: 00 00 24 8e  	lw	$4, 0x0($17)
  1906f0: f2 ff 50 14  	bne	$2, $16, 0x1906bc <.text+0x906bc>
  1906f4: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1906f8: e5 ff 00 10  	b	0x190690 <.text+0x90690>
  1906fc: 00 00 00 00  	nop
