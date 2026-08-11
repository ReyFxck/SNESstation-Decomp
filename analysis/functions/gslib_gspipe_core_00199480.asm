  199480: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  199484: 00 10 a2 2c  	sltiu	$2, $5, 0x1000
  199488: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19948c: 2d 80 80 00  	move	$16, $4
  199490: 1c 00 04 3c  	lui	$4, 0x1c
  199494: 10 00 b1 ff  	sd	$17, 0x10($sp)
  199498: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19949c: e0 a2 84 24  	addiu	$4, $4, -0x5d20 <.text+0xffffffffffefa2e0>
  1994a0: 2d 88 a0 00  	move	$17, $5
  1994a4: 13 00 40 14  	bnez	$2, 0x1994f4 <.text+0x994f4>
  1994a8: 08 00 00 ae  	sw	$zero, 0x8($16)
  1994ac: 18 00 02 8e  	lw	$2, 0x18($16)
  1994b0: 33 00 40 14  	bnez	$2, 0x199580 <.text+0x99580>
  1994b4: 2d 20 40 00  	move	$4, $2
  1994b8: 40 00 04 24  	addiu	$4, $zero, 0x40
  1994bc: a6 79 06 0c  	jal	0x19e698 <.text+0x9e698>
  1994c0: 2d 28 20 02  	move	$5, $17
  1994c4: 1c 00 04 3c  	lui	$4, 0x1c
  1994c8: 2d 18 40 00  	move	$3, $2
  1994cc: 0f 00 46 30  	andi	$6, $2, 0xf
  1994d0: 18 a3 84 24  	addiu	$4, $4, -0x5ce8 <.text+0xffffffffffefa318>
  1994d4: 07 00 40 10  	beqz	$2, 0x1994f4 <.text+0x994f4>
  1994d8: 18 00 02 ae  	sw	$2, 0x18($16)
  1994dc: 02 11 11 00  	srl	$2, $17, 0x4
  1994e0: 2d 28 60 00  	move	$5, $3
  1994e4: 0a 00 c0 10  	beqz	$6, 0x199510 <.text+0x99510>
  1994e8: 2d 20 00 02  	move	$4, $16
  1994ec: 1c 00 04 3c  	lui	$4, 0x1c
  1994f0: 40 a3 84 24  	addiu	$4, $4, -0x5cc0 <.text+0xffffffffffefa340>
  1994f4: 05 79 06 0c  	jal	0x19e414 <.text+0x9e414>
  1994f8: 00 00 00 00  	nop
  1994fc: 20 00 bf df  	ld	$ra, 0x20($sp)
  199500: 10 00 b1 df  	ld	$17, 0x10($sp)
  199504: 00 00 b0 df  	ld	$16, 0x0($sp)
  199508: 08 00 e0 03  	jr	$ra
  19950c: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  199510: c0 10 02 00  	sll	$2, $2, 0x3
  199514: 00 00 03 ae  	sw	$3, 0x0($16)
  199518: 21 10 62 00  	addu	$2, $3, $2
  19951c: 08 00 11 ae  	sw	$17, 0x8($16)
  199520: 0e 66 06 0c  	jal	0x199838 <.text+0x99838>
  199524: 04 00 02 ae  	sw	$2, 0x4($16)
  199528: 2d 20 00 02  	move	$4, $16
  19952c: 00 00 06 8e  	lw	$6, 0x0($16)
  199530: 44 00 03 3c  	lui	$3, 0x44
  199534: 44 00 02 3c  	lui	$2, 0x44
  199538: e0 ec 67 8c  	lw	$7, -0x1320($3)
  19953c: dc ec 48 8c  	lw	$8, -0x1324($2)
  199540: 01 00 05 24  	addiu	$5, $zero, 0x1
  199544: 10 00 c2 24  	addiu	$2, $6, 0x10
  199548: 2c 00 07 ae  	sw	$7, 0x2c($16)
  19954c: 14 00 02 ae  	sw	$2, 0x14($16)
  199550: 28 00 08 ae  	sw	$8, 0x28($16)
  199554: 10 00 06 ae  	sw	$6, 0x10($16)
  199558: e0 66 06 0c  	jal	0x199b80 <.text+0x99b80>
  19955c: 0c 00 06 ae  	sw	$6, 0xc($16)
  199560: 2d 28 00 00  	move	$5, $zero
  199564: aa 66 06 0c  	jal	0x199aa8 <.text+0x99aa8>
  199568: 2d 20 00 02  	move	$4, $16
  19956c: 2d 28 00 00  	move	$5, $zero
  199570: 60 69 06 0c  	jal	0x19a580 <.text+0x9a580>
  199574: 2d 20 00 02  	move	$4, $16
  199578: e1 ff 00 10  	b	0x199500 <.text+0x99500>
  19957c: 20 00 bf df  	ld	$ra, 0x20($sp)
  199580: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  199584: 00 00 00 00  	nop
  199588: cc ff 00 10  	b	0x1994bc <.text+0x994bc>
  19958c: 40 00 04 24  	addiu	$4, $zero, 0x40
  199590: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  199594: 00 10 a2 2c  	sltiu	$2, $5, 0x1000
  199598: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19959c: 2d 80 80 00  	move	$16, $4
  1995a0: 1c 00 04 3c  	lui	$4, 0x1c
  1995a4: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1995a8: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1995ac: e0 a2 84 24  	addiu	$4, $4, -0x5d20 <.text+0xffffffffffefa2e0>
  1995b0: 2d 88 a0 00  	move	$17, $5
  1995b4: 13 00 40 14  	bnez	$2, 0x199604 <.text+0x99604>
  1995b8: 08 00 00 ae  	sw	$zero, 0x8($16)
  1995bc: 18 00 02 8e  	lw	$2, 0x18($16)
  1995c0: 33 00 40 14  	bnez	$2, 0x199690 <.text+0x99690>
  1995c4: 2d 20 40 00  	move	$4, $2
  1995c8: 40 00 04 24  	addiu	$4, $zero, 0x40
  1995cc: a6 79 06 0c  	jal	0x19e698 <.text+0x9e698>
  1995d0: 2d 28 20 02  	move	$5, $17
  1995d4: 1c 00 04 3c  	lui	$4, 0x1c
  1995d8: 2d 18 40 00  	move	$3, $2
  1995dc: 0f 00 46 30  	andi	$6, $2, 0xf
  1995e0: 18 a3 84 24  	addiu	$4, $4, -0x5ce8 <.text+0xffffffffffefa318>
  1995e4: 07 00 40 10  	beqz	$2, 0x199604 <.text+0x99604>
  1995e8: 18 00 02 ae  	sw	$2, 0x18($16)
  1995ec: 02 11 11 00  	srl	$2, $17, 0x4
  1995f0: 2d 28 60 00  	move	$5, $3
  1995f4: 0a 00 c0 10  	beqz	$6, 0x199620 <.text+0x99620>
  1995f8: 2d 20 00 02  	move	$4, $16
  1995fc: 1c 00 04 3c  	lui	$4, 0x1c
  199600: 40 a3 84 24  	addiu	$4, $4, -0x5cc0 <.text+0xffffffffffefa340>
  199604: 05 79 06 0c  	jal	0x19e414 <.text+0x9e414>
  199608: 00 00 00 00  	nop
  19960c: 20 00 bf df  	ld	$ra, 0x20($sp)
  199610: 10 00 b1 df  	ld	$17, 0x10($sp)
  199614: 00 00 b0 df  	ld	$16, 0x0($sp)
  199618: 08 00 e0 03  	jr	$ra
  19961c: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  199620: c0 10 02 00  	sll	$2, $2, 0x3
  199624: 00 00 03 ae  	sw	$3, 0x0($16)
  199628: 21 10 62 00  	addu	$2, $3, $2
  19962c: 08 00 11 ae  	sw	$17, 0x8($16)
  199630: 0e 66 06 0c  	jal	0x199838 <.text+0x99838>
  199634: 04 00 02 ae  	sw	$2, 0x4($16)
  199638: 2d 20 00 02  	move	$4, $16
  19963c: 00 00 06 8e  	lw	$6, 0x0($16)
  199640: 44 00 03 3c  	lui	$3, 0x44
  199644: 44 00 02 3c  	lui	$2, 0x44
  199648: e0 ec 67 8c  	lw	$7, -0x1320($3)
  19964c: dc ec 48 8c  	lw	$8, -0x1324($2)
  199650: 01 00 05 24  	addiu	$5, $zero, 0x1
  199654: 10 00 c2 24  	addiu	$2, $6, 0x10
  199658: 2c 00 07 ae  	sw	$7, 0x2c($16)
  19965c: 14 00 02 ae  	sw	$2, 0x14($16)
  199660: 28 00 08 ae  	sw	$8, 0x28($16)
  199664: 10 00 06 ae  	sw	$6, 0x10($16)
  199668: e0 66 06 0c  	jal	0x199b80 <.text+0x99b80>
  19966c: 0c 00 06 ae  	sw	$6, 0xc($16)
  199670: 2d 28 00 00  	move	$5, $zero
  199674: aa 66 06 0c  	jal	0x199aa8 <.text+0x99aa8>
  199678: 2d 20 00 02  	move	$4, $16
  19967c: 2d 28 00 00  	move	$5, $zero
  199680: 60 69 06 0c  	jal	0x19a580 <.text+0x9a580>
  199684: 2d 20 00 02  	move	$4, $16
  199688: e1 ff 00 10  	b	0x199610 <.text+0x99610>
  19968c: 20 00 bf df  	ld	$ra, 0x20($sp)
  199690: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  199694: 00 00 00 00  	nop
  199698: cc ff 00 10  	b	0x1995cc <.text+0x995cc>
  19969c: 40 00 04 24  	addiu	$4, $zero, 0x40
  1996a0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1996a4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1996a8: 18 00 82 8c  	lw	$2, 0x18($4)
  1996ac: 04 00 40 14  	bnez	$2, 0x1996c0 <.text+0x996c0>
  1996b0: 2d 20 40 00  	move	$4, $2
  1996b4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1996b8: 08 00 e0 03  	jr	$ra
  1996bc: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1996c0: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1996c4: 00 00 00 00  	nop
  1996c8: fb ff 00 10  	b	0x1996b8 <.text+0x996b8>
  1996cc: 00 00 bf df  	ld	$ra, 0x0($sp)
  1996d0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1996d4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1996d8: 18 00 82 8c  	lw	$2, 0x18($4)
  1996dc: 04 00 40 14  	bnez	$2, 0x1996f0 <.text+0x996f0>
  1996e0: 2d 20 40 00  	move	$4, $2
  1996e4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1996e8: 08 00 e0 03  	jr	$ra
  1996ec: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1996f0: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1996f4: 00 00 00 00  	nop
  1996f8: fb ff 00 10  	b	0x1996e8 <.text+0x996e8>
  1996fc: 00 00 bf df  	ld	$ra, 0x0($sp)
  199700: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199704: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199708: d0 65 06 0c  	jal	0x199740 <.text+0x99740>
  19970c: 00 00 00 00  	nop
  199710: 00 00 bf df  	ld	$ra, 0x0($sp)
  199714: 08 00 e0 03  	jr	$ra
  199718: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19971c: 00 00 00 00  	nop
  199720: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199724: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199728: d0 65 06 0c  	jal	0x199740 <.text+0x99740>
  19972c: 00 00 00 00  	nop
  199730: 00 00 bf df  	ld	$ra, 0x0($sp)
  199734: 08 00 e0 03  	jr	$ra
  199738: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  19973c: 00 00 00 00  	nop
  199740: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  199744: 10 00 b1 ff  	sd	$17, 0x10($sp)
  199748: 2d 88 a0 00  	move	$17, $5
  19974c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199750: 2d 80 80 00  	move	$16, $4
  199754: 2c 00 a4 10  	beq	$5, $4, 0x199808 <.text+0x99808>
  199758: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19975c: 28 00 a5 8c  	lw	$5, 0x28($5)
  199760: 2c 00 26 8e  	lw	$6, 0x2c($17)
  199764: 1c 00 27 8e  	lw	$7, 0x1c($17)
  199768: 20 00 28 8e  	lw	$8, 0x20($17)
  19976c: 08 00 22 8e  	lw	$2, 0x8($17)
  199770: 18 00 83 8c  	lw	$3, 0x18($4)
  199774: 08 00 82 ac  	sw	$2, 0x8($4)
  199778: 28 00 05 ae  	sw	$5, 0x28($16)
  19977c: 2d 20 60 00  	move	$4, $3
  199780: 2c 00 06 ae  	sw	$6, 0x2c($16)
  199784: 1c 00 07 ae  	sw	$7, 0x1c($16)
  199788: 25 00 60 14  	bnez	$3, 0x199820 <.text+0x99820>
  19978c: 20 00 08 ae  	sw	$8, 0x20($16)
  199790: 08 00 05 8e  	lw	$5, 0x8($16)
  199794: a6 79 06 0c  	jal	0x19e698 <.text+0x9e698>
  199798: 40 00 04 24  	addiu	$4, $zero, 0x40
  19979c: 08 00 06 8e  	lw	$6, 0x8($16)
  1997a0: 18 00 02 ae  	sw	$2, 0x18($16)
  1997a4: 2d 20 40 00  	move	$4, $2
  1997a8: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  1997ac: 18 00 25 8e  	lw	$5, 0x18($17)
  1997b0: 18 00 02 8e  	lw	$2, 0x18($16)
  1997b4: 18 00 23 8e  	lw	$3, 0x18($17)
  1997b8: 14 00 28 8e  	lw	$8, 0x14($17)
  1997bc: 00 00 26 8e  	lw	$6, 0x0($17)
  1997c0: 04 00 27 8e  	lw	$7, 0x4($17)
  1997c4: 23 40 03 01  	subu	$8, $8, $3
  1997c8: 0c 00 25 8e  	lw	$5, 0xc($17)
  1997cc: 23 30 c3 00  	subu	$6, $6, $3
  1997d0: 10 00 24 8e  	lw	$4, 0x10($17)
  1997d4: 23 38 e3 00  	subu	$7, $7, $3
  1997d8: 23 28 a3 00  	subu	$5, $5, $3
  1997dc: 21 40 48 00  	addu	$8, $2, $8
  1997e0: 23 20 83 00  	subu	$4, $4, $3
  1997e4: 21 30 46 00  	addu	$6, $2, $6
  1997e8: 21 38 47 00  	addu	$7, $2, $7
  1997ec: 21 28 45 00  	addu	$5, $2, $5
  1997f0: 21 10 44 00  	addu	$2, $2, $4
  1997f4: 00 00 06 ae  	sw	$6, 0x0($16)
  1997f8: 04 00 07 ae  	sw	$7, 0x4($16)
  1997fc: 0c 00 05 ae  	sw	$5, 0xc($16)
  199800: 10 00 02 ae  	sw	$2, 0x10($16)
  199804: 14 00 08 ae  	sw	$8, 0x14($16)
  199808: 2d 10 00 02  	move	$2, $16
  19980c: 20 00 bf df  	ld	$ra, 0x20($sp)
  199810: 10 00 b1 df  	ld	$17, 0x10($sp)
  199814: 00 00 b0 df  	ld	$16, 0x0($sp)
  199818: 08 00 e0 03  	jr	$ra
  19981c: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  199820: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  199824: 00 00 00 00  	nop
  199828: da ff 00 10  	b	0x199794 <.text+0x99794>
  19982c: 08 00 05 8e  	lw	$5, 0x8($16)
  199830: 08 00 e0 03  	jr	$ra
  199834: 08 00 82 8c  	lw	$2, 0x8($4)
  199838: 00 70 02 3c  	lui	$2, 0x7000
  19983c: 08 00 a0 fc  	sd	$zero, 0x8($5)
  199840: 08 00 e0 03  	jr	$ra
  199844: 00 00 a2 fc  	sd	$2, 0x0($5)
  199848: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19984c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  199850: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199854: 2d 80 80 00  	move	$16, $4
  199858: aa 66 06 0c  	jal	0x199aa8 <.text+0x99aa8>
  19985c: 24 00 85 8c  	lw	$5, 0x24($4)
  199860: 1c 00 05 8e  	lw	$5, 0x1c($16)
  199864: e0 66 06 0c  	jal	0x199b80 <.text+0x99b80>
  199868: 2d 20 00 02  	move	$4, $16
  19986c: 28 00 05 8e  	lw	$5, 0x28($16)
  199870: 2c 00 06 8e  	lw	$6, 0x2c($16)
  199874: 96 67 06 0c  	jal	0x199e58 <.text+0x99e58>
  199878: 2d 20 00 02  	move	$4, $16
  19987c: 3e 66 06 0c  	jal	0x1998f8 <.text+0x998f8>
  199880: 2d 20 00 02  	move	$4, $16
  199884: 00 00 b0 df  	ld	$16, 0x0($sp)
  199888: 10 00 bf df  	ld	$ra, 0x10($sp)
  19988c: 08 00 e0 03  	jr	$ra
  199890: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  199894: 00 00 00 00  	nop
  199898: 08 00 83 8c  	lw	$3, 0x8($4)
  19989c: 0c 00 82 8c  	lw	$2, 0xc($4)
  1998a0: 42 18 03 00  	srl	$3, $3, 0x1
  1998a4: 14 00 85 8c  	lw	$5, 0x14($4)
  1998a8: 21 10 43 00  	addu	$2, $2, $3
  1998ac: 08 00 e0 03  	jr	$ra
  1998b0: 23 10 45 00  	subu	$2, $2, $5
  1998b4: 00 00 00 00  	nop
  1998b8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1998bc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1998c0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1998c4: 26 66 06 0c  	jal	0x199898 <.text+0x99898>
  1998c8: 2d 80 80 00  	move	$16, $4
  1998cc: 90 00 42 2c  	sltiu	$2, $2, 0x90
  1998d0: 05 00 40 14  	bnez	$2, 0x1998e8 <.text+0x998e8>
  1998d4: 2d 20 00 02  	move	$4, $16
  1998d8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1998dc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1998e0: 08 00 e0 03  	jr	$ra
  1998e4: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1998e8: 3e 66 06 0c  	jal	0x1998f8 <.text+0x998f8>
  1998ec: 00 00 00 00  	nop
  1998f0: fa ff 00 10  	b	0x1998dc <.text+0x998dc>
  1998f4: 10 00 bf df  	ld	$ra, 0x10($sp)
  1998f8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1998fc: 10 00 bf ff  	sd	$ra, 0x10($sp)
  199900: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199904: 90 6f 06 0c  	jal	0x19be40 <.text+0x9be40>
  199908: 2d 80 80 00  	move	$16, $4
  19990c: ac 73 06 0c  	jal	0x19ceb0 <.text+0x9ceb0>
  199910: 2d 20 00 00  	move	$4, $zero
  199914: 88 6f 06 0c  	jal	0x19be20 <.text+0x9be20>
  199918: 0c 00 04 8e  	lw	$4, 0xc($16)
  19991c: 0c 00 02 8e  	lw	$2, 0xc($16)
  199920: 00 00 03 8e  	lw	$3, 0x0($16)
  199924: 2d 20 00 02  	move	$4, $16
  199928: 0d 00 43 10  	beq	$2, $3, 0x199960 <.text+0x99960>
  19992c: 2d 28 60 00  	move	$5, $3
  199930: 0e 66 06 0c  	jal	0x199838 <.text+0x99838>
  199934: 00 00 00 00  	nop
  199938: 00 00 02 8e  	lw	$2, 0x0($16)
  19993c: 10 00 43 24  	addiu	$3, $2, 0x10
  199940: 10 00 02 ae  	sw	$2, 0x10($16)
  199944: 14 00 03 ae  	sw	$3, 0x14($16)
  199948: 0c 00 02 ae  	sw	$2, 0xc($16)
  19994c: 10 00 bf df  	ld	$ra, 0x10($sp)
  199950: 00 00 b0 df  	ld	$16, 0x0($sp)
  199954: 08 00 e0 03  	jr	$ra
  199958: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  19995c: 00 00 00 00  	nop
  199960: 0e 66 06 0c  	jal	0x199838 <.text+0x99838>
  199964: 04 00 05 8e  	lw	$5, 0x4($16)
  199968: f4 ff 00 10  	b	0x19993c <.text+0x9993c>
  19996c: 04 00 02 8e  	lw	$2, 0x4($16)
  199970: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  199974: 10 00 bf ff  	sd	$ra, 0x10($sp)
  199978: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19997c: 90 6f 06 0c  	jal	0x19be40 <.text+0x9be40>
  199980: 2d 80 80 00  	move	$16, $4
  199984: b0 73 06 0c  	jal	0x19cec0 <.text+0x9cec0>
  199988: 2d 20 00 00  	move	$4, $zero
  19998c: 88 6f 06 0c  	jal	0x19be20 <.text+0x9be20>
  199990: 0c 00 04 8e  	lw	$4, 0xc($16)
  199994: 0c 00 02 8e  	lw	$2, 0xc($16)
  199998: 00 00 03 8e  	lw	$3, 0x0($16)
  19999c: 2d 20 00 02  	move	$4, $16
  1999a0: 0d 00 43 10  	beq	$2, $3, 0x1999d8 <.text+0x999d8>
  1999a4: 2d 28 60 00  	move	$5, $3
  1999a8: 0e 66 06 0c  	jal	0x199838 <.text+0x99838>
  1999ac: 00 00 00 00  	nop
  1999b0: 00 00 02 8e  	lw	$2, 0x0($16)
  1999b4: 10 00 43 24  	addiu	$3, $2, 0x10
  1999b8: 10 00 02 ae  	sw	$2, 0x10($16)
  1999bc: 14 00 03 ae  	sw	$3, 0x14($16)
  1999c0: 0c 00 02 ae  	sw	$2, 0xc($16)
  1999c4: 10 00 bf df  	ld	$ra, 0x10($sp)
  1999c8: 00 00 b0 df  	ld	$16, 0x0($sp)
  1999cc: 08 00 e0 03  	jr	$ra
  1999d0: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1999d4: 00 00 00 00  	nop
  1999d8: 0e 66 06 0c  	jal	0x199838 <.text+0x99838>
  1999dc: 04 00 05 8e  	lw	$5, 0x4($16)
  1999e0: f4 ff 00 10  	b	0x1999b4 <.text+0x999b4>
  1999e4: 04 00 02 8e  	lw	$2, 0x4($16)
  1999e8: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1999ec: ff ff 02 3c  	lui	$2, 0xffff
  1999f0: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1999f4: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1999f8: 38 36 06 00  	dsll	$6, $6, 0x18
  1999fc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199a00: 2d 88 e0 00  	move	$17, $7
  199a04: 01 00 31 32  	andi	$17, $17, 0x1
  199a08: 7a 2b 05 00  	dsrl	$5, $5, 0xd
  199a0c: 10 00 88 8c  	lw	$8, 0x10($4)
  199a10: 25 28 a6 00  	or	$5, $5, $6
  199a14: 2d 80 80 00  	move	$16, $4
  199a18: 00 00 07 dd  	ld	$7, 0x0($8)
  199a1c: ff ff e3 30  	andi	$3, $7, 0xffff
  199a20: 24 38 e2 00  	and	$7, $7, $2
  199a24: 02 00 63 64  	daddiu	$3, $3, 0x2
  199a28: 01 00 22 3a  	xori	$2, $17, 0x1
  199a2c: 25 38 e3 00  	or	$7, $7, $3
  199a30: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199a34: 00 00 07 fd  	sd	$7, 0x0($8)
  199a38: 00 10 03 3c  	lui	$3, 0x1000
  199a3c: 3c 18 03 00  	dsll32	$3, $3, 0x0
  199a40: 01 80 63 34  	ori	$3, $3, 0x8001
  199a44: 25 28 a2 00  	or	$5, $5, $2
  199a48: 3c 88 11 00  	dsll32	$17, $17, 0x0
  199a4c: 3f 88 11 00  	dsra32	$17, $17, 0x0
  199a50: 14 00 87 8c  	lw	$7, 0x14($4)
  199a54: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199a58: 00 00 e3 fc  	sd	$3, 0x0($7)
  199a5c: 14 00 86 8c  	lw	$6, 0x14($4)
  199a60: 08 00 c2 fc  	sd	$2, 0x8($6)
  199a64: 14 00 83 8c  	lw	$3, 0x14($4)
  199a68: 10 00 65 fc  	sd	$5, 0x10($3)
  199a6c: 4e 00 03 24  	addiu	$3, $zero, 0x4e
  199a70: 14 00 85 8c  	lw	$5, 0x14($4)
  199a74: 18 00 a3 fc  	sd	$3, 0x18($5)
  199a78: 14 00 82 8c  	lw	$2, 0x14($4)
  199a7c: 20 00 42 24  	addiu	$2, $2, 0x20
  199a80: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199a84: 14 00 82 ac  	sw	$2, 0x14($4)
  199a88: 20 00 bf df  	ld	$ra, 0x20($sp)
  199a8c: 44 00 02 3c  	lui	$2, 0x44
  199a90: d4 ec 51 ac  	sw	$17, -0x132c($2)
  199a94: 20 00 11 ae  	sw	$17, 0x20($16)
  199a98: 10 00 b1 df  	ld	$17, 0x10($sp)
  199a9c: 00 00 b0 df  	ld	$16, 0x0($sp)
  199aa0: 08 00 e0 03  	jr	$ra
  199aa4: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  199aa8: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  199aac: 44 00 02 3c  	lui	$2, 0x44
  199ab0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  199ab4: 01 00 b1 30  	andi	$17, $5, 0x1
  199ab8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199abc: 20 00 bf ff  	sd	$ra, 0x20($sp)
  199ac0: d4 ec 43 8c  	lw	$3, -0x132c($2)
  199ac4: 04 00 60 10  	beqz	$3, 0x199ad8 <.text+0x99ad8>
  199ac8: 2d 80 80 00  	move	$16, $4
  199acc: 20 00 82 8c  	lw	$2, 0x20($4)
  199ad0: 03 00 40 54  	bnezl	$2, 0x199ae0 <.text+0x99ae0>
  199ad4: 10 00 05 8e  	lw	$5, 0x10($16)
  199ad8: 2d 88 00 00  	move	$17, $zero
  199adc: 10 00 05 8e  	lw	$5, 0x10($16)
  199ae0: ff ff 04 3c  	lui	$4, 0xffff
  199ae4: 00 00 a2 dc  	ld	$2, 0x0($5)
  199ae8: ff ff 43 30  	andi	$3, $2, 0xffff
  199aec: 24 10 44 00  	and	$2, $2, $4
  199af0: 02 00 63 64  	daddiu	$3, $3, 0x2
  199af4: 25 10 43 00  	or	$2, $2, $3
  199af8: 00 00 a2 fc  	sd	$2, 0x0($5)
  199afc: 00 10 02 3c  	lui	$2, 0x1000
  199b00: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199b04: 01 80 42 34  	ori	$2, $2, 0x8001
  199b08: 14 00 03 8e  	lw	$3, 0x14($16)
  199b0c: 00 00 62 fc  	sd	$2, 0x0($3)
  199b10: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199b14: 14 00 04 8e  	lw	$4, 0x14($16)
  199b18: 15 00 20 12  	beqz	$17, 0x199b70 <.text+0x99b70>
  199b1c: 08 00 82 fc  	sd	$2, 0x8($4)
  199b20: 14 00 03 8e  	lw	$3, 0x14($16)
  199b24: 07 00 02 3c  	lui	$2, 0x7
  199b28: 10 00 62 fc  	sd	$2, 0x10($3)
  199b2c: 2d 20 00 02  	move	$4, $16
  199b30: 47 00 03 24  	addiu	$3, $zero, 0x47
  199b34: 14 00 05 8e  	lw	$5, 0x14($16)
  199b38: 18 00 a3 fc  	sd	$3, 0x18($5)
  199b3c: 14 00 02 8e  	lw	$2, 0x14($16)
  199b40: 20 00 42 24  	addiu	$2, $2, 0x20
  199b44: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199b48: 14 00 02 ae  	sw	$2, 0x14($16)
  199b4c: 20 00 bf df  	ld	$ra, 0x20($sp)
  199b50: 44 00 02 3c  	lui	$2, 0x44
  199b54: d8 ec 51 ac  	sw	$17, -0x1328($2)
  199b58: 24 00 11 ae  	sw	$17, 0x24($16)
  199b5c: 10 00 b1 df  	ld	$17, 0x10($sp)
  199b60: 00 00 b0 df  	ld	$16, 0x0($sp)
  199b64: 08 00 e0 03  	jr	$ra
  199b68: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  199b6c: 00 00 00 00  	nop
  199b70: 14 00 03 8e  	lw	$3, 0x14($16)
  199b74: ec ff 00 10  	b	0x199b28 <.text+0x99b28>
  199b78: 03 00 02 3c  	lui	$2, 0x3
  199b7c: 00 00 00 00  	nop
  199b80: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  199b84: 10 00 b1 ff  	sd	$17, 0x10($sp)
  199b88: 01 00 b1 30  	andi	$17, $5, 0x1
  199b8c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  199b90: 2d 80 80 00  	move	$16, $4
  199b94: 20 00 bf ff  	sd	$ra, 0x20($sp)
  199b98: 10 00 86 8c  	lw	$6, 0x10($4)
  199b9c: ff ff 04 3c  	lui	$4, 0xffff
  199ba0: 00 00 c2 dc  	ld	$2, 0x0($6)
  199ba4: ff ff 43 30  	andi	$3, $2, 0xffff
  199ba8: 24 10 44 00  	and	$2, $2, $4
  199bac: 03 00 63 64  	daddiu	$3, $3, 0x3
  199bb0: 25 10 43 00  	or	$2, $2, $3
  199bb4: 00 00 c2 fc  	sd	$2, 0x0($6)
  199bb8: 00 10 02 3c  	lui	$2, 0x1000
  199bbc: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199bc0: 01 80 42 34  	ori	$2, $2, 0x8001
  199bc4: 14 00 03 8e  	lw	$3, 0x14($16)
  199bc8: 00 00 62 fc  	sd	$2, 0x0($3)
  199bcc: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199bd0: 14 00 04 8e  	lw	$4, 0x14($16)
  199bd4: 1c 00 20 12  	beqz	$17, 0x199c48 <.text+0x99c48>
  199bd8: 08 00 82 fc  	sd	$2, 0x8($4)
  199bdc: 14 00 03 8e  	lw	$3, 0x14($16)
  199be0: 7f 00 02 24  	addiu	$2, $zero, 0x7f
  199be4: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199be8: 44 00 42 34  	ori	$2, $2, 0x44
  199bec: 10 00 62 fc  	sd	$2, 0x10($3)
  199bf0: 14 00 05 8e  	lw	$5, 0x14($16)
  199bf4: 42 00 02 24  	addiu	$2, $zero, 0x42
  199bf8: 2d 20 00 02  	move	$4, $16
  199bfc: 18 00 a2 fc  	sd	$2, 0x18($5)
  199c00: 14 00 03 8e  	lw	$3, 0x14($16)
  199c04: 20 00 60 fc  	sd	$zero, 0x20($3)
  199c08: 49 00 03 24  	addiu	$3, $zero, 0x49
  199c0c: 14 00 05 8e  	lw	$5, 0x14($16)
  199c10: 28 00 a3 fc  	sd	$3, 0x28($5)
  199c14: 14 00 02 8e  	lw	$2, 0x14($16)
  199c18: 30 00 42 24  	addiu	$2, $2, 0x30
  199c1c: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199c20: 14 00 02 ae  	sw	$2, 0x14($16)
  199c24: 20 00 bf df  	ld	$ra, 0x20($sp)
  199c28: 44 00 02 3c  	lui	$2, 0x44
  199c2c: d0 ec 51 ac  	sw	$17, -0x1330($2)
  199c30: 1c 00 11 ae  	sw	$17, 0x1c($16)
  199c34: 10 00 b1 df  	ld	$17, 0x10($sp)
  199c38: 00 00 b0 df  	ld	$16, 0x0($sp)
  199c3c: 08 00 e0 03  	jr	$ra
  199c40: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  199c44: 00 00 00 00  	nop
  199c48: 14 00 02 8e  	lw	$2, 0x14($16)
  199c4c: e8 ff 00 10  	b	0x199bf0 <.text+0x99bf0>
  199c50: 10 00 40 fc  	sd	$zero, 0x10($2)
  199c54: 00 00 00 00  	nop
  199c58: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199c5c: ff ff 06 3c  	lui	$6, 0xffff
  199c60: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199c64: 10 00 88 8c  	lw	$8, 0x10($4)
  199c68: 00 00 02 dd  	ld	$2, 0x0($8)
  199c6c: ff ff 43 30  	andi	$3, $2, 0xffff
  199c70: 24 10 46 00  	and	$2, $2, $6
  199c74: 02 00 63 64  	daddiu	$3, $3, 0x2
  199c78: 25 10 43 00  	or	$2, $2, $3
  199c7c: 00 00 02 fd  	sd	$2, 0x0($8)
  199c80: 00 10 02 3c  	lui	$2, 0x1000
  199c84: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199c88: 01 80 42 34  	ori	$2, $2, 0x8001
  199c8c: 14 00 83 8c  	lw	$3, 0x14($4)
  199c90: 00 00 62 fc  	sd	$2, 0x0($3)
  199c94: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199c98: 14 00 86 8c  	lw	$6, 0x14($4)
  199c9c: 08 00 c2 fc  	sd	$2, 0x8($6)
  199ca0: 14 00 83 8c  	lw	$3, 0x14($4)
  199ca4: 10 00 65 fc  	sd	$5, 0x10($3)
  199ca8: 45 00 03 24  	addiu	$3, $zero, 0x45
  199cac: 14 00 85 8c  	lw	$5, 0x14($4)
  199cb0: 18 00 a3 fc  	sd	$3, 0x18($5)
  199cb4: 14 00 82 8c  	lw	$2, 0x14($4)
  199cb8: 20 00 42 24  	addiu	$2, $2, 0x20
  199cbc: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199cc0: 14 00 82 ac  	sw	$2, 0x14($4)
  199cc4: 00 00 bf df  	ld	$ra, 0x0($sp)
  199cc8: 08 00 e0 03  	jr	$ra
  199ccc: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  199cd0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199cd4: ff ff 06 3c  	lui	$6, 0xffff
  199cd8: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199cdc: 10 00 88 8c  	lw	$8, 0x10($4)
  199ce0: 00 00 02 dd  	ld	$2, 0x0($8)
  199ce4: ff ff 43 30  	andi	$3, $2, 0xffff
  199ce8: 24 10 46 00  	and	$2, $2, $6
  199cec: 02 00 63 64  	daddiu	$3, $3, 0x2
  199cf0: 25 10 43 00  	or	$2, $2, $3
  199cf4: 00 00 02 fd  	sd	$2, 0x0($8)
  199cf8: 00 10 02 3c  	lui	$2, 0x1000
  199cfc: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199d00: 01 80 42 34  	ori	$2, $2, 0x8001
  199d04: 14 00 83 8c  	lw	$3, 0x14($4)
  199d08: 00 00 62 fc  	sd	$2, 0x0($3)
  199d0c: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199d10: 14 00 86 8c  	lw	$6, 0x14($4)
  199d14: 08 00 c2 fc  	sd	$2, 0x8($6)
  199d18: 14 00 83 8c  	lw	$3, 0x14($4)
  199d1c: 10 00 65 fc  	sd	$5, 0x10($3)
  199d20: 46 00 03 24  	addiu	$3, $zero, 0x46
  199d24: 14 00 85 8c  	lw	$5, 0x14($4)
  199d28: 18 00 a3 fc  	sd	$3, 0x18($5)
  199d2c: 14 00 82 8c  	lw	$2, 0x14($4)
  199d30: 20 00 42 24  	addiu	$2, $2, 0x20
  199d34: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199d38: 14 00 82 ac  	sw	$2, 0x14($4)
  199d3c: 00 00 bf df  	ld	$ra, 0x0($sp)
  199d40: 08 00 e0 03  	jr	$ra
  199d44: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  199d48: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199d4c: ff ff 06 3c  	lui	$6, 0xffff
  199d50: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199d54: 10 00 88 8c  	lw	$8, 0x10($4)
  199d58: 00 00 02 dd  	ld	$2, 0x0($8)
  199d5c: ff ff 43 30  	andi	$3, $2, 0xffff
  199d60: 24 10 46 00  	and	$2, $2, $6
  199d64: 02 00 63 64  	daddiu	$3, $3, 0x2
  199d68: 25 10 43 00  	or	$2, $2, $3
  199d6c: 00 00 02 fd  	sd	$2, 0x0($8)
  199d70: 00 10 02 3c  	lui	$2, 0x1000
  199d74: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199d78: 01 80 42 34  	ori	$2, $2, 0x8001
  199d7c: 14 00 83 8c  	lw	$3, 0x14($4)
  199d80: 00 00 62 fc  	sd	$2, 0x0($3)
  199d84: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199d88: 14 00 86 8c  	lw	$6, 0x14($4)
  199d8c: 08 00 c2 fc  	sd	$2, 0x8($6)
  199d90: 14 00 83 8c  	lw	$3, 0x14($4)
  199d94: 10 00 65 fc  	sd	$5, 0x10($3)
  199d98: 1a 00 03 24  	addiu	$3, $zero, 0x1a
  199d9c: 14 00 85 8c  	lw	$5, 0x14($4)
  199da0: 18 00 a3 fc  	sd	$3, 0x18($5)
  199da4: 14 00 82 8c  	lw	$2, 0x14($4)
  199da8: 20 00 42 24  	addiu	$2, $2, 0x20
  199dac: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199db0: 14 00 82 ac  	sw	$2, 0x14($4)
  199db4: 00 00 bf df  	ld	$ra, 0x0($sp)
  199db8: 08 00 e0 03  	jr	$ra
  199dbc: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  199dc0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199dc4: ff ff 02 3c  	lui	$2, 0xffff
  199dc8: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199dcc: ba 31 06 00  	dsrl	$6, $6, 0x6
  199dd0: 38 34 06 00  	dsll	$6, $6, 0x10
  199dd4: 7a 2b 05 00  	dsrl	$5, $5, 0xd
  199dd8: 10 00 8b 8c  	lw	$11, 0x10($4)
  199ddc: 25 28 a6 00  	or	$5, $5, $6
  199de0: 3c 40 08 00  	dsll32	$8, $8, 0x0
  199de4: 00 00 69 dd  	ld	$9, 0x0($11)
  199de8: 38 3e 07 00  	dsll	$7, $7, 0x18
  199dec: 25 28 a7 00  	or	$5, $5, $7
  199df0: ff ff 23 31  	andi	$3, $9, 0xffff
  199df4: 25 28 a8 00  	or	$5, $5, $8
  199df8: 24 48 22 01  	and	$9, $9, $2
  199dfc: 02 00 63 64  	daddiu	$3, $3, 0x2
  199e00: 25 48 23 01  	or	$9, $9, $3
  199e04: 00 10 02 3c  	lui	$2, 0x1000
  199e08: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199e0c: 01 80 42 34  	ori	$2, $2, 0x8001
  199e10: 00 00 69 fd  	sd	$9, 0x0($11)
  199e14: 14 00 83 8c  	lw	$3, 0x14($4)
  199e18: 00 00 62 fc  	sd	$2, 0x0($3)
  199e1c: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199e20: 14 00 86 8c  	lw	$6, 0x14($4)
  199e24: 08 00 c2 fc  	sd	$2, 0x8($6)
  199e28: 14 00 83 8c  	lw	$3, 0x14($4)
  199e2c: 10 00 65 fc  	sd	$5, 0x10($3)
  199e30: 4c 00 03 24  	addiu	$3, $zero, 0x4c
  199e34: 14 00 85 8c  	lw	$5, 0x14($4)
  199e38: 18 00 a3 fc  	sd	$3, 0x18($5)
  199e3c: 14 00 82 8c  	lw	$2, 0x14($4)
  199e40: 20 00 42 24  	addiu	$2, $2, 0x20
  199e44: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199e48: 14 00 82 ac  	sw	$2, 0x14($4)
  199e4c: 00 00 bf df  	ld	$ra, 0x0($sp)
  199e50: 08 00 e0 03  	jr	$ra
  199e54: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  199e58: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199e5c: 44 00 02 3c  	lui	$2, 0x44
  199e60: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199e64: dc ec 45 ac  	sw	$5, -0x1324($2)
  199e68: 44 00 02 3c  	lui	$2, 0x44
  199e6c: 28 00 85 ac  	sw	$5, 0x28($4)
  199e70: 00 29 05 00  	sll	$5, $5, 0x4
  199e74: 10 00 88 8c  	lw	$8, 0x10($4)
  199e78: e0 ec 46 ac  	sw	$6, -0x1320($2)
  199e7c: 2c 00 86 ac  	sw	$6, 0x2c($4)
  199e80: 00 31 06 00  	sll	$6, $6, 0x4
  199e84: 3c 30 06 00  	dsll32	$6, $6, 0x0
  199e88: 00 00 02 dd  	ld	$2, 0x0($8)
  199e8c: 25 30 a6 00  	or	$6, $5, $6
  199e90: ff ff 05 3c  	lui	$5, 0xffff
  199e94: ff ff 43 30  	andi	$3, $2, 0xffff
  199e98: 24 10 45 00  	and	$2, $2, $5
  199e9c: 02 00 63 64  	daddiu	$3, $3, 0x2
  199ea0: 25 10 43 00  	or	$2, $2, $3
  199ea4: 00 00 02 fd  	sd	$2, 0x0($8)
  199ea8: 00 10 02 3c  	lui	$2, 0x1000
  199eac: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199eb0: 01 80 42 34  	ori	$2, $2, 0x8001
  199eb4: 14 00 83 8c  	lw	$3, 0x14($4)
  199eb8: 00 00 62 fc  	sd	$2, 0x0($3)
  199ebc: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199ec0: 14 00 85 8c  	lw	$5, 0x14($4)
  199ec4: 08 00 a2 fc  	sd	$2, 0x8($5)
  199ec8: 14 00 83 8c  	lw	$3, 0x14($4)
  199ecc: 10 00 66 fc  	sd	$6, 0x10($3)
  199ed0: 18 00 03 24  	addiu	$3, $zero, 0x18
  199ed4: 14 00 85 8c  	lw	$5, 0x14($4)
  199ed8: 18 00 a3 fc  	sd	$3, 0x18($5)
  199edc: 14 00 82 8c  	lw	$2, 0x14($4)
  199ee0: 20 00 42 24  	addiu	$2, $2, 0x20
  199ee4: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199ee8: 14 00 82 ac  	sw	$2, 0x14($4)
  199eec: 00 00 bf df  	ld	$ra, 0x0($sp)
  199ef0: 08 00 e0 03  	jr	$ra
  199ef4: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  199ef8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199efc: ff ff 02 3c  	lui	$2, 0xffff
  199f00: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199f04: 38 3c 07 00  	dsll	$7, $7, 0x10
  199f08: 25 28 a7 00  	or	$5, $5, $7
  199f0c: 3c 30 06 00  	dsll32	$6, $6, 0x0
  199f10: 10 00 8b 8c  	lw	$11, 0x10($4)
  199f14: 25 28 a6 00  	or	$5, $5, $6
  199f18: 3c 44 08 00  	dsll32	$8, $8, 0x10
  199f1c: 00 00 69 dd  	ld	$9, 0x0($11)
  199f20: 25 28 a8 00  	or	$5, $5, $8
  199f24: ff ff 23 31  	andi	$3, $9, 0xffff
  199f28: 24 48 22 01  	and	$9, $9, $2
  199f2c: 02 00 63 64  	daddiu	$3, $3, 0x2
  199f30: 00 10 02 3c  	lui	$2, 0x1000
  199f34: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199f38: 01 80 42 34  	ori	$2, $2, 0x8001
  199f3c: 25 48 23 01  	or	$9, $9, $3
  199f40: 00 00 69 fd  	sd	$9, 0x0($11)
  199f44: 14 00 83 8c  	lw	$3, 0x14($4)
  199f48: 00 00 62 fc  	sd	$2, 0x0($3)
  199f4c: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199f50: 14 00 86 8c  	lw	$6, 0x14($4)
  199f54: 08 00 c2 fc  	sd	$2, 0x8($6)
  199f58: 14 00 83 8c  	lw	$3, 0x14($4)
  199f5c: 10 00 65 fc  	sd	$5, 0x10($3)
  199f60: 40 00 03 24  	addiu	$3, $zero, 0x40
  199f64: 14 00 85 8c  	lw	$5, 0x14($4)
  199f68: 18 00 a3 fc  	sd	$3, 0x18($5)
  199f6c: 14 00 82 8c  	lw	$2, 0x14($4)
  199f70: 20 00 42 24  	addiu	$2, $2, 0x20
  199f74: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199f78: 14 00 82 ac  	sw	$2, 0x14($4)
  199f7c: 00 00 bf df  	ld	$ra, 0x0($sp)
  199f80: 08 00 e0 03  	jr	$ra
  199f84: 10 00 bd 27  	addiu	$sp, $sp, 0x10
