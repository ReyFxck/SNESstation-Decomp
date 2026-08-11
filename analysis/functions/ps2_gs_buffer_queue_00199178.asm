  199178: 58 00 83 8c  	lw	$3, 0x58($4)
  19917c: 64 00 82 8c  	lw	$2, 0x64($4)
  199180: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  199184: 2b 20 65 00  	sltu	$4, $3, $5
  199188: 0b 28 64 00  	movn	$5, $3, $4
  19918c: 08 00 e0 03  	jr	$ra
  199190: 18 10 45 00  	mult	$v0, $v0, $a1  # R5900 3-operand
  199194: 00 00 00 00  	nop
  199198: 08 00 e0 03  	jr	$ra
  19919c: 70 00 82 8c  	lw	$2, 0x70($4)
  1991a0: 08 00 e0 03  	jr	$ra
  1991a4: 50 00 82 8c  	lw	$2, 0x50($4)
  1991a8: 08 00 e0 03  	jr	$ra
  1991ac: 54 00 82 8c  	lw	$2, 0x54($4)
  1991b0: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1991b4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1991b8: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1991bc: 9a 64 06 0c  	jal	0x199268 <.text+0x99268>
  1991c0: 2d 80 80 00  	move	$16, $4
  1991c4: a4 64 06 0c  	jal	0x199290 <.text+0x99290>
  1991c8: 2d 20 00 02  	move	$4, $16
  1991cc: 82 64 06 0c  	jal	0x199208 <.text+0x99208>
  1991d0: 2d 20 00 02  	move	$4, $16
  1991d4: 00 00 b0 df  	ld	$16, 0x0($sp)
  1991d8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1991dc: 08 00 e0 03  	jr	$ra
  1991e0: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1991e4: 00 00 00 00  	nop
  1991e8: 5c 00 82 8c  	lw	$2, 0x5c($4)
  1991ec: 08 00 e0 03  	jr	$ra
  1991f0: 2b 10 02 00  	sltu	$2, $zero, $2
  1991f4: 00 00 00 00  	nop
  1991f8: 60 00 82 8c  	lw	$2, 0x60($4)
  1991fc: 08 00 e0 03  	jr	$ra
  199200: 2b 10 02 00  	sltu	$2, $zero, $2
  199204: 00 00 00 00  	nop
  199208: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19920c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199210: 10 00 bf ff  	sd	$ra, 0x10($sp)
  199214: 5c 00 82 8c  	lw	$2, 0x5c($4)
  199218: 0e 00 40 10  	beqz	$2, 0x199254 <.text+0x99254>
  19921c: 2d 80 80 00  	move	$16, $4
  199220: 54 00 83 8c  	lw	$3, 0x54($4)
  199224: 58 00 82 8c  	lw	$2, 0x58($4)
  199228: 01 00 63 24  	addiu	$3, $3, 0x1
  19922c: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  199230: 2b 10 43 00  	sltu	$2, $2, $3
  199234: 02 00 40 10  	beqz	$2, 0x199240 <.text+0x99240>
  199238: 54 00 83 ac  	sw	$3, 0x54($4)
  19923c: 54 00 80 ac  	sw	$zero, 0x54($4)
  199240: d8 64 06 0c  	jal	0x199360 <.text+0x99360>
  199244: 54 00 05 8e  	lw	$5, 0x54($16)
  199248: 5c 00 02 8e  	lw	$2, 0x5c($16)
  19924c: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  199250: 5c 00 02 ae  	sw	$2, 0x5c($16)
  199254: 10 00 bf df  	ld	$ra, 0x10($sp)
  199258: 00 00 b0 df  	ld	$16, 0x0($sp)
  19925c: 08 00 e0 03  	jr	$ra
  199260: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  199264: 00 00 00 00  	nop
  199268: 58 00 83 8c  	lw	$3, 0x58($4)
  19926c: 60 00 82 8c  	lw	$2, 0x60($4)
  199270: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  199274: 01 00 45 24  	addiu	$5, $2, 0x1
  199278: 2b 10 43 00  	sltu	$2, $2, $3
  19927c: 01 00 40 54  	bnezl	$2, 0x199284 <.text+0x99284>
  199280: 60 00 85 ac  	sw	$5, 0x60($4)
  199284: 08 00 e0 03  	jr	$ra
  199290: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  199294: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199298: 10 00 bf ff  	sd	$ra, 0x10($sp)
  19929c: 60 00 82 8c  	lw	$2, 0x60($4)
  1992a0: 11 00 40 10  	beqz	$2, 0x1992e8 <.text+0x992e8>
  1992a4: 2d 80 80 00  	move	$16, $4
  1992a8: 50 00 83 8c  	lw	$3, 0x50($4)
  1992ac: 58 00 82 8c  	lw	$2, 0x58($4)
  1992b0: 01 00 63 24  	addiu	$3, $3, 0x1
  1992b4: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  1992b8: 2b 10 43 00  	sltu	$2, $2, $3
  1992bc: 02 00 40 10  	beqz	$2, 0x1992c8 <.text+0x992c8>
  1992c0: 50 00 83 ac  	sw	$3, 0x50($4)
  1992c4: 50 00 80 ac  	sw	$zero, 0x50($4)
  1992c8: be 64 06 0c  	jal	0x1992f8 <.text+0x992f8>
  1992cc: 50 00 05 8e  	lw	$5, 0x50($16)
  1992d0: 60 00 03 8e  	lw	$3, 0x60($16)
  1992d4: 5c 00 02 8e  	lw	$2, 0x5c($16)
  1992d8: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  1992dc: 01 00 42 24  	addiu	$2, $2, 0x1
  1992e0: 60 00 03 ae  	sw	$3, 0x60($16)
  1992e4: 5c 00 02 ae  	sw	$2, 0x5c($16)
  1992e8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1992ec: 00 00 b0 df  	ld	$16, 0x0($sp)
  1992f0: 08 00 e0 03  	jr	$ra
  1992f4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
