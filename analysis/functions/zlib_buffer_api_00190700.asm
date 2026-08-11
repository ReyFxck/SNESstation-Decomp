# SNES Station v0.23 WIP — zlib 1.1.3 buffer wrappers
# Target VA: 0x00190700..0x001908eb

  190700: 80 ff bd 27  	addiu	$sp, $sp, -0x80 <.text+0xffffffffffefff80>
  190704: 3c 38 07 00  	dsll32	$7, $7, 0x0
  190708: 3f 38 07 00  	dsra32	$7, $7, 0x0
  19070c: 60 00 b1 ff  	sd	$17, 0x60($sp)
  190710: 2d 88 a0 00  	move	$17, $5
  190714: 70 00 bf ff  	sd	$ra, 0x70($sp)
  190718: 50 00 b0 ff  	sd	$16, 0x50($sp)
  19071c: 10 00 a4 af  	sw	$4, 0x10($sp)
  190720: fb ff 04 24  	addiu	$4, $zero, -0x5 <.text+0xffffffffffeffffb>
  190724: 00 00 a3 8c  	lw	$3, 0x0($5)
  190728: 00 00 a5 dc  	ld	$5, 0x0($5)
  19072c: 3c 10 03 00  	dsll32	$2, $3, 0x0
  190730: 00 00 a6 af  	sw	$6, 0x0($sp)
  190734: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  190738: 04 00 a7 af  	sw	$7, 0x4($sp)
  19073c: 07 00 45 10  	beq	$2, $5, 0x19075c <.text+0x9075c>
  190740: 14 00 a3 af  	sw	$3, 0x14($sp)
  190744: 70 00 bf df  	ld	$ra, 0x70($sp)
  190748: 2d 10 80 00  	move	$2, $4
  19074c: 60 00 b1 df  	ld	$17, 0x60($sp)
  190750: 50 00 b0 df  	ld	$16, 0x50($sp)
  190754: 08 00 e0 03  	jr	$ra
  190758: 80 00 bd 27  	addiu	$sp, $sp, 0x80
  19075c: 1c 00 06 3c  	lui	$6, 0x1c
  190760: 2d 20 a0 03  	move	$4, $sp
  190764: 2d 28 00 01  	move	$5, $8
  190768: 00 89 c6 24  	addiu	$6, $6, -0x7700 <.text+0xffffffffffef8900>
  19076c: 48 00 07 24  	addiu	$7, $zero, 0x48
  190770: 28 00 a0 af  	sw	$zero, 0x28($sp)
  190774: 2c 00 a0 af  	sw	$zero, 0x2c($sp)
  190778: 2f 42 06 0c  	jal	0x1908bc <.text+0x908bc>
  19077c: 30 00 a0 af  	sw	$zero, 0x30($sp)
  190780: f0 ff 40 14  	bnez	$2, 0x190744 <.text+0x90744>
  190784: 2d 20 40 00  	move	$4, $2
  190788: 2d 20 a0 03  	move	$4, $sp
  19078c: e8 43 06 0c  	jal	0x190fa0 <.text+0x90fa0>
  190790: 04 00 05 24  	addiu	$5, $zero, 0x4
  190794: 2d 80 40 00  	move	$16, $2
  190798: 01 00 02 24  	addiu	$2, $zero, 0x1
  19079c: 06 00 02 12  	beq	$16, $2, 0x1907b8 <.text+0x907b8>
  1907a0: 2d 20 a0 03  	move	$4, $sp
  1907a4: c2 44 06 0c  	jal	0x191308 <.text+0x91308>
  1907a8: 00 00 00 00  	nop
  1907ac: fb ff 04 24  	addiu	$4, $zero, -0x5 <.text+0xffffffffffeffffb>
  1907b0: e4 ff 00 10  	b	0x190744 <.text+0x90744>
  1907b4: 0b 20 10 02  	movn	$4, $16, $16
  1907b8: 18 00 a2 df  	ld	$2, 0x18($sp)
  1907bc: c2 44 06 0c  	jal	0x191308 <.text+0x91308>
  1907c0: 00 00 22 fe  	sd	$2, 0x0($17)
  1907c4: df ff 00 10  	b	0x190744 <.text+0x90744>
  1907c8: 2d 20 40 00  	move	$4, $2
  1907cc: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1907d0: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1907d4: c0 41 06 0c  	jal	0x190700 <.text+0x90700>
  1907d8: ff ff 08 24  	addiu	$8, $zero, -0x1 <.text+0xffffffffffefffff>
  1907dc: 00 00 bf df  	ld	$ra, 0x0($sp)
  1907e0: 08 00 e0 03  	jr	$ra
  1907e4: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1907e8: 3c 18 07 00  	dsll32	$3, $7, 0x0
  1907ec: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1907f0: 80 ff bd 27  	addiu	$sp, $sp, -0x80 <.text+0xffffffffffefff80>
  1907f4: 3c 10 03 00  	dsll32	$2, $3, 0x0
  1907f8: 60 00 b1 ff  	sd	$17, 0x60($sp)
  1907fc: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  190800: 00 00 a6 af  	sw	$6, 0x0($sp)
  190804: 2d 88 a0 00  	move	$17, $5
  190808: 70 00 bf ff  	sd	$ra, 0x70($sp)
  19080c: 50 00 b0 ff  	sd	$16, 0x50($sp)
  190810: 2d 30 80 00  	move	$6, $4
  190814: 04 00 a3 af  	sw	$3, 0x4($sp)
  190818: 07 00 47 10  	beq	$2, $7, 0x190838 <.text+0x90838>
  19081c: fb ff 05 24  	addiu	$5, $zero, -0x5 <.text+0xffffffffffeffffb>
  190820: 70 00 bf df  	ld	$ra, 0x70($sp)
  190824: 2d 10 a0 00  	move	$2, $5
  190828: 60 00 b1 df  	ld	$17, 0x60($sp)
  19082c: 50 00 b0 df  	ld	$16, 0x50($sp)
  190830: 08 00 e0 03  	jr	$ra
  190834: 80 00 bd 27  	addiu	$sp, $sp, 0x80
  190838: 00 00 22 8e  	lw	$2, 0x0($17)
  19083c: 00 00 24 de  	ld	$4, 0x0($17)
  190840: 3c 18 02 00  	dsll32	$3, $2, 0x0
  190844: 10 00 a6 af  	sw	$6, 0x10($sp)
  190848: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  19084c: f4 ff 64 14  	bne	$3, $4, 0x190820 <.text+0x90820>
  190850: 14 00 a2 af  	sw	$2, 0x14($sp)
  190854: 1c 00 05 3c  	lui	$5, 0x1c
  190858: 2d 20 a0 03  	move	$4, $sp
  19085c: 08 89 a5 24  	addiu	$5, $5, -0x76f8 <.text+0xffffffffffef8908>
  190860: 48 00 06 24  	addiu	$6, $zero, 0x48
  190864: 28 00 a0 af  	sw	$zero, 0x28($sp)
  190868: 52 4a 06 0c  	jal	0x192948 <.text+0x92948>
  19086c: 2c 00 a0 af  	sw	$zero, 0x2c($sp)
  190870: eb ff 40 14  	bnez	$2, 0x190820 <.text+0x90820>
  190874: 2d 28 40 00  	move	$5, $2
  190878: 2d 20 a0 03  	move	$4, $sp
  19087c: 5b 4a 06 0c  	jal	0x19296c <.text+0x9296c>
  190880: 04 00 05 24  	addiu	$5, $zero, 0x4
  190884: 2d 80 40 00  	move	$16, $2
  190888: 01 00 02 24  	addiu	$2, $zero, 0x1
  19088c: 06 00 02 12  	beq	$16, $2, 0x1908a8 <.text+0x908a8>
  190890: 2d 20 a0 03  	move	$4, $sp
  190894: e1 49 06 0c  	jal	0x192784 <.text+0x92784>
  190898: 00 00 00 00  	nop
  19089c: fb ff 05 24  	addiu	$5, $zero, -0x5 <.text+0xffffffffffeffffb>
  1908a0: df ff 00 10  	b	0x190820 <.text+0x90820>
  1908a4: 0b 28 10 02  	movn	$5, $16, $16
  1908a8: 18 00 a2 df  	ld	$2, 0x18($sp)
  1908ac: e1 49 06 0c  	jal	0x192784 <.text+0x92784>
  1908b0: 00 00 22 fe  	sd	$2, 0x0($17)
  1908b4: da ff 00 10  	b	0x190820 <.text+0x90820>
  1908b8: 2d 28 40 00  	move	$5, $2
  1908bc: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1908c0: 2d 50 c0 00  	move	$10, $6
  1908c4: 2d 58 e0 00  	move	$11, $7
  1908c8: 08 00 08 24  	addiu	$8, $zero, 0x8
  1908cc: 08 00 06 24  	addiu	$6, $zero, 0x8
  1908d0: 0f 00 07 24  	addiu	$7, $zero, 0xf
  1908d4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1908d8: 3b 42 06 0c  	jal	0x1908ec <.text+0x908ec>
  1908dc: 2d 48 00 00  	move	$9, $zero
  1908e0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1908e4: 08 00 e0 03  	jr	$ra
  1908e8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
