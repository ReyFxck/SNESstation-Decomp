
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  176c80: 01 00 ef 25  	addiu	$15, $15, 0x1
  176c84: 2a 10 f3 01  	slt	$2, $15, $19
  176c88: c3 ff 40 14  	bnez	$2, 0x176b98 <.text+0x76b98>
  176c8c: 02 00 6b 25  	addiu	$11, $11, 0x2
  176c90: ab fe 00 10  	b	0x176740 <.text+0x76740>
  176c94: 50 00 bf df  	ld	$ra, 0x50($sp)
  176c98: 1c 00 03 3c  	lui	$3, 0x1c
  176c9c: c0 10 0f 00  	sll	$2, $15, 0x3
  176ca0: 80 34 84 24  	addiu	$4, $4, 0x3480
  176ca4: 80 bd 63 24  	addiu	$3, $3, -0x4280 <.text+0xffffffffffefbd80>
  176ca8: 21 20 44 00  	addu	$4, $2, $4
  176cac: 21 10 43 00  	addu	$2, $2, $3
  176cb0: 06 00 86 a4  	sh	$6, 0x6($4)
  176cb4: 06 00 46 a4  	sh	$6, 0x6($2)
  176cb8: 00 00 46 a4  	sh	$6, 0x0($2)
  176cbc: 02 00 46 a4  	sh	$6, 0x2($2)
  176cc0: 04 00 46 a4  	sh	$6, 0x4($2)
  176cc4: 00 00 86 a4  	sh	$6, 0x0($4)
  176cc8: 02 00 86 a4  	sh	$6, 0x2($4)
  176ccc: ec ff 00 10  	b	0x176c80 <.text+0x76c80>
  176cd0: 04 00 86 a4  	sh	$6, 0x4($4)
  176cd4: eb ff 62 54  	bnel	$3, $2, 0x176c84 <.text+0x76c84>
  176cd8: 01 00 ef 25  	addiu	$15, $15, 0x1
  176cdc: 1d 00 03 3c  	lui	$3, 0x1d
  176ce0: 1c 00 02 3c  	lui	$2, 0x1c
  176ce4: 40 20 0f 00  	sll	$4, $15, 0x1
  176ce8: 80 34 63 24  	addiu	$3, $3, 0x3480
  176cec: 80 bd 42 24  	addiu	$2, $2, -0x4280 <.text+0xffffffffffefbd80>
  176cf0: 21 18 83 00  	addu	$3, $4, $3
  176cf4: 21 20 82 00  	addu	$4, $4, $2
  176cf8: 00 00 66 a4  	sh	$6, 0x0($3)
  176cfc: e0 ff 00 10  	b	0x176c80 <.text+0x76c80>
  176d00: 00 00 86 a4  	sh	$6, 0x0($4)
  176d04: 1d 00 03 3c  	lui	$3, 0x1d
  176d08: 1c 00 02 3c  	lui	$2, 0x1c
  176d0c: 80 34 63 24  	addiu	$3, $3, 0x3480
  176d10: 80 bd 42 24  	addiu	$2, $2, -0x4280 <.text+0xffffffffffefbd80>
  176d14: 21 18 23 01  	addu	$3, $9, $3
  176d18: 21 10 22 01  	addu	$2, $9, $2
  176d1c: 02 00 66 a4  	sh	$6, 0x2($3)
  176d20: 02 00 46 a4  	sh	$6, 0x2($2)
  176d24: 00 00 46 a4  	sh	$6, 0x0($2)
  176d28: d5 ff 00 10  	b	0x176c80 <.text+0x76c80>
  176d2c: 00 00 66 a4  	sh	$6, 0x0($3)
  176d30: 82 fe 60 1a  	blez	$19, 0x17673c <.text+0x7673c>
  176d34: 2d 78 00 00  	move	$15, $zero
  176d38: 40 10 04 00  	sll	$2, $4, 0x1
  176d3c: 21 88 51 00  	addu	$17, $2, $17
  176d40: 35 00 02 3c  	lui	$2, 0x35
  176d44: 3f 00 0e 3c  	lui	$14, 0x3f
  176d48: 50 db 50 24  	addiu	$16, $2, -0x24b0 <.text+0xffffffffffefdb50>
  176d4c: 68 2e c7 dd  	ld	$7, 0x2e68($14)
  176d50: 10 00 0c 8e  	lw	$12, 0x10($16)
  176d54: 3b 00 02 3c  	lui	$2, 0x3b
  176d58: 07 00 e3 30  	andi	$3, $7, 0x7
  176d5c: 48 b7 42 24  	addiu	$2, $2, -0x48b8 <.text+0xffffffffffefb748>
  176d60: 80 68 0c 00  	sll	$13, $12, 0x2
  176d64: 3c 18 03 00  	dsll32	$3, $3, 0x0
  176d68: 3f 18 03 00  	dsra32	$3, $3, 0x0
  176d6c: 21 68 a2 01  	addu	$13, $13, $2
  176d70: 3f 00 05 3c  	lui	$5, 0x3f
  176d74: ff ff e2 64  	daddiu	$2, $7, -0x1 <.text+0xffffffffffefffff>
  176d78: 70 2e a5 24  	addiu	$5, $5, 0x2e70
  176d7c: 00 00 b2 8d  	lw	$18, 0x0($13)
  176d80: 80 18 03 00  	sll	$3, $3, 0x2
  176d84: 07 00 42 30  	andi	$2, $2, 0x7
  176d88: 21 18 65 00  	addu	$3, $3, $5
  176d8c: 3c 10 02 00  	dsll32	$2, $2, 0x0
  176d90: 3f 10 02 00  	dsra32	$2, $2, 0x0
  176d94: 00 00 72 ac  	sw	$18, 0x0($3)
  176d98: 80 10 02 00  	sll	$2, $2, 0x2
  176d9c: 3f 00 08 3c  	lui	$8, 0x3f
  176da0: 21 10 45 00  	addu	$2, $2, $5
  176da4: 48 2e 06 25  	addiu	$6, $8, 0x2e48
  176da8: 00 00 44 8c  	lw	$4, 0x0($2)
  176dac: 01 00 8c 25  	addiu	$12, $12, 0x1
  176db0: fe ff e2 64  	daddiu	$2, $7, -0x2 <.text+0xffffffffffeffffe>
  176db4: 04 00 c3 8c  	lw	$3, 0x4($6)
  176db8: 07 00 42 30  	andi	$2, $2, 0x7
  176dbc: 48 2e 08 8d  	lw	$8, 0x2e48($8)
  176dc0: 3c 10 02 00  	dsll32	$2, $2, 0x0
  176dc4: 3f 10 02 00  	dsra32	$2, $2, 0x0
  176dc8: 18 20 83 00  	<unknown>
  176dcc: 80 10 02 00  	sll	$2, $2, 0x2
  176dd0: fd ff e3 64  	daddiu	$3, $7, -0x3 <.text+0xffffffffffeffffd>
  176dd4: 21 10 45 00  	addu	$2, $2, $5
  176dd8: 07 00 63 30  	andi	$3, $3, 0x7
  176ddc: 00 00 4a 8c  	lw	$10, 0x0($2)
  176de0: 3c 18 03 00  	dsll32	$3, $3, 0x0
  176de4: 3f 18 03 00  	dsra32	$3, $3, 0x0
  176de8: 18 10 48 02  	<unknown>
  176dec: 08 00 c9 8c  	lw	$9, 0x8($6)
  176df0: 80 18 03 00  	sll	$3, $3, 0x2
  176df4: 10 00 c8 8c  	lw	$8, 0x10($6)
  176df8: 21 18 65 00  	addu	$3, $3, $5
  176dfc: 10 00 0c ae  	sw	$12, 0x10($16)
  176e00: 00 00 6b 8c  	lw	$11, 0x0($3)
  176e04: 18 18 49 01  	<unknown>
  176e08: 21 90 44 00  	addu	$18, $2, $4
  176e0c: 0c 00 c4 8c  	lw	$4, 0xc($6)
  176e10: fc ff e2 64  	daddiu	$2, $7, -0x4 <.text+0xffffffffffeffffc>
  176e14: 07 00 42 30  	andi	$2, $2, 0x7
  176e18: 3c 10 02 00  	dsll32	$2, $2, 0x0
  176e1c: 3f 10 02 00  	dsra32	$2, $2, 0x0
  176e20: 21 90 72 00  	addu	$18, $3, $18
  176e24: 80 10 02 00  	sll	$2, $2, 0x2
  176e28: fb ff e3 64  	daddiu	$3, $7, -0x5 <.text+0xffffffffffeffffb>
  176e2c: 21 10 45 00  	addu	$2, $2, $5
  176e30: 07 00 63 30  	andi	$3, $3, 0x7
  176e34: 00 00 49 8c  	lw	$9, 0x0($2)
  176e38: 3c 18 03 00  	dsll32	$3, $3, 0x0
  176e3c: 3f 18 03 00  	dsra32	$3, $3, 0x0
  176e40: 18 10 64 01  	<unknown>
  176e44: 80 18 03 00  	sll	$3, $3, 0x2
  176e48: 21 18 65 00  	addu	$3, $3, $5
  176e4c: 14 00 c4 8c  	lw	$4, 0x14($6)
  176e50: 00 00 6a 8c  	lw	$10, 0x0($3)
  176e54: 18 18 28 01  	<unknown>
  176e58: 21 90 52 00  	addu	$18, $2, $18
  176e5c: 18 48 44 01  	<unknown>
  176e60: fa ff e2 64  	daddiu	$2, $7, -0x6 <.text+0xffffffffffeffffa>
  176e64: 0c 00 04 8e  	lw	$4, 0xc($16)
  176e68: 07 00 42 30  	andi	$2, $2, 0x7
  176e6c: 21 90 72 00  	addu	$18, $3, $18
  176e70: 3c 10 02 00  	dsll32	$2, $2, 0x0
  176e74: 3f 10 02 00  	dsra32	$2, $2, 0x0
  176e78: f9 ff e3 64  	daddiu	$3, $7, -0x7 <.text+0xffffffffffeffff9>
  176e7c: 80 10 02 00  	sll	$2, $2, 0x2
  176e80: 07 00 63 30  	andi	$3, $3, 0x7
  176e84: 21 10 45 00  	addu	$2, $2, $5
  176e88: 3c 18 03 00  	dsll32	$3, $3, 0x0
  176e8c: 3f 18 03 00  	dsra32	$3, $3, 0x0
  176e90: 00 00 48 8c  	lw	$8, 0x0($2)
  176e94: 80 18 03 00  	sll	$3, $3, 0x2
  176e98: 18 00 c2 8c  	lw	$2, 0x18($6)
  176e9c: 21 18 65 00  	addu	$3, $3, $5
  176ea0: 21 90 32 01  	addu	$18, $9, $18
  176ea4: 00 00 63 8c  	lw	$3, 0x0($3)
  176ea8: 18 28 02 01  	<unknown>
  176eac: 1c 00 c6 8c  	lw	$6, 0x1c($6)
  176eb0: 3e 00 02 3c  	lui	$2, 0x3e
  176eb4: 80 48 0f 00  	sll	$9, $15, 0x2
  176eb8: 48 2e 42 24  	addiu	$2, $2, 0x2e48
  176ebc: 01 00 e7 64  	daddiu	$7, $7, 0x1
  176ec0: 21 10 22 01  	addu	$2, $9, $2
  176ec4: 68 2e c7 fd  	sd	$7, 0x2e68($14)
  176ec8: 00 00 48 8c  	lw	$8, 0x0($2)
  176ecc: 21 90 b2 00  	addu	$18, $5, $18
  176ed0: 18 10 66 00  	<unknown>
  176ed4: 14 00 05 8e  	lw	$5, 0x14($16)
  176ed8: 2a 28 85 01  	slt	$5, $12, $5
  176edc: 21 90 52 00  	addu	$18, $2, $18
  176ee0: 00 00 43 2a  	slti	$3, $18, 0x0
  176ee4: 7f 00 42 26  	addiu	$2, $18, 0x7f
  176ee8: 0a 10 43 02  	movz	$2, $18, $3
  176eec: c3 91 02 00  	sra	$18, $2, 0x7
  176ef0: 18 20 44 02  	<unknown>
  176ef4: 7f 00 83 24  	addiu	$3, $4, 0x7f
  176ef8: 00 00 82 28  	slti	$2, $4, 0x0
  176efc: 0b 20 62 00  	movn	$4, $3, $2
  176f00: c3 21 04 00  	sra	$4, $4, 0x7
  176f04: 21 20 88 00  	addu	$4, $4, $8
  176f08: 02 00 a0 14  	bnez	$5, 0x176f14 <.text+0x76f14>
  176f0c: 00 00 a4 ad  	sw	$4, 0x0($13)
  176f10: 10 00 00 ae  	sw	$zero, 0x10($16)
  176f14: 3d 00 02 3c  	lui	$2, 0x3d
  176f18: 34 07 04 8e  	lw	$4, 0x734($16)
  176f1c: 48 2e 42 24  	addiu	$2, $2, 0x2e48
  176f20: 3c 07 03 8e  	lw	$3, 0x73c($16)
  176f24: 21 10 22 01  	addu	$2, $9, $2
  176f28: ff 7f 05 24  	addiu	$5, $zero, 0x7fff
  176f2c: 00 00 42 8c  	lw	$2, 0x0($2)
  176f30: 18 18 43 02  	<unknown>
  176f34: 18 30 44 00  	<unknown>
  176f38: 21 10 c3 00  	addu	$2, $6, $3
  176f3c: 00 00 43 28  	slti	$3, $2, 0x0
  176f40: 7f 00 44 24  	addiu	$4, $2, 0x7f
  176f44: 0b 10 83 00  	movn	$2, $4, $3
  176f48: c3 31 02 00  	sra	$6, $2, 0x7
  176f4c: 00 80 c3 28  	slti	$3, $6, -0x8000 <.text+0xffffffffffef8000>
  176f50: 2a 10 a6 00  	slt	$2, $5, $6
  176f54: 0b 30 a2 00  	movn	$6, $5, $2
  176f58: 00 00 63 38  	xori	$3, $3, 0x0
  176f5c: 00 80 02 24  	addiu	$2, $zero, -0x8000 <.text+0xffffffffffef8000>
  176f60: 0b 30 43 00  	movn	$6, $2, $3
  176f64: 34 00 02 3c  	lui	$2, 0x34
  176f68: 00 00 26 a6  	sh	$6, 0x0($17)
  176f6c: 48 55 43 8c  	lw	$3, 0x5548($2)
  176f70: 02 00 02 24  	addiu	$2, $zero, 0x2
  176f74: 27 00 62 10  	beq	$3, $2, 0x177014 <.text+0x77014>
  176f78: 03 00 62 2c  	sltiu	$2, $3, 0x3
  176f7c: 19 00 40 10  	beqz	$2, 0x176fe4 <.text+0x76fe4>
  176f80: 03 00 02 24  	addiu	$2, $zero, 0x3
  176f84: 01 00 02 24  	addiu	$2, $zero, 0x1
  176f88: 07 00 62 10  	beq	$3, $2, 0x176fa8 <.text+0x76fa8>
  176f8c: 1d 00 04 3c  	lui	$4, 0x1d
  176f90: 01 00 ef 25  	addiu	$15, $15, 0x1
  176f94: 2a 10 f3 01  	slt	$2, $15, $19
  176f98: 69 ff 40 14  	bnez	$2, 0x176d40 <.text+0x76d40>
  176f9c: 02 00 31 26  	addiu	$17, $17, 0x2
  176fa0: e7 fd 00 10  	b	0x176740 <.text+0x76740>
  176fa4: 50 00 bf df  	ld	$ra, 0x50($sp)
  176fa8: 1c 00 03 3c  	lui	$3, 0x1c
  176fac: c0 10 0f 00  	sll	$2, $15, 0x3
  176fb0: 80 34 84 24  	addiu	$4, $4, 0x3480
  176fb4: 80 bd 63 24  	addiu	$3, $3, -0x4280 <.text+0xffffffffffefbd80>
  176fb8: 21 20 44 00  	addu	$4, $2, $4
  176fbc: 21 10 43 00  	addu	$2, $2, $3
  176fc0: 06 00 86 a4  	sh	$6, 0x6($4)
  176fc4: 06 00 46 a4  	sh	$6, 0x6($2)
  176fc8: 00 00 46 a4  	sh	$6, 0x0($2)
  176fcc: 02 00 46 a4  	sh	$6, 0x2($2)
  176fd0: 04 00 46 a4  	sh	$6, 0x4($2)
  176fd4: 00 00 86 a4  	sh	$6, 0x0($4)
  176fd8: 02 00 86 a4  	sh	$6, 0x2($4)
  176fdc: ec ff 00 10  	b	0x176f90 <.text+0x76f90>
  176fe0: 04 00 86 a4  	sh	$6, 0x4($4)
  176fe4: eb ff 62 54  	bnel	$3, $2, 0x176f94 <.text+0x76f94>
  176fe8: 01 00 ef 25  	addiu	$15, $15, 0x1
  176fec: 1d 00 03 3c  	lui	$3, 0x1d
  176ff0: 1c 00 02 3c  	lui	$2, 0x1c
  176ff4: 40 20 0f 00  	sll	$4, $15, 0x1
  176ff8: 80 34 63 24  	addiu	$3, $3, 0x3480
  176ffc: 80 bd 42 24  	addiu	$2, $2, -0x4280 <.text+0xffffffffffefbd80>
  177000: 21 18 83 00  	addu	$3, $4, $3
  177004: 21 20 82 00  	addu	$4, $4, $2
  177008: 00 00 66 a4  	sh	$6, 0x0($3)
  17700c: e0 ff 00 10  	b	0x176f90 <.text+0x76f90>
  177010: 00 00 86 a4  	sh	$6, 0x0($4)
  177014: 1d 00 03 3c  	lui	$3, 0x1d
  177018: 1c 00 02 3c  	lui	$2, 0x1c
  17701c: 80 34 63 24  	addiu	$3, $3, 0x3480
  177020: 80 bd 42 24  	addiu	$2, $2, -0x4280 <.text+0xffffffffffefbd80>
  177024: 21 18 23 01  	addu	$3, $9, $3
  177028: 21 10 22 01  	addu	$2, $9, $2
  17702c: 02 00 66 a4  	sh	$6, 0x2($3)
  177030: 02 00 46 a4  	sh	$6, 0x2($2)
  177034: 00 00 46 a4  	sh	$6, 0x0($2)
  177038: d5 ff 00 10  	b	0x176f90 <.text+0x76f90>
  17703c: 00 00 66 a4  	sh	$6, 0x0($3)
  177040: be fd 60 1a  	blez	$19, 0x17673c <.text+0x7673c>
  177044: 2d 78 00 00  	move	$15, $zero
  177048: 40 10 04 00  	sll	$2, $4, 0x1
  17704c: 21 48 51 00  	addu	$9, $2, $17
  177050: 01 00 e7 31  	andi	$7, $15, 0x1
  177054: 3d 00 03 3c  	lui	$3, 0x3d
  177058: 35 00 02 3c  	lui	$2, 0x35
  17705c: 80 40 0f 00  	sll	$8, $15, 0x2
  177060: 50 db 42 24  	addiu	$2, $2, -0x24b0 <.text+0xffffffffffefdb50>
  177064: 80 20 07 00  	sll	$4, $7, 0x2
  177068: 48 2e 63 24  	addiu	$3, $3, 0x2e48
  17706c: 21 20 82 00  	addu	$4, $4, $2
  177070: 21 18 03 01  	addu	$3, $8, $3
  177074: ff 7f 05 24  	addiu	$5, $zero, 0x7fff
  177078: 00 00 62 8c  	lw	$2, 0x0($3)
  17707c: 34 07 83 8c  	lw	$3, 0x734($4)
  177080: 18 10 43 00  	<unknown>
  177084: 00 00 43 28  	slti	$3, $2, 0x0
  177088: 7f 00 44 24  	addiu	$4, $2, 0x7f
  17708c: 0b 10 83 00  	movn	$2, $4, $3
  177090: c3 31 02 00  	sra	$6, $2, 0x7
  177094: 00 80 c3 28  	slti	$3, $6, -0x8000 <.text+0xffffffffffef8000>
  177098: 2a 10 a6 00  	slt	$2, $5, $6
  17709c: 0b 30 a2 00  	movn	$6, $5, $2
  1770a0: 00 00 63 38  	xori	$3, $3, 0x0
  1770a4: 00 80 02 24  	addiu	$2, $zero, -0x8000 <.text+0xffffffffffef8000>
  1770a8: 0b 30 43 00  	movn	$6, $2, $3
  1770ac: 3b 00 02 3c  	lui	$2, 0x3b
  1770b0: 00 00 26 a5  	sh	$6, 0x0($9)
  1770b4: 18 b7 42 24  	addiu	$2, $2, -0x48e8 <.text+0xffffffffffefb718>
  1770b8: 18 00 42 8c  	lw	$2, 0x18($2)
  1770bc: 43 00 40 10  	beqz	$2, 0x1771cc <.text+0x771cc>
  1770c0: 34 00 02 3c  	lui	$2, 0x34
  1770c4: 48 55 43 8c  	lw	$3, 0x5548($2)
  1770c8: 02 00 02 24  	addiu	$2, $zero, 0x2
  1770cc: 2e 00 62 10  	beq	$3, $2, 0x177188 <.text+0x77188>
  1770d0: 03 00 62 2c  	sltiu	$2, $3, 0x3
  1770d4: 1d 00 40 10  	beqz	$2, 0x17714c <.text+0x7714c>
  1770d8: 03 00 02 24  	addiu	$2, $zero, 0x3
  1770dc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1770e0: 07 00 62 10  	beq	$3, $2, 0x177100 <.text+0x77100>
  1770e4: ff 00 e2 30  	andi	$2, $7, 0xff
  1770e8: 01 00 ef 25  	addiu	$15, $15, 0x1
  1770ec: 2a 10 f3 01  	slt	$2, $15, $19
  1770f0: d7 ff 40 14  	bnez	$2, 0x177050 <.text+0x77050>
  1770f4: 02 00 29 25  	addiu	$9, $9, 0x2
  1770f8: 91 fd 00 10  	b	0x176740 <.text+0x76740>
  1770fc: 50 00 bf df  	ld	$ra, 0x50($sp)
  177100: 0c 00 40 10  	beqz	$2, 0x177134 <.text+0x77134>
  177104: 01 00 03 3c  	lui	$3, 0x1
  177108: 1c 00 02 3c  	lui	$2, 0x1c
  17710c: fc ff 63 34  	ori	$3, $3, 0xfffc
  177110: 80 bd 42 24  	addiu	$2, $2, -0x4280 <.text+0xffffffffffefbd80>
  177114: 24 18 e3 01  	and	$3, $15, $3
  177118: 40 18 03 00  	sll	$3, $3, 0x1
  17711c: 21 18 62 00  	addu	$3, $3, $2
  177120: 06 00 66 a4  	sh	$6, 0x6($3)
  177124: 00 00 66 a4  	sh	$6, 0x0($3)
  177128: 02 00 66 a4  	sh	$6, 0x2($3)
  17712c: ee ff 00 10  	b	0x1770e8 <.text+0x770e8>
  177130: 04 00 66 a4  	sh	$6, 0x4($3)
  177134: 1d 00 02 3c  	lui	$2, 0x1d
  177138: fc ff 63 34  	ori	$3, $3, 0xfffc
  17713c: 80 34 42 24  	addiu	$2, $2, 0x3480
  177140: 24 18 e3 01  	and	$3, $15, $3
  177144: f5 ff 00 10  	b	0x17711c <.text+0x7711c>
  177148: 40 18 03 00  	sll	$3, $3, 0x1
  17714c: e7 ff 62 54  	bnel	$3, $2, 0x1770ec <.text+0x770ec>
  177150: 01 00 ef 25  	addiu	$15, $15, 0x1
  177154: ff 00 e2 30  	andi	$2, $7, 0xff
  177158: 07 00 40 10  	beqz	$2, 0x177178 <.text+0x77178>
  17715c: 43 18 0f 00  	sra	$3, $15, 0x1
  177160: 1c 00 02 3c  	lui	$2, 0x1c
  177164: 40 18 03 00  	sll	$3, $3, 0x1
  177168: 80 bd 42 24  	addiu	$2, $2, -0x4280 <.text+0xffffffffffefbd80>
  17716c: 21 18 62 00  	addu	$3, $3, $2
  177170: dd ff 00 10  	b	0x1770e8 <.text+0x770e8>
  177174: 00 00 66 a4  	sh	$6, 0x0($3)
  177178: 1d 00 02 3c  	lui	$2, 0x1d
  17717c: 40 18 03 00  	sll	$3, $3, 0x1
