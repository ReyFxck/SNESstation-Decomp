
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  12a400: 36 00 02 3c  	lui	$2, 0x36
  12a404: 92 b7 43 94  	lhu	$3, -0x486e($2)
  12a408: 36 00 02 3c  	lui	$2, 0x36
  12a40c: 6e 00 60 14  	bnez	$3, 0x12a5c8 <.text+0x2a5c8>
  12a410: 9c c2 45 a0  	sb	$5, -0x3d64($2)
  12a414: 02 00 a2 2a  	slti	$2, $21, 0x2
  12a418: 48 00 40 14  	bnez	$2, 0x12a53c <.text+0x2a53c>
  12a41c: 01 00 02 24  	addiu	$2, $zero, 0x1
  12a420: 36 00 02 3c  	lui	$2, 0x36
  12a424: 88 b7 46 24  	addiu	$6, $2, -0x4878 <.text+0xffffffffffefb788>
  12a428: 35 00 02 3c  	lui	$2, 0x35
  12a42c: 06 00 c3 94  	lhu	$3, 0x6($6)
  12a430: b0 e2 49 24  	addiu	$9, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  12a434: 08 00 24 8d  	lw	$4, 0x8($9)
  12a438: 21 10 30 02  	addu	$2, $17, $16
  12a43c: 40 18 03 00  	sll	$3, $3, 0x1
  12a440: 00 00 48 90  	lbu	$8, 0x0($2)
  12a444: ff ff 63 30  	andi	$3, $3, 0xffff
  12a448: 36 00 02 3c  	lui	$2, 0x36
  12a44c: 21 20 83 00  	addu	$4, $4, $3
  12a450: 68 c2 47 24  	addiu	$7, $2, -0x3d98 <.text+0xffffffffffefc268>
  12a454: 00 00 88 a0  	sb	$8, 0x0($4)
  12a458: 02 29 03 00  	srl	$5, $3, 0x4
  12a45c: 42 21 03 00  	srl	$4, $3, 0x5
  12a460: 82 19 03 00  	srl	$3, $3, 0x6
  12a464: 28 00 e2 8c  	lw	$2, 0x28($7)
  12a468: 21 10 45 00  	addu	$2, $2, $5
  12a46c: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a470: 2c 00 e2 8c  	lw	$2, 0x2c($7)
  12a474: 21 10 44 00  	addu	$2, $2, $4
  12a478: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a47c: 30 00 e2 8c  	lw	$2, 0x30($7)
  12a480: 21 10 43 00  	addu	$2, $2, $3
  12a484: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a488: 04 00 c2 90  	lbu	$2, 0x4($6)
  12a48c: 06 00 40 14  	bnez	$2, 0x12a4a8 <.text+0x2a4a8>
  12a490: 14 00 a5 8f  	lw	$5, 0x14($sp)
  12a494: 06 00 c2 94  	lhu	$2, 0x6($6)
  12a498: 05 00 c3 90  	lbu	$3, 0x5($6)
  12a49c: 21 10 43 00  	addu	$2, $2, $3
  12a4a0: 06 00 c2 a4  	sh	$2, 0x6($6)
  12a4a4: 14 00 a5 8f  	lw	$5, 0x14($sp)
  12a4a8: 06 00 c3 94  	lhu	$3, 0x6($6)
  12a4ac: 21 10 05 02  	addu	$2, $16, $5
  12a4b0: ff ff 50 30  	andi	$16, $2, 0xffff
  12a4b4: 40 18 03 00  	sll	$3, $3, 0x1
  12a4b8: 08 00 22 8d  	lw	$2, 0x8($9)
  12a4bc: 21 20 30 02  	addu	$4, $17, $16
  12a4c0: 01 00 63 24  	addiu	$3, $3, 0x1
  12a4c4: 00 00 88 90  	lbu	$8, 0x0($4)
  12a4c8: ff ff 63 30  	andi	$3, $3, 0xffff
  12a4cc: 21 10 43 00  	addu	$2, $2, $3
  12a4d0: 02 21 03 00  	srl	$4, $3, 0x4
  12a4d4: 00 00 48 a0  	sb	$8, 0x0($2)
  12a4d8: 42 29 03 00  	srl	$5, $3, 0x5
  12a4dc: 82 19 03 00  	srl	$3, $3, 0x6
  12a4e0: 28 00 e2 8c  	lw	$2, 0x28($7)
  12a4e4: 21 10 44 00  	addu	$2, $2, $4
  12a4e8: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a4ec: 2c 00 e2 8c  	lw	$2, 0x2c($7)
  12a4f0: 21 10 45 00  	addu	$2, $2, $5
  12a4f4: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a4f8: 30 00 e2 8c  	lw	$2, 0x30($7)
  12a4fc: 21 10 43 00  	addu	$2, $2, $3
  12a500: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a504: 04 00 c2 90  	lbu	$2, 0x4($6)
  12a508: 06 00 40 10  	beqz	$2, 0x12a524 <.text+0x2a524>
  12a50c: 14 00 a8 8f  	lw	$8, 0x14($sp)
  12a510: 06 00 c2 94  	lhu	$2, 0x6($6)
  12a514: 05 00 c3 90  	lbu	$3, 0x5($6)
  12a518: 21 10 43 00  	addu	$2, $2, $3
  12a51c: 06 00 c2 a4  	sh	$2, 0x6($6)
  12a520: 14 00 a8 8f  	lw	$8, 0x14($sp)
  12a524: fe ff b5 26  	addiu	$21, $21, -0x2 <.text+0xffffffffffeffffe>
  12a528: 02 00 a3 2a  	slti	$3, $21, 0x2
  12a52c: 21 10 08 02  	addu	$2, $16, $8
  12a530: bb ff 60 10  	beqz	$3, 0x12a420 <.text+0x2a420>
  12a534: ff ff 50 30  	andi	$16, $2, 0xffff
  12a538: 01 00 02 24  	addiu	$2, $zero, 0x1
  12a53c: 17 fe a2 16  	bne	$21, $2, 0x129d9c <.text+0x29d9c>
  12a540: 34 00 02 3c  	lui	$2, 0x34
  12a544: 36 00 02 3c  	lui	$2, 0x36
  12a548: 21 28 30 02  	addu	$5, $17, $16
  12a54c: 88 b7 47 24  	addiu	$7, $2, -0x4878 <.text+0xffffffffffefb788>
  12a550: 00 00 a8 90  	lbu	$8, 0x0($5)
  12a554: 06 00 e3 94  	lhu	$3, 0x6($7)
  12a558: 35 00 02 3c  	lui	$2, 0x35
  12a55c: b8 e2 42 8c  	lw	$2, -0x1d48($2)
  12a560: 36 00 04 3c  	lui	$4, 0x36
  12a564: 40 18 03 00  	sll	$3, $3, 0x1
  12a568: 68 c2 84 24  	addiu	$4, $4, -0x3d98 <.text+0xffffffffffefc268>
  12a56c: ff ff 63 30  	andi	$3, $3, 0xffff
  12a570: 21 10 43 00  	addu	$2, $2, $3
  12a574: 02 29 03 00  	srl	$5, $3, 0x4
  12a578: 00 00 48 a0  	sb	$8, 0x0($2)
  12a57c: 42 31 03 00  	srl	$6, $3, 0x5
  12a580: 82 19 03 00  	srl	$3, $3, 0x6
  12a584: 28 00 82 8c  	lw	$2, 0x28($4)
  12a588: 21 10 45 00  	addu	$2, $2, $5
  12a58c: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a590: 2c 00 82 8c  	lw	$2, 0x2c($4)
  12a594: 21 10 46 00  	addu	$2, $2, $6
  12a598: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a59c: 30 00 82 8c  	lw	$2, 0x30($4)
  12a5a0: 21 10 43 00  	addu	$2, $2, $3
  12a5a4: 00 00 40 a0  	sb	$zero, 0x0($2)
  12a5a8: 04 00 e2 90  	lbu	$2, 0x4($7)
  12a5ac: fb fd 40 14  	bnez	$2, 0x129d9c <.text+0x29d9c>
  12a5b0: 34 00 02 3c  	lui	$2, 0x34
  12a5b4: 06 00 e2 94  	lhu	$2, 0x6($7)
  12a5b8: 05 00 e3 90  	lbu	$3, 0x5($7)
  12a5bc: 21 10 43 00  	addu	$2, $2, $3
  12a5c0: f5 fd 00 10  	b	0x129d98 <.text+0x29d98>
  12a5c4: 06 00 e2 a4  	sh	$2, 0x6($7)
  12a5c8: 02 00 a2 2a  	slti	$2, $21, 0x2
  12a5cc: 60 00 40 14  	bnez	$2, 0x12a750 <.text+0x2a750>
  12a5d0: 01 00 02 24  	addiu	$2, $zero, 0x1
  12a5d4: 36 00 02 3c  	lui	$2, 0x36
  12a5d8: 36 00 04 3c  	lui	$4, 0x36
  12a5dc: 88 b7 4a 24  	addiu	$10, $2, -0x4878 <.text+0xffffffffffefb788>
