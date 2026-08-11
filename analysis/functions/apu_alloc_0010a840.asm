
build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  10a840: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  10a844: 01 00 04 3c  	lui	$4, 0x1
  10a848: 34 00 02 3c  	lui	$2, 0x34
  10a84c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  10a850: 00 00 b0 ff  	sd	$16, 0x0($sp)
  10a854: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  10a858: 98 54 50 24  	addiu	$16, $2, 0x5498
  10a85c: 01 00 04 3c  	lui	$4, 0x1
  10a860: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  10a864: 04 00 02 ae  	sw	$2, 0x4($16)
  10a868: 04 00 04 3c  	lui	$4, 0x4
  10a86c: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  10a870: 20 00 02 ae  	sw	$2, 0x20($16)
  10a874: 2d 18 40 00  	move	$3, $2
  10a878: 04 00 02 8e  	lw	$2, 0x4($16)
  10a87c: 0b 00 40 10  	beqz	$2, 0x10a8ac <.text+0xa8ac>
  10a880: 24 00 03 ae  	sw	$3, 0x24($16)
  10a884: 20 00 02 8e  	lw	$2, 0x20($16)
  10a888: 08 00 40 10  	beqz	$2, 0x10a8ac <.text+0xa8ac>
  10a88c: 00 00 00 00  	nop
  10a890: 06 00 60 10  	beqz	$3, 0x10a8ac <.text+0xa8ac>
  10a894: 00 00 00 00  	nop
  10a898: 01 00 02 24  	addiu	$2, $zero, 0x1
  10a89c: 10 00 bf df  	ld	$ra, 0x10($sp)
  10a8a0: 00 00 b0 df  	ld	$16, 0x0($sp)
  10a8a4: 08 00 e0 03  	jr	$ra
  10a8a8: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  10a8ac: 2f 2a 04 0c  	jal	0x10a8bc <.text+0xa8bc>
  10a8b0: 00 00 00 00  	nop
  10a8b4: f9 ff 00 10  	b	0x10a89c <.text+0xa89c>
  10a8b8: 2d 10 00 00  	move	$2, $zero
  10a8bc: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  10a8c0: 34 00 02 3c  	lui	$2, 0x34
  10a8c4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  10a8c8: 98 54 50 24  	addiu	$16, $2, 0x5498
  10a8cc: 04 00 02 8e  	lw	$2, 0x4($16)
  10a8d0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  10a8d4: 13 00 40 14  	bnez	$2, 0x10a924 <.text+0xa924>
  10a8d8: 2d 20 40 00  	move	$4, $2
  10a8dc: 20 00 02 8e  	lw	$2, 0x20($16)
  10a8e0: 0c 00 40 14  	bnez	$2, 0x10a914 <.text+0xa914>
  10a8e4: 2d 20 40 00  	move	$4, $2
  10a8e8: 24 00 02 8e  	lw	$2, 0x24($16)
  10a8ec: 05 00 40 14  	bnez	$2, 0x10a904 <.text+0xa904>
  10a8f0: 2d 20 40 00  	move	$4, $2
  10a8f4: 10 00 bf df  	ld	$ra, 0x10($sp)
  10a8f8: 00 00 b0 df  	ld	$16, 0x0($sp)
  10a8fc: 08 00 e0 03  	jr	$ra
  10a900: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  10a904: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  10a908: 00 00 00 00  	nop
  10a90c: f9 ff 00 10  	b	0x10a8f4 <.text+0xa8f4>
  10a910: 24 00 00 ae  	sw	$zero, 0x24($16)
  10a914: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  10a918: 00 00 00 00  	nop
  10a91c: f2 ff 00 10  	b	0x10a8e8 <.text+0xa8e8>
  10a920: 20 00 00 ae  	sw	$zero, 0x20($16)
  10a924: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  10a928: 00 00 00 00  	nop
  10a92c: eb ff 00 10  	b	0x10a8dc <.text+0xa8dc>
  10a930: 04 00 00 ae  	sw	$zero, 0x4($16)
  10a934: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  10a938: 34 00 03 3c  	lui	$3, 0x34
  10a93c: 40 00 bf ff  	sd	$ra, 0x40($sp)
  10a940: e0 54 62 24  	addiu	$2, $3, 0x54e0
  10a944: 00 00 b0 ff  	sd	$16, 0x0($sp)
  10a948: 33 00 04 3c  	lui	$4, 0x33
  10a94c: 30 00 b3 ff  	sd	$19, 0x30($sp)
  10a950: 34 00 10 3c  	lui	$16, 0x34
  10a954: 20 00 b2 ff  	sd	$18, 0x20($sp)
  10a958: 98 54 10 26  	addiu	$16, $16, 0x5498
  10a95c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  10a960: 78 53 84 24  	addiu	$4, $4, 0x5378
  10a964: 2d 28 00 00  	move	$5, $zero
  10a968: 00 01 06 24  	addiu	$6, $zero, 0x100
  10a96c: 80 00 42 90  	lbu	$2, 0x80($2)
  10a970: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  10a974: e0 54 62 a0  	sb	$2, 0x54e0($3)
  10a978: 04 00 04 8e  	lw	$4, 0x4($16)
  10a97c: 2d 28 00 00  	move	$5, $zero
