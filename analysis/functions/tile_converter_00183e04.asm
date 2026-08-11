
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  183d80: 35 00 11 3c  	lui	$17, 0x35
  183d84: 2a 18 b2 00  	slt	$3, $5, $18
  183d88: 98 e2 22 8e  	lw	$2, -0x1d68($17)
  183d8c: 0b 90 a3 00  	movn	$18, $5, $3
  183d90: 42 00 07 3c  	lui	$7, 0x42
  183d94: 21 10 52 00  	addu	$2, $2, $18
  183d98: 58 38 f0 24  	addiu	$16, $7, 0x3858
  183d9c: 00 00 43 90  	lbu	$3, 0x0($2)
  183da0: 02 00 45 24  	addiu	$5, $2, 0x2
  183da4: 0d 00 06 24  	addiu	$6, $zero, 0xd
  183da8: 02 00 04 26  	addiu	$4, $16, 0x2
  183dac: 58 38 e3 a0  	sb	$3, 0x3858($7)
  183db0: 01 00 42 90  	lbu	$2, 0x1($2)
  183db4: 28 71 06 0c  	jal	0x19c4a0 <.text+0x9c4a0>
  183db8: 01 00 02 a2  	sb	$2, 0x1($16)
  183dbc: 98 e2 22 8e  	lw	$2, -0x1d68($17)
  183dc0: 14 00 04 26  	addiu	$4, $16, 0x14
  183dc4: 08 00 06 24  	addiu	$6, $zero, 0x8
  183dc8: 21 10 52 00  	addu	$2, $2, $18
  183dcc: 0f 00 43 90  	lbu	$3, 0xf($2)
  183dd0: 11 00 45 24  	addiu	$5, $2, 0x11
  183dd4: 0f 00 03 a2  	sb	$3, 0xf($16)
  183dd8: 10 00 42 90  	lbu	$2, 0x10($2)
  183ddc: 28 71 06 0c  	jal	0x19c4a0 <.text+0x9c4a0>
  183de0: 10 00 02 a2  	sb	$2, 0x10($16)
  183de4: f3 0d 06 0c  	jal	0x1837cc <.text+0x837cc>
  183de8: 00 00 00 00  	nop
  183dec: 30 00 bf df  	ld	$ra, 0x30($sp)
  183df0: 20 00 b2 df  	ld	$18, 0x20($sp)
  183df4: 10 00 b1 df  	ld	$17, 0x10($sp)
  183df8: 00 00 b0 df  	ld	$16, 0x0($sp)
  183dfc: 08 00 e0 03  	jr	$ra
  183e00: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  183e04: 35 00 02 3c  	lui	$2, 0x35
  183e08: 2d 40 80 00  	move	$8, $4
  183e0c: b8 e2 43 8c  	lw	$3, -0x1d48($2)
  183e10: 2d 58 00 00  	move	$11, $zero
  183e14: 36 00 02 3c  	lui	$2, 0x36
  183e18: 54 d4 46 8c  	lw	$6, -0x2bac($2)
  183e1c: 04 00 02 24  	addiu	$2, $zero, 0x4
  183e20: cd 00 c2 10  	beq	$6, $2, 0x184158 <.text+0x84158>
  183e24: 21 38 65 00  	addu	$7, $3, $5
  183e28: 05 00 c2 2c  	sltiu	$2, $6, 0x5
  183e2c: 34 00 40 10  	beqz	$2, 0x183f00 <.text+0x83f00>
  183e30: 08 00 02 24  	addiu	$2, $zero, 0x8
  183e34: 02 00 02 24  	addiu	$2, $zero, 0x2
  183e38: 05 00 c2 50  	beql	$6, $2, 0x183e50 <.text+0x83e50>
  183e3c: 08 00 0a 24  	addiu	$10, $zero, 0x8
  183e40: 02 00 03 24  	addiu	$3, $zero, 0x2
  183e44: 01 00 02 24  	addiu	$2, $zero, 0x1
  183e48: 08 00 e0 03  	jr	$ra
  183e4c: 0a 10 6b 00  	movz	$2, $3, $11
  183e50: 00 00 e5 90  	lbu	$5, 0x0($7)
  183e54: ff ff 42 25  	addiu	$2, $10, -0x1 <.text+0xffffffffffefffff>
  183e58: ff 00 4a 30  	andi	$10, $2, 0xff
  183e5c: 2d 30 00 00  	move	$6, $zero
  183e60: 02 19 05 00  	srl	$3, $5, 0x4
  183e64: 36 00 02 3c  	lui	$2, 0x36
  183e68: a0 f9 42 24  	addiu	$2, $2, -0x660 <.text+0xffffffffffeff9a0>
  183e6c: 80 18 03 00  	sll	$3, $3, 0x2
  183e70: 0f 00 a4 30  	andi	$4, $5, 0xf
  183e74: 21 18 62 00  	addu	$3, $3, $2
  183e78: 36 00 02 3c  	lui	$2, 0x36
  183e7c: 80 20 04 00  	sll	$4, $4, 0x2
  183e80: a0 fa 42 24  	addiu	$2, $2, -0x560 <.text+0xffffffffffeffaa0>
  183e84: 2d 48 00 00  	move	$9, $zero
  183e88: 03 00 a0 10  	beqz	$5, 0x183e98 <.text+0x83e98>
  183e8c: 21 20 82 00  	addu	$4, $4, $2
  183e90: 00 00 66 8c  	lw	$6, 0x0($3)
  183e94: 00 00 89 8c  	lw	$9, 0x0($4)
  183e98: 01 00 e5 90  	lbu	$5, 0x1($7)
  183e9c: 36 00 02 3c  	lui	$2, 0x36
  183ea0: a0 fb 42 24  	addiu	$2, $2, -0x460 <.text+0xffffffffffeffba0>
  183ea4: 02 00 e7 24  	addiu	$7, $7, 0x2
  183ea8: 02 19 05 00  	srl	$3, $5, 0x4
  183eac: 0f 00 a4 30  	andi	$4, $5, 0xf
  183eb0: 80 18 03 00  	sll	$3, $3, 0x2
  183eb4: 80 20 04 00  	sll	$4, $4, 0x2
  183eb8: 21 18 62 00  	addu	$3, $3, $2
  183ebc: 36 00 02 3c  	lui	$2, 0x36
  183ec0: a0 fc 42 24  	addiu	$2, $2, -0x360 <.text+0xffffffffffeffca0>
  183ec4: 05 00 a0 10  	beqz	$5, 0x183edc <.text+0x83edc>
  183ec8: 21 20 82 00  	addu	$4, $4, $2
  183ecc: 00 00 62 8c  	lw	$2, 0x0($3)
  183ed0: 00 00 83 8c  	lw	$3, 0x0($4)
  183ed4: 25 30 c2 00  	or	$6, $6, $2
  183ed8: 25 48 23 01  	or	$9, $9, $3
  183edc: 00 00 06 ad  	sw	$6, 0x0($8)
  183ee0: 25 10 c9 00  	or	$2, $6, $9
  183ee4: 04 00 08 25  	addiu	$8, $8, 0x4
  183ee8: 25 58 62 01  	or	$11, $11, $2
  183eec: 00 00 09 ad  	sw	$9, 0x0($8)
  183ef0: d7 ff 40 15  	bnez	$10, 0x183e50 <.text+0x83e50>
  183ef4: 04 00 08 25  	addiu	$8, $8, 0x4
  183ef8: d2 ff 00 10  	b	0x183e44 <.text+0x83e44>
  183efc: 02 00 03 24  	addiu	$3, $zero, 0x2
  183f00: d0 ff c2 14  	bne	$6, $2, 0x183e44 <.text+0x83e44>
  183f04: 02 00 03 24  	addiu	$3, $zero, 0x2
  183f08: 08 00 0a 24  	addiu	$10, $zero, 0x8
  183f0c: 00 00 e3 90  	lbu	$3, 0x0($7)
  183f10: 2d 28 00 00  	move	$5, $zero
  183f14: 0d 00 60 10  	beqz	$3, 0x183f4c <.text+0x83f4c>
  183f18: 2d 30 00 00  	move	$6, $zero
  183f1c: 0f 00 64 30  	andi	$4, $3, 0xf
  183f20: 36 00 02 3c  	lui	$2, 0x36
  183f24: 02 19 03 00  	srl	$3, $3, 0x4
  183f28: a0 f9 42 24  	addiu	$2, $2, -0x660 <.text+0xffffffffffeff9a0>
  183f2c: 80 18 03 00  	sll	$3, $3, 0x2
  183f30: 80 20 04 00  	sll	$4, $4, 0x2
  183f34: 21 18 62 00  	addu	$3, $3, $2
  183f38: 36 00 02 3c  	lui	$2, 0x36
  183f3c: a0 fa 42 24  	addiu	$2, $2, -0x560 <.text+0xffffffffffeffaa0>
  183f40: 00 00 65 8c  	lw	$5, 0x0($3)
  183f44: 21 20 82 00  	addu	$4, $4, $2
  183f48: 00 00 86 8c  	lw	$6, 0x0($4)
  183f4c: 01 00 e3 90  	lbu	$3, 0x1($7)
  183f50: 10 00 60 50  	beqzl	$3, 0x183f94 <.text+0x83f94>
  183f54: 10 00 e3 90  	lbu	$3, 0x10($7)
  183f58: 0f 00 64 30  	andi	$4, $3, 0xf
  183f5c: 36 00 02 3c  	lui	$2, 0x36
  183f60: 02 19 03 00  	srl	$3, $3, 0x4
  183f64: a0 fb 42 24  	addiu	$2, $2, -0x460 <.text+0xffffffffffeffba0>
  183f68: 80 18 03 00  	sll	$3, $3, 0x2
  183f6c: 80 20 04 00  	sll	$4, $4, 0x2
  183f70: 21 18 62 00  	addu	$3, $3, $2
  183f74: 36 00 02 3c  	lui	$2, 0x36
  183f78: a0 fc 42 24  	addiu	$2, $2, -0x360 <.text+0xffffffffffeffca0>
  183f7c: 21 20 82 00  	addu	$4, $4, $2
  183f80: 00 00 62 8c  	lw	$2, 0x0($3)
  183f84: 00 00 83 8c  	lw	$3, 0x0($4)
  183f88: 25 28 a2 00  	or	$5, $5, $2
  183f8c: 25 30 c3 00  	or	$6, $6, $3
  183f90: 10 00 e3 90  	lbu	$3, 0x10($7)
  183f94: 10 00 60 50  	beqzl	$3, 0x183fd8 <.text+0x83fd8>
  183f98: 11 00 e3 90  	lbu	$3, 0x11($7)
  183f9c: 0f 00 64 30  	andi	$4, $3, 0xf
  183fa0: 36 00 02 3c  	lui	$2, 0x36
  183fa4: 02 19 03 00  	srl	$3, $3, 0x4
  183fa8: a0 f9 42 24  	addiu	$2, $2, -0x660 <.text+0xffffffffffeff9a0>
  183fac: 80 18 03 00  	sll	$3, $3, 0x2
  183fb0: 80 20 04 00  	sll	$4, $4, 0x2
  183fb4: 21 18 62 00  	addu	$3, $3, $2
  183fb8: 36 00 02 3c  	lui	$2, 0x36
  183fbc: a0 fa 42 24  	addiu	$2, $2, -0x560 <.text+0xffffffffffeffaa0>
  183fc0: 21 20 82 00  	addu	$4, $4, $2
  183fc4: 40 00 62 8c  	lw	$2, 0x40($3)
  183fc8: 40 00 83 8c  	lw	$3, 0x40($4)
  183fcc: 25 28 a2 00  	or	$5, $5, $2
  183fd0: 25 30 c3 00  	or	$6, $6, $3
  183fd4: 11 00 e3 90  	lbu	$3, 0x11($7)
  183fd8: 10 00 60 50  	beqzl	$3, 0x18401c <.text+0x8401c>
  183fdc: 20 00 e3 90  	lbu	$3, 0x20($7)
  183fe0: 0f 00 64 30  	andi	$4, $3, 0xf
  183fe4: 36 00 02 3c  	lui	$2, 0x36
  183fe8: 02 19 03 00  	srl	$3, $3, 0x4
  183fec: a0 fb 42 24  	addiu	$2, $2, -0x460 <.text+0xffffffffffeffba0>
  183ff0: 80 18 03 00  	sll	$3, $3, 0x2
  183ff4: 80 20 04 00  	sll	$4, $4, 0x2
  183ff8: 21 18 62 00  	addu	$3, $3, $2
  183ffc: 36 00 02 3c  	lui	$2, 0x36
  184000: a0 fc 42 24  	addiu	$2, $2, -0x360 <.text+0xffffffffffeffca0>
  184004: 21 20 82 00  	addu	$4, $4, $2
  184008: 40 00 62 8c  	lw	$2, 0x40($3)
  18400c: 40 00 83 8c  	lw	$3, 0x40($4)
  184010: 25 28 a2 00  	or	$5, $5, $2
  184014: 25 30 c3 00  	or	$6, $6, $3
  184018: 20 00 e3 90  	lbu	$3, 0x20($7)
  18401c: 10 00 60 50  	beqzl	$3, 0x184060 <.text+0x84060>
  184020: 21 00 e3 90  	lbu	$3, 0x21($7)
  184024: 0f 00 64 30  	andi	$4, $3, 0xf
  184028: 36 00 02 3c  	lui	$2, 0x36
  18402c: 02 19 03 00  	srl	$3, $3, 0x4
  184030: a0 f9 42 24  	addiu	$2, $2, -0x660 <.text+0xffffffffffeff9a0>
  184034: 80 18 03 00  	sll	$3, $3, 0x2
  184038: 80 20 04 00  	sll	$4, $4, 0x2
  18403c: 21 18 62 00  	addu	$3, $3, $2
  184040: 36 00 02 3c  	lui	$2, 0x36
  184044: a0 fa 42 24  	addiu	$2, $2, -0x560 <.text+0xffffffffffeffaa0>
  184048: 21 20 82 00  	addu	$4, $4, $2
  18404c: 80 00 62 8c  	lw	$2, 0x80($3)
  184050: 80 00 83 8c  	lw	$3, 0x80($4)
  184054: 25 28 a2 00  	or	$5, $5, $2
  184058: 25 30 c3 00  	or	$6, $6, $3
  18405c: 21 00 e3 90  	lbu	$3, 0x21($7)
  184060: 10 00 60 50  	beqzl	$3, 0x1840a4 <.text+0x840a4>
  184064: 30 00 e3 90  	lbu	$3, 0x30($7)
  184068: 0f 00 64 30  	andi	$4, $3, 0xf
  18406c: 36 00 02 3c  	lui	$2, 0x36
  184070: 02 19 03 00  	srl	$3, $3, 0x4
  184074: a0 fb 42 24  	addiu	$2, $2, -0x460 <.text+0xffffffffffeffba0>
  184078: 80 18 03 00  	sll	$3, $3, 0x2
  18407c: 80 20 04 00  	sll	$4, $4, 0x2
  184080: 21 18 62 00  	addu	$3, $3, $2
  184084: 36 00 02 3c  	lui	$2, 0x36
  184088: a0 fc 42 24  	addiu	$2, $2, -0x360 <.text+0xffffffffffeffca0>
  18408c: 21 20 82 00  	addu	$4, $4, $2
  184090: 80 00 62 8c  	lw	$2, 0x80($3)
  184094: 80 00 83 8c  	lw	$3, 0x80($4)
  184098: 25 28 a2 00  	or	$5, $5, $2
  18409c: 25 30 c3 00  	or	$6, $6, $3
  1840a0: 30 00 e3 90  	lbu	$3, 0x30($7)
  1840a4: 10 00 60 50  	beqzl	$3, 0x1840e8 <.text+0x840e8>
  1840a8: 31 00 e3 90  	lbu	$3, 0x31($7)
  1840ac: 0f 00 64 30  	andi	$4, $3, 0xf
  1840b0: 36 00 02 3c  	lui	$2, 0x36
  1840b4: 02 19 03 00  	srl	$3, $3, 0x4
  1840b8: a0 f9 42 24  	addiu	$2, $2, -0x660 <.text+0xffffffffffeff9a0>
  1840bc: 80 18 03 00  	sll	$3, $3, 0x2
  1840c0: 80 20 04 00  	sll	$4, $4, 0x2
  1840c4: 21 18 62 00  	addu	$3, $3, $2
  1840c8: 36 00 02 3c  	lui	$2, 0x36
  1840cc: a0 fa 42 24  	addiu	$2, $2, -0x560 <.text+0xffffffffffeffaa0>
  1840d0: 21 20 82 00  	addu	$4, $4, $2
  1840d4: c0 00 62 8c  	lw	$2, 0xc0($3)
  1840d8: c0 00 83 8c  	lw	$3, 0xc0($4)
  1840dc: 25 28 a2 00  	or	$5, $5, $2
  1840e0: 25 30 c3 00  	or	$6, $6, $3
  1840e4: 31 00 e3 90  	lbu	$3, 0x31($7)
  1840e8: 10 00 60 10  	beqz	$3, 0x18412c <.text+0x8412c>
  1840ec: ff ff 42 25  	addiu	$2, $10, -0x1 <.text+0xffffffffffefffff>
  1840f0: 0f 00 64 30  	andi	$4, $3, 0xf
  1840f4: 36 00 02 3c  	lui	$2, 0x36
  1840f8: 02 19 03 00  	srl	$3, $3, 0x4
  1840fc: a0 fb 42 24  	addiu	$2, $2, -0x460 <.text+0xffffffffffeffba0>
  184100: 80 18 03 00  	sll	$3, $3, 0x2
  184104: 80 20 04 00  	sll	$4, $4, 0x2
  184108: 21 18 62 00  	addu	$3, $3, $2
  18410c: 36 00 02 3c  	lui	$2, 0x36
  184110: a0 fc 42 24  	addiu	$2, $2, -0x360 <.text+0xffffffffffeffca0>
  184114: 21 20 82 00  	addu	$4, $4, $2
  184118: c0 00 62 8c  	lw	$2, 0xc0($3)
  18411c: c0 00 83 8c  	lw	$3, 0xc0($4)
  184120: 25 28 a2 00  	or	$5, $5, $2
  184124: 25 30 c3 00  	or	$6, $6, $3
  184128: ff ff 42 25  	addiu	$2, $10, -0x1 <.text+0xffffffffffefffff>
  18412c: 00 00 05 ad  	sw	$5, 0x0($8)
  184130: 25 18 a6 00  	or	$3, $5, $6
  184134: ff 00 4a 30  	andi	$10, $2, 0xff
  184138: 04 00 08 25  	addiu	$8, $8, 0x4
  18413c: 25 58 63 01  	or	$11, $11, $3
  184140: 00 00 06 ad  	sw	$6, 0x0($8)
  184144: 02 00 e7 24  	addiu	$7, $7, 0x2
  184148: 70 ff 40 15  	bnez	$10, 0x183f0c <.text+0x83f0c>
  18414c: 04 00 08 25  	addiu	$8, $8, 0x4
  184150: 3c ff 00 10  	b	0x183e44 <.text+0x83e44>
  184154: 02 00 03 24  	addiu	$3, $zero, 0x2
  184158: 08 00 0a 24  	addiu	$10, $zero, 0x8
  18415c: 00 00 e5 90  	lbu	$5, 0x0($7)
  184160: ff ff 42 25  	addiu	$2, $10, -0x1 <.text+0xffffffffffefffff>
  184164: ff 00 4a 30  	andi	$10, $2, 0xff
  184168: 2d 30 00 00  	move	$6, $zero
  18416c: 02 19 05 00  	srl	$3, $5, 0x4
  184170: 36 00 02 3c  	lui	$2, 0x36
  184174: a0 f9 42 24  	addiu	$2, $2, -0x660 <.text+0xffffffffffeff9a0>
  184178: 80 18 03 00  	sll	$3, $3, 0x2
  18417c: 0f 00 a4 30  	andi	$4, $5, 0xf
  184180: 21 18 62 00  	addu	$3, $3, $2
  184184: 36 00 02 3c  	lui	$2, 0x36
  184188: 80 20 04 00  	sll	$4, $4, 0x2
  18418c: a0 fa 42 24  	addiu	$2, $2, -0x560 <.text+0xffffffffffeffaa0>
  184190: 2d 48 00 00  	move	$9, $zero
  184194: 03 00 a0 10  	beqz	$5, 0x1841a4 <.text+0x841a4>
  184198: 21 20 82 00  	addu	$4, $4, $2
  18419c: 00 00 89 8c  	lw	$9, 0x0($4)
  1841a0: 00 00 66 8c  	lw	$6, 0x0($3)
  1841a4: 01 00 e5 90  	lbu	$5, 0x1($7)
  1841a8: 36 00 02 3c  	lui	$2, 0x36
  1841ac: a0 fb 42 24  	addiu	$2, $2, -0x460 <.text+0xffffffffffeffba0>
  1841b0: 02 19 05 00  	srl	$3, $5, 0x4
  1841b4: 0f 00 a4 30  	andi	$4, $5, 0xf
  1841b8: 80 18 03 00  	sll	$3, $3, 0x2
  1841bc: 80 20 04 00  	sll	$4, $4, 0x2
  1841c0: 21 18 62 00  	addu	$3, $3, $2
  1841c4: 36 00 02 3c  	lui	$2, 0x36
  1841c8: a0 fc 42 24  	addiu	$2, $2, -0x360 <.text+0xffffffffffeffca0>
  1841cc: 05 00 a0 10  	beqz	$5, 0x1841e4 <.text+0x841e4>
  1841d0: 21 20 82 00  	addu	$4, $4, $2
  1841d4: 00 00 62 8c  	lw	$2, 0x0($3)
  1841d8: 00 00 83 8c  	lw	$3, 0x0($4)
  1841dc: 25 30 c2 00  	or	$6, $6, $2
  1841e0: 25 48 23 01  	or	$9, $9, $3
  1841e4: 10 00 e5 90  	lbu	$5, 0x10($7)
  1841e8: 36 00 02 3c  	lui	$2, 0x36
  1841ec: a0 f9 42 24  	addiu	$2, $2, -0x660 <.text+0xffffffffffeff9a0>
  1841f0: 02 19 05 00  	srl	$3, $5, 0x4
  1841f4: 0f 00 a4 30  	andi	$4, $5, 0xf
  1841f8: 80 18 03 00  	sll	$3, $3, 0x2
  1841fc: 80 20 04 00  	sll	$4, $4, 0x2
  184200: 21 18 62 00  	addu	$3, $3, $2
  184204: 36 00 02 3c  	lui	$2, 0x36
  184208: a0 fa 42 24  	addiu	$2, $2, -0x560 <.text+0xffffffffffeffaa0>
  18420c: 05 00 a0 10  	beqz	$5, 0x184224 <.text+0x84224>
  184210: 21 20 82 00  	addu	$4, $4, $2
  184214: 40 00 62 8c  	lw	$2, 0x40($3)
  184218: 40 00 83 8c  	lw	$3, 0x40($4)
  18421c: 25 30 c2 00  	or	$6, $6, $2
  184220: 25 48 23 01  	or	$9, $9, $3
  184224: 11 00 e5 90  	lbu	$5, 0x11($7)
  184228: 36 00 02 3c  	lui	$2, 0x36
  18422c: a0 fb 42 24  	addiu	$2, $2, -0x460 <.text+0xffffffffffeffba0>
  184230: 02 00 e7 24  	addiu	$7, $7, 0x2
  184234: 02 19 05 00  	srl	$3, $5, 0x4
  184238: 0f 00 a4 30  	andi	$4, $5, 0xf
  18423c: 80 18 03 00  	sll	$3, $3, 0x2
  184240: 80 20 04 00  	sll	$4, $4, 0x2
  184244: 21 18 62 00  	addu	$3, $3, $2
  184248: 36 00 02 3c  	lui	$2, 0x36
  18424c: a0 fc 42 24  	addiu	$2, $2, -0x360 <.text+0xffffffffffeffca0>
  184250: 05 00 a0 10  	beqz	$5, 0x184268 <.text+0x84268>
  184254: 21 20 82 00  	addu	$4, $4, $2
  184258: 40 00 62 8c  	lw	$2, 0x40($3)
  18425c: 40 00 83 8c  	lw	$3, 0x40($4)
  184260: 25 30 c2 00  	or	$6, $6, $2
  184264: 25 48 23 01  	or	$9, $9, $3
  184268: 00 00 06 ad  	sw	$6, 0x0($8)
  18426c: 25 10 c9 00  	or	$2, $6, $9
  184270: 04 00 08 25  	addiu	$8, $8, 0x4
  184274: 25 58 62 01  	or	$11, $11, $2
  184278: 00 00 09 ad  	sw	$9, 0x0($8)
  18427c: b7 ff 40 15  	bnez	$10, 0x18415c <.text+0x8415c>
  184280: 04 00 08 25  	addiu	$8, $8, 0x4
  184284: ef fe 00 10  	b	0x183e44 <.text+0x83e44>
  184288: 02 00 03 24  	addiu	$3, $zero, 0x2
  18428c: 80 ff bd 27  	addiu	$sp, $sp, -0x80 <.text+0xffffffffffefff80>
