  193298: 40 ff bd 27  	addiu	$sp, $sp, -0xc0 <.text+0xffffffffffefff40>
  19329c: a0 00 b5 ff  	sd	$21, 0xa0($sp)
  1932a0: ff ff 15 24  	addiu	$21, $zero, -0x1 <.text+0xffffffffffefffff>
  1932a4: 90 00 b4 ff  	sd	$20, 0x90($sp)
  1932a8: 2d a0 00 00  	move	$20, $zero
  1932ac: 80 00 b3 ff  	sd	$19, 0x80($sp)
  1932b0: 2d 98 00 00  	move	$19, $zero
  1932b4: 70 00 b2 ff  	sd	$18, 0x70($sp)
  1932b8: 2d 90 80 00  	move	$18, $4
  1932bc: 60 00 b1 ff  	sd	$17, 0x60($sp)
  1932c0: 2d 88 a0 00  	move	$17, $5
  1932c4: b0 00 bf ff  	sd	$ra, 0xb0($sp)
  1932c8: 89 00 80 10  	beqz	$4, 0x1934f0 <.text+0x934f0>
  1932cc: 50 00 b0 ff  	sd	$16, 0x50($sp)
  1932d0: 66 00 a0 50  	beqzl	$5, 0x19346c <.text+0x9346c>
  1932d4: 2d 10 00 00  	move	$2, $zero
  1932d8: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1932dc: 80 00 04 24  	addiu	$4, $zero, 0x80
  1932e0: 2d 80 40 00  	move	$16, $2
  1932e4: 61 00 00 12  	beqz	$16, 0x19346c <.text+0x9346c>
  1932e8: 2d 10 00 00  	move	$2, $zero
  1932ec: 2d 28 00 00  	move	$5, $zero
  1932f0: 2d 30 00 00  	move	$6, $zero
  1932f4: 2d 20 00 00  	move	$4, $zero
  1932f8: 28 00 00 ae  	sw	$zero, 0x28($16)
  1932fc: 2c 00 00 ae  	sw	$zero, 0x2c($16)
  193300: 30 00 00 ae  	sw	$zero, 0x30($16)
  193304: 54 00 00 ae  	sw	$zero, 0x54($16)
  193308: 00 00 00 ae  	sw	$zero, 0x0($16)
  19330c: 58 00 00 ae  	sw	$zero, 0x58($16)
  193310: 10 00 00 ae  	sw	$zero, 0x10($16)
  193314: 14 00 00 ae  	sw	$zero, 0x14($16)
  193318: 04 00 00 ae  	sw	$zero, 0x4($16)
  19331c: 50 00 00 ae  	sw	$zero, 0x50($16)
  193320: 48 00 00 ae  	sw	$zero, 0x48($16)
  193324: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  193328: 4c 00 00 ae  	sw	$zero, 0x4c($16)
  19332c: 2d 20 40 02  	move	$4, $18
  193330: 60 00 02 fe  	sd	$2, 0x60($16)
  193334: 68 00 00 ae  	sw	$zero, 0x68($16)
  193338: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  19333c: 70 00 00 ae  	sw	$zero, 0x70($16)
  193340: 01 00 42 24  	addiu	$2, $2, 0x1
  193344: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  193348: 2d 20 40 00  	move	$4, $2
  19334c: 66 00 40 10  	beqz	$2, 0x1934e8 <.text+0x934e8>
  193350: 6c 00 02 ae  	sw	$2, 0x6c($16)
  193354: 2d 20 40 00  	move	$4, $2
  193358: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  19335c: 2d 28 40 02  	move	$5, $18
  193360: 74 00 00 a2  	sb	$zero, 0x74($16)
  193364: 00 00 24 82  	lb	$4, 0x0($17)
  193368: 72 00 02 24  	addiu	$2, $zero, 0x72
  19336c: 77 00 09 24  	addiu	$9, $zero, 0x77
  193370: 79 00 82 10  	beq	$4, $2, 0x193558 <.text+0x93558>
  193374: 00 00 25 92  	lbu	$5, 0x0($17)
  193378: 00 16 05 00  	sll	$2, $5, 0x18
  19337c: d0 ff a6 24  	addiu	$6, $5, -0x30 <.text+0xffffffffffefffd0>
  193380: 03 16 02 00  	sra	$2, $2, 0x18
  193384: 2d 18 40 00  	move	$3, $2
  193388: 68 00 47 38  	xori	$7, $2, 0x68
  19338c: 6f 00 89 10  	beq	$4, $9, 0x19354c <.text+0x9354c>
  193390: 66 00 48 38  	xori	$8, $2, 0x66
  193394: 61 00 02 24  	addiu	$2, $zero, 0x61
  193398: 6d 00 82 50  	beql	$4, $2, 0x193550 <.text+0x93550>
  19339c: 74 00 09 a2  	sb	$9, 0x74($16)
  1933a0: 0a 00 c2 2c  	sltiu	$2, $6, 0xa
  1933a4: 65 00 40 10  	beqz	$2, 0x19353c <.text+0x9353c>
  1933a8: 02 00 02 24  	addiu	$2, $zero, 0x2
  1933ac: d0 ff 75 24  	addiu	$21, $3, -0x30 <.text+0xffffffffffefffd0>
  1933b0: ec ff a0 14  	bnez	$5, 0x193364 <.text+0x93364>
  1933b4: 01 00 31 26  	addiu	$17, $17, 0x1
  1933b8: 74 00 03 82  	lb	$3, 0x74($16)
  1933bc: 4a 00 60 10  	beqz	$3, 0x1934e8 <.text+0x934e8>
  1933c0: 00 00 00 00  	nop
  1933c4: 77 00 02 24  	addiu	$2, $zero, 0x77
  1933c8: 4b 00 62 10  	beq	$3, $2, 0x1934f8 <.text+0x934f8>
  1933cc: 1c 00 0a 3c  	lui	$10, 0x1c
  1933d0: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1933d4: 00 40 04 24  	addiu	$4, $zero, 0x4000
  1933d8: 1c 00 06 3c  	lui	$6, 0x1c
  1933dc: 48 00 07 24  	addiu	$7, $zero, 0x48
  1933e0: 2d 20 00 02  	move	$4, $16
  1933e4: f1 ff 05 24  	addiu	$5, $zero, -0xf <.text+0xffffffffffeffff1>
  1933e8: 80 92 c6 24  	addiu	$6, $6, -0x6d80 <.text+0xffffffffffef9280>
  1933ec: 00 00 02 ae  	sw	$2, 0x0($16)
  1933f0: 00 4a 06 0c  	jal	0x192800 <.text+0x92800>
  1933f4: 54 00 02 ae  	sw	$2, 0x54($16)
  1933f8: 3b 00 40 14  	bnez	$2, 0x1934e8 <.text+0x934e8>
  1933fc: 00 00 00 00  	nop
  193400: 54 00 02 8e  	lw	$2, 0x54($16)
  193404: 38 00 40 10  	beqz	$2, 0x1934e8 <.text+0x934e8>
  193408: 2d 20 40 02  	move	$4, $18
  19340c: 00 40 02 24  	addiu	$2, $zero, 0x4000
  193410: 14 00 02 ae  	sw	$2, 0x14($16)
  193414: 2d 28 80 02  	move	$5, $20
  193418: 42 00 02 3c  	lui	$2, 0x42
  19341c: f0 73 06 0c  	jal	0x19cfc0 <.text+0x9cfc0>
  193420: 70 5a 40 ac  	sw	$zero, 0x5a70($2)
  193424: 30 00 40 04  	bltz	$2, 0x1934e8 <.text+0x934e8>
  193428: 50 00 02 ae  	sw	$2, 0x50($16)
  19342c: 74 00 03 82  	lb	$3, 0x74($16)
  193430: 77 00 02 24  	addiu	$2, $zero, 0x77
  193434: 16 00 62 10  	beq	$3, $2, 0x193490 <.text+0x93490>
  193438: 42 00 02 3c  	lui	$2, 0x42
  19343c: c7 4d 06 0c  	jal	0x19371c <.text+0x9371c>
  193440: 2d 20 00 02  	move	$4, $16
  193444: 2d 28 00 00  	move	$5, $zero
  193448: 50 00 04 8e  	lw	$4, 0x50($16)
  19344c: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  193450: 01 00 06 24  	addiu	$6, $zero, 0x1
  193454: 04 00 03 8e  	lw	$3, 0x4($16)
  193458: 23 10 43 00  	subu	$2, $2, $3
  19345c: 3c 10 02 00  	dsll32	$2, $2, 0x0
  193460: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  193464: 78 00 02 fe  	sd	$2, 0x78($16)
  193468: 2d 10 00 02  	move	$2, $16
  19346c: b0 00 bf df  	ld	$ra, 0xb0($sp)
  193470: a0 00 b5 df  	ld	$21, 0xa0($sp)
  193474: 90 00 b4 df  	ld	$20, 0x90($sp)
  193478: 80 00 b3 df  	ld	$19, 0x80($sp)
  19347c: 70 00 b2 df  	ld	$18, 0x70($sp)
  193480: 60 00 b1 df  	ld	$17, 0x60($sp)
  193484: 50 00 b0 df  	ld	$16, 0x50($sp)
  193488: 08 00 e0 03  	jr	$ra
  19348c: c0 00 bd 27  	addiu	$sp, $sp, 0xc0
  193490: 1c 00 05 3c  	lui	$5, 0x1c
  193494: 60 48 43 24  	addiu	$3, $2, 0x4860
  193498: 60 48 46 8c  	lw	$6, 0x4860($2)
  19349c: 04 00 67 8c  	lw	$7, 0x4($3)
  1934a0: 88 92 a5 24  	addiu	$5, $5, -0x6d78 <.text+0xffffffffffef9288>
  1934a4: 20 00 a4 27  	addiu	$4, $sp, 0x20
  1934a8: 08 00 08 24  	addiu	$8, $zero, 0x8
  1934ac: 2d 48 00 00  	move	$9, $zero
  1934b0: 2d 50 00 00  	move	$10, $zero
  1934b4: 2d 58 00 00  	move	$11, $zero
  1934b8: 03 00 02 24  	addiu	$2, $zero, 0x3
  1934bc: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1934c0: 18 00 a2 af  	sw	$2, 0x18($sp)
  1934c4: 08 00 a0 af  	sw	$zero, 0x8($sp)
  1934c8: f4 78 06 0c  	jal	0x19e3d0 <.text+0x9e3d0>
  1934cc: 10 00 a0 af  	sw	$zero, 0x10($sp)
  1934d0: 50 00 04 8e  	lw	$4, 0x50($16)
  1934d4: 20 00 a5 27  	addiu	$5, $sp, 0x20
  1934d8: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  1934dc: 0a 00 06 24  	addiu	$6, $zero, 0xa
  1934e0: e0 ff 00 10  	b	0x193464 <.text+0x93464>
  1934e4: 0a 00 02 24  	addiu	$2, $zero, 0xa
  1934e8: 45 4e 06 0c  	jal	0x193914 <.text+0x93914>
  1934ec: 2d 20 00 02  	move	$4, $16
  1934f0: de ff 00 10  	b	0x19346c <.text+0x9346c>
  1934f4: 2d 10 00 00  	move	$2, $zero
  1934f8: 2d 28 a0 02  	move	$5, $21
  1934fc: 2d 48 60 02  	move	$9, $19
  193500: 80 92 4a 25  	addiu	$10, $10, -0x6d80 <.text+0xffffffffffef9280>
  193504: 2d 20 00 02  	move	$4, $16
  193508: 08 00 06 24  	addiu	$6, $zero, 0x8
  19350c: f1 ff 07 24  	addiu	$7, $zero, -0xf <.text+0xffffffffffeffff1>
  193510: 08 00 08 24  	addiu	$8, $zero, 0x8
  193514: 3b 42 06 0c  	jal	0x1908ec <.text+0x908ec>
  193518: 48 00 0b 24  	addiu	$11, $zero, 0x48
  19351c: 00 40 04 24  	addiu	$4, $zero, 0x4000
  193520: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  193524: 2d 88 40 00  	move	$17, $2
  193528: 58 00 02 ae  	sw	$2, 0x58($16)
  19352c: ee ff 20 16  	bnez	$17, 0x1934e8 <.text+0x934e8>
  193530: 10 00 02 ae  	sw	$2, 0x10($16)
  193534: b3 ff 00 10  	b	0x193404 <.text+0x93404>
  193538: 00 00 00 00  	nop
  19353c: 0a 98 47 00  	movz	$19, $2, $7
  193540: 01 00 02 24  	addiu	$2, $zero, 0x1
  193544: 9a ff 00 10  	b	0x1933b0 <.text+0x933b0>
  193548: 0a 98 48 00  	movz	$19, $2, $8
  19354c: 74 00 09 a2  	sb	$9, 0x74($16)
  193550: 93 ff 00 10  	b	0x1933a0 <.text+0x933a0>
  193554: 02 02 94 36  	ori	$20, $20, 0x202
  193558: 74 00 04 a2  	sb	$4, 0x74($16)
  19355c: 86 ff 00 10  	b	0x193378 <.text+0x93378>
  193560: 01 00 94 36  	ori	$20, $20, 0x1
  193564: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  193568: 00 00 bf ff  	sd	$ra, 0x0($sp)
  19356c: a6 4c 06 0c  	jal	0x193298 <.text+0x93298>
  193570: ff ff 06 24  	addiu	$6, $zero, -0x1 <.text+0xffffffffffefffff>
  193574: 00 00 bf df  	ld	$ra, 0x0($sp)
  193578: 08 00 e0 03  	jr	$ra
  19357c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  193580: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  193584: 2d 10 00 00  	move	$2, $zero
  193588: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19358c: 2d 88 a0 00  	move	$17, $5
  193590: 20 00 b0 ff  	sd	$16, 0x20($sp)
  193594: 2d 80 80 00  	move	$16, $4
  193598: 0a 00 80 04  	bltz	$4, 0x1935c4 <.text+0x935c4>
  19359c: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1935a0: 1c 00 05 3c  	lui	$5, 0x1c
  1935a4: 2d 20 a0 03  	move	$4, $sp
  1935a8: 2d 30 00 02  	move	$6, $16
  1935ac: f4 78 06 0c  	jal	0x19e3d0 <.text+0x9e3d0>
  1935b0: a0 92 a5 24  	addiu	$5, $5, -0x6d60 <.text+0xffffffffffef92a0>
  1935b4: 2d 30 00 02  	move	$6, $16
  1935b8: 2d 20 a0 03  	move	$4, $sp
  1935bc: a6 4c 06 0c  	jal	0x193298 <.text+0x93298>
  1935c0: 2d 28 20 02  	move	$5, $17
  1935c4: 40 00 bf df  	ld	$ra, 0x40($sp)
  1935c8: 30 00 b1 df  	ld	$17, 0x30($sp)
  1935cc: 20 00 b0 df  	ld	$16, 0x20($sp)
  1935d0: 08 00 e0 03  	jr	$ra
  1935d4: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1935d8: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1935dc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1935e0: 2d 80 80 00  	move	$16, $4
  1935e4: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1935e8: fe ff 04 24  	addiu	$4, $zero, -0x2 <.text+0xffffffffffeffffe>
  1935ec: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1935f0: 2d 90 c0 00  	move	$18, $6
  1935f4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1935f8: 05 00 00 12  	beqz	$16, 0x193610 <.text+0x93610>
  1935fc: 2d 88 a0 00  	move	$17, $5
  193600: 74 00 03 82  	lb	$3, 0x74($16)
  193604: 77 00 02 24  	addiu	$2, $zero, 0x77
  193608: 08 00 62 50  	beql	$3, $2, 0x19362c <.text+0x9362c>
  19360c: 14 00 02 8e  	lw	$2, 0x14($16)
  193610: 30 00 bf df  	ld	$ra, 0x30($sp)
  193614: 2d 10 80 00  	move	$2, $4
  193618: 20 00 b2 df  	ld	$18, 0x20($sp)
  19361c: 10 00 b1 df  	ld	$17, 0x10($sp)
  193620: 00 00 b0 df  	ld	$16, 0x0($sp)
  193624: 08 00 e0 03  	jr	$ra
  193628: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19362c: 07 00 40 10  	beqz	$2, 0x19364c <.text+0x9364c>
  193630: 00 40 06 24  	addiu	$6, $zero, 0x4000
  193634: 2d 20 00 02  	move	$4, $16
  193638: 2d 28 20 02  	move	$5, $17
  19363c: 70 43 06 0c  	jal	0x190dc0 <.text+0x90dc0>
  193640: 2d 30 40 02  	move	$6, $18
  193644: f2 ff 00 10  	b	0x193610 <.text+0x93610>
  193648: 2d 20 40 00  	move	$4, $2
  19364c: 58 00 05 8e  	lw	$5, 0x58($16)
  193650: 50 00 04 8e  	lw	$4, 0x50($16)
  193654: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  193658: 10 00 05 ae  	sw	$5, 0x10($16)
  19365c: 00 40 03 24  	addiu	$3, $zero, 0x4000
  193660: 02 00 43 10  	beq	$2, $3, 0x19366c <.text+0x9366c>
  193664: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  193668: 48 00 02 ae  	sw	$2, 0x48($16)
  19366c: f1 ff 00 10  	b	0x193634 <.text+0x93634>
  193670: 14 00 03 ae  	sw	$3, 0x14($16)
  193674: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  193678: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19367c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  193680: 10 00 bf ff  	sd	$ra, 0x10($sp)
  193684: 4c 00 82 8c  	lw	$2, 0x4c($4)
  193688: 0c 00 40 14  	bnez	$2, 0x1936bc <.text+0x936bc>
  19368c: 2d 80 80 00  	move	$16, $4
  193690: 04 00 82 8c  	lw	$2, 0x4($4)
  193694: 0e 00 40 10  	beqz	$2, 0x1936d0 <.text+0x936d0>
  193698: 00 40 06 24  	addiu	$6, $zero, 0x4000
  19369c: 04 00 02 8e  	lw	$2, 0x4($16)
  1936a0: 00 00 03 8e  	lw	$3, 0x0($16)
  1936a4: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  1936a8: 04 00 02 ae  	sw	$2, 0x4($16)
  1936ac: 00 00 62 90  	lbu	$2, 0x0($3)
  1936b0: 01 00 63 24  	addiu	$3, $3, 0x1
  1936b4: 00 00 03 ae  	sw	$3, 0x0($16)
  1936b8: 2d 18 40 00  	move	$3, $2
  1936bc: 10 00 bf df  	ld	$ra, 0x10($sp)
  1936c0: 2d 10 60 00  	move	$2, $3
  1936c4: 00 00 b0 df  	ld	$16, 0x0($sp)
  1936c8: 08 00 e0 03  	jr	$ra
  1936cc: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1936d0: 50 00 84 8c  	lw	$4, 0x50($4)
  1936d4: 42 00 02 3c  	lui	$2, 0x42
  1936d8: 54 00 05 8e  	lw	$5, 0x54($16)
  1936dc: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1936e0: 70 5a 40 ac  	sw	$zero, 0x5a70($2)
  1936e4: 04 00 40 10  	beqz	$2, 0x1936f8 <.text+0x936f8>
  1936e8: 04 00 02 ae  	sw	$2, 0x4($16)
  1936ec: 54 00 02 8e  	lw	$2, 0x54($16)
  1936f0: ea ff 00 10  	b	0x19369c <.text+0x9369c>
  1936f4: 00 00 02 ae  	sw	$2, 0x0($16)
  1936f8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1936fc: 50 00 04 8e  	lw	$4, 0x50($16)
  193700: a4 4c 06 0c  	jal	0x193290 <.text+0x93290>
  193704: 4c 00 02 ae  	sw	$2, 0x4c($16)
  193708: ec ff 40 10  	beqz	$2, 0x1936bc <.text+0x936bc>
  19370c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  193710: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  193714: e9 ff 00 10  	b	0x1936bc <.text+0x936bc>
  193718: 48 00 02 ae  	sw	$2, 0x48($16)
  19371c: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  193720: 40 00 bf ff  	sd	$ra, 0x40($sp)
  193724: 30 00 b3 ff  	sd	$19, 0x30($sp)
  193728: 20 00 b2 ff  	sd	$18, 0x20($sp)
  19372c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  193730: 2d 88 80 00  	move	$17, $4
  193734: 00 00 b0 ff  	sd	$16, 0x0($sp)
  193738: 2d 80 00 00  	move	$16, $zero
  19373c: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  193740: 2d 20 20 02  	move	$4, $17
  193744: 42 00 03 3c  	lui	$3, 0x42
  193748: 2d 20 40 00  	move	$4, $2
  19374c: 60 48 63 24  	addiu	$3, $3, 0x4860
  193750: 80 10 10 00  	sll	$2, $16, 0x2
  193754: 21 10 43 00  	addu	$2, $2, $3
  193758: 00 00 42 8c  	lw	$2, 0x0($2)
  19375c: 58 00 82 14  	bne	$4, $2, 0x1938c0 <.text+0x938c0>
  193760: 00 00 00 00  	nop
  193764: 01 00 10 26  	addiu	$16, $16, 0x1
  193768: 02 00 02 2e  	sltiu	$2, $16, 0x2
  19376c: f3 ff 40 14  	bnez	$2, 0x19373c <.text+0x9373c>
  193770: 00 00 00 00  	nop
  193774: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  193778: 2d 20 20 02  	move	$4, $17
  19377c: 2d 20 20 02  	move	$4, $17
  193780: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  193784: 2d 80 40 00  	move	$16, $2
  193788: 2d 90 40 00  	move	$18, $2
  19378c: 08 00 02 24  	addiu	$2, $zero, 0x8
  193790: 0a 00 02 12  	beq	$16, $2, 0x1937bc <.text+0x937bc>
  193794: e0 00 42 32  	andi	$2, $18, 0xe0
  193798: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  19379c: 48 00 22 ae  	sw	$2, 0x48($17)
  1937a0: 40 00 bf df  	ld	$ra, 0x40($sp)
  1937a4: 30 00 b3 df  	ld	$19, 0x30($sp)
  1937a8: 20 00 b2 df  	ld	$18, 0x20($sp)
  1937ac: 10 00 b1 df  	ld	$17, 0x10($sp)
  1937b0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1937b4: 08 00 e0 03  	jr	$ra
  1937b8: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1937bc: f6 ff 40 14  	bnez	$2, 0x193798 <.text+0x93798>
  1937c0: 2d 80 00 00  	move	$16, $zero
  1937c4: 2d 20 20 02  	move	$4, $17
  1937c8: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  1937cc: 01 00 10 26  	addiu	$16, $16, 0x1
  1937d0: 06 00 02 2e  	sltiu	$2, $16, 0x6
  1937d4: fc ff 40 14  	bnez	$2, 0x1937c8 <.text+0x937c8>
  1937d8: 2d 20 20 02  	move	$4, $17
  1937dc: 04 00 42 32  	andi	$2, $18, 0x4
  1937e0: 26 00 40 14  	bnez	$2, 0x19387c <.text+0x9387c>
  1937e4: 00 00 00 00  	nop
  1937e8: 08 00 42 32  	andi	$2, $18, 0x8
  1937ec: 1a 00 40 14  	bnez	$2, 0x193858 <.text+0x93858>
  1937f0: 00 00 00 00  	nop
  1937f4: 10 00 42 32  	andi	$2, $18, 0x10
  1937f8: 0e 00 40 14  	bnez	$2, 0x193834 <.text+0x93834>
  1937fc: 00 00 00 00  	nop
  193800: 02 00 42 32  	andi	$2, $18, 0x2
  193804: 05 00 40 14  	bnez	$2, 0x19381c <.text+0x9381c>
  193808: 00 00 00 00  	nop
  19380c: 4c 00 23 8e  	lw	$3, 0x4c($17)
  193810: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  193814: e1 ff 00 10  	b	0x19379c <.text+0x9379c>
  193818: 0a 10 03 00  	movz	$2, $zero, $3
  19381c: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  193820: 2d 20 20 02  	move	$4, $17
  193824: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  193828: 2d 20 20 02  	move	$4, $17
  19382c: f8 ff 00 10  	b	0x193810 <.text+0x93810>
  193830: 4c 00 23 8e  	lw	$3, 0x4c($17)
  193834: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  193838: 2d 20 20 02  	move	$4, $17
  19383c: f0 ff 40 10  	beqz	$2, 0x193800 <.text+0x93800>
  193840: 2d 20 40 00  	move	$4, $2
  193844: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  193848: fa ff 82 14  	bne	$4, $2, 0x193834 <.text+0x93834>
  19384c: 02 00 42 32  	andi	$2, $18, 0x2
  193850: ec ff 00 10  	b	0x193804 <.text+0x93804>
  193854: 00 00 00 00  	nop
  193858: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  19385c: 2d 20 20 02  	move	$4, $17
  193860: e4 ff 40 10  	beqz	$2, 0x1937f4 <.text+0x937f4>
  193864: 2d 20 40 00  	move	$4, $2
  193868: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  19386c: fa ff 82 14  	bne	$4, $2, 0x193858 <.text+0x93858>
  193870: 10 00 42 32  	andi	$2, $18, 0x10
  193874: e0 ff 00 10  	b	0x1937f8 <.text+0x937f8>
  193878: 00 00 00 00  	nop
  19387c: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  193880: 2d 20 20 02  	move	$4, $17
  193884: 2d 20 20 02  	move	$4, $17
  193888: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  19388c: 2d 80 40 00  	move	$16, $2
  193890: 00 12 02 00  	sll	$2, $2, 0x8
  193894: 21 80 02 02  	addu	$16, $16, $2
  193898: ff ff 13 24  	addiu	$19, $zero, -0x1 <.text+0xffffffffffefffff>
  19389c: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  1938a0: d1 ff 13 12  	beq	$16, $19, 0x1937e8 <.text+0x937e8>
  1938a4: 2d 20 20 02  	move	$4, $17
  1938a8: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  1938ac: 00 00 00 00  	nop
  1938b0: fb ff 53 54  	bnel	$2, $19, 0x1938a0 <.text+0x938a0>
  1938b4: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  1938b8: cc ff 00 10  	b	0x1937ec <.text+0x937ec>
  1938bc: 08 00 42 32  	andi	$2, $18, 0x8
  1938c0: 08 00 00 12  	beqz	$16, 0x1938e4 <.text+0x938e4>
  1938c4: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1938c8: 04 00 22 8e  	lw	$2, 0x4($17)
  1938cc: 00 00 23 8e  	lw	$3, 0x0($17)
  1938d0: 01 00 42 24  	addiu	$2, $2, 0x1
  1938d4: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  1938d8: 04 00 22 ae  	sw	$2, 0x4($17)
  1938dc: 00 00 23 ae  	sw	$3, 0x0($17)
  1938e0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1938e4: 08 00 82 10  	beq	$4, $2, 0x193908 <.text+0x93908>
  1938e8: 01 00 03 24  	addiu	$3, $zero, 0x1
  1938ec: 04 00 22 8e  	lw	$2, 0x4($17)
  1938f0: 00 00 24 8e  	lw	$4, 0x0($17)
  1938f4: 01 00 42 24  	addiu	$2, $2, 0x1
  1938f8: 70 00 23 ae  	sw	$3, 0x70($17)
  1938fc: ff ff 84 24  	addiu	$4, $4, -0x1 <.text+0xffffffffffefffff>
  193900: 04 00 22 ae  	sw	$2, 0x4($17)
  193904: 00 00 24 ae  	sw	$4, 0x0($17)
  193908: 04 00 22 8e  	lw	$2, 0x4($17)
  19390c: a3 ff 00 10  	b	0x19379c <.text+0x9379c>
  193910: 01 00 42 2c  	sltiu	$2, $2, 0x1
  193914: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  193918: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  19391c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  193920: 2d 88 00 00  	move	$17, $zero
  193924: 00 00 b0 ff  	sd	$16, 0x0($sp)
  193928: 2d 80 80 00  	move	$16, $4
  19392c: 24 00 80 10  	beqz	$4, 0x1939c0 <.text+0x939c0>
  193930: 20 00 bf ff  	sd	$ra, 0x20($sp)
  193934: 68 00 84 8c  	lw	$4, 0x68($4)
  193938: 3a 00 80 14  	bnez	$4, 0x193a24 <.text+0x93a24>
  19393c: 00 00 00 00  	nop
  193940: 24 00 02 8e  	lw	$2, 0x24($16)
  193944: 08 00 40 50  	beqzl	$2, 0x193968 <.text+0x93968>
  193948: 50 00 04 8e  	lw	$4, 0x50($16)
  19394c: 74 00 03 82  	lb	$3, 0x74($16)
  193950: 77 00 02 24  	addiu	$2, $zero, 0x77
  193954: 2f 00 62 10  	beq	$3, $2, 0x193a14 <.text+0x93a14>
  193958: 72 00 02 24  	addiu	$2, $zero, 0x72
  19395c: 29 00 62 10  	beq	$3, $2, 0x193a04 <.text+0x93a04>
  193960: 00 00 00 00  	nop
  193964: 50 00 04 8e  	lw	$4, 0x50($16)
  193968: 06 00 82 04  	bltzl	$4, 0x193984 <.text+0x93984>
  19396c: 48 00 03 8e  	lw	$3, 0x48($16)
  193970: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  193974: 00 00 00 00  	nop
  193978: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  19397c: 0b 88 62 00  	movn	$17, $3, $2
  193980: 48 00 03 8e  	lw	$3, 0x48($16)
  193984: 54 00 04 8e  	lw	$4, 0x54($16)
  193988: 00 00 62 28  	slti	$2, $3, 0x0
  19398c: 19 00 80 14  	bnez	$4, 0x1939f4 <.text+0x939f4>
  193990: 0b 88 62 00  	movn	$17, $3, $2
  193994: 58 00 04 8e  	lw	$4, 0x58($16)
  193998: 12 00 80 14  	bnez	$4, 0x1939e4 <.text+0x939e4>
  19399c: 00 00 00 00  	nop
  1939a0: 6c 00 04 8e  	lw	$4, 0x6c($16)
  1939a4: 0b 00 80 14  	bnez	$4, 0x1939d4 <.text+0x939d4>
  1939a8: 00 00 00 00  	nop
  1939ac: 04 00 00 12  	beqz	$16, 0x1939c0 <.text+0x939c0>
  1939b0: 2d 10 20 02  	move	$2, $17
  1939b4: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1939b8: 2d 20 00 02  	move	$4, $16
  1939bc: 2d 10 20 02  	move	$2, $17
  1939c0: 20 00 bf df  	ld	$ra, 0x20($sp)
  1939c4: 10 00 b1 df  	ld	$17, 0x10($sp)
  1939c8: 00 00 b0 df  	ld	$16, 0x0($sp)
  1939cc: 08 00 e0 03  	jr	$ra
  1939d0: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1939d4: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1939d8: 00 00 00 00  	nop
  1939dc: f3 ff 00 10  	b	0x1939ac <.text+0x939ac>
  1939e0: 00 00 00 00  	nop
  1939e4: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1939e8: 00 00 00 00  	nop
  1939ec: ed ff 00 10  	b	0x1939a4 <.text+0x939a4>
  1939f0: 6c 00 04 8e  	lw	$4, 0x6c($16)
  1939f4: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1939f8: 00 00 00 00  	nop
  1939fc: e6 ff 00 10  	b	0x193998 <.text+0x93998>
  193a00: 58 00 04 8e  	lw	$4, 0x58($16)
  193a04: e1 49 06 0c  	jal	0x192784 <.text+0x92784>
  193a08: 2d 20 00 02  	move	$4, $16
  193a0c: d5 ff 00 10  	b	0x193964 <.text+0x93964>
  193a10: 2d 88 40 00  	move	$17, $2
  193a14: c2 44 06 0c  	jal	0x191308 <.text+0x91308>
  193a18: 2d 20 00 02  	move	$4, $16
  193a1c: d1 ff 00 10  	b	0x193964 <.text+0x93964>
  193a20: 2d 88 40 00  	move	$17, $2
  193a24: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  193a28: 00 00 00 00  	nop
  193a2c: c5 ff 00 10  	b	0x193944 <.text+0x93944>
  193a30: 24 00 02 8e  	lw	$2, 0x24($16)
  193a34: 80 ff bd 27  	addiu	$sp, $sp, -0x80 <.text+0xffffffffffefff80>
  193a38: 60 00 b6 ff  	sd	$22, 0x60($sp)
  193a3c: 2d b0 a0 00  	move	$22, $5
  193a40: 40 00 b4 ff  	sd	$20, 0x40($sp)
  193a44: 2d a0 c0 00  	move	$20, $6
  193a48: 30 00 b3 ff  	sd	$19, 0x30($sp)
  193a4c: 2d 98 a0 00  	move	$19, $5
  193a50: 20 00 b2 ff  	sd	$18, 0x20($sp)
  193a54: 2d 90 80 00  	move	$18, $4
  193a58: 70 00 bf ff  	sd	$ra, 0x70($sp)
  193a5c: 50 00 b5 ff  	sd	$21, 0x50($sp)
  193a60: 10 00 b1 ff  	sd	$17, 0x10($sp)
  193a64: 05 00 80 10  	beqz	$4, 0x193a7c <.text+0x93a7c>
  193a68: 00 00 b0 ff  	sd	$16, 0x0($sp)
  193a6c: 74 00 83 80  	lb	$3, 0x74($4)
  193a70: 72 00 02 24  	addiu	$2, $zero, 0x72
  193a74: 0d 00 62 50  	beql	$3, $2, 0x193aac <.text+0x93aac>
  193a78: 48 00 83 8c  	lw	$3, 0x48($4)
  193a7c: fe ff 04 24  	addiu	$4, $zero, -0x2 <.text+0xffffffffffeffffe>
  193a80: 70 00 bf df  	ld	$ra, 0x70($sp)
  193a84: 2d 10 80 00  	move	$2, $4
  193a88: 60 00 b6 df  	ld	$22, 0x60($sp)
  193a8c: 50 00 b5 df  	ld	$21, 0x50($sp)
  193a90: 40 00 b4 df  	ld	$20, 0x40($sp)
  193a94: 30 00 b3 df  	ld	$19, 0x30($sp)
  193a98: 20 00 b2 df  	ld	$18, 0x20($sp)
  193a9c: 10 00 b1 df  	ld	$17, 0x10($sp)
  193aa0: 00 00 b0 df  	ld	$16, 0x0($sp)
  193aa4: 08 00 e0 03  	jr	$ra
  193aa8: 80 00 bd 27  	addiu	$sp, $sp, 0x80
  193aac: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  193ab0: 87 00 62 10  	beq	$3, $2, 0x193cd0 <.text+0x93cd0>
  193ab4: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  193ab8: 85 00 62 10  	beq	$3, $2, 0x193cd0 <.text+0x93cd0>
  193abc: 01 00 02 24  	addiu	$2, $zero, 0x1
  193ac0: ef ff 62 10  	beq	$3, $2, 0x193a80 <.text+0x93a80>
  193ac4: 2d 20 00 00  	move	$4, $zero
  193ac8: 10 00 45 ae  	sw	$5, 0x10($18)
  193acc: 2d a8 a0 00  	move	$21, $5
  193ad0: 16 00 c0 10  	beqz	$6, 0x193b2c <.text+0x93b2c>
  193ad4: 14 00 46 ae  	sw	$6, 0x14($18)
  193ad8: 70 00 42 8e  	lw	$2, 0x70($18)
  193adc: 50 00 40 54  	bnezl	$2, 0x193c20 <.text+0x93c20>
  193ae0: 04 00 50 8e  	lw	$16, 0x4($18)
  193ae4: 04 00 42 8e  	lw	$2, 0x4($18)
  193ae8: 05 00 40 14  	bnez	$2, 0x193b00 <.text+0x93b00>
  193aec: 2d 20 40 02  	move	$4, $18
  193af0: 4c 00 42 8e  	lw	$2, 0x4c($18)
  193af4: 37 00 40 50  	beqzl	$2, 0x193bd4 <.text+0x93bd4>
  193af8: 50 00 44 8e  	lw	$4, 0x50($18)
  193afc: 2d 20 40 02  	move	$4, $18
  193b00: 5b 4a 06 0c  	jal	0x19296c <.text+0x9296c>
  193b04: 2d 28 00 00  	move	$5, $zero
  193b08: 01 00 03 24  	addiu	$3, $zero, 0x1
  193b0c: 10 00 43 10  	beq	$2, $3, 0x193b50 <.text+0x93b50>
  193b10: 48 00 42 ae  	sw	$2, 0x48($18)
  193b14: 48 00 42 de  	ld	$2, 0x48($18)
  193b18: 05 00 40 54  	bnezl	$2, 0x193b30 <.text+0x93b30>
  193b1c: 10 00 46 8e  	lw	$6, 0x10($18)
  193b20: 14 00 42 8e  	lw	$2, 0x14($18)
  193b24: ed ff 40 54  	bnezl	$2, 0x193adc <.text+0x93adc>
  193b28: 70 00 42 8e  	lw	$2, 0x70($18)
  193b2c: 10 00 46 8e  	lw	$6, 0x10($18)
  193b30: 2d 28 60 02  	move	$5, $19
  193b34: 60 00 44 de  	ld	$4, 0x60($18)
  193b38: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  193b3c: 23 30 d3 00  	subu	$6, $6, $19
  193b40: 14 00 43 8e  	lw	$3, 0x14($18)
  193b44: 60 00 42 fe  	sd	$2, 0x60($18)
  193b48: cd ff 00 10  	b	0x193a80 <.text+0x93a80>
  193b4c: 23 20 83 02  	subu	$4, $20, $3
  193b50: 10 00 46 8e  	lw	$6, 0x10($18)
  193b54: 2d 28 60 02  	move	$5, $19
  193b58: 60 00 44 de  	ld	$4, 0x60($18)
  193b5c: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  193b60: 23 30 d3 00  	subu	$6, $6, $19
  193b64: 10 00 53 8e  	lw	$19, 0x10($18)
  193b68: 60 00 42 fe  	sd	$2, 0x60($18)
  193b6c: 07 51 06 0c  	jal	0x19441c <.text+0x9441c>
  193b70: 2d 20 40 02  	move	$4, $18
  193b74: 60 00 43 de  	ld	$3, 0x60($18)
  193b78: 03 00 43 10  	beq	$2, $3, 0x193b88 <.text+0x93b88>
  193b7c: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  193b80: e4 ff 00 10  	b	0x193b14 <.text+0x93b14>
  193b84: 48 00 42 ae  	sw	$2, 0x48($18)
  193b88: 07 51 06 0c  	jal	0x19441c <.text+0x9441c>
  193b8c: 2d 20 40 02  	move	$4, $18
  193b90: c7 4d 06 0c  	jal	0x19371c <.text+0x9371c>
  193b94: 2d 20 40 02  	move	$4, $18
  193b98: 48 00 42 8e  	lw	$2, 0x48($18)
  193b9c: de ff 40 54  	bnezl	$2, 0x193b18 <.text+0x93b18>
  193ba0: 48 00 42 de  	ld	$2, 0x48($18)
  193ba4: 08 00 50 de  	ld	$16, 0x8($18)
  193ba8: 2d 20 40 02  	move	$4, $18
  193bac: cb 49 06 0c  	jal	0x19272c <.text+0x9272c>
  193bb0: 18 00 51 de  	ld	$17, 0x18($18)
  193bb4: 08 00 50 fe  	sd	$16, 0x8($18)
  193bb8: 2d 20 00 00  	move	$4, $zero
  193bbc: 18 00 51 fe  	sd	$17, 0x18($18)
  193bc0: 2d 28 00 00  	move	$5, $zero
  193bc4: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  193bc8: 2d 30 00 00  	move	$6, $zero
  193bcc: d1 ff 00 10  	b	0x193b14 <.text+0x93b14>
  193bd0: 60 00 42 fe  	sd	$2, 0x60($18)
  193bd4: 42 00 02 3c  	lui	$2, 0x42
  193bd8: 54 00 45 8e  	lw	$5, 0x54($18)
  193bdc: 00 40 06 24  	addiu	$6, $zero, 0x4000
  193be0: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  193be4: 70 5a 40 ac  	sw	$zero, 0x5a70($2)
  193be8: 04 00 40 10  	beqz	$2, 0x193bfc <.text+0x93bfc>
  193bec: 04 00 42 ae  	sw	$2, 0x4($18)
  193bf0: 54 00 42 8e  	lw	$2, 0x54($18)
  193bf4: c1 ff 00 10  	b	0x193afc <.text+0x93afc>
  193bf8: 00 00 42 ae  	sw	$2, 0x0($18)
  193bfc: 01 00 02 24  	addiu	$2, $zero, 0x1
  193c00: 50 00 44 8e  	lw	$4, 0x50($18)
  193c04: a4 4c 06 0c  	jal	0x193290 <.text+0x93290>
  193c08: 4c 00 42 ae  	sw	$2, 0x4c($18)
  193c0c: f9 ff 40 50  	beqzl	$2, 0x193bf4 <.text+0x93bf4>
  193c10: 54 00 42 8e  	lw	$2, 0x54($18)
  193c14: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  193c18: c4 ff 00 10  	b	0x193b2c <.text+0x93b2c>
  193c1c: 48 00 42 ae  	sw	$2, 0x48($18)
  193c20: 14 00 43 8e  	lw	$3, 0x14($18)
  193c24: 2b 10 70 00  	sltu	$2, $3, $16
  193c28: 0b 80 62 00  	movn	$16, $3, $2
  193c2c: 19 00 00 56  	bnezl	$16, 0x193c94 <.text+0x93c94>
  193c30: 10 00 44 8e  	lw	$4, 0x10($18)
  193c34: 14 00 46 8e  	lw	$6, 0x14($18)
  193c38: 10 00 c0 54  	bnezl	$6, 0x193c7c <.text+0x93c7c>
  193c3c: 50 00 44 8e  	lw	$4, 0x50($18)
  193c40: 14 00 42 8e  	lw	$2, 0x14($18)
  193c44: 08 00 44 de  	ld	$4, 0x8($18)
  193c48: 23 a0 82 02  	subu	$20, $20, $2
  193c4c: 18 00 43 de  	ld	$3, 0x18($18)
  193c50: 3c 10 14 00  	dsll32	$2, $20, 0x0
  193c54: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  193c58: 2d 18 62 00  	daddu	$3, $3, $2
  193c5c: 2d 20 82 00  	daddu	$4, $4, $2
  193c60: 08 00 44 fe  	sd	$4, 0x8($18)
  193c64: 03 00 80 16  	bnez	$20, 0x193c74 <.text+0x93c74>
  193c68: 18 00 43 fe  	sd	$3, 0x18($18)
  193c6c: 01 00 02 24  	addiu	$2, $zero, 0x1
  193c70: 4c 00 42 ae  	sw	$2, 0x4c($18)
  193c74: 82 ff 00 10  	b	0x193a80 <.text+0x93a80>
  193c78: 2d 20 80 02  	move	$4, $20
  193c7c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  193c80: 2d 28 a0 02  	move	$5, $21
  193c84: 14 00 43 8e  	lw	$3, 0x14($18)
  193c88: 23 18 62 00  	subu	$3, $3, $2
  193c8c: ec ff 00 10  	b	0x193c40 <.text+0x93c40>
  193c90: 14 00 43 ae  	sw	$3, 0x14($18)
  193c94: 2d 30 00 02  	move	$6, $16
  193c98: 00 00 45 8e  	lw	$5, 0x0($18)
  193c9c: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  193ca0: 21 a8 d0 02  	addu	$21, $22, $16
  193ca4: 10 00 55 ae  	sw	$21, 0x10($18)
  193ca8: 04 00 42 8e  	lw	$2, 0x4($18)
  193cac: 00 00 43 8e  	lw	$3, 0x0($18)
  193cb0: 14 00 44 8e  	lw	$4, 0x14($18)
  193cb4: 23 10 50 00  	subu	$2, $2, $16
  193cb8: 21 18 70 00  	addu	$3, $3, $16
  193cbc: 04 00 42 ae  	sw	$2, 0x4($18)
  193cc0: 23 20 90 00  	subu	$4, $4, $16
  193cc4: 00 00 43 ae  	sw	$3, 0x0($18)
  193cc8: da ff 00 10  	b	0x193c34 <.text+0x93c34>
  193ccc: 14 00 44 ae  	sw	$4, 0x14($18)
  193cd0: 6b ff 00 10  	b	0x193a80 <.text+0x93a80>
  193cd4: ff ff 04 24  	addiu	$4, $zero, -0x1 <.text+0xffffffffffefffff>
  193cd8: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  193cdc: 01 00 06 24  	addiu	$6, $zero, 0x1
  193ce0: 10 00 bf ff  	sd	$ra, 0x10($sp)
  193ce4: 8d 4e 06 0c  	jal	0x193a34 <.text+0x93a34>
  193ce8: 2d 28 a0 03  	move	$5, $sp
  193cec: 10 00 bf df  	ld	$ra, 0x10($sp)
  193cf0: 00 00 a4 93  	lbu	$4, 0x0($sp)
  193cf4: 01 00 42 38  	xori	$2, $2, 0x1
  193cf8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  193cfc: 0a 18 82 00  	movz	$3, $4, $2
  193d00: 2d 10 60 00  	move	$2, $3
  193d04: 08 00 e0 03  	jr	$ra
  193d08: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  193d0c: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  193d10: 30 00 b3 ff  	sd	$19, 0x30($sp)
  193d14: 2d 98 80 00  	move	$19, $4
  193d18: 20 00 b2 ff  	sd	$18, 0x20($sp)
  193d1c: 2d 90 a0 00  	move	$18, $5
  193d20: 10 00 b1 ff  	sd	$17, 0x10($sp)
  193d24: 2d 88 c0 00  	move	$17, $6
  193d28: 00 00 b0 ff  	sd	$16, 0x0($sp)
  193d2c: 2d 80 a0 00  	move	$16, $5
  193d30: 20 00 a0 10  	beqz	$5, 0x193db4 <.text+0x93db4>
  193d34: 40 00 bf ff  	sd	$ra, 0x40($sp)
  193d38: 13 00 c0 18  	blez	$6, 0x193d88 <.text+0x93d88>
  193d3c: 2d 10 00 00  	move	$2, $zero
  193d40: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  193d44: 2d 28 00 02  	move	$5, $16
  193d48: 2d 20 60 02  	move	$4, $19
  193d4c: 0b 00 20 1a  	blez	$17, 0x193d7c <.text+0x93d7c>
  193d50: 01 00 06 24  	addiu	$6, $zero, 0x1
  193d54: 8d 4e 06 0c  	jal	0x193a34 <.text+0x93a34>
  193d58: 00 00 00 00  	nop
  193d5c: 01 00 03 24  	addiu	$3, $zero, 0x1
  193d60: 06 00 43 14  	bne	$2, $3, 0x193d7c <.text+0x93d7c>
  193d64: 0a 00 03 24  	addiu	$3, $zero, 0xa
  193d68: 00 00 02 92  	lbu	$2, 0x0($16)
  193d6c: 00 16 02 00  	sll	$2, $2, 0x18
  193d70: 03 16 02 00  	sra	$2, $2, 0x18
  193d74: f2 ff 43 14  	bne	$2, $3, 0x193d40 <.text+0x93d40>
  193d78: 01 00 10 26  	addiu	$16, $16, 0x1
  193d7c: 09 00 50 12  	beq	$18, $16, 0x193da4 <.text+0x93da4>
  193d80: 00 00 00 a2  	sb	$zero, 0x0($16)
  193d84: 2d 10 40 02  	move	$2, $18
  193d88: 40 00 bf df  	ld	$ra, 0x40($sp)
  193d8c: 30 00 b3 df  	ld	$19, 0x30($sp)
  193d90: 20 00 b2 df  	ld	$18, 0x20($sp)
  193d94: 10 00 b1 df  	ld	$17, 0x10($sp)
  193d98: 00 00 b0 df  	ld	$16, 0x0($sp)
  193d9c: 08 00 e0 03  	jr	$ra
  193da0: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  193da4: f8 ff 20 1e  	bgtz	$17, 0x193d88 <.text+0x93d88>
  193da8: 2d 10 00 00  	move	$2, $zero
  193dac: f6 ff 00 10  	b	0x193d88 <.text+0x93d88>
  193db0: 2d 10 40 02  	move	$2, $18
  193db4: f4 ff 00 10  	b	0x193d88 <.text+0x93d88>
  193db8: 2d 10 00 00  	move	$2, $zero
  193dbc: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  193dc0: 20 00 b2 ff  	sd	$18, 0x20($sp)
  193dc4: 2d 90 a0 00  	move	$18, $5
  193dc8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  193dcc: 2d 88 c0 00  	move	$17, $6
  193dd0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  193dd4: 2d 80 80 00  	move	$16, $4
  193dd8: 05 00 80 10  	beqz	$4, 0x193df0 <.text+0x93df0>
  193ddc: 30 00 bf ff  	sd	$ra, 0x30($sp)
  193de0: 74 00 83 80  	lb	$3, 0x74($4)
  193de4: 77 00 02 24  	addiu	$2, $zero, 0x77
  193de8: 08 00 62 50  	beql	$3, $2, 0x193e0c <.text+0x93e0c>
  193dec: 00 00 05 ae  	sw	$5, 0x0($16)
  193df0: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  193df4: 30 00 bf df  	ld	$ra, 0x30($sp)
  193df8: 20 00 b2 df  	ld	$18, 0x20($sp)
  193dfc: 10 00 b1 df  	ld	$17, 0x10($sp)
  193e00: 00 00 b0 df  	ld	$16, 0x0($sp)
  193e04: 08 00 e0 03  	jr	$ra
  193e08: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  193e0c: 15 00 c0 10  	beqz	$6, 0x193e64 <.text+0x93e64>
  193e10: 04 00 06 ae  	sw	$6, 0x4($16)
  193e14: 14 00 02 8e  	lw	$2, 0x14($16)
  193e18: 0a 00 40 14  	bnez	$2, 0x193e44 <.text+0x93e44>
  193e1c: 00 40 06 24  	addiu	$6, $zero, 0x4000
  193e20: 58 00 05 8e  	lw	$5, 0x58($16)
  193e24: 50 00 04 8e  	lw	$4, 0x50($16)
  193e28: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  193e2c: 10 00 05 ae  	sw	$5, 0x10($16)
  193e30: 2d 18 40 00  	move	$3, $2
  193e34: 00 40 02 24  	addiu	$2, $zero, 0x4000
  193e38: 12 00 62 14  	bne	$3, $2, 0x193e84 <.text+0x93e84>
  193e3c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  193e40: 14 00 03 ae  	sw	$3, 0x14($16)
  193e44: 2d 20 00 02  	move	$4, $16
  193e48: e8 43 06 0c  	jal	0x190fa0 <.text+0x90fa0>
  193e4c: 2d 28 00 00  	move	$5, $zero
  193e50: 04 00 40 14  	bnez	$2, 0x193e64 <.text+0x93e64>
  193e54: 48 00 02 ae  	sw	$2, 0x48($16)
  193e58: 04 00 02 8e  	lw	$2, 0x4($16)
  193e5c: ee ff 40 54  	bnezl	$2, 0x193e18 <.text+0x93e18>
  193e60: 14 00 02 8e  	lw	$2, 0x14($16)
  193e64: 60 00 04 de  	ld	$4, 0x60($16)
  193e68: 2d 28 40 02  	move	$5, $18
  193e6c: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  193e70: 2d 30 20 02  	move	$6, $17
  193e74: 04 00 03 8e  	lw	$3, 0x4($16)
  193e78: 60 00 02 fe  	sd	$2, 0x60($16)
  193e7c: dd ff 00 10  	b	0x193df4 <.text+0x93df4>
  193e80: 23 10 23 02  	subu	$2, $17, $3
  193e84: f7 ff 00 10  	b	0x193e64 <.text+0x93e64>
  193e88: 48 00 02 ae  	sw	$2, 0x48($16)
  193e8c: 90 ef bd 27  	addiu	$sp, $sp, -0x1070 <.text+0xffffffffffefef90>
  193e90: 00 10 b0 ff  	sd	$16, 0x1000($sp)
  193e94: 2d 80 80 00  	move	$16, $4
  193e98: 40 10 a6 ff  	sd	$6, 0x1040($sp)
  193e9c: 2d 20 a0 03  	move	$4, $sp
  193ea0: 40 10 a6 27  	addiu	$6, $sp, 0x1040
  193ea4: 10 10 bf ff  	sd	$ra, 0x1010($sp)
  193ea8: 48 10 a7 ff  	sd	$7, 0x1048($sp)
  193eac: 50 10 a8 ff  	sd	$8, 0x1050($sp)
  193eb0: 58 10 a9 ff  	sd	$9, 0x1058($sp)
  193eb4: 60 10 aa ff  	sd	$10, 0x1060($sp)
  193eb8: 68 10 ab ff  	sd	$11, 0x1068($sp)
  193ebc: 30 10 ac e7  	swc1	$f12, 0x1030($sp)
  193ec0: 34 10 ae e7  	swc1	$f14, 0x1034($sp)
  193ec4: 38 10 b0 e7  	swc1	$f16, 0x1038($sp)
  193ec8: d9 78 06 0c  	jal	0x19e364 <.text+0x9e364>
  193ecc: 3c 10 b2 e7  	swc1	$f18, 0x103c($sp)
  193ed0: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  193ed4: 2d 20 a0 03  	move	$4, $sp
  193ed8: 06 00 40 18  	blez	$2, 0x193ef4 <.text+0x93ef4>
  193edc: 2d 18 00 00  	move	$3, $zero
  193ee0: 2d 20 00 02  	move	$4, $16
  193ee4: 2d 30 40 00  	move	$6, $2
  193ee8: 6f 4f 06 0c  	jal	0x193dbc <.text+0x93dbc>
  193eec: 2d 28 a0 03  	move	$5, $sp
  193ef0: 2d 18 40 00  	move	$3, $2
  193ef4: 10 10 bf df  	ld	$ra, 0x1010($sp)
  193ef8: 2d 10 60 00  	move	$2, $3
  193efc: 00 10 b0 df  	ld	$16, 0x1000($sp)
  193f00: 08 00 e0 03  	jr	$ra
  193f04: 70 10 bd 27  	addiu	$sp, $sp, 0x1070
  193f08: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  193f0c: 2d 10 a0 00  	move	$2, $5
  193f10: 01 00 06 24  	addiu	$6, $zero, 0x1
  193f14: 2d 28 a0 03  	move	$5, $sp
  193f18: 10 00 bf ff  	sd	$ra, 0x10($sp)
  193f1c: 6f 4f 06 0c  	jal	0x193dbc <.text+0x93dbc>
  193f20: 00 00 a2 a3  	sb	$2, 0x0($sp)
  193f24: 00 00 a4 93  	lbu	$4, 0x0($sp)
  193f28: 01 00 42 38  	xori	$2, $2, 0x1
  193f2c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  193f30: 10 00 bf df  	ld	$ra, 0x10($sp)
  193f34: 0a 18 82 00  	movz	$3, $4, $2
  193f38: 2d 10 60 00  	move	$2, $3
  193f3c: 08 00 e0 03  	jr	$ra
  193f40: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  193f44: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  193f48: 10 00 b1 ff  	sd	$17, 0x10($sp)
  193f4c: 2d 88 80 00  	move	$17, $4
  193f50: 2d 20 a0 00  	move	$4, $5
  193f54: 20 00 bf ff  	sd	$ra, 0x20($sp)
  193f58: 00 00 b0 ff  	sd	$16, 0x0($sp)
  193f5c: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  193f60: 2d 80 a0 00  	move	$16, $5
  193f64: 2d 28 00 02  	move	$5, $16
  193f68: 2d 20 20 02  	move	$4, $17
  193f6c: 6f 4f 06 0c  	jal	0x193dbc <.text+0x93dbc>
  193f70: 2d 30 40 00  	move	$6, $2
  193f74: 00 00 b0 df  	ld	$16, 0x0($sp)
  193f78: 20 00 bf df  	ld	$ra, 0x20($sp)
  193f7c: 10 00 b1 df  	ld	$17, 0x10($sp)
  193f80: 08 00 e0 03  	jr	$ra
  193f84: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  193f88: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  193f8c: 40 00 b4 ff  	sd	$20, 0x40($sp)
  193f90: 2d a0 a0 00  	move	$20, $5
  193f94: 30 00 b3 ff  	sd	$19, 0x30($sp)
  193f98: 2d 98 00 00  	move	$19, $zero
  193f9c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  193fa0: 2d 80 80 00  	move	$16, $4
  193fa4: 50 00 bf ff  	sd	$ra, 0x50($sp)
  193fa8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  193fac: 05 00 80 10  	beqz	$4, 0x193fc4 <.text+0x93fc4>
  193fb0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  193fb4: 74 00 83 80  	lb	$3, 0x74($4)
  193fb8: 77 00 02 24  	addiu	$2, $zero, 0x77
  193fbc: 0b 00 62 50  	beql	$3, $2, 0x193fec <.text+0x93fec>
  193fc0: 04 00 80 ac  	sw	$zero, 0x4($4)
  193fc4: fe ff 03 24  	addiu	$3, $zero, -0x2 <.text+0xffffffffffeffffe>
  193fc8: 50 00 bf df  	ld	$ra, 0x50($sp)
  193fcc: 2d 10 60 00  	move	$2, $3
  193fd0: 40 00 b4 df  	ld	$20, 0x40($sp)
  193fd4: 30 00 b3 df  	ld	$19, 0x30($sp)
  193fd8: 20 00 b2 df  	ld	$18, 0x20($sp)
  193fdc: 10 00 b1 df  	ld	$17, 0x10($sp)
  193fe0: 00 00 b0 df  	ld	$16, 0x0($sp)
  193fe4: 08 00 e0 03  	jr	$ra
  193fe8: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  193fec: 14 00 02 8e  	lw	$2, 0x14($16)
  193ff0: 00 40 12 24  	addiu	$18, $zero, 0x4000
  193ff4: 23 88 42 02  	subu	$17, $18, $2
  193ff8: 20 00 20 56  	bnezl	$17, 0x19407c <.text+0x9407c>
  193ffc: 50 00 04 8e  	lw	$4, 0x50($16)
  194000: 14 00 60 16  	bnez	$19, 0x194054 <.text+0x94054>
  194004: 2d 20 00 02  	move	$4, $16
  194008: e8 43 06 0c  	jal	0x190fa0 <.text+0x90fa0>
  19400c: 2d 28 80 02  	move	$5, $20
  194010: 2d 18 40 00  	move	$3, $2
  194014: 04 00 20 16  	bnez	$17, 0x194028 <.text+0x94028>
  194018: 48 00 02 ae  	sw	$2, 0x48($16)
  19401c: fb ff 02 24  	addiu	$2, $zero, -0x5 <.text+0xffffffffffeffffb>
  194020: 01 00 62 50  	beql	$3, $2, 0x194028 <.text+0x94028>
  194024: 48 00 00 ae  	sw	$zero, 0x48($16)
  194028: 14 00 02 8e  	lw	$2, 0x14($16)
  19402c: 11 00 40 14  	bnez	$2, 0x194074 <.text+0x94074>
  194030: 2d 98 00 00  	move	$19, $zero
  194034: 48 00 03 8e  	lw	$3, 0x48($16)
  194038: 01 00 02 24  	addiu	$2, $zero, 0x1
  19403c: 01 00 62 50  	beql	$3, $2, 0x194044 <.text+0x94044>
  194040: 01 00 13 24  	addiu	$19, $zero, 0x1
  194044: 48 00 02 8e  	lw	$2, 0x48($16)
  194048: 02 00 42 2c  	sltiu	$2, $2, 0x2
  19404c: e8 ff 40 54  	bnezl	$2, 0x193ff0 <.text+0x93ff0>
  194050: 14 00 02 8e  	lw	$2, 0x14($16)
  194054: 48 00 03 8e  	lw	$3, 0x48($16)
  194058: 01 00 02 24  	addiu	$2, $zero, 0x1
  19405c: 03 00 62 10  	beq	$3, $2, 0x19406c <.text+0x9406c>
  194060: 2d 28 60 00  	move	$5, $3
  194064: d8 ff 00 10  	b	0x193fc8 <.text+0x93fc8>
  194068: 2d 18 a0 00  	move	$3, $5
  19406c: fd ff 00 10  	b	0x194064 <.text+0x94064>
  194070: 2d 28 00 00  	move	$5, $zero
  194074: f3 ff 00 10  	b	0x194044 <.text+0x94044>
  194078: 01 00 13 24  	addiu	$19, $zero, 0x1
  19407c: 2d 30 20 02  	move	$6, $17
  194080: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  194084: 58 00 05 8e  	lw	$5, 0x58($16)
  194088: 05 00 51 14  	bne	$2, $17, 0x1940a0 <.text+0x940a0>
  19408c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  194090: 58 00 02 8e  	lw	$2, 0x58($16)
  194094: 14 00 12 ae  	sw	$18, 0x14($16)
  194098: d9 ff 00 10  	b	0x194000 <.text+0x94000>
  19409c: 10 00 02 ae  	sw	$2, 0x10($16)
  1940a0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1940a4: c8 ff 00 10  	b	0x193fc8 <.text+0x93fc8>
  1940a8: 48 00 02 ae  	sw	$2, 0x48($16)
  1940ac: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1940b0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1940b4: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1940b8: e2 4f 06 0c  	jal	0x193f88 <.text+0x93f88>
  1940bc: 2d 80 80 00  	move	$16, $4
  1940c0: 05 00 40 14  	bnez	$2, 0x1940d8 <.text+0x940d8>
  1940c4: 2d 20 40 00  	move	$4, $2
  1940c8: 48 00 03 8e  	lw	$3, 0x48($16)
  1940cc: 2d 20 00 00  	move	$4, $zero
  1940d0: 01 00 62 38  	xori	$2, $3, 0x1
  1940d4: 0b 20 62 00  	movn	$4, $3, $2
  1940d8: 10 00 bf df  	ld	$ra, 0x10($sp)
  1940dc: 2d 10 80 00  	move	$2, $4
  1940e0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1940e4: 08 00 e0 03  	jr	$ra
  1940e8: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1940ec: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1940f0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1940f4: 2d 88 80 00  	move	$17, $4
  1940f8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1940fc: 2d 80 a0 00  	move	$16, $5
  194100: 69 00 80 10  	beqz	$4, 0x1942a8 <.text+0x942a8>
  194104: 20 00 bf ff  	sd	$ra, 0x20($sp)
  194108: 02 00 02 24  	addiu	$2, $zero, 0x2
  19410c: 66 00 c2 10  	beq	$6, $2, 0x1942a8 <.text+0x942a8>
  194110: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  194114: 48 00 83 8c  	lw	$3, 0x48($4)
  194118: 63 00 62 10  	beq	$3, $2, 0x1942a8 <.text+0x942a8>
  19411c: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  194120: 61 00 62 10  	beq	$3, $2, 0x1942a8 <.text+0x942a8>
  194124: 77 00 02 24  	addiu	$2, $zero, 0x77
  194128: 74 00 83 80  	lb	$3, 0x74($4)
  19412c: 42 00 62 10  	beq	$3, $2, 0x194238 <.text+0x94238>
  194130: 01 00 02 24  	addiu	$2, $zero, 0x1
  194134: 3e 00 c2 50  	beql	$6, $2, 0x194230 <.text+0x94230>
  194138: 18 00 82 dc  	ld	$2, 0x18($4)
  19413c: 1f 00 00 06  	bltz	$16, 0x1941bc <.text+0x941bc>
  194140: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  194144: 70 00 22 8e  	lw	$2, 0x70($17)
  194148: 2c 00 40 54  	bnezl	$2, 0x1941fc <.text+0x941fc>
  19414c: 54 00 22 8e  	lw	$2, 0x54($17)
  194150: 18 00 23 de  	ld	$3, 0x18($17)
  194154: 2b 10 03 02  	sltu	$2, $16, $3
  194158: 22 00 40 14  	bnez	$2, 0x1941e4 <.text+0x941e4>
  19415c: 00 00 00 00  	nop
  194160: 2f 80 03 02  	dsubu	$16, $16, $3
  194164: 15 00 00 52  	beqzl	$16, 0x1941bc <.text+0x941bc>
  194168: 18 00 23 de  	ld	$3, 0x18($17)
  19416c: 58 00 22 8e  	lw	$2, 0x58($17)
  194170: 18 00 40 10  	beqz	$2, 0x1941d4 <.text+0x941d4>
  194174: 00 00 00 00  	nop
  194178: 10 00 00 5a  	blezl	$16, 0x1941bc <.text+0x941bc>
  19417c: 18 00 23 de  	ld	$3, 0x18($17)
  194180: 00 40 03 2a  	slti	$3, $16, 0x4000
  194184: 3c 10 10 00  	dsll32	$2, $16, 0x0
  194188: 3f 10 02 00  	dsra32	$2, $2, 0x0
  19418c: 00 40 06 24  	addiu	$6, $zero, 0x4000
  194190: 58 00 25 8e  	lw	$5, 0x58($17)
  194194: 0b 30 43 00  	movn	$6, $2, $3
  194198: 8d 4e 06 0c  	jal	0x193a34 <.text+0x93a34>
  19419c: 2d 20 20 02  	move	$4, $17
  1941a0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1941a4: 2d 30 40 00  	move	$6, $2
  1941a8: 04 00 40 18  	blez	$2, 0x1941bc <.text+0x941bc>
  1941ac: 2f 80 06 02  	dsubu	$16, $16, $6
  1941b0: f4 ff 00 1e  	bgtz	$16, 0x194184 <.text+0x94184>
  1941b4: 00 40 03 2a  	slti	$3, $16, 0x4000
  1941b8: 18 00 23 de  	ld	$3, 0x18($17)
  1941bc: 20 00 bf df  	ld	$ra, 0x20($sp)
  1941c0: 2d 10 60 00  	move	$2, $3
  1941c4: 10 00 b1 df  	ld	$17, 0x10($sp)
  1941c8: 00 00 b0 df  	ld	$16, 0x0($sp)
  1941cc: 08 00 e0 03  	jr	$ra
  1941d0: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1941d4: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1941d8: 00 40 04 24  	addiu	$4, $zero, 0x4000
  1941dc: e6 ff 00 10  	b	0x194178 <.text+0x94178>
  1941e0: 58 00 22 ae  	sw	$2, 0x58($17)
  1941e4: b5 50 06 0c  	jal	0x1942d4 <.text+0x942d4>
  1941e8: 00 00 00 00  	nop
  1941ec: f3 ff 40 04  	bltz	$2, 0x1941bc <.text+0x941bc>
  1941f0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1941f4: db ff 00 10  	b	0x194164 <.text+0x94164>
  1941f8: 00 00 00 00  	nop
  1941fc: 3c 28 10 00  	dsll32	$5, $16, 0x0
  194200: 3f 28 05 00  	dsra32	$5, $5, 0x0
  194204: 04 00 20 ae  	sw	$zero, 0x4($17)
  194208: 2d 30 00 00  	move	$6, $zero
  19420c: 00 00 22 ae  	sw	$2, 0x0($17)
  194210: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  194214: 50 00 24 8e  	lw	$4, 0x50($17)
  194218: e8 ff 40 04  	bltz	$2, 0x1941bc <.text+0x941bc>
  19421c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  194220: 2d 18 00 02  	move	$3, $16
  194224: 08 00 30 fe  	sd	$16, 0x8($17)
  194228: e4 ff 00 10  	b	0x1941bc <.text+0x941bc>
  19422c: 18 00 30 fe  	sd	$16, 0x18($17)
  194230: c2 ff 00 10  	b	0x19413c <.text+0x9413c>
  194234: 2d 80 a2 00  	daddu	$16, $5, $2
  194238: 03 00 c0 14  	bnez	$6, 0x194248 <.text+0x94248>
  19423c: 00 00 00 00  	nop
  194240: 08 00 82 dc  	ld	$2, 0x8($4)
  194244: 2f 80 a2 00  	dsubu	$16, $5, $2
  194248: dc ff 00 06  	bltz	$16, 0x1941bc <.text+0x941bc>
  19424c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  194250: 54 00 22 8e  	lw	$2, 0x54($17)
  194254: 16 00 40 10  	beqz	$2, 0x1942b0 <.text+0x942b0>
  194258: 00 00 00 00  	nop
  19425c: d7 ff 00 5a  	blezl	$16, 0x1941bc <.text+0x941bc>
  194260: 08 00 23 de  	ld	$3, 0x8($17)
  194264: 00 40 03 2a  	slti	$3, $16, 0x4000
  194268: 3c 10 10 00  	dsll32	$2, $16, 0x0
  19426c: 3f 10 02 00  	dsra32	$2, $2, 0x0
  194270: 00 40 06 24  	addiu	$6, $zero, 0x4000
  194274: 54 00 25 8e  	lw	$5, 0x54($17)
  194278: 0b 30 43 00  	movn	$6, $2, $3
  19427c: 6f 4f 06 0c  	jal	0x193dbc <.text+0x93dbc>
  194280: 2d 20 20 02  	move	$4, $17
  194284: 2d 30 40 00  	move	$6, $2
  194288: 07 00 c0 10  	beqz	$6, 0x1942a8 <.text+0x942a8>
  19428c: 3c 10 06 00  	dsll32	$2, $6, 0x0
  194290: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  194294: 2f 80 02 02  	dsubu	$16, $16, $2
  194298: f3 ff 00 5e  	bgtzl	$16, 0x194268 <.text+0x94268>
  19429c: 00 40 03 2a  	slti	$3, $16, 0x4000
  1942a0: c6 ff 00 10  	b	0x1941bc <.text+0x941bc>
  1942a4: 08 00 23 de  	ld	$3, 0x8($17)
  1942a8: c4 ff 00 10  	b	0x1941bc <.text+0x941bc>
  1942ac: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1942b0: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1942b4: 00 40 04 24  	addiu	$4, $zero, 0x4000
  1942b8: 2d 28 00 00  	move	$5, $zero
  1942bc: 54 00 22 ae  	sw	$2, 0x54($17)
  1942c0: 00 40 06 24  	addiu	$6, $zero, 0x4000
  1942c4: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  1942c8: 2d 20 40 00  	move	$4, $2
  1942cc: e3 ff 00 10  	b	0x19425c <.text+0x9425c>
  1942d0: 00 00 00 00  	nop
  1942d4: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1942d8: ff ff 07 24  	addiu	$7, $zero, -0x1 <.text+0xffffffffffefffff>
  1942dc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1942e0: 2d 80 80 00  	move	$16, $4
  1942e4: 07 00 80 10  	beqz	$4, 0x194304 <.text+0x94304>
  1942e8: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1942ec: 74 00 03 82  	lb	$3, 0x74($16)
  1942f0: 72 00 02 24  	addiu	$2, $zero, 0x72
  1942f4: 2d 28 00 00  	move	$5, $zero
  1942f8: 2d 30 00 00  	move	$6, $zero
  1942fc: 06 00 62 10  	beq	$3, $2, 0x194318 <.text+0x94318>
  194300: 2d 20 00 00  	move	$4, $zero
  194304: 10 00 bf df  	ld	$ra, 0x10($sp)
  194308: 2d 10 e0 00  	move	$2, $7
  19430c: 00 00 b0 df  	ld	$16, 0x0($sp)
  194310: 08 00 e0 03  	jr	$ra
  194314: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  194318: 54 00 02 8e  	lw	$2, 0x54($16)
  19431c: 48 00 00 ae  	sw	$zero, 0x48($16)
  194320: 4c 00 00 ae  	sw	$zero, 0x4c($16)
  194324: 00 00 02 ae  	sw	$2, 0x0($16)
  194328: 28 4c 06 0c  	jal	0x1930a0 <.text+0x930a0>
  19432c: 04 00 00 ae  	sw	$zero, 0x4($16)
  194330: 2d 30 00 00  	move	$6, $zero
  194334: 78 00 03 de  	ld	$3, 0x78($16)
  194338: 2d 20 00 02  	move	$4, $16
  19433c: 2d 28 00 00  	move	$5, $zero
  194340: 05 00 60 14  	bnez	$3, 0x194358 <.text+0x94358>
  194344: 60 00 02 fe  	sd	$2, 0x60($16)
  194348: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  19434c: 50 00 04 8e  	lw	$4, 0x50($16)
  194350: ec ff 00 10  	b	0x194304 <.text+0x94304>
  194354: 2d 38 00 00  	move	$7, $zero
  194358: cb 49 06 0c  	jal	0x19272c <.text+0x9272c>
  19435c: 00 00 00 00  	nop
  194360: 50 00 04 8e  	lw	$4, 0x50($16)
  194364: 78 00 05 8e  	lw	$5, 0x78($16)
  194368: d8 74 06 0c  	jal	0x19d360 <.text+0x9d360>
  19436c: 2d 30 00 00  	move	$6, $zero
  194370: e4 ff 00 10  	b	0x194304 <.text+0x94304>
  194374: 2d 38 40 00  	move	$7, $2
  194378: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  19437c: 2d 28 00 00  	move	$5, $zero
  194380: 00 00 bf ff  	sd	$ra, 0x0($sp)
  194384: 3b 50 06 0c  	jal	0x1940ec <.text+0x940ec>
  194388: 01 00 06 24  	addiu	$6, $zero, 0x1
  19438c: 00 00 bf df  	ld	$ra, 0x0($sp)
  194390: 08 00 e0 03  	jr	$ra
  194394: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  194398: 05 00 80 10  	beqz	$4, 0x1943b0 <.text+0x943b0>
  19439c: 00 00 00 00  	nop
  1943a0: 74 00 83 80  	lb	$3, 0x74($4)
  1943a4: 72 00 02 24  	addiu	$2, $zero, 0x72
  1943a8: 03 00 62 10  	beq	$3, $2, 0x1943b8 <.text+0x943b8>
  1943ac: 00 00 00 00  	nop
  1943b0: 08 00 e0 03  	jr	$ra
  1943b4: 2d 10 00 00  	move	$2, $zero
  1943b8: 08 00 e0 03  	jr	$ra
  1943bc: 4c 00 82 8c  	lw	$2, 0x4c($4)
  1943c0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1943c4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1943c8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1943cc: 2d 90 80 00  	move	$18, $4
  1943d0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1943d4: 03 00 11 24  	addiu	$17, $zero, 0x3
  1943d8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1943dc: 2d 80 a0 00  	move	$16, $5
  1943e0: ff 00 04 32  	andi	$4, $16, 0xff
  1943e4: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  1943e8: 2d 28 40 02  	move	$5, $18
  1943ec: 3c 20 04 00  	dsll32	$4, $4, 0x0
  1943f0: 3f 20 04 00  	dsra32	$4, $4, 0x0
  1943f4: 4d 75 06 0c  	jal	0x19d534 <.text+0x9d534>
  1943f8: 3a 82 10 00  	dsrl	$16, $16, 0x8
  1943fc: f9 ff 21 06  	bgez	$17, 0x1943e4 <.text+0x943e4>
  194400: ff 00 04 32  	andi	$4, $16, 0xff
  194404: 30 00 bf df  	ld	$ra, 0x30($sp)
  194408: 20 00 b2 df  	ld	$18, 0x20($sp)
  19440c: 10 00 b1 df  	ld	$17, 0x10($sp)
  194410: 00 00 b0 df  	ld	$16, 0x0($sp)
  194414: 08 00 e0 03  	jr	$ra
  194418: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  19441c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  194420: 10 00 b1 ff  	sd	$17, 0x10($sp)
  194424: 2d 88 80 00  	move	$17, $4
  194428: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19442c: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  194430: 00 00 b0 ff  	sd	$16, 0x0($sp)
  194434: 2d 20 20 02  	move	$4, $17
  194438: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  19443c: 2d 80 40 00  	move	$16, $2
  194440: 2d 20 20 02  	move	$4, $17
  194444: 38 12 02 00  	dsll	$2, $2, 0x8
  194448: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  19444c: 2d 80 02 02  	daddu	$16, $16, $2
  194450: 2d 20 20 02  	move	$4, $17
  194454: 38 14 02 00  	dsll	$2, $2, 0x10
  194458: 9d 4d 06 0c  	jal	0x193674 <.text+0x93674>
  19445c: 2d 80 02 02  	daddu	$16, $16, $2
  194460: 2d 18 40 00  	move	$3, $2
  194464: 38 1e 03 00  	dsll	$3, $3, 0x18
  194468: 2d 80 03 02  	daddu	$16, $16, $3
  19446c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  194470: 07 00 43 10  	beq	$2, $3, 0x194490 <.text+0x94490>
  194474: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  194478: 2d 10 00 02  	move	$2, $16
  19447c: 20 00 bf df  	ld	$ra, 0x20($sp)
  194480: 10 00 b1 df  	ld	$17, 0x10($sp)
  194484: 00 00 b0 df  	ld	$16, 0x0($sp)
  194488: 08 00 e0 03  	jr	$ra
  19448c: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  194490: f9 ff 00 10  	b	0x194478 <.text+0x94478>
  194494: 48 00 22 ae  	sw	$2, 0x48($17)
  194498: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  19449c: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  1944a0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1944a4: 2d 80 80 00  	move	$16, $4
  1944a8: 08 00 80 10  	beqz	$4, 0x1944cc <.text+0x944cc>
  1944ac: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1944b0: 74 00 83 80  	lb	$3, 0x74($4)
  1944b4: 77 00 02 24  	addiu	$2, $zero, 0x77
  1944b8: 08 00 62 10  	beq	$3, $2, 0x1944dc <.text+0x944dc>
  1944bc: 04 00 05 24  	addiu	$5, $zero, 0x4
  1944c0: 2d 20 00 02  	move	$4, $16
  1944c4: 45 4e 06 0c  	jal	0x193914 <.text+0x93914>
  1944c8: 00 00 00 00  	nop
  1944cc: 10 00 bf df  	ld	$ra, 0x10($sp)
  1944d0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1944d4: 08 00 e0 03  	jr	$ra
  1944d8: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1944dc: e2 4f 06 0c  	jal	0x193f88 <.text+0x93f88>
  1944e0: 00 00 00 00  	nop
  1944e4: f7 ff 40 14  	bnez	$2, 0x1944c4 <.text+0x944c4>
  1944e8: 2d 20 00 02  	move	$4, $16
  1944ec: 50 00 04 8e  	lw	$4, 0x50($16)
  1944f0: f0 50 06 0c  	jal	0x1943c0 <.text+0x943c0>
  1944f4: 60 00 05 de  	ld	$5, 0x60($16)
  1944f8: 50 00 04 8e  	lw	$4, 0x50($16)
  1944fc: f0 50 06 0c  	jal	0x1943c0 <.text+0x943c0>
  194500: 08 00 05 de  	ld	$5, 0x8($16)
  194504: ef ff 00 10  	b	0x1944c4 <.text+0x944c4>
  194508: 2d 20 00 02  	move	$4, $16
  19450c: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  194510: 10 00 b1 ff  	sd	$17, 0x10($sp)
  194514: 2d 88 80 00  	move	$17, $4
  194518: 30 00 bf ff  	sd	$ra, 0x30($sp)
  19451c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  194520: 3c 00 80 10  	beqz	$4, 0x194614 <.text+0x94614>
  194524: 00 00 b0 ff  	sd	$16, 0x0($sp)
  194528: 48 00 86 8c  	lw	$6, 0x48($4)
  19452c: 0a 00 c0 14  	bnez	$6, 0x194558 <.text+0x94558>
  194530: 00 00 a6 ac  	sw	$6, 0x0($5)
  194534: 1c 00 02 3c  	lui	$2, 0x1c
  194538: a8 92 44 24  	addiu	$4, $2, -0x6d58 <.text+0xffffffffffef92a8>
  19453c: 30 00 bf df  	ld	$ra, 0x30($sp)
  194540: 2d 10 80 00  	move	$2, $4
  194544: 20 00 b2 df  	ld	$18, 0x20($sp)
  194548: 10 00 b1 df  	ld	$17, 0x10($sp)
  19454c: 00 00 b0 df  	ld	$16, 0x0($sp)
  194550: 08 00 e0 03  	jr	$ra
  194554: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  194558: 1c 00 02 3c  	lui	$2, 0x1c
  19455c: a8 92 52 24  	addiu	$18, $2, -0x6d58 <.text+0xffffffffffef92a8>
  194560: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  194564: 01 00 c2 54  	bnel	$6, $2, 0x19456c <.text+0x9456c>
  194568: 20 00 92 8c  	lw	$18, 0x20($4)
  19456c: 05 00 40 52  	beqzl	$18, 0x194584 <.text+0x94584>
  194570: 48 00 22 8e  	lw	$2, 0x48($17)
  194574: 00 00 42 82  	lb	$2, 0x0($18)
  194578: 0a 00 40 54  	bnezl	$2, 0x1945a4 <.text+0x945a4>
  19457c: 68 00 24 8e  	lw	$4, 0x68($17)
  194580: 48 00 22 8e  	lw	$2, 0x48($17)
  194584: 02 00 03 24  	addiu	$3, $zero, 0x2
  194588: 23 18 62 00  	subu	$3, $3, $2
  19458c: 42 00 02 3c  	lui	$2, 0x42
  194590: 80 18 03 00  	sll	$3, $3, 0x2
  194594: 00 5a 42 24  	addiu	$2, $2, 0x5a00
  194598: 21 18 62 00  	addu	$3, $3, $2
  19459c: 00 00 72 8c  	lw	$18, 0x0($3)
  1945a0: 68 00 24 8e  	lw	$4, 0x68($17)
  1945a4: 17 00 80 14  	bnez	$4, 0x194604 <.text+0x94604>
  1945a8: 00 00 00 00  	nop
  1945ac: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  1945b0: 6c 00 24 8e  	lw	$4, 0x6c($17)
  1945b4: 2d 20 40 02  	move	$4, $18
  1945b8: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  1945bc: 2d 80 40 00  	move	$16, $2
  1945c0: 21 80 02 02  	addu	$16, $16, $2
  1945c4: 03 00 10 26  	addiu	$16, $16, 0x3
  1945c8: 2d 79 06 0c  	jal	0x19e4b4 <.text+0x9e4b4>
  1945cc: 2d 20 00 02  	move	$4, $16
  1945d0: 6c 00 25 8e  	lw	$5, 0x6c($17)
  1945d4: 68 00 22 ae  	sw	$2, 0x68($17)
  1945d8: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  1945dc: 2d 20 40 00  	move	$4, $2
  1945e0: 1c 00 05 3c  	lui	$5, 0x1c
  1945e4: 68 00 24 8e  	lw	$4, 0x68($17)
  1945e8: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  1945ec: b0 92 a5 24  	addiu	$5, $5, -0x6d50 <.text+0xffffffffffef92b0>
  1945f0: 68 00 24 8e  	lw	$4, 0x68($17)
  1945f4: f5 70 06 0c  	jal	0x19c3d4 <.text+0x9c3d4>
  1945f8: 2d 28 40 02  	move	$5, $18
  1945fc: cf ff 00 10  	b	0x19453c <.text+0x9453c>
  194600: 68 00 24 8e  	lw	$4, 0x68($17)
  194604: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  194608: 00 00 00 00  	nop
  19460c: e7 ff 00 10  	b	0x1945ac <.text+0x945ac>
  194610: 00 00 00 00  	nop
  194614: 42 00 02 3c  	lui	$2, 0x42
  194618: 10 5a 44 8c  	lw	$4, 0x5a10($2)
  19461c: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  194620: c6 ff 00 10  	b	0x19453c <.text+0x9453c>
  194624: 00 00 a2 ac  	sw	$2, 0x0($5)
