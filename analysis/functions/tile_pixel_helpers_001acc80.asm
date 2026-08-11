
build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
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
  1acd04: 36 00 02 3c  	lui	$2, 0x36
  1acd08: 2d 40 a0 00  	move	$8, $5
  1acd0c: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  1acd10: 40 00 e2 8c  	lw	$2, 0x40($7)
  1acd14: 4c 00 e5 90  	lbu	$5, 0x4c($7)
  1acd18: 21 30 44 00  	addu	$6, $2, $4
  1acd1c: 3c 00 e3 8c  	lw	$3, 0x3c($7)
  1acd20: 00 00 c2 90  	lbu	$2, 0x0($6)
  1acd24: 2b 10 45 00  	sltu	$2, $2, $5
  1acd28: 0b 00 40 10  	beqz	$2, 0x1acd58 <.text+0xacd58>
  1acd2c: 21 48 64 00  	addu	$9, $3, $4
  1acd30: 00 00 02 91  	lbu	$2, 0x0($8)
  1acd34: 09 00 40 10  	beqz	$2, 0x1acd5c <.text+0xacd5c>
  1acd38: 36 00 03 3c  	lui	$3, 0x36
  1acd3c: 44 00 e3 8c  	lw	$3, 0x44($7)
  1acd40: 40 10 02 00  	sll	$2, $2, 0x1
  1acd44: 21 10 43 00  	addu	$2, $2, $3
  1acd48: 00 00 42 90  	lbu	$2, 0x0($2)
  1acd4c: 00 00 22 a1  	sb	$2, 0x0($9)
  1acd50: 4d 00 e2 90  	lbu	$2, 0x4d($7)
  1acd54: 00 00 c2 a0  	sb	$2, 0x0($6)
  1acd58: 36 00 03 3c  	lui	$3, 0x36
  1acd5c: 01 00 c2 90  	lbu	$2, 0x1($6)
  1acd60: 80 d4 64 24  	addiu	$4, $3, -0x2b80 <.text+0xffffffffffefd480>
  1acd64: 4c 00 83 90  	lbu	$3, 0x4c($4)
  1acd68: 2b 10 43 00  	sltu	$2, $2, $3
  1acd6c: 0c 00 40 10  	beqz	$2, 0x1acda0 <.text+0xacda0>
  1acd70: 36 00 03 3c  	lui	$3, 0x36
  1acd74: 01 00 02 91  	lbu	$2, 0x1($8)
  1acd78: 0a 00 40 50  	beqzl	$2, 0x1acda4 <.text+0xacda4>
  1acd7c: 02 00 c2 90  	lbu	$2, 0x2($6)
  1acd80: 44 00 83 8c  	lw	$3, 0x44($4)
  1acd84: 40 10 02 00  	sll	$2, $2, 0x1
  1acd88: 21 10 43 00  	addu	$2, $2, $3
  1acd8c: 00 00 42 90  	lbu	$2, 0x0($2)
  1acd90: 01 00 22 a1  	sb	$2, 0x1($9)
  1acd94: 4d 00 82 90  	lbu	$2, 0x4d($4)
  1acd98: 01 00 c2 a0  	sb	$2, 0x1($6)
  1acd9c: 36 00 03 3c  	lui	$3, 0x36
  1acda0: 02 00 c2 90  	lbu	$2, 0x2($6)
  1acda4: 80 d4 64 24  	addiu	$4, $3, -0x2b80 <.text+0xffffffffffefd480>
  1acda8: 4c 00 83 90  	lbu	$3, 0x4c($4)
  1acdac: 2b 10 43 00  	sltu	$2, $2, $3
  1acdb0: 0c 00 40 10  	beqz	$2, 0x1acde4 <.text+0xacde4>
  1acdb4: 36 00 03 3c  	lui	$3, 0x36
  1acdb8: 02 00 02 91  	lbu	$2, 0x2($8)
  1acdbc: 0a 00 40 50  	beqzl	$2, 0x1acde8 <.text+0xacde8>
  1acdc0: 03 00 c2 90  	lbu	$2, 0x3($6)
  1acdc4: 44 00 83 8c  	lw	$3, 0x44($4)
  1acdc8: 40 10 02 00  	sll	$2, $2, 0x1
  1acdcc: 21 10 43 00  	addu	$2, $2, $3
  1acdd0: 00 00 42 90  	lbu	$2, 0x0($2)
  1acdd4: 02 00 22 a1  	sb	$2, 0x2($9)
  1acdd8: 4d 00 82 90  	lbu	$2, 0x4d($4)
  1acddc: 02 00 c2 a0  	sb	$2, 0x2($6)
  1acde0: 36 00 03 3c  	lui	$3, 0x36
  1acde4: 03 00 c2 90  	lbu	$2, 0x3($6)
  1acde8: 80 d4 64 24  	addiu	$4, $3, -0x2b80 <.text+0xffffffffffefd480>
  1acdec: 4c 00 83 90  	lbu	$3, 0x4c($4)
  1acdf0: 2b 10 43 00  	sltu	$2, $2, $3
  1acdf4: 0a 00 40 10  	beqz	$2, 0x1ace20 <.text+0xace20>
  1acdf8: 00 00 00 00  	nop
  1acdfc: 03 00 02 91  	lbu	$2, 0x3($8)
  1ace00: 07 00 40 10  	beqz	$2, 0x1ace20 <.text+0xace20>
  1ace04: 40 10 02 00  	sll	$2, $2, 0x1
  1ace08: 44 00 83 8c  	lw	$3, 0x44($4)
  1ace0c: 21 10 43 00  	addu	$2, $2, $3
  1ace10: 00 00 42 90  	lbu	$2, 0x0($2)
  1ace14: 03 00 22 a1  	sb	$2, 0x3($9)
  1ace18: 4d 00 82 90  	lbu	$2, 0x4d($4)
  1ace1c: 03 00 c2 a0  	sb	$2, 0x3($6)
  1ace20: 08 00 e0 03  	jr	$ra
  1ace24: 00 00 00 00  	nop
  1ace28: 36 00 02 3c  	lui	$2, 0x36
  1ace2c: 2d 40 a0 00  	move	$8, $5
  1ace30: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ace34: 40 00 e2 8c  	lw	$2, 0x40($7)
  1ace38: 4c 00 e5 90  	lbu	$5, 0x4c($7)
  1ace3c: 21 30 44 00  	addu	$6, $2, $4
  1ace40: 3c 00 e3 8c  	lw	$3, 0x3c($7)
  1ace44: 00 00 c2 90  	lbu	$2, 0x0($6)
  1ace48: 2b 10 45 00  	sltu	$2, $2, $5
  1ace4c: 0b 00 40 10  	beqz	$2, 0x1ace7c <.text+0xace7c>
  1ace50: 21 48 64 00  	addu	$9, $3, $4
  1ace54: 03 00 02 91  	lbu	$2, 0x3($8)
  1ace58: 09 00 40 10  	beqz	$2, 0x1ace80 <.text+0xace80>
  1ace5c: 36 00 03 3c  	lui	$3, 0x36
  1ace60: 44 00 e3 8c  	lw	$3, 0x44($7)
  1ace64: 40 10 02 00  	sll	$2, $2, 0x1
  1ace68: 21 10 43 00  	addu	$2, $2, $3
  1ace6c: 00 00 42 90  	lbu	$2, 0x0($2)
  1ace70: 00 00 22 a1  	sb	$2, 0x0($9)
  1ace74: 4d 00 e2 90  	lbu	$2, 0x4d($7)
  1ace78: 00 00 c2 a0  	sb	$2, 0x0($6)
  1ace7c: 36 00 03 3c  	lui	$3, 0x36
  1ace80: 01 00 c2 90  	lbu	$2, 0x1($6)
  1ace84: 80 d4 64 24  	addiu	$4, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ace88: 4c 00 83 90  	lbu	$3, 0x4c($4)
  1ace8c: 2b 10 43 00  	sltu	$2, $2, $3
  1ace90: 0c 00 40 10  	beqz	$2, 0x1acec4 <.text+0xacec4>
  1ace94: 36 00 03 3c  	lui	$3, 0x36
  1ace98: 02 00 02 91  	lbu	$2, 0x2($8)
  1ace9c: 0a 00 40 50  	beqzl	$2, 0x1acec8 <.text+0xacec8>
  1acea0: 02 00 c2 90  	lbu	$2, 0x2($6)
  1acea4: 44 00 83 8c  	lw	$3, 0x44($4)
  1acea8: 40 10 02 00  	sll	$2, $2, 0x1
  1aceac: 21 10 43 00  	addu	$2, $2, $3
  1aceb0: 00 00 42 90  	lbu	$2, 0x0($2)
  1aceb4: 01 00 22 a1  	sb	$2, 0x1($9)
  1aceb8: 4d 00 82 90  	lbu	$2, 0x4d($4)
  1acebc: 01 00 c2 a0  	sb	$2, 0x1($6)
  1acec0: 36 00 03 3c  	lui	$3, 0x36
  1acec4: 02 00 c2 90  	lbu	$2, 0x2($6)
  1acec8: 80 d4 64 24  	addiu	$4, $3, -0x2b80 <.text+0xffffffffffefd480>
  1acecc: 4c 00 83 90  	lbu	$3, 0x4c($4)
  1aced0: 2b 10 43 00  	sltu	$2, $2, $3
  1aced4: 0c 00 40 10  	beqz	$2, 0x1acf08 <.text+0xacf08>
  1aced8: 36 00 03 3c  	lui	$3, 0x36
  1acedc: 01 00 02 91  	lbu	$2, 0x1($8)
  1acee0: 0a 00 40 50  	beqzl	$2, 0x1acf0c <.text+0xacf0c>
  1acee4: 03 00 c2 90  	lbu	$2, 0x3($6)
  1acee8: 44 00 83 8c  	lw	$3, 0x44($4)
  1aceec: 40 10 02 00  	sll	$2, $2, 0x1
  1acef0: 21 10 43 00  	addu	$2, $2, $3
  1acef4: 00 00 42 90  	lbu	$2, 0x0($2)
  1acef8: 02 00 22 a1  	sb	$2, 0x2($9)
  1acefc: 4d 00 82 90  	lbu	$2, 0x4d($4)
  1acf00: 02 00 c2 a0  	sb	$2, 0x2($6)
  1acf04: 36 00 03 3c  	lui	$3, 0x36
  1acf08: 03 00 c2 90  	lbu	$2, 0x3($6)
  1acf0c: 80 d4 64 24  	addiu	$4, $3, -0x2b80 <.text+0xffffffffffefd480>
  1acf10: 4c 00 83 90  	lbu	$3, 0x4c($4)
  1acf14: 2b 10 43 00  	sltu	$2, $2, $3
  1acf18: 0a 00 40 10  	beqz	$2, 0x1acf44 <.text+0xacf44>
  1acf1c: 00 00 00 00  	nop
  1acf20: 00 00 02 91  	lbu	$2, 0x0($8)
  1acf24: 07 00 40 10  	beqz	$2, 0x1acf44 <.text+0xacf44>
  1acf28: 40 10 02 00  	sll	$2, $2, 0x1
  1acf2c: 44 00 83 8c  	lw	$3, 0x44($4)
  1acf30: 21 10 43 00  	addu	$2, $2, $3
  1acf34: 00 00 42 90  	lbu	$2, 0x0($2)
  1acf38: 03 00 22 a1  	sb	$2, 0x3($9)
  1acf3c: 4d 00 82 90  	lbu	$2, 0x4d($4)
  1acf40: 03 00 c2 a0  	sb	$2, 0x3($6)
  1acf44: 08 00 e0 03  	jr	$ra
  1acf48: 00 00 00 00  	nop
  1acf4c: 36 00 02 3c  	lui	$2, 0x36
  1acf50: 2d 40 a0 00  	move	$8, $5
  1acf54: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  1acf58: 40 00 e2 8c  	lw	$2, 0x40($7)
  1acf5c: 4c 00 e5 90  	lbu	$5, 0x4c($7)
  1acf60: 21 30 44 00  	addu	$6, $2, $4
  1acf64: 3c 00 e3 8c  	lw	$3, 0x3c($7)
  1acf68: 00 00 c2 90  	lbu	$2, 0x0($6)
  1acf6c: 2b 10 45 00  	sltu	$2, $2, $5
  1acf70: 0d 00 40 10  	beqz	$2, 0x1acfa8 <.text+0xacfa8>
  1acf74: 21 20 64 00  	addu	$4, $3, $4
  1acf78: 00 00 02 91  	lbu	$2, 0x0($8)
  1acf7c: 0b 00 40 10  	beqz	$2, 0x1acfac <.text+0xacfac>
