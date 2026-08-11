  18dc60: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  18dc64: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18dc68: 42 00 11 3c  	lui	$17, 0x42
  18dc6c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18dc70: 2d 90 80 00  	move	$18, $4
  18dc74: 30 00 bf ff  	sd	$ra, 0x30($sp)
  18dc78: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18dc7c: 50 48 24 8e  	lw	$4, 0x4850($17)
  18dc80: 0c 00 82 8c  	lw	$2, 0xc($4)
  18dc84: 2e 00 40 14  	bnez	$2, 0x18dd40 <.text+0x8dd40>
  18dc88: 42 00 02 3c  	lui	$2, 0x42
  18dc8c: 88 00 82 dc  	ld	$2, 0x88($4)
  18dc90: 00 40 10 24  	addiu	$16, $zero, 0x4000
  18dc94: 07 00 40 10  	beqz	$2, 0x18dcb4 <.text+0x8dcb4>
  18dc98: 2d 30 00 00  	move	$6, $zero
  18dc9c: 3c 18 02 00  	dsll32	$3, $2, 0x0
  18dca0: 3f 18 03 00  	dsra32	$3, $3, 0x0
  18dca4: 00 40 42 2c  	sltiu	$2, $2, 0x4000
  18dca8: 0b 80 62 00  	movn	$16, $3, $2
  18dcac: 08 00 00 56  	bnezl	$16, 0x18dcd0 <.text+0x8dcd0>
  18dcb0: 50 00 85 dc  	ld	$5, 0x50($4)
  18dcb4: 30 00 bf df  	ld	$ra, 0x30($sp)
  18dcb8: 2d 10 c0 00  	move	$2, $6
  18dcbc: 20 00 b2 df  	ld	$18, 0x20($sp)
  18dcc0: 10 00 b1 df  	ld	$17, 0x10($sp)
  18dcc4: 00 00 b0 df  	ld	$16, 0x0($sp)
  18dcc8: 08 00 e0 03  	jr	$ra
  18dccc: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  18dcd0: a8 00 82 dc  	ld	$2, 0xa8($4)
  18dcd4: 2d 28 a2 00  	daddu	$5, $5, $2
  18dcd8: 3c 28 05 00  	dsll32	$5, $5, 0x0
  18dcdc: 3f 28 05 00  	dsra32	$5, $5, 0x0
  18dce0: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  18dce4: 98 00 84 8c  	lw	$4, 0x98($4)
  18dce8: f2 ff 40 14  	bnez	$2, 0x18dcb4 <.text+0x8dcb4>
  18dcec: ff ff 06 24  	addiu	$6, $zero, -0x1 <.text+0xffffffffffefffff>
  18dcf0: 50 48 22 8e  	lw	$2, 0x4850($17)
  18dcf4: 2d 30 00 02  	move	$6, $16
  18dcf8: 00 00 45 8c  	lw	$5, 0x0($2)
  18dcfc: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18dd00: 98 00 44 8c  	lw	$4, 0x98($2)
  18dd04: eb ff 50 14  	bne	$2, $16, 0x18dcb4 <.text+0x8dcb4>
  18dd08: ff ff 06 24  	addiu	$6, $zero, -0x1 <.text+0xffffffffffefffff>
  18dd0c: 50 48 22 8e  	lw	$2, 0x4850($17)
  18dd10: 3c 18 10 00  	dsll32	$3, $16, 0x0
  18dd14: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  18dd18: 50 00 44 dc  	ld	$4, 0x50($2)
  18dd1c: 88 00 45 dc  	ld	$5, 0x88($2)
  18dd20: 00 00 46 8c  	lw	$6, 0x0($2)
  18dd24: 2d 20 83 00  	daddu	$4, $4, $3
  18dd28: 2f 28 a3 00  	dsubu	$5, $5, $3
  18dd2c: 0c 00 50 ac  	sw	$16, 0xc($2)
  18dd30: 50 00 44 fc  	sd	$4, 0x50($2)
  18dd34: 88 00 45 fc  	sd	$5, 0x88($2)
  18dd38: 08 00 46 ac  	sw	$6, 0x8($2)
  18dd3c: 42 00 02 3c  	lui	$2, 0x42
  18dd40: 08 00 06 24  	addiu	$6, $zero, 0x8
  18dd44: 50 48 44 8c  	lw	$4, 0x4850($2)
  18dd48: 08 00 82 8c  	lw	$2, 0x8($4)
  18dd4c: 0c 00 83 8c  	lw	$3, 0xc($4)
  18dd50: 00 00 45 90  	lbu	$5, 0x0($2)
  18dd54: 01 00 42 24  	addiu	$2, $2, 0x1
  18dd58: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  18dd5c: 08 00 82 ac  	sw	$2, 0x8($4)
  18dd60: 00 00 45 a6  	sh	$5, 0x0($18)
  18dd64: d3 ff 00 10  	b	0x18dcb4 <.text+0x8dcb4>
  18dd68: 0c 00 83 ac  	sw	$3, 0xc($4)
  18dd6c: 80 f9 bd 27  	addiu	$sp, $sp, -0x680 <.text+0xffffffffffeff980>
  18dd70: 30 06 b5 ff  	sd	$21, 0x630($sp)
  18dd74: 2d a8 80 00  	move	$21, $4
  18dd78: 70 05 a5 af  	sw	$5, 0x570($sp)
  18dd7c: 2d 20 a0 03  	move	$4, $sp
  18dd80: 74 05 a6 af  	sw	$6, 0x574($sp)
  18dd84: 2d 28 00 00  	move	$5, $zero
  18dd88: 44 00 06 24  	addiu	$6, $zero, 0x44
  18dd8c: 20 06 b4 ff  	sd	$20, 0x620($sp)
  18dd90: 00 06 b2 ff  	sd	$18, 0x600($sp)
  18dd94: 2d a0 40 01  	move	$20, $10
  18dd98: f0 05 b1 ff  	sd	$17, 0x5f0($sp)
  18dd9c: 2d 90 a0 02  	move	$18, $21
  18dda0: 70 06 bf ff  	sd	$ra, 0x670($sp)
  18dda4: 60 06 be ff  	sd	$fp, 0x660($sp)
  18dda8: 50 06 b7 ff  	sd	$23, 0x650($sp)
  18ddac: 40 06 b6 ff  	sd	$22, 0x640($sp)
  18ddb0: 10 06 b3 ff  	sd	$19, 0x610($sp)
  18ddb4: e0 05 b0 ff  	sd	$16, 0x5e0($sp)
  18ddb8: 78 05 a7 af  	sw	$7, 0x578($sp)
  18ddbc: 7c 05 a8 af  	sw	$8, 0x57c($sp)
  18ddc0: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18ddc4: 80 05 a9 af  	sw	$9, 0x580($sp)
  18ddc8: 70 05 b1 8f  	lw	$17, 0x570($sp)
  18ddcc: 00 00 43 8e  	lw	$3, 0x0($18)
  18ddd0: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18ddd4: 04 00 52 26  	addiu	$18, $18, 0x4
  18ddd8: 80 18 03 00  	sll	$3, $3, 0x2
  18dddc: 21 18 7d 00  	addu	$3, $3, $sp
  18dde0: 00 00 62 8c  	lw	$2, 0x0($3)
  18dde4: 01 00 42 24  	addiu	$2, $2, 0x1
  18dde8: f8 ff 20 16  	bnez	$17, 0x18ddcc <.text+0x8ddcc>
  18ddec: 00 00 62 ac  	sw	$2, 0x0($3)
  18ddf0: 00 00 a2 8f  	lw	$2, 0x0($sp)
  18ddf4: 70 05 a3 8f  	lw	$3, 0x570($sp)
  18ddf8: 34 01 43 10  	beq	$2, $3, 0x18e2cc <.text+0x8e2cc>
  18ddfc: 01 00 10 24  	addiu	$16, $zero, 0x1
  18de00: 00 00 93 8e  	lw	$19, 0x0($20)
  18de04: 04 00 a3 27  	addiu	$3, $sp, 0x4
  18de08: 00 00 62 8c  	lw	$2, 0x0($3)
  18de0c: 05 00 40 14  	bnez	$2, 0x18de24 <.text+0x8de24>
  18de10: 04 00 63 24  	addiu	$3, $3, 0x4
  18de14: 01 00 10 26  	addiu	$16, $16, 0x1
  18de18: 11 00 02 2e  	sltiu	$2, $16, 0x11
  18de1c: fb ff 40 54  	bnezl	$2, 0x18de0c <.text+0x8de0c>
  18de20: 00 00 62 8c  	lw	$2, 0x0($3)
  18de24: 2b 10 70 02  	sltu	$2, $19, $16
  18de28: 2d f0 00 02  	move	$fp, $16
  18de2c: 10 00 11 24  	addiu	$17, $zero, 0x10
  18de30: 40 00 a3 27  	addiu	$3, $sp, 0x40
  18de34: 0b 98 02 02  	movn	$19, $16, $2
  18de38: 00 00 62 8c  	lw	$2, 0x0($3)
  18de3c: 04 00 40 14  	bnez	$2, 0x18de50 <.text+0x8de50>
  18de40: fc ff 63 24  	addiu	$3, $3, -0x4 <.text+0xffffffffffeffffc>
  18de44: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18de48: fc ff 20 56  	bnezl	$17, 0x18de3c <.text+0x8de3c>
  18de4c: 00 00 62 8c  	lw	$2, 0x0($3)
  18de50: 2b 10 33 02  	sltu	$2, $17, $19
  18de54: 2b 18 11 02  	sltu	$3, $16, $17
  18de58: 0b 98 22 02  	movn	$19, $17, $2
  18de5c: 2d 40 20 02  	move	$8, $17
  18de60: 01 00 02 24  	addiu	$2, $zero, 0x1
  18de64: 00 00 93 ae  	sw	$19, 0x0($20)
  18de68: 0b 00 60 10  	beqz	$3, 0x18de98 <.text+0x8de98>
  18de6c: 04 30 02 02  	sllv	$6, $2, $16
  18de70: 80 10 10 00  	sll	$2, $16, 0x2
  18de74: 21 18 5d 00  	addu	$3, $2, $sp
  18de78: 00 00 62 8c  	lw	$2, 0x0($3)
  18de7c: 01 00 10 26  	addiu	$16, $16, 0x1
  18de80: 2b 20 11 02  	sltu	$4, $16, $17
  18de84: 23 30 c2 00  	subu	$6, $6, $2
  18de88: 0e 01 c0 04  	bltz	$6, 0x18e2c4 <.text+0x8e2c4>
  18de8c: 04 00 63 24  	addiu	$3, $3, 0x4
  18de90: f9 ff 80 14  	bnez	$4, 0x18de78 <.text+0x8de78>
  18de94: 40 30 06 00  	sll	$6, $6, 0x1
  18de98: 80 10 11 00  	sll	$2, $17, 0x2
  18de9c: 21 20 5d 00  	addu	$4, $2, $sp
  18dea0: 00 00 82 8c  	lw	$2, 0x0($4)
  18dea4: 23 30 c2 00  	subu	$6, $6, $2
  18dea8: d5 00 c0 04  	bltz	$6, 0x18e200 <.text+0x8e200>
  18deac: 02 00 03 24  	addiu	$3, $zero, 0x2
  18deb0: 21 10 46 00  	addu	$2, $2, $6
  18deb4: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18deb8: 00 00 82 ac  	sw	$2, 0x0($4)
  18debc: 2d 80 00 00  	move	$16, $zero
  18dec0: 24 05 a0 af  	sw	$zero, 0x524($sp)
  18dec4: 04 00 b2 27  	addiu	$18, $sp, 0x4
  18dec8: 09 00 20 12  	beqz	$17, 0x18def0 <.text+0x8def0>
  18decc: 28 05 a5 27  	addiu	$5, $sp, 0x528
  18ded0: 00 00 42 8e  	lw	$2, 0x0($18)
  18ded4: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  18ded8: 04 00 52 26  	addiu	$18, $18, 0x4
  18dedc: 21 80 02 02  	addu	$16, $16, $2
  18dee0: 00 00 b0 ac  	sw	$16, 0x0($5)
  18dee4: 00 00 00 00  	nop
  18dee8: f9 ff 20 16  	bnez	$17, 0x18ded0 <.text+0x8ded0>
  18deec: 04 00 a5 24  	addiu	$5, $5, 0x4
  18def0: 2d 90 a0 02  	move	$18, $21
  18def4: 2d 88 00 00  	move	$17, $zero
  18def8: 00 00 50 8e  	lw	$16, 0x0($18)
  18defc: 04 00 52 26  	addiu	$18, $18, 0x4
  18df00: 08 00 00 12  	beqz	$16, 0x18df24 <.text+0x8df24>
  18df04: 80 10 10 00  	sll	$2, $16, 0x2
  18df08: 21 10 5d 00  	addu	$2, $2, $sp
  18df0c: 20 05 43 8c  	lw	$3, 0x520($2)
  18df10: 80 20 03 00  	sll	$4, $3, 0x2
  18df14: 01 00 63 24  	addiu	$3, $3, 0x1
  18df18: 21 20 9d 00  	addu	$4, $4, $sp
  18df1c: 20 05 43 ac  	sw	$3, 0x520($2)
  18df20: a0 00 91 ac  	sw	$17, 0xa0($4)
  18df24: 70 05 ab 8f  	lw	$11, 0x570($sp)
  18df28: 01 00 31 26  	addiu	$17, $17, 0x1
  18df2c: 2b 10 2b 02  	sltu	$2, $17, $11
  18df30: f2 ff 40 54  	bnezl	$2, 0x18defc <.text+0x8defc>
  18df34: 00 00 50 8e  	lw	$16, 0x0($18)
  18df38: 2a 10 1e 01  	slt	$2, $8, $fp
  18df3c: a0 00 b2 27  	addiu	$18, $sp, 0xa0
  18df40: 20 05 a0 af  	sw	$zero, 0x520($sp)
  18df44: 2d 88 00 00  	move	$17, $zero
  18df48: 60 00 a0 af  	sw	$zero, 0x60($sp)
  18df4c: ff ff 09 24  	addiu	$9, $zero, -0x1 <.text+0xffffffffffefffff>
  18df50: 23 a0 13 00  	negu	$20, $19
  18df54: 2d 28 00 00  	move	$5, $zero
  18df58: a4 00 40 14  	bnez	$2, 0x18e1ec <.text+0x8e1ec>
  18df5c: 2d 38 00 00  	move	$7, $zero
  18df60: 80 60 1e 00  	sll	$12, $fp, 0x2
  18df64: 21 10 9d 01  	addu	$2, $12, $sp
  18df68: 84 05 ac af  	sw	$12, 0x584($sp)
  18df6c: 00 00 57 8c  	lw	$23, 0x0($2)
  18df70: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18df74: ff ff f7 26  	addiu	$23, $23, -0x1 <.text+0xffffffffffefffff>
  18df78: 98 00 e2 12  	beq	$23, $2, 0x18e1dc <.text+0x8e1dc>
  18df7c: 80 10 09 00  	sll	$2, $9, 0x2
  18df80: 21 50 5d 00  	addu	$10, $2, $sp
  18df84: 21 a8 93 02  	addu	$21, $20, $19
  18df88: 2a 10 be 02  	slt	$2, $21, $fp
  18df8c: 51 00 40 10  	beqz	$2, 0x18e0d4 <.text+0x8e0d4>
  18df90: 80 10 09 00  	sll	$2, $9, 0x2
  18df94: 21 b0 5d 00  	addu	$22, $2, $sp
  18df98: 01 00 02 24  	addiu	$2, $zero, 0x1
  18df9c: 23 80 d5 03  	subu	$16, $fp, $21
  18dfa0: 23 38 15 01  	subu	$7, $8, $21
  18dfa4: 04 20 02 02  	sllv	$4, $2, $16
  18dfa8: 01 00 e3 26  	addiu	$3, $23, 0x1
  18dfac: 2b 10 67 02  	sltu	$2, $19, $7
  18dfb0: 2b 18 64 00  	sltu	$3, $3, $4
  18dfb4: 2d a0 a0 02  	move	$20, $21
  18dfb8: 0b 38 62 02  	movn	$7, $19, $2
  18dfbc: 04 00 d6 26  	addiu	$22, $22, 0x4
  18dfc0: 04 00 4a 25  	addiu	$10, $10, 0x4
  18dfc4: 12 00 60 10  	beqz	$3, 0x18e010 <.text+0x8e010>
  18dfc8: 01 00 29 25  	addiu	$9, $9, 0x1
  18dfcc: 01 00 10 26  	addiu	$16, $16, 0x1
  18dfd0: 84 05 ad 8f  	lw	$13, 0x584($sp)
  18dfd4: 23 10 97 00  	subu	$2, $4, $23
  18dfd8: 2b 18 07 02  	sltu	$3, $16, $7
  18dfdc: ff ff 44 24  	addiu	$4, $2, -0x1 <.text+0xffffffffffefffff>
  18dfe0: 0b 00 60 10  	beqz	$3, 0x18e010 <.text+0x8e010>
  18dfe4: 21 28 ad 03  	addu	$5, $sp, $13
  18dfe8: 04 00 a5 24  	addiu	$5, $5, 0x4
  18dfec: 40 20 04 00  	sll	$4, $4, 0x1
  18dff0: 00 00 a2 8c  	lw	$2, 0x0($5)
  18dff4: 2b 18 44 00  	sltu	$3, $2, $4
  18dff8: 05 00 60 10  	beqz	$3, 0x18e010 <.text+0x8e010>
  18dffc: 23 20 82 00  	subu	$4, $4, $2
  18e000: 01 00 10 26  	addiu	$16, $16, 0x1
  18e004: 2b 10 07 02  	sltu	$2, $16, $7
  18e008: f8 ff 40 14  	bnez	$2, 0x18dfec <.text+0x8dfec>
  18e00c: 04 00 a5 24  	addiu	$5, $5, 0x4
  18e010: 01 00 02 24  	addiu	$2, $zero, 0x1
  18e014: 90 05 a6 7f  	<unknown>
  18e018: 04 38 02 02  	sllv	$7, $2, $16
  18e01c: b0 05 a8 7f  	<unknown>
  18e020: c0 20 07 00  	sll	$4, $7, 0x3
  18e024: a0 05 a7 7f  	<unknown>
  18e028: 08 00 84 24  	addiu	$4, $4, 0x8
  18e02c: c0 05 a9 7f  	ext	$9, $sp, 0x17, 0x1
  18e030: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  18e034: d0 05 aa 7f  	<unknown>
  18e038: 2d 28 40 00  	move	$5, $2
  18e03c: 90 05 a6 7b  	<unknown>
  18e040: a0 05 a7 7b  	<unknown>
  18e044: b0 05 a8 7b  	<unknown>
  18e048: c0 05 a9 7b  	<unknown>
  18e04c: 95 00 40 10  	beqz	$2, 0x18e2a4 <.text+0x8e2a4>
  18e050: d0 05 aa 7b  	<unknown>
  18e054: 42 00 04 3c  	lui	$4, 0x42
  18e058: 08 00 43 24  	addiu	$3, $2, 0x8
  18e05c: 18 6e 82 8c  	lw	$2, 0x6e18($4)
  18e060: 04 00 ab 24  	addiu	$11, $5, 0x4
  18e064: 80 05 ae 8f  	lw	$14, 0x580($sp)
  18e068: 21 10 47 00  	addu	$2, $2, $7
  18e06c: 80 05 ab af  	sw	$11, 0x580($sp)
  18e070: 01 00 42 24  	addiu	$2, $2, 0x1
  18e074: 00 00 c3 ad  	sw	$3, 0x0($14)
  18e078: 18 6e 82 ac  	sw	$2, 0x6e18($4)
  18e07c: 04 00 a0 ac  	sw	$zero, 0x4($5)
  18e080: 2d 28 60 00  	move	$5, $3
  18e084: 0f 00 20 11  	beqz	$9, 0x18e0c4 <.text+0x8e0c4>
  18e088: 60 00 c3 ae  	sw	$3, 0x60($22)
  18e08c: 23 10 b3 02  	subu	$2, $21, $19
  18e090: 10 00 03 26  	addiu	$3, $16, 0x10
  18e094: 5c 00 c4 8e  	lw	$4, 0x5c($22)
  18e098: 06 80 51 00  	srlv	$16, $17, $2
  18e09c: 50 00 a3 a3  	sb	$3, 0x50($sp)
  18e0a0: c0 10 10 00  	sll	$2, $16, 0x3
  18e0a4: 51 00 b3 a3  	sb	$19, 0x51($sp)
  18e0a8: 21 10 44 00  	addu	$2, $2, $4
  18e0ac: 54 00 a5 af  	sw	$5, 0x54($sp)
  18e0b0: 20 05 d1 ae  	sw	$17, 0x520($22)
  18e0b4: 57 00 ac 6b  	ldl	$12, 0x57($sp)
  18e0b8: 50 00 ac 6f  	ldr	$12, 0x50($sp)
  18e0bc: 07 00 4c b0  	sdl	$12, 0x7($2)
  18e0c0: 00 00 4c b4  	sdr	$12, 0x0($2)
  18e0c4: 21 a8 b3 02  	addu	$21, $21, $19
  18e0c8: 2a 10 be 02  	slt	$2, $21, $fp
  18e0cc: b3 ff 40 14  	bnez	$2, 0x18df9c <.text+0x8df9c>
  18e0d0: 01 00 02 24  	addiu	$2, $zero, 0x1
  18e0d4: 70 05 a4 8f  	lw	$4, 0x570($sp)
  18e0d8: a0 00 ab 27  	addiu	$11, $sp, 0xa0
  18e0dc: 80 10 04 00  	sll	$2, $4, 0x2
  18e0e0: 23 20 d4 03  	subu	$4, $fp, $20
  18e0e4: 21 10 62 01  	addu	$2, $11, $2
  18e0e8: 2b 10 42 02  	sltu	$2, $18, $2
  18e0ec: 51 00 40 14  	bnez	$2, 0x18e234 <.text+0x8e234>
  18e0f0: 51 00 a4 a3  	sb	$4, 0x51($sp)
  18e0f4: 63 00 02 24  	addiu	$2, $zero, 0x63
  18e0f8: 50 00 a2 a3  	sb	$2, 0x50($sp)
  18e0fc: 06 80 91 02  	srlv	$16, $17, $20
  18e100: 01 00 02 24  	addiu	$2, $zero, 0x1
  18e104: 2b 18 07 02  	sltu	$3, $16, $7
  18e108: 0d 00 60 10  	beqz	$3, 0x18e140 <.text+0x8e140>
  18e10c: 04 20 82 00  	sllv	$4, $2, $4
  18e110: c0 10 10 00  	sll	$2, $16, 0x3
  18e114: 21 80 04 02  	addu	$16, $16, $4
  18e118: 21 10 45 00  	addu	$2, $2, $5
  18e11c: 2b 18 07 02  	sltu	$3, $16, $7
  18e120: 57 00 ab 6b  	ldl	$11, 0x57($sp)
  18e124: 50 00 ab 6f  	ldr	$11, 0x50($sp)
  18e128: 07 00 4b b0  	sdl	$11, 0x7($2)
  18e12c: 00 00 4b b4  	sdr	$11, 0x0($2)
  18e138: f6 ff 60 14  	bnez	$3, 0x18e114 <.text+0x8e114>
  18e13c: c0 10 10 00  	sll	$2, $16, 0x3
  18e140: ff ff c2 27  	addiu	$2, $fp, -0x1 <.text+0xffffffffffefffff>
  18e144: 01 00 03 24  	addiu	$3, $zero, 0x1
  18e148: 04 80 43 00  	sllv	$16, $3, $2
  18e14c: 24 10 30 02  	and	$2, $17, $16
  18e150: 0b 00 40 10  	beqz	$2, 0x18e180 <.text+0x8e180>
  18e154: 01 00 02 24  	addiu	$2, $zero, 0x1
  18e158: 26 88 30 02  	xor	$17, $17, $16
  18e15c: 42 80 10 00  	srl	$16, $16, 0x1
  18e160: 24 10 30 02  	and	$2, $17, $16
  18e174: f9 ff 40 54  	bnezl	$2, 0x18e15c <.text+0x8e15c>
  18e178: 26 88 30 02  	xor	$17, $17, $16
  18e17c: 01 00 02 24  	addiu	$2, $zero, 0x1
  18e180: 26 88 30 02  	xor	$17, $17, $16
  18e184: 04 10 82 02  	sllv	$2, $2, $20
  18e188: 20 05 43 8d  	lw	$3, 0x520($10)
  18e18c: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  18e190: 24 10 22 02  	and	$2, $17, $2
  18e194: 0d 00 43 10  	beq	$2, $3, 0x18e1cc <.text+0x8e1cc>
  18e198: 80 10 09 00  	sll	$2, $9, 0x2
  18e19c: 21 10 5d 00  	addu	$2, $2, $sp
  18e1a0: 20 05 44 24  	addiu	$4, $2, 0x520
  18e1a4: 23 a0 93 02  	subu	$20, $20, $19
  18e1a8: 01 00 02 24  	addiu	$2, $zero, 0x1
  18e1ac: 04 10 82 02  	sllv	$2, $2, $20
  18e1b0: fc ff 84 24  	addiu	$4, $4, -0x4 <.text+0xffffffffffeffffc>
  18e1b4: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  18e1b8: 00 00 83 8c  	lw	$3, 0x0($4)
  18e1bc: 24 10 22 02  	and	$2, $17, $2
  18e1c0: fc ff 4a 25  	addiu	$10, $10, -0x4 <.text+0xffffffffffeffffc>
  18e1c4: f7 ff 43 14  	bne	$2, $3, 0x18e1a4 <.text+0x8e1a4>
  18e1c8: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18e1cc: ff ff f7 26  	addiu	$23, $23, -0x1 <.text+0xffffffffffefffff>
  18e1d0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  18e1d4: 6c ff e2 56  	bnel	$23, $2, 0x18df88 <.text+0x8df88>
  18e1d8: 21 a8 93 02  	addu	$21, $20, $19
  18e1dc: 01 00 de 27  	addiu	$fp, $fp, 0x1
  18e1e0: 2a 10 1e 01  	slt	$2, $8, $fp
  18e1e4: 5f ff 40 10  	beqz	$2, 0x18df64 <.text+0x8df64>
  18e1e8: 80 60 1e 00  	sll	$12, $fp, 0x2
  18e1ec: 03 00 c0 10  	beqz	$6, 0x18e1fc <.text+0x8e1fc>
  18e1f0: 2d 10 00 00  	move	$2, $zero
  18e1f4: 01 00 02 39  	xori	$2, $8, 0x1
  18e1f8: 2b 10 02 00  	sltu	$2, $zero, $2
  18e1fc: 2d 18 40 00  	move	$3, $2
  18e200: 70 06 bf df  	ld	$ra, 0x670($sp)
  18e204: 2d 10 60 00  	move	$2, $3
  18e208: 60 06 be df  	ld	$fp, 0x660($sp)
  18e20c: 50 06 b7 df  	ld	$23, 0x650($sp)
  18e210: 40 06 b6 df  	ld	$22, 0x640($sp)
  18e214: 30 06 b5 df  	ld	$21, 0x630($sp)
  18e218: 20 06 b4 df  	ld	$20, 0x620($sp)
  18e21c: 10 06 b3 df  	ld	$19, 0x610($sp)
  18e220: 00 06 b2 df  	ld	$18, 0x600($sp)
  18e224: f0 05 b1 df  	ld	$17, 0x5f0($sp)
  18e228: e0 05 b0 df  	ld	$16, 0x5e0($sp)
  18e22c: 08 00 e0 03  	jr	$ra
  18e230: 80 06 bd 27  	addiu	$sp, $sp, 0x680
  18e234: 00 00 43 8e  	lw	$3, 0x0($18)
  18e238: 74 05 ac 8f  	lw	$12, 0x574($sp)
  18e23c: 2b 10 6c 00  	sltu	$2, $3, $12
  18e240: 0a 00 40 10  	beqz	$2, 0x18e26c <.text+0x8e26c>
  18e244: 74 05 ad 8f  	lw	$13, 0x574($sp)
  18e248: 00 01 62 2c  	sltiu	$2, $3, 0x100
  18e24c: 10 00 03 24  	addiu	$3, $zero, 0x10
  18e250: 01 00 42 2c  	sltiu	$2, $2, 0x1
  18e254: 23 18 62 00  	subu	$3, $3, $2
  18e258: 50 00 a3 a3  	sb	$3, 0x50($sp)
  18e25c: 00 00 42 96  	lhu	$2, 0x0($18)
  18e260: 04 00 52 26  	addiu	$18, $18, 0x4
  18e264: a5 ff 00 10  	b	0x18e0fc <.text+0x8e0fc>
  18e268: 54 00 a2 a7  	sh	$2, 0x54($sp)
  18e26c: 7c 05 ae 8f  	lw	$14, 0x57c($sp)
  18e270: 23 10 6d 00  	subu	$2, $3, $13
  18e274: 78 05 a3 8f  	lw	$3, 0x578($sp)
  18e278: 40 10 02 00  	sll	$2, $2, 0x1
  18e27c: 21 10 4e 00  	addu	$2, $2, $14
  18e280: 00 00 42 90  	lbu	$2, 0x0($2)
  18e284: 50 00 a2 a3  	sb	$2, 0x50($sp)
  18e288: 00 00 42 8e  	lw	$2, 0x0($18)
  18e28c: 04 00 52 26  	addiu	$18, $18, 0x4
  18e290: 23 10 4d 00  	subu	$2, $2, $13
  18e294: 40 10 02 00  	sll	$2, $2, 0x1
  18e298: 21 10 43 00  	addu	$2, $2, $3
  18e29c: f1 ff 00 10  	b	0x18e264 <.text+0x8e264>
  18e2a0: 00 00 42 94  	lhu	$2, 0x0($2)
  18e2a4: 03 00 20 15  	bnez	$9, 0x18e2b4 <.text+0x8e2b4>
  18e2a8: 00 00 00 00  	nop
  18e2ac: d4 ff 00 10  	b	0x18e200 <.text+0x8e200>
  18e2b0: 03 00 03 24  	addiu	$3, $zero, 0x3
  18e2b4: b8 38 06 0c  	jal	0x18e2e0 <.text+0x8e2e0>
  18e2b8: 60 00 a4 8f  	lw	$4, 0x60($sp)
  18e2bc: d0 ff 00 10  	b	0x18e200 <.text+0x8e200>
  18e2c0: 03 00 03 24  	addiu	$3, $zero, 0x3
  18e2c4: ce ff 00 10  	b	0x18e200 <.text+0x8e200>
  18e2c8: 02 00 03 24  	addiu	$3, $zero, 0x2
  18e2cc: 80 05 a4 8f  	lw	$4, 0x580($sp)
  18e2d0: 2d 18 00 00  	move	$3, $zero
  18e2d4: 00 00 80 ae  	sw	$zero, 0x0($20)
  18e2d8: c9 ff 00 10  	b	0x18e200 <.text+0x8e200>
  18e2dc: 00 00 80 ac  	sw	$zero, 0x0($4)
  18e2e0: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  18e2e4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  18e2e8: 06 00 80 10  	beqz	$4, 0x18e304 <.text+0x8e304>
  18e2ec: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18e2f0: f8 ff 84 24  	addiu	$4, $4, -0x8 <.text+0xffffffffffeffff8>
  18e2f4: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  18e2f8: 04 00 90 8c  	lw	$16, 0x4($4)
  18e2fc: fc ff 00 16  	bnez	$16, 0x18e2f0 <.text+0x8e2f0>
  18e300: 2d 20 00 02  	move	$4, $16
  18e304: 10 00 bf df  	ld	$ra, 0x10($sp)
  18e308: 2d 10 00 00  	move	$2, $zero
  18e30c: 00 00 b0 df  	ld	$16, 0x0($sp)
  18e310: 08 00 e0 03  	jr	$ra
  18e314: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  18e318: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  18e31c: 2d 30 80 00  	move	$6, $4
  18e320: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18e324: 45 00 05 3c  	lui	$5, 0x45
  18e328: 42 00 12 3c  	lui	$18, 0x42
  18e32c: 30 00 bf ff  	sd	$ra, 0x30($sp)
  18e330: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18e334: 00 82 a5 24  	addiu	$5, $5, -0x7e00 <.text+0xffffffffffef8200>
  18e338: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18e33c: 2d 88 80 00  	move	$17, $4
  18e340: 50 48 42 8e  	lw	$2, 0x4850($18)
  18e344: 28 71 06 0c  	jal	0x19c4a0 <.text+0x9c4a0>
  18e348: 18 00 44 8c  	lw	$4, 0x18($2)
  18e34c: 50 48 50 8e  	lw	$16, 0x4850($18)
  18e350: 2d 30 20 02  	move	$6, $17
  18e354: 78 00 04 de  	ld	$4, 0x78($16)
  18e358: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  18e35c: 18 00 05 8e  	lw	$5, 0x18($16)
  18e360: 30 00 bf df  	ld	$ra, 0x30($sp)
  18e364: 50 48 45 8e  	lw	$5, 0x4850($18)
  18e368: 3c 30 11 00  	dsll32	$6, $17, 0x0
  18e36c: 78 00 02 fe  	sd	$2, 0x78($16)
  18e370: 3e 30 06 00  	dsrl32	$6, $6, 0x0
  18e374: 18 00 a2 8c  	lw	$2, 0x18($5)
  18e378: 1c 00 a3 8c  	lw	$3, 0x1c($5)
  18e37c: 20 00 a4 dc  	ld	$4, 0x20($5)
  18e380: 21 10 51 00  	addu	$2, $2, $17
  18e384: 23 18 71 00  	subu	$3, $3, $17
  18e388: 20 00 b2 df  	ld	$18, 0x20($sp)
  18e38c: 10 00 b1 df  	ld	$17, 0x10($sp)
  18e390: 2d 20 86 00  	daddu	$4, $4, $6
  18e394: 00 00 b0 df  	ld	$16, 0x0($sp)
  18e398: 18 00 a2 ac  	sw	$2, 0x18($5)
  18e39c: 1c 00 a3 ac  	sw	$3, 0x1c($5)
  18e3a0: 20 00 a4 fc  	sd	$4, 0x20($5)
  18e3a4: 08 00 e0 03  	jr	$ra
  18e3a8: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  18e3ac: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  18e3b0: 2d 30 80 00  	move	$6, $4
  18e3b4: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18e3b8: 45 00 05 3c  	lui	$5, 0x45
  18e3bc: 42 00 12 3c  	lui	$18, 0x42
  18e3c0: 30 00 bf ff  	sd	$ra, 0x30($sp)
  18e3c4: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18e3c8: 06 e2 a5 24  	addiu	$5, $5, -0x1dfa <.text+0xffffffffffefe206>
  18e3cc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18e3d0: 2d 88 80 00  	move	$17, $4
  18e3d4: 50 48 42 8e  	lw	$2, 0x4850($18)
  18e3d8: 28 71 06 0c  	jal	0x19c4a0 <.text+0x9c4a0>
  18e3dc: 18 00 44 8c  	lw	$4, 0x18($2)
  18e3e0: 50 48 50 8e  	lw	$16, 0x4850($18)
  18e3e4: 2d 30 20 02  	move	$6, $17
  18e3e8: 78 00 04 de  	ld	$4, 0x78($16)
  18e3ec: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  18e3f0: 18 00 05 8e  	lw	$5, 0x18($16)
  18e3f4: 30 00 bf df  	ld	$ra, 0x30($sp)
  18e3f8: 50 48 45 8e  	lw	$5, 0x4850($18)
  18e3fc: 3c 30 11 00  	dsll32	$6, $17, 0x0
  18e400: 78 00 02 fe  	sd	$2, 0x78($16)
  18e404: 3e 30 06 00  	dsrl32	$6, $6, 0x0
  18e408: 18 00 a2 8c  	lw	$2, 0x18($5)
  18e40c: 1c 00 a3 8c  	lw	$3, 0x1c($5)
  18e410: 20 00 a4 dc  	ld	$4, 0x20($5)
  18e414: 21 10 51 00  	addu	$2, $2, $17
  18e418: 23 18 71 00  	subu	$3, $3, $17
  18e41c: 20 00 b2 df  	ld	$18, 0x20($sp)
  18e420: 10 00 b1 df  	ld	$17, 0x10($sp)
  18e424: 2d 20 86 00  	daddu	$4, $4, $6
  18e428: 00 00 b0 df  	ld	$16, 0x0($sp)
  18e42c: 18 00 a2 ac  	sw	$2, 0x18($5)
  18e430: 1c 00 a3 ac  	sw	$3, 0x1c($5)
  18e434: 20 00 a4 fc  	sd	$4, 0x20($5)
  18e438: 08 00 e0 03  	jr	$ra
  18e43c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  18e440: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  18e444: 01 00 03 24  	addiu	$3, $zero, 0x1
  18e448: 42 00 02 3c  	lui	$2, 0x42
  18e44c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  18e450: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18e454: 2a 6e 43 a0  	sb	$3, 0x6e2a($2)
  18e458: 42 00 10 3c  	lui	$16, 0x42
  18e45c: 2c 6e 02 8e  	lw	$2, 0x6e2c($16)
  18e460: 19 00 42 28  	slti	$2, $2, 0x19
  18e464: 11 00 40 10  	beqz	$2, 0x18e4ac <.text+0x8e4ac>
  18e468: 2d 20 a0 03  	move	$4, $sp
  18e46c: 18 37 06 0c  	jal	0x18dc60 <.text+0x8dc60>
  18e470: 00 00 00 00  	nop
  18e474: 08 00 03 24  	addiu	$3, $zero, 0x8
  18e478: 0c 00 43 14  	bne	$2, $3, 0x18e4ac <.text+0x8e4ac>
  18e47c: 42 00 06 3c  	lui	$6, 0x42
  18e480: 2c 6e 05 8e  	lw	$5, 0x6e2c($16)
  18e484: 00 00 a2 97  	lhu	$2, 0x0($sp)
  18e488: 20 6e c3 dc  	ld	$3, 0x6e20($6)
  18e48c: 08 00 a4 24  	addiu	$4, $5, 0x8
  18e490: 14 10 a2 00  	dsllv	$2, $2, $5
  18e494: 2c 6e 04 ae  	sw	$4, 0x6e2c($16)
  18e498: 25 18 62 00  	or	$3, $3, $2
  18e49c: 42 00 02 3c  	lui	$2, 0x42
  18e4a0: 2a 6e 40 a0  	sb	$zero, 0x6e2a($2)
  18e4a4: ed ff 00 10  	b	0x18e45c <.text+0x8e45c>
  18e4a8: 20 6e c3 fc  	sd	$3, 0x6e20($6)
  18e4ac: 20 00 bf df  	ld	$ra, 0x20($sp)
  18e4b0: 2d 10 00 00  	move	$2, $zero
  18e4b4: 10 00 b0 df  	ld	$16, 0x10($sp)
  18e4b8: 08 00 e0 03  	jr	$ra
  18e4bc: 30 00 bd 27  	addiu	$sp, $sp, 0x30
