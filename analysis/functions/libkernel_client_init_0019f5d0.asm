# SifStopDma + fio/loadfile/iopheap helpers: 0x0019f5d0..0x0019faa7
  19f5d0: 6b 00 03 24  	addiu	$3, $zero, 0x6b
  19f5d4: 0c 00 00 00  	syscall
  19f5d8: 08 00 e0 03  	jr	$ra
  19f5dc: 00 00 00 00  	nop
  19f5e0: 89 ff 03 24  	addiu	$3, $zero, -0x77 <.text+0xffffffffffefff89>
  19f5e4: 0c 00 00 00  	syscall
  19f5e8: 08 00 e0 03  	jr	$ra
  19f5ec: 00 00 00 00  	nop
  19f5f0: 78 00 03 24  	addiu	$3, $zero, 0x78
  19f5f4: 0c 00 00 00  	syscall
  19f5f8: 08 00 e0 03  	jr	$ra
  19f5fc: 00 00 00 00  	nop
  19f600: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19f604: 42 00 02 3c  	lui	$2, 0x42
  19f608: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19f60c: 20 00 b0 ff  	sd	$16, 0x20($sp)
  19f610: b0 5a 42 8c  	lw	$2, 0x5ab0($2)
  19f614: 06 00 40 10  	beqz	$2, 0x19f630 <.text+0x9f630>
  19f618: 2d 20 00 00  	move	$4, $zero
  19f61c: 30 00 bf df  	ld	$ra, 0x30($sp)
  19f620: 2d 10 80 00  	move	$2, $4
  19f624: 20 00 b0 df  	ld	$16, 0x20($sp)
  19f628: 08 00 e0 03  	jr	$ra
  19f62c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19f630: 03 73 06 0c  	jal	0x19cc0c <.text+0x9cc0c>
  19f634: 2d 20 00 00  	move	$4, $zero
  19f638: 45 00 02 3c  	lui	$2, 0x45
  19f63c: 00 80 05 3c  	lui	$5, 0x8000
  19f640: 10 03 50 24  	addiu	$16, $2, 0x310
  19f644: 01 00 a5 34  	ori	$5, $5, 0x1
  19f648: 2d 20 00 02  	move	$4, $16
  19f64c: a2 71 06 0c  	jal	0x19c688 <.text+0x9c688>
  19f650: 2d 30 00 00  	move	$6, $zero
  19f654: f1 ff 40 04  	bltz	$2, 0x19f61c <.text+0x9f61c>
  19f658: 2d 20 40 00  	move	$4, $2
  19f65c: 24 00 02 8e  	lw	$2, 0x24($16)
  19f660: 0f 00 03 3c  	lui	$3, 0xf
  19f664: 0f 00 40 14  	bnez	$2, 0x19f6a4 <.text+0x9f6a4>
  19f668: ff ff 63 34  	ori	$3, $3, 0xffff
		...
  19f680: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  19f684: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
		...
  19f694: f5 ff 62 14  	bne	$3, $2, 0x19f66c <.text+0x9f66c>
  19f698: 45 00 02 3c  	lui	$2, 0x45
  19f69c: e8 ff 00 10  	b	0x19f640 <.text+0x9f640>
  19f6a0: 00 80 05 3c  	lui	$5, 0x8000
  19f6a4: 01 00 10 24  	addiu	$16, $zero, 0x1
  19f6a8: 2d 20 a0 03  	move	$4, $sp
  19f6ac: 08 00 b0 af  	sw	$16, 0x8($sp)
  19f6b0: 04 00 b0 af  	sw	$16, 0x4($sp)
  19f6b4: 9c 73 06 0c  	jal	0x19ce70 <.text+0x9ce70>
  19f6b8: 14 00 a0 af  	sw	$zero, 0x14($sp)
  19f6bc: ff ff 04 3c  	lui	$4, 0xffff
  19f6c0: 42 00 03 3c  	lui	$3, 0x42
  19f6c4: 44 6e 62 ac  	sw	$2, 0x6e44($3)
  19f6c8: d4 ff 40 04  	bltz	$2, 0x19f61c <.text+0x9f61c>
  19f6cc: fe 29 84 34  	ori	$4, $4, 0x29fe
  19f6d0: 42 00 02 3c  	lui	$2, 0x42
  19f6d4: 2d 20 00 00  	move	$4, $zero
  19f6d8: b0 5a 50 ac  	sw	$16, 0x5ab0($2)
  19f6dc: 42 00 02 3c  	lui	$2, 0x42
  19f6e0: ce ff 00 10  	b	0x19f61c <.text+0x9f61c>
  19f6e4: 48 6e 40 ac  	sw	$zero, 0x6e48($2)
  19f6e8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19f6ec: 42 00 02 3c  	lui	$2, 0x42
  19f6f0: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19f6f4: a4 73 06 0c  	jal	0x19ce90 <.text+0x9ce90>
  19f6f8: 44 6e 44 8c  	lw	$4, 0x6e44($2)
  19f6fc: 00 00 bf df  	ld	$ra, 0x0($sp)
  19f700: 08 00 e0 03  	jr	$ra
  19f704: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19f708: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19f70c: 42 00 02 3c  	lui	$2, 0x42
  19f710: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19f714: 2d 88 a0 00  	move	$17, $5
  19f718: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19f71c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19f720: b0 5a 42 8c  	lw	$2, 0x5ab0($2)
  19f724: 27 00 40 10  	beqz	$2, 0x19f7c4 <.text+0x9f7c4>
  19f728: 2d 80 80 00  	move	$16, $4
  19f72c: 42 00 02 3c  	lui	$2, 0x42
  19f730: ff ff 03 3c  	lui	$3, 0xffff
  19f734: 48 6e 44 8c  	lw	$4, 0x6e48($2)
  19f738: 01 00 02 24  	addiu	$2, $zero, 0x1
  19f73c: 07 00 82 10  	beq	$4, $2, 0x19f75c <.text+0x9f75c>
  19f740: fb 29 63 34  	ori	$3, $3, 0x29fb
  19f744: 20 00 bf df  	ld	$ra, 0x20($sp)
  19f748: 2d 10 60 00  	move	$2, $3
  19f74c: 10 00 b1 df  	ld	$17, 0x10($sp)
  19f750: 00 00 b0 df  	ld	$16, 0x0($sp)
  19f754: 08 00 e0 03  	jr	$ra
  19f758: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  19f75c: 15 00 00 52  	beqzl	$16, 0x19f7b4 <.text+0x9f7b4>
  19f760: 42 00 10 3c  	lui	$16, 0x42
  19f764: 03 00 04 12  	beq	$16, $4, 0x19f774 <.text+0x9f774>
  19f768: ff ff 03 3c  	lui	$3, 0xffff
  19f76c: f5 ff 00 10  	b	0x19f744 <.text+0x9f744>
  19f770: fb 29 63 34  	ori	$3, $3, 0x29fb
  19f774: 42 00 10 3c  	lui	$16, 0x42
  19f778: 40 7f 06 0c  	jal	0x19fd00 <.text+0x9fd00>
  19f77c: 44 6e 04 8e  	lw	$4, 0x6e44($16)
  19f780: f0 ff 40 04  	bltz	$2, 0x19f744 <.text+0x9f744>
  19f784: 2d 18 00 00  	move	$3, $zero
  19f788: 3c 7f 06 0c  	jal	0x19fcf0 <.text+0x9fcf0>
  19f78c: 44 6e 04 8e  	lw	$4, 0x6e44($16)
  19f790: 06 00 20 12  	beqz	$17, 0x19f7ac <.text+0x9f7ac>
  19f794: 45 00 02 3c  	lui	$2, 0x45
  19f798: 00 20 03 3c  	lui	$3, 0x2000
  19f79c: c0 03 42 24  	addiu	$2, $2, 0x3c0
  19f7a0: 25 10 43 00  	or	$2, $2, $3
  19f7a4: 00 00 42 8c  	lw	$2, 0x0($2)
  19f7a8: 00 00 22 ae  	sw	$2, 0x0($17)
  19f7ac: e5 ff 00 10  	b	0x19f744 <.text+0x9f744>
  19f7b0: 01 00 03 24  	addiu	$3, $zero, 0x1
  19f7b4: a8 73 06 0c  	jal	0x19cea0 <.text+0x9cea0>
  19f7b8: 44 6e 04 8e  	lw	$4, 0x6e44($16)
  19f7bc: f2 ff 00 10  	b	0x19f788 <.text+0x9f788>
  19f7c0: 00 00 00 00  	nop
  19f7c4: 80 7d 06 0c  	jal	0x19f600 <.text+0x9f600>
  19f7c8: 00 00 00 00  	nop
  19f7cc: d7 ff 41 04  	bgez	$2, 0x19f72c <.text+0x9f72c>
  19f7d0: 2d 18 40 00  	move	$3, $2
  19f7d4: dc ff 00 10  	b	0x19f748 <.text+0x9f748>
  19f7d8: 20 00 bf df  	ld	$ra, 0x20($sp)
  19f7dc: 42 00 02 3c  	lui	$2, 0x42
  19f7e0: 08 00 e0 03  	jr	$ra
  19f7e4: 48 6e 44 ac  	sw	$4, 0x6e48($2)
  19f7e8: 90 fd bd 27  	addiu	$sp, $sp, -0x270 <.text+0xffffffffffeffd90>
  19f7ec: 42 00 02 3c  	lui	$2, 0x42
  19f7f0: 50 02 b4 ff  	sd	$20, 0x250($sp)
  19f7f4: 2d a0 e0 00  	move	$20, $7
  19f7f8: 40 02 b3 ff  	sd	$19, 0x240($sp)
  19f7fc: 2d 98 00 01  	move	$19, $8
  19f800: 30 02 b2 ff  	sd	$18, 0x230($sp)
  19f804: 2d 90 80 00  	move	$18, $4
  19f808: 20 02 b1 ff  	sd	$17, 0x220($sp)
  19f80c: 2d 88 c0 00  	move	$17, $6
  19f810: 10 02 b0 ff  	sd	$16, 0x210($sp)
  19f814: 60 02 bf ff  	sd	$ra, 0x260($sp)
  19f818: b8 5a 42 8c  	lw	$2, 0x5ab8($2)
  19f81c: 06 00 40 14  	bnez	$2, 0x19f838 <.text+0x9f838>
  19f820: 2d 80 a0 00  	move	$16, $5
  19f824: 48 7f 06 0c  	jal	0x19fd20 <.text+0x9fd20>
  19f828: 00 00 00 00  	nop
  19f82c: ff ff 03 3c  	lui	$3, 0xffff
  19f830: 1e 00 40 04  	bltz	$2, 0x19f8ac <.text+0x9f8ac>
  19f834: ff 29 63 34  	ori	$3, $3, 0x29ff
  19f838: 2d 28 00 00  	move	$5, $zero
  19f83c: 10 00 a4 27  	addiu	$4, $sp, 0x10
  19f840: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  19f844: 00 02 06 24  	addiu	$6, $zero, 0x200
  19f848: 18 00 a4 27  	addiu	$4, $sp, 0x18
  19f84c: fc 00 06 24  	addiu	$6, $zero, 0xfc
  19f850: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  19f854: 2d 28 40 02  	move	$5, $18
  19f858: 03 00 20 12  	beqz	$17, 0x19f868 <.text+0x9f868>
  19f85c: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19f860: 1b 00 00 56  	bnezl	$16, 0x19f8d0 <.text+0x9f8d0>
  19f864: fd 00 03 2a  	slti	$3, $16, 0xfd
  19f868: 45 00 04 3c  	lui	$4, 0x45
  19f86c: 2d 28 60 02  	move	$5, $19
  19f870: f0 0b 84 24  	addiu	$4, $4, 0xbf0
  19f874: 2d 30 00 00  	move	$6, $zero
  19f878: 00 02 08 24  	addiu	$8, $zero, 0x200
  19f87c: 2d 48 e0 00  	move	$9, $7
  19f880: 08 00 0a 24  	addiu	$10, $zero, 0x8
  19f884: 2d 58 00 00  	move	$11, $zero
  19f888: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19f88c: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19f890: ff ff 03 3c  	lui	$3, 0xffff
  19f894: 05 00 40 04  	bltz	$2, 0x19f8ac <.text+0x9f8ac>
  19f898: ed 29 63 34  	ori	$3, $3, 0x29ed
  19f89c: 02 00 80 12  	beqz	$20, 0x19f8a8 <.text+0x9f8a8>
  19f8a0: 14 00 a2 8f  	lw	$2, 0x14($sp)
  19f8a4: 00 00 82 ae  	sw	$2, 0x0($20)
  19f8a8: 10 00 a3 8f  	lw	$3, 0x10($sp)
  19f8ac: 60 02 bf df  	ld	$ra, 0x260($sp)
  19f8b0: 2d 10 60 00  	move	$2, $3
  19f8b4: 50 02 b4 df  	ld	$20, 0x250($sp)
  19f8b8: 40 02 b3 df  	ld	$19, 0x240($sp)
  19f8bc: 30 02 b2 df  	ld	$18, 0x230($sp)
  19f8c0: 20 02 b1 df  	ld	$17, 0x220($sp)
  19f8c4: 10 02 b0 df  	ld	$16, 0x210($sp)
  19f8c8: 08 00 e0 03  	jr	$ra
  19f8cc: 70 02 bd 27  	addiu	$sp, $sp, 0x270
  19f8d0: fc 00 02 24  	addiu	$2, $zero, 0xfc
  19f8d4: 0b 10 03 02  	movn	$2, $16, $3
  19f8d8: 14 01 a4 27  	addiu	$4, $sp, 0x114
  19f8dc: 2d 28 20 02  	move	$5, $17
  19f8e0: 2d 30 40 00  	move	$6, $2
  19f8e4: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  19f8e8: 10 00 a2 af  	sw	$2, 0x10($sp)
  19f8ec: de ff 00 10  	b	0x19f868 <.text+0x9f868>
  19f8f0: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19f8f4: a0 fd bd 27  	addiu	$sp, $sp, -0x260 <.text+0xffffffffffeffda0>
  19f8f8: 42 00 02 3c  	lui	$2, 0x42
  19f8fc: 40 02 b3 ff  	sd	$19, 0x240($sp)
  19f900: 2d 98 e0 00  	move	$19, $7
  19f904: 30 02 b2 ff  	sd	$18, 0x230($sp)
  19f908: 2d 90 80 00  	move	$18, $4
  19f90c: 20 02 b1 ff  	sd	$17, 0x220($sp)
  19f910: 2d 88 c0 00  	move	$17, $6
  19f914: 10 02 b0 ff  	sd	$16, 0x210($sp)
  19f918: 50 02 bf ff  	sd	$ra, 0x250($sp)
  19f91c: b8 5a 42 8c  	lw	$2, 0x5ab8($2)
  19f920: 06 00 40 14  	bnez	$2, 0x19f93c <.text+0x9f93c>
  19f924: 2d 80 a0 00  	move	$16, $5
  19f928: 48 7f 06 0c  	jal	0x19fd20 <.text+0x9fd20>
  19f92c: 00 00 00 00  	nop
  19f930: ff ff 03 3c  	lui	$3, 0xffff
  19f934: 1b 00 40 04  	bltz	$2, 0x19f9a4 <.text+0x9f9a4>
  19f938: ff 29 63 34  	ori	$3, $3, 0x29ff
  19f93c: 2d 28 00 00  	move	$5, $zero
  19f940: 10 00 a4 27  	addiu	$4, $sp, 0x10
  19f944: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  19f948: 00 02 06 24  	addiu	$6, $zero, 0x200
  19f94c: 03 00 20 12  	beqz	$17, 0x19f95c <.text+0x9f95c>
  19f950: 10 00 b2 af  	sw	$18, 0x10($sp)
  19f954: 1b 00 00 56  	bnezl	$16, 0x19f9c4 <.text+0x9f9c4>
  19f958: fd 00 03 2a  	slti	$3, $16, 0xfd
  19f95c: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19f960: 45 00 04 3c  	lui	$4, 0x45
  19f964: 06 00 05 24  	addiu	$5, $zero, 0x6
  19f968: f0 0b 84 24  	addiu	$4, $4, 0xbf0
  19f96c: 2d 30 00 00  	move	$6, $zero
  19f970: 00 02 08 24  	addiu	$8, $zero, 0x200
  19f974: 2d 48 e0 00  	move	$9, $7
  19f978: 08 00 0a 24  	addiu	$10, $zero, 0x8
  19f97c: 2d 58 00 00  	move	$11, $zero
  19f980: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  19f984: 00 00 a0 af  	sw	$zero, 0x0($sp)
  19f988: ff ff 03 3c  	lui	$3, 0xffff
  19f98c: 05 00 40 04  	bltz	$2, 0x19f9a4 <.text+0x9f9a4>
  19f990: ed 29 63 34  	ori	$3, $3, 0x29ed
  19f994: 02 00 60 12  	beqz	$19, 0x19f9a0 <.text+0x9f9a0>
  19f998: 14 00 a2 8f  	lw	$2, 0x14($sp)
  19f99c: 00 00 62 ae  	sw	$2, 0x0($19)
  19f9a0: 10 00 a3 8f  	lw	$3, 0x10($sp)
  19f9a4: 50 02 bf df  	ld	$ra, 0x250($sp)
  19f9a8: 2d 10 60 00  	move	$2, $3
  19f9ac: 40 02 b3 df  	ld	$19, 0x240($sp)
  19f9b0: 30 02 b2 df  	ld	$18, 0x230($sp)
  19f9b4: 20 02 b1 df  	ld	$17, 0x220($sp)
  19f9b8: 10 02 b0 df  	ld	$16, 0x210($sp)
  19f9bc: 08 00 e0 03  	jr	$ra
  19f9c0: 60 02 bd 27  	addiu	$sp, $sp, 0x260
  19f9c4: fc 00 02 24  	addiu	$2, $zero, 0xfc
  19f9c8: 0b 10 03 02  	movn	$2, $16, $3
  19f9cc: 14 01 a4 27  	addiu	$4, $sp, 0x114
  19f9d0: 2d 28 20 02  	move	$5, $17
  19f9d4: 2d 30 40 00  	move	$6, $2
  19f9d8: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  19f9dc: 14 00 a2 af  	sw	$2, 0x14($sp)
  19f9e0: df ff 00 10  	b	0x19f960 <.text+0x9f960>
  19f9e4: 10 00 a7 27  	addiu	$7, $sp, 0x10
  19f9e8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19f9ec: 42 00 02 3c  	lui	$2, 0x42
  19f9f0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19f9f4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19f9f8: b4 5a 42 8c  	lw	$2, 0x5ab4($2)
  19f9fc: 06 00 40 10  	beqz	$2, 0x19fa18 <.text+0x9fa18>
  19fa00: 2d 20 00 00  	move	$4, $zero
  19fa04: 10 00 bf df  	ld	$ra, 0x10($sp)
  19fa08: 2d 10 80 00  	move	$2, $4
  19fa0c: 00 00 b0 df  	ld	$16, 0x0($sp)
  19fa10: 08 00 e0 03  	jr	$ra
  19fa14: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19fa18: 03 73 06 0c  	jal	0x19cc0c <.text+0x9cc0c>
  19fa1c: 2d 20 00 00  	move	$4, $zero
  19fa20: 45 00 02 3c  	lui	$2, 0x45
  19fa24: 00 80 05 3c  	lui	$5, 0x8000
  19fa28: c0 0b 50 24  	addiu	$16, $2, 0xbc0
  19fa2c: 03 00 a5 34  	ori	$5, $5, 0x3
  19fa30: 2d 20 00 02  	move	$4, $16
  19fa34: a2 71 06 0c  	jal	0x19c688 <.text+0x9c688>
  19fa38: 2d 30 00 00  	move	$6, $zero
  19fa3c: ff ff 04 3c  	lui	$4, 0xffff
  19fa40: f0 ff 40 04  	bltz	$2, 0x19fa04 <.text+0x9fa04>
  19fa44: ee 29 84 34  	ori	$4, $4, 0x29ee
  19fa48: 24 00 02 8e  	lw	$2, 0x24($16)
  19fa4c: 0f 00 03 3c  	lui	$3, 0xf
  19fa50: 0f 00 40 14  	bnez	$2, 0x19fa90 <.text+0x9fa90>
  19fa54: ff ff 63 34  	ori	$3, $3, 0xffff
		...
  19fa6c: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  19fa70: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
		...
  19fa80: f5 ff 62 14  	bne	$3, $2, 0x19fa58 <.text+0x9fa58>
  19fa84: 45 00 02 3c  	lui	$2, 0x45
  19fa88: e7 ff 00 10  	b	0x19fa28 <.text+0x9fa28>
  19fa8c: 00 80 05 3c  	lui	$5, 0x8000
  19fa90: 42 00 03 3c  	lui	$3, 0x42
  19fa94: 2d 20 00 00  	move	$4, $zero
  19fa98: b4 5a 62 8c  	lw	$2, 0x5ab4($3)
  19fa9c: 01 00 42 34  	ori	$2, $2, 0x1
  19faa0: d8 ff 00 10  	b	0x19fa04 <.text+0x9fa04>
  19faa4: b4 5a 62 ac  	sw	$2, 0x5ab4($3)

