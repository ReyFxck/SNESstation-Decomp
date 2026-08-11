# old EE FileIO wrappers: 0x0019cfc0..0x0019d5ff
  19cfc0: 90 fe bd 27  	addiu	$sp, $sp, -0x170 <.text+0xffffffffffeffe90>
  19cfc4: 42 00 02 3c  	lui	$2, 0x42
  19cfc8: 30 01 b1 ff  	sd	$17, 0x130($sp)
  19cfcc: 2d 88 a0 00  	move	$17, $5
  19cfd0: 20 01 b0 ff  	sd	$16, 0x120($sp)
  19cfd4: 60 01 bf ff  	sd	$ra, 0x160($sp)
  19cfd8: 50 01 b3 ff  	sd	$19, 0x150($sp)
  19cfdc: 40 01 b2 ff  	sd	$18, 0x140($sp)
  19cfe0: b0 5a 42 8c  	lw	$2, 0x5ab0($2)
  19cfe4: 05 00 40 14  	bnez	$2, 0x19cffc <.text+0x9cffc>
  19cfe8: 2d 80 80 00  	move	$16, $4
  19cfec: 80 7d 06 0c  	jal	0x19f600 <.text+0x9f600>
  19cff0: 00 00 00 00  	nop
  19cff4: 1e 00 40 04  	bltz	$2, 0x19d070 <.text+0x9d070>
  19cff8: 2d 48 40 00  	move	$9, $2
  19cffc: 42 00 02 3c  	lui	$2, 0x42
  19d000: 42 00 13 3c  	lui	$19, 0x42
  19d004: 44 6e 44 8c  	lw	$4, 0x6e44($2)
  19d008: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19d00c: 45 00 12 3c  	lui	$18, 0x45
  19d010: 10 00 b1 af  	sw	$17, 0x10($sp)
  19d014: 00 01 06 24  	addiu	$6, $zero, 0x100
  19d018: 2d 28 00 02  	move	$5, $16
  19d01c: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  19d020: 14 00 a4 27  	addiu	$4, $sp, 0x14
  19d024: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19d028: 48 6e 66 8e  	lw	$6, 0x6e48($19)
  19d02c: 45 00 04 3c  	lui	$4, 0x45
  19d030: 1a 00 0b 3c  	lui	$11, 0x1a
  19d034: c0 03 49 26  	addiu	$9, $18, 0x3c0
  19d038: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19d03c: 10 03 84 24  	addiu	$4, $4, 0x310
  19d040: e8 f6 6b 25  	addiu	$11, $11, -0x918 <.text+0xffffffffffeff6e8>
  19d044: 2d 28 00 00  	move	$5, $zero
  19d048: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19d04c: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19d050: 10 01 08 24  	addiu	$8, $zero, 0x110
  19d054: 06 00 40 04  	bltz	$2, 0x19d070 <.text+0x9d070>
  19d058: 2d 48 40 00  	move	$9, $2
  19d05c: 48 6e 62 8e  	lw	$2, 0x6e48($19)
  19d060: 2d 48 00 00  	move	$9, $zero
  19d064: c0 03 43 8e  	lw	$3, 0x3c0($18)
  19d068: 01 00 42 38  	xori	$2, $2, 0x1
  19d06c: 0b 48 62 00  	movn	$9, $3, $2
  19d070: 60 01 bf df  	ld	$ra, 0x160($sp)
  19d074: 2d 10 20 01  	move	$2, $9
  19d078: 50 01 b3 df  	ld	$19, 0x150($sp)
  19d07c: 40 01 b2 df  	ld	$18, 0x140($sp)
  19d080: 30 01 b1 df  	ld	$17, 0x130($sp)
  19d084: 20 01 b0 df  	ld	$16, 0x120($sp)
  19d088: 08 00 e0 03  	jr	$ra
  19d08c: 70 01 bd 27  	addiu	$sp, $sp, 0x170
  19d090: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19d094: 42 00 02 3c  	lui	$2, 0x42
  19d098: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19d09c: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19d0a0: b0 5a 42 8c  	lw	$2, 0x5ab0($2)
  19d0a4: 05 00 40 14  	bnez	$2, 0x19d0bc <.text+0x9d0bc>
  19d0a8: 2d 80 80 00  	move	$16, $4
  19d0ac: 80 7d 06 0c  	jal	0x19f600 <.text+0x9f600>
  19d0b0: 00 00 00 00  	nop
  19d0b4: 15 00 40 04  	bltz	$2, 0x19d10c <.text+0x9d10c>
  19d0b8: 2d 20 40 00  	move	$4, $2
  19d0bc: 42 00 02 3c  	lui	$2, 0x42
  19d0c0: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19d0c4: 44 6e 44 8c  	lw	$4, 0x6e44($2)
  19d0c8: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19d0cc: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19d0d0: 45 00 04 3c  	lui	$4, 0x45
  19d0d4: 1a 00 0b 3c  	lui	$11, 0x1a
  19d0d8: 10 03 84 24  	addiu	$4, $4, 0x310
  19d0dc: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19d0e0: e8 f6 6b 25  	addiu	$11, $11, -0x918 <.text+0xffffffffffeff6e8>
  19d0e4: 01 00 05 24  	addiu	$5, $zero, 0x1
  19d0e8: 2d 30 00 00  	move	$6, $zero
  19d0ec: 04 00 08 24  	addiu	$8, $zero, 0x4
  19d0f0: 2d 48 e0 00  	move	$9, $7
  19d0f4: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19d0f8: 10 00 b0 af  	sw	$16, 0x10($sp)
  19d0fc: 10 00 a4 8f  	lw	$4, 0x10($sp)
  19d100: 2d 18 40 00  	move	$3, $2
  19d104: 00 00 42 28  	slti	$2, $2, 0x0
  19d108: 0b 20 62 00  	movn	$4, $3, $2
  19d10c: 30 00 bf df  	ld	$ra, 0x30($sp)
  19d110: 2d 10 80 00  	move	$2, $4
  19d114: 20 00 b0 df  	ld	$16, 0x20($sp)
  19d118: 08 00 e0 03  	jr	$ra
  19d11c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19d120: 70 ff bd 27  	addiu	$sp, $sp, -0x90 <.text+0xffffffffffefff70>
  19d124: 42 00 02 3c  	lui	$2, 0x42
  19d128: 50 00 b3 ff  	sd	$19, 0x50($sp)
  19d12c: 2d 98 80 00  	move	$19, $4
  19d130: 40 00 b2 ff  	sd	$18, 0x40($sp)
  19d134: 2d 90 c0 00  	move	$18, $6
  19d138: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19d13c: 80 00 bf ff  	sd	$ra, 0x80($sp)
  19d140: 70 00 b5 ff  	sd	$21, 0x70($sp)
  19d144: 60 00 b4 ff  	sd	$20, 0x60($sp)
  19d148: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19d14c: b0 5a 42 8c  	lw	$2, 0x5ab0($2)
  19d150: 05 00 40 14  	bnez	$2, 0x19d168 <.text+0x9d168>
  19d154: 2d 80 a0 00  	move	$16, $5
  19d158: 80 7d 06 0c  	jal	0x19f600 <.text+0x9f600>
  19d15c: 00 00 00 00  	nop
  19d160: 2a 00 40 04  	bltz	$2, 0x19d20c <.text+0x9d20c>
  19d164: 2d 48 40 00  	move	$9, $2
  19d168: 42 00 02 3c  	lui	$2, 0x42
  19d16c: 42 00 14 3c  	lui	$20, 0x42
  19d170: 44 6e 44 8c  	lw	$4, 0x6e44($2)
  19d174: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19d178: 45 00 15 3c  	lui	$21, 0x45
  19d17c: 18 00 b2 af  	sw	$18, 0x18($sp)
  19d180: 00 20 03 3c  	lui	$3, 0x2000
  19d184: 45 00 02 3c  	lui	$2, 0x45
  19d188: 40 03 51 24  	addiu	$17, $2, 0x340
  19d18c: 24 18 03 02  	and	$3, $16, $3
  19d190: 2d 20 00 02  	move	$4, $16
  19d194: 2d 28 40 02  	move	$5, $18
  19d198: 10 00 b3 af  	sw	$19, 0x10($sp)
  19d19c: 14 00 b0 af  	sw	$16, 0x14($sp)
  19d1a0: 24 00 60 10  	beqz	$3, 0x19d234 <.text+0x9d234>
  19d1a4: 1c 00 b1 af  	sw	$17, 0x1c($sp)
  19d1a8: 2d 20 20 02  	move	$4, $17
  19d1ac: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19d1b0: 80 00 05 24  	addiu	$5, $zero, 0x80
  19d1b4: 10 00 a4 27  	addiu	$4, $sp, 0x10
  19d1b8: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19d1bc: 10 00 05 24  	addiu	$5, $zero, 0x10
  19d1c0: 00 00 b1 af  	sw	$17, 0x0($sp)
  19d1c4: 48 6e 86 8e  	lw	$6, 0x6e48($20)
  19d1c8: 45 00 04 3c  	lui	$4, 0x45
  19d1cc: 1a 00 0b 3c  	lui	$11, 0x1a
  19d1d0: c0 03 a9 26  	addiu	$9, $21, 0x3c0
  19d1d4: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19d1d8: 10 03 84 24  	addiu	$4, $4, 0x310
  19d1dc: b0 d4 6b 25  	addiu	$11, $11, -0x2b50 <.text+0xffffffffffefd4b0>
  19d1e0: 02 00 05 24  	addiu	$5, $zero, 0x2
  19d1e4: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19d1e8: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19d1ec: 10 00 08 24  	addiu	$8, $zero, 0x10
  19d1f0: 06 00 40 04  	bltz	$2, 0x19d20c <.text+0x9d20c>
  19d1f4: 2d 48 40 00  	move	$9, $2
  19d1f8: 48 6e 82 8e  	lw	$2, 0x6e48($20)
  19d1fc: 2d 48 00 00  	move	$9, $zero
  19d200: c0 03 a3 8e  	lw	$3, 0x3c0($21)
  19d204: 01 00 42 38  	xori	$2, $2, 0x1
  19d208: 0b 48 62 00  	movn	$9, $3, $2
  19d20c: 80 00 bf df  	ld	$ra, 0x80($sp)
  19d210: 2d 10 20 01  	move	$2, $9
  19d214: 70 00 b5 df  	ld	$21, 0x70($sp)
  19d218: 60 00 b4 df  	ld	$20, 0x60($sp)
  19d21c: 50 00 b3 df  	ld	$19, 0x50($sp)
  19d220: 40 00 b2 df  	ld	$18, 0x40($sp)
  19d224: 30 00 b1 df  	ld	$17, 0x30($sp)
  19d228: 20 00 b0 df  	ld	$16, 0x20($sp)
  19d22c: 08 00 e0 03  	jr	$ra
  19d230: 90 00 bd 27  	addiu	$sp, $sp, 0x90
  19d234: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19d238: 00 00 00 00  	nop
  19d23c: db ff 00 10  	b	0x19d1ac <.text+0x9d1ac>
  19d240: 2d 20 20 02  	move	$4, $17
  19d244: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  19d248: 42 00 02 3c  	lui	$2, 0x42
  19d24c: 50 00 b2 ff  	sd	$18, 0x50($sp)
  19d250: 2d 90 80 00  	move	$18, $4
  19d254: 40 00 b1 ff  	sd	$17, 0x40($sp)
  19d258: 2d 88 c0 00  	move	$17, $6
  19d25c: 30 00 b0 ff  	sd	$16, 0x30($sp)
  19d260: 60 00 bf ff  	sd	$ra, 0x60($sp)
  19d264: b0 5a 42 8c  	lw	$2, 0x5ab0($2)
  19d268: 05 00 40 14  	bnez	$2, 0x19d280 <.text+0x9d280>
  19d26c: 2d 80 a0 00  	move	$16, $5
  19d270: 80 7d 06 0c  	jal	0x19f600 <.text+0x9f600>
  19d274: 00 00 00 00  	nop
  19d278: 29 00 40 04  	bltz	$2, 0x19d320 <.text+0x9d320>
  19d27c: 2d 48 40 00  	move	$9, $2
  19d280: 42 00 02 3c  	lui	$2, 0x42
  19d284: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19d288: 44 6e 44 8c  	lw	$4, 0x6e44($2)
  19d28c: 18 00 b1 af  	sw	$17, 0x18($sp)
  19d290: 0f 00 03 32  	andi	$3, $16, 0xf
  19d294: 10 00 b2 af  	sw	$18, 0x10($sp)
  19d298: 2d 30 00 00  	move	$6, $zero
  19d29c: 05 00 60 10  	beqz	$3, 0x19d2b4 <.text+0x9d2b4>
  19d2a0: 14 00 b0 af  	sw	$16, 0x14($sp)
  19d2a4: 10 00 02 24  	addiu	$2, $zero, 0x10
  19d2a8: 23 30 43 00  	subu	$6, $2, $3
  19d2ac: 2a 10 26 02  	slt	$2, $17, $6
  19d2b0: 0b 30 22 02  	movn	$6, $17, $2
  19d2b4: 25 00 c0 14  	bnez	$6, 0x19d34c <.text+0x9d34c>
  19d2b8: 1c 00 a6 af  	sw	$6, 0x1c($sp)
  19d2bc: 00 20 02 3c  	lui	$2, 0x2000
  19d2c0: 24 10 02 02  	and	$2, $16, $2
  19d2c4: 1d 00 40 10  	beqz	$2, 0x19d33c <.text+0x9d33c>
  19d2c8: 2d 20 00 02  	move	$4, $16
  19d2cc: 42 00 10 3c  	lui	$16, 0x42
  19d2d0: 45 00 11 3c  	lui	$17, 0x45
  19d2d4: 48 6e 06 8e  	lw	$6, 0x6e48($16)
  19d2d8: 45 00 04 3c  	lui	$4, 0x45
  19d2dc: 1a 00 0b 3c  	lui	$11, 0x1a
  19d2e0: c0 03 29 26  	addiu	$9, $17, 0x3c0
  19d2e4: 10 03 84 24  	addiu	$4, $4, 0x310
  19d2e8: e8 f6 6b 25  	addiu	$11, $11, -0x918 <.text+0xffffffffffeff6e8>
  19d2ec: 03 00 05 24  	addiu	$5, $zero, 0x3
  19d2f0: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19d2f4: 20 00 08 24  	addiu	$8, $zero, 0x20
  19d2f8: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19d2fc: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19d300: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19d304: 06 00 40 04  	bltz	$2, 0x19d320 <.text+0x9d320>
  19d308: 2d 48 40 00  	move	$9, $2
  19d30c: 48 6e 02 8e  	lw	$2, 0x6e48($16)
  19d310: 2d 48 00 00  	move	$9, $zero
  19d314: c0 03 23 8e  	lw	$3, 0x3c0($17)
  19d318: 01 00 42 38  	xori	$2, $2, 0x1
  19d31c: 0b 48 62 00  	movn	$9, $3, $2
  19d320: 60 00 bf df  	ld	$ra, 0x60($sp)
  19d324: 2d 10 20 01  	move	$2, $9
  19d328: 50 00 b2 df  	ld	$18, 0x50($sp)
  19d32c: 40 00 b1 df  	ld	$17, 0x40($sp)
  19d330: 30 00 b0 df  	ld	$16, 0x30($sp)
  19d334: 08 00 e0 03  	jr	$ra
  19d338: 70 00 bd 27  	addiu	$sp, $sp, 0x70
  19d33c: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19d340: 2d 28 20 02  	move	$5, $17
  19d344: e2 ff 00 10  	b	0x19d2d0 <.text+0x9d2d0>
  19d348: 42 00 10 3c  	lui	$16, 0x42
  19d34c: 20 00 a4 27  	addiu	$4, $sp, 0x20
  19d350: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  19d354: 2d 28 00 02  	move	$5, $16
  19d358: d9 ff 00 10  	b	0x19d2c0 <.text+0x9d2c0>
  19d35c: 00 20 02 3c  	lui	$2, 0x2000
  19d360: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  19d364: 42 00 02 3c  	lui	$2, 0x42
  19d368: 40 00 b2 ff  	sd	$18, 0x40($sp)
  19d36c: 2d 90 a0 00  	move	$18, $5
  19d370: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19d374: 2d 88 c0 00  	move	$17, $6
  19d378: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19d37c: 50 00 bf ff  	sd	$ra, 0x50($sp)
  19d380: b0 5a 42 8c  	lw	$2, 0x5ab0($2)
  19d384: 05 00 40 14  	bnez	$2, 0x19d39c <.text+0x9d39c>
  19d388: 2d 80 80 00  	move	$16, $4
  19d38c: 80 7d 06 0c  	jal	0x19f600 <.text+0x9f600>
  19d390: 00 00 00 00  	nop
  19d394: 17 00 40 04  	bltz	$2, 0x19d3f4 <.text+0x9d3f4>
  19d398: 2d 20 40 00  	move	$4, $2
  19d39c: 42 00 02 3c  	lui	$2, 0x42
  19d3a0: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19d3a4: 44 6e 44 8c  	lw	$4, 0x6e44($2)
  19d3a8: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19d3ac: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19d3b0: 45 00 04 3c  	lui	$4, 0x45
  19d3b4: 1a 00 0b 3c  	lui	$11, 0x1a
  19d3b8: 10 03 84 24  	addiu	$4, $4, 0x310
  19d3bc: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19d3c0: e8 f6 6b 25  	addiu	$11, $11, -0x918 <.text+0xffffffffffeff6e8>
  19d3c4: 04 00 05 24  	addiu	$5, $zero, 0x4
  19d3c8: 2d 30 00 00  	move	$6, $zero
  19d3cc: 10 00 08 24  	addiu	$8, $zero, 0x10
  19d3d0: 2d 48 e0 00  	move	$9, $7
  19d3d4: 10 00 b0 af  	sw	$16, 0x10($sp)
  19d3d8: 14 00 b2 af  	sw	$18, 0x14($sp)
  19d3dc: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19d3e0: 18 00 b1 af  	sw	$17, 0x18($sp)
  19d3e4: 10 00 a4 8f  	lw	$4, 0x10($sp)
  19d3e8: 2d 18 40 00  	move	$3, $2
  19d3ec: 00 00 42 28  	slti	$2, $2, 0x0
  19d3f0: 0b 20 62 00  	movn	$4, $3, $2
  19d3f4: 50 00 bf df  	ld	$ra, 0x50($sp)
  19d3f8: 2d 10 80 00  	move	$2, $4
  19d3fc: 40 00 b2 df  	ld	$18, 0x40($sp)
  19d400: 30 00 b1 df  	ld	$17, 0x30($sp)
  19d404: 20 00 b0 df  	ld	$16, 0x20($sp)
  19d408: 08 00 e0 03  	jr	$ra
  19d40c: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  19d410: d0 fe bd 27  	addiu	$sp, $sp, -0x130 <.text+0xffffffffffeffed0>
  19d414: 42 00 02 3c  	lui	$2, 0x42
  19d418: 10 01 b0 ff  	sd	$16, 0x110($sp)
  19d41c: 20 01 bf ff  	sd	$ra, 0x120($sp)
  19d420: b0 5a 42 8c  	lw	$2, 0x5ab0($2)
  19d424: 05 00 40 14  	bnez	$2, 0x19d43c <.text+0x9d43c>
  19d428: 2d 80 80 00  	move	$16, $4
  19d42c: 80 7d 06 0c  	jal	0x19f600 <.text+0x9f600>
  19d430: 00 00 00 00  	nop
  19d434: 19 00 40 04  	bltz	$2, 0x19d49c <.text+0x9d49c>
  19d438: 2d 20 40 00  	move	$4, $2
  19d43c: 42 00 02 3c  	lui	$2, 0x42
  19d440: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19d444: 44 6e 44 8c  	lw	$4, 0x6e44($2)
  19d448: 10 00 a4 27  	addiu	$4, $sp, 0x10
  19d44c: 00 01 06 24  	addiu	$6, $zero, 0x100
  19d450: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  19d454: 2d 28 00 02  	move	$5, $16
  19d458: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19d45c: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19d460: 45 00 04 3c  	lui	$4, 0x45
  19d464: 1a 00 0b 3c  	lui	$11, 0x1a
  19d468: 10 03 84 24  	addiu	$4, $4, 0x310
  19d46c: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19d470: e8 f6 6b 25  	addiu	$11, $11, -0x918 <.text+0xffffffffffeff6e8>
  19d474: 07 00 05 24  	addiu	$5, $zero, 0x7
  19d478: 2d 30 00 00  	move	$6, $zero
  19d47c: 00 01 08 24  	addiu	$8, $zero, 0x100
  19d480: 2d 48 e0 00  	move	$9, $7
  19d484: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19d488: 10 01 a0 a3  	sb	$zero, 0x110($sp)
  19d48c: 10 00 a4 8f  	lw	$4, 0x10($sp)
  19d490: 2d 18 40 00  	move	$3, $2
  19d494: 00 00 42 28  	slti	$2, $2, 0x0
  19d498: 0b 20 62 00  	movn	$4, $3, $2
  19d49c: 20 01 bf df  	ld	$ra, 0x120($sp)
  19d4a0: 2d 10 80 00  	move	$2, $4
  19d4a4: 10 01 b0 df  	ld	$16, 0x110($sp)
  19d4a8: 08 00 e0 03  	jr	$ra
  19d4ac: 30 01 bd 27  	addiu	$sp, $sp, 0x130
  19d4b0: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19d4b4: 00 20 02 3c  	lui	$2, 0x2000
  19d4b8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19d4bc: 25 80 82 00  	or	$16, $4, $2
  19d4c0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19d4c4: 00 00 06 8e  	lw	$6, 0x0($16)
  19d4c8: 06 00 c0 50  	beqzl	$6, 0x19d4e4 <.text+0x9d4e4>
  19d4cc: 04 00 06 8e  	lw	$6, 0x4($16)
  19d4d0: 08 00 02 8e  	lw	$2, 0x8($16)
  19d4d4: 10 00 05 26  	addiu	$5, $16, 0x10
  19d4d8: 12 00 40 14  	bnez	$2, 0x19d524 <.text+0x9d524>
  19d4dc: 2d 20 40 00  	move	$4, $2
  19d4e0: 04 00 06 8e  	lw	$6, 0x4($16)
  19d4e4: 04 00 c0 10  	beqz	$6, 0x19d4f8 <.text+0x9d4f8>
  19d4e8: 20 00 05 26  	addiu	$5, $16, 0x20
  19d4ec: 0c 00 02 8e  	lw	$2, 0xc($16)
  19d4f0: 08 00 40 14  	bnez	$2, 0x19d514 <.text+0x9d514>
  19d4f4: 2d 20 40 00  	move	$4, $2
  19d4f8: 42 00 02 3c  	lui	$2, 0x42
  19d4fc: a4 73 06 0c  	jal	0x19ce90 <.text+0x9ce90>
  19d500: 44 6e 44 8c  	lw	$4, 0x6e44($2)
  19d504: 00 00 b0 df  	ld	$16, 0x0($sp)
  19d508: 10 00 bf df  	ld	$ra, 0x10($sp)
  19d50c: 08 00 e0 03  	jr	$ra
  19d510: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19d514: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  19d518: 00 00 00 00  	nop
  19d51c: f7 ff 00 10  	b	0x19d4fc <.text+0x9d4fc>
  19d520: 42 00 02 3c  	lui	$2, 0x42
  19d524: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  19d528: 00 00 00 00  	nop
  19d52c: ed ff 00 10  	b	0x19d4e4 <.text+0x9d4e4>
  19d530: 04 00 06 8e  	lw	$6, 0x4($16)
  19d534: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19d538: 01 00 06 24  	addiu	$6, $zero, 0x1
  19d53c: 00 00 a5 af  	sw	$5, 0x0($sp)
  19d540: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19d544: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  19d548: 2d 28 a0 03  	move	$5, $sp
  19d54c: 10 00 bf df  	ld	$ra, 0x10($sp)
  19d550: 08 00 e0 03  	jr	$ra
  19d554: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19d558: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19d55c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19d560: 2d 90 a0 00  	move	$18, $5
  19d564: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19d568: 2d 88 80 00  	move	$17, $4
  19d56c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19d570: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19d574: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  19d578: 2d 80 00 00  	move	$16, $zero
  19d57c: ff ff 45 24  	addiu	$5, $2, -0x1 <.text+0xffffffffffefffff>
  19d580: 0c 00 a0 18  	blez	$5, 0x19d5b4 <.text+0x9d5b4>
  19d584: 2d 20 40 00  	move	$4, $2
  19d588: 21 10 50 02  	addu	$2, $18, $16
  19d58c: 00 00 43 80  	lb	$3, 0x0($2)
  19d590: 16 00 60 50  	beqzl	$3, 0x19d5ec <.text+0x9d5ec>
  19d594: 23 28 04 02  	subu	$5, $16, $4
  19d598: 0a 00 02 24  	addiu	$2, $zero, 0xa
  19d59c: 0c 00 62 50  	beql	$3, $2, 0x19d5d0 <.text+0x9d5d0>
  19d5a0: 23 28 04 02  	subu	$5, $16, $4
  19d5a4: 01 00 10 26  	addiu	$16, $16, 0x1
  19d5a8: 2a 10 05 02  	slt	$2, $16, $5
  19d5ac: f7 ff 40 14  	bnez	$2, 0x19d58c <.text+0x9d58c>
  19d5b0: 21 10 50 02  	addu	$2, $18, $16
  19d5b4: 2d 10 00 02  	move	$2, $16
  19d5b8: 30 00 bf df  	ld	$ra, 0x30($sp)
  19d5bc: 20 00 b2 df  	ld	$18, 0x20($sp)
  19d5c0: 10 00 b1 df  	ld	$17, 0x10($sp)
  19d5c4: 00 00 b0 df  	ld	$16, 0x0($sp)
  19d5c8: 08 00 e0 03  	jr	$ra
  19d5cc: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19d5d0: 01 00 06 24  	addiu	$6, $zero, 0x1
  19d5d4: 2d 20 20 02  	move	$4, $17
  19d5d8: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  19d5dc: 01 00 10 26  	addiu	$16, $16, 0x1
  19d5e0: 21 10 50 02  	addu	$2, $18, $16
  19d5e4: f3 ff 00 10  	b	0x19d5b4 <.text+0x9d5b4>
  19d5e8: 00 00 40 a0  	sb	$zero, 0x0($2)
  19d5ec: 01 00 06 24  	addiu	$6, $zero, 0x1
  19d5f0: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  19d5f4: 2d 20 20 02  	move	$4, $17
  19d5f8: ef ff 00 10  	b	0x19d5b8 <.text+0x9d5b8>
  19d5fc: 2d 10 00 02  	move	$2, $16

