# SNES Station v0.23 — Progress 13 focused target assembly
# Extracted from the authoritative unpacked ELF disassembly.
# Addresses are EE virtual addresses; no matching claim is implied.

## APU mirrored write/timer targets [0x0010b72c..0x0010b7f8)
  10b72c: 34 00 02 3c  	lui	$2, 0x34
  10b730: ff ff 84 30  	andi	$4, $4, 0xffff
  10b734: 98 54 43 24  	addiu	$3, $2, 0x5498
  10b738: 04 00 62 8c  	lw	$2, 0x4($3)
  10b73c: 21 10 44 00  	addu	$2, $2, $4
  10b740: 00 00 45 a0  	sb	$5, 0x0($2)
  10b744: fb 00 02 24  	addiu	$2, $zero, 0xfb
  10b748: 20 00 82 10  	beq	$4, $2, 0x10b7cc <.text+0xb7cc>
  10b74c: fc 00 82 28  	slti	$2, $4, 0xfc
  10b750: 10 00 40 10  	beqz	$2, 0x10b794 <.text+0xb794>
  10b754: fc 00 02 24  	addiu	$2, $zero, 0xfc
  10b758: fa 00 02 24  	addiu	$2, $zero, 0xfa
  10b75c: 03 00 82 50  	beql	$4, $2, 0x10b76c <.text+0xb76c>
  10b760: 04 00 62 8c  	lw	$2, 0x4($3)
  10b764: 08 00 e0 03  	jr	$ra
  10b768: 00 00 00 00  	nop
  10b76c: 34 00 03 3c  	lui	$3, 0x34
  10b770: b8 53 63 24  	addiu	$3, $3, 0x53b8
  10b774: fa 00 42 90  	lbu	$2, 0xfa($2)
  10b778: 03 00 40 14  	bnez	$2, 0x10b788 <.text+0xb788>
  10b77c: d2 00 62 a4  	sh	$2, 0xd2($3)
  10b780: 00 01 02 24  	addiu	$2, $zero, 0x100
  10b784: d2 00 62 a4  	sh	$2, 0xd2($3)
  10b788: 01 00 02 24  	addiu	$2, $zero, 0x1
  10b78c: 08 00 e0 03  	jr	$ra
  10b790: db 00 62 a0  	sb	$2, 0xdb($3)
  10b794: 03 00 82 50  	beql	$4, $2, 0x10b7a4 <.text+0xb7a4>
  10b798: 04 00 62 8c  	lw	$2, 0x4($3)
  10b79c: 08 00 e0 03  	jr	$ra
  10b7a0: 00 00 00 00  	nop
  10b7a4: 34 00 03 3c  	lui	$3, 0x34
  10b7a8: b8 53 63 24  	addiu	$3, $3, 0x53b8
  10b7ac: fc 00 42 90  	lbu	$2, 0xfc($2)
  10b7b0: 03 00 40 14  	bnez	$2, 0x10b7c0 <.text+0xb7c0>
  10b7b4: d6 00 62 a4  	sh	$2, 0xd6($3)
  10b7b8: 00 01 02 24  	addiu	$2, $zero, 0x100
  10b7bc: d6 00 62 a4  	sh	$2, 0xd6($3)
  10b7c0: 01 00 02 24  	addiu	$2, $zero, 0x1
  10b7c4: 08 00 e0 03  	jr	$ra
  10b7c8: dd 00 62 a0  	sb	$2, 0xdd($3)
  10b7cc: 04 00 62 8c  	lw	$2, 0x4($3)
  10b7d0: 34 00 03 3c  	lui	$3, 0x34
  10b7d4: b8 53 63 24  	addiu	$3, $3, 0x53b8
  10b7d8: fb 00 42 90  	lbu	$2, 0xfb($2)
  10b7dc: 03 00 40 14  	bnez	$2, 0x10b7ec <.text+0xb7ec>
  10b7e0: d4 00 62 a4  	sh	$2, 0xd4($3)
  10b7e4: 00 01 02 24  	addiu	$2, $zero, 0x100
  10b7e8: d4 00 62 a4  	sh	$2, 0xd4($3)
  10b7ec: 01 00 02 24  	addiu	$2, $zero, 0x1
  10b7f0: 08 00 e0 03  	jr	$ra
  10b7f4: dc 00 62 a0  	sb	$2, 0xdc($3)

## core gate predicate [0x001308f8..0x001309c4)
  1308f8: 34 00 02 3c  	lui	$2, 0x34
  1308fc: 38 2a 43 24  	addiu	$3, $2, 0x2a38
  130900: ec 05 62 90  	lbu	$2, 0x5ec($3)
  130904: 0b 00 40 10  	beqz	$2, 0x130934 <.text+0x30934>
  130908: 34 00 02 3c  	lui	$2, 0x34
  13090c: 58 00 64 8c  	lw	$4, 0x58($3)
  130910: 3c 00 63 8c  	lw	$3, 0x3c($3)
  130914: 2b 10 64 00  	sltu	$2, $3, $4
  130918: 06 00 40 14  	bnez	$2, 0x130934 <.text+0x30934>
  13091c: 34 00 02 3c  	lui	$2, 0x34
  130920: 00 02 82 24  	addiu	$2, $4, 0x200
  130924: 2b 10 62 00  	sltu	$2, $3, $2
  130928: 24 00 40 14  	bnez	$2, 0x1309bc <.text+0x309bc>
  13092c: 01 00 04 24  	addiu	$4, $zero, 0x1
  130930: 34 00 02 3c  	lui	$2, 0x34
  130934: 38 2a 43 24  	addiu	$3, $2, 0x2a38
  130938: 4c 00 62 8c  	lw	$2, 0x4c($3)
  13093c: 40 00 42 2c  	sltiu	$2, $2, 0x40
  130940: 07 00 40 10  	beqz	$2, 0x130960 <.text+0x30960>
  130944: 34 00 02 3c  	lui	$2, 0x34
  130948: 3c 00 63 8c  	lw	$3, 0x3c($3)
  13094c: ff 7f 02 24  	addiu	$2, $zero, 0x7fff
  130950: 2b 10 43 00  	sltu	$2, $2, $3
  130954: 19 00 40 10  	beqz	$2, 0x1309bc <.text+0x309bc>
  130958: 2d 20 00 00  	move	$4, $zero
  13095c: 34 00 02 3c  	lui	$2, 0x34
  130960: 38 2a 45 24  	addiu	$5, $2, 0x2a38
  130964: 4c 00 a3 8c  	lw	$3, 0x4c($5)
  130968: a0 ff 62 24  	addiu	$2, $3, -0x60 <.text+0xffffffffffefffa0>
  13096c: 10 00 42 2c  	sltiu	$2, $2, 0x10
  130970: 12 00 40 14  	bnez	$2, 0x1309bc <.text+0x309bc>
  130974: 2d 20 00 00  	move	$4, $zero
  130978: 74 00 62 2c  	sltiu	$2, $3, 0x74
  13097c: 0f 00 40 10  	beqz	$2, 0x1309bc <.text+0x309bc>
  130980: 00 00 00 00  	nop
  130984: 90 ff 62 24  	addiu	$2, $3, -0x70 <.text+0xffffffffffefff90>
  130988: 04 00 42 2c  	sltiu	$2, $2, 0x4
  13098c: 07 00 40 10  	beqz	$2, 0x1309ac <.text+0x309ac>
  130990: 34 00 02 3c  	lui	$2, 0x34
  130994: 98 00 a2 8c  	lw	$2, 0x98($5)
  130998: 3a 00 42 90  	lbu	$2, 0x3a($2)
  13099c: 08 00 42 30  	andi	$2, $2, 0x8
  1309a0: 06 00 40 10  	beqz	$2, 0x1309bc <.text+0x309bc>
  1309a4: 00 00 00 00  	nop
  1309a8: 34 00 02 3c  	lui	$2, 0x34
  1309ac: d0 2a 42 8c  	lw	$2, 0x2ad0($2)
  1309b0: 3a 00 42 90  	lbu	$2, 0x3a($2)
  1309b4: 10 00 42 30  	andi	$2, $2, 0x10
  1309b8: 2b 20 02 00  	sltu	$4, $zero, $2
  1309bc: 08 00 e0 03  	jr	$ra
  1309c0: 2d 10 80 00  	move	$2, $4

## palette component expansion [0x001591a8..0x00159268)
  1591a8: 36 00 02 3c  	lui	$2, 0x36
  1591ac: 8a b7 43 90  	lbu	$3, -0x4876($2)
  1591b0: 34 00 02 3c  	lui	$2, 0x34
  1591b4: 63 55 44 90  	lbu	$4, 0x5563($2)
  1591b8: 34 00 02 3c  	lui	$2, 0x34
  1591bc: c8 ca 42 24  	addiu	$2, $2, -0x3538 <.text+0xffffffffffefcac8>
  1591c0: 40 19 03 00  	sll	$3, $3, 0x5
  1591c4: 21 18 62 00  	addu	$3, $3, $2
  1591c8: 36 00 02 3c  	lui	$2, 0x36
  1591cc: 24 00 80 10  	beqz	$4, 0x159260 <.text+0x59260>
  1591d0: ac ce 43 ac  	sw	$3, -0x3154($2)
  1591d4: 2d 50 00 00  	move	$10, $zero
  1591d8: 36 00 02 3c  	lui	$2, 0x36
  1591dc: 40 40 0a 00  	sll	$8, $10, 0x1
  1591e0: 88 b7 42 24  	addiu	$2, $2, -0x4878 <.text+0xffffffffffefb788>
  1591e4: 36 00 05 3c  	lui	$5, 0x36
  1591e8: 21 10 02 01  	addu	$2, $8, $2
  1591ec: 68 c2 a5 24  	addiu	$5, $5, -0x3d98 <.text+0xffffffffffefc268>
  1591f0: 40 00 42 94  	lhu	$2, 0x40($2)
  1591f4: 80 30 0a 00  	sll	$6, $10, 0x2
  1591f8: 44 0c a7 8c  	lw	$7, 0xc44($5)
  1591fc: 21 30 c5 00  	addu	$6, $6, $5
  159200: 1f 00 43 30  	andi	$3, $2, 0x1f
  159204: 42 21 02 00  	srl	$4, $2, 0x5
  159208: 21 18 e3 00  	addu	$3, $7, $3
  15920c: 1f 00 84 30  	andi	$4, $4, 0x1f
  159210: 00 00 63 90  	lbu	$3, 0x0($3)
  159214: 21 20 e4 00  	addu	$4, $7, $4
  159218: 82 12 02 00  	srl	$2, $2, 0xa
  15921c: 21 40 05 01  	addu	$8, $8, $5
  159220: 44 00 c3 ac  	sw	$3, 0x44($6)
  159224: 1f 00 42 30  	andi	$2, $2, 0x1f
  159228: 21 38 e2 00  	addu	$7, $7, $2
  15922c: 01 00 4a 25  	addiu	$10, $10, 0x1
  159230: 00 00 84 90  	lbu	$4, 0x0($4)
  159234: 00 01 49 29  	slti	$9, $10, 0x100
  159238: 44 00 c5 94  	lhu	$5, 0x44($6)
  15923c: 44 04 c4 ac  	sw	$4, 0x444($6)
  159240: 40 21 04 00  	sll	$4, $4, 0x5
  159244: 00 00 e3 90  	lbu	$3, 0x0($7)
  159248: 80 12 03 00  	sll	$2, $3, 0xa
  15924c: 44 08 c3 ac  	sw	$3, 0x844($6)
  159250: 25 10 44 00  	or	$2, $2, $4
  159254: 25 28 a2 00  	or	$5, $5, $2
  159258: df ff 20 15  	bnez	$9, 0x1591d8 <.text+0x591d8>
  15925c: 48 0c 05 a5  	sh	$5, 0xc48($8)
  159260: 08 00 e0 03  	jr	$ra
  159264: 00 00 00 00  	nop

## PPU reset helper [0x0015d8ec..0x0015d9ac)
  15d8ec: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  15d8f0: 2d 28 00 00  	move	$5, $zero
  15d8f4: 20 00 bf ff  	sd	$ra, 0x20($sp)
  15d8f8: 00 02 06 24  	addiu	$6, $zero, 0x200
  15d8fc: 10 00 b1 ff  	sd	$17, 0x10($sp)
  15d900: 35 00 11 3c  	lui	$17, 0x35
  15d904: 00 00 b0 ff  	sd	$16, 0x0($sp)
  15d908: b0 e2 31 26  	addiu	$17, $17, -0x1d50 <.text+0xffffffffffefe2b0>
  15d90c: 34 00 10 3c  	lui	$16, 0x34
  15d910: f8 5a 10 26  	addiu	$16, $16, 0x5af8
  15d914: 14 00 24 8e  	lw	$4, 0x14($17)
  15d918: 19 00 00 a2  	sb	$zero, 0x19($16)
  15d91c: 1a 00 00 a2  	sb	$zero, 0x1a($16)
  15d920: 00 22 84 24  	addiu	$4, $4, 0x2200
  15d924: 1b 00 00 a2  	sb	$zero, 0x1b($16)
  15d928: 1c 00 00 a2  	sb	$zero, 0x1c($16)
  15d92c: 14 00 00 ae  	sw	$zero, 0x14($16)
  15d930: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  15d934: 18 00 00 a2  	sb	$zero, 0x18($16)
  15d938: 20 00 bf df  	ld	$ra, 0x20($sp)
  15d93c: 14 00 23 8e  	lw	$3, 0x14($17)
  15d940: 20 00 02 24  	addiu	$2, $zero, 0x20
  15d944: 00 22 62 a0  	sb	$2, 0x2200($3)
  15d948: 00 80 02 34  	ori	$2, $zero, 0x8000
  15d94c: 21 80 02 02  	addu	$16, $16, $2
  15d950: 01 00 02 24  	addiu	$2, $zero, 0x1
  15d954: 14 00 23 8e  	lw	$3, 0x14($17)
  15d958: 20 22 60 a0  	sb	$zero, 0x2220($3)
  15d95c: 14 00 23 8e  	lw	$3, 0x14($17)
  15d960: 21 22 62 a0  	sb	$2, 0x2221($3)
  15d964: 02 00 02 24  	addiu	$2, $zero, 0x2
  15d968: 14 00 23 8e  	lw	$3, 0x14($17)
  15d96c: 22 22 62 a0  	sb	$2, 0x2222($3)
  15d970: 03 00 02 24  	addiu	$2, $zero, 0x3
  15d974: 14 00 23 8e  	lw	$3, 0x14($17)
  15d978: 23 22 62 a0  	sb	$2, 0x2223($3)
  15d97c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  15d980: 14 00 23 8e  	lw	$3, 0x14($17)
  15d984: 10 00 b1 df  	ld	$17, 0x10($sp)
  15d988: 28 22 62 a0  	sb	$2, 0x2228($3)
  15d98c: 50 00 00 a2  	sb	$zero, 0x50($16)
  15d990: 40 00 00 a6  	sh	$zero, 0x40($16)
  15d994: 42 00 00 a6  	sh	$zero, 0x42($16)
  15d998: 44 00 00 ae  	sw	$zero, 0x44($16)
  15d99c: 48 00 00 fe  	sd	$zero, 0x48($16)
  15d9a0: 00 00 b0 df  	ld	$16, 0x0($sp)
  15d9a4: 08 00 e0 03  	jr	$ra
  15d9a8: 30 00 bd 27  	addiu	$sp, $sp, 0x30

## memory map window fill [0x0015e198..0x0015e298)
  15e198: 40 4a 04 00  	sll	$9, $4, 0x9
  15e19c: 02 00 83 2c  	sltiu	$3, $4, 0x2
  15e1a0: 00 04 22 25  	addiu	$2, $9, 0x400
  15e1a4: 00 22 04 00  	sll	$4, $4, 0x8
  15e1a8: 0a 48 43 00  	movz	$9, $2, $3
  15e1ac: ff 00 a5 30  	andi	$5, $5, 0xff
  15e1b0: 00 0c 8a 24  	addiu	$10, $4, 0xc00
  15e1b4: 2d 30 00 00  	move	$6, $zero
  15e1b8: 35 00 02 3c  	lui	$2, 0x35
  15e1bc: 07 00 ab 30  	andi	$11, $5, 0x7
  15e1c0: 00 1b 06 00  	sll	$3, $6, 0xc
  15e1c4: b4 e2 44 8c  	lw	$4, -0x1d4c($2)
  15e1c8: 00 15 0b 00  	sll	$2, $11, 0x14
  15e1cc: 10 00 c8 24  	addiu	$8, $6, 0x10
  15e1d0: 21 10 43 00  	addu	$2, $2, $3
  15e1d4: 21 18 ca 00  	addu	$3, $6, $10
  15e1d8: 21 20 82 00  	addu	$4, $4, $2
  15e1dc: 80 38 03 00  	sll	$7, $3, 0x2
  15e1e0: 23 30 06 01  	subu	$6, $8, $6
  15e1e4: 35 00 03 3c  	lui	$3, 0x35
  15e1e8: 34 00 02 3c  	lui	$2, 0x34
  15e1ec: b0 e2 63 24  	addiu	$3, $3, -0x1d50 <.text+0xffffffffffefe2b0>
  15e1f0: f8 5a 42 24  	addiu	$2, $2, 0x5af8
  15e1f4: 21 18 e3 00  	addu	$3, $7, $3
  15e1f8: 21 10 e2 00  	addu	$2, $7, $2
  15e1fc: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  15e200: 40 00 44 ac  	sw	$4, 0x40($2)
  15e204: 28 00 64 ac  	sw	$4, 0x28($3)
  15e208: f6 ff c0 14  	bnez	$6, 0x15e1e4 <.text+0x5e1e4>
  15e20c: 04 00 e7 24  	addiu	$7, $7, 0x4
  15e210: 00 01 02 29  	slti	$2, $8, 0x100
  15e214: e8 ff 40 14  	bnez	$2, 0x15e1b8 <.text+0x5e1b8>
  15e218: 2d 30 00 01  	move	$6, $8
  15e21c: 2d 30 00 00  	move	$6, $zero
  15e220: 35 00 02 3c  	lui	$2, 0x35
  15e224: 00 1d 0b 00  	sll	$3, $11, 0x14
  15e228: b4 e2 44 8c  	lw	$4, -0x1d4c($2)
  15e22c: 10 00 c7 24  	addiu	$7, $6, 0x10
  15e230: c0 12 06 00  	sll	$2, $6, 0xb
  15e234: 08 00 c5 24  	addiu	$5, $6, 0x8
  15e238: 21 18 62 00  	addu	$3, $3, $2
  15e23c: 2a 10 a7 00  	slt	$2, $5, $7
  15e240: 21 20 83 00  	addu	$4, $4, $3
  15e244: 0f 00 40 10  	beqz	$2, 0x15e284 <.text+0x5e284>
  15e248: 00 80 84 24  	addiu	$4, $4, -0x8000 <.text+0xffffffffffef8000>
  15e24c: 21 10 a9 00  	addu	$2, $5, $9
  15e250: 23 28 e5 00  	subu	$5, $7, $5
  15e254: 80 30 02 00  	sll	$6, $2, 0x2
  15e258: 35 00 03 3c  	lui	$3, 0x35
  15e25c: 34 00 02 3c  	lui	$2, 0x34
  15e260: b0 e2 63 24  	addiu	$3, $3, -0x1d50 <.text+0xffffffffffefe2b0>
  15e264: f8 5a 42 24  	addiu	$2, $2, 0x5af8
  15e268: 21 18 c3 00  	addu	$3, $6, $3
  15e26c: 21 10 c2 00  	addu	$2, $6, $2
  15e270: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  15e274: 40 00 44 ac  	sw	$4, 0x40($2)
  15e278: 28 00 64 ac  	sw	$4, 0x28($3)
  15e27c: f6 ff a0 14  	bnez	$5, 0x15e258 <.text+0x5e258>
  15e280: 04 00 c6 24  	addiu	$6, $6, 0x4
  15e284: 00 02 e2 28  	slti	$2, $7, 0x200
  15e288: e5 ff 40 14  	bnez	$2, 0x15e220 <.text+0x5e220>
  15e28c: 2d 30 e0 00  	move	$6, $7
  15e290: 08 00 e0 03  	jr	$ra
  15e294: 00 00 00 00  	nop

