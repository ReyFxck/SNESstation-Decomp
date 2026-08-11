# libc string/memory corridor: 0x0019c364..0x0019c687
  19c364: 2d 40 80 00  	move	$8, $4
  19c368: 2d 18 00 01  	move	$3, $8
  19c36c: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c370: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19c374: 07 00 c2 10  	beq	$6, $2, 0x19c394 <.text+0x9c394>
  19c378: 2d 20 40 00  	move	$4, $2
  19c37c: 00 00 a2 90  	lbu	$2, 0x0($5)
  19c380: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c384: 01 00 a5 24  	addiu	$5, $5, 0x1
  19c388: 00 00 62 a0  	sb	$2, 0x0($3)
  19c38c: fb ff c4 14  	bne	$6, $4, 0x19c37c <.text+0x9c37c>
  19c390: 01 00 63 24  	addiu	$3, $3, 0x1
  19c394: 08 00 e0 03  	jr	$ra
  19c398: 2d 10 00 01  	move	$2, $8
  19c39c: 2d 18 80 00  	move	$3, $4
  19c3a0: ff ff 02 3c  	lui	$2, 0xffff
  19c3a4: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c3a8: ff ff 42 34  	ori	$2, $2, 0xffff
  19c3ac: 07 00 c2 10  	beq	$6, $2, 0x19c3cc <.text+0x9c3cc>
  19c3b0: 00 00 00 00  	nop
  19c3b4: ff ff 02 3c  	lui	$2, 0xffff
  19c3b8: ff ff 42 34  	ori	$2, $2, 0xffff
  19c3bc: 00 00 65 a0  	sb	$5, 0x0($3)
  19c3c0: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c3c4: fd ff c2 14  	bne	$6, $2, 0x19c3bc <.text+0x9c3bc>
  19c3c8: 01 00 63 24  	addiu	$3, $3, 0x1
  19c3cc: 08 00 e0 03  	jr	$ra
  19c3d0: 2d 10 80 00  	move	$2, $4
  19c3d4: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19c3d8: 00 00 b0 7f  	ext	$16, $sp, 0x0, 0x1
  19c3dc: 2d 80 80 00  	move	$16, $4
  19c3e0: 10 00 bf 7f  	sq	$ra, 0x10($sp)
  19c3e4: 00 00 82 80  	lb	$2, 0x0($4)
  19c3e8: fe ff 40 54  	bnezl	$2, 0x19c3e4 <.text+0x9c3e4>
  19c3ec: 01 00 84 24  	addiu	$4, $4, 0x1
  19c3f0: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  19c3f4: 00 00 00 00  	nop
  19c3f8: 2d 10 00 02  	move	$2, $16
  19c3fc: 10 00 bf 7b  	lq	$ra, 0x10($sp)
  19c400: 00 00 b0 7b  	lq	$s0, 0x0($sp)
  19c404: 08 00 e0 03  	jr	$ra
  19c408: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19c40c: 00 00 00 00  	nop
  19c410: 09 00 c0 14  	bnez	$6, 0x19c438 <.text+0x9c438>
  19c414: 00 00 83 80  	lb	$3, 0x0($4)
  19c418: 08 00 e0 03  	jr	$ra
  19c41c: 2d 10 00 00  	move	$2, $zero
  19c420: fd ff c0 10  	beqz	$6, 0x19c418 <.text+0x9c418>
  19c424: 00 00 00 00  	nop
  19c428: fb ff e0 10  	beqz	$7, 0x19c418 <.text+0x9c418>
  19c42c: 01 00 84 24  	addiu	$4, $4, 0x1
  19c430: 01 00 a5 24  	addiu	$5, $5, 0x1
  19c434: 00 00 83 80  	lb	$3, 0x0($4)
  19c438: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c43c: 00 00 a2 80  	lb	$2, 0x0($5)
  19c440: f7 ff 62 10  	beq	$3, $2, 0x19c420 <.text+0x9c420>
  19c444: 00 00 87 90  	lbu	$7, 0x0($4)
  19c448: 00 00 a2 90  	lbu	$2, 0x0($5)
  19c44c: 00 00 83 90  	lbu	$3, 0x0($4)
  19c450: 08 00 e0 03  	jr	$ra
  19c454: 23 10 62 00  	subu	$2, $3, $2
  19c458: ff ff 02 3c  	lui	$2, 0xffff
  19c45c: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c460: ff ff 42 34  	ori	$2, $2, 0xffff
  19c464: 0c 00 c2 10  	beq	$6, $2, 0x19c498 <.text+0x9c498>
  19c468: 00 00 00 00  	nop
  19c46c: ff ff 07 3c  	lui	$7, 0xffff
  19c470: ff ff e7 34  	ori	$7, $7, 0xffff
  19c474: 00 00 83 90  	lbu	$3, 0x0($4)
  19c478: 00 00 a2 90  	lbu	$2, 0x0($5)
  19c47c: 03 00 62 10  	beq	$3, $2, 0x19c48c <.text+0x9c48c>
  19c480: 01 00 84 24  	addiu	$4, $4, 0x1
  19c484: 08 00 e0 03  	jr	$ra
  19c488: 23 10 62 00  	subu	$2, $3, $2
  19c48c: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c490: f8 ff c7 14  	bne	$6, $7, 0x19c474 <.text+0x9c474>
  19c494: 01 00 a5 24  	addiu	$5, $5, 0x1
  19c498: 08 00 e0 03  	jr	$ra
  19c49c: 2d 10 00 00  	move	$2, $zero
  19c4a0: 2d 40 80 00  	move	$8, $4
  19c4a4: 2b 10 a8 00  	sltu	$2, $5, $8
  19c4a8: 12 00 40 10  	beqz	$2, 0x19c4f4 <.text+0x9c4f4>
  19c4ac: 2d 18 00 01  	move	$3, $8
  19c4b0: 21 38 a6 00  	addu	$7, $5, $6
  19c4b4: 2b 10 07 01  	sltu	$2, $8, $7
  19c4b8: 0e 00 40 10  	beqz	$2, 0x19c4f4 <.text+0x9c4f4>
  19c4bc: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19c4c0: 21 18 06 01  	addu	$3, $8, $6
  19c4c4: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c4c8: 15 00 c2 10  	beq	$6, $2, 0x19c520 <.text+0x9c520>
  19c4cc: 2d 28 e0 00  	move	$5, $7
  19c4d0: 2d 20 40 00  	move	$4, $2
  19c4d4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  19c4d8: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  19c4dc: 00 00 a2 90  	lbu	$2, 0x0($5)
  19c4e0: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c4e4: fb ff c4 14  	bne	$6, $4, 0x19c4d4 <.text+0x9c4d4>
  19c4e8: 00 00 62 a0  	sb	$2, 0x0($3)
  19c4ec: 08 00 e0 03  	jr	$ra
  19c4f0: 2d 10 00 01  	move	$2, $8
  19c4f4: 2d 18 80 00  	move	$3, $4
  19c4f8: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c4fc: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19c500: 07 00 c2 10  	beq	$6, $2, 0x19c520 <.text+0x9c520>
  19c504: 2d 20 40 00  	move	$4, $2
  19c508: 00 00 a2 90  	lbu	$2, 0x0($5)
  19c50c: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c510: 01 00 a5 24  	addiu	$5, $5, 0x1
  19c514: 00 00 62 a0  	sb	$2, 0x0($3)
  19c518: fb ff c4 14  	bne	$6, $4, 0x19c508 <.text+0x9c508>
  19c51c: 01 00 63 24  	addiu	$3, $3, 0x1
  19c520: 08 00 e0 03  	jr	$ra
  19c524: 2d 10 00 01  	move	$2, $8
  19c528: 2d 38 80 00  	move	$7, $4
  19c52c: 2d 18 80 00  	move	$3, $4
  19c530: 00 00 a2 90  	lbu	$2, 0x0($5)
  19c534: 01 00 a5 24  	addiu	$5, $5, 0x1
  19c538: 00 00 62 a0  	sb	$2, 0x0($3)
  19c53c: 00 16 02 00  	sll	$2, $2, 0x18
  19c540: fb ff 40 14  	bnez	$2, 0x19c530 <.text+0x9c530>
  19c544: 01 00 63 24  	addiu	$3, $3, 0x1
  19c548: 08 00 e0 03  	jr	$ra
  19c54c: 2d 10 e0 00  	move	$2, $7
  19c550: 2d 40 80 00  	move	$8, $4
  19c554: 00 00 00 00  	nop
  19c558: 10 00 c0 10  	beqz	$6, 0x19c59c <.text+0x9c59c>
  19c55c: 2d 10 c0 00  	move	$2, $6
  19c560: 00 00 a2 90  	lbu	$2, 0x0($5)
  19c564: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c568: 01 00 a5 24  	addiu	$5, $5, 0x1
  19c56c: 00 00 82 a0  	sb	$2, 0x0($4)
  19c570: 00 16 02 00  	sll	$2, $2, 0x18
  19c574: f8 ff 40 14  	bnez	$2, 0x19c558 <.text+0x9c558>
  19c578: 01 00 84 24  	addiu	$4, $4, 0x1
  19c57c: 2d 10 c0 00  	move	$2, $6
  19c580: 06 00 40 10  	beqz	$2, 0x19c59c <.text+0x9c59c>
  19c584: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c588: 00 00 80 a0  	sb	$zero, 0x0($4)
  19c58c: 2d 10 c0 00  	move	$2, $6
  19c590: 01 00 84 24  	addiu	$4, $4, 0x1
  19c594: fc ff 40 14  	bnez	$2, 0x19c588 <.text+0x9c588>
  19c598: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  19c59c: 08 00 e0 03  	jr	$ra
  19c5a0: 2d 10 00 01  	move	$2, $8
  19c5a4: 00 00 00 00  	nop
  19c5a8: 1c 00 04 3c  	lui	$4, 0x1c
  19c5ac: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19c5b0: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19c5b4: 05 79 06 0c  	jal	0x19e414 <.text+0x9e414>
  19c5b8: 60 a4 84 24  	addiu	$4, $4, -0x5ba0 <.text+0xffffffffffefa460>
  19c5bc: 38 00 04 0c  	jal	0x1000e0 <.text+0xe0>
  19c5c0: 01 00 04 24  	addiu	$4, $zero, 0x1
  19c5c4: fd ff 00 10  	b	0x19c5bc <.text+0x9c5bc>
  19c5c8: 00 00 00 00  	nop
  19c5cc: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19c5d0: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19c5d4: 38 00 04 0c  	jal	0x1000e0 <.text+0xe0>
  19c5d8: 00 00 00 00  	nop
  19c5dc: 00 00 bf df  	ld	$ra, 0x0($sp)
  19c5e0: 08 00 e0 03  	jr	$ra
  19c5e4: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19c5e8: 2d 38 80 00  	move	$7, $4
  19c5ec: 00 00 82 80  	lb	$2, 0x0($4)
  19c5f0: 04 00 40 10  	beqz	$2, 0x19c604 <.text+0x9c604>
  19c5f4: 01 00 84 24  	addiu	$4, $4, 0x1
  19c5f8: 00 00 82 80  	lb	$2, 0x0($4)
  19c5fc: fb ff 40 14  	bnez	$2, 0x19c5ec <.text+0x9c5ec>
  19c600: 01 00 84 24  	addiu	$4, $4, 0x1
  19c604: ff ff 84 24  	addiu	$4, $4, -0x1 <.text+0xffffffffffefffff>
  19c608: 08 00 e0 03  	jr	$ra
  19c60c: 23 10 87 00  	subu	$2, $4, $7
  19c610: ff 00 a5 30  	andi	$5, $5, 0xff
  19c614: 05 00 00 10  	b	0x19c62c <.text+0x9c62c>
  19c618: 00 00 82 90  	lbu	$2, 0x0($4)
  19c61c: 06 00 45 50  	beql	$2, $5, 0x19c638 <.text+0x9c638>
  19c620: 00 00 83 90  	lbu	$3, 0x0($4)
  19c624: 01 00 84 24  	addiu	$4, $4, 0x1
  19c628: 00 00 82 90  	lbu	$2, 0x0($4)
  19c62c: fb ff 40 14  	bnez	$2, 0x19c61c <.text+0x9c61c>
  19c630: 00 00 00 00  	nop
  19c634: 00 00 83 90  	lbu	$3, 0x0($4)
  19c638: 2d 10 00 00  	move	$2, $zero
  19c63c: 26 18 65 00  	xor	$3, $3, $5
  19c640: 08 00 e0 03  	jr	$ra
  19c644: 0a 10 83 00  	movz	$2, $4, $3
  19c648: 00 00 82 80  	lb	$2, 0x0($4)
  19c64c: 0b 00 40 10  	beqz	$2, 0x19c67c <.text+0x9c67c>
  19c650: 00 00 83 90  	lbu	$3, 0x0($4)
  19c654: 00 16 03 00  	sll	$2, $3, 0x18
  19c658: 00 00 a3 80  	lb	$3, 0x0($5)
  19c65c: 03 16 02 00  	sra	$2, $2, 0x18
  19c660: 06 00 43 54  	bnel	$2, $3, 0x19c67c <.text+0x9c67c>
  19c664: 00 00 83 90  	lbu	$3, 0x0($4)
  19c668: 01 00 84 24  	addiu	$4, $4, 0x1
  19c66c: 01 00 a5 24  	addiu	$5, $5, 0x1
  19c670: 00 00 82 80  	lb	$2, 0x0($4)
  19c674: f7 ff 40 14  	bnez	$2, 0x19c654 <.text+0x9c654>
  19c678: 00 00 83 90  	lbu	$3, 0x0($4)
  19c67c: 00 00 a2 90  	lbu	$2, 0x0($5)
  19c680: 08 00 e0 03  	jr	$ra
  19c684: 23 10 62 00  	subu	$2, $3, $2
