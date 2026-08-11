  198a70: 00 5a 63 24  	addiu	$3, $3, 0x5a00
  198a74: 80 10 02 00  	sll	$2, $2, 0x2
  198a78: 21 10 43 00  	addu	$2, $2, $3
  198a7c: 08 00 e0 03  	jr	$ra
  198a80: 00 00 42 8c  	lw	$2, 0x0($2)
  198a84: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  198a88: 2d 20 a0 00  	move	$4, $5
  198a8c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  198a90: 92 79 06 0c  	jal	0x19e648 <.text+0x9e648>
  198a94: 2d 28 c0 00  	move	$5, $6
  198a98: 00 00 bf df  	ld	$ra, 0x0($sp)
  198a9c: 08 00 e0 03  	jr	$ra
  198aa0: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  198aa4: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  198aa8: 00 00 bf ff  	sd	$ra, 0x0($sp)
  198aac: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  198ab0: 2d 20 a0 00  	move	$4, $5
  198ab4: 00 00 bf df  	ld	$ra, 0x0($sp)
  198ab8: 08 00 e0 03  	jr	$ra
  198abc: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  198ac0: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  198ac4: 3a 14 04 00  	dsrl	$2, $4, 0x10
  198ac8: 30 00 b3 ff  	sd	$19, 0x30($sp)
  198acc: 2d 98 c0 00  	move	$19, $6
  198ad0: 20 00 b2 ff  	sd	$18, 0x20($sp)
  198ad4: 2d 90 a0 00  	move	$18, $5
  198ad8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  198adc: ff ff 51 30  	andi	$17, $2, 0xffff
  198ae0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  198ae4: 01 00 02 24  	addiu	$2, $zero, 0x1
  198ae8: 40 00 bf ff  	sd	$ra, 0x40($sp)
  198aec: 52 00 a0 10  	beqz	$5, 0x198c38 <.text+0x98c38>
  198af0: ff ff 90 30  	andi	$16, $4, 0xffff
  198af4: 4f 00 c0 10  	beqz	$6, 0x198c34 <.text+0x98c34>
  198af8: 38 14 11 00  	dsll	$2, $17, 0x10
  198afc: b1 15 62 2e  	sltiu	$2, $19, 0x15b1
  198b00: b0 15 0c 24  	addiu	$12, $zero, 0x15b0
  198b04: 0b 60 62 02  	movn	$12, $19, $2
  198b08: 10 00 82 29  	slti	$2, $12, 0x10
  198b0c: 35 00 40 14  	bnez	$2, 0x198be4 <.text+0x98be4>
  198b10: 23 98 6c 02  	subu	$19, $19, $12
  198b14: 00 00 42 92  	lbu	$2, 0x0($18)
  198b18: f0 ff 8c 25  	addiu	$12, $12, -0x10 <.text+0xffffffffffeffff0>
  198b1c: 01 00 43 92  	lbu	$3, 0x1($18)
  198b20: 10 00 8b 29  	slti	$11, $12, 0x10
  198b24: 2d 80 02 02  	daddu	$16, $16, $2
  198b28: 03 00 44 92  	lbu	$4, 0x3($18)
  198b2c: 02 00 42 92  	lbu	$2, 0x2($18)
  198b30: 2d 88 30 02  	daddu	$17, $17, $16
  198b34: 2d 80 03 02  	daddu	$16, $16, $3
  198b38: 05 00 45 92  	lbu	$5, 0x5($18)
  198b3c: 04 00 43 92  	lbu	$3, 0x4($18)
  198b40: 2d 88 30 02  	daddu	$17, $17, $16
  198b44: 2d 80 02 02  	daddu	$16, $16, $2
  198b48: 07 00 46 92  	lbu	$6, 0x7($18)
  198b4c: 2d 88 30 02  	daddu	$17, $17, $16
  198b50: 06 00 42 92  	lbu	$2, 0x6($18)
  198b54: 2d 80 04 02  	daddu	$16, $16, $4
  198b58: 09 00 47 92  	lbu	$7, 0x9($18)
  198b5c: 2d 88 30 02  	daddu	$17, $17, $16
  198b60: 08 00 44 92  	lbu	$4, 0x8($18)
  198b64: 2d 80 03 02  	daddu	$16, $16, $3
  198b68: 0b 00 48 92  	lbu	$8, 0xb($18)
  198b6c: 2d 88 30 02  	daddu	$17, $17, $16
  198b70: 0a 00 43 92  	lbu	$3, 0xa($18)
  198b74: 2d 80 05 02  	daddu	$16, $16, $5
  198b78: 0d 00 49 92  	lbu	$9, 0xd($18)
  198b7c: 2d 88 30 02  	daddu	$17, $17, $16
  198b80: 0c 00 45 92  	lbu	$5, 0xc($18)
  198b84: 2d 80 02 02  	daddu	$16, $16, $2
  198b88: 0f 00 4a 92  	lbu	$10, 0xf($18)
  198b8c: 2d 88 30 02  	daddu	$17, $17, $16
  198b90: 0e 00 42 92  	lbu	$2, 0xe($18)
  198b94: 2d 80 06 02  	daddu	$16, $16, $6
  198b98: 10 00 52 26  	addiu	$18, $18, 0x10
  198b9c: 2d 88 30 02  	daddu	$17, $17, $16
  198ba0: 2d 80 04 02  	daddu	$16, $16, $4
  198ba4: 2d 88 30 02  	daddu	$17, $17, $16
  198ba8: 2d 80 07 02  	daddu	$16, $16, $7
  198bac: 2d 88 30 02  	daddu	$17, $17, $16
  198bb0: 2d 80 03 02  	daddu	$16, $16, $3
  198bb4: 2d 88 30 02  	daddu	$17, $17, $16
  198bb8: 2d 80 08 02  	daddu	$16, $16, $8
  198bbc: 2d 88 30 02  	daddu	$17, $17, $16
  198bc0: 2d 80 05 02  	daddu	$16, $16, $5
  198bc4: 2d 88 30 02  	daddu	$17, $17, $16
  198bc8: 2d 80 09 02  	daddu	$16, $16, $9
  198bcc: 2d 88 30 02  	daddu	$17, $17, $16
  198bd0: 2d 80 02 02  	daddu	$16, $16, $2
  198bd4: 2d 88 30 02  	daddu	$17, $17, $16
  198bd8: 2d 80 0a 02  	daddu	$16, $16, $10
  198bdc: cd ff 60 11  	beqz	$11, 0x198b14 <.text+0x98b14>
  198be0: 2d 88 30 02  	daddu	$17, $17, $16
  198be4: 0a 00 80 11  	beqz	$12, 0x198c10 <.text+0x98c10>
  198be8: 2d 20 00 02  	move	$4, $16
  198bec: 00 00 42 92  	lbu	$2, 0x0($18)
  198bf0: ff ff 8c 25  	addiu	$12, $12, -0x1 <.text+0xffffffffffefffff>
  198bf4: 01 00 52 26  	addiu	$18, $18, 0x1
  198bf8: 2d 80 02 02  	daddu	$16, $16, $2
  198c04: f9 ff 80 15  	bnez	$12, 0x198bec <.text+0x98bec>
  198c08: 2d 88 30 02  	daddu	$17, $17, $16
  198c0c: 2d 20 00 02  	move	$4, $16
  198c10: 1e 8b 06 0c  	jal	0x1a2c78 <.text+0xa2c78>
  198c14: f1 ff 05 34  	ori	$5, $zero, 0xfff1
  198c18: f1 ff 05 34  	ori	$5, $zero, 0xfff1
  198c1c: 2d 20 20 02  	move	$4, $17
  198c20: 1e 8b 06 0c  	jal	0x1a2c78 <.text+0xa2c78>
  198c24: 2d 80 40 00  	move	$16, $2
  198c28: b4 ff 60 16  	bnez	$19, 0x198afc <.text+0x98afc>
  198c2c: 2d 88 40 00  	move	$17, $2
  198c30: 38 14 11 00  	dsll	$2, $17, 0x10
  198c34: 25 10 50 00  	or	$2, $2, $16
  198c38: 40 00 bf df  	ld	$ra, 0x40($sp)
  198c3c: 30 00 b3 df  	ld	$19, 0x30($sp)
  198c40: 20 00 b2 df  	ld	$18, 0x20($sp)
  198c44: 10 00 b1 df  	ld	$17, 0x10($sp)
  198c48: 00 00 b0 df  	ld	$16, 0x0($sp)
  198c4c: 08 00 e0 03  	jr	$ra
  198c50: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  198c54: 00 00 00 00  	nop
  198c58: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  198c5c: 02 00 05 3c  	lui	$5, 0x2
  198c60: 30 00 bf ff  	sd	$ra, 0x30($sp)
  198c64: 20 00 b0 ff  	sd	$16, 0x20($sp)
  198c68: 64 65 06 0c  	jal	0x199590 <.text+0x99590>
  198c6c: 2d 80 80 00  	move	$16, $4
