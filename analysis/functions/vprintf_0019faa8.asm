
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  19faa8: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  19faac: 2d 38 a0 00  	move	$7, $5
  19fab0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  19fab4: 2d 30 80 00  	move	$6, $4
  19fab8: 44 00 10 3c  	lui	$16, 0x44
  19fabc: 00 10 05 24  	addiu	$5, $zero, 0x1000
  19fac0: c0 54 10 26  	addiu	$16, $16, 0x54c0
  19fac4: 20 00 bf ff  	sd	$ra, 0x20($sp)
  19fac8: 2d 20 00 02  	move	$4, $16
  19facc: b8 78 06 0c  	jal	0x19e2e0 <.text+0x9e2e0>
  19fad0: 10 00 b1 ff  	sd	$17, 0x10($sp)
  19fad4: 01 00 04 24  	addiu	$4, $zero, 0x1
  19fad8: 2d 88 40 00  	move	$17, $2
  19fadc: 2d 28 00 02  	move	$5, $16
  19fae0: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  19fae4: 2d 30 40 00  	move	$6, $2
  19fae8: 00 00 b0 df  	ld	$16, 0x0($sp)
  19faec: 2d 10 20 02  	move	$2, $17
  19faf0: 20 00 bf df  	ld	$ra, 0x20($sp)
  19faf4: 10 00 b1 df  	ld	$17, 0x10($sp)
  19faf8: 08 00 e0 03  	jr	$ra
  19fafc: 30 00 bd 27  	addiu	$sp, $sp, 0x30
