# loadfile / IOP heap / IOP reset public corridor: 0x0019d600..0x0019d84b
  19d600: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19d604: 2d 38 00 00  	move	$7, $zero
  19d608: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19d60c: fa 7d 06 0c  	jal	0x19f7e8 <.text+0x9f7e8>
  19d610: 2d 40 00 00  	move	$8, $zero
  19d614: 00 00 bf df  	ld	$ra, 0x0($sp)
  19d618: 08 00 e0 03  	jr	$ra
  19d61c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19d620: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19d624: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19d628: 3d 7e 06 0c  	jal	0x19f8f4 <.text+0x9f8f4>
  19d62c: 2d 38 00 00  	move	$7, $zero
  19d630: 00 00 bf df  	ld	$ra, 0x0($sp)
  19d634: 08 00 e0 03  	jr	$ra
  19d638: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19d63c: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19d640: 42 00 02 3c  	lui	$2, 0x42
  19d644: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19d648: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19d64c: b4 5a 42 8c  	lw	$2, 0x5ab4($2)
  19d650: 05 00 40 14  	bnez	$2, 0x19d668 <.text+0x9d668>
  19d654: 2d 80 80 00  	move	$16, $4
  19d658: 7a 7e 06 0c  	jal	0x19f9e8 <.text+0x9f9e8>
  19d65c: 00 00 00 00  	nop
  19d660: 10 00 40 04  	bltz	$2, 0x19d6a4 <.text+0x9d6a4>
  19d664: 2d 18 00 00  	move	$3, $zero
  19d668: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19d66c: 45 00 04 3c  	lui	$4, 0x45
  19d670: 01 00 05 24  	addiu	$5, $zero, 0x1
  19d674: c0 0b 84 24  	addiu	$4, $4, 0xbc0
  19d678: 2d 30 00 00  	move	$6, $zero
  19d67c: 04 00 08 24  	addiu	$8, $zero, 0x4
  19d680: 2d 48 e0 00  	move	$9, $7
  19d684: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19d688: 2d 58 00 00  	move	$11, $zero
  19d68c: 10 00 b0 af  	sw	$16, 0x10($sp)
  19d690: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19d694: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19d698: 10 00 a3 8f  	lw	$3, 0x10($sp)
  19d69c: 00 00 42 28  	slti	$2, $2, 0x0
  19d6a0: 0b 18 02 00  	movn	$3, $zero, $2
  19d6a4: 30 00 bf df  	ld	$ra, 0x30($sp)
  19d6a8: 2d 10 60 00  	move	$2, $3
  19d6ac: 20 00 b0 df  	ld	$16, 0x20($sp)
  19d6b0: 08 00 e0 03  	jr	$ra
  19d6b4: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19d6b8: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19d6bc: 42 00 02 3c  	lui	$2, 0x42
  19d6c0: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19d6c4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19d6c8: b4 5a 42 8c  	lw	$2, 0x5ab4($2)
  19d6cc: 06 00 40 14  	bnez	$2, 0x19d6e8 <.text+0x9d6e8>
  19d6d0: 2d 80 80 00  	move	$16, $4
  19d6d4: 7a 7e 06 0c  	jal	0x19f9e8 <.text+0x9f9e8>
  19d6d8: 00 00 00 00  	nop
  19d6dc: ff ff 04 3c  	lui	$4, 0xffff
  19d6e0: 12 00 40 04  	bltz	$2, 0x19d72c <.text+0x9d72c>
  19d6e4: ff 29 84 34  	ori	$4, $4, 0x29ff
  19d6e8: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19d6ec: 45 00 04 3c  	lui	$4, 0x45
  19d6f0: c0 0b 84 24  	addiu	$4, $4, 0xbc0
  19d6f4: 02 00 05 24  	addiu	$5, $zero, 0x2
  19d6f8: 2d 30 00 00  	move	$6, $zero
  19d6fc: 04 00 08 24  	addiu	$8, $zero, 0x4
  19d700: 2d 48 e0 00  	move	$9, $7
  19d704: 04 00 0a 24  	addiu	$10, $zero, 0x4
  19d708: 2d 58 00 00  	move	$11, $zero
  19d70c: 10 00 b0 af  	sw	$16, 0x10($sp)
  19d710: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19d714: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19d718: ff ff 04 3c  	lui	$4, 0xffff
  19d71c: 10 00 a3 8f  	lw	$3, 0x10($sp)
  19d720: 00 00 42 28  	slti	$2, $2, 0x0
  19d724: ed 29 84 34  	ori	$4, $4, 0x29ed
  19d728: 0a 20 62 00  	movz	$4, $3, $2
  19d72c: 30 00 bf df  	ld	$ra, 0x30($sp)
  19d730: 2d 10 80 00  	move	$2, $4
  19d734: 20 00 b0 df  	ld	$16, 0x20($sp)
  19d738: 08 00 e0 03  	jr	$ra
  19d73c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19d740: 50 ff bd 27  	addiu	$sp, $sp, -0xb0 <.text+0xffffffffffefff50>
  19d744: a0 00 bf ff  	sd	$ra, 0xa0($sp)
  19d748: 90 00 b1 ff  	sd	$17, 0x90($sp)
  19d74c: 2d 88 80 00  	move	$17, $4
  19d750: 80 00 b0 ff  	sd	$16, 0x80($sp)
  19d754: 74 7d 06 0c  	jal	0x19f5d0 <.text+0x9f5d0>
  19d758: 2d 80 a0 00  	move	$16, $5
  19d75c: 2d 20 a0 03  	move	$4, $sp
  19d760: 70 00 06 24  	addiu	$6, $zero, 0x70
  19d764: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  19d768: 2d 28 00 00  	move	$5, $zero
  19d76c: 70 00 02 24  	addiu	$2, $zero, 0x70
  19d770: 00 80 03 3c  	lui	$3, 0x8000
  19d774: 14 00 b0 af  	sw	$16, 0x14($sp)
  19d778: 03 00 63 34  	ori	$3, $3, 0x3
  19d77c: 2d 20 20 02  	move	$4, $17
  19d780: 00 00 a2 af  	sw	$2, 0x0($sp)
  19d784: 0c 00 20 12  	beqz	$17, 0x19d7b8 <.text+0x9d7b8>
  19d788: 08 00 a3 af  	sw	$3, 0x8($sp)
  19d78c: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  19d790: 00 00 00 00  	nop
  19d794: 18 00 a4 27  	addiu	$4, $sp, 0x18
  19d798: 51 00 43 28  	slti	$3, $2, 0x51
  19d79c: 10 00 a2 af  	sw	$2, 0x10($sp)
  19d7a0: 03 00 60 14  	bnez	$3, 0x19d7b0 <.text+0x9d7b0>
  19d7a4: 2d 28 20 02  	move	$5, $17
  19d7a8: 50 00 02 24  	addiu	$2, $zero, 0x50
  19d7ac: 10 00 a2 af  	sw	$2, 0x10($sp)
  19d7b0: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  19d7b4: 10 00 a6 8f  	lw	$6, 0x10($sp)
  19d7b8: 02 00 04 24  	addiu	$4, $zero, 0x2
  19d7bc: c0 73 06 0c  	jal	0x19cf00 <.text+0x9cf00>
  19d7c0: 70 00 bd af  	sw	$sp, 0x70($sp)
  19d7c4: 2d 20 a0 03  	move	$4, $sp
  19d7c8: 74 00 a2 af  	sw	$2, 0x74($sp)
  19d7cc: 70 00 05 24  	addiu	$5, $zero, 0x70
  19d7d0: 70 00 02 24  	addiu	$2, $zero, 0x70
  19d7d4: 78 00 a2 af  	sw	$2, 0x78($sp)
  19d7d8: 44 00 02 24  	addiu	$2, $zero, 0x44
  19d7dc: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  19d7e0: 7c 00 a2 af  	sw	$2, 0x7c($sp)
  19d7e4: 01 00 05 24  	addiu	$5, $zero, 0x1
  19d7e8: b8 73 06 0c  	jal	0x19cee0 <.text+0x9cee0>
  19d7ec: 70 00 a4 27  	addiu	$4, $sp, 0x70
  19d7f0: 07 00 40 14  	bnez	$2, 0x19d810 <.text+0x9d810>
  19d7f4: 2d 18 00 00  	move	$3, $zero
  19d7f8: a0 00 bf df  	ld	$ra, 0xa0($sp)
  19d7fc: 2d 10 60 00  	move	$2, $3
  19d800: 90 00 b1 df  	ld	$17, 0x90($sp)
  19d804: 80 00 b0 df  	ld	$16, 0x80($sp)
  19d808: 08 00 e0 03  	jr	$ra
  19d80c: b0 00 bd 27  	addiu	$sp, $sp, 0xb0
  19d810: 04 00 04 24  	addiu	$4, $zero, 0x4
  19d814: bc 73 06 0c  	jal	0x19cef0 <.text+0x9cef0>
  19d818: 01 00 05 3c  	lui	$5, 0x1
  19d81c: 04 00 04 24  	addiu	$4, $zero, 0x4
  19d820: bc 73 06 0c  	jal	0x19cef0 <.text+0x9cef0>
  19d824: 02 00 05 3c  	lui	$5, 0x2
  19d828: 00 80 04 3c  	lui	$4, 0x8000
  19d82c: 2d 28 00 00  	move	$5, $zero
  19d830: bc 73 06 0c  	jal	0x19cef0 <.text+0x9cef0>
  19d834: 02 00 84 34  	ori	$4, $4, 0x2
  19d838: 00 80 04 3c  	lui	$4, 0x8000
  19d83c: bc 73 06 0c  	jal	0x19cef0 <.text+0x9cef0>
  19d840: 2d 28 00 00  	move	$5, $zero
  19d844: ec ff 00 10  	b	0x19d7f8 <.text+0x9d7f8>
  19d848: 01 00 03 24  	addiu	$3, $zero, 0x1
