
/mnt/data/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  1a90f8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a90fc: 03 00 80 10  	beqz	$4, 0x1a910c <.text+0xa910c>
  1a9100: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a9104: e1 79 06 0c  	jal	0x19e784 <.text+0x9e784>
  1a9108: 00 00 00 00  	nop
  1a910c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a9110: 08 00 e0 03  	jr	$ra
  1a9114: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a9118: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a911c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a9120: 3e a4 06 0c  	jal	0x1a90f8 <.text+0xa90f8>
  1a9124: 00 00 00 00  	nop
  1a9128: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a912c: 08 00 e0 03  	jr	$ra
  1a9130: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a9134: 00 00 00 00  	nop
  1a9138: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a913c: ff 00 84 30  	andi	$4, $4, 0xff
  1a9140: ff 00 02 24  	addiu	$2, $zero, 0xff
  1a9144: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a9148: 0c 00 82 10  	beq	$4, $2, 0x1a917c <.text+0xa917c>
  1a914c: 2d 18 00 00  	move	$3, $zero
  1a9150: 07 00 84 30  	andi	$4, $4, 0x7
  1a9154: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a9158: 08 00 82 10  	beq	$4, $2, 0x1a917c <.text+0xa917c>
  1a915c: 02 00 03 24  	addiu	$3, $zero, 0x2
  1a9160: 03 00 82 28  	slti	$2, $4, 0x3
  1a9164: 0a 00 40 10  	beqz	$2, 0x1a9190 <.text+0xa9190>
  1a9168: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a916c: 03 00 80 10  	beqz	$4, 0x1a917c <.text+0xa917c>
  1a9170: 04 00 03 24  	addiu	$3, $zero, 0x4
  1a9174: 5e 1d 04 0c  	jal	0x107578 <.text+0x7578>
  1a9178: 00 00 00 00  	nop
  1a917c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a9180: 2d 10 60 00  	move	$2, $3
  1a9184: 08 00 e0 03  	jr	$ra
  1a9188: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a918c: 00 00 00 00  	nop
  1a9190: fa ff 82 10  	beq	$4, $2, 0x1a917c <.text+0xa917c>
  1a9194: 04 00 03 24  	addiu	$3, $zero, 0x4
  1a9198: 04 00 02 24  	addiu	$2, $zero, 0x4
  1a919c: f5 ff 82 14  	bne	$4, $2, 0x1a9174 <.text+0xa9174>
  1a91a0: 08 00 03 24  	addiu	$3, $zero, 0x8
  1a91a4: f6 ff 00 10  	b	0x1a9180 <.text+0xa9180>
  1a91a8: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a91ac: 00 00 00 00  	nop
  1a91b0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a91b4: ff 00 84 30  	andi	$4, $4, 0xff
  1a91b8: ff 00 02 24  	addiu	$2, $zero, 0xff
  1a91bc: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a91c0: 0a 00 82 10  	beq	$4, $2, 0x1a91ec <.text+0xa91ec>
  1a91c4: 2d 18 00 00  	move	$3, $zero
  1a91c8: 70 00 84 30  	andi	$4, $4, 0x70
  1a91cc: 20 00 02 24  	addiu	$2, $zero, 0x20
  1a91d0: 21 00 82 10  	beq	$4, $2, 0x1a9258 <.text+0xa9258>
  1a91d4: 21 00 82 28  	slti	$2, $4, 0x21
  1a91d8: 0f 00 40 10  	beqz	$2, 0x1a9218 <.text+0xa9218>
  1a91dc: 40 00 02 24  	addiu	$2, $zero, 0x40
  1a91e0: 07 00 80 14  	bnez	$4, 0x1a9200 <.text+0xa9200>
  1a91e4: 10 00 02 24  	addiu	$2, $zero, 0x10
  1a91e8: 2d 18 00 00  	move	$3, $zero
  1a91ec: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a91f0: 2d 10 60 00  	move	$2, $3
  1a91f4: 08 00 e0 03  	jr	$ra
  1a91f8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a91fc: 00 00 00 00  	nop
  1a9200: fa ff 82 50  	beql	$4, $2, 0x1a91ec <.text+0xa91ec>
  1a9204: 2d 18 00 00  	move	$3, $zero
  1a9208: 5e 1d 04 0c  	jal	0x107578 <.text+0x7578>
  1a920c: 00 00 00 00  	nop
  1a9210: f7 ff 00 10  	b	0x1a91f0 <.text+0xa91f0>
  1a9214: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a9218: 0a 00 82 10  	beq	$4, $2, 0x1a9244 <.text+0xa9244>
  1a921c: 41 00 82 28  	slti	$2, $4, 0x41
  1a9220: f7 ff 40 10  	beqz	$2, 0x1a9200 <.text+0xa9200>
  1a9224: 50 00 02 24  	addiu	$2, $zero, 0x50
  1a9228: 30 00 02 24  	addiu	$2, $zero, 0x30
  1a922c: f6 ff 82 14  	bne	$4, $2, 0x1a9208 <.text+0xa9208>
  1a9230: 00 00 00 00  	nop
  1a9234: 3e 90 06 0c  	jal	0x1a40f8 <.text+0xa40f8>
  1a9238: 2d 20 a0 00  	move	$4, $5
  1a923c: eb ff 00 10  	b	0x1a91ec <.text+0xa91ec>
  1a9240: 2d 18 40 00  	move	$3, $2
  1a9244: 3c 90 06 0c  	jal	0x1a40f0 <.text+0xa40f0>
  1a9248: 2d 20 a0 00  	move	$4, $5
  1a924c: e7 ff 00 10  	b	0x1a91ec <.text+0xa91ec>
  1a9250: 2d 18 40 00  	move	$3, $2
  1a9254: 00 00 00 00  	nop
  1a9258: 40 90 06 0c  	jal	0x1a4100 <.text+0xa4100>
  1a925c: 2d 20 a0 00  	move	$4, $5
  1a9260: e2 ff 00 10  	b	0x1a91ec <.text+0xa91ec>
  1a9264: 2d 18 40 00  	move	$3, $2
  1a9268: 2d 30 00 00  	move	$6, $zero
  1a926c: 2d 38 00 00  	move	$7, $zero
  1a9270: 80 ff 08 24  	addiu	$8, $zero, -0x80 <.text+0xffffffffffefff80>
  1a9274: 00 00 00 00  	nop
  1a9278: 00 00 83 90  	lbu	$3, 0x0($4)
  1a927c: 01 00 84 24  	addiu	$4, $4, 0x1
  1a9280: 7f 00 62 30  	andi	$2, $3, 0x7f
  1a9284: 24 18 68 00  	and	$3, $3, $8
  1a9288: 04 10 c2 00  	sllv	$2, $2, $6
  1a928c: 07 00 c6 24  	addiu	$6, $6, 0x7
  1a9290: f9 ff 60 14  	bnez	$3, 0x1a9278 <.text+0xa9278>
  1a9294: 25 38 47 00  	or	$7, $2, $7
  1a9298: 00 00 a7 fc  	sd	$7, 0x0($5)
  1a929c: 08 00 e0 03  	jr	$ra
  1a92a0: 2d 10 80 00  	move	$2, $4
  1a92a4: 00 00 00 00  	nop
  1a92a8: 2d 38 00 00  	move	$7, $zero
  1a92ac: 2d 40 00 00  	move	$8, $zero
  1a92b0: 80 ff 09 24  	addiu	$9, $zero, -0x80 <.text+0xffffffffffefff80>
  1a92b4: 00 00 00 00  	nop
  1a92b8: 00 00 86 90  	lbu	$6, 0x0($4)
  1a92bc: 01 00 84 24  	addiu	$4, $4, 0x1
  1a92c0: 7f 00 c2 30  	andi	$2, $6, 0x7f
  1a92c4: 24 18 c9 00  	and	$3, $6, $9
  1a92c8: 04 10 e2 00  	sllv	$2, $2, $7
  1a92cc: 07 00 e7 24  	addiu	$7, $7, 0x7
  1a92d0: f9 ff 60 14  	bnez	$3, 0x1a92b8 <.text+0xa92b8>
  1a92d4: 25 40 48 00  	or	$8, $2, $8
  1a92d8: 40 00 e2 2c  	sltiu	$2, $7, 0x40
  1a92dc: 08 00 40 50  	beqzl	$2, 0x1a9300 <.text+0xa9300>
  1a92e0: 00 00 a8 fc  	sd	$8, 0x0($5)
  1a92e4: 40 00 c2 30  	andi	$2, $6, 0x40
  1a92e8: 05 00 40 50  	beqzl	$2, 0x1a9300 <.text+0xa9300>
  1a92ec: 00 00 a8 fc  	sd	$8, 0x0($5)
  1a92f0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a92f4: 14 10 e2 00  	dsllv	$2, $2, $7
  1a92f8: 25 40 02 01  	or	$8, $8, $2
  1a92fc: 00 00 a8 fc  	sd	$8, 0x0($5)
  1a9300: 08 00 e0 03  	jr	$ra
  1a9304: 2d 10 80 00  	move	$2, $4
  1a9308: 80 ff bd 27  	addiu	$sp, $sp, -0x80 <.text+0xffffffffffefff80>
  1a930c: 50 00 02 24  	addiu	$2, $zero, 0x50
  1a9310: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a9314: ff 00 92 30  	andi	$18, $4, 0xff
  1a9318: 60 00 b5 ff  	sd	$21, 0x60($sp)
  1a931c: 2d a8 a0 00  	move	$21, $5
  1a9320: 50 00 b4 ff  	sd	$20, 0x50($sp)
  1a9324: 2d a0 c0 00  	move	$20, $6
  1a9328: 40 00 b3 ff  	sd	$19, 0x40($sp)
  1a932c: 2d 98 e0 00  	move	$19, $7
  1a9330: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a9334: 2d 88 c0 00  	move	$17, $6
  1a9338: 70 00 bf ff  	sd	$ra, 0x70($sp)
  1a933c: 4c 00 42 12  	beq	$18, $2, 0x1a9470 <.text+0xa9470>
  1a9340: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a9344: 0f 00 43 32  	andi	$3, $18, 0xf
  1a9348: 0d 00 62 2c  	sltiu	$2, $3, 0xd
  1a934c: 2a 00 40 10  	beqz	$2, 0x1a93f8 <.text+0xa93f8>
  1a9350: 80 10 03 00  	sll	$2, $3, 0x2
  1a9354: 1c 00 03 3c  	lui	$3, 0x1c
  1a9358: 78 ad 63 24  	addiu	$3, $3, -0x5288 <.text+0xffffffffffefad78>
  1a935c: 21 10 43 00  	addu	$2, $2, $3
  1a9360: 00 00 44 8c  	lw	$4, 0x0($2)
  1a9364: 08 00 80 00  	jr	$4
  1a9368: 00 00 00 00  	nop
  1a936c: 2d 20 c0 00  	move	$4, $6
  1a9370: 9a a4 06 0c  	jal	0x1a9268 <.text+0xa9268>
  1a9374: 2d 28 a0 03  	move	$5, $sp
  1a9378: 2d 88 40 00  	move	$17, $2
  1a937c: 00 00 a2 df  	ld	$2, 0x0($sp)
  1a9380: 3c 80 02 00  	dsll32	$16, $2, 0x0
  1a9384: 3f 80 10 00  	dsra32	$16, $16, 0x0
  1a9388: 09 00 00 12  	beqz	$16, 0x1a93b0 <.text+0xa93b0>
  1a938c: 70 00 42 32  	andi	$2, $18, 0x70
  1a9390: 21 20 14 02  	addu	$4, $16, $20
  1a9394: 21 28 15 02  	addu	$5, $16, $21
  1a9398: 10 00 42 38  	xori	$2, $2, 0x10
  1a939c: 2d 80 80 00  	move	$16, $4
  1a93a0: 80 00 43 32  	andi	$3, $18, 0x80
  1a93a4: 02 00 60 10  	beqz	$3, 0x1a93b0 <.text+0xa93b0>
  1a93a8: 0b 80 a2 00  	movn	$16, $5, $2
  1a93ac: 00 00 10 8e  	lw	$16, 0x0($16)
  1a93b0: 00 00 70 ae  	sw	$16, 0x0($19)
  1a93b4: 2d 10 20 02  	move	$2, $17
  1a93b8: 70 00 bf df  	ld	$ra, 0x70($sp)
  1a93bc: 60 00 b5 df  	ld	$21, 0x60($sp)
  1a93c0: 50 00 b4 df  	ld	$20, 0x50($sp)
  1a93c4: 40 00 b3 df  	ld	$19, 0x40($sp)
  1a93c8: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a93cc: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a93d0: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a93d4: 08 00 e0 03  	jr	$ra
  1a93d8: 80 00 bd 27  	addiu	$sp, $sp, 0x80
  1a93dc: 01 00 c2 90  	lbu	$2, 0x1($6)
  1a93e0: 02 00 d1 24  	addiu	$17, $6, 0x2
  1a93e4: 00 00 c3 90  	lbu	$3, 0x0($6)
  1a93e8: 38 12 02 00  	dsll	$2, $2, 0x8
  1a93ec: 25 10 43 00  	or	$2, $2, $3
  1a93f0: e5 ff 00 10  	b	0x1a9388 <.text+0xa9388>
  1a93f4: ff ff 50 30  	andi	$16, $2, 0xffff
  1a93f8: 5e 1d 04 0c  	jal	0x107578 <.text+0x7578>
  1a93fc: 00 00 00 00  	nop
  1a9400: e1 ff 00 10  	b	0x1a9388 <.text+0xa9388>
  1a9404: 00 00 00 00  	nop
  1a9408: 2d 20 c0 00  	move	$4, $6
  1a940c: aa a4 06 0c  	jal	0x1a92a8 <.text+0xa92a8>
  1a9410: 08 00 a5 27  	addiu	$5, $sp, 0x8
  1a9414: 2d 88 40 00  	move	$17, $2
  1a9418: d9 ff 00 10  	b	0x1a9380 <.text+0xa9380>
  1a941c: 08 00 a2 df  	ld	$2, 0x8($sp)
  1a9420: 01 00 c2 90  	lbu	$2, 0x1($6)
  1a9424: 02 00 d1 24  	addiu	$17, $6, 0x2
  1a9428: 00 00 c3 90  	lbu	$3, 0x0($6)
  1a942c: 38 12 02 00  	dsll	$2, $2, 0x8
  1a9430: 25 10 43 00  	or	$2, $2, $3
  1a9434: 3c 14 02 00  	dsll32	$2, $2, 0x10
  1a9438: 3f 14 02 00  	dsra32	$2, $2, 0x10
  1a943c: ff ff 42 30  	andi	$2, $2, 0xffff
  1a9440: 00 14 02 00  	sll	$2, $2, 0x10
  1a9444: d0 ff 00 10  	b	0x1a9388 <.text+0xa9388>
  1a9448: 03 84 02 00  	sra	$16, $2, 0x10
  1a944c: 00 00 00 00  	nop
  1a9450: 03 00 d0 88  	lwl	$16, 0x3($6)
  1a9454: 00 00 d0 98  	lwr	$16, 0x0($6)
  1a9458: cb ff 00 10  	b	0x1a9388 <.text+0xa9388>
  1a945c: 04 00 d1 24  	addiu	$17, $6, 0x4
  1a9460: 07 00 c2 68  	ldl	$2, 0x7($6)
  1a9464: 00 00 c2 6c  	ldr	$2, 0x0($6)
  1a9468: c5 ff 00 10  	b	0x1a9380 <.text+0xa9380>
  1a946c: 08 00 d1 24  	addiu	$17, $6, 0x8
  1a9470: 03 00 c2 24  	addiu	$2, $6, 0x3
  1a9474: fc ff 03 24  	addiu	$3, $zero, -0x4 <.text+0xffffffffffeffffc>
  1a9478: 24 10 43 00  	and	$2, $2, $3
  1a947c: 00 00 50 8c  	lw	$16, 0x0($2)
  1a9480: cb ff 00 10  	b	0x1a93b0 <.text+0xa93b0>
  1a9484: 04 00 51 24  	addiu	$17, $2, 0x4
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
  1a9728: 00 ff bd 27  	addiu	$sp, $sp, -0x100 <.text+0xffffffffffefff00>
  1a972c: 20 00 e2 24  	addiu	$2, $7, 0x20
  1a9730: 48 00 a2 af  	sw	$2, 0x48($sp)
  1a9734: 03 00 03 24  	addiu	$3, $zero, 0x3
  1a9738: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a973c: c0 00 b6 ff  	sd	$22, 0xc0($sp)
  1a9740: b0 00 b5 ff  	sd	$21, 0xb0($sp)
  1a9744: 2d b0 c0 00  	move	$22, $6
  1a9748: a0 00 b4 ff  	sd	$20, 0xa0($sp)
  1a974c: 2d a8 a0 00  	move	$21, $5
  1a9750: 80 00 b2 ff  	sd	$18, 0x80($sp)
  1a9754: d0 ff f4 24  	addiu	$20, $7, -0x30 <.text+0xffffffffffefffd0>
  1a9758: f0 00 bf ff  	sd	$ra, 0xf0($sp)
  1a975c: 2d 90 00 01  	move	$18, $8
  1a9760: e0 00 be ff  	sd	$fp, 0xe0($sp)
  1a9764: d0 00 b7 ff  	sd	$23, 0xd0($sp)
  1a9768: 90 00 b3 ff  	sd	$19, 0x90($sp)
  1a976c: 70 00 b1 ff  	sd	$17, 0x70($sp)
  1a9770: 0f 00 82 10  	beq	$4, $2, 0x1a97b0 <.text+0xa97b0>
  1a9774: 60 00 b0 ff  	sd	$16, 0x60($sp)
  1a9778: f0 00 bf df  	ld	$ra, 0xf0($sp)
  1a977c: 2d 10 60 00  	move	$2, $3
  1a9780: e0 00 be df  	ld	$fp, 0xe0($sp)
  1a9784: d0 00 b7 df  	ld	$23, 0xd0($sp)
  1a9788: c0 00 b6 df  	ld	$22, 0xc0($sp)
  1a978c: b0 00 b5 df  	ld	$21, 0xb0($sp)
  1a9790: a0 00 b4 df  	ld	$20, 0xa0($sp)
  1a9794: 90 00 b3 df  	ld	$19, 0x90($sp)
  1a9798: 80 00 b2 df  	ld	$18, 0x80($sp)
  1a979c: 70 00 b1 df  	ld	$17, 0x70($sp)
  1a97a0: 60 00 b0 df  	ld	$16, 0x60($sp)
  1a97a4: 08 00 e0 03  	jr	$ra
  1a97a8: 00 01 bd 27  	addiu	$sp, $sp, 0x100
  1a97ac: 00 00 00 00  	nop
  1a97b0: 06 00 02 24  	addiu	$2, $zero, 0x6
  1a97b4: e0 00 a2 10  	beq	$5, $2, 0x1a9b38 <.text+0xa9b38>
  1a97b8: 00 00 00 00  	nop
  1a97bc: 3a 90 06 0c  	jal	0x1a40e8 <.text+0xa40e8>
  1a97c0: 2d 20 40 02  	move	$4, $18
  1a97c4: 08 00 03 24  	addiu	$3, $zero, 0x8
  1a97c8: eb ff 40 10  	beqz	$2, 0x1a9778 <.text+0xa9778>
  1a97cc: 4c 00 a2 af  	sw	$2, 0x4c($sp)
  1a97d0: 4c 00 a5 8f  	lw	$5, 0x4c($sp)
  1a97d4: 2d 30 a0 03  	move	$6, $sp
  1a97d8: 2d 20 40 02  	move	$4, $18
  1a97dc: 22 a5 06 0c  	jal	0x1a9488 <.text+0xa9488>
  1a97e0: 54 00 a0 af  	sw	$zero, 0x54($sp)
  1a97e4: 58 00 a0 af  	sw	$zero, 0x58($sp)
  1a97e8: 14 00 a4 93  	lbu	$4, 0x14($sp)
  1a97ec: 2d 28 40 02  	move	$5, $18
  1a97f0: 2d 88 40 00  	move	$17, $2
  1a97f4: 6c a4 06 0c  	jal	0x1a91b0 <.text+0xa91b0>
  1a97f8: 50 00 a0 af  	sw	$zero, 0x50($sp)
  1a97fc: 2d 20 40 02  	move	$4, $18
  1a9800: 2c 97 06 0c  	jal	0x1a5cb0 <.text+0xa5cb0>
  1a9804: 08 00 a2 af  	sw	$2, 0x8($sp)
  1a9808: ff ff 53 24  	addiu	$19, $2, -0x1 <.text+0xffffffffffefffff>
  1a980c: 10 00 a2 8f  	lw	$2, 0x10($sp)
  1a9810: 2b 10 22 02  	sltu	$2, $17, $2
  1a9814: 30 00 40 10  	beqz	$2, 0x1a98d8 <.text+0xa98d8>
  1a9818: c3 10 15 00  	sra	$2, $21, 0x3
  1a981c: 24 00 be 27  	addiu	$fp, $sp, 0x24
  1a9820: 28 00 b7 27  	addiu	$23, $sp, 0x28
  1a9824: 15 00 b0 93  	lbu	$16, 0x15($sp)
  1a9828: 2d 28 00 00  	move	$5, $zero
  1a982c: 6c a4 06 0c  	jal	0x1a91b0 <.text+0xa91b0>
  1a9830: 2d 20 00 02  	move	$4, $16
  1a9834: 20 00 a7 27  	addiu	$7, $sp, 0x20
  1a9838: 2d 30 20 02  	move	$6, $17
  1a983c: 2d 20 00 02  	move	$4, $16
  1a9840: c2 a4 06 0c  	jal	0x1a9308 <.text+0xa9308>
  1a9844: 2d 28 40 00  	move	$5, $2
  1a9848: 15 00 b0 93  	lbu	$16, 0x15($sp)
  1a984c: 2d 28 00 00  	move	$5, $zero
  1a9850: 2d 88 40 00  	move	$17, $2
  1a9854: 6c a4 06 0c  	jal	0x1a91b0 <.text+0xa91b0>
  1a9858: 2d 20 00 02  	move	$4, $16
  1a985c: 2d 38 c0 03  	move	$7, $fp
  1a9860: 2d 30 20 02  	move	$6, $17
  1a9864: 2d 20 00 02  	move	$4, $16
  1a9868: c2 a4 06 0c  	jal	0x1a9308 <.text+0xa9308>
  1a986c: 2d 28 40 00  	move	$5, $2
  1a9870: 15 00 b0 93  	lbu	$16, 0x15($sp)
  1a9874: 2d 28 00 00  	move	$5, $zero
  1a9878: 2d 88 40 00  	move	$17, $2
  1a987c: 6c a4 06 0c  	jal	0x1a91b0 <.text+0xa91b0>
  1a9880: 2d 20 00 02  	move	$4, $16
  1a9884: 2d 38 e0 02  	move	$7, $23
  1a9888: 2d 30 20 02  	move	$6, $17
  1a988c: 2d 20 00 02  	move	$4, $16
  1a9890: c2 a4 06 0c  	jal	0x1a9308 <.text+0xa9308>
  1a9894: 2d 28 40 00  	move	$5, $2
  1a9898: 30 00 a5 27  	addiu	$5, $sp, 0x30
  1a989c: 9a a4 06 0c  	jal	0x1a9268 <.text+0xa9268>
  1a98a0: 2d 20 40 00  	move	$4, $2
  1a98a4: 00 00 a4 8f  	lw	$4, 0x0($sp)
  1a98a8: 2d 88 40 00  	move	$17, $2
  1a98ac: 20 00 a2 8f  	lw	$2, 0x20($sp)
  1a98b0: 21 20 82 00  	addu	$4, $4, $2
  1a98b4: 2b 18 64 02  	sltu	$3, $19, $4
  1a98b8: 41 00 60 10  	beqz	$3, 0x1a99c0 <.text+0xa99c0>
  1a98bc: 24 00 a2 8f  	lw	$2, 0x24($sp)
  1a98c0: 10 00 b1 8f  	lw	$17, 0x10($sp)
  1a98c4: 2d 10 20 02  	move	$2, $17
  1a98c8: 2b 10 22 02  	sltu	$2, $17, $2
  1a98cc: d6 ff 40 14  	bnez	$2, 0x1a9828 <.text+0xa9828>
  1a98d0: 15 00 b0 93  	lbu	$16, 0x15($sp)
  1a98d4: c3 10 15 00  	sra	$2, $21, 0x3
  1a98d8: 01 00 42 38  	xori	$2, $2, 0x1
  1a98dc: 01 00 44 30  	andi	$4, $2, 0x1
  1a98e0: a5 ff 80 10  	beqz	$4, 0x1a9778 <.text+0xa9778>
  1a98e4: 08 00 03 24  	addiu	$3, $zero, 0x8
  1a98e8: 01 00 a2 32  	andi	$2, $21, 0x1
  1a98ec: 19 00 40 10  	beqz	$2, 0x1a9954 <.text+0xa9954>
  1a98f0: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a98f4: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a98f8: a0 ff 82 10  	beq	$4, $2, 0x1a977c <.text+0xa977c>
  1a98fc: f0 00 bf df  	ld	$ra, 0xf0($sp)
  1a9900: 4e 47 02 3c  	lui	$2, 0x474e
  1a9904: 43 55 42 34  	ori	$2, $2, 0x5543
  1a9908: 38 14 02 00  	dsll	$2, $2, 0x10
  1a990c: 2b 43 42 34  	ori	$2, $2, 0x432b
  1a9910: 38 14 02 00  	dsll	$2, $2, 0x10
  1a9914: 00 2b 42 34  	ori	$2, $2, 0x2b00
  1a9918: 03 00 c2 12  	beq	$22, $2, 0x1a9928 <.text+0xa9928>
  1a991c: 54 00 a2 8f  	lw	$2, 0x54($sp)
  1a9920: 95 ff 00 10  	b	0x1a9778 <.text+0xa9778>
  1a9924: 06 00 03 24  	addiu	$3, $zero, 0x6
  1a9928: 24 00 82 ae  	sw	$2, 0x24($20)
  1a992c: 58 00 a3 8f  	lw	$3, 0x58($sp)
  1a9930: 18 00 83 ae  	sw	$3, 0x18($20)
  1a9934: 50 00 a2 8f  	lw	$2, 0x50($sp)
  1a9938: 1c 00 82 ae  	sw	$2, 0x1c($20)
  1a993c: 4c 00 a3 8f  	lw	$3, 0x4c($sp)
  1a9940: 20 00 83 ae  	sw	$3, 0x20($20)
  1a9944: 48 00 a2 8f  	lw	$2, 0x48($sp)
  1a9948: f5 ff 00 10  	b	0x1a9920 <.text+0xa9920>
  1a994c: 28 00 82 ae  	sw	$2, 0x28($20)
  1a9950: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a9954: 88 00 82 10  	beq	$4, $2, 0x1a9b78 <.text+0xa9b78>
  1a9958: 58 00 a2 8f  	lw	$2, 0x58($sp)
  1a995c: 10 00 42 04  	bltzl	$2, 0x1a99a0 <.text+0xa99a0>
  1a9960: 20 00 85 8e  	lw	$5, 0x20($20)
  1a9964: 30 00 86 26  	addiu	$6, $20, 0x30
  1a9968: 2d 20 40 02  	move	$4, $18
  1a996c: 3c 30 06 00  	dsll32	$6, $6, 0x0
  1a9970: 04 00 05 24  	addiu	$5, $zero, 0x4
  1a9974: 26 97 06 0c  	jal	0x1a5c98 <.text+0xa5c98>
  1a9978: 3e 30 06 00  	dsrl32	$6, $6, 0x0
  1a997c: 2d 20 40 02  	move	$4, $18
  1a9980: 58 00 a6 8f  	lw	$6, 0x58($sp)
  1a9984: 26 97 06 0c  	jal	0x1a5c98 <.text+0xa5c98>
  1a9988: 05 00 05 24  	addiu	$5, $zero, 0x5
  1a998c: 54 00 a5 8f  	lw	$5, 0x54($sp)
  1a9990: 2e 97 06 0c  	jal	0x1a5cb8 <.text+0xa5cb8>
  1a9994: 2d 20 40 02  	move	$4, $18
  1a9998: 77 ff 00 10  	b	0x1a9778 <.text+0xa9778>
  1a999c: 07 00 03 24  	addiu	$3, $zero, 0x7
  1a99a0: 2d 20 40 02  	move	$4, $18
  1a99a4: 22 a5 06 0c  	jal	0x1a9488 <.text+0xa9488>
  1a99a8: 2d 30 a0 03  	move	$6, $sp
  1a99ac: 14 00 a4 93  	lbu	$4, 0x14($sp)
  1a99b0: 6c a4 06 0c  	jal	0x1a91b0 <.text+0xa91b0>
  1a99b4: 2d 28 40 02  	move	$5, $18
  1a99b8: ea ff 00 10  	b	0x1a9964 <.text+0xa9964>
  1a99bc: 24 00 82 ae  	sw	$2, 0x24($20)
  1a99c0: 21 10 82 00  	addu	$2, $4, $2
  1a99c4: 2b 10 62 02  	sltu	$2, $19, $2
  1a99c8: bf ff 40 10  	beqz	$2, 0x1a98c8 <.text+0xa98c8>
  1a99cc: 10 00 a2 8f  	lw	$2, 0x10($sp)
  1a99d0: 28 00 a3 8f  	lw	$3, 0x28($sp)
  1a99d4: 05 00 60 10  	beqz	$3, 0x1a99ec <.text+0xa99ec>
  1a99d8: 30 00 a2 df  	ld	$2, 0x30($sp)
  1a99dc: 04 00 a2 8f  	lw	$2, 0x4($sp)
  1a99e0: 21 10 43 00  	addu	$2, $2, $3
  1a99e4: 54 00 a2 af  	sw	$2, 0x54($sp)
  1a99e8: 30 00 a2 df  	ld	$2, 0x30($sp)
  1a99ec: 07 00 40 10  	beqz	$2, 0x1a9a0c <.text+0xa9a0c>
  1a99f0: 54 00 a2 8f  	lw	$2, 0x54($sp)
  1a99f4: 10 00 a2 8f  	lw	$2, 0x10($sp)
  1a99f8: 30 00 a3 8f  	lw	$3, 0x30($sp)
  1a99fc: 21 10 43 00  	addu	$2, $2, $3
  1a9a00: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  1a9a04: 50 00 a2 af  	sw	$2, 0x50($sp)
  1a9a08: 54 00 a2 8f  	lw	$2, 0x54($sp)
  1a9a0c: b4 ff 40 10  	beqz	$2, 0x1a98e0 <.text+0xa98e0>
  1a9a10: 2d 20 00 00  	move	$4, $zero
  1a9a14: 50 00 a3 8f  	lw	$3, 0x50($sp)
  1a9a18: b1 ff 60 10  	beqz	$3, 0x1a98e0 <.text+0xa98e0>
  1a9a1c: 02 00 04 24  	addiu	$4, $zero, 0x2
  1a9a20: 08 00 b7 32  	andi	$23, $21, 0x8
  1a9a24: 2d 98 00 00  	move	$19, $zero
  1a9a28: 09 00 e0 16  	bnez	$23, 0x1a9a50 <.text+0xa9a50>
  1a9a2c: 2d f0 00 00  	move	$fp, $zero
  1a9a30: 4e 47 02 3c  	lui	$2, 0x474e
  1a9a34: 43 55 42 34  	ori	$2, $2, 0x5543
  1a9a38: 38 14 02 00  	dsll	$2, $2, 0x10
  1a9a3c: 2b 43 42 34  	ori	$2, $2, 0x432b
  1a9a40: 38 14 02 00  	dsll	$2, $2, 0x10
  1a9a44: 00 2b 42 34  	ori	$2, $2, 0x2b00
  1a9a48: 03 00 c2 52  	beql	$22, $2, 0x1a9a58 <.text+0xa9a58>
  1a9a4c: 00 00 90 8e  	lw	$16, 0x0($20)
  1a9a50: 2d 80 00 00  	move	$16, $zero
  1a9a54: 00 00 00 00  	nop
  1a9a58: 50 00 a4 8f  	lw	$4, 0x50($sp)
  1a9a5c: aa a4 06 0c  	jal	0x1a92a8 <.text+0xa92a8>
  1a9a60: 38 00 a5 27  	addiu	$5, $sp, 0x38
  1a9a64: 40 00 a5 27  	addiu	$5, $sp, 0x40
  1a9a68: 2d 20 40 00  	move	$4, $2
  1a9a6c: aa a4 06 0c  	jal	0x1a92a8 <.text+0xa92a8>
  1a9a70: 2d 88 40 00  	move	$17, $2
  1a9a74: 38 00 a5 df  	ld	$5, 0x38($sp)
  1a9a78: 11 00 a0 14  	bnez	$5, 0x1a9ac0 <.text+0xa9ac0>
  1a9a7c: 00 00 00 00  	nop
  1a9a80: 01 00 13 24  	addiu	$19, $zero, 0x1
  1a9a84: 40 00 a2 df  	ld	$2, 0x40($sp)
  1a9a88: 04 00 40 10  	beqz	$2, 0x1a9a9c <.text+0xa9a9c>
  1a9a8c: 40 00 a2 8f  	lw	$2, 0x40($sp)
  1a9a90: 21 88 22 02  	addu	$17, $17, $2
  1a9a94: f0 ff 00 10  	b	0x1a9a58 <.text+0xa9a58>
  1a9a98: 50 00 b1 af  	sw	$17, 0x50($sp)
  1a9a9c: 90 ff c0 53  	beqzl	$fp, 0x1a98e0 <.text+0xa98e0>
  1a9aa0: 40 20 13 00  	sll	$4, $19, 0x1
  1a9aa4: 38 00 a2 df  	ld	$2, 0x38($sp)
  1a9aa8: 03 00 04 24  	addiu	$4, $zero, 0x3
  1a9aac: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a9ab0: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a9ab4: 8a ff 00 10  	b	0x1a98e0 <.text+0xa98e0>
  1a9ab8: 58 00 a2 af  	sw	$2, 0x58($sp)
  1a9abc: 00 00 00 00  	nop
  1a9ac0: 13 00 a0 18  	blez	$5, 0x1a9b10 <.text+0xa9b10>
  1a9ac4: 00 00 00 00  	nop
  1a9ac8: 62 a5 06 0c  	jal	0x1a9588 <.text+0xa9588>
  1a9acc: 2d 20 a0 03  	move	$4, $sp
  1a9ad0: 0a 00 40 10  	beqz	$2, 0x1a9afc <.text+0xa9afc>
  1a9ad4: 00 00 00 00  	nop
  1a9ad8: ea ff 00 12  	beqz	$16, 0x1a9a84 <.text+0xa9a84>
  1a9adc: 2d 20 40 00  	move	$4, $2
  1a9ae0: 2d 28 00 02  	move	$5, $16
  1a9ae4: 7e a5 06 0c  	jal	0x1a95f8 <.text+0xa95f8>
  1a9ae8: 48 00 a6 27  	addiu	$6, $sp, 0x48
  1a9aec: e6 ff 40 10  	beqz	$2, 0x1a9a88 <.text+0xa9a88>
  1a9af0: 40 00 a2 df  	ld	$2, 0x40($sp)
  1a9af4: e9 ff 00 10  	b	0x1a9a9c <.text+0xa9a9c>
  1a9af8: 01 00 1e 24  	addiu	$fp, $zero, 0x1
  1a9afc: e2 ff e0 16  	bnez	$23, 0x1a9a88 <.text+0xa9a88>
  1a9b00: 40 00 a2 df  	ld	$2, 0x40($sp)
  1a9b04: e5 ff 00 10  	b	0x1a9a9c <.text+0xa9a9c>
  1a9b08: 01 00 1e 24  	addiu	$fp, $zero, 0x1
  1a9b0c: 00 00 00 00  	nop
  1a9b10: dc ff 00 12  	beqz	$16, 0x1a9a84 <.text+0xa9a84>
  1a9b14: 48 00 a6 8f  	lw	$6, 0x48($sp)
  1a9b18: 2d 38 a0 00  	move	$7, $5
  1a9b1c: 2d 20 a0 03  	move	$4, $sp
  1a9b20: a6 a5 06 0c  	jal	0x1a9698 <.text+0xa9698>
  1a9b24: 2d 28 00 02  	move	$5, $16
  1a9b28: d7 ff 40 14  	bnez	$2, 0x1a9a88 <.text+0xa9a88>
  1a9b2c: 40 00 a2 df  	ld	$2, 0x40($sp)
  1a9b30: da ff 00 10  	b	0x1a9a9c <.text+0xa9a9c>
  1a9b34: 01 00 1e 24  	addiu	$fp, $zero, 0x1
  1a9b38: 4e 47 02 3c  	lui	$2, 0x474e
  1a9b3c: 43 55 42 34  	ori	$2, $2, 0x5543
  1a9b40: 38 14 02 00  	dsll	$2, $2, 0x10
  1a9b44: 2b 43 42 34  	ori	$2, $2, 0x432b
  1a9b48: 38 14 02 00  	dsll	$2, $2, 0x10
  1a9b4c: 00 2b 42 34  	ori	$2, $2, 0x2b00
  1a9b50: 1a ff c2 14  	bne	$6, $2, 0x1a97bc <.text+0xa97bc>
  1a9b54: 00 00 00 00  	nop
  1a9b58: 24 00 82 8e  	lw	$2, 0x24($20)
  1a9b5c: 54 00 a2 af  	sw	$2, 0x54($sp)
  1a9b60: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a9b64: 18 00 83 8e  	lw	$3, 0x18($20)
  1a9b68: 58 00 a3 af  	sw	$3, 0x58($sp)
  1a9b6c: 54 00 a3 8f  	lw	$3, 0x54($sp)
  1a9b70: 77 ff 00 10  	b	0x1a9950 <.text+0xa9950>
  1a9b74: 0b 20 43 00  	movn	$4, $2, $3
  1a9b78: 3c ac 06 0c  	jal	0x1ab0f0 <.text+0xab0f0>
  1a9b7c: 30 00 84 26  	addiu	$4, $20, 0x30
  1a9b80: 2a a7 06 0c  	jal	0x1a9ca8 <.text+0xa9ca8>
  1a9b84: 0c 00 84 8e  	lw	$4, 0xc($20)
  1a9b88: 60 ff bd 27  	addiu	$sp, $sp, -0xa0 <.text+0xffffffffffefff60>
  1a9b8c: 40 00 b0 ff  	sd	$16, 0x40($sp)
  1a9b90: 2d 80 80 00  	move	$16, $4
  1a9b94: d0 ff 10 26  	addiu	$16, $16, -0x30 <.text+0xffffffffffefffd0>
  1a9b98: 80 00 b4 ff  	sd	$20, 0x80($sp)
  1a9b9c: 70 00 b3 ff  	sd	$19, 0x70($sp)
  1a9ba0: 50 00 b1 ff  	sd	$17, 0x50($sp)
  1a9ba4: 90 00 bf ff  	sd	$ra, 0x90($sp)
  1a9ba8: 3c ac 06 0c  	jal	0x1ab0f0 <.text+0xab0f0>
  1a9bac: 60 00 b2 ff  	sd	$18, 0x60($sp)
  1a9bb0: 08 00 04 8e  	lw	$4, 0x8($16)
  1a9bb4: 24 00 02 8e  	lw	$2, 0x24($16)
  1a9bb8: 20 00 11 8e  	lw	$17, 0x20($16)
  1a9bbc: 18 00 13 8e  	lw	$19, 0x18($16)
  1a9bc0: 0c 00 14 8e  	lw	$20, 0xc($16)
  1a9bc4: 42 a7 06 0c  	jal	0x1a9d08 <.text+0xa9d08>
  1a9bc8: 18 00 a2 af  	sw	$2, 0x18($sp)
  1a9bcc: 00 00 00 00  	nop
  1a9bd0: 3c ac 06 0c  	jal	0x1ab0f0 <.text+0xab0f0>
  1a9bd4: 00 00 00 00  	nop
  1a9bd8: c2 ac 06 0c  	jal	0x1ab308 <.text+0xab308>
  1a9bdc: 10 00 b2 27  	addiu	$18, $sp, 0x10
  1a9be0: 2d 20 00 00  	move	$4, $zero
  1a9be4: 00 00 50 8c  	lw	$16, 0x0($2)
  1a9be8: 2d 28 20 02  	move	$5, $17
  1a9bec: 2d 30 40 02  	move	$6, $18
  1a9bf0: 22 a5 06 0c  	jal	0x1a9488 <.text+0xa9488>
  1a9bf4: 50 00 11 26  	addiu	$17, $16, 0x50
  1a9bf8: 00 00 05 8e  	lw	$5, 0x0($16)
  1a9bfc: 2d 30 20 02  	move	$6, $17
  1a9c00: 2d 20 40 02  	move	$4, $18
  1a9c04: a6 a5 06 0c  	jal	0x1a9698 <.text+0xa9698>
  1a9c08: 2d 38 60 02  	move	$7, $19
  1a9c0c: 04 00 40 50  	beqzl	$2, 0x1a9c20 <.text+0xa9c20>
  1a9c10: 42 00 02 3c  	lui	$2, 0x42
  1a9c14: 90 a7 06 0c  	jal	0x1a9e40 <.text+0xa9e40>
		...
  1a9c20: 2d 20 40 02  	move	$4, $18
  1a9c24: b0 6d 51 24  	addiu	$17, $2, 0x6db0
  1a9c28: 2d 38 60 02  	move	$7, $19
  1a9c2c: 2d 28 20 02  	move	$5, $17
  1a9c30: a6 a5 06 0c  	jal	0x1a9698 <.text+0xa9698>
  1a9c34: 2d 30 00 00  	move	$6, $zero
  1a9c38: 0b 00 40 10  	beqz	$2, 0x1a9c68 <.text+0xa9c68>
  1a9c3c: 42 00 10 3c  	lui	$16, 0x42
  1a9c40: 04 00 04 24  	addiu	$4, $zero, 0x4
  1a9c44: 80 6d 10 26  	addiu	$16, $16, 0x6d80
  1a9c48: ee ab 06 0c  	jal	0x1aafb8 <.text+0xaafb8>
  1a9c4c: 30 00 b0 af  	sw	$16, 0x30($sp)
  1a9c50: 00 00 50 ac  	sw	$16, 0x0($2)
  1a9c54: 1b 00 06 3c  	lui	$6, 0x1b
  1a9c58: 2d 20 40 00  	move	$4, $2
  1a9c5c: 2d 28 20 02  	move	$5, $17
  1a9c60: 6e a7 06 0c  	jal	0x1a9db8 <.text+0xa9db8>
  1a9c64: 98 b2 c6 24  	addiu	$6, $6, -0x4d68 <.text+0xffffffffffefb298>
  1a9c68: 2a a7 06 0c  	jal	0x1a9ca8 <.text+0xa9ca8>
  1a9c6c: 2d 20 80 02  	move	$4, $20
  1a9c70: 2d 80 80 00  	move	$16, $4
  1a9c74: a6 ac 06 0c  	jal	0x1ab298 <.text+0xab298>
  1a9c78: 30 00 a4 27  	addiu	$4, $sp, 0x30
  1a9c7c: 03 00 00 10  	b	0x1a9c8c <.text+0xa9c8c>
		...
  1a9c88: 2d 80 80 00  	move	$16, $4
  1a9c8c: 56 ac 06 0c  	jal	0x1ab158 <.text+0xab158>
  1a9c90: 00 00 00 00  	nop
  1a9c94: 56 ac 06 0c  	jal	0x1ab158 <.text+0xab158>
  1a9c98: 00 00 00 00  	nop
  1a9c9c: b0 96 06 0c  	jal	0x1a5ac0 <.text+0xa5ac0>
  1a9ca0: 2d 20 00 02  	move	$4, $16
  1a9ca4: 00 00 00 00  	nop
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
		...
