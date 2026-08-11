# Focused R5900 extract: 0x0019f138..0x0019f5a0

  19f138: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  19f13c: 2d 18 00 00  	move	$3, $zero
  19f140: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19f144: ff 00 f1 30  	andi	$17, $7, 0xff
  19f148: 71 00 22 2e  	sltiu	$2, $17, 0x71
  19f14c: 80 00 b6 ff  	sd	$22, 0x80($sp)
  19f150: 70 00 b5 ff  	sd	$21, 0x70($sp)
  19f154: 2d b0 a0 00  	move	$22, $5
  19f158: 60 00 b4 ff  	sd	$20, 0x60($sp)
  19f15c: 2d a8 20 01  	move	$21, $9
  19f160: 50 00 b3 ff  	sd	$19, 0x50($sp)
  19f164: 2d a0 00 01  	move	$20, $8
  19f168: 40 00 b2 ff  	sd	$18, 0x40($sp)
  19f16c: 2d 98 00 00  	move	$19, $zero
  19f170: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19f174: 2d 90 40 01  	move	$18, $10
  19f178: 90 00 bf ff  	sd	$ra, 0x90($sp)
  19f17c: 26 00 40 10  	beqz	$2, 0x19f218 <.text+0x9f218>
  19f180: 2d 80 c0 00  	move	$16, $6
  19f184: 00 12 0a 00  	sll	$2, $10, 0x8
  19f188: 04 00 a3 30  	andi	$3, $5, 0x4
  19f18c: 25 10 22 02  	or	$2, $17, $2
  19f190: 08 00 c4 ac  	sw	$4, 0x8($6)
  19f194: 04 00 c0 ac  	sw	$zero, 0x4($6)
  19f198: 0b 00 40 19  	blez	$10, 0x19f1c8 <.text+0x9f1c8>
  19f19c: 00 00 d1 ac  	sw	$17, 0x0($6)
  19f1a0: 2d 20 00 01  	move	$4, $8
  19f1a4: 2d 28 40 01  	move	$5, $10
  19f1a8: 00 00 c2 ac  	sw	$2, 0x0($6)
  19f1ac: 01 00 13 24  	addiu	$19, $zero, 0x1
  19f1b0: 28 00 60 14  	bnez	$3, 0x19f254 <.text+0x9f254>
  19f1b4: 04 00 09 ae  	sw	$9, 0x4($16)
  19f1b8: 00 00 b4 af  	sw	$20, 0x0($sp)
  19f1bc: 04 00 b5 af  	sw	$21, 0x4($sp)
  19f1c0: 08 00 b2 af  	sw	$18, 0x8($sp)
  19f1c4: 0c 00 a0 af  	sw	$zero, 0xc($sp)
  19f1c8: 00 19 13 00  	sll	$3, $19, 0x4
  19f1cc: 44 00 02 24  	addiu	$2, $zero, 0x44
  19f1d0: 21 18 7d 00  	addu	$3, $3, $sp
  19f1d4: 2d 20 00 02  	move	$4, $16
  19f1d8: 0c 00 62 ac  	sw	$2, 0xc($3)
  19f1dc: 2d 28 20 02  	move	$5, $17
  19f1e0: 42 00 02 3c  	lui	$2, 0x42
  19f1e4: 08 00 71 ac  	sw	$17, 0x8($3)
  19f1e8: 90 5a 42 8c  	lw	$2, 0x5a90($2)
  19f1ec: 01 00 73 26  	addiu	$19, $19, 0x1
  19f1f0: 00 00 70 ac  	sw	$16, 0x0($3)
  19f1f4: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19f1f8: 04 00 62 ac  	sw	$2, 0x4($3)
  19f1fc: 01 00 c2 32  	andi	$2, $22, 0x1
  19f200: 2d 28 60 02  	move	$5, $19
  19f204: 0f 00 40 10  	beqz	$2, 0x19f244 <.text+0x9f244>
  19f208: 2d 20 a0 03  	move	$4, $sp
  19f20c: 78 7d 06 0c  	jal	0x19f5e0 <.text+0x9f5e0>
  19f210: 00 00 00 00  	nop
  19f214: 2d 18 40 00  	move	$3, $2
  19f218: 90 00 bf df  	ld	$ra, 0x90($sp)
  19f21c: 2d 10 60 00  	move	$2, $3
  19f220: 80 00 b6 df  	ld	$22, 0x80($sp)
  19f224: 70 00 b5 df  	ld	$21, 0x70($sp)
  19f228: 60 00 b4 df  	ld	$20, 0x60($sp)
  19f22c: 50 00 b3 df  	ld	$19, 0x50($sp)
  19f230: 40 00 b2 df  	ld	$18, 0x40($sp)
  19f234: 30 00 b1 df  	ld	$17, 0x30($sp)
  19f238: 20 00 b0 df  	ld	$16, 0x20($sp)
  19f23c: 08 00 e0 03  	jr	$ra
  19f240: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  19f244: b8 73 06 0c  	jal	0x19cee0 <.text+0x9cee0>
  19f248: 2d 20 a0 03  	move	$4, $sp
  19f24c: f2 ff 00 10  	b	0x19f218 <.text+0x9f218>
  19f250: 2d 18 40 00  	move	$3, $2
  19f254: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19f258: 00 00 00 00  	nop
  19f25c: d7 ff 00 10  	b	0x19f1bc <.text+0x9f1bc>
  19f260: 00 00 b4 af  	sw	$20, 0x0($sp)
  19f264: 2d 18 e0 00  	move	$3, $7
  19f268: 2d 58 00 01  	move	$11, $8
  19f26c: 2d 10 c0 00  	move	$2, $6
  19f270: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19f274: 2d 50 20 01  	move	$10, $9
  19f278: 2d 30 a0 00  	move	$6, $5
  19f27c: 2d 38 40 00  	move	$7, $2
  19f280: 2d 40 60 00  	move	$8, $3
  19f284: 2d 48 60 01  	move	$9, $11
  19f288: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19f28c: 4e 7c 06 0c  	jal	0x19f138 <.text+0x9f138>
  19f290: 2d 28 00 00  	move	$5, $zero
  19f294: 00 00 bf df  	ld	$ra, 0x0($sp)
  19f298: 08 00 e0 03  	jr	$ra
  19f29c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19f2a0: 2d 18 e0 00  	move	$3, $7
  19f2a4: 2d 58 00 01  	move	$11, $8
  19f2a8: 2d 10 c0 00  	move	$2, $6
  19f2ac: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19f2b0: 2d 50 20 01  	move	$10, $9
  19f2b4: 2d 30 a0 00  	move	$6, $5
  19f2b8: 2d 38 40 00  	move	$7, $2
  19f2bc: 2d 40 60 00  	move	$8, $3
  19f2c0: 2d 48 60 01  	move	$9, $11
  19f2c4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19f2c8: 4e 7c 06 0c  	jal	0x19f138 <.text+0x9f138>
  19f2cc: 01 00 05 24  	addiu	$5, $zero, 0x1
  19f2d0: 00 00 bf df  	ld	$ra, 0x0($sp)
  19f2d4: 08 00 e0 03  	jr	$ra
  19f2d8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19f2dc: 10 00 82 8c  	lw	$2, 0x10($4)
  19f2e0: 08 00 e0 03  	jr	$ra
  19f2e4: 08 00 a2 ac  	sw	$2, 0x8($5)
  19f2e8: 10 00 82 8c  	lw	$2, 0x10($4)
  19f2ec: 1c 00 a5 8c  	lw	$5, 0x1c($5)
  19f2f0: 80 10 02 00  	sll	$2, $2, 0x2
  19f2f4: 14 00 83 8c  	lw	$3, 0x14($4)
  19f2f8: 21 10 45 00  	addu	$2, $2, $5
  19f2fc: 08 00 e0 03  	jr	$ra
  19f300: 00 00 43 ac  	sw	$3, 0x0($2)
  19f304: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  19f308: 42 00 02 3c  	lui	$2, 0x42
  19f30c: 50 00 bf ff  	sd	$ra, 0x50($sp)
  19f310: 40 00 b2 ff  	sd	$18, 0x40($sp)
  19f314: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19f318: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19f31c: a8 5a 42 8c  	lw	$2, 0x5aa8($2)
  19f320: 06 00 40 10  	beqz	$2, 0x19f33c <.text+0x9f33c>
  19f324: 50 00 bf df  	ld	$ra, 0x50($sp)
  19f328: 40 00 b2 df  	ld	$18, 0x40($sp)
  19f32c: 30 00 b1 df  	ld	$17, 0x30($sp)
  19f330: 20 00 b0 df  	ld	$16, 0x20($sp)
  19f334: 08 00 e0 03  	jr	$ra
  19f338: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  19f33c: 06 7c 06 0c  	jal	0x19f018 <.text+0x9f018>
  19f340: 2d 80 00 00  	move	$16, $zero
  19f344: 42 00 04 3c  	lui	$4, 0x42
  19f348: 00 20 05 3c  	lui	$5, 0x2000
  19f34c: 88 5a 86 24  	addiu	$6, $4, 0x5a88
  19f350: 88 5a 82 8c  	lw	$2, 0x5a88($4)
  19f354: 04 00 c3 8c  	lw	$3, 0x4($6)
  19f358: 25 10 45 00  	or	$2, $2, $5
  19f35c: 25 18 65 00  	or	$3, $3, $5
  19f360: 88 5a 82 ac  	sw	$2, 0x5a88($4)
  19f364: 04 00 c3 ac  	sw	$3, 0x4($6)
  19f368: 42 00 02 3c  	lui	$2, 0x42
  19f36c: c0 18 10 00  	sll	$3, $16, 0x3
  19f370: 94 5a 42 8c  	lw	$2, 0x5a94($2)
  19f374: 01 00 10 26  	addiu	$16, $16, 0x1
  19f378: 20 00 04 2a  	slti	$4, $16, 0x20
  19f37c: 21 18 62 00  	addu	$3, $3, $2
  19f380: 04 00 60 ac  	sw	$zero, 0x4($3)
  19f384: f8 ff 80 14  	bnez	$4, 0x19f368 <.text+0x9f368>
  19f388: 00 00 60 ac  	sw	$zero, 0x0($3)
  19f38c: 2d 80 00 00  	move	$16, $zero
  19f390: 42 00 12 3c  	lui	$18, 0x42
  19f394: 80 10 10 00  	sll	$2, $16, 0x2
  19f398: 88 5a 51 26  	addiu	$17, $18, 0x5a88
  19f39c: 01 00 10 26  	addiu	$16, $16, 0x1
  19f3a0: 1c 00 23 8e  	lw	$3, 0x1c($17)
  19f3a4: 20 00 04 2a  	slti	$4, $16, 0x20
  19f3a8: 21 10 43 00  	addu	$2, $2, $3
  19f3ac: f9 ff 80 14  	bnez	$4, 0x19f394 <.text+0x9f394>
  19f3b0: 00 00 40 ac  	sw	$zero, 0x0($2)
  19f3b4: 0c 00 23 8e  	lw	$3, 0xc($17)
  19f3b8: 1a 00 02 3c  	lui	$2, 0x1a
  19f3bc: dc f2 42 24  	addiu	$2, $2, -0xd24 <.text+0xffffffffffeff2dc>
  19f3c0: 20 00 10 24  	addiu	$16, $zero, 0x20
  19f3c4: 00 00 62 ac  	sw	$2, 0x0($3)
  19f3c8: 1a 00 02 3c  	lui	$2, 0x1a
  19f3cc: e8 f2 42 24  	addiu	$2, $2, -0xd18 <.text+0xffffffffffeff2e8>
  19f3d0: 0c 00 71 ac  	sw	$17, 0xc($3)
  19f3d4: 08 00 62 ac  	sw	$2, 0x8($3)
  19f3d8: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19f3dc: 04 00 71 ac  	sw	$17, 0x4($3)
  19f3e0: ac 73 06 0c  	jal	0x19ceb0 <.text+0x9ceb0>
  19f3e4: 2d 20 00 00  	move	$4, $zero
  19f3e8: 00 10 02 3c  	lui	$2, 0x1000
  19f3ec: 10 e0 42 34  	ori	$2, $2, 0xe010
  19f3f0: 00 00 42 8c  	lw	$2, 0x0($2)
  19f3f4: 20 00 42 30  	andi	$2, $2, 0x20
  19f3f8: 04 00 40 10  	beqz	$2, 0x19f40c <.text+0x9f40c>
  19f3fc: 00 10 02 3c  	lui	$2, 0x1000
  19f400: 01 10 01 3c  	lui	$1, 0x1001
  19f404: 10 e0 30 ac  	sw	$16, -0x1ff0($1)
  19f408: 00 10 02 3c  	lui	$2, 0x1000
  19f40c: 00 c0 42 34  	ori	$2, $2, 0xc000
  19f410: 00 00 42 8c  	lw	$2, 0x0($2)
  19f414: 00 01 42 30  	andi	$2, $2, 0x100
  19f418: 39 00 40 10  	beqz	$2, 0x19f500 <.text+0x9f500>
  19f41c: 00 00 00 00  	nop
  19f420: 1a 00 05 3c  	lui	$5, 0x1a
  19f424: 2d 30 00 00  	move	$6, $zero
  19f428: f0 fb a5 24  	addiu	$5, $5, -0x410 <.text+0xffffffffffeffbf0>
  19f42c: 68 7d 06 0c  	jal	0x19f5a0 <.text+0x9f5a0>
  19f430: 05 00 04 24  	addiu	$4, $zero, 0x5
  19f434: 42 00 03 3c  	lui	$3, 0x42
  19f438: 05 00 04 24  	addiu	$4, $zero, 0x5
  19f43c: c0 7e 06 0c  	jal	0x19fb00 <.text+0x9fb00>
  19f440: ac 5a 62 ac  	sw	$2, 0x5aac($3)
  19f444: 42 00 02 3c  	lui	$2, 0x42
  19f448: 01 00 03 24  	addiu	$3, $zero, 0x1
  19f44c: 00 80 04 3c  	lui	$4, 0x8000
  19f450: c0 73 06 0c  	jal	0x19cf00 <.text+0x9cf00>
  19f454: a8 5a 43 ac  	sw	$3, 0x5aa8($2)
  19f458: 0d 00 40 10  	beqz	$2, 0x19f490 <.text+0x9f490>
  19f45c: 08 00 22 ae  	sw	$2, 0x8($17)
  19f460: 88 5a 42 8e  	lw	$2, 0x5a88($18)
  19f464: 00 80 04 3c  	lui	$4, 0x8000
  19f468: 2d 28 a0 03  	move	$5, $sp
  19f46c: 14 00 06 24  	addiu	$6, $zero, 0x14
  19f470: 2d 38 00 00  	move	$7, $zero
  19f474: 2d 40 00 00  	move	$8, $zero
  19f478: 2d 48 00 00  	move	$9, $zero
  19f47c: 10 00 a2 af  	sw	$2, 0x10($sp)
  19f480: 99 7c 06 0c  	jal	0x19f264 <.text+0x9f264>
  19f484: 00 00 00 00  	nop
  19f488: a7 ff 00 10  	b	0x19f328 <.text+0x9f328>
  19f48c: 50 00 bf df  	ld	$ra, 0x50($sp)
  19f490: c0 73 06 0c  	jal	0x19cf00 <.text+0x9cf00>
  19f494: 04 00 04 24  	addiu	$4, $zero, 0x4
  19f498: 02 00 03 3c  	lui	$3, 0x2
  19f49c: 24 10 43 00  	and	$2, $2, $3
  19f4a0: fb ff 40 10  	beqz	$2, 0x19f490 <.text+0x9f490>
  19f4a4: 02 00 04 24  	addiu	$4, $zero, 0x2
  19f4a8: c0 73 06 0c  	jal	0x19cf00 <.text+0x9cf00>
  19f4ac: 42 00 11 3c  	lui	$17, 0x42
  19f4b0: 88 5a 30 26  	addiu	$16, $17, 0x5a88
  19f4b4: 08 00 02 ae  	sw	$2, 0x8($16)
  19f4b8: 2d 28 40 00  	move	$5, $2
  19f4bc: bc 73 06 0c  	jal	0x19cef0 <.text+0x9cef0>
  19f4c0: 00 80 04 3c  	lui	$4, 0x8000
  19f4c4: 2d 28 00 02  	move	$5, $16
  19f4c8: 00 80 04 3c  	lui	$4, 0x8000
  19f4cc: bc 73 06 0c  	jal	0x19cef0 <.text+0x9cef0>
  19f4d0: 01 00 84 34  	ori	$4, $4, 0x1
  19f4d4: 0c 00 a0 af  	sw	$zero, 0xc($sp)
  19f4d8: 88 5a 22 8e  	lw	$2, 0x5a88($17)
  19f4dc: 00 80 04 3c  	lui	$4, 0x8000
  19f4e0: 2d 48 00 00  	move	$9, $zero
  19f4e4: 02 00 84 34  	ori	$4, $4, 0x2
  19f4e8: 10 00 a2 af  	sw	$2, 0x10($sp)
  19f4ec: 2d 28 a0 03  	move	$5, $sp
  19f4f0: 14 00 06 24  	addiu	$6, $zero, 0x14
  19f4f4: 2d 38 00 00  	move	$7, $zero
  19f4f8: e1 ff 00 10  	b	0x19f480 <.text+0x9f480>
  19f4fc: 2d 40 00 00  	move	$8, $zero
  19f500: 7c 7d 06 0c  	jal	0x19f5f0 <.text+0x9f5f0>
  19f504: 00 00 00 00  	nop
  19f508: c6 ff 00 10  	b	0x19f424 <.text+0x9f424>
  19f50c: 1a 00 05 3c  	lui	$5, 0x1a
  19f510: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19f514: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19f518: de 7e 06 0c  	jal	0x19fb78 <.text+0x9fb78>
  19f51c: 05 00 04 24  	addiu	$4, $zero, 0x5
  19f520: 42 00 02 3c  	lui	$2, 0x42
  19f524: 05 00 04 24  	addiu	$4, $zero, 0x5
  19f528: 6c 7d 06 0c  	jal	0x19f5b0 <.text+0x9f5b0>
  19f52c: ac 5a 45 8c  	lw	$5, 0x5aac($2)
  19f530: 00 00 bf df  	ld	$ra, 0x0($sp)
  19f534: 42 00 02 3c  	lui	$2, 0x42
  19f538: a8 5a 40 ac  	sw	$zero, 0x5aa8($2)
  19f53c: 08 00 e0 03  	jr	$ra
  19f540: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19f544: ff 7f 02 3c  	lui	$2, 0x7fff
  19f548: 42 00 03 3c  	lui	$3, 0x42
  19f54c: ff ff 42 34  	ori	$2, $2, 0xffff
  19f550: 88 5a 63 24  	addiu	$3, $3, 0x5a88
  19f554: 07 00 81 04  	bgez	$4, 0x19f574 <.text+0x9f574>
  19f558: 24 10 82 00  	and	$2, $4, $2
  19f55c: 0c 00 63 8c  	lw	$3, 0xc($3)
  19f560: c0 10 02 00  	sll	$2, $2, 0x3
  19f564: 21 10 43 00  	addu	$2, $2, $3
  19f568: 04 00 46 ac  	sw	$6, 0x4($2)
  19f56c: 08 00 e0 03  	jr	$ra
  19f570: 00 00 45 ac  	sw	$5, 0x0($2)
  19f574: fa ff 00 10  	b	0x19f560 <.text+0x9f560>
  19f578: 14 00 63 8c  	lw	$3, 0x14($3)
  19f57c: 42 00 02 3c  	lui	$2, 0x42
  19f580: 80 20 04 00  	sll	$4, $4, 0x2
  19f584: a4 5a 42 8c  	lw	$2, 0x5aa4($2)
  19f588: 21 20 82 00  	addu	$4, $4, $2
  19f58c: 08 00 e0 03  	jr	$ra
  19f590: 00 00 82 8c  	lw	$2, 0x0($4)