## signed-angle pair transform [0x0016fd08..0x0016fe00)
  16fd08: 00 24 04 00  	sll	$4, $4, 0x10
  16fd0c: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  16fd10: 03 24 04 00  	sra	$4, $4, 0x10
  16fd14: 34 00 b6 e7  	swc1	$f22, 0x34($sp)
  16fd18: 00 b0 84 44  	mtc1	$4, $f22
  16fd1c: 00 00 00 00  	nop
  16fd20: a0 b5 80 46  	cvt.s.w	$f22, $f22
  16fd24: c9 38 01 3c  	lui	$1, 0x38c9
  16fd28: db 0f 21 34  	ori	$1, $1, 0xfdb
  16fd2c: 00 00 81 44  	mtc1	$1, $f0
  16fd30: 00 34 06 00  	sll	$6, $6, 0x10
  16fd34: 00 2c 05 00  	sll	$5, $5, 0x10
  16fd38: 03 34 06 00  	sra	$6, $6, 0x10
  16fd3c: 03 2c 05 00  	sra	$5, $5, 0x10
  16fd40: 82 b5 00 46  	mul.s	$f22, $f22, $f0
  16fd44: 3c 00 ba e7  	swc1	$f26, 0x3c($sp)
  16fd48: 00 d0 86 44  	mtc1	$6, $f26
  16fd4c: 00 00 00 00  	nop
  16fd50: a0 d6 80 46  	cvt.s.w	$f26, $f26
  16fd54: 38 00 b8 e7  	swc1	$f24, 0x38($sp)
  16fd58: 00 c0 85 44  	mtc1	$5, $f24
  16fd5c: 00 00 00 00  	nop
  16fd60: 20 c6 80 46  	cvt.s.w	$f24, $f24
  16fd64: 20 00 bf ff  	sd	$ra, 0x20($sp)
  16fd68: 06 b3 00 46  	mov.s	$f12, $f22
  16fd6c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  16fd70: 00 00 b0 ff  	sd	$16, 0x0($sp)
  16fd74: 2d 88 00 01  	move	$17, $8
  16fd78: 2d 80 e0 00  	move	$16, $7
  16fd7c: 09 80 06 0c  	jal	0x1a0024 <.text+0xa0024>
  16fd80: 30 00 b4 e7  	swc1	$f20, 0x30($sp)
  16fd84: 06 b3 00 46  	mov.s	$f12, $f22
  16fd88: 77 7f 06 0c  	jal	0x19fddc <.text+0x9fddc>
  16fd8c: 02 05 1a 46  	mul.s	$f20, $f0, $f26
  16fd90: 02 00 18 46  	mul.s	$f0, $f0, $f24
  16fd94: 06 b3 00 46  	mov.s	$f12, $f22
  16fd98: 00 a5 14 46  	add.s	$f20, $f20, $f20
  16fd9c: 00 00 00 46  	add.s	$f0, $f0, $f0
  16fda0: 00 a5 00 46  	add.s	$f20, $f20, $f0
  16fda4: 24 a0 00 46  	cvt.w.s	$f0, $f20
  16fda8: 00 00 02 44  	mfc1	$2, $f0
  16fdac: 77 7f 06 0c  	jal	0x19fddc <.text+0x9fddc>
  16fdb0: 00 00 02 a6  	sh	$2, 0x0($16)
  16fdb4: 06 b3 00 46  	mov.s	$f12, $f22
  16fdb8: 09 80 06 0c  	jal	0x1a0024 <.text+0xa0024>
  16fdbc: 02 05 1a 46  	mul.s	$f20, $f0, $f26
  16fdc0: 02 00 18 46  	mul.s	$f0, $f0, $f24
  16fdc4: 20 00 bf df  	ld	$ra, 0x20($sp)
  16fdc8: 00 00 b0 df  	ld	$16, 0x0($sp)
  16fdcc: 00 a5 14 46  	add.s	$f20, $f20, $f20
  16fdd0: 3c 00 ba c7  	lwc1	$f26, 0x3c($sp)
  16fdd4: 00 00 00 46  	add.s	$f0, $f0, $f0
  16fdd8: 38 00 b8 c7  	lwc1	$f24, 0x38($sp)
  16fddc: 34 00 b6 c7  	lwc1	$f22, 0x34($sp)
  16fde0: 01 a5 00 46  	sub.s	$f20, $f20, $f0
  16fde4: 24 a0 00 46  	cvt.w.s	$f0, $f20
  16fde8: 00 00 02 44  	mfc1	$2, $f0
  16fdec: 30 00 b4 c7  	lwc1	$f20, 0x30($sp)
  16fdf0: 00 00 22 a6  	sh	$2, 0x0($17)
  16fdf4: 10 00 b1 df  	ld	$17, 0x10($sp)
  16fdf8: 08 00 e0 03  	jr	$ra
  16fdfc: 40 00 bd 27  	addiu	$sp, $sp, 0x40

## runtime block initializer [0x001832a4..0x0018339c)
  1832a4: 41 00 03 3c  	lui	$3, 0x41
  1832a8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1832ac: 08 35 62 24  	addiu	$2, $3, 0x3508
  1832b0: 01 00 07 24  	addiu	$7, $zero, 0x1
  1832b4: 08 35 60 a0  	sb	$zero, 0x3508($3)
  1832b8: 3c 00 44 24  	addiu	$4, $2, 0x3c
  1832bc: 02 00 03 24  	addiu	$3, $zero, 0x2
  1832c0: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1832c4: 01 00 40 a0  	sb	$zero, 0x1($2)
  1832c8: 2d 28 00 00  	move	$5, $zero
  1832cc: 02 00 40 a0  	sb	$zero, 0x2($2)
  1832d0: 01 00 06 3c  	lui	$6, 0x1
  1832d4: 03 00 40 a0  	sb	$zero, 0x3($2)
  1832d8: 04 00 40 a0  	sb	$zero, 0x4($2)
  1832dc: 05 00 40 a0  	sb	$zero, 0x5($2)
  1832e0: 06 00 40 a0  	sb	$zero, 0x6($2)
  1832e4: 07 00 40 a0  	sb	$zero, 0x7($2)
  1832e8: 08 00 40 a0  	sb	$zero, 0x8($2)
  1832ec: 09 00 40 a0  	sb	$zero, 0x9($2)
  1832f0: 0a 00 40 a0  	sb	$zero, 0xa($2)
  1832f4: 0b 00 40 a0  	sb	$zero, 0xb($2)
  1832f8: 0c 00 40 a0  	sb	$zero, 0xc($2)
  1832fc: 0d 00 40 a0  	sb	$zero, 0xd($2)
  183300: 0e 00 40 a0  	sb	$zero, 0xe($2)
  183304: 0f 00 40 a0  	sb	$zero, 0xf($2)
  183308: 10 00 40 a0  	sb	$zero, 0x10($2)
  18330c: 11 00 40 a0  	sb	$zero, 0x11($2)
  183310: 12 00 40 a0  	sb	$zero, 0x12($2)
  183314: 13 00 40 a0  	sb	$zero, 0x13($2)
  183318: 14 00 40 a0  	sb	$zero, 0x14($2)
  18331c: 15 00 40 a0  	sb	$zero, 0x15($2)
  183320: 16 00 40 a0  	sb	$zero, 0x16($2)
  183324: 17 00 40 a0  	sb	$zero, 0x17($2)
  183328: 18 00 40 a0  	sb	$zero, 0x18($2)
  18332c: 19 00 40 a0  	sb	$zero, 0x19($2)
  183330: 1a 00 40 a0  	sb	$zero, 0x1a($2)
  183334: 1b 00 40 a0  	sb	$zero, 0x1b($2)
  183338: 1c 00 40 a0  	sb	$zero, 0x1c($2)
  18333c: 1d 00 40 a0  	sb	$zero, 0x1d($2)
  183340: 1e 00 40 a0  	sb	$zero, 0x1e($2)
  183344: 1f 00 40 a0  	sb	$zero, 0x1f($2)
  183348: 20 00 40 a0  	sb	$zero, 0x20($2)
  18334c: 28 00 43 a0  	sb	$3, 0x28($2)
  183350: 2d 00 47 a0  	sb	$7, 0x2d($2)
  183354: 27 00 47 a0  	sb	$7, 0x27($2)
  183358: 21 00 40 a0  	sb	$zero, 0x21($2)
  18335c: 22 00 40 a0  	sb	$zero, 0x22($2)
  183360: 23 00 40 a0  	sb	$zero, 0x23($2)
  183364: 24 00 40 a0  	sb	$zero, 0x24($2)
  183368: 25 00 40 a0  	sb	$zero, 0x25($2)
  18336c: 26 00 40 a0  	sb	$zero, 0x26($2)
  183370: 29 00 40 a0  	sb	$zero, 0x29($2)
  183374: 2a 00 40 a0  	sb	$zero, 0x2a($2)
  183378: 2b 00 40 a0  	sb	$zero, 0x2b($2)
  18337c: 2c 00 40 a0  	sb	$zero, 0x2c($2)
  183380: 2e 00 40 a0  	sb	$zero, 0x2e($2)
  183384: 2f 00 40 a0  	sb	$zero, 0x2f($2)
  183388: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18338c: 38 00 40 ac  	sw	$zero, 0x38($2)
  183390: 00 00 bf df  	ld	$ra, 0x0($sp)
  183394: 08 00 e0 03  	jr	$ra
  183398: 10 00 bd 27  	addiu	$sp, $sp, 0x10

## __make_dp [0x001a3c90..0x001a3cc0)
  1a3c90: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a3c94: 2d 10 80 00  	move	$2, $4
  1a3c98: 2d 20 a0 03  	move	$4, $sp
  1a3c9c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a3ca0: 00 00 a2 af  	sw	$2, 0x0($sp)
  1a3ca4: 04 00 a5 af  	sw	$5, 0x4($sp)
  1a3ca8: 08 00 a6 af  	sw	$6, 0x8($sp)
  1a3cac: d0 9f 06 0c  	jal	0x1a7f40 <.text+0xa7f40>
  1a3cb0: 10 00 a7 ff  	sd	$7, 0x10($sp)
  1a3cb4: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a3cb8: 08 00 e0 03  	jr	$ra
  1a3cbc: 30 00 bd 27  	addiu	$sp, $sp, 0x30

## __register_frame [0x001a5fa0..0x001a5fec)
  1a5fa0: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a5fa4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a5fa8: 2d 80 80 00  	move	$16, $4
  1a5fac: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a5fb0: 00 00 02 8e  	lw	$2, 0x0($16)
  1a5fb4: 06 00 40 14  	bnez	$2, 0x1a5fd0 <.text+0xa5fd0>
  1a5fb8: 20 00 04 24  	addiu	$4, $zero, 0x20
  1a5fbc: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a5fc0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a5fc4: 08 00 e0 03  	jr	$ra
  1a5fc8: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a5fcc: 00 00 00 00  	nop
  1a5fd0: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1a5fd4: 00 00 00 00  	nop
  1a5fd8: 2d 20 00 02  	move	$4, $16
  1a5fdc: e0 97 06 0c  	jal	0x1a5f80 <.text+0xa5f80>
  1a5fe0: 2d 28 40 00  	move	$5, $2
  1a5fe4: f6 ff 00 10  	b	0x1a5fc0 <.text+0xa5fc0>
  1a5fe8: 10 00 bf df  	ld	$ra, 0x10($sp)

## __register_frame_table [0x001a6058..0x001a608c)
  1a6058: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a605c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a6060: 2d 80 80 00  	move	$16, $4
  1a6064: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a6068: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1a606c: 20 00 04 24  	addiu	$4, $zero, 0x20
  1a6070: 2d 20 00 02  	move	$4, $16
  1a6074: 0e 98 06 0c  	jal	0x1a6038 <.text+0xa6038>
  1a6078: 2d 28 40 00  	move	$5, $2
  1a607c: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a6080: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a6084: 08 00 e0 03  	jr	$ra
  1a6088: 20 00 bd 27  	addiu	$sp, $sp, 0x20

## __deregister_frame [0x001a6188..0x001a61c0)
  1a6188: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a618c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a6190: 00 00 83 8c  	lw	$3, 0x0($4)
  1a6194: 04 00 60 14  	bnez	$3, 0x1a61a8 <.text+0xa61a8>
  1a6198: 00 00 00 00  	nop
  1a619c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a61a0: 08 00 e0 03  	jr	$ra
  1a61a4: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a61a8: 5a 98 06 0c  	jal	0x1a6168 <.text+0xa6168>
  1a61ac: 00 00 00 00  	nop
  1a61b0: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1a61b4: 2d 20 40 00  	move	$4, $2
  1a61b8: f9 ff 00 10  	b	0x1a61a0 <.text+0xa61a0>
  1a61bc: 00 00 bf df  	ld	$ra, 0x0($sp)

