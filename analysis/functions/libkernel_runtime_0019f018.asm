# Focused R5900 extract: 0x0019f018..0x0019f138

  19f018: 00 60 03 40  	mfc0	$3, $12, 0x0
  19f01c: 01 00 02 3c  	lui	$2, 0x1
  19f020: 24 18 62 00  	and	$3, $3, $2
  19f024: 2d 20 00 00  	move	$4, $zero
  19f028: 0a 00 60 10  	beqz	$3, 0x19f054 <.text+0x9f054>
  19f02c: 2b 28 03 00  	sltu	$5, $zero, $3
  19f030: 39 00 00 42  	di	# R5900
  19f034: 0f 04 00 00  	sync 0x10
  19f038: 00 60 03 40  	mfc0	$3, $12, 0x0
  19f03c: 01 00 02 3c  	lui	$2, 0x1
  19f040: 24 18 62 00  	and	$3, $3, $2
  19f044: 00 00 00 00  	nop
  19f048: f9 ff 60 14  	bnez	$3, 0x19f030 <.text+0x9f030>
  19f04c: 00 00 00 00  	nop
  19f050: 2d 20 a0 00  	move	$4, $5
  19f054: 08 00 e0 03  	jr	$ra
  19f058: 2d 10 80 00  	move	$2, $4
  19f05c: 00 00 00 00  	nop
  19f060: 00 60 02 40  	mfc0	$2, $12, 0x0
  19f064: 01 00 03 3c  	lui	$3, 0x1
  19f068: 24 10 43 00  	and	$2, $2, $3
  19f06c: 38 00 00 42  	ei	# R5900
  19f070: 08 00 e0 03  	jr	$ra
  19f074: 2b 10 02 00  	sltu	$2, $zero, $2
  19f078: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  19f07c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19f080: 42 00 12 3c  	lui	$18, 0x42
  19f084: 80 5a 42 8e  	lw	$2, 0x5a80($18)
  19f088: 30 00 b3 ff  	sd	$19, 0x30($sp)
  19f08c: ff ff 13 24  	addiu	$19, $zero, -0x1 <.text+0xffffffffffefffff>
  19f090: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19f094: 2d 88 80 00  	move	$17, $4
  19f098: 40 00 bf ff  	sd	$ra, 0x40($sp)
  19f09c: 04 00 40 14  	bnez	$2, 0x19f0b0 <.text+0x9f0b0>
  19f0a0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19f0a4: 45 00 02 3c  	lui	$2, 0x45
  19f0a8: 18 0c 42 24  	addiu	$2, $2, 0xc18
  19f0ac: 80 5a 42 ae  	sw	$2, 0x5a80($18)
  19f0b0: 08 00 20 16  	bnez	$17, 0x19f0d4 <.text+0x9f0d4>
  19f0b4: 80 5a 42 8e  	lw	$2, 0x5a80($18)
  19f0b8: 40 00 bf df  	ld	$ra, 0x40($sp)
  19f0bc: 30 00 b3 df  	ld	$19, 0x30($sp)
  19f0c0: 20 00 b2 df  	ld	$18, 0x20($sp)
  19f0c4: 10 00 b1 df  	ld	$17, 0x10($sp)
  19f0c8: 00 00 b0 df  	ld	$16, 0x0($sp)
  19f0cc: 08 00 e0 03  	jr	$ra
  19f0d0: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  19f0d4: 00 60 10 40  	mfc0	$16, $12, 0x0
  19f0d8: 01 00 02 3c  	lui	$2, 0x1
  19f0dc: 24 80 02 02  	and	$16, $16, $2
  19f0e0: 11 00 00 16  	bnez	$16, 0x19f128 <.text+0x9f128>
  19f0e4: 00 00 00 00  	nop
  19f0e8: 80 5a 42 8e  	lw	$2, 0x5a80($18)
  19f0ec: 70 7d 06 0c  	jal	0x19f5c0 <.text+0x9f5c0>
  19f0f0: 21 88 51 00  	addu	$17, $2, $17
  19f0f4: 2b 10 51 00  	sltu	$2, $2, $17
  19f0f8: 03 00 40 14  	bnez	$2, 0x19f108 <.text+0x9f108>
  19f0fc: 00 00 00 00  	nop
  19f100: 80 5a 53 8e  	lw	$19, 0x5a80($18)
  19f104: 80 5a 51 ae  	sw	$17, 0x5a80($18)
  19f108: 03 00 00 16  	bnez	$16, 0x19f118 <.text+0x9f118>
  19f10c: 00 00 00 00  	nop
  19f110: e9 ff 00 10  	b	0x19f0b8 <.text+0x9f0b8>
  19f114: 2d 10 60 02  	move	$2, $19
  19f118: 18 7c 06 0c  	jal	0x19f060 <.text+0x9f060>
  19f11c: 00 00 00 00  	nop
  19f120: e5 ff 00 10  	b	0x19f0b8 <.text+0x9f0b8>
  19f124: 2d 10 60 02  	move	$2, $19
  19f128: 06 7c 06 0c  	jal	0x19f018 <.text+0x9f018>
  19f12c: 00 00 00 00  	nop
  19f130: ee ff 00 10  	b	0x19f0ec <.text+0x9f0ec>
  19f134: 80 5a 42 8e  	lw	$2, 0x5a80($18)
