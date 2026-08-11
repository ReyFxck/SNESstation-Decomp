  199838: 00 70 02 3c  	lui	$2, 0x7000
  19983c: 08 00 a0 fc  	sd	$zero, 0x8($5)
  199840: 08 00 e0 03  	jr	$ra
  199844: 00 00 a2 fc  	sd	$2, 0x0($5)
  199848: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19984c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  199850: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199854: 2d 80 80 00  	move	$16, $4
  199858: aa 66 06 0c  	jal	0x199aa8 <.text+0x99aa8>
  19985c: 24 00 85 8c  	lw	$5, 0x24($4)
  199860: 1c 00 05 8e  	lw	$5, 0x1c($16)
  199864: e0 66 06 0c  	jal	0x199b80 <.text+0x99b80>
  199868: 2d 20 00 02  	move	$4, $16
  19986c: 28 00 05 8e  	lw	$5, 0x28($16)
  199870: 2c 00 06 8e  	lw	$6, 0x2c($16)
  199874: 96 67 06 0c  	jal	0x199e58 <.text+0x99e58>
  199878: 2d 20 00 02  	move	$4, $16
  19987c: 3e 66 06 0c  	jal	0x1998f8 <.text+0x998f8>
  199880: 2d 20 00 02  	move	$4, $16
  199884: 00 00 b0 df  	ld	$16, 0x0($sp)
  199888: 10 00 bf df  	ld	$ra, 0x10($sp)
  19988c: 08 00 e0 03  	jr	$ra
  199890: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  199894: 00 00 00 00  	nop
  199898: 08 00 83 8c  	lw	$3, 0x8($4)
  19989c: 0c 00 82 8c  	lw	$2, 0xc($4)
  1998a0: 42 18 03 00  	srl	$3, $3, 0x1
  1998a4: 14 00 85 8c  	lw	$5, 0x14($4)
  1998a8: 21 10 43 00  	addu	$2, $2, $3
  1998ac: 08 00 e0 03  	jr	$ra
  1998b0: 23 10 45 00  	subu	$2, $2, $5
  1998b4: 00 00 00 00  	nop
  1998b8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1998bc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1998c0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1998c4: 26 66 06 0c  	jal	0x199898 <.text+0x99898>
  1998c8: 2d 80 80 00  	move	$16, $4
  1998cc: 90 00 42 2c  	sltiu	$2, $2, 0x90
  1998d0: 05 00 40 14  	bnez	$2, 0x1998e8 <.text+0x998e8>
  1998d4: 2d 20 00 02  	move	$4, $16
  1998d8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1998dc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1998e0: 08 00 e0 03  	jr	$ra
  1998e4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1998e8: 3e 66 06 0c  	jal	0x1998f8 <.text+0x998f8>
  1998ec: 00 00 00 00  	nop
  1998f0: fa ff 00 10  	b	0x1998dc <.text+0x998dc>
  1998f4: 10 00 bf df  	ld	$ra, 0x10($sp)
  1998f8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1998fc: 10 00 bf ff  	sd	$ra, 0x10($sp)
  199900: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199904: 90 6f 06 0c  	jal	0x19be40 <.text+0x9be40>
  199908: 2d 80 80 00  	move	$16, $4
  19990c: ac 73 06 0c  	jal	0x19ceb0 <.text+0x9ceb0>
  199910: 2d 20 00 00  	move	$4, $zero
  199914: 88 6f 06 0c  	jal	0x19be20 <.text+0x9be20>
  199918: 0c 00 04 8e  	lw	$4, 0xc($16)
  19991c: 0c 00 02 8e  	lw	$2, 0xc($16)
  199920: 00 00 03 8e  	lw	$3, 0x0($16)
  199924: 2d 20 00 02  	move	$4, $16
  199928: 0d 00 43 10  	beq	$2, $3, 0x199960 <.text+0x99960>
  19992c: 2d 28 60 00  	move	$5, $3
  199930: 0e 66 06 0c  	jal	0x199838 <.text+0x99838>
  199934: 00 00 00 00  	nop
  199938: 00 00 02 8e  	lw	$2, 0x0($16)
  19993c: 10 00 43 24  	addiu	$3, $2, 0x10
  199940: 10 00 02 ae  	sw	$2, 0x10($16)
  199944: 14 00 03 ae  	sw	$3, 0x14($16)
  199948: 0c 00 02 ae  	sw	$2, 0xc($16)
  19994c: 10 00 bf df  	ld	$ra, 0x10($sp)
  199950: 00 00 b0 df  	ld	$16, 0x0($sp)
  199954: 08 00 e0 03  	jr	$ra
  199958: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19995c: 00 00 00 00  	nop
  199960: 0e 66 06 0c  	jal	0x199838 <.text+0x99838>
  199964: 04 00 05 8e  	lw	$5, 0x4($16)
  199968: f4 ff 00 10  	b	0x19993c <.text+0x9993c>
  19996c: 04 00 02 8e  	lw	$2, 0x4($16)