## soft-float unpack/make/pack/compare corridor [0x001a7e30..0x001a8420)
  1a7e30: 00 00 82 8c  	lw	$2, 0x0($4)
  1a7e34: 7f 00 06 3c  	lui	$6, 0x7f
  1a7e38: ff ff c6 34  	ori	$6, $6, 0xffff
  1a7e3c: c2 25 02 00  	srl	$4, $2, 0x17
  1a7e40: c2 1f 02 00  	srl	$3, $2, 0x1f
  1a7e44: ff 00 84 30  	andi	$4, $4, 0xff
  1a7e48: 04 00 a3 ac  	sw	$3, 0x4($5)
  1a7e4c: 1c 00 80 14  	bnez	$4, 0x1a7ec0 <.text+0xa7ec0>
  1a7e50: 24 30 46 00  	and	$6, $2, $6
  1a7e54: 18 00 c0 10  	beqz	$6, 0x1a7eb8 <.text+0xa7eb8>
  1a7e58: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a7e5c: ff 3f 02 3c  	lui	$2, 0x3fff
  1a7e60: c0 31 06 00  	sll	$6, $6, 0x7
  1a7e64: ff ff 42 34  	ori	$2, $2, 0xffff
  1a7e68: 82 ff 03 24  	addiu	$3, $zero, -0x7e <.text+0xffffffffffefff82>
  1a7e6c: 03 00 04 24  	addiu	$4, $zero, 0x3
  1a7e70: 2b 10 46 00  	sltu	$2, $2, $6
  1a7e74: 08 00 a3 ac  	sw	$3, 0x8($5)
  1a7e78: 0c 00 40 14  	bnez	$2, 0x1a7eac <.text+0xa7eac>
  1a7e7c: 00 00 a4 ac  	sw	$4, 0x0($5)
  1a7e80: ff 3f 04 3c  	lui	$4, 0x3fff
  1a7e84: ff ff 84 34  	ori	$4, $4, 0xffff
  1a7e88: 40 30 06 00  	sll	$6, $6, 0x1
  1a7e8c: 2b 10 86 00  	sltu	$2, $4, $6
  1a7ea0: f9 ff 40 10  	beqz	$2, 0x1a7e88 <.text+0xa7e88>
  1a7ea4: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  1a7ea8: 08 00 a3 ac  	sw	$3, 0x8($5)
  1a7eac: 08 00 e0 03  	jr	$ra
  1a7eb0: 0c 00 a6 ac  	sw	$6, 0xc($5)
  1a7eb4: 00 00 00 00  	nop
  1a7eb8: 08 00 e0 03  	jr	$ra
  1a7ebc: 00 00 a2 ac  	sw	$2, 0x0($5)
  1a7ec0: ff 00 02 24  	addiu	$2, $zero, 0xff
  1a7ec4: 08 00 82 10  	beq	$4, $2, 0x1a7ee8 <.text+0xa7ee8>
  1a7ec8: c0 11 06 00  	sll	$2, $6, 0x7
  1a7ecc: 00 40 03 3c  	lui	$3, 0x4000
  1a7ed0: 81 ff 84 24  	addiu	$4, $4, -0x7f <.text+0xffffffffffefff81>
  1a7ed4: 25 10 43 00  	or	$2, $2, $3
  1a7ed8: 08 00 a4 ac  	sw	$4, 0x8($5)
  1a7edc: 0c 00 a2 ac  	sw	$2, 0xc($5)
  1a7ee0: f5 ff 00 10  	b	0x1a7eb8 <.text+0xa7eb8>
  1a7ee4: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a7ee8: f3 ff c0 10  	beqz	$6, 0x1a7eb8 <.text+0xa7eb8>
  1a7eec: 04 00 02 24  	addiu	$2, $zero, 0x4
  1a7ef0: 10 00 02 3c  	lui	$2, 0x10
  1a7ef4: 24 10 c2 00  	and	$2, $6, $2
  1a7ef8: ec ff 40 50  	beqzl	$2, 0x1a7eac <.text+0xa7eac>
  1a7efc: 00 00 a0 ac  	sw	$zero, 0x0($5)
  1a7f00: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a7f04: e9 ff 00 10  	b	0x1a7eac <.text+0xa7eac>
  1a7f08: 00 00 a2 ac  	sw	$2, 0x0($5)
  1a7f0c: 00 00 00 00  	nop
  1a7f10: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a7f14: 2d 10 80 00  	move	$2, $4
  1a7f18: 2d 20 a0 03  	move	$4, $sp
  1a7f1c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a7f20: 00 00 a2 af  	sw	$2, 0x0($sp)
  1a7f24: 04 00 a5 af  	sw	$5, 0x4($sp)
  1a7f28: 08 00 a6 af  	sw	$6, 0x8($sp)
  1a7f2c: b0 a0 06 0c  	jal	0x1a82c0 <.text+0xa82c0>
  1a7f30: 0c 00 a7 af  	sw	$7, 0xc($sp)
  1a7f34: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a7f38: 08 00 e0 03  	jr	$ra
  1a7f3c: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a7f40: 00 00 83 8c  	lw	$3, 0x0($4)
  1a7f44: 2d 28 00 00  	move	$5, $zero
  1a7f48: 10 00 87 dc  	ld	$7, 0x10($4)
  1a7f4c: 02 00 62 2c  	sltiu	$2, $3, 0x2
  1a7f50: 1d 00 40 10  	beqz	$2, 0x1a7fc8 <.text+0xa7fc8>
  1a7f54: 04 00 88 8c  	lw	$8, 0x4($4)
  1a7f58: 00 80 02 34  	ori	$2, $zero, 0x8000
  1a7f5c: 3c 11 02 00  	dsll32	$2, $2, 0x4
  1a7f60: ff 07 05 24  	addiu	$5, $zero, 0x7ff
  1a7f64: 25 38 e2 00  	or	$7, $7, $2
  1a7f68: f0 ff 03 34  	ori	$3, $zero, 0xfff0
  1a7f6c: 3c 1c 03 00  	dsll32	$3, $3, 0x10
  1a7f70: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a7f74: 3a 13 02 00  	dsrl	$2, $2, 0xc
  1a7f78: 24 10 e2 00  	and	$2, $7, $2
  1a7f7c: 24 30 c3 00  	and	$6, $6, $3
  1a7f80: 25 30 c2 00  	or	$6, $6, $2
  1a7f84: ff 07 a3 30  	andi	$3, $5, 0x7ff
  1a7f88: 0f 80 02 3c  	lui	$2, 0x800f
  1a7f8c: ff ff 42 34  	ori	$2, $2, 0xffff
  1a7f90: 38 14 02 00  	dsll	$2, $2, 0x10
  1a7f94: ff ff 42 34  	ori	$2, $2, 0xffff
  1a7f98: 38 14 02 00  	dsll	$2, $2, 0x10
  1a7f9c: ff ff 42 34  	ori	$2, $2, 0xffff
  1a7fa0: 3c 1d 03 00  	dsll32	$3, $3, 0x14
  1a7fa4: 24 30 c2 00  	and	$6, $6, $2
  1a7fa8: fc 27 08 00  	dsll32	$4, $8, 0x1f
  1a7fac: 25 30 c3 00  	or	$6, $6, $3
  1a7fb0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a7fb4: 7a 10 02 00  	dsrl	$2, $2, 0x1
  1a7fb8: 24 30 c2 00  	and	$6, $6, $2
  1a7fbc: 08 00 e0 03  	jr	$ra
  1a7fc0: 25 10 c4 00  	or	$2, $6, $4
  1a7fc4: 00 00 00 00  	nop
  1a7fc8: 04 00 62 38  	xori	$2, $3, 0x4
  1a7fcc: 2b 00 40 50  	beqzl	$2, 0x1a807c <.text+0xa807c>
  1a7fd0: ff 07 05 24  	addiu	$5, $zero, 0x7ff
  1a7fd4: 02 00 62 38  	xori	$2, $3, 0x2
  1a7fd8: e3 ff 40 50  	beqzl	$2, 0x1a7f68 <.text+0xa7f68>
  1a7fdc: 2d 38 00 00  	move	$7, $zero
  1a7fe0: e1 ff e0 10  	beqz	$7, 0x1a7f68 <.text+0xa7f68>
  1a7fe4: 00 00 00 00  	nop
  1a7fe8: 08 00 84 8c  	lw	$4, 0x8($4)
  1a7fec: 02 fc 82 28  	slti	$2, $4, -0x3fe <.text+0xffffffffffeffc02>
  1a7ff0: 1f 00 40 10  	beqz	$2, 0x1a8070 <.text+0xa8070>
  1a7ff4: 00 04 82 28  	slti	$2, $4, 0x400
  1a7ff8: 02 fc 03 24  	addiu	$3, $zero, -0x3fe <.text+0xffffffffffeffc02>
  1a7ffc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a8000: 23 18 64 00  	subu	$3, $3, $4
  1a8004: 14 10 62 00  	dsllv	$2, $2, $3
  1a8008: 16 28 67 00  	dsrlv	$5, $7, $3
  1a800c: ff ff 42 64  	daddiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  1a8010: 39 00 63 28  	slti	$3, $3, 0x39
  1a8014: 24 10 e2 00  	and	$2, $7, $2
  1a8018: 00 00 63 38  	xori	$3, $3, 0x0
  1a801c: 2b 10 02 00  	sltu	$2, $zero, $2
  1a8020: 2d 38 00 00  	move	$7, $zero
  1a8024: 25 28 a2 00  	or	$5, $5, $2
  1a8028: 80 00 02 24  	addiu	$2, $zero, 0x80
  1a802c: 0b 38 a3 00  	movn	$7, $5, $3
  1a8030: ff 00 e4 30  	andi	$4, $7, 0xff
  1a8034: 0a 00 82 50  	beql	$4, $2, 0x1a8060 <.text+0xa8060>
  1a8038: 00 01 e2 30  	andi	$2, $7, 0x100
  1a803c: 7f 00 e7 64  	daddiu	$7, $7, 0x7f
  1a8040: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8044: 3a 11 02 00  	dsrl	$2, $2, 0x4
  1a8048: 01 00 03 24  	addiu	$3, $zero, 0x1
  1a804c: 2b 10 47 00  	sltu	$2, $2, $7
  1a8050: 2d 28 00 00  	move	$5, $zero
  1a8054: 3a 3a 07 00  	dsrl	$7, $7, 0x8
  1a8058: c3 ff 00 10  	b	0x1a7f68 <.text+0xa7f68>
  1a805c: 0b 28 62 00  	movn	$5, $3, $2
  1a8060: f7 ff 40 54  	bnezl	$2, 0x1a8040 <.text+0xa8040>
  1a8064: 80 00 e7 64  	daddiu	$7, $7, 0x80
  1a8068: f5 ff 00 10  	b	0x1a8040 <.text+0xa8040>
  1a806c: 00 00 00 00  	nop
  1a8070: 04 00 40 54  	bnezl	$2, 0x1a8084 <.text+0xa8084>
  1a8074: ff 00 e3 30  	andi	$3, $7, 0xff
  1a8078: ff 07 05 24  	addiu	$5, $zero, 0x7ff
  1a807c: ba ff 00 10  	b	0x1a7f68 <.text+0xa7f68>
  1a8080: 2d 38 00 00  	move	$7, $zero
  1a8084: 80 00 02 24  	addiu	$2, $zero, 0x80
  1a8088: 0b 00 62 10  	beq	$3, $2, 0x1a80b8 <.text+0xa80b8>
  1a808c: ff 03 85 24  	addiu	$5, $4, 0x3ff
  1a8090: 7f 00 e7 64  	daddiu	$7, $7, 0x7f
  1a8094: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8098: fa 10 02 00  	dsrl	$2, $2, 0x3
  1a809c: 2b 10 47 00  	sltu	$2, $2, $7
  1a80a0: b1 ff 40 50  	beqzl	$2, 0x1a7f68 <.text+0xa7f68>
  1a80a4: 3a 3a 07 00  	dsrl	$7, $7, 0x8
  1a80a8: 7a 38 07 00  	dsrl	$7, $7, 0x1
  1a80ac: 01 00 a5 24  	addiu	$5, $5, 0x1
  1a80b0: ad ff 00 10  	b	0x1a7f68 <.text+0xa7f68>
  1a80b4: 3a 3a 07 00  	dsrl	$7, $7, 0x8
  1a80b8: 00 01 e2 30  	andi	$2, $7, 0x100
  1a80bc: f5 ff 40 54  	bnezl	$2, 0x1a8094 <.text+0xa8094>
  1a80c0: 80 00 e7 64  	daddiu	$7, $7, 0x80
  1a80c4: f3 ff 00 10  	b	0x1a8094 <.text+0xa8094>
  1a80d0: 00 00 83 dc  	ld	$3, 0x0($4)
  1a80d4: 3e 15 03 00  	dsrl32	$2, $3, 0x14
  1a80d8: fe 27 03 00  	dsrl32	$4, $3, 0x1f
  1a80dc: ff 07 47 30  	andi	$7, $2, 0x7ff
  1a80e0: 04 00 a4 ac  	sw	$4, 0x4($5)
  1a80e4: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a80e8: 3a 13 02 00  	dsrl	$2, $2, 0xc
  1a80ec: 1c 00 e0 14  	bnez	$7, 0x1a8160 <.text+0xa8160>
  1a80f0: 24 30 62 00  	and	$6, $3, $2
  1a80f4: 18 00 c0 10  	beqz	$6, 0x1a8158 <.text+0xa8158>
  1a80f8: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a80fc: 38 32 06 00  	dsll	$6, $6, 0x8
  1a8100: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8104: 3a 11 02 00  	dsrl	$2, $2, 0x4
  1a8108: 02 fc 03 24  	addiu	$3, $zero, -0x3fe <.text+0xffffffffffeffc02>
  1a810c: 03 00 04 24  	addiu	$4, $zero, 0x3
  1a8110: 2b 10 46 00  	sltu	$2, $2, $6
  1a8114: 08 00 a3 ac  	sw	$3, 0x8($5)
  1a8118: 0c 00 40 14  	bnez	$2, 0x1a814c <.text+0xa814c>
  1a811c: 00 00 a4 ac  	sw	$4, 0x0($5)
  1a8120: ff ff 04 24  	addiu	$4, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8124: 3a 21 04 00  	dsrl	$4, $4, 0x4
  1a8128: 78 30 06 00  	dsll	$6, $6, 0x1
  1a812c: 2b 10 86 00  	sltu	$2, $4, $6
  1a8140: f9 ff 40 10  	beqz	$2, 0x1a8128 <.text+0xa8128>
  1a8144: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  1a8148: 08 00 a3 ac  	sw	$3, 0x8($5)
  1a814c: 08 00 e0 03  	jr	$ra
  1a8150: 10 00 a6 fc  	sd	$6, 0x10($5)
  1a8154: 00 00 00 00  	nop
  1a8158: 08 00 e0 03  	jr	$ra
  1a815c: 00 00 a2 ac  	sw	$2, 0x0($5)
  1a8160: ff 07 02 24  	addiu	$2, $zero, 0x7ff
  1a8164: 09 00 e2 10  	beq	$7, $2, 0x1a818c <.text+0xa818c>
  1a8168: 38 12 06 00  	dsll	$2, $6, 0x8
  1a816c: 00 80 03 34  	ori	$3, $zero, 0x8000
  1a8170: 7c 1b 03 00  	dsll32	$3, $3, 0xd
  1a8174: 01 fc e4 24  	addiu	$4, $7, -0x3ff <.text+0xffffffffffeffc01>
  1a8178: 25 10 43 00  	or	$2, $2, $3
  1a817c: 08 00 a4 ac  	sw	$4, 0x8($5)
  1a8180: 10 00 a2 fc  	sd	$2, 0x10($5)
  1a8184: f4 ff 00 10  	b	0x1a8158 <.text+0xa8158>
  1a8188: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a818c: f2 ff c0 10  	beqz	$6, 0x1a8158 <.text+0xa8158>
  1a8190: 04 00 02 24  	addiu	$2, $zero, 0x4
  1a8194: 00 80 02 34  	ori	$2, $zero, 0x8000
  1a8198: 3c 11 02 00  	dsll32	$2, $2, 0x4
  1a819c: 24 10 c2 00  	and	$2, $6, $2
  1a81a0: ea ff 40 50  	beqzl	$2, 0x1a814c <.text+0xa814c>
  1a81a4: 00 00 a0 ac  	sw	$zero, 0x0($5)
  1a81a8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a81ac: e7 ff 00 10  	b	0x1a814c <.text+0xa814c>
  1a81b0: 00 00 a2 ac  	sw	$2, 0x0($5)
  1a81b4: 00 00 00 00  	nop
  1a81b8: 00 00 83 8c  	lw	$3, 0x0($4)
  1a81bc: 02 00 62 2c  	sltiu	$2, $3, 0x2
  1a81c0: 06 00 40 14  	bnez	$2, 0x1a81dc <.text+0xa81dc>
  1a81c4: 01 00 07 24  	addiu	$7, $zero, 0x1
  1a81c8: 00 00 a6 8c  	lw	$6, 0x0($5)
  1a81cc: 02 00 c2 2c  	sltiu	$2, $6, 0x2
  1a81d0: 05 00 40 50  	beqzl	$2, 0x1a81e8 <.text+0xa81e8>
  1a81d4: 04 00 62 38  	xori	$2, $3, 0x4
  1a81d8: 01 00 07 24  	addiu	$7, $zero, 0x1
  1a81dc: 08 00 e0 03  	jr	$ra
  1a81e0: 2d 10 e0 00  	move	$2, $7
  1a81e4: 00 00 00 00  	nop
  1a81e8: 09 00 40 14  	bnez	$2, 0x1a8210 <.text+0xa8210>
  1a81ec: 04 00 62 38  	xori	$2, $3, 0x4
  1a81f0: 04 00 c2 38  	xori	$2, $6, 0x4
  1a81f4: 06 00 40 54  	bnezl	$2, 0x1a8210 <.text+0xa8210>
  1a81f8: 04 00 62 38  	xori	$2, $3, 0x4
  1a81fc: 04 00 a3 8c  	lw	$3, 0x4($5)
  1a8200: 04 00 82 8c  	lw	$2, 0x4($4)
  1a8204: f5 ff 00 10  	b	0x1a81dc <.text+0xa81dc>
  1a8208: 23 38 62 00  	subu	$7, $3, $2
  1a820c: 00 00 00 00  	nop
  1a8210: 13 00 40 50  	beqzl	$2, 0x1a8260 <.text+0xa8260>
  1a8214: 04 00 83 8c  	lw	$3, 0x4($4)
  1a8218: 04 00 c2 38  	xori	$2, $6, 0x4
  1a821c: 0a 00 40 50  	beqzl	$2, 0x1a8248 <.text+0xa8248>
  1a8220: 04 00 a3 8c  	lw	$3, 0x4($5)
  1a8224: 02 00 62 38  	xori	$2, $3, 0x2
  1a8228: 0b 00 40 14  	bnez	$2, 0x1a8258 <.text+0xa8258>
  1a822c: 02 00 c2 38  	xori	$2, $6, 0x2
  1a8230: ea ff 40 10  	beqz	$2, 0x1a81dc <.text+0xa81dc>
  1a8234: 2d 38 00 00  	move	$7, $zero
  1a8238: 02 00 62 38  	xori	$2, $3, 0x2
  1a823c: 06 00 40 54  	bnezl	$2, 0x1a8258 <.text+0xa8258>
  1a8240: 02 00 c2 38  	xori	$2, $6, 0x2
  1a8244: 04 00 a3 8c  	lw	$3, 0x4($5)
  1a8248: 01 00 07 24  	addiu	$7, $zero, 0x1
  1a824c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8250: e2 ff 00 10  	b	0x1a81dc <.text+0xa81dc>
  1a8254: 0a 38 43 00  	movz	$7, $2, $3
  1a8258: 04 00 40 14  	bnez	$2, 0x1a826c <.text+0xa826c>
  1a825c: 04 00 83 8c  	lw	$3, 0x4($4)
  1a8260: ff ff 07 24  	addiu	$7, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8264: fa ff 00 10  	b	0x1a8250 <.text+0xa8250>
  1a8268: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a826c: 04 00 a2 8c  	lw	$2, 0x4($5)
  1a8270: fc ff 62 14  	bne	$3, $2, 0x1a8264 <.text+0xa8264>
  1a8274: ff ff 07 24  	addiu	$7, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8278: 08 00 87 8c  	lw	$7, 0x8($4)
  1a827c: 08 00 a6 8c  	lw	$6, 0x8($5)
  1a8280: 2a 10 c7 00  	slt	$2, $6, $7
  1a8284: f7 ff 40 54  	bnezl	$2, 0x1a8264 <.text+0xa8264>
  1a8288: ff ff 07 24  	addiu	$7, $zero, -0x1 <.text+0xffffffffffefffff>
  1a828c: 2a 10 e6 00  	slt	$2, $7, $6
  1a8290: ee ff 40 54  	bnezl	$2, 0x1a824c <.text+0xa824c>
  1a8294: 01 00 07 24  	addiu	$7, $zero, 0x1
  1a8298: 10 00 86 dc  	ld	$6, 0x10($4)
  1a829c: 10 00 a4 dc  	ld	$4, 0x10($5)
  1a82a0: 2b 10 86 00  	sltu	$2, $4, $6
  1a82a4: ef ff 40 54  	bnezl	$2, 0x1a8264 <.text+0xa8264>
  1a82a8: ff ff 07 24  	addiu	$7, $zero, -0x1 <.text+0xffffffffffefffff>
  1a82ac: 2b 10 c4 00  	sltu	$2, $6, $4
  1a82b0: e6 ff 40 54  	bnezl	$2, 0x1a824c <.text+0xa824c>
  1a82b4: 01 00 07 24  	addiu	$7, $zero, 0x1
  1a82b8: c8 ff 00 10  	b	0x1a81dc <.text+0xa81dc>
  1a82bc: 2d 38 00 00  	move	$7, $zero
  1a82c0: 00 00 83 8c  	lw	$3, 0x0($4)
  1a82c4: 2d 28 00 00  	move	$5, $zero
  1a82c8: 0c 00 87 8c  	lw	$7, 0xc($4)
  1a82cc: 02 00 62 2c  	sltiu	$2, $3, 0x2
  1a82d0: 19 00 40 10  	beqz	$2, 0x1a8338 <.text+0xa8338>
  1a82d4: 04 00 88 8c  	lw	$8, 0x4($4)
  1a82d8: 10 00 02 3c  	lui	$2, 0x10
  1a82dc: ff 00 05 24  	addiu	$5, $zero, 0xff
  1a82e0: 25 38 e2 00  	or	$7, $7, $2
  1a82e4: 7f 00 03 3c  	lui	$3, 0x7f
  1a82e8: 80 ff 02 3c  	lui	$2, 0xff80
  1a82ec: ff ff 63 34  	ori	$3, $3, 0xffff
  1a82f0: 24 30 c2 00  	and	$6, $6, $2
  1a82f4: 24 18 e3 00  	and	$3, $7, $3
  1a82f8: 7f 80 04 3c  	lui	$4, 0x807f
  1a82fc: 25 30 c3 00  	or	$6, $6, $3
  1a8300: ff 00 a2 30  	andi	$2, $5, 0xff
  1a8304: ff ff 84 34  	ori	$4, $4, 0xffff
  1a8308: c0 15 02 00  	sll	$2, $2, 0x17
  1a830c: 24 30 c4 00  	and	$6, $6, $4
  1a8310: ff 7f 03 3c  	lui	$3, 0x7fff
  1a8314: 25 30 c2 00  	or	$6, $6, $2
  1a8318: ff ff 63 34  	ori	$3, $3, 0xffff
  1a831c: c0 27 08 00  	sll	$4, $8, 0x1f
  1a8320: 24 30 c3 00  	and	$6, $6, $3
  1a8324: 25 10 c4 00  	or	$2, $6, $4
  1a8328: 00 00 82 44  	mtc1	$2, $f0
  1a832c: 08 00 e0 03  	jr	$ra
  1a8338: 04 00 62 38  	xori	$2, $3, 0x4
  1a833c: 28 00 40 50  	beqzl	$2, 0x1a83e0 <.text+0xa83e0>
  1a8340: ff 00 05 24  	addiu	$5, $zero, 0xff
  1a8344: 02 00 62 38  	xori	$2, $3, 0x2
  1a8348: e6 ff 40 50  	beqzl	$2, 0x1a82e4 <.text+0xa82e4>
  1a834c: 2d 38 00 00  	move	$7, $zero
  1a8350: e5 ff e0 10  	beqz	$7, 0x1a82e8 <.text+0xa82e8>
  1a8354: 7f 00 03 3c  	lui	$3, 0x7f
  1a8358: 08 00 84 8c  	lw	$4, 0x8($4)
  1a835c: 82 ff 82 28  	slti	$2, $4, -0x7e <.text+0xffffffffffefff82>
  1a8360: 1c 00 40 10  	beqz	$2, 0x1a83d4 <.text+0xa83d4>
  1a8364: 80 00 82 28  	slti	$2, $4, 0x80
  1a8368: 82 ff 03 24  	addiu	$3, $zero, -0x7e <.text+0xffffffffffefff82>
  1a836c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a8370: 23 18 64 00  	subu	$3, $3, $4
  1a8374: 04 10 62 00  	sllv	$2, $2, $3
  1a8378: 06 28 67 00  	srlv	$5, $7, $3
  1a837c: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  1a8380: 1a 00 63 28  	slti	$3, $3, 0x1a
  1a8384: 24 10 e2 00  	and	$2, $7, $2
  1a8388: 00 00 63 38  	xori	$3, $3, 0x0
  1a838c: 2b 10 02 00  	sltu	$2, $zero, $2
  1a8390: 2d 38 00 00  	move	$7, $zero
  1a8394: 25 28 a2 00  	or	$5, $5, $2
  1a8398: 40 00 02 24  	addiu	$2, $zero, 0x40
  1a839c: 0b 38 a3 00  	movn	$7, $5, $3
  1a83a0: 7f 00 e4 30  	andi	$4, $7, 0x7f
  1a83a4: 07 00 82 50  	beql	$4, $2, 0x1a83c4 <.text+0xa83c4>
  1a83a8: 80 00 e2 30  	andi	$2, $7, 0x80
  1a83ac: 3f 00 e7 24  	addiu	$7, $7, 0x3f
  1a83b0: ff 3f 02 3c  	lui	$2, 0x3fff
  1a83b4: ff ff 42 34  	ori	$2, $2, 0xffff
  1a83b8: 2b 28 47 00  	sltu	$5, $2, $7
  1a83bc: c9 ff 00 10  	b	0x1a82e4 <.text+0xa82e4>
  1a83c0: c2 39 07 00  	srl	$7, $7, 0x7
  1a83c4: fa ff 40 54  	bnezl	$2, 0x1a83b0 <.text+0xa83b0>
  1a83c8: 40 00 e7 24  	addiu	$7, $7, 0x40
  1a83cc: f9 ff 00 10  	b	0x1a83b4 <.text+0xa83b4>
  1a83d0: ff 3f 02 3c  	lui	$2, 0x3fff
  1a83d4: 04 00 40 54  	bnezl	$2, 0x1a83e8 <.text+0xa83e8>
  1a83d8: 7f 00 e3 30  	andi	$3, $7, 0x7f
  1a83dc: ff 00 05 24  	addiu	$5, $zero, 0xff
  1a83e0: c0 ff 00 10  	b	0x1a82e4 <.text+0xa82e4>
  1a83e4: 2d 38 00 00  	move	$7, $zero
  1a83e8: 40 00 02 24  	addiu	$2, $zero, 0x40
  1a83ec: 07 00 62 10  	beq	$3, $2, 0x1a840c <.text+0xa840c>
  1a83f0: 7f 00 85 24  	addiu	$5, $4, 0x7f
  1a83f4: 3f 00 e7 24  	addiu	$7, $7, 0x3f
  1a83f8: ba ff e3 04  	bgezl	$7, 0x1a82e4 <.text+0xa82e4>
  1a83fc: c2 39 07 00  	srl	$7, $7, 0x7
  1a8400: 42 38 07 00  	srl	$7, $7, 0x1
  1a8404: ed ff 00 10  	b	0x1a83bc <.text+0xa83bc>
  1a8408: 01 00 a5 24  	addiu	$5, $5, 0x1
  1a840c: 80 00 e2 30  	andi	$2, $7, 0x80
  1a8410: f9 ff 40 54  	bnezl	$2, 0x1a83f8 <.text+0xa83f8>
  1a8414: 40 00 e7 24  	addiu	$7, $7, 0x40
  1a8418: f7 ff 00 10  	b	0x1a83f8 <.text+0xa83f8>
  1a841c: 00 00 00 00  	nop