# SifLoadFileInit: 0x0019fd20..0x0019fddb
  19fd20: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19fd24: 42 00 02 3c  	lui	$2, 0x42
  19fd28: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19fd2c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19fd30: b8 5a 42 8c  	lw	$2, 0x5ab8($2)
  19fd34: 06 00 40 10  	beqz	$2, 0x19fd50 <.text+0x9fd50>
  19fd38: 2d 18 00 00  	move	$3, $zero
  19fd3c: 10 00 bf df  	ld	$ra, 0x10($sp)
  19fd40: 2d 10 60 00  	move	$2, $3
  19fd44: 00 00 b0 df  	ld	$16, 0x0($sp)
  19fd48: 08 00 e0 03  	jr	$ra
  19fd4c: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19fd50: 03 73 06 0c  	jal	0x19cc0c <.text+0x9cc0c>
  19fd54: 2d 20 00 00  	move	$4, $zero
  19fd58: 45 00 02 3c  	lui	$2, 0x45
  19fd5c: 00 80 05 3c  	lui	$5, 0x8000
  19fd60: f0 0b 50 24  	addiu	$16, $2, 0xbf0
  19fd64: 06 00 a5 34  	ori	$5, $5, 0x6
  19fd68: 2d 20 00 02  	move	$4, $16
  19fd6c: a2 71 06 0c  	jal	0x19c688 <.text+0x9c688>
  19fd70: 2d 30 00 00  	move	$6, $zero
  19fd74: ff ff 03 3c  	lui	$3, 0xffff
  19fd78: f0 ff 40 04  	bltz	$2, 0x19fd3c <.text+0x9fd3c>
  19fd7c: ee 29 63 34  	ori	$3, $3, 0x29ee
  19fd80: 24 00 02 8e  	lw	$2, 0x24($16)
  19fd84: 0f 00 03 3c  	lui	$3, 0xf
  19fd88: 0f 00 40 14  	bnez	$2, 0x19fdc8 <.text+0x9fdc8>
  19fd8c: ff ff 63 34  	ori	$3, $3, 0xffff
		...
  19fda4: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  19fda8: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
		...
  19fdb8: f5 ff 62 14  	bne	$3, $2, 0x19fd90 <.text+0x9fd90>
  19fdbc: 45 00 02 3c  	lui	$2, 0x45
  19fdc0: e7 ff 00 10  	b	0x19fd60 <.text+0x9fd60>
  19fdc4: 00 80 05 3c  	lui	$5, 0x8000
  19fdc8: 01 00 03 24  	addiu	$3, $zero, 0x1
  19fdcc: 42 00 02 3c  	lui	$2, 0x42
  19fdd0: b8 5a 43 ac  	sw	$3, 0x5ab8($2)
  19fdd4: d9 ff 00 10  	b	0x19fd3c <.text+0x9fd3c>
  19fdd8: 2d 18 00 00  	move	$3, $zero
