
build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
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
  1acf80: 36 00 03 3c  	lui	$3, 0x36
  1acf84: 44 00 e3 8c  	lw	$3, 0x44($7)
  1acf88: 40 10 02 00  	sll	$2, $2, 0x1
  1acf8c: 21 10 43 00  	addu	$2, $2, $3
  1acf90: 00 00 42 90  	lbu	$2, 0x0($2)
  1acf94: 00 00 82 a0  	sb	$2, 0x0($4)
  1acf98: 01 00 82 a0  	sb	$2, 0x1($4)
  1acf9c: 4d 00 e2 90  	lbu	$2, 0x4d($7)
  1acfa0: 00 00 c2 a0  	sb	$2, 0x0($6)
  1acfa4: 01 00 c2 a0  	sb	$2, 0x1($6)
  1acfa8: 36 00 03 3c  	lui	$3, 0x36
  1acfac: 02 00 c2 90  	lbu	$2, 0x2($6)
  1acfb0: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1acfb4: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1acfb8: 2b 10 43 00  	sltu	$2, $2, $3
  1acfbc: 0e 00 40 10  	beqz	$2, 0x1acff8 <.text+0xacff8>
  1acfc0: 36 00 03 3c  	lui	$3, 0x36
  1acfc4: 01 00 02 91  	lbu	$2, 0x1($8)
  1acfc8: 0c 00 40 50  	beqzl	$2, 0x1acffc <.text+0xacffc>
  1acfcc: 04 00 c2 90  	lbu	$2, 0x4($6)
  1acfd0: 44 00 a3 8c  	lw	$3, 0x44($5)
  1acfd4: 40 10 02 00  	sll	$2, $2, 0x1
  1acfd8: 21 10 43 00  	addu	$2, $2, $3
  1acfdc: 00 00 42 90  	lbu	$2, 0x0($2)
  1acfe0: 02 00 82 a0  	sb	$2, 0x2($4)
  1acfe4: 03 00 82 a0  	sb	$2, 0x3($4)
  1acfe8: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1acfec: 02 00 c2 a0  	sb	$2, 0x2($6)
  1acff0: 03 00 c2 a0  	sb	$2, 0x3($6)
  1acff4: 36 00 03 3c  	lui	$3, 0x36
  1acff8: 04 00 c2 90  	lbu	$2, 0x4($6)
  1acffc: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad000: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad004: 2b 10 43 00  	sltu	$2, $2, $3
  1ad008: 0e 00 40 10  	beqz	$2, 0x1ad044 <.text+0xad044>
  1ad00c: 36 00 03 3c  	lui	$3, 0x36
  1ad010: 02 00 02 91  	lbu	$2, 0x2($8)
  1ad014: 0c 00 40 50  	beqzl	$2, 0x1ad048 <.text+0xad048>
  1ad018: 06 00 c2 90  	lbu	$2, 0x6($6)
  1ad01c: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad020: 40 10 02 00  	sll	$2, $2, 0x1
  1ad024: 21 10 43 00  	addu	$2, $2, $3
  1ad028: 00 00 42 90  	lbu	$2, 0x0($2)
  1ad02c: 04 00 82 a0  	sb	$2, 0x4($4)
  1ad030: 05 00 82 a0  	sb	$2, 0x5($4)
  1ad034: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad038: 04 00 c2 a0  	sb	$2, 0x4($6)
  1ad03c: 05 00 c2 a0  	sb	$2, 0x5($6)
  1ad040: 36 00 03 3c  	lui	$3, 0x36
  1ad044: 06 00 c2 90  	lbu	$2, 0x6($6)
  1ad048: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad04c: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad050: 2b 10 43 00  	sltu	$2, $2, $3
  1ad054: 0c 00 40 10  	beqz	$2, 0x1ad088 <.text+0xad088>
  1ad058: 00 00 00 00  	nop
  1ad05c: 03 00 02 91  	lbu	$2, 0x3($8)
  1ad060: 09 00 40 10  	beqz	$2, 0x1ad088 <.text+0xad088>
  1ad064: 40 10 02 00  	sll	$2, $2, 0x1
  1ad068: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad06c: 21 10 43 00  	addu	$2, $2, $3
  1ad070: 00 00 42 90  	lbu	$2, 0x0($2)
  1ad074: 06 00 82 a0  	sb	$2, 0x6($4)
  1ad078: 07 00 82 a0  	sb	$2, 0x7($4)
  1ad07c: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad080: 06 00 c2 a0  	sb	$2, 0x6($6)
  1ad084: 07 00 c2 a0  	sb	$2, 0x7($6)
  1ad088: 08 00 e0 03  	jr	$ra
  1ad08c: 00 00 00 00  	nop
  1ad090: 36 00 02 3c  	lui	$2, 0x36
  1ad094: 2d 40 a0 00  	move	$8, $5
  1ad098: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ad09c: 40 00 e2 8c  	lw	$2, 0x40($7)
  1ad0a0: 4c 00 e5 90  	lbu	$5, 0x4c($7)
  1ad0a4: 21 30 44 00  	addu	$6, $2, $4
  1ad0a8: 3c 00 e3 8c  	lw	$3, 0x3c($7)
  1ad0ac: 00 00 c2 90  	lbu	$2, 0x0($6)
  1ad0b0: 2b 10 45 00  	sltu	$2, $2, $5
  1ad0b4: 0d 00 40 10  	beqz	$2, 0x1ad0ec <.text+0xad0ec>
  1ad0b8: 21 20 64 00  	addu	$4, $3, $4
  1ad0bc: 03 00 02 91  	lbu	$2, 0x3($8)
  1ad0c0: 0b 00 40 10  	beqz	$2, 0x1ad0f0 <.text+0xad0f0>
  1ad0c4: 36 00 03 3c  	lui	$3, 0x36
  1ad0c8: 44 00 e3 8c  	lw	$3, 0x44($7)
  1ad0cc: 40 10 02 00  	sll	$2, $2, 0x1
  1ad0d0: 21 10 43 00  	addu	$2, $2, $3
  1ad0d4: 00 00 42 90  	lbu	$2, 0x0($2)
  1ad0d8: 00 00 82 a0  	sb	$2, 0x0($4)
  1ad0dc: 01 00 82 a0  	sb	$2, 0x1($4)
  1ad0e0: 4d 00 e2 90  	lbu	$2, 0x4d($7)
  1ad0e4: 00 00 c2 a0  	sb	$2, 0x0($6)
  1ad0e8: 01 00 c2 a0  	sb	$2, 0x1($6)
  1ad0ec: 36 00 03 3c  	lui	$3, 0x36
  1ad0f0: 02 00 c2 90  	lbu	$2, 0x2($6)
  1ad0f4: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad0f8: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad0fc: 2b 10 43 00  	sltu	$2, $2, $3
  1ad100: 0e 00 40 10  	beqz	$2, 0x1ad13c <.text+0xad13c>
  1ad104: 36 00 03 3c  	lui	$3, 0x36
  1ad108: 02 00 02 91  	lbu	$2, 0x2($8)
  1ad10c: 0c 00 40 50  	beqzl	$2, 0x1ad140 <.text+0xad140>
  1ad110: 04 00 c2 90  	lbu	$2, 0x4($6)
  1ad114: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad118: 40 10 02 00  	sll	$2, $2, 0x1
  1ad11c: 21 10 43 00  	addu	$2, $2, $3
  1ad120: 00 00 42 90  	lbu	$2, 0x0($2)
  1ad124: 02 00 82 a0  	sb	$2, 0x2($4)
  1ad128: 03 00 82 a0  	sb	$2, 0x3($4)
  1ad12c: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad130: 02 00 c2 a0  	sb	$2, 0x2($6)
  1ad134: 03 00 c2 a0  	sb	$2, 0x3($6)
  1ad138: 36 00 03 3c  	lui	$3, 0x36
  1ad13c: 04 00 c2 90  	lbu	$2, 0x4($6)
  1ad140: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad144: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad148: 2b 10 43 00  	sltu	$2, $2, $3
  1ad14c: 0e 00 40 10  	beqz	$2, 0x1ad188 <.text+0xad188>
  1ad150: 36 00 03 3c  	lui	$3, 0x36
  1ad154: 01 00 02 91  	lbu	$2, 0x1($8)
  1ad158: 0c 00 40 50  	beqzl	$2, 0x1ad18c <.text+0xad18c>
  1ad15c: 06 00 c2 90  	lbu	$2, 0x6($6)
  1ad160: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad164: 40 10 02 00  	sll	$2, $2, 0x1
  1ad168: 21 10 43 00  	addu	$2, $2, $3
  1ad16c: 00 00 42 90  	lbu	$2, 0x0($2)
  1ad170: 04 00 82 a0  	sb	$2, 0x4($4)
  1ad174: 05 00 82 a0  	sb	$2, 0x5($4)
  1ad178: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad17c: 04 00 c2 a0  	sb	$2, 0x4($6)
  1ad180: 05 00 c2 a0  	sb	$2, 0x5($6)
  1ad184: 36 00 03 3c  	lui	$3, 0x36
  1ad188: 06 00 c2 90  	lbu	$2, 0x6($6)
  1ad18c: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad190: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad194: 2b 10 43 00  	sltu	$2, $2, $3
  1ad198: 0c 00 40 10  	beqz	$2, 0x1ad1cc <.text+0xad1cc>
  1ad19c: 00 00 00 00  	nop
  1ad1a0: 00 00 02 91  	lbu	$2, 0x0($8)
  1ad1a4: 09 00 40 10  	beqz	$2, 0x1ad1cc <.text+0xad1cc>
  1ad1a8: 40 10 02 00  	sll	$2, $2, 0x1
  1ad1ac: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad1b0: 21 10 43 00  	addu	$2, $2, $3
  1ad1b4: 00 00 42 90  	lbu	$2, 0x0($2)
  1ad1b8: 06 00 82 a0  	sb	$2, 0x6($4)
  1ad1bc: 07 00 82 a0  	sb	$2, 0x7($4)
  1ad1c0: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad1c4: 06 00 c2 a0  	sb	$2, 0x6($6)
  1ad1c8: 07 00 c2 a0  	sb	$2, 0x7($6)
  1ad1cc: 08 00 e0 03  	jr	$ra