## C++ terminate/throw/rethrow/operator new corridor [0x001a9ca8..0x001a9f68)
  1a9ca8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a9cac: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a9cb0: 09 f8 80 00  	jalr	$4
  1a9cb4: 00 00 00 00  	nop
  1a9cb8: 5e 1d 04 0c  	jal	0x107578 <.text+0x7578>
  1a9cbc: 00 00 00 00  	nop
  1a9cc0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a9cc4: 08 00 e0 03  	jr	$ra
  1a9cc8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a9ccc: 00 00 00 00  	nop
  1a9cd0: 3c ac 06 0c  	jal	0x1ab0f0 <.text+0xab0f0>
  1a9cd4: 00 00 00 00  	nop
  1a9cd8: 5e 1d 04 0c  	jal	0x107578 <.text+0x7578>
  1a9cdc: 00 00 00 00  	nop
  1a9ce0: 56 ac 06 0c  	jal	0x1ab158 <.text+0xab158>
  1a9ce4: 00 00 00 00  	nop
  1a9ce8: f6 ff 00 10  	b	0x1a9cc4 <.text+0xa9cc4>
  1a9cec: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a9cf0: 42 00 02 3c  	lui	$2, 0x42
  1a9cf4: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a9cf8: a8 66 44 8c  	lw	$4, 0x66a8($2)
  1a9cfc: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a9d00: 2a a7 06 0c  	jal	0x1a9ca8 <.text+0xa9ca8>
  1a9d04: 00 00 00 00  	nop
  1a9d08: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a9d0c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a9d10: 09 f8 80 00  	jalr	$4
  1a9d14: 00 00 00 00  	nop
  1a9d18: 3c a7 06 0c  	jal	0x1a9cf0 <.text+0xa9cf0>
  1a9d1c: 00 00 00 00  	nop
  1a9d20: 42 00 02 3c  	lui	$2, 0x42
  1a9d24: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a9d28: ac 66 44 8c  	lw	$4, 0x66ac($2)
  1a9d2c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a9d30: 42 a7 06 0c  	jal	0x1a9d08 <.text+0xa9d08>
  1a9d34: 00 00 00 00  	nop
  1a9d38: 42 00 03 3c  	lui	$3, 0x42
  1a9d3c: a8 66 62 8c  	lw	$2, 0x66a8($3)
  1a9d40: 08 00 e0 03  	jr	$ra
  1a9d44: a8 66 64 ac  	sw	$4, 0x66a8($3)
  1a9d48: 42 00 03 3c  	lui	$3, 0x42
  1a9d4c: ac 66 62 8c  	lw	$2, 0x66ac($3)
  1a9d50: 08 00 e0 03  	jr	$ra
  1a9d54: ac 66 64 ac  	sw	$4, 0x66ac($3)
  1a9d58: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a9d5c: 2d 18 80 00  	move	$3, $4
  1a9d60: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a9d64: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a9d68: 20 00 b0 24  	addiu	$16, $5, 0x20
  1a9d6c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a9d70: 2d 20 00 02  	move	$4, $16
  1a9d74: 0e 00 62 14  	bne	$3, $2, 0x1a9db0 <.text+0xa9db0>
  1a9d78: d0 ff a6 24  	addiu	$6, $5, -0x30 <.text+0xffffffffffefffd0>
  1a9d7c: 04 00 c2 8c  	lw	$2, 0x4($6)
  1a9d80: 07 00 40 14  	bnez	$2, 0x1a9da0 <.text+0xa9da0>
  1a9d84: 00 00 00 00  	nop
  1a9d88: 20 ac 06 0c  	jal	0x1ab080 <.text+0xab080>
  1a9d8c: 2d 20 00 02  	move	$4, $16
  1a9d90: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a9d94: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a9d98: 08 00 e0 03  	jr	$ra
  1a9d9c: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a9da0: 09 f8 40 00  	jalr	$2
  1a9da4: 00 00 00 00  	nop
  1a9da8: f7 ff 00 10  	b	0x1a9d88 <.text+0xa9d88>
  1a9dac: 00 00 00 00  	nop
  1a9db0: 2a a7 06 0c  	jal	0x1a9ca8 <.text+0xa9ca8>
  1a9db4: 0c 00 c4 8c  	lw	$4, 0xc($6)
  1a9db8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a9dbc: 42 00 02 3c  	lui	$2, 0x42
  1a9dc0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a9dc4: 42 00 03 3c  	lui	$3, 0x42
  1a9dc8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a9dcc: 2d 80 80 00  	move	$16, $4
  1a9dd0: ac 66 47 8c  	lw	$7, 0x66ac($2)
  1a9dd4: 1b 00 02 3c  	lui	$2, 0x1b
  1a9dd8: a8 66 64 8c  	lw	$4, 0x66a8($3)
  1a9ddc: 58 9d 42 24  	addiu	$2, $2, -0x62a8 <.text+0xffffffffffef9d58>
  1a9de0: b0 ff 03 26  	addiu	$3, $16, -0x50 <.text+0xffffffffffefffb0>
  1a9de4: b0 ff 05 ae  	sw	$5, -0x50($16)
  1a9de8: 38 00 62 ac  	sw	$2, 0x38($3)
  1a9dec: e0 ff 10 26  	addiu	$16, $16, -0x20 <.text+0xffffffffffefffe0>
  1a9df0: 4e 47 02 3c  	lui	$2, 0x474e
  1a9df4: 43 55 42 34  	ori	$2, $2, 0x5543
  1a9df8: 38 14 02 00  	dsll	$2, $2, 0x10
  1a9dfc: 2b 43 42 34  	ori	$2, $2, 0x432b
  1a9e00: 38 14 02 00  	dsll	$2, $2, 0x10
  1a9e04: 00 2b 42 34  	ori	$2, $2, 0x2b00
  1a9e08: 04 00 66 ac  	sw	$6, 0x4($3)
  1a9e0c: 08 00 67 ac  	sw	$7, 0x8($3)
  1a9e10: 0c 00 64 ac  	sw	$4, 0xc($3)
  1a9e14: c6 ac 06 0c  	jal	0x1ab318 <.text+0xab318>
  1a9e18: 30 00 62 fc  	sd	$2, 0x30($3)
  1a9e1c: 2d 20 00 02  	move	$4, $16
  1a9e20: 04 00 43 8c  	lw	$3, 0x4($2)
  1a9e24: 01 00 63 24  	addiu	$3, $3, 0x1
  1a9e28: 6a 95 06 0c  	jal	0x1a55a8 <.text+0xa55a8>
  1a9e2c: 04 00 43 ac  	sw	$3, 0x4($2)
  1a9e30: 3c ac 06 0c  	jal	0x1ab0f0 <.text+0xab0f0>
  1a9e34: 2d 20 00 02  	move	$4, $16
  1a9e38: 3c a7 06 0c  	jal	0x1a9cf0 <.text+0xa9cf0>
  1a9e3c: 00 00 00 00  	nop
  1a9e40: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a9e44: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a9e48: c6 ac 06 0c  	jal	0x1ab318 <.text+0xab318>
  1a9e4c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a9e50: 00 00 43 8c  	lw	$3, 0x0($2)
  1a9e54: 09 00 60 10  	beqz	$3, 0x1a9e7c <.text+0xa9e7c>
  1a9e58: 00 00 00 00  	nop
  1a9e5c: 14 00 62 8c  	lw	$2, 0x14($3)
  1a9e60: 30 00 70 24  	addiu	$16, $3, 0x30
  1a9e64: 2d 20 00 02  	move	$4, $16
  1a9e68: 23 10 02 00  	negu	$2, $2
  1a9e6c: 6a 95 06 0c  	jal	0x1a55a8 <.text+0xa55a8>
  1a9e70: 14 00 62 ac  	sw	$2, 0x14($3)
  1a9e74: 3c ac 06 0c  	jal	0x1ab0f0 <.text+0xab0f0>
  1a9e78: 2d 20 00 02  	move	$4, $16
  1a9e7c: 3c a7 06 0c  	jal	0x1a9cf0 <.text+0xa9cf0>
  1a9e88: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a9e8c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a9e90: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a9e94: 2d 80 80 00  	move	$16, $4
  1a9e98: 0a 80 44 00  	movz	$16, $2, $4
  1a9e9c: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a9ea0: 2d 20 00 02  	move	$4, $16
  1a9ea4: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1a9ea8: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a9eac: 06 00 40 10  	beqz	$2, 0x1a9ec8 <.text+0xa9ec8>
  1a9eb0: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a9eb4: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a9eb8: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a9ebc: 08 00 e0 03  	jr	$ra
  1a9ec0: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a9ec4: 00 00 00 00  	nop
  1a9ec8: 42 00 02 3c  	lui	$2, 0x42
  1a9ecc: 1c 6b 42 8c  	lw	$2, 0x6b1c($2)
  1a9ed0: 09 00 40 50  	beqzl	$2, 0x1a9ef8 <.text+0xa9ef8>
  1a9ed4: 42 00 10 3c  	lui	$16, 0x42
  1a9ed8: 09 f8 40 00  	jalr	$2
  1a9edc: 00 00 00 00  	nop
  1a9ee0: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1a9ee4: 2d 20 00 02  	move	$4, $16
  1a9ee8: f8 ff 40 50  	beqzl	$2, 0x1a9ecc <.text+0xa9ecc>
  1a9eec: 42 00 02 3c  	lui	$2, 0x42
  1a9ef0: f0 ff 00 10  	b	0x1a9eb4 <.text+0xa9eb4>
  1a9ef4: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a9ef8: 04 00 04 24  	addiu	$4, $zero, 0x4
  1a9efc: c8 6d 10 26  	addiu	$16, $16, 0x6dc8
  1a9f00: ee ab 06 0c  	jal	0x1aafb8 <.text+0xaafb8>
  1a9f04: 00 00 b0 af  	sw	$16, 0x0($sp)
  1a9f08: 00 00 50 ac  	sw	$16, 0x0($2)
  1a9f0c: 42 00 05 3c  	lui	$5, 0x42
  1a9f10: 1b 00 06 3c  	lui	$6, 0x1b
  1a9f14: 2d 20 40 00  	move	$4, $2
  1a9f18: d8 6d a5 24  	addiu	$5, $5, 0x6dd8
  1a9f1c: 6e a7 06 0c  	jal	0x1a9db8 <.text+0xa9db8>
  1a9f20: 60 b3 c6 24  	addiu	$6, $6, -0x4ca0 <.text+0xffffffffffefb360>
  1a9f24: 2d 80 80 00  	move	$16, $4
  1a9f28: 2d 88 a0 00  	move	$17, $5
  1a9f2c: d8 ac 06 0c  	jal	0x1ab360 <.text+0xab360>
  1a9f30: 2d 20 a0 03  	move	$4, $sp
  1a9f34: 00 00 00 00  	nop
  1a9f38: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a9f3c: 07 00 22 12  	beq	$17, $2, 0x1a9f5c <.text+0xa9f5c>
  1a9f40: 00 00 00 00  	nop
  1a9f44: b0 96 06 0c  	jal	0x1a5ac0 <.text+0xa5ac0>
  1a9f48: 2d 20 00 02  	move	$4, $16
  1a9f4c: 00 00 00 00  	nop
  1a9f50: 2d 80 80 00  	move	$16, $4
  1a9f54: f8 ff 00 10  	b	0x1a9f38 <.text+0xa9f38>
  1a9f58: 2d 88 a0 00  	move	$17, $5
  1a9f5c: e2 a6 06 0c  	jal	0x1a9b88 <.text+0xa9b88>
  1a9f60: 2d 20 00 02  	move	$4, $16
  1a9f64: 00 00 00 00  	nop

## single-inheritance public-source walker [0x001aa3a8..0x001aa400)
  1aa3a8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1aa3ac: 06 00 09 24  	addiu	$9, $zero, 0x6
  1aa3b0: 0d 00 06 11  	beq	$8, $6, 0x1aa3e8 <.text+0xaa3e8>
  1aa3b4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1aa3b8: 08 00 82 8c  	lw	$2, 0x8($4)
  1aa3bc: 00 00 43 8c  	lw	$3, 0x0($2)
  1aa3c0: 2d 20 40 00  	move	$4, $2
  1aa3c4: 20 00 62 8c  	lw	$2, 0x20($3)
  1aa3c8: 09 f8 40 00  	jalr	$2
  1aa3cc: 00 00 00 00  	nop
  1aa3d0: 2d 48 40 00  	move	$9, $2
  1aa3d4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1aa3d8: 2d 10 20 01  	move	$2, $9
  1aa3dc: 08 00 e0 03  	jr	$ra
  1aa3e0: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1aa3e4: 00 00 00 00  	nop
  1aa3e8: 04 00 83 8c  	lw	$3, 0x4($4)
  1aa3ec: 04 00 e2 8c  	lw	$2, 0x4($7)
  1aa3f0: f9 ff 62 10  	beq	$3, $2, 0x1aa3d8 <.text+0xaa3d8>
  1aa3f4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1aa3f8: f0 ff 00 10  	b	0x1aa3bc <.text+0xaa3bc>
  1aa3fc: 08 00 82 8c  	lw	$2, 0x8($4)

## base/SI dyncast walkers [0x001aa548..0x001aa638)
  1aa548: 0b 00 0a 11  	beq	$8, $10, 0x1aa578 <.text+0xaa578>
  1aa54c: 04 00 84 8c  	lw	$4, 0x4($4)
  1aa550: 04 00 e2 8c  	lw	$2, 0x4($7)
  1aa554: 05 00 82 14  	bne	$4, $2, 0x1aa56c <.text+0xaa56c>
  1aa558: 00 00 00 00  	nop
  1aa55c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aa560: 00 00 68 ad  	sw	$8, 0x0($11)
  1aa564: 0c 00 62 ad  	sw	$2, 0xc($11)
  1aa568: 04 00 66 ad  	sw	$6, 0x4($11)
  1aa56c: 08 00 e0 03  	jr	$ra
  1aa570: 2d 10 00 00  	move	$2, $zero
  1aa574: 00 00 00 00  	nop
  1aa578: 04 00 22 8d  	lw	$2, 0x4($9)
  1aa57c: f5 ff 82 54  	bnel	$4, $2, 0x1aa554 <.text+0xaa554>
  1aa580: 04 00 e2 8c  	lw	$2, 0x4($7)
  1aa584: f9 ff 00 10  	b	0x1aa56c <.text+0xaa56c>
  1aa588: 08 00 66 ad  	sw	$6, 0x8($11)
  1aa58c: 00 00 00 00  	nop
  1aa590: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1aa594: 04 00 e2 8c  	lw	$2, 0x4($7)
  1aa598: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1aa59c: 04 00 83 8c  	lw	$3, 0x4($4)
  1aa5a0: 15 00 62 14  	bne	$3, $2, 0x1aa5f8 <.text+0xaa5f8>
  1aa5a4: 00 00 00 00  	nop
  1aa5a8: 04 00 66 ad  	sw	$6, 0x4($11)
  1aa5ac: 0c 00 a0 04  	bltz	$5, 0x1aa5e0 <.text+0xaa5e0>
  1aa5b0: 00 00 68 ad  	sw	$8, 0x0($11)
  1aa5b4: 21 10 05 01  	addu	$2, $8, $5
  1aa5b8: 06 00 03 24  	addiu	$3, $zero, 0x6
  1aa5bc: 26 10 4a 00  	xor	$2, $2, $10
  1aa5c0: 01 00 04 24  	addiu	$4, $zero, 0x1
  1aa5c4: 0b 18 82 00  	movn	$3, $4, $2
  1aa5c8: 0c 00 63 ad  	sw	$3, 0xc($11)
  1aa5cc: 2d 10 00 00  	move	$2, $zero
  1aa5d0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1aa5d4: 08 00 e0 03  	jr	$ra
  1aa5d8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1aa5dc: 00 00 00 00  	nop
  1aa5e0: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  1aa5e4: fa ff a2 14  	bne	$5, $2, 0x1aa5d0 <.text+0xaa5d0>
  1aa5e8: 2d 10 00 00  	move	$2, $zero
  1aa5ec: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aa5f0: f6 ff 00 10  	b	0x1aa5cc <.text+0xaa5cc>
  1aa5f4: 0c 00 62 ad  	sw	$2, 0xc($11)
  1aa5f8: 09 00 0a 51  	beql	$8, $10, 0x1aa620 <.text+0xaa620>
  1aa5fc: 04 00 22 8d  	lw	$2, 0x4($9)
  1aa600: 08 00 84 8c  	lw	$4, 0x8($4)
  1aa604: 00 00 83 8c  	lw	$3, 0x0($4)
  1aa608: 1c 00 62 8c  	lw	$2, 0x1c($3)
  1aa60c: 09 f8 40 00  	jalr	$2
  1aa610: 00 00 00 00  	nop
  1aa614: ef ff 00 10  	b	0x1aa5d4 <.text+0xaa5d4>
  1aa618: 00 00 bf df  	ld	$ra, 0x0($sp)
  1aa61c: 00 00 00 00  	nop
  1aa620: f8 ff 62 54  	bnel	$3, $2, 0x1aa604 <.text+0xaa604>
  1aa624: 08 00 84 8c  	lw	$4, 0x8($4)
  1aa628: 2d 10 00 00  	move	$2, $zero
  1aa62c: e8 ff 00 10  	b	0x1aa5d0 <.text+0xaa5d0>
  1aa630: 08 00 66 ad  	sw	$6, 0x8($11)
  1aa634: 00 00 00 00  	nop


# ---------------------------------------------------------------------------
# Progress 13 second pass — 70% milestone helpers
# Full function/control-flow excerpts for the newly promoted targets below.
# ---------------------------------------------------------------------------

## path construction wrapper [0x00101924..0x001019a0)
  101924: 60 ef bd 27  	addiu	$sp, $sp, -0x10a0 <.text+0xffffffffffefef60>
  101928: 80 10 b3 ff  	sd	$19, 0x1080($sp)
  10192c: 30 08 a8 27  	addiu	$8, $sp, 0x830
  101930: 70 10 b2 ff  	sd	$18, 0x1070($sp)
  101934: 2d 98 80 00  	move	$19, $4
  101938: 20 04 b2 27  	addiu	$18, $sp, 0x420
  10193c: 60 10 b1 ff  	sd	$17, 0x1060($sp)
  101940: 36 00 04 3c  	lui	$4, 0x36
  101944: 10 00 b1 27  	addiu	$17, $sp, 0x10
  101948: 50 10 b0 ff  	sd	$16, 0x1050($sp)
  10194c: 2d 28 a0 03  	move	$5, $sp
  101950: 40 0c b0 27  	addiu	$16, $sp, 0xc40
  101954: 2d 30 20 02  	move	$6, $17
  101958: 2d 38 40 02  	move	$7, $18
  10195c: 90 10 bf ff  	sd	$ra, 0x1090($sp)
  101960: ba 16 04 0c  	jal	0x105ae8 <.text+0x5ae8>
  101964: 28 b3 84 24  	addiu	$4, $4, -0x4cd8 <.text+0xffffffffffefb328>
  101968: 2d 40 60 02  	move	$8, $19
  10196c: 2d 30 20 02  	move	$6, $17
  101970: 2d 38 40 02  	move	$7, $18
  101974: 2d 20 00 02  	move	$4, $16
  101978: 73 16 04 0c  	jal	0x1059cc <.text+0x59cc>
  10197c: 2d 28 a0 03  	move	$5, $sp
  101980: 60 10 b1 df  	ld	$17, 0x1060($sp)
  101984: 2d 10 00 02  	move	$2, $16
  101988: 90 10 bf df  	ld	$ra, 0x1090($sp)
  10198c: 80 10 b3 df  	ld	$19, 0x1080($sp)
  101990: 70 10 b2 df  	ld	$18, 0x1070($sp)
  101994: 50 10 b0 df  	ld	$16, 0x1050($sp)
  101998: 08 00 e0 03  	jr	$ra
  10199c: a0 10 bd 27  	addiu	$sp, $sp, 0x10a0

## packed weighted color blend [0x0010a18c..0x0010a264)
  10a18c: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  10a190: 2d 10 a0 00  	move	$2, $5
  10a194: 40 00 b4 ff  	sd	$20, 0x40($sp)
  10a198: 2d a0 a0 00  	move	$20, $5
  10a19c: 50 00 bf ff  	sd	$ra, 0x50($sp)
  10a1a0: 30 00 b3 ff  	sd	$19, 0x30($sp)
  10a1a4: 20 00 b2 ff  	sd	$18, 0x20($sp)
  10a1a8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  10a1ac: 25 00 85 10  	beq	$4, $5, 0x10a244 <.text+0xa244>
  10a1b0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  10a1b4: 33 00 02 3c  	lui	$2, 0x33
  10a1b8: c2 9a 06 00  	srl	$19, $6, 0xb
  10a1bc: 9c 52 52 8c  	lw	$18, 0x529c($2)
  10a1c0: 1f 00 73 32  	andi	$19, $19, 0x1f
  10a1c4: 33 00 02 3c  	lui	$2, 0x33
  10a1c8: 98 52 51 8c  	lw	$17, 0x5298($2)
  10a1cc: 24 30 b2 00  	and	$6, $5, $18
  10a1d0: 24 10 92 00  	and	$2, $4, $18
  10a1d4: 00 34 06 00  	sll	$6, $6, 0x10
  10a1d8: 24 18 91 00  	and	$3, $4, $17
  10a1dc: 00 14 02 00  	sll	$2, $2, 0x10
  10a1e0: 25 20 62 00  	or	$4, $3, $2
  10a1e4: 3c 90 12 00  	dsll32	$18, $18, 0x0
  10a1e8: 24 10 91 02  	and	$2, $20, $17
  10a1ec: 3c 28 04 00  	dsll32	$5, $4, 0x0
  10a1f0: 20 00 04 24  	addiu	$4, $zero, 0x20
  10a1f4: 25 a0 46 00  	or	$20, $2, $6
  10a1f8: 2f 20 93 00  	dsubu	$4, $4, $19
  10a1fc: c8 86 06 0c  	jal	0x1a1b20 <.text+0xa1b20>
  10a200: 3e 28 05 00  	dsrl32	$5, $5, 0x0
  10a204: 3c 88 11 00  	dsll32	$17, $17, 0x0
  10a208: 3c 28 14 00  	dsll32	$5, $20, 0x0
  10a20c: 2d 20 60 02  	move	$4, $19
  10a210: 3e 28 05 00  	dsrl32	$5, $5, 0x0
  10a214: c8 86 06 0c  	jal	0x1a1b20 <.text+0xa1b20>
  10a218: 2d 80 40 00  	move	$16, $2
  10a21c: 3e 88 11 00  	dsrl32	$17, $17, 0x0
  10a220: 2d 80 02 02  	daddu	$16, $16, $2
  10a224: 3e 90 12 00  	dsrl32	$18, $18, 0x0
  10a228: 7a 15 10 00  	dsrl	$2, $16, 0x15
  10a22c: 7a 81 10 00  	dsrl	$16, $16, 0x5
  10a230: 24 10 52 00  	and	$2, $2, $18
  10a234: 24 80 11 02  	and	$16, $16, $17
  10a238: 25 80 02 02  	or	$16, $16, $2
  10a23c: 3c 10 10 00  	dsll32	$2, $16, 0x0
  10a240: 3f 10 02 00  	dsra32	$2, $2, 0x0
  10a244: 50 00 bf df  	ld	$ra, 0x50($sp)
  10a248: 40 00 b4 df  	ld	$20, 0x40($sp)
  10a24c: 30 00 b3 df  	ld	$19, 0x30($sp)
  10a250: 20 00 b2 df  	ld	$18, 0x20($sp)
  10a254: 10 00 b1 df  	ld	$17, 0x10($sp)
  10a258: 00 00 b0 df  	ld	$16, 0x0($sp)
  10a25c: 08 00 e0 03  	jr	$ra
  10a260: 60 00 bd 27  	addiu	$sp, $sp, 0x60

## record/RAM sentinel initialization [0x0012b9a4..0x0012ba5c)
  12b9a4: 2d 38 00 00  	move	$7, $zero
  12b9a8: 16 00 03 24  	addiu	$3, $zero, 0x16
  12b9ac: 36 00 02 3c  	lui	$2, 0x36
  12b9b0: 18 28 e3 00  	<unknown>
  12b9b4: 60 d3 42 24  	addiu	$2, $2, -0x2ca0 <.text+0xffffffffffefd360>
  12b9b8: 01 00 e7 24  	addiu	$7, $7, 0x1
  12b9bc: ff ff 04 24  	addiu	$4, $zero, -0x1 <.text+0xffffffffffefffff>
  12b9c0: 08 00 e6 28  	slti	$6, $7, 0x8
  12b9c4: 21 18 a2 00  	addu	$3, $5, $2
  12b9c8: 01 00 02 24  	addiu	$2, $zero, 0x1
  12b9cc: ff ff 05 24  	addiu	$5, $zero, -0x1 <.text+0xffffffffffefffff>
  12b9d0: 0c 00 64 a4  	sh	$4, 0xc($3)
  12b9d4: 01 00 62 a0  	sb	$2, 0x1($3)
  12b9d8: 04 00 65 a0  	sb	$5, 0x4($3)
  12b9dc: 00 00 60 a0  	sb	$zero, 0x0($3)
  12b9e0: 0e 00 60 a0  	sb	$zero, 0xe($3)
  12b9e4: 02 00 60 a0  	sb	$zero, 0x2($3)
  12b9e8: 03 00 65 a0  	sb	$5, 0x3($3)
  12b9ec: 06 00 64 a4  	sh	$4, 0x6($3)
  12b9f0: 08 00 64 a4  	sh	$4, 0x8($3)
  12b9f4: ec ff c0 14  	bnez	$6, 0x12b9a8 <.text+0x2b9a8>
  12b9f8: 0a 00 64 a0  	sb	$4, 0xa($3)
  12b9fc: 00 43 05 24  	addiu	$5, $zero, 0x4300
  12ba00: 0c 43 06 24  	addiu	$6, $zero, 0x430c
  12ba04: 2a 10 a6 00  	slt	$2, $5, $6
  12ba08: 09 00 40 10  	beqz	$2, 0x12ba30 <.text+0x2ba30>
  12ba0c: 2d 38 a0 00  	move	$7, $5
  12ba10: 35 00 02 3c  	lui	$2, 0x35
  12ba14: c4 e2 43 8c  	lw	$3, -0x1d3c($2)
  12ba18: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  12ba1c: 21 18 67 00  	addu	$3, $3, $7
  12ba20: 01 00 e7 24  	addiu	$7, $7, 0x1
  12ba24: 2a 20 e6 00  	slt	$4, $7, $6
  12ba28: f9 ff 80 14  	bnez	$4, 0x12ba10 <.text+0x2ba10>
  12ba2c: 00 00 62 a0  	sb	$2, 0x0($3)
  12ba30: 35 00 02 3c  	lui	$2, 0x35
  12ba34: 10 00 c6 24  	addiu	$6, $6, 0x10
  12ba38: c4 e2 43 8c  	lw	$3, -0x1d3c($2)
  12ba3c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  12ba40: 21 18 65 00  	addu	$3, $3, $5
  12ba44: 10 00 a5 24  	addiu	$5, $5, 0x10
  12ba48: 80 43 a4 28  	slti	$4, $5, 0x4380
  12ba4c: ed ff 80 14  	bnez	$4, 0x12ba04 <.text+0x2ba04>
  12ba50: 0f 00 62 a0  	sb	$2, 0xf($3)
  12ba54: 08 00 e0 03  	jr	$ra
  12ba58: 00 00 00 00  	nop

