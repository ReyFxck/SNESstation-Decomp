
build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  105750: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  105754: 43 00 08 3c  	lui	$8, 0x43
  105758: 00 00 b0 ff  	sd	$16, 0x0($sp)
  10575c: 36 00 04 3c  	lui	$4, 0x36
  105760: 43 00 10 3c  	lui	$16, 0x43
  105764: 43 00 05 3c  	lui	$5, 0x43
  105768: 28 9b 10 26  	addiu	$16, $16, -0x64d8 <.text+0xffffffffffef9b28>
  10576c: 43 00 06 3c  	lui	$6, 0x43
  105770: 2d 38 00 02  	move	$7, $16
  105774: 28 9f 08 25  	addiu	$8, $8, -0x60d8 <.text+0xffffffffffef9f28>
  105778: 28 b3 84 24  	addiu	$4, $4, -0x4cd8 <.text+0xffffffffffefb328>
  10577c: 20 97 a5 24  	addiu	$5, $5, -0x68e0 <.text+0xffffffffffef9720>
  105780: 10 00 bf ff  	sd	$ra, 0x10($sp)
  105784: ba 16 04 0c  	jal	0x105ae8 <.text+0x5ae8>
  105788: 28 97 c6 24  	addiu	$6, $6, -0x68d8 <.text+0xffffffffffef9728>
  10578c: 2d 28 00 02  	move	$5, $16
  105790: 43 00 10 3c  	lui	$16, 0x43
  105794: 1b 00 06 24  	addiu	$6, $zero, 0x1b
  105798: 00 97 10 26  	addiu	$16, $16, -0x6900 <.text+0xffffffffffef9700>
  10579c: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  1057a0: 2d 20 00 02  	move	$4, $16
  1057a4: 43 00 06 3c  	lui	$6, 0x43
  1057a8: 1b 00 02 3c  	lui	$2, 0x1b
  1057ac: 2d 28 00 02  	move	$5, $16
  1057b0: 98 0f 43 24  	addiu	$3, $2, 0xf98
  1057b4: 98 0f 47 dc  	ld	$7, 0xf98($2)
  1057b8: 0c 00 68 94  	lhu	$8, 0xc($3)
  1057bc: 00 93 d0 24  	addiu	$16, $6, -0x6d00 <.text+0xffffffffffef9300>
  1057c0: 08 00 62 8c  	lw	$2, 0x8($3)
  1057c4: 2d 20 00 02  	move	$4, $16
  1057c8: 0c 00 08 a6  	sh	$8, 0xc($16)
  1057cc: 08 00 02 ae  	sw	$2, 0x8($16)
  1057d0: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  1057d4: 00 93 c7 fc  	sd	$7, -0x6d00($6)
  1057d8: 1b 00 05 3c  	lui	$5, 0x1b
  1057dc: 2d 20 00 02  	move	$4, $16
  1057e0: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  1057e4: a8 0f a5 24  	addiu	$5, $5, 0xfa8
  1057e8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1057ec: 2d 10 00 02  	move	$2, $16
  1057f0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1057f4: 08 00 e0 03  	jr	$ra
  1057f8: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1057fc: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  105800: 43 00 08 3c  	lui	$8, 0x43
  105804: 00 00 b0 ff  	sd	$16, 0x0($sp)
  105808: 43 00 05 3c  	lui	$5, 0x43
  10580c: 43 00 10 3c  	lui	$16, 0x43
  105810: 20 00 b2 ff  	sd	$18, 0x20($sp)
  105814: 10 00 b1 ff  	sd	$17, 0x10($sp)
  105818: a8 ab 10 26  	addiu	$16, $16, -0x5458 <.text+0xffffffffffefaba8>
  10581c: 2d 90 80 00  	move	$18, $4
  105820: 43 00 06 3c  	lui	$6, 0x43
  105824: 36 00 04 3c  	lui	$4, 0x36
  105828: 43 00 11 3c  	lui	$17, 0x43
  10582c: a8 af 08 25  	addiu	$8, $8, -0x5058 <.text+0xffffffffffefafa8>
  105830: 2d 38 00 02  	move	$7, $16
  105834: 80 a7 31 26  	addiu	$17, $17, -0x5880 <.text+0xffffffffffefa780>
  105838: a0 a7 a5 24  	addiu	$5, $5, -0x5860 <.text+0xffffffffffefa7a0>
  10583c: a8 a7 c6 24  	addiu	$6, $6, -0x5858 <.text+0xffffffffffefa7a8>
  105840: 30 00 bf ff  	sd	$ra, 0x30($sp)
  105844: ba 16 04 0c  	jal	0x105ae8 <.text+0x5ae8>
  105848: 28 b3 84 24  	addiu	$4, $4, -0x4cd8 <.text+0xffffffffffefb328>
  10584c: 2d 28 00 02  	move	$5, $16
  105850: 2d 20 20 02  	move	$4, $17
  105854: 1b 00 06 24  	addiu	$6, $zero, 0x1b
  105858: 54 71 06 0c  	jal	0x19c550 <.text+0x9c550>
  10585c: 43 00 10 3c  	lui	$16, 0x43
  105860: 80 a3 10 26  	addiu	$16, $16, -0x5c80 <.text+0xffffffffffefa380>
  105864: 1b 00 05 3c  	lui	$5, 0x1b
  105868: 2d 38 40 02  	move	$7, $18
  10586c: 2d 30 20 02  	move	$6, $17
  105870: 2d 20 00 02  	move	$4, $16
  105874: f4 78 06 0c  	jal	0x19e3d0 <.text+0x9e3d0>
  105878: b0 0f a5 24  	addiu	$5, $5, 0xfb0
  10587c: 10 00 b1 df  	ld	$17, 0x10($sp)
  105880: 2d 10 00 02  	move	$2, $16
  105884: 30 00 bf df  	ld	$ra, 0x30($sp)
  105888: 20 00 b2 df  	ld	$18, 0x20($sp)
  10588c: 00 00 b0 df  	ld	$16, 0x0($sp)
  105890: 08 00 e0 03  	jr	$ra
  105894: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  105898: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  10589c: 34 00 02 3c  	lui	$2, 0x34
  1058a0: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1058a4: 1c 55 46 8c  	lw	$6, 0x551c($2)
  1058a8: c8 00 02 24  	addiu	$2, $zero, 0xc8
  1058ac: 36 00 c2 10  	beq	$6, $2, 0x105988 <.text+0x5988>
  1058b0: 35 00 02 3c  	lui	$2, 0x35
  1058b4: 00 80 03 34  	ori	$3, $zero, 0x8000
  1058b8: b0 e2 42 24  	addiu	$2, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  1058bc: 1c 00 05 3c  	lui	$5, 0x1c
  1058c0: 21 10 43 00  	addu	$2, $2, $3
  1058c4: 48 bd a4 dc  	ld	$4, -0x42b8($5)
  1058c8: 4c 30 42 8c  	lw	$2, 0x304c($2)
  1058cc: 2a 20 82 00  	slt	$4, $4, $2
  1058d0: 08 00 80 14  	bnez	$4, 0x1058f4 <.text+0x58f4>
  1058d4: 36 00 02 3c  	lui	$2, 0x36
  1058d8: 48 bd a0 fc  	sd	$zero, -0x42b8($5)
  1058dc: 1c 00 02 3c  	lui	$2, 0x1c
  1058e0: 54 bd 43 8c  	lw	$3, -0x42ac($2)
  1058e4: 54 bd 40 ac  	sw	$zero, -0x42ac($2)
  1058e8: 1c 00 02 3c  	lui	$2, 0x1c
  1058ec: 50 bd 43 ac  	sw	$3, -0x42b0($2)
  1058f0: 36 00 02 3c  	lui	$2, 0x36
  1058f4: 68 c2 44 24  	addiu	$4, $2, -0x3d98 <.text+0xffffffffffefc268>
  1058f8: 18 00 82 8c  	lw	$2, 0x18($4)
  1058fc: 01 00 42 24  	addiu	$2, $2, 0x1
  105900: 2b 18 46 00  	sltu	$3, $2, $6
  105904: 1b 00 60 14  	bnez	$3, 0x105974 <.text+0x5974>
  105908: 18 00 82 ac  	sw	$2, 0x18($4)
  10590c: 01 00 02 24  	addiu	$2, $zero, 0x1
  105910: 14 00 80 ac  	sw	$zero, 0x14($4)
  105914: 06 00 82 a0  	sb	$2, 0x6($4)
  105918: 18 00 80 ac  	sw	$zero, 0x18($4)
  10591c: 1f 00 02 3c  	lui	$2, 0x1f
  105920: 40 bd 43 90  	lbu	$3, -0x42c0($2)
  105924: 01 00 02 24  	addiu	$2, $zero, 0x1
  105928: 04 00 62 10  	beq	$3, $2, 0x10593c <.text+0x593c>
  10592c: 1f 00 02 3c  	lui	$2, 0x1f
  105930: 00 00 bf df  	ld	$ra, 0x0($sp)
  105934: 08 00 e0 03  	jr	$ra
  105938: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  10593c: 1f 00 04 3c  	lui	$4, 0x1f
  105940: e4 ba 45 8c  	lw	$5, -0x451c($2)
  105944: 5e d9 05 0c  	jal	0x176578 <.text+0x76578>
  105948: 80 ab 84 24  	addiu	$4, $4, -0x5480 <.text+0xffffffffffefab80>
  10594c: 1d 00 05 3c  	lui	$5, 0x1d
  105950: 1f 00 02 3c  	lui	$2, 0x1f
  105954: 1c 00 04 3c  	lui	$4, 0x1c
  105958: e0 ba 46 8c  	lw	$6, -0x4520($2)
  10595c: 01 00 07 24  	addiu	$7, $zero, 0x1
  105960: 80 bd 84 24  	addiu	$4, $4, -0x4280 <.text+0xffffffffffefbd80>
  105964: 3e 1e 04 0c  	jal	0x1078f8 <.text+0x78f8>
  105968: 80 34 a5 24  	addiu	$5, $5, 0x3480
  10596c: f1 ff 00 10  	b	0x105934 <.text+0x5934>
  105970: 00 00 bf df  	ld	$ra, 0x0($sp)
  105974: 14 00 82 8c  	lw	$2, 0x14($4)
  105978: 06 00 80 a0  	sb	$zero, 0x6($4)
  10597c: 01 00 42 24  	addiu	$2, $2, 0x1
  105980: e6 ff 00 10  	b	0x10591c <.text+0x591c>
  105984: 14 00 82 ac  	sw	$2, 0x14($4)
  105988: 1c 00 03 3c  	lui	$3, 0x1c
  10598c: 01 00 04 24  	addiu	$4, $zero, 0x1
  105990: 18 af 62 8c  	lw	$2, -0x50e8($3)
  105994: 05 00 44 10  	beq	$2, $4, 0x1059ac <.text+0x59ac>
  105998: 36 00 02 3c  	lui	$2, 0x36
  10599c: 68 c2 42 24  	addiu	$2, $2, -0x3d98 <.text+0xffffffffffefc268>
  1059a0: 06 00 44 a0  	sb	$4, 0x6($2)
  1059a4: dd ff 00 10  	b	0x10591c <.text+0x591c>
  1059a8: 14 00 40 ac  	sw	$zero, 0x14($2)
  1059ac: 18 af 60 ac  	sw	$zero, -0x50e8($3)
  1059b0: 36 00 03 3c  	lui	$3, 0x36
  1059b4: 68 c2 63 24  	addiu	$3, $3, -0x3d98 <.text+0xffffffffffefc268>
  1059b8: 14 00 62 8c  	lw	$2, 0x14($3)
  1059bc: 06 00 60 a0  	sb	$zero, 0x6($3)
  1059c0: 01 00 42 24  	addiu	$2, $2, 0x1
  1059c4: d5 ff 00 10  	b	0x10591c <.text+0x591c>
  1059c8: 14 00 62 ac  	sw	$2, 0x14($3)
  1059cc: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  1059d0: 40 00 b4 ff  	sd	$20, 0x40($sp)
  1059d4: 2d a0 e0 00  	move	$20, $7
  1059d8: 30 00 b3 ff  	sd	$19, 0x30($sp)
  1059dc: 2d 98 00 01  	move	$19, $8
  1059e0: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1059e4: 2d 90 c0 00  	move	$18, $6
  1059e8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1059ec: 2d 88 80 00  	move	$17, $4
  1059f0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1059f4: 2d 80 a0 00  	move	$16, $5
  1059f8: 04 00 a0 10  	beqz	$5, 0x105a0c <.text+0x5a0c>
  1059fc: 50 00 bf ff  	sd	$ra, 0x50($sp)
  105a00: 00 00 a2 80  	lb	$2, 0x0($5)
  105a04: 2f 00 40 14  	bnez	$2, 0x105ac4 <.text+0x5ac4>
  105a08: 00 00 00 00  	nop
  105a0c: 00 00 20 a2  	sb	$zero, 0x0($17)
  105a10: 04 00 40 12  	beqz	$18, 0x105a24 <.text+0x5a24>
  105a14: 2d 28 80 02  	move	$5, $20
  105a18: 00 00 42 82  	lb	$2, 0x0($18)
  105a1c: 17 00 40 54  	bnezl	$2, 0x105a7c <.text+0x5a7c>
  105a20: 2d 28 40 02  	move	$5, $18
  105a24: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  105a28: 2d 20 20 02  	move	$4, $17
  105a2c: 04 00 60 12  	beqz	$19, 0x105a40 <.text+0x5a40>
  105a30: 50 00 bf df  	ld	$ra, 0x50($sp)
  105a34: 00 00 62 82  	lb	$2, 0x0($19)
  105a38: 08 00 40 14  	bnez	$2, 0x105a5c <.text+0x5a5c>
  105a3c: 1b 00 05 3c  	lui	$5, 0x1b
  105a40: 40 00 b4 df  	ld	$20, 0x40($sp)
  105a44: 30 00 b3 df  	ld	$19, 0x30($sp)
  105a48: 20 00 b2 df  	ld	$18, 0x20($sp)
  105a4c: 10 00 b1 df  	ld	$17, 0x10($sp)
  105a50: 00 00 b0 df  	ld	$16, 0x0($sp)
  105a54: 08 00 e0 03  	jr	$ra
  105a58: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  105a5c: 2d 20 20 02  	move	$4, $17
  105a60: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  105a64: c8 0f a5 24  	addiu	$5, $5, 0xfc8
  105a68: 2d 28 60 02  	move	$5, $19
  105a6c: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  105a70: 2d 20 20 02  	move	$4, $17
  105a74: f2 ff 00 10  	b	0x105a40 <.text+0x5a40>
  105a78: 50 00 bf df  	ld	$ra, 0x50($sp)
  105a7c: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  105a80: 2d 20 20 02  	move	$4, $17
  105a84: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  105a88: 2d 20 40 02  	move	$4, $18
  105a8c: 01 00 03 24  	addiu	$3, $zero, 0x1
  105a90: 07 00 43 50  	beql	$2, $3, 0x105ab0 <.text+0x5ab0>
  105a94: 00 00 43 82  	lb	$3, 0x0($18)
  105a98: 1b 00 05 3c  	lui	$5, 0x1b
  105a9c: 2d 20 20 02  	move	$4, $17
  105aa0: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  105aa4: b8 0e a5 24  	addiu	$5, $5, 0xeb8
  105aa8: de ff 00 10  	b	0x105a24 <.text+0x5a24>
  105aac: 2d 28 80 02  	move	$5, $20
  105ab0: 2f 00 02 24  	addiu	$2, $zero, 0x2f
  105ab4: f9 ff 62 54  	bnel	$3, $2, 0x105a9c <.text+0x5a9c>
  105ab8: 1b 00 05 3c  	lui	$5, 0x1b
  105abc: d9 ff 00 10  	b	0x105a24 <.text+0x5a24>
  105ac0: 2d 28 80 02  	move	$5, $20
  105ac4: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  105ac8: 00 00 00 00  	nop
  105acc: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  105ad0: 2d 20 00 02  	move	$4, $16
  105ad4: 3a 00 03 24  	addiu	$3, $zero, 0x3a
  105ad8: 21 10 22 02  	addu	$2, $17, $2
  105adc: 00 00 43 a0  	sb	$3, 0x0($2)
  105ae0: cb ff 00 10  	b	0x105a10 <.text+0x5a10>
  105ae4: 01 00 40 a0  	sb	$zero, 0x1($2)
  105ae8: 70 fb bd 27  	addiu	$sp, $sp, -0x490 <.text+0xffffffffffeffb70>
  105aec: 50 04 b5 ff  	sd	$21, 0x450($sp)
  105af0: 2d a8 a0 00  	move	$21, $5
  105af4: 2d 28 80 00  	move	$5, $4
  105af8: 80 04 bf ff  	sd	$ra, 0x480($sp)
  105afc: 2d 20 a0 03  	move	$4, $sp
  105b00: 70 04 b7 ff  	sd	$23, 0x470($sp)
  105b04: 60 04 b6 ff  	sd	$22, 0x460($sp)
  105b08: 2d b8 00 01  	move	$23, $8
  105b0c: 40 04 b4 ff  	sd	$20, 0x440($sp)
  105b10: 2d b0 e0 00  	move	$22, $7
  105b14: 30 04 b3 ff  	sd	$19, 0x430($sp)
  105b18: 2d 98 c0 00  	move	$19, $6
  105b1c: 20 04 b2 ff  	sd	$18, 0x420($sp)
  105b20: 10 04 b1 ff  	sd	$17, 0x410($sp)
  105b24: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  105b28: 00 04 b0 ff  	sd	$16, 0x400($sp)
  105b2c: 2d 20 a0 03  	move	$4, $sp
  105b30: 84 71 06 0c  	jal	0x19c610 <.text+0x9c610>
  105b34: 3a 00 05 24  	addiu	$5, $zero, 0x3a
  105b38: 02 00 40 10  	beqz	$2, 0x105b44 <.text+0x5b44>
  105b3c: 2d a0 40 00  	move	$20, $2
  105b40: 00 00 40 a0  	sb	$zero, 0x0($2)
  105b44: 2d 20 a0 02  	move	$4, $21
  105b48: 2d 28 a0 03  	move	$5, $sp
  105b4c: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  105b50: 01 00 91 26  	addiu	$17, $20, 0x1
  105b54: 2d 20 20 02  	move	$4, $17
  105b58: a9 7a 06 0c  	jal	0x19eaa4 <.text+0x9eaa4>
  105b5c: 2f 00 05 24  	addiu	$5, $zero, 0x2f
  105b60: 50 00 40 10  	beqz	$2, 0x105ca4 <.text+0x5ca4>
  105b64: 2d 80 40 00  	move	$16, $2
  105b68: 2d 20 20 02  	move	$4, $17
  105b6c: a9 7a 06 0c  	jal	0x19eaa4 <.text+0x9eaa4>
  105b70: 2e 00 05 24  	addiu	$5, $zero, 0x2e
  105b74: 04 00 40 10  	beqz	$2, 0x105b88 <.text+0x5b88>
  105b78: 2d 90 40 00  	move	$18, $2
  105b7c: 36 00 00 12  	beqz	$16, 0x105c58 <.text+0x5c58>
  105b80: 2b 10 50 00  	sltu	$2, $2, $16
  105b84: 0b 90 02 00  	movn	$18, $zero, $2
  105b88: 34 00 00 12  	beqz	$16, 0x105c5c <.text+0x5c5c>
  105b8c: 00 00 a2 82  	lb	$2, 0x0($21)
  105b90: 27 00 40 10  	beqz	$2, 0x105c30 <.text+0x5c30>
  105b94: 2f 00 02 24  	addiu	$2, $zero, 0x2f
  105b98: 01 00 83 82  	lb	$3, 0x1($20)
  105b9c: 24 00 62 10  	beq	$3, $2, 0x105c30 <.text+0x5c30>
  105ba0: 1b 00 02 3c  	lui	$2, 0x1b
  105ba4: 2d 20 60 02  	move	$4, $19
  105ba8: b8 0e 42 24  	addiu	$2, $2, 0xeb8
  105bac: 00 00 47 80  	lb	$7, 0x0($2)
  105bb0: 01 00 43 80  	lb	$3, 0x1($2)
  105bb4: 00 00 67 a2  	sb	$7, 0x0($19)
  105bb8: 01 00 63 a2  	sb	$3, 0x1($19)
  105bbc: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  105bc0: 2d 28 20 02  	move	$5, $17
  105bc4: 23 10 11 02  	subu	$2, $16, $17
  105bc8: 21 10 62 02  	addu	$2, $19, $2
  105bcc: 01 00 40 a0  	sb	$zero, 0x1($2)
  105bd0: 01 00 05 26  	addiu	$5, $16, 0x1
  105bd4: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  105bd8: 2d 20 c0 02  	move	$4, $22
  105bdc: 12 00 40 12  	beqz	$18, 0x105c28 <.text+0x5c28>
  105be0: 23 10 50 02  	subu	$2, $18, $16
  105be4: 01 00 45 26  	addiu	$5, $18, 0x1
  105be8: 21 10 c2 02  	addu	$2, $22, $2
  105bec: 2d 20 e0 02  	move	$4, $23
  105bf0: ff ff 40 a0  	sb	$zero, -0x1($2)
  105bf4: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  105bf8: 00 00 00 00  	nop
  105bfc: 80 04 bf df  	ld	$ra, 0x480($sp)
  105c00: 70 04 b7 df  	ld	$23, 0x470($sp)
  105c04: 60 04 b6 df  	ld	$22, 0x460($sp)
  105c08: 50 04 b5 df  	ld	$21, 0x450($sp)
  105c0c: 40 04 b4 df  	ld	$20, 0x440($sp)
  105c10: 30 04 b3 df  	ld	$19, 0x430($sp)
  105c14: 20 04 b2 df  	ld	$18, 0x420($sp)
  105c18: 10 04 b1 df  	ld	$17, 0x410($sp)
  105c1c: 00 04 b0 df  	ld	$16, 0x400($sp)
  105c20: 08 00 e0 03  	jr	$ra
  105c24: 90 04 bd 27  	addiu	$sp, $sp, 0x490
  105c28: f4 ff 00 10  	b	0x105bfc <.text+0x5bfc>
  105c2c: 00 00 e0 a2  	sb	$zero, 0x0($23)
  105c30: 2d 20 60 02  	move	$4, $19
  105c34: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  105c38: 2d 28 20 02  	move	$5, $17
  105c3c: 04 00 11 12  	beq	$16, $17, 0x105c50 <.text+0x5c50>
  105c40: 23 10 11 02  	subu	$2, $16, $17
  105c44: 21 10 62 02  	addu	$2, $19, $2
  105c48: e1 ff 00 10  	b	0x105bd0 <.text+0x5bd0>
  105c4c: 00 00 40 a0  	sb	$zero, 0x0($2)
  105c50: df ff 00 10  	b	0x105bd0 <.text+0x5bd0>
  105c54: 01 00 60 a2  	sb	$zero, 0x1($19)
  105c58: 00 00 a2 82  	lb	$2, 0x0($21)
  105c5c: 07 00 40 50  	beqzl	$2, 0x105c7c <.text+0x5c7c>
  105c60: 00 00 60 a2  	sb	$zero, 0x0($19)
  105c64: 1b 00 02 3c  	lui	$2, 0x1b
  105c68: b8 0e 42 24  	addiu	$2, $2, 0xeb8
  105c6c: 00 00 43 80  	lb	$3, 0x0($2)
  105c70: 01 00 44 80  	lb	$4, 0x1($2)
  105c74: 00 00 63 a2  	sb	$3, 0x0($19)
  105c78: 01 00 64 a2  	sb	$4, 0x1($19)
  105c7c: 2d 20 c0 02  	move	$4, $22
  105c80: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  105c84: 2d 28 20 02  	move	$5, $17
  105c88: e7 ff 40 12  	beqz	$18, 0x105c28 <.text+0x5c28>
  105c8c: 23 10 51 02  	subu	$2, $18, $17
  105c90: 01 00 45 26  	addiu	$5, $18, 0x1
  105c94: 21 10 c2 02  	addu	$2, $22, $2
  105c98: 2d 20 e0 02  	move	$4, $23
  105c9c: d5 ff 00 10  	b	0x105bf4 <.text+0x5bf4>
  105ca0: 00 00 40 a0  	sb	$zero, 0x0($2)
  105ca4: 2d 20 20 02  	move	$4, $17
  105ca8: a9 7a 06 0c  	jal	0x19eaa4 <.text+0x9eaa4>
  105cac: 2f 00 05 24  	addiu	$5, $zero, 0x2f
  105cb0: ad ff 00 10  	b	0x105b68 <.text+0x5b68>
  105cb4: 2d 80 40 00  	move	$16, $2
  105cb8: 3b 00 02 3c  	lui	$2, 0x3b
  105cbc: ff 00 a5 30  	andi	$5, $5, 0xff
  105cc0: 18 b7 43 24  	addiu	$3, $2, -0x48e8 <.text+0xffffffffffefb718>
  105cc4: 01 00 02 24  	addiu	$2, $zero, 0x1
  105cc8: 18 00 65 ac  	sw	$5, 0x18($3)
  105ccc: 1c 00 62 a0  	sb	$2, 0x1c($3)
  105cd0: 02 00 02 24  	addiu	$2, $zero, 0x2
  105cd4: 0f 00 82 10  	beq	$4, $2, 0x105d14 <.text+0x5d14>
  105cd8: c0 5d 02 24  	addiu	$2, $zero, 0x5dc0
  105cdc: 03 00 82 28  	slti	$2, $4, 0x3
  105ce0: 0f 00 40 10  	beqz	$2, 0x105d20 <.text+0x5d20>
  105ce4: 03 00 02 24  	addiu	$2, $zero, 0x3
  105ce8: 01 00 02 24  	addiu	$2, $zero, 0x1
  105cec: 09 00 82 50  	beql	$4, $2, 0x105d14 <.text+0x5d14>
  105cf0: e0 2e 02 24  	addiu	$2, $zero, 0x2ee0
  105cf4: 3b 00 02 3c  	lui	$2, 0x3b
  105cf8: 18 b7 42 24  	addiu	$2, $2, -0x48e8 <.text+0xffffffffffefb718>
  105cfc: 08 00 40 ac  	sw	$zero, 0x8($2)
  105d00: 3b 00 03 3c  	lui	$3, 0x3b
  105d04: 01 00 02 24  	addiu	$2, $zero, 0x1
  105d08: 18 b7 63 24  	addiu	$3, $3, -0x48e8 <.text+0xffffffffffefb718>
  105d0c: 08 00 e0 03  	jr	$ra
  105d10: 0c 00 66 ac  	sw	$6, 0xc($3)
  105d14: 08 00 62 ac  	sw	$2, 0x8($3)
  105d18: fa ff 00 10  	b	0x105d04 <.text+0x5d04>
  105d1c: 3b 00 03 3c  	lui	$3, 0x3b
  105d20: f5 ff 82 54  	bnel	$4, $2, 0x105cf8 <.text+0x5cf8>
  105d24: 3b 00 02 3c  	lui	$2, 0x3b
  105d28: fa ff 00 10  	b	0x105d14 <.text+0x5d14>
  105d2c: 80 bb 02 34  	ori	$2, $zero, 0xbb80
  105d30: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  105d34: 1f 00 02 3c  	lui	$2, 0x1f
  105d38: 00 00 bf ff  	sd	$ra, 0x0($sp)
  105d3c: ec ba 43 8c  	lw	$3, -0x4514($2)
  105d40: 01 00 02 24  	addiu	$2, $zero, 0x1
  105d44: 04 00 62 10  	beq	$3, $2, 0x105d58 <.text+0x5d58>
  105d48: 00 00 00 00  	nop
  105d4c: 00 00 bf df  	ld	$ra, 0x0($sp)
  105d50: 08 00 e0 03  	jr	$ra
  105d54: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  105d58: d4 15 04 0c  	jal	0x105750 <.text+0x5750>
  105d5c: 00 00 00 00  	nop
  105d60: 35 00 04 3c  	lui	$4, 0x35
  105d64: 2d 28 40 00  	move	$5, $2
  105d68: 2e 4d 05 0c  	jal	0x1534b8 <.text+0x534b8>
  105d6c: b0 e2 84 24  	addiu	$4, $4, -0x1d50 <.text+0xffffffffffefe2b0>
  105d70: f7 ff 00 10  	b	0x105d50 <.text+0x5d50>
  105d74: 00 00 bf df  	ld	$ra, 0x0($sp)
  105d78: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  105d7c: 2d 20 00 00  	move	$4, $zero
  105d80: 00 00 b0 ff  	sd	$16, 0x0($sp)
  105d84: 10 00 bf ff  	sd	$ra, 0x10($sp)
  105d88: cf 1d 04 0c  	jal	0x10773c <.text+0x773c>
  105d8c: 32 00 10 3c  	lui	$16, 0x32
  105d90: c7 1e 04 0c  	jal	0x107b1c <.text+0x7b1c>
  105d94: 00 00 00 00  	nop
  105d98: 2d 20 00 00  	move	$4, $zero
  105d9c: 4c 29 02 8e  	lw	$2, 0x294c($16)
  105da0: 0f 00 45 24  	addiu	$5, $2, 0xf
  105da4: 00 00 43 28  	slti	$3, $2, 0x0
  105da8: 0f 00 46 30  	andi	$6, $2, 0xf
  105dac: 0b 10 a3 00  	movn	$2, $5, $3
  105db0: 04 00 c0 10  	beqz	$6, 0x105dc4 <.text+0x5dc4>
  105db4: 03 11 02 00  	sra	$2, $2, 0x4
  105db8: 00 11 02 00  	sll	$2, $2, 0x4
  105dbc: 10 00 42 24  	addiu	$2, $2, 0x10
  105dc0: 4c 29 02 ae  	sw	$2, 0x294c($16)
  105dc4: df 1e 04 0c  	jal	0x107b7c <.text+0x7b7c>
  105dc8: 00 00 00 00  	nop
  105dcc: 6d 1f 04 0c  	jal	0x107db4 <.text+0x7db4>
  105dd0: 00 00 00 00  	nop
  105dd4: 2f 00 04 3c  	lui	$4, 0x2f
  105dd8: 4c 29 05 8e  	lw	$5, 0x294c($16)
  105ddc: 19 1f 04 0c  	jal	0x107c64 <.text+0x7c64>
  105de0: 40 c5 84 24  	addiu	$4, $4, -0x3ac0 <.text+0xffffffffffefc540>
  105de4: 51 1f 04 0c  	jal	0x107d44 <.text+0x7d44>
  105de8: 01 00 04 24  	addiu	$4, $zero, 0x1
  105dec: 85 1f 04 0c  	jal	0x107e14 <.text+0x7e14>
  105df0: ff 3f 04 24  	addiu	$4, $zero, 0x3fff
  105df4: 00 00 b0 df  	ld	$16, 0x0($sp)
  105df8: 10 00 bf df  	ld	$ra, 0x10($sp)
  105dfc: 08 00 e0 03  	jr	$ra
  105e00: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  105e04: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  105e08: 00 00 bf ff  	sd	$ra, 0x0($sp)
  105e0c: 85 1f 04 0c  	jal	0x107e14 <.text+0x7e14>
  105e10: 2d 20 00 00  	move	$4, $zero
  105e14: c6 1f 04 0c  	jal	0x107f18 <.text+0x7f18>
  105e18: 00 00 00 00  	nop
  105e1c: fe 1d 04 0c  	jal	0x1077f8 <.text+0x77f8>
