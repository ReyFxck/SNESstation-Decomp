
/mnt/data/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  19fddc: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  19fde0: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19fde4: 34 00 b6 e7  	swc1	$f22, 0x34($sp)
  19fde8: 86 65 00 46  	mov.s	$f22, $f12
  19fdec: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19fdf0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19fdf4: 38 00 b8 e7  	swc1	$f24, 0x38($sp)
  19fdf8: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  19fdfc: 30 00 b4 e7  	swc1	$f20, 0x30($sp)
  19fe00: 2d 20 40 00  	move	$4, $2
  19fe04: b0 81 06 0c  	jal	0x1a06c0 <.text+0xa06c0>
  19fe08: 00 00 00 00  	nop
  19fe0c: 2d 18 40 00  	move	$3, $2
  19fe10: 01 00 02 24  	addiu	$2, $zero, 0x1
  19fe14: 80 00 62 10  	beq	$3, $2, 0x1a0018 <.text+0xa0018>
  19fe18: 02 00 02 24  	addiu	$2, $zero, 0x2
  19fe1c: 7c 00 62 10  	beq	$3, $2, 0x1a0010 <.text+0xa0010>
  19fe20: 06 b0 00 46  	mov.s	$f0, $f22
  19fe24: 05 b6 00 46  	abs.s	$f24, $f22
  19fe28: c9 3f 01 3c  	lui	$1, 0x3fc9
  19fe2c: db 0f 21 34  	ori	$1, $1, 0xfdb
  19fe30: 00 00 81 44  	mtc1	$1, $f0
  19fe34: 00 00 00 00  	nop
  19fe38: 00 c5 00 46  	add.s	$f20, $f24, $f0
  19fe3c: 49 4d 01 3c  	lui	$1, 0x4d49
  19fe40: db 0f 21 34  	ori	$1, $1, 0xfdb
  19fe44: 00 00 81 44  	mtc1	$1, $f0
  19fe48: 00 00 00 00  	nop
  19fe4c: 34 00 14 46  	c.olt.s	$f0, $f20
  19fe50: 00 00 00 00  	nop
  19fe54: 0d 00 00 45  	bc1f	0x19fe8c <.text+0x9fe8c>
  19fe58: 01 00 11 24  	addiu	$17, $zero, 0x1
  19fe5c: 06 b0 00 46  	mov.s	$f0, $f22
  19fe60: 22 00 03 24  	addiu	$3, $zero, 0x22
  19fe64: 42 00 02 3c  	lui	$2, 0x42
  19fe68: 70 5a 43 ac  	sw	$3, 0x5a70($2)
  19fe6c: 20 00 bf df  	ld	$ra, 0x20($sp)
  19fe70: 10 00 b1 df  	ld	$17, 0x10($sp)
  19fe74: 00 00 b0 df  	ld	$16, 0x0($sp)
  19fe78: 38 00 b8 c7  	lwc1	$f24, 0x38($sp)
  19fe7c: 34 00 b6 c7  	lwc1	$f22, 0x34($sp)
  19fe80: 30 00 b4 c7  	lwc1	$f20, 0x30($sp)
  19fe84: 08 00 e0 03  	jr	$ra
  19fe88: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19fe8c: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  19fe90: 06 a3 00 46  	mov.s	$f12, $f20
  19fe94: 2d 28 00 00  	move	$5, $zero
  19fe98: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  19fe9c: 2d 20 40 00  	move	$4, $2
  19fea0: 50 00 40 04  	bltz	$2, 0x19ffe4 <.text+0x9ffe4>
  19fea4: 00 00 00 00  	nop
  19fea8: a2 3e 01 3c  	lui	$1, 0x3ea2
  19feac: 83 f9 21 34  	ori	$1, $1, 0xf983
  19feb0: 00 60 81 44  	mtc1	$1, $f12
  19feb4: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  19feb8: 02 a3 0c 46  	mul.s	$f12, $f20, $f12
  19febc: 80 ff 05 34  	ori	$5, $zero, 0xff80
  19fec0: bc 2b 05 00  	dsll32	$5, $5, 0xe
  19fec4: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  19fec8: 2d 20 40 00  	move	$4, $2
  19fecc: fc 8e 06 0c  	jal	0x1a3bf0 <.text+0xa3bf0>
  19fed0: 2d 20 40 00  	move	$4, $2
  19fed4: 00 60 82 44  	mtc1	$2, $f12
  19fed8: 00 00 00 00  	nop
  19fedc: 20 63 80 46  	cvt.s.w	$f12, $f12
  19fee0: 01 00 43 30  	andi	$3, $2, 0x1
  19fee4: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19fee8: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  19feec: 0b 88 43 00  	movn	$17, $2, $3
  19fef0: 80 ff 05 34  	ori	$5, $zero, 0xff80
  19fef4: bc 2b 05 00  	dsll32	$5, $5, 0xe
  19fef8: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  19fefc: 2d 20 40 00  	move	$4, $2
  19ff00: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  19ff04: 2d 20 40 00  	move	$4, $2
  19ff08: 06 c3 00 46  	mov.s	$f12, $f24
  19ff0c: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  19ff10: 06 05 00 46  	mov.s	$f20, $f0
  19ff14: 2d 80 40 00  	move	$16, $2
  19ff18: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  19ff1c: 06 a3 00 46  	mov.s	$f12, $f20
  19ff20: 1c 00 01 3c  	lui	$1, 0x1c
  19ff24: c0 a6 25 dc  	ld	$5, -0x5940($1)
  19ff28: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  19ff2c: 2d 20 40 00  	move	$4, $2
  19ff30: 2d 20 00 02  	move	$4, $16
  19ff34: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  19ff38: 2d 28 40 00  	move	$5, $2
  19ff3c: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  19ff40: 2d 20 40 00  	move	$4, $2
  19ff44: 42 00 02 3c  	lui	$2, 0x42
  19ff48: c4 5a 42 c4  	lwc1	$f2, 0x5ac4($2)
  19ff4c: 06 05 00 46  	mov.s	$f20, $f0
  19ff50: 07 10 00 46  	neg.s	$f0, $f2
  19ff54: 34 00 14 46  	c.olt.s	$f0, $f20
  19ff58: 00 00 00 00  	nop
  19ff5c: 0b 00 02 45  	bc1fl	0x19ff8c <.text+0x9ff8c>
  19ff60: 02 a1 14 46  	mul.s	$f4, $f20, $f20
  19ff64: 34 a0 02 46  	c.olt.s	$f20, $f2
  19ff68: 00 00 00 00  	nop
  19ff6c: 07 00 02 45  	bc1fl	0x19ff8c <.text+0x9ff8c>
  19ff70: 02 a1 14 46  	mul.s	$f4, $f20, $f20
  19ff74: 06 a3 00 46  	mov.s	$f12, $f20
  19ff78: 00 00 91 44  	mtc1	$17, $f0
  19ff7c: 00 00 00 00  	nop
  19ff80: 20 00 80 46  	cvt.s.w	$f0, $f0
  19ff84: b9 ff 00 10  	b	0x19fe6c <.text+0x9fe6c>
  19ff88: 02 60 00 46  	mul.s	$f0, $f12, $f0
  19ff8c: 2e 36 01 3c  	lui	$1, 0x362e
  19ff90: 5b 9c 21 34  	ori	$1, $1, 0x9c5b
  19ff94: 00 00 81 44  	mtc1	$1, $f0
  19ff98: 4f b9 01 3c  	lui	$1, 0xb94f
  19ff9c: 22 b2 21 34  	ori	$1, $1, 0xb222
  19ffa0: 00 10 81 44  	mtc1	$1, $f2
  19ffa4: 02 20 00 46  	mul.s	$f0, $f4, $f0
  19ffa8: 00 00 02 46  	add.s	$f0, $f0, $f2
  19ffac: 08 3c 01 3c  	lui	$1, 0x3c08
  19ffb0: 3e 87 21 34  	ori	$1, $1, 0x873e
  19ffb4: 00 10 81 44  	mtc1	$1, $f2
  19ffb8: 02 00 04 46  	mul.s	$f0, $f0, $f4
  19ffbc: 00 00 02 46  	add.s	$f0, $f0, $f2
  19ffc0: 2a be 01 3c  	lui	$1, 0xbe2a
  19ffc4: a4 aa 21 34  	ori	$1, $1, 0xaaa4
  19ffc8: 00 10 81 44  	mtc1	$1, $f2
  19ffcc: 02 00 04 46  	mul.s	$f0, $f0, $f4
  19ffd0: 00 00 02 46  	add.s	$f0, $f0, $f2
  19ffd4: 02 00 04 46  	mul.s	$f0, $f0, $f4
  19ffd8: 02 a0 00 46  	mul.s	$f0, $f20, $f0
  19ffdc: e6 ff 00 10  	b	0x19ff78 <.text+0x9ff78>
  19ffe0: 00 a3 00 46  	add.s	$f12, $f20, $f0
  19ffe4: a2 3e 01 3c  	lui	$1, 0x3ea2
  19ffe8: 83 f9 21 34  	ori	$1, $1, 0xf983
  19ffec: 00 60 81 44  	mtc1	$1, $f12
  19fff0: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  19fff4: 02 a3 0c 46  	mul.s	$f12, $f20, $f12
  19fff8: 80 ff 05 34  	ori	$5, $zero, 0xff80
  19fffc: bc 2b 05 00  	dsll32	$5, $5, 0xe
  1a0000: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a0004: 2d 20 40 00  	move	$4, $2
  1a0008: b0 ff 00 10  	b	0x19fecc <.text+0x9fecc>
  1a000c: 00 00 00 00  	nop
  1a0010: 94 ff 00 10  	b	0x19fe64 <.text+0x9fe64>
  1a0014: 21 00 03 24  	addiu	$3, $zero, 0x21
  1a0018: 1c 00 02 3c  	lui	$2, 0x1c
  1a001c: fc ff 00 10  	b	0x1a0010 <.text+0xa0010>
  1a0020: 50 a7 40 c4  	lwc1	$f0, -0x58b0($2)
  1a0024: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a0028: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a002c: 34 00 b6 e7  	swc1	$f22, 0x34($sp)
  1a0030: 86 65 00 46  	mov.s	$f22, $f12
  1a0034: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a0038: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a003c: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0040: 30 00 b4 e7  	swc1	$f20, 0x30($sp)
  1a0044: 2d 20 40 00  	move	$4, $2
  1a0048: b0 81 06 0c  	jal	0x1a06c0 <.text+0xa06c0>
  1a004c: 00 00 00 00  	nop
  1a0050: 2d 18 40 00  	move	$3, $2
  1a0054: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a0058: 7b 00 62 10  	beq	$3, $2, 0x1a0248 <.text+0xa0248>
  1a005c: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a0060: 77 00 62 10  	beq	$3, $2, 0x1a0240 <.text+0xa0240>
  1a0064: 06 b0 00 46  	mov.s	$f0, $f22
  1a0068: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a006c: 06 b3 00 46  	mov.s	$f12, $f22
  1a0070: 2d 28 00 00  	move	$5, $zero
  1a0074: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a0078: 2d 20 40 00  	move	$4, $2
  1a007c: 6e 00 40 04  	bltz	$2, 0x1a0238 <.text+0xa0238>
  1a0080: 07 b5 00 46  	neg.s	$f20, $f22
  1a0084: 01 00 11 24  	addiu	$17, $zero, 0x1
  1a0088: 06 b5 00 46  	mov.s	$f20, $f22
  1a008c: 49 4d 01 3c  	lui	$1, 0x4d49
  1a0090: db 0f 21 34  	ori	$1, $1, 0xfdb
  1a0094: 00 00 81 44  	mtc1	$1, $f0
  1a0098: 00 00 00 00  	nop
  1a009c: 34 00 14 46  	c.olt.s	$f0, $f20
  1a00a0: 00 00 00 00  	nop
  1a00a4: 0b 00 00 45  	bc1f	0x1a00d4 <.text+0xa00d4>
  1a00a8: 06 b0 00 46  	mov.s	$f0, $f22
  1a00ac: 22 00 03 24  	addiu	$3, $zero, 0x22
  1a00b0: 42 00 02 3c  	lui	$2, 0x42
  1a00b4: 70 5a 43 ac  	sw	$3, 0x5a70($2)
  1a00b8: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a00bc: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a00c0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a00c4: 34 00 b6 c7  	lwc1	$f22, 0x34($sp)
  1a00c8: 30 00 b4 c7  	lwc1	$f20, 0x30($sp)
  1a00cc: 08 00 e0 03  	jr	$ra
  1a00d0: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a00d4: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a00d8: 06 a3 00 46  	mov.s	$f12, $f20
  1a00dc: 2d 28 00 00  	move	$5, $zero
  1a00e0: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a00e4: 2d 20 40 00  	move	$4, $2
  1a00e8: 48 00 40 04  	bltz	$2, 0x1a020c <.text+0xa020c>
  1a00ec: 00 00 00 00  	nop
  1a00f0: a2 3e 01 3c  	lui	$1, 0x3ea2
  1a00f4: 83 f9 21 34  	ori	$1, $1, 0xf983
  1a00f8: 00 60 81 44  	mtc1	$1, $f12
  1a00fc: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0100: 02 a3 0c 46  	mul.s	$f12, $f20, $f12
  1a0104: 80 ff 05 34  	ori	$5, $zero, 0xff80
  1a0108: bc 2b 05 00  	dsll32	$5, $5, 0xe
  1a010c: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a0110: 2d 20 40 00  	move	$4, $2
  1a0114: fc 8e 06 0c  	jal	0x1a3bf0 <.text+0xa3bf0>
  1a0118: 2d 20 40 00  	move	$4, $2
  1a011c: 05 b3 00 46  	abs.s	$f12, $f22
  1a0120: 00 a0 82 44  	mtc1	$2, $f20
  1a0124: 00 00 00 00  	nop
  1a0128: 20 a5 80 46  	cvt.s.w	$f20, $f20
  1a012c: 01 00 43 30  	andi	$3, $2, 0x1
  1a0130: 23 10 11 00  	negu	$2, $17
  1a0134: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0138: 0b 88 43 00  	movn	$17, $2, $3
  1a013c: 2d 80 40 00  	move	$16, $2
  1a0140: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0144: 06 a3 00 46  	mov.s	$f12, $f20
  1a0148: 1c 00 01 3c  	lui	$1, 0x1c
  1a014c: e0 a6 25 dc  	ld	$5, -0x5920($1)
  1a0150: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a0154: 2d 20 40 00  	move	$4, $2
  1a0158: 2d 20 00 02  	move	$4, $16
  1a015c: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a0160: 2d 28 40 00  	move	$5, $2
  1a0164: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  1a0168: 2d 20 40 00  	move	$4, $2
  1a016c: 42 00 02 3c  	lui	$2, 0x42
  1a0170: c4 5a 42 c4  	lwc1	$f2, 0x5ac4($2)
  1a0174: 06 05 00 46  	mov.s	$f20, $f0
  1a0178: 07 10 00 46  	neg.s	$f0, $f2
  1a017c: 34 00 14 46  	c.olt.s	$f0, $f20
  1a0180: 00 00 00 00  	nop
  1a0184: 0b 00 02 45  	bc1fl	0x1a01b4 <.text+0xa01b4>
  1a0188: 02 a1 14 46  	mul.s	$f4, $f20, $f20
  1a018c: 34 a0 02 46  	c.olt.s	$f20, $f2
  1a0190: 00 00 00 00  	nop
  1a0194: 07 00 02 45  	bc1fl	0x1a01b4 <.text+0xa01b4>
  1a0198: 02 a1 14 46  	mul.s	$f4, $f20, $f20
  1a019c: 06 a3 00 46  	mov.s	$f12, $f20
  1a01a0: 00 00 91 44  	mtc1	$17, $f0
  1a01a4: 00 00 00 00  	nop
  1a01a8: 20 00 80 46  	cvt.s.w	$f0, $f0
  1a01ac: c2 ff 00 10  	b	0x1a00b8 <.text+0xa00b8>
  1a01b0: 02 60 00 46  	mul.s	$f0, $f12, $f0
  1a01b4: 2e 36 01 3c  	lui	$1, 0x362e
  1a01b8: 5b 9c 21 34  	ori	$1, $1, 0x9c5b
  1a01bc: 00 00 81 44  	mtc1	$1, $f0
  1a01c0: 4f b9 01 3c  	lui	$1, 0xb94f
  1a01c4: 22 b2 21 34  	ori	$1, $1, 0xb222
  1a01c8: 00 10 81 44  	mtc1	$1, $f2
  1a01cc: 02 20 00 46  	mul.s	$f0, $f4, $f0
  1a01d0: 00 00 02 46  	add.s	$f0, $f0, $f2
  1a01d4: 08 3c 01 3c  	lui	$1, 0x3c08
  1a01d8: 3e 87 21 34  	ori	$1, $1, 0x873e
  1a01dc: 00 10 81 44  	mtc1	$1, $f2
  1a01e0: 02 00 04 46  	mul.s	$f0, $f0, $f4
  1a01e4: 00 00 02 46  	add.s	$f0, $f0, $f2
  1a01e8: 2a be 01 3c  	lui	$1, 0xbe2a
  1a01ec: a4 aa 21 34  	ori	$1, $1, 0xaaa4
  1a01f0: 00 10 81 44  	mtc1	$1, $f2
  1a01f4: 02 00 04 46  	mul.s	$f0, $f0, $f4
  1a01f8: 00 00 02 46  	add.s	$f0, $f0, $f2
  1a01fc: 02 00 04 46  	mul.s	$f0, $f0, $f4
  1a0200: 02 a0 00 46  	mul.s	$f0, $f20, $f0
  1a0204: e6 ff 00 10  	b	0x1a01a0 <.text+0xa01a0>
  1a0208: 00 a3 00 46  	add.s	$f12, $f20, $f0
  1a020c: a2 3e 01 3c  	lui	$1, 0x3ea2
  1a0210: 83 f9 21 34  	ori	$1, $1, 0xf983
  1a0214: 00 60 81 44  	mtc1	$1, $f12
  1a0218: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a021c: 02 a3 0c 46  	mul.s	$f12, $f20, $f12
  1a0220: 80 ff 05 34  	ori	$5, $zero, 0xff80
  1a0224: bc 2b 05 00  	dsll32	$5, $5, 0xe
  1a0228: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a022c: 2d 20 40 00  	move	$4, $2
  1a0230: b8 ff 00 10  	b	0x1a0114 <.text+0xa0114>
  1a0234: 00 00 00 00  	nop
  1a0238: 94 ff 00 10  	b	0x1a008c <.text+0xa008c>
  1a023c: ff ff 11 24  	addiu	$17, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0240: 9b ff 00 10  	b	0x1a00b0 <.text+0xa00b0>
  1a0244: 21 00 03 24  	addiu	$3, $zero, 0x21
  1a0248: 1c 00 02 3c  	lui	$2, 0x1c
  1a024c: fc ff 00 10  	b	0x1a0240 <.text+0xa0240>
  1a0250: 50 a7 40 c4  	lwc1	$f0, -0x58b0($2)
  1a0254: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a0258: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a025c: 34 00 b6 e7  	swc1	$f22, 0x34($sp)
  1a0260: 86 65 00 46  	mov.s	$f22, $f12
  1a0264: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a0268: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a026c: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0270: 30 00 b4 e7  	swc1	$f20, 0x30($sp)
  1a0274: 2d 20 40 00  	move	$4, $2
  1a0278: b0 81 06 0c  	jal	0x1a06c0 <.text+0xa06c0>
  1a027c: 00 00 00 00  	nop
  1a0280: 2d 18 40 00  	move	$3, $2
  1a0284: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a0288: 71 00 62 10  	beq	$3, $2, 0x1a0450 <.text+0xa0450>
  1a028c: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a0290: 6d 00 62 10  	beq	$3, $2, 0x1a0448 <.text+0xa0448>
  1a0294: 06 b0 00 46  	mov.s	$f0, $f22
  1a0298: 05 b5 00 46  	abs.s	$f20, $f22
  1a029c: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a02a0: 06 a3 00 46  	mov.s	$f12, $f20
  1a02a4: 1c 00 01 3c  	lui	$1, 0x1c
  1a02a8: 00 a7 25 dc  	ld	$5, -0x5900($1)
  1a02ac: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a02b0: 2d 20 40 00  	move	$4, $2
  1a02b4: 0b 00 40 18  	blez	$2, 0x1a02e4 <.text+0xa02e4>
  1a02b8: 06 a0 00 46  	mov.s	$f0, $f20
  1a02bc: 22 00 03 24  	addiu	$3, $zero, 0x22
  1a02c0: 42 00 02 3c  	lui	$2, 0x42
  1a02c4: 70 5a 43 ac  	sw	$3, 0x5a70($2)
  1a02c8: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a02cc: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a02d0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a02d4: 34 00 b6 c7  	lwc1	$f22, 0x34($sp)
  1a02d8: 30 00 b4 c7  	lwc1	$f20, 0x30($sp)
  1a02dc: 08 00 e0 03  	jr	$ra
  1a02e0: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a02e4: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a02e8: 06 b3 00 46  	mov.s	$f12, $f22
  1a02ec: 2d 28 00 00  	move	$5, $zero
  1a02f0: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a02f4: 2d 20 40 00  	move	$4, $2
  1a02f8: 48 00 40 04  	bltz	$2, 0x1a041c <.text+0xa041c>
  1a02fc: 00 00 00 00  	nop
  1a0300: 22 3f 01 3c  	lui	$1, 0x3f22
  1a0304: 83 f9 21 34  	ori	$1, $1, 0xf983
  1a0308: 00 60 81 44  	mtc1	$1, $f12
  1a030c: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0310: 02 b3 0c 46  	mul.s	$f12, $f22, $f12
  1a0314: 80 ff 05 34  	ori	$5, $zero, 0xff80
  1a0318: bc 2b 05 00  	dsll32	$5, $5, 0xe
  1a031c: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a0320: 2d 20 40 00  	move	$4, $2
  1a0324: fc 8e 06 0c  	jal	0x1a3bf0 <.text+0xa3bf0>
  1a0328: 2d 20 40 00  	move	$4, $2
  1a032c: 06 b3 00 46  	mov.s	$f12, $f22
  1a0330: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0334: 2d 88 40 00  	move	$17, $2
  1a0338: 2d 80 40 00  	move	$16, $2
  1a033c: cc 8e 06 0c  	jal	0x1a3b30 <.text+0xa3b30>
  1a0340: 2d 20 20 02  	move	$4, $17
  1a0344: 1c 00 01 3c  	lui	$1, 0x1c
  1a0348: 08 a7 25 dc  	ld	$5, -0x58f8($1)
  1a034c: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a0350: 2d 20 40 00  	move	$4, $2
  1a0354: 2d 20 00 02  	move	$4, $16
  1a0358: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a035c: 2d 28 40 00  	move	$5, $2
  1a0360: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  1a0364: 2d 20 40 00  	move	$4, $2
  1a0368: 42 00 02 3c  	lui	$2, 0x42
  1a036c: c4 5a 42 c4  	lwc1	$f2, 0x5ac4($2)
  1a0370: 86 01 00 46  	mov.s	$f6, $f0
  1a0374: 07 10 00 46  	neg.s	$f0, $f2
  1a0378: 34 00 06 46  	c.olt.s	$f0, $f6
  1a037c: 00 00 00 00  	nop
  1a0380: 0e 00 02 45  	bc1fl	0x1a03bc <.text+0xa03bc>
  1a0384: 02 31 06 46  	mul.s	$f4, $f6, $f6
  1a0388: 34 30 02 46  	c.olt.s	$f6, $f2
  1a038c: 00 00 00 00  	nop
  1a0390: 0a 00 02 45  	bc1fl	0x1a03bc <.text+0xa03bc>
  1a0394: 02 31 06 46  	mul.s	$f4, $f6, $f6
  1a0398: 80 3f 01 3c  	lui	$1, 0x3f80
  1a039c: 00 00 81 44  	mtc1	$1, $f0
  1a03a0: 06 35 00 46  	mov.s	$f20, $f6
  1a03a4: 01 00 22 32  	andi	$2, $17, 0x1
  1a03a8: c7 ff 40 50  	beqzl	$2, 0x1a02c8 <.text+0xa02c8>
  1a03ac: 03 a0 00 46  	div.s	$f0, $f20, $f0
  1a03b0: 07 a5 00 46  	neg.s	$f20, $f20
  1a03b4: c4 ff 00 10  	b	0x1a02c8 <.text+0xa02c8>
  1a03b8: 03 00 14 46  	div.s	$f0, $f0, $f20
  1a03bc: 1f 3c 01 3c  	lui	$1, 0x3c1f
  1a03c0: 75 33 21 34  	ori	$1, $1, 0x3375
  1a03c4: 00 60 81 44  	mtc1	$1, $f12
  1a03c8: db be 01 3c  	lui	$1, 0xbedb
  1a03cc: af b7 21 34  	ori	$1, $1, 0xb7af
  1a03d0: 00 10 81 44  	mtc1	$1, $f2
  1a03d4: c4 bd 01 3c  	lui	$1, 0xbdc4
  1a03d8: b8 33 21 34  	ori	$1, $1, 0x33b8
  1a03dc: 00 00 81 44  	mtc1	$1, $f0
  1a03e0: 02 23 0c 46  	mul.s	$f12, $f4, $f12
  1a03e4: 02 20 00 46  	mul.s	$f0, $f4, $f0
  1a03e8: 00 63 02 46  	add.s	$f12, $f12, $f2
  1a03ec: 02 30 00 46  	mul.s	$f0, $f6, $f0
  1a03f0: 02 63 04 46  	mul.s	$f12, $f12, $f4
  1a03f4: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a03f8: 00 05 06 46  	add.s	$f20, $f0, $f6
  1a03fc: c0 ff 05 34  	ori	$5, $zero, 0xffc0
  1a0400: bc 2b 05 00  	dsll32	$5, $5, 0xe
  1a0404: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a0408: 2d 20 40 00  	move	$4, $2
  1a040c: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  1a0410: 2d 20 40 00  	move	$4, $2
  1a0414: e4 ff 00 10  	b	0x1a03a8 <.text+0xa03a8>
  1a0418: 01 00 22 32  	andi	$2, $17, 0x1
  1a041c: 22 3f 01 3c  	lui	$1, 0x3f22
  1a0420: 83 f9 21 34  	ori	$1, $1, 0xf983
  1a0424: 00 60 81 44  	mtc1	$1, $f12
  1a0428: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a042c: 02 b3 0c 46  	mul.s	$f12, $f22, $f12
  1a0430: 80 ff 05 34  	ori	$5, $zero, 0xff80
  1a0434: bc 2b 05 00  	dsll32	$5, $5, 0xe
  1a0438: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a043c: 2d 20 40 00  	move	$4, $2
  1a0440: b8 ff 00 10  	b	0x1a0324 <.text+0xa0324>
  1a0444: 00 00 00 00  	nop
  1a0448: 9d ff 00 10  	b	0x1a02c0 <.text+0xa02c0>
  1a044c: 21 00 03 24  	addiu	$3, $zero, 0x21
  1a0450: 1c 00 02 3c  	lui	$2, 0x1c
  1a0454: fc ff 00 10  	b	0x1a0448 <.text+0xa0448>
  1a0458: 50 a7 40 c4  	lwc1	$f0, -0x58b0($2)
  1a045c: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a0460: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a0464: 44 00 b6 e7  	swc1	$f22, 0x44($sp)
  1a0468: 86 65 00 46  	mov.s	$f22, $f12
  1a046c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a0470: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a0474: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a0478: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a047c: 40 00 b4 e7  	swc1	$f20, 0x40($sp)
  1a0480: 2d 20 40 00  	move	$4, $2
  1a0484: b0 81 06 0c  	jal	0x1a06c0 <.text+0xa06c0>
  1a0488: 00 00 00 00  	nop
  1a048c: c9 3f 01 3c  	lui	$1, 0x3fc9
  1a0490: db 0f 21 34  	ori	$1, $1, 0xfdb
  1a0494: 00 00 81 44  	mtc1	$1, $f0
  1a0498: 2d 18 40 00  	move	$3, $2
  1a049c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a04a0: 6a 00 62 10  	beq	$3, $2, 0x1a064c <.text+0xa064c>
  1a04a4: 02 00 62 28  	slti	$2, $3, 0x2
  1a04a8: 75 00 40 14  	bnez	$2, 0x1a0680 <.text+0xa0680>
  1a04ac: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a04b0: 6e 00 62 10  	beq	$3, $2, 0x1a066c <.text+0xa066c>
  1a04b4: 05 b5 00 46  	abs.s	$f20, $f22
  1a04b8: c0 ff 11 34  	ori	$17, $zero, 0xffc0
  1a04bc: bc 8b 11 00  	dsll32	$17, $17, 0xe
  1a04c0: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a04c4: 06 a3 00 46  	mov.s	$f12, $f20
  1a04c8: 2d 28 20 02  	move	$5, $17
  1a04cc: 2d 20 40 00  	move	$4, $2
  1a04d0: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a04d4: 2d 80 40 00  	move	$16, $2
  1a04d8: 09 00 40 18  	blez	$2, 0x1a0500 <.text+0xa0500>
  1a04dc: 2d 90 00 00  	move	$18, $zero
  1a04e0: 2d 28 00 02  	move	$5, $16
  1a04e4: 2d 20 20 02  	move	$4, $17
  1a04e8: 54 8e 06 0c  	jal	0x1a3950 <.text+0xa3950>
  1a04ec: 02 00 12 24  	addiu	$18, $zero, 0x2
  1a04f0: 2d 20 40 00  	move	$4, $2
  1a04f4: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  1a04f8: 00 00 00 00  	nop
  1a04fc: 06 05 00 46  	mov.s	$f20, $f0
  1a0500: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0504: 06 a3 00 46  	mov.s	$f12, $f20
  1a0508: 1c 00 01 3c  	lui	$1, 0x1c
  1a050c: 38 a7 25 dc  	ld	$5, -0x58c8($1)
  1a0510: 2d 20 40 00  	move	$4, $2
  1a0514: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a0518: 2d 88 40 00  	move	$17, $2
  1a051c: 1f 00 40 18  	blez	$2, 0x1a059c <.text+0xa059c>
  1a0520: 42 00 02 3c  	lui	$2, 0x42
  1a0524: 3b 3f 01 3c  	lui	$1, 0x3f3b
  1a0528: ae 67 21 34  	ori	$1, $1, 0x67ae
  1a052c: 00 60 81 44  	mtc1	$1, $f12
  1a0530: 01 00 52 26  	addiu	$18, $18, 0x1
  1a0534: 80 ff 10 34  	ori	$16, $zero, 0xff80
  1a0538: bc 83 10 00  	dsll32	$16, $16, 0xe
  1a053c: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0540: 02 a3 0c 46  	mul.s	$f12, $f20, $f12
  1a0544: 2d 28 00 02  	move	$5, $16
  1a0548: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a054c: 2d 20 40 00  	move	$4, $2
  1a0550: 2d 28 00 02  	move	$5, $16
  1a0554: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a0558: 2d 20 40 00  	move	$4, $2
  1a055c: 2d 28 20 02  	move	$5, $17
  1a0560: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a0564: 2d 20 40 00  	move	$4, $2
  1a0568: dd 3f 01 3c  	lui	$1, 0x3fdd
  1a056c: d7 b3 21 34  	ori	$1, $1, 0xb3d7
  1a0570: 00 60 81 44  	mtc1	$1, $f12
  1a0574: 2d 80 40 00  	move	$16, $2
  1a0578: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a057c: 00 a3 0c 46  	add.s	$f12, $f20, $f12
  1a0580: 2d 20 00 02  	move	$4, $16
  1a0584: 54 8e 06 0c  	jal	0x1a3950 <.text+0xa3950>
  1a0588: 2d 28 40 00  	move	$5, $2
  1a058c: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  1a0590: 2d 20 40 00  	move	$4, $2
  1a0594: 06 05 00 46  	mov.s	$f20, $f0
  1a0598: 42 00 02 3c  	lui	$2, 0x42
  1a059c: c4 5a 42 c4  	lwc1	$f2, 0x5ac4($2)
  1a05a0: 07 10 00 46  	neg.s	$f0, $f2
  1a05a4: 34 00 14 46  	c.olt.s	$f0, $f20
  1a05a8: 00 00 00 00  	nop
  1a05ac: 06 00 02 45  	bc1fl	0x1a05c8 <.text+0xa05c8>
  1a05b0: 82 a1 14 46  	mul.s	$f6, $f20, $f20
  1a05b4: 34 a0 02 46  	c.olt.s	$f20, $f2
  1a05b8: 00 00 00 00  	nop
  1a05bc: 13 00 01 45  	bc1t	0x1a060c <.text+0xa060c>
  1a05c0: 02 00 42 2a  	slti	$2, $18, 0x2
  1a05c4: 82 a1 14 46  	mul.s	$f6, $f20, $f20
  1a05c8: 50 bd 01 3c  	lui	$1, 0xbd50
  1a05cc: 91 86 21 34  	ori	$1, $1, 0x8691
  1a05d0: 00 00 81 44  	mtc1	$1, $f0
  1a05d4: f1 be 01 3c  	lui	$1, 0xbef1
  1a05d8: f6 10 21 34  	ori	$1, $1, 0x10f6
  1a05dc: 00 10 81 44  	mtc1	$1, $f2
  1a05e0: b4 3f 01 3c  	lui	$1, 0x3fb4
  1a05e4: d3 cc 21 34  	ori	$1, $1, 0xccd3
  1a05e8: 00 20 81 44  	mtc1	$1, $f4
  1a05ec: 02 30 00 46  	mul.s	$f0, $f6, $f0
  1a05f0: 00 31 04 46  	add.s	$f4, $f6, $f4
  1a05f4: 00 00 02 46  	add.s	$f0, $f0, $f2
  1a05f8: 02 00 06 46  	mul.s	$f0, $f0, $f6
  1a05fc: 03 00 04 46  	div.s	$f0, $f0, $f4
  1a0600: 02 a0 00 46  	mul.s	$f0, $f20, $f0
  1a0604: 00 a5 00 46  	add.s	$f20, $f20, $f0
  1a0608: 02 00 42 2a  	slti	$2, $18, 0x2
  1a060c: 01 00 40 50  	beqzl	$2, 0x1a0614 <.text+0xa0614>
  1a0610: 07 a5 00 46  	neg.s	$f20, $f20
  1a0614: 1c 00 02 3c  	lui	$2, 0x1c
  1a0618: 80 18 12 00  	sll	$3, $18, 0x2
  1a061c: 18 a7 42 24  	addiu	$2, $2, -0x58e8 <.text+0xffffffffffefa718>
  1a0620: 06 b3 00 46  	mov.s	$f12, $f22
  1a0624: 21 18 62 00  	addu	$3, $3, $2
  1a0628: 00 00 60 c4  	lwc1	$f0, 0x0($3)
  1a062c: d0 8c 06 0c  	jal	0x1a3340 <.text+0xa3340>
  1a0630: 00 a5 00 46  	add.s	$f20, $f20, $f0
  1a0634: 2d 20 40 00  	move	$4, $2
  1a0638: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a063c: 2d 28 00 00  	move	$5, $zero
  1a0640: 01 00 42 04  	bltzl	$2, 0x1a0648 <.text+0xa0648>
  1a0644: 07 a5 00 46  	neg.s	$f20, $f20
  1a0648: 06 a0 00 46  	mov.s	$f0, $f20
  1a064c: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a0650: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a0654: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a0658: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a065c: 44 00 b6 c7  	lwc1	$f22, 0x44($sp)
  1a0660: 40 00 b4 c7  	lwc1	$f20, 0x40($sp)
  1a0664: 08 00 e0 03  	jr	$ra
  1a0668: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a066c: 21 00 03 24  	addiu	$3, $zero, 0x21
  1a0670: 42 00 02 3c  	lui	$2, 0x42
  1a0674: 06 b0 00 46  	mov.s	$f0, $f22
  1a0678: f4 ff 00 10  	b	0x1a064c <.text+0xa064c>
  1a067c: 70 5a 43 ac  	sw	$3, 0x5a70($2)
  1a0680: 00 00 80 44  	mtc1	$zero, $f0
  1a0684: f2 ff 60 10  	beqz	$3, 0x1a0650 <.text+0xa0650>
  1a0688: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a068c: 8a ff 00 10  	b	0x1a04b8 <.text+0xa04b8>
  1a0690: 05 b5 00 46  	abs.s	$f20, $f22
		...
  1a06a0: 08 00 e0 03  	jr	$ra
  1a06a4: 04 00 0c 46  	<unknown>
		...
  1a06b0: 08 00 e0 03  	jr	$ra
  1a06b4: 05 60 00 46  	abs.s	$f0, $f12
		...
  1a06c0: 00 58 80 44  	mtc1	$zero, $f11
  1a06c4: 00 00 00 00  	nop
  1a06c8: 32 60 0b 46  	c.eq.s	$f12, $f11
  1a06cc: 00 00 00 00  	nop
  1a06d0: 05 00 00 45  	bc1f	0x1a06e8 <.text+0xa06e8>
		...
  1a06dc: 08 00 e0 03  	jr	$ra
  1a06e0: 00 00 02 20  	addi	$2, $zero, 0x0
  1a06e4: 00 00 00 00  	nop
  1a06e8: 00 60 04 44  	mfc1	$4, $f12
  1a06ec: 00 00 00 00  	nop
  1a06f0: fa 2d 04 00  	dsrl	$5, $4, 0x17
  1a06f4: f8 07 06 20  	addi	$6, $zero, 0x7f8
  1a06f8: 24 28 a6 00  	and	$5, $5, $6
  1a06fc: 05 00 a6 10  	beq	$5, $6, 0x1a0714 <.text+0xa0714>
		...
  1a0708: 08 00 e0 03  	jr	$ra
  1a070c: 03 00 02 20  	addi	$2, $zero, 0x3
  1a0710: 00 00 00 00  	nop
  1a0714: 7f 00 05 3c  	lui	$5, 0x7f
  1a0718: ff ff a5 34  	ori	$5, $5, 0xffff
  1a071c: 24 20 85 00  	and	$4, $4, $5
  1a0720: 04 00 80 10  	beqz	$4, 0x1a0734 <.text+0xa0734>
  1a0724: 00 00 00 00  	nop
  1a0728: 08 00 e0 03  	jr	$ra
  1a072c: 02 00 02 20  	addi	$2, $zero, 0x2
  1a0730: 00 00 00 00  	nop
  1a0734: 08 00 e0 03  	jr	$ra
  1a0738: 01 00 02 20  	addi	$2, $zero, 0x1
  1a073c: 00 00 00 00  	nop
  1a0740: 44 00 02 3c  	lui	$2, 0x44
  1a0744: 00 6d 43 8c  	lw	$3, 0x6d00($2)
  1a0748: 00 20 02 3c  	lui	$2, 0x2000
  1a074c: 03 00 60 10  	beqz	$3, 0x1a075c <.text+0xa075c>
  1a0750: 25 20 82 00  	or	$4, $4, $2
  1a0754: 00 00 82 8c  	lw	$2, 0x0($4)
  1a0758: 00 00 62 ac  	sw	$2, 0x0($3)
  1a075c: 44 00 02 3c  	lui	$2, 0x44
  1a0760: 04 6d 43 8c  	lw	$3, 0x6d04($2)
  1a0764: 04 00 60 10  	beqz	$3, 0x1a0778 <.text+0xa0778>
  1a0768: 44 00 02 3c  	lui	$2, 0x44
  1a076c: 04 00 82 8c  	lw	$2, 0x4($4)
  1a0770: 00 00 62 ac  	sw	$2, 0x0($3)
  1a0774: 44 00 02 3c  	lui	$2, 0x44
  1a0778: 08 6d 45 8c  	lw	$5, 0x6d08($2)
  1a077c: 0d 00 a0 10  	beqz	$5, 0x1a07b4 <.text+0xa07b4>
  1a0780: 42 00 02 3c  	lui	$2, 0x42
  1a0784: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0788: 08 00 60 14  	bnez	$3, 0x1a07ac <.text+0xa07ac>
  1a078c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a0790: 00 00 82 8c  	lw	$2, 0x0($4)
  1a0794: 03 00 40 54  	bnezl	$2, 0x1a07a4 <.text+0xa07a4>
  1a0798: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a079c: 08 00 e0 03  	jr	$ra
  1a07a0: 00 00 a0 ac  	sw	$zero, 0x0($5)
  1a07a4: 08 00 e0 03  	jr	$ra
  1a07a8: 00 00 a2 ac  	sw	$2, 0x0($5)
  1a07ac: 03 00 62 10  	beq	$3, $2, 0x1a07bc <.text+0xa07bc>
  1a07b0: 00 00 00 00  	nop
  1a07b4: 08 00 e0 03  	jr	$ra
  1a07b8: 00 00 00 00  	nop
  1a07bc: 90 00 82 8c  	lw	$2, 0x90($4)
  1a07c0: 08 00 e0 03  	jr	$ra
  1a07c4: 00 00 a2 ac  	sw	$2, 0x0($5)
  1a07c8: 00 20 02 3c  	lui	$2, 0x2000
  1a07cc: 25 30 82 00  	or	$6, $4, $2
  1a07d0: 08 00 c7 8c  	lw	$7, 0x8($6)
  1a07d4: 0d 00 e0 10  	beqz	$7, 0x1a080c <.text+0xa080c>
  1a07d8: 10 00 c8 24  	addiu	$8, $6, 0x10
  1a07dc: 00 00 c2 8c  	lw	$2, 0x0($6)
  1a07e0: 0a 00 40 18  	blez	$2, 0x1a080c <.text+0xa080c>
  1a07e4: 2d 20 00 00  	move	$4, $zero
  1a07e8: 21 10 04 01  	addu	$2, $8, $4
  1a07ec: 21 18 e4 00  	addu	$3, $7, $4
  1a07f0: 00 00 42 90  	lbu	$2, 0x0($2)
  1a07f4: 01 00 84 24  	addiu	$4, $4, 0x1
  1a07f8: 00 00 62 a0  	sb	$2, 0x0($3)
  1a07fc: 00 00 c2 8c  	lw	$2, 0x0($6)
  1a0800: 2a 10 82 00  	slt	$2, $4, $2
  1a0804: f9 ff 40 14  	bnez	$2, 0x1a07ec <.text+0xa07ec>
  1a0808: 21 10 04 01  	addu	$2, $8, $4
  1a080c: 42 00 02 3c  	lui	$2, 0x42
  1a0810: 50 00 c4 24  	addiu	$4, $6, 0x50
  1a0814: d0 5a 42 8c  	lw	$2, 0x5ad0($2)
  1a0818: 20 00 c5 24  	addiu	$5, $6, 0x20
  1a081c: 0c 00 c7 8c  	lw	$7, 0xc($6)
  1a0820: 01 00 43 38  	xori	$3, $2, 0x1
  1a0824: 00 00 42 38  	xori	$2, $2, 0x0
  1a0828: 0a 40 83 00  	movz	$8, $4, $3
  1a082c: 0d 00 e0 10  	beqz	$7, 0x1a0864 <.text+0xa0864>
  1a0830: 0a 40 a2 00  	movz	$8, $5, $2
  1a0834: 04 00 c2 8c  	lw	$2, 0x4($6)
  1a0838: 0a 00 40 18  	blez	$2, 0x1a0864 <.text+0xa0864>
  1a083c: 2d 20 00 00  	move	$4, $zero
  1a0840: 21 10 04 01  	addu	$2, $8, $4
  1a0844: 21 18 e4 00  	addu	$3, $7, $4
  1a0848: 00 00 42 90  	lbu	$2, 0x0($2)
  1a084c: 01 00 84 24  	addiu	$4, $4, 0x1
  1a0850: 00 00 62 a0  	sb	$2, 0x0($3)
  1a0854: 04 00 c2 8c  	lw	$2, 0x4($6)
  1a0858: 2a 10 82 00  	slt	$2, $4, $2
  1a085c: f9 ff 40 14  	bnez	$2, 0x1a0844 <.text+0xa0844>
  1a0860: 21 10 04 01  	addu	$2, $8, $4
  1a0864: 08 00 e0 03  	jr	$ra
  1a0868: 00 00 00 00  	nop
  1a086c: 44 00 02 3c  	lui	$2, 0x44
  1a0870: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a0874: d0 6d 42 24  	addiu	$2, $2, 0x6dd0
  1a0878: 00 20 03 3c  	lui	$3, 0x2000
  1a087c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a0880: 25 88 43 00  	or	$17, $2, $3
  1a0884: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a0888: 2d 90 80 00  	move	$18, $4
  1a088c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a0890: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a0894: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  1a0898: 2d 20 20 02  	move	$4, $17
  1a089c: 2d 80 40 00  	move	$16, $2
  1a08a0: 00 04 42 28  	slti	$2, $2, 0x400
  1a08a4: 0d 00 40 10  	beqz	$2, 0x1a08dc <.text+0xa08dc>
  1a08a8: ff 03 24 26  	addiu	$4, $17, 0x3ff
  1a08ac: 2d 20 40 02  	move	$4, $18
  1a08b0: 2d 28 20 02  	move	$5, $17
  1a08b4: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  1a08b8: 2d 30 00 02  	move	$6, $16
  1a08bc: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a08c0: 21 10 30 02  	addu	$2, $17, $16
  1a08c4: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a08c8: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a08cc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a08d0: 00 00 40 a0  	sb	$zero, 0x0($2)
  1a08d4: 08 00 e0 03  	jr	$ra
  1a08d8: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a08dc: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  1a08e0: 00 00 00 00  	nop
  1a08e4: f1 ff 00 10  	b	0x1a08ac <.text+0xa08ac>
  1a08e8: 2d 80 40 00  	move	$16, $2
  1a08ec: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a08f0: 42 00 02 3c  	lui	$2, 0x42
  1a08f4: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a08f8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a08fc: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a0900: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a0904: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0908: 07 00 40 10  	beqz	$2, 0x1a0928 <.text+0xa0928>
  1a090c: 2d 80 80 00  	move	$16, $4
  1a0910: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a0914: 2d 10 60 00  	move	$2, $3
  1a0918: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a091c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0920: 08 00 e0 03  	jr	$ra
  1a0924: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a0928: 03 73 06 0c  	jal	0x19cc0c <.text+0x9cc0c>
  1a092c: 2d 20 00 00  	move	$4, $zero
  1a0930: 42 00 02 3c  	lui	$2, 0x42
  1a0934: d0 5a 50 ac  	sw	$16, 0x5ad0($2)
  1a0938: 44 00 02 3c  	lui	$2, 0x44
  1a093c: 00 80 05 3c  	lui	$5, 0x8000
  1a0940: c0 64 51 24  	addiu	$17, $2, 0x64c0
  1a0944: 00 04 a5 34  	ori	$5, $5, 0x400
  1a0948: 2d 20 20 02  	move	$4, $17
  1a094c: a2 71 06 0c  	jal	0x19c688 <.text+0x9c688>
  1a0950: 2d 30 00 00  	move	$6, $zero
  1a0954: 2d 48 40 00  	move	$9, $2
  1a0958: ed ff 40 04  	bltz	$2, 0x1a0910 <.text+0xa0910>
  1a095c: 2d 18 40 00  	move	$3, $2
  1a0960: 24 00 22 8e  	lw	$2, 0x24($17)
  1a0964: 0f 00 03 3c  	lui	$3, 0xf
  1a0968: 0f 00 40 14  	bnez	$2, 0x1a09a8 <.text+0xa09a8>
  1a096c: ff ff 63 34  	ori	$3, $3, 0xffff
		...
  1a0984: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  1a0988: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
		...
  1a0998: f5 ff 62 14  	bne	$3, $2, 0x1a0970 <.text+0xa0970>
  1a099c: 44 00 02 3c  	lui	$2, 0x44
  1a09a0: e7 ff 00 10  	b	0x1a0940 <.text+0xa0940>
  1a09a4: 00 80 05 3c  	lui	$5, 0x8000
  1a09a8: 42 00 02 3c  	lui	$2, 0x42
  1a09ac: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a09b0: 04 00 60 10  	beqz	$3, 0x1a09c4 <.text+0xa09c4>
  1a09b4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a09b8: 08 00 62 10  	beq	$3, $2, 0x1a09dc <.text+0xa09dc>
  1a09bc: 44 00 02 3c  	lui	$2, 0x44
  1a09c0: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a09c4: 42 00 03 3c  	lui	$3, 0x42
  1a09c8: c8 5a 62 ac  	sw	$2, 0x5ac8($3)
  1a09cc: 2d 18 20 01  	move	$3, $9
  1a09d0: 42 00 02 3c  	lui	$2, 0x42
  1a09d4: ce ff 00 10  	b	0x1a0910 <.text+0xa0910>
  1a09d8: cc 5a 40 ac  	sw	$zero, 0x5acc($2)
  1a09dc: 44 00 07 3c  	lui	$7, 0x44
  1a09e0: 00 65 50 24  	addiu	$16, $2, 0x6500
  1a09e4: 2d 20 20 02  	move	$4, $17
  1a09e8: 1c 00 02 3c  	lui	$2, 0x1c
  1a09ec: 40 72 e7 24  	addiu	$7, $7, 0x7240
  1a09f0: 9c a7 45 8c  	lw	$5, -0x5864($2)
  1a09f4: 2d 30 00 00  	move	$6, $zero
  1a09f8: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a09fc: 2d 48 00 02  	move	$9, $16
  1a0a00: 0c 00 0a 24  	addiu	$10, $zero, 0xc
  1a0a04: 2d 58 00 00  	move	$11, $zero
  1a0a08: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0a0c: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0a10: 09 00 40 14  	bnez	$2, 0x1a0a38 <.text+0xa0a38>
  1a0a14: 9c ff 43 24  	addiu	$3, $2, -0x64 <.text+0xffffffffffefff9c>
  1a0a18: 04 00 02 26  	addiu	$2, $16, 0x4
  1a0a1c: 00 20 03 3c  	lui	$3, 0x2000
  1a0a20: 25 10 43 00  	or	$2, $2, $3
  1a0a24: 00 00 49 8c  	lw	$9, 0x0($2)
  1a0a28: 05 02 22 29  	slti	$2, $9, 0x205
  1a0a2c: 05 00 40 10  	beqz	$2, 0x1a0a44 <.text+0xa0a44>
  1a0a30: 08 00 02 26  	addiu	$2, $16, 0x8
  1a0a34: 88 ff 03 24  	addiu	$3, $zero, -0x78 <.text+0xffffffffffefff88>
  1a0a38: 42 00 02 3c  	lui	$2, 0x42
  1a0a3c: b4 ff 00 10  	b	0x1a0910 <.text+0xa0910>
  1a0a40: c8 5a 40 ac  	sw	$zero, 0x5ac8($2)
  1a0a44: 25 10 62 00  	or	$2, $3, $2
  1a0a48: 00 00 49 8c  	lw	$9, 0x0($2)
  1a0a4c: 06 02 22 29  	slti	$2, $9, 0x206
  1a0a50: 03 00 40 10  	beqz	$2, 0x1a0a60 <.text+0xa0a60>
  1a0a54: 25 10 03 02  	or	$2, $16, $3
  1a0a58: f7 ff 00 10  	b	0x1a0a38 <.text+0xa0a38>
  1a0a5c: 87 ff 03 24  	addiu	$3, $zero, -0x79 <.text+0xffffffffffefff87>
  1a0a60: d7 ff 00 10  	b	0x1a09c0 <.text+0xa09c0>
  1a0a64: 00 00 49 8c  	lw	$9, 0x0($2)
  1a0a68: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a0a6c: 42 00 02 3c  	lui	$2, 0x42
  1a0a70: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a0a74: 2d 50 80 00  	move	$10, $4
  1a0a78: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0a7c: 2d 58 a0 00  	move	$11, $5
  1a0a80: 2d 48 c0 00  	move	$9, $6
  1a0a84: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0a88: 3b 00 40 10  	beqz	$2, 0x1a0b78 <.text+0xa0b78>
  1a0a8c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0a90: 42 00 02 3c  	lui	$2, 0x42
  1a0a94: cc 5a 42 8c  	lw	$2, 0x5acc($2)
  1a0a98: 37 00 40 14  	bnez	$2, 0x1a0b78 <.text+0xa0b78>
  1a0a9c: 2d 18 40 00  	move	$3, $2
  1a0aa0: 42 00 02 3c  	lui	$2, 0x42
  1a0aa4: d0 5a 42 8c  	lw	$2, 0x5ad0($2)
  1a0aa8: 38 00 40 14  	bnez	$2, 0x1a0b8c <.text+0xa0b8c>
  1a0aac: 44 00 03 3c  	lui	$3, 0x44
  1a0ab0: 2b 20 06 00  	sltu	$4, $zero, $6
  1a0ab4: c0 76 63 24  	addiu	$3, $3, 0x76c0
  1a0ab8: 2b 28 07 00  	sltu	$5, $zero, $7
  1a0abc: 2b 30 08 00  	sltu	$6, $zero, $8
  1a0ac0: 44 00 02 3c  	lui	$2, 0x44
  1a0ac4: 44 00 10 3c  	lui	$16, 0x44
  1a0ac8: 10 6d 42 24  	addiu	$2, $2, 0x6d10
  1a0acc: 04 00 6a ac  	sw	$10, 0x4($3)
  1a0ad0: 1c 00 62 ac  	sw	$2, 0x1c($3)
  1a0ad4: 10 6d 10 26  	addiu	$16, $16, 0x6d10
  1a0ad8: 44 00 02 3c  	lui	$2, 0x44
  1a0adc: 08 00 6b ac  	sw	$11, 0x8($3)
  1a0ae0: 0c 00 64 ac  	sw	$4, 0xc($3)
  1a0ae4: 2d 20 00 02  	move	$4, $16
  1a0ae8: 10 00 65 ac  	sw	$5, 0x10($3)
  1a0aec: c0 00 05 24  	addiu	$5, $zero, 0xc0
  1a0af0: 14 00 66 ac  	sw	$6, 0x14($3)
  1a0af4: 00 6d 49 ac  	sw	$9, 0x6d00($2)
  1a0af8: 44 00 02 3c  	lui	$2, 0x44
  1a0afc: 04 6d 47 ac  	sw	$7, 0x6d04($2)
  1a0b00: 44 00 02 3c  	lui	$2, 0x44
  1a0b04: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  1a0b08: 08 6d 48 ac  	sw	$8, 0x6d08($2)
  1a0b0c: 42 00 02 3c  	lui	$2, 0x42
  1a0b10: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0b14: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0b18: 44 00 07 3c  	lui	$7, 0x44
  1a0b1c: 1c 00 02 3c  	lui	$2, 0x1c
  1a0b20: 44 00 09 3c  	lui	$9, 0x44
  1a0b24: 18 28 64 00  	<unknown>
  1a0b28: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0b2c: 44 00 04 3c  	lui	$4, 0x44
  1a0b30: 1a 00 0b 3c  	lui	$11, 0x1a
  1a0b34: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0b38: c0 76 e7 24  	addiu	$7, $7, 0x76c0
  1a0b3c: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0b40: 40 07 6b 25  	addiu	$11, $11, 0x740
  1a0b44: 21 18 a2 00  	addu	$3, $5, $2
  1a0b48: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0b4c: 04 00 65 8c  	lw	$5, 0x4($3)
  1a0b50: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0b54: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0b58: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0b5c: 00 00 b0 af  	sw	$16, 0x0($sp)
  1a0b60: 05 00 40 14  	bnez	$2, 0x1a0b78 <.text+0xa0b78>
  1a0b64: 2d 18 40 00  	move	$3, $2
  1a0b68: 01 00 03 24  	addiu	$3, $zero, 0x1
  1a0b6c: 42 00 02 3c  	lui	$2, 0x42
  1a0b70: cc 5a 43 ac  	sw	$3, 0x5acc($2)
  1a0b74: 2d 18 00 00  	move	$3, $zero
  1a0b78: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a0b7c: 2d 10 60 00  	move	$2, $3
  1a0b80: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0b84: 08 00 e0 03  	jr	$ra
  1a0b88: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a0b8c: 2b 20 08 00  	sltu	$4, $zero, $8
  1a0b90: c0 76 63 24  	addiu	$3, $3, 0x76c0
  1a0b94: 2b 28 07 00  	sltu	$5, $zero, $7
  1a0b98: c9 ff 00 10  	b	0x1a0ac0 <.text+0xa0ac0>
  1a0b9c: 2b 30 06 00  	sltu	$6, $zero, $6
  1a0ba0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a0ba4: 42 00 02 3c  	lui	$2, 0x42
  1a0ba8: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a0bac: 2d 48 a0 00  	move	$9, $5
  1a0bb0: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a0bb4: 2d 40 80 00  	move	$8, $4
  1a0bb8: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0bbc: 2d 28 c0 00  	move	$5, $6
  1a0bc0: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0bc4: 05 00 40 10  	beqz	$2, 0x1a0bdc <.text+0xa0bdc>
  1a0bc8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0bcc: 42 00 11 3c  	lui	$17, 0x42
  1a0bd0: cc 5a 22 8e  	lw	$2, 0x5acc($17)
  1a0bd4: 07 00 40 10  	beqz	$2, 0x1a0bf4 <.text+0xa0bf4>
  1a0bd8: 2d 18 40 00  	move	$3, $2
  1a0bdc: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a0be0: 2d 10 60 00  	move	$2, $3
  1a0be4: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a0be8: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0bec: 08 00 e0 03  	jr	$ra
  1a0bf0: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a0bf4: 44 00 02 3c  	lui	$2, 0x44
  1a0bf8: ff 03 06 24  	addiu	$6, $zero, 0x3ff
  1a0bfc: 40 72 50 24  	addiu	$16, $2, 0x7240
  1a0c00: 40 72 48 ac  	sw	$8, 0x7240($2)
  1a0c04: 14 00 04 26  	addiu	$4, $16, 0x14
  1a0c08: 04 00 09 ae  	sw	$9, 0x4($16)
  1a0c0c: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1a0c10: 08 00 07 ae  	sw	$7, 0x8($16)
  1a0c14: 42 00 02 3c  	lui	$2, 0x42
  1a0c18: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0c1c: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0c20: 44 00 09 3c  	lui	$9, 0x44
  1a0c24: 1c 00 02 3c  	lui	$2, 0x1c
  1a0c28: 2d 38 00 02  	move	$7, $16
  1a0c2c: 18 28 64 00  	<unknown>
  1a0c30: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0c34: 44 00 04 3c  	lui	$4, 0x44
  1a0c38: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0c3c: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0c40: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0c44: 14 04 08 24  	addiu	$8, $zero, 0x414
  1a0c48: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0c4c: 21 18 a2 00  	addu	$3, $5, $2
  1a0c50: 2d 58 00 00  	move	$11, $zero
  1a0c54: 08 00 65 8c  	lw	$5, 0x8($3)
  1a0c58: 13 04 00 a2  	sb	$zero, 0x413($16)
  1a0c5c: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0c60: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0c64: dd ff 40 14  	bnez	$2, 0x1a0bdc <.text+0xa0bdc>
  1a0c68: 2d 18 40 00  	move	$3, $2
  1a0c6c: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a0c70: 2d 18 00 00  	move	$3, $zero
  1a0c74: d9 ff 00 10  	b	0x1a0bdc <.text+0xa0bdc>
  1a0c78: cc 5a 22 ae  	sw	$2, 0x5acc($17)
  1a0c7c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a0c80: 42 00 02 3c  	lui	$2, 0x42
  1a0c84: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a0c88: 2d 68 80 00  	move	$13, $4
  1a0c8c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0c90: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0c94: 05 00 40 10  	beqz	$2, 0x1a0cac <.text+0xa0cac>
  1a0c98: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0c9c: 42 00 10 3c  	lui	$16, 0x42
  1a0ca0: cc 5a 02 8e  	lw	$2, 0x5acc($16)
  1a0ca4: 06 00 40 10  	beqz	$2, 0x1a0cc0 <.text+0xa0cc0>
  1a0ca8: 2d 18 40 00  	move	$3, $2
  1a0cac: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a0cb0: 2d 10 60 00  	move	$2, $3
  1a0cb4: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0cb8: 08 00 e0 03  	jr	$ra
  1a0cbc: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a0cc0: 42 00 02 3c  	lui	$2, 0x42
  1a0cc4: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0cc8: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0ccc: 44 00 0c 3c  	lui	$12, 0x44
  1a0cd0: 1c 00 02 3c  	lui	$2, 0x1c
  1a0cd4: 44 00 09 3c  	lui	$9, 0x44
  1a0cd8: 18 28 64 00  	<unknown>
  1a0cdc: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0ce0: 44 00 04 3c  	lui	$4, 0x44
  1a0ce4: 80 76 87 25  	addiu	$7, $12, 0x7680
  1a0ce8: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0cec: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0cf0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0cf4: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0cf8: 21 18 a2 00  	addu	$3, $5, $2
  1a0cfc: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0d00: 0c 00 65 8c  	lw	$5, 0xc($3)
  1a0d04: 2d 58 00 00  	move	$11, $zero
  1a0d08: 80 76 8d ad  	sw	$13, 0x7680($12)
  1a0d0c: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0d10: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0d14: e5 ff 40 14  	bnez	$2, 0x1a0cac <.text+0xa0cac>
  1a0d18: 2d 18 40 00  	move	$3, $2
  1a0d1c: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a0d20: 2d 18 00 00  	move	$3, $zero
  1a0d24: e1 ff 00 10  	b	0x1a0cac <.text+0xa0cac>
  1a0d28: cc 5a 02 ae  	sw	$2, 0x5acc($16)
  1a0d2c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a0d30: 42 00 02 3c  	lui	$2, 0x42
  1a0d34: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a0d38: 2d 78 80 00  	move	$15, $4
  1a0d3c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0d40: 2d 68 a0 00  	move	$13, $5
  1a0d44: 2d 70 c0 00  	move	$14, $6
  1a0d48: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0d4c: 05 00 40 10  	beqz	$2, 0x1a0d64 <.text+0xa0d64>
  1a0d50: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0d54: 42 00 10 3c  	lui	$16, 0x42
  1a0d58: cc 5a 02 8e  	lw	$2, 0x5acc($16)
  1a0d5c: 06 00 40 10  	beqz	$2, 0x1a0d78 <.text+0xa0d78>
  1a0d60: 2d 18 40 00  	move	$3, $2
  1a0d64: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a0d68: 2d 10 60 00  	move	$2, $3
  1a0d6c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0d70: 08 00 e0 03  	jr	$ra
  1a0d74: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a0d78: 42 00 02 3c  	lui	$2, 0x42
  1a0d7c: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0d80: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0d84: 44 00 0c 3c  	lui	$12, 0x44
  1a0d88: 1c 00 02 3c  	lui	$2, 0x1c
  1a0d8c: 44 00 09 3c  	lui	$9, 0x44
  1a0d90: 18 28 64 00  	<unknown>
  1a0d94: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0d98: 44 00 04 3c  	lui	$4, 0x44
  1a0d9c: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0da0: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0da4: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0da8: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0dac: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0db0: 21 18 a2 00  	addu	$3, $5, $2
  1a0db4: 2d 58 00 00  	move	$11, $zero
  1a0db8: 80 76 82 25  	addiu	$2, $12, 0x7680
  1a0dbc: 10 00 65 8c  	lw	$5, 0x10($3)
  1a0dc0: 10 00 4d ac  	sw	$13, 0x10($2)
  1a0dc4: 2d 38 40 00  	move	$7, $2
  1a0dc8: 14 00 4e ac  	sw	$14, 0x14($2)
  1a0dcc: 80 76 8f ad  	sw	$15, 0x7680($12)
  1a0dd0: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0dd4: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0dd8: e2 ff 40 14  	bnez	$2, 0x1a0d64 <.text+0xa0d64>
  1a0ddc: 2d 18 40 00  	move	$3, $2
  1a0de0: 04 00 02 24  	addiu	$2, $zero, 0x4
  1a0de4: 2d 18 00 00  	move	$3, $zero
  1a0de8: de ff 00 10  	b	0x1a0d64 <.text+0xa0d64>
  1a0dec: cc 5a 02 ae  	sw	$2, 0x5acc($16)
  1a0df0: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a0df4: 42 00 02 3c  	lui	$2, 0x42
  1a0df8: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1a0dfc: 2d 40 80 00  	move	$8, $4
  1a0e00: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a0e04: 2d 38 a0 00  	move	$7, $5
  1a0e08: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a0e0c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a0e10: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0e14: 05 00 40 10  	beqz	$2, 0x1a0e2c <.text+0xa0e2c>
  1a0e18: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0e1c: 42 00 12 3c  	lui	$18, 0x42
  1a0e20: cc 5a 42 8e  	lw	$2, 0x5acc($18)
  1a0e24: 08 00 40 10  	beqz	$2, 0x1a0e48 <.text+0xa0e48>
  1a0e28: 2d 18 40 00  	move	$3, $2
  1a0e2c: 40 00 bf df  	ld	$ra, 0x40($sp)
  1a0e30: 2d 10 60 00  	move	$2, $3
  1a0e34: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a0e38: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a0e3c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a0e40: 08 00 e0 03  	jr	$ra
  1a0e44: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a0e48: 44 00 02 3c  	lui	$2, 0x44
  1a0e4c: 44 00 11 3c  	lui	$17, 0x44
  1a0e50: 80 76 50 24  	addiu	$16, $2, 0x7680
  1a0e54: 2d 28 c0 00  	move	$5, $6
  1a0e58: 2d 20 e0 00  	move	$4, $7
  1a0e5c: 10 6d 31 26  	addiu	$17, $17, 0x6d10
  1a0e60: 80 76 48 ac  	sw	$8, 0x7680($2)
  1a0e64: 0c 00 06 ae  	sw	$6, 0xc($16)
  1a0e68: 18 00 07 ae  	sw	$7, 0x18($16)
  1a0e6c: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  1a0e70: 1c 00 11 ae  	sw	$17, 0x1c($16)
  1a0e74: 2d 20 20 02  	move	$4, $17
  1a0e78: c4 73 06 0c  	jal	0x19cf10 <.text+0x9cf10>
  1a0e7c: c0 00 05 24  	addiu	$5, $zero, 0xc0
  1a0e80: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0e84: 42 00 02 3c  	lui	$2, 0x42
  1a0e88: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0e8c: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0e90: 44 00 09 3c  	lui	$9, 0x44
  1a0e94: 1c 00 02 3c  	lui	$2, 0x1c
  1a0e98: 1a 00 0b 3c  	lui	$11, 0x1a
  1a0e9c: 18 28 64 00  	<unknown>
  1a0ea0: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0ea4: 44 00 04 3c  	lui	$4, 0x44
  1a0ea8: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0eac: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0eb0: 2d 38 00 02  	move	$7, $16
  1a0eb4: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0eb8: c8 07 6b 25  	addiu	$11, $11, 0x7c8
  1a0ebc: 21 18 a2 00  	addu	$3, $5, $2
  1a0ec0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0ec4: 14 00 65 8c  	lw	$5, 0x14($3)
  1a0ec8: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0ecc: 00 00 b1 af  	sw	$17, 0x0($sp)
  1a0ed0: d6 ff 40 14  	bnez	$2, 0x1a0e2c <.text+0xa0e2c>
  1a0ed4: 2d 18 40 00  	move	$3, $2
  1a0ed8: 05 00 02 24  	addiu	$2, $zero, 0x5
  1a0edc: 2d 18 00 00  	move	$3, $zero
  1a0ee0: d2 ff 00 10  	b	0x1a0e2c <.text+0xa0e2c>
  1a0ee4: cc 5a 42 ae  	sw	$2, 0x5acc($18)
  1a0ee8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a0eec: 42 00 02 3c  	lui	$2, 0x42
  1a0ef0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a0ef4: c8 5a 42 8c  	lw	$2, 0x5ac8($2)
  1a0ef8: 37 00 40 10  	beqz	$2, 0x1a0fd8 <.text+0xa0fd8>
  1a0efc: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a0f00: 42 00 02 3c  	lui	$2, 0x42
  1a0f04: cc 5a 42 8c  	lw	$2, 0x5acc($2)
  1a0f08: 33 00 40 14  	bnez	$2, 0x1a0fd8 <.text+0xa0fd8>
  1a0f0c: 2d 18 40 00  	move	$3, $2
  1a0f10: 44 00 02 3c  	lui	$2, 0x44
  1a0f14: 11 00 c3 28  	slti	$3, $6, 0x11
  1a0f18: 80 76 44 ac  	sw	$4, 0x7680($2)
  1a0f1c: 32 00 60 10  	beqz	$3, 0x1a0fe8 <.text+0xa0fe8>
  1a0f20: 80 76 47 24  	addiu	$7, $2, 0x7680
  1a0f24: 14 00 e6 ac  	sw	$6, 0x14($7)
  1a0f28: 18 00 e0 ac  	sw	$zero, 0x18($7)
  1a0f2c: 0c 00 e0 ac  	sw	$zero, 0xc($7)
  1a0f30: 44 00 07 3c  	lui	$7, 0x44
  1a0f34: 80 76 e2 24  	addiu	$2, $7, 0x7680
  1a0f38: 14 00 42 8c  	lw	$2, 0x14($2)
  1a0f3c: 0a 00 40 18  	blez	$2, 0x1a0f68 <.text+0xa0f68>
  1a0f40: 2d 30 00 00  	move	$6, $zero
  1a0f44: 80 76 e2 24  	addiu	$2, $7, 0x7680
  1a0f48: 21 20 a6 00  	addu	$4, $5, $6
  1a0f4c: 14 00 43 8c  	lw	$3, 0x14($2)
  1a0f50: 21 10 c2 00  	addu	$2, $6, $2
  1a0f54: 00 00 84 90  	lbu	$4, 0x0($4)
  1a0f58: 01 00 c6 24  	addiu	$6, $6, 0x1
  1a0f5c: 2a 18 c3 00  	slt	$3, $6, $3
  1a0f60: f8 ff 60 14  	bnez	$3, 0x1a0f44 <.text+0xa0f44>
  1a0f64: 20 00 44 a0  	sb	$4, 0x20($2)
  1a0f68: ac 73 06 0c  	jal	0x19ceb0 <.text+0x9ceb0>
  1a0f6c: 2d 20 00 00  	move	$4, $zero
  1a0f70: 04 00 0a 24  	addiu	$10, $zero, 0x4
  1a0f74: 42 00 02 3c  	lui	$2, 0x42
  1a0f78: 44 00 04 24  	addiu	$4, $zero, 0x44
  1a0f7c: d0 5a 43 8c  	lw	$3, 0x5ad0($2)
  1a0f80: 44 00 07 3c  	lui	$7, 0x44
  1a0f84: 1c 00 02 3c  	lui	$2, 0x1c
  1a0f88: 44 00 09 3c  	lui	$9, 0x44
  1a0f8c: 18 28 64 00  	<unknown>
  1a0f90: 58 a7 42 24  	addiu	$2, $2, -0x58a8 <.text+0xffffffffffefa758>
  1a0f94: 44 00 04 3c  	lui	$4, 0x44
  1a0f98: 2d 58 00 00  	move	$11, $zero
  1a0f9c: c0 64 84 24  	addiu	$4, $4, 0x64c0
  1a0fa0: 80 76 e7 24  	addiu	$7, $7, 0x7680
  1a0fa4: 00 65 29 25  	addiu	$9, $9, 0x6500
  1a0fa8: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a0fac: 21 18 a2 00  	addu	$3, $5, $2
  1a0fb0: 30 00 08 24  	addiu	$8, $zero, 0x30
  1a0fb4: 18 00 65 8c  	lw	$5, 0x18($3)
  1a0fb8: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a0fbc: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a0fc0: 05 00 40 14  	bnez	$2, 0x1a0fd8 <.text+0xa0fd8>
  1a0fc4: 2d 18 40 00  	move	$3, $2
  1a0fc8: 06 00 03 24  	addiu	$3, $zero, 0x6
  1a0fcc: 42 00 02 3c  	lui	$2, 0x42
  1a0fd0: cc 5a 43 ac  	sw	$3, 0x5acc($2)
  1a0fd4: 2d 18 00 00  	move	$3, $zero
  1a0fd8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a0fdc: 2d 10 60 00  	move	$2, $3
  1a0fe0: 08 00 e0 03  	jr	$ra
  1a0fe4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a0fe8: f0 ff 03 24  	addiu	$3, $zero, -0x10 <.text+0xffffffffffeffff0>
  1a0fec: ff ff a2 24  	addiu	$2, $5, -0x1 <.text+0xffffffffffefffff>
  1a0ff0: 24 10 43 00  	and	$2, $2, $3
  1a0ff4: 23 20 45 00  	subu	$4, $2, $5
  1a0ff8: 23 10 a2 00  	subu	$2, $5, $2
  1a0ffc: 21 18 a4 00  	addu	$3, $5, $4