## bit-plane transpose [0x0012e374..0x0012e568)
  12e374: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  12e378: 34 00 02 3c  	lui	$2, 0x34
  12e37c: 3c 56 58 24  	addiu	$24, $2, 0x563c
  12e380: 20 00 b2 ff  	sd	$18, 0x20($sp)
  12e384: 10 00 b1 ff  	sd	$17, 0x10($sp)
  12e388: 10 02 19 27  	addiu	$25, $24, 0x210
  12e38c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  12e390: 40 00 b4 ff  	sd	$20, 0x40($sp)
  12e394: 07 00 14 24  	addiu	$20, $zero, 0x7
  12e398: 30 00 b3 ff  	sd	$19, 0x30($sp)
  12e39c: 00 02 13 27  	addiu	$19, $24, 0x200
  12e3a0: 00 00 02 93  	lbu	$2, 0x0($24)
  12e3a4: 80 ff 12 24  	addiu	$18, $zero, -0x80 <.text+0xffffffffffefff80>
  12e3a8: 01 00 18 27  	addiu	$24, $24, 0x1
  12e3ac: ff ff 94 26  	addiu	$20, $20, -0x1 <.text+0xffffffffffefffff>
  12e3b0: 00 00 07 93  	lbu	$7, 0x0($24)
  12e3b4: 10 00 46 30  	andi	$6, $2, 0x10
  12e3b8: 01 00 18 27  	addiu	$24, $24, 0x1
  12e3bc: 01 00 43 30  	andi	$3, $2, 0x1
  12e3c0: 00 00 0f 93  	lbu	$15, 0x0($24)
  12e3c4: 80 19 03 00  	sll	$3, $3, 0x6
  12e3c8: 10 00 ea 30  	andi	$10, $7, 0x10
  12e3cc: 20 00 44 30  	andi	$4, $2, 0x20
  12e3d0: 02 00 49 30  	andi	$9, $2, 0x2
  12e3d4: 40 00 45 30  	andi	$5, $2, 0x40
  12e3d8: 04 00 48 30  	andi	$8, $2, 0x4
  12e3dc: c0 30 06 00  	sll	$6, $6, 0x3
  12e3e0: 25 30 c3 00  	or	$6, $6, $3
  12e3e4: 40 50 0a 00  	sll	$10, $10, 0x1
  12e3e8: 40 49 09 00  	sll	$9, $9, 0x5
  12e3ec: 00 41 08 00  	sll	$8, $8, 0x4
  12e3f0: 08 00 51 30  	andi	$17, $2, 0x8
  12e3f4: 01 00 18 27  	addiu	$24, $24, 0x1
  12e3f8: 01 00 e3 30  	andi	$3, $7, 0x1
  12e3fc: 40 00 eb 30  	andi	$11, $7, 0x40
  12e400: 80 20 04 00  	sll	$4, $4, 0x2
  12e404: 40 28 05 00  	sll	$5, $5, 0x1
  12e408: 24 60 f2 00  	and	$12, $7, $18
  12e40c: 25 30 ca 00  	or	$6, $6, $10
  12e410: 25 20 89 00  	or	$4, $4, $9
  12e414: 25 28 a8 00  	or	$5, $5, $8
  12e418: 00 00 09 93  	lbu	$9, 0x0($24)
  12e41c: 00 19 03 00  	sll	$3, $3, 0x4
  12e420: 20 00 e8 30  	andi	$8, $7, 0x20
  12e424: 42 58 0b 00  	srl	$11, $11, 0x1
  12e428: 10 00 f0 31  	andi	$16, $15, 0x10
  12e42c: 02 00 ed 30  	andi	$13, $7, 0x2
  12e430: 04 00 ee 30  	andi	$14, $7, 0x4
  12e434: 24 10 52 00  	and	$2, $2, $18
  12e438: c0 88 11 00  	sll	$17, $17, 0x3
  12e43c: 25 30 c3 00  	or	$6, $6, $3
  12e440: 25 20 88 00  	or	$4, $4, $8
  12e444: 25 28 ab 00  	or	$5, $5, $11
  12e448: 42 80 10 00  	srl	$16, $16, 0x1
  12e44c: c0 68 0d 00  	sll	$13, $13, 0x3
  12e450: 80 70 0e 00  	sll	$14, $14, 0x2
  12e454: 82 60 0c 00  	srl	$12, $12, 0x2
  12e458: 01 00 e3 31  	andi	$3, $15, 0x1
  12e45c: 20 00 e8 31  	andi	$8, $15, 0x20
  12e460: 40 00 ea 31  	andi	$10, $15, 0x40
  12e464: 25 10 51 00  	or	$2, $2, $17
  12e468: 08 00 e7 30  	andi	$7, $7, 0x8
  12e46c: 25 30 d0 00  	or	$6, $6, $16
  12e470: 25 20 8d 00  	or	$4, $4, $13
  12e474: 25 28 ae 00  	or	$5, $5, $14
  12e478: 24 68 f2 01  	and	$13, $15, $18
  12e47c: 25 10 4c 00  	or	$2, $2, $12
  12e480: 80 18 03 00  	sll	$3, $3, 0x2
  12e484: 82 40 08 00  	srl	$8, $8, 0x2
  12e488: c2 50 0a 00  	srl	$10, $10, 0x3
  12e48c: 40 38 07 00  	sll	$7, $7, 0x1
  12e490: 10 00 2b 31  	andi	$11, $9, 0x10
  12e494: 02 00 ec 31  	andi	$12, $15, 0x2
  12e498: 25 30 c3 00  	or	$6, $6, $3
  12e49c: 25 20 88 00  	or	$4, $4, $8
  12e4a0: 25 28 aa 00  	or	$5, $5, $10
  12e4a4: 25 10 47 00  	or	$2, $2, $7
  12e4a8: 08 00 f0 31  	andi	$16, $15, 0x8
  12e4ac: 20 00 2e 31  	andi	$14, $9, 0x20
  12e4b0: 40 00 2a 31  	andi	$10, $9, 0x40
  12e4b4: c2 58 0b 00  	srl	$11, $11, 0x3
  12e4b8: 40 60 0c 00  	sll	$12, $12, 0x1
  12e4bc: 02 69 0d 00  	srl	$13, $13, 0x4
  12e4c0: 04 00 ef 31  	andi	$15, $15, 0x4
  12e4c4: 24 90 32 01  	and	$18, $9, $18
  12e4c8: 01 00 28 31  	andi	$8, $9, 0x1
  12e4cc: 02 00 23 31  	andi	$3, $9, 0x2
  12e4d0: 04 00 27 31  	andi	$7, $9, 0x4
  12e4d4: 25 30 cb 00  	or	$6, $6, $11
  12e4d8: 25 20 8c 00  	or	$4, $4, $12
  12e4dc: 25 28 af 00  	or	$5, $5, $15
  12e4e0: 25 10 4d 00  	or	$2, $2, $13
  12e4e4: 02 71 0e 00  	srl	$14, $14, 0x4
  12e4e8: 42 51 0a 00  	srl	$10, $10, 0x5
  12e4ec: 42 80 10 00  	srl	$16, $16, 0x1
  12e4f0: 25 30 c8 00  	or	$6, $6, $8
  12e4f4: 25 20 8e 00  	or	$4, $4, $14
  12e4f8: 25 28 aa 00  	or	$5, $5, $10
  12e4fc: 25 10 50 00  	or	$2, $2, $16
  12e500: 42 18 03 00  	srl	$3, $3, 0x1
  12e504: 82 38 07 00  	srl	$7, $7, 0x2
  12e508: 82 91 12 00  	srl	$18, $18, 0x6
  12e50c: 08 00 29 31  	andi	$9, $9, 0x8
  12e510: 00 00 66 a2  	sb	$6, 0x0($19)
  12e514: 25 20 83 00  	or	$4, $4, $3
  12e518: 01 00 73 26  	addiu	$19, $19, 0x1
  12e51c: 25 28 a7 00  	or	$5, $5, $7
  12e520: 25 10 52 00  	or	$2, $2, $18
  12e524: c2 48 09 00  	srl	$9, $9, 0x3
  12e528: 00 00 64 a2  	sb	$4, 0x0($19)
  12e52c: 25 10 49 00  	or	$2, $2, $9
  12e530: 00 00 25 a3  	sb	$5, 0x0($25)
  12e534: 01 00 18 27  	addiu	$24, $24, 0x1
  12e538: 01 00 39 27  	addiu	$25, $25, 0x1
  12e53c: 01 00 73 26  	addiu	$19, $19, 0x1
  12e540: 00 00 22 a3  	sb	$2, 0x0($25)
  12e544: 96 ff 81 06  	bgez	$20, 0x12e3a0 <.text+0x2e3a0>
  12e548: 01 00 39 27  	addiu	$25, $25, 0x1
  12e54c: 40 00 b4 df  	ld	$20, 0x40($sp)
  12e550: 30 00 b3 df  	ld	$19, 0x30($sp)
  12e554: 20 00 b2 df  	ld	$18, 0x20($sp)
  12e558: 10 00 b1 df  	ld	$17, 0x10($sp)
  12e55c: 00 00 b0 df  	ld	$16, 0x0($sp)
  12e560: 08 00 e0 03  	jr	$ra
  12e564: 50 00 bd 27  	addiu	$sp, $sp, 0x50

## frontend table initialization [0x00153674..0x00153780)
  153674: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  153678: 35 00 02 3c  	lui	$2, 0x35
  15367c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  153680: cd e2 42 90  	lbu	$2, -0x1d33($2)
  153684: 20 00 40 10  	beqz	$2, 0x153708 <.text+0x53708>
  153688: 2d 58 80 00  	move	$11, $4
  15368c: 2d 50 00 00  	move	$10, $zero
  153690: 2d 60 00 00  	move	$12, $zero
  153694: 00 19 0a 00  	sll	$3, $10, 0x4
  153698: 20 af 02 34  	ori	$2, $zero, 0xaf20
  15369c: 21 20 6b 00  	addu	$4, $3, $11
  1536a0: 20 a7 07 34  	ori	$7, $zero, 0xa720
  1536a4: 21 18 6c 00  	addu	$3, $3, $12
  1536a8: 21 20 8c 00  	addu	$4, $4, $12
  1536ac: 80 18 03 00  	sll	$3, $3, 0x2
  1536b0: 20 9f 06 34  	ori	$6, $zero, 0x9f20
  1536b4: 01 00 8c 25  	addiu	$12, $12, 0x1
  1536b8: 20 97 05 34  	ori	$5, $zero, 0x9720
  1536bc: 21 38 87 00  	addu	$7, $4, $7
  1536c0: 21 18 6b 00  	addu	$3, $3, $11
  1536c4: 21 30 86 00  	addu	$6, $4, $6
  1536c8: 21 28 85 00  	addu	$5, $4, $5
  1536cc: 03 00 08 24  	addiu	$8, $zero, 0x3
  1536d0: 08 00 89 29  	slti	$9, $12, 0x8
  1536d4: 21 20 82 00  	addu	$4, $4, $2
  1536d8: 08 00 e0 a0  	sb	$zero, 0x8($7)
  1536dc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1536e0: 28 3c 68 ac  	sw	$8, 0x3c28($3)
  1536e4: 08 00 a2 a0  	sb	$2, 0x8($5)
  1536e8: 08 00 80 a0  	sb	$zero, 0x8($4)
  1536ec: 08 00 c2 a0  	sb	$2, 0x8($6)
  1536f0: e8 ff 20 15  	bnez	$9, 0x153694 <.text+0x53694>
  1536f4: 28 1c 68 ac  	sw	$8, 0x1c28($3)
  1536f8: 01 00 4a 25  	addiu	$10, $10, 0x1
  1536fc: 0f 00 42 29  	slti	$2, $10, 0xf
  153700: e4 ff 40 14  	bnez	$2, 0x153694 <.text+0x53694>
  153704: 2d 60 00 00  	move	$12, $zero
  153708: 00 00 6c 8d  	lw	$12, 0x0($11)
  15370c: 2d 50 00 00  	move	$10, $zero
  153710: 2d 48 60 01  	move	$9, $11
  153714: 21 28 4b 01  	addu	$5, $10, $11
  153718: 00 a8 02 34  	ori	$2, $zero, 0xa800
  15371c: 01 00 4a 25  	addiu	$10, $10, 0x1
  153720: 10 a8 07 34  	ori	$7, $zero, 0xa810
  153724: 01 00 06 3c  	lui	$6, 0x1
  153728: 00 98 04 34  	ori	$4, $zero, 0x9800
  15372c: 10 98 03 34  	ori	$3, $zero, 0x9810
  153730: 21 38 a7 00  	addu	$7, $5, $7
  153734: 21 20 a4 00  	addu	$4, $5, $4
  153738: 21 18 a3 00  	addu	$3, $5, $3
  15373c: 21 30 86 01  	addu	$6, $12, $6
  153740: 10 00 48 29  	slti	$8, $10, 0x10
  153744: 21 28 a2 00  	addu	$5, $5, $2
  153748: e8 1f 26 ad  	sw	$6, 0x1fe8($9)
  15374c: 01 00 02 24  	addiu	$2, $zero, 0x1
  153750: 08 00 a0 a0  	sb	$zero, 0x8($5)
  153754: 08 00 82 a0  	sb	$2, 0x8($4)
  153758: a8 1f 2c ad  	sw	$12, 0x1fa8($9)
  15375c: 04 00 29 25  	addiu	$9, $9, 0x4
  153760: 08 00 62 a0  	sb	$2, 0x8($3)
  153764: eb ff 00 15  	bnez	$8, 0x153714 <.text+0x53714>
  153768: 08 00 e0 a0  	sb	$zero, 0x8($7)
  15376c: 82 4d 05 0c  	jal	0x153608 <.text+0x53608>
  153770: 2d 20 60 01  	move	$4, $11
  153774: 00 00 bf df  	ld	$ra, 0x0($sp)
  153778: 08 00 e0 03  	jr	$ra
  15377c: 10 00 bd 27  	addiu	$sp, $sp, 0x10

## frontend bank initialization [0x00153780..0x001538b0)
  153780: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  153784: 2d c8 00 00  	move	$25, $zero
  153788: 00 00 b0 ff  	sd	$16, 0x0($sp)
  15378c: 2d 48 80 00  	move	$9, $4
  153790: 2d 80 80 00  	move	$16, $4
  153794: 00 00 8a 8c  	lw	$10, 0x0($4)
  153798: 21 28 30 03  	addu	$5, $25, $16
  15379c: 00 a8 02 34  	ori	$2, $zero, 0xa800
  1537a0: 01 00 39 27  	addiu	$25, $25, 0x1
  1537a4: 10 a8 07 34  	ori	$7, $zero, 0xa810
  1537a8: 01 00 06 3c  	lui	$6, 0x1
  1537ac: 00 98 04 34  	ori	$4, $zero, 0x9800
  1537b0: 10 98 03 34  	ori	$3, $zero, 0x9810
  1537b4: 21 38 a7 00  	addu	$7, $5, $7
  1537b8: 21 20 a4 00  	addu	$4, $5, $4
  1537bc: 21 18 a3 00  	addu	$3, $5, $3
  1537c0: 21 30 46 01  	addu	$6, $10, $6
  1537c4: 10 00 28 2b  	slti	$8, $25, 0x10
  1537c8: 21 28 a2 00  	addu	$5, $5, $2
  1537cc: e8 1f 26 ad  	sw	$6, 0x1fe8($9)
  1537d0: 01 00 02 24  	addiu	$2, $zero, 0x1
  1537d4: 08 00 a0 a0  	sb	$zero, 0x8($5)
  1537d8: 08 00 82 a0  	sb	$2, 0x8($4)
  1537dc: a8 1f 2a ad  	sw	$10, 0x1fa8($9)
  1537e0: 04 00 29 25  	addiu	$9, $9, 0x4
  1537e4: 08 00 62 a0  	sb	$2, 0x8($3)
  1537e8: eb ff 00 15  	bnez	$8, 0x153798 <.text+0x53798>
  1537ec: 08 00 e0 a0  	sb	$zero, 0x8($7)
  1537f0: 2d c8 00 00  	move	$25, $zero
  1537f4: 2d c0 00 02  	move	$24, $16
  1537f8: 35 00 02 3c  	lui	$2, 0x35
  1537fc: 01 00 0c 3c  	lui	$12, 0x1
  153800: 98 e2 4d 8c  	lw	$13, -0x1d68($2)
  153804: 21 18 30 03  	addu	$3, $25, $16
  153808: 50 97 02 34  	ori	$2, $zero, 0x9750
  15380c: 00 80 8c 35  	ori	$12, $12, 0x8000
  153810: 01 00 39 27  	addiu	$25, $25, 0x1
  153814: 50 a7 0e 34  	ori	$14, $zero, 0xa750
  153818: 00 80 0b 34  	ori	$11, $zero, 0x8000
  15381c: 01 00 0a 3c  	lui	$10, 0x1
  153820: 20 97 09 34  	ori	$9, $zero, 0x9720
  153824: 20 a7 06 34  	ori	$6, $zero, 0xa720
  153828: 30 97 05 34  	ori	$5, $zero, 0x9730
  15382c: 30 a7 04 34  	ori	$4, $zero, 0xa730
  153830: 40 97 07 34  	ori	$7, $zero, 0x9740
  153834: 40 a7 08 34  	ori	$8, $zero, 0xa740
  153838: 21 70 6e 00  	addu	$14, $3, $14
  15383c: 21 48 69 00  	addu	$9, $3, $9
  153840: 21 30 66 00  	addu	$6, $3, $6
  153844: 21 28 65 00  	addu	$5, $3, $5
  153848: 21 20 64 00  	addu	$4, $3, $4
  15384c: 21 38 67 00  	addu	$7, $3, $7
  153850: 21 40 68 00  	addu	$8, $3, $8
  153854: 21 60 ac 01  	addu	$12, $13, $12
  153858: 21 18 62 00  	addu	$3, $3, $2
  15385c: 21 58 ab 01  	addu	$11, $13, $11
  153860: 01 00 02 24  	addiu	$2, $zero, 0x1
  153864: 21 50 aa 01  	addu	$10, $13, $10
  153868: 10 00 2f 2b  	slti	$15, $25, 0x10
  15386c: 08 00 22 a1  	sb	$2, 0x8($9)
  153870: 08 00 c0 a0  	sb	$zero, 0x8($6)
  153874: 08 00 a2 a0  	sb	$2, 0x8($5)
  153878: 08 00 80 a0  	sb	$zero, 0x8($4)
  15387c: 68 1c 0b af  	sw	$11, 0x1c68($24)
  153880: a8 1c 0a af  	sw	$10, 0x1ca8($24)
  153884: e8 1c 0c af  	sw	$12, 0x1ce8($24)
  153888: 08 00 e2 a0  	sb	$2, 0x8($7)
  15388c: 08 00 00 a1  	sb	$zero, 0x8($8)
  153890: 28 1c 0d af  	sw	$13, 0x1c28($24)
  153894: 04 00 18 27  	addiu	$24, $24, 0x4
  153898: 08 00 62 a0  	sb	$2, 0x8($3)
  15389c: d6 ff e0 15  	bnez	$15, 0x1537f8 <.text+0x537f8>
  1538a0: 08 00 c0 a1  	sb	$zero, 0x8($14)
  1538a4: 00 00 b0 df  	ld	$16, 0x0($sp)
  1538a8: 08 00 e0 03  	jr	$ra
  1538ac: 10 00 bd 27  	addiu	$sp, $sp, 0x10

## structured header validator [0x00158974..0x00158a58)
  158974: 19 00 82 90  	lbu	$2, 0x19($4)
  158978: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  15897c: 4f 00 42 30  	andi	$2, $2, 0x4f
  158980: 07 00 40 14  	bnez	$2, 0x1589a0 <.text+0x589a0>
  158984: 00 00 bf ff  	sd	$ra, 0x0($sp)
  158988: 1a 00 83 90  	lbu	$3, 0x1a($4)
  15898c: 33 00 02 24  	addiu	$2, $zero, 0x33
  158990: 08 00 62 10  	beq	$3, $2, 0x1589b4 <.text+0x589b4>
  158994: ff 00 02 24  	addiu	$2, $zero, 0xff
  158998: 07 00 62 50  	beql	$3, $2, 0x1589b8 <.text+0x589b8>
  15899c: 17 00 82 90  	lbu	$2, 0x17($4)
  1589a0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1589a4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1589a8: 2d 10 60 00  	move	$2, $3
  1589ac: 08 00 e0 03  	jr	$ra
  1589b0: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1589b4: 17 00 82 90  	lbu	$2, 0x17($4)
  1589b8: 16 00 83 90  	lbu	$3, 0x16($4)
  1589bc: 00 12 02 00  	sll	$2, $2, 0x8
  1589c0: 25 18 43 00  	or	$3, $2, $3
  1589c4: 0b 00 60 50  	beqzl	$3, 0x1589f4 <.text+0x589f4>
  1589c8: 18 00 83 90  	lbu	$3, 0x18($4)
  1589cc: ff ff 02 34  	ori	$2, $zero, 0xffff
  1589d0: 07 00 62 10  	beq	$3, $2, 0x1589f0 <.text+0x589f0>
  1589d4: 0f 04 62 30  	andi	$2, $3, 0x40f
  1589d8: f2 ff 40 54  	bnezl	$2, 0x1589a4 <.text+0x589a4>
  1589dc: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1589e0: ff 00 62 30  	andi	$2, $3, 0xff
  1589e4: c1 00 42 2c  	sltiu	$2, $2, 0xc1
  1589e8: ee ff 40 10  	beqz	$2, 0x1589a4 <.text+0x589a4>
  1589ec: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1589f0: 18 00 83 90  	lbu	$3, 0x18($4)
  1589f4: ce 00 62 30  	andi	$2, $3, 0xce
  1589f8: ea ff 40 54  	bnezl	$2, 0x1589a4 <.text+0x589a4>
  1589fc: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  158a00: 30 00 62 30  	andi	$2, $3, 0x30
  158a04: e7 ff 40 10  	beqz	$2, 0x1589a4 <.text+0x589a4>
  158a08: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  158a0c: 15 00 82 90  	lbu	$2, 0x15($4)
  158a10: 03 00 42 30  	andi	$2, $2, 0x3
  158a14: e4 ff 40 14  	bnez	$2, 0x1589a8 <.text+0x589a8>
  158a18: 00 00 bf df  	ld	$ra, 0x0($sp)
  158a1c: 13 00 83 90  	lbu	$3, 0x13($4)
  158a20: 05 00 60 50  	beqzl	$3, 0x158a38 <.text+0x58a38>
  158a24: 14 00 82 90  	lbu	$2, 0x14($4)
  158a28: ff 00 02 24  	addiu	$2, $zero, 0xff
  158a2c: de ff 62 14  	bne	$3, $2, 0x1589a8 <.text+0x589a8>
  158a30: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  158a34: 14 00 82 90  	lbu	$2, 0x14($4)
  158a38: da ff 40 14  	bnez	$2, 0x1589a4 <.text+0x589a4>
  158a3c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  158a40: 96 62 05 0c  	jal	0x158a58 <.text+0x58a58>
  158a44: 00 00 00 00  	nop
  158a48: d6 ff 40 10  	beqz	$2, 0x1589a4 <.text+0x589a4>
  158a4c: 2d 18 00 00  	move	$3, $zero
  158a50: d4 ff 00 10  	b	0x1589a4 <.text+0x589a4>
  158a54: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>

