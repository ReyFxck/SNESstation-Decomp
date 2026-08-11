  18c124: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  18c128: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18c12c: 42 00 10 3c  	lui	$16, 0x42
  18c130: 40 00 b4 ff  	sd	$20, 0x40($sp)
  18c134: 2d a0 80 00  	move	$20, $4
  18c138: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c13c: 30 00 b3 ff  	sd	$19, 0x30($sp)
  18c140: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18c144: 2d 98 a0 00  	move	$19, $5
  18c148: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18c14c: 50 00 bf ff  	sd	$ra, 0x50($sp)
  18c150: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c154: 2d 90 00 00  	move	$18, $zero
  18c158: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c15c: 01 00 51 24  	addiu	$17, $2, 0x1
  18c160: 42 00 10 3c  	lui	$16, 0x42
  18c164: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c168: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c16c: 28 6e 03 96  	lhu	$3, 0x6e28($16)
  18c170: f0 00 62 30  	andi	$2, $3, 0xf0
  18c174: 0f 00 63 30  	andi	$3, $3, 0xf
  18c178: 02 11 02 00  	srl	$2, $2, 0x4
  18c17c: 01 00 64 24  	addiu	$4, $3, 0x1
  18c180: 01 00 43 24  	addiu	$3, $2, 0x1
  18c184: 21 10 43 02  	addu	$2, $18, $3
  18c188: 2b 10 62 02  	sltu	$2, $19, $2
  18c18c: 11 00 40 14  	bnez	$2, 0x18c1d4 <.text+0x8c1d4>
  18c190: 04 00 05 24  	addiu	$5, $zero, 0x4
  18c194: 80 10 12 00  	sll	$2, $18, 0x2
  18c198: 21 10 54 00  	addu	$2, $2, $20
  18c19c: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  18c1a0: 00 00 44 ac  	sw	$4, 0x0($2)
  18c1a4: 01 00 52 26  	addiu	$18, $18, 0x1
  18c1b4: f9 ff 60 14  	bnez	$3, 0x18c19c <.text+0x8c19c>
  18c1b8: 04 00 42 24  	addiu	$2, $2, 0x4
  18c1bc: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18c1c0: e8 ff 20 16  	bnez	$17, 0x18c164 <.text+0x8c164>
  18c1c4: 42 00 10 3c  	lui	$16, 0x42
  18c1c8: 26 10 53 02  	xor	$2, $18, $19
  18c1cc: 04 00 05 24  	addiu	$5, $zero, 0x4
  18c1d0: 0a 28 02 00  	movz	$5, $zero, $2
  18c1d4: 50 00 bf df  	ld	$ra, 0x50($sp)
  18c1d8: 2d 10 a0 00  	move	$2, $5
  18c1dc: 40 00 b4 df  	ld	$20, 0x40($sp)
  18c1e0: 30 00 b3 df  	ld	$19, 0x30($sp)
  18c1e4: 20 00 b2 df  	ld	$18, 0x20($sp)
  18c1e8: 10 00 b1 df  	ld	$17, 0x10($sp)
  18c1ec: 00 00 b0 df  	ld	$16, 0x0($sp)
  18c1f0: 08 00 e0 03  	jr	$ra
  18c1f4: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  18c1f8: 30 ff bd 27  	addiu	$sp, $sp, -0xd0 <.text+0xffffffffffefff30>
  18c1fc: 42 00 02 3c  	lui	$2, 0x42
  18c200: b0 00 be ff  	sd	$fp, 0xb0($sp)
  18c204: 40 18 07 00  	sll	$3, $7, 0x1
  18c208: 90 00 b6 ff  	sd	$22, 0x90($sp)
  18c20c: 2d f0 20 01  	move	$fp, $9
  18c210: 70 00 b4 ff  	sd	$20, 0x70($sp)
  18c214: 2d b0 00 00  	move	$22, $zero
  18c218: 60 00 b3 ff  	sd	$19, 0x60($sp)
  18c21c: 2d a0 00 00  	move	$20, $zero
  18c220: c0 00 bf ff  	sd	$ra, 0xc0($sp)
  18c224: 2d 98 00 00  	move	$19, $zero
  18c228: a0 00 b7 ff  	sd	$23, 0xa0($sp)
  18c22c: 80 00 b5 ff  	sd	$21, 0x80($sp)
  18c230: 50 00 b2 ff  	sd	$18, 0x50($sp)
  18c234: 40 00 b1 ff  	sd	$17, 0x40($sp)
  18c238: 30 00 b0 ff  	sd	$16, 0x30($sp)
  18c23c: 10 00 a8 af  	sw	$8, 0x10($sp)
  18c240: 54 48 42 8c  	lw	$2, 0x4854($2)
  18c244: 10 00 a9 8f  	lw	$9, 0x10($sp)
  18c248: e0 00 48 8c  	lw	$8, 0xe0($2)
  18c24c: 42 00 02 3c  	lui	$2, 0x42
  18c250: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18c254: 0c 00 a7 af  	sw	$7, 0xc($sp)
  18c258: 90 00 17 dd  	ld	$23, 0x90($8)
  18c25c: 40 38 09 00  	sll	$7, $9, 0x1
  18c260: 40 40 1e 00  	sll	$8, $fp, 0x1
  18c264: 21 18 62 00  	addu	$3, $3, $2
  18c268: 21 40 02 01  	addu	$8, $8, $2
  18c26c: 21 38 e2 00  	addu	$7, $7, $2
  18c270: 00 00 63 94  	lhu	$3, 0x0($3)
  18c274: 01 00 02 24  	addiu	$2, $zero, 0x1
  18c278: 00 00 e7 94  	lhu	$7, 0x0($7)
  18c27c: 00 00 08 95  	lhu	$8, 0x0($8)
  18c280: 00 00 a4 af  	sw	$4, 0x0($sp)
  18c284: 04 00 a5 af  	sw	$5, 0x4($sp)
  18c288: 08 00 a6 af  	sw	$6, 0x8($sp)
  18c28c: 14 00 a3 af  	sw	$3, 0x14($sp)
  18c290: 18 00 a7 af  	sw	$7, 0x18($sp)
  18c294: 1c 00 a8 af  	sw	$8, 0x1c($sp)
  18c298: 3f 00 e0 1a  	blez	$23, 0x18c398 <.text+0x8c398>
  18c29c: 20 00 a2 af  	sw	$2, 0x20($sp)
  18c2a0: 5b 01 60 12  	beqz	$19, 0x18c810 <.text+0x8c810>
  18c2a4: 42 00 10 3c  	lui	$16, 0x42
  18c2a8: 01 00 82 32  	andi	$2, $20, 0x1
  18c2ac: 3c 10 02 00  	dsll32	$2, $2, 0x0
  18c2b0: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18c2b4: 69 00 40 10  	beqz	$2, 0x18c45c <.text+0x8c45c>
  18c2b8: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  18c2bc: 0c 00 a3 8f  	lw	$3, 0xc($sp)
  18c2c0: 7a a0 14 00  	dsrl	$20, $20, 0x1
  18c2c4: 2b 10 63 02  	sltu	$2, $19, $3
  18c2c8: 58 00 40 14  	bnez	$2, 0x18c42c <.text+0x8c42c>
  18c2cc: ff ff f7 66  	daddiu	$23, $23, -0x1 <.text+0xffffffffffefffff>
  18c2d0: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18c2d4: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18c2d8: 14 00 a5 8f  	lw	$5, 0x14($sp)
  18c2dc: 27 10 02 00  	nor	$2, $zero, $2
  18c2e0: 00 00 a7 8f  	lw	$7, 0x0($sp)
  18c2e4: 24 10 45 00  	and	$2, $2, $5
  18c2e8: c0 10 02 00  	sll	$2, $2, 0x3
  18c2ec: 21 90 e2 00  	addu	$18, $7, $2
  18c2f0: 00 00 51 92  	lbu	$17, 0x0($18)
  18c2f4: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18c2f8: 1b 00 40 14  	bnez	$2, 0x18c368 <.text+0x8c368>
  18c2fc: 45 00 02 3c  	lui	$2, 0x45
  18c300: 63 00 02 24  	addiu	$2, $zero, 0x63
  18c304: 2b 00 22 12  	beq	$17, $2, 0x18c3b4 <.text+0x8c3b4>
  18c308: 01 00 03 24  	addiu	$3, $zero, 0x1
  18c30c: 01 00 42 92  	lbu	$2, 0x1($18)
  18c310: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18c314: 23 98 62 02  	subu	$19, $19, $2
  18c318: 16 a0 54 00  	dsrlv	$20, $20, $2
  18c31c: 2b 10 71 02  	sltu	$2, $19, $17
  18c320: 37 00 40 14  	bnez	$2, 0x18c400 <.text+0x8c400>
  18c324: 42 00 02 3c  	lui	$2, 0x42
  18c328: 40 20 11 00  	sll	$4, $17, 0x1
  18c32c: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18c330: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18c334: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18c338: 21 20 82 00  	addu	$4, $4, $2
  18c33c: 27 18 03 00  	nor	$3, $zero, $3
  18c340: 00 00 82 94  	lhu	$2, 0x0($4)
  18c344: 04 00 44 8e  	lw	$4, 0x4($18)
  18c348: 24 18 62 00  	and	$3, $3, $2
  18c34c: c0 18 03 00  	sll	$3, $3, 0x3
  18c350: 21 90 83 00  	addu	$18, $4, $3
  18c354: 00 00 51 92  	lbu	$17, 0x0($18)
  18c358: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18c35c: e9 ff 40 10  	beqz	$2, 0x18c304 <.text+0x8c304>
  18c360: 63 00 02 24  	addiu	$2, $zero, 0x63
  18c364: 45 00 02 3c  	lui	$2, 0x45
  18c368: 04 00 44 92  	lbu	$4, 0x4($18)
  18c36c: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18c370: 01 00 43 92  	lbu	$3, 0x1($18)
  18c374: 21 10 c2 02  	addu	$2, $22, $2
  18c378: 01 00 d6 26  	addiu	$22, $22, 0x1
  18c37c: 00 00 44 a0  	sb	$4, 0x0($2)
  18c380: 23 98 63 02  	subu	$19, $19, $3
  18c384: 00 80 02 34  	ori	$2, $zero, 0x8000
  18c388: 17 00 c2 12  	beq	$22, $2, 0x18c3e8 <.text+0x8c3e8>
  18c38c: 16 a0 74 00  	dsrlv	$20, $20, $3
  18c390: c3 ff e0 1e  	bgtz	$23, 0x18c2a0 <.text+0x8c2a0>
  18c394: 00 00 00 00  	nop
  18c398: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18c39c: 2d 20 c0 02  	move	$4, $22
  18c3a0: 42 00 02 3c  	lui	$2, 0x42
  18c3a4: 50 48 42 8c  	lw	$2, 0x4850($2)
  18c3a8: 05 00 03 24  	addiu	$3, $zero, 0x5
  18c3ac: 88 00 42 dc  	ld	$2, 0x88($2)
  18c3b0: 0a 18 02 00  	movz	$3, $zero, $2
  18c3b4: c0 00 bf df  	ld	$ra, 0xc0($sp)
  18c3b8: 2d 10 60 00  	move	$2, $3
  18c3bc: b0 00 be df  	ld	$fp, 0xb0($sp)
  18c3c0: a0 00 b7 df  	ld	$23, 0xa0($sp)
  18c3c4: 90 00 b6 df  	ld	$22, 0x90($sp)
  18c3c8: 80 00 b5 df  	ld	$21, 0x80($sp)
  18c3cc: 70 00 b4 df  	ld	$20, 0x70($sp)
  18c3d0: 60 00 b3 df  	ld	$19, 0x60($sp)
  18c3d4: 50 00 b2 df  	ld	$18, 0x50($sp)
  18c3d8: 40 00 b1 df  	ld	$17, 0x40($sp)
  18c3dc: 30 00 b0 df  	ld	$16, 0x30($sp)
  18c3e0: 08 00 e0 03  	jr	$ra
  18c3e4: d0 00 bd 27  	addiu	$sp, $sp, 0xd0
  18c3e8: 00 80 04 34  	ori	$4, $zero, 0x8000
  18c3ec: 20 00 a0 af  	sw	$zero, 0x20($sp)
  18c3f0: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18c3f4: 2d b0 00 00  	move	$22, $zero
  18c3f8: e5 ff 00 10  	b	0x18c390 <.text+0x8c390>
  18c3fc: 00 00 00 00  	nop
  18c400: 42 00 10 3c  	lui	$16, 0x42
  18c404: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c408: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c40c: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c410: 14 10 62 02  	dsllv	$2, $2, $19
  18c414: 08 00 73 26  	addiu	$19, $19, 0x8
  18c418: 2b 18 71 02  	sltu	$3, $19, $17
  18c41c: f8 ff 60 14  	bnez	$3, 0x18c400 <.text+0x8c400>
  18c420: 25 a0 82 02  	or	$20, $20, $2
  18c424: c0 ff 00 10  	b	0x18c328 <.text+0x8c328>
  18c428: 42 00 02 3c  	lui	$2, 0x42
  18c42c: 42 00 10 3c  	lui	$16, 0x42
  18c430: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c434: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c438: 0c 00 a4 8f  	lw	$4, 0xc($sp)
  18c43c: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c440: 14 10 62 02  	dsllv	$2, $2, $19
  18c444: 08 00 73 26  	addiu	$19, $19, 0x8
  18c448: 2b 18 64 02  	sltu	$3, $19, $4
  18c44c: f7 ff 60 14  	bnez	$3, 0x18c42c <.text+0x8c42c>
  18c450: 25 a0 82 02  	or	$20, $20, $2
  18c454: 9e ff 00 10  	b	0x18c2d0 <.text+0x8c2d0>
  18c458: 00 00 00 00  	nop
  18c45c: 07 00 62 2e  	sltiu	$2, $19, 0x7
  18c460: df 00 40 14  	bnez	$2, 0x18c7e0 <.text+0x8c7e0>
  18c464: 7a a0 14 00  	dsrl	$20, $20, 0x1
  18c468: f9 ff 73 26  	addiu	$19, $19, -0x7 <.text+0xffffffffffeffff9>
  18c46c: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18c470: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18c474: 2b 18 7e 02  	sltu	$3, $19, $fp
  18c478: 7f 00 55 30  	andi	$21, $2, 0x7f
  18c47c: cd 00 60 14  	bnez	$3, 0x18c7b4 <.text+0x8c7b4>
  18c480: fa a1 14 00  	dsrl	$20, $20, 0x7
  18c484: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18c488: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18c48c: 1c 00 a9 8f  	lw	$9, 0x1c($sp)
  18c490: 27 10 02 00  	nor	$2, $zero, $2
  18c494: 08 00 a3 8f  	lw	$3, 0x8($sp)
  18c498: 24 10 49 00  	and	$2, $2, $9
  18c49c: c0 10 02 00  	sll	$2, $2, 0x3
  18c4a0: 21 90 62 00  	addu	$18, $3, $2
  18c4a4: 00 00 51 92  	lbu	$17, 0x0($18)
  18c4a8: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18c4ac: 1b 00 40 54  	bnezl	$2, 0x18c51c <.text+0x8c51c>
  18c4b0: 01 00 42 92  	lbu	$2, 0x1($18)
  18c4b4: 63 00 02 24  	addiu	$2, $zero, 0x63
  18c4b8: be ff 22 12  	beq	$17, $2, 0x18c3b4 <.text+0x8c3b4>
  18c4bc: 01 00 03 24  	addiu	$3, $zero, 0x1
  18c4c0: 01 00 42 92  	lbu	$2, 0x1($18)
  18c4c4: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18c4c8: 23 98 62 02  	subu	$19, $19, $2
  18c4cc: 16 a0 54 00  	dsrlv	$20, $20, $2
  18c4d0: 2b 10 71 02  	sltu	$2, $19, $17
  18c4d4: ac 00 40 14  	bnez	$2, 0x18c788 <.text+0x8c788>
  18c4d8: 42 00 02 3c  	lui	$2, 0x42
  18c4dc: 40 20 11 00  	sll	$4, $17, 0x1
  18c4e0: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18c4e4: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18c4e8: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18c4ec: 21 20 82 00  	addu	$4, $4, $2
  18c4f0: 27 18 03 00  	nor	$3, $zero, $3
  18c4f4: 00 00 82 94  	lhu	$2, 0x0($4)
  18c4f8: 04 00 44 8e  	lw	$4, 0x4($18)
  18c4fc: 24 18 62 00  	and	$3, $3, $2
  18c500: c0 18 03 00  	sll	$3, $3, 0x3
  18c504: 21 90 83 00  	addu	$18, $4, $3
  18c508: 00 00 51 92  	lbu	$17, 0x0($18)
  18c50c: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18c510: e9 ff 40 10  	beqz	$2, 0x18c4b8 <.text+0x8c4b8>
  18c514: 63 00 02 24  	addiu	$2, $zero, 0x63
  18c518: 01 00 42 92  	lbu	$2, 0x1($18)
  18c51c: 23 20 d5 02  	subu	$4, $22, $21
  18c520: 10 00 a5 8f  	lw	$5, 0x10($sp)
  18c524: 04 00 43 96  	lhu	$3, 0x4($18)
  18c528: 23 98 62 02  	subu	$19, $19, $2
  18c52c: 16 a0 54 00  	dsrlv	$20, $20, $2
  18c530: 2b 10 65 02  	sltu	$2, $19, $5
  18c534: 88 00 40 14  	bnez	$2, 0x18c758 <.text+0x8c758>
  18c538: 23 a8 83 00  	subu	$21, $4, $3
  18c53c: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18c540: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18c544: 18 00 a9 8f  	lw	$9, 0x18($sp)
  18c548: 27 10 02 00  	nor	$2, $zero, $2
  18c54c: 04 00 a3 8f  	lw	$3, 0x4($sp)
  18c550: 24 10 49 00  	and	$2, $2, $9
  18c554: c0 10 02 00  	sll	$2, $2, 0x3
  18c558: 21 90 62 00  	addu	$18, $3, $2
  18c55c: 00 00 51 92  	lbu	$17, 0x0($18)
  18c560: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18c564: 1b 00 40 54  	bnezl	$2, 0x18c5d4 <.text+0x8c5d4>
  18c568: 01 00 42 92  	lbu	$2, 0x1($18)
  18c56c: 63 00 02 24  	addiu	$2, $zero, 0x63
  18c570: 90 ff 22 12  	beq	$17, $2, 0x18c3b4 <.text+0x8c3b4>
  18c574: 01 00 03 24  	addiu	$3, $zero, 0x1
  18c578: 01 00 42 92  	lbu	$2, 0x1($18)
  18c57c: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18c580: 23 98 62 02  	subu	$19, $19, $2
  18c584: 16 a0 54 00  	dsrlv	$20, $20, $2
  18c588: 2b 10 71 02  	sltu	$2, $19, $17
  18c58c: 67 00 40 14  	bnez	$2, 0x18c72c <.text+0x8c72c>
  18c590: 42 00 02 3c  	lui	$2, 0x42
  18c594: 40 20 11 00  	sll	$4, $17, 0x1
  18c598: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18c59c: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18c5a0: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18c5a4: 21 20 82 00  	addu	$4, $4, $2
  18c5a8: 27 18 03 00  	nor	$3, $zero, $3
  18c5ac: 00 00 82 94  	lhu	$2, 0x0($4)
  18c5b0: 04 00 44 8e  	lw	$4, 0x4($18)
  18c5b4: 24 18 62 00  	and	$3, $3, $2
  18c5b8: c0 18 03 00  	sll	$3, $3, 0x3
  18c5bc: 21 90 83 00  	addu	$18, $4, $3
  18c5c0: 00 00 51 92  	lbu	$17, 0x0($18)
  18c5c4: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18c5c8: e9 ff 40 10  	beqz	$2, 0x18c570 <.text+0x8c570>
  18c5cc: 63 00 02 24  	addiu	$2, $zero, 0x63
  18c5d0: 01 00 42 92  	lbu	$2, 0x1($18)
  18c5d4: 04 00 52 96  	lhu	$18, 0x4($18)
  18c5d8: 23 98 62 02  	subu	$19, $19, $2
  18c5dc: 0a 00 20 12  	beqz	$17, 0x18c608 <.text+0x8c608>
  18c5e0: 16 a0 54 00  	dsrlv	$20, $20, $2
  18c5e4: 08 00 62 2e  	sltiu	$2, $19, 0x8
  18c5e8: 46 00 40 14  	bnez	$2, 0x18c704 <.text+0x8c704>
  18c5ec: 42 00 10 3c  	lui	$16, 0x42
  18c5f0: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18c5f4: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18c5f8: f8 ff 73 26  	addiu	$19, $19, -0x8 <.text+0xffffffffffeffff8>
  18c5fc: ff 00 42 30  	andi	$2, $2, 0xff
  18c600: 3a a2 14 00  	dsrl	$20, $20, 0x8
  18c604: 21 90 42 02  	addu	$18, $18, $2
  18c608: 3c 10 12 00  	dsll32	$2, $18, 0x0
  18c60c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  18c610: 2f b8 e2 02  	dsubu	$23, $23, $2
  18c614: ff 7f b5 32  	andi	$21, $21, 0x7fff
  18c618: 2b 10 d5 02  	sltu	$2, $22, $21
  18c61c: 36 00 40 10  	beqz	$2, 0x18c6f8 <.text+0x8c6f8>
  18c620: 00 80 02 34  	ori	$2, $zero, 0x8000
  18c624: 23 88 55 00  	subu	$17, $2, $21
  18c628: 2b 10 51 02  	sltu	$2, $18, $17
  18c62c: 20 00 a4 8f  	lw	$4, 0x20($sp)
  18c630: 0b 88 42 02  	movn	$17, $18, $2
  18c634: 04 00 80 10  	beqz	$4, 0x18c648 <.text+0x8c648>
  18c638: 23 90 51 02  	subu	$18, $18, $17
  18c63c: 2b 10 b6 02  	sltu	$2, $21, $22
  18c640: 24 00 40 10  	beqz	$2, 0x18c6d4 <.text+0x8c6d4>
  18c644: 45 00 04 3c  	lui	$4, 0x45
  18c648: 23 10 d5 02  	subu	$2, $22, $21
  18c64c: 2b 10 51 00  	sltu	$2, $2, $17
  18c650: 17 00 40 10  	beqz	$2, 0x18c6b0 <.text+0x8c6b0>
  18c654: 45 00 04 3c  	lui	$4, 0x45
  18c658: 45 00 02 3c  	lui	$2, 0x45
  18c65c: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18c660: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18c664: 21 18 a2 02  	addu	$3, $21, $2
  18c668: 01 00 b5 26  	addiu	$21, $21, 0x1
  18c66c: 00 00 63 90  	lbu	$3, 0x0($3)
  18c670: 21 10 c2 02  	addu	$2, $22, $2
  18c674: 01 00 d6 26  	addiu	$22, $22, 0x1
  18c678: f7 ff 20 16  	bnez	$17, 0x18c658 <.text+0x8c658>
  18c67c: 00 00 43 a0  	sb	$3, 0x0($2)
  18c680: 00 80 02 34  	ori	$2, $zero, 0x8000
  18c684: 05 00 c2 12  	beq	$22, $2, 0x18c69c <.text+0x8c69c>
  18c688: 00 80 04 34  	ori	$4, $zero, 0x8000
  18c68c: e2 ff 40 56  	bnezl	$18, 0x18c618 <.text+0x8c618>
  18c690: ff 7f b5 32  	andi	$21, $21, 0x7fff
  18c694: 3e ff 00 10  	b	0x18c390 <.text+0x8c390>
  18c698: 00 00 00 00  	nop
  18c69c: 20 00 a0 af  	sw	$zero, 0x20($sp)
  18c6a0: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18c6a4: 2d b0 00 00  	move	$22, $zero
  18c6a8: f8 ff 00 10  	b	0x18c68c <.text+0x8c68c>
  18c6ac: 00 00 00 00  	nop
  18c6b0: 2d 30 20 02  	move	$6, $17
  18c6b4: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18c6b8: 21 28 a4 02  	addu	$5, $21, $4
  18c6bc: 21 a8 b1 02  	addu	$21, $21, $17
  18c6c0: 21 20 c4 02  	addu	$4, $22, $4
  18c6c4: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  18c6c8: 21 b0 d1 02  	addu	$22, $22, $17
  18c6cc: ed ff 00 10  	b	0x18c684 <.text+0x8c684>
  18c6d0: 00 80 02 34  	ori	$2, $zero, 0x8000
  18c6d4: 2d 28 00 00  	move	$5, $zero
  18c6d8: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18c6dc: 2d 30 20 02  	move	$6, $17
  18c6e0: 21 20 c4 02  	addu	$4, $22, $4
  18c6e4: 21 a8 b1 02  	addu	$21, $21, $17
  18c6e8: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18c6ec: 21 b0 d1 02  	addu	$22, $22, $17
  18c6f0: e4 ff 00 10  	b	0x18c684 <.text+0x8c684>
  18c6f4: 00 80 02 34  	ori	$2, $zero, 0x8000
  18c6f8: cb ff 00 10  	b	0x18c628 <.text+0x8c628>
  18c6fc: 23 88 56 00  	subu	$17, $2, $22
  18c700: 42 00 10 3c  	lui	$16, 0x42
  18c704: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c708: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c70c: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c710: 14 10 62 02  	dsllv	$2, $2, $19
  18c714: 08 00 73 26  	addiu	$19, $19, 0x8
  18c718: 08 00 63 2e  	sltiu	$3, $19, 0x8
  18c71c: f8 ff 60 14  	bnez	$3, 0x18c700 <.text+0x8c700>
  18c720: 25 a0 82 02  	or	$20, $20, $2
  18c724: b2 ff 00 10  	b	0x18c5f0 <.text+0x8c5f0>
  18c728: 00 00 00 00  	nop
  18c72c: 42 00 10 3c  	lui	$16, 0x42
  18c730: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c734: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c738: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c73c: 14 10 62 02  	dsllv	$2, $2, $19
  18c740: 08 00 73 26  	addiu	$19, $19, 0x8
  18c744: 2b 18 71 02  	sltu	$3, $19, $17
  18c748: f8 ff 60 14  	bnez	$3, 0x18c72c <.text+0x8c72c>
  18c74c: 25 a0 82 02  	or	$20, $20, $2
  18c750: 90 ff 00 10  	b	0x18c594 <.text+0x8c594>
  18c754: 42 00 02 3c  	lui	$2, 0x42
  18c758: 42 00 10 3c  	lui	$16, 0x42
  18c75c: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c760: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c764: 10 00 a7 8f  	lw	$7, 0x10($sp)
  18c768: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c76c: 14 10 62 02  	dsllv	$2, $2, $19
  18c770: 08 00 73 26  	addiu	$19, $19, 0x8
  18c774: 2b 18 67 02  	sltu	$3, $19, $7
  18c778: f7 ff 60 14  	bnez	$3, 0x18c758 <.text+0x8c758>
  18c77c: 25 a0 82 02  	or	$20, $20, $2
  18c780: 6e ff 00 10  	b	0x18c53c <.text+0x8c53c>
  18c784: 00 00 00 00  	nop
  18c788: 42 00 10 3c  	lui	$16, 0x42
  18c78c: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c790: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c794: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c798: 14 10 62 02  	dsllv	$2, $2, $19
  18c79c: 08 00 73 26  	addiu	$19, $19, 0x8
  18c7a0: 2b 18 71 02  	sltu	$3, $19, $17
  18c7a4: f8 ff 60 14  	bnez	$3, 0x18c788 <.text+0x8c788>
  18c7a8: 25 a0 82 02  	or	$20, $20, $2
  18c7ac: 4b ff 00 10  	b	0x18c4dc <.text+0x8c4dc>
  18c7b0: 42 00 02 3c  	lui	$2, 0x42
  18c7b4: 42 00 10 3c  	lui	$16, 0x42
  18c7b8: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c7bc: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c7c0: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c7c4: 14 10 62 02  	dsllv	$2, $2, $19
  18c7c8: 08 00 73 26  	addiu	$19, $19, 0x8
  18c7cc: 2b 18 7e 02  	sltu	$3, $19, $fp
  18c7d0: f8 ff 60 14  	bnez	$3, 0x18c7b4 <.text+0x8c7b4>
  18c7d4: 25 a0 82 02  	or	$20, $20, $2
  18c7d8: 2a ff 00 10  	b	0x18c484 <.text+0x8c484>
  18c7dc: 00 00 00 00  	nop
  18c7e0: 42 00 10 3c  	lui	$16, 0x42
  18c7e4: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c7e8: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c7ec: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c7f0: 14 10 62 02  	dsllv	$2, $2, $19
  18c7f4: 08 00 73 26  	addiu	$19, $19, 0x8
  18c7f8: 07 00 63 2e  	sltiu	$3, $19, 0x7
  18c7fc: f8 ff 60 14  	bnez	$3, 0x18c7e0 <.text+0x8c7e0>
  18c800: 25 a0 82 02  	or	$20, $20, $2
  18c804: 19 ff 00 10  	b	0x18c46c <.text+0x8c46c>
  18c808: f9 ff 73 26  	addiu	$19, $19, -0x7 <.text+0xffffffffffeffff9>
  18c80c: 42 00 10 3c  	lui	$16, 0x42
  18c810: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18c814: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18c818: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18c81c: 14 10 62 02  	dsllv	$2, $2, $19
  18c820: 08 00 73 26  	addiu	$19, $19, 0x8
  18c824: f9 ff 60 12  	beqz	$19, 0x18c80c <.text+0x8c80c>
  18c828: 25 a0 82 02  	or	$20, $20, $2
  18c82c: 9e fe 00 10  	b	0x18c2a8 <.text+0x8c2a8>
  18c830: 08 00 13 24  	addiu	$19, $zero, 0x8
  18c834: 30 ff bd 27  	addiu	$sp, $sp, -0xd0 <.text+0xffffffffffefff30>
  18c838: 42 00 02 3c  	lui	$2, 0x42
  18c83c: b0 00 be ff  	sd	$fp, 0xb0($sp)
  18c840: 40 18 07 00  	sll	$3, $7, 0x1
  18c844: 90 00 b6 ff  	sd	$22, 0x90($sp)
  18c848: 2d f0 20 01  	move	$fp, $9
  18c84c: 70 00 b4 ff  	sd	$20, 0x70($sp)
  18c850: 2d b0 00 00  	move	$22, $zero
  18c854: 60 00 b3 ff  	sd	$19, 0x60($sp)
  18c858: 2d a0 00 00  	move	$20, $zero
  18c85c: c0 00 bf ff  	sd	$ra, 0xc0($sp)
  18c860: 2d 98 00 00  	move	$19, $zero
  18c864: a0 00 b7 ff  	sd	$23, 0xa0($sp)
  18c868: 80 00 b5 ff  	sd	$21, 0x80($sp)
  18c86c: 50 00 b2 ff  	sd	$18, 0x50($sp)
  18c870: 40 00 b1 ff  	sd	$17, 0x40($sp)
  18c874: 30 00 b0 ff  	sd	$16, 0x30($sp)
  18c878: 10 00 a8 af  	sw	$8, 0x10($sp)
  18c87c: 54 48 42 8c  	lw	$2, 0x4854($2)
  18c880: 10 00 a9 8f  	lw	$9, 0x10($sp)
  18c884: e0 00 48 8c  	lw	$8, 0xe0($2)
  18c888: 42 00 02 3c  	lui	$2, 0x42
  18c88c: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18c890: 0c 00 a7 af  	sw	$7, 0xc($sp)
  18c894: 90 00 17 dd  	ld	$23, 0x90($8)
  18c898: 40 38 09 00  	sll	$7, $9, 0x1
  18c89c: 40 40 1e 00  	sll	$8, $fp, 0x1
  18c8a0: 21 18 62 00  	addu	$3, $3, $2
  18c8a4: 21 40 02 01  	addu	$8, $8, $2
  18c8a8: 21 38 e2 00  	addu	$7, $7, $2
  18c8ac: 00 00 63 94  	lhu	$3, 0x0($3)
  18c8b0: 01 00 02 24  	addiu	$2, $zero, 0x1
  18c8b4: 00 00 e7 94  	lhu	$7, 0x0($7)
  18c8b8: 00 00 08 95  	lhu	$8, 0x0($8)
  18c8bc: 00 00 a4 af  	sw	$4, 0x0($sp)
  18c8c0: 04 00 a5 af  	sw	$5, 0x4($sp)
  18c8c4: 08 00 a6 af  	sw	$6, 0x8($sp)
  18c8c8: 14 00 a3 af  	sw	$3, 0x14($sp)
  18c8cc: 18 00 a7 af  	sw	$7, 0x18($sp)
  18c8d0: 1c 00 a8 af  	sw	$8, 0x1c($sp)
  18c8d4: 3f 00 e0 1a  	blez	$23, 0x18c9d4 <.text+0x8c9d4>
  18c8d8: 20 00 a2 af  	sw	$2, 0x20($sp)
  18c8dc: 5b 01 60 12  	beqz	$19, 0x18ce4c <.text+0x8ce4c>
  18c8e0: 42 00 10 3c  	lui	$16, 0x42
  18c8e4: 01 00 82 32  	andi	$2, $20, 0x1
  18c8e8: 3c 10 02 00  	dsll32	$2, $2, 0x0
  18c8ec: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18c8f0: 69 00 40 10  	beqz	$2, 0x18ca98 <.text+0x8ca98>
  18c8f4: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  18c8f8: 0c 00 a3 8f  	lw	$3, 0xc($sp)
  18c8fc: 7a a0 14 00  	dsrl	$20, $20, 0x1
  18c900: 2b 10 63 02  	sltu	$2, $19, $3
  18c904: 58 00 40 14  	bnez	$2, 0x18ca68 <.text+0x8ca68>
  18c908: ff ff f7 66  	daddiu	$23, $23, -0x1 <.text+0xffffffffffefffff>
  18c90c: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18c910: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18c914: 14 00 a5 8f  	lw	$5, 0x14($sp)
  18c918: 27 10 02 00  	nor	$2, $zero, $2
  18c91c: 00 00 a7 8f  	lw	$7, 0x0($sp)
  18c920: 24 10 45 00  	and	$2, $2, $5
  18c924: c0 10 02 00  	sll	$2, $2, 0x3
  18c928: 21 90 e2 00  	addu	$18, $7, $2
  18c92c: 00 00 51 92  	lbu	$17, 0x0($18)
  18c930: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18c934: 1b 00 40 14  	bnez	$2, 0x18c9a4 <.text+0x8c9a4>
  18c938: 45 00 02 3c  	lui	$2, 0x45
  18c93c: 63 00 02 24  	addiu	$2, $zero, 0x63
  18c940: 2b 00 22 12  	beq	$17, $2, 0x18c9f0 <.text+0x8c9f0>
  18c944: 01 00 03 24  	addiu	$3, $zero, 0x1
  18c948: 01 00 42 92  	lbu	$2, 0x1($18)
  18c94c: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18c950: 23 98 62 02  	subu	$19, $19, $2
  18c954: 16 a0 54 00  	dsrlv	$20, $20, $2
  18c958: 2b 10 71 02  	sltu	$2, $19, $17
  18c95c: 37 00 40 14  	bnez	$2, 0x18ca3c <.text+0x8ca3c>
  18c960: 42 00 02 3c  	lui	$2, 0x42
  18c964: 40 20 11 00  	sll	$4, $17, 0x1
  18c968: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18c96c: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18c970: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18c974: 21 20 82 00  	addu	$4, $4, $2
  18c978: 27 18 03 00  	nor	$3, $zero, $3
  18c97c: 00 00 82 94  	lhu	$2, 0x0($4)
  18c980: 04 00 44 8e  	lw	$4, 0x4($18)
  18c984: 24 18 62 00  	and	$3, $3, $2
  18c988: c0 18 03 00  	sll	$3, $3, 0x3
  18c98c: 21 90 83 00  	addu	$18, $4, $3
  18c990: 00 00 51 92  	lbu	$17, 0x0($18)
  18c994: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18c998: e9 ff 40 10  	beqz	$2, 0x18c940 <.text+0x8c940>
  18c99c: 63 00 02 24  	addiu	$2, $zero, 0x63
  18c9a0: 45 00 02 3c  	lui	$2, 0x45
  18c9a4: 04 00 44 92  	lbu	$4, 0x4($18)
  18c9a8: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18c9ac: 01 00 43 92  	lbu	$3, 0x1($18)
  18c9b0: 21 10 c2 02  	addu	$2, $22, $2
  18c9b4: 01 00 d6 26  	addiu	$22, $22, 0x1
  18c9b8: 00 00 44 a0  	sb	$4, 0x0($2)
  18c9bc: 23 98 63 02  	subu	$19, $19, $3
  18c9c0: 00 80 02 34  	ori	$2, $zero, 0x8000
  18c9c4: 17 00 c2 12  	beq	$22, $2, 0x18ca24 <.text+0x8ca24>
  18c9c8: 16 a0 74 00  	dsrlv	$20, $20, $3
  18c9cc: c3 ff e0 1e  	bgtz	$23, 0x18c8dc <.text+0x8c8dc>
  18c9d0: 00 00 00 00  	nop
  18c9d4: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18c9d8: 2d 20 c0 02  	move	$4, $22
  18c9dc: 42 00 02 3c  	lui	$2, 0x42
  18c9e0: 50 48 42 8c  	lw	$2, 0x4850($2)
  18c9e4: 05 00 03 24  	addiu	$3, $zero, 0x5
  18c9e8: 88 00 42 dc  	ld	$2, 0x88($2)
  18c9ec: 0a 18 02 00  	movz	$3, $zero, $2
  18c9f0: c0 00 bf df  	ld	$ra, 0xc0($sp)
  18c9f4: 2d 10 60 00  	move	$2, $3
  18c9f8: b0 00 be df  	ld	$fp, 0xb0($sp)
  18c9fc: a0 00 b7 df  	ld	$23, 0xa0($sp)
  18ca00: 90 00 b6 df  	ld	$22, 0x90($sp)
  18ca04: 80 00 b5 df  	ld	$21, 0x80($sp)
  18ca08: 70 00 b4 df  	ld	$20, 0x70($sp)
  18ca0c: 60 00 b3 df  	ld	$19, 0x60($sp)
  18ca10: 50 00 b2 df  	ld	$18, 0x50($sp)
  18ca14: 40 00 b1 df  	ld	$17, 0x40($sp)
  18ca18: 30 00 b0 df  	ld	$16, 0x30($sp)
  18ca1c: 08 00 e0 03  	jr	$ra
  18ca20: d0 00 bd 27  	addiu	$sp, $sp, 0xd0
  18ca24: 00 80 04 34  	ori	$4, $zero, 0x8000
  18ca28: 20 00 a0 af  	sw	$zero, 0x20($sp)
  18ca2c: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18ca30: 2d b0 00 00  	move	$22, $zero
  18ca34: e5 ff 00 10  	b	0x18c9cc <.text+0x8c9cc>
  18ca38: 00 00 00 00  	nop
  18ca3c: 42 00 10 3c  	lui	$16, 0x42
  18ca40: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18ca44: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18ca48: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18ca4c: 14 10 62 02  	dsllv	$2, $2, $19
  18ca50: 08 00 73 26  	addiu	$19, $19, 0x8
  18ca54: 2b 18 71 02  	sltu	$3, $19, $17
  18ca58: f8 ff 60 14  	bnez	$3, 0x18ca3c <.text+0x8ca3c>
  18ca5c: 25 a0 82 02  	or	$20, $20, $2
  18ca60: c0 ff 00 10  	b	0x18c964 <.text+0x8c964>
  18ca64: 42 00 02 3c  	lui	$2, 0x42
  18ca68: 42 00 10 3c  	lui	$16, 0x42
  18ca6c: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18ca70: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18ca74: 0c 00 a4 8f  	lw	$4, 0xc($sp)
  18ca78: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18ca7c: 14 10 62 02  	dsllv	$2, $2, $19
  18ca80: 08 00 73 26  	addiu	$19, $19, 0x8
  18ca84: 2b 18 64 02  	sltu	$3, $19, $4
  18ca88: f7 ff 60 14  	bnez	$3, 0x18ca68 <.text+0x8ca68>
  18ca8c: 25 a0 82 02  	or	$20, $20, $2
  18ca90: 9e ff 00 10  	b	0x18c90c <.text+0x8c90c>
  18ca94: 00 00 00 00  	nop
  18ca98: 06 00 62 2e  	sltiu	$2, $19, 0x6
  18ca9c: df 00 40 14  	bnez	$2, 0x18ce1c <.text+0x8ce1c>
  18caa0: 7a a0 14 00  	dsrl	$20, $20, 0x1
  18caa4: fa ff 73 26  	addiu	$19, $19, -0x6 <.text+0xffffffffffeffffa>
  18caa8: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18caac: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18cab0: 2b 18 7e 02  	sltu	$3, $19, $fp
  18cab4: 3f 00 55 30  	andi	$21, $2, 0x3f
  18cab8: cd 00 60 14  	bnez	$3, 0x18cdf0 <.text+0x8cdf0>
  18cabc: ba a1 14 00  	dsrl	$20, $20, 0x6
  18cac0: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18cac4: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18cac8: 1c 00 a9 8f  	lw	$9, 0x1c($sp)
  18cacc: 27 10 02 00  	nor	$2, $zero, $2
  18cad0: 08 00 a3 8f  	lw	$3, 0x8($sp)
  18cad4: 24 10 49 00  	and	$2, $2, $9
  18cad8: c0 10 02 00  	sll	$2, $2, 0x3
  18cadc: 21 90 62 00  	addu	$18, $3, $2
  18cae0: 00 00 51 92  	lbu	$17, 0x0($18)
  18cae4: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18cae8: 1b 00 40 54  	bnezl	$2, 0x18cb58 <.text+0x8cb58>
  18caec: 01 00 42 92  	lbu	$2, 0x1($18)
  18caf0: 63 00 02 24  	addiu	$2, $zero, 0x63
  18caf4: be ff 22 12  	beq	$17, $2, 0x18c9f0 <.text+0x8c9f0>
  18caf8: 01 00 03 24  	addiu	$3, $zero, 0x1
  18cafc: 01 00 42 92  	lbu	$2, 0x1($18)
  18cb00: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18cb04: 23 98 62 02  	subu	$19, $19, $2
  18cb08: 16 a0 54 00  	dsrlv	$20, $20, $2
  18cb0c: 2b 10 71 02  	sltu	$2, $19, $17
  18cb10: ac 00 40 14  	bnez	$2, 0x18cdc4 <.text+0x8cdc4>
  18cb14: 42 00 02 3c  	lui	$2, 0x42
  18cb18: 40 20 11 00  	sll	$4, $17, 0x1
  18cb1c: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18cb20: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18cb24: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18cb28: 21 20 82 00  	addu	$4, $4, $2
  18cb2c: 27 18 03 00  	nor	$3, $zero, $3
  18cb30: 00 00 82 94  	lhu	$2, 0x0($4)
  18cb34: 04 00 44 8e  	lw	$4, 0x4($18)
  18cb38: 24 18 62 00  	and	$3, $3, $2
  18cb3c: c0 18 03 00  	sll	$3, $3, 0x3
  18cb40: 21 90 83 00  	addu	$18, $4, $3
  18cb44: 00 00 51 92  	lbu	$17, 0x0($18)
  18cb48: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18cb4c: e9 ff 40 10  	beqz	$2, 0x18caf4 <.text+0x8caf4>
  18cb50: 63 00 02 24  	addiu	$2, $zero, 0x63
  18cb54: 01 00 42 92  	lbu	$2, 0x1($18)
  18cb58: 23 20 d5 02  	subu	$4, $22, $21
  18cb5c: 10 00 a5 8f  	lw	$5, 0x10($sp)
  18cb60: 04 00 43 96  	lhu	$3, 0x4($18)
  18cb64: 23 98 62 02  	subu	$19, $19, $2
  18cb68: 16 a0 54 00  	dsrlv	$20, $20, $2
  18cb6c: 2b 10 65 02  	sltu	$2, $19, $5
  18cb70: 88 00 40 14  	bnez	$2, 0x18cd94 <.text+0x8cd94>
  18cb74: 23 a8 83 00  	subu	$21, $4, $3
  18cb78: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18cb7c: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18cb80: 18 00 a9 8f  	lw	$9, 0x18($sp)
  18cb84: 27 10 02 00  	nor	$2, $zero, $2
  18cb88: 04 00 a3 8f  	lw	$3, 0x4($sp)
  18cb8c: 24 10 49 00  	and	$2, $2, $9
  18cb90: c0 10 02 00  	sll	$2, $2, 0x3
  18cb94: 21 90 62 00  	addu	$18, $3, $2
  18cb98: 00 00 51 92  	lbu	$17, 0x0($18)
  18cb9c: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18cba0: 1b 00 40 54  	bnezl	$2, 0x18cc10 <.text+0x8cc10>
  18cba4: 01 00 42 92  	lbu	$2, 0x1($18)
  18cba8: 63 00 02 24  	addiu	$2, $zero, 0x63
  18cbac: 90 ff 22 12  	beq	$17, $2, 0x18c9f0 <.text+0x8c9f0>
  18cbb0: 01 00 03 24  	addiu	$3, $zero, 0x1
  18cbb4: 01 00 42 92  	lbu	$2, 0x1($18)
  18cbb8: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18cbbc: 23 98 62 02  	subu	$19, $19, $2
  18cbc0: 16 a0 54 00  	dsrlv	$20, $20, $2
  18cbc4: 2b 10 71 02  	sltu	$2, $19, $17
  18cbc8: 67 00 40 14  	bnez	$2, 0x18cd68 <.text+0x8cd68>
  18cbcc: 42 00 02 3c  	lui	$2, 0x42
  18cbd0: 40 20 11 00  	sll	$4, $17, 0x1
  18cbd4: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18cbd8: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18cbdc: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18cbe0: 21 20 82 00  	addu	$4, $4, $2
  18cbe4: 27 18 03 00  	nor	$3, $zero, $3
  18cbe8: 00 00 82 94  	lhu	$2, 0x0($4)
  18cbec: 04 00 44 8e  	lw	$4, 0x4($18)
  18cbf0: 24 18 62 00  	and	$3, $3, $2
  18cbf4: c0 18 03 00  	sll	$3, $3, 0x3
  18cbf8: 21 90 83 00  	addu	$18, $4, $3
  18cbfc: 00 00 51 92  	lbu	$17, 0x0($18)
  18cc00: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18cc04: e9 ff 40 10  	beqz	$2, 0x18cbac <.text+0x8cbac>
  18cc08: 63 00 02 24  	addiu	$2, $zero, 0x63
  18cc0c: 01 00 42 92  	lbu	$2, 0x1($18)
  18cc10: 04 00 52 96  	lhu	$18, 0x4($18)
  18cc14: 23 98 62 02  	subu	$19, $19, $2
  18cc18: 0a 00 20 12  	beqz	$17, 0x18cc44 <.text+0x8cc44>
  18cc1c: 16 a0 54 00  	dsrlv	$20, $20, $2
  18cc20: 08 00 62 2e  	sltiu	$2, $19, 0x8
  18cc24: 46 00 40 14  	bnez	$2, 0x18cd40 <.text+0x8cd40>
  18cc28: 42 00 10 3c  	lui	$16, 0x42
  18cc2c: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18cc30: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18cc34: f8 ff 73 26  	addiu	$19, $19, -0x8 <.text+0xffffffffffeffff8>
  18cc38: ff 00 42 30  	andi	$2, $2, 0xff
  18cc3c: 3a a2 14 00  	dsrl	$20, $20, 0x8
  18cc40: 21 90 42 02  	addu	$18, $18, $2
  18cc44: 3c 10 12 00  	dsll32	$2, $18, 0x0
  18cc48: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  18cc4c: 2f b8 e2 02  	dsubu	$23, $23, $2
  18cc50: ff 7f b5 32  	andi	$21, $21, 0x7fff
  18cc54: 2b 10 d5 02  	sltu	$2, $22, $21
  18cc58: 36 00 40 10  	beqz	$2, 0x18cd34 <.text+0x8cd34>
  18cc5c: 00 80 02 34  	ori	$2, $zero, 0x8000
  18cc60: 23 88 55 00  	subu	$17, $2, $21
  18cc64: 2b 10 51 02  	sltu	$2, $18, $17
  18cc68: 20 00 a4 8f  	lw	$4, 0x20($sp)
  18cc6c: 0b 88 42 02  	movn	$17, $18, $2
  18cc70: 04 00 80 10  	beqz	$4, 0x18cc84 <.text+0x8cc84>
  18cc74: 23 90 51 02  	subu	$18, $18, $17
  18cc78: 2b 10 b6 02  	sltu	$2, $21, $22
  18cc7c: 24 00 40 10  	beqz	$2, 0x18cd10 <.text+0x8cd10>
  18cc80: 45 00 04 3c  	lui	$4, 0x45
  18cc84: 23 10 d5 02  	subu	$2, $22, $21
  18cc88: 2b 10 51 00  	sltu	$2, $2, $17
  18cc8c: 17 00 40 10  	beqz	$2, 0x18ccec <.text+0x8ccec>
  18cc90: 45 00 04 3c  	lui	$4, 0x45
  18cc94: 45 00 02 3c  	lui	$2, 0x45
  18cc98: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18cc9c: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18cca0: 21 18 a2 02  	addu	$3, $21, $2
  18cca4: 01 00 b5 26  	addiu	$21, $21, 0x1
  18cca8: 00 00 63 90  	lbu	$3, 0x0($3)
  18ccac: 21 10 c2 02  	addu	$2, $22, $2
  18ccb0: 01 00 d6 26  	addiu	$22, $22, 0x1
  18ccb4: f7 ff 20 16  	bnez	$17, 0x18cc94 <.text+0x8cc94>
  18ccb8: 00 00 43 a0  	sb	$3, 0x0($2)
  18ccbc: 00 80 02 34  	ori	$2, $zero, 0x8000
  18ccc0: 05 00 c2 12  	beq	$22, $2, 0x18ccd8 <.text+0x8ccd8>
  18ccc4: 00 80 04 34  	ori	$4, $zero, 0x8000
  18ccc8: e2 ff 40 56  	bnezl	$18, 0x18cc54 <.text+0x8cc54>
  18cccc: ff 7f b5 32  	andi	$21, $21, 0x7fff
  18ccd0: 3e ff 00 10  	b	0x18c9cc <.text+0x8c9cc>
  18ccd4: 00 00 00 00  	nop
  18ccd8: 20 00 a0 af  	sw	$zero, 0x20($sp)
  18ccdc: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18cce0: 2d b0 00 00  	move	$22, $zero
  18cce4: f8 ff 00 10  	b	0x18ccc8 <.text+0x8ccc8>
  18cce8: 00 00 00 00  	nop
  18ccec: 2d 30 20 02  	move	$6, $17
  18ccf0: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18ccf4: 21 28 a4 02  	addu	$5, $21, $4
  18ccf8: 21 a8 b1 02  	addu	$21, $21, $17
  18ccfc: 21 20 c4 02  	addu	$4, $22, $4
  18cd00: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  18cd04: 21 b0 d1 02  	addu	$22, $22, $17
  18cd08: ed ff 00 10  	b	0x18ccc0 <.text+0x8ccc0>
  18cd0c: 00 80 02 34  	ori	$2, $zero, 0x8000
  18cd10: 2d 28 00 00  	move	$5, $zero
  18cd14: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18cd18: 2d 30 20 02  	move	$6, $17
  18cd1c: 21 20 c4 02  	addu	$4, $22, $4
  18cd20: 21 a8 b1 02  	addu	$21, $21, $17
  18cd24: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18cd28: 21 b0 d1 02  	addu	$22, $22, $17
  18cd2c: e4 ff 00 10  	b	0x18ccc0 <.text+0x8ccc0>
  18cd30: 00 80 02 34  	ori	$2, $zero, 0x8000
  18cd34: cb ff 00 10  	b	0x18cc64 <.text+0x8cc64>
  18cd38: 23 88 56 00  	subu	$17, $2, $22
  18cd3c: 42 00 10 3c  	lui	$16, 0x42
  18cd40: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18cd44: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18cd48: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18cd4c: 14 10 62 02  	dsllv	$2, $2, $19
  18cd50: 08 00 73 26  	addiu	$19, $19, 0x8
  18cd54: 08 00 63 2e  	sltiu	$3, $19, 0x8
  18cd58: f8 ff 60 14  	bnez	$3, 0x18cd3c <.text+0x8cd3c>
  18cd5c: 25 a0 82 02  	or	$20, $20, $2
  18cd60: b2 ff 00 10  	b	0x18cc2c <.text+0x8cc2c>
  18cd64: 00 00 00 00  	nop
  18cd68: 42 00 10 3c  	lui	$16, 0x42
  18cd6c: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18cd70: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18cd74: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18cd78: 14 10 62 02  	dsllv	$2, $2, $19
  18cd7c: 08 00 73 26  	addiu	$19, $19, 0x8
  18cd80: 2b 18 71 02  	sltu	$3, $19, $17
  18cd84: f8 ff 60 14  	bnez	$3, 0x18cd68 <.text+0x8cd68>
  18cd88: 25 a0 82 02  	or	$20, $20, $2
  18cd8c: 90 ff 00 10  	b	0x18cbd0 <.text+0x8cbd0>
  18cd90: 42 00 02 3c  	lui	$2, 0x42
  18cd94: 42 00 10 3c  	lui	$16, 0x42
  18cd98: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18cd9c: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18cda0: 10 00 a7 8f  	lw	$7, 0x10($sp)
  18cda4: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18cda8: 14 10 62 02  	dsllv	$2, $2, $19
  18cdac: 08 00 73 26  	addiu	$19, $19, 0x8
  18cdb0: 2b 18 67 02  	sltu	$3, $19, $7
  18cdb4: f7 ff 60 14  	bnez	$3, 0x18cd94 <.text+0x8cd94>
  18cdb8: 25 a0 82 02  	or	$20, $20, $2
  18cdbc: 6e ff 00 10  	b	0x18cb78 <.text+0x8cb78>
  18cdc0: 00 00 00 00  	nop
  18cdc4: 42 00 10 3c  	lui	$16, 0x42
  18cdc8: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18cdcc: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18cdd0: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18cdd4: 14 10 62 02  	dsllv	$2, $2, $19
  18cdd8: 08 00 73 26  	addiu	$19, $19, 0x8
  18cddc: 2b 18 71 02  	sltu	$3, $19, $17
  18cde0: f8 ff 60 14  	bnez	$3, 0x18cdc4 <.text+0x8cdc4>
  18cde4: 25 a0 82 02  	or	$20, $20, $2
  18cde8: 4b ff 00 10  	b	0x18cb18 <.text+0x8cb18>
  18cdec: 42 00 02 3c  	lui	$2, 0x42
  18cdf0: 42 00 10 3c  	lui	$16, 0x42
  18cdf4: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18cdf8: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18cdfc: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18ce00: 14 10 62 02  	dsllv	$2, $2, $19
  18ce04: 08 00 73 26  	addiu	$19, $19, 0x8
  18ce08: 2b 18 7e 02  	sltu	$3, $19, $fp
  18ce0c: f8 ff 60 14  	bnez	$3, 0x18cdf0 <.text+0x8cdf0>
  18ce10: 25 a0 82 02  	or	$20, $20, $2
  18ce14: 2a ff 00 10  	b	0x18cac0 <.text+0x8cac0>
  18ce18: 00 00 00 00  	nop
  18ce1c: 42 00 10 3c  	lui	$16, 0x42
  18ce20: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18ce24: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18ce28: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18ce2c: 14 10 62 02  	dsllv	$2, $2, $19
  18ce30: 08 00 73 26  	addiu	$19, $19, 0x8
  18ce34: 06 00 63 2e  	sltiu	$3, $19, 0x6
  18ce38: f8 ff 60 14  	bnez	$3, 0x18ce1c <.text+0x8ce1c>
  18ce3c: 25 a0 82 02  	or	$20, $20, $2
  18ce40: 19 ff 00 10  	b	0x18caa8 <.text+0x8caa8>
  18ce44: fa ff 73 26  	addiu	$19, $19, -0x6 <.text+0xffffffffffeffffa>
  18ce48: 42 00 10 3c  	lui	$16, 0x42
  18ce4c: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18ce50: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18ce54: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18ce58: 14 10 62 02  	dsllv	$2, $2, $19
  18ce5c: 08 00 73 26  	addiu	$19, $19, 0x8
  18ce60: f9 ff 60 12  	beqz	$19, 0x18ce48 <.text+0x8ce48>
  18ce64: 25 a0 82 02  	or	$20, $20, $2
  18ce68: 9e fe 00 10  	b	0x18c8e4 <.text+0x8c8e4>
  18ce6c: 08 00 13 24  	addiu	$19, $zero, 0x8
  18ce70: 40 ff bd 27  	addiu	$sp, $sp, -0xc0 <.text+0xffffffffffefff40>
  18ce74: 42 00 02 3c  	lui	$2, 0x42
  18ce78: a0 00 be ff  	sd	$fp, 0xa0($sp)
  18ce7c: 2d f0 e0 00  	move	$fp, $7
  18ce80: 80 00 b6 ff  	sd	$22, 0x80($sp)
  18ce84: 40 18 1e 00  	sll	$3, $fp, 0x1
  18ce88: 60 00 b4 ff  	sd	$20, 0x60($sp)
  18ce8c: 2d b0 00 00  	move	$22, $zero
  18ce90: 50 00 b3 ff  	sd	$19, 0x50($sp)
  18ce94: 2d a0 00 00  	move	$20, $zero
  18ce98: b0 00 bf ff  	sd	$ra, 0xb0($sp)
  18ce9c: 2d 98 00 00  	move	$19, $zero
  18cea0: 90 00 b7 ff  	sd	$23, 0x90($sp)
  18cea4: 70 00 b5 ff  	sd	$21, 0x70($sp)
  18cea8: 40 00 b2 ff  	sd	$18, 0x40($sp)
  18ceac: 30 00 b1 ff  	sd	$17, 0x30($sp)
  18ceb0: 20 00 b0 ff  	sd	$16, 0x20($sp)
  18ceb4: 08 00 a6 af  	sw	$6, 0x8($sp)
  18ceb8: 40 30 06 00  	sll	$6, $6, 0x1
  18cebc: 54 48 42 8c  	lw	$2, 0x4854($2)
  18cec0: 00 00 a4 af  	sw	$4, 0x0($sp)
  18cec4: e0 00 47 8c  	lw	$7, 0xe0($2)
  18cec8: 42 00 02 3c  	lui	$2, 0x42
  18cecc: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18ced0: 04 00 a5 af  	sw	$5, 0x4($sp)
  18ced4: 21 30 c2 00  	addu	$6, $6, $2
  18ced8: 21 18 62 00  	addu	$3, $3, $2
  18cedc: 00 00 c6 94  	lhu	$6, 0x0($6)
  18cee0: 01 00 02 24  	addiu	$2, $zero, 0x1
  18cee4: 00 00 63 94  	lhu	$3, 0x0($3)
  18cee8: 90 00 f7 dc  	ld	$23, 0x90($7)
  18ceec: 0c 00 a6 af  	sw	$6, 0xc($sp)
  18cef0: 10 00 a3 af  	sw	$3, 0x10($sp)
  18cef4: 19 00 e0 1a  	blez	$23, 0x18cf5c <.text+0x8cf5c>
  18cef8: 14 00 a2 af  	sw	$2, 0x14($sp)
  18cefc: 28 01 60 12  	beqz	$19, 0x18d3a0 <.text+0x8d3a0>
  18cf00: 42 00 10 3c  	lui	$16, 0x42
  18cf04: 01 00 82 32  	andi	$2, $20, 0x1
  18cf08: 3c 10 02 00  	dsll32	$2, $2, 0x0
  18cf0c: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18cf10: 36 00 40 10  	beqz	$2, 0x18cfec <.text+0x8cfec>
  18cf14: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  18cf18: 7a a0 14 00  	dsrl	$20, $20, 0x1
  18cf1c: 08 00 62 2e  	sltiu	$2, $19, 0x8
  18cf20: 27 00 40 14  	bnez	$2, 0x18cfc0 <.text+0x8cfc0>
  18cf24: ff ff f7 66  	daddiu	$23, $23, -0x1 <.text+0xffffffffffefffff>
  18cf28: 45 00 03 3c  	lui	$3, 0x45
  18cf2c: ff 00 82 32  	andi	$2, $20, 0xff
  18cf30: 00 82 63 24  	addiu	$3, $3, -0x7e00 <.text+0xffffffffffef8200>
  18cf34: 21 18 c3 02  	addu	$3, $22, $3
  18cf38: 01 00 d6 26  	addiu	$22, $22, 0x1
  18cf3c: 00 00 62 a0  	sb	$2, 0x0($3)
  18cf40: 00 80 02 34  	ori	$2, $zero, 0x8000
  18cf44: 19 00 c2 12  	beq	$22, $2, 0x18cfac <.text+0x8cfac>
  18cf48: 00 80 04 34  	ori	$4, $zero, 0x8000
  18cf4c: 3a a2 14 00  	dsrl	$20, $20, 0x8
  18cf50: f8 ff 73 26  	addiu	$19, $19, -0x8 <.text+0xffffffffffeffff8>
  18cf54: e9 ff e0 1e  	bgtz	$23, 0x18cefc <.text+0x8cefc>
  18cf58: 00 00 00 00  	nop
  18cf5c: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18cf60: 2d 20 c0 02  	move	$4, $22
  18cf64: 42 00 02 3c  	lui	$2, 0x42
  18cf68: 50 48 42 8c  	lw	$2, 0x4850($2)
  18cf6c: 05 00 03 24  	addiu	$3, $zero, 0x5
  18cf70: 88 00 42 dc  	ld	$2, 0x88($2)
  18cf74: 0a 18 02 00  	movz	$3, $zero, $2
  18cf78: b0 00 bf df  	ld	$ra, 0xb0($sp)
  18cf7c: 2d 10 60 00  	move	$2, $3
  18cf80: a0 00 be df  	ld	$fp, 0xa0($sp)
  18cf84: 90 00 b7 df  	ld	$23, 0x90($sp)
  18cf88: 80 00 b6 df  	ld	$22, 0x80($sp)
  18cf8c: 70 00 b5 df  	ld	$21, 0x70($sp)
  18cf90: 60 00 b4 df  	ld	$20, 0x60($sp)
  18cf94: 50 00 b3 df  	ld	$19, 0x50($sp)
  18cf98: 40 00 b2 df  	ld	$18, 0x40($sp)
  18cf9c: 30 00 b1 df  	ld	$17, 0x30($sp)
  18cfa0: 20 00 b0 df  	ld	$16, 0x20($sp)
  18cfa4: 08 00 e0 03  	jr	$ra
  18cfa8: c0 00 bd 27  	addiu	$sp, $sp, 0xc0
  18cfac: 14 00 a0 af  	sw	$zero, 0x14($sp)
  18cfb0: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18cfb4: 2d b0 00 00  	move	$22, $zero
  18cfb8: e5 ff 00 10  	b	0x18cf50 <.text+0x8cf50>
  18cfbc: 3a a2 14 00  	dsrl	$20, $20, 0x8
  18cfc0: 42 00 10 3c  	lui	$16, 0x42
  18cfc4: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18cfc8: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18cfcc: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18cfd0: 14 10 62 02  	dsllv	$2, $2, $19
  18cfd4: 08 00 73 26  	addiu	$19, $19, 0x8
  18cfd8: 08 00 63 2e  	sltiu	$3, $19, 0x8
  18cfdc: f8 ff 60 14  	bnez	$3, 0x18cfc0 <.text+0x8cfc0>
  18cfe0: 25 a0 82 02  	or	$20, $20, $2
  18cfe4: d1 ff 00 10  	b	0x18cf2c <.text+0x8cf2c>
  18cfe8: 45 00 03 3c  	lui	$3, 0x45
  18cfec: 07 00 62 2e  	sltiu	$2, $19, 0x7
  18cff0: df 00 40 14  	bnez	$2, 0x18d370 <.text+0x8d370>
  18cff4: 7a a0 14 00  	dsrl	$20, $20, 0x1
  18cff8: f9 ff 73 26  	addiu	$19, $19, -0x7 <.text+0xffffffffffeffff9>
  18cffc: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18d000: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d004: 2b 18 7e 02  	sltu	$3, $19, $fp
  18d008: 7f 00 55 30  	andi	$21, $2, 0x7f
  18d00c: cd 00 60 14  	bnez	$3, 0x18d344 <.text+0x8d344>
  18d010: fa a1 14 00  	dsrl	$20, $20, 0x7
  18d014: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18d018: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d01c: 10 00 a3 8f  	lw	$3, 0x10($sp)
  18d020: 27 10 02 00  	nor	$2, $zero, $2
  18d024: 04 00 a4 8f  	lw	$4, 0x4($sp)
  18d028: 24 10 43 00  	and	$2, $2, $3
  18d02c: c0 10 02 00  	sll	$2, $2, 0x3
  18d030: 21 90 82 00  	addu	$18, $4, $2
  18d034: 00 00 51 92  	lbu	$17, 0x0($18)
  18d038: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18d03c: 1b 00 40 54  	bnezl	$2, 0x18d0ac <.text+0x8d0ac>
  18d040: 01 00 42 92  	lbu	$2, 0x1($18)
  18d044: 63 00 02 24  	addiu	$2, $zero, 0x63
  18d048: cb ff 22 12  	beq	$17, $2, 0x18cf78 <.text+0x8cf78>
  18d04c: 01 00 03 24  	addiu	$3, $zero, 0x1
  18d050: 01 00 42 92  	lbu	$2, 0x1($18)
  18d054: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18d058: 23 98 62 02  	subu	$19, $19, $2
  18d05c: 16 a0 54 00  	dsrlv	$20, $20, $2
  18d060: 2b 10 71 02  	sltu	$2, $19, $17
  18d064: ac 00 40 14  	bnez	$2, 0x18d318 <.text+0x8d318>
  18d068: 42 00 02 3c  	lui	$2, 0x42
  18d06c: 40 20 11 00  	sll	$4, $17, 0x1
  18d070: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18d074: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18d078: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18d07c: 21 20 82 00  	addu	$4, $4, $2
  18d080: 27 18 03 00  	nor	$3, $zero, $3
  18d084: 00 00 82 94  	lhu	$2, 0x0($4)
  18d088: 04 00 44 8e  	lw	$4, 0x4($18)
  18d08c: 24 18 62 00  	and	$3, $3, $2
  18d090: c0 18 03 00  	sll	$3, $3, 0x3
  18d094: 21 90 83 00  	addu	$18, $4, $3
  18d098: 00 00 51 92  	lbu	$17, 0x0($18)
  18d09c: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18d0a0: e9 ff 40 10  	beqz	$2, 0x18d048 <.text+0x8d048>
  18d0a4: 63 00 02 24  	addiu	$2, $zero, 0x63
  18d0a8: 01 00 42 92  	lbu	$2, 0x1($18)
  18d0ac: 23 20 d5 02  	subu	$4, $22, $21
  18d0b0: 08 00 a5 8f  	lw	$5, 0x8($sp)
  18d0b4: 04 00 43 96  	lhu	$3, 0x4($18)
  18d0b8: 23 98 62 02  	subu	$19, $19, $2
  18d0bc: 16 a0 54 00  	dsrlv	$20, $20, $2
  18d0c0: 2b 10 65 02  	sltu	$2, $19, $5
  18d0c4: 88 00 40 14  	bnez	$2, 0x18d2e8 <.text+0x8d2e8>
  18d0c8: 23 a8 83 00  	subu	$21, $4, $3
  18d0cc: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18d0d0: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d0d4: 0c 00 a5 8f  	lw	$5, 0xc($sp)
  18d0d8: 27 10 02 00  	nor	$2, $zero, $2
  18d0dc: 00 00 a3 8f  	lw	$3, 0x0($sp)
  18d0e0: 24 10 45 00  	and	$2, $2, $5
  18d0e4: c0 10 02 00  	sll	$2, $2, 0x3
  18d0e8: 21 90 62 00  	addu	$18, $3, $2
  18d0ec: 00 00 51 92  	lbu	$17, 0x0($18)
  18d0f0: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18d0f4: 1b 00 40 54  	bnezl	$2, 0x18d164 <.text+0x8d164>
  18d0f8: 01 00 42 92  	lbu	$2, 0x1($18)
  18d0fc: 63 00 02 24  	addiu	$2, $zero, 0x63
  18d100: 9d ff 22 12  	beq	$17, $2, 0x18cf78 <.text+0x8cf78>
  18d104: 01 00 03 24  	addiu	$3, $zero, 0x1
  18d108: 01 00 42 92  	lbu	$2, 0x1($18)
  18d10c: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18d110: 23 98 62 02  	subu	$19, $19, $2
  18d114: 16 a0 54 00  	dsrlv	$20, $20, $2
  18d118: 2b 10 71 02  	sltu	$2, $19, $17
  18d11c: 67 00 40 14  	bnez	$2, 0x18d2bc <.text+0x8d2bc>
  18d120: 42 00 02 3c  	lui	$2, 0x42
  18d124: 40 20 11 00  	sll	$4, $17, 0x1
  18d128: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18d12c: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18d130: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18d134: 21 20 82 00  	addu	$4, $4, $2
  18d138: 27 18 03 00  	nor	$3, $zero, $3
  18d13c: 00 00 82 94  	lhu	$2, 0x0($4)
  18d140: 04 00 44 8e  	lw	$4, 0x4($18)
  18d144: 24 18 62 00  	and	$3, $3, $2
  18d148: c0 18 03 00  	sll	$3, $3, 0x3
  18d14c: 21 90 83 00  	addu	$18, $4, $3
  18d150: 00 00 51 92  	lbu	$17, 0x0($18)
  18d154: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18d158: e9 ff 40 10  	beqz	$2, 0x18d100 <.text+0x8d100>
  18d15c: 63 00 02 24  	addiu	$2, $zero, 0x63
  18d160: 01 00 42 92  	lbu	$2, 0x1($18)
  18d164: 04 00 52 96  	lhu	$18, 0x4($18)
  18d168: 23 98 62 02  	subu	$19, $19, $2
  18d16c: 0a 00 20 12  	beqz	$17, 0x18d198 <.text+0x8d198>
  18d170: 16 a0 54 00  	dsrlv	$20, $20, $2
  18d174: 08 00 62 2e  	sltiu	$2, $19, 0x8
  18d178: 46 00 40 14  	bnez	$2, 0x18d294 <.text+0x8d294>
  18d17c: 42 00 10 3c  	lui	$16, 0x42
  18d180: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18d184: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d188: f8 ff 73 26  	addiu	$19, $19, -0x8 <.text+0xffffffffffeffff8>
  18d18c: ff 00 42 30  	andi	$2, $2, 0xff
  18d190: 3a a2 14 00  	dsrl	$20, $20, 0x8
  18d194: 21 90 42 02  	addu	$18, $18, $2
  18d198: 3c 10 12 00  	dsll32	$2, $18, 0x0
  18d19c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  18d1a0: 2f b8 e2 02  	dsubu	$23, $23, $2
  18d1a4: ff 7f b5 32  	andi	$21, $21, 0x7fff
  18d1a8: 2b 10 d5 02  	sltu	$2, $22, $21
  18d1ac: 36 00 40 10  	beqz	$2, 0x18d288 <.text+0x8d288>
  18d1b0: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d1b4: 23 88 55 00  	subu	$17, $2, $21
  18d1b8: 2b 10 51 02  	sltu	$2, $18, $17
  18d1bc: 14 00 a4 8f  	lw	$4, 0x14($sp)
  18d1c0: 0b 88 42 02  	movn	$17, $18, $2
  18d1c4: 04 00 80 10  	beqz	$4, 0x18d1d8 <.text+0x8d1d8>
  18d1c8: 23 90 51 02  	subu	$18, $18, $17
  18d1cc: 2b 10 b6 02  	sltu	$2, $21, $22
  18d1d0: 24 00 40 10  	beqz	$2, 0x18d264 <.text+0x8d264>
  18d1d4: 45 00 04 3c  	lui	$4, 0x45
  18d1d8: 23 10 d5 02  	subu	$2, $22, $21
  18d1dc: 2b 10 51 00  	sltu	$2, $2, $17
  18d1e0: 17 00 40 10  	beqz	$2, 0x18d240 <.text+0x8d240>
  18d1e4: 45 00 04 3c  	lui	$4, 0x45
  18d1e8: 45 00 02 3c  	lui	$2, 0x45
  18d1ec: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18d1f0: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18d1f4: 21 18 a2 02  	addu	$3, $21, $2
  18d1f8: 01 00 b5 26  	addiu	$21, $21, 0x1
  18d1fc: 00 00 63 90  	lbu	$3, 0x0($3)
  18d200: 21 10 c2 02  	addu	$2, $22, $2
  18d204: 01 00 d6 26  	addiu	$22, $22, 0x1
  18d208: f7 ff 20 16  	bnez	$17, 0x18d1e8 <.text+0x8d1e8>
  18d20c: 00 00 43 a0  	sb	$3, 0x0($2)
  18d210: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d214: 05 00 c2 12  	beq	$22, $2, 0x18d22c <.text+0x8d22c>
  18d218: 00 80 04 34  	ori	$4, $zero, 0x8000
  18d21c: e2 ff 40 56  	bnezl	$18, 0x18d1a8 <.text+0x8d1a8>
  18d220: ff 7f b5 32  	andi	$21, $21, 0x7fff
  18d224: 4b ff 00 10  	b	0x18cf54 <.text+0x8cf54>
  18d228: 00 00 00 00  	nop
  18d22c: 14 00 a0 af  	sw	$zero, 0x14($sp)
  18d230: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18d234: 2d b0 00 00  	move	$22, $zero
  18d238: f8 ff 00 10  	b	0x18d21c <.text+0x8d21c>
  18d23c: 00 00 00 00  	nop
  18d240: 2d 30 20 02  	move	$6, $17
  18d244: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18d248: 21 28 a4 02  	addu	$5, $21, $4
  18d24c: 21 a8 b1 02  	addu	$21, $21, $17
  18d250: 21 20 c4 02  	addu	$4, $22, $4
  18d254: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  18d258: 21 b0 d1 02  	addu	$22, $22, $17
  18d25c: ed ff 00 10  	b	0x18d214 <.text+0x8d214>
  18d260: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d264: 2d 28 00 00  	move	$5, $zero
  18d268: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18d26c: 2d 30 20 02  	move	$6, $17
  18d270: 21 20 c4 02  	addu	$4, $22, $4
  18d274: 21 a8 b1 02  	addu	$21, $21, $17
  18d278: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18d27c: 21 b0 d1 02  	addu	$22, $22, $17
  18d280: e4 ff 00 10  	b	0x18d214 <.text+0x8d214>
  18d284: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d288: cb ff 00 10  	b	0x18d1b8 <.text+0x8d1b8>
  18d28c: 23 88 56 00  	subu	$17, $2, $22
  18d290: 42 00 10 3c  	lui	$16, 0x42
  18d294: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d298: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d29c: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d2a0: 14 10 62 02  	dsllv	$2, $2, $19
  18d2a4: 08 00 73 26  	addiu	$19, $19, 0x8
  18d2a8: 08 00 63 2e  	sltiu	$3, $19, 0x8
  18d2ac: f8 ff 60 14  	bnez	$3, 0x18d290 <.text+0x8d290>
  18d2b0: 25 a0 82 02  	or	$20, $20, $2
  18d2b4: b2 ff 00 10  	b	0x18d180 <.text+0x8d180>
  18d2b8: 00 00 00 00  	nop
  18d2bc: 42 00 10 3c  	lui	$16, 0x42
  18d2c0: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d2c4: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d2c8: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d2cc: 14 10 62 02  	dsllv	$2, $2, $19
  18d2d0: 08 00 73 26  	addiu	$19, $19, 0x8
  18d2d4: 2b 18 71 02  	sltu	$3, $19, $17
  18d2d8: f8 ff 60 14  	bnez	$3, 0x18d2bc <.text+0x8d2bc>
  18d2dc: 25 a0 82 02  	or	$20, $20, $2
  18d2e0: 90 ff 00 10  	b	0x18d124 <.text+0x8d124>
  18d2e4: 42 00 02 3c  	lui	$2, 0x42
  18d2e8: 42 00 10 3c  	lui	$16, 0x42
  18d2ec: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d2f0: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d2f4: 08 00 a4 8f  	lw	$4, 0x8($sp)
  18d2f8: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d2fc: 14 10 62 02  	dsllv	$2, $2, $19
  18d300: 08 00 73 26  	addiu	$19, $19, 0x8
  18d304: 2b 18 64 02  	sltu	$3, $19, $4
  18d308: f7 ff 60 14  	bnez	$3, 0x18d2e8 <.text+0x8d2e8>
  18d30c: 25 a0 82 02  	or	$20, $20, $2
  18d310: 6e ff 00 10  	b	0x18d0cc <.text+0x8d0cc>
  18d314: 00 00 00 00  	nop
  18d318: 42 00 10 3c  	lui	$16, 0x42
  18d31c: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d320: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d324: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d328: 14 10 62 02  	dsllv	$2, $2, $19
  18d32c: 08 00 73 26  	addiu	$19, $19, 0x8
  18d330: 2b 18 71 02  	sltu	$3, $19, $17
  18d334: f8 ff 60 14  	bnez	$3, 0x18d318 <.text+0x8d318>
  18d338: 25 a0 82 02  	or	$20, $20, $2
  18d33c: 4b ff 00 10  	b	0x18d06c <.text+0x8d06c>
  18d340: 42 00 02 3c  	lui	$2, 0x42
  18d344: 42 00 10 3c  	lui	$16, 0x42
  18d348: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d34c: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d350: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d354: 14 10 62 02  	dsllv	$2, $2, $19
  18d358: 08 00 73 26  	addiu	$19, $19, 0x8
  18d35c: 2b 18 7e 02  	sltu	$3, $19, $fp
  18d360: f8 ff 60 14  	bnez	$3, 0x18d344 <.text+0x8d344>
  18d364: 25 a0 82 02  	or	$20, $20, $2
  18d368: 2a ff 00 10  	b	0x18d014 <.text+0x8d014>
  18d36c: 00 00 00 00  	nop
  18d370: 42 00 10 3c  	lui	$16, 0x42
  18d374: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d378: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d37c: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d380: 14 10 62 02  	dsllv	$2, $2, $19
  18d384: 08 00 73 26  	addiu	$19, $19, 0x8
  18d388: 07 00 63 2e  	sltiu	$3, $19, 0x7
  18d38c: f8 ff 60 14  	bnez	$3, 0x18d370 <.text+0x8d370>
  18d390: 25 a0 82 02  	or	$20, $20, $2
  18d394: 19 ff 00 10  	b	0x18cffc <.text+0x8cffc>
  18d398: f9 ff 73 26  	addiu	$19, $19, -0x7 <.text+0xffffffffffeffff9>
  18d39c: 42 00 10 3c  	lui	$16, 0x42
  18d3a0: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d3a4: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d3a8: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d3ac: 14 10 62 02  	dsllv	$2, $2, $19
  18d3b0: 08 00 73 26  	addiu	$19, $19, 0x8
  18d3b4: f9 ff 60 12  	beqz	$19, 0x18d39c <.text+0x8d39c>
  18d3b8: 25 a0 82 02  	or	$20, $20, $2
  18d3bc: d1 fe 00 10  	b	0x18cf04 <.text+0x8cf04>
  18d3c0: 08 00 13 24  	addiu	$19, $zero, 0x8
  18d3c4: 40 ff bd 27  	addiu	$sp, $sp, -0xc0 <.text+0xffffffffffefff40>
  18d3c8: 42 00 02 3c  	lui	$2, 0x42
  18d3cc: a0 00 be ff  	sd	$fp, 0xa0($sp)
  18d3d0: 2d f0 e0 00  	move	$fp, $7
  18d3d4: 80 00 b6 ff  	sd	$22, 0x80($sp)
  18d3d8: 40 18 1e 00  	sll	$3, $fp, 0x1
  18d3dc: 60 00 b4 ff  	sd	$20, 0x60($sp)
  18d3e0: 2d b0 00 00  	move	$22, $zero
  18d3e4: 50 00 b3 ff  	sd	$19, 0x50($sp)
  18d3e8: 2d a0 00 00  	move	$20, $zero
  18d3ec: b0 00 bf ff  	sd	$ra, 0xb0($sp)
  18d3f0: 2d 98 00 00  	move	$19, $zero
  18d3f4: 90 00 b7 ff  	sd	$23, 0x90($sp)
  18d3f8: 70 00 b5 ff  	sd	$21, 0x70($sp)
  18d3fc: 40 00 b2 ff  	sd	$18, 0x40($sp)
  18d400: 30 00 b1 ff  	sd	$17, 0x30($sp)
  18d404: 20 00 b0 ff  	sd	$16, 0x20($sp)
  18d408: 08 00 a6 af  	sw	$6, 0x8($sp)
  18d40c: 40 30 06 00  	sll	$6, $6, 0x1
  18d410: 54 48 42 8c  	lw	$2, 0x4854($2)
  18d414: 00 00 a4 af  	sw	$4, 0x0($sp)
  18d418: e0 00 47 8c  	lw	$7, 0xe0($2)
  18d41c: 42 00 02 3c  	lui	$2, 0x42
  18d420: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18d424: 04 00 a5 af  	sw	$5, 0x4($sp)
  18d428: 21 30 c2 00  	addu	$6, $6, $2
  18d42c: 21 18 62 00  	addu	$3, $3, $2
  18d430: 00 00 c6 94  	lhu	$6, 0x0($6)
  18d434: 01 00 02 24  	addiu	$2, $zero, 0x1
  18d438: 00 00 63 94  	lhu	$3, 0x0($3)
  18d43c: 90 00 f7 dc  	ld	$23, 0x90($7)
  18d440: 0c 00 a6 af  	sw	$6, 0xc($sp)
  18d444: 10 00 a3 af  	sw	$3, 0x10($sp)
  18d448: 19 00 e0 1a  	blez	$23, 0x18d4b0 <.text+0x8d4b0>
  18d44c: 14 00 a2 af  	sw	$2, 0x14($sp)
  18d450: 28 01 60 12  	beqz	$19, 0x18d8f4 <.text+0x8d8f4>
  18d454: 42 00 10 3c  	lui	$16, 0x42
  18d458: 01 00 82 32  	andi	$2, $20, 0x1
  18d45c: 3c 10 02 00  	dsll32	$2, $2, 0x0
  18d460: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d464: 36 00 40 10  	beqz	$2, 0x18d540 <.text+0x8d540>
  18d468: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  18d46c: 7a a0 14 00  	dsrl	$20, $20, 0x1
  18d470: 08 00 62 2e  	sltiu	$2, $19, 0x8
  18d474: 27 00 40 14  	bnez	$2, 0x18d514 <.text+0x8d514>
  18d478: ff ff f7 66  	daddiu	$23, $23, -0x1 <.text+0xffffffffffefffff>
  18d47c: 45 00 03 3c  	lui	$3, 0x45
  18d480: ff 00 82 32  	andi	$2, $20, 0xff
  18d484: 00 82 63 24  	addiu	$3, $3, -0x7e00 <.text+0xffffffffffef8200>
  18d488: 21 18 c3 02  	addu	$3, $22, $3
  18d48c: 01 00 d6 26  	addiu	$22, $22, 0x1
  18d490: 00 00 62 a0  	sb	$2, 0x0($3)
  18d494: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d498: 19 00 c2 12  	beq	$22, $2, 0x18d500 <.text+0x8d500>
  18d49c: 00 80 04 34  	ori	$4, $zero, 0x8000
  18d4a0: 3a a2 14 00  	dsrl	$20, $20, 0x8
  18d4a4: f8 ff 73 26  	addiu	$19, $19, -0x8 <.text+0xffffffffffeffff8>
  18d4a8: e9 ff e0 1e  	bgtz	$23, 0x18d450 <.text+0x8d450>
  18d4ac: 00 00 00 00  	nop
  18d4b0: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18d4b4: 2d 20 c0 02  	move	$4, $22
  18d4b8: 42 00 02 3c  	lui	$2, 0x42
  18d4bc: 50 48 42 8c  	lw	$2, 0x4850($2)
  18d4c0: 05 00 03 24  	addiu	$3, $zero, 0x5
  18d4c4: 88 00 42 dc  	ld	$2, 0x88($2)
  18d4c8: 0a 18 02 00  	movz	$3, $zero, $2
  18d4cc: b0 00 bf df  	ld	$ra, 0xb0($sp)
  18d4d0: 2d 10 60 00  	move	$2, $3
  18d4d4: a0 00 be df  	ld	$fp, 0xa0($sp)
  18d4d8: 90 00 b7 df  	ld	$23, 0x90($sp)
  18d4dc: 80 00 b6 df  	ld	$22, 0x80($sp)
  18d4e0: 70 00 b5 df  	ld	$21, 0x70($sp)
  18d4e4: 60 00 b4 df  	ld	$20, 0x60($sp)
  18d4e8: 50 00 b3 df  	ld	$19, 0x50($sp)
  18d4ec: 40 00 b2 df  	ld	$18, 0x40($sp)
  18d4f0: 30 00 b1 df  	ld	$17, 0x30($sp)
  18d4f4: 20 00 b0 df  	ld	$16, 0x20($sp)
  18d4f8: 08 00 e0 03  	jr	$ra
  18d4fc: c0 00 bd 27  	addiu	$sp, $sp, 0xc0
  18d500: 14 00 a0 af  	sw	$zero, 0x14($sp)
  18d504: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18d508: 2d b0 00 00  	move	$22, $zero
  18d50c: e5 ff 00 10  	b	0x18d4a4 <.text+0x8d4a4>
  18d510: 3a a2 14 00  	dsrl	$20, $20, 0x8
  18d514: 42 00 10 3c  	lui	$16, 0x42
  18d518: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d51c: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d520: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d524: 14 10 62 02  	dsllv	$2, $2, $19
  18d528: 08 00 73 26  	addiu	$19, $19, 0x8
  18d52c: 08 00 63 2e  	sltiu	$3, $19, 0x8
  18d530: f8 ff 60 14  	bnez	$3, 0x18d514 <.text+0x8d514>
  18d534: 25 a0 82 02  	or	$20, $20, $2
  18d538: d1 ff 00 10  	b	0x18d480 <.text+0x8d480>
  18d53c: 45 00 03 3c  	lui	$3, 0x45
  18d540: 06 00 62 2e  	sltiu	$2, $19, 0x6
  18d544: df 00 40 14  	bnez	$2, 0x18d8c4 <.text+0x8d8c4>
  18d548: 7a a0 14 00  	dsrl	$20, $20, 0x1
  18d54c: fa ff 73 26  	addiu	$19, $19, -0x6 <.text+0xffffffffffeffffa>
  18d550: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18d554: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d558: 2b 18 7e 02  	sltu	$3, $19, $fp
  18d55c: 3f 00 55 30  	andi	$21, $2, 0x3f
  18d560: cd 00 60 14  	bnez	$3, 0x18d898 <.text+0x8d898>
  18d564: ba a1 14 00  	dsrl	$20, $20, 0x6
  18d568: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18d56c: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d570: 10 00 a3 8f  	lw	$3, 0x10($sp)
  18d574: 27 10 02 00  	nor	$2, $zero, $2
  18d578: 04 00 a4 8f  	lw	$4, 0x4($sp)
  18d57c: 24 10 43 00  	and	$2, $2, $3
  18d580: c0 10 02 00  	sll	$2, $2, 0x3
  18d584: 21 90 82 00  	addu	$18, $4, $2
  18d588: 00 00 51 92  	lbu	$17, 0x0($18)
  18d58c: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18d590: 1b 00 40 54  	bnezl	$2, 0x18d600 <.text+0x8d600>
  18d594: 01 00 42 92  	lbu	$2, 0x1($18)
  18d598: 63 00 02 24  	addiu	$2, $zero, 0x63
  18d59c: cb ff 22 12  	beq	$17, $2, 0x18d4cc <.text+0x8d4cc>
  18d5a0: 01 00 03 24  	addiu	$3, $zero, 0x1
  18d5a4: 01 00 42 92  	lbu	$2, 0x1($18)
  18d5a8: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18d5ac: 23 98 62 02  	subu	$19, $19, $2
  18d5b0: 16 a0 54 00  	dsrlv	$20, $20, $2
  18d5b4: 2b 10 71 02  	sltu	$2, $19, $17
  18d5b8: ac 00 40 14  	bnez	$2, 0x18d86c <.text+0x8d86c>
  18d5bc: 42 00 02 3c  	lui	$2, 0x42
  18d5c0: 40 20 11 00  	sll	$4, $17, 0x1
  18d5c4: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18d5c8: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18d5cc: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18d5d0: 21 20 82 00  	addu	$4, $4, $2
  18d5d4: 27 18 03 00  	nor	$3, $zero, $3
  18d5d8: 00 00 82 94  	lhu	$2, 0x0($4)
  18d5dc: 04 00 44 8e  	lw	$4, 0x4($18)
  18d5e0: 24 18 62 00  	and	$3, $3, $2
  18d5e4: c0 18 03 00  	sll	$3, $3, 0x3
  18d5e8: 21 90 83 00  	addu	$18, $4, $3
  18d5ec: 00 00 51 92  	lbu	$17, 0x0($18)
  18d5f0: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18d5f4: e9 ff 40 10  	beqz	$2, 0x18d59c <.text+0x8d59c>
  18d5f8: 63 00 02 24  	addiu	$2, $zero, 0x63
  18d5fc: 01 00 42 92  	lbu	$2, 0x1($18)
  18d600: 23 20 d5 02  	subu	$4, $22, $21
  18d604: 08 00 a5 8f  	lw	$5, 0x8($sp)
  18d608: 04 00 43 96  	lhu	$3, 0x4($18)
  18d60c: 23 98 62 02  	subu	$19, $19, $2
  18d610: 16 a0 54 00  	dsrlv	$20, $20, $2
  18d614: 2b 10 65 02  	sltu	$2, $19, $5
  18d618: 88 00 40 14  	bnez	$2, 0x18d83c <.text+0x8d83c>
  18d61c: 23 a8 83 00  	subu	$21, $4, $3
  18d620: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18d624: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d628: 0c 00 a5 8f  	lw	$5, 0xc($sp)
  18d62c: 27 10 02 00  	nor	$2, $zero, $2
  18d630: 00 00 a3 8f  	lw	$3, 0x0($sp)
  18d634: 24 10 45 00  	and	$2, $2, $5
  18d638: c0 10 02 00  	sll	$2, $2, 0x3
  18d63c: 21 90 62 00  	addu	$18, $3, $2
  18d640: 00 00 51 92  	lbu	$17, 0x0($18)
  18d644: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18d648: 1b 00 40 54  	bnezl	$2, 0x18d6b8 <.text+0x8d6b8>
  18d64c: 01 00 42 92  	lbu	$2, 0x1($18)
  18d650: 63 00 02 24  	addiu	$2, $zero, 0x63
  18d654: 9d ff 22 12  	beq	$17, $2, 0x18d4cc <.text+0x8d4cc>
  18d658: 01 00 03 24  	addiu	$3, $zero, 0x1
  18d65c: 01 00 42 92  	lbu	$2, 0x1($18)
  18d660: f0 ff 31 26  	addiu	$17, $17, -0x10 <.text+0xffffffffffeffff0>
  18d664: 23 98 62 02  	subu	$19, $19, $2
  18d668: 16 a0 54 00  	dsrlv	$20, $20, $2
  18d66c: 2b 10 71 02  	sltu	$2, $19, $17
  18d670: 67 00 40 14  	bnez	$2, 0x18d810 <.text+0x8d810>
  18d674: 42 00 02 3c  	lui	$2, 0x42
  18d678: 40 20 11 00  	sll	$4, $17, 0x1
  18d67c: d8 43 42 24  	addiu	$2, $2, 0x43d8
  18d680: 3c 18 14 00  	dsll32	$3, $20, 0x0
  18d684: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18d688: 21 20 82 00  	addu	$4, $4, $2
  18d68c: 27 18 03 00  	nor	$3, $zero, $3
  18d690: 00 00 82 94  	lhu	$2, 0x0($4)
  18d694: 04 00 44 8e  	lw	$4, 0x4($18)
  18d698: 24 18 62 00  	and	$3, $3, $2
  18d69c: c0 18 03 00  	sll	$3, $3, 0x3
  18d6a0: 21 90 83 00  	addu	$18, $4, $3
  18d6a4: 00 00 51 92  	lbu	$17, 0x0($18)
  18d6a8: 11 00 22 2e  	sltiu	$2, $17, 0x11
  18d6ac: e9 ff 40 10  	beqz	$2, 0x18d654 <.text+0x8d654>
  18d6b0: 63 00 02 24  	addiu	$2, $zero, 0x63
  18d6b4: 01 00 42 92  	lbu	$2, 0x1($18)
  18d6b8: 04 00 52 96  	lhu	$18, 0x4($18)
  18d6bc: 23 98 62 02  	subu	$19, $19, $2
  18d6c0: 0a 00 20 12  	beqz	$17, 0x18d6ec <.text+0x8d6ec>
  18d6c4: 16 a0 54 00  	dsrlv	$20, $20, $2
  18d6c8: 08 00 62 2e  	sltiu	$2, $19, 0x8
  18d6cc: 46 00 40 14  	bnez	$2, 0x18d7e8 <.text+0x8d7e8>
  18d6d0: 42 00 10 3c  	lui	$16, 0x42
  18d6d4: 3c 10 14 00  	dsll32	$2, $20, 0x0
  18d6d8: 3f 10 02 00  	dsra32	$2, $2, 0x0
  18d6dc: f8 ff 73 26  	addiu	$19, $19, -0x8 <.text+0xffffffffffeffff8>
  18d6e0: ff 00 42 30  	andi	$2, $2, 0xff
  18d6e4: 3a a2 14 00  	dsrl	$20, $20, 0x8
  18d6e8: 21 90 42 02  	addu	$18, $18, $2
  18d6ec: 3c 10 12 00  	dsll32	$2, $18, 0x0
  18d6f0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  18d6f4: 2f b8 e2 02  	dsubu	$23, $23, $2
  18d6f8: ff 7f b5 32  	andi	$21, $21, 0x7fff
  18d6fc: 2b 10 d5 02  	sltu	$2, $22, $21
  18d700: 36 00 40 10  	beqz	$2, 0x18d7dc <.text+0x8d7dc>
  18d704: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d708: 23 88 55 00  	subu	$17, $2, $21
  18d70c: 2b 10 51 02  	sltu	$2, $18, $17
  18d710: 14 00 a4 8f  	lw	$4, 0x14($sp)
  18d714: 0b 88 42 02  	movn	$17, $18, $2
  18d718: 04 00 80 10  	beqz	$4, 0x18d72c <.text+0x8d72c>
  18d71c: 23 90 51 02  	subu	$18, $18, $17
  18d720: 2b 10 b6 02  	sltu	$2, $21, $22
  18d724: 24 00 40 10  	beqz	$2, 0x18d7b8 <.text+0x8d7b8>
  18d728: 45 00 04 3c  	lui	$4, 0x45
  18d72c: 23 10 d5 02  	subu	$2, $22, $21
  18d730: 2b 10 51 00  	sltu	$2, $2, $17
  18d734: 17 00 40 10  	beqz	$2, 0x18d794 <.text+0x8d794>
  18d738: 45 00 04 3c  	lui	$4, 0x45
  18d73c: 45 00 02 3c  	lui	$2, 0x45
  18d740: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18d744: 00 82 42 24  	addiu	$2, $2, -0x7e00 <.text+0xffffffffffef8200>
  18d748: 21 18 a2 02  	addu	$3, $21, $2
  18d74c: 01 00 b5 26  	addiu	$21, $21, 0x1
  18d750: 00 00 63 90  	lbu	$3, 0x0($3)
  18d754: 21 10 c2 02  	addu	$2, $22, $2
  18d758: 01 00 d6 26  	addiu	$22, $22, 0x1
  18d75c: f7 ff 20 16  	bnez	$17, 0x18d73c <.text+0x8d73c>
  18d760: 00 00 43 a0  	sb	$3, 0x0($2)
  18d764: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d768: 05 00 c2 12  	beq	$22, $2, 0x18d780 <.text+0x8d780>
  18d76c: 00 80 04 34  	ori	$4, $zero, 0x8000
  18d770: e2 ff 40 56  	bnezl	$18, 0x18d6fc <.text+0x8d6fc>
  18d774: ff 7f b5 32  	andi	$21, $21, 0x7fff
  18d778: 4b ff 00 10  	b	0x18d4a8 <.text+0x8d4a8>
  18d77c: 00 00 00 00  	nop
  18d780: 14 00 a0 af  	sw	$zero, 0x14($sp)
  18d784: c6 38 06 0c  	jal	0x18e318 <.text+0x8e318>
  18d788: 2d b0 00 00  	move	$22, $zero
  18d78c: f8 ff 00 10  	b	0x18d770 <.text+0x8d770>
  18d790: 00 00 00 00  	nop
  18d794: 2d 30 20 02  	move	$6, $17
  18d798: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18d79c: 21 28 a4 02  	addu	$5, $21, $4
  18d7a0: 21 a8 b1 02  	addu	$21, $21, $17
  18d7a4: 21 20 c4 02  	addu	$4, $22, $4
  18d7a8: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  18d7ac: 21 b0 d1 02  	addu	$22, $22, $17
  18d7b0: ed ff 00 10  	b	0x18d768 <.text+0x8d768>
  18d7b4: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d7b8: 2d 28 00 00  	move	$5, $zero
  18d7bc: 00 82 84 24  	addiu	$4, $4, -0x7e00 <.text+0xffffffffffef8200>
  18d7c0: 2d 30 20 02  	move	$6, $17
  18d7c4: 21 20 c4 02  	addu	$4, $22, $4
  18d7c8: 21 a8 b1 02  	addu	$21, $21, $17
  18d7cc: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18d7d0: 21 b0 d1 02  	addu	$22, $22, $17
  18d7d4: e4 ff 00 10  	b	0x18d768 <.text+0x8d768>
  18d7d8: 00 80 02 34  	ori	$2, $zero, 0x8000
  18d7dc: cb ff 00 10  	b	0x18d70c <.text+0x8d70c>
  18d7e0: 23 88 56 00  	subu	$17, $2, $22
  18d7e4: 42 00 10 3c  	lui	$16, 0x42
  18d7e8: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d7ec: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d7f0: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d7f4: 14 10 62 02  	dsllv	$2, $2, $19
  18d7f8: 08 00 73 26  	addiu	$19, $19, 0x8
  18d7fc: 08 00 63 2e  	sltiu	$3, $19, 0x8
  18d800: f8 ff 60 14  	bnez	$3, 0x18d7e4 <.text+0x8d7e4>
  18d804: 25 a0 82 02  	or	$20, $20, $2
  18d808: b2 ff 00 10  	b	0x18d6d4 <.text+0x8d6d4>
  18d80c: 00 00 00 00  	nop
  18d810: 42 00 10 3c  	lui	$16, 0x42
  18d814: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d818: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d81c: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d820: 14 10 62 02  	dsllv	$2, $2, $19
  18d824: 08 00 73 26  	addiu	$19, $19, 0x8
  18d828: 2b 18 71 02  	sltu	$3, $19, $17
  18d82c: f8 ff 60 14  	bnez	$3, 0x18d810 <.text+0x8d810>
  18d830: 25 a0 82 02  	or	$20, $20, $2
  18d834: 90 ff 00 10  	b	0x18d678 <.text+0x8d678>
  18d838: 42 00 02 3c  	lui	$2, 0x42
  18d83c: 42 00 10 3c  	lui	$16, 0x42
  18d840: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d844: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d848: 08 00 a4 8f  	lw	$4, 0x8($sp)
  18d84c: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d850: 14 10 62 02  	dsllv	$2, $2, $19
  18d854: 08 00 73 26  	addiu	$19, $19, 0x8
  18d858: 2b 18 64 02  	sltu	$3, $19, $4
  18d85c: f7 ff 60 14  	bnez	$3, 0x18d83c <.text+0x8d83c>
  18d860: 25 a0 82 02  	or	$20, $20, $2
  18d864: 6e ff 00 10  	b	0x18d620 <.text+0x8d620>
  18d868: 00 00 00 00  	nop
  18d86c: 42 00 10 3c  	lui	$16, 0x42
  18d870: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d874: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d878: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d87c: 14 10 62 02  	dsllv	$2, $2, $19
  18d880: 08 00 73 26  	addiu	$19, $19, 0x8
  18d884: 2b 18 71 02  	sltu	$3, $19, $17
  18d888: f8 ff 60 14  	bnez	$3, 0x18d86c <.text+0x8d86c>
  18d88c: 25 a0 82 02  	or	$20, $20, $2
  18d890: 4b ff 00 10  	b	0x18d5c0 <.text+0x8d5c0>
  18d894: 42 00 02 3c  	lui	$2, 0x42
  18d898: 42 00 10 3c  	lui	$16, 0x42
  18d89c: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d8a0: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d8a4: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d8a8: 14 10 62 02  	dsllv	$2, $2, $19
  18d8ac: 08 00 73 26  	addiu	$19, $19, 0x8
  18d8b0: 2b 18 7e 02  	sltu	$3, $19, $fp
  18d8b4: f8 ff 60 14  	bnez	$3, 0x18d898 <.text+0x8d898>
  18d8b8: 25 a0 82 02  	or	$20, $20, $2
  18d8bc: 2a ff 00 10  	b	0x18d568 <.text+0x8d568>
  18d8c0: 00 00 00 00  	nop
  18d8c4: 42 00 10 3c  	lui	$16, 0x42
  18d8c8: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d8cc: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d8d0: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d8d4: 14 10 62 02  	dsllv	$2, $2, $19
  18d8d8: 08 00 73 26  	addiu	$19, $19, 0x8
  18d8dc: 06 00 63 2e  	sltiu	$3, $19, 0x6
  18d8e0: f8 ff 60 14  	bnez	$3, 0x18d8c4 <.text+0x8d8c4>
  18d8e4: 25 a0 82 02  	or	$20, $20, $2
  18d8e8: 19 ff 00 10  	b	0x18d550 <.text+0x8d550>
  18d8ec: fa ff 73 26  	addiu	$19, $19, -0x6 <.text+0xffffffffffeffffa>
  18d8f0: 42 00 10 3c  	lui	$16, 0x42
  18d8f4: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18d8f8: 28 6e 04 26  	addiu	$4, $16, 0x6e28
  18d8fc: 28 6e 02 96  	lhu	$2, 0x6e28($16)
  18d900: 14 10 62 02  	dsllv	$2, $2, $19
  18d904: 08 00 73 26  	addiu	$19, $19, 0x8
  18d908: f9 ff 60 12  	beqz	$19, 0x18d8f0 <.text+0x8d8f0>
  18d90c: 25 a0 82 02  	or	$20, $20, $2
  18d910: d1 fe 00 10  	b	0x18d458 <.text+0x8d458>
  18d914: 08 00 13 24  	addiu	$19, $zero, 0x8
  18d918: a0 fb bd 27  	addiu	$sp, $sp, -0x460 <.text+0xffffffffffeffba0>
  18d91c: 07 00 06 24  	addiu	$6, $zero, 0x7
  18d920: 40 04 b2 ff  	sd	$18, 0x440($sp)
  18d924: 08 00 04 24  	addiu	$4, $zero, 0x8
  18d928: 42 00 12 3c  	lui	$18, 0x42
  18d92c: 50 04 bf ff  	sd	$ra, 0x450($sp)
  18d930: 30 04 b1 ff  	sd	$17, 0x430($sp)
  18d934: 20 04 b0 ff  	sd	$16, 0x420($sp)
  18d938: 54 48 43 8e  	lw	$3, 0x4854($18)
  18d93c: e0 00 62 8c  	lw	$2, 0xe0($3)
  18d940: 60 00 63 dc  	ld	$3, 0x60($3)
  18d944: 88 00 45 dc  	ld	$5, 0x88($2)
  18d948: 42 00 02 3c  	lui	$2, 0x42
  18d94c: 18 6e 40 ac  	sw	$zero, 0x6e18($2)
  18d950: 04 00 63 30  	andi	$3, $3, 0x4
  18d954: 03 00 02 3c  	lui	$2, 0x3
  18d958: 40 0d 42 34  	ori	$2, $2, 0xd40
  18d95c: 0c 04 a6 af  	sw	$6, 0x40c($sp)
  18d960: 2b 10 45 00  	sltu	$2, $2, $5
  18d964: 0a 20 c2 00  	movz	$4, $6, $2
  18d968: 72 00 60 10  	beqz	$3, 0x18db34 <.text+0x8db34>
  18d96c: 14 04 a4 af  	sw	$4, 0x414($sp)
  18d970: 09 00 02 24  	addiu	$2, $zero, 0x9
  18d974: 2d 20 a0 03  	move	$4, $sp
  18d978: 00 01 05 24  	addiu	$5, $zero, 0x100
  18d97c: 49 30 06 0c  	jal	0x18c124 <.text+0x8c124>
  18d980: 04 04 a2 af  	sw	$2, 0x404($sp)
  18d984: 08 00 40 10  	beqz	$2, 0x18d9a8 <.text+0x8d9a8>
  18d988: 2d 80 40 00  	move	$16, $2
  18d98c: 2d 10 00 02  	move	$2, $16
  18d990: 50 04 bf df  	ld	$ra, 0x450($sp)
  18d994: 40 04 b2 df  	ld	$18, 0x440($sp)
  18d998: 30 04 b1 df  	ld	$17, 0x430($sp)
  18d99c: 20 04 b0 df  	ld	$16, 0x420($sp)
  18d9a0: 08 00 e0 03  	jr	$ra
  18d9a4: 60 04 bd 27  	addiu	$sp, $sp, 0x460
  18d9a8: 2d 20 a0 03  	move	$4, $sp
  18d9ac: 00 01 05 24  	addiu	$5, $zero, 0x100
  18d9b0: 00 01 06 24  	addiu	$6, $zero, 0x100
  18d9b4: 2d 38 00 00  	move	$7, $zero
  18d9b8: 2d 40 00 00  	move	$8, $zero
  18d9bc: 00 04 a9 27  	addiu	$9, $sp, 0x400
  18d9c0: 5b 37 06 0c  	jal	0x18dd6c <.text+0x8dd6c>
  18d9c4: 04 04 aa 27  	addiu	$10, $sp, 0x404
  18d9c8: 09 00 40 10  	beqz	$2, 0x18d9f0 <.text+0x8d9f0>
  18d9cc: 2d 80 40 00  	move	$16, $2
  18d9d0: 01 00 02 24  	addiu	$2, $zero, 0x1
  18d9d4: ee ff 02 56  	bnel	$16, $2, 0x18d990 <.text+0x8d990>
  18d9d8: 2d 10 00 02  	move	$2, $16
  18d9dc: 00 04 a4 8f  	lw	$4, 0x400($sp)
  18d9e0: b8 38 06 0c  	jal	0x18e2e0 <.text+0x8e2e0>
  18d9e4: 00 00 00 00  	nop
  18d9e8: e9 ff 00 10  	b	0x18d990 <.text+0x8d990>
  18d9ec: 2d 10 00 02  	move	$2, $16
  18d9f0: 2d 20 a0 03  	move	$4, $sp
  18d9f4: 49 30 06 0c  	jal	0x18c124 <.text+0x8c124>
  18d9f8: 40 00 05 24  	addiu	$5, $zero, 0x40
  18d9fc: e3 ff 40 14  	bnez	$2, 0x18d98c <.text+0x8d98c>
  18da00: 2d 80 40 00  	move	$16, $2
  18da04: 42 00 02 3c  	lui	$2, 0x42
  18da08: 42 00 07 3c  	lui	$7, 0x42
  18da0c: 58 42 51 24  	addiu	$17, $2, 0x4258
  18da10: d8 41 e7 24  	addiu	$7, $7, 0x41d8
  18da14: 2d 20 a0 03  	move	$4, $sp
  18da18: 40 00 05 24  	addiu	$5, $zero, 0x40
  18da1c: 2d 30 00 00  	move	$6, $zero
  18da20: 2d 40 20 02  	move	$8, $17
  18da24: 08 04 a9 27  	addiu	$9, $sp, 0x408
  18da28: 5b 37 06 0c  	jal	0x18dd6c <.text+0x8dd6c>
  18da2c: 0c 04 aa 27  	addiu	$10, $sp, 0x40c
  18da30: 08 00 40 10  	beqz	$2, 0x18da54 <.text+0x8da54>
  18da34: 2d 80 40 00  	move	$16, $2
  18da38: 01 00 02 24  	addiu	$2, $zero, 0x1
  18da3c: e8 ff 02 56  	bnel	$16, $2, 0x18d9e0 <.text+0x8d9e0>
  18da40: 00 04 a4 8f  	lw	$4, 0x400($sp)
  18da44: b8 38 06 0c  	jal	0x18e2e0 <.text+0x8e2e0>
  18da48: 08 04 a4 8f  	lw	$4, 0x408($sp)
  18da4c: e4 ff 00 10  	b	0x18d9e0 <.text+0x8d9e0>
  18da50: 00 04 a4 8f  	lw	$4, 0x400($sp)
  18da54: 2d 20 a0 03  	move	$4, $sp
  18da58: 49 30 06 0c  	jal	0x18c124 <.text+0x8c124>
  18da5c: 40 00 05 24  	addiu	$5, $zero, 0x40
  18da60: ca ff 40 14  	bnez	$2, 0x18d98c <.text+0x8d98c>
  18da64: 2d 80 40 00  	move	$16, $2
  18da68: 54 48 42 8e  	lw	$2, 0x4854($18)
  18da6c: 60 00 42 dc  	ld	$2, 0x60($2)
  18da70: 02 00 42 30  	andi	$2, $2, 0x2
  18da74: 1c 00 40 10  	beqz	$2, 0x18dae8 <.text+0x8dae8>
  18da78: 42 00 07 3c  	lui	$7, 0x42
  18da7c: 42 00 07 3c  	lui	$7, 0x42
  18da80: 2d 40 20 02  	move	$8, $17
  18da84: 2d 20 a0 03  	move	$4, $sp
  18da88: 58 43 e7 24  	addiu	$7, $7, 0x4358
  18da8c: 40 00 05 24  	addiu	$5, $zero, 0x40
  18da90: 2d 30 00 00  	move	$6, $zero
  18da94: 10 04 a9 27  	addiu	$9, $sp, 0x410
  18da98: 5b 37 06 0c  	jal	0x18dd6c <.text+0x8dd6c>
  18da9c: 14 04 aa 27  	addiu	$10, $sp, 0x414
  18daa0: 08 00 40 10  	beqz	$2, 0x18dac4 <.text+0x8dac4>
  18daa4: 2d 80 40 00  	move	$16, $2
  18daa8: 01 00 02 24  	addiu	$2, $zero, 0x1
  18daac: e5 ff 02 16  	bne	$16, $2, 0x18da44 <.text+0x8da44>
  18dab0: 00 00 00 00  	nop
  18dab4: b8 38 06 0c  	jal	0x18e2e0 <.text+0x8e2e0>
  18dab8: 10 04 a4 8f  	lw	$4, 0x410($sp)
  18dabc: e1 ff 00 10  	b	0x18da44 <.text+0x8da44>
  18dac0: 00 00 00 00  	nop
  18dac4: 00 04 a4 8f  	lw	$4, 0x400($sp)
  18dac8: 08 04 a5 8f  	lw	$5, 0x408($sp)
  18dacc: 10 04 a6 8f  	lw	$6, 0x410($sp)
  18dad0: 04 04 a7 8f  	lw	$7, 0x404($sp)
  18dad4: 0c 04 a8 8f  	lw	$8, 0x40c($sp)
  18dad8: 7e 30 06 0c  	jal	0x18c1f8 <.text+0x8c1f8>
  18dadc: 14 04 a9 8f  	lw	$9, 0x414($sp)
  18dae0: f4 ff 00 10  	b	0x18dab4 <.text+0x8dab4>
  18dae4: 2d 80 40 00  	move	$16, $2
  18dae8: 2d 40 20 02  	move	$8, $17
  18daec: 2d 20 a0 03  	move	$4, $sp
  18daf0: d8 42 e7 24  	addiu	$7, $7, 0x42d8
  18daf4: 40 00 05 24  	addiu	$5, $zero, 0x40
  18daf8: 2d 30 00 00  	move	$6, $zero
  18dafc: 10 04 a9 27  	addiu	$9, $sp, 0x410
  18db00: 5b 37 06 0c  	jal	0x18dd6c <.text+0x8dd6c>
  18db04: 14 04 aa 27  	addiu	$10, $sp, 0x414
  18db08: e7 ff 40 14  	bnez	$2, 0x18daa8 <.text+0x8daa8>
  18db0c: 2d 80 40 00  	move	$16, $2
  18db10: 00 04 a4 8f  	lw	$4, 0x400($sp)
  18db14: 08 04 a5 8f  	lw	$5, 0x408($sp)
  18db18: 10 04 a6 8f  	lw	$6, 0x410($sp)
  18db1c: 04 04 a7 8f  	lw	$7, 0x404($sp)
  18db20: 0c 04 a8 8f  	lw	$8, 0x40c($sp)
  18db24: 0d 32 06 0c  	jal	0x18c834 <.text+0x8c834>
  18db28: 14 04 a9 8f  	lw	$9, 0x414($sp)
  18db2c: e1 ff 00 10  	b	0x18dab4 <.text+0x8dab4>
  18db30: 2d 80 40 00  	move	$16, $2
  18db34: 2d 20 a0 03  	move	$4, $sp
  18db38: 49 30 06 0c  	jal	0x18c124 <.text+0x8c124>
  18db3c: 40 00 05 24  	addiu	$5, $zero, 0x40
  18db40: 92 ff 40 14  	bnez	$2, 0x18d98c <.text+0x8d98c>
  18db44: 2d 80 40 00  	move	$16, $2
  18db48: 42 00 02 3c  	lui	$2, 0x42
  18db4c: 42 00 07 3c  	lui	$7, 0x42
  18db50: 58 42 51 24  	addiu	$17, $2, 0x4258
  18db54: 58 41 e7 24  	addiu	$7, $7, 0x4158
  18db58: 2d 20 a0 03  	move	$4, $sp
  18db5c: 40 00 05 24  	addiu	$5, $zero, 0x40
  18db60: 2d 30 00 00  	move	$6, $zero
  18db64: 2d 40 20 02  	move	$8, $17
  18db68: 08 04 a9 27  	addiu	$9, $sp, 0x408
  18db6c: 5b 37 06 0c  	jal	0x18dd6c <.text+0x8dd6c>
  18db70: 0c 04 aa 27  	addiu	$10, $sp, 0x40c
  18db74: 06 00 40 10  	beqz	$2, 0x18db90 <.text+0x8db90>
  18db78: 2d 80 40 00  	move	$16, $2
  18db7c: 01 00 02 24  	addiu	$2, $zero, 0x1
  18db80: 83 ff 02 56  	bnel	$16, $2, 0x18d990 <.text+0x8d990>
  18db84: 2d 10 00 02  	move	$2, $16
  18db88: 95 ff 00 10  	b	0x18d9e0 <.text+0x8d9e0>
  18db8c: 08 04 a4 8f  	lw	$4, 0x408($sp)
  18db90: 2d 20 a0 03  	move	$4, $sp
  18db94: 49 30 06 0c  	jal	0x18c124 <.text+0x8c124>
  18db98: 40 00 05 24  	addiu	$5, $zero, 0x40
  18db9c: 7b ff 40 14  	bnez	$2, 0x18d98c <.text+0x8d98c>
  18dba0: 2d 80 40 00  	move	$16, $2
  18dba4: 54 48 42 8e  	lw	$2, 0x4854($18)
  18dba8: 60 00 42 dc  	ld	$2, 0x60($2)
  18dbac: 02 00 42 30  	andi	$2, $2, 0x2
  18dbb0: 1a 00 40 10  	beqz	$2, 0x18dc1c <.text+0x8dc1c>
  18dbb4: 42 00 07 3c  	lui	$7, 0x42
  18dbb8: 42 00 07 3c  	lui	$7, 0x42
  18dbbc: 2d 40 20 02  	move	$8, $17
  18dbc0: 2d 20 a0 03  	move	$4, $sp
  18dbc4: 58 43 e7 24  	addiu	$7, $7, 0x4358
  18dbc8: 40 00 05 24  	addiu	$5, $zero, 0x40
  18dbcc: 2d 30 00 00  	move	$6, $zero
  18dbd0: 10 04 a9 27  	addiu	$9, $sp, 0x410
  18dbd4: 5b 37 06 0c  	jal	0x18dd6c <.text+0x8dd6c>
  18dbd8: 14 04 aa 27  	addiu	$10, $sp, 0x414
  18dbdc: 08 00 40 10  	beqz	$2, 0x18dc00 <.text+0x8dc00>
  18dbe0: 2d 80 40 00  	move	$16, $2
  18dbe4: 01 00 02 24  	addiu	$2, $zero, 0x1
  18dbe8: 7d ff 02 56  	bnel	$16, $2, 0x18d9e0 <.text+0x8d9e0>
  18dbec: 08 04 a4 8f  	lw	$4, 0x408($sp)
  18dbf0: b8 38 06 0c  	jal	0x18e2e0 <.text+0x8e2e0>
  18dbf4: 10 04 a4 8f  	lw	$4, 0x410($sp)
  18dbf8: 79 ff 00 10  	b	0x18d9e0 <.text+0x8d9e0>
  18dbfc: 08 04 a4 8f  	lw	$4, 0x408($sp)
  18dc00: 08 04 a4 8f  	lw	$4, 0x408($sp)
  18dc04: 10 04 a5 8f  	lw	$5, 0x410($sp)
  18dc08: 0c 04 a6 8f  	lw	$6, 0x40c($sp)
  18dc0c: 9c 33 06 0c  	jal	0x18ce70 <.text+0x8ce70>
  18dc10: 14 04 a7 8f  	lw	$7, 0x414($sp)
  18dc14: f6 ff 00 10  	b	0x18dbf0 <.text+0x8dbf0>
  18dc18: 2d 80 40 00  	move	$16, $2
  18dc1c: 2d 40 20 02  	move	$8, $17
  18dc20: 2d 20 a0 03  	move	$4, $sp
  18dc24: d8 42 e7 24  	addiu	$7, $7, 0x42d8
  18dc28: 40 00 05 24  	addiu	$5, $zero, 0x40
  18dc2c: 2d 30 00 00  	move	$6, $zero
  18dc30: 10 04 a9 27  	addiu	$9, $sp, 0x410
  18dc34: 5b 37 06 0c  	jal	0x18dd6c <.text+0x8dd6c>
  18dc38: 14 04 aa 27  	addiu	$10, $sp, 0x414
  18dc3c: e9 ff 40 14  	bnez	$2, 0x18dbe4 <.text+0x8dbe4>
  18dc40: 2d 80 40 00  	move	$16, $2
  18dc44: 08 04 a4 8f  	lw	$4, 0x408($sp)
  18dc48: 10 04 a5 8f  	lw	$5, 0x410($sp)
  18dc4c: 0c 04 a6 8f  	lw	$6, 0x40c($sp)
  18dc50: f1 34 06 0c  	jal	0x18d3c4 <.text+0x8d3c4>
  18dc54: 14 04 a7 8f  	lw	$7, 0x414($sp)
  18dc58: e5 ff 00 10  	b	0x18dbf0 <.text+0x8dbf0>
  18dc5c: 2d 80 40 00  	move	$16, $2
