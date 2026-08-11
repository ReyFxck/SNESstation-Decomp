
/mnt/data/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  1a8420: 14 00 02 24  	addiu	$2, $zero, 0x14
  1a8424: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a8428: 18 28 a2 00  	<unknown>
  1a842c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a8430: 28 00 02 24  	addiu	$2, $zero, 0x28
  1a8434: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a8438: 18 18 82 00  	<unknown>
  1a843c: 44 00 02 3c  	lui	$2, 0x44
  1a8440: 50 78 42 24  	addiu	$2, $2, 0x7850
  1a8444: 21 20 65 00  	addu	$4, $3, $5
  1a8448: 21 20 82 00  	addu	$4, $4, $2
  1a844c: 0c 00 90 8c  	lw	$16, 0xc($4)
  1a8450: 00 01 05 26  	addiu	$5, $16, 0x100
  1a8454: f0 ac 06 0c  	jal	0x1ab3c0 <.text+0xab3c0>
  1a8458: 2d 20 00 02  	move	$4, $16
  1a845c: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a8460: 58 00 02 8e  	lw	$2, 0x58($16)
  1a8464: 80 00 04 26  	addiu	$4, $16, 0x80
  1a8468: d8 00 03 8e  	lw	$3, 0xd8($16)
  1a846c: 2b 10 43 00  	sltu	$2, $2, $3
  1a8470: 0b 80 82 00  	movn	$16, $4, $2
  1a8474: 2d 10 00 02  	move	$2, $16
  1a8478: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a847c: 08 00 e0 03  	jr	$ra
  1a8480: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a8484: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a8488: 42 00 02 3c  	lui	$2, 0x42
  1a848c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a8490: 24 65 42 8c  	lw	$2, 0x6524($2)
  1a8494: 5b 00 40 14  	bnez	$2, 0x1a8604 <.text+0xa8604>
  1a8498: 2d 18 00 00  	move	$3, $zero
  1a849c: 44 00 02 3c  	lui	$2, 0x44
  1a84a0: 80 77 42 24  	addiu	$2, $2, 0x7780
  1a84a4: 4c 00 40 ac  	sw	$zero, 0x4c($2)
  1a84a8: 24 00 40 ac  	sw	$zero, 0x24($2)
  1a84ac: 00 80 05 3c  	lui	$5, 0x8000
  1a84b0: 44 00 04 3c  	lui	$4, 0x44
  1a84b4: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a84b8: 00 01 a5 34  	ori	$5, $5, 0x100
  1a84bc: a2 71 06 0c  	jal	0x19c688 <.text+0x9c688>
  1a84c0: 2d 30 00 00  	move	$6, $zero
  1a84c4: 4f 00 40 04  	bltz	$2, 0x1a8604 <.text+0xa8604>
  1a84c8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a84cc: 0f 00 03 3c  	lui	$3, 0xf
  1a84d0: ff ff 63 34  	ori	$3, $3, 0xffff
		...
  1a84e8: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  1a84ec: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
		...
  1a84fc: f5 ff 62 14  	bne	$3, $2, 0x1a84d4 <.text+0xa84d4>
  1a8500: 44 00 02 3c  	lui	$2, 0x44
  1a8504: a4 77 42 8c  	lw	$2, 0x77a4($2)
  1a8508: e9 ff 40 10  	beqz	$2, 0x1a84b0 <.text+0xa84b0>
  1a850c: 00 80 05 3c  	lui	$5, 0x8000
  1a8510: 44 00 04 3c  	lui	$4, 0x44
  1a8514: a8 77 84 24  	addiu	$4, $4, 0x77a8
  1a8518: 01 01 a5 34  	ori	$5, $5, 0x101
  1a851c: a2 71 06 0c  	jal	0x19c688 <.text+0x9c688>
  1a8520: 2d 30 00 00  	move	$6, $zero
  1a8524: 37 00 40 04  	bltz	$2, 0x1a8604 <.text+0xa8604>
  1a8528: fd ff 03 24  	addiu	$3, $zero, -0x3 <.text+0xffffffffffeffffd>
  1a852c: 0f 00 03 3c  	lui	$3, 0xf
  1a8530: ff ff 63 34  	ori	$3, $3, 0xffff
		...
  1a8548: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  1a854c: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
		...
  1a855c: f5 ff 62 14  	bne	$3, $2, 0x1a8534 <.text+0xa8534>
  1a8560: 44 00 02 3c  	lui	$2, 0x44
  1a8564: cc 77 42 8c  	lw	$2, 0x77cc($2)
  1a8568: e9 ff 40 10  	beqz	$2, 0x1a8510 <.text+0xa8510>
  1a856c: 00 80 05 3c  	lui	$5, 0x8000
  1a8570: af a2 06 0c  	jal	0x1a8abc <.text+0xa8abc>
  1a8574: 00 00 00 00  	nop
  1a8578: 2d 20 00 00  	move	$4, $zero
  1a857c: 28 00 02 24  	addiu	$2, $zero, 0x28
  1a8580: 44 00 03 3c  	lui	$3, 0x44
  1a8584: 18 28 82 00  	<unknown>
  1a8588: 50 78 63 24  	addiu	$3, $3, 0x7850
  1a858c: 01 00 84 24  	addiu	$4, $4, 0x1
  1a8590: 21 10 a3 00  	addu	$2, $5, $3
  1a8594: 08 00 83 28  	slti	$3, $4, 0x8
  1a8598: 1c 00 40 ac  	sw	$zero, 0x1c($2)
  1a859c: 00 00 40 ac  	sw	$zero, 0x0($2)
  1a85a0: 04 00 40 ac  	sw	$zero, 0x4($2)
  1a85a4: 08 00 40 ac  	sw	$zero, 0x8($2)
  1a85a8: 14 00 40 ac  	sw	$zero, 0x14($2)
  1a85ac: f3 ff 60 14  	bnez	$3, 0x1a857c <.text+0xa857c>
  1a85b0: 18 00 40 ac  	sw	$zero, 0x18($2)
  1a85b4: 44 00 03 3c  	lui	$3, 0x44
  1a85b8: 44 00 04 3c  	lui	$4, 0x44
  1a85bc: d0 77 67 24  	addiu	$7, $3, 0x77d0
  1a85c0: 10 00 02 24  	addiu	$2, $zero, 0x10
  1a85c4: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a85c8: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a85cc: 2d 30 00 00  	move	$6, $zero
  1a85d0: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a85d4: 2d 48 e0 00  	move	$9, $7
  1a85d8: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a85dc: 2d 58 00 00  	move	$11, $zero
  1a85e0: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a85e4: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a85e8: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a85ec: 05 00 40 04  	bltz	$2, 0x1a8604 <.text+0xa8604>
  1a85f0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a85f4: 01 00 03 24  	addiu	$3, $zero, 0x1
  1a85f8: 42 00 02 3c  	lui	$2, 0x42
  1a85fc: 24 65 43 ac  	sw	$3, 0x6524($2)
  1a8600: 2d 18 00 00  	move	$3, $zero
  1a8604: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a8608: 2d 10 60 00  	move	$2, $3
  1a860c: 08 00 e0 03  	jr	$ra
  1a8610: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a8614: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a8618: 44 00 03 3c  	lui	$3, 0x44
  1a861c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a8620: 44 00 04 3c  	lui	$4, 0x44
  1a8624: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a8628: 0f 00 02 24  	addiu	$2, $zero, 0xf
  1a862c: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8630: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8634: 2d 30 00 00  	move	$6, $zero
  1a8638: 2d 38 00 02  	move	$7, $16
  1a863c: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a8640: 2d 48 00 02  	move	$9, $16
  1a8644: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8648: 2d 58 00 00  	move	$11, $zero
  1a864c: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a8650: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a8654: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8658: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a865c: 05 00 40 04  	bltz	$2, 0x1a8674 <.text+0xa8674>
  1a8660: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8664: 0c 00 03 8e  	lw	$3, 0xc($16)
  1a8668: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a866c: 06 00 62 10  	beq	$3, $2, 0x1a8688 <.text+0xa8688>
  1a8670: 42 00 02 3c  	lui	$2, 0x42
  1a8674: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8678: 2d 10 60 00  	move	$2, $3
  1a867c: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a8680: 08 00 e0 03  	jr	$ra
  1a8684: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a8688: fa ff 00 10  	b	0x1a8674 <.text+0xa8674>
  1a868c: 24 65 40 ac  	sw	$zero, 0x6524($2)
  1a8690: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  1a8694: 3f 00 c2 30  	andi	$2, $6, 0x3f
  1a8698: 50 00 b4 ff  	sd	$20, 0x50($sp)
  1a869c: 2d a0 80 00  	move	$20, $4
  1a86a0: 40 00 b3 ff  	sd	$19, 0x40($sp)
  1a86a4: 2d 98 a0 00  	move	$19, $5
  1a86a8: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a86ac: 2d 28 00 00  	move	$5, $zero
  1a86b0: 60 00 bf ff  	sd	$ra, 0x60($sp)
  1a86b4: 2d 90 c0 00  	move	$18, $6
  1a86b8: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a86bc: 33 00 40 14  	bnez	$2, 0x1a878c <.text+0xa878c>
  1a86c0: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a86c4: 2d 80 c0 00  	move	$16, $6
  1a86c8: 01 00 11 24  	addiu	$17, $zero, 0x1
  1a86cc: 2d 20 00 02  	move	$4, $16
  1a86d0: ff 00 05 24  	addiu	$5, $zero, 0xff
  1a86d4: 20 00 06 24  	addiu	$6, $zero, 0x20
  1a86d8: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  1a86dc: ff ff 31 26  	addiu	$17, $17, -0x1 <.text+0xffffffffffefffff>
  1a86e0: 60 00 00 ae  	sw	$zero, 0x60($16)
  1a86e4: 05 00 02 24  	addiu	$2, $zero, 0x5
  1a86e8: 67 00 00 a2  	sb	$zero, 0x67($16)
  1a86ec: 70 00 02 a2  	sb	$2, 0x70($16)
  1a86f0: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a86f4: 71 00 02 a2  	sb	$2, 0x71($16)
  1a86f8: 58 00 00 ae  	sw	$zero, 0x58($16)
  1a86fc: 72 00 00 a2  	sb	$zero, 0x72($16)
  1a8700: f2 ff 21 06  	bgez	$17, 0x1a86cc <.text+0xa86cc>
  1a8704: 80 00 10 26  	addiu	$16, $16, 0x80
  1a8708: 44 00 02 3c  	lui	$2, 0x44
  1a870c: 01 00 11 24  	addiu	$17, $zero, 0x1
  1a8710: d0 77 50 24  	addiu	$16, $2, 0x77d0
  1a8714: d0 77 51 ac  	sw	$17, 0x77d0($2)
  1a8718: 44 00 04 3c  	lui	$4, 0x44
  1a871c: 04 00 14 ae  	sw	$20, 0x4($16)
  1a8720: 08 00 13 ae  	sw	$19, 0x8($16)
  1a8724: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8728: 10 00 12 ae  	sw	$18, 0x10($16)
  1a872c: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8730: 2d 30 00 00  	move	$6, $zero
  1a8734: 2d 38 00 02  	move	$7, $16
  1a8738: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a873c: 2d 48 00 02  	move	$9, $16
  1a8740: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8744: 2d 58 00 00  	move	$11, $zero
  1a8748: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a874c: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8750: 0e 00 40 04  	bltz	$2, 0x1a878c <.text+0xa878c>
  1a8754: 2d 28 00 00  	move	$5, $zero
  1a8758: 28 00 03 24  	addiu	$3, $zero, 0x28
  1a875c: 14 00 02 24  	addiu	$2, $zero, 0x14
  1a8760: 18 10 62 02  	<unknown>
  1a8764: 18 30 83 02  	<unknown>
  1a8768: 14 00 04 8e  	lw	$4, 0x14($16)
  1a876c: 0c 00 05 8e  	lw	$5, 0xc($16)
  1a8770: 21 18 c2 00  	addu	$3, $6, $2
  1a8774: 44 00 02 3c  	lui	$2, 0x44
  1a8778: 50 78 42 24  	addiu	$2, $2, 0x7850
  1a877c: 21 18 62 00  	addu	$3, $3, $2
  1a8780: 10 00 64 ac  	sw	$4, 0x10($3)
  1a8784: 00 00 71 ac  	sw	$17, 0x0($3)
  1a8788: 0c 00 72 ac  	sw	$18, 0xc($3)
  1a878c: 60 00 bf df  	ld	$ra, 0x60($sp)
  1a8790: 2d 10 a0 00  	move	$2, $5
  1a8794: 50 00 b4 df  	ld	$20, 0x50($sp)
  1a8798: 40 00 b3 df  	ld	$19, 0x40($sp)
  1a879c: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a87a0: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a87a4: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a87a8: 08 00 e0 03  	jr	$ra
  1a87ac: 70 00 bd 27  	addiu	$sp, $sp, 0x70
  1a87b0: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a87b4: 44 00 03 3c  	lui	$3, 0x44
  1a87b8: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a87bc: 0e 00 02 24  	addiu	$2, $zero, 0xe
  1a87c0: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a87c4: 2d 90 80 00  	move	$18, $4
  1a87c8: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a87cc: 44 00 04 3c  	lui	$4, 0x44
  1a87d0: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a87d4: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a87d8: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a87dc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a87e0: 2d 88 a0 00  	move	$17, $5
  1a87e4: 2d 30 00 00  	move	$6, $zero
  1a87e8: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a87ec: 2d 38 00 02  	move	$7, $16
  1a87f0: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a87f4: 2d 48 00 02  	move	$9, $16
  1a87f8: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a87fc: 2d 58 00 00  	move	$11, $zero
  1a8800: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1a8804: 10 00 02 ae  	sw	$2, 0x10($16)
  1a8808: 04 00 12 ae  	sw	$18, 0x4($16)
  1a880c: 08 00 11 ae  	sw	$17, 0x8($16)
  1a8810: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8814: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8818: 0b 00 40 04  	bltz	$2, 0x1a8848 <.text+0xa8848>
  1a881c: 2d 20 40 00  	move	$4, $2
  1a8820: 28 00 03 24  	addiu	$3, $zero, 0x28
  1a8824: 14 00 02 24  	addiu	$2, $zero, 0x14
  1a8828: 18 10 22 02  	<unknown>
  1a882c: 18 28 43 02  	<unknown>
  1a8830: 0c 00 04 8e  	lw	$4, 0xc($16)
  1a8834: 21 18 a2 00  	addu	$3, $5, $2
  1a8838: 44 00 02 3c  	lui	$2, 0x44
  1a883c: 50 78 42 24  	addiu	$2, $2, 0x7850
  1a8840: 21 18 62 00  	addu	$3, $3, $2
  1a8844: 00 00 60 ac  	sw	$zero, 0x0($3)
  1a8848: 40 00 bf df  	ld	$ra, 0x40($sp)
  1a884c: 2d 10 80 00  	move	$2, $4
  1a8850: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a8854: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a8858: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a885c: 08 00 e0 03  	jr	$ra
  1a8860: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a8864: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a8868: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a886c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a8870: 2d 88 c0 00  	move	$17, $6
  1a8874: 08 a1 06 0c  	jal	0x1a8420 <.text+0xa8420>
  1a8878: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a887c: 60 00 46 8c  	lw	$6, 0x60($2)
  1a8880: 2d 80 40 00  	move	$16, $2
  1a8884: 2d 20 20 02  	move	$4, $17
  1a8888: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  1a888c: 2d 28 40 00  	move	$5, $2
  1a8890: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a8894: 60 00 02 92  	lbu	$2, 0x60($16)
  1a8898: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a889c: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a88a0: 08 00 e0 03  	jr	$ra
  1a88a4: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a88a8: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a88ac: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a88b0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a88b4: 2d 88 a0 00  	move	$17, $5
  1a88b8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a88bc: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a88c0: 08 a1 06 0c  	jal	0x1a8420 <.text+0xa8420>
  1a88c4: 2d 80 80 00  	move	$16, $4
  1a88c8: 70 00 52 90  	lbu	$18, 0x70($2)
  1a88cc: 2d 20 00 02  	move	$4, $16
  1a88d0: 06 00 02 24  	addiu	$2, $zero, 0x6
  1a88d4: 09 00 42 12  	beq	$18, $2, 0x1a88fc <.text+0xa88fc>
  1a88d8: 2d 28 20 02  	move	$5, $17
  1a88dc: 2d 20 40 02  	move	$4, $18
  1a88e0: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a88e4: 2d 10 80 00  	move	$2, $4
  1a88e8: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a88ec: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a88f0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a88f4: 08 00 e0 03  	jr	$ra
  1a88f8: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a88fc: 46 a2 06 0c  	jal	0x1a8918 <.text+0xa8918>
  1a8900: 00 00 00 00  	nop
  1a8904: 02 00 03 24  	addiu	$3, $zero, 0x2
  1a8908: f4 ff 43 14  	bne	$2, $3, 0x1a88dc <.text+0xa88dc>
  1a890c: 05 00 04 24  	addiu	$4, $zero, 0x5
  1a8910: f4 ff 00 10  	b	0x1a88e4 <.text+0xa88e4>
  1a8914: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a8918: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a891c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a8920: 08 a1 06 0c  	jal	0x1a8420 <.text+0xa8420>
  1a8924: 00 00 00 00  	nop
  1a8928: 71 00 42 90  	lbu	$2, 0x71($2)
  1a892c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a8930: 08 00 e0 03  	jr	$ra
  1a8934: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a8938: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a893c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a8940: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a8944: 08 a1 06 0c  	jal	0x1a8420 <.text+0xa8420>
  1a8948: 2d 80 c0 00  	move	$16, $6
  1a894c: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a8950: 71 00 50 a0  	sb	$16, 0x71($2)
  1a8954: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a8958: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a895c: 08 00 e0 03  	jr	$ra
  1a8960: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a8964: 1c 00 02 3c  	lui	$2, 0x1c
  1a8968: 00 19 04 00  	sll	$3, $4, 0x4
  1a896c: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a8970: c8 ac 42 24  	addiu	$2, $2, -0x5338 <.text+0xffffffffffefacc8>
  1a8974: 08 00 84 28  	slti	$4, $4, 0x8
  1a8978: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a897c: 04 00 80 14  	bnez	$4, 0x1a8990 <.text+0xa8990>
  1a8980: 21 18 62 00  	addu	$3, $3, $2
  1a8984: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a8988: 08 00 e0 03  	jr	$ra
  1a898c: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a8990: 2d 20 a0 00  	move	$4, $5
  1a8994: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  1a8998: 2d 28 60 00  	move	$5, $3
  1a899c: fa ff 00 10  	b	0x1a8988 <.text+0xa8988>
  1a89a0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a89a4: 1c 00 02 3c  	lui	$2, 0x1c
  1a89a8: 00 19 04 00  	sll	$3, $4, 0x4
  1a89ac: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a89b0: 48 ad 42 24  	addiu	$2, $2, -0x52b8 <.text+0xffffffffffefad48>
  1a89b4: 04 00 84 28  	slti	$4, $4, 0x4
  1a89b8: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a89bc: 04 00 80 14  	bnez	$4, 0x1a89d0 <.text+0xa89d0>
  1a89c0: 21 18 62 00  	addu	$3, $3, $2
  1a89c4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a89c8: 08 00 e0 03  	jr	$ra
  1a89cc: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a89d0: 2d 20 a0 00  	move	$4, $5
  1a89d4: 4a 71 06 0c  	jal	0x19c528 <.text+0x9c528>
  1a89d8: 2d 28 60 00  	move	$5, $3
  1a89dc: fa ff 00 10  	b	0x1a89c8 <.text+0xa89c8>
  1a89e0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a89e4: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a89e8: 44 00 03 3c  	lui	$3, 0x44
  1a89ec: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a89f0: 44 00 04 3c  	lui	$4, 0x44
  1a89f4: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a89f8: 0c 00 02 24  	addiu	$2, $zero, 0xc
  1a89fc: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8a00: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8a04: 2d 30 00 00  	move	$6, $zero
  1a8a08: 2d 38 00 02  	move	$7, $16
  1a8a0c: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a8a10: 2d 48 00 02  	move	$9, $16
  1a8a14: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8a18: 2d 58 00 00  	move	$11, $zero
  1a8a1c: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a8a20: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a8a24: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8a28: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8a2c: 02 00 40 04  	bltz	$2, 0x1a8a38 <.text+0xa8a38>
  1a8a30: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8a34: 0c 00 03 8e  	lw	$3, 0xc($16)
  1a8a38: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8a3c: 2d 10 60 00  	move	$2, $3
  1a8a40: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a8a44: 08 00 e0 03  	jr	$ra
  1a8a48: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a8a4c: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a8a50: 44 00 03 3c  	lui	$3, 0x44
  1a8a54: 2d 60 80 00  	move	$12, $4
  1a8a58: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a8a5c: 44 00 04 3c  	lui	$4, 0x44
  1a8a60: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a8a64: 0d 00 02 24  	addiu	$2, $zero, 0xd
  1a8a68: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8a6c: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8a70: 2d 30 00 00  	move	$6, $zero
  1a8a74: 2d 38 00 02  	move	$7, $16
  1a8a78: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a8a7c: 2d 48 00 02  	move	$9, $16
  1a8a80: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8a84: 2d 58 00 00  	move	$11, $zero
  1a8a88: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a8a8c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a8a90: 04 00 0c ae  	sw	$12, 0x4($16)
  1a8a94: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8a98: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8a9c: 02 00 40 04  	bltz	$2, 0x1a8aa8 <.text+0xa8aa8>
  1a8aa0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8aa4: 0c 00 03 8e  	lw	$3, 0xc($16)
  1a8aa8: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8aac: 2d 10 60 00  	move	$2, $3
  1a8ab0: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a8ab4: 08 00 e0 03  	jr	$ra
  1a8ab8: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a8abc: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a8ac0: 44 00 03 3c  	lui	$3, 0x44
  1a8ac4: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a8ac8: 44 00 04 3c  	lui	$4, 0x44
  1a8acc: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a8ad0: 12 00 02 24  	addiu	$2, $zero, 0x12
  1a8ad4: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8ad8: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8adc: 2d 30 00 00  	move	$6, $zero
  1a8ae0: 2d 38 00 02  	move	$7, $16
  1a8ae4: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a8ae8: 2d 48 00 02  	move	$9, $16
  1a8aec: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8af0: 2d 58 00 00  	move	$11, $zero
  1a8af4: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a8af8: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a8afc: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8b00: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8b04: 02 00 40 04  	bltz	$2, 0x1a8b10 <.text+0xa8b10>
  1a8b08: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8b0c: 0c 00 03 8e  	lw	$3, 0xc($16)
  1a8b10: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8b14: 2d 10 60 00  	move	$2, $3
  1a8b18: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a8b1c: 08 00 e0 03  	jr	$ra
  1a8b20: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a8b24: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a8b28: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a8b2c: 2d 88 e0 00  	move	$17, $7
  1a8b30: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a8b34: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a8b38: 08 a1 06 0c  	jal	0x1a8420 <.text+0xa8420>
  1a8b3c: 2d 80 c0 00  	move	$16, $6
  1a8b40: 72 00 43 90  	lbu	$3, 0x72($2)
  1a8b44: 2d 38 40 00  	move	$7, $2
  1a8b48: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a8b4c: 07 00 62 10  	beq	$3, $2, 0x1a8b6c <.text+0xa8b6c>
  1a8b50: 2d 20 00 00  	move	$4, $zero
  1a8b54: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8b58: 2d 10 80 00  	move	$2, $4
  1a8b5c: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a8b60: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a8b64: 08 00 e0 03  	jr	$ra
  1a8b68: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a8b6c: 71 00 e2 90  	lbu	$2, 0x71($7)
  1a8b70: 02 00 05 24  	addiu	$5, $zero, 0x2
  1a8b74: f8 ff 45 10  	beq	$2, $5, 0x1a8b58 <.text+0xa8b58>
  1a8b78: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8b7c: 22 00 05 12  	beq	$16, $5, 0x1a8c08 <.text+0xa8c08>
  1a8b80: 03 00 02 2a  	slti	$2, $16, 0x3
  1a8b84: 0a 00 40 10  	beqz	$2, 0x1a8bb0 <.text+0xa8bb0>
  1a8b88: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a8b8c: 03 00 03 52  	beql	$16, $3, 0x1a8b9c <.text+0xa8b9c>
  1a8b90: 65 00 e2 90  	lbu	$2, 0x65($7)
  1a8b94: f0 ff 00 10  	b	0x1a8b58 <.text+0xa8b58>
  1a8b98: 2d 20 00 00  	move	$4, $zero
  1a8b9c: 2d 20 00 00  	move	$4, $zero
  1a8ba0: f3 00 43 38  	xori	$3, $2, 0xf3
  1a8ba4: 02 11 02 00  	srl	$2, $2, 0x4
  1a8ba8: ea ff 00 10  	b	0x1a8b54 <.text+0xa8b54>
  1a8bac: 0b 20 43 00  	movn	$4, $2, $3
  1a8bb0: 10 00 02 12  	beq	$16, $2, 0x1a8bf4 <.text+0xa8bf4>
  1a8bb4: 04 00 02 24  	addiu	$2, $zero, 0x4
  1a8bb8: e6 ff 02 16  	bne	$16, $2, 0x1a8b54 <.text+0xa8b54>
  1a8bbc: 2d 20 00 00  	move	$4, $zero
  1a8bc0: 64 00 e2 90  	lbu	$2, 0x64($7)
  1a8bc4: e4 ff 40 10  	beqz	$2, 0x1a8b58 <.text+0xa8b58>
  1a8bc8: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8bcc: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8bd0: e1 ff 22 52  	beql	$17, $2, 0x1a8b58 <.text+0xa8b58>
  1a8bd4: 68 00 e4 90  	lbu	$4, 0x68($7)
  1a8bd8: 68 00 e2 90  	lbu	$2, 0x68($7)
  1a8bdc: 2a 10 51 00  	slt	$2, $2, $17
  1a8be0: dd ff 40 10  	beqz	$2, 0x1a8b58 <.text+0xa8b58>
  1a8be4: 40 10 11 00  	sll	$2, $17, 0x1
  1a8be8: 21 10 47 00  	addu	$2, $2, $7
  1a8bec: d9 ff 00 10  	b	0x1a8b54 <.text+0xa8b54>
  1a8bf0: 50 00 44 94  	lhu	$4, 0x50($2)
  1a8bf4: 64 00 e2 90  	lbu	$2, 0x64($7)
  1a8bf8: d6 ff 40 10  	beqz	$2, 0x1a8b54 <.text+0xa8b54>
  1a8bfc: 2d 20 00 00  	move	$4, $zero
  1a8c00: d4 ff 00 10  	b	0x1a8b54 <.text+0xa8b54>
  1a8c04: 69 00 e4 90  	lbu	$4, 0x69($7)
  1a8c08: 64 00 e2 90  	lbu	$2, 0x64($7)
  1a8c0c: d1 ff 43 10  	beq	$2, $3, 0x1a8b54 <.text+0xa8b54>
  1a8c10: 2d 20 00 00  	move	$4, $zero
  1a8c14: 69 00 e2 90  	lbu	$2, 0x69($7)
  1a8c18: f3 ff 00 10  	b	0x1a8be8 <.text+0xa8be8>
  1a8c1c: 40 10 02 00  	sll	$2, $2, 0x1
  1a8c20: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a8c24: 44 00 03 3c  	lui	$3, 0x44
  1a8c28: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a8c2c: 2d 60 c0 00  	move	$12, $6
  1a8c30: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a8c34: 2d 88 80 00  	move	$17, $4
  1a8c38: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a8c3c: 44 00 04 3c  	lui	$4, 0x44
  1a8c40: 2d 68 e0 00  	move	$13, $7
  1a8c44: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a8c48: 06 00 02 24  	addiu	$2, $zero, 0x6
  1a8c4c: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8c50: 2d 90 a0 00  	move	$18, $5
  1a8c54: 2d 30 00 00  	move	$6, $zero
  1a8c58: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8c5c: 2d 38 00 02  	move	$7, $16
  1a8c60: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a8c64: 2d 48 00 02  	move	$9, $16
  1a8c68: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8c6c: 2d 58 00 00  	move	$11, $zero
  1a8c70: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a8c74: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1a8c78: 0c 00 0c ae  	sw	$12, 0xc($16)
  1a8c7c: 10 00 0d ae  	sw	$13, 0x10($16)
  1a8c80: 04 00 11 ae  	sw	$17, 0x4($16)
  1a8c84: 08 00 12 ae  	sw	$18, 0x8($16)
  1a8c88: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8c8c: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8c90: 08 00 40 04  	bltz	$2, 0x1a8cb4 <.text+0xa8cb4>
  1a8c94: 2d 18 00 00  	move	$3, $zero
  1a8c98: 14 00 03 8e  	lw	$3, 0x14($16)
  1a8c9c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a8ca0: 2d 20 20 02  	move	$4, $17
  1a8ca4: 2d 28 40 02  	move	$5, $18
  1a8ca8: 09 00 62 10  	beq	$3, $2, 0x1a8cd0 <.text+0xa8cd0>
  1a8cac: 02 00 06 24  	addiu	$6, $zero, 0x2
  1a8cb0: 14 00 03 8e  	lw	$3, 0x14($16)
  1a8cb4: 40 00 bf df  	ld	$ra, 0x40($sp)
  1a8cb8: 2d 10 60 00  	move	$2, $3
  1a8cbc: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a8cc0: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a8cc4: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a8cc8: 08 00 e0 03  	jr	$ra
  1a8ccc: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a8cd0: 4e a2 06 0c  	jal	0x1a8938 <.text+0xa8938>
  1a8cd4: 00 00 00 00  	nop
  1a8cd8: f6 ff 00 10  	b	0x1a8cb4 <.text+0xa8cb4>
  1a8cdc: 14 00 03 8e  	lw	$3, 0x14($16)
  1a8ce0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a8ce4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a8ce8: 51 a3 06 0c  	jal	0x1a8d44 <.text+0xa8d44>
  1a8cec: 00 00 00 00  	nop
  1a8cf0: 03 00 03 3c  	lui	$3, 0x3
  1a8cf4: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a8cf8: ff ff 63 34  	ori	$3, $3, 0xffff
  1a8cfc: 26 10 43 00  	xor	$2, $2, $3
  1a8d00: 01 00 42 2c  	sltiu	$2, $2, 0x1
  1a8d04: 08 00 e0 03  	jr	$ra
  1a8d08: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a8d0c: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a8d10: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a8d14: 6f a3 06 0c  	jal	0x1a8dbc <.text+0xa8dbc>
  1a8d18: ff 0f 06 24  	addiu	$6, $zero, 0xfff
  1a8d1c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a8d20: 08 00 e0 03  	jr	$ra
  1a8d24: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a8d28: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a8d2c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a8d30: 6f a3 06 0c  	jal	0x1a8dbc <.text+0xa8dbc>
  1a8d34: 2d 30 00 00  	move	$6, $zero
  1a8d38: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a8d3c: 08 00 e0 03  	jr	$ra
  1a8d40: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a8d44: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a8d48: 44 00 03 3c  	lui	$3, 0x44
  1a8d4c: 2d 60 80 00  	move	$12, $4
  1a8d50: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a8d54: 44 00 04 3c  	lui	$4, 0x44
  1a8d58: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a8d5c: 2d 68 a0 00  	move	$13, $5
  1a8d60: 09 00 02 24  	addiu	$2, $zero, 0x9
  1a8d64: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8d68: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8d6c: 2d 30 00 00  	move	$6, $zero
  1a8d70: 2d 38 00 02  	move	$7, $16
  1a8d74: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a8d78: 2d 48 00 02  	move	$9, $16
  1a8d7c: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8d80: 2d 58 00 00  	move	$11, $zero
  1a8d84: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a8d88: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a8d8c: 04 00 0c ae  	sw	$12, 0x4($16)
  1a8d90: 08 00 0d ae  	sw	$13, 0x8($16)
  1a8d94: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8d98: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8d9c: 02 00 40 04  	bltz	$2, 0x1a8da8 <.text+0xa8da8>
  1a8da0: 2d 18 00 00  	move	$3, $zero
  1a8da4: 0c 00 03 8e  	lw	$3, 0xc($16)
  1a8da8: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8dac: 2d 10 60 00  	move	$2, $3
  1a8db0: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a8db4: 08 00 e0 03  	jr	$ra
  1a8db8: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a8dbc: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a8dc0: 44 00 03 3c  	lui	$3, 0x44
  1a8dc4: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a8dc8: 2d 60 c0 00  	move	$12, $6
  1a8dcc: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a8dd0: 2d 88 80 00  	move	$17, $4
  1a8dd4: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a8dd8: 44 00 04 3c  	lui	$4, 0x44
  1a8ddc: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a8de0: 0a 00 02 24  	addiu	$2, $zero, 0xa
  1a8de4: 2d 90 a0 00  	move	$18, $5
  1a8de8: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8dec: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8df0: 2d 30 00 00  	move	$6, $zero
  1a8df4: 2d 38 00 02  	move	$7, $16
  1a8df8: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a8dfc: 2d 48 00 02  	move	$9, $16
  1a8e00: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8e04: 2d 58 00 00  	move	$11, $zero
  1a8e08: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a8e0c: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1a8e10: 0c 00 0c ae  	sw	$12, 0xc($16)
  1a8e14: 04 00 11 ae  	sw	$17, 0x4($16)
  1a8e18: 08 00 12 ae  	sw	$18, 0x8($16)
  1a8e1c: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8e20: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8e24: 08 00 40 04  	bltz	$2, 0x1a8e48 <.text+0xa8e48>
  1a8e28: 2d 18 00 00  	move	$3, $zero
  1a8e2c: 10 00 03 8e  	lw	$3, 0x10($16)
  1a8e30: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a8e34: 2d 20 20 02  	move	$4, $17
  1a8e38: 2d 28 40 02  	move	$5, $18
  1a8e3c: 09 00 62 10  	beq	$3, $2, 0x1a8e64 <.text+0xa8e64>
  1a8e40: 02 00 06 24  	addiu	$6, $zero, 0x2
  1a8e44: 10 00 03 8e  	lw	$3, 0x10($16)
  1a8e48: 40 00 bf df  	ld	$ra, 0x40($sp)
  1a8e4c: 2d 10 60 00  	move	$2, $3
  1a8e50: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a8e54: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a8e58: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a8e5c: 08 00 e0 03  	jr	$ra
  1a8e60: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a8e64: 4e a2 06 0c  	jal	0x1a8938 <.text+0xa8938>
  1a8e68: 00 00 00 00  	nop
  1a8e6c: f6 ff 00 10  	b	0x1a8e48 <.text+0xa8e48>
  1a8e70: 10 00 03 8e  	lw	$3, 0x10($16)
  1a8e74: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a8e78: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a8e7c: 2d 88 e0 00  	move	$17, $7
  1a8e80: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a8e84: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a8e88: 08 a1 06 0c  	jal	0x1a8420 <.text+0xa8420>
  1a8e8c: 2d 80 c0 00  	move	$16, $6
  1a8e90: 72 00 43 90  	lbu	$3, 0x72($2)
  1a8e94: 2d 30 40 00  	move	$6, $2
  1a8e98: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a8e9c: 07 00 62 10  	beq	$3, $2, 0x1a8ebc <.text+0xa8ebc>
  1a8ea0: 2d 38 00 00  	move	$7, $zero
  1a8ea4: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8ea8: 2d 10 e0 00  	move	$2, $7
  1a8eac: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a8eb0: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a8eb4: 08 00 e0 03  	jr	$ra
  1a8eb8: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a8ebc: 64 00 c2 90  	lbu	$2, 0x64($6)
  1a8ec0: 02 00 42 2c  	sltiu	$2, $2, 0x2
  1a8ec4: f8 ff 40 14  	bnez	$2, 0x1a8ea8 <.text+0xa8ea8>
  1a8ec8: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a8ecc: 6a 00 c3 90  	lbu	$3, 0x6a($6)
  1a8ed0: 2a 10 03 02  	slt	$2, $16, $3
  1a8ed4: f4 ff 40 10  	beqz	$2, 0x1a8ea8 <.text+0xa8ea8>
  1a8ed8: 2d 38 00 00  	move	$7, $zero
  1a8edc: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a8ee0: f1 ff 02 12  	beq	$16, $2, 0x1a8ea8 <.text+0xa8ea8>
  1a8ee4: 2d 38 60 00  	move	$7, $3
  1a8ee8: 04 00 22 2a  	slti	$2, $17, 0x4
  1a8eec: ee ff 40 10  	beqz	$2, 0x1a8ea8 <.text+0xa8ea8>
  1a8ef0: 2d 38 00 00  	move	$7, $zero
  1a8ef4: 80 10 10 00  	sll	$2, $16, 0x2
  1a8ef8: 21 10 46 00  	addu	$2, $2, $6
  1a8efc: 21 10 51 00  	addu	$2, $2, $17
  1a8f00: e9 ff 00 10  	b	0x1a8ea8 <.text+0xa8ea8>
  1a8f04: 30 00 47 90  	lbu	$7, 0x30($2)
  1a8f08: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1a8f0c: 44 00 07 3c  	lui	$7, 0x44
  1a8f10: d0 77 e3 24  	addiu	$3, $7, 0x77d0
  1a8f14: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a8f18: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1a8f1c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a8f20: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a8f24: 2d 90 a0 00  	move	$18, $5
  1a8f28: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a8f2c: 2d 88 80 00  	move	$17, $4
  1a8f30: 04 00 64 ac  	sw	$4, 0x4($3)
  1a8f34: d0 77 e2 ac  	sw	$2, 0x77d0($7)
  1a8f38: 0c 00 67 24  	addiu	$7, $3, 0xc
  1a8f3c: 08 00 65 ac  	sw	$5, 0x8($3)
  1a8f40: 2d 28 00 00  	move	$5, $zero
  1a8f44: 21 10 c5 00  	addu	$2, $6, $5
  1a8f48: 21 20 e5 00  	addu	$4, $7, $5
  1a8f4c: 00 00 43 90  	lbu	$3, 0x0($2)
  1a8f50: 01 00 a5 24  	addiu	$5, $5, 0x1
  1a8f54: 06 00 a2 28  	slti	$2, $5, 0x6
  1a8f58: 00 00 00 00  	nop
  1a8f5c: f9 ff 40 14  	bnez	$2, 0x1a8f44 <.text+0xa8f44>
  1a8f60: 00 00 83 a0  	sb	$3, 0x0($4)
  1a8f64: 44 00 02 3c  	lui	$2, 0x44
  1a8f68: 44 00 04 3c  	lui	$4, 0x44
  1a8f6c: d0 77 50 24  	addiu	$16, $2, 0x77d0
  1a8f70: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a8f74: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a8f78: 2d 30 00 00  	move	$6, $zero
  1a8f7c: 2d 38 00 02  	move	$7, $16
  1a8f80: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a8f84: 2d 48 00 02  	move	$9, $16
  1a8f88: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a8f8c: 2d 58 00 00  	move	$11, $zero
  1a8f90: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a8f94: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a8f98: 05 00 40 04  	bltz	$2, 0x1a8fb0 <.text+0xa8fb0>
  1a8f9c: 2d 18 00 00  	move	$3, $zero
  1a8fa0: 14 00 03 8e  	lw	$3, 0x14($16)
  1a8fa4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a8fa8: 08 00 62 10  	beq	$3, $2, 0x1a8fcc <.text+0xa8fcc>
  1a8fac: 2d 20 20 02  	move	$4, $17
  1a8fb0: 40 00 bf df  	ld	$ra, 0x40($sp)
  1a8fb4: 2d 10 60 00  	move	$2, $3
  1a8fb8: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a8fbc: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a8fc0: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a8fc4: 08 00 e0 03  	jr	$ra
  1a8fc8: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1a8fcc: 2d 28 40 02  	move	$5, $18
  1a8fd0: 4e a2 06 0c  	jal	0x1a8938 <.text+0xa8938>
  1a8fd4: 02 00 06 24  	addiu	$6, $zero, 0x2
  1a8fd8: f5 ff 00 10  	b	0x1a8fb0 <.text+0xa8fb0>
  1a8fdc: 14 00 03 8e  	lw	$3, 0x14($16)
  1a8fe0: 44 00 07 3c  	lui	$7, 0x44
  1a8fe4: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a8fe8: d0 77 e3 24  	addiu	$3, $7, 0x77d0
  1a8fec: 07 00 02 24  	addiu	$2, $zero, 0x7
  1a8ff0: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a8ff4: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a8ff8: 04 00 64 ac  	sw	$4, 0x4($3)
  1a8ffc: d0 77 e2 ac  	sw	$2, 0x77d0($7)
  1a9000: 0c 00 67 24  	addiu	$7, $3, 0xc
  1a9004: 08 00 65 ac  	sw	$5, 0x8($3)
  1a9008: 2d 28 00 00  	move	$5, $zero
  1a900c: 21 10 c5 00  	addu	$2, $6, $5
  1a9010: 21 20 e5 00  	addu	$4, $7, $5
  1a9014: 00 00 43 90  	lbu	$3, 0x0($2)
  1a9018: 01 00 a5 24  	addiu	$5, $5, 0x1
  1a901c: 06 00 a2 28  	slti	$2, $5, 0x6
  1a9020: 00 00 00 00  	nop
  1a9024: f9 ff 40 14  	bnez	$2, 0x1a900c <.text+0xa900c>
  1a9028: 00 00 83 a0  	sb	$3, 0x0($4)
  1a902c: 44 00 02 3c  	lui	$2, 0x44
  1a9030: 44 00 04 3c  	lui	$4, 0x44
  1a9034: d0 77 50 24  	addiu	$16, $2, 0x77d0
  1a9038: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a903c: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a9040: 2d 30 00 00  	move	$6, $zero
  1a9044: 2d 38 00 02  	move	$7, $16
  1a9048: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a904c: 2d 48 00 02  	move	$9, $16
  1a9050: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a9054: 2d 58 00 00  	move	$11, $zero
  1a9058: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a905c: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a9060: 02 00 40 04  	bltz	$2, 0x1a906c <.text+0xa906c>
  1a9064: 2d 18 00 00  	move	$3, $zero
  1a9068: 14 00 03 8e  	lw	$3, 0x14($16)
  1a906c: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a9070: 2d 10 60 00  	move	$2, $3
  1a9074: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a9078: 08 00 e0 03  	jr	$ra
  1a907c: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a9080: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a9084: 44 00 03 3c  	lui	$3, 0x44
  1a9088: 2d 60 80 00  	move	$12, $4
  1a908c: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a9090: 44 00 04 3c  	lui	$4, 0x44
  1a9094: d0 77 70 24  	addiu	$16, $3, 0x77d0
  1a9098: 2d 68 a0 00  	move	$13, $5
  1a909c: 11 00 02 24  	addiu	$2, $zero, 0x11
  1a90a0: 80 77 84 24  	addiu	$4, $4, 0x7780
  1a90a4: 01 00 05 24  	addiu	$5, $zero, 0x1
  1a90a8: 2d 30 00 00  	move	$6, $zero
  1a90ac: 2d 38 00 02  	move	$7, $16
  1a90b0: 80 00 08 24  	addiu	$8, $zero, 0x80
  1a90b4: 2d 48 00 02  	move	$9, $16
  1a90b8: 80 00 0a 24  	addiu	$10, $zero, 0x80
  1a90bc: 2d 58 00 00  	move	$11, $zero
  1a90c0: d0 77 62 ac  	sw	$2, 0x77d0($3)
  1a90c4: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a90c8: 04 00 0c ae  	sw	$12, 0x4($16)
  1a90cc: 08 00 0d ae  	sw	$13, 0x8($16)
  1a90d0: ec 71 06 0c  	jal	0x19c7b0 <.text+0x9c7b0>
  1a90d4: 00 00 a0 af  	sw	$zero, 0x0($sp)
  1a90d8: 02 00 40 04  	bltz	$2, 0x1a90e4 <.text+0xa90e4>
  1a90dc: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a90e0: 0c 00 03 8e  	lw	$3, 0xc($16)
  1a90e4: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a90e8: 2d 10 60 00  	move	$2, $3
  1a90ec: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a90f0: 08 00 e0 03  	jr	$ra
  1a90f4: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a90f8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a90fc: 03 00 80 10  	beqz	$4, 0x1a910c <.text+0xa910c>
  1a9100: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a9104: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1a9108: 00 00 00 00  	nop
  1a910c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a9110: 08 00 e0 03  	jr	$ra
  1a9114: 10 00 bd 27  	addiu	$sp, $sp, 0x10