## bounded multibyte validator [0x00158a58..0x00158b3c)
  158a58: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  158a5c: 40 00 bf ff  	sd	$ra, 0x40($sp)
  158a60: 00 00 b0 ff  	sd	$16, 0x0($sp)
  158a64: 30 00 b3 ff  	sd	$19, 0x30($sp)
  158a68: 10 00 13 24  	addiu	$19, $zero, 0x10
  158a6c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  158a70: 2d 90 00 00  	move	$18, $zero
  158a74: 10 00 b1 ff  	sd	$17, 0x10($sp)
  158a78: 2d 88 80 00  	move	$17, $4
  158a7c: 00 00 30 92  	lbu	$16, 0x0($17)
  158a80: 01 00 31 26  	addiu	$17, $17, 0x1
  158a84: cf 62 05 0c  	jal	0x158b3c <.text+0x58b3c>
  158a88: 2d 20 00 02  	move	$4, $16
  158a8c: 1a 00 40 10  	beqz	$2, 0x158af8 <.text+0x58af8>
  158a90: 00 00 00 00  	nop
  158a94: 00 00 30 92  	lbu	$16, 0x0($17)
  158a98: 20 00 02 2e  	sltiu	$2, $16, 0x20
  158a9c: 06 00 40 10  	beqz	$2, 0x158ab8 <.text+0x58ab8>
  158aa0: 01 00 31 26  	addiu	$17, $17, 0x1
  158aa4: 0b 00 02 24  	addiu	$2, $zero, 0xb
  158aa8: 0a 00 42 16  	bne	$18, $2, 0x158ad4 <.text+0x58ad4>
  158aac: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  158ab0: 09 00 00 16  	bnez	$16, 0x158ad8 <.text+0x58ad8>
  158ab4: 40 00 bf df  	ld	$ra, 0x40($sp)
  158ab8: 01 00 52 26  	addiu	$18, $18, 0x1
  158abc: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  158ac0: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  158ac4: ee ff 60 5e  	bgtzl	$19, 0x158a80 <.text+0x58a80>
  158ac8: 00 00 30 92  	lbu	$16, 0x0($17)
  158acc: 08 00 40 1a  	blez	$18, 0x158af0 <.text+0x58af0>
  158ad0: 2d 10 00 00  	move	$2, $zero
  158ad4: 40 00 bf df  	ld	$ra, 0x40($sp)
  158ad8: 30 00 b3 df  	ld	$19, 0x30($sp)
  158adc: 20 00 b2 df  	ld	$18, 0x20($sp)
  158ae0: 10 00 b1 df  	ld	$17, 0x10($sp)
  158ae4: 00 00 b0 df  	ld	$16, 0x0($sp)
  158ae8: 08 00 e0 03  	jr	$ra
  158aec: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  158af0: f8 ff 00 10  	b	0x158ad4 <.text+0x58ad4>
  158af4: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  158af8: 05 00 00 16  	bnez	$16, 0x158b10 <.text+0x58b10>
  158afc: 20 00 02 2e  	sltiu	$2, $16, 0x20
  158b00: f0 ff 40 16  	bnez	$18, 0x158ac4 <.text+0x58ac4>
  158b04: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  158b08: f2 ff 00 10  	b	0x158ad4 <.text+0x58ad4>
  158b0c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  158b10: f0 ff 40 14  	bnez	$2, 0x158ad4 <.text+0x58ad4>
  158b14: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  158b18: 80 00 02 2e  	sltiu	$2, $16, 0x80
  158b1c: e8 ff 40 14  	bnez	$2, 0x158ac0 <.text+0x58ac0>
  158b20: 01 00 52 26  	addiu	$18, $18, 0x1
  158b24: 60 ff 02 26  	addiu	$2, $16, -0xa0 <.text+0xffffffffffefff60>
  158b28: 50 00 42 2c  	sltiu	$2, $2, 0x50
  158b2c: e5 ff 40 14  	bnez	$2, 0x158ac4 <.text+0x58ac4>
  158b30: ff ff 73 26  	addiu	$19, $19, -0x1 <.text+0xffffffffffefffff>
  158b34: e7 ff 00 10  	b	0x158ad4 <.text+0x58ad4>
  158b38: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>

## frontend stage state machine [0x0015cdf8..0x0015cea4)
  15cdf8: 36 00 02 3c  	lui	$2, 0x36
  15cdfc: b8 d0 43 8c  	lw	$3, -0x2f48($2)
  15ce00: 07 00 62 2c  	sltiu	$2, $3, 0x7
  15ce04: 24 00 40 50  	beqzl	$2, 0x15ce98 <.text+0x5ce98>
  15ce08: 01 00 03 24  	addiu	$3, $zero, 0x1
  15ce0c: 80 10 03 00  	sll	$2, $3, 0x2
  15ce10: 1b 00 03 3c  	lui	$3, 0x1b
  15ce14: bc 7d 63 24  	addiu	$3, $3, 0x7dbc
  15ce18: 21 10 43 00  	addu	$2, $2, $3
  15ce1c: 00 00 42 8c  	lw	$2, 0x0($2)
  15ce20: 08 00 40 00  	jr	$2
  15ce24: 00 00 00 00  	nop
  15ce28: 34 00 02 3c  	lui	$2, 0x34
  15ce2c: 3f 55 42 90  	lbu	$2, 0x553f($2)
  15ce30: 19 00 40 14  	bnez	$2, 0x15ce98 <.text+0x5ce98>
  15ce34: 02 00 03 24  	addiu	$3, $zero, 0x2
  15ce38: 34 00 02 3c  	lui	$2, 0x34
  15ce3c: 3f 55 42 90  	lbu	$2, 0x553f($2)
  15ce40: 15 00 40 14  	bnez	$2, 0x15ce98 <.text+0x5ce98>
  15ce44: 03 00 03 24  	addiu	$3, $zero, 0x3
  15ce48: 34 00 02 3c  	lui	$2, 0x34
  15ce4c: 3e 55 42 90  	lbu	$2, 0x553e($2)
  15ce50: 11 00 40 14  	bnez	$2, 0x15ce98 <.text+0x5ce98>
  15ce54: 04 00 03 24  	addiu	$3, $zero, 0x4
  15ce58: 34 00 02 3c  	lui	$2, 0x34
  15ce5c: 11 56 42 90  	lbu	$2, 0x5611($2)
  15ce60: 0d 00 40 14  	bnez	$2, 0x15ce98 <.text+0x5ce98>
  15ce64: 05 00 03 24  	addiu	$3, $zero, 0x5
  15ce68: 34 00 02 3c  	lui	$2, 0x34
  15ce6c: 11 56 42 90  	lbu	$2, 0x5611($2)
  15ce70: 09 00 40 14  	bnez	$2, 0x15ce98 <.text+0x5ce98>
  15ce74: 06 00 03 24  	addiu	$3, $zero, 0x6
  15ce78: 34 00 02 3c  	lui	$2, 0x34
  15ce7c: 3d 55 42 90  	lbu	$2, 0x553d($2)
  15ce80: 05 00 40 50  	beqzl	$2, 0x15ce98 <.text+0x5ce98>
  15ce84: 01 00 03 24  	addiu	$3, $zero, 0x1
  15ce88: 36 00 02 3c  	lui	$2, 0x36
  15ce8c: 08 00 e0 03  	jr	$ra
  15ce90: b8 d0 40 ac  	sw	$zero, -0x2f48($2)
  15ce94: 01 00 03 24  	addiu	$3, $zero, 0x1
  15ce98: 36 00 02 3c  	lui	$2, 0x36
  15ce9c: 08 00 e0 03  	jr	$ra
  15cea0: b8 d0 43 ac  	sw	$3, -0x2f48($2)

## PPU/frontend reset [0x0015d9ac..0x0015dab4)
  15d9ac: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  15d9b0: 50 00 bf ff  	sd	$ra, 0x50($sp)
  15d9b4: 40 00 b4 ff  	sd	$20, 0x40($sp)
  15d9b8: 01 00 14 24  	addiu	$20, $zero, 0x1
  15d9bc: 30 00 b3 ff  	sd	$19, 0x30($sp)
  15d9c0: 34 00 13 3c  	lui	$19, 0x34
  15d9c4: 20 00 b2 ff  	sd	$18, 0x20($sp)
  15d9c8: 35 00 12 3c  	lui	$18, 0x35
  15d9cc: 10 00 b1 ff  	sd	$17, 0x10($sp)
  15d9d0: b0 e2 52 26  	addiu	$18, $18, -0x1d50 <.text+0xffffffffffefe2b0>
  15d9d4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  15d9d8: f8 5a 71 26  	addiu	$17, $19, 0x5af8
  15d9dc: 34 00 10 3c  	lui	$16, 0x34
  15d9e0: e8 5a 00 a2  	sb	$zero, 0x5ae8($16)
  15d9e4: e8 5a 10 26  	addiu	$16, $16, 0x5ae8
  15d9e8: 14 00 43 8e  	lw	$3, 0x14($18)
  15d9ec: 04 22 62 90  	lbu	$2, 0x2204($3)
  15d9f0: 03 22 63 90  	lbu	$3, 0x2203($3)
  15d9f4: 00 12 02 00  	sll	$2, $2, 0x8
  15d9f8: 09 00 14 a2  	sb	$20, 0x9($16)
  15d9fc: 25 18 62 00  	or	$3, $3, $2
  15da00: 06 00 00 a6  	sh	$zero, 0x6($16)
  15da04: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  15da08: 0e 00 03 a6  	sh	$3, 0xe($16)
  15da0c: 08 00 02 a2  	sb	$2, 0x8($16)
  15da10: 2d 20 60 00  	move	$4, $3
  15da14: 34 01 02 24  	addiu	$2, $zero, 0x134
  15da18: 01 00 00 a2  	sb	$zero, 0x1($16)
  15da1c: 02 00 02 a6  	sh	$2, 0x2($16)
  15da20: 0b 00 00 a2  	sb	$zero, 0xb($16)
  15da24: 0d 00 00 a2  	sb	$zero, 0xd($16)
  15da28: 0c 00 20 ae  	sw	$zero, 0xc($17)
  15da2c: 10 00 20 ae  	sw	$zero, 0x10($17)
  15da30: 1b 00 20 a2  	sb	$zero, 0x1b($17)
  15da34: 20 00 20 ae  	sw	$zero, 0x20($17)
  15da38: 1f 78 05 0c  	jal	0x15e07c <.text+0x5e07c>
  15da3c: 24 00 20 ae  	sw	$zero, 0x24($17)
  15da40: 02 00 04 92  	lbu	$4, 0x2($16)
  15da44: 3f 00 02 3c  	lui	$2, 0x3f
  15da48: 40 50 42 24  	addiu	$2, $2, 0x5040
  15da4c: 42 18 04 00  	srl	$3, $4, 0x1
  15da50: 40 00 85 30  	andi	$5, $4, 0x40
  15da54: 01 00 63 38  	xori	$3, $3, 0x1
  15da58: 80 00 86 30  	andi	$6, $4, 0x80
  15da5c: 82 29 05 00  	srl	$5, $5, 0x6
  15da60: 01 00 63 30  	andi	$3, $3, 0x1
  15da64: 01 00 84 30  	andi	$4, $4, 0x1
  15da68: f8 5a 62 ae  	sw	$2, 0x5af8($19)
  15da6c: 05 00 23 a2  	sb	$3, 0x5($17)
  15da70: 06 00 26 a2  	sb	$6, 0x6($17)
  15da74: 04 00 24 a2  	sb	$4, 0x4($17)
  15da78: 57 7c 05 0c  	jal	0x15f15c <.text+0x5f15c>
  15da7c: 07 00 25 a2  	sb	$5, 0x7($17)
  15da80: 0c 00 42 8e  	lw	$2, 0xc($18)
  15da84: 18 00 34 a2  	sb	$20, 0x18($17)
  15da88: 14 00 43 8e  	lw	$3, 0x14($18)
  15da8c: 28 00 22 ae  	sw	$2, 0x28($17)
  15da90: 50 00 bf df  	ld	$ra, 0x50($sp)
  15da94: 40 00 b4 df  	ld	$20, 0x40($sp)
  15da98: 30 00 b3 df  	ld	$19, 0x30($sp)
  15da9c: 20 00 b2 df  	ld	$18, 0x20($sp)
  15daa0: 10 00 b1 df  	ld	$17, 0x10($sp)
  15daa4: 00 00 b0 df  	ld	$16, 0x0($sp)
  15daa8: 25 22 60 a0  	sb	$zero, 0x2225($3)
  15daac: 08 00 e0 03  	jr	$ra
  15dab0: 60 00 bd 27  	addiu	$sp, $sp, 0x60

## PPU/frontend reload [0x0015db74..0x0015dc84)
  15db74: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  15db78: 30 00 bf ff  	sd	$ra, 0x30($sp)
  15db7c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  15db80: 80 ff 12 24  	addiu	$18, $zero, -0x80 <.text+0xffffffffffefff80>
  15db84: 10 00 b1 ff  	sd	$17, 0x10($sp)
  15db88: 34 00 11 3c  	lui	$17, 0x34
  15db8c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  15db90: 34 00 10 3c  	lui	$16, 0x34
  15db94: f8 5a 10 26  	addiu	$16, $16, 0x5af8
  15db98: e8 5a 23 92  	lbu	$3, 0x5ae8($17)
  15db9c: e8 5a 31 26  	addiu	$17, $17, 0x5ae8
  15dba0: 01 00 22 92  	lbu	$2, 0x1($17)
  15dba4: 00 1c 03 00  	sll	$3, $3, 0x10
  15dba8: 0e 00 24 96  	lhu	$4, 0xe($17)
  15dbac: 00 14 02 00  	sll	$2, $2, 0x10
  15dbb0: 0c 00 03 ae  	sw	$3, 0xc($16)
  15dbb4: 10 00 02 ae  	sw	$2, 0x10($16)
  15dbb8: 1f 78 05 0c  	jal	0x15e07c <.text+0x5e07c>
  15dbbc: 21 20 64 00  	addu	$4, $3, $4
  15dbc0: 02 00 23 92  	lbu	$3, 0x2($17)
  15dbc4: 42 10 03 00  	srl	$2, $3, 0x1
  15dbc8: 40 00 64 30  	andi	$4, $3, 0x40
  15dbcc: 01 00 42 38  	xori	$2, $2, 0x1
  15dbd0: 82 21 04 00  	srl	$4, $4, 0x6
  15dbd4: 01 00 42 30  	andi	$2, $2, 0x1
  15dbd8: 24 28 72 00  	and	$5, $3, $18
  15dbdc: 01 00 63 30  	andi	$3, $3, 0x1
  15dbe0: 05 00 02 a2  	sb	$2, 0x5($16)
  15dbe4: 04 00 03 a2  	sb	$3, 0x4($16)
  15dbe8: 07 00 04 a2  	sb	$4, 0x7($16)
  15dbec: 57 7c 05 0c  	jal	0x15f15c <.text+0x5f15c>
  15dbf0: 06 00 05 a2  	sb	$5, 0x6($16)
  15dbf4: 35 00 02 3c  	lui	$2, 0x35
  15dbf8: c4 e2 43 8c  	lw	$3, -0x1d3c($2)
  15dbfc: 00 80 02 34  	ori	$2, $zero, 0x8000
  15dc00: 21 80 02 02  	addu	$16, $16, $2
  15dc04: 3f 22 62 90  	lbu	$2, 0x223f($3)
  15dc08: 24 10 52 00  	and	$2, $2, $18
  15dc0c: 02 00 40 14  	bnez	$2, 0x15dc18 <.text+0x5dc18>
  15dc10: 02 00 04 24  	addiu	$4, $zero, 0x2
  15dc14: 04 00 04 24  	addiu	$4, $zero, 0x4
  15dc18: 51 00 04 a2  	sb	$4, 0x51($16)
  15dc1c: 35 00 10 3c  	lui	$16, 0x35
  15dc20: b0 e2 10 26  	addiu	$16, $16, -0x1d50 <.text+0xffffffffffefe2b0>
  15dc24: 14 00 04 8e  	lw	$4, 0x14($16)
  15dc28: 0c 00 03 8e  	lw	$3, 0xc($16)
  15dc2c: 24 22 82 90  	lbu	$2, 0x2224($4)
  15dc30: 07 00 42 30  	andi	$2, $2, 0x7
  15dc34: 40 13 02 00  	sll	$2, $2, 0xd
  15dc38: 21 18 62 00  	addu	$3, $3, $2
  15dc3c: 10 00 03 ae  	sw	$3, 0x10($16)
  15dc40: ad 76 05 0c  	jal	0x15dab4 <.text+0x5dab4>
  15dc44: 25 22 84 90  	lbu	$4, 0x2225($4)
  15dc48: 10 00 b1 df  	ld	$17, 0x10($sp)
  15dc4c: 14 00 02 8e  	lw	$2, 0x14($16)
  15dc50: 34 00 03 3c  	lui	$3, 0x34
  15dc54: 30 00 bf df  	ld	$ra, 0x30($sp)
  15dc58: f8 5a 63 24  	addiu	$3, $3, 0x5af8
  15dc5c: 00 22 42 90  	lbu	$2, 0x2200($2)
  15dc60: 20 00 b2 df  	ld	$18, 0x20($sp)
  15dc64: 60 00 42 30  	andi	$2, $2, 0x60
  15dc68: 00 00 b0 df  	ld	$16, 0x0($sp)
  15dc6c: 2b 10 02 00  	sltu	$2, $zero, $2
  15dc70: 01 00 44 38  	xori	$4, $2, 0x1
  15dc74: 1c 00 62 a0  	sb	$2, 0x1c($3)
  15dc78: 18 00 64 a0  	sb	$4, 0x18($3)
  15dc7c: 08 00 e0 03  	jr	$ra
  15dc80: 40 00 bd 27  	addiu	$sp, $sp, 0x40

## Snes9x map cursor selector [0x0015e07c..0x0015e190)
  15e07c: 82 12 04 00  	srl	$2, $4, 0xa
  15e080: 34 00 03 3c  	lui	$3, 0x34
  15e084: f8 5a 63 24  	addiu	$3, $3, 0x5af8
  15e088: fc 3f 42 30  	andi	$2, $2, 0x3ffc
  15e08c: 21 10 43 00  	addu	$2, $2, $3
  15e090: 40 00 45 8c  	lw	$5, 0x40($2)
  15e094: 12 00 a2 2c  	sltiu	$2, $5, 0x12
  15e098: 06 00 40 14  	bnez	$2, 0x15e0b4 <.text+0x5e0b4>
  15e09c: 0c 00 a2 2c  	sltiu	$2, $5, 0xc
  15e0a0: ff ff 82 30  	andi	$2, $4, 0xffff
  15e0a4: 24 00 65 ac  	sw	$5, 0x24($3)
  15e0a8: 21 10 a2 00  	addu	$2, $5, $2
  15e0ac: 08 00 e0 03  	jr	$ra
  15e0b0: 20 00 62 ac  	sw	$2, 0x20($3)
  15e0b4: 2a 00 40 10  	beqz	$2, 0x15e160 <.text+0x5e160>
  15e0b8: 35 00 02 3c  	lui	$2, 0x35
  15e0bc: 1b 00 03 3c  	lui	$3, 0x1b
  15e0c0: 80 10 05 00  	sll	$2, $5, 0x2
  15e0c4: 38 7e 63 24  	addiu	$3, $3, 0x7e38
  15e0c8: 21 10 43 00  	addu	$2, $2, $3
  15e0cc: 00 00 42 8c  	lw	$2, 0x0($2)
  15e0d0: 08 00 40 00  	jr	$2
  15e0d4: 00 00 00 00  	nop
  15e0d8: 35 00 02 3c  	lui	$2, 0x35
  15e0dc: 34 00 03 3c  	lui	$3, 0x34
  15e0e0: c4 e2 42 8c  	lw	$2, -0x1d3c($2)
  15e0e4: ff ff 84 30  	andi	$4, $4, 0xffff
  15e0e8: f8 5a 63 24  	addiu	$3, $3, 0x5af8
  15e0ec: 00 e0 42 24  	addiu	$2, $2, -0x2000 <.text+0xffffffffffefe000>
  15e0f0: 21 20 44 00  	addu	$4, $2, $4
  15e0f4: 24 00 62 ac  	sw	$2, 0x24($3)
  15e0f8: 08 00 e0 03  	jr	$ra
  15e0fc: 20 00 64 ac  	sw	$4, 0x20($3)
  15e100: 35 00 02 3c  	lui	$2, 0x35
  15e104: 34 00 03 3c  	lui	$3, 0x34
  15e108: c4 e2 42 8c  	lw	$2, -0x1d3c($2)
  15e10c: ff ff 84 30  	andi	$4, $4, 0xffff
  15e110: f8 5a 63 24  	addiu	$3, $3, 0x5af8
  15e114: f6 ff 00 10  	b	0x15e0f0 <.text+0x5e0f0>
  15e118: 00 c0 42 24  	addiu	$2, $2, -0x4000 <.text+0xffffffffffefc000>
  15e11c: 35 00 02 3c  	lui	$2, 0x35
  15e120: ff ff 84 30  	andi	$4, $4, 0xffff
  15e124: c4 e2 42 8c  	lw	$2, -0x1d3c($2)
  15e128: 34 00 03 3c  	lui	$3, 0x34
  15e12c: f8 5a 63 24  	addiu	$3, $3, 0x5af8
  15e130: ef ff 00 10  	b	0x15e0f0 <.text+0x5e0f0>
  15e134: 00 a0 42 24  	addiu	$2, $2, -0x6000 <.text+0xffffffffffefa000>
  15e138: 35 00 02 3c  	lui	$2, 0x35
  15e13c: ff ff 84 30  	andi	$4, $4, 0xffff
  15e140: f9 ff 00 10  	b	0x15e128 <.text+0x5e128>
  15e144: bc e2 42 8c  	lw	$2, -0x1d44($2)
  15e148: 34 00 03 3c  	lui	$3, 0x34
  15e14c: ff ff 84 30  	andi	$4, $4, 0xffff
  15e150: f8 5a 63 24  	addiu	$3, $3, 0x5af8
  15e154: f6 ff 00 10  	b	0x15e130 <.text+0x5e130>
  15e158: 28 00 62 8c  	lw	$2, 0x28($3)
  15e15c: 35 00 02 3c  	lui	$2, 0x35
  15e160: ff ff 83 30  	andi	$3, $4, 0xffff
  15e164: b0 e2 44 8c  	lw	$4, -0x1d50($2)
  15e168: 34 00 02 3c  	lui	$2, 0x34
  15e16c: 21 18 83 00  	addu	$3, $4, $3
  15e170: f8 5a 42 24  	addiu	$2, $2, 0x5af8
  15e174: 24 00 44 ac  	sw	$4, 0x24($2)
  15e178: 08 00 e0 03  	jr	$ra
  15e17c: 20 00 43 ac  	sw	$3, 0x20($2)
  15e180: 35 00 02 3c  	lui	$2, 0x35
  15e184: ff ff 83 30  	andi	$3, $4, 0xffff
  15e188: f7 ff 00 10  	b	0x15e168 <.text+0x5e168>
  15e18c: bc e2 44 8c  	lw	$4, -0x1d44($2)

