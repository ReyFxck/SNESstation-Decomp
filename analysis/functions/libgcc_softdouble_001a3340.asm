
/mnt/data/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  1a3340: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a3344: 10 00 a4 27  	addiu	$4, $sp, 0x10
  1a3348: 2d 28 a0 03  	move	$5, $sp
  1a334c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a3350: 8c 9f 06 0c  	jal	0x1a7e30 <.text+0xa7e30>
  1a3354: 10 00 ac e7  	swc1	$f12, 0x10($sp)
  1a3358: 0c 00 a7 9f  	lwu	$7, 0xc($sp)
  1a335c: 08 00 a6 8f  	lw	$6, 0x8($sp)
  1a3360: 00 00 a4 8f  	lw	$4, 0x0($sp)
  1a3364: b8 3f 07 00  	dsll	$7, $7, 0x1e
  1a3368: 24 8f 06 0c  	jal	0x1a3c90 <.text+0xa3c90>
  1a336c: 04 00 a5 8f  	lw	$5, 0x4($sp)
  1a3370: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a3374: 08 00 e0 03  	jr	$ra
  1a3378: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a337c: 00 00 00 00  	nop
  1a3380: 00 00 83 8c  	lw	$3, 0x0($4)
  1a3384: 2d 40 80 00  	move	$8, $4
  1a3388: 02 00 62 2c  	sltiu	$2, $3, 0x2
  1a338c: 10 00 40 14  	bnez	$2, 0x1a33d0 <.text+0xa33d0>
  1a3390: 2d 38 80 00  	move	$7, $4
  1a3394: 00 00 a4 8c  	lw	$4, 0x0($5)
  1a3398: 02 00 82 2c  	sltiu	$2, $4, 0x2
  1a339c: 0c 00 40 14  	bnez	$2, 0x1a33d0 <.text+0xa33d0>
  1a33a0: 2d 38 a0 00  	move	$7, $5
  1a33a4: 04 00 62 38  	xori	$2, $3, 0x4
  1a33a8: 0b 00 40 14  	bnez	$2, 0x1a33d8 <.text+0xa33d8>
  1a33ac: 04 00 82 38  	xori	$2, $4, 0x4
  1a33b0: 07 00 40 14  	bnez	$2, 0x1a33d0 <.text+0xa33d0>
  1a33b4: 2d 38 00 01  	move	$7, $8
  1a33b8: 04 00 a3 8c  	lw	$3, 0x4($5)
  1a33bc: 04 00 02 8d  	lw	$2, 0x4($8)
  1a33c0: 03 00 43 10  	beq	$2, $3, 0x1a33d0 <.text+0xa33d0>
  1a33c4: 00 00 00 00  	nop
  1a33c8: 1c 00 02 3c  	lui	$2, 0x1c
  1a33cc: e0 a7 47 24  	addiu	$7, $2, -0x5820 <.text+0xffffffffffefa7e0>
  1a33d0: 08 00 e0 03  	jr	$ra
  1a33d4: 2d 10 e0 00  	move	$2, $7
  1a33d8: fd ff 40 10  	beqz	$2, 0x1a33d0 <.text+0xa33d0>
  1a33dc: 2d 38 a0 00  	move	$7, $5
  1a33e0: 02 00 82 38  	xori	$2, $4, 0x2
  1a33e4: 0d 00 40 14  	bnez	$2, 0x1a341c <.text+0xa341c>
  1a33e8: 02 00 62 38  	xori	$2, $3, 0x2
  1a33ec: f8 ff 40 54  	bnezl	$2, 0x1a33d0 <.text+0xa33d0>
  1a33f0: 2d 38 00 01  	move	$7, $8
  1a33f4: 00 00 02 79  	<unknown>
  1a33f8: 2d 38 c0 00  	move	$7, $6
  1a33fc: 00 00 c2 7c  	ext	$2, $6, 0x0, 0x1
  1a3400: 10 00 03 dd  	ld	$3, 0x10($8)
  1a3404: 10 00 c3 fc  	sd	$3, 0x10($6)
  1a3408: 04 00 02 8d  	lw	$2, 0x4($8)
  1a340c: 04 00 a3 8c  	lw	$3, 0x4($5)
  1a3410: 24 10 43 00  	and	$2, $2, $3
  1a3414: ee ff 00 10  	b	0x1a33d0 <.text+0xa33d0>
  1a3418: 04 00 c2 ac  	sw	$2, 0x4($6)
  1a341c: ec ff 40 10  	beqz	$2, 0x1a33d0 <.text+0xa33d0>
  1a3420: 2d 38 a0 00  	move	$7, $5
  1a3424: 08 00 0b 8d  	lw	$11, 0x8($8)
  1a3428: 08 00 a7 8c  	lw	$7, 0x8($5)
  1a342c: 10 00 0a dd  	ld	$10, 0x10($8)
  1a3430: 23 10 67 01  	subu	$2, $11, $7
  1a3434: 01 00 42 04  	bltzl	$2, 0x1a343c <.text+0xa343c>
  1a3438: 23 10 02 00  	negu	$2, $2
  1a343c: 40 00 42 28  	slti	$2, $2, 0x40
  1a3440: 53 00 40 10  	beqz	$2, 0x1a3590 <.text+0xa3590>
  1a3444: 10 00 a9 dc  	ld	$9, 0x10($5)
  1a3448: 2a 10 eb 00  	slt	$2, $7, $11
  1a344c: 0c 00 40 10  	beqz	$2, 0x1a3480 <.text+0xa3480>
  1a3450: 2a 10 67 01  	slt	$2, $11, $7
  1a3454: 23 38 67 01  	subu	$7, $11, $7
  1a3458: 7a 18 09 00  	dsrl	$3, $9, 0x1
  1a345c: 01 00 22 31  	andi	$2, $9, 0x1
  1a3460: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
		...
  1a3470: f9 ff e0 14  	bnez	$7, 0x1a3458 <.text+0xa3458>
  1a3474: 25 48 43 00  	or	$9, $2, $3
  1a3478: 2d 38 60 01  	move	$7, $11
  1a347c: 2a 10 67 01  	slt	$2, $11, $7
  1a3480: 0a 00 40 50  	beqzl	$2, 0x1a34ac <.text+0xa34ac>
  1a3484: 04 00 04 8d  	lw	$4, 0x4($8)
  1a3488: 01 00 6b 25  	addiu	$11, $11, 0x1
  1a348c: 7a 10 0a 00  	dsrl	$2, $10, 0x1
  1a3490: 01 00 43 31  	andi	$3, $10, 0x1
  1a3494: 2a 20 67 01  	slt	$4, $11, $7
		...
  1a34a0: f9 ff 80 14  	bnez	$4, 0x1a3488 <.text+0xa3488>
  1a34a4: 25 50 62 00  	or	$10, $3, $2
  1a34a8: 04 00 04 8d  	lw	$4, 0x4($8)
  1a34ac: 04 00 a2 8c  	lw	$2, 0x4($5)
  1a34b0: 31 00 82 10  	beq	$4, $2, 0x1a3578 <.text+0xa3578>
  1a34b4: 2f 10 49 01  	dsubu	$2, $10, $9
  1a34b8: 2f 18 2a 01  	dsubu	$3, $9, $10
  1a34bc: 0a 18 44 00  	movz	$3, $2, $4
  1a34c0: 28 00 62 04  	bltzl	$3, 0x1a3564 <.text+0xa3564>
  1a34c4: 2f 18 03 00  	dnegu	$3, $3
  1a34c8: 08 00 cb ac  	sw	$11, 0x8($6)
  1a34cc: 10 00 c3 fc  	sd	$3, 0x10($6)
  1a34d0: 04 00 c0 ac  	sw	$zero, 0x4($6)
  1a34d4: 10 00 c5 dc  	ld	$5, 0x10($6)
  1a34d8: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a34dc: 78 11 02 00  	dsll	$2, $2, 0x5
  1a34e0: 3a 11 02 00  	dsrl	$2, $2, 0x4
  1a34e4: ff ff 07 24  	addiu	$7, $zero, -0x1 <.text+0xffffffffffefffff>
  1a34e8: 78 39 07 00  	dsll	$7, $7, 0x5
  1a34ec: 3a 39 07 00  	dsrl	$7, $7, 0x4
  1a34f0: ff ff a3 64  	daddiu	$3, $5, -0x1 <.text+0xffffffffffefffff>
  1a34f4: 2b 10 43 00  	sltu	$2, $2, $3
  1a34f8: 0b 00 40 14  	bnez	$2, 0x1a3528 <.text+0xa3528>
  1a34fc: 78 20 05 00  	dsll	$4, $5, 0x1
  1a3500: 08 00 c2 8c  	lw	$2, 0x8($6)
  1a3504: ff ff 83 64  	daddiu	$3, $4, -0x1 <.text+0xffffffffffefffff>
  1a3508: 10 00 c4 fc  	sd	$4, 0x10($6)
  1a350c: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  1a3510: 2b 18 e3 00  	sltu	$3, $7, $3
  1a3514: 08 00 c2 ac  	sw	$2, 0x8($6)
  1a3518: 03 00 60 14  	bnez	$3, 0x1a3528 <.text+0xa3528>
  1a351c: 2d 28 80 00  	move	$5, $4
  1a3520: f7 ff 00 10  	b	0x1a3500 <.text+0xa3500>
  1a3524: 78 20 05 00  	dsll	$4, $5, 0x1
  1a3528: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a352c: fa 10 02 00  	dsrl	$2, $2, 0x3
  1a3530: 03 00 03 24  	addiu	$3, $zero, 0x3
  1a3534: 2b 10 45 00  	sltu	$2, $2, $5
  1a3538: 08 00 40 10  	beqz	$2, 0x1a355c <.text+0xa355c>
  1a353c: 00 00 c3 ac  	sw	$3, 0x0($6)
  1a3540: 08 00 c2 8c  	lw	$2, 0x8($6)
  1a3544: 7a 20 05 00  	dsrl	$4, $5, 0x1
  1a3548: 01 00 a3 30  	andi	$3, $5, 0x1
  1a354c: 25 18 64 00  	or	$3, $3, $4
  1a3550: 01 00 42 24  	addiu	$2, $2, 0x1
  1a3554: 10 00 c3 fc  	sd	$3, 0x10($6)
  1a3558: 08 00 c2 ac  	sw	$2, 0x8($6)
  1a355c: 9c ff 00 10  	b	0x1a33d0 <.text+0xa33d0>
  1a3560: 2d 38 c0 00  	move	$7, $6
  1a3564: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a3568: 04 00 c2 ac  	sw	$2, 0x4($6)
  1a356c: 08 00 cb ac  	sw	$11, 0x8($6)
  1a3570: d8 ff 00 10  	b	0x1a34d4 <.text+0xa34d4>
  1a3574: 10 00 c3 fc  	sd	$3, 0x10($6)
  1a3578: 2d 10 49 01  	daddu	$2, $10, $9
  1a357c: 04 00 c4 ac  	sw	$4, 0x4($6)
  1a3580: 08 00 cb ac  	sw	$11, 0x8($6)
  1a3584: 2d 28 40 00  	move	$5, $2
  1a3588: e7 ff 00 10  	b	0x1a3528 <.text+0xa3528>
  1a358c: 10 00 c2 fc  	sd	$2, 0x10($6)
  1a3590: 2a 10 eb 00  	slt	$2, $7, $11
  1a3594: 03 00 40 50  	beqzl	$2, 0x1a35a4 <.text+0xa35a4>
  1a3598: 2d 58 e0 00  	move	$11, $7
  1a359c: c2 ff 00 10  	b	0x1a34a8 <.text+0xa34a8>
  1a35a0: 2d 48 00 00  	move	$9, $zero
  1a35a4: c0 ff 00 10  	b	0x1a34a8 <.text+0xa34a8>
  1a35a8: 2d 50 00 00  	move	$10, $zero
  1a35ac: 00 00 00 00  	nop
  1a35b0: 70 ff bd 27  	addiu	$sp, $sp, -0x90 <.text+0xffffffffffefff70>
  1a35b4: 2d 18 a0 00  	move	$3, $5
  1a35b8: 2d 10 80 00  	move	$2, $4
  1a35bc: 70 00 b0 ff  	sd	$16, 0x70($sp)
  1a35c0: 60 00 a4 27  	addiu	$4, $sp, 0x60
  1a35c4: 2d 28 a0 03  	move	$5, $sp
  1a35c8: 80 00 bf ff  	sd	$ra, 0x80($sp)
  1a35cc: 68 00 a3 ff  	sd	$3, 0x68($sp)
  1a35d0: 60 00 a2 ff  	sd	$2, 0x60($sp)
  1a35d4: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a35d8: 20 00 b0 27  	addiu	$16, $sp, 0x20
  1a35dc: 68 00 a4 27  	addiu	$4, $sp, 0x68
  1a35e0: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a35e4: 2d 28 00 02  	move	$5, $16
  1a35e8: 40 00 a6 27  	addiu	$6, $sp, 0x40
  1a35ec: 2d 28 00 02  	move	$5, $16
  1a35f0: e0 8c 06 0c  	jal	0x1a3380 <.text+0xa3380>
  1a35f4: 2d 20 a0 03  	move	$4, $sp
  1a35f8: d0 9f 06 0c  	jal	0x1a7f40 <.text+0xa7f40>
  1a35fc: 2d 20 40 00  	move	$4, $2
  1a3600: 70 00 b0 df  	ld	$16, 0x70($sp)
  1a3604: 80 00 bf df  	ld	$ra, 0x80($sp)
  1a3608: 08 00 e0 03  	jr	$ra
  1a360c: 90 00 bd 27  	addiu	$sp, $sp, 0x90
  1a3610: 70 ff bd 27  	addiu	$sp, $sp, -0x90 <.text+0xffffffffffefff70>
  1a3614: 2d 18 a0 00  	move	$3, $5
  1a3618: 2d 10 80 00  	move	$2, $4
  1a361c: 70 00 b0 ff  	sd	$16, 0x70($sp)
  1a3620: 60 00 a4 27  	addiu	$4, $sp, 0x60
  1a3624: 2d 28 a0 03  	move	$5, $sp
  1a3628: 80 00 bf ff  	sd	$ra, 0x80($sp)
  1a362c: 68 00 a3 ff  	sd	$3, 0x68($sp)
  1a3630: 60 00 a2 ff  	sd	$2, 0x60($sp)
  1a3634: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3638: 20 00 b0 27  	addiu	$16, $sp, 0x20
  1a363c: 68 00 a4 27  	addiu	$4, $sp, 0x68
  1a3640: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3644: 2d 28 00 02  	move	$5, $16
  1a3648: 24 00 a2 8f  	lw	$2, 0x24($sp)
  1a364c: 2d 28 00 02  	move	$5, $16
  1a3650: 40 00 a6 27  	addiu	$6, $sp, 0x40
  1a3654: 2d 20 a0 03  	move	$4, $sp
  1a3658: 01 00 42 38  	xori	$2, $2, 0x1
  1a365c: e0 8c 06 0c  	jal	0x1a3380 <.text+0xa3380>
  1a3660: 24 00 a2 af  	sw	$2, 0x24($sp)
  1a3664: d0 9f 06 0c  	jal	0x1a7f40 <.text+0xa7f40>
  1a3668: 2d 20 40 00  	move	$4, $2
  1a366c: 70 00 b0 df  	ld	$16, 0x70($sp)
  1a3670: 80 00 bf df  	ld	$ra, 0x80($sp)
  1a3674: 08 00 e0 03  	jr	$ra
  1a3678: 90 00 bd 27  	addiu	$sp, $sp, 0x90
  1a367c: 00 00 00 00  	nop
  1a3680: 70 ff bd 27  	addiu	$sp, $sp, -0x90 <.text+0xffffffffffefff70>
  1a3684: 2d 18 a0 00  	move	$3, $5
  1a3688: 2d 10 80 00  	move	$2, $4
  1a368c: 70 00 b0 ff  	sd	$16, 0x70($sp)
  1a3690: 60 00 a4 27  	addiu	$4, $sp, 0x60
  1a3694: 2d 28 a0 03  	move	$5, $sp
  1a3698: 80 00 bf ff  	sd	$ra, 0x80($sp)
  1a369c: 68 00 a3 ff  	sd	$3, 0x68($sp)
  1a36a0: 60 00 a2 ff  	sd	$2, 0x60($sp)
  1a36a4: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a36a8: 20 00 b0 27  	addiu	$16, $sp, 0x20
  1a36ac: 68 00 a4 27  	addiu	$4, $sp, 0x68
  1a36b0: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a36b4: 2d 28 00 02  	move	$5, $16
  1a36b8: 40 00 a6 27  	addiu	$6, $sp, 0x40
  1a36bc: 2d 28 00 02  	move	$5, $16
  1a36c0: b8 8d 06 0c  	jal	0x1a36e0 <.text+0xa36e0>
  1a36c4: 2d 20 a0 03  	move	$4, $sp
  1a36c8: d0 9f 06 0c  	jal	0x1a7f40 <.text+0xa7f40>
  1a36cc: 2d 20 40 00  	move	$4, $2
  1a36d0: 70 00 b0 df  	ld	$16, 0x70($sp)
  1a36d4: 80 00 bf df  	ld	$ra, 0x80($sp)
  1a36d8: 08 00 e0 03  	jr	$ra
  1a36dc: 90 00 bd 27  	addiu	$sp, $sp, 0x90
  1a36e0: 00 00 83 8c  	lw	$3, 0x0($4)
  1a36e4: 2d 48 80 00  	move	$9, $4
  1a36e8: 2d 58 a0 00  	move	$11, $5
  1a36ec: 02 00 62 2c  	sltiu	$2, $3, 0x2
  1a36f0: 0b 00 40 14  	bnez	$2, 0x1a3720 <.text+0xa3720>
  1a36f4: 2d 60 c0 00  	move	$12, $6
  1a36f8: 00 00 a4 8c  	lw	$4, 0x0($5)
  1a36fc: 02 00 82 2c  	sltiu	$2, $4, 0x2
  1a3700: 16 00 40 54  	bnezl	$2, 0x1a375c <.text+0xa375c>
  1a3704: 04 00 22 8d  	lw	$2, 0x4($9)
  1a3708: 04 00 62 38  	xori	$2, $3, 0x4
  1a370c: 0e 00 40 14  	bnez	$2, 0x1a3748 <.text+0xa3748>
  1a3710: 04 00 82 38  	xori	$2, $4, 0x4
  1a3714: 02 00 82 38  	xori	$2, $4, 0x2
  1a3718: 09 00 40 50  	beqzl	$2, 0x1a3740 <.text+0xa3740>
  1a371c: 1c 00 02 3c  	lui	$2, 0x1c
  1a3720: 04 00 63 8d  	lw	$3, 0x4($11)
  1a3724: 2d 30 20 01  	move	$6, $9
  1a3728: 04 00 22 8d  	lw	$2, 0x4($9)
  1a372c: 26 10 43 00  	xor	$2, $2, $3
  1a3730: 2b 10 02 00  	sltu	$2, $zero, $2
  1a3734: 04 00 22 ad  	sw	$2, 0x4($9)
  1a3738: 08 00 e0 03  	jr	$ra
  1a373c: 2d 10 c0 00  	move	$2, $6
  1a3740: fd ff 00 10  	b	0x1a3738 <.text+0xa3738>
  1a3744: e0 a7 46 24  	addiu	$6, $2, -0x5820 <.text+0xffffffffffefa7e0>
  1a3748: 0a 00 40 14  	bnez	$2, 0x1a3774 <.text+0xa3774>
  1a374c: 02 00 62 38  	xori	$2, $3, 0x2
  1a3750: fb ff 40 50  	beqzl	$2, 0x1a3740 <.text+0xa3740>
  1a3754: 1c 00 02 3c  	lui	$2, 0x1c
  1a3758: 04 00 22 8d  	lw	$2, 0x4($9)
  1a375c: 2d 30 60 01  	move	$6, $11
  1a3760: 04 00 63 8d  	lw	$3, 0x4($11)
  1a3764: 26 10 43 00  	xor	$2, $2, $3
  1a3768: 2b 10 02 00  	sltu	$2, $zero, $2
  1a376c: f2 ff 00 10  	b	0x1a3738 <.text+0xa3738>
  1a3770: 04 00 62 ad  	sw	$2, 0x4($11)
  1a3774: eb ff 40 50  	beqzl	$2, 0x1a3724 <.text+0xa3724>
  1a3778: 04 00 63 8d  	lw	$3, 0x4($11)
  1a377c: 02 00 82 38  	xori	$2, $4, 0x2
  1a3780: f6 ff 40 50  	beqzl	$2, 0x1a375c <.text+0xa375c>
  1a3784: 04 00 22 8d  	lw	$2, 0x4($9)
  1a3788: 10 00 24 dd  	ld	$4, 0x10($9)
  1a378c: 10 00 a3 dc  	ld	$3, 0x10($5)
  1a3790: 3f 28 04 00  	dsra32	$5, $4, 0x0
  1a3794: 04 00 28 8d  	lw	$8, 0x4($9)
  1a3798: 3f 38 03 00  	dsra32	$7, $3, 0x0
  1a379c: 3c 20 04 00  	dsll32	$4, $4, 0x0
  1a37a0: 3f 20 04 00  	dsra32	$4, $4, 0x0
  1a37a4: 19 00 e4 00  	multu	$7, $4
  1a37a8: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a37ac: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1a37b0: 19 00 65 70  	<unknown>
  1a37b4: 04 00 6a 8d  	lw	$10, 0x4($11)
  1a37b8: 26 40 0a 01  	xor	$8, $8, $10
  1a37bc: 12 10 00 00  	mflo	$2
  1a37c0: 10 68 00 00  	mfhi	$13
  1a37c4: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a37c8: 3c 68 0d 00  	dsll32	$13, $13, 0x0
  1a37cc: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a37d0: 25 68 a2 01  	or	$13, $13, $2
  1a37d4: 12 30 00 70  	<unknown>
  1a37d8: 19 00 64 00  	multu	$3, $4
  1a37dc: 3c 30 06 00  	dsll32	$6, $6, 0x0
  1a37e0: 2b 40 08 00  	sltu	$8, $zero, $8
  1a37e4: 10 10 00 70  	vmm0	$2, $zero, $zero
  1a37e8: 3e 30 06 00  	dsrl32	$6, $6, 0x0
  1a37ec: 19 00 e5 70  	<unknown>
  1a37f0: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a37f4: 25 10 46 00  	or	$2, $2, $6
  1a37f8: 12 30 00 00  	mflo	$6
  1a37fc: 2d 10 a2 01  	daddu	$2, $13, $2
  1a3800: 3c 30 06 00  	dsll32	$6, $6, 0x0
  1a3804: 10 18 00 00  	mfhi	$3
  1a3808: 3c 28 02 00  	dsll32	$5, $2, 0x0
  1a380c: 3f 28 05 00  	dsra32	$5, $5, 0x0
  1a3810: 3e 30 06 00  	dsrl32	$6, $6, 0x0
  1a3814: 12 20 00 70  	<unknown>
  1a3818: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a381c: 3c 20 04 00  	dsll32	$4, $4, 0x0
  1a3820: 10 38 00 70  	vmm0	$7, $zero, $zero
  1a3824: 3e 20 04 00  	dsrl32	$4, $4, 0x0
  1a3828: 3c 38 07 00  	dsll32	$7, $7, 0x0
  1a382c: 25 18 66 00  	or	$3, $3, $6
  1a3830: 25 38 e4 00  	or	$7, $7, $4
  1a3834: 2b 20 4d 00  	sltu	$4, $2, $13
  1a3838: 08 00 26 8d  	lw	$6, 0x8($9)
  1a383c: 3c 68 05 00  	dsll32	$13, $5, 0x0
  1a3840: 3c 20 04 00  	dsll32	$4, $4, 0x0
  1a3844: 2d 48 6d 00  	daddu	$9, $3, $13
  1a3848: 08 00 65 8d  	lw	$5, 0x8($11)
  1a384c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a3850: 2b 18 23 01  	sltu	$3, $9, $3
  1a3854: 2d 10 47 00  	daddu	$2, $2, $7
  1a3858: 2d 20 83 00  	daddu	$4, $4, $3
  1a385c: 2d 38 82 00  	daddu	$7, $4, $2
  1a3860: 21 30 c5 00  	addu	$6, $6, $5
  1a3864: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a3868: fa 10 02 00  	dsrl	$2, $2, 0x3
  1a386c: 04 00 c6 24  	addiu	$6, $6, 0x4
  1a3870: 2b 10 47 00  	sltu	$2, $2, $7
  1a3874: 04 00 88 ad  	sw	$8, 0x4($12)
  1a3878: 12 00 40 10  	beqz	$2, 0x1a38c4 <.text+0xa38c4>
  1a387c: 08 00 86 ad  	sw	$6, 0x8($12)
  1a3880: 00 80 06 34  	ori	$6, $zero, 0x8000
  1a3884: 3c 34 06 00  	dsll32	$6, $6, 0x10
  1a3888: ff ff 05 24  	addiu	$5, $zero, -0x1 <.text+0xffffffffffefffff>
  1a388c: fa 28 05 00  	dsrl	$5, $5, 0x3
  1a3890: 08 00 82 8d  	lw	$2, 0x8($12)
  1a3894: 01 00 e3 30  	andi	$3, $7, 0x1
  1a3898: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a389c: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1a38a0: 7a 38 07 00  	dsrl	$7, $7, 0x1
  1a38a4: 01 00 42 24  	addiu	$2, $2, 0x1
  1a38a8: 2b 20 a7 00  	sltu	$4, $5, $7
  1a38ac: 03 00 60 10  	beqz	$3, 0x1a38bc <.text+0xa38bc>
  1a38b0: 08 00 82 ad  	sw	$2, 0x8($12)
  1a38b4: 7a 48 09 00  	dsrl	$9, $9, 0x1
  1a38b8: 25 48 26 01  	or	$9, $9, $6
  1a38bc: f5 ff 80 54  	bnezl	$4, 0x1a3894 <.text+0xa3894>
  1a38c0: 08 00 82 8d  	lw	$2, 0x8($12)
  1a38c4: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a38c8: 3a 11 02 00  	dsrl	$2, $2, 0x4
  1a38cc: 2b 10 47 00  	sltu	$2, $2, $7
  1a38d0: 11 00 40 14  	bnez	$2, 0x1a3918 <.text+0xa3918>
  1a38d4: ff 00 e3 30  	andi	$3, $7, 0xff
  1a38d8: 00 80 08 34  	ori	$8, $zero, 0x8000
  1a38dc: 3c 44 08 00  	dsll32	$8, $8, 0x10
  1a38e0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1a38e4: ff ff 05 24  	addiu	$5, $zero, -0x1 <.text+0xffffffffffefffff>
  1a38e8: 3a 29 05 00  	dsrl	$5, $5, 0x4
  1a38ec: 78 38 07 00  	dsll	$7, $7, 0x1
  1a38f0: 08 00 83 8d  	lw	$3, 0x8($12)
  1a38f4: 24 20 28 01  	and	$4, $9, $8
  1a38f8: 25 10 e6 00  	or	$2, $7, $6
  1a38fc: 0b 38 44 00  	movn	$7, $2, $4
  1a3900: ff ff 63 24  	addiu	$3, $3, -0x1 <.text+0xffffffffffefffff>
  1a3904: 2b 10 a7 00  	sltu	$2, $5, $7
  1a3908: 08 00 83 ad  	sw	$3, 0x8($12)
  1a390c: f7 ff 40 10  	beqz	$2, 0x1a38ec <.text+0xa38ec>
  1a3910: 78 48 09 00  	dsll	$9, $9, 0x1
  1a3914: ff 00 e3 30  	andi	$3, $7, 0xff
  1a3918: 80 00 02 24  	addiu	$2, $zero, 0x80
  1a391c: 06 00 62 50  	beql	$3, $2, 0x1a3938 <.text+0xa3938>
  1a3920: 00 01 e2 30  	andi	$2, $7, 0x100
  1a3924: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a3928: 10 00 87 fd  	sd	$7, 0x10($12)
  1a392c: 00 00 82 ad  	sw	$2, 0x0($12)
  1a3930: 81 ff 00 10  	b	0x1a3738 <.text+0xa3738>
  1a3934: 2d 30 80 01  	move	$6, $12
  1a3938: 03 00 40 50  	beqzl	$2, 0x1a3948 <.text+0xa3948>
  1a393c: 80 00 e2 64  	daddiu	$2, $7, 0x80
  1a3940: f8 ff 00 10  	b	0x1a3924 <.text+0xa3924>
  1a3944: 80 00 e7 64  	daddiu	$7, $7, 0x80
  1a3948: f6 ff 00 10  	b	0x1a3924 <.text+0xa3924>
  1a394c: 0b 38 49 00  	movn	$7, $2, $9
  1a3950: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  1a3954: 2d 18 a0 00  	move	$3, $5
  1a3958: 2d 10 80 00  	move	$2, $4
  1a395c: 50 00 b0 ff  	sd	$16, 0x50($sp)
  1a3960: 40 00 a4 27  	addiu	$4, $sp, 0x40
  1a3964: 2d 28 a0 03  	move	$5, $sp
  1a3968: 60 00 bf ff  	sd	$ra, 0x60($sp)
  1a396c: 48 00 a3 ff  	sd	$3, 0x48($sp)
  1a3970: 40 00 a2 ff  	sd	$2, 0x40($sp)
  1a3974: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3978: 20 00 b0 27  	addiu	$16, $sp, 0x20
  1a397c: 48 00 a4 27  	addiu	$4, $sp, 0x48
  1a3980: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3984: 2d 28 00 02  	move	$5, $16
  1a3988: 2d 28 00 02  	move	$5, $16
  1a398c: 6c 8e 06 0c  	jal	0x1a39b0 <.text+0xa39b0>
  1a3990: 2d 20 a0 03  	move	$4, $sp
  1a3994: d0 9f 06 0c  	jal	0x1a7f40 <.text+0xa7f40>
  1a3998: 2d 20 40 00  	move	$4, $2
  1a399c: 50 00 b0 df  	ld	$16, 0x50($sp)
  1a39a0: 60 00 bf df  	ld	$ra, 0x60($sp)
  1a39a4: 08 00 e0 03  	jr	$ra
  1a39a8: 70 00 bd 27  	addiu	$sp, $sp, 0x70
  1a39ac: 00 00 00 00  	nop
  1a39b0: 00 00 86 8c  	lw	$6, 0x0($4)
  1a39b4: 2d 38 80 00  	move	$7, $4
  1a39b8: 02 00 c2 2c  	sltiu	$2, $6, 0x2
  1a39bc: 12 00 40 14  	bnez	$2, 0x1a3a08 <.text+0xa3a08>
  1a39c0: 2d 18 80 00  	move	$3, $4
  1a39c4: 00 00 a8 8c  	lw	$8, 0x0($5)
  1a39c8: 02 00 02 2d  	sltiu	$2, $8, 0x2
  1a39cc: 0e 00 40 14  	bnez	$2, 0x1a3a08 <.text+0xa3a08>
  1a39d0: 2d 18 a0 00  	move	$3, $5
  1a39d4: 04 00 82 8c  	lw	$2, 0x4($4)
  1a39d8: 04 00 c4 38  	xori	$4, $6, 0x4
  1a39dc: 04 00 a3 8c  	lw	$3, 0x4($5)
  1a39e0: 26 10 43 00  	xor	$2, $2, $3
  1a39e4: 04 00 80 10  	beqz	$4, 0x1a39f8 <.text+0xa39f8>
  1a39e8: 04 00 e2 ac  	sw	$2, 0x4($7)
  1a39ec: 02 00 c2 38  	xori	$2, $6, 0x2
  1a39f0: 09 00 40 14  	bnez	$2, 0x1a3a18 <.text+0xa3a18>
  1a39f4: 04 00 02 39  	xori	$2, $8, 0x4
  1a39f8: 05 00 c8 50  	beql	$6, $8, 0x1a3a10 <.text+0xa3a10>
  1a39fc: 1c 00 02 3c  	lui	$2, 0x1c
  1a3a00: 2d 18 e0 00  	move	$3, $7
  1a3a04: 00 00 00 00  	nop
  1a3a08: 08 00 e0 03  	jr	$ra
  1a3a0c: 2d 10 60 00  	move	$2, $3
  1a3a10: fd ff 00 10  	b	0x1a3a08 <.text+0xa3a08>
  1a3a14: e0 a7 43 24  	addiu	$3, $2, -0x5820 <.text+0xffffffffffefa7e0>
  1a3a18: 05 00 40 54  	bnezl	$2, 0x1a3a30 <.text+0xa3a30>
  1a3a1c: 02 00 02 39  	xori	$2, $8, 0x2
  1a3a20: 2d 18 e0 00  	move	$3, $7
  1a3a24: 10 00 e0 fc  	sd	$zero, 0x10($7)
  1a3a28: f7 ff 00 10  	b	0x1a3a08 <.text+0xa3a08>
  1a3a2c: 08 00 e0 ac  	sw	$zero, 0x8($7)
  1a3a30: 05 00 40 54  	bnezl	$2, 0x1a3a48 <.text+0xa3a48>
  1a3a34: 08 00 e3 8c  	lw	$3, 0x8($7)
  1a3a38: 04 00 02 24  	addiu	$2, $zero, 0x4
  1a3a3c: 2d 18 e0 00  	move	$3, $7
  1a3a40: f1 ff 00 10  	b	0x1a3a08 <.text+0xa3a08>
  1a3a44: 00 00 e2 ac  	sw	$2, 0x0($7)
  1a3a48: 08 00 a2 8c  	lw	$2, 0x8($5)
  1a3a4c: 10 00 a8 dc  	ld	$8, 0x10($5)
  1a3a50: 10 00 e6 dc  	ld	$6, 0x10($7)
  1a3a54: 23 10 62 00  	subu	$2, $3, $2
  1a3a58: 2b 20 c8 00  	sltu	$4, $6, $8
  1a3a5c: 05 00 80 10  	beqz	$4, 0x1a3a74 <.text+0xa3a74>
  1a3a60: 08 00 e2 ac  	sw	$2, 0x8($7)
  1a3a64: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  1a3a68: 78 30 06 00  	dsll	$6, $6, 0x1
  1a3a6c: 08 00 e2 ac  	sw	$2, 0x8($7)
  1a3a70: 2b 20 c8 00  	sltu	$4, $6, $8
  1a3a74: 00 80 02 34  	ori	$2, $zero, 0x8000
  1a3a78: 7c 13 02 00  	dsll32	$2, $2, 0xd
  1a3a7c: 2d 28 00 00  	move	$5, $zero
  1a3a80: 04 00 80 54  	bnezl	$4, 0x1a3a94 <.text+0xa3a94>
  1a3a84: 7a 10 02 00  	dsrl	$2, $2, 0x1
  1a3a88: 25 28 a2 00  	or	$5, $5, $2
  1a3a8c: 2f 30 c8 00  	dsubu	$6, $6, $8
  1a3a90: 7a 10 02 00  	dsrl	$2, $2, 0x1
  1a3a94: 03 00 40 10  	beqz	$2, 0x1a3aa4 <.text+0xa3aa4>
  1a3a98: 78 30 06 00  	dsll	$6, $6, 0x1
  1a3a9c: f8 ff 00 10  	b	0x1a3a80 <.text+0xa3a80>
  1a3aa0: 2b 20 c8 00  	sltu	$4, $6, $8
  1a3aa4: ff 00 a3 30  	andi	$3, $5, 0xff
  1a3aa8: 80 00 02 24  	addiu	$2, $zero, 0x80
  1a3aac: 03 00 62 50  	beql	$3, $2, 0x1a3abc <.text+0xa3abc>
  1a3ab0: 00 01 a2 30  	andi	$2, $5, 0x100
  1a3ab4: d2 ff 00 10  	b	0x1a3a00 <.text+0xa3a00>
  1a3ab8: 10 00 e5 fc  	sd	$5, 0x10($7)
  1a3abc: 03 00 40 50  	beqzl	$2, 0x1a3acc <.text+0xa3acc>
  1a3ac0: 80 00 a2 64  	daddiu	$2, $5, 0x80
  1a3ac4: fb ff 00 10  	b	0x1a3ab4 <.text+0xa3ab4>
  1a3ac8: 80 00 a5 64  	daddiu	$5, $5, 0x80
  1a3acc: f9 ff 00 10  	b	0x1a3ab4 <.text+0xa3ab4>
  1a3ad0: 0b 28 46 00  	movn	$5, $2, $6
  1a3ad4: 00 00 00 00  	nop
  1a3ad8: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  1a3adc: 2d 18 a0 00  	move	$3, $5
  1a3ae0: 2d 10 80 00  	move	$2, $4
  1a3ae4: 50 00 b0 ff  	sd	$16, 0x50($sp)
  1a3ae8: 40 00 a4 27  	addiu	$4, $sp, 0x40
  1a3aec: 2d 28 a0 03  	move	$5, $sp
  1a3af0: 60 00 bf ff  	sd	$ra, 0x60($sp)
  1a3af4: 40 00 a2 ff  	sd	$2, 0x40($sp)
  1a3af8: 48 00 a3 ff  	sd	$3, 0x48($sp)
  1a3afc: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3b00: 20 00 b0 27  	addiu	$16, $sp, 0x20
  1a3b04: 48 00 a4 27  	addiu	$4, $sp, 0x48
  1a3b08: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3b0c: 2d 28 00 02  	move	$5, $16
  1a3b10: 2d 28 00 02  	move	$5, $16
  1a3b14: 6e a0 06 0c  	jal	0x1a81b8 <.text+0xa81b8>
  1a3b18: 2d 20 a0 03  	move	$4, $sp
  1a3b1c: 50 00 b0 df  	ld	$16, 0x50($sp)
  1a3b20: 60 00 bf df  	ld	$ra, 0x60($sp)
  1a3b24: 08 00 e0 03  	jr	$ra
  1a3b28: 70 00 bd 27  	addiu	$sp, $sp, 0x70
  1a3b2c: 00 00 00 00  	nop
  1a3b30: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a3b34: c2 1f 04 00  	srl	$3, $4, 0x1f
  1a3b38: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a3b3c: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a3b40: 00 00 a2 af  	sw	$2, 0x0($sp)
  1a3b44: 0a 00 80 14  	bnez	$4, 0x1a3b70 <.text+0xa3b70>
  1a3b48: 04 00 a3 af  	sw	$3, 0x4($sp)
  1a3b4c: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a3b50: 00 00 a2 af  	sw	$2, 0x0($sp)
  1a3b54: d0 9f 06 0c  	jal	0x1a7f40 <.text+0xa7f40>
  1a3b58: 2d 20 a0 03  	move	$4, $sp
  1a3b5c: 2d 18 40 00  	move	$3, $2
  1a3b60: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a3b64: 2d 10 60 00  	move	$2, $3
  1a3b68: 08 00 e0 03  	jr	$ra
  1a3b6c: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a3b70: 3c 00 02 24  	addiu	$2, $zero, 0x3c
  1a3b74: 1c 00 60 10  	beqz	$3, 0x1a3be8 <.text+0xa3be8>
  1a3b78: 08 00 a2 af  	sw	$2, 0x8($sp)
  1a3b7c: 00 80 02 3c  	lui	$2, 0x8000
  1a3b80: e0 c1 03 34  	ori	$3, $zero, 0xc1e0
  1a3b84: 3c 1c 03 00  	dsll32	$3, $3, 0x10
  1a3b88: f5 ff 82 10  	beq	$4, $2, 0x1a3b60 <.text+0xa3b60>
  1a3b8c: 23 10 04 00  	negu	$2, $4
  1a3b90: 10 00 a2 ff  	sd	$2, 0x10($sp)
  1a3b94: 10 00 a4 df  	ld	$4, 0x10($sp)
  1a3b98: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a3b9c: 3a 11 02 00  	dsrl	$2, $2, 0x4
  1a3ba0: 2b 10 44 00  	sltu	$2, $2, $4
  1a3ba4: eb ff 40 14  	bnez	$2, 0x1a3b54 <.text+0xa3b54>
  1a3ba8: 08 00 a5 8f  	lw	$5, 0x8($sp)
  1a3bac: ff ff 06 24  	addiu	$6, $zero, -0x1 <.text+0xffffffffffefffff>
  1a3bb0: 3a 31 06 00  	dsrl	$6, $6, 0x4
  1a3bb4: 00 00 00 00  	nop
  1a3bb8: 78 18 04 00  	dsll	$3, $4, 0x1
  1a3bbc: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a3bc0: 2b 10 c3 00  	sltu	$2, $6, $3
		...
  1a3bd0: f9 ff 40 10  	beqz	$2, 0x1a3bb8 <.text+0xa3bb8>
  1a3bd4: 2d 20 60 00  	move	$4, $3
  1a3bd8: 08 00 a5 af  	sw	$5, 0x8($sp)
  1a3bdc: dd ff 00 10  	b	0x1a3b54 <.text+0xa3b54>
  1a3be0: 10 00 a3 ff  	sd	$3, 0x10($sp)
  1a3be4: 00 00 00 00  	nop
  1a3be8: ea ff 00 10  	b	0x1a3b94 <.text+0xa3b94>
  1a3bec: 10 00 a4 ff  	sd	$4, 0x10($sp)
  1a3bf0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a3bf4: 2d 10 80 00  	move	$2, $4
  1a3bf8: 2d 28 a0 03  	move	$5, $sp
  1a3bfc: 20 00 a2 ff  	sd	$2, 0x20($sp)
  1a3c00: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a3c04: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3c08: 20 00 a4 27  	addiu	$4, $sp, 0x20
  1a3c0c: 00 00 a3 8f  	lw	$3, 0x0($sp)
  1a3c10: 02 00 62 38  	xori	$2, $3, 0x2
  1a3c14: 11 00 40 10  	beqz	$2, 0x1a3c5c <.text+0xa3c5c>
  1a3c18: 2d 20 00 00  	move	$4, $zero
  1a3c1c: 02 00 62 2c  	sltiu	$2, $3, 0x2
  1a3c20: 0f 00 40 14  	bnez	$2, 0x1a3c60 <.text+0xa3c60>
  1a3c24: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a3c28: 04 00 62 38  	xori	$2, $3, 0x4
  1a3c2c: 07 00 40 50  	beqzl	$2, 0x1a3c4c <.text+0xa3c4c>
  1a3c30: ff 7f 03 3c  	lui	$3, 0x7fff
  1a3c34: 08 00 a5 8f  	lw	$5, 0x8($sp)
  1a3c38: 09 00 a0 04  	bltz	$5, 0x1a3c60 <.text+0xa3c60>
  1a3c3c: 1f 00 a2 28  	slti	$2, $5, 0x1f
  1a3c40: 0a 00 40 14  	bnez	$2, 0x1a3c6c <.text+0xa3c6c>
  1a3c44: 3c 00 03 24  	addiu	$3, $zero, 0x3c
  1a3c48: ff 7f 03 3c  	lui	$3, 0x7fff
  1a3c4c: 04 00 a2 8f  	lw	$2, 0x4($sp)
  1a3c50: ff ff 63 34  	ori	$3, $3, 0xffff
  1a3c54: 00 80 04 3c  	lui	$4, 0x8000
  1a3c58: 0a 20 62 00  	movz	$4, $3, $2
  1a3c5c: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a3c60: 2d 10 80 00  	move	$2, $4
  1a3c64: 08 00 e0 03  	jr	$ra
  1a3c68: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a3c6c: 10 00 a2 df  	ld	$2, 0x10($sp)
  1a3c70: 23 18 65 00  	subu	$3, $3, $5
  1a3c74: 16 10 62 00  	dsrlv	$2, $2, $3
  1a3c78: 04 00 a3 8f  	lw	$3, 0x4($sp)
  1a3c7c: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a3c80: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a3c84: 23 20 02 00  	negu	$4, $2
  1a3c88: f4 ff 00 10  	b	0x1a3c5c <.text+0xa3c5c>
  1a3c8c: 0a 20 43 00  	movz	$4, $2, $3
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
  1a3cc0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a3cc4: 2d 10 80 00  	move	$2, $4
  1a3cc8: 2d 28 a0 03  	move	$5, $sp
  1a3ccc: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a3cd0: 20 00 a2 ff  	sd	$2, 0x20($sp)
  1a3cd4: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3cd8: 20 00 a4 27  	addiu	$4, $sp, 0x20
  1a3cdc: 10 00 a3 df  	ld	$3, 0x10($sp)
  1a3ce0: ff 3f 02 3c  	lui	$2, 0x3fff
  1a3ce4: ff ff 42 34  	ori	$2, $2, 0xffff
  1a3ce8: 08 00 a6 8f  	lw	$6, 0x8($sp)
  1a3cec: b8 38 03 00  	dsll	$7, $3, 0x2
  1a3cf0: 3f 38 07 00  	dsra32	$7, $7, 0x0
  1a3cf4: 00 00 a4 8f  	lw	$4, 0x0($sp)
  1a3cf8: 24 18 62 00  	and	$3, $3, $2
  1a3cfc: 01 00 e8 34  	ori	$8, $7, 0x1
  1a3d00: 04 00 a5 8f  	lw	$5, 0x4($sp)
  1a3d04: c4 9f 06 0c  	jal	0x1a7f10 <.text+0xa7f10>
  1a3d08: 0b 38 03 01  	movn	$7, $8, $3
  1a3d0c: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a3d10: 08 00 e0 03  	jr	$ra
  1a3d14: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a3d18: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a3d1c: 2d 10 80 00  	move	$2, $4
  1a3d20: 2d 28 a0 03  	move	$5, $sp
  1a3d24: 20 00 a2 ff  	sd	$2, 0x20($sp)
  1a3d28: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a3d2c: 34 a0 06 0c  	jal	0x1a80d0 <.text+0xa80d0>
  1a3d30: 20 00 a4 27  	addiu	$4, $sp, 0x20
  1a3d34: 00 00 a4 8f  	lw	$4, 0x0($sp)
  1a3d38: 02 00 82 38  	xori	$2, $4, 0x2
  1a3d3c: 18 00 40 10  	beqz	$2, 0x1a3da0 <.text+0xa3da0>
  1a3d40: 2d 18 00 00  	move	$3, $zero
  1a3d44: 02 00 82 2c  	sltiu	$2, $4, 0x2
  1a3d48: 16 00 40 14  	bnez	$2, 0x1a3da4 <.text+0xa3da4>
  1a3d4c: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a3d50: 04 00 a2 8f  	lw	$2, 0x4($sp)
  1a3d54: 14 00 40 14  	bnez	$2, 0x1a3da8 <.text+0xa3da8>
  1a3d58: 2d 10 60 00  	move	$2, $3
  1a3d5c: 04 00 82 38  	xori	$2, $4, 0x4
  1a3d60: 10 00 40 10  	beqz	$2, 0x1a3da4 <.text+0xa3da4>
  1a3d64: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a3d68: 08 00 a4 8f  	lw	$4, 0x8($sp)
  1a3d6c: 0d 00 80 04  	bltz	$4, 0x1a3da4 <.text+0xa3da4>
  1a3d70: 2d 18 00 00  	move	$3, $zero
  1a3d74: 20 00 82 28  	slti	$2, $4, 0x20
  1a3d78: 0a 00 40 10  	beqz	$2, 0x1a3da4 <.text+0xa3da4>
  1a3d7c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a3d80: 3d 00 82 28  	slti	$2, $4, 0x3d
  1a3d84: 0a 00 40 14  	bnez	$2, 0x1a3db0 <.text+0xa3db0>
  1a3d88: 3c 00 03 24  	addiu	$3, $zero, 0x3c
  1a3d8c: 10 00 a2 df  	ld	$2, 0x10($sp)
  1a3d90: c4 ff 83 24  	addiu	$3, $4, -0x3c <.text+0xffffffffffefffc4>
  1a3d94: 14 10 62 00  	dsllv	$2, $2, $3
  1a3d98: 3c 18 02 00  	dsll32	$3, $2, 0x0
  1a3d9c: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1a3da0: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a3da4: 2d 10 60 00  	move	$2, $3
  1a3da8: 08 00 e0 03  	jr	$ra
  1a3dac: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a3db0: 10 00 a2 df  	ld	$2, 0x10($sp)
  1a3db4: 23 18 64 00  	subu	$3, $3, $4
  1a3db8: f7 ff 00 10  	b	0x1a3d98 <.text+0xa3d98>
  1a3dbc: 16 10 62 00  	dsrlv	$2, $2, $3
