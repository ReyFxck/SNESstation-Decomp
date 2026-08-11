  18a6d4: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  18a6d8: 36 00 02 3c  	lui	$2, 0x36
  18a6dc: 80 00 be ff  	sd	$fp, 0x80($sp)
  18a6e0: ff 01 83 30  	andi	$3, $4, 0x1ff
  18a6e4: 70 00 b7 ff  	sd	$23, 0x70($sp)
  18a6e8: 00 01 63 2c  	sltiu	$3, $3, 0x100
  18a6ec: 60 00 b6 ff  	sd	$22, 0x60($sp)
  18a6f0: 2d b8 a0 00  	move	$23, $5
  18a6f4: 50 00 b5 ff  	sd	$21, 0x50($sp)
  18a6f8: 2d f0 e0 00  	move	$fp, $7
  18a6fc: 30 00 b3 ff  	sd	$19, 0x30($sp)
  18a700: 2d a8 00 01  	move	$21, $8
  18a704: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18a708: 2d 98 80 00  	move	$19, $4
  18a70c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18a710: 2d 90 c0 00  	move	$18, $6
  18a714: 90 00 bf ff  	sd	$ra, 0x90($sp)
  18a718: 50 d4 51 24  	addiu	$17, $2, -0x2bb0 <.text+0xffffffffffefd450>
  18a71c: 40 00 b4 ff  	sd	$20, 0x40($sp)
  18a720: ff 03 82 30  	andi	$2, $4, 0x3ff
  18a724: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18a728: 2d b0 20 01  	move	$22, $9
  18a72c: 08 00 2a 8e  	lw	$10, 0x8($17)
  18a730: 0c 00 24 8e  	lw	$4, 0xc($17)
  18a734: 04 10 42 01  	sllv	$2, $2, $10
  18a738: 03 00 60 14  	bnez	$3, 0x18a748 <.text+0x8a748>
  18a73c: 21 28 82 00  	addu	$5, $4, $2
  18a740: 10 00 22 8e  	lw	$2, 0x10($17)
  18a744: 21 28 a2 00  	addu	$5, $5, $2
  18a748: ff ff a5 30  	andi	$5, $5, 0xffff
  18a74c: 28 00 22 8e  	lw	$2, 0x28($17)
  18a750: 06 80 45 01  	srlv	$16, $5, $10
  18a754: 24 00 24 8e  	lw	$4, 0x24($17)
  18a758: 21 10 50 00  	addu	$2, $2, $16
  18a75c: 80 19 10 00  	sll	$3, $16, 0x6
  18a760: 00 00 42 90  	lbu	$2, 0x0($2)
  18a764: 8e 01 40 10  	beqz	$2, 0x18ada0 <.text+0x8ada0>
  18a768: 21 a0 83 00  	addu	$20, $4, $3
  18a76c: 28 00 22 8e  	lw	$2, 0x28($17)
  18a770: 21 10 50 00  	addu	$2, $2, $16
  18a774: 00 00 43 90  	lbu	$3, 0x0($2)
  18a778: 02 00 02 24  	addiu	$2, $zero, 0x2
  18a77c: 5e 00 62 10  	beq	$3, $2, 0x18a8f8 <.text+0x8a8f8>
  18a780: 90 00 bf df  	ld	$ra, 0x90($sp)
  18a784: 2c 00 22 92  	lbu	$2, 0x2c($17)
  18a788: 7b 01 40 50  	beqzl	$2, 0x18ad78 <.text+0x8ad78>
  18a78c: 20 00 22 8e  	lw	$2, 0x20($17)
  18a790: 36 00 02 3c  	lui	$2, 0x36
  18a794: 6f c2 42 90  	lbu	$2, -0x3d91($2)
  18a798: 73 01 40 14  	bnez	$2, 0x18ad68 <.text+0x8ad68>
  18a79c: 00 00 00 00  	nop
  18a7a0: 20 00 22 8e  	lw	$2, 0x20($17)
  18a7a4: 82 1a 13 00  	srl	$3, $19, 0xa
  18a7a8: 24 18 62 00  	and	$3, $3, $2
  18a7ac: 3f 00 02 3c  	lui	$2, 0x3f
  18a7b0: 40 1a 03 00  	sll	$3, $3, 0x9
  18a7b4: 80 2f 42 24  	addiu	$2, $2, 0x2f80
  18a7b8: 21 18 62 00  	addu	$3, $3, $2
  18a7bc: 40 28 17 00  	sll	$5, $23, 0x1
  18a7c0: 36 00 02 3c  	lui	$2, 0x36
  18a7c4: 00 c0 64 32  	andi	$4, $19, 0xc000
  18a7c8: c4 d4 43 ac  	sw	$3, -0x2b3c($2)
  18a7cc: 36 00 02 3c  	lui	$2, 0x36
  18a7d0: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18a7d4: 3c 00 e2 8c  	lw	$2, 0x3c($7)
  18a7d8: 08 00 e3 8c  	lw	$3, 0x8($7)
  18a7dc: 21 60 45 00  	addu	$12, $2, $5
  18a7e0: 5e 00 80 14  	bnez	$4, 0x18a95c <.text+0x8a95c>
  18a7e4: 21 58 77 00  	addu	$11, $3, $23
  18a7e8: 21 10 95 02  	addu	$2, $20, $21
  18a7ec: 21 10 52 00  	addu	$2, $2, $18
  18a7f0: 00 00 46 90  	lbu	$6, 0x0($2)
  18a7f4: 3f 00 c0 10  	beqz	$6, 0x18a8f4 <.text+0x8a8f4>
  18a7f8: 40 10 06 00  	sll	$2, $6, 0x1
  18a7fc: 44 00 e3 8c  	lw	$3, 0x44($7)
  18a800: 2d 48 c0 02  	move	$9, $22
  18a804: 21 10 43 00  	addu	$2, $2, $3
  18a808: 3a 00 c0 12  	beqz	$22, 0x18a8f4 <.text+0x8a8f4>
  18a80c: 00 00 46 94  	lhu	$6, 0x0($2)
  18a810: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18a814: 30 00 00 05  	bltz	$8, 0x18a8d8 <.text+0x8a8d8>
  18a818: 40 10 08 00  	sll	$2, $8, 0x1
  18a81c: 21 70 4c 00  	addu	$14, $2, $12
  18a820: 36 00 02 3c  	lui	$2, 0x36
  18a824: 21 80 68 01  	addu	$16, $11, $8
  18a828: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18a82c: 2d 20 c0 00  	move	$4, $6
  18a830: 00 00 02 92  	lbu	$2, 0x0($16)
  18a834: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18a838: 2b 10 43 00  	sltu	$2, $2, $3
  18a83c: 23 00 40 10  	beqz	$2, 0x18a8cc <.text+0x8a8cc>
  18a840: 2d 88 c0 01  	move	$17, $14
  18a844: 48 00 e2 8c  	lw	$2, 0x48($7)
  18a848: 21 10 02 01  	addu	$2, $8, $2
  18a84c: 21 10 62 01  	addu	$2, $11, $2
  18a850: 00 00 43 90  	lbu	$3, 0x0($2)
  18a854: 1a 00 60 50  	beqzl	$3, 0x18a8c0 <.text+0x8a8c0>
  18a858: 00 00 24 a6  	sh	$4, 0x0($17)
  18a85c: de fb 0f 24  	addiu	$15, $zero, -0x422 <.text+0xffffffffffeffbde>
  18a860: 01 00 02 24  	addiu	$2, $zero, 0x1
  18a864: de fb 0a 24  	addiu	$10, $zero, -0x422 <.text+0xffffffffffeffbde>
  18a868: 2e 00 62 10  	beq	$3, $2, 0x18a924 <.text+0x8a924>
  18a86c: 24 68 cf 00  	and	$13, $6, $15
  18a870: 14 00 e2 8c  	lw	$2, 0x14($7)
  18a874: 18 00 e5 8c  	lw	$5, 0x18($7)
  18a878: 21 10 02 01  	addu	$2, $8, $2
  18a87c: 40 10 02 00  	sll	$2, $2, 0x1
  18a880: 21 10 4c 00  	addu	$2, $2, $12
  18a884: 00 00 44 94  	lhu	$4, 0x0($2)
  18a888: ff ff 83 30  	andi	$3, $4, 0xffff
  18a88c: 26 20 c4 00  	xor	$4, $6, $4
  18a890: 24 10 6f 00  	and	$2, $3, $15
  18a894: 21 04 84 30  	andi	$4, $4, 0x421
  18a898: 24 18 c3 00  	and	$3, $6, $3
  18a89c: 21 10 a2 01  	addu	$2, $13, $2
  18a8a0: 21 04 63 30  	andi	$3, $3, 0x421
  18a8a4: 43 10 02 00  	sra	$2, $2, 0x1
  18a8a8: 21 10 43 00  	addu	$2, $2, $3
  18a8ac: 40 10 02 00  	sll	$2, $2, 0x1
  18a8b0: 21 10 45 00  	addu	$2, $2, $5
  18a8b4: 00 00 42 94  	lhu	$2, 0x0($2)
  18a8b8: 25 20 82 00  	or	$4, $4, $2
  18a8bc: 00 00 24 a6  	sh	$4, 0x0($17)
  18a8c0: 36 00 02 3c  	lui	$2, 0x36
  18a8c4: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18a8c8: 00 00 02 a2  	sb	$2, 0x0($16)
  18a8cc: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18a8d0: d3 ff 01 05  	bgez	$8, 0x18a820 <.text+0x8a820>
  18a8d4: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18a8d8: 36 00 02 3c  	lui	$2, 0x36
  18a8dc: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18a8e0: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18a8e4: 40 18 02 00  	sll	$3, $2, 0x1
  18a8e8: 21 58 62 01  	addu	$11, $11, $2
  18a8ec: c8 ff 20 15  	bnez	$9, 0x18a810 <.text+0x8a810>
  18a8f0: 21 60 83 01  	addu	$12, $12, $3
  18a8f4: 90 00 bf df  	ld	$ra, 0x90($sp)
  18a8f8: 80 00 be df  	ld	$fp, 0x80($sp)
  18a8fc: 70 00 b7 df  	ld	$23, 0x70($sp)
  18a900: 60 00 b6 df  	ld	$22, 0x60($sp)
  18a904: 50 00 b5 df  	ld	$21, 0x50($sp)
  18a908: 40 00 b4 df  	ld	$20, 0x40($sp)
  18a90c: 30 00 b3 df  	ld	$19, 0x30($sp)
  18a910: 20 00 b2 df  	ld	$18, 0x20($sp)
  18a914: 10 00 b1 df  	ld	$17, 0x10($sp)
  18a918: 00 00 b0 df  	ld	$16, 0x0($sp)
  18a91c: 08 00 e0 03  	jr	$ra
  18a920: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  18a924: 50 00 e3 8c  	lw	$3, 0x50($7)
  18a928: 50 00 e4 94  	lhu	$4, 0x50($7)
  18a92c: 24 10 6a 00  	and	$2, $3, $10
  18a930: 18 00 e5 8c  	lw	$5, 0x18($7)
  18a934: 24 18 c3 00  	and	$3, $6, $3
  18a938: 21 10 a2 01  	addu	$2, $13, $2
  18a93c: 21 04 63 30  	andi	$3, $3, 0x421
  18a940: 42 10 02 00  	srl	$2, $2, 0x1
  18a944: 21 10 43 00  	addu	$2, $2, $3
  18a948: 26 20 c4 00  	xor	$4, $6, $4
  18a94c: 40 10 02 00  	sll	$2, $2, 0x1
  18a950: 21 04 84 30  	andi	$4, $4, 0x421
  18a954: d7 ff 00 10  	b	0x18a8b4 <.text+0x8a8b4>
  18a958: 21 10 45 00  	addu	$2, $2, $5
  18a95c: 00 80 62 32  	andi	$2, $19, 0x8000
  18a960: 57 00 40 14  	bnez	$2, 0x18aac0 <.text+0x8aac0>
  18a964: 00 40 62 32  	andi	$2, $19, 0x4000
  18a968: 07 00 03 24  	addiu	$3, $zero, 0x7
  18a96c: 21 10 95 02  	addu	$2, $20, $21
  18a970: 23 90 72 00  	subu	$18, $3, $18
  18a974: 21 10 52 00  	addu	$2, $2, $18
  18a978: 00 00 46 90  	lbu	$6, 0x0($2)
  18a97c: dd ff c0 10  	beqz	$6, 0x18a8f4 <.text+0x8a8f4>
  18a980: 40 10 06 00  	sll	$2, $6, 0x1
  18a984: 36 00 03 3c  	lui	$3, 0x36
  18a988: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18a98c: 2d 48 c0 02  	move	$9, $22
  18a990: 21 10 43 00  	addu	$2, $2, $3
  18a994: d7 ff c0 12  	beqz	$22, 0x18a8f4 <.text+0x8a8f4>
  18a998: 00 00 46 94  	lhu	$6, 0x0($2)
  18a99c: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18a9a0: 30 00 00 05  	bltz	$8, 0x18aa64 <.text+0x8aa64>
  18a9a4: 40 10 08 00  	sll	$2, $8, 0x1
  18a9a8: 21 70 4c 00  	addu	$14, $2, $12
  18a9ac: 36 00 02 3c  	lui	$2, 0x36
  18a9b0: 21 80 68 01  	addu	$16, $11, $8
  18a9b4: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18a9b8: 2d 20 c0 00  	move	$4, $6
  18a9bc: 00 00 02 92  	lbu	$2, 0x0($16)
  18a9c0: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18a9c4: 2b 10 43 00  	sltu	$2, $2, $3
  18a9c8: 23 00 40 10  	beqz	$2, 0x18aa58 <.text+0x8aa58>
  18a9cc: 2d 88 c0 01  	move	$17, $14
  18a9d0: 48 00 e2 8c  	lw	$2, 0x48($7)
  18a9d4: 21 10 02 01  	addu	$2, $8, $2
  18a9d8: 21 10 62 01  	addu	$2, $11, $2
  18a9dc: 00 00 43 90  	lbu	$3, 0x0($2)
  18a9e0: 1a 00 60 50  	beqzl	$3, 0x18aa4c <.text+0x8aa4c>
  18a9e4: 00 00 24 a6  	sh	$4, 0x0($17)
  18a9e8: de fb 0f 24  	addiu	$15, $zero, -0x422 <.text+0xffffffffffeffbde>
  18a9ec: 01 00 02 24  	addiu	$2, $zero, 0x1
  18a9f0: de fb 0a 24  	addiu	$10, $zero, -0x422 <.text+0xffffffffffeffbde>
  18a9f4: 24 00 62 10  	beq	$3, $2, 0x18aa88 <.text+0x8aa88>
  18a9f8: 24 68 cf 00  	and	$13, $6, $15
  18a9fc: 14 00 e2 8c  	lw	$2, 0x14($7)
  18aa00: 18 00 e5 8c  	lw	$5, 0x18($7)
  18aa04: 21 10 02 01  	addu	$2, $8, $2
  18aa08: 40 10 02 00  	sll	$2, $2, 0x1
  18aa0c: 21 10 4c 00  	addu	$2, $2, $12
  18aa10: 00 00 44 94  	lhu	$4, 0x0($2)
  18aa14: ff ff 83 30  	andi	$3, $4, 0xffff
  18aa18: 26 20 c4 00  	xor	$4, $6, $4
  18aa1c: 24 10 6f 00  	and	$2, $3, $15
  18aa20: 21 04 84 30  	andi	$4, $4, 0x421
  18aa24: 24 18 c3 00  	and	$3, $6, $3
  18aa28: 21 10 a2 01  	addu	$2, $13, $2
  18aa2c: 21 04 63 30  	andi	$3, $3, 0x421
  18aa30: 43 10 02 00  	sra	$2, $2, 0x1
  18aa34: 21 10 43 00  	addu	$2, $2, $3
  18aa38: 40 10 02 00  	sll	$2, $2, 0x1
  18aa3c: 21 10 45 00  	addu	$2, $2, $5
  18aa40: 00 00 42 94  	lhu	$2, 0x0($2)
  18aa44: 25 20 82 00  	or	$4, $4, $2
  18aa48: 00 00 24 a6  	sh	$4, 0x0($17)
  18aa4c: 36 00 02 3c  	lui	$2, 0x36
  18aa50: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18aa54: 00 00 02 a2  	sb	$2, 0x0($16)
  18aa58: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18aa5c: d3 ff 01 05  	bgez	$8, 0x18a9ac <.text+0x8a9ac>
  18aa60: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18aa64: 36 00 02 3c  	lui	$2, 0x36
  18aa68: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18aa6c: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18aa70: 40 18 02 00  	sll	$3, $2, 0x1
  18aa74: 21 58 62 01  	addu	$11, $11, $2
  18aa78: c8 ff 20 15  	bnez	$9, 0x18a99c <.text+0x8a99c>
  18aa7c: 21 60 83 01  	addu	$12, $12, $3
  18aa80: 9d ff 00 10  	b	0x18a8f8 <.text+0x8a8f8>
  18aa84: 90 00 bf df  	ld	$ra, 0x90($sp)
  18aa88: 50 00 e3 8c  	lw	$3, 0x50($7)
  18aa8c: 50 00 e4 94  	lhu	$4, 0x50($7)
  18aa90: 24 10 6a 00  	and	$2, $3, $10
  18aa94: 18 00 e5 8c  	lw	$5, 0x18($7)
  18aa98: 24 18 c3 00  	and	$3, $6, $3
  18aa9c: 21 10 a2 01  	addu	$2, $13, $2
  18aaa0: 21 04 63 30  	andi	$3, $3, 0x421
  18aaa4: 42 10 02 00  	srl	$2, $2, 0x1
  18aaa8: 21 10 43 00  	addu	$2, $2, $3
  18aaac: 26 20 c4 00  	xor	$4, $6, $4
  18aab0: 40 10 02 00  	sll	$2, $2, 0x1
  18aab4: 21 04 84 30  	andi	$4, $4, 0x421
  18aab8: e1 ff 00 10  	b	0x18aa40 <.text+0x8aa40>
  18aabc: 21 10 45 00  	addu	$2, $2, $5
  18aac0: 56 00 40 10  	beqz	$2, 0x18ac1c <.text+0x8ac1c>
  18aac4: 23 10 95 02  	subu	$2, $20, $21
  18aac8: 07 00 03 24  	addiu	$3, $zero, 0x7
  18aacc: 23 90 72 00  	subu	$18, $3, $18
  18aad0: 21 10 52 00  	addu	$2, $2, $18
  18aad4: 38 00 46 90  	lbu	$6, 0x38($2)
  18aad8: 86 ff c0 10  	beqz	$6, 0x18a8f4 <.text+0x8a8f4>
  18aadc: 40 10 06 00  	sll	$2, $6, 0x1
  18aae0: 36 00 03 3c  	lui	$3, 0x36
  18aae4: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18aae8: 2d 48 c0 02  	move	$9, $22
  18aaec: 21 10 43 00  	addu	$2, $2, $3
  18aaf0: 80 ff c0 12  	beqz	$22, 0x18a8f4 <.text+0x8a8f4>
  18aaf4: 00 00 46 94  	lhu	$6, 0x0($2)
  18aaf8: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18aafc: 30 00 00 05  	bltz	$8, 0x18abc0 <.text+0x8abc0>
  18ab00: 40 10 08 00  	sll	$2, $8, 0x1
  18ab04: 21 70 4c 00  	addu	$14, $2, $12
  18ab08: 36 00 02 3c  	lui	$2, 0x36
  18ab0c: 21 80 68 01  	addu	$16, $11, $8
  18ab10: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18ab14: 2d 20 c0 00  	move	$4, $6
  18ab18: 00 00 02 92  	lbu	$2, 0x0($16)
  18ab1c: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18ab20: 2b 10 43 00  	sltu	$2, $2, $3
  18ab24: 23 00 40 10  	beqz	$2, 0x18abb4 <.text+0x8abb4>
  18ab28: 2d 88 c0 01  	move	$17, $14
  18ab2c: 48 00 e2 8c  	lw	$2, 0x48($7)
  18ab30: 21 10 02 01  	addu	$2, $8, $2
  18ab34: 21 10 62 01  	addu	$2, $11, $2
  18ab38: 00 00 43 90  	lbu	$3, 0x0($2)
  18ab3c: 1a 00 60 50  	beqzl	$3, 0x18aba8 <.text+0x8aba8>
  18ab40: 00 00 24 a6  	sh	$4, 0x0($17)
  18ab44: de fb 0f 24  	addiu	$15, $zero, -0x422 <.text+0xffffffffffeffbde>
  18ab48: 01 00 02 24  	addiu	$2, $zero, 0x1
  18ab4c: de fb 0a 24  	addiu	$10, $zero, -0x422 <.text+0xffffffffffeffbde>
  18ab50: 24 00 62 10  	beq	$3, $2, 0x18abe4 <.text+0x8abe4>
  18ab54: 24 68 cf 00  	and	$13, $6, $15
  18ab58: 14 00 e2 8c  	lw	$2, 0x14($7)
  18ab5c: 18 00 e5 8c  	lw	$5, 0x18($7)
  18ab60: 21 10 02 01  	addu	$2, $8, $2
  18ab64: 40 10 02 00  	sll	$2, $2, 0x1
  18ab68: 21 10 4c 00  	addu	$2, $2, $12
  18ab6c: 00 00 44 94  	lhu	$4, 0x0($2)
  18ab70: ff ff 83 30  	andi	$3, $4, 0xffff
  18ab74: 26 20 c4 00  	xor	$4, $6, $4
  18ab78: 24 10 6f 00  	and	$2, $3, $15
  18ab7c: 21 04 84 30  	andi	$4, $4, 0x421
  18ab80: 24 18 c3 00  	and	$3, $6, $3
  18ab84: 21 10 a2 01  	addu	$2, $13, $2
  18ab88: 21 04 63 30  	andi	$3, $3, 0x421
  18ab8c: 43 10 02 00  	sra	$2, $2, 0x1
  18ab90: 21 10 43 00  	addu	$2, $2, $3
  18ab94: 40 10 02 00  	sll	$2, $2, 0x1
  18ab98: 21 10 45 00  	addu	$2, $2, $5
  18ab9c: 00 00 42 94  	lhu	$2, 0x0($2)
  18aba0: 25 20 82 00  	or	$4, $4, $2
  18aba4: 00 00 24 a6  	sh	$4, 0x0($17)
  18aba8: 36 00 02 3c  	lui	$2, 0x36
  18abac: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18abb0: 00 00 02 a2  	sb	$2, 0x0($16)
  18abb4: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18abb8: d3 ff 01 05  	bgez	$8, 0x18ab08 <.text+0x8ab08>
  18abbc: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18abc0: 36 00 02 3c  	lui	$2, 0x36
  18abc4: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18abc8: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18abcc: 40 18 02 00  	sll	$3, $2, 0x1
  18abd0: 21 58 62 01  	addu	$11, $11, $2
  18abd4: c8 ff 20 15  	bnez	$9, 0x18aaf8 <.text+0x8aaf8>
  18abd8: 21 60 83 01  	addu	$12, $12, $3
  18abdc: 46 ff 00 10  	b	0x18a8f8 <.text+0x8a8f8>
  18abe0: 90 00 bf df  	ld	$ra, 0x90($sp)
  18abe4: 50 00 e3 8c  	lw	$3, 0x50($7)
  18abe8: 50 00 e4 94  	lhu	$4, 0x50($7)
  18abec: 24 10 6a 00  	and	$2, $3, $10
  18abf0: 18 00 e5 8c  	lw	$5, 0x18($7)
  18abf4: 24 18 c3 00  	and	$3, $6, $3
  18abf8: 21 10 a2 01  	addu	$2, $13, $2
  18abfc: 21 04 63 30  	andi	$3, $3, 0x421
  18ac00: 42 10 02 00  	srl	$2, $2, 0x1
  18ac04: 21 10 43 00  	addu	$2, $2, $3
  18ac08: 26 20 c4 00  	xor	$4, $6, $4
  18ac0c: 40 10 02 00  	sll	$2, $2, 0x1
  18ac10: 21 04 84 30  	andi	$4, $4, 0x421
  18ac14: e1 ff 00 10  	b	0x18ab9c <.text+0x8ab9c>
  18ac18: 21 10 45 00  	addu	$2, $2, $5
  18ac1c: 21 10 52 00  	addu	$2, $2, $18
  18ac20: 38 00 46 90  	lbu	$6, 0x38($2)
  18ac24: 33 ff c0 10  	beqz	$6, 0x18a8f4 <.text+0x8a8f4>
  18ac28: 40 10 06 00  	sll	$2, $6, 0x1
  18ac2c: 36 00 03 3c  	lui	$3, 0x36
  18ac30: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18ac34: 2d 48 c0 02  	move	$9, $22
  18ac38: 21 10 43 00  	addu	$2, $2, $3
  18ac3c: 2d ff c0 12  	beqz	$22, 0x18a8f4 <.text+0x8a8f4>
  18ac40: 00 00 46 94  	lhu	$6, 0x0($2)
  18ac44: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18ac48: 30 00 00 05  	bltz	$8, 0x18ad0c <.text+0x8ad0c>
  18ac4c: 40 10 08 00  	sll	$2, $8, 0x1
  18ac50: 21 70 4c 00  	addu	$14, $2, $12
  18ac54: 36 00 02 3c  	lui	$2, 0x36
  18ac58: 21 80 68 01  	addu	$16, $11, $8
  18ac5c: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18ac60: 2d 20 c0 00  	move	$4, $6
  18ac64: 00 00 02 92  	lbu	$2, 0x0($16)
  18ac68: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18ac6c: 2b 10 43 00  	sltu	$2, $2, $3
  18ac70: 23 00 40 10  	beqz	$2, 0x18ad00 <.text+0x8ad00>
  18ac74: 2d 88 c0 01  	move	$17, $14
  18ac78: 48 00 e2 8c  	lw	$2, 0x48($7)
  18ac7c: 21 10 02 01  	addu	$2, $8, $2
  18ac80: 21 10 62 01  	addu	$2, $11, $2
  18ac84: 00 00 43 90  	lbu	$3, 0x0($2)
  18ac88: 1a 00 60 50  	beqzl	$3, 0x18acf4 <.text+0x8acf4>
  18ac8c: 00 00 24 a6  	sh	$4, 0x0($17)
  18ac90: de fb 0f 24  	addiu	$15, $zero, -0x422 <.text+0xffffffffffeffbde>
  18ac94: 01 00 02 24  	addiu	$2, $zero, 0x1
  18ac98: de fb 0a 24  	addiu	$10, $zero, -0x422 <.text+0xffffffffffeffbde>
  18ac9c: 24 00 62 10  	beq	$3, $2, 0x18ad30 <.text+0x8ad30>
  18aca0: 24 68 cf 00  	and	$13, $6, $15
  18aca4: 14 00 e2 8c  	lw	$2, 0x14($7)
  18aca8: 18 00 e5 8c  	lw	$5, 0x18($7)
  18acac: 21 10 02 01  	addu	$2, $8, $2
  18acb0: 40 10 02 00  	sll	$2, $2, 0x1
  18acb4: 21 10 4c 00  	addu	$2, $2, $12
  18acb8: 00 00 44 94  	lhu	$4, 0x0($2)
  18acbc: ff ff 83 30  	andi	$3, $4, 0xffff
  18acc0: 26 20 c4 00  	xor	$4, $6, $4
  18acc4: 24 10 6f 00  	and	$2, $3, $15
  18acc8: 21 04 84 30  	andi	$4, $4, 0x421
  18accc: 24 18 c3 00  	and	$3, $6, $3
  18acd0: 21 10 a2 01  	addu	$2, $13, $2
  18acd4: 21 04 63 30  	andi	$3, $3, 0x421
  18acd8: 43 10 02 00  	sra	$2, $2, 0x1
  18acdc: 21 10 43 00  	addu	$2, $2, $3
  18ace0: 40 10 02 00  	sll	$2, $2, 0x1
  18ace4: 21 10 45 00  	addu	$2, $2, $5
  18ace8: 00 00 42 94  	lhu	$2, 0x0($2)
  18acec: 25 20 82 00  	or	$4, $4, $2
  18acf0: 00 00 24 a6  	sh	$4, 0x0($17)
  18acf4: 36 00 02 3c  	lui	$2, 0x36
  18acf8: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18acfc: 00 00 02 a2  	sb	$2, 0x0($16)
  18ad00: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18ad04: d3 ff 01 05  	bgez	$8, 0x18ac54 <.text+0x8ac54>
  18ad08: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18ad0c: 36 00 02 3c  	lui	$2, 0x36
  18ad10: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18ad14: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18ad18: 40 18 02 00  	sll	$3, $2, 0x1
  18ad1c: 21 58 62 01  	addu	$11, $11, $2
  18ad20: c8 ff 20 15  	bnez	$9, 0x18ac44 <.text+0x8ac44>
  18ad24: 21 60 83 01  	addu	$12, $12, $3
  18ad28: f3 fe 00 10  	b	0x18a8f8 <.text+0x8a8f8>
  18ad2c: 90 00 bf df  	ld	$ra, 0x90($sp)
  18ad30: 50 00 e3 8c  	lw	$3, 0x50($7)
  18ad34: 50 00 e4 94  	lhu	$4, 0x50($7)
  18ad38: 24 10 6a 00  	and	$2, $3, $10
  18ad3c: 18 00 e5 8c  	lw	$5, 0x18($7)
  18ad40: 24 18 c3 00  	and	$3, $6, $3
  18ad44: 21 10 a2 01  	addu	$2, $13, $2
  18ad48: 21 04 63 30  	andi	$3, $3, 0x421
  18ad4c: 42 10 02 00  	srl	$2, $2, 0x1
  18ad50: 21 10 43 00  	addu	$2, $2, $3
  18ad54: 26 20 c4 00  	xor	$4, $6, $4
  18ad58: 40 10 02 00  	sll	$2, $2, 0x1
  18ad5c: 21 04 84 30  	andi	$4, $4, 0x421
  18ad60: e1 ff 00 10  	b	0x18ace8 <.text+0x8ace8>
  18ad64: 21 10 45 00  	addu	$2, $2, $5
  18ad68: 23 0c 05 0c  	jal	0x14308c <.text+0x4308c>
  18ad6c: 00 00 00 00  	nop
  18ad70: 8c fe 00 10  	b	0x18a7a4 <.text+0x8a7a4>
  18ad74: 20 00 22 8e  	lw	$2, 0x20($17)
  18ad78: 82 1a 13 00  	srl	$3, $19, 0xa
  18ad7c: 1c 00 24 8e  	lw	$4, 0x1c($17)
  18ad80: 24 18 62 00  	and	$3, $3, $2
  18ad84: 18 00 22 8e  	lw	$2, 0x18($17)
  18ad88: 04 18 83 00  	sllv	$3, $3, $4
  18ad8c: 21 18 62 00  	addu	$3, $3, $2
  18ad90: 36 00 02 3c  	lui	$2, 0x36
  18ad94: 40 18 03 00  	sll	$3, $3, 0x1
  18ad98: 87 fe 00 10  	b	0x18a7b8 <.text+0x8a7b8>
  18ad9c: b0 ce 42 24  	addiu	$2, $2, -0x3150 <.text+0xffffffffffefceb0>
  18ada0: 81 0f 06 0c  	jal	0x183e04 <.text+0x83e04>
  18ada4: 2d 20 80 02  	move	$4, $20
  18ada8: 28 00 23 8e  	lw	$3, 0x28($17)
  18adac: 21 18 70 00  	addu	$3, $3, $16
  18adb0: 6e fe 00 10  	b	0x18a76c <.text+0x8a76c>
  18adb4: 00 00 62 a0  	sb	$2, 0x0($3)
  18adb8: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  18adbc: 36 00 02 3c  	lui	$2, 0x36
  18adc0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18adc4: ff 01 83 30  	andi	$3, $4, 0x1ff
  18adc8: 50 d4 51 24  	addiu	$17, $2, -0x2bb0 <.text+0xffffffffffefd450>
  18adcc: 30 00 b3 ff  	sd	$19, 0x30($sp)
  18add0: 08 00 2a 8e  	lw	$10, 0x8($17)
  18add4: ff 03 82 30  	andi	$2, $4, 0x3ff
  18add8: 2d 98 80 00  	move	$19, $4
  18addc: 00 01 63 2c  	sltiu	$3, $3, 0x100
  18ade0: 0c 00 24 8e  	lw	$4, 0xc($17)
  18ade4: 04 10 42 01  	sllv	$2, $2, $10
  18ade8: 80 00 be ff  	sd	$fp, 0x80($sp)
  18adec: 2d f0 e0 00  	move	$fp, $7
  18adf0: 70 00 b7 ff  	sd	$23, 0x70($sp)
  18adf4: 2d b8 a0 00  	move	$23, $5
  18adf8: 60 00 b6 ff  	sd	$22, 0x60($sp)
  18adfc: 21 28 82 00  	addu	$5, $4, $2
  18ae00: 50 00 b5 ff  	sd	$21, 0x50($sp)
  18ae04: 2d b0 20 01  	move	$22, $9
  18ae08: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18ae0c: 2d a8 00 01  	move	$21, $8
  18ae10: 90 00 bf ff  	sd	$ra, 0x90($sp)
  18ae14: 2d 90 c0 00  	move	$18, $6
  18ae18: 40 00 b4 ff  	sd	$20, 0x40($sp)
  18ae1c: 03 00 60 14  	bnez	$3, 0x18ae2c <.text+0x8ae2c>
  18ae20: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18ae24: 10 00 22 8e  	lw	$2, 0x10($17)
  18ae28: 21 28 a2 00  	addu	$5, $5, $2
  18ae2c: ff ff a5 30  	andi	$5, $5, 0xffff
  18ae30: 28 00 22 8e  	lw	$2, 0x28($17)
  18ae34: 06 80 45 01  	srlv	$16, $5, $10
  18ae38: 24 00 24 8e  	lw	$4, 0x24($17)
  18ae3c: 21 10 50 00  	addu	$2, $2, $16
  18ae40: 80 19 10 00  	sll	$3, $16, 0x6
  18ae44: 00 00 42 90  	lbu	$2, 0x0($2)
  18ae48: 76 01 40 10  	beqz	$2, 0x18b424 <.text+0x8b424>
  18ae4c: 21 a0 83 00  	addu	$20, $4, $3
  18ae50: 28 00 22 8e  	lw	$2, 0x28($17)
  18ae54: 21 10 50 00  	addu	$2, $2, $16
  18ae58: 00 00 43 90  	lbu	$3, 0x0($2)
  18ae5c: 02 00 02 24  	addiu	$2, $zero, 0x2
  18ae60: 56 00 62 10  	beq	$3, $2, 0x18afbc <.text+0x8afbc>
  18ae64: 90 00 bf df  	ld	$ra, 0x90($sp)
  18ae68: 2c 00 22 92  	lbu	$2, 0x2c($17)
  18ae6c: 63 01 40 50  	beqzl	$2, 0x18b3fc <.text+0x8b3fc>
  18ae70: 20 00 22 8e  	lw	$2, 0x20($17)
  18ae74: 36 00 02 3c  	lui	$2, 0x36
  18ae78: 6f c2 42 90  	lbu	$2, -0x3d91($2)
  18ae7c: 5b 01 40 14  	bnez	$2, 0x18b3ec <.text+0x8b3ec>
  18ae80: 00 00 00 00  	nop
  18ae84: 20 00 22 8e  	lw	$2, 0x20($17)
  18ae88: 82 1a 13 00  	srl	$3, $19, 0xa
  18ae8c: 24 18 62 00  	and	$3, $3, $2
  18ae90: 3f 00 02 3c  	lui	$2, 0x3f
  18ae94: 40 1a 03 00  	sll	$3, $3, 0x9
  18ae98: 80 2f 42 24  	addiu	$2, $2, 0x2f80
  18ae9c: 21 18 62 00  	addu	$3, $3, $2
  18aea0: 40 28 17 00  	sll	$5, $23, 0x1
  18aea4: 36 00 02 3c  	lui	$2, 0x36
  18aea8: 00 c0 64 32  	andi	$4, $19, 0xc000
  18aeac: c4 d4 43 ac  	sw	$3, -0x2b3c($2)
  18aeb0: 36 00 02 3c  	lui	$2, 0x36
  18aeb4: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18aeb8: 3c 00 e2 8c  	lw	$2, 0x3c($7)
  18aebc: 08 00 e3 8c  	lw	$3, 0x8($7)
  18aec0: 21 58 45 00  	addu	$11, $2, $5
  18aec4: 58 00 80 14  	bnez	$4, 0x18b028 <.text+0x8b028>
  18aec8: 21 50 77 00  	addu	$10, $3, $23
  18aecc: 21 10 95 02  	addu	$2, $20, $21
  18aed0: 21 10 52 00  	addu	$2, $2, $18
  18aed4: 00 00 46 90  	lbu	$6, 0x0($2)
  18aed8: 37 00 c0 10  	beqz	$6, 0x18afb8 <.text+0x8afb8>
  18aedc: 40 10 06 00  	sll	$2, $6, 0x1
  18aee0: 44 00 e3 8c  	lw	$3, 0x44($7)
  18aee4: 2d 48 c0 02  	move	$9, $22
  18aee8: 21 10 43 00  	addu	$2, $2, $3
  18aeec: 32 00 c0 12  	beqz	$22, 0x18afb8 <.text+0x8afb8>
  18aef0: 00 00 46 94  	lhu	$6, 0x0($2)
  18aef4: ff ff c7 27  	addiu	$7, $fp, -0x1 <.text+0xffffffffffefffff>
  18aef8: 28 00 e0 04  	bltz	$7, 0x18af9c <.text+0x8af9c>
  18aefc: 40 10 07 00  	sll	$2, $7, 0x1
  18af00: 21 70 4b 00  	addu	$14, $2, $11
  18af04: 36 00 02 3c  	lui	$2, 0x36
  18af08: 21 78 47 01  	addu	$15, $10, $7
  18af0c: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  18af10: 2d 20 c0 00  	move	$4, $6
  18af14: 00 00 e2 91  	lbu	$2, 0x0($15)
  18af18: 4c 00 03 91  	lbu	$3, 0x4c($8)
  18af1c: 2b 10 43 00  	sltu	$2, $2, $3
  18af20: 1b 00 40 10  	beqz	$2, 0x18af90 <.text+0x8af90>
  18af24: 2d 80 c0 01  	move	$16, $14
  18af28: 48 00 02 8d  	lw	$2, 0x48($8)
  18af2c: 21 10 e2 00  	addu	$2, $7, $2
  18af30: 21 10 42 01  	addu	$2, $10, $2
  18af34: 00 00 43 90  	lbu	$3, 0x0($2)
  18af38: 12 00 60 50  	beqzl	$3, 0x18af84 <.text+0x8af84>
  18af3c: 00 00 04 a6  	sh	$4, 0x0($16)
  18af40: de fb 0c 24  	addiu	$12, $zero, -0x422 <.text+0xffffffffffeffbde>
  18af44: 01 00 02 24  	addiu	$2, $zero, 0x1
  18af48: de fb 0d 24  	addiu	$13, $zero, -0x422 <.text+0xffffffffffeffbde>
  18af4c: 26 00 62 10  	beq	$3, $2, 0x18afe8 <.text+0x8afe8>
  18af50: 24 20 cc 00  	and	$4, $6, $12
  18af54: 14 00 02 8d  	lw	$2, 0x14($8)
  18af58: 21 10 e2 00  	addu	$2, $7, $2
  18af5c: 40 10 02 00  	sll	$2, $2, 0x1
  18af60: 21 10 4b 00  	addu	$2, $2, $11
  18af64: 00 00 43 94  	lhu	$3, 0x0($2)
  18af68: 24 10 6d 00  	and	$2, $3, $13
  18af6c: 24 18 c3 00  	and	$3, $6, $3
  18af70: 21 10 82 00  	addu	$2, $4, $2
  18af74: 21 04 63 30  	andi	$3, $3, 0x421
  18af78: 43 10 02 00  	sra	$2, $2, 0x1
  18af7c: 21 20 43 00  	addu	$4, $2, $3
  18af80: 00 00 04 a6  	sh	$4, 0x0($16)
  18af84: 36 00 02 3c  	lui	$2, 0x36
  18af88: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18af8c: 00 00 e2 a1  	sb	$2, 0x0($15)
  18af90: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  18af94: db ff e1 04  	bgez	$7, 0x18af04 <.text+0x8af04>
  18af98: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18af9c: 36 00 02 3c  	lui	$2, 0x36
  18afa0: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18afa4: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18afa8: 40 18 02 00  	sll	$3, $2, 0x1
  18afac: 21 50 42 01  	addu	$10, $10, $2
  18afb0: d0 ff 20 15  	bnez	$9, 0x18aef4 <.text+0x8aef4>
  18afb4: 21 58 63 01  	addu	$11, $11, $3
  18afb8: 90 00 bf df  	ld	$ra, 0x90($sp)
  18afbc: 80 00 be df  	ld	$fp, 0x80($sp)
  18afc0: 70 00 b7 df  	ld	$23, 0x70($sp)
  18afc4: 60 00 b6 df  	ld	$22, 0x60($sp)
  18afc8: 50 00 b5 df  	ld	$21, 0x50($sp)
  18afcc: 40 00 b4 df  	ld	$20, 0x40($sp)
  18afd0: 30 00 b3 df  	ld	$19, 0x30($sp)
  18afd4: 20 00 b2 df  	ld	$18, 0x20($sp)
  18afd8: 10 00 b1 df  	ld	$17, 0x10($sp)
  18afdc: 00 00 b0 df  	ld	$16, 0x0($sp)
  18afe0: 08 00 e0 03  	jr	$ra
  18afe4: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  18afe8: 50 00 03 8d  	lw	$3, 0x50($8)
  18afec: 18 00 05 8d  	lw	$5, 0x18($8)
  18aff0: 24 10 6c 00  	and	$2, $3, $12
  18aff4: 24 18 c3 00  	and	$3, $6, $3
  18aff8: 21 10 82 00  	addu	$2, $4, $2
  18affc: 21 04 63 30  	andi	$3, $3, 0x421
  18b000: 42 10 02 00  	srl	$2, $2, 0x1
  18b004: 50 00 04 95  	lhu	$4, 0x50($8)
  18b008: 21 10 43 00  	addu	$2, $2, $3
  18b00c: 40 10 02 00  	sll	$2, $2, 0x1
  18b010: 26 20 c4 00  	xor	$4, $6, $4
  18b014: 21 10 45 00  	addu	$2, $2, $5
  18b018: 21 04 84 30  	andi	$4, $4, 0x421
  18b01c: 00 00 42 94  	lhu	$2, 0x0($2)
  18b020: d7 ff 00 10  	b	0x18af80 <.text+0x8af80>
  18b024: 25 20 82 00  	or	$4, $4, $2
  18b028: 00 80 62 32  	andi	$2, $19, 0x8000
  18b02c: 51 00 40 14  	bnez	$2, 0x18b174 <.text+0x8b174>
  18b030: 00 40 62 32  	andi	$2, $19, 0x4000
  18b034: 07 00 03 24  	addiu	$3, $zero, 0x7
  18b038: 21 10 95 02  	addu	$2, $20, $21
  18b03c: 23 90 72 00  	subu	$18, $3, $18
  18b040: 21 10 52 00  	addu	$2, $2, $18
  18b044: 00 00 46 90  	lbu	$6, 0x0($2)
  18b048: db ff c0 10  	beqz	$6, 0x18afb8 <.text+0x8afb8>
  18b04c: 40 10 06 00  	sll	$2, $6, 0x1
  18b050: 36 00 03 3c  	lui	$3, 0x36
  18b054: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18b058: 2d 48 c0 02  	move	$9, $22
  18b05c: 21 10 43 00  	addu	$2, $2, $3
  18b060: d5 ff c0 12  	beqz	$22, 0x18afb8 <.text+0x8afb8>
  18b064: 00 00 46 94  	lhu	$6, 0x0($2)
  18b068: ff ff c7 27  	addiu	$7, $fp, -0x1 <.text+0xffffffffffefffff>
  18b06c: 28 00 e0 04  	bltz	$7, 0x18b110 <.text+0x8b110>
  18b070: 40 10 07 00  	sll	$2, $7, 0x1
  18b074: 21 70 4b 00  	addu	$14, $2, $11
  18b078: 36 00 02 3c  	lui	$2, 0x36
  18b07c: 21 78 47 01  	addu	$15, $10, $7
  18b080: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  18b084: 2d 20 c0 00  	move	$4, $6
  18b088: 00 00 e2 91  	lbu	$2, 0x0($15)
  18b08c: 4c 00 03 91  	lbu	$3, 0x4c($8)
  18b090: 2b 10 43 00  	sltu	$2, $2, $3
  18b094: 1b 00 40 10  	beqz	$2, 0x18b104 <.text+0x8b104>
  18b098: 2d 80 c0 01  	move	$16, $14
  18b09c: 48 00 02 8d  	lw	$2, 0x48($8)
  18b0a0: 21 10 e2 00  	addu	$2, $7, $2
  18b0a4: 21 10 42 01  	addu	$2, $10, $2
  18b0a8: 00 00 43 90  	lbu	$3, 0x0($2)
  18b0ac: 12 00 60 50  	beqzl	$3, 0x18b0f8 <.text+0x8b0f8>
  18b0b0: 00 00 04 a6  	sh	$4, 0x0($16)
  18b0b4: de fb 0c 24  	addiu	$12, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b0b8: 01 00 02 24  	addiu	$2, $zero, 0x1
  18b0bc: de fb 0d 24  	addiu	$13, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b0c0: 1c 00 62 10  	beq	$3, $2, 0x18b134 <.text+0x8b134>
  18b0c4: 24 20 cc 00  	and	$4, $6, $12
  18b0c8: 14 00 02 8d  	lw	$2, 0x14($8)
  18b0cc: 21 10 e2 00  	addu	$2, $7, $2
  18b0d0: 40 10 02 00  	sll	$2, $2, 0x1
  18b0d4: 21 10 4b 00  	addu	$2, $2, $11
  18b0d8: 00 00 43 94  	lhu	$3, 0x0($2)
  18b0dc: 24 10 6d 00  	and	$2, $3, $13
  18b0e0: 24 18 c3 00  	and	$3, $6, $3
  18b0e4: 21 10 82 00  	addu	$2, $4, $2
  18b0e8: 21 04 63 30  	andi	$3, $3, 0x421
  18b0ec: 43 10 02 00  	sra	$2, $2, 0x1
  18b0f0: 21 20 43 00  	addu	$4, $2, $3
  18b0f4: 00 00 04 a6  	sh	$4, 0x0($16)
  18b0f8: 36 00 02 3c  	lui	$2, 0x36
  18b0fc: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18b100: 00 00 e2 a1  	sb	$2, 0x0($15)
  18b104: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  18b108: db ff e1 04  	bgez	$7, 0x18b078 <.text+0x8b078>
  18b10c: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18b110: 36 00 02 3c  	lui	$2, 0x36
  18b114: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18b118: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18b11c: 40 18 02 00  	sll	$3, $2, 0x1
  18b120: 21 50 42 01  	addu	$10, $10, $2
  18b124: d0 ff 20 15  	bnez	$9, 0x18b068 <.text+0x8b068>
  18b128: 21 58 63 01  	addu	$11, $11, $3
  18b12c: a3 ff 00 10  	b	0x18afbc <.text+0x8afbc>
  18b130: 90 00 bf df  	ld	$ra, 0x90($sp)
  18b134: 50 00 03 8d  	lw	$3, 0x50($8)
  18b138: 18 00 05 8d  	lw	$5, 0x18($8)
  18b13c: 24 10 6c 00  	and	$2, $3, $12
  18b140: 24 18 c3 00  	and	$3, $6, $3
  18b144: 21 10 82 00  	addu	$2, $4, $2
  18b148: 21 04 63 30  	andi	$3, $3, 0x421
  18b14c: 42 10 02 00  	srl	$2, $2, 0x1
  18b150: 50 00 04 95  	lhu	$4, 0x50($8)
  18b154: 21 10 43 00  	addu	$2, $2, $3
  18b158: 40 10 02 00  	sll	$2, $2, 0x1
  18b15c: 26 20 c4 00  	xor	$4, $6, $4
  18b160: 21 10 45 00  	addu	$2, $2, $5
  18b164: 21 04 84 30  	andi	$4, $4, 0x421
  18b168: 00 00 42 94  	lhu	$2, 0x0($2)
  18b16c: e1 ff 00 10  	b	0x18b0f4 <.text+0x8b0f4>
  18b170: 25 20 82 00  	or	$4, $4, $2
  18b174: 50 00 40 10  	beqz	$2, 0x18b2b8 <.text+0x8b2b8>
  18b178: 23 10 95 02  	subu	$2, $20, $21
  18b17c: 07 00 03 24  	addiu	$3, $zero, 0x7
  18b180: 23 90 72 00  	subu	$18, $3, $18
  18b184: 21 10 52 00  	addu	$2, $2, $18
  18b188: 38 00 46 90  	lbu	$6, 0x38($2)
  18b18c: 8a ff c0 10  	beqz	$6, 0x18afb8 <.text+0x8afb8>
  18b190: 40 10 06 00  	sll	$2, $6, 0x1
  18b194: 36 00 03 3c  	lui	$3, 0x36
  18b198: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18b19c: 2d 48 c0 02  	move	$9, $22
  18b1a0: 21 10 43 00  	addu	$2, $2, $3
  18b1a4: 84 ff c0 12  	beqz	$22, 0x18afb8 <.text+0x8afb8>
  18b1a8: 00 00 46 94  	lhu	$6, 0x0($2)
  18b1ac: ff ff c7 27  	addiu	$7, $fp, -0x1 <.text+0xffffffffffefffff>
  18b1b0: 28 00 e0 04  	bltz	$7, 0x18b254 <.text+0x8b254>
  18b1b4: 40 10 07 00  	sll	$2, $7, 0x1
  18b1b8: 21 70 4b 00  	addu	$14, $2, $11
  18b1bc: 36 00 02 3c  	lui	$2, 0x36
  18b1c0: 21 78 47 01  	addu	$15, $10, $7
  18b1c4: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  18b1c8: 2d 20 c0 00  	move	$4, $6
  18b1cc: 00 00 e2 91  	lbu	$2, 0x0($15)
  18b1d0: 4c 00 03 91  	lbu	$3, 0x4c($8)
  18b1d4: 2b 10 43 00  	sltu	$2, $2, $3
  18b1d8: 1b 00 40 10  	beqz	$2, 0x18b248 <.text+0x8b248>
  18b1dc: 2d 80 c0 01  	move	$16, $14
  18b1e0: 48 00 02 8d  	lw	$2, 0x48($8)
  18b1e4: 21 10 e2 00  	addu	$2, $7, $2
  18b1e8: 21 10 42 01  	addu	$2, $10, $2
  18b1ec: 00 00 43 90  	lbu	$3, 0x0($2)
  18b1f0: 12 00 60 50  	beqzl	$3, 0x18b23c <.text+0x8b23c>
  18b1f4: 00 00 04 a6  	sh	$4, 0x0($16)
  18b1f8: de fb 0c 24  	addiu	$12, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b1fc: 01 00 02 24  	addiu	$2, $zero, 0x1
  18b200: de fb 0d 24  	addiu	$13, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b204: 1c 00 62 10  	beq	$3, $2, 0x18b278 <.text+0x8b278>
  18b208: 24 20 cc 00  	and	$4, $6, $12
  18b20c: 14 00 02 8d  	lw	$2, 0x14($8)
  18b210: 21 10 e2 00  	addu	$2, $7, $2
  18b214: 40 10 02 00  	sll	$2, $2, 0x1
  18b218: 21 10 4b 00  	addu	$2, $2, $11
  18b21c: 00 00 43 94  	lhu	$3, 0x0($2)
  18b220: 24 10 6d 00  	and	$2, $3, $13
  18b224: 24 18 c3 00  	and	$3, $6, $3
  18b228: 21 10 82 00  	addu	$2, $4, $2
  18b22c: 21 04 63 30  	andi	$3, $3, 0x421
  18b230: 43 10 02 00  	sra	$2, $2, 0x1
  18b234: 21 20 43 00  	addu	$4, $2, $3
  18b238: 00 00 04 a6  	sh	$4, 0x0($16)
  18b23c: 36 00 02 3c  	lui	$2, 0x36
  18b240: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18b244: 00 00 e2 a1  	sb	$2, 0x0($15)
  18b248: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  18b24c: db ff e1 04  	bgez	$7, 0x18b1bc <.text+0x8b1bc>
  18b250: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18b254: 36 00 02 3c  	lui	$2, 0x36
  18b258: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18b25c: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18b260: 40 18 02 00  	sll	$3, $2, 0x1
  18b264: 21 50 42 01  	addu	$10, $10, $2
  18b268: d0 ff 20 15  	bnez	$9, 0x18b1ac <.text+0x8b1ac>
  18b26c: 21 58 63 01  	addu	$11, $11, $3
  18b270: 52 ff 00 10  	b	0x18afbc <.text+0x8afbc>
  18b274: 90 00 bf df  	ld	$ra, 0x90($sp)
  18b278: 50 00 03 8d  	lw	$3, 0x50($8)
  18b27c: 18 00 05 8d  	lw	$5, 0x18($8)
  18b280: 24 10 6c 00  	and	$2, $3, $12
  18b284: 24 18 c3 00  	and	$3, $6, $3
  18b288: 21 10 82 00  	addu	$2, $4, $2
  18b28c: 21 04 63 30  	andi	$3, $3, 0x421
  18b290: 42 10 02 00  	srl	$2, $2, 0x1
  18b294: 50 00 04 95  	lhu	$4, 0x50($8)
  18b298: 21 10 43 00  	addu	$2, $2, $3
  18b29c: 40 10 02 00  	sll	$2, $2, 0x1
  18b2a0: 26 20 c4 00  	xor	$4, $6, $4
  18b2a4: 21 10 45 00  	addu	$2, $2, $5
  18b2a8: 21 04 84 30  	andi	$4, $4, 0x421
  18b2ac: 00 00 42 94  	lhu	$2, 0x0($2)
  18b2b0: e1 ff 00 10  	b	0x18b238 <.text+0x8b238>
  18b2b4: 25 20 82 00  	or	$4, $4, $2
  18b2b8: 21 10 52 00  	addu	$2, $2, $18
  18b2bc: 38 00 46 90  	lbu	$6, 0x38($2)
  18b2c0: 3d ff c0 10  	beqz	$6, 0x18afb8 <.text+0x8afb8>
  18b2c4: 40 10 06 00  	sll	$2, $6, 0x1
  18b2c8: 36 00 03 3c  	lui	$3, 0x36
  18b2cc: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18b2d0: 2d 48 c0 02  	move	$9, $22
  18b2d4: 21 10 43 00  	addu	$2, $2, $3
  18b2d8: 37 ff c0 12  	beqz	$22, 0x18afb8 <.text+0x8afb8>
  18b2dc: 00 00 46 94  	lhu	$6, 0x0($2)
  18b2e0: ff ff c7 27  	addiu	$7, $fp, -0x1 <.text+0xffffffffffefffff>
  18b2e4: 28 00 e0 04  	bltz	$7, 0x18b388 <.text+0x8b388>
  18b2e8: 40 10 07 00  	sll	$2, $7, 0x1
  18b2ec: 21 70 4b 00  	addu	$14, $2, $11
  18b2f0: 36 00 02 3c  	lui	$2, 0x36
  18b2f4: 21 78 47 01  	addu	$15, $10, $7
  18b2f8: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  18b2fc: 2d 20 c0 00  	move	$4, $6
  18b300: 00 00 e2 91  	lbu	$2, 0x0($15)
  18b304: 4c 00 03 91  	lbu	$3, 0x4c($8)
  18b308: 2b 10 43 00  	sltu	$2, $2, $3
  18b30c: 1b 00 40 10  	beqz	$2, 0x18b37c <.text+0x8b37c>
  18b310: 2d 80 c0 01  	move	$16, $14
  18b314: 48 00 02 8d  	lw	$2, 0x48($8)
  18b318: 21 10 e2 00  	addu	$2, $7, $2
  18b31c: 21 10 42 01  	addu	$2, $10, $2
  18b320: 00 00 43 90  	lbu	$3, 0x0($2)
  18b324: 12 00 60 50  	beqzl	$3, 0x18b370 <.text+0x8b370>
  18b328: 00 00 04 a6  	sh	$4, 0x0($16)
  18b32c: de fb 0c 24  	addiu	$12, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b330: 01 00 02 24  	addiu	$2, $zero, 0x1
  18b334: de fb 0d 24  	addiu	$13, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b338: 1c 00 62 10  	beq	$3, $2, 0x18b3ac <.text+0x8b3ac>
  18b33c: 24 20 cc 00  	and	$4, $6, $12
  18b340: 14 00 02 8d  	lw	$2, 0x14($8)
  18b344: 21 10 e2 00  	addu	$2, $7, $2
  18b348: 40 10 02 00  	sll	$2, $2, 0x1
  18b34c: 21 10 4b 00  	addu	$2, $2, $11
  18b350: 00 00 43 94  	lhu	$3, 0x0($2)
  18b354: 24 10 6d 00  	and	$2, $3, $13
  18b358: 24 18 c3 00  	and	$3, $6, $3
  18b35c: 21 10 82 00  	addu	$2, $4, $2
  18b360: 21 04 63 30  	andi	$3, $3, 0x421
  18b364: 43 10 02 00  	sra	$2, $2, 0x1
  18b368: 21 20 43 00  	addu	$4, $2, $3
  18b36c: 00 00 04 a6  	sh	$4, 0x0($16)
  18b370: 36 00 02 3c  	lui	$2, 0x36
  18b374: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18b378: 00 00 e2 a1  	sb	$2, 0x0($15)
  18b37c: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  18b380: db ff e1 04  	bgez	$7, 0x18b2f0 <.text+0x8b2f0>
  18b384: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18b388: 36 00 02 3c  	lui	$2, 0x36
  18b38c: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18b390: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18b394: 40 18 02 00  	sll	$3, $2, 0x1
  18b398: 21 50 42 01  	addu	$10, $10, $2
  18b39c: d0 ff 20 15  	bnez	$9, 0x18b2e0 <.text+0x8b2e0>
  18b3a0: 21 58 63 01  	addu	$11, $11, $3
  18b3a4: 05 ff 00 10  	b	0x18afbc <.text+0x8afbc>
  18b3a8: 90 00 bf df  	ld	$ra, 0x90($sp)
  18b3ac: 50 00 03 8d  	lw	$3, 0x50($8)
  18b3b0: 18 00 05 8d  	lw	$5, 0x18($8)
  18b3b4: 24 10 6c 00  	and	$2, $3, $12
  18b3b8: 24 18 c3 00  	and	$3, $6, $3
  18b3bc: 21 10 82 00  	addu	$2, $4, $2
  18b3c0: 21 04 63 30  	andi	$3, $3, 0x421
  18b3c4: 42 10 02 00  	srl	$2, $2, 0x1
  18b3c8: 50 00 04 95  	lhu	$4, 0x50($8)
  18b3cc: 21 10 43 00  	addu	$2, $2, $3
  18b3d0: 40 10 02 00  	sll	$2, $2, 0x1
  18b3d4: 26 20 c4 00  	xor	$4, $6, $4
  18b3d8: 21 10 45 00  	addu	$2, $2, $5
  18b3dc: 21 04 84 30  	andi	$4, $4, 0x421
  18b3e0: 00 00 42 94  	lhu	$2, 0x0($2)
  18b3e4: e1 ff 00 10  	b	0x18b36c <.text+0x8b36c>
  18b3e8: 25 20 82 00  	or	$4, $4, $2
  18b3ec: 23 0c 05 0c  	jal	0x14308c <.text+0x4308c>
  18b3f0: 00 00 00 00  	nop
  18b3f4: a4 fe 00 10  	b	0x18ae88 <.text+0x8ae88>
  18b3f8: 20 00 22 8e  	lw	$2, 0x20($17)
  18b3fc: 82 1a 13 00  	srl	$3, $19, 0xa
  18b400: 1c 00 24 8e  	lw	$4, 0x1c($17)
  18b404: 24 18 62 00  	and	$3, $3, $2
  18b408: 18 00 22 8e  	lw	$2, 0x18($17)
  18b40c: 04 18 83 00  	sllv	$3, $3, $4
  18b410: 21 18 62 00  	addu	$3, $3, $2
  18b414: 36 00 02 3c  	lui	$2, 0x36
  18b418: 40 18 03 00  	sll	$3, $3, 0x1
  18b41c: 9f fe 00 10  	b	0x18ae9c <.text+0x8ae9c>
  18b420: b0 ce 42 24  	addiu	$2, $2, -0x3150 <.text+0xffffffffffefceb0>
  18b424: 81 0f 06 0c  	jal	0x183e04 <.text+0x83e04>
  18b428: 2d 20 80 02  	move	$4, $20
  18b42c: 28 00 23 8e  	lw	$3, 0x28($17)
  18b430: 21 18 70 00  	addu	$3, $3, $16
  18b434: 86 fe 00 10  	b	0x18ae50 <.text+0x8ae50>
  18b438: 00 00 62 a0  	sb	$2, 0x0($3)
  18b43c: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  18b440: 36 00 02 3c  	lui	$2, 0x36
  18b444: 80 00 be ff  	sd	$fp, 0x80($sp)
  18b448: ff 01 83 30  	andi	$3, $4, 0x1ff
  18b44c: 70 00 b7 ff  	sd	$23, 0x70($sp)
  18b450: 00 01 63 2c  	sltiu	$3, $3, 0x100
  18b454: 60 00 b6 ff  	sd	$22, 0x60($sp)
  18b458: 2d b8 a0 00  	move	$23, $5
  18b45c: 50 00 b5 ff  	sd	$21, 0x50($sp)
  18b460: 2d f0 e0 00  	move	$fp, $7
  18b464: 30 00 b3 ff  	sd	$19, 0x30($sp)
  18b468: 2d a8 00 01  	move	$21, $8
  18b46c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18b470: 2d 98 80 00  	move	$19, $4
  18b474: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18b478: 2d 90 c0 00  	move	$18, $6
  18b47c: 90 00 bf ff  	sd	$ra, 0x90($sp)
  18b480: 50 d4 51 24  	addiu	$17, $2, -0x2bb0 <.text+0xffffffffffefd450>
  18b484: 40 00 b4 ff  	sd	$20, 0x40($sp)
  18b488: ff 03 82 30  	andi	$2, $4, 0x3ff
  18b48c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18b490: 2d b0 20 01  	move	$22, $9
  18b494: 08 00 2a 8e  	lw	$10, 0x8($17)
  18b498: 0c 00 24 8e  	lw	$4, 0xc($17)
  18b49c: 04 10 42 01  	sllv	$2, $2, $10
  18b4a0: 03 00 60 14  	bnez	$3, 0x18b4b0 <.text+0x8b4b0>
  18b4a4: 21 28 82 00  	addu	$5, $4, $2
  18b4a8: 10 00 22 8e  	lw	$2, 0x10($17)
  18b4ac: 21 28 a2 00  	addu	$5, $5, $2
  18b4b0: ff ff a5 30  	andi	$5, $5, 0xffff
  18b4b4: 28 00 22 8e  	lw	$2, 0x28($17)
  18b4b8: 06 80 45 01  	srlv	$16, $5, $10
  18b4bc: 24 00 24 8e  	lw	$4, 0x24($17)
  18b4c0: 21 10 50 00  	addu	$2, $2, $16
  18b4c4: 80 19 10 00  	sll	$3, $16, 0x6
  18b4c8: 00 00 42 90  	lbu	$2, 0x0($2)
  18b4cc: 76 01 40 10  	beqz	$2, 0x18baa8 <.text+0x8baa8>
  18b4d0: 21 a0 83 00  	addu	$20, $4, $3
  18b4d4: 28 00 22 8e  	lw	$2, 0x28($17)
  18b4d8: 21 10 50 00  	addu	$2, $2, $16
  18b4dc: 00 00 43 90  	lbu	$3, 0x0($2)
  18b4e0: 02 00 02 24  	addiu	$2, $zero, 0x2
  18b4e4: 59 00 62 10  	beq	$3, $2, 0x18b64c <.text+0x8b64c>
  18b4e8: 90 00 bf df  	ld	$ra, 0x90($sp)
  18b4ec: 2c 00 22 92  	lbu	$2, 0x2c($17)
  18b4f0: 63 01 40 50  	beqzl	$2, 0x18ba80 <.text+0x8ba80>
  18b4f4: 20 00 22 8e  	lw	$2, 0x20($17)
  18b4f8: 36 00 02 3c  	lui	$2, 0x36
  18b4fc: 6f c2 42 90  	lbu	$2, -0x3d91($2)
  18b500: 5b 01 40 14  	bnez	$2, 0x18ba70 <.text+0x8ba70>
  18b504: 00 00 00 00  	nop
  18b508: 20 00 22 8e  	lw	$2, 0x20($17)
  18b50c: 82 1a 13 00  	srl	$3, $19, 0xa
  18b510: 24 18 62 00  	and	$3, $3, $2
  18b514: 3f 00 02 3c  	lui	$2, 0x3f
  18b518: 40 1a 03 00  	sll	$3, $3, 0x9
  18b51c: 80 2f 42 24  	addiu	$2, $2, 0x2f80
  18b520: 21 18 62 00  	addu	$3, $3, $2
  18b524: 40 28 17 00  	sll	$5, $23, 0x1
  18b528: 36 00 02 3c  	lui	$2, 0x36
  18b52c: 00 c0 64 32  	andi	$4, $19, 0xc000
  18b530: c4 d4 43 ac  	sw	$3, -0x2b3c($2)
  18b534: 36 00 02 3c  	lui	$2, 0x36
  18b538: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18b53c: 3c 00 e2 8c  	lw	$2, 0x3c($7)
  18b540: 08 00 e3 8c  	lw	$3, 0x8($7)
  18b544: 21 58 45 00  	addu	$11, $2, $5
  18b548: 58 00 80 14  	bnez	$4, 0x18b6ac <.text+0x8b6ac>
  18b54c: 21 28 77 00  	addu	$5, $3, $23
  18b550: 21 10 95 02  	addu	$2, $20, $21
  18b554: 21 10 52 00  	addu	$2, $2, $18
  18b558: 00 00 46 90  	lbu	$6, 0x0($2)
  18b55c: 3a 00 c0 10  	beqz	$6, 0x18b648 <.text+0x8b648>
  18b560: 40 10 06 00  	sll	$2, $6, 0x1
  18b564: 44 00 e3 8c  	lw	$3, 0x44($7)
  18b568: 2d 48 c0 02  	move	$9, $22
  18b56c: 21 10 43 00  	addu	$2, $2, $3
  18b570: 35 00 c0 12  	beqz	$22, 0x18b648 <.text+0x8b648>
  18b574: 00 00 46 94  	lhu	$6, 0x0($2)
  18b578: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18b57c: 2b 00 00 05  	bltz	$8, 0x18b62c <.text+0x8b62c>
  18b580: 40 10 08 00  	sll	$2, $8, 0x1
  18b584: 21 70 4b 00  	addu	$14, $2, $11
  18b588: 36 00 02 3c  	lui	$2, 0x36
  18b58c: 21 80 a8 00  	addu	$16, $5, $8
  18b590: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18b594: 2d 20 c0 00  	move	$4, $6
  18b598: 00 00 02 92  	lbu	$2, 0x0($16)
  18b59c: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18b5a0: 2b 10 43 00  	sltu	$2, $2, $3
  18b5a4: 1e 00 40 10  	beqz	$2, 0x18b620 <.text+0x8b620>
  18b5a8: 2d 88 c0 01  	move	$17, $14
  18b5ac: 48 00 e2 8c  	lw	$2, 0x48($7)
  18b5b0: 21 10 02 01  	addu	$2, $8, $2
  18b5b4: 21 10 a2 00  	addu	$2, $5, $2
  18b5b8: 00 00 43 90  	lbu	$3, 0x0($2)
  18b5bc: 15 00 60 50  	beqzl	$3, 0x18b614 <.text+0x8b614>
  18b5c0: 00 00 24 a6  	sh	$4, 0x0($17)
  18b5c4: 01 00 02 24  	addiu	$2, $zero, 0x1
  18b5c8: 20 84 cd 34  	ori	$13, $6, 0x8420
  18b5cc: 2a 00 62 10  	beq	$3, $2, 0x18b678 <.text+0x8b678>
  18b5d0: 21 04 cf 30  	andi	$15, $6, 0x421
  18b5d4: 14 00 e2 8c  	lw	$2, 0x14($7)
  18b5d8: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18b5dc: 21 10 02 01  	addu	$2, $8, $2
  18b5e0: 40 10 02 00  	sll	$2, $2, 0x1
  18b5e4: 21 10 4b 00  	addu	$2, $2, $11
  18b5e8: 00 00 43 94  	lhu	$3, 0x0($2)
  18b5ec: de fb 62 30  	andi	$2, $3, 0xfbde
  18b5f0: 21 04 63 30  	andi	$3, $3, 0x421
  18b5f4: 23 10 a2 01  	subu	$2, $13, $2
  18b5f8: 43 10 02 00  	sra	$2, $2, 0x1
  18b5fc: 40 10 02 00  	sll	$2, $2, 0x1
  18b600: 21 10 44 00  	addu	$2, $2, $4
  18b604: 00 00 42 94  	lhu	$2, 0x0($2)
  18b608: 21 10 4f 00  	addu	$2, $2, $15
  18b60c: 23 20 43 00  	subu	$4, $2, $3
  18b610: 00 00 24 a6  	sh	$4, 0x0($17)
  18b614: 36 00 02 3c  	lui	$2, 0x36
  18b618: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18b61c: 00 00 02 a2  	sb	$2, 0x0($16)
  18b620: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18b624: d8 ff 01 05  	bgez	$8, 0x18b588 <.text+0x8b588>
  18b628: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18b62c: 36 00 02 3c  	lui	$2, 0x36
  18b630: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18b634: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18b638: 40 18 02 00  	sll	$3, $2, 0x1
  18b63c: 21 28 a2 00  	addu	$5, $5, $2
  18b640: cd ff 20 15  	bnez	$9, 0x18b578 <.text+0x8b578>
  18b644: 21 58 63 01  	addu	$11, $11, $3
  18b648: 90 00 bf df  	ld	$ra, 0x90($sp)
  18b64c: 80 00 be df  	ld	$fp, 0x80($sp)
  18b650: 70 00 b7 df  	ld	$23, 0x70($sp)
  18b654: 60 00 b6 df  	ld	$22, 0x60($sp)
  18b658: 50 00 b5 df  	ld	$21, 0x50($sp)
  18b65c: 40 00 b4 df  	ld	$20, 0x40($sp)
  18b660: 30 00 b3 df  	ld	$19, 0x30($sp)
  18b664: 20 00 b2 df  	ld	$18, 0x20($sp)
  18b668: 10 00 b1 df  	ld	$17, 0x10($sp)
  18b66c: 00 00 b0 df  	ld	$16, 0x0($sp)
  18b670: 08 00 e0 03  	jr	$ra
  18b674: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  18b678: 50 00 e2 8c  	lw	$2, 0x50($7)
  18b67c: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b680: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18b684: 24 10 43 00  	and	$2, $2, $3
  18b688: 50 00 e3 94  	lhu	$3, 0x50($7)
  18b68c: 23 10 a2 01  	subu	$2, $13, $2
  18b690: 42 10 02 00  	srl	$2, $2, 0x1
  18b694: 21 04 63 30  	andi	$3, $3, 0x421
  18b698: 40 10 02 00  	sll	$2, $2, 0x1
  18b69c: 21 10 44 00  	addu	$2, $2, $4
  18b6a0: 00 00 42 94  	lhu	$2, 0x0($2)
  18b6a4: d9 ff 00 10  	b	0x18b60c <.text+0x8b60c>
  18b6a8: 21 10 4f 00  	addu	$2, $2, $15
  18b6ac: 00 80 62 32  	andi	$2, $19, 0x8000
  18b6b0: 51 00 40 14  	bnez	$2, 0x18b7f8 <.text+0x8b7f8>
  18b6b4: 00 40 62 32  	andi	$2, $19, 0x4000
  18b6b8: 07 00 03 24  	addiu	$3, $zero, 0x7
  18b6bc: 21 10 95 02  	addu	$2, $20, $21
  18b6c0: 23 90 72 00  	subu	$18, $3, $18
  18b6c4: 21 10 52 00  	addu	$2, $2, $18
  18b6c8: 00 00 46 90  	lbu	$6, 0x0($2)
  18b6cc: de ff c0 10  	beqz	$6, 0x18b648 <.text+0x8b648>
  18b6d0: 40 10 06 00  	sll	$2, $6, 0x1
  18b6d4: 36 00 03 3c  	lui	$3, 0x36
  18b6d8: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18b6dc: 2d 48 c0 02  	move	$9, $22
  18b6e0: 21 10 43 00  	addu	$2, $2, $3
  18b6e4: d8 ff c0 12  	beqz	$22, 0x18b648 <.text+0x8b648>
  18b6e8: 00 00 46 94  	lhu	$6, 0x0($2)
  18b6ec: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18b6f0: 2b 00 00 05  	bltz	$8, 0x18b7a0 <.text+0x8b7a0>
  18b6f4: 40 10 08 00  	sll	$2, $8, 0x1
  18b6f8: 21 70 4b 00  	addu	$14, $2, $11
  18b6fc: 36 00 02 3c  	lui	$2, 0x36
  18b700: 21 80 a8 00  	addu	$16, $5, $8
  18b704: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18b708: 2d 20 c0 00  	move	$4, $6
  18b70c: 00 00 02 92  	lbu	$2, 0x0($16)
  18b710: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18b714: 2b 10 43 00  	sltu	$2, $2, $3
  18b718: 1e 00 40 10  	beqz	$2, 0x18b794 <.text+0x8b794>
  18b71c: 2d 88 c0 01  	move	$17, $14
  18b720: 48 00 e2 8c  	lw	$2, 0x48($7)
  18b724: 21 10 02 01  	addu	$2, $8, $2
  18b728: 21 10 a2 00  	addu	$2, $5, $2
  18b72c: 00 00 43 90  	lbu	$3, 0x0($2)
  18b730: 15 00 60 50  	beqzl	$3, 0x18b788 <.text+0x8b788>
  18b734: 00 00 24 a6  	sh	$4, 0x0($17)
  18b738: 01 00 02 24  	addiu	$2, $zero, 0x1
  18b73c: 20 84 cd 34  	ori	$13, $6, 0x8420
  18b740: 20 00 62 10  	beq	$3, $2, 0x18b7c4 <.text+0x8b7c4>
  18b744: 21 04 cf 30  	andi	$15, $6, 0x421
  18b748: 14 00 e2 8c  	lw	$2, 0x14($7)
  18b74c: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18b750: 21 10 02 01  	addu	$2, $8, $2
  18b754: 40 10 02 00  	sll	$2, $2, 0x1
  18b758: 21 10 4b 00  	addu	$2, $2, $11
  18b75c: 00 00 43 94  	lhu	$3, 0x0($2)
  18b760: de fb 62 30  	andi	$2, $3, 0xfbde
  18b764: 21 04 63 30  	andi	$3, $3, 0x421
  18b768: 23 10 a2 01  	subu	$2, $13, $2
  18b76c: 43 10 02 00  	sra	$2, $2, 0x1
  18b770: 40 10 02 00  	sll	$2, $2, 0x1
  18b774: 21 10 44 00  	addu	$2, $2, $4
  18b778: 00 00 42 94  	lhu	$2, 0x0($2)
  18b77c: 21 10 4f 00  	addu	$2, $2, $15
  18b780: 23 20 43 00  	subu	$4, $2, $3
  18b784: 00 00 24 a6  	sh	$4, 0x0($17)
  18b788: 36 00 02 3c  	lui	$2, 0x36
  18b78c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18b790: 00 00 02 a2  	sb	$2, 0x0($16)
  18b794: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18b798: d8 ff 01 05  	bgez	$8, 0x18b6fc <.text+0x8b6fc>
  18b79c: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18b7a0: 36 00 02 3c  	lui	$2, 0x36
  18b7a4: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18b7a8: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18b7ac: 40 18 02 00  	sll	$3, $2, 0x1
  18b7b0: 21 28 a2 00  	addu	$5, $5, $2
  18b7b4: cd ff 20 15  	bnez	$9, 0x18b6ec <.text+0x8b6ec>
  18b7b8: 21 58 63 01  	addu	$11, $11, $3
  18b7bc: a3 ff 00 10  	b	0x18b64c <.text+0x8b64c>
  18b7c0: 90 00 bf df  	ld	$ra, 0x90($sp)
  18b7c4: 50 00 e2 8c  	lw	$2, 0x50($7)
  18b7c8: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b7cc: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18b7d0: 24 10 43 00  	and	$2, $2, $3
  18b7d4: 50 00 e3 94  	lhu	$3, 0x50($7)
  18b7d8: 23 10 a2 01  	subu	$2, $13, $2
  18b7dc: 42 10 02 00  	srl	$2, $2, 0x1
  18b7e0: 21 04 63 30  	andi	$3, $3, 0x421
  18b7e4: 40 10 02 00  	sll	$2, $2, 0x1
  18b7e8: 21 10 44 00  	addu	$2, $2, $4
  18b7ec: 00 00 42 94  	lhu	$2, 0x0($2)
  18b7f0: e3 ff 00 10  	b	0x18b780 <.text+0x8b780>
  18b7f4: 21 10 4f 00  	addu	$2, $2, $15
  18b7f8: 50 00 40 10  	beqz	$2, 0x18b93c <.text+0x8b93c>
  18b7fc: 23 10 95 02  	subu	$2, $20, $21
  18b800: 07 00 03 24  	addiu	$3, $zero, 0x7
  18b804: 23 90 72 00  	subu	$18, $3, $18
  18b808: 21 10 52 00  	addu	$2, $2, $18
  18b80c: 38 00 46 90  	lbu	$6, 0x38($2)
  18b810: 8d ff c0 10  	beqz	$6, 0x18b648 <.text+0x8b648>
  18b814: 40 10 06 00  	sll	$2, $6, 0x1
  18b818: 36 00 03 3c  	lui	$3, 0x36
  18b81c: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18b820: 2d 48 c0 02  	move	$9, $22
  18b824: 21 10 43 00  	addu	$2, $2, $3
  18b828: 87 ff c0 12  	beqz	$22, 0x18b648 <.text+0x8b648>
  18b82c: 00 00 46 94  	lhu	$6, 0x0($2)
  18b830: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18b834: 2b 00 00 05  	bltz	$8, 0x18b8e4 <.text+0x8b8e4>
  18b838: 40 10 08 00  	sll	$2, $8, 0x1
  18b83c: 21 70 4b 00  	addu	$14, $2, $11
  18b840: 36 00 02 3c  	lui	$2, 0x36
  18b844: 21 80 a8 00  	addu	$16, $5, $8
  18b848: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18b84c: 2d 20 c0 00  	move	$4, $6
  18b850: 00 00 02 92  	lbu	$2, 0x0($16)
  18b854: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18b858: 2b 10 43 00  	sltu	$2, $2, $3
  18b85c: 1e 00 40 10  	beqz	$2, 0x18b8d8 <.text+0x8b8d8>
  18b860: 2d 88 c0 01  	move	$17, $14
  18b864: 48 00 e2 8c  	lw	$2, 0x48($7)
  18b868: 21 10 02 01  	addu	$2, $8, $2
  18b86c: 21 10 a2 00  	addu	$2, $5, $2
  18b870: 00 00 43 90  	lbu	$3, 0x0($2)
  18b874: 15 00 60 50  	beqzl	$3, 0x18b8cc <.text+0x8b8cc>
  18b878: 00 00 24 a6  	sh	$4, 0x0($17)
  18b87c: 01 00 02 24  	addiu	$2, $zero, 0x1
  18b880: 20 84 cd 34  	ori	$13, $6, 0x8420
  18b884: 20 00 62 10  	beq	$3, $2, 0x18b908 <.text+0x8b908>
  18b888: 21 04 cf 30  	andi	$15, $6, 0x421
  18b88c: 14 00 e2 8c  	lw	$2, 0x14($7)
  18b890: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18b894: 21 10 02 01  	addu	$2, $8, $2
  18b898: 40 10 02 00  	sll	$2, $2, 0x1
  18b89c: 21 10 4b 00  	addu	$2, $2, $11
  18b8a0: 00 00 43 94  	lhu	$3, 0x0($2)
  18b8a4: de fb 62 30  	andi	$2, $3, 0xfbde
  18b8a8: 21 04 63 30  	andi	$3, $3, 0x421
  18b8ac: 23 10 a2 01  	subu	$2, $13, $2
  18b8b0: 43 10 02 00  	sra	$2, $2, 0x1
  18b8b4: 40 10 02 00  	sll	$2, $2, 0x1
  18b8b8: 21 10 44 00  	addu	$2, $2, $4
  18b8bc: 00 00 42 94  	lhu	$2, 0x0($2)
  18b8c0: 21 10 4f 00  	addu	$2, $2, $15
  18b8c4: 23 20 43 00  	subu	$4, $2, $3
  18b8c8: 00 00 24 a6  	sh	$4, 0x0($17)
  18b8cc: 36 00 02 3c  	lui	$2, 0x36
  18b8d0: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18b8d4: 00 00 02 a2  	sb	$2, 0x0($16)
  18b8d8: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18b8dc: d8 ff 01 05  	bgez	$8, 0x18b840 <.text+0x8b840>
  18b8e0: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18b8e4: 36 00 02 3c  	lui	$2, 0x36
  18b8e8: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18b8ec: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18b8f0: 40 18 02 00  	sll	$3, $2, 0x1
  18b8f4: 21 28 a2 00  	addu	$5, $5, $2
  18b8f8: cd ff 20 15  	bnez	$9, 0x18b830 <.text+0x8b830>
  18b8fc: 21 58 63 01  	addu	$11, $11, $3
  18b900: 52 ff 00 10  	b	0x18b64c <.text+0x8b64c>
  18b904: 90 00 bf df  	ld	$ra, 0x90($sp)
  18b908: 50 00 e2 8c  	lw	$2, 0x50($7)
  18b90c: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  18b910: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18b914: 24 10 43 00  	and	$2, $2, $3
  18b918: 50 00 e3 94  	lhu	$3, 0x50($7)
  18b91c: 23 10 a2 01  	subu	$2, $13, $2
  18b920: 42 10 02 00  	srl	$2, $2, 0x1
  18b924: 21 04 63 30  	andi	$3, $3, 0x421
  18b928: 40 10 02 00  	sll	$2, $2, 0x1
  18b92c: 21 10 44 00  	addu	$2, $2, $4
  18b930: 00 00 42 94  	lhu	$2, 0x0($2)
  18b934: e3 ff 00 10  	b	0x18b8c4 <.text+0x8b8c4>
  18b938: 21 10 4f 00  	addu	$2, $2, $15
  18b93c: 21 10 52 00  	addu	$2, $2, $18
  18b940: 38 00 46 90  	lbu	$6, 0x38($2)
  18b944: 40 ff c0 10  	beqz	$6, 0x18b648 <.text+0x8b648>
  18b948: 40 10 06 00  	sll	$2, $6, 0x1
  18b94c: 36 00 03 3c  	lui	$3, 0x36
  18b950: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18b954: 2d 48 c0 02  	move	$9, $22
  18b958: 21 10 43 00  	addu	$2, $2, $3
  18b95c: 3a ff c0 12  	beqz	$22, 0x18b648 <.text+0x8b648>
  18b960: 00 00 46 94  	lhu	$6, 0x0($2)
  18b964: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18b968: 2b 00 00 05  	bltz	$8, 0x18ba18 <.text+0x8ba18>
  18b96c: 40 10 08 00  	sll	$2, $8, 0x1
  18b970: 21 70 4b 00  	addu	$14, $2, $11
  18b974: 36 00 02 3c  	lui	$2, 0x36
  18b978: 21 80 a8 00  	addu	$16, $5, $8
  18b97c: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18b980: 2d 20 c0 00  	move	$4, $6
  18b984: 00 00 02 92  	lbu	$2, 0x0($16)
  18b988: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18b98c: 2b 10 43 00  	sltu	$2, $2, $3
  18b990: 1e 00 40 10  	beqz	$2, 0x18ba0c <.text+0x8ba0c>
  18b994: 2d 88 c0 01  	move	$17, $14
  18b998: 48 00 e2 8c  	lw	$2, 0x48($7)
  18b99c: 21 10 02 01  	addu	$2, $8, $2
  18b9a0: 21 10 a2 00  	addu	$2, $5, $2
  18b9a4: 00 00 43 90  	lbu	$3, 0x0($2)
  18b9a8: 15 00 60 50  	beqzl	$3, 0x18ba00 <.text+0x8ba00>
  18b9ac: 00 00 24 a6  	sh	$4, 0x0($17)
  18b9b0: 01 00 02 24  	addiu	$2, $zero, 0x1
  18b9b4: 20 84 cd 34  	ori	$13, $6, 0x8420
  18b9b8: 20 00 62 10  	beq	$3, $2, 0x18ba3c <.text+0x8ba3c>
  18b9bc: 21 04 cf 30  	andi	$15, $6, 0x421
  18b9c0: 14 00 e2 8c  	lw	$2, 0x14($7)
  18b9c4: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18b9c8: 21 10 02 01  	addu	$2, $8, $2
  18b9cc: 40 10 02 00  	sll	$2, $2, 0x1
  18b9d0: 21 10 4b 00  	addu	$2, $2, $11
  18b9d4: 00 00 43 94  	lhu	$3, 0x0($2)
  18b9d8: de fb 62 30  	andi	$2, $3, 0xfbde
  18b9dc: 21 04 63 30  	andi	$3, $3, 0x421
  18b9e0: 23 10 a2 01  	subu	$2, $13, $2
  18b9e4: 43 10 02 00  	sra	$2, $2, 0x1
  18b9e8: 40 10 02 00  	sll	$2, $2, 0x1
  18b9ec: 21 10 44 00  	addu	$2, $2, $4
  18b9f0: 00 00 42 94  	lhu	$2, 0x0($2)
  18b9f4: 21 10 4f 00  	addu	$2, $2, $15
  18b9f8: 23 20 43 00  	subu	$4, $2, $3
  18b9fc: 00 00 24 a6  	sh	$4, 0x0($17)
  18ba00: 36 00 02 3c  	lui	$2, 0x36
  18ba04: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18ba08: 00 00 02 a2  	sb	$2, 0x0($16)
  18ba0c: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18ba10: d8 ff 01 05  	bgez	$8, 0x18b974 <.text+0x8b974>
  18ba14: fe ff ce 25  	addiu	$14, $14, -0x2 <.text+0xffffffffffeffffe>
  18ba18: 36 00 02 3c  	lui	$2, 0x36
  18ba1c: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18ba20: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18ba24: 40 18 02 00  	sll	$3, $2, 0x1
  18ba28: 21 28 a2 00  	addu	$5, $5, $2
  18ba2c: cd ff 20 15  	bnez	$9, 0x18b964 <.text+0x8b964>
  18ba30: 21 58 63 01  	addu	$11, $11, $3
  18ba34: 05 ff 00 10  	b	0x18b64c <.text+0x8b64c>
  18ba38: 90 00 bf df  	ld	$ra, 0x90($sp)
  18ba3c: 50 00 e2 8c  	lw	$2, 0x50($7)
  18ba40: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  18ba44: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18ba48: 24 10 43 00  	and	$2, $2, $3
  18ba4c: 50 00 e3 94  	lhu	$3, 0x50($7)
  18ba50: 23 10 a2 01  	subu	$2, $13, $2
  18ba54: 42 10 02 00  	srl	$2, $2, 0x1
  18ba58: 21 04 63 30  	andi	$3, $3, 0x421
  18ba5c: 40 10 02 00  	sll	$2, $2, 0x1
  18ba60: 21 10 44 00  	addu	$2, $2, $4
  18ba64: 00 00 42 94  	lhu	$2, 0x0($2)
  18ba68: e3 ff 00 10  	b	0x18b9f8 <.text+0x8b9f8>
  18ba6c: 21 10 4f 00  	addu	$2, $2, $15
  18ba70: 23 0c 05 0c  	jal	0x14308c <.text+0x4308c>
  18ba74: 00 00 00 00  	nop
  18ba78: a4 fe 00 10  	b	0x18b50c <.text+0x8b50c>
  18ba7c: 20 00 22 8e  	lw	$2, 0x20($17)
  18ba80: 82 1a 13 00  	srl	$3, $19, 0xa
  18ba84: 1c 00 24 8e  	lw	$4, 0x1c($17)
  18ba88: 24 18 62 00  	and	$3, $3, $2
  18ba8c: 18 00 22 8e  	lw	$2, 0x18($17)
  18ba90: 04 18 83 00  	sllv	$3, $3, $4
  18ba94: 21 18 62 00  	addu	$3, $3, $2
  18ba98: 36 00 02 3c  	lui	$2, 0x36
  18ba9c: 40 18 03 00  	sll	$3, $3, 0x1
  18baa0: 9f fe 00 10  	b	0x18b520 <.text+0x8b520>
  18baa4: b0 ce 42 24  	addiu	$2, $2, -0x3150 <.text+0xffffffffffefceb0>
  18baa8: 81 0f 06 0c  	jal	0x183e04 <.text+0x83e04>
  18baac: 2d 20 80 02  	move	$4, $20
  18bab0: 28 00 23 8e  	lw	$3, 0x28($17)
  18bab4: 21 18 70 00  	addu	$3, $3, $16
  18bab8: 86 fe 00 10  	b	0x18b4d4 <.text+0x8b4d4>
  18babc: 00 00 62 a0  	sb	$2, 0x0($3)
  18bac0: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  18bac4: 36 00 02 3c  	lui	$2, 0x36
  18bac8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  18bacc: ff 01 83 30  	andi	$3, $4, 0x1ff
  18bad0: 50 d4 51 24  	addiu	$17, $2, -0x2bb0 <.text+0xffffffffffefd450>
  18bad4: 30 00 b3 ff  	sd	$19, 0x30($sp)
  18bad8: 08 00 2a 8e  	lw	$10, 0x8($17)
  18badc: ff 03 82 30  	andi	$2, $4, 0x3ff
  18bae0: 2d 98 80 00  	move	$19, $4
  18bae4: 00 01 63 2c  	sltiu	$3, $3, 0x100
  18bae8: 0c 00 24 8e  	lw	$4, 0xc($17)
  18baec: 04 10 42 01  	sllv	$2, $2, $10
  18baf0: 80 00 be ff  	sd	$fp, 0x80($sp)
  18baf4: 2d f0 e0 00  	move	$fp, $7
  18baf8: 70 00 b7 ff  	sd	$23, 0x70($sp)
  18bafc: 2d b8 a0 00  	move	$23, $5
  18bb00: 60 00 b6 ff  	sd	$22, 0x60($sp)
  18bb04: 21 28 82 00  	addu	$5, $4, $2
  18bb08: 50 00 b5 ff  	sd	$21, 0x50($sp)
  18bb0c: 2d b0 20 01  	move	$22, $9
  18bb10: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18bb14: 2d a8 00 01  	move	$21, $8
  18bb18: 90 00 bf ff  	sd	$ra, 0x90($sp)
  18bb1c: 2d 90 c0 00  	move	$18, $6
  18bb20: 40 00 b4 ff  	sd	$20, 0x40($sp)
  18bb24: 03 00 60 14  	bnez	$3, 0x18bb34 <.text+0x8bb34>
  18bb28: 00 00 b0 ff  	sd	$16, 0x0($sp)
  18bb2c: 10 00 22 8e  	lw	$2, 0x10($17)
  18bb30: 21 28 a2 00  	addu	$5, $5, $2
  18bb34: ff ff a5 30  	andi	$5, $5, 0xffff
  18bb38: 28 00 22 8e  	lw	$2, 0x28($17)
  18bb3c: 06 80 45 01  	srlv	$16, $5, $10
  18bb40: 24 00 24 8e  	lw	$4, 0x24($17)
  18bb44: 21 10 50 00  	addu	$2, $2, $16
  18bb48: 80 19 10 00  	sll	$3, $16, 0x6
  18bb4c: 00 00 42 90  	lbu	$2, 0x0($2)
  18bb50: 6e 01 40 10  	beqz	$2, 0x18c10c <.text+0x8c10c>
  18bb54: 21 a0 83 00  	addu	$20, $4, $3
  18bb58: 28 00 22 8e  	lw	$2, 0x28($17)
  18bb5c: 21 10 50 00  	addu	$2, $2, $16
  18bb60: 00 00 43 90  	lbu	$3, 0x0($2)
  18bb64: 02 00 02 24  	addiu	$2, $zero, 0x2
  18bb68: 56 00 62 10  	beq	$3, $2, 0x18bcc4 <.text+0x8bcc4>
  18bb6c: 90 00 bf df  	ld	$ra, 0x90($sp)
  18bb70: 2c 00 22 92  	lbu	$2, 0x2c($17)
  18bb74: 5b 01 40 50  	beqzl	$2, 0x18c0e4 <.text+0x8c0e4>
  18bb78: 20 00 22 8e  	lw	$2, 0x20($17)
  18bb7c: 36 00 02 3c  	lui	$2, 0x36
  18bb80: 6f c2 42 90  	lbu	$2, -0x3d91($2)
  18bb84: 53 01 40 14  	bnez	$2, 0x18c0d4 <.text+0x8c0d4>
  18bb88: 00 00 00 00  	nop
  18bb8c: 20 00 22 8e  	lw	$2, 0x20($17)
  18bb90: 82 1a 13 00  	srl	$3, $19, 0xa
  18bb94: 24 18 62 00  	and	$3, $3, $2
  18bb98: 3f 00 02 3c  	lui	$2, 0x3f
  18bb9c: 40 1a 03 00  	sll	$3, $3, 0x9
  18bba0: 80 2f 42 24  	addiu	$2, $2, 0x2f80
  18bba4: 21 18 62 00  	addu	$3, $3, $2
  18bba8: 40 28 17 00  	sll	$5, $23, 0x1
  18bbac: 36 00 02 3c  	lui	$2, 0x36
  18bbb0: 00 c0 64 32  	andi	$4, $19, 0xc000
  18bbb4: c4 d4 43 ac  	sw	$3, -0x2b3c($2)
  18bbb8: 36 00 02 3c  	lui	$2, 0x36
  18bbbc: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18bbc0: 3c 00 e2 8c  	lw	$2, 0x3c($7)
  18bbc4: 08 00 e3 8c  	lw	$3, 0x8($7)
  18bbc8: 21 58 45 00  	addu	$11, $2, $5
  18bbcc: 56 00 80 14  	bnez	$4, 0x18bd28 <.text+0x8bd28>
  18bbd0: 21 28 77 00  	addu	$5, $3, $23
  18bbd4: 21 10 95 02  	addu	$2, $20, $21
  18bbd8: 21 10 52 00  	addu	$2, $2, $18
  18bbdc: 00 00 46 90  	lbu	$6, 0x0($2)
  18bbe0: 37 00 c0 10  	beqz	$6, 0x18bcc0 <.text+0x8bcc0>
  18bbe4: 40 10 06 00  	sll	$2, $6, 0x1
  18bbe8: 44 00 e3 8c  	lw	$3, 0x44($7)
  18bbec: 2d 48 c0 02  	move	$9, $22
  18bbf0: 21 10 43 00  	addu	$2, $2, $3
  18bbf4: 32 00 c0 12  	beqz	$22, 0x18bcc0 <.text+0x8bcc0>
  18bbf8: 00 00 46 94  	lhu	$6, 0x0($2)
  18bbfc: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18bc00: 28 00 00 05  	bltz	$8, 0x18bca4 <.text+0x8bca4>
  18bc04: 40 10 08 00  	sll	$2, $8, 0x1
  18bc08: 21 68 4b 00  	addu	$13, $2, $11
  18bc0c: 36 00 02 3c  	lui	$2, 0x36
  18bc10: 21 70 a8 00  	addu	$14, $5, $8
  18bc14: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18bc18: 2d 20 c0 00  	move	$4, $6
  18bc1c: 00 00 c2 91  	lbu	$2, 0x0($14)
  18bc20: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18bc24: 2b 10 43 00  	sltu	$2, $2, $3
  18bc28: 1b 00 40 10  	beqz	$2, 0x18bc98 <.text+0x8bc98>
  18bc2c: 2d 78 a0 01  	move	$15, $13
  18bc30: 48 00 e2 8c  	lw	$2, 0x48($7)
  18bc34: 21 10 02 01  	addu	$2, $8, $2
  18bc38: 21 10 a2 00  	addu	$2, $5, $2
  18bc3c: 00 00 43 90  	lbu	$3, 0x0($2)
  18bc40: 12 00 60 50  	beqzl	$3, 0x18bc8c <.text+0x8bc8c>
  18bc44: 00 00 e4 a5  	sh	$4, 0x0($15)
  18bc48: 01 00 02 24  	addiu	$2, $zero, 0x1
  18bc4c: 20 84 ca 34  	ori	$10, $6, 0x8420
  18bc50: 27 00 62 10  	beq	$3, $2, 0x18bcf0 <.text+0x8bcf0>
  18bc54: 21 04 cc 30  	andi	$12, $6, 0x421
  18bc58: 14 00 e2 8c  	lw	$2, 0x14($7)
  18bc5c: 20 00 e3 8c  	lw	$3, 0x20($7)
  18bc60: 21 10 02 01  	addu	$2, $8, $2
  18bc64: 40 10 02 00  	sll	$2, $2, 0x1
  18bc68: 21 10 4b 00  	addu	$2, $2, $11
  18bc6c: 00 00 42 94  	lhu	$2, 0x0($2)
  18bc70: de fb 42 30  	andi	$2, $2, 0xfbde
  18bc74: 23 10 42 01  	subu	$2, $10, $2
  18bc78: 43 10 02 00  	sra	$2, $2, 0x1
  18bc7c: 40 10 02 00  	sll	$2, $2, 0x1
  18bc80: 21 10 43 00  	addu	$2, $2, $3
  18bc84: 00 00 44 94  	lhu	$4, 0x0($2)
  18bc88: 00 00 e4 a5  	sh	$4, 0x0($15)
  18bc8c: 36 00 02 3c  	lui	$2, 0x36
  18bc90: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18bc94: 00 00 c2 a1  	sb	$2, 0x0($14)
  18bc98: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18bc9c: db ff 01 05  	bgez	$8, 0x18bc0c <.text+0x8bc0c>
  18bca0: fe ff ad 25  	addiu	$13, $13, -0x2 <.text+0xffffffffffeffffe>
  18bca4: 36 00 02 3c  	lui	$2, 0x36
  18bca8: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18bcac: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18bcb0: 40 18 02 00  	sll	$3, $2, 0x1
  18bcb4: 21 28 a2 00  	addu	$5, $5, $2
  18bcb8: d0 ff 20 15  	bnez	$9, 0x18bbfc <.text+0x8bbfc>
  18bcbc: 21 58 63 01  	addu	$11, $11, $3
  18bcc0: 90 00 bf df  	ld	$ra, 0x90($sp)
  18bcc4: 80 00 be df  	ld	$fp, 0x80($sp)
  18bcc8: 70 00 b7 df  	ld	$23, 0x70($sp)
  18bccc: 60 00 b6 df  	ld	$22, 0x60($sp)
  18bcd0: 50 00 b5 df  	ld	$21, 0x50($sp)
  18bcd4: 40 00 b4 df  	ld	$20, 0x40($sp)
  18bcd8: 30 00 b3 df  	ld	$19, 0x30($sp)
  18bcdc: 20 00 b2 df  	ld	$18, 0x20($sp)
  18bce0: 10 00 b1 df  	ld	$17, 0x10($sp)
  18bce4: 00 00 b0 df  	ld	$16, 0x0($sp)
  18bce8: 08 00 e0 03  	jr	$ra
  18bcec: a0 00 bd 27  	addiu	$sp, $sp, 0xa0
  18bcf0: 50 00 e2 8c  	lw	$2, 0x50($7)
  18bcf4: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  18bcf8: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18bcfc: 24 10 43 00  	and	$2, $2, $3
  18bd00: 50 00 e3 94  	lhu	$3, 0x50($7)
  18bd04: 23 10 42 01  	subu	$2, $10, $2
  18bd08: 42 10 02 00  	srl	$2, $2, 0x1
  18bd0c: 21 04 63 30  	andi	$3, $3, 0x421
  18bd10: 40 10 02 00  	sll	$2, $2, 0x1
  18bd14: 21 10 44 00  	addu	$2, $2, $4
  18bd18: 00 00 42 94  	lhu	$2, 0x0($2)
  18bd1c: 21 10 4c 00  	addu	$2, $2, $12
  18bd20: d9 ff 00 10  	b	0x18bc88 <.text+0x8bc88>
  18bd24: 23 20 43 00  	subu	$4, $2, $3
  18bd28: 00 80 62 32  	andi	$2, $19, 0x8000
  18bd2c: 4f 00 40 14  	bnez	$2, 0x18be6c <.text+0x8be6c>
  18bd30: 00 40 62 32  	andi	$2, $19, 0x4000
  18bd34: 07 00 03 24  	addiu	$3, $zero, 0x7
  18bd38: 21 10 95 02  	addu	$2, $20, $21
  18bd3c: 23 90 72 00  	subu	$18, $3, $18
  18bd40: 21 10 52 00  	addu	$2, $2, $18
  18bd44: 00 00 46 90  	lbu	$6, 0x0($2)
  18bd48: dd ff c0 10  	beqz	$6, 0x18bcc0 <.text+0x8bcc0>
  18bd4c: 40 10 06 00  	sll	$2, $6, 0x1
  18bd50: 36 00 03 3c  	lui	$3, 0x36
  18bd54: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18bd58: 2d 48 c0 02  	move	$9, $22
  18bd5c: 21 10 43 00  	addu	$2, $2, $3
  18bd60: d7 ff c0 12  	beqz	$22, 0x18bcc0 <.text+0x8bcc0>
  18bd64: 00 00 46 94  	lhu	$6, 0x0($2)
  18bd68: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18bd6c: 28 00 00 05  	bltz	$8, 0x18be10 <.text+0x8be10>
  18bd70: 40 10 08 00  	sll	$2, $8, 0x1
  18bd74: 21 68 4b 00  	addu	$13, $2, $11
  18bd78: 36 00 02 3c  	lui	$2, 0x36
  18bd7c: 21 70 a8 00  	addu	$14, $5, $8
  18bd80: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18bd84: 2d 20 c0 00  	move	$4, $6
  18bd88: 00 00 c2 91  	lbu	$2, 0x0($14)
  18bd8c: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18bd90: 2b 10 43 00  	sltu	$2, $2, $3
  18bd94: 1b 00 40 10  	beqz	$2, 0x18be04 <.text+0x8be04>
  18bd98: 2d 78 a0 01  	move	$15, $13
  18bd9c: 48 00 e2 8c  	lw	$2, 0x48($7)
  18bda0: 21 10 02 01  	addu	$2, $8, $2
  18bda4: 21 10 a2 00  	addu	$2, $5, $2
  18bda8: 00 00 43 90  	lbu	$3, 0x0($2)
  18bdac: 12 00 60 50  	beqzl	$3, 0x18bdf8 <.text+0x8bdf8>
  18bdb0: 00 00 e4 a5  	sh	$4, 0x0($15)
  18bdb4: 01 00 02 24  	addiu	$2, $zero, 0x1
  18bdb8: 20 84 ca 34  	ori	$10, $6, 0x8420
  18bdbc: 1d 00 62 10  	beq	$3, $2, 0x18be34 <.text+0x8be34>
  18bdc0: 21 04 cc 30  	andi	$12, $6, 0x421
  18bdc4: 14 00 e2 8c  	lw	$2, 0x14($7)
  18bdc8: 20 00 e3 8c  	lw	$3, 0x20($7)
  18bdcc: 21 10 02 01  	addu	$2, $8, $2
  18bdd0: 40 10 02 00  	sll	$2, $2, 0x1
  18bdd4: 21 10 4b 00  	addu	$2, $2, $11
  18bdd8: 00 00 42 94  	lhu	$2, 0x0($2)
  18bddc: de fb 42 30  	andi	$2, $2, 0xfbde
  18bde0: 23 10 42 01  	subu	$2, $10, $2
  18bde4: 43 10 02 00  	sra	$2, $2, 0x1
  18bde8: 40 10 02 00  	sll	$2, $2, 0x1
  18bdec: 21 10 43 00  	addu	$2, $2, $3
  18bdf0: 00 00 44 94  	lhu	$4, 0x0($2)
  18bdf4: 00 00 e4 a5  	sh	$4, 0x0($15)
  18bdf8: 36 00 02 3c  	lui	$2, 0x36
  18bdfc: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18be00: 00 00 c2 a1  	sb	$2, 0x0($14)
  18be04: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18be08: db ff 01 05  	bgez	$8, 0x18bd78 <.text+0x8bd78>
  18be0c: fe ff ad 25  	addiu	$13, $13, -0x2 <.text+0xffffffffffeffffe>
  18be10: 36 00 02 3c  	lui	$2, 0x36
  18be14: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18be18: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18be1c: 40 18 02 00  	sll	$3, $2, 0x1
  18be20: 21 28 a2 00  	addu	$5, $5, $2
  18be24: d0 ff 20 15  	bnez	$9, 0x18bd68 <.text+0x8bd68>
  18be28: 21 58 63 01  	addu	$11, $11, $3
  18be2c: a5 ff 00 10  	b	0x18bcc4 <.text+0x8bcc4>
  18be30: 90 00 bf df  	ld	$ra, 0x90($sp)
  18be34: 50 00 e2 8c  	lw	$2, 0x50($7)
  18be38: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  18be3c: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18be40: 24 10 43 00  	and	$2, $2, $3
  18be44: 50 00 e3 94  	lhu	$3, 0x50($7)
  18be48: 23 10 42 01  	subu	$2, $10, $2
  18be4c: 42 10 02 00  	srl	$2, $2, 0x1
  18be50: 21 04 63 30  	andi	$3, $3, 0x421
  18be54: 40 10 02 00  	sll	$2, $2, 0x1
  18be58: 21 10 44 00  	addu	$2, $2, $4
  18be5c: 00 00 42 94  	lhu	$2, 0x0($2)
  18be60: 21 10 4c 00  	addu	$2, $2, $12
  18be64: e3 ff 00 10  	b	0x18bdf4 <.text+0x8bdf4>
  18be68: 23 20 43 00  	subu	$4, $2, $3
  18be6c: 4e 00 40 10  	beqz	$2, 0x18bfa8 <.text+0x8bfa8>
  18be70: 23 10 95 02  	subu	$2, $20, $21
  18be74: 07 00 03 24  	addiu	$3, $zero, 0x7
  18be78: 23 90 72 00  	subu	$18, $3, $18
  18be7c: 21 10 52 00  	addu	$2, $2, $18
  18be80: 38 00 46 90  	lbu	$6, 0x38($2)
  18be84: 8e ff c0 10  	beqz	$6, 0x18bcc0 <.text+0x8bcc0>
  18be88: 40 10 06 00  	sll	$2, $6, 0x1
  18be8c: 36 00 03 3c  	lui	$3, 0x36
  18be90: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18be94: 2d 48 c0 02  	move	$9, $22
  18be98: 21 10 43 00  	addu	$2, $2, $3
  18be9c: 88 ff c0 12  	beqz	$22, 0x18bcc0 <.text+0x8bcc0>
  18bea0: 00 00 46 94  	lhu	$6, 0x0($2)
  18bea4: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18bea8: 28 00 00 05  	bltz	$8, 0x18bf4c <.text+0x8bf4c>
  18beac: 40 10 08 00  	sll	$2, $8, 0x1
  18beb0: 21 68 4b 00  	addu	$13, $2, $11
  18beb4: 36 00 02 3c  	lui	$2, 0x36
  18beb8: 21 70 a8 00  	addu	$14, $5, $8
  18bebc: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18bec0: 2d 20 c0 00  	move	$4, $6
  18bec4: 00 00 c2 91  	lbu	$2, 0x0($14)
  18bec8: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18becc: 2b 10 43 00  	sltu	$2, $2, $3
  18bed0: 1b 00 40 10  	beqz	$2, 0x18bf40 <.text+0x8bf40>
  18bed4: 2d 78 a0 01  	move	$15, $13
  18bed8: 48 00 e2 8c  	lw	$2, 0x48($7)
  18bedc: 21 10 02 01  	addu	$2, $8, $2
  18bee0: 21 10 a2 00  	addu	$2, $5, $2
  18bee4: 00 00 43 90  	lbu	$3, 0x0($2)
  18bee8: 12 00 60 50  	beqzl	$3, 0x18bf34 <.text+0x8bf34>
  18beec: 00 00 e4 a5  	sh	$4, 0x0($15)
  18bef0: 01 00 02 24  	addiu	$2, $zero, 0x1
  18bef4: 20 84 ca 34  	ori	$10, $6, 0x8420
  18bef8: 1d 00 62 10  	beq	$3, $2, 0x18bf70 <.text+0x8bf70>
  18befc: 21 04 cc 30  	andi	$12, $6, 0x421
  18bf00: 14 00 e2 8c  	lw	$2, 0x14($7)
  18bf04: 20 00 e3 8c  	lw	$3, 0x20($7)
  18bf08: 21 10 02 01  	addu	$2, $8, $2
  18bf0c: 40 10 02 00  	sll	$2, $2, 0x1
  18bf10: 21 10 4b 00  	addu	$2, $2, $11
  18bf14: 00 00 42 94  	lhu	$2, 0x0($2)
  18bf18: de fb 42 30  	andi	$2, $2, 0xfbde
  18bf1c: 23 10 42 01  	subu	$2, $10, $2
  18bf20: 43 10 02 00  	sra	$2, $2, 0x1
  18bf24: 40 10 02 00  	sll	$2, $2, 0x1
  18bf28: 21 10 43 00  	addu	$2, $2, $3
  18bf2c: 00 00 44 94  	lhu	$4, 0x0($2)
  18bf30: 00 00 e4 a5  	sh	$4, 0x0($15)
  18bf34: 36 00 02 3c  	lui	$2, 0x36
  18bf38: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18bf3c: 00 00 c2 a1  	sb	$2, 0x0($14)
  18bf40: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18bf44: db ff 01 05  	bgez	$8, 0x18beb4 <.text+0x8beb4>
  18bf48: fe ff ad 25  	addiu	$13, $13, -0x2 <.text+0xffffffffffeffffe>
  18bf4c: 36 00 02 3c  	lui	$2, 0x36
  18bf50: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18bf54: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18bf58: 40 18 02 00  	sll	$3, $2, 0x1
  18bf5c: 21 28 a2 00  	addu	$5, $5, $2
  18bf60: d0 ff 20 15  	bnez	$9, 0x18bea4 <.text+0x8bea4>
  18bf64: 21 58 63 01  	addu	$11, $11, $3
  18bf68: 56 ff 00 10  	b	0x18bcc4 <.text+0x8bcc4>
  18bf6c: 90 00 bf df  	ld	$ra, 0x90($sp)
  18bf70: 50 00 e2 8c  	lw	$2, 0x50($7)
  18bf74: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  18bf78: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18bf7c: 24 10 43 00  	and	$2, $2, $3
  18bf80: 50 00 e3 94  	lhu	$3, 0x50($7)
  18bf84: 23 10 42 01  	subu	$2, $10, $2
  18bf88: 42 10 02 00  	srl	$2, $2, 0x1
  18bf8c: 21 04 63 30  	andi	$3, $3, 0x421
  18bf90: 40 10 02 00  	sll	$2, $2, 0x1
  18bf94: 21 10 44 00  	addu	$2, $2, $4
  18bf98: 00 00 42 94  	lhu	$2, 0x0($2)
  18bf9c: 21 10 4c 00  	addu	$2, $2, $12
  18bfa0: e3 ff 00 10  	b	0x18bf30 <.text+0x8bf30>
  18bfa4: 23 20 43 00  	subu	$4, $2, $3
  18bfa8: 21 10 52 00  	addu	$2, $2, $18
  18bfac: 38 00 46 90  	lbu	$6, 0x38($2)
  18bfb0: 43 ff c0 10  	beqz	$6, 0x18bcc0 <.text+0x8bcc0>
  18bfb4: 40 10 06 00  	sll	$2, $6, 0x1
  18bfb8: 36 00 03 3c  	lui	$3, 0x36
  18bfbc: c4 d4 63 8c  	lw	$3, -0x2b3c($3)
  18bfc0: 2d 48 c0 02  	move	$9, $22
  18bfc4: 21 10 43 00  	addu	$2, $2, $3
  18bfc8: 3d ff c0 12  	beqz	$22, 0x18bcc0 <.text+0x8bcc0>
  18bfcc: 00 00 46 94  	lhu	$6, 0x0($2)
  18bfd0: ff ff c8 27  	addiu	$8, $fp, -0x1 <.text+0xffffffffffefffff>
  18bfd4: 28 00 00 05  	bltz	$8, 0x18c078 <.text+0x8c078>
  18bfd8: 40 10 08 00  	sll	$2, $8, 0x1
  18bfdc: 21 68 4b 00  	addu	$13, $2, $11
  18bfe0: 36 00 02 3c  	lui	$2, 0x36
  18bfe4: 21 70 a8 00  	addu	$14, $5, $8
  18bfe8: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  18bfec: 2d 20 c0 00  	move	$4, $6
  18bff0: 00 00 c2 91  	lbu	$2, 0x0($14)
  18bff4: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  18bff8: 2b 10 43 00  	sltu	$2, $2, $3
  18bffc: 1b 00 40 10  	beqz	$2, 0x18c06c <.text+0x8c06c>
  18c000: 2d 78 a0 01  	move	$15, $13
  18c004: 48 00 e2 8c  	lw	$2, 0x48($7)
  18c008: 21 10 02 01  	addu	$2, $8, $2
  18c00c: 21 10 a2 00  	addu	$2, $5, $2
  18c010: 00 00 43 90  	lbu	$3, 0x0($2)
  18c014: 12 00 60 50  	beqzl	$3, 0x18c060 <.text+0x8c060>
  18c018: 00 00 e4 a5  	sh	$4, 0x0($15)
  18c01c: 01 00 02 24  	addiu	$2, $zero, 0x1
  18c020: 20 84 ca 34  	ori	$10, $6, 0x8420
  18c024: 1d 00 62 10  	beq	$3, $2, 0x18c09c <.text+0x8c09c>
  18c028: 21 04 cc 30  	andi	$12, $6, 0x421
  18c02c: 14 00 e2 8c  	lw	$2, 0x14($7)
  18c030: 20 00 e3 8c  	lw	$3, 0x20($7)
  18c034: 21 10 02 01  	addu	$2, $8, $2
  18c038: 40 10 02 00  	sll	$2, $2, 0x1
  18c03c: 21 10 4b 00  	addu	$2, $2, $11
  18c040: 00 00 42 94  	lhu	$2, 0x0($2)
  18c044: de fb 42 30  	andi	$2, $2, 0xfbde
  18c048: 23 10 42 01  	subu	$2, $10, $2
  18c04c: 43 10 02 00  	sra	$2, $2, 0x1
  18c050: 40 10 02 00  	sll	$2, $2, 0x1
  18c054: 21 10 43 00  	addu	$2, $2, $3
  18c058: 00 00 44 94  	lhu	$4, 0x0($2)
  18c05c: 00 00 e4 a5  	sh	$4, 0x0($15)
  18c060: 36 00 02 3c  	lui	$2, 0x36
  18c064: cd d4 42 90  	lbu	$2, -0x2b33($2)
  18c068: 00 00 c2 a1  	sb	$2, 0x0($14)
  18c06c: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  18c070: db ff 01 05  	bgez	$8, 0x18bfe0 <.text+0x8bfe0>
  18c074: fe ff ad 25  	addiu	$13, $13, -0x2 <.text+0xffffffffffeffffe>
  18c078: 36 00 02 3c  	lui	$2, 0x36
  18c07c: ff ff 29 25  	addiu	$9, $9, -0x1 <.text+0xffffffffffefffff>
  18c080: b0 d4 42 8c  	lw	$2, -0x2b50($2)
  18c084: 40 18 02 00  	sll	$3, $2, 0x1
  18c088: 21 28 a2 00  	addu	$5, $5, $2
  18c08c: d0 ff 20 15  	bnez	$9, 0x18bfd0 <.text+0x8bfd0>
  18c090: 21 58 63 01  	addu	$11, $11, $3
  18c094: 0b ff 00 10  	b	0x18bcc4 <.text+0x8bcc4>
  18c098: 90 00 bf df  	ld	$ra, 0x90($sp)
  18c09c: 50 00 e2 8c  	lw	$2, 0x50($7)
  18c0a0: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  18c0a4: 1c 00 e4 8c  	lw	$4, 0x1c($7)
  18c0a8: 24 10 43 00  	and	$2, $2, $3
  18c0ac: 50 00 e3 94  	lhu	$3, 0x50($7)
  18c0b0: 23 10 42 01  	subu	$2, $10, $2
  18c0b4: 42 10 02 00  	srl	$2, $2, 0x1
  18c0b8: 21 04 63 30  	andi	$3, $3, 0x421
  18c0bc: 40 10 02 00  	sll	$2, $2, 0x1
  18c0c0: 21 10 44 00  	addu	$2, $2, $4
  18c0c4: 00 00 42 94  	lhu	$2, 0x0($2)
  18c0c8: 21 10 4c 00  	addu	$2, $2, $12
  18c0cc: e3 ff 00 10  	b	0x18c05c <.text+0x8c05c>
  18c0d0: 23 20 43 00  	subu	$4, $2, $3
  18c0d4: 23 0c 05 0c  	jal	0x14308c <.text+0x4308c>
  18c0d8: 00 00 00 00  	nop
  18c0dc: ac fe 00 10  	b	0x18bb90 <.text+0x8bb90>
  18c0e0: 20 00 22 8e  	lw	$2, 0x20($17)
  18c0e4: 82 1a 13 00  	srl	$3, $19, 0xa
  18c0e8: 1c 00 24 8e  	lw	$4, 0x1c($17)
  18c0ec: 24 18 62 00  	and	$3, $3, $2
  18c0f0: 18 00 22 8e  	lw	$2, 0x18($17)
  18c0f4: 04 18 83 00  	sllv	$3, $3, $4
  18c0f8: 21 18 62 00  	addu	$3, $3, $2
  18c0fc: 36 00 02 3c  	lui	$2, 0x36
  18c100: 40 18 03 00  	sll	$3, $3, 0x1
  18c104: a7 fe 00 10  	b	0x18bba4 <.text+0x8bba4>
  18c108: b0 ce 42 24  	addiu	$2, $2, -0x3150 <.text+0xffffffffffefceb0>
  18c10c: 81 0f 06 0c  	jal	0x183e04 <.text+0x83e04>
  18c110: 2d 20 80 02  	move	$4, $20
  18c114: 28 00 23 8e  	lw	$3, 0x28($17)
  18c118: 21 18 70 00  	addu	$3, $3, $16
  18c11c: 8e fe 00 10  	b	0x18bb58 <.text+0x8bb58>
  18c120: 00 00 62 a0  	sb	$2, 0x0($3)