## PPU shifted read window [0x0015f030..0x0015f15c)
  15f030: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  15f034: 35 00 02 3c  	lui	$2, 0x35
  15f038: 50 00 b5 ff  	sd	$21, 0x50($sp)
  15f03c: 00 80 03 34  	ori	$3, $zero, 0x8000
  15f040: 40 00 b4 ff  	sd	$20, 0x40($sp)
  15f044: ff 00 a5 30  	andi	$5, $5, 0xff
  15f048: 30 00 b3 ff  	sd	$19, 0x30($sp)
  15f04c: 00 00 a5 38  	xori	$5, $5, 0x0
  15f050: 20 00 b2 ff  	sd	$18, 0x20($sp)
  15f054: ff 00 94 30  	andi	$20, $4, 0xff
  15f058: 10 00 b1 ff  	sd	$17, 0x10($sp)
  15f05c: 60 00 bf ff  	sd	$ra, 0x60($sp)
  15f060: 00 00 b0 ff  	sd	$16, 0x0($sp)
  15f064: c4 e2 47 8c  	lw	$7, -0x1d3c($2)
  15f068: 34 00 02 3c  	lui	$2, 0x34
  15f06c: f8 5a 42 24  	addiu	$2, $2, 0x5af8
  15f070: 58 22 e6 90  	lbu	$6, 0x2258($7)
  15f074: 21 a8 43 00  	addu	$21, $2, $3
  15f078: 10 00 02 24  	addiu	$2, $zero, 0x10
  15f07c: 5a 22 e3 90  	lbu	$3, 0x225a($7)
  15f080: 0f 00 d2 30  	andi	$18, $6, 0xf
  15f084: 59 22 e6 90  	lbu	$6, 0x2259($7)
  15f088: 0a 90 52 00  	movz	$18, $2, $18
  15f08c: 00 1a 03 00  	sll	$3, $3, 0x8
  15f090: 53 00 a2 92  	lbu	$2, 0x53($21)
  15f094: 0b 90 05 00  	movn	$18, $zero, $5
  15f098: 5b 22 e5 90  	lbu	$5, 0x225b($7)
  15f09c: 25 30 c3 00  	or	$6, $6, $3
  15f0a0: 21 10 42 02  	addu	$2, $18, $2
  15f0a4: ff 00 53 30  	andi	$19, $2, 0xff
  15f0a8: 00 2c 05 00  	sll	$5, $5, 0x10
  15f0ac: 02 11 13 00  	srl	$2, $19, 0x4
  15f0b0: 40 18 02 00  	sll	$3, $2, 0x1
  15f0b4: 10 00 62 2e  	sltiu	$2, $19, 0x10
  15f0b8: 03 00 40 14  	bnez	$2, 0x15f0c8 <.text+0x5f0c8>
  15f0bc: 25 88 c5 00  	or	$17, $6, $5
  15f0c0: 21 88 23 02  	addu	$17, $17, $3
  15f0c4: 0f 00 73 32  	andi	$19, $19, 0xf
  15f0c8: 85 77 05 0c  	jal	0x15de14 <.text+0x5de14>
  15f0cc: 2d 20 20 02  	move	$4, $17
  15f0d0: 02 00 24 26  	addiu	$4, $17, 0x2
  15f0d4: 85 77 05 0c  	jal	0x15de14 <.text+0x5de14>
  15f0d8: 2d 80 40 00  	move	$16, $2
  15f0dc: 35 00 03 3c  	lui	$3, 0x35
  15f0e0: 00 14 02 00  	sll	$2, $2, 0x10
  15f0e4: b0 e2 64 24  	addiu	$4, $3, -0x1d50 <.text+0xffffffffffefe2b0>
  15f0e8: 25 80 02 02  	or	$16, $16, $2
  15f0ec: 14 00 82 8c  	lw	$2, 0x14($4)
  15f0f0: 06 80 70 02  	srlv	$16, $16, $19
  15f0f4: 02 1a 10 00  	srl	$3, $16, 0x8
  15f0f8: 02 32 11 00  	srl	$6, $17, 0x8
  15f0fc: 0c 23 50 a0  	sb	$16, 0x230c($2)
  15f100: 02 2c 11 00  	srl	$5, $17, 0x10
  15f104: 14 00 82 8c  	lw	$2, 0x14($4)
  15f108: 0b 00 80 12  	beqz	$20, 0x15f138 <.text+0x5f138>
  15f10c: 0d 23 43 a0  	sb	$3, 0x230d($2)
  15f110: 53 00 a2 92  	lbu	$2, 0x53($21)
  15f114: 14 00 83 8c  	lw	$3, 0x14($4)
  15f118: 21 10 42 02  	addu	$2, $18, $2
  15f11c: 0f 00 42 30  	andi	$2, $2, 0xf
  15f120: 53 00 a2 a2  	sb	$2, 0x53($21)
  15f124: 59 22 71 a0  	sb	$17, 0x2259($3)
  15f128: 14 00 82 8c  	lw	$2, 0x14($4)
  15f12c: 5a 22 46 a0  	sb	$6, 0x225a($2)
  15f130: 14 00 82 8c  	lw	$2, 0x14($4)
  15f134: 5b 22 45 a0  	sb	$5, 0x225b($2)
  15f138: 60 00 bf df  	ld	$ra, 0x60($sp)
  15f13c: 50 00 b5 df  	ld	$21, 0x50($sp)
  15f140: 40 00 b4 df  	ld	$20, 0x40($sp)
  15f144: 30 00 b3 df  	ld	$19, 0x30($sp)
  15f148: 20 00 b2 df  	ld	$18, 0x20($sp)
  15f14c: 10 00 b1 df  	ld	$17, 0x10($sp)
  15f150: 00 00 b0 df  	ld	$16, 0x0($sp)
  15f154: 08 00 e0 03  	jr	$ra
  15f158: 70 00 bd 27  	addiu	$sp, $sp, 0x70

## runtime/controller slot period configuration [0x00173e6c..0x00173f24)
  173e6c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  173e70: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  173e74: 10 00 b1 ff  	sd	$17, 0x10($sp)
  173e78: 2d 88 80 00  	move	$17, $4
  173e7c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  173e80: 2d 20 a0 00  	move	$4, $5
  173e84: 00 00 b0 ff  	sd	$16, 0x0($sp)
  173e88: 23 00 a2 10  	beq	$5, $2, 0x173f18 <.text+0x73f18>
  173e8c: 24 00 27 a6  	sh	$7, 0x24($17)
  173e90: 38 00 26 ae  	sw	$6, 0x38($17)
  173e94: 07 00 80 50  	beqzl	$4, 0x173eb4 <.text+0x73eb4>
  173e98: 30 00 20 fe  	sd	$zero, 0x30($17)
  173e9c: 3b 00 02 3c  	lui	$2, 0x3b
  173ea0: 18 b7 45 24  	addiu	$5, $2, -0x48e8 <.text+0xffffffffffefb718>
  173ea4: 08 00 a2 8c  	lw	$2, 0x8($5)
  173ea8: 07 00 40 54  	bnezl	$2, 0x173ec8 <.text+0x73ec8>
  173eac: 00 00 23 8e  	lw	$3, 0x0($17)
  173eb0: 30 00 20 fe  	sd	$zero, 0x30($17)
  173eb4: 20 00 bf df  	ld	$ra, 0x20($sp)
  173eb8: 10 00 b1 df  	ld	$17, 0x10($sp)
  173ebc: 00 00 b0 df  	ld	$16, 0x0($sp)
  173ec0: 08 00 e0 03  	jr	$ra
  173ec4: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  173ec8: 41 00 02 3c  	lui	$2, 0x41
  173ecc: 00 0f 42 24  	addiu	$2, $2, 0xf00
  173ed0: 08 00 a5 8c  	lw	$5, 0x8($5)
  173ed4: 80 18 03 00  	sll	$3, $3, 0x2
  173ed8: 21 18 62 00  	addu	$3, $3, $2
  173edc: 00 00 70 8c  	lw	$16, 0x0($3)
  173ee0: e8 03 02 3c  	lui	$2, 0x3e8
  173ee4: 18 00 02 02  	mult	$16, $2
  173ee8: 12 18 00 00  	mflo	$3
  173eec: 10 80 00 00  	mfhi	$16
  173ef0: 3c 18 03 00  	dsll32	$3, $3, 0x0
  173ef4: 3c 80 10 00  	dsll32	$16, $16, 0x0
  173ef8: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  173efc: c8 86 06 0c  	jal	0x1a1b20 <.text+0xa1b20>
  173f00: 25 80 03 02  	or	$16, $16, $3
  173f04: 2d 28 40 00  	move	$5, $2
  173f08: 6c 89 06 0c  	jal	0x1a25b0 <.text+0xa25b0>
  173f0c: 2d 20 00 02  	move	$4, $16
  173f10: e8 ff 00 10  	b	0x173eb4 <.text+0x73eb4>
  173f14: 30 00 22 fe  	sd	$2, 0x30($17)
  173f18: 2d 20 00 00  	move	$4, $zero
  173f1c: dd ff 00 10  	b	0x173e94 <.text+0x73e94>
  173f20: 38 00 20 ae  	sw	$zero, 0x38($17)

## controller mask/callback gate [0x00174120..0x00174204)
  174120: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  174124: 35 00 02 3c  	lui	$2, 0x35
  174128: 00 00 b0 ff  	sd	$16, 0x0($sp)
  17412c: 50 db 42 24  	addiu	$2, $2, -0x24b0 <.text+0xffffffffffefdb50>
  174130: 10 00 bf ff  	sd	$ra, 0x10($sp)
  174134: ff 00 90 30  	andi	$16, $4, 0xff
  174138: 1c 00 50 ac  	sw	$16, 0x1c($2)
  17413c: 18 00 43 8c  	lw	$3, 0x18($2)
  174140: 05 00 60 50  	beqzl	$3, 0x174158 <.text+0x74158>
  174144: 2d 80 00 00  	move	$16, $zero
  174148: 34 00 02 3c  	lui	$2, 0x34
  17414c: 59 55 42 90  	lbu	$2, 0x5559($2)
  174150: 01 00 40 54  	bnezl	$2, 0x174158 <.text+0x74158>
  174154: 2d 80 00 00  	move	$16, $zero
  174158: 05 00 00 12  	beqz	$16, 0x174170 <.text+0x74170>
  17415c: 35 00 02 3c  	lui	$2, 0x35
  174160: 58 db 42 8c  	lw	$2, -0x24a8($2)
  174164: 1b 00 40 10  	beqz	$2, 0x1741d4 <.text+0x741d4>
  174168: 01 00 06 3c  	lui	$6, 0x1
  17416c: 35 00 02 3c  	lui	$2, 0x35
  174170: 2d 20 00 00  	move	$4, $zero
  174174: 58 db 50 ac  	sw	$16, -0x24a8($2)
  174178: e0 00 03 24  	addiu	$3, $zero, 0xe0
  17417c: 35 00 02 3c  	lui	$2, 0x35
  174180: 18 30 83 00  	<unknown>
  174184: 50 db 42 24  	addiu	$2, $2, -0x24b0 <.text+0xffffffffffefdb50>
  174188: 21 28 c2 00  	addu	$5, $6, $2
  17418c: 07 10 90 00  	srav	$2, $16, $4
  174190: 01 00 42 30  	andi	$2, $2, 0x1
  174194: 0c 00 40 10  	beqz	$2, 0x1741c8 <.text+0x741c8>
  174198: 3c 00 02 3c  	lui	$2, 0x3c
  17419c: 3e 00 02 3c  	lui	$2, 0x3e
  1741a0: 48 2e 42 24  	addiu	$2, $2, 0x2e48
  1741a4: d0 00 a2 ac  	sw	$2, 0xd0($5)
  1741a8: 01 00 84 24  	addiu	$4, $4, 0x1
  1741ac: 08 00 82 28  	slti	$2, $4, 0x8
  1741b0: f2 ff 40 14  	bnez	$2, 0x17417c <.text+0x7417c>
  1741b4: e0 00 03 24  	addiu	$3, $zero, 0xe0
  1741b8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1741bc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1741c0: 08 00 e0 03  	jr	$ra
  1741c4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1741c8: 48 2e 42 24  	addiu	$2, $2, 0x2e48
  1741cc: f6 ff 00 10  	b	0x1741a8 <.text+0x741a8>
  1741d0: d0 00 a2 ac  	sw	$2, 0xd0($5)
  1741d4: 3b 00 04 3c  	lui	$4, 0x3b
  1741d8: 2d 28 00 00  	move	$5, $zero
  1741dc: 48 b7 84 24  	addiu	$4, $4, -0x48b8 <.text+0xffffffffffefb748>
  1741e0: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  1741e4: 00 77 c6 34  	ori	$6, $6, 0x7700
  1741e8: 3f 00 04 3c  	lui	$4, 0x3f
  1741ec: 40 00 06 24  	addiu	$6, $zero, 0x40
  1741f0: 2d 28 00 00  	move	$5, $zero
  1741f4: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  1741f8: 70 2e 84 24  	addiu	$4, $4, 0x2e70
  1741fc: dc ff 00 10  	b	0x174170 <.text+0x74170>
  174200: 35 00 02 3c  	lui	$2, 0x35

## month length helper A [0x001825e4..0x00182638)
  1825e4: fe ff 84 24  	addiu	$4, $4, -0x2 <.text+0xffffffffffeffffe>
  1825e8: 0a 00 82 2c  	sltiu	$2, $4, 0xa
  1825ec: 0c 00 40 50  	beqzl	$2, 0x182620 <.text+0x82620>
  1825f0: 1f 00 04 24  	addiu	$4, $zero, 0x1f
  1825f4: 1c 00 03 3c  	lui	$3, 0x1c
  1825f8: 80 10 04 00  	sll	$2, $4, 0x2
  1825fc: ec 87 63 24  	addiu	$3, $3, -0x7814 <.text+0xffffffffffef87ec>
  182600: 21 10 43 00  	addu	$2, $2, $3
  182604: 00 00 42 8c  	lw	$2, 0x0($2)
  182608: 08 00 40 00  	jr	$2
  18260c: 00 00 00 00  	nop
  182610: 03 00 a3 30  	andi	$3, $5, 0x3
  182614: 1d 00 04 24  	addiu	$4, $zero, 0x1d
  182618: 1c 00 02 24  	addiu	$2, $zero, 0x1c
  18261c: 0b 20 43 00  	movn	$4, $2, $3
  182620: 08 00 e0 03  	jr	$ra
  182624: 2d 10 80 00  	move	$2, $4
  182628: fd ff 00 10  	b	0x182620 <.text+0x82620>
  18262c: 1f 00 04 24  	addiu	$4, $zero, 0x1f
  182630: fb ff 00 10  	b	0x182620 <.text+0x82620>
  182634: 1e 00 04 24  	addiu	$4, $zero, 0x1e

## fixed 24-byte record writer [0x001833a4..0x001834f0)
  1833a4: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1833a8: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1833ac: 2d 90 80 00  	move	$18, $4
  1833b0: 1c 00 04 3c  	lui	$4, 0x1c
  1833b4: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1833b8: 70 88 84 24  	addiu	$4, $4, -0x7790 <.text+0xffffffffffef8870>
  1833bc: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1833c0: 49 06 04 0c  	jal	0x101924 <.text+0x1924>
  1833c4: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1833c8: 2d 20 40 00  	move	$4, $2
  1833cc: f0 73 06 0c  	jal	0x19cfc0 <.text+0x9cfc0>
  1833d0: 02 02 05 24  	addiu	$5, $zero, 0x202
  1833d4: 2d 80 40 00  	move	$16, $2
  1833d8: 3f 00 00 06  	bltz	$16, 0x1834d8 <.text+0x834d8>
  1833dc: 2d 10 00 00  	move	$2, $zero
  1833e0: 00 00 a0 a3  	sb	$zero, 0x0($sp)
  1833e4: 2d 88 00 00  	move	$17, $zero
  1833e8: 21 28 51 02  	addu	$5, $18, $17
  1833ec: 2d 20 00 02  	move	$4, $16
  1833f0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1833f4: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  1833f8: 01 00 31 26  	addiu	$17, $17, 0x1
  1833fc: 10 00 22 2a  	slti	$2, $17, 0x10
  183400: fa ff 40 14  	bnez	$2, 0x1833ec <.text+0x833ec>
  183404: 21 28 51 02  	addu	$5, $18, $17
  183408: 10 00 42 92  	lbu	$2, 0x10($18)
  18340c: 2d 20 00 02  	move	$4, $16
  183410: 2d 28 a0 03  	move	$5, $sp
  183414: 01 00 06 24  	addiu	$6, $zero, 0x1
  183418: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  18341c: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183420: 11 00 42 82  	lb	$2, 0x11($18)
  183424: 2d 20 00 02  	move	$4, $16
  183428: 2d 28 a0 03  	move	$5, $sp
  18342c: 01 00 06 24  	addiu	$6, $zero, 0x1
  183430: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  183434: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183438: 12 00 42 92  	lbu	$2, 0x12($18)
  18343c: 2d 20 00 02  	move	$4, $16
  183440: 2d 28 a0 03  	move	$5, $sp
  183444: 01 00 06 24  	addiu	$6, $zero, 0x1
  183448: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  18344c: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183450: 13 00 42 92  	lbu	$2, 0x13($18)
  183454: 2d 20 00 02  	move	$4, $16
  183458: 2d 28 a0 03  	move	$5, $sp
  18345c: 01 00 06 24  	addiu	$6, $zero, 0x1
  183460: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  183464: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183468: 14 00 42 92  	lbu	$2, 0x14($18)
  18346c: 2d 20 00 02  	move	$4, $16
  183470: 2d 28 a0 03  	move	$5, $sp
  183474: 01 00 06 24  	addiu	$6, $zero, 0x1
  183478: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  18347c: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183480: 14 00 42 8e  	lw	$2, 0x14($18)
  183484: 2d 20 00 02  	move	$4, $16
  183488: 2d 28 a0 03  	move	$5, $sp
  18348c: 01 00 06 24  	addiu	$6, $zero, 0x1
  183490: 03 12 02 00  	sra	$2, $2, 0x8
  183494: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  183498: 00 00 a2 a3  	sb	$2, 0x0($sp)
  18349c: 16 00 42 86  	lh	$2, 0x16($18)
  1834a0: 2d 20 00 02  	move	$4, $16
  1834a4: 2d 28 a0 03  	move	$5, $sp
  1834a8: 01 00 06 24  	addiu	$6, $zero, 0x1
  1834ac: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  1834b0: 00 00 a2 a3  	sb	$2, 0x0($sp)
  1834b4: 17 00 42 82  	lb	$2, 0x17($18)
  1834b8: 2d 28 a0 03  	move	$5, $sp
  1834bc: 01 00 06 24  	addiu	$6, $zero, 0x1
  1834c0: 2d 20 00 02  	move	$4, $16
  1834c4: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  1834c8: 00 00 a2 a3  	sb	$2, 0x0($sp)
  1834cc: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  1834d0: 2d 20 00 02  	move	$4, $16
  1834d4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1834d8: 40 00 bf df  	ld	$ra, 0x40($sp)
  1834dc: 30 00 b2 df  	ld	$18, 0x30($sp)
  1834e0: 20 00 b1 df  	ld	$17, 0x20($sp)
  1834e4: 10 00 b0 df  	ld	$16, 0x10($sp)
  1834e8: 08 00 e0 03  	jr	$ra
  1834ec: 50 00 bd 27  	addiu	$sp, $sp, 0x50

