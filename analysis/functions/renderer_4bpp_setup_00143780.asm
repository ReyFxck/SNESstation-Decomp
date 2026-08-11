
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  143780: 0c 00 02 24  	addiu	$2, $zero, 0xc
  143784: 36 00 02 3c  	lui	$2, 0x36
  143788: 80 18 0c 00  	sll	$3, $12, 0x2
  14378c: 80 d4 42 24  	addiu	$2, $2, -0x2b80 <.text+0xffffffffffefd480>
  143790: 21 18 62 00  	addu	$3, $3, $2
  143794: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  143798: 70 00 62 ac  	sw	$2, 0x70($3)
  14379c: 36 00 02 3c  	lui	$2, 0x36
  1437a0: 08 00 e0 03  	jr	$ra
  1437a4: 6d c2 40 a0  	sb	$zero, -0x3d93($2)
  1437a8: 08 00 0e 24  	addiu	$14, $zero, 0x8
  1437ac: c5 ff 00 10  	b	0x1436c4 <.text+0x436c4>
  1437b0: 20 00 0b 24  	addiu	$11, $zero, 0x20
  1437b4: 08 00 0e 24  	addiu	$14, $zero, 0x8
  1437b8: c2 ff 00 10  	b	0x1436c4 <.text+0x436c4>
  1437bc: 40 00 0b 24  	addiu	$11, $zero, 0x40
  1437c0: fa ff 00 10  	b	0x1437ac <.text+0x437ac>
  1437c4: 10 00 0e 24  	addiu	$14, $zero, 0x10
  1437c8: fb ff 00 10  	b	0x1437b8 <.text+0x437b8>
  1437cc: 10 00 0e 24  	addiu	$14, $zero, 0x10
  1437d0: f9 ff 00 10  	b	0x1437b8 <.text+0x437b8>
  1437d4: 20 00 0e 24  	addiu	$14, $zero, 0x20
  1437d8: 20 ff bd 27  	addiu	$sp, $sp, -0xe0 <.text+0xffffffffffefff20>
  1437dc: 36 00 02 3c  	lui	$2, 0x36
  1437e0: 68 c2 4d 24  	addiu	$13, $2, -0x3d98 <.text+0xffffffffffefc268>
  1437e4: d0 00 bf ff  	sd	$ra, 0xd0($sp)
  1437e8: c0 00 be ff  	sd	$fp, 0xc0($sp)
  1437ec: 34 00 02 3c  	lui	$2, 0x34
  1437f0: b0 00 b7 ff  	sd	$23, 0xb0($sp)
  1437f4: 36 00 0f 3c  	lui	$15, 0x36
  1437f8: a0 00 b6 ff  	sd	$22, 0xa0($sp)
  1437fc: e0 54 4c 24  	addiu	$12, $2, 0x54e0
  143800: 90 00 b5 ff  	sd	$21, 0x90($sp)
  143804: 88 b7 e2 25  	addiu	$2, $15, -0x4878 <.text+0xffffffffffefb788>
  143808: 80 00 b4 ff  	sd	$20, 0x80($sp)
  14380c: 36 00 03 3c  	lui	$3, 0x36
  143810: 70 00 b3 ff  	sd	$19, 0x70($sp)
  143814: 50 d4 63 24  	addiu	$3, $3, -0x2bb0 <.text+0xffffffffffefd450>
  143818: 60 00 b2 ff  	sd	$18, 0x60($sp)
  14381c: 04 00 06 24  	addiu	$6, $zero, 0x4
  143820: 50 00 b1 ff  	sd	$17, 0x50($sp)
  143824: ff 00 84 30  	andi	$4, $4, 0xff
  143828: 40 00 b0 ff  	sd	$16, 0x40($sp)
  14382c: ff 00 a5 30  	andi	$5, $5, 0xff
  143830: 20 00 a9 8d  	lw	$9, 0x20($13)
  143834: d0 0a 4b 94  	lhu	$11, 0xad0($2)
  143838: 76 08 47 94  	lhu	$7, 0x876($2)
  14383c: 36 00 02 3c  	lui	$2, 0x36
  143840: 80 d4 4e 24  	addiu	$14, $2, -0x2b80 <.text+0xffffffffffefd480>
  143844: 2c 00 a8 8d  	lw	$8, 0x2c($13)
  143848: 05 00 02 24  	addiu	$2, $zero, 0x5
  14384c: 85 00 8a 91  	lbu	$10, 0x85($12)
  143850: 08 00 62 ac  	sw	$2, 0x8($3)
  143854: 80 00 02 24  	addiu	$2, $zero, 0x80
  143858: 18 00 62 ac  	sw	$2, 0x18($3)
  14385c: 07 00 02 24  	addiu	$2, $zero, 0x7
  143860: 20 00 62 ac  	sw	$2, 0x20($3)
  143864: 01 00 02 24  	addiu	$2, $zero, 0x1
  143868: 00 00 a4 af  	sw	$4, 0x0($sp)
  14386c: 0c 00 67 ac  	sw	$7, 0xc($3)
  143870: 04 00 a5 af  	sw	$5, 0x4($sp)
  143874: 1c 00 66 ac  	sw	$6, 0x1c($3)
  143878: 24 00 69 ac  	sw	$9, 0x24($3)
  14387c: 28 00 68 ac  	sw	$8, 0x28($3)
  143880: 10 00 6b ac  	sw	$11, 0x10($3)
  143884: 2c 00 60 a0  	sb	$zero, 0x2c($3)
  143888: 38 00 c2 ad  	sw	$2, 0x38($14)
  14388c: 16 00 40 11  	beqz	$10, 0x1438e8 <.text+0x438e8>
  143890: 04 00 66 ac  	sw	$6, 0x4($3)
  143894: 88 b7 e2 91  	lbu	$2, -0x4878($15)
  143898: fb ff 42 24  	addiu	$2, $2, -0x5 <.text+0xffffffffffeffffb>
  14389c: 02 00 42 2c  	sltiu	$2, $2, 0x2
  1438a0: 67 01 40 50  	beqzl	$2, 0x143e40 <.text+0x43e40>
  1438a4: 83 00 82 91  	lbu	$2, 0x83($12)
  1438a8: 35 00 a3 91  	lbu	$3, 0x35($13)
  1438ac: 02 00 02 24  	addiu	$2, $zero, 0x2
  1438b0: 38 00 c2 ad  	sw	$2, 0x38($14)
  1438b4: 52 01 60 10  	beqz	$3, 0x143e00 <.text+0x43e00>
  1438b8: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1438bc: 83 00 82 91  	lbu	$2, 0x83($12)
  1438c0: 49 01 40 10  	beqz	$2, 0x143de8 <.text+0x43de8>
  1438c4: 18 00 02 3c  	lui	$2, 0x18
  1438c8: 18 00 02 3c  	lui	$2, 0x18
  1438cc: 36 00 03 3c  	lui	$3, 0x36
  1438d0: f4 6c 42 24  	addiu	$2, $2, 0x6cf4
  1438d4: 8c f9 62 ac  	sw	$2, -0x674($3)
  1438d8: 18 00 02 3c  	lui	$2, 0x18
  1438dc: 10 70 42 24  	addiu	$2, $2, 0x7010
  1438e0: 36 00 03 3c  	lui	$3, 0x36
  1438e4: 90 f9 62 ac  	sw	$2, -0x670($3)
  1438e8: 04 00 a2 8f  	lw	$2, 0x4($sp)
  1438ec: 01 00 04 24  	addiu	$4, $zero, 0x1
  1438f0: 08 00 a4 af  	sw	$4, 0x8($sp)
  1438f4: 02 00 43 24  	addiu	$3, $2, 0x2
  1438f8: 36 00 02 3c  	lui	$2, 0x36
  1438fc: 80 d4 42 24  	addiu	$2, $2, -0x2b80 <.text+0xffffffffffefd480>
  143900: 70 00 57 8c  	lw	$23, 0x70($2)
  143904: f5 00 e0 06  	bltz	$23, 0x143cdc <.text+0x43cdc>
  143908: 4c 00 43 a0  	sb	$3, 0x4c($2)
  14390c: 36 00 02 3c  	lui	$2, 0x36
  143910: 80 20 17 00  	sll	$4, $23, 0x2
  143914: 80 d4 45 24  	addiu	$5, $2, -0x2b80 <.text+0xffffffffffefd480>
  143918: 01 00 06 24  	addiu	$6, $zero, 0x1
  14391c: 21 20 85 00  	addu	$4, $4, $5
  143920: 5c 00 a3 8c  	lw	$3, 0x5c($5)
  143924: 30 06 87 8c  	lw	$7, 0x630($4)
  143928: b8 08 84 8c  	lw	$4, 0x8b8($4)
  14392c: 14 00 a6 af  	sw	$6, 0x14($sp)
  143930: 21 10 87 00  	addu	$2, $4, $7
  143934: 10 00 a7 af  	sw	$7, 0x10($sp)
  143938: 2a 18 62 00  	slt	$3, $3, $2
  14393c: dd 00 60 10  	beqz	$3, 0x143cb4 <.text+0x43cb4>
  143940: 0c 00 a4 af  	sw	$4, 0xc($sp)
  143944: 60 00 a2 8c  	lw	$2, 0x60($5)
  143948: 2a 10 44 00  	slt	$2, $2, $4
  14394c: da 00 40 54  	bnezl	$2, 0x143cb8 <.text+0x43cb8>
  143950: 08 00 a4 8f  	lw	$4, 0x8($sp)
  143954: 00 00 a2 8f  	lw	$2, 0x0($sp)
  143958: 12 00 40 10  	beqz	$2, 0x1439a4 <.text+0x439a4>
  14395c: 0c 00 03 24  	addiu	$3, $zero, 0xc
  143960: bf 0a a2 90  	lbu	$2, 0xabf($5)
  143964: 10 00 42 30  	andi	$2, $2, 0x10
  143968: 0f 00 40 10  	beqz	$2, 0x1439a8 <.text+0x439a8>
  14396c: 36 00 02 3c  	lui	$2, 0x36
  143970: c0 0a a2 90  	lbu	$2, 0xac0($5)
  143974: 08 00 40 14  	bnez	$2, 0x143998 <.text+0x43998>
  143978: 2d 20 00 00  	move	$4, $zero
  14397c: 0c 00 02 24  	addiu	$2, $zero, 0xc
