  1ab3c0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1ab3c4: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1ab3c8: 2d 90 80 00  	move	$18, $4
  1ab3cc: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1ab3d0: 2d 88 a0 00  	move	$17, $5
  1ab3d4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1ab3d8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1ab3dc: 00 60 10 40  	mfc0	$16, $12, 0x0
  1ab3e0: 01 00 02 3c  	lui	$2, 0x1
  1ab3e4: 24 80 02 02  	and	$16, $16, $2
  1ab3e8: 11 00 00 16  	bnez	$16, 0x1ab430 <.text+0xab430>
  1ab3ec: 00 00 00 00  	nop
  1ab3f0: c0 ff 04 24  	addiu	$4, $zero, -0x40 <.text+0xffffffffffefffc0>
  1ab3f4: 24 28 24 02  	and	$5, $17, $4
  1ab3f8: 10 ad 06 0c  	jal	0x1ab440 <.text+0xab440>
  1ab3fc: 24 20 44 02  	and	$4, $18, $4
  1ab400: 07 00 00 16  	bnez	$16, 0x1ab420 <.text+0xab420>
  1ab404: 00 00 00 00  	nop
  1ab408: 30 00 bf df  	ld	$ra, 0x30($sp)
  1ab40c: 20 00 b2 df  	ld	$18, 0x20($sp)
  1ab410: 10 00 b1 df  	ld	$17, 0x10($sp)
  1ab414: 00 00 b0 df  	ld	$16, 0x0($sp)
  1ab418: 08 00 e0 03  	jr	$ra
  1ab41c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1ab420: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  1ab424: 00 00 00 00  	nop
  1ab428: f8 ff 00 10  	b	0x1ab40c <.text+0xab40c>
  1ab42c: 30 00 bf df  	ld	$ra, 0x30($sp)
  1ab430: 06 7c 06 0c  	jal	0x19f018 <.text+0x9f018>
  1ab434: 00 00 00 00  	nop
  1ab438: ee ff 00 10  	b	0x1ab3f4 <.text+0xab3f4>
  1ab43c: c0 ff 04 24  	addiu	$4, $zero, -0x40 <.text+0xffffffffffefffc0>
  1ab440: ff ff 07 3c  	lui	$7, 0xffff
  1ab444: 2d 30 00 00  	move	$6, $zero
  1ab448: 00 f0 e7 34  	ori	$7, $7, 0xf000
  1ab44c: 00 00 00 00  	nop
  1ab450: 0f 00 00 00  	sync
  1ab454: 00 00 d0 bc  	cache	0x10, 0x0($6)
  1ab458: 0f 00 00 00  	sync
  1ab45c: 00 e0 02 40  	mfc0	$2, $28, 0x0
  1ab460: 24 10 47 00  	and	$2, $2, $7
  1ab464: 21 10 46 00  	addu	$2, $2, $6
  1ab468: 2b 18 a2 00  	sltu	$3, $5, $2
  1ab46c: 2b 10 44 00  	sltu	$2, $2, $4
  1ab470: 06 00 40 14  	bnez	$2, 0x1ab48c <.text+0xab48c>
  1ab474: 00 00 00 00  	nop
  1ab478: 04 00 60 14  	bnez	$3, 0x1ab48c <.text+0xab48c>
  1ab47c: 00 00 00 00  	nop
  1ab480: 0f 00 00 00  	sync
  1ab484: 00 00 d4 bc  	cache	0x14, 0x0($6)
  1ab488: 0f 00 00 00  	sync
  1ab48c: 0f 00 00 00  	sync
  1ab490: 01 00 d0 bc  	cache	0x10, 0x1($6)
  1ab494: 0f 00 00 00  	sync
  1ab498: 00 e0 02 40  	mfc0	$2, $28, 0x0
  1ab49c: 24 10 47 00  	and	$2, $2, $7
  1ab4a0: 21 10 46 00  	addu	$2, $2, $6
  1ab4a4: 2b 18 a2 00  	sltu	$3, $5, $2
  1ab4a8: 2b 10 44 00  	sltu	$2, $2, $4
  1ab4ac: 06 00 40 14  	bnez	$2, 0x1ab4c8 <.text+0xab4c8>
  1ab4b0: 00 00 00 00  	nop
  1ab4b4: 04 00 60 14  	bnez	$3, 0x1ab4c8 <.text+0xab4c8>
  1ab4b8: 00 00 00 00  	nop
  1ab4bc: 0f 00 00 00  	sync
  1ab4c0: 01 00 d4 bc  	cache	0x14, 0x1($6)
  1ab4c4: 0f 00 00 00  	sync
  1ab4c8: 0f 00 00 00  	sync
  1ab4cc: 40 00 c6 24  	addiu	$6, $6, 0x40
  1ab4d0: 00 10 c2 28  	slti	$2, $6, 0x1000
  1ab4d4: de ff 40 14  	bnez	$2, 0x1ab450 <.text+0xab450>
  1ab4d8: 00 00 00 00  	nop
  1ab4dc: 08 00 e0 03  	jr	$ra
  1ab4e8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1ab4ec: 82 12 04 00  	srl	$2, $4, 0xa
  1ab4f0: 35 00 03 3c  	lui	$3, 0x35
  1ab4f4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1ab4f8: fc 3f 42 30  	andi	$2, $2, 0x3ffc
  1ab4fc: b0 e2 63 24  	addiu	$3, $3, -0x1d50 <.text+0xffffffffffefe2b0>
  1ab500: 21 10 43 00  	addu	$2, $2, $3
  1ab504: 28 00 45 8c  	lw	$5, 0x28($2)
  1ab508: 12 00 a2 2c  	sltiu	$2, $5, 0x12
  1ab50c: 06 00 40 14  	bnez	$2, 0x1ab528 <.text+0xab528>
  1ab510: 34 00 02 3c  	lui	$2, 0x34
  1ab514: ff ff 82 30  	andi	$2, $4, 0xffff
  1ab518: 21 10 a2 00  	addu	$2, $5, $2
  1ab51c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1ab520: 08 00 e0 03  	jr	$ra
  1ab524: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1ab528: 45 55 42 90  	lbu	$2, 0x5545($2)
  1ab52c: 08 00 40 10  	beqz	$2, 0x1ab550 <.text+0xab550>
  1ab530: 11 00 a2 2c  	sltiu	$2, $5, 0x11
  1ab534: 7f 00 02 3c  	lui	$2, 0x7f
  1ab538: 00 48 03 24  	addiu	$3, $zero, 0x4800
  1ab53c: ff ff 42 34  	ori	$2, $2, 0xffff
  1ab540: 24 10 82 00  	and	$2, $4, $2
  1ab544: 3b 00 43 10  	beq	$2, $3, 0x1ab634 <.text+0xab634>
  1ab548: 41 00 02 3c  	lui	$2, 0x41
  1ab54c: 11 00 a2 2c  	sltiu	$2, $5, 0x11
  1ab550: f2 ff 40 10  	beqz	$2, 0x1ab51c <.text+0xab51c>
  1ab554: 2d 10 00 00  	move	$2, $zero
  1ab558: 1b 00 03 3c  	lui	$3, 0x1b
  1ab55c: 80 10 05 00  	sll	$2, $5, 0x2
  1ab560: 54 1b 63 24  	addiu	$3, $3, 0x1b54
  1ab564: 21 10 43 00  	addu	$2, $2, $3
  1ab568: 00 00 42 8c  	lw	$2, 0x0($2)
  1ab56c: 08 00 40 00  	jr	$2
  1ab570: 00 00 00 00  	nop
  1ab574: 41 00 02 3c  	lui	$2, 0x41
  1ab578: ff ff 83 30  	andi	$3, $4, 0xffff
  1ab57c: 44 35 42 24  	addiu	$2, $2, 0x3544
  1ab580: e6 ff 00 10  	b	0x1ab51c <.text+0xab51c>
  1ab584: 21 10 62 00  	addu	$2, $3, $2
  1ab588: 35 00 02 3c  	lui	$2, 0x35
  1ab58c: ff ff 83 30  	andi	$3, $4, 0xffff
  1ab590: c4 e2 42 8c  	lw	$2, -0x1d3c($2)
  1ab594: 21 10 43 00  	addu	$2, $2, $3
  1ab598: e0 ff 00 10  	b	0x1ab51c <.text+0xab51c>
  1ab59c: 00 e0 42 24  	addiu	$2, $2, -0x2000 <.text+0xffffffffffefe000>
  1ab5a0: 35 00 02 3c  	lui	$2, 0x35
  1ab5a4: ff ff 83 30  	andi	$3, $4, 0xffff
  1ab5a8: c4 e2 42 8c  	lw	$2, -0x1d3c($2)
  1ab5ac: 21 10 43 00  	addu	$2, $2, $3
  1ab5b0: da ff 00 10  	b	0x1ab51c <.text+0xab51c>
  1ab5b4: 00 c0 42 24  	addiu	$2, $2, -0x4000 <.text+0xffffffffffefc000>
  1ab5b8: 35 00 02 3c  	lui	$2, 0x35
  1ab5bc: ff ff 83 30  	andi	$3, $4, 0xffff
  1ab5c0: c4 e2 42 8c  	lw	$2, -0x1d3c($2)
  1ab5c4: 21 10 43 00  	addu	$2, $2, $3
  1ab5c8: d4 ff 00 10  	b	0x1ab51c <.text+0xab51c>
  1ab5cc: 00 a0 42 24  	addiu	$2, $2, -0x6000 <.text+0xffffffffffefa000>
  1ab5d0: 35 00 02 3c  	lui	$2, 0x35
  1ab5d4: ff ff 83 30  	andi	$3, $4, 0xffff
  1ab5d8: fa ff 00 10  	b	0x1ab5c4 <.text+0xab5c4>
  1ab5dc: bc e2 42 8c  	lw	$2, -0x1d44($2)
  1ab5e0: 35 00 02 3c  	lui	$2, 0x35
  1ab5e4: ff ff 83 30  	andi	$3, $4, 0xffff
  1ab5e8: f6 ff 00 10  	b	0x1ab5c4 <.text+0xab5c4>
  1ab5ec: c8 e2 42 8c  	lw	$2, -0x1d38($2)
  1ab5f0: 35 00 02 3c  	lui	$2, 0x35
  1ab5f4: ff ff 83 30  	andi	$3, $4, 0xffff
  1ab5f8: f2 ff 00 10  	b	0x1ab5c4 <.text+0xab5c4>
  1ab5fc: c0 e2 42 8c  	lw	$2, -0x1d40($2)
  1ab600: 35 00 03 3c  	lui	$3, 0x35
  1ab604: ff ff 82 30  	andi	$2, $4, 0xffff
  1ab608: dd ff 00 10  	b	0x1ab580 <.text+0xab580>
  1ab60c: bc e2 63 8c  	lw	$3, -0x1d44($3)
  1ab610: c2 ff 00 10  	b	0x1ab51c <.text+0xab51c>
  1ab614: 2d 10 00 00  	move	$2, $zero
  1ab618: f7 63 05 0c  	jal	0x158fdc <.text+0x58fdc>
  1ab61c: 00 00 00 00  	nop
  1ab620: bf ff 00 10  	b	0x1ab520 <.text+0xab520>
  1ab624: 00 00 bf df  	ld	$ra, 0x0($sp)
  1ab628: 35 00 02 3c  	lui	$2, 0x35
  1ab62c: bb ff 00 10  	b	0x1ab51c <.text+0xab51c>
  1ab630: bc e2 42 8c  	lw	$2, -0x1d44($2)
  1ab634: b9 ff 00 10  	b	0x1ab51c <.text+0xab51c>
  1ab638: 44 35 42 24  	addiu	$2, $2, 0x3544
  1ab63c: 02 1b 04 00  	srl	$3, $4, 0xc
  1ab640: 35 00 02 3c  	lui	$2, 0x35
  1ab644: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1ab648: ff 0f 63 30  	andi	$3, $3, 0xfff
  1ab64c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1ab650: 2d 38 80 00  	move	$7, $4
  1ab654: b0 e2 44 24  	addiu	$4, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1ab658: 80 10 03 00  	sll	$2, $3, 0x2
  1ab65c: 21 10 44 00  	addu	$2, $2, $4
  1ab660: 28 00 46 8c  	lw	$6, 0x28($2)
  1ab664: 12 00 c2 2c  	sltiu	$2, $6, 0x12
  1ab668: 17 00 40 54  	bnezl	$2, 0x1ab6c8 <.text+0xab6c8>
  1ab66c: 1b 00 03 3c  	lui	$3, 0x1b
  1ab670: 34 00 02 3c  	lui	$2, 0x34
  1ab674: 21 20 64 00  	addu	$4, $3, $4
  1ab678: 40 53 45 24  	addiu	$5, $2, 0x5340
  1ab67c: 20 90 03 34  	ori	$3, $zero, 0x9020
  1ab680: 20 80 02 34  	ori	$2, $zero, 0x8020
  1ab684: 21 10 82 00  	addu	$2, $4, $2
  1ab688: 21 20 83 00  	addu	$4, $4, $3
  1ab68c: 08 00 43 90  	lbu	$3, 0x8($2)
  1ab690: 20 00 a2 dc  	ld	$2, 0x20($5)
  1ab694: 08 00 84 90  	lbu	$4, 0x8($4)
  1ab698: 2d 10 43 00  	daddu	$2, $2, $3
  1ab69c: 03 00 80 10  	beqz	$4, 0x1ab6ac <.text+0xab6ac>
  1ab6a0: 20 00 a2 fc  	sd	$2, 0x20($5)
  1ab6a4: 14 00 a2 8c  	lw	$2, 0x14($5)
  1ab6a8: 18 00 a2 ac  	sw	$2, 0x18($5)
  1ab6ac: ff ff e2 30  	andi	$2, $7, 0xffff
  1ab6b0: 21 10 c2 00  	addu	$2, $6, $2
  1ab6b4: 00 00 44 90  	lbu	$4, 0x0($2)
  1ab6b8: 00 00 bf df  	ld	$ra, 0x0($sp)
  1ab6bc: 2d 10 80 00  	move	$2, $4
  1ab6c0: 08 00 e0 03  	jr	$ra
  1ab6c4: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1ab6c8: 80 10 06 00  	sll	$2, $6, 0x2
  1ab6cc: 80 1c 63 24  	addiu	$3, $3, 0x1c80
  1ab6d0: 21 10 43 00  	addu	$2, $2, $3
  1ab6d4: 00 00 42 8c  	lw	$2, 0x0($2)
  1ab6d8: 08 00 40 00  	jr	$2
  1ab6dc: 00 00 00 00  	nop
  1ab6e0: 34 00 02 3c  	lui	$2, 0x34
  1ab6e4: 40 53 43 24  	addiu	$3, $2, 0x5340
  1ab6e8: 08 00 62 90  	lbu	$2, 0x8($3)
  1ab6ec: 04 00 40 14  	bnez	$2, 0x1ab700 <.text+0xab700>
  1ab6f0: 00 00 00 00  	nop
  1ab6f4: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab6f8: 06 00 42 64  	daddiu	$2, $2, 0x6
  1ab6fc: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab700: 7c 69 05 0c  	jal	0x15a5f0 <.text+0x5a5f0>
  1ab704: ff ff e4 30  	andi	$4, $7, 0xffff
  1ab708: eb ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab70c: 2d 20 40 00  	move	$4, $2
  1ab710: 34 00 03 3c  	lui	$3, 0x34
  1ab714: ff ff e4 30  	andi	$4, $7, 0xffff
  1ab718: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab71c: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab720: 06 00 42 64  	daddiu	$2, $2, 0x6
  1ab724: 1c 6f 05 0c  	jal	0x15bc70 <.text+0x5bc70>
  1ab728: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab72c: e2 ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab730: 2d 20 40 00  	move	$4, $2
  1ab734: 34 00 03 3c  	lui	$3, 0x34
  1ab738: ff ff e4 30  	andi	$4, $7, 0xffff
  1ab73c: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab740: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab744: 08 00 42 64  	daddiu	$2, $2, 0x8
  1ab748: c1 b9 04 0c  	jal	0x12e704 <.text+0x2e704>
  1ab74c: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab750: d9 ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab754: 2d 20 40 00  	move	$4, $2
  1ab758: 36 00 02 3c  	lui	$2, 0x36
  1ab75c: d6 ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab760: 68 b7 44 90  	lbu	$4, -0x4898($2)
  1ab764: ca 30 04 0c  	jal	0x10c328 <.text+0xc328>
  1ab768: ff ff e4 30  	andi	$4, $7, 0xffff
  1ab76c: d2 ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab770: 2d 20 40 00  	move	$4, $2
  1ab774: 34 00 05 3c  	lui	$5, 0x34
  1ab778: 35 00 02 3c  	lui	$2, 0x35
  1ab77c: 40 53 a5 24  	addiu	$5, $5, 0x5340
  1ab780: c0 e2 44 8c  	lw	$4, -0x1d40($2)
  1ab784: 20 00 a2 dc  	ld	$2, 0x20($5)
  1ab788: ff 7f e3 30  	andi	$3, $7, 0x7fff
  1ab78c: 21 20 83 00  	addu	$4, $4, $3
  1ab790: 08 00 42 64  	daddiu	$2, $2, 0x8
  1ab794: 20 00 a2 fc  	sd	$2, 0x20($5)
  1ab798: c7 ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab79c: 00 a0 84 90  	lbu	$4, -0x6000($4)
  1ab7a0: 34 00 03 3c  	lui	$3, 0x34
  1ab7a4: 36 00 02 3c  	lui	$2, 0x36
  1ab7a8: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab7ac: 68 b7 44 90  	lbu	$4, -0x4898($2)
  1ab7b0: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab7b4: 08 00 42 64  	daddiu	$2, $2, 0x8
  1ab7b8: bf ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab7bc: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab7c0: 34 00 06 3c  	lui	$6, 0x34
  1ab7c4: 35 00 03 3c  	lui	$3, 0x35
  1ab7c8: ff 00 02 3c  	lui	$2, 0xff
  1ab7cc: 40 53 c6 24  	addiu	$6, $6, 0x5340
  1ab7d0: b0 e2 63 24  	addiu	$3, $3, -0x1d50 <.text+0xffffffffffefe2b0>
  1ab7d4: 24 10 e2 00  	and	$2, $7, $2
  1ab7d8: 20 00 c4 dc  	ld	$4, 0x20($6)
  1ab7dc: ff 7f e5 30  	andi	$5, $7, 0x7fff
  1ab7e0: 42 10 02 00  	srl	$2, $2, 0x1
  1ab7e4: 20 00 67 8c  	lw	$7, 0x20($3)
  1ab7e8: 25 10 45 00  	or	$2, $2, $5
  1ab7ec: 0c 00 63 8c  	lw	$3, 0xc($3)
  1ab7f0: 08 00 84 64  	daddiu	$4, $4, 0x8
  1ab7f4: 24 10 47 00  	and	$2, $2, $7
  1ab7f8: 20 00 c4 fc  	sd	$4, 0x20($6)
  1ab7fc: 21 18 62 00  	addu	$3, $3, $2
  1ab800: ad ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab804: 00 00 64 90  	lbu	$4, 0x0($3)
  1ab808: 34 00 03 3c  	lui	$3, 0x34
  1ab80c: 2d 20 e0 00  	move	$4, $7
  1ab810: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab814: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab818: 08 00 42 64  	daddiu	$2, $2, 0x8
  1ab81c: 57 09 06 0c  	jal	0x18255c <.text+0x8255c>
  1ab820: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab824: a4 ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab828: 2d 20 40 00  	move	$4, $2
  1ab82c: 34 00 03 3c  	lui	$3, 0x34
  1ab830: 00 48 04 24  	addiu	$4, $zero, 0x4800
  1ab834: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab838: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab83c: 08 00 42 64  	daddiu	$2, $2, 0x8
  1ab840: fc 04 06 0c  	jal	0x1813f0 <.text+0x813f0>
  1ab844: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab848: 9b ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab84c: 2d 20 40 00  	move	$4, $2
  1ab850: 34 00 06 3c  	lui	$6, 0x34
  1ab854: 0f 00 04 3c  	lui	$4, 0xf
  1ab858: 40 53 c6 24  	addiu	$6, $6, 0x5340
  1ab85c: 24 20 e4 00  	and	$4, $7, $4
  1ab860: 20 00 c5 dc  	ld	$5, 0x20($6)
  1ab864: 35 00 03 3c  	lui	$3, 0x35
  1ab868: b0 e2 63 24  	addiu	$3, $3, -0x1d50 <.text+0xffffffffffefe2b0>
  1ab86c: ff 7f e2 30  	andi	$2, $7, 0x7fff
  1ab870: c2 20 04 00  	srl	$4, $4, 0x3
  1ab874: 08 00 a5 64  	daddiu	$5, $5, 0x8
  1ab878: 20 00 67 8c  	lw	$7, 0x20($3)
  1ab87c: 21 10 44 00  	addu	$2, $2, $4
  1ab880: 00 a0 42 24  	addiu	$2, $2, -0x6000 <.text+0xffffffffffefa000>
  1ab884: 0c 00 63 8c  	lw	$3, 0xc($3)
  1ab888: 20 00 c5 fc  	sd	$5, 0x20($6)
  1ab88c: db ff 00 10  	b	0x1ab7fc <.text+0xab7fc>
  1ab890: 24 10 47 00  	and	$2, $2, $7
  1ab894: 34 00 03 3c  	lui	$3, 0x34
  1ab898: ff ff e4 30  	andi	$4, $7, 0xffff
  1ab89c: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab8a0: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab8a4: 08 00 42 64  	daddiu	$2, $2, 0x8
  1ab8a8: d7 62 05 0c  	jal	0x158b5c <.text+0x58b5c>
  1ab8ac: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab8b0: 81 ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab8b4: 2d 20 40 00  	move	$4, $2
  1ab8b8: 34 00 03 3c  	lui	$3, 0x34
  1ab8bc: 2d 20 e0 00  	move	$4, $7
  1ab8c0: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab8c4: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab8c8: 08 00 42 64  	daddiu	$2, $2, 0x8
  1ab8cc: 12 bf 05 0c  	jal	0x16fc48 <.text+0x6fc48>
  1ab8d0: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab8d4: 78 ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab8d8: 2d 20 40 00  	move	$4, $2
  1ab8dc: 34 00 03 3c  	lui	$3, 0x34
  1ab8e0: 2d 20 e0 00  	move	$4, $7
  1ab8e4: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab8e8: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab8ec: 08 00 42 64  	daddiu	$2, $2, 0x8
  1ab8f0: 7f c0 05 0c  	jal	0x1701fc <.text+0x701fc>
  1ab8f4: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab8f8: 6f ff 00 10  	b	0x1ab6b8 <.text+0xab6b8>
  1ab8fc: 2d 20 40 00  	move	$4, $2
  1ab900: 02 1b 05 00  	srl	$3, $5, 0xc
  1ab904: 35 00 02 3c  	lui	$2, 0x35
  1ab908: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1ab90c: 2d 38 a0 00  	move	$7, $5
  1ab910: ff 0f 65 30  	andi	$5, $3, 0xfff
  1ab914: b0 e2 48 24  	addiu	$8, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1ab918: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1ab91c: 80 18 05 00  	sll	$3, $5, 0x2
  1ab920: 21 18 68 00  	addu	$3, $3, $8
  1ab924: 34 00 02 3c  	lui	$2, 0x34
  1ab928: 28 40 66 8c  	lw	$6, 0x4028($3)
  1ab92c: 40 53 4a 24  	addiu	$10, $2, 0x5340
  1ab930: ff 00 89 30  	andi	$9, $4, 0xff
  1ab934: 12 00 c2 2c  	sltiu	$2, $6, 0x12
  1ab938: 1a 00 40 14  	bnez	$2, 0x1ab9a4 <.text+0xab9a4>
  1ab93c: 18 00 40 ad  	sw	$zero, 0x18($10)
  1ab940: 21 10 a8 00  	addu	$2, $5, $8
  1ab944: 20 80 03 34  	ori	$3, $zero, 0x8020
  1ab948: 21 10 43 00  	addu	$2, $2, $3
  1ab94c: 34 00 0b 3c  	lui	$11, 0x34
  1ab950: 08 00 45 90  	lbu	$5, 0x8($2)
  1ab954: f8 5a 68 25  	addiu	$8, $11, 0x5af8
  1ab958: 20 00 42 dd  	ld	$2, 0x20($10)
  1ab95c: ff ff e3 30  	andi	$3, $7, 0xffff
  1ab960: 38 00 04 8d  	lw	$4, 0x38($8)
  1ab964: 21 30 c3 00  	addu	$6, $6, $3
  1ab968: 2d 10 45 00  	daddu	$2, $2, $5
  1ab96c: 08 00 c4 10  	beq	$6, $4, 0x1ab990 <.text+0xab990>
  1ab970: 20 00 42 fd  	sd	$2, 0x20($10)
  1ab974: 3c 00 02 8d  	lw	$2, 0x3c($8)
  1ab978: 06 00 c2 10  	beq	$6, $2, 0x1ab994 <.text+0xab994>
  1ab97c: f8 5a 62 8d  	lw	$2, 0x5af8($11)
  1ab980: 00 00 c9 a0  	sb	$9, 0x0($6)
  1ab984: 00 00 bf df  	ld	$ra, 0x0($sp)
  1ab988: 08 00 e0 03  	jr	$ra
  1ab98c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1ab990: f8 5a 62 8d  	lw	$2, 0x5af8($11)
  1ab994: 34 00 00 ad  	sw	$zero, 0x34($8)
  1ab998: 2b 10 02 00  	sltu	$2, $zero, $2
  1ab99c: f8 ff 00 10  	b	0x1ab980 <.text+0xab980>
  1ab9a0: 18 00 02 a1  	sb	$2, 0x18($8)
  1ab9a4: 1b 00 03 3c  	lui	$3, 0x1b
  1ab9a8: 80 10 06 00  	sll	$2, $6, 0x2
  1ab9ac: c8 1c 63 24  	addiu	$3, $3, 0x1cc8
  1ab9b0: 21 10 43 00  	addu	$2, $2, $3
  1ab9b4: 00 00 42 8c  	lw	$2, 0x0($2)
  1ab9b8: 08 00 40 00  	jr	$2
  1ab9bc: 00 00 00 00  	nop
  1ab9c0: 34 00 02 3c  	lui	$2, 0x34
  1ab9c4: 40 53 43 24  	addiu	$3, $2, 0x5340
  1ab9c8: 08 00 62 90  	lbu	$2, 0x8($3)
  1ab9cc: 04 00 40 14  	bnez	$2, 0x1ab9e0 <.text+0xab9e0>
  1ab9d0: ff ff e5 30  	andi	$5, $7, 0xffff
  1ab9d4: 20 00 62 dc  	ld	$2, 0x20($3)
  1ab9d8: 06 00 42 64  	daddiu	$2, $2, 0x6
  1ab9dc: 20 00 62 fc  	sd	$2, 0x20($3)
  1ab9e0: 9a 64 05 0c  	jal	0x159268 <.text+0x59268>
  1ab9e4: 2d 20 20 01  	move	$4, $9
  1ab9e8: e7 ff 00 10  	b	0x1ab988 <.text+0xab988>
  1ab9ec: 00 00 bf df  	ld	$ra, 0x0($sp)
  1ab9f0: 34 00 03 3c  	lui	$3, 0x34
  1ab9f4: ff ff e5 30  	andi	$5, $7, 0xffff
  1ab9f8: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ab9fc: 2d 20 20 01  	move	$4, $9
  1aba00: 20 00 62 dc  	ld	$2, 0x20($3)
  1aba04: 06 00 42 64  	daddiu	$2, $2, 0x6
  1aba08: 5b 6b 05 0c  	jal	0x15ad6c <.text+0x5ad6c>
  1aba0c: 20 00 62 fc  	sd	$2, 0x20($3)
  1aba10: dd ff 00 10  	b	0x1ab988 <.text+0xab988>
  1aba14: 00 00 bf df  	ld	$ra, 0x0($sp)
  1aba18: 34 00 03 3c  	lui	$3, 0x34
  1aba1c: ff ff e5 30  	andi	$5, $7, 0xffff
  1aba20: 40 53 63 24  	addiu	$3, $3, 0x5340
  1aba24: 2d 20 20 01  	move	$4, $9
  1aba28: 20 00 62 dc  	ld	$2, 0x20($3)
  1aba2c: 08 00 42 64  	daddiu	$2, $2, 0x8
  1aba30: ca b9 04 0c  	jal	0x12e728 <.text+0x2e728>
  1aba34: 20 00 62 fc  	sd	$2, 0x20($3)
  1aba38: d3 ff 00 10  	b	0x1ab988 <.text+0xab988>
  1aba3c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1aba40: 34 00 02 3c  	lui	$2, 0x34
  1aba44: 40 53 46 24  	addiu	$6, $2, 0x5340
  1aba48: 35 00 02 3c  	lui	$2, 0x35
  1aba4c: b0 e2 43 24  	addiu	$3, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1aba50: 20 00 c2 dc  	ld	$2, 0x20($6)
  1aba54: 20 00 65 8c  	lw	$5, 0x20($3)
  1aba58: 08 00 42 64  	daddiu	$2, $2, 0x8
  1aba5c: c9 ff a0 10  	beqz	$5, 0x1ab984 <.text+0xab984>
  1aba60: 20 00 c2 fc  	sd	$2, 0x20($6)
  1aba64: ff 00 02 3c  	lui	$2, 0xff
  1aba68: ff 7f e4 30  	andi	$4, $7, 0x7fff
  1aba6c: 24 10 e2 00  	and	$2, $7, $2
  1aba70: 0c 00 63 8c  	lw	$3, 0xc($3)
  1aba74: 42 10 02 00  	srl	$2, $2, 0x1
  1aba78: 25 10 44 00  	or	$2, $2, $4
  1aba7c: 24 10 45 00  	and	$2, $2, $5
  1aba80: 21 18 62 00  	addu	$3, $3, $2
  1aba84: 00 00 69 a0  	sb	$9, 0x0($3)
  1aba88: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aba8c: bd ff 00 10  	b	0x1ab984 <.text+0xab984>
  1aba90: 54 00 c2 a0  	sb	$2, 0x54($6)
  1aba94: 34 00 02 3c  	lui	$2, 0x34
  1aba98: 40 53 46 24  	addiu	$6, $2, 0x5340
  1aba9c: 35 00 02 3c  	lui	$2, 0x35
  1abaa0: b0 e2 44 24  	addiu	$4, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1abaa4: 20 00 c2 dc  	ld	$2, 0x20($6)
  1abaa8: 20 00 85 8c  	lw	$5, 0x20($4)
  1abaac: 08 00 42 64  	daddiu	$2, $2, 0x8
  1abab0: b4 ff a0 10  	beqz	$5, 0x1ab984 <.text+0xab984>
  1abab4: 20 00 c2 fc  	sd	$2, 0x20($6)
  1abab8: 0f 00 03 3c  	lui	$3, 0xf
  1ababc: ff 7f e2 30  	andi	$2, $7, 0x7fff
  1abac0: 24 18 e3 00  	and	$3, $7, $3
  1abac4: 0c 00 84 8c  	lw	$4, 0xc($4)
  1abac8: c2 18 03 00  	srl	$3, $3, 0x3
  1abacc: 21 10 43 00  	addu	$2, $2, $3
  1abad0: 00 a0 42 24  	addiu	$2, $2, -0x6000 <.text+0xffffffffffefa000>
  1abad4: 24 10 45 00  	and	$2, $2, $5
  1abad8: 21 20 82 00  	addu	$4, $4, $2
  1abadc: ea ff 00 10  	b	0x1aba88 <.text+0xaba88>
  1abae0: 00 00 89 a0  	sb	$9, 0x0($4)
  1abae4: ff ff e5 30  	andi	$5, $7, 0xffff
  1abae8: f7 35 04 0c  	jal	0x10d7dc <.text+0xd7dc>
  1abaec: 2d 20 20 01  	move	$4, $9
  1abaf0: a5 ff 00 10  	b	0x1ab988 <.text+0xab988>
  1abaf4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1abaf8: 34 00 05 3c  	lui	$5, 0x34
  1abafc: 35 00 02 3c  	lui	$2, 0x35
  1abb00: 40 53 a5 24  	addiu	$5, $5, 0x5340
  1abb04: c0 e2 44 8c  	lw	$4, -0x1d40($2)
  1abb08: 20 00 a2 dc  	ld	$2, 0x20($5)
  1abb0c: ff 7f e3 30  	andi	$3, $7, 0x7fff
  1abb10: 21 20 83 00  	addu	$4, $4, $3
  1abb14: 08 00 42 64  	daddiu	$2, $2, 0x8
  1abb18: 20 00 a2 fc  	sd	$2, 0x20($5)
  1abb1c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1abb20: 00 a0 89 a0  	sb	$9, -0x6000($4)
  1abb24: 97 ff 00 10  	b	0x1ab984 <.text+0xab984>
  1abb28: 54 00 a2 a0  	sb	$2, 0x54($5)
  1abb2c: 34 00 05 3c  	lui	$5, 0x34
  1abb30: 35 00 02 3c  	lui	$2, 0x35
  1abb34: 40 53 a5 24  	addiu	$5, $5, 0x5340
  1abb38: bc e2 46 8c  	lw	$6, -0x1d44($2)
  1abb3c: 20 00 a4 dc  	ld	$4, 0x20($5)
  1abb40: ff ff e2 30  	andi	$2, $7, 0xffff
  1abb44: 21 30 c2 00  	addu	$6, $6, $2
  1abb48: 34 00 03 3c  	lui	$3, 0x34
  1abb4c: 08 00 84 64  	daddiu	$4, $4, 0x8
  1abb50: f8 5a 63 24  	addiu	$3, $3, 0x5af8
  1abb54: 20 00 a4 fc  	sd	$4, 0x20($5)
  1abb58: 00 00 c9 a0  	sb	$9, 0x0($6)
  1abb5c: 1c 00 62 90  	lbu	$2, 0x1c($3)
  1abb60: 01 00 42 2c  	sltiu	$2, $2, 0x1
  1abb64: 87 ff 00 10  	b	0x1ab984 <.text+0xab984>
  1abb68: 18 00 62 a0  	sb	$2, 0x18($3)
  1abb6c: 34 00 05 3c  	lui	$5, 0x34
  1abb70: 41 00 02 3c  	lui	$2, 0x41
  1abb74: 40 53 a5 24  	addiu	$5, $5, 0x5340
  1abb78: ff ff e3 30  	andi	$3, $7, 0xffff
  1abb7c: 20 00 a4 dc  	ld	$4, 0x20($5)
  1abb80: 08 35 42 24  	addiu	$2, $2, 0x3508
  1abb84: 21 18 62 00  	addu	$3, $3, $2
  1abb88: 08 00 84 64  	daddiu	$4, $4, 0x8
  1abb8c: 3c 00 69 a0  	sb	$9, 0x3c($3)
  1abb90: 7c ff 00 10  	b	0x1ab984 <.text+0xab984>
  1abb94: 20 00 a4 fc  	sd	$4, 0x20($5)
  1abb98: 34 00 03 3c  	lui	$3, 0x34
  1abb9c: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abba0: 20 00 62 dc  	ld	$2, 0x20($3)
  1abba4: 08 00 42 64  	daddiu	$2, $2, 0x8
  1abba8: 76 ff 00 10  	b	0x1ab984 <.text+0xab984>
  1abbac: 20 00 62 fc  	sd	$2, 0x20($3)
  1abbb0: 34 00 03 3c  	lui	$3, 0x34
  1abbb4: ff ff e5 30  	andi	$5, $7, 0xffff
  1abbb8: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abbbc: 2d 20 20 01  	move	$4, $9
  1abbc0: 20 00 62 dc  	ld	$2, 0x20($3)
  1abbc4: 08 00 42 64  	daddiu	$2, $2, 0x8
  1abbc8: dd 62 05 0c  	jal	0x158b74 <.text+0x58b74>
  1abbcc: 20 00 62 fc  	sd	$2, 0x20($3)
  1abbd0: 6d ff 00 10  	b	0x1ab988 <.text+0xab988>
  1abbd4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1abbd8: 34 00 03 3c  	lui	$3, 0x34
  1abbdc: 2d 20 e0 00  	move	$4, $7
  1abbe0: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abbe4: 2d 28 20 01  	move	$5, $9
  1abbe8: 20 00 62 dc  	ld	$2, 0x20($3)
  1abbec: 08 00 42 64  	daddiu	$2, $2, 0x8
  1abbf0: 1b bf 05 0c  	jal	0x16fc6c <.text+0x6fc6c>
  1abbf4: 20 00 62 fc  	sd	$2, 0x20($3)
  1abbf8: 63 ff 00 10  	b	0x1ab988 <.text+0xab988>
  1abbfc: 00 00 bf df  	ld	$ra, 0x0($sp)
  1abc00: 34 00 03 3c  	lui	$3, 0x34
  1abc04: 2d 20 e0 00  	move	$4, $7
  1abc08: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abc0c: 2d 28 20 01  	move	$5, $9
  1abc10: 20 00 62 dc  	ld	$2, 0x20($3)
  1abc14: 08 00 42 64  	daddiu	$2, $2, 0x8
  1abc18: 81 c0 05 0c  	jal	0x170204 <.text+0x70204>
  1abc1c: 20 00 62 fc  	sd	$2, 0x20($3)
  1abc20: 59 ff 00 10  	b	0x1ab988 <.text+0xab988>
  1abc24: 00 00 bf df  	ld	$ra, 0x0($sp)
  1abc28: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1abc2c: ff 0f 83 30  	andi	$3, $4, 0xfff
  1abc30: ff 0f 02 24  	addiu	$2, $zero, 0xfff
  1abc34: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1abc38: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1abc3c: 2d 88 80 00  	move	$17, $4
  1abc40: ef 00 62 10  	beq	$3, $2, 0x1ac000 <.text+0xac000>
  1abc44: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1abc48: 02 13 04 00  	srl	$2, $4, 0xc
  1abc4c: ff 0f 45 30  	andi	$5, $2, 0xfff
  1abc50: 35 00 02 3c  	lui	$2, 0x35
  1abc54: b0 e2 43 24  	addiu	$3, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1abc58: 80 10 05 00  	sll	$2, $5, 0x2
  1abc5c: 21 10 43 00  	addu	$2, $2, $3
  1abc60: 28 00 47 8c  	lw	$7, 0x28($2)
  1abc64: 12 00 e2 2c  	sltiu	$2, $7, 0x12
  1abc68: 1d 00 40 54  	bnezl	$2, 0x1abce0 <.text+0xabce0>
  1abc6c: 1b 00 03 3c  	lui	$3, 0x1b
  1abc70: 21 28 a3 00  	addu	$5, $5, $3
  1abc74: 20 80 02 34  	ori	$2, $zero, 0x8020
  1abc78: 21 10 a2 00  	addu	$2, $5, $2
  1abc7c: 34 00 03 3c  	lui	$3, 0x34
  1abc80: 40 53 66 24  	addiu	$6, $3, 0x5340
  1abc84: 08 00 43 90  	lbu	$3, 0x8($2)
  1abc88: 20 90 02 34  	ori	$2, $zero, 0x9020
  1abc8c: 20 00 c4 dc  	ld	$4, 0x20($6)
  1abc90: 21 28 a2 00  	addu	$5, $5, $2
  1abc94: 78 18 03 00  	dsll	$3, $3, 0x1
  1abc98: 08 00 a2 90  	lbu	$2, 0x8($5)
  1abc9c: 2d 20 83 00  	daddu	$4, $4, $3
  1abca0: 03 00 40 10  	beqz	$2, 0x1abcb0 <.text+0xabcb0>
  1abca4: 20 00 c4 fc  	sd	$4, 0x20($6)
  1abca8: 14 00 c2 8c  	lw	$2, 0x14($6)
  1abcac: 18 00 c2 ac  	sw	$2, 0x18($6)
  1abcb0: ff ff 22 32  	andi	$2, $17, 0xffff
  1abcb4: 21 10 e2 00  	addu	$2, $7, $2
  1abcb8: 03 00 50 88  	lwl	$16, 0x3($2)
  1abcbc: 00 00 50 98  	lwr	$16, 0x0($2)
  1abcc0: ff ff 10 32  	andi	$16, $16, 0xffff
  1abcc4: ff ff 04 32  	andi	$4, $16, 0xffff
  1abcc8: 20 00 bf df  	ld	$ra, 0x20($sp)
  1abccc: 2d 10 80 00  	move	$2, $4
  1abcd0: 10 00 b1 df  	ld	$17, 0x10($sp)
  1abcd4: 00 00 b0 df  	ld	$16, 0x0($sp)
  1abcd8: 08 00 e0 03  	jr	$ra
  1abcdc: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1abce0: 80 10 07 00  	sll	$2, $7, 0x2
  1abce4: 10 1d 63 24  	addiu	$3, $3, 0x1d10
  1abce8: 21 10 43 00  	addu	$2, $2, $3
  1abcec: 00 00 42 8c  	lw	$2, 0x0($2)
  1abcf0: 08 00 40 00  	jr	$2
  1abcf4: 00 00 00 00  	nop
  1abcf8: 34 00 02 3c  	lui	$2, 0x34
  1abcfc: 40 53 43 24  	addiu	$3, $2, 0x5340
  1abd00: 08 00 62 90  	lbu	$2, 0x8($3)
  1abd04: 04 00 40 14  	bnez	$2, 0x1abd18 <.text+0xabd18>
  1abd08: 00 00 00 00  	nop
  1abd0c: 20 00 62 dc  	ld	$2, 0x20($3)
  1abd10: 0c 00 42 64  	daddiu	$2, $2, 0xc
  1abd14: 20 00 62 fc  	sd	$2, 0x20($3)
  1abd18: 7c 69 05 0c  	jal	0x15a5f0 <.text+0x5a5f0>
  1abd1c: ff ff 24 32  	andi	$4, $17, 0xffff
  1abd20: 01 00 24 26  	addiu	$4, $17, 0x1
  1abd24: ff ff 84 30  	andi	$4, $4, 0xffff
  1abd28: 7c 69 05 0c  	jal	0x15a5f0 <.text+0x5a5f0>
  1abd2c: 2d 80 40 00  	move	$16, $2
  1abd30: 00 12 02 00  	sll	$2, $2, 0x8
  1abd34: e3 ff 00 10  	b	0x1abcc4 <.text+0xabcc4>
  1abd38: 25 80 02 02  	or	$16, $16, $2
  1abd3c: 34 00 03 3c  	lui	$3, 0x34
  1abd40: ff ff 84 30  	andi	$4, $4, 0xffff
  1abd44: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abd48: 20 00 62 dc  	ld	$2, 0x20($3)
  1abd4c: 0c 00 42 64  	daddiu	$2, $2, 0xc
  1abd50: 1c 6f 05 0c  	jal	0x15bc70 <.text+0x5bc70>
  1abd54: 20 00 62 fc  	sd	$2, 0x20($3)
  1abd58: 01 00 24 26  	addiu	$4, $17, 0x1
  1abd5c: ff ff 84 30  	andi	$4, $4, 0xffff
  1abd60: 1c 6f 05 0c  	jal	0x15bc70 <.text+0x5bc70>
  1abd64: 2d 80 40 00  	move	$16, $2
  1abd68: f2 ff 00 10  	b	0x1abd34 <.text+0xabd34>
  1abd6c: 00 12 02 00  	sll	$2, $2, 0x8
  1abd70: 34 00 03 3c  	lui	$3, 0x34
  1abd74: ff ff 84 30  	andi	$4, $4, 0xffff
  1abd78: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abd7c: 20 00 62 dc  	ld	$2, 0x20($3)
  1abd80: 10 00 42 64  	daddiu	$2, $2, 0x10
  1abd84: c1 b9 04 0c  	jal	0x12e704 <.text+0x2e704>
  1abd88: 20 00 62 fc  	sd	$2, 0x20($3)
  1abd8c: 01 00 24 26  	addiu	$4, $17, 0x1
  1abd90: ff ff 84 30  	andi	$4, $4, 0xffff
  1abd94: c1 b9 04 0c  	jal	0x12e704 <.text+0x2e704>
  1abd98: 2d 80 40 00  	move	$16, $2
  1abd9c: e5 ff 00 10  	b	0x1abd34 <.text+0xabd34>
  1abda0: 00 12 02 00  	sll	$2, $2, 0x8
  1abda4: 36 00 02 3c  	lui	$2, 0x36
  1abda8: 68 b7 43 90  	lbu	$3, -0x4898($2)
  1abdac: 00 12 03 00  	sll	$2, $3, 0x8
  1abdb0: c5 ff 00 10  	b	0x1abcc8 <.text+0xabcc8>
  1abdb4: 25 20 43 00  	or	$4, $2, $3
  1abdb8: ca 30 04 0c  	jal	0x10c328 <.text+0xc328>
  1abdbc: ff ff 84 30  	andi	$4, $4, 0xffff
  1abdc0: 01 00 24 26  	addiu	$4, $17, 0x1
  1abdc4: ff ff 84 30  	andi	$4, $4, 0xffff
  1abdc8: ca 30 04 0c  	jal	0x10c328 <.text+0xc328>
  1abdcc: 2d 80 40 00  	move	$16, $2
  1abdd0: d8 ff 00 10  	b	0x1abd34 <.text+0xabd34>
  1abdd4: 00 12 02 00  	sll	$2, $2, 0x8
  1abdd8: 34 00 04 3c  	lui	$4, 0x34
  1abddc: 35 00 02 3c  	lui	$2, 0x35
  1abde0: 40 53 84 24  	addiu	$4, $4, 0x5340
  1abde4: c0 e2 45 8c  	lw	$5, -0x1d40($2)
  1abde8: 20 00 83 dc  	ld	$3, 0x20($4)
  1abdec: 01 00 22 26  	addiu	$2, $17, 0x1
  1abdf0: ff 7f 42 30  	andi	$2, $2, 0x7fff
  1abdf4: ff 7f 26 32  	andi	$6, $17, 0x7fff
  1abdf8: 10 00 63 64  	daddiu	$3, $3, 0x10
  1abdfc: 21 10 a2 00  	addu	$2, $5, $2
  1abe00: 20 00 83 fc  	sd	$3, 0x20($4)
  1abe04: 21 28 a6 00  	addu	$5, $5, $6
  1abe08: 00 a0 42 90  	lbu	$2, -0x6000($2)
  1abe0c: 00 a0 a3 90  	lbu	$3, -0x6000($5)
  1abe10: e7 ff 00 10  	b	0x1abdb0 <.text+0xabdb0>
  1abe14: 00 12 02 00  	sll	$2, $2, 0x8
  1abe18: 34 00 03 3c  	lui	$3, 0x34
  1abe1c: 36 00 02 3c  	lui	$2, 0x36
  1abe20: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abe24: 68 b7 45 90  	lbu	$5, -0x4898($2)
  1abe28: 20 00 62 dc  	ld	$2, 0x20($3)
  1abe2c: 00 22 05 00  	sll	$4, $5, 0x8
  1abe30: 10 00 42 64  	daddiu	$2, $2, 0x10
  1abe34: 25 20 85 00  	or	$4, $4, $5
  1abe38: a3 ff 00 10  	b	0x1abcc8 <.text+0xabcc8>
  1abe3c: 20 00 62 fc  	sd	$2, 0x20($3)
  1abe40: 01 00 85 24  	addiu	$5, $4, 0x1
  1abe44: ff 00 02 3c  	lui	$2, 0xff
  1abe48: 34 00 07 3c  	lui	$7, 0x34
  1abe4c: 24 18 a2 00  	and	$3, $5, $2
  1abe50: 40 53 e7 24  	addiu	$7, $7, 0x5340
  1abe54: 35 00 04 3c  	lui	$4, 0x35
  1abe58: b0 e2 84 24  	addiu	$4, $4, -0x1d50 <.text+0xffffffffffefe2b0>
  1abe5c: 20 00 e6 dc  	ld	$6, 0x20($7)
  1abe60: ff 7f a5 30  	andi	$5, $5, 0x7fff
  1abe64: 42 18 03 00  	srl	$3, $3, 0x1
  1abe68: 24 10 22 02  	and	$2, $17, $2
  1abe6c: 20 00 88 8c  	lw	$8, 0x20($4)
  1abe70: 25 18 65 00  	or	$3, $3, $5
  1abe74: 42 10 02 00  	srl	$2, $2, 0x1
  1abe78: 0c 00 85 8c  	lw	$5, 0xc($4)
  1abe7c: 10 00 c6 64  	daddiu	$6, $6, 0x10
  1abe80: ff 7f 24 32  	andi	$4, $17, 0x7fff
  1abe84: 20 00 e6 fc  	sd	$6, 0x20($7)
  1abe88: 25 10 44 00  	or	$2, $2, $4
  1abe8c: 24 18 68 00  	and	$3, $3, $8
  1abe90: 24 10 48 00  	and	$2, $2, $8
  1abe94: 21 18 a3 00  	addu	$3, $5, $3
  1abe98: 21 28 a2 00  	addu	$5, $5, $2
  1abe9c: 00 00 62 90  	lbu	$2, 0x0($3)
  1abea0: db ff 00 10  	b	0x1abe10 <.text+0xabe10>
  1abea4: 00 00 a3 90  	lbu	$3, 0x0($5)
  1abea8: 34 00 03 3c  	lui	$3, 0x34
  1abeac: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abeb0: 20 00 62 dc  	ld	$2, 0x20($3)
  1abeb4: 10 00 42 64  	daddiu	$2, $2, 0x10
  1abeb8: 57 09 06 0c  	jal	0x18255c <.text+0x8255c>
  1abebc: 20 00 62 fc  	sd	$2, 0x20($3)
  1abec0: 01 00 24 26  	addiu	$4, $17, 0x1
  1abec4: 57 09 06 0c  	jal	0x18255c <.text+0x8255c>
  1abec8: 2d 80 40 00  	move	$16, $2
  1abecc: 99 ff 00 10  	b	0x1abd34 <.text+0xabd34>
  1abed0: 00 12 02 00  	sll	$2, $2, 0x8
  1abed4: 34 00 03 3c  	lui	$3, 0x34
  1abed8: 00 48 04 24  	addiu	$4, $zero, 0x4800
  1abedc: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abee0: 20 00 62 dc  	ld	$2, 0x20($3)
  1abee4: 10 00 42 64  	daddiu	$2, $2, 0x10
  1abee8: fc 04 06 0c  	jal	0x1813f0 <.text+0x813f0>
  1abeec: 20 00 62 fc  	sd	$2, 0x20($3)
  1abef0: 00 48 04 24  	addiu	$4, $zero, 0x4800
  1abef4: fc 04 06 0c  	jal	0x1813f0 <.text+0x813f0>
  1abef8: 2d 80 40 00  	move	$16, $2
  1abefc: 8d ff 00 10  	b	0x1abd34 <.text+0xabd34>
  1abf00: 00 12 02 00  	sll	$2, $2, 0x8
  1abf04: 01 00 83 24  	addiu	$3, $4, 0x1
  1abf08: 0f 00 05 3c  	lui	$5, 0xf
  1abf0c: 24 10 65 00  	and	$2, $3, $5
  1abf10: 34 00 07 3c  	lui	$7, 0x34
  1abf14: 40 53 e7 24  	addiu	$7, $7, 0x5340
  1abf18: c2 10 02 00  	srl	$2, $2, 0x3
  1abf1c: 35 00 04 3c  	lui	$4, 0x35
  1abf20: 24 28 25 02  	and	$5, $17, $5
  1abf24: ff 7f 63 30  	andi	$3, $3, 0x7fff
  1abf28: b0 e2 84 24  	addiu	$4, $4, -0x1d50 <.text+0xffffffffffefe2b0>
  1abf2c: 20 00 e6 dc  	ld	$6, 0x20($7)
  1abf30: 21 18 62 00  	addu	$3, $3, $2
  1abf34: c2 28 05 00  	srl	$5, $5, 0x3
  1abf38: ff 7f 22 32  	andi	$2, $17, 0x7fff
  1abf3c: 20 00 88 8c  	lw	$8, 0x20($4)
  1abf40: 21 10 45 00  	addu	$2, $2, $5
  1abf44: 0c 00 84 8c  	lw	$4, 0xc($4)
  1abf48: 10 00 c6 64  	daddiu	$6, $6, 0x10
  1abf4c: 00 a0 63 24  	addiu	$3, $3, -0x6000 <.text+0xffffffffffefa000>
  1abf50: 00 a0 42 24  	addiu	$2, $2, -0x6000 <.text+0xffffffffffefa000>
  1abf54: 20 00 e6 fc  	sd	$6, 0x20($7)
  1abf58: 24 10 48 00  	and	$2, $2, $8
  1abf5c: 24 18 68 00  	and	$3, $3, $8
  1abf60: 21 18 83 00  	addu	$3, $4, $3
  1abf64: 21 20 82 00  	addu	$4, $4, $2
  1abf68: 00 00 62 90  	lbu	$2, 0x0($3)
  1abf6c: a8 ff 00 10  	b	0x1abe10 <.text+0xabe10>
  1abf70: 00 00 83 90  	lbu	$3, 0x0($4)
  1abf74: 34 00 03 3c  	lui	$3, 0x34
  1abf78: ff ff 84 30  	andi	$4, $4, 0xffff
  1abf7c: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abf80: 20 00 62 dc  	ld	$2, 0x20($3)
  1abf84: 10 00 42 64  	daddiu	$2, $2, 0x10
  1abf88: d7 62 05 0c  	jal	0x158b5c <.text+0x58b5c>
  1abf8c: 20 00 62 fc  	sd	$2, 0x20($3)
  1abf90: 01 00 24 26  	addiu	$4, $17, 0x1
  1abf94: ff ff 84 30  	andi	$4, $4, 0xffff
  1abf98: d7 62 05 0c  	jal	0x158b5c <.text+0x58b5c>
  1abf9c: 2d 80 40 00  	move	$16, $2
  1abfa0: 48 ff 00 10  	b	0x1abcc4 <.text+0xabcc4>
  1abfa4: 25 80 02 02  	or	$16, $16, $2
  1abfa8: 34 00 03 3c  	lui	$3, 0x34
  1abfac: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abfb0: 20 00 62 dc  	ld	$2, 0x20($3)
  1abfb4: 10 00 42 64  	daddiu	$2, $2, 0x10
  1abfb8: 12 bf 05 0c  	jal	0x16fc48 <.text+0x6fc48>
  1abfbc: 20 00 62 fc  	sd	$2, 0x20($3)
  1abfc0: 01 00 24 26  	addiu	$4, $17, 0x1
  1abfc4: 12 bf 05 0c  	jal	0x16fc48 <.text+0x6fc48>
  1abfc8: 2d 80 40 00  	move	$16, $2
  1abfcc: 59 ff 00 10  	b	0x1abd34 <.text+0xabd34>
  1abfd0: 00 12 02 00  	sll	$2, $2, 0x8
  1abfd4: 34 00 03 3c  	lui	$3, 0x34
  1abfd8: 40 53 63 24  	addiu	$3, $3, 0x5340
  1abfdc: 20 00 62 dc  	ld	$2, 0x20($3)
  1abfe0: 10 00 42 64  	daddiu	$2, $2, 0x10
  1abfe4: 7f c0 05 0c  	jal	0x1701fc <.text+0x701fc>
  1abfe8: 20 00 62 fc  	sd	$2, 0x20($3)
  1abfec: 01 00 24 26  	addiu	$4, $17, 0x1
  1abff0: 7f c0 05 0c  	jal	0x1701fc <.text+0x701fc>
  1abff4: 2d 80 40 00  	move	$16, $2
  1abff8: 32 ff 00 10  	b	0x1abcc4 <.text+0xabcc4>
  1abffc: 25 80 02 02  	or	$16, $16, $2
  1ac000: 8f ad 06 0c  	jal	0x1ab63c <.text+0xab63c>
  1ac004: 00 00 00 00  	nop
  1ac008: 01 00 24 26  	addiu	$4, $17, 0x1
  1ac00c: 2d 80 40 00  	move	$16, $2
  1ac010: 36 00 02 3c  	lui	$2, 0x36
  1ac014: 8f ad 06 0c  	jal	0x1ab63c <.text+0xab63c>
  1ac018: 68 b7 50 a0  	sb	$16, -0x4898($2)
  1ac01c: 45 ff 00 10  	b	0x1abd34 <.text+0xabd34>
  1ac020: 00 12 02 00  	sll	$2, $2, 0x8
  1ac024: 02 1b 04 00  	srl	$3, $4, 0xc
  1ac028: 35 00 02 3c  	lui	$2, 0x35
  1ac02c: ff 0f 63 30  	andi	$3, $3, 0xfff
  1ac030: b0 e2 45 24  	addiu	$5, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1ac034: 80 10 03 00  	sll	$2, $3, 0x2
  1ac038: 21 10 45 00  	addu	$2, $2, $5
  1ac03c: 28 00 46 8c  	lw	$6, 0x28($2)
  1ac040: 12 00 c2 2c  	sltiu	$2, $6, 0x12
  1ac044: 0f 00 40 14  	bnez	$2, 0x1ac084 <.text+0xac084>
  1ac048: 0c 00 c2 2c  	sltiu	$2, $6, 0xc
  1ac04c: 21 10 65 00  	addu	$2, $3, $5
  1ac050: 20 80 03 34  	ori	$3, $zero, 0x8020
  1ac054: 21 10 43 00  	addu	$2, $2, $3
  1ac058: ff ff 83 30  	andi	$3, $4, 0xffff
  1ac05c: 08 00 45 90  	lbu	$5, 0x8($2)
  1ac060: 21 18 c3 00  	addu	$3, $6, $3
  1ac064: 34 00 02 3c  	lui	$2, 0x34
  1ac068: 40 53 42 24  	addiu	$2, $2, 0x5340
  1ac06c: 78 20 05 00  	dsll	$4, $5, 0x1
  1ac070: 0c 00 43 ac  	sw	$3, 0xc($2)
  1ac074: 40 00 44 fc  	sd	$4, 0x40($2)
  1ac078: 38 00 45 fc  	sd	$5, 0x38($2)
  1ac07c: 08 00 e0 03  	jr	$ra
  1ac080: 10 00 46 ac  	sw	$6, 0x10($2)
  1ac084: 36 00 40 10  	beqz	$2, 0x1ac160 <.text+0xac160>
  1ac088: 35 00 02 3c  	lui	$2, 0x35
  1ac08c: 1b 00 03 3c  	lui	$3, 0x1b
  1ac090: 80 10 06 00  	sll	$2, $6, 0x2
  1ac094: 58 1d 63 24  	addiu	$3, $3, 0x1d58
  1ac098: 21 10 43 00  	addu	$2, $2, $3
  1ac09c: 00 00 42 8c  	lw	$2, 0x0($2)
  1ac0a0: 08 00 40 00  	jr	$2
  1ac0a4: 00 00 00 00  	nop
  1ac0a8: 35 00 02 3c  	lui	$2, 0x35
  1ac0ac: ff ff 85 30  	andi	$5, $4, 0xffff
  1ac0b0: c4 e2 44 8c  	lw	$4, -0x1d3c($2)
  1ac0b4: 34 00 03 3c  	lui	$3, 0x34
  1ac0b8: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac0bc: 00 e0 84 24  	addiu	$4, $4, -0x2000 <.text+0xffffffffffefe000>
  1ac0c0: 06 00 02 24  	addiu	$2, $zero, 0x6
  1ac0c4: 21 28 85 00  	addu	$5, $4, $5
  1ac0c8: 38 00 62 fc  	sd	$2, 0x38($3)
  1ac0cc: 0c 00 02 24  	addiu	$2, $zero, 0xc
  1ac0d0: 0c 00 65 ac  	sw	$5, 0xc($3)
  1ac0d4: 40 00 62 fc  	sd	$2, 0x40($3)
  1ac0d8: 08 00 e0 03  	jr	$ra
  1ac0dc: 10 00 64 ac  	sw	$4, 0x10($3)
  1ac0e0: 35 00 02 3c  	lui	$2, 0x35
  1ac0e4: ff ff 85 30  	andi	$5, $4, 0xffff
  1ac0e8: c4 e2 44 8c  	lw	$4, -0x1d3c($2)
  1ac0ec: 34 00 03 3c  	lui	$3, 0x34
  1ac0f0: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac0f4: f2 ff 00 10  	b	0x1ac0c0 <.text+0xac0c0>
  1ac0f8: 00 c0 84 24  	addiu	$4, $4, -0x4000 <.text+0xffffffffffefc000>
  1ac0fc: 35 00 02 3c  	lui	$2, 0x35
  1ac100: ff ff 85 30  	andi	$5, $4, 0xffff
  1ac104: c4 e2 44 8c  	lw	$4, -0x1d3c($2)
  1ac108: 34 00 03 3c  	lui	$3, 0x34
  1ac10c: 00 a0 84 24  	addiu	$4, $4, -0x6000 <.text+0xffffffffffefa000>
  1ac110: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac114: 21 28 85 00  	addu	$5, $4, $5
  1ac118: 08 00 02 24  	addiu	$2, $zero, 0x8
  1ac11c: 0c 00 65 ac  	sw	$5, 0xc($3)
  1ac120: 38 00 62 fc  	sd	$2, 0x38($3)
  1ac124: eb ff 00 10  	b	0x1ac0d4 <.text+0xac0d4>
  1ac128: 10 00 02 24  	addiu	$2, $zero, 0x10
  1ac12c: 35 00 02 3c  	lui	$2, 0x35
  1ac130: ff ff 85 30  	andi	$5, $4, 0xffff
  1ac134: f4 ff 00 10  	b	0x1ac108 <.text+0xac108>
  1ac138: bc e2 44 8c  	lw	$4, -0x1d44($2)
  1ac13c: 35 00 02 3c  	lui	$2, 0x35
  1ac140: ff ff 85 30  	andi	$5, $4, 0xffff
  1ac144: f0 ff 00 10  	b	0x1ac108 <.text+0xac108>
  1ac148: c8 e2 44 8c  	lw	$4, -0x1d38($2)
  1ac14c: 35 00 02 3c  	lui	$2, 0x35
  1ac150: ff ff 85 30  	andi	$5, $4, 0xffff
  1ac154: ec ff 00 10  	b	0x1ac108 <.text+0xac108>
  1ac158: c0 e2 44 8c  	lw	$4, -0x1d40($2)
  1ac15c: 35 00 02 3c  	lui	$2, 0x35
  1ac160: 34 00 03 3c  	lui	$3, 0x34
  1ac164: bc e2 45 8c  	lw	$5, -0x1d44($2)
  1ac168: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac16c: 08 00 02 24  	addiu	$2, $zero, 0x8
  1ac170: ff ff 84 30  	andi	$4, $4, 0xffff
  1ac174: 21 20 a4 00  	addu	$4, $5, $4
  1ac178: 38 00 62 fc  	sd	$2, 0x38($3)
  1ac17c: 10 00 02 24  	addiu	$2, $zero, 0x10
  1ac180: 0c 00 64 ac  	sw	$4, 0xc($3)
  1ac184: 40 00 62 fc  	sd	$2, 0x40($3)
  1ac188: 08 00 e0 03  	jr	$ra
  1ac18c: 10 00 65 ac  	sw	$5, 0x10($3)
  1ac190: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1ac194: ff 0f a3 30  	andi	$3, $5, 0xfff
  1ac198: ff 0f 02 24  	addiu	$2, $zero, 0xfff
  1ac19c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1ac1a0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1ac1a4: ff ff 91 30  	andi	$17, $4, 0xffff
  1ac1a8: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1ac1ac: 0e 01 62 10  	beq	$3, $2, 0x1ac5e8 <.text+0xac5e8>
  1ac1b0: 2d 80 a0 00  	move	$16, $5
  1ac1b4: 02 13 05 00  	srl	$2, $5, 0xc
  1ac1b8: 34 00 03 3c  	lui	$3, 0x34
  1ac1bc: ff 0f 44 30  	andi	$4, $2, 0xfff
  1ac1c0: 40 53 67 24  	addiu	$7, $3, 0x5340
  1ac1c4: 35 00 02 3c  	lui	$2, 0x35
  1ac1c8: 18 00 e0 ac  	sw	$zero, 0x18($7)
  1ac1cc: b0 e2 45 24  	addiu	$5, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1ac1d0: 80 10 04 00  	sll	$2, $4, 0x2
  1ac1d4: 21 10 45 00  	addu	$2, $2, $5
  1ac1d8: 28 40 48 8c  	lw	$8, 0x4028($2)
  1ac1dc: 12 00 02 2d  	sltiu	$2, $8, 0x12
  1ac1e0: 1f 00 40 14  	bnez	$2, 0x1ac260 <.text+0xac260>
  1ac1e4: 1b 00 03 3c  	lui	$3, 0x1b
  1ac1e8: 21 10 85 00  	addu	$2, $4, $5
  1ac1ec: 20 80 03 34  	ori	$3, $zero, 0x8020
  1ac1f0: 21 10 43 00  	addu	$2, $2, $3
  1ac1f4: 34 00 09 3c  	lui	$9, 0x34
  1ac1f8: 08 00 42 90  	lbu	$2, 0x8($2)
  1ac1fc: f8 5a 26 25  	addiu	$6, $9, 0x5af8
  1ac200: 20 00 e3 dc  	ld	$3, 0x20($7)
  1ac204: ff ff 04 32  	andi	$4, $16, 0xffff
  1ac208: 78 10 02 00  	dsll	$2, $2, 0x1
  1ac20c: 38 00 c5 8c  	lw	$5, 0x38($6)
  1ac210: 2d 18 62 00  	daddu	$3, $3, $2
  1ac214: 21 40 04 01  	addu	$8, $8, $4
  1ac218: 0c 00 05 11  	beq	$8, $5, 0x1ac24c <.text+0xac24c>
  1ac21c: 20 00 e3 fc  	sd	$3, 0x20($7)
  1ac220: 3c 00 c2 8c  	lw	$2, 0x3c($6)
  1ac224: 0a 00 02 11  	beq	$8, $2, 0x1ac250 <.text+0xac250>
  1ac228: f8 5a 22 8d  	lw	$2, 0x5af8($9)
  1ac22c: 02 12 11 00  	srl	$2, $17, 0x8
  1ac230: 00 00 11 a1  	sb	$17, 0x0($8)
  1ac234: 01 00 02 a1  	sb	$2, 0x1($8)
  1ac238: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac23c: 10 00 b1 df  	ld	$17, 0x10($sp)
  1ac240: 00 00 b0 df  	ld	$16, 0x0($sp)
  1ac244: 08 00 e0 03  	jr	$ra
  1ac248: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1ac24c: f8 5a 22 8d  	lw	$2, 0x5af8($9)
  1ac250: 34 00 c0 ac  	sw	$zero, 0x34($6)
  1ac254: 2b 10 02 00  	sltu	$2, $zero, $2
  1ac258: f4 ff 00 10  	b	0x1ac22c <.text+0xac22c>
  1ac25c: 18 00 c2 a0  	sb	$2, 0x18($6)
  1ac260: 80 10 08 00  	sll	$2, $8, 0x2
  1ac264: a8 1e 63 24  	addiu	$3, $3, 0x1ea8
  1ac268: 21 10 43 00  	addu	$2, $2, $3
  1ac26c: 00 00 42 8c  	lw	$2, 0x0($2)
  1ac270: 08 00 40 00  	jr	$2
  1ac274: 00 00 00 00  	nop
  1ac278: 34 00 02 3c  	lui	$2, 0x34
  1ac27c: 40 53 43 24  	addiu	$3, $2, 0x5340
  1ac280: 08 00 62 90  	lbu	$2, 0x8($3)
  1ac284: 04 00 40 14  	bnez	$2, 0x1ac298 <.text+0xac298>
  1ac288: ff 00 24 32  	andi	$4, $17, 0xff
  1ac28c: 20 00 62 dc  	ld	$2, 0x20($3)
  1ac290: 0c 00 42 64  	daddiu	$2, $2, 0xc
  1ac294: 20 00 62 fc  	sd	$2, 0x20($3)
  1ac298: 9a 64 05 0c  	jal	0x159268 <.text+0x59268>
  1ac29c: ff ff 05 32  	andi	$5, $16, 0xffff
  1ac2a0: 02 22 11 00  	srl	$4, $17, 0x8
  1ac2a4: 01 00 05 26  	addiu	$5, $16, 0x1
  1ac2a8: 9a 64 05 0c  	jal	0x159268 <.text+0x59268>
  1ac2ac: ff ff a5 30  	andi	$5, $5, 0xffff
  1ac2b0: e2 ff 00 10  	b	0x1ac23c <.text+0xac23c>
  1ac2b4: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac2b8: 34 00 03 3c  	lui	$3, 0x34
  1ac2bc: ff 00 24 32  	andi	$4, $17, 0xff
  1ac2c0: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac2c4: ff ff 05 32  	andi	$5, $16, 0xffff
  1ac2c8: 20 00 62 dc  	ld	$2, 0x20($3)
  1ac2cc: 0c 00 42 64  	daddiu	$2, $2, 0xc
  1ac2d0: 5b 6b 05 0c  	jal	0x15ad6c <.text+0x5ad6c>
  1ac2d4: 20 00 62 fc  	sd	$2, 0x20($3)
  1ac2d8: 02 22 11 00  	srl	$4, $17, 0x8
  1ac2dc: 01 00 05 26  	addiu	$5, $16, 0x1
  1ac2e0: 5b 6b 05 0c  	jal	0x15ad6c <.text+0x5ad6c>
  1ac2e4: ff ff a5 30  	andi	$5, $5, 0xffff
  1ac2e8: d4 ff 00 10  	b	0x1ac23c <.text+0xac23c>
  1ac2ec: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac2f0: 34 00 03 3c  	lui	$3, 0x34
  1ac2f4: ff 00 24 32  	andi	$4, $17, 0xff
  1ac2f8: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac2fc: ff ff 05 32  	andi	$5, $16, 0xffff
  1ac300: 20 00 62 dc  	ld	$2, 0x20($3)
  1ac304: 10 00 42 64  	daddiu	$2, $2, 0x10
  1ac308: ca b9 04 0c  	jal	0x12e728 <.text+0x2e728>
  1ac30c: 20 00 62 fc  	sd	$2, 0x20($3)
  1ac310: 02 22 11 00  	srl	$4, $17, 0x8
  1ac314: 01 00 05 26  	addiu	$5, $16, 0x1
  1ac318: ca b9 04 0c  	jal	0x12e728 <.text+0x2e728>
  1ac31c: ff ff a5 30  	andi	$5, $5, 0xffff
  1ac320: c6 ff 00 10  	b	0x1ac23c <.text+0xac23c>
  1ac324: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac328: 34 00 02 3c  	lui	$2, 0x34
  1ac32c: 40 53 49 24  	addiu	$9, $2, 0x5340
  1ac330: 35 00 02 3c  	lui	$2, 0x35
  1ac334: b0 e2 48 24  	addiu	$8, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1ac338: 20 00 22 dd  	ld	$2, 0x20($9)
  1ac33c: 20 00 07 8d  	lw	$7, 0x20($8)
  1ac340: 10 00 42 64  	daddiu	$2, $2, 0x10
  1ac344: bc ff e0 10  	beqz	$7, 0x1ac238 <.text+0xac238>
  1ac348: 20 00 22 fd  	sd	$2, 0x20($9)
  1ac34c: ff 00 03 3c  	lui	$3, 0xff
  1ac350: 01 00 04 26  	addiu	$4, $16, 0x1
  1ac354: 24 10 03 02  	and	$2, $16, $3
  1ac358: ff 7f 06 32  	andi	$6, $16, 0x7fff
  1ac35c: 42 10 02 00  	srl	$2, $2, 0x1
  1ac360: 0c 00 05 8d  	lw	$5, 0xc($8)
  1ac364: 24 18 83 00  	and	$3, $4, $3
  1ac368: 25 10 46 00  	or	$2, $2, $6
  1ac36c: 24 10 47 00  	and	$2, $2, $7
  1ac370: 42 18 03 00  	srl	$3, $3, 0x1
  1ac374: ff 7f 84 30  	andi	$4, $4, 0x7fff
  1ac378: 21 28 a2 00  	addu	$5, $5, $2
  1ac37c: 25 18 64 00  	or	$3, $3, $4
  1ac380: 00 00 b1 a0  	sb	$17, 0x0($5)
  1ac384: 20 00 04 8d  	lw	$4, 0x20($8)
  1ac388: 02 2a 11 00  	srl	$5, $17, 0x8
  1ac38c: 0c 00 02 8d  	lw	$2, 0xc($8)
  1ac390: 24 18 64 00  	and	$3, $3, $4
  1ac394: 21 10 43 00  	addu	$2, $2, $3
  1ac398: 00 00 45 a0  	sb	$5, 0x0($2)
  1ac39c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ac3a0: a5 ff 00 10  	b	0x1ac238 <.text+0xac238>
  1ac3a4: 54 00 22 a1  	sb	$2, 0x54($9)
  1ac3a8: 34 00 02 3c  	lui	$2, 0x34
  1ac3ac: 40 53 49 24  	addiu	$9, $2, 0x5340
  1ac3b0: 35 00 02 3c  	lui	$2, 0x35
  1ac3b4: b0 e2 48 24  	addiu	$8, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1ac3b8: 20 00 22 dd  	ld	$2, 0x20($9)
  1ac3bc: 20 00 07 8d  	lw	$7, 0x20($8)
  1ac3c0: 10 00 42 64  	daddiu	$2, $2, 0x10
  1ac3c4: 9c ff e0 10  	beqz	$7, 0x1ac238 <.text+0xac238>
  1ac3c8: 20 00 22 fd  	sd	$2, 0x20($9)
  1ac3cc: 0f 00 05 3c  	lui	$5, 0xf
  1ac3d0: ff 7f 02 32  	andi	$2, $16, 0x7fff
  1ac3d4: 24 20 05 02  	and	$4, $16, $5
  1ac3d8: 0c 00 06 8d  	lw	$6, 0xc($8)
  1ac3dc: c2 20 04 00  	srl	$4, $4, 0x3
  1ac3e0: 01 00 03 26  	addiu	$3, $16, 0x1
  1ac3e4: 21 10 44 00  	addu	$2, $2, $4
  1ac3e8: 24 28 65 00  	and	$5, $3, $5
  1ac3ec: 00 a0 42 24  	addiu	$2, $2, -0x6000 <.text+0xffffffffffefa000>
  1ac3f0: c2 28 05 00  	srl	$5, $5, 0x3
  1ac3f4: 24 10 47 00  	and	$2, $2, $7
  1ac3f8: ff 7f 63 30  	andi	$3, $3, 0x7fff
  1ac3fc: 21 30 c2 00  	addu	$6, $6, $2
  1ac400: 21 18 65 00  	addu	$3, $3, $5
  1ac404: 00 00 d1 a0  	sb	$17, 0x0($6)
  1ac408: de ff 00 10  	b	0x1ac384 <.text+0xac384>
  1ac40c: 00 a0 63 24  	addiu	$3, $3, -0x6000 <.text+0xffffffffffefa000>
  1ac410: 34 00 03 3c  	lui	$3, 0x34
  1ac414: ff 00 24 32  	andi	$4, $17, 0xff
  1ac418: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac41c: ff ff 05 32  	andi	$5, $16, 0xffff
  1ac420: 20 00 62 dc  	ld	$2, 0x20($3)
  1ac424: 10 00 42 64  	daddiu	$2, $2, 0x10
  1ac428: f7 35 04 0c  	jal	0x10d7dc <.text+0xd7dc>
  1ac42c: 20 00 62 fc  	sd	$2, 0x20($3)
  1ac430: 02 22 11 00  	srl	$4, $17, 0x8
  1ac434: 01 00 05 26  	addiu	$5, $16, 0x1
  1ac438: f7 35 04 0c  	jal	0x10d7dc <.text+0xd7dc>
  1ac43c: ff ff a5 30  	andi	$5, $5, 0xffff
  1ac440: 7e ff 00 10  	b	0x1ac23c <.text+0xac23c>
  1ac444: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac448: 34 00 06 3c  	lui	$6, 0x34
  1ac44c: 35 00 05 3c  	lui	$5, 0x35
  1ac450: 40 53 c6 24  	addiu	$6, $6, 0x5340
  1ac454: b0 e2 a5 24  	addiu	$5, $5, -0x1d50 <.text+0xffffffffffefe2b0>
  1ac458: 20 00 c3 dc  	ld	$3, 0x20($6)
  1ac45c: ff 7f 07 32  	andi	$7, $16, 0x7fff
  1ac460: 10 00 a2 8c  	lw	$2, 0x10($5)
  1ac464: 01 00 04 26  	addiu	$4, $16, 0x1
  1ac468: 10 00 63 64  	daddiu	$3, $3, 0x10
  1ac46c: ff 7f 84 30  	andi	$4, $4, 0x7fff
  1ac470: 21 10 47 00  	addu	$2, $2, $7
  1ac474: 20 00 c3 fc  	sd	$3, 0x20($6)
  1ac478: 00 a0 51 a0  	sb	$17, -0x6000($2)
  1ac47c: 02 1a 11 00  	srl	$3, $17, 0x8
  1ac480: 10 00 a2 8c  	lw	$2, 0x10($5)
  1ac484: 21 10 44 00  	addu	$2, $2, $4
  1ac488: 00 a0 43 a0  	sb	$3, -0x6000($2)
  1ac48c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ac490: 69 ff 00 10  	b	0x1ac238 <.text+0xac238>
  1ac494: 54 00 c2 a0  	sb	$2, 0x54($6)
  1ac498: 34 00 04 3c  	lui	$4, 0x34
  1ac49c: 35 00 05 3c  	lui	$5, 0x35
  1ac4a0: 40 53 84 24  	addiu	$4, $4, 0x5340
  1ac4a4: b0 e2 a5 24  	addiu	$5, $5, -0x1d50 <.text+0xffffffffffefe2b0>
  1ac4a8: 20 00 83 dc  	ld	$3, 0x20($4)
  1ac4ac: ff ff 07 32  	andi	$7, $16, 0xffff
  1ac4b0: 0c 00 a2 8c  	lw	$2, 0xc($5)
  1ac4b4: 01 00 06 26  	addiu	$6, $16, 0x1
  1ac4b8: 08 00 63 64  	daddiu	$3, $3, 0x8
  1ac4bc: ff ff c6 30  	andi	$6, $6, 0xffff
  1ac4c0: 21 10 47 00  	addu	$2, $2, $7
  1ac4c4: 20 00 83 fc  	sd	$3, 0x20($4)
  1ac4c8: 00 00 51 a0  	sb	$17, 0x0($2)
  1ac4cc: 02 22 11 00  	srl	$4, $17, 0x8
  1ac4d0: 34 00 03 3c  	lui	$3, 0x34
  1ac4d4: 0c 00 a2 8c  	lw	$2, 0xc($5)
  1ac4d8: f8 5a 63 24  	addiu	$3, $3, 0x5af8
  1ac4dc: 21 10 46 00  	addu	$2, $2, $6
  1ac4e0: 00 00 44 a0  	sb	$4, 0x0($2)
  1ac4e4: 1c 00 62 90  	lbu	$2, 0x1c($3)
  1ac4e8: 01 00 42 2c  	sltiu	$2, $2, 0x1
  1ac4ec: 52 ff 00 10  	b	0x1ac238 <.text+0xac238>
  1ac4f0: 18 00 62 a0  	sb	$2, 0x18($3)
  1ac4f4: 34 00 04 3c  	lui	$4, 0x34
  1ac4f8: 01 00 06 26  	addiu	$6, $16, 0x1
  1ac4fc: 40 53 84 24  	addiu	$4, $4, 0x5340
  1ac500: 41 00 02 3c  	lui	$2, 0x41
  1ac504: 20 00 85 dc  	ld	$5, 0x20($4)
  1ac508: 08 35 42 24  	addiu	$2, $2, 0x3508
  1ac50c: ff ff c6 30  	andi	$6, $6, 0xffff
  1ac510: ff ff 03 32  	andi	$3, $16, 0xffff
  1ac514: 21 30 c2 00  	addu	$6, $6, $2
  1ac518: 10 00 a5 64  	daddiu	$5, $5, 0x10
  1ac51c: 21 18 62 00  	addu	$3, $3, $2
  1ac520: 20 00 85 fc  	sd	$5, 0x20($4)
  1ac524: 3c 00 71 a0  	sb	$17, 0x3c($3)
  1ac528: 43 ff 00 10  	b	0x1ac238 <.text+0xac238>
  1ac52c: 3c 00 d1 a0  	sb	$17, 0x3c($6)
  1ac530: 34 00 03 3c  	lui	$3, 0x34
  1ac534: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac538: 20 00 62 dc  	ld	$2, 0x20($3)
  1ac53c: 10 00 42 64  	daddiu	$2, $2, 0x10
  1ac540: 3d ff 00 10  	b	0x1ac238 <.text+0xac238>
  1ac544: 20 00 62 fc  	sd	$2, 0x20($3)
  1ac548: 34 00 03 3c  	lui	$3, 0x34
  1ac54c: ff 00 24 32  	andi	$4, $17, 0xff
  1ac550: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac554: ff ff 05 32  	andi	$5, $16, 0xffff
  1ac558: 20 00 62 dc  	ld	$2, 0x20($3)
  1ac55c: 10 00 42 64  	daddiu	$2, $2, 0x10
  1ac560: dd 62 05 0c  	jal	0x158b74 <.text+0x58b74>
  1ac564: 20 00 62 fc  	sd	$2, 0x20($3)
  1ac568: 02 22 11 00  	srl	$4, $17, 0x8
  1ac56c: 01 00 05 26  	addiu	$5, $16, 0x1
  1ac570: dd 62 05 0c  	jal	0x158b74 <.text+0x58b74>
  1ac574: ff ff a5 30  	andi	$5, $5, 0xffff
  1ac578: 30 ff 00 10  	b	0x1ac23c <.text+0xac23c>
  1ac57c: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac580: 34 00 03 3c  	lui	$3, 0x34
  1ac584: ff 00 25 32  	andi	$5, $17, 0xff
  1ac588: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac58c: 2d 20 00 02  	move	$4, $16
  1ac590: 20 00 62 dc  	ld	$2, 0x20($3)
  1ac594: 10 00 42 64  	daddiu	$2, $2, 0x10
  1ac598: 1b bf 05 0c  	jal	0x16fc6c <.text+0x6fc6c>
  1ac59c: 20 00 62 fc  	sd	$2, 0x20($3)
  1ac5a0: 02 2a 11 00  	srl	$5, $17, 0x8
  1ac5a4: 1b bf 05 0c  	jal	0x16fc6c <.text+0x6fc6c>
  1ac5a8: 01 00 04 26  	addiu	$4, $16, 0x1
  1ac5ac: 23 ff 00 10  	b	0x1ac23c <.text+0xac23c>
  1ac5b0: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac5b4: 34 00 03 3c  	lui	$3, 0x34
  1ac5b8: ff 00 25 32  	andi	$5, $17, 0xff
  1ac5bc: 40 53 63 24  	addiu	$3, $3, 0x5340
  1ac5c0: 2d 20 00 02  	move	$4, $16
  1ac5c4: 20 00 62 dc  	ld	$2, 0x20($3)
  1ac5c8: 10 00 42 64  	daddiu	$2, $2, 0x10
  1ac5cc: 81 c0 05 0c  	jal	0x170204 <.text+0x70204>
  1ac5d0: 20 00 62 fc  	sd	$2, 0x20($3)
  1ac5d4: 02 2a 11 00  	srl	$5, $17, 0x8
  1ac5d8: 81 c0 05 0c  	jal	0x170204 <.text+0x70204>
  1ac5dc: 01 00 04 26  	addiu	$4, $16, 0x1
  1ac5e0: 16 ff 00 10  	b	0x1ac23c <.text+0xac23c>
  1ac5e4: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac5e8: 40 ae 06 0c  	jal	0x1ab900 <.text+0xab900>
  1ac5ec: ff 00 24 32  	andi	$4, $17, 0xff
  1ac5f0: 02 22 11 00  	srl	$4, $17, 0x8
  1ac5f4: 40 ae 06 0c  	jal	0x1ab900 <.text+0xab900>
  1ac5f8: 01 00 05 26  	addiu	$5, $16, 0x1
  1ac5fc: 0f ff 00 10  	b	0x1ac23c <.text+0xac23c>
  1ac600: 20 00 bf df  	ld	$ra, 0x20($sp)
  1ac604: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1ac608: 34 00 02 3c  	lui	$2, 0x34
  1ac60c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1ac610: e0 54 45 24  	addiu	$5, $2, 0x54e0
  1ac614: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1ac618: 01 00 a2 90  	lbu	$2, 0x1($5)
  1ac61c: 08 00 40 10  	beqz	$2, 0x1ac640 <.text+0xac640>
  1ac620: 10 00 bf df  	ld	$ra, 0x10($sp)
  1ac624: 34 00 04 3c  	lui	$4, 0x34
  1ac628: 40 53 90 24  	addiu	$16, $4, 0x5340
  1ac62c: 0c 00 03 8e  	lw	$3, 0xc($16)
  1ac630: 18 00 02 8e  	lw	$2, 0x18($16)
  1ac634: 05 00 62 50  	beql	$3, $2, 0x1ac64c <.text+0xac64c>
  1ac638: 1c 00 02 8e  	lw	$2, 0x1c($16)
  1ac63c: 10 00 bf df  	ld	$ra, 0x10($sp)
  1ac640: 00 00 b0 df  	ld	$16, 0x0($sp)
  1ac644: 08 00 e0 03  	jr	$ra
  1ac648: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1ac64c: 31 00 40 14  	bnez	$2, 0x1ac714 <.text+0xac714>
  1ac650: 34 00 02 3c  	lui	$2, 0x34
  1ac654: 40 53 82 8c  	lw	$2, 0x5340($4)
  1ac658: 80 08 42 30  	andi	$2, $2, 0x880
  1ac65c: 2d 00 40 14  	bnez	$2, 0x1ac714 <.text+0xac714>
  1ac660: 34 00 02 3c  	lui	$2, 0x34
  1ac664: 62 00 a2 90  	lbu	$2, 0x62($5)
  1ac668: 26 00 40 14  	bnez	$2, 0x1ac704 <.text+0xac704>
  1ac66c: 18 00 00 ae  	sw	$zero, 0x18($16)
  1ac670: 34 00 03 3c  	lui	$3, 0x34
  1ac674: 28 00 02 de  	ld	$2, 0x28($16)
  1ac678: a4 54 63 90  	lbu	$3, 0x54a4($3)
  1ac67c: ef ff 60 10  	beqz	$3, 0x1ac63c <.text+0xac63c>
  1ac680: 20 00 02 fe  	sd	$2, 0x20($16)
  1ac684: 34 00 02 3c  	lui	$2, 0x34
  1ac688: 24 53 40 a0  	sb	$zero, 0x5324($2)
  1ac68c: 34 00 02 3c  	lui	$2, 0x34
  1ac690: 34 00 10 3c  	lui	$16, 0x34
  1ac694: 98 54 45 8c  	lw	$5, 0x5498($2)
  1ac698: 3f 00 02 3c  	lui	$2, 0x3f
  1ac69c: a8 44 42 24  	addiu	$2, $2, 0x44a8
  1ac6a0: b8 53 04 8e  	lw	$4, 0x53b8($16)
  1ac6a4: 00 00 a3 90  	lbu	$3, 0x0($5)
  1ac6a8: 80 18 03 00  	sll	$3, $3, 0x2
  1ac6ac: 21 18 62 00  	addu	$3, $3, $2
  1ac6b0: 00 00 62 8c  	lw	$2, 0x0($3)
  1ac6b4: 21 20 82 00  	addu	$4, $4, $2
  1ac6b8: 41 00 02 3c  	lui	$2, 0x41
  1ac6bc: b8 53 04 ae  	sw	$4, 0x53b8($16)
  1ac6c0: 10 10 42 24  	addiu	$2, $2, 0x1010
  1ac6c4: 00 00 a3 90  	lbu	$3, 0x0($5)
  1ac6c8: 80 18 03 00  	sll	$3, $3, 0x2
  1ac6cc: 21 18 62 00  	addu	$3, $3, $2
  1ac6d0: 00 00 62 8c  	lw	$2, 0x0($3)
  1ac6d4: 09 f8 40 00  	jalr	$2
  1ac6d8: 00 00 00 00  	nop
  1ac6dc: 34 00 03 3c  	lui	$3, 0x34
  1ac6e0: b8 53 02 8e  	lw	$2, 0x53b8($16)
  1ac6e4: 68 53 63 dc  	ld	$3, 0x5368($3)
  1ac6e8: 2a 10 43 00  	slt	$2, $2, $3
  1ac6ec: e9 ff 40 14  	bnez	$2, 0x1ac694 <.text+0xac694>
  1ac6f0: 34 00 02 3c  	lui	$2, 0x34
  1ac6f4: 01 00 03 24  	addiu	$3, $zero, 0x1
  1ac6f8: 34 00 02 3c  	lui	$2, 0x34
  1ac6fc: cf ff 00 10  	b	0x1ac63c <.text+0xac63c>
  1ac700: 24 53 43 a0  	sb	$3, 0x5324($2)
  1ac704: 64 78 05 0c  	jal	0x15e190 <.text+0x5e190>
  1ac708: 00 00 00 00  	nop
  1ac70c: d9 ff 00 10  	b	0x1ac674 <.text+0xac674>
  1ac710: 34 00 03 3c  	lui	$3, 0x34
  1ac714: 40 53 44 24  	addiu	$4, $2, 0x5340
  1ac718: 1c 00 83 8c  	lw	$3, 0x1c($4)
  1ac71c: 02 00 62 2c  	sltiu	$2, $3, 0x2
  1ac720: 02 00 40 14  	bnez	$2, 0x1ac72c <.text+0xac72c>
  1ac724: ff ff 62 24  	addiu	$2, $3, -0x1 <.text+0xffffffffffefffff>
  1ac728: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ac72c: c3 ff 00 10  	b	0x1ac63c <.text+0xac63c>
  1ac730: 1c 00 82 ac  	sw	$2, 0x1c($4)
  1ac734: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1ac738: 82 12 04 00  	srl	$2, $4, 0xa
  1ac73c: 35 00 03 3c  	lui	$3, 0x35
  1ac740: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1ac744: b0 e2 63 24  	addiu	$3, $3, -0x1d50 <.text+0xffffffffffefe2b0>
  1ac748: fc 3f 42 30  	andi	$2, $2, 0x3ffc
  1ac74c: 21 10 43 00  	addu	$2, $2, $3
  1ac750: 28 00 45 8c  	lw	$5, 0x28($2)
  1ac754: 12 00 a2 2c  	sltiu	$2, $5, 0x12
  1ac758: 16 00 40 10  	beqz	$2, 0x1ac7b4 <.text+0xac7b4>
  1ac75c: 2d 18 a0 00  	move	$3, $5
  1ac760: 34 00 02 3c  	lui	$2, 0x34
  1ac764: 45 55 42 90  	lbu	$2, 0x5545($2)
  1ac768: 07 00 40 10  	beqz	$2, 0x1ac788 <.text+0xac788>
  1ac76c: 11 00 a2 2c  	sltiu	$2, $5, 0x11
  1ac770: 7f 00 02 3c  	lui	$2, 0x7f
  1ac774: 00 48 03 24  	addiu	$3, $zero, 0x4800
  1ac778: ff ff 42 34  	ori	$2, $2, 0xffff
  1ac77c: 24 10 82 00  	and	$2, $4, $2
  1ac780: 0a 00 43 10  	beq	$2, $3, 0x1ac7ac <.text+0xac7ac>
  1ac784: 11 00 a2 2c  	sltiu	$2, $5, 0x11
  1ac788: 0a 00 40 10  	beqz	$2, 0x1ac7b4 <.text+0xac7b4>
  1ac78c: 2d 18 00 00  	move	$3, $zero
  1ac790: 1b 00 03 3c  	lui	$3, 0x1b
  1ac794: 80 10 05 00  	sll	$2, $5, 0x2
  1ac798: 30 20 63 24  	addiu	$3, $3, 0x2030
  1ac79c: 21 10 43 00  	addu	$2, $2, $3
  1ac7a0: 00 00 42 8c  	lw	$2, 0x0($2)
  1ac7a4: 08 00 40 00  	jr	$2
  1ac7a8: 00 00 00 00  	nop
  1ac7ac: 41 00 02 3c  	lui	$2, 0x41
  1ac7b0: 44 35 43 24  	addiu	$3, $2, 0x3544
  1ac7b4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1ac7b8: 2d 10 60 00  	move	$2, $3
  1ac7bc: 08 00 e0 03  	jr	$ra
  1ac7c0: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1ac7c4: 35 00 02 3c  	lui	$2, 0x35
  1ac7c8: fa ff 00 10  	b	0x1ac7b4 <.text+0xac7b4>
  1ac7cc: c4 e2 43 8c  	lw	$3, -0x1d3c($2)
  1ac7d0: 35 00 02 3c  	lui	$2, 0x35
  1ac7d4: c4 e2 42 8c  	lw	$2, -0x1d3c($2)
  1ac7d8: f6 ff 00 10  	b	0x1ac7b4 <.text+0xac7b4>
  1ac7dc: 00 a0 43 24  	addiu	$3, $2, -0x6000 <.text+0xffffffffffefa000>
  1ac7e0: 35 00 02 3c  	lui	$2, 0x35
  1ac7e4: fc ff 00 10  	b	0x1ac7d8 <.text+0xac7d8>
  1ac7e8: bc e2 42 8c  	lw	$2, -0x1d44($2)
  1ac7ec: 35 00 02 3c  	lui	$2, 0x35
  1ac7f0: f9 ff 00 10  	b	0x1ac7d8 <.text+0xac7d8>
  1ac7f4: c8 e2 42 8c  	lw	$2, -0x1d38($2)
  1ac7f8: 35 00 02 3c  	lui	$2, 0x35
  1ac7fc: f6 ff 00 10  	b	0x1ac7d8 <.text+0xac7d8>
  1ac800: c0 e2 42 8c  	lw	$2, -0x1d40($2)
  1ac804: 44 0a 06 0c  	jal	0x182910 <.text+0x82910>
  1ac808: 00 00 00 00  	nop
  1ac80c: e9 ff 00 10  	b	0x1ac7b4 <.text+0xac7b4>
  1ac810: 2d 18 40 00  	move	$3, $2
  1ac814: e7 ff 00 10  	b	0x1ac7b4 <.text+0xac7b4>
  1ac818: 2d 18 00 00  	move	$3, $zero
  1ac81c: f4 63 05 0c  	jal	0x158fd0 <.text+0x58fd0>
  1ac820: 00 00 00 00  	nop
  1ac824: e3 ff 00 10  	b	0x1ac7b4 <.text+0xac7b4>
  1ac828: 2d 18 40 00  	move	$3, $2
  1ac82c: 35 00 02 3c  	lui	$2, 0x35
  1ac830: e0 ff 00 10  	b	0x1ac7b4 <.text+0xac7b4>
  1ac834: bc e2 43 8c  	lw	$3, -0x1d44($2)
  1ac838: ff 00 84 30  	andi	$4, $4, 0xff
  1ac83c: 0e 00 80 10  	beqz	$4, 0x1ac878 <.text+0xac878>
  1ac840: 36 00 02 3c  	lui	$2, 0x36
  1ac844: 18 00 02 3c  	lui	$2, 0x18
  1ac848: 36 00 03 3c  	lui	$3, 0x36
  1ac84c: 8c 5d 42 24  	addiu	$2, $2, 0x5d8c
  1ac850: 8c f9 62 ac  	sw	$2, -0x674($3)
  1ac854: 36 00 03 3c  	lui	$3, 0x36
  1ac858: 18 00 02 3c  	lui	$2, 0x18
  1ac85c: a8 60 42 24  	addiu	$2, $2, 0x60a8
  1ac860: 90 f9 62 ac  	sw	$2, -0x670($3)
  1ac864: 18 00 02 3c  	lui	$2, 0x18
  1ac868: a8 74 42 24  	addiu	$2, $2, 0x74a8
  1ac86c: 36 00 03 3c  	lui	$3, 0x36
  1ac870: 08 00 e0 03  	jr	$ra
  1ac874: 9c f9 62 ac  	sw	$2, -0x664($3)
  1ac878: 80 d4 44 24  	addiu	$4, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ac87c: bf 0a 83 90  	lbu	$3, 0xabf($4)
  1ac880: 80 00 62 30  	andi	$2, $3, 0x80
  1ac884: 22 00 40 10  	beqz	$2, 0x1ac910 <.text+0xac910>
  1ac888: 40 00 62 30  	andi	$2, $3, 0x40
  1ac88c: 16 00 40 10  	beqz	$2, 0x1ac8e8 <.text+0xac8e8>
  1ac890: 19 00 02 3c  	lui	$2, 0x19
  1ac894: be 0a 82 90  	lbu	$2, 0xabe($4)
  1ac898: 02 00 42 30  	andi	$2, $2, 0x2
  1ac89c: 0c 00 40 50  	beqzl	$2, 0x1ac8d0 <.text+0xac8d0>
  1ac8a0: 19 00 02 3c  	lui	$2, 0x19
  1ac8a4: 19 00 02 3c  	lui	$2, 0x19
  1ac8a8: 36 00 03 3c  	lui	$3, 0x36
  1ac8ac: b8 8f 42 24  	addiu	$2, $2, -0x7048 <.text+0xffffffffffef8fb8>
  1ac8b0: 8c f9 62 ac  	sw	$2, -0x674($3)
  1ac8b4: 19 00 02 3c  	lui	$2, 0x19
  1ac8b8: d4 92 42 24  	addiu	$2, $2, -0x6d2c <.text+0xffffffffffef92d4>
  1ac8bc: 36 00 03 3c  	lui	$3, 0x36
  1ac8c0: 90 f9 62 ac  	sw	$2, -0x670($3)
  1ac8c4: 19 00 02 3c  	lui	$2, 0x19
  1ac8c8: e8 ff 00 10  	b	0x1ac86c <.text+0xac86c>
  1ac8cc: c0 ba 42 24  	addiu	$2, $2, -0x4540 <.text+0xffffffffffefbac0>
  1ac8d0: 36 00 03 3c  	lui	$3, 0x36
  1ac8d4: 20 9f 42 24  	addiu	$2, $2, -0x60e0 <.text+0xffffffffffef9f20>
  1ac8d8: 8c f9 62 ac  	sw	$2, -0x674($3)
  1ac8dc: 19 00 02 3c  	lui	$2, 0x19
  1ac8e0: f6 ff 00 10  	b	0x1ac8bc <.text+0xac8bc>
  1ac8e4: 3c a2 42 24  	addiu	$2, $2, -0x5dc4 <.text+0xffffffffffefa23c>
  1ac8e8: 36 00 03 3c  	lui	$3, 0x36
  1ac8ec: 04 88 42 24  	addiu	$2, $2, -0x77fc <.text+0xffffffffffef8804>
  1ac8f0: 8c f9 62 ac  	sw	$2, -0x674($3)
  1ac8f4: 36 00 03 3c  	lui	$3, 0x36
  1ac8f8: 19 00 02 3c  	lui	$2, 0x19
  1ac8fc: 20 8b 42 24  	addiu	$2, $2, -0x74e0 <.text+0xffffffffffef8b20>
  1ac900: 90 f9 62 ac  	sw	$2, -0x670($3)
  1ac904: 19 00 02 3c  	lui	$2, 0x19
  1ac908: d8 ff 00 10  	b	0x1ac86c <.text+0xac86c>
  1ac90c: 3c b4 42 24  	addiu	$2, $2, -0x4bc4 <.text+0xffffffffffefb43c>
  1ac910: 16 00 40 10  	beqz	$2, 0x1ac96c <.text+0xac96c>
  1ac914: 18 00 02 3c  	lui	$2, 0x18
  1ac918: be 0a 82 90  	lbu	$2, 0xabe($4)
  1ac91c: 02 00 42 30  	andi	$2, $2, 0x2
  1ac920: 0c 00 40 50  	beqzl	$2, 0x1ac954 <.text+0xac954>
  1ac924: 19 00 02 3c  	lui	$2, 0x19
  1ac928: 19 00 02 3c  	lui	$2, 0x19
  1ac92c: 36 00 03 3c  	lui	$3, 0x36
  1ac930: 50 80 42 24  	addiu	$2, $2, -0x7fb0 <.text+0xffffffffffef8050>
  1ac934: 8c f9 62 ac  	sw	$2, -0x674($3)
  1ac938: 19 00 02 3c  	lui	$2, 0x19
  1ac93c: 6c 83 42 24  	addiu	$2, $2, -0x7c94 <.text+0xffffffffffef836c>
  1ac940: 36 00 03 3c  	lui	$3, 0x36
  1ac944: 90 f9 62 ac  	sw	$2, -0x670($3)
  1ac948: 19 00 02 3c  	lui	$2, 0x19
  1ac94c: c7 ff 00 10  	b	0x1ac86c <.text+0xac86c>
  1ac950: b8 ad 42 24  	addiu	$2, $2, -0x5248 <.text+0xffffffffffefadb8>
  1ac954: 36 00 03 3c  	lui	$3, 0x36
  1ac958: 6c 97 42 24  	addiu	$2, $2, -0x6894 <.text+0xffffffffffef976c>
  1ac95c: 8c f9 62 ac  	sw	$2, -0x674($3)
  1ac960: 19 00 02 3c  	lui	$2, 0x19
  1ac964: f6 ff 00 10  	b	0x1ac940 <.text+0xac940>
  1ac968: 88 9a 42 24  	addiu	$2, $2, -0x6578 <.text+0xffffffffffef9a88>
  1ac96c: 36 00 03 3c  	lui	$3, 0x36
  1ac970: 9c 78 42 24  	addiu	$2, $2, 0x789c
  1ac974: 8c f9 62 ac  	sw	$2, -0x674($3)
  1ac978: 36 00 03 3c  	lui	$3, 0x36
  1ac97c: 18 00 02 3c  	lui	$2, 0x18
  1ac980: b8 7b 42 24  	addiu	$2, $2, 0x7bb8
  1ac984: 90 f9 62 ac  	sw	$2, -0x670($3)
  1ac988: 19 00 02 3c  	lui	$2, 0x19
  1ac98c: b7 ff 00 10  	b	0x1ac86c <.text+0xac86c>
  1ac990: d4 a6 42 24  	addiu	$2, $2, -0x592c <.text+0xffffffffffefa6d4>
  1ac994: ff 00 84 30  	andi	$4, $4, 0xff
  1ac998: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1ac99c: f0 00 82 2c  	sltiu	$2, $4, 0xf0
  1ac9a0: 07 00 40 14  	bnez	$2, 0x1ac9c0 <.text+0xac9c0>
  1ac9a4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1ac9a8: 34 00 03 3c  	lui	$3, 0x34
  1ac9ac: 98 54 65 24  	addiu	$5, $3, 0x5498
  1ac9b0: 08 00 a6 8c  	lw	$6, 0x8($5)
  1ac9b4: 04 00 a2 8c  	lw	$2, 0x4($5)
  1ac9b8: 08 00 c2 10  	beq	$6, $2, 0x1ac9dc <.text+0xac9dc>
  1ac9bc: 0c 00 82 24  	addiu	$2, $4, 0xc
  1ac9c0: 34 00 02 3c  	lui	$2, 0x34
  1ac9c4: a0 54 42 8c  	lw	$2, 0x54a0($2)
  1ac9c8: 21 10 44 00  	addu	$2, $2, $4
  1ac9cc: 00 00 42 90  	lbu	$2, 0x0($2)
  1ac9d0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1ac9d4: 08 00 e0 03  	jr	$ra
  1ac9d8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1ac9dc: ff 00 42 30  	andi	$2, $2, 0xff
  1ac9e0: 04 00 42 2c  	sltiu	$2, $2, 0x4
  1ac9e4: 08 00 40 50  	beqzl	$2, 0x1aca08 <.text+0xaca08>
  1ac9e8: fd 00 82 2c  	sltiu	$2, $4, 0xfd
  1ac9ec: 98 54 62 8c  	lw	$2, 0x5498($3)
  1ac9f0: 21 20 c4 00  	addu	$4, $6, $4
  1ac9f4: 14 00 a3 8c  	lw	$3, 0x14($5)
  1ac9f8: 14 00 a2 ac  	sw	$2, 0x14($5)
  1ac9fc: 18 00 a3 ac  	sw	$3, 0x18($5)
  1aca00: f3 ff 00 10  	b	0x1ac9d0 <.text+0xac9d0>
  1aca04: 00 00 82 90  	lbu	$2, 0x0($4)
  1aca08: 09 00 40 54  	bnezl	$2, 0x1aca30 <.text+0xaca30>
  1aca0c: f3 00 02 24  	addiu	$2, $zero, 0xf3
  1aca10: 14 00 a2 8c  	lw	$2, 0x14($5)
  1aca14: 21 20 c4 00  	addu	$4, $6, $4
  1aca18: 98 54 63 8c  	lw	$3, 0x5498($3)
  1aca1c: 18 00 a2 ac  	sw	$2, 0x18($5)
  1aca20: 14 00 a3 ac  	sw	$3, 0x14($5)
  1aca24: 00 00 82 90  	lbu	$2, 0x0($4)
  1aca28: e9 ff 00 10  	b	0x1ac9d0 <.text+0xac9d0>
  1aca2c: 00 00 80 a0  	sb	$zero, 0x0($4)
  1aca30: 03 00 82 10  	beq	$4, $2, 0x1aca40 <.text+0xaca40>
  1aca34: 00 00 00 00  	nop
  1aca38: e4 ff 00 10  	b	0x1ac9cc <.text+0xac9cc>
  1aca3c: 21 10 c4 00  	addu	$2, $6, $4
  1aca40: fe 2d 04 0c  	jal	0x10b7f8 <.text+0xb7f8>
  1aca44: 00 00 00 00  	nop
  1aca48: e2 ff 00 10  	b	0x1ac9d4 <.text+0xac9d4>
  1aca4c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1aca50: ff 00 a5 30  	andi	$5, $5, 0xff
  1aca54: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1aca58: f0 00 a2 2c  	sltiu	$2, $5, 0xf0
  1aca5c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1aca60: 07 00 40 14  	bnez	$2, 0x1aca80 <.text+0xaca80>
  1aca64: ff 00 84 30  	andi	$4, $4, 0xff
  1aca68: 34 00 02 3c  	lui	$2, 0x34
  1aca6c: 98 54 42 24  	addiu	$2, $2, 0x5498
  1aca70: 04 00 43 8c  	lw	$3, 0x4($2)
  1aca74: 08 00 46 8c  	lw	$6, 0x8($2)
  1aca78: 08 00 c3 10  	beq	$6, $3, 0x1aca9c <.text+0xaca9c>
  1aca7c: f3 00 02 24  	addiu	$2, $zero, 0xf3
  1aca80: 34 00 02 3c  	lui	$2, 0x34
  1aca84: a0 54 42 8c  	lw	$2, 0x54a0($2)
  1aca88: 21 10 45 00  	addu	$2, $2, $5
  1aca8c: 00 00 44 a0  	sb	$4, 0x0($2)
  1aca90: 00 00 bf df  	ld	$ra, 0x0($sp)
  1aca94: 08 00 e0 03  	jr	$ra
  1aca98: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1aca9c: 23 00 a2 10  	beq	$5, $2, 0x1acb2c <.text+0xacb2c>
  1acaa0: 0c 00 a2 24  	addiu	$2, $5, 0xc
  1acaa4: ff 00 42 30  	andi	$2, $2, 0xff
  1acaa8: 04 00 42 2c  	sltiu	$2, $2, 0x4
  1acaac: 06 00 40 10  	beqz	$2, 0x1acac8 <.text+0xacac8>
  1acab0: f1 00 02 24  	addiu	$2, $zero, 0xf1
  1acab4: 34 00 02 3c  	lui	$2, 0x34
  1acab8: b8 53 42 24  	addiu	$2, $2, 0x53b8
  1acabc: 21 10 a2 00  	addu	$2, $5, $2
  1acac0: f3 ff 00 10  	b	0x1aca90 <.text+0xaca90>
  1acac4: 13 ff 44 a0  	sb	$4, -0xed($2)
  1acac8: 14 00 a2 10  	beq	$5, $2, 0x1acb1c <.text+0xacb1c>
  1acacc: fd 00 a2 2c  	sltiu	$2, $5, 0xfd
  1acad0: ef ff 40 10  	beqz	$2, 0x1aca90 <.text+0xaca90>
  1acad4: 21 10 c5 00  	addu	$2, $6, $5
  1acad8: fa 00 a3 2c  	sltiu	$3, $5, 0xfa
  1acadc: ec ff 60 14  	bnez	$3, 0x1aca90 <.text+0xaca90>
  1acae0: 00 00 44 a0  	sb	$4, 0x0($2)
  1acae4: 08 00 80 14  	bnez	$4, 0x1acb08 <.text+0xacb08>
  1acae8: 34 00 02 3c  	lui	$2, 0x34
  1acaec: 34 00 03 3c  	lui	$3, 0x34
  1acaf0: 40 10 05 00  	sll	$2, $5, 0x1
  1acaf4: b8 53 63 24  	addiu	$3, $3, 0x53b8
  1acaf8: 21 10 43 00  	addu	$2, $2, $3
  1acafc: 00 01 03 24  	addiu	$3, $zero, 0x100
  1acb00: e3 ff 00 10  	b	0x1aca90 <.text+0xaca90>
  1acb04: de fe 43 a4  	sh	$3, -0x122($2)
  1acb08: 40 18 05 00  	sll	$3, $5, 0x1
  1acb0c: b8 53 42 24  	addiu	$2, $2, 0x53b8
  1acb10: 21 18 62 00  	addu	$3, $3, $2
  1acb14: de ff 00 10  	b	0x1aca90 <.text+0xaca90>
  1acb18: de fe 64 a4  	sh	$4, -0x122($3)
  1acb1c: 59 2d 04 0c  	jal	0x10b564 <.text+0xb564>
  1acb20: 00 00 00 00  	nop
  1acb24: db ff 00 10  	b	0x1aca94 <.text+0xaca94>
  1acb28: 00 00 bf df  	ld	$ra, 0x0($sp)
  1acb2c: 52 2b 04 0c  	jal	0x10ad48 <.text+0xad48>
  1acb30: 00 00 00 00  	nop
  1acb34: d7 ff 00 10  	b	0x1aca94 <.text+0xaca94>
  1acb38: 00 00 bf df  	ld	$ra, 0x0($sp)
  1acb3c: ff ff 85 30  	andi	$5, $4, 0xffff
  1acb40: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1acb44: 10 ff a2 24  	addiu	$2, $5, -0xf0 <.text+0xffffffffffefff10>
  1acb48: 0c ff a3 24  	addiu	$3, $5, -0xf4 <.text+0xffffffffffefff0c>
  1acb4c: 10 00 42 2c  	sltiu	$2, $2, 0x10
  1acb50: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1acb54: 1e 00 40 10  	beqz	$2, 0x1acbd0 <.text+0xacbd0>
  1acb58: 04 00 63 2c  	sltiu	$3, $3, 0x4
  1acb5c: 34 00 02 3c  	lui	$2, 0x34
  1acb60: 0b 00 60 10  	beqz	$3, 0x1acb90 <.text+0xacb90>
  1acb64: 98 54 46 24  	addiu	$6, $2, 0x5498
  1acb68: 98 54 44 8c  	lw	$4, 0x5498($2)
  1acb6c: 04 00 c2 8c  	lw	$2, 0x4($6)
  1acb70: 14 00 c3 8c  	lw	$3, 0x14($6)
  1acb74: 21 10 45 00  	addu	$2, $2, $5
  1acb78: 14 00 c4 ac  	sw	$4, 0x14($6)
  1acb7c: 18 00 c3 ac  	sw	$3, 0x18($6)
  1acb80: 00 00 42 90  	lbu	$2, 0x0($2)
  1acb84: 00 00 bf df  	ld	$ra, 0x0($sp)
  1acb88: 08 00 e0 03  	jr	$ra
  1acb8c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1acb90: f3 00 02 24  	addiu	$2, $zero, 0xf3
  1acb94: 12 00 a2 10  	beq	$5, $2, 0x1acbe0 <.text+0xacbe0>
  1acb98: fd 00 a3 2c  	sltiu	$3, $5, 0xfd
  1acb9c: 34 00 02 3c  	lui	$2, 0x34
  1acba0: 0b 00 60 14  	bnez	$3, 0x1acbd0 <.text+0xacbd0>
  1acba4: 98 54 46 24  	addiu	$6, $2, 0x5498
  1acba8: 14 00 c3 8c  	lw	$3, 0x14($6)
  1acbac: 98 54 44 8c  	lw	$4, 0x5498($2)
  1acbb0: 04 00 c2 8c  	lw	$2, 0x4($6)
  1acbb4: 18 00 c3 ac  	sw	$3, 0x18($6)
  1acbb8: 14 00 c4 ac  	sw	$4, 0x14($6)
  1acbbc: 21 10 45 00  	addu	$2, $2, $5
  1acbc0: 00 00 43 90  	lbu	$3, 0x0($2)
  1acbc4: 00 00 40 a0  	sb	$zero, 0x0($2)
  1acbc8: ee ff 00 10  	b	0x1acb84 <.text+0xacb84>
  1acbcc: 2d 10 60 00  	move	$2, $3
  1acbd0: 34 00 02 3c  	lui	$2, 0x34
  1acbd4: 9c 54 42 8c  	lw	$2, 0x549c($2)
  1acbd8: e9 ff 00 10  	b	0x1acb80 <.text+0xacb80>
  1acbdc: 21 10 45 00  	addu	$2, $2, $5
  1acbe0: fe 2d 04 0c  	jal	0x10b7f8 <.text+0xb7f8>
  1acbe4: 00 00 00 00  	nop
  1acbe8: e7 ff 00 10  	b	0x1acb88 <.text+0xacb88>
  1acbec: 00 00 bf df  	ld	$ra, 0x0($sp)
  1acbf0: ff ff a5 30  	andi	$5, $5, 0xffff
  1acbf4: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1acbf8: 10 ff a2 24  	addiu	$2, $5, -0xf0 <.text+0xffffffffffefff10>
  1acbfc: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1acc00: 10 00 42 2c  	sltiu	$2, $2, 0x10
  1acc04: 2d 00 40 10  	beqz	$2, 0x1accbc <.text+0xaccbc>
  1acc08: ff 00 86 30  	andi	$6, $4, 0xff
  1acc0c: f3 00 02 24  	addiu	$2, $zero, 0xf3
  1acc10: 26 00 a2 10  	beq	$5, $2, 0x1accac <.text+0xaccac>
  1acc14: 0c ff a3 24  	addiu	$3, $5, -0xf4 <.text+0xffffffffffefff0c>
  1acc18: 04 00 62 2c  	sltiu	$2, $3, 0x4
  1acc1c: 08 00 40 10  	beqz	$2, 0x1acc40 <.text+0xacc40>
  1acc20: f1 00 02 24  	addiu	$2, $zero, 0xf1
  1acc24: 34 00 02 3c  	lui	$2, 0x34
  1acc28: b8 53 42 24  	addiu	$2, $2, 0x53b8
  1acc2c: 21 10 62 00  	addu	$2, $3, $2
  1acc30: 07 00 46 a0  	sb	$6, 0x7($2)
  1acc34: 00 00 bf df  	ld	$ra, 0x0($sp)
  1acc38: 08 00 e0 03  	jr	$ra
  1acc3c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1acc40: 16 00 a2 10  	beq	$5, $2, 0x1acc9c <.text+0xacc9c>
  1acc44: fd 00 a2 2c  	sltiu	$2, $5, 0xfd
  1acc48: fa ff 40 10  	beqz	$2, 0x1acc34 <.text+0xacc34>
  1acc4c: 34 00 02 3c  	lui	$2, 0x34
  1acc50: fa 00 a3 2c  	sltiu	$3, $5, 0xfa
  1acc54: 9c 54 42 8c  	lw	$2, 0x549c($2)
  1acc58: 21 10 45 00  	addu	$2, $2, $5
  1acc5c: f5 ff 60 14  	bnez	$3, 0x1acc34 <.text+0xacc34>
  1acc60: 00 00 46 a0  	sb	$6, 0x0($2)
  1acc64: 08 00 c0 14  	bnez	$6, 0x1acc88 <.text+0xacc88>
  1acc68: 34 00 02 3c  	lui	$2, 0x34
  1acc6c: 34 00 03 3c  	lui	$3, 0x34
  1acc70: 40 10 05 00  	sll	$2, $5, 0x1
  1acc74: b8 53 63 24  	addiu	$3, $3, 0x53b8
  1acc78: 21 10 43 00  	addu	$2, $2, $3
  1acc7c: 00 01 03 24  	addiu	$3, $zero, 0x100
  1acc80: ec ff 00 10  	b	0x1acc34 <.text+0xacc34>
  1acc84: de fe 43 a4  	sh	$3, -0x122($2)
  1acc88: 40 18 05 00  	sll	$3, $5, 0x1
  1acc8c: b8 53 42 24  	addiu	$2, $2, 0x53b8
  1acc90: 21 18 62 00  	addu	$3, $3, $2
  1acc94: e7 ff 00 10  	b	0x1acc34 <.text+0xacc34>
  1acc98: de fe 66 a4  	sh	$6, -0x122($3)
  1acc9c: 59 2d 04 0c  	jal	0x10b564 <.text+0xb564>
  1acca0: 2d 20 c0 00  	move	$4, $6
  1acca4: e4 ff 00 10  	b	0x1acc38 <.text+0xacc38>
  1acca8: 00 00 bf df  	ld	$ra, 0x0($sp)
  1accac: 52 2b 04 0c  	jal	0x10ad48 <.text+0xad48>
  1accb0: 2d 20 c0 00  	move	$4, $6
  1accb4: e0 ff 00 10  	b	0x1acc38 <.text+0xacc38>
  1accb8: 00 00 bf df  	ld	$ra, 0x0($sp)
  1accbc: bf ff 02 34  	ori	$2, $zero, 0xffbf
  1accc0: 2b 10 45 00  	sltu	$2, $2, $5
  1accc4: 0b 00 40 10  	beqz	$2, 0x1accf4 <.text+0xaccf4>
  1accc8: 34 00 02 3c  	lui	$2, 0x34
  1acccc: ff ff 02 3c  	lui	$2, 0xffff
  1accd0: 34 00 03 3c  	lui	$3, 0x34
  1accd4: b8 53 63 24  	addiu	$3, $3, 0x53b8
  1accd8: c0 00 42 34  	ori	$2, $2, 0xc0
  1accdc: 21 10 a2 00  	addu	$2, $5, $2
  1acce0: 04 00 64 90  	lbu	$4, 0x4($3)
  1acce4: 21 10 43 00  	addu	$2, $2, $3
  1acce8: d2 ff 80 14  	bnez	$4, 0x1acc34 <.text+0xacc34>
  1accec: 0b 00 46 a0  	sb	$6, 0xb($2)
  1accf0: 34 00 02 3c  	lui	$2, 0x34
  1accf4: 9c 54 42 8c  	lw	$2, 0x549c($2)
  1accf8: 21 10 45 00  	addu	$2, $2, $5
  1accfc: cd ff 00 10  	b	0x1acc34 <.text+0xacc34>
  1acd00: 00 00 46 a0  	sb	$6, 0x0($2)