## fixed 24-byte record reader [0x001834f0..0x00183658)
  1834f0: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1834f4: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1834f8: 2d 88 80 00  	move	$17, $4
  1834fc: 1c 00 04 3c  	lui	$4, 0x1c
  183500: 40 00 bf ff  	sd	$ra, 0x40($sp)
  183504: 70 88 84 24  	addiu	$4, $4, -0x7790 <.text+0xffffffffffef8870>
  183508: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18350c: 49 06 04 0c  	jal	0x101924 <.text+0x1924>
  183510: 30 00 b2 ff  	sd	$18, 0x30($sp)
  183514: 2d 20 40 00  	move	$4, $2
  183518: f0 73 06 0c  	jal	0x19cfc0 <.text+0x9cfc0>
  18351c: 01 00 05 24  	addiu	$5, $zero, 0x1
  183520: 2d 80 40 00  	move	$16, $2
  183524: 46 00 00 06  	bltz	$16, 0x183640 <.text+0x83640>
  183528: 2d 10 00 00  	move	$2, $zero
  18352c: 2d 90 00 00  	move	$18, $zero
  183530: 21 28 32 02  	addu	$5, $17, $18
  183534: 2d 20 00 02  	move	$4, $16
  183538: 01 00 06 24  	addiu	$6, $zero, 0x1
  18353c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  183540: 01 00 52 26  	addiu	$18, $18, 0x1
  183544: 10 00 42 2a  	slti	$2, $18, 0x10
  183548: fa ff 40 54  	bnezl	$2, 0x183534 <.text+0x83534>
  18354c: 21 28 32 02  	addu	$5, $17, $18
  183550: 2d 20 00 02  	move	$4, $16
  183554: 2d 28 a0 03  	move	$5, $sp
  183558: 01 00 06 24  	addiu	$6, $zero, 0x1
  18355c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  183560: 00 00 a0 a3  	sb	$zero, 0x0($sp)
  183564: 2d 28 a0 03  	move	$5, $sp
  183568: 00 00 a2 93  	lbu	$2, 0x0($sp)
  18356c: 01 00 06 24  	addiu	$6, $zero, 0x1
  183570: 2d 20 00 02  	move	$4, $16
  183574: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  183578: 10 00 22 a6  	sh	$2, 0x10($17)
  18357c: 2d 20 00 02  	move	$4, $16
  183580: 00 00 a3 93  	lbu	$3, 0x0($sp)
  183584: 01 00 06 24  	addiu	$6, $zero, 0x1
  183588: 10 00 22 96  	lhu	$2, 0x10($17)
  18358c: 12 00 25 26  	addiu	$5, $17, 0x12
  183590: 00 1a 03 00  	sll	$3, $3, 0x8
  183594: 25 10 43 00  	or	$2, $2, $3
  183598: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18359c: 10 00 22 a6  	sh	$2, 0x10($17)
  1835a0: 2d 20 00 02  	move	$4, $16
  1835a4: 01 00 06 24  	addiu	$6, $zero, 0x1
  1835a8: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1835ac: 13 00 25 26  	addiu	$5, $17, 0x13
  1835b0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1835b4: 2d 20 00 02  	move	$4, $16
  1835b8: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1835bc: 2d 28 a0 03  	move	$5, $sp
  1835c0: 2d 28 a0 03  	move	$5, $sp
  1835c4: 00 00 a2 93  	lbu	$2, 0x0($sp)
  1835c8: 01 00 06 24  	addiu	$6, $zero, 0x1
  1835cc: 2d 20 00 02  	move	$4, $16
  1835d0: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1835d4: 14 00 22 ae  	sw	$2, 0x14($17)
  1835d8: 2d 28 a0 03  	move	$5, $sp
  1835dc: 00 00 a3 93  	lbu	$3, 0x0($sp)
  1835e0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1835e4: 14 00 22 8e  	lw	$2, 0x14($17)
  1835e8: 2d 20 00 02  	move	$4, $16
  1835ec: 00 1a 03 00  	sll	$3, $3, 0x8
  1835f0: 25 10 43 00  	or	$2, $2, $3
  1835f4: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1835f8: 14 00 22 ae  	sw	$2, 0x14($17)
  1835fc: 2d 28 a0 03  	move	$5, $sp
  183600: 00 00 a3 93  	lbu	$3, 0x0($sp)
  183604: 01 00 06 24  	addiu	$6, $zero, 0x1
  183608: 14 00 22 8e  	lw	$2, 0x14($17)
  18360c: 2d 20 00 02  	move	$4, $16
  183610: 00 1c 03 00  	sll	$3, $3, 0x10
  183614: 25 10 43 00  	or	$2, $2, $3
  183618: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18361c: 14 00 22 ae  	sw	$2, 0x14($17)
  183620: 14 00 23 8e  	lw	$3, 0x14($17)
  183624: 00 00 a2 93  	lbu	$2, 0x0($sp)
  183628: 2d 20 00 02  	move	$4, $16
  18362c: 00 16 02 00  	sll	$2, $2, 0x18
  183630: 25 18 62 00  	or	$3, $3, $2
  183634: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  183638: 14 00 23 ae  	sw	$3, 0x14($17)
  18363c: 01 00 02 24  	addiu	$2, $zero, 0x1
  183640: 40 00 bf df  	ld	$ra, 0x40($sp)
  183644: 30 00 b2 df  	ld	$18, 0x30($sp)
  183648: 20 00 b1 df  	ld	$17, 0x20($sp)
  18364c: 10 00 b0 df  	ld	$16, 0x10($sp)
  183650: 08 00 e0 03  	jr	$ra
  183654: 50 00 bd 27  	addiu	$sp, $sp, 0x50

## weekday/date helper [0x001836d0..0x00183778)
  1836d0: 42 00 02 3c  	lui	$2, 0x42
  1836d4: 0a 00 08 24  	addiu	$8, $zero, 0xa
  1836d8: 58 38 42 24  	addiu	$2, $2, 0x3858
  1836dc: 07 00 0a 24  	addiu	$10, $zero, 0x7
  1836e0: 0c 00 44 90  	lbu	$4, 0xc($2)
  1836e4: 0b 00 43 90  	lbu	$3, 0xb($2)
  1836e8: 18 30 88 00  	<unknown>
  1836ec: 0d 00 45 90  	lbu	$5, 0xd($2)
  1836f0: 0a 00 44 90  	lbu	$4, 0xa($2)
  1836f4: 08 00 47 90  	lbu	$7, 0x8($2)
  1836f8: 21 48 c3 00  	addu	$9, $6, $3
  1836fc: 0d 00 83 2c  	sltiu	$3, $4, 0xd
  183700: 09 00 46 90  	lbu	$6, 0x9($2)
  183704: 01 00 02 24  	addiu	$2, $zero, 0x1
  183708: 0a 20 43 00  	movz	$4, $2, $3
  18370c: 64 00 02 24  	addiu	$2, $zero, 0x64
  183710: 18 18 a2 00  	<unknown>
  183714: 18 10 c8 00  	<unknown>
  183718: 21 30 47 00  	addu	$6, $2, $7
  18371c: 21 28 69 00  	addu	$5, $3, $9
  183720: 42 00 02 3c  	lui	$2, 0x42
  183724: 80 18 04 00  	sll	$3, $4, 0x2
  183728: 78 38 42 24  	addiu	$2, $2, 0x3878
  18372c: 7c fc a9 24  	addiu	$9, $5, -0x384 <.text+0xffffffffffeffc7c>
  183730: 21 18 62 00  	addu	$3, $3, $2
  183734: 03 00 87 2c  	sltiu	$7, $4, 0x3
  183738: fc ff 63 8c  	lw	$3, -0x4($3)
  18373c: 82 10 09 00  	srl	$2, $9, 0x2
  183740: 21 10 22 01  	addu	$2, $9, $2
  183744: 03 00 24 31  	andi	$4, $9, 0x3
  183748: 21 10 43 00  	addu	$2, $2, $3
  18374c: 21 10 46 00  	addu	$2, $2, $6
  183750: fe ff 43 24  	addiu	$3, $2, -0x2 <.text+0xffffffffffeffffe>
  183754: 02 00 80 14  	bnez	$4, 0x183760 <.text+0x83760>
  183758: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  18375c: 0b 10 67 00  	movn	$2, $3, $7
  183760: 1b 00 4a 00  	divu	$zero, $2, $10
  183764: 01 00 40 51  	beqzl	$10, 0x18376c <.text+0x8376c>
  183768: cd 01 00 00  	break	0x0, 0x7
  18376c: 10 10 00 00  	mfhi	$2
  183770: 08 00 e0 03  	jr	$ra
  183774: 00 00 00 00  	nop

## month length helper B [0x00183778..0x001837cc)
  183778: fe ff 84 24  	addiu	$4, $4, -0x2 <.text+0xffffffffffeffffe>
  18377c: 0a 00 82 2c  	sltiu	$2, $4, 0xa
  183780: 0c 00 40 50  	beqzl	$2, 0x1837b4 <.text+0x837b4>
  183784: 1f 00 04 24  	addiu	$4, $zero, 0x1f
  183788: 1c 00 03 3c  	lui	$3, 0x1c
  18378c: 80 10 04 00  	sll	$2, $4, 0x2
  183790: 78 88 63 24  	addiu	$3, $3, -0x7788 <.text+0xffffffffffef8878>
  183794: 21 10 43 00  	addu	$2, $2, $3
  183798: 00 00 42 8c  	lw	$2, 0x0($2)
  18379c: 08 00 40 00  	jr	$2
  1837a0: 00 00 00 00  	nop
  1837a4: 03 00 a3 30  	andi	$3, $5, 0x3
  1837a8: 1d 00 04 24  	addiu	$4, $zero, 0x1d
  1837ac: 1c 00 02 24  	addiu	$2, $zero, 0x1c
  1837b0: 0b 20 43 00  	movn	$4, $2, $3
  1837b4: 08 00 e0 03  	jr	$ra
  1837b8: 2d 10 80 00  	move	$2, $4
  1837bc: fd ff 00 10  	b	0x1837b4 <.text+0x837b4>
  1837c0: 1f 00 04 24  	addiu	$4, $zero, 0x1f
  1837c4: fb ff 00 10  	b	0x1837b4 <.text+0x837b4>
  1837c8: 1e 00 04 24  	addiu	$4, $zero, 0x1e

## _Unwind_DeleteException [0x001a5c58..0x001a5c7c)
  1a5c58: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a5c5c: 2d 28 80 00  	move	$5, $4
  1a5c60: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a5c64: 08 00 a2 8c  	lw	$2, 0x8($5)
  1a5c68: 09 f8 40 00  	jalr	$2
  1a5c6c: 01 00 04 24  	addiu	$4, $zero, 0x1
  1a5c70: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a5c74: 08 00 e0 03  	jr	$ra
  1a5c78: 10 00 bd 27  	addiu	$sp, $sp, 0x10

## signed DI to double helper [0x001a7580..0x001a7638)
  1a7580: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a7584: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a7588: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a758c: 2d 80 80 00  	move	$16, $4
  1a7590: e0 81 11 34  	ori	$17, $zero, 0x81e0
  1a7594: fc 8b 11 00  	dsll32	$17, $17, 0xf
  1a7598: 3f 20 04 00  	dsra32	$4, $4, 0x0
  1a759c: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a75a0: cc 8e 06 0c  	jal	0x1a3b30 <.text+0xa3b30>
  1a75a4: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a75a8: 2d 20 40 00  	move	$4, $2
  1a75ac: 2d 28 20 02  	move	$5, $17
  1a75b0: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a75b4: 00 00 00 00  	nop
  1a75b8: 2d 28 20 02  	move	$5, $17
  1a75bc: 2d 20 40 00  	move	$4, $2
  1a75c0: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a75c4: 00 00 00 00  	nop
  1a75c8: 2d 90 40 00  	move	$18, $2
  1a75cc: ff ff 02 3c  	lui	$2, 0xffff
  1a75d0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a75d4: 24 80 02 02  	and	$16, $16, $2
  1a75d8: 3c 80 10 00  	dsll32	$16, $16, 0x0
  1a75dc: 3f 80 10 00  	dsra32	$16, $16, 0x0
  1a75e0: cc 8e 06 0c  	jal	0x1a3b30 <.text+0xa3b30>
  1a75e4: 2d 20 00 02  	move	$4, $16
  1a75e8: 0b 00 00 06  	bltz	$16, 0x1a7618 <.text+0xa7618>
  1a75ec: 00 00 00 00  	nop
  1a75f0: 2d 20 40 02  	move	$4, $18
  1a75f4: 2d 28 40 00  	move	$5, $2
  1a75f8: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a75fc: 00 00 00 00  	nop
  1a7600: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a7604: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a7608: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a760c: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a7610: 08 00 e0 03  	jr	$ra
  1a7614: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a7618: e0 83 05 34  	ori	$5, $zero, 0x83e0
  1a761c: fc 2b 05 00  	dsll32	$5, $5, 0xf
  1a7620: 2d 20 40 00  	move	$4, $2
  1a7624: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a7628: 00 00 00 00  	nop
  1a762c: f0 ff 00 10  	b	0x1a75f0 <.text+0xa75f0>
		...

## LSDA header parser [0x001a9488..0x001a9588)
  1a9488: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  1a948c: 2d 18 00 00  	move	$3, $zero
  1a9490: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a9494: 2d 90 c0 00  	move	$18, $6
  1a9498: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a949c: 2d 88 80 00  	move	$17, $4
  1a94a0: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a94a4: 2d 80 a0 00  	move	$16, $5
  1a94a8: 50 00 bf ff  	sd	$ra, 0x50($sp)
  1a94ac: 04 00 80 10  	beqz	$4, 0x1a94c0 <.text+0xa94c0>
  1a94b0: 40 00 b3 ff  	sd	$19, 0x40($sp)
  1a94b4: 3c 90 06 0c  	jal	0x1a40f0 <.text+0xa40f0>
  1a94b8: 00 00 00 00  	nop
  1a94bc: 2d 18 40 00  	move	$3, $2
  1a94c0: 00 00 43 ae  	sw	$3, 0x0($18)
  1a94c4: 2d 28 20 02  	move	$5, $17
  1a94c8: ff 00 02 24  	addiu	$2, $zero, 0xff
  1a94cc: 04 00 53 26  	addiu	$19, $18, 0x4
  1a94d0: 00 00 11 92  	lbu	$17, 0x0($16)
  1a94d4: 01 00 10 26  	addiu	$16, $16, 0x1
  1a94d8: 29 00 22 12  	beq	$17, $2, 0x1a9580 <.text+0xa9580>
  1a94dc: 2d 20 20 02  	move	$4, $17
  1a94e0: 6c a4 06 0c  	jal	0x1a91b0 <.text+0xa91b0>
  1a94e4: 00 00 00 00  	nop
  1a94e8: 2d 38 60 02  	move	$7, $19
  1a94ec: 2d 30 00 02  	move	$6, $16
  1a94f0: 2d 20 20 02  	move	$4, $17
  1a94f4: c2 a4 06 0c  	jal	0x1a9308 <.text+0xa9308>
  1a94f8: 2d 28 40 00  	move	$5, $2
  1a94fc: 2d 80 40 00  	move	$16, $2
  1a9500: 00 00 03 92  	lbu	$3, 0x0($16)
  1a9504: ff 00 02 24  	addiu	$2, $zero, 0xff
  1a9508: 01 00 10 26  	addiu	$16, $16, 0x1
  1a950c: 2d 28 a0 03  	move	$5, $sp
  1a9510: 14 00 43 a2  	sb	$3, 0x14($18)
  1a9514: 18 00 43 10  	beq	$2, $3, 0x1a9578 <.text+0xa9578>
  1a9518: 2d 20 00 02  	move	$4, $16
  1a951c: 9a a4 06 0c  	jal	0x1a9268 <.text+0xa9268>
  1a9520: 00 00 00 00  	nop
  1a9524: 2d 80 40 00  	move	$16, $2
  1a9528: 00 00 a2 8f  	lw	$2, 0x0($sp)
  1a952c: 21 10 02 02  	addu	$2, $16, $2
  1a9530: 0c 00 42 ae  	sw	$2, 0xc($18)
  1a9534: 00 00 02 92  	lbu	$2, 0x0($16)
  1a9538: 2d 28 a0 03  	move	$5, $sp
  1a953c: 01 00 10 26  	addiu	$16, $16, 0x1
  1a9540: 15 00 42 a2  	sb	$2, 0x15($18)
  1a9544: 9a a4 06 0c  	jal	0x1a9268 <.text+0xa9268>
  1a9548: 2d 20 00 02  	move	$4, $16
  1a954c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a9550: 00 00 a3 8f  	lw	$3, 0x0($sp)
  1a9554: 50 00 bf df  	ld	$ra, 0x50($sp)
  1a9558: 21 18 43 00  	addu	$3, $2, $3
  1a955c: 40 00 b3 df  	ld	$19, 0x40($sp)
  1a9560: 10 00 43 ae  	sw	$3, 0x10($18)
  1a9564: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a9568: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a956c: 08 00 e0 03  	jr	$ra
  1a9570: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  1a9574: 00 00 00 00  	nop
  1a9578: ee ff 00 10  	b	0x1a9534 <.text+0xa9534>
  1a957c: 0c 00 40 ae  	sw	$zero, 0xc($18)
  1a9580: df ff 00 10  	b	0x1a9500 <.text+0xa9500>
  1a9584: 04 00 43 ae  	sw	$3, 0x4($18)

## type-table entry decoder [0x001a9588..0x001a95f8)
  1a9588: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a958c: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a9590: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a9594: 2d 88 a0 00  	move	$17, $5
  1a9598: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a959c: 2d 80 80 00  	move	$16, $4
  1a95a0: 4e a4 06 0c  	jal	0x1a9138 <.text+0xa9138>
  1a95a4: 14 00 84 90  	lbu	$4, 0x14($4)
  1a95a8: 2d 20 20 02  	move	$4, $17
  1a95ac: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a95b0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a95b4: c8 86 06 0c  	jal	0x1a1b20 <.text+0xa1b20>
  1a95b8: 2d 28 40 00  	move	$5, $2
  1a95bc: 08 00 05 8e  	lw	$5, 0x8($16)
  1a95c0: 0c 00 06 8e  	lw	$6, 0xc($16)
  1a95c4: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a95c8: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a95cc: 14 00 04 92  	lbu	$4, 0x14($16)
  1a95d0: 2d 38 a0 03  	move	$7, $sp
  1a95d4: c2 a4 06 0c  	jal	0x1a9308 <.text+0xa9308>
  1a95d8: 23 30 c2 00  	subu	$6, $6, $2
  1a95dc: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a95e0: 00 00 a2 8f  	lw	$2, 0x0($sp)
  1a95e4: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a95e8: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a95ec: 08 00 e0 03  	jr	$ra
  1a95f0: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a95f4: 00 00 00 00  	nop

## adjusted RTTI pointer helper [0x001a95f8..0x001a9698)
  1a95f8: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a95fc: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a9600: 2d 90 c0 00  	move	$18, $6
  1a9604: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a9608: 2d 88 80 00  	move	$17, $4
  1a960c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a9610: 2d 20 a0 00  	move	$4, $5
  1a9614: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1a9618: 00 00 c2 8c  	lw	$2, 0x0($6)
  1a961c: 00 00 a3 8c  	lw	$3, 0x0($5)
  1a9620: 00 00 a2 af  	sw	$2, 0x0($sp)
  1a9624: 08 00 62 8c  	lw	$2, 0x8($3)
  1a9628: 09 f8 40 00  	jalr	$2
  1a962c: 2d 80 a0 00  	move	$16, $5
  1a9630: 2d 30 a0 03  	move	$6, $sp
  1a9634: 01 00 07 24  	addiu	$7, $zero, 0x1
  1a9638: 2d 20 20 02  	move	$4, $17
  1a963c: 04 00 40 10  	beqz	$2, 0x1a9650 <.text+0xa9650>
  1a9640: 2d 28 00 02  	move	$5, $16
  1a9644: 00 00 a2 8f  	lw	$2, 0x0($sp)
  1a9648: 00 00 43 8c  	lw	$3, 0x0($2)
  1a964c: 00 00 a3 af  	sw	$3, 0x0($sp)
  1a9650: 00 00 23 8e  	lw	$3, 0x0($17)
  1a9654: 10 00 62 8c  	lw	$2, 0x10($3)
  1a9658: 09 f8 40 00  	jalr	$2
  1a965c: 00 00 00 00  	nop
  1a9660: 0b 00 40 10  	beqz	$2, 0x1a9690 <.text+0xa9690>
  1a9664: 01 00 03 24  	addiu	$3, $zero, 0x1
  1a9668: 00 00 a2 8f  	lw	$2, 0x0($sp)
  1a966c: 00 00 42 ae  	sw	$2, 0x0($18)
  1a9670: 40 00 bf df  	ld	$ra, 0x40($sp)
  1a9674: 2d 10 60 00  	move	$2, $3
  1a9678: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a967c: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a9680: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a9684: 08 00 e0 03  	jr	$ra
  1a9688: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a968c: 00 00 00 00  	nop
  1a9690: f7 ff 00 10  	b	0x1a9670 <.text+0xa9670>
  1a9694: 2d 18 00 00  	move	$3, $zero

## exception-spec type-list walker [0x001a9698..0x001a9728)
  1a9698: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a969c: 3c 38 07 00  	dsll32	$7, $7, 0x0
  1a96a0: 3f 38 07 00  	dsra32	$7, $7, 0x0
  1a96a4: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a96a8: 2d 90 80 00  	move	$18, $4
  1a96ac: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a96b0: 2d 88 a0 00  	move	$17, $5
  1a96b4: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a96b8: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1a96bc: 0c 00 82 8c  	lw	$2, 0xc($4)
  1a96c0: 08 00 a6 af  	sw	$6, 0x8($sp)
  1a96c4: 23 10 47 00  	subu	$2, $2, $7
  1a96c8: ff ff 50 24  	addiu	$16, $2, -0x1 <.text+0xffffffffffefffff>
  1a96cc: 2d 20 00 02  	move	$4, $16
  1a96d0: 9a a4 06 0c  	jal	0x1a9268 <.text+0xa9268>
  1a96d4: 2d 28 a0 03  	move	$5, $sp
  1a96d8: 00 00 a5 df  	ld	$5, 0x0($sp)
  1a96dc: 2d 80 40 00  	move	$16, $2
  1a96e0: 0a 00 a0 10  	beqz	$5, 0x1a970c <.text+0xa970c>
  1a96e4: 2d 10 00 00  	move	$2, $zero
  1a96e8: 62 a5 06 0c  	jal	0x1a9588 <.text+0xa9588>
  1a96ec: 2d 20 40 02  	move	$4, $18
  1a96f0: 08 00 a6 27  	addiu	$6, $sp, 0x8
  1a96f4: 2d 20 40 00  	move	$4, $2
  1a96f8: 7e a5 06 0c  	jal	0x1a95f8 <.text+0xa95f8>
  1a96fc: 2d 28 20 02  	move	$5, $17
  1a9700: f3 ff 40 10  	beqz	$2, 0x1a96d0 <.text+0xa96d0>
  1a9704: 2d 20 00 02  	move	$4, $16
  1a9708: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a970c: 40 00 bf df  	ld	$ra, 0x40($sp)
  1a9710: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a9714: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a9718: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a971c: 08 00 e0 03  	jr	$ra
  1a9720: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a9724: 00 00 00 00  	nop
