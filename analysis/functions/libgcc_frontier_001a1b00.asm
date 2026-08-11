
/mnt/data/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  1a1b00: 04 00 83 8c  	lw	$3, 0x4($4)
  1a1b04: 18 00 a2 8c  	lw	$2, 0x18($5)
  1a1b08: 03 00 62 50  	beql	$3, $2, 0x1a1b18 <.text+0xa1b18>
  1a1b0c: 10 00 a2 8c  	lw	$2, 0x10($5)
  1a1b10: 08 00 e0 03  	jr	$ra
  1a1b14: 2d 10 c0 00  	move	$2, $6
  1a1b18: fd ff 00 10  	b	0x1a1b10 <.text+0xa1b10>
  1a1b1c: 01 00 46 30  	andi	$6, $2, 0x1
  1a1b20: 3c 10 05 00  	dsll32	$2, $5, 0x0
  1a1b24: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1b28: 3f 30 04 00  	dsra32	$6, $4, 0x0
  1a1b2c: 18 30 c2 00  	<unknown>
  1a1b30: 3c 20 04 00  	dsll32	$4, $4, 0x0
  1a1b34: 3f 20 04 00  	dsra32	$4, $4, 0x0
  1a1b38: 19 00 82 00  	multu	$4, $2
  1a1b3c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1b40: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a1b44: 12 10 00 00  	mflo	$2
  1a1b48: 3f 28 05 00  	dsra32	$5, $5, 0x0
  1a1b4c: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1b50: 10 38 00 00  	mfhi	$7
  1a1b54: 24 40 03 01  	and	$8, $8, $3
  1a1b58: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1b5c: 18 18 85 00  	<unknown>
  1a1b60: 25 40 02 01  	or	$8, $8, $2
  1a1b64: ff ff 02 3c  	lui	$2, 0xffff
  1a1b68: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1b6c: 3c 38 07 00  	dsll32	$7, $7, 0x0
  1a1b70: 24 40 02 01  	and	$8, $8, $2
  1a1b74: 25 40 07 01  	or	$8, $8, $7
  1a1b78: 21 20 66 00  	addu	$4, $3, $6
  1a1b7c: 24 10 02 01  	and	$2, $8, $2
  1a1b80: 3f 18 08 00  	dsra32	$3, $8, 0x0
  1a1b84: 21 18 64 00  	addu	$3, $3, $4
  1a1b88: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a1b8c: 08 00 e0 03  	jr	$ra
  1a1b90: 25 10 43 00  	or	$2, $2, $3
  1a1b94: 00 00 00 00  	nop
  1a1b98: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1b9c: fa 1a 03 00  	dsrl	$3, $3, 0xb
  1a1ba0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1ba4: f8 12 02 00  	dsll	$2, $2, 0xb
  1a1ba8: ba 12 02 00  	dsrl	$2, $2, 0xa
  1a1bac: 2d 18 83 00  	daddu	$3, $4, $3
  1a1bb0: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a1bb4: 2b 10 43 00  	sltu	$2, $2, $3
  1a1bb8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a1bbc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1bc0: 2d 90 80 00  	move	$18, $4
  1a1bc4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a1bc8: ff 07 84 30  	andi	$4, $4, 0x7ff
  1a1bcc: e0 81 10 34  	ori	$16, $zero, 0x81e0
  1a1bd0: fc 83 10 00  	dsll32	$16, $16, 0xf
  1a1bd4: 06 00 40 10  	beqz	$2, 0x1a1bf0 <.text+0xa1bf0>
  1a1bd8: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a1bdc: 04 00 80 10  	beqz	$4, 0x1a1bf0 <.text+0xa1bf0>
  1a1be0: 00 f8 02 24  	addiu	$2, $zero, -0x800 <.text+0xffffffffffeff800>
  1a1be4: 00 08 03 24  	addiu	$3, $zero, 0x800
  1a1be8: 24 90 42 02  	and	$18, $18, $2
  1a1bec: 25 90 43 02  	or	$18, $18, $3
  1a1bf0: 3f 20 12 00  	dsra32	$4, $18, 0x0
  1a1bf4: cc 8e 06 0c  	jal	0x1a3b30 <.text+0xa3b30>
  1a1bf8: 00 00 00 00  	nop
  1a1bfc: 2d 28 00 02  	move	$5, $16
  1a1c00: 2d 20 40 00  	move	$4, $2
  1a1c04: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a1c08: 00 00 00 00  	nop
  1a1c0c: 2d 28 00 02  	move	$5, $16
  1a1c10: ff ff 10 3c  	lui	$16, 0xffff
  1a1c14: 3e 80 10 00  	dsrl32	$16, $16, 0x0
  1a1c18: 2d 20 40 00  	move	$4, $2
  1a1c1c: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a1c20: 24 80 50 02  	and	$16, $18, $16
  1a1c24: 3c 80 10 00  	dsll32	$16, $16, 0x0
  1a1c28: 3f 80 10 00  	dsra32	$16, $16, 0x0
  1a1c2c: 2d 88 40 00  	move	$17, $2
  1a1c30: cc 8e 06 0c  	jal	0x1a3b30 <.text+0xa3b30>
  1a1c34: 2d 20 00 02  	move	$4, $16
  1a1c38: 0f 00 00 06  	bltz	$16, 0x1a1c78 <.text+0xa1c78>
  1a1c3c: 00 00 00 00  	nop
  1a1c40: 2d 28 40 00  	move	$5, $2
  1a1c44: 2d 20 20 02  	move	$4, $17
  1a1c48: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a1c4c: 00 00 00 00  	nop
  1a1c50: 2d 20 40 00  	move	$4, $2
  1a1c54: 30 8f 06 0c  	jal	0x1a3cc0 <.text+0xa3cc0>
  1a1c58: 00 00 00 00  	nop
  1a1c5c: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a1c60: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a1c64: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a1c68: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a1c6c: 08 00 e0 03  	jr	$ra
  1a1c70: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a1c74: 00 00 00 00  	nop
  1a1c78: e0 83 05 34  	ori	$5, $zero, 0x83e0
  1a1c7c: fc 2b 05 00  	dsll32	$5, $5, 0xf
  1a1c80: 2d 20 40 00  	move	$4, $2
  1a1c84: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a1c88: 00 00 00 00  	nop
  1a1c8c: ec ff 00 10  	b	0x1a1c40 <.text+0xa1c40>
		...
  1a1c98: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1a1c9c: 2d 28 00 00  	move	$5, $zero
  1a1ca0: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1a1ca4: 2d 90 80 00  	move	$18, $4
  1a1ca8: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1a1cac: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a1cb0: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a1cb4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1cb8: 26 00 40 04  	bltz	$2, 0x1a1d54 <.text+0xa1d54>
  1a1cbc: 2d 20 00 00  	move	$4, $zero
  1a1cc0: c0 f7 05 34  	ori	$5, $zero, 0xf7c0
  1a1cc4: bc 2b 05 00  	dsll32	$5, $5, 0xe
  1a1cc8: 2d 20 40 02  	move	$4, $18
  1a1ccc: a0 8d 06 0c  	jal	0x1a3680 <.text+0xa3680>
  1a1cd0: 00 00 00 00  	nop
  1a1cd4: 2d 20 40 00  	move	$4, $2
  1a1cd8: 46 8f 06 0c  	jal	0x1a3d18 <.text+0xa3d18>
  1a1cdc: 00 00 00 00  	nop
  1a1ce0: 3c 88 02 00  	dsll32	$17, $2, 0x0
  1a1ce4: 7a 10 11 00  	dsrl	$2, $17, 0x1
  1a1ce8: 01 00 23 32  	andi	$3, $17, 0x1
  1a1cec: 25 18 62 00  	or	$3, $3, $2
  1a1cf0: 29 00 20 06  	bltz	$17, 0x1a1d98 <.text+0xa1d98>
  1a1cf4: 2d 20 20 02  	move	$4, $17
  1a1cf8: 60 9d 06 0c  	jal	0x1a7580 <.text+0xa7580>
  1a1cfc: 00 00 00 00  	nop
  1a1d00: 2d 20 40 02  	move	$4, $18
  1a1d04: 2d 28 40 00  	move	$5, $2
  1a1d08: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a1d0c: 00 00 00 00  	nop
  1a1d10: 2d 80 00 00  	move	$16, $zero
  1a1d14: 2d 28 00 02  	move	$5, $16
  1a1d18: 2d 20 40 00  	move	$4, $2
  1a1d1c: 2d 90 40 00  	move	$18, $2
  1a1d20: b6 8e 06 0c  	jal	0x1a3ad8 <.text+0xa3ad8>
  1a1d24: 00 00 00 00  	nop
  1a1d28: 2d 28 40 02  	move	$5, $18
  1a1d2c: 2d 20 00 02  	move	$4, $16
  1a1d30: 0f 00 40 04  	bltz	$2, 0x1a1d70 <.text+0xa1d70>
  1a1d34: 00 00 00 00  	nop
  1a1d38: 2d 20 40 02  	move	$4, $18
  1a1d3c: 46 8f 06 0c  	jal	0x1a3d18 <.text+0xa3d18>
  1a1d40: 00 00 00 00  	nop
  1a1d44: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1d48: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1d4c: 2d 88 22 02  	daddu	$17, $17, $2
  1a1d50: 2d 20 20 02  	move	$4, $17
  1a1d54: 30 00 bf df  	ld	$ra, 0x30($sp)
  1a1d58: 2d 10 80 00  	move	$2, $4
  1a1d5c: 20 00 b2 df  	ld	$18, 0x20($sp)
  1a1d60: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a1d64: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a1d68: 08 00 e0 03  	jr	$ra
  1a1d6c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1a1d70: 84 8d 06 0c  	jal	0x1a3610 <.text+0xa3610>
  1a1d74: 00 00 00 00  	nop
  1a1d78: 2d 20 40 00  	move	$4, $2
  1a1d7c: 46 8f 06 0c  	jal	0x1a3d18 <.text+0xa3d18>
  1a1d80: 00 00 00 00  	nop
  1a1d84: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1d88: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1d8c: f0 ff 00 10  	b	0x1a1d50 <.text+0xa1d50>
  1a1d90: 2f 88 22 02  	dsubu	$17, $17, $2
  1a1d94: 00 00 00 00  	nop
  1a1d98: 60 9d 06 0c  	jal	0x1a7580 <.text+0xa7580>
  1a1d9c: 2d 20 60 00  	move	$4, $3
  1a1da0: 2d 20 40 00  	move	$4, $2
  1a1da4: 2d 28 40 00  	move	$5, $2
  1a1da8: 6c 8d 06 0c  	jal	0x1a35b0 <.text+0xa35b0>
  1a1dac: 00 00 00 00  	nop
  1a1db0: d3 ff 00 10  	b	0x1a1d00 <.text+0xa1d00>
  1a1db4: 00 00 00 00  	nop
  1a1db8: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1dbc: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1dc0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1dc4: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a1dc8: 24 40 02 01  	and	$8, $8, $2
  1a1dcc: 24 48 23 01  	and	$9, $9, $3
  1a1dd0: 3c 10 04 00  	dsll32	$2, $4, 0x0
  1a1dd4: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1dd8: 3f 38 04 00  	dsra32	$7, $4, 0x0
  1a1ddc: 23 10 02 00  	negu	$2, $2
  1a1de0: 2d 50 80 00  	move	$10, $4
  1a1de4: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1de8: 23 20 07 00  	negu	$4, $7
  1a1dec: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1df0: 3f 58 05 00  	dsra32	$11, $5, 0x0
  1a1df4: 25 40 02 01  	or	$8, $8, $2
  1a1df8: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  1a1dfc: 3c 10 05 00  	dsll32	$2, $5, 0x0
  1a1e00: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1e04: 3c 18 08 00  	dsll32	$3, $8, 0x0
  1a1e08: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1a1e0c: 23 10 02 00  	negu	$2, $2
  1a1e10: 2b 18 03 00  	sltu	$3, $zero, $3
  1a1e14: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1e18: 23 20 83 00  	subu	$4, $4, $3
  1a1e1c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1e20: 3c 60 04 00  	dsll32	$12, $4, 0x0
  1a1e24: 25 48 22 01  	or	$9, $9, $2
  1a1e28: 23 20 0b 00  	negu	$4, $11
  1a1e2c: 3c 10 09 00  	dsll32	$2, $9, 0x0
  1a1e30: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1e34: ff ff 03 3c  	lui	$3, 0xffff
  1a1e38: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a1e3c: 2b 10 02 00  	sltu	$2, $zero, $2
  1a1e40: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1e44: 23 20 82 00  	subu	$4, $4, $2
  1a1e48: 20 00 bf ff  	sd	$ra, 0x20($sp)
  1a1e4c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1a1e50: 2d 30 00 00  	move	$6, $zero
  1a1e54: 24 40 03 01  	and	$8, $8, $3
  1a1e58: 24 48 23 01  	and	$9, $9, $3
  1a1e5c: 3c 10 04 00  	dsll32	$2, $4, 0x0
  1a1e60: 25 00 e0 04  	bltz	$7, 0x1a1ef8 <.text+0xa1ef8>
  1a1e64: 2d 80 00 00  	move	$16, $zero
  1a1e68: 1f 00 60 05  	bltz	$11, 0x1a1ee8 <.text+0xa1ee8>
  1a1e6c: 2d 20 40 01  	move	$4, $10
  1a1e70: c2 87 06 0c  	jal	0x1a1f08 <.text+0xa1f08>
  1a1e74: 00 00 00 00  	nop
  1a1e78: 15 00 00 12  	beqz	$16, 0x1a1ed0 <.text+0xa1ed0>
  1a1e7c: 2d 20 40 00  	move	$4, $2
  1a1e80: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1e84: 3f 10 02 00  	dsra32	$2, $2, 0x0
  1a1e88: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1e8c: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a1e90: 23 10 02 00  	negu	$2, $2
  1a1e94: 24 88 23 02  	and	$17, $17, $3
  1a1e98: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a1e9c: 3f 20 04 00  	dsra32	$4, $4, 0x0
  1a1ea0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1ea4: 23 20 04 00  	negu	$4, $4
  1a1ea8: 25 88 22 02  	or	$17, $17, $2
  1a1eac: ff ff 02 3c  	lui	$2, 0xffff
  1a1eb0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a1eb4: 3c 18 11 00  	dsll32	$3, $17, 0x0
  1a1eb8: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1a1ebc: 24 88 22 02  	and	$17, $17, $2
  1a1ec0: 2b 18 03 00  	sltu	$3, $zero, $3
  1a1ec4: 23 20 83 00  	subu	$4, $4, $3
  1a1ec8: 3c 20 04 00  	dsll32	$4, $4, 0x0
  1a1ecc: 25 20 24 02  	or	$4, $17, $4
  1a1ed0: 20 00 bf df  	ld	$ra, 0x20($sp)
  1a1ed4: 2d 10 80 00  	move	$2, $4
  1a1ed8: 10 00 b1 df  	ld	$17, 0x10($sp)
  1a1edc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a1ee0: 08 00 e0 03  	jr	$ra
  1a1ee4: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1a1ee8: 25 28 22 01  	or	$5, $9, $2
  1a1eec: e0 ff 00 10  	b	0x1a1e70 <.text+0xa1e70>
  1a1ef0: 27 80 10 00  	nor	$16, $zero, $16
  1a1ef4: 00 00 00 00  	nop
  1a1ef8: 25 50 0c 01  	or	$10, $8, $12
  1a1efc: da ff 00 10  	b	0x1a1e68 <.text+0xa1e68>
  1a1f00: ff ff 10 24  	addiu	$16, $zero, -0x1 <.text+0xffffffffffefffff>
  1a1f04: 00 00 00 00  	nop
  1a1f08: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a1f0c: 3f 38 05 00  	dsra32	$7, $5, 0x0
  1a1f10: 3f 40 04 00  	dsra32	$8, $4, 0x0
  1a1f14: 3c 48 05 00  	dsll32	$9, $5, 0x0
  1a1f18: 3f 48 09 00  	dsra32	$9, $9, 0x0
  1a1f1c: 3c 68 04 00  	dsll32	$13, $4, 0x0
  1a1f20: 3f 68 0d 00  	dsra32	$13, $13, 0x0
  1a1f24: 08 01 e0 14  	bnez	$7, 0x1a2348 <.text+0xa2348>
  1a1f28: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a1f2c: 2b 10 09 01  	sltu	$2, $8, $9
  1a1f30: 71 00 40 10  	beqz	$2, 0x1a20f8 <.text+0xa20f8>
  1a1f34: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a1f38: 2b 10 49 00  	sltu	$2, $2, $9
  1a1f3c: 66 00 40 14  	bnez	$2, 0x1a20d8 <.text+0xa20d8>
  1a1f40: ff 00 02 3c  	lui	$2, 0xff
  1a1f44: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a1f48: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a1f4c: 2d 28 40 00  	move	$5, $2
  1a1f50: 0b 28 03 00  	movn	$5, $zero, $3
  1a1f54: 1c 00 03 3c  	lui	$3, 0x1c
  1a1f58: 06 10 a9 00  	srlv	$2, $9, $5
  1a1f5c: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a1f60: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a1f64: 21 10 43 00  	addu	$2, $2, $3
  1a1f68: 00 00 44 90  	lbu	$4, 0x0($2)
  1a1f6c: 21 20 85 00  	addu	$4, $4, $5
  1a1f70: 23 78 e4 00  	subu	$15, $7, $4
  1a1f74: 08 00 e0 51  	beqzl	$15, 0x1a1f98 <.text+0xa1f98>
  1a1f78: 02 3c 09 00  	srl	$7, $9, 0x10
  1a1f7c: 23 10 ef 00  	subu	$2, $7, $15
  1a1f80: 04 18 e8 01  	sllv	$3, $8, $15
  1a1f84: 06 10 4d 00  	srlv	$2, $13, $2
  1a1f88: 04 48 e9 01  	sllv	$9, $9, $15
  1a1f8c: 25 40 62 00  	or	$8, $3, $2
  1a1f90: 04 68 ed 01  	sllv	$13, $13, $15
  1a1f94: 02 3c 09 00  	srl	$7, $9, 0x10
  1a1f98: ff ff 2a 31  	andi	$10, $9, 0xffff
  1a1f9c: 1b 00 07 01  	divu	$zero, $8, $7
  1a1fa0: 02 24 0d 00  	srl	$4, $13, 0x10
  1a1fa4: 01 00 e0 50  	beqzl	$7, 0x1a1fac <.text+0xa1fac>
  1a1fa8: cd 01 00 00  	break	0x0, 0x7
  1a1fac: 12 10 00 00  	mflo	$2
  1a1fb0: 10 18 00 00  	mfhi	$3
  1a1fb4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a1fb8: 25 18 64 00  	or	$3, $3, $4
  1a1fbc: 12 28 00 00  	mflo	$5
  1a1fc0: 18 40 4a 00  	<unknown>
  1a1fc4: 2b 10 68 00  	sltu	$2, $3, $8
  1a1fc8: 0c 00 40 50  	beqzl	$2, 0x1a1ffc <.text+0xa1ffc>
  1a1fcc: 23 18 68 00  	subu	$3, $3, $8
  1a1fd0: 21 18 69 00  	addu	$3, $3, $9
  1a1fd4: 2b 10 69 00  	sltu	$2, $3, $9
  1a1fd8: 07 00 40 14  	bnez	$2, 0x1a1ff8 <.text+0xa1ff8>
  1a1fdc: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a1fe0: 2b 10 68 00  	sltu	$2, $3, $8
  1a1fe4: 05 00 40 50  	beqzl	$2, 0x1a1ffc <.text+0xa1ffc>
  1a1fe8: 23 18 68 00  	subu	$3, $3, $8
  1a1fec: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a1ff0: 21 18 69 00  	addu	$3, $3, $9
  1a1ff4: 00 00 00 00  	nop
  1a1ff8: 23 18 68 00  	subu	$3, $3, $8
  1a1ffc: 01 00 e0 50  	beqzl	$7, 0x1a2004 <.text+0xa2004>
  1a2000: cd 01 00 00  	break	0x0, 0x7
  1a2004: 1b 00 67 00  	divu	$zero, $3, $7
  1a2008: ff ff a4 31  	andi	$4, $13, 0xffff
  1a200c: 12 10 00 00  	mflo	$2
  1a2010: 10 18 00 00  	mfhi	$3
  1a2014: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2018: 25 18 64 00  	or	$3, $3, $4
  1a201c: 12 38 00 00  	mflo	$7
  1a2020: 18 40 4a 00  	<unknown>
  1a2024: 2b 10 68 00  	sltu	$2, $3, $8
  1a2028: 0c 00 40 10  	beqz	$2, 0x1a205c <.text+0xa205c>
  1a202c: 00 14 05 00  	sll	$2, $5, 0x10
  1a2030: 21 18 69 00  	addu	$3, $3, $9
  1a2034: 2b 10 69 00  	sltu	$2, $3, $9
  1a2038: 07 00 40 14  	bnez	$2, 0x1a2058 <.text+0xa2058>
  1a203c: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2040: 2b 10 68 00  	sltu	$2, $3, $8
  1a2044: 05 00 40 10  	beqz	$2, 0x1a205c <.text+0xa205c>
  1a2048: 00 14 05 00  	sll	$2, $5, 0x10
  1a204c: 21 18 69 00  	addu	$3, $3, $9
  1a2050: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2054: 00 00 00 00  	nop
  1a2058: 00 14 05 00  	sll	$2, $5, 0x10
  1a205c: 23 68 68 00  	subu	$13, $3, $8
  1a2060: 25 50 47 00  	or	$10, $2, $7
  1a2064: 2d 80 00 00  	move	$16, $zero
  1a2068: 0b 00 c0 10  	beqz	$6, 0x1a2098 <.text+0xa2098>
  1a206c: 06 10 ed 01  	srlv	$2, $13, $15
  1a2070: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2074: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2078: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a207c: 24 60 83 01  	and	$12, $12, $3
  1a2080: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2084: ff ff 03 3c  	lui	$3, 0xffff
  1a2088: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a208c: 25 60 82 01  	or	$12, $12, $2
  1a2090: 24 60 83 01  	and	$12, $12, $3
  1a2094: 00 00 cc fc  	sd	$12, 0x0($6)
  1a2098: 3c 10 0a 00  	dsll32	$2, $10, 0x0
  1a209c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a20a0: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a20a4: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a20a8: 24 70 c3 01  	and	$14, $14, $3
  1a20ac: 25 70 c2 01  	or	$14, $14, $2
  1a20b0: 3c 18 10 00  	dsll32	$3, $16, 0x0
  1a20b4: ff ff 02 3c  	lui	$2, 0xffff
  1a20b8: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a20bc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a20c0: 24 70 c2 01  	and	$14, $14, $2
  1a20c4: 25 70 c3 01  	or	$14, $14, $3
  1a20c8: 2d 10 c0 01  	move	$2, $14
  1a20cc: 08 00 e0 03  	jr	$ra
  1a20d0: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a20d4: 00 00 00 00  	nop
  1a20d8: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a20dc: ff ff 42 34  	ori	$2, $2, 0xffff
  1a20e0: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a20e4: 2b 10 49 00  	sltu	$2, $2, $9
  1a20e8: 2d 28 60 00  	move	$5, $3
  1a20ec: 99 ff 00 10  	b	0x1a1f54 <.text+0xa1f54>
  1a20f0: 0b 28 82 00  	movn	$5, $4, $2
  1a20f4: 00 00 00 00  	nop
  1a20f8: 08 00 20 15  	bnez	$9, 0x1a211c <.text+0xa211c>
  1a20fc: 2b 10 49 00  	sltu	$2, $2, $9
  1a2100: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a2104: 01 00 20 51  	beqzl	$9, 0x1a210c <.text+0xa210c>
  1a2108: cd 01 00 00  	break	0x0, 0x7
  1a210c: 1b 00 47 00  	divu	$zero, $2, $7
  1a2110: 12 48 00 00  	mflo	$9
  1a2114: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2118: 2b 10 49 00  	sltu	$2, $2, $9
  1a211c: 82 00 40 14  	bnez	$2, 0x1a2328 <.text+0xa2328>
  1a2120: ff 00 02 3c  	lui	$2, 0xff
  1a2124: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2128: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a212c: 2d 28 40 00  	move	$5, $2
  1a2130: 0b 28 03 00  	movn	$5, $zero, $3
  1a2134: 1c 00 03 3c  	lui	$3, 0x1c
  1a2138: 06 10 a9 00  	srlv	$2, $9, $5
  1a213c: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2140: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a2144: 21 10 43 00  	addu	$2, $2, $3
  1a2148: 00 00 44 90  	lbu	$4, 0x0($2)
  1a214c: 21 20 85 00  	addu	$4, $4, $5
  1a2150: 23 78 e4 00  	subu	$15, $7, $4
  1a2154: 38 00 e0 15  	bnez	$15, 0x1a2238 <.text+0xa2238>
  1a2158: 23 c0 ef 00  	subu	$24, $7, $15
  1a215c: 23 40 09 01  	subu	$8, $8, $9
  1a2160: 01 00 10 24  	addiu	$16, $zero, 0x1
  1a2164: 02 54 09 00  	srl	$10, $9, 0x10
  1a2168: ff ff 39 31  	andi	$25, $9, 0xffff
  1a216c: 1b 00 0a 01  	divu	$zero, $8, $10
  1a2170: 02 2c 0d 00  	srl	$5, $13, 0x10
  1a2174: 01 00 40 51  	beqzl	$10, 0x1a217c <.text+0xa217c>
  1a2178: cd 01 00 00  	break	0x0, 0x7
  1a217c: 12 20 00 00  	mflo	$4
  1a2180: 10 18 00 00  	mfhi	$3
  1a2184: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2188: 25 18 65 00  	or	$3, $3, $5
  1a218c: 12 58 00 00  	mflo	$11
  1a2190: 18 38 99 00  	<unknown>
  1a2194: 2b 10 67 00  	sltu	$2, $3, $7
  1a2198: 0c 00 40 50  	beqzl	$2, 0x1a21cc <.text+0xa21cc>
  1a219c: 23 18 67 00  	subu	$3, $3, $7
  1a21a0: 21 18 69 00  	addu	$3, $3, $9
  1a21a4: 2b 10 69 00  	sltu	$2, $3, $9
  1a21a8: 07 00 40 14  	bnez	$2, 0x1a21c8 <.text+0xa21c8>
  1a21ac: ff ff 8b 24  	addiu	$11, $4, -0x1 <.text+0xffffffffffefffff>
  1a21b0: 2b 10 67 00  	sltu	$2, $3, $7
  1a21b4: 05 00 40 50  	beqzl	$2, 0x1a21cc <.text+0xa21cc>
  1a21b8: 23 18 67 00  	subu	$3, $3, $7
  1a21bc: ff ff 6b 25  	addiu	$11, $11, -0x1 <.text+0xffffffffffefffff>
  1a21c0: 21 18 69 00  	addu	$3, $3, $9
  1a21c4: 00 00 00 00  	nop
  1a21c8: 23 18 67 00  	subu	$3, $3, $7
  1a21cc: ff ff a4 31  	andi	$4, $13, 0xffff
  1a21d0: 1b 00 6a 00  	divu	$zero, $3, $10
  1a21d4: 01 00 40 51  	beqzl	$10, 0x1a21dc <.text+0xa21dc>
  1a21d8: cd 01 00 00  	break	0x0, 0x7
  1a21dc: 12 10 00 00  	mflo	$2
  1a21e0: 10 18 00 00  	mfhi	$3
  1a21e4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a21e8: 25 18 64 00  	or	$3, $3, $4
  1a21ec: 12 40 00 00  	mflo	$8
  1a21f0: 18 38 59 00  	<unknown>
  1a21f4: 2b 10 67 00  	sltu	$2, $3, $7
  1a21f8: 0c 00 40 10  	beqz	$2, 0x1a222c <.text+0xa222c>
  1a21fc: 00 14 0b 00  	sll	$2, $11, 0x10
  1a2200: 21 18 69 00  	addu	$3, $3, $9
  1a2204: 2b 10 69 00  	sltu	$2, $3, $9
  1a2208: 07 00 40 14  	bnez	$2, 0x1a2228 <.text+0xa2228>
  1a220c: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2210: 2b 10 67 00  	sltu	$2, $3, $7
  1a2214: 05 00 40 10  	beqz	$2, 0x1a222c <.text+0xa222c>
  1a2218: 00 14 0b 00  	sll	$2, $11, 0x10
  1a221c: 21 18 69 00  	addu	$3, $3, $9
  1a2220: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2224: 00 00 00 00  	nop
  1a2228: 00 14 0b 00  	sll	$2, $11, 0x10
  1a222c: 23 68 67 00  	subu	$13, $3, $7
  1a2230: 8d ff 00 10  	b	0x1a2068 <.text+0xa2068>
  1a2234: 25 50 48 00  	or	$10, $2, $8
  1a2238: 04 48 e9 01  	sllv	$9, $9, $15
  1a223c: 06 28 08 03  	srlv	$5, $8, $24
  1a2240: 02 54 09 00  	srl	$10, $9, 0x10
  1a2244: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2248: ff ff 39 31  	andi	$25, $9, 0xffff
  1a224c: 06 18 0d 03  	srlv	$3, $13, $24
  1a2250: 04 10 e8 01  	sllv	$2, $8, $15
  1a2254: 25 40 43 00  	or	$8, $2, $3
  1a2258: 01 00 40 51  	beqzl	$10, 0x1a2260 <.text+0xa2260>
  1a225c: cd 01 00 00  	break	0x0, 0x7
  1a2260: 02 1c 08 00  	srl	$3, $8, 0x10
  1a2264: 2d 58 40 01  	move	$11, $10
  1a2268: 04 68 ed 01  	sllv	$13, $13, $15
  1a226c: 12 28 00 00  	mflo	$5
  1a2270: 10 20 00 00  	mfhi	$4
  1a2274: 00 24 04 00  	sll	$4, $4, 0x10
  1a2278: 25 18 83 00  	or	$3, $4, $3
  1a227c: 12 c0 00 00  	mflo	$24
  1a2280: 18 38 b9 00  	<unknown>
  1a2284: 2b 10 67 00  	sltu	$2, $3, $7
  1a2288: 0b 00 40 10  	beqz	$2, 0x1a22b8 <.text+0xa22b8>
  1a228c: 2d 80 20 03  	move	$16, $25
  1a2290: 21 18 69 00  	addu	$3, $3, $9
  1a2294: 2b 10 69 00  	sltu	$2, $3, $9
  1a2298: 07 00 40 14  	bnez	$2, 0x1a22b8 <.text+0xa22b8>
  1a229c: ff ff b8 24  	addiu	$24, $5, -0x1 <.text+0xffffffffffefffff>
  1a22a0: 2b 10 67 00  	sltu	$2, $3, $7
  1a22a4: 05 00 40 50  	beqzl	$2, 0x1a22bc <.text+0xa22bc>
  1a22a8: 23 18 67 00  	subu	$3, $3, $7
  1a22ac: ff ff 18 27  	addiu	$24, $24, -0x1 <.text+0xffffffffffefffff>
  1a22b0: 21 18 69 00  	addu	$3, $3, $9
  1a22b4: 00 00 00 00  	nop
  1a22b8: 23 18 67 00  	subu	$3, $3, $7
  1a22bc: ff ff 04 31  	andi	$4, $8, 0xffff
  1a22c0: 1b 00 6b 00  	divu	$zero, $3, $11
  1a22c4: 01 00 60 51  	beqzl	$11, 0x1a22cc <.text+0xa22cc>
  1a22c8: cd 01 00 00  	break	0x0, 0x7
  1a22cc: 12 10 00 00  	mflo	$2
  1a22d0: 10 18 00 00  	mfhi	$3
  1a22d4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a22d8: 25 18 64 00  	or	$3, $3, $4
  1a22dc: 12 28 00 00  	mflo	$5
  1a22e0: 18 38 50 00  	<unknown>
  1a22e4: 2b 10 67 00  	sltu	$2, $3, $7
  1a22e8: 0c 00 40 10  	beqz	$2, 0x1a231c <.text+0xa231c>
  1a22ec: 00 14 18 00  	sll	$2, $24, 0x10
  1a22f0: 21 18 69 00  	addu	$3, $3, $9
  1a22f4: 2b 10 69 00  	sltu	$2, $3, $9
  1a22f8: 07 00 40 14  	bnez	$2, 0x1a2318 <.text+0xa2318>
  1a22fc: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a2300: 2b 10 67 00  	sltu	$2, $3, $7
  1a2304: 05 00 40 10  	beqz	$2, 0x1a231c <.text+0xa231c>
  1a2308: 00 14 18 00  	sll	$2, $24, 0x10
  1a230c: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a2310: 21 18 69 00  	addu	$3, $3, $9
  1a2314: 00 00 00 00  	nop
  1a2318: 00 14 18 00  	sll	$2, $24, 0x10
  1a231c: 23 40 67 00  	subu	$8, $3, $7
  1a2320: 92 ff 00 10  	b	0x1a216c <.text+0xa216c>
  1a2324: 25 80 45 00  	or	$16, $2, $5
  1a2328: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a232c: ff ff 42 34  	ori	$2, $2, 0xffff
  1a2330: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a2334: 2b 10 49 00  	sltu	$2, $2, $9
  1a2338: 2d 28 60 00  	move	$5, $3
  1a233c: 7d ff 00 10  	b	0x1a2134 <.text+0xa2134>
  1a2340: 0b 28 82 00  	movn	$5, $4, $2
  1a2344: 00 00 00 00  	nop
  1a2348: 2b 10 07 01  	sltu	$2, $8, $7
  1a234c: 1f 00 40 14  	bnez	$2, 0x1a23cc <.text+0xa23cc>
  1a2350: 2d 50 00 00  	move	$10, $zero
  1a2354: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2358: 2b 10 47 00  	sltu	$2, $2, $7
  1a235c: 8c 00 40 14  	bnez	$2, 0x1a2590 <.text+0xa2590>
  1a2360: ff 00 02 3c  	lui	$2, 0xff
  1a2364: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2368: 00 01 e3 2c  	sltiu	$3, $7, 0x100
  1a236c: 2d 28 40 00  	move	$5, $2
  1a2370: 0b 28 03 00  	movn	$5, $zero, $3
  1a2374: 1c 00 03 3c  	lui	$3, 0x1c
  1a2378: 06 10 a7 00  	srlv	$2, $7, $5
  1a237c: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2380: 20 00 0a 24  	addiu	$10, $zero, 0x20
  1a2384: 21 10 43 00  	addu	$2, $2, $3
  1a2388: 00 00 44 90  	lbu	$4, 0x0($2)
  1a238c: 21 20 85 00  	addu	$4, $4, $5
  1a2390: 23 78 44 01  	subu	$15, $10, $4
  1a2394: 1c 00 e0 15  	bnez	$15, 0x1a2408 <.text+0xa2408>
  1a2398: 23 c0 4f 01  	subu	$24, $10, $15
  1a239c: 2b 10 e8 00  	sltu	$2, $7, $8
  1a23a0: 05 00 40 14  	bnez	$2, 0x1a23b8 <.text+0xa23b8>
  1a23a4: 23 20 a9 01  	subu	$4, $13, $9
  1a23a8: 2b 10 a9 01  	sltu	$2, $13, $9
  1a23ac: 07 00 40 14  	bnez	$2, 0x1a23cc <.text+0xa23cc>
  1a23b0: 2d 50 00 00  	move	$10, $zero
  1a23b4: 23 20 a9 01  	subu	$4, $13, $9
  1a23b8: 23 18 07 01  	subu	$3, $8, $7
  1a23bc: 2b 10 a4 01  	sltu	$2, $13, $4
  1a23c0: 01 00 0a 24  	addiu	$10, $zero, 0x1
  1a23c4: 23 40 62 00  	subu	$8, $3, $2
  1a23c8: 2d 68 80 00  	move	$13, $4
  1a23cc: 32 ff c0 10  	beqz	$6, 0x1a2098 <.text+0xa2098>
  1a23d0: 2d 80 00 00  	move	$16, $zero
  1a23d4: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a23d8: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a23dc: 3c 10 0d 00  	dsll32	$2, $13, 0x0
  1a23e0: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a23e4: 24 60 83 01  	and	$12, $12, $3
  1a23e8: 25 60 82 01  	or	$12, $12, $2
  1a23ec: 3c 18 08 00  	dsll32	$3, $8, 0x0
  1a23f0: ff ff 02 3c  	lui	$2, 0xffff
  1a23f4: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a23f8: 24 60 82 01  	and	$12, $12, $2
  1a23fc: 25 ff 00 10  	b	0x1a2094 <.text+0xa2094>
  1a2400: 25 60 83 01  	or	$12, $12, $3
  1a2404: 00 00 00 00  	nop
  1a2408: 04 18 e7 01  	sllv	$3, $7, $15
  1a240c: 06 10 09 03  	srlv	$2, $9, $24
  1a2410: 06 28 08 03  	srlv	$5, $8, $24
  1a2414: 25 38 62 00  	or	$7, $3, $2
  1a2418: 04 20 e8 01  	sllv	$4, $8, $15
  1a241c: 02 54 07 00  	srl	$10, $7, 0x10
  1a2420: ff ff f0 30  	andi	$16, $7, 0xffff
  1a2424: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2428: 06 10 0d 03  	srlv	$2, $13, $24
  1a242c: 25 40 82 00  	or	$8, $4, $2
  1a2430: 01 00 40 51  	beqzl	$10, 0x1a2438 <.text+0xa2438>
  1a2434: cd 01 00 00  	break	0x0, 0x7
  1a2438: 02 24 08 00  	srl	$4, $8, 0x10
  1a243c: 04 48 e9 01  	sllv	$9, $9, $15
  1a2440: 12 28 00 00  	mflo	$5
  1a2444: 10 18 00 00  	mfhi	$3
  1a2448: 00 1c 03 00  	sll	$3, $3, 0x10
  1a244c: 25 18 64 00  	or	$3, $3, $4
  1a2450: 12 c8 00 00  	mflo	$25
  1a2454: 18 58 b0 00  	<unknown>
  1a2458: 2b 10 6b 00  	sltu	$2, $3, $11
  1a245c: 0a 00 40 10  	beqz	$2, 0x1a2488 <.text+0xa2488>
  1a2460: 04 68 ed 01  	sllv	$13, $13, $15
  1a2464: 21 18 67 00  	addu	$3, $3, $7
  1a2468: 2b 10 67 00  	sltu	$2, $3, $7
  1a246c: 06 00 40 14  	bnez	$2, 0x1a2488 <.text+0xa2488>
  1a2470: ff ff b9 24  	addiu	$25, $5, -0x1 <.text+0xffffffffffefffff>
  1a2474: 2b 10 6b 00  	sltu	$2, $3, $11
  1a2478: 04 00 40 50  	beqzl	$2, 0x1a248c <.text+0xa248c>
  1a247c: 23 18 6b 00  	subu	$3, $3, $11
  1a2480: ff ff 39 27  	addiu	$25, $25, -0x1 <.text+0xffffffffffefffff>
  1a2484: 21 18 67 00  	addu	$3, $3, $7
  1a2488: 23 18 6b 00  	subu	$3, $3, $11
  1a248c: ff ff 04 31  	andi	$4, $8, 0xffff
  1a2490: 1b 00 6a 00  	divu	$zero, $3, $10
  1a2494: 01 00 40 51  	beqzl	$10, 0x1a249c <.text+0xa249c>
  1a2498: cd 01 00 00  	break	0x0, 0x7
  1a249c: 12 10 00 00  	mflo	$2
  1a24a0: 10 18 00 00  	mfhi	$3
  1a24a4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a24a8: 25 28 64 00  	or	$5, $3, $4
  1a24ac: 12 40 00 00  	mflo	$8
  1a24b0: 18 58 50 00  	<unknown>
  1a24b4: 2b 10 ab 00  	sltu	$2, $5, $11
  1a24b8: 0c 00 40 10  	beqz	$2, 0x1a24ec <.text+0xa24ec>
  1a24bc: 00 14 19 00  	sll	$2, $25, 0x10
  1a24c0: 21 28 a7 00  	addu	$5, $5, $7
  1a24c4: 2b 10 a7 00  	sltu	$2, $5, $7
  1a24c8: 07 00 40 14  	bnez	$2, 0x1a24e8 <.text+0xa24e8>
  1a24cc: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a24d0: 2b 10 ab 00  	sltu	$2, $5, $11
  1a24d4: 05 00 40 10  	beqz	$2, 0x1a24ec <.text+0xa24ec>
  1a24d8: 00 14 19 00  	sll	$2, $25, 0x10
  1a24dc: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a24e0: 21 28 a7 00  	addu	$5, $5, $7
  1a24e4: 00 00 00 00  	nop
  1a24e8: 00 14 19 00  	sll	$2, $25, 0x10
  1a24ec: 23 28 ab 00  	subu	$5, $5, $11
  1a24f0: 25 50 48 00  	or	$10, $2, $8
  1a24f4: 19 00 49 01  	multu	$10, $9
  1a24f8: 10 18 00 00  	mfhi	$3
  1a24fc: 12 58 00 00  	mflo	$11
  1a2500: 2b 10 a3 00  	sltu	$2, $5, $3
  1a2504: 1b 00 40 14  	bnez	$2, 0x1a2574 <.text+0xa2574>
  1a2508: 23 20 69 01  	subu	$4, $11, $9
  1a250c: 17 00 65 10  	beq	$3, $5, 0x1a256c <.text+0xa256c>
  1a2510: 2b 10 ab 01  	sltu	$2, $13, $11
  1a2514: e0 fe c0 10  	beqz	$6, 0x1a2098 <.text+0xa2098>
  1a2518: 2d 80 00 00  	move	$16, $zero
  1a251c: 23 20 ab 01  	subu	$4, $13, $11
  1a2520: 23 28 a3 00  	subu	$5, $5, $3
  1a2524: 2b 18 a4 01  	sltu	$3, $13, $4
  1a2528: 06 20 e4 01  	srlv	$4, $4, $15
  1a252c: 23 40 a3 00  	subu	$8, $5, $3
  1a2530: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2534: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2538: 04 10 08 03  	sllv	$2, $8, $24
  1a253c: 24 60 83 01  	and	$12, $12, $3
  1a2540: 25 10 44 00  	or	$2, $2, $4
  1a2544: 06 28 e8 01  	srlv	$5, $8, $15
  1a2548: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a254c: ff ff 03 3c  	lui	$3, 0xffff
  1a2550: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a2554: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2558: 3c 28 05 00  	dsll32	$5, $5, 0x0
  1a255c: 25 60 82 01  	or	$12, $12, $2
  1a2560: 24 60 83 01  	and	$12, $12, $3
  1a2564: cb fe 00 10  	b	0x1a2094 <.text+0xa2094>
  1a2568: 25 60 85 01  	or	$12, $12, $5
  1a256c: e9 ff 40 10  	beqz	$2, 0x1a2514 <.text+0xa2514>
  1a2570: 23 20 69 01  	subu	$4, $11, $9
  1a2574: 23 18 67 00  	subu	$3, $3, $7
  1a2578: 2b 10 64 01  	sltu	$2, $11, $4
  1a257c: ff ff 4a 25  	addiu	$10, $10, -0x1 <.text+0xffffffffffefffff>
  1a2580: 23 18 62 00  	subu	$3, $3, $2
  1a2584: e3 ff 00 10  	b	0x1a2514 <.text+0xa2514>
  1a2588: 2d 58 80 00  	move	$11, $4
  1a258c: 00 00 00 00  	nop
  1a2590: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a2594: ff ff 42 34  	ori	$2, $2, 0xffff
  1a2598: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a259c: 2b 10 47 00  	sltu	$2, $2, $7
  1a25a0: 2d 28 60 00  	move	$5, $3
  1a25a4: 73 ff 00 10  	b	0x1a2374 <.text+0xa2374>
  1a25a8: 0b 28 82 00  	movn	$5, $4, $2
  1a25ac: 00 00 00 00  	nop
  1a25b0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a25b4: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a25b8: 74 89 06 0c  	jal	0x1a25d0 <.text+0xa25d0>
  1a25bc: 2d 30 00 00  	move	$6, $zero
  1a25c0: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a25c4: 08 00 e0 03  	jr	$ra
  1a25c8: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a25cc: 00 00 00 00  	nop
  1a25d0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a25d4: 3f 38 05 00  	dsra32	$7, $5, 0x0
  1a25d8: 3f 40 04 00  	dsra32	$8, $4, 0x0
  1a25dc: 3c 48 05 00  	dsll32	$9, $5, 0x0
  1a25e0: 3f 48 09 00  	dsra32	$9, $9, 0x0
  1a25e4: 3c 68 04 00  	dsll32	$13, $4, 0x0
  1a25e8: 3f 68 0d 00  	dsra32	$13, $13, 0x0
  1a25ec: 08 01 e0 14  	bnez	$7, 0x1a2a10 <.text+0xa2a10>
  1a25f0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a25f4: 2b 10 09 01  	sltu	$2, $8, $9
  1a25f8: 71 00 40 10  	beqz	$2, 0x1a27c0 <.text+0xa27c0>
  1a25fc: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2600: 2b 10 49 00  	sltu	$2, $2, $9
  1a2604: 66 00 40 14  	bnez	$2, 0x1a27a0 <.text+0xa27a0>
  1a2608: ff 00 02 3c  	lui	$2, 0xff
  1a260c: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2610: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a2614: 2d 28 40 00  	move	$5, $2
  1a2618: 0b 28 03 00  	movn	$5, $zero, $3
  1a261c: 1c 00 03 3c  	lui	$3, 0x1c
  1a2620: 06 10 a9 00  	srlv	$2, $9, $5
  1a2624: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2628: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a262c: 21 10 43 00  	addu	$2, $2, $3
  1a2630: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2634: 21 20 85 00  	addu	$4, $4, $5
  1a2638: 23 78 e4 00  	subu	$15, $7, $4
  1a263c: 08 00 e0 51  	beqzl	$15, 0x1a2660 <.text+0xa2660>
  1a2640: 02 3c 09 00  	srl	$7, $9, 0x10
  1a2644: 23 10 ef 00  	subu	$2, $7, $15
  1a2648: 04 18 e8 01  	sllv	$3, $8, $15
  1a264c: 06 10 4d 00  	srlv	$2, $13, $2
  1a2650: 04 48 e9 01  	sllv	$9, $9, $15
  1a2654: 25 40 62 00  	or	$8, $3, $2
  1a2658: 04 68 ed 01  	sllv	$13, $13, $15
  1a265c: 02 3c 09 00  	srl	$7, $9, 0x10
  1a2660: ff ff 2a 31  	andi	$10, $9, 0xffff
  1a2664: 1b 00 07 01  	divu	$zero, $8, $7
  1a2668: 02 24 0d 00  	srl	$4, $13, 0x10
  1a266c: 01 00 e0 50  	beqzl	$7, 0x1a2674 <.text+0xa2674>
  1a2670: cd 01 00 00  	break	0x0, 0x7
  1a2674: 12 10 00 00  	mflo	$2
  1a2678: 10 18 00 00  	mfhi	$3
  1a267c: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2680: 25 18 64 00  	or	$3, $3, $4
  1a2684: 12 28 00 00  	mflo	$5
  1a2688: 18 40 4a 00  	<unknown>
  1a268c: 2b 10 68 00  	sltu	$2, $3, $8
  1a2690: 0c 00 40 50  	beqzl	$2, 0x1a26c4 <.text+0xa26c4>
  1a2694: 23 18 68 00  	subu	$3, $3, $8
  1a2698: 21 18 69 00  	addu	$3, $3, $9
  1a269c: 2b 10 69 00  	sltu	$2, $3, $9
  1a26a0: 07 00 40 14  	bnez	$2, 0x1a26c0 <.text+0xa26c0>
  1a26a4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a26a8: 2b 10 68 00  	sltu	$2, $3, $8
  1a26ac: 05 00 40 50  	beqzl	$2, 0x1a26c4 <.text+0xa26c4>
  1a26b0: 23 18 68 00  	subu	$3, $3, $8
  1a26b4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a26b8: 21 18 69 00  	addu	$3, $3, $9
  1a26bc: 00 00 00 00  	nop
  1a26c0: 23 18 68 00  	subu	$3, $3, $8
  1a26c4: 01 00 e0 50  	beqzl	$7, 0x1a26cc <.text+0xa26cc>
  1a26c8: cd 01 00 00  	break	0x0, 0x7
  1a26cc: 1b 00 67 00  	divu	$zero, $3, $7
  1a26d0: ff ff a4 31  	andi	$4, $13, 0xffff
  1a26d4: 12 10 00 00  	mflo	$2
  1a26d8: 10 18 00 00  	mfhi	$3
  1a26dc: 00 1c 03 00  	sll	$3, $3, 0x10
  1a26e0: 25 18 64 00  	or	$3, $3, $4
  1a26e4: 12 38 00 00  	mflo	$7
  1a26e8: 18 40 4a 00  	<unknown>
  1a26ec: 2b 10 68 00  	sltu	$2, $3, $8
  1a26f0: 0c 00 40 10  	beqz	$2, 0x1a2724 <.text+0xa2724>
  1a26f4: 00 14 05 00  	sll	$2, $5, 0x10
  1a26f8: 21 18 69 00  	addu	$3, $3, $9
  1a26fc: 2b 10 69 00  	sltu	$2, $3, $9
  1a2700: 07 00 40 14  	bnez	$2, 0x1a2720 <.text+0xa2720>
  1a2704: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2708: 2b 10 68 00  	sltu	$2, $3, $8
  1a270c: 05 00 40 10  	beqz	$2, 0x1a2724 <.text+0xa2724>
  1a2710: 00 14 05 00  	sll	$2, $5, 0x10
  1a2714: 21 18 69 00  	addu	$3, $3, $9
  1a2718: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a271c: 00 00 00 00  	nop
  1a2720: 00 14 05 00  	sll	$2, $5, 0x10
  1a2724: 23 68 68 00  	subu	$13, $3, $8
  1a2728: 25 50 47 00  	or	$10, $2, $7
  1a272c: 2d 80 00 00  	move	$16, $zero
  1a2730: 0b 00 c0 10  	beqz	$6, 0x1a2760 <.text+0xa2760>
  1a2734: 06 10 ed 01  	srlv	$2, $13, $15
  1a2738: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a273c: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2740: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a2744: 24 60 83 01  	and	$12, $12, $3
  1a2748: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a274c: ff ff 03 3c  	lui	$3, 0xffff
  1a2750: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a2754: 25 60 82 01  	or	$12, $12, $2
  1a2758: 24 60 83 01  	and	$12, $12, $3
  1a275c: 00 00 cc fc  	sd	$12, 0x0($6)
  1a2760: 3c 10 0a 00  	dsll32	$2, $10, 0x0
  1a2764: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2768: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a276c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2770: 24 70 c3 01  	and	$14, $14, $3
  1a2774: 25 70 c2 01  	or	$14, $14, $2
  1a2778: 3c 18 10 00  	dsll32	$3, $16, 0x0
  1a277c: ff ff 02 3c  	lui	$2, 0xffff
  1a2780: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2784: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a2788: 24 70 c2 01  	and	$14, $14, $2
  1a278c: 25 70 c3 01  	or	$14, $14, $3
  1a2790: 2d 10 c0 01  	move	$2, $14
  1a2794: 08 00 e0 03  	jr	$ra
  1a2798: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a279c: 00 00 00 00  	nop
  1a27a0: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a27a4: ff ff 42 34  	ori	$2, $2, 0xffff
  1a27a8: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a27ac: 2b 10 49 00  	sltu	$2, $2, $9
  1a27b0: 2d 28 60 00  	move	$5, $3
  1a27b4: 99 ff 00 10  	b	0x1a261c <.text+0xa261c>
  1a27b8: 0b 28 82 00  	movn	$5, $4, $2
  1a27bc: 00 00 00 00  	nop
  1a27c0: 08 00 20 15  	bnez	$9, 0x1a27e4 <.text+0xa27e4>
  1a27c4: 2b 10 49 00  	sltu	$2, $2, $9
  1a27c8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a27cc: 01 00 20 51  	beqzl	$9, 0x1a27d4 <.text+0xa27d4>
  1a27d0: cd 01 00 00  	break	0x0, 0x7
  1a27d4: 1b 00 47 00  	divu	$zero, $2, $7
  1a27d8: 12 48 00 00  	mflo	$9
  1a27dc: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a27e0: 2b 10 49 00  	sltu	$2, $2, $9
  1a27e4: 82 00 40 14  	bnez	$2, 0x1a29f0 <.text+0xa29f0>
  1a27e8: ff 00 02 3c  	lui	$2, 0xff
  1a27ec: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a27f0: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a27f4: 2d 28 40 00  	move	$5, $2
  1a27f8: 0b 28 03 00  	movn	$5, $zero, $3
  1a27fc: 1c 00 03 3c  	lui	$3, 0x1c
  1a2800: 06 10 a9 00  	srlv	$2, $9, $5
  1a2804: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2808: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a280c: 21 10 43 00  	addu	$2, $2, $3
  1a2810: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2814: 21 20 85 00  	addu	$4, $4, $5
  1a2818: 23 78 e4 00  	subu	$15, $7, $4
  1a281c: 38 00 e0 15  	bnez	$15, 0x1a2900 <.text+0xa2900>
  1a2820: 23 c0 ef 00  	subu	$24, $7, $15
  1a2824: 23 40 09 01  	subu	$8, $8, $9
  1a2828: 01 00 10 24  	addiu	$16, $zero, 0x1
  1a282c: 02 54 09 00  	srl	$10, $9, 0x10
  1a2830: ff ff 39 31  	andi	$25, $9, 0xffff
  1a2834: 1b 00 0a 01  	divu	$zero, $8, $10
  1a2838: 02 2c 0d 00  	srl	$5, $13, 0x10
  1a283c: 01 00 40 51  	beqzl	$10, 0x1a2844 <.text+0xa2844>
  1a2840: cd 01 00 00  	break	0x0, 0x7
  1a2844: 12 20 00 00  	mflo	$4
  1a2848: 10 18 00 00  	mfhi	$3
  1a284c: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2850: 25 18 65 00  	or	$3, $3, $5
  1a2854: 12 58 00 00  	mflo	$11
  1a2858: 18 38 99 00  	<unknown>
  1a285c: 2b 10 67 00  	sltu	$2, $3, $7
  1a2860: 0c 00 40 50  	beqzl	$2, 0x1a2894 <.text+0xa2894>
  1a2864: 23 18 67 00  	subu	$3, $3, $7
  1a2868: 21 18 69 00  	addu	$3, $3, $9
  1a286c: 2b 10 69 00  	sltu	$2, $3, $9
  1a2870: 07 00 40 14  	bnez	$2, 0x1a2890 <.text+0xa2890>
  1a2874: ff ff 8b 24  	addiu	$11, $4, -0x1 <.text+0xffffffffffefffff>
  1a2878: 2b 10 67 00  	sltu	$2, $3, $7
  1a287c: 05 00 40 50  	beqzl	$2, 0x1a2894 <.text+0xa2894>
  1a2880: 23 18 67 00  	subu	$3, $3, $7
  1a2884: ff ff 6b 25  	addiu	$11, $11, -0x1 <.text+0xffffffffffefffff>
  1a2888: 21 18 69 00  	addu	$3, $3, $9
  1a288c: 00 00 00 00  	nop
  1a2890: 23 18 67 00  	subu	$3, $3, $7
  1a2894: ff ff a4 31  	andi	$4, $13, 0xffff
  1a2898: 1b 00 6a 00  	divu	$zero, $3, $10
  1a289c: 01 00 40 51  	beqzl	$10, 0x1a28a4 <.text+0xa28a4>
  1a28a0: cd 01 00 00  	break	0x0, 0x7
  1a28a4: 12 10 00 00  	mflo	$2
  1a28a8: 10 18 00 00  	mfhi	$3
  1a28ac: 00 1c 03 00  	sll	$3, $3, 0x10
  1a28b0: 25 18 64 00  	or	$3, $3, $4
  1a28b4: 12 40 00 00  	mflo	$8
  1a28b8: 18 38 59 00  	<unknown>
  1a28bc: 2b 10 67 00  	sltu	$2, $3, $7
  1a28c0: 0c 00 40 10  	beqz	$2, 0x1a28f4 <.text+0xa28f4>
  1a28c4: 00 14 0b 00  	sll	$2, $11, 0x10
  1a28c8: 21 18 69 00  	addu	$3, $3, $9
  1a28cc: 2b 10 69 00  	sltu	$2, $3, $9
  1a28d0: 07 00 40 14  	bnez	$2, 0x1a28f0 <.text+0xa28f0>
  1a28d4: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a28d8: 2b 10 67 00  	sltu	$2, $3, $7
  1a28dc: 05 00 40 10  	beqz	$2, 0x1a28f4 <.text+0xa28f4>
  1a28e0: 00 14 0b 00  	sll	$2, $11, 0x10
  1a28e4: 21 18 69 00  	addu	$3, $3, $9
  1a28e8: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a28ec: 00 00 00 00  	nop
  1a28f0: 00 14 0b 00  	sll	$2, $11, 0x10
  1a28f4: 23 68 67 00  	subu	$13, $3, $7
  1a28f8: 8d ff 00 10  	b	0x1a2730 <.text+0xa2730>
  1a28fc: 25 50 48 00  	or	$10, $2, $8
  1a2900: 04 48 e9 01  	sllv	$9, $9, $15
  1a2904: 06 28 08 03  	srlv	$5, $8, $24
  1a2908: 02 54 09 00  	srl	$10, $9, 0x10
  1a290c: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2910: ff ff 39 31  	andi	$25, $9, 0xffff
  1a2914: 06 18 0d 03  	srlv	$3, $13, $24
  1a2918: 04 10 e8 01  	sllv	$2, $8, $15
  1a291c: 25 40 43 00  	or	$8, $2, $3
  1a2920: 01 00 40 51  	beqzl	$10, 0x1a2928 <.text+0xa2928>
  1a2924: cd 01 00 00  	break	0x0, 0x7
  1a2928: 02 1c 08 00  	srl	$3, $8, 0x10
  1a292c: 2d 58 40 01  	move	$11, $10
  1a2930: 04 68 ed 01  	sllv	$13, $13, $15
  1a2934: 12 28 00 00  	mflo	$5
  1a2938: 10 20 00 00  	mfhi	$4
  1a293c: 00 24 04 00  	sll	$4, $4, 0x10
  1a2940: 25 18 83 00  	or	$3, $4, $3
  1a2944: 12 c0 00 00  	mflo	$24
  1a2948: 18 38 b9 00  	<unknown>
  1a294c: 2b 10 67 00  	sltu	$2, $3, $7
  1a2950: 0b 00 40 10  	beqz	$2, 0x1a2980 <.text+0xa2980>
  1a2954: 2d 80 20 03  	move	$16, $25
  1a2958: 21 18 69 00  	addu	$3, $3, $9
  1a295c: 2b 10 69 00  	sltu	$2, $3, $9
  1a2960: 07 00 40 14  	bnez	$2, 0x1a2980 <.text+0xa2980>
  1a2964: ff ff b8 24  	addiu	$24, $5, -0x1 <.text+0xffffffffffefffff>
  1a2968: 2b 10 67 00  	sltu	$2, $3, $7
  1a296c: 05 00 40 50  	beqzl	$2, 0x1a2984 <.text+0xa2984>
  1a2970: 23 18 67 00  	subu	$3, $3, $7
  1a2974: ff ff 18 27  	addiu	$24, $24, -0x1 <.text+0xffffffffffefffff>
  1a2978: 21 18 69 00  	addu	$3, $3, $9
  1a297c: 00 00 00 00  	nop
  1a2980: 23 18 67 00  	subu	$3, $3, $7
  1a2984: ff ff 04 31  	andi	$4, $8, 0xffff
  1a2988: 1b 00 6b 00  	divu	$zero, $3, $11
  1a298c: 01 00 60 51  	beqzl	$11, 0x1a2994 <.text+0xa2994>
  1a2990: cd 01 00 00  	break	0x0, 0x7
  1a2994: 12 10 00 00  	mflo	$2
  1a2998: 10 18 00 00  	mfhi	$3
  1a299c: 00 1c 03 00  	sll	$3, $3, 0x10
  1a29a0: 25 18 64 00  	or	$3, $3, $4
  1a29a4: 12 28 00 00  	mflo	$5
  1a29a8: 18 38 50 00  	<unknown>
  1a29ac: 2b 10 67 00  	sltu	$2, $3, $7
  1a29b0: 0c 00 40 10  	beqz	$2, 0x1a29e4 <.text+0xa29e4>
  1a29b4: 00 14 18 00  	sll	$2, $24, 0x10
  1a29b8: 21 18 69 00  	addu	$3, $3, $9
  1a29bc: 2b 10 69 00  	sltu	$2, $3, $9
  1a29c0: 07 00 40 14  	bnez	$2, 0x1a29e0 <.text+0xa29e0>
  1a29c4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a29c8: 2b 10 67 00  	sltu	$2, $3, $7
  1a29cc: 05 00 40 10  	beqz	$2, 0x1a29e4 <.text+0xa29e4>
  1a29d0: 00 14 18 00  	sll	$2, $24, 0x10
  1a29d4: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a29d8: 21 18 69 00  	addu	$3, $3, $9
  1a29dc: 00 00 00 00  	nop
  1a29e0: 00 14 18 00  	sll	$2, $24, 0x10
  1a29e4: 23 40 67 00  	subu	$8, $3, $7
  1a29e8: 92 ff 00 10  	b	0x1a2834 <.text+0xa2834>
  1a29ec: 25 80 45 00  	or	$16, $2, $5
  1a29f0: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a29f4: ff ff 42 34  	ori	$2, $2, 0xffff
  1a29f8: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a29fc: 2b 10 49 00  	sltu	$2, $2, $9
  1a2a00: 2d 28 60 00  	move	$5, $3
  1a2a04: 7d ff 00 10  	b	0x1a27fc <.text+0xa27fc>
  1a2a08: 0b 28 82 00  	movn	$5, $4, $2
  1a2a0c: 00 00 00 00  	nop
  1a2a10: 2b 10 07 01  	sltu	$2, $8, $7
  1a2a14: 1f 00 40 14  	bnez	$2, 0x1a2a94 <.text+0xa2a94>
  1a2a18: 2d 50 00 00  	move	$10, $zero
  1a2a1c: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2a20: 2b 10 47 00  	sltu	$2, $2, $7
  1a2a24: 8c 00 40 14  	bnez	$2, 0x1a2c58 <.text+0xa2c58>
  1a2a28: ff 00 02 3c  	lui	$2, 0xff
  1a2a2c: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2a30: 00 01 e3 2c  	sltiu	$3, $7, 0x100
  1a2a34: 2d 28 40 00  	move	$5, $2
  1a2a38: 0b 28 03 00  	movn	$5, $zero, $3
  1a2a3c: 1c 00 03 3c  	lui	$3, 0x1c
  1a2a40: 06 10 a7 00  	srlv	$2, $7, $5
  1a2a44: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2a48: 20 00 0a 24  	addiu	$10, $zero, 0x20
  1a2a4c: 21 10 43 00  	addu	$2, $2, $3
  1a2a50: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2a54: 21 20 85 00  	addu	$4, $4, $5
  1a2a58: 23 78 44 01  	subu	$15, $10, $4
  1a2a5c: 1c 00 e0 15  	bnez	$15, 0x1a2ad0 <.text+0xa2ad0>
  1a2a60: 23 c0 4f 01  	subu	$24, $10, $15
  1a2a64: 2b 10 e8 00  	sltu	$2, $7, $8
  1a2a68: 05 00 40 14  	bnez	$2, 0x1a2a80 <.text+0xa2a80>
  1a2a6c: 23 20 a9 01  	subu	$4, $13, $9
  1a2a70: 2b 10 a9 01  	sltu	$2, $13, $9
  1a2a74: 07 00 40 14  	bnez	$2, 0x1a2a94 <.text+0xa2a94>
  1a2a78: 2d 50 00 00  	move	$10, $zero
  1a2a7c: 23 20 a9 01  	subu	$4, $13, $9
  1a2a80: 23 18 07 01  	subu	$3, $8, $7
  1a2a84: 2b 10 a4 01  	sltu	$2, $13, $4
  1a2a88: 01 00 0a 24  	addiu	$10, $zero, 0x1
  1a2a8c: 23 40 62 00  	subu	$8, $3, $2
  1a2a90: 2d 68 80 00  	move	$13, $4
  1a2a94: 32 ff c0 10  	beqz	$6, 0x1a2760 <.text+0xa2760>
  1a2a98: 2d 80 00 00  	move	$16, $zero
  1a2a9c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2aa0: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2aa4: 3c 10 0d 00  	dsll32	$2, $13, 0x0
  1a2aa8: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2aac: 24 60 83 01  	and	$12, $12, $3
  1a2ab0: 25 60 82 01  	or	$12, $12, $2
  1a2ab4: 3c 18 08 00  	dsll32	$3, $8, 0x0
  1a2ab8: ff ff 02 3c  	lui	$2, 0xffff
  1a2abc: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2ac0: 24 60 82 01  	and	$12, $12, $2
  1a2ac4: 25 ff 00 10  	b	0x1a275c <.text+0xa275c>
  1a2ac8: 25 60 83 01  	or	$12, $12, $3
  1a2acc: 00 00 00 00  	nop
  1a2ad0: 04 18 e7 01  	sllv	$3, $7, $15
  1a2ad4: 06 10 09 03  	srlv	$2, $9, $24
  1a2ad8: 06 28 08 03  	srlv	$5, $8, $24
  1a2adc: 25 38 62 00  	or	$7, $3, $2
  1a2ae0: 04 20 e8 01  	sllv	$4, $8, $15
  1a2ae4: 02 54 07 00  	srl	$10, $7, 0x10
  1a2ae8: ff ff f0 30  	andi	$16, $7, 0xffff
  1a2aec: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2af0: 06 10 0d 03  	srlv	$2, $13, $24
  1a2af4: 25 40 82 00  	or	$8, $4, $2
  1a2af8: 01 00 40 51  	beqzl	$10, 0x1a2b00 <.text+0xa2b00>
  1a2afc: cd 01 00 00  	break	0x0, 0x7
  1a2b00: 02 24 08 00  	srl	$4, $8, 0x10
  1a2b04: 04 48 e9 01  	sllv	$9, $9, $15
  1a2b08: 12 28 00 00  	mflo	$5
  1a2b0c: 10 18 00 00  	mfhi	$3
  1a2b10: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2b14: 25 18 64 00  	or	$3, $3, $4
  1a2b18: 12 c8 00 00  	mflo	$25
  1a2b1c: 18 58 b0 00  	<unknown>
  1a2b20: 2b 10 6b 00  	sltu	$2, $3, $11
  1a2b24: 0a 00 40 10  	beqz	$2, 0x1a2b50 <.text+0xa2b50>
  1a2b28: 04 68 ed 01  	sllv	$13, $13, $15
  1a2b2c: 21 18 67 00  	addu	$3, $3, $7
  1a2b30: 2b 10 67 00  	sltu	$2, $3, $7
  1a2b34: 06 00 40 14  	bnez	$2, 0x1a2b50 <.text+0xa2b50>
  1a2b38: ff ff b9 24  	addiu	$25, $5, -0x1 <.text+0xffffffffffefffff>
  1a2b3c: 2b 10 6b 00  	sltu	$2, $3, $11
  1a2b40: 04 00 40 50  	beqzl	$2, 0x1a2b54 <.text+0xa2b54>
  1a2b44: 23 18 6b 00  	subu	$3, $3, $11
  1a2b48: ff ff 39 27  	addiu	$25, $25, -0x1 <.text+0xffffffffffefffff>
  1a2b4c: 21 18 67 00  	addu	$3, $3, $7
  1a2b50: 23 18 6b 00  	subu	$3, $3, $11
  1a2b54: ff ff 04 31  	andi	$4, $8, 0xffff
  1a2b58: 1b 00 6a 00  	divu	$zero, $3, $10
  1a2b5c: 01 00 40 51  	beqzl	$10, 0x1a2b64 <.text+0xa2b64>
  1a2b60: cd 01 00 00  	break	0x0, 0x7
  1a2b64: 12 10 00 00  	mflo	$2
  1a2b68: 10 18 00 00  	mfhi	$3
  1a2b6c: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2b70: 25 28 64 00  	or	$5, $3, $4
  1a2b74: 12 40 00 00  	mflo	$8
  1a2b78: 18 58 50 00  	<unknown>
  1a2b7c: 2b 10 ab 00  	sltu	$2, $5, $11
  1a2b80: 0c 00 40 10  	beqz	$2, 0x1a2bb4 <.text+0xa2bb4>
  1a2b84: 00 14 19 00  	sll	$2, $25, 0x10
  1a2b88: 21 28 a7 00  	addu	$5, $5, $7
  1a2b8c: 2b 10 a7 00  	sltu	$2, $5, $7
  1a2b90: 07 00 40 14  	bnez	$2, 0x1a2bb0 <.text+0xa2bb0>
  1a2b94: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2b98: 2b 10 ab 00  	sltu	$2, $5, $11
  1a2b9c: 05 00 40 10  	beqz	$2, 0x1a2bb4 <.text+0xa2bb4>
  1a2ba0: 00 14 19 00  	sll	$2, $25, 0x10
  1a2ba4: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2ba8: 21 28 a7 00  	addu	$5, $5, $7
  1a2bac: 00 00 00 00  	nop
  1a2bb0: 00 14 19 00  	sll	$2, $25, 0x10
  1a2bb4: 23 28 ab 00  	subu	$5, $5, $11
  1a2bb8: 25 50 48 00  	or	$10, $2, $8
  1a2bbc: 19 00 49 01  	multu	$10, $9
  1a2bc0: 10 18 00 00  	mfhi	$3
  1a2bc4: 12 58 00 00  	mflo	$11
  1a2bc8: 2b 10 a3 00  	sltu	$2, $5, $3
  1a2bcc: 1b 00 40 14  	bnez	$2, 0x1a2c3c <.text+0xa2c3c>
  1a2bd0: 23 20 69 01  	subu	$4, $11, $9
  1a2bd4: 17 00 65 10  	beq	$3, $5, 0x1a2c34 <.text+0xa2c34>
  1a2bd8: 2b 10 ab 01  	sltu	$2, $13, $11
  1a2bdc: e0 fe c0 10  	beqz	$6, 0x1a2760 <.text+0xa2760>
  1a2be0: 2d 80 00 00  	move	$16, $zero
  1a2be4: 23 20 ab 01  	subu	$4, $13, $11
  1a2be8: 23 28 a3 00  	subu	$5, $5, $3
  1a2bec: 2b 18 a4 01  	sltu	$3, $13, $4
  1a2bf0: 06 20 e4 01  	srlv	$4, $4, $15
  1a2bf4: 23 40 a3 00  	subu	$8, $5, $3
  1a2bf8: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2bfc: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2c00: 04 10 08 03  	sllv	$2, $8, $24
  1a2c04: 24 60 83 01  	and	$12, $12, $3
  1a2c08: 25 10 44 00  	or	$2, $2, $4
  1a2c0c: 06 28 e8 01  	srlv	$5, $8, $15
  1a2c10: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a2c14: ff ff 03 3c  	lui	$3, 0xffff
  1a2c18: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a2c1c: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2c20: 3c 28 05 00  	dsll32	$5, $5, 0x0
  1a2c24: 25 60 82 01  	or	$12, $12, $2
  1a2c28: 24 60 83 01  	and	$12, $12, $3
  1a2c2c: cb fe 00 10  	b	0x1a275c <.text+0xa275c>
  1a2c30: 25 60 85 01  	or	$12, $12, $5
  1a2c34: e9 ff 40 10  	beqz	$2, 0x1a2bdc <.text+0xa2bdc>
  1a2c38: 23 20 69 01  	subu	$4, $11, $9
  1a2c3c: 23 18 67 00  	subu	$3, $3, $7
  1a2c40: 2b 10 64 01  	sltu	$2, $11, $4
  1a2c44: ff ff 4a 25  	addiu	$10, $10, -0x1 <.text+0xffffffffffefffff>
  1a2c48: 23 18 62 00  	subu	$3, $3, $2
  1a2c4c: e3 ff 00 10  	b	0x1a2bdc <.text+0xa2bdc>
  1a2c50: 2d 58 80 00  	move	$11, $4
  1a2c54: 00 00 00 00  	nop
  1a2c58: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a2c5c: ff ff 42 34  	ori	$2, $2, 0xffff
  1a2c60: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a2c64: 2b 10 47 00  	sltu	$2, $2, $7
  1a2c68: 2d 28 60 00  	move	$5, $3
  1a2c6c: 73 ff 00 10  	b	0x1a2a3c <.text+0xa2a3c>
  1a2c70: 0b 28 82 00  	movn	$5, $4, $2
  1a2c74: 00 00 00 00  	nop
  1a2c78: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  1a2c7c: 10 00 bf ff  	sd	$ra, 0x10($sp)
  1a2c80: 26 8b 06 0c  	jal	0x1a2c98 <.text+0xa2c98>
  1a2c84: 2d 30 a0 03  	move	$6, $sp
  1a2c88: 10 00 bf df  	ld	$ra, 0x10($sp)
  1a2c8c: 00 00 a2 df  	ld	$2, 0x0($sp)
  1a2c90: 08 00 e0 03  	jr	$ra
  1a2c94: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  1a2c98: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a2c9c: 3f 38 05 00  	dsra32	$7, $5, 0x0
  1a2ca0: 3f 40 04 00  	dsra32	$8, $4, 0x0
  1a2ca4: 3c 48 05 00  	dsll32	$9, $5, 0x0
  1a2ca8: 3f 48 09 00  	dsra32	$9, $9, 0x0
  1a2cac: 3c 68 04 00  	dsll32	$13, $4, 0x0
  1a2cb0: 3f 68 0d 00  	dsra32	$13, $13, 0x0
  1a2cb4: 08 01 e0 14  	bnez	$7, 0x1a30d8 <.text+0xa30d8>
  1a2cb8: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1a2cbc: 2b 10 09 01  	sltu	$2, $8, $9
  1a2cc0: 71 00 40 10  	beqz	$2, 0x1a2e88 <.text+0xa2e88>
  1a2cc4: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2cc8: 2b 10 49 00  	sltu	$2, $2, $9
  1a2ccc: 66 00 40 14  	bnez	$2, 0x1a2e68 <.text+0xa2e68>
  1a2cd0: ff 00 02 3c  	lui	$2, 0xff
  1a2cd4: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2cd8: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a2cdc: 2d 28 40 00  	move	$5, $2
  1a2ce0: 0b 28 03 00  	movn	$5, $zero, $3
  1a2ce4: 1c 00 03 3c  	lui	$3, 0x1c
  1a2ce8: 06 10 a9 00  	srlv	$2, $9, $5
  1a2cec: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2cf0: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a2cf4: 21 10 43 00  	addu	$2, $2, $3
  1a2cf8: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2cfc: 21 20 85 00  	addu	$4, $4, $5
  1a2d00: 23 78 e4 00  	subu	$15, $7, $4
  1a2d04: 08 00 e0 51  	beqzl	$15, 0x1a2d28 <.text+0xa2d28>
  1a2d08: 02 3c 09 00  	srl	$7, $9, 0x10
  1a2d0c: 23 10 ef 00  	subu	$2, $7, $15
  1a2d10: 04 18 e8 01  	sllv	$3, $8, $15
  1a2d14: 06 10 4d 00  	srlv	$2, $13, $2
  1a2d18: 04 48 e9 01  	sllv	$9, $9, $15
  1a2d1c: 25 40 62 00  	or	$8, $3, $2
  1a2d20: 04 68 ed 01  	sllv	$13, $13, $15
  1a2d24: 02 3c 09 00  	srl	$7, $9, 0x10
  1a2d28: ff ff 2a 31  	andi	$10, $9, 0xffff
  1a2d2c: 1b 00 07 01  	divu	$zero, $8, $7
  1a2d30: 02 24 0d 00  	srl	$4, $13, 0x10
  1a2d34: 01 00 e0 50  	beqzl	$7, 0x1a2d3c <.text+0xa2d3c>
  1a2d38: cd 01 00 00  	break	0x0, 0x7
  1a2d3c: 12 10 00 00  	mflo	$2
  1a2d40: 10 18 00 00  	mfhi	$3
  1a2d44: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2d48: 25 18 64 00  	or	$3, $3, $4
  1a2d4c: 12 28 00 00  	mflo	$5
  1a2d50: 18 40 4a 00  	<unknown>
  1a2d54: 2b 10 68 00  	sltu	$2, $3, $8
  1a2d58: 0c 00 40 50  	beqzl	$2, 0x1a2d8c <.text+0xa2d8c>
  1a2d5c: 23 18 68 00  	subu	$3, $3, $8
  1a2d60: 21 18 69 00  	addu	$3, $3, $9
  1a2d64: 2b 10 69 00  	sltu	$2, $3, $9
  1a2d68: 07 00 40 14  	bnez	$2, 0x1a2d88 <.text+0xa2d88>
  1a2d6c: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a2d70: 2b 10 68 00  	sltu	$2, $3, $8
  1a2d74: 05 00 40 50  	beqzl	$2, 0x1a2d8c <.text+0xa2d8c>
  1a2d78: 23 18 68 00  	subu	$3, $3, $8
  1a2d7c: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a2d80: 21 18 69 00  	addu	$3, $3, $9
  1a2d84: 00 00 00 00  	nop
  1a2d88: 23 18 68 00  	subu	$3, $3, $8
  1a2d8c: 01 00 e0 50  	beqzl	$7, 0x1a2d94 <.text+0xa2d94>
  1a2d90: cd 01 00 00  	break	0x0, 0x7
  1a2d94: 1b 00 67 00  	divu	$zero, $3, $7
  1a2d98: ff ff a4 31  	andi	$4, $13, 0xffff
  1a2d9c: 12 10 00 00  	mflo	$2
  1a2da0: 10 18 00 00  	mfhi	$3
  1a2da4: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2da8: 25 18 64 00  	or	$3, $3, $4
  1a2dac: 12 38 00 00  	mflo	$7
  1a2db0: 18 40 4a 00  	<unknown>
  1a2db4: 2b 10 68 00  	sltu	$2, $3, $8
  1a2db8: 0c 00 40 10  	beqz	$2, 0x1a2dec <.text+0xa2dec>
  1a2dbc: 00 14 05 00  	sll	$2, $5, 0x10
  1a2dc0: 21 18 69 00  	addu	$3, $3, $9
  1a2dc4: 2b 10 69 00  	sltu	$2, $3, $9
  1a2dc8: 07 00 40 14  	bnez	$2, 0x1a2de8 <.text+0xa2de8>
  1a2dcc: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2dd0: 2b 10 68 00  	sltu	$2, $3, $8
  1a2dd4: 05 00 40 10  	beqz	$2, 0x1a2dec <.text+0xa2dec>
  1a2dd8: 00 14 05 00  	sll	$2, $5, 0x10
  1a2ddc: 21 18 69 00  	addu	$3, $3, $9
  1a2de0: ff ff e7 24  	addiu	$7, $7, -0x1 <.text+0xffffffffffefffff>
  1a2de4: 00 00 00 00  	nop
  1a2de8: 00 14 05 00  	sll	$2, $5, 0x10
  1a2dec: 23 68 68 00  	subu	$13, $3, $8
  1a2df0: 25 50 47 00  	or	$10, $2, $7
  1a2df4: 2d 80 00 00  	move	$16, $zero
  1a2df8: 0b 00 c0 10  	beqz	$6, 0x1a2e28 <.text+0xa2e28>
  1a2dfc: 06 10 ed 01  	srlv	$2, $13, $15
  1a2e00: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2e04: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2e08: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a2e0c: 24 60 83 01  	and	$12, $12, $3
  1a2e10: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2e14: ff ff 03 3c  	lui	$3, 0xffff
  1a2e18: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a2e1c: 25 60 82 01  	or	$12, $12, $2
  1a2e20: 24 60 83 01  	and	$12, $12, $3
  1a2e24: 00 00 cc fc  	sd	$12, 0x0($6)
  1a2e28: 3c 10 0a 00  	dsll32	$2, $10, 0x0
  1a2e2c: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a2e30: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a2e34: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2e38: 24 70 c3 01  	and	$14, $14, $3
  1a2e3c: 25 70 c2 01  	or	$14, $14, $2
  1a2e40: 3c 18 10 00  	dsll32	$3, $16, 0x0
  1a2e44: ff ff 02 3c  	lui	$2, 0xffff
  1a2e48: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a2e4c: 00 00 b0 df  	ld	$16, 0x0($sp)
  1a2e50: 24 70 c2 01  	and	$14, $14, $2
  1a2e54: 25 70 c3 01  	or	$14, $14, $3
  1a2e58: 2d 10 c0 01  	move	$2, $14
  1a2e5c: 08 00 e0 03  	jr	$ra
  1a2e60: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a2e64: 00 00 00 00  	nop
  1a2e68: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a2e6c: ff ff 42 34  	ori	$2, $2, 0xffff
  1a2e70: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a2e74: 2b 10 49 00  	sltu	$2, $2, $9
  1a2e78: 2d 28 60 00  	move	$5, $3
  1a2e7c: 99 ff 00 10  	b	0x1a2ce4 <.text+0xa2ce4>
  1a2e80: 0b 28 82 00  	movn	$5, $4, $2
  1a2e84: 00 00 00 00  	nop
  1a2e88: 08 00 20 15  	bnez	$9, 0x1a2eac <.text+0xa2eac>
  1a2e8c: 2b 10 49 00  	sltu	$2, $2, $9
  1a2e90: 01 00 02 24  	addiu	$2, $zero, 0x1
  1a2e94: 01 00 20 51  	beqzl	$9, 0x1a2e9c <.text+0xa2e9c>
  1a2e98: cd 01 00 00  	break	0x0, 0x7
  1a2e9c: 1b 00 47 00  	divu	$zero, $2, $7
  1a2ea0: 12 48 00 00  	mflo	$9
  1a2ea4: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a2ea8: 2b 10 49 00  	sltu	$2, $2, $9
  1a2eac: 82 00 40 14  	bnez	$2, 0x1a30b8 <.text+0xa30b8>
  1a2eb0: ff 00 02 3c  	lui	$2, 0xff
  1a2eb4: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a2eb8: 00 01 23 2d  	sltiu	$3, $9, 0x100
  1a2ebc: 2d 28 40 00  	move	$5, $2
  1a2ec0: 0b 28 03 00  	movn	$5, $zero, $3
  1a2ec4: 1c 00 03 3c  	lui	$3, 0x1c
  1a2ec8: 06 10 a9 00  	srlv	$2, $9, $5
  1a2ecc: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a2ed0: 20 00 07 24  	addiu	$7, $zero, 0x20
  1a2ed4: 21 10 43 00  	addu	$2, $2, $3
  1a2ed8: 00 00 44 90  	lbu	$4, 0x0($2)
  1a2edc: 21 20 85 00  	addu	$4, $4, $5
  1a2ee0: 23 78 e4 00  	subu	$15, $7, $4
  1a2ee4: 38 00 e0 15  	bnez	$15, 0x1a2fc8 <.text+0xa2fc8>
  1a2ee8: 23 c0 ef 00  	subu	$24, $7, $15
  1a2eec: 23 40 09 01  	subu	$8, $8, $9
  1a2ef0: 01 00 10 24  	addiu	$16, $zero, 0x1
  1a2ef4: 02 54 09 00  	srl	$10, $9, 0x10
  1a2ef8: ff ff 39 31  	andi	$25, $9, 0xffff
  1a2efc: 1b 00 0a 01  	divu	$zero, $8, $10
  1a2f00: 02 2c 0d 00  	srl	$5, $13, 0x10
  1a2f04: 01 00 40 51  	beqzl	$10, 0x1a2f0c <.text+0xa2f0c>
  1a2f08: cd 01 00 00  	break	0x0, 0x7
  1a2f0c: 12 20 00 00  	mflo	$4
  1a2f10: 10 18 00 00  	mfhi	$3
  1a2f14: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2f18: 25 18 65 00  	or	$3, $3, $5
  1a2f1c: 12 58 00 00  	mflo	$11
  1a2f20: 18 38 99 00  	<unknown>
  1a2f24: 2b 10 67 00  	sltu	$2, $3, $7
  1a2f28: 0c 00 40 50  	beqzl	$2, 0x1a2f5c <.text+0xa2f5c>
  1a2f2c: 23 18 67 00  	subu	$3, $3, $7
  1a2f30: 21 18 69 00  	addu	$3, $3, $9
  1a2f34: 2b 10 69 00  	sltu	$2, $3, $9
  1a2f38: 07 00 40 14  	bnez	$2, 0x1a2f58 <.text+0xa2f58>
  1a2f3c: ff ff 8b 24  	addiu	$11, $4, -0x1 <.text+0xffffffffffefffff>
  1a2f40: 2b 10 67 00  	sltu	$2, $3, $7
  1a2f44: 05 00 40 50  	beqzl	$2, 0x1a2f5c <.text+0xa2f5c>
  1a2f48: 23 18 67 00  	subu	$3, $3, $7
  1a2f4c: ff ff 6b 25  	addiu	$11, $11, -0x1 <.text+0xffffffffffefffff>
  1a2f50: 21 18 69 00  	addu	$3, $3, $9
  1a2f54: 00 00 00 00  	nop
  1a2f58: 23 18 67 00  	subu	$3, $3, $7
  1a2f5c: ff ff a4 31  	andi	$4, $13, 0xffff
  1a2f60: 1b 00 6a 00  	divu	$zero, $3, $10
  1a2f64: 01 00 40 51  	beqzl	$10, 0x1a2f6c <.text+0xa2f6c>
  1a2f68: cd 01 00 00  	break	0x0, 0x7
  1a2f6c: 12 10 00 00  	mflo	$2
  1a2f70: 10 18 00 00  	mfhi	$3
  1a2f74: 00 1c 03 00  	sll	$3, $3, 0x10
  1a2f78: 25 18 64 00  	or	$3, $3, $4
  1a2f7c: 12 40 00 00  	mflo	$8
  1a2f80: 18 38 59 00  	<unknown>
  1a2f84: 2b 10 67 00  	sltu	$2, $3, $7
  1a2f88: 0c 00 40 10  	beqz	$2, 0x1a2fbc <.text+0xa2fbc>
  1a2f8c: 00 14 0b 00  	sll	$2, $11, 0x10
  1a2f90: 21 18 69 00  	addu	$3, $3, $9
  1a2f94: 2b 10 69 00  	sltu	$2, $3, $9
  1a2f98: 07 00 40 14  	bnez	$2, 0x1a2fb8 <.text+0xa2fb8>
  1a2f9c: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2fa0: 2b 10 67 00  	sltu	$2, $3, $7
  1a2fa4: 05 00 40 10  	beqz	$2, 0x1a2fbc <.text+0xa2fbc>
  1a2fa8: 00 14 0b 00  	sll	$2, $11, 0x10
  1a2fac: 21 18 69 00  	addu	$3, $3, $9
  1a2fb0: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a2fb4: 00 00 00 00  	nop
  1a2fb8: 00 14 0b 00  	sll	$2, $11, 0x10
  1a2fbc: 23 68 67 00  	subu	$13, $3, $7
  1a2fc0: 8d ff 00 10  	b	0x1a2df8 <.text+0xa2df8>
  1a2fc4: 25 50 48 00  	or	$10, $2, $8
  1a2fc8: 04 48 e9 01  	sllv	$9, $9, $15
  1a2fcc: 06 28 08 03  	srlv	$5, $8, $24
  1a2fd0: 02 54 09 00  	srl	$10, $9, 0x10
  1a2fd4: 1b 00 aa 00  	divu	$zero, $5, $10
  1a2fd8: ff ff 39 31  	andi	$25, $9, 0xffff
  1a2fdc: 06 18 0d 03  	srlv	$3, $13, $24
  1a2fe0: 04 10 e8 01  	sllv	$2, $8, $15
  1a2fe4: 25 40 43 00  	or	$8, $2, $3
  1a2fe8: 01 00 40 51  	beqzl	$10, 0x1a2ff0 <.text+0xa2ff0>
  1a2fec: cd 01 00 00  	break	0x0, 0x7
  1a2ff0: 02 1c 08 00  	srl	$3, $8, 0x10
  1a2ff4: 2d 58 40 01  	move	$11, $10
  1a2ff8: 04 68 ed 01  	sllv	$13, $13, $15
  1a2ffc: 12 28 00 00  	mflo	$5
  1a3000: 10 20 00 00  	mfhi	$4
  1a3004: 00 24 04 00  	sll	$4, $4, 0x10
  1a3008: 25 18 83 00  	or	$3, $4, $3
  1a300c: 12 c0 00 00  	mflo	$24
  1a3010: 18 38 b9 00  	<unknown>
  1a3014: 2b 10 67 00  	sltu	$2, $3, $7
  1a3018: 0b 00 40 10  	beqz	$2, 0x1a3048 <.text+0xa3048>
  1a301c: 2d 80 20 03  	move	$16, $25
  1a3020: 21 18 69 00  	addu	$3, $3, $9
  1a3024: 2b 10 69 00  	sltu	$2, $3, $9
  1a3028: 07 00 40 14  	bnez	$2, 0x1a3048 <.text+0xa3048>
  1a302c: ff ff b8 24  	addiu	$24, $5, -0x1 <.text+0xffffffffffefffff>
  1a3030: 2b 10 67 00  	sltu	$2, $3, $7
  1a3034: 05 00 40 50  	beqzl	$2, 0x1a304c <.text+0xa304c>
  1a3038: 23 18 67 00  	subu	$3, $3, $7
  1a303c: ff ff 18 27  	addiu	$24, $24, -0x1 <.text+0xffffffffffefffff>
  1a3040: 21 18 69 00  	addu	$3, $3, $9
  1a3044: 00 00 00 00  	nop
  1a3048: 23 18 67 00  	subu	$3, $3, $7
  1a304c: ff ff 04 31  	andi	$4, $8, 0xffff
  1a3050: 1b 00 6b 00  	divu	$zero, $3, $11
  1a3054: 01 00 60 51  	beqzl	$11, 0x1a305c <.text+0xa305c>
  1a3058: cd 01 00 00  	break	0x0, 0x7
  1a305c: 12 10 00 00  	mflo	$2
  1a3060: 10 18 00 00  	mfhi	$3
  1a3064: 00 1c 03 00  	sll	$3, $3, 0x10
  1a3068: 25 18 64 00  	or	$3, $3, $4
  1a306c: 12 28 00 00  	mflo	$5
  1a3070: 18 38 50 00  	<unknown>
  1a3074: 2b 10 67 00  	sltu	$2, $3, $7
  1a3078: 0c 00 40 10  	beqz	$2, 0x1a30ac <.text+0xa30ac>
  1a307c: 00 14 18 00  	sll	$2, $24, 0x10
  1a3080: 21 18 69 00  	addu	$3, $3, $9
  1a3084: 2b 10 69 00  	sltu	$2, $3, $9
  1a3088: 07 00 40 14  	bnez	$2, 0x1a30a8 <.text+0xa30a8>
  1a308c: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a3090: 2b 10 67 00  	sltu	$2, $3, $7
  1a3094: 05 00 40 10  	beqz	$2, 0x1a30ac <.text+0xa30ac>
  1a3098: 00 14 18 00  	sll	$2, $24, 0x10
  1a309c: ff ff a5 24  	addiu	$5, $5, -0x1 <.text+0xffffffffffefffff>
  1a30a0: 21 18 69 00  	addu	$3, $3, $9
  1a30a4: 00 00 00 00  	nop
  1a30a8: 00 14 18 00  	sll	$2, $24, 0x10
  1a30ac: 23 40 67 00  	subu	$8, $3, $7
  1a30b0: 92 ff 00 10  	b	0x1a2efc <.text+0xa2efc>
  1a30b4: 25 80 45 00  	or	$16, $2, $5
  1a30b8: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a30bc: ff ff 42 34  	ori	$2, $2, 0xffff
  1a30c0: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a30c4: 2b 10 49 00  	sltu	$2, $2, $9
  1a30c8: 2d 28 60 00  	move	$5, $3
  1a30cc: 7d ff 00 10  	b	0x1a2ec4 <.text+0xa2ec4>
  1a30d0: 0b 28 82 00  	movn	$5, $4, $2
  1a30d4: 00 00 00 00  	nop
  1a30d8: 2b 10 07 01  	sltu	$2, $8, $7
  1a30dc: 1f 00 40 14  	bnez	$2, 0x1a315c <.text+0xa315c>
  1a30e0: 2d 50 00 00  	move	$10, $zero
  1a30e4: ff ff 02 34  	ori	$2, $zero, 0xffff
  1a30e8: 2b 10 47 00  	sltu	$2, $2, $7
  1a30ec: 8c 00 40 14  	bnez	$2, 0x1a3320 <.text+0xa3320>
  1a30f0: ff 00 02 3c  	lui	$2, 0xff
  1a30f4: 08 00 02 24  	addiu	$2, $zero, 0x8
  1a30f8: 00 01 e3 2c  	sltiu	$3, $7, 0x100
  1a30fc: 2d 28 40 00  	move	$5, $2
  1a3100: 0b 28 03 00  	movn	$5, $zero, $3
  1a3104: 1c 00 03 3c  	lui	$3, 0x1c
  1a3108: 06 10 a7 00  	srlv	$2, $7, $5
  1a310c: c8 ab 63 24  	addiu	$3, $3, -0x5438 <.text+0xffffffffffefabc8>
  1a3110: 20 00 0a 24  	addiu	$10, $zero, 0x20
  1a3114: 21 10 43 00  	addu	$2, $2, $3
  1a3118: 00 00 44 90  	lbu	$4, 0x0($2)
  1a311c: 21 20 85 00  	addu	$4, $4, $5
  1a3120: 23 78 44 01  	subu	$15, $10, $4
  1a3124: 1c 00 e0 15  	bnez	$15, 0x1a3198 <.text+0xa3198>
  1a3128: 23 c0 4f 01  	subu	$24, $10, $15
  1a312c: 2b 10 e8 00  	sltu	$2, $7, $8
  1a3130: 05 00 40 14  	bnez	$2, 0x1a3148 <.text+0xa3148>
  1a3134: 23 20 a9 01  	subu	$4, $13, $9
  1a3138: 2b 10 a9 01  	sltu	$2, $13, $9
  1a313c: 07 00 40 14  	bnez	$2, 0x1a315c <.text+0xa315c>
  1a3140: 2d 50 00 00  	move	$10, $zero
  1a3144: 23 20 a9 01  	subu	$4, $13, $9
  1a3148: 23 18 07 01  	subu	$3, $8, $7
  1a314c: 2b 10 a4 01  	sltu	$2, $13, $4
  1a3150: 01 00 0a 24  	addiu	$10, $zero, 0x1
  1a3154: 23 40 62 00  	subu	$8, $3, $2
  1a3158: 2d 68 80 00  	move	$13, $4
  1a315c: 32 ff c0 10  	beqz	$6, 0x1a2e28 <.text+0xa2e28>
  1a3160: 2d 80 00 00  	move	$16, $zero
  1a3164: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a3168: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a316c: 3c 10 0d 00  	dsll32	$2, $13, 0x0
  1a3170: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a3174: 24 60 83 01  	and	$12, $12, $3
  1a3178: 25 60 82 01  	or	$12, $12, $2
  1a317c: 3c 18 08 00  	dsll32	$3, $8, 0x0
  1a3180: ff ff 02 3c  	lui	$2, 0xffff
  1a3184: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a3188: 24 60 82 01  	and	$12, $12, $2
  1a318c: 25 ff 00 10  	b	0x1a2e24 <.text+0xa2e24>
  1a3190: 25 60 83 01  	or	$12, $12, $3
  1a3194: 00 00 00 00  	nop
  1a3198: 04 18 e7 01  	sllv	$3, $7, $15
  1a319c: 06 10 09 03  	srlv	$2, $9, $24
  1a31a0: 06 28 08 03  	srlv	$5, $8, $24
  1a31a4: 25 38 62 00  	or	$7, $3, $2
  1a31a8: 04 20 e8 01  	sllv	$4, $8, $15
  1a31ac: 02 54 07 00  	srl	$10, $7, 0x10
  1a31b0: ff ff f0 30  	andi	$16, $7, 0xffff
  1a31b4: 1b 00 aa 00  	divu	$zero, $5, $10
  1a31b8: 06 10 0d 03  	srlv	$2, $13, $24
  1a31bc: 25 40 82 00  	or	$8, $4, $2
  1a31c0: 01 00 40 51  	beqzl	$10, 0x1a31c8 <.text+0xa31c8>
  1a31c4: cd 01 00 00  	break	0x0, 0x7
  1a31c8: 02 24 08 00  	srl	$4, $8, 0x10
  1a31cc: 04 48 e9 01  	sllv	$9, $9, $15
  1a31d0: 12 28 00 00  	mflo	$5
  1a31d4: 10 18 00 00  	mfhi	$3
  1a31d8: 00 1c 03 00  	sll	$3, $3, 0x10
  1a31dc: 25 18 64 00  	or	$3, $3, $4
  1a31e0: 12 c8 00 00  	mflo	$25
  1a31e4: 18 58 b0 00  	<unknown>
  1a31e8: 2b 10 6b 00  	sltu	$2, $3, $11
  1a31ec: 0a 00 40 10  	beqz	$2, 0x1a3218 <.text+0xa3218>
  1a31f0: 04 68 ed 01  	sllv	$13, $13, $15
  1a31f4: 21 18 67 00  	addu	$3, $3, $7
  1a31f8: 2b 10 67 00  	sltu	$2, $3, $7
  1a31fc: 06 00 40 14  	bnez	$2, 0x1a3218 <.text+0xa3218>
  1a3200: ff ff b9 24  	addiu	$25, $5, -0x1 <.text+0xffffffffffefffff>
  1a3204: 2b 10 6b 00  	sltu	$2, $3, $11
  1a3208: 04 00 40 50  	beqzl	$2, 0x1a321c <.text+0xa321c>
  1a320c: 23 18 6b 00  	subu	$3, $3, $11
  1a3210: ff ff 39 27  	addiu	$25, $25, -0x1 <.text+0xffffffffffefffff>
  1a3214: 21 18 67 00  	addu	$3, $3, $7
  1a3218: 23 18 6b 00  	subu	$3, $3, $11
  1a321c: ff ff 04 31  	andi	$4, $8, 0xffff
  1a3220: 1b 00 6a 00  	divu	$zero, $3, $10
  1a3224: 01 00 40 51  	beqzl	$10, 0x1a322c <.text+0xa322c>
  1a3228: cd 01 00 00  	break	0x0, 0x7
  1a322c: 12 10 00 00  	mflo	$2
  1a3230: 10 18 00 00  	mfhi	$3
  1a3234: 00 1c 03 00  	sll	$3, $3, 0x10
  1a3238: 25 28 64 00  	or	$5, $3, $4
  1a323c: 12 40 00 00  	mflo	$8
  1a3240: 18 58 50 00  	<unknown>
  1a3244: 2b 10 ab 00  	sltu	$2, $5, $11
  1a3248: 0c 00 40 10  	beqz	$2, 0x1a327c <.text+0xa327c>
  1a324c: 00 14 19 00  	sll	$2, $25, 0x10
  1a3250: 21 28 a7 00  	addu	$5, $5, $7
  1a3254: 2b 10 a7 00  	sltu	$2, $5, $7
  1a3258: 07 00 40 14  	bnez	$2, 0x1a3278 <.text+0xa3278>
  1a325c: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a3260: 2b 10 ab 00  	sltu	$2, $5, $11
  1a3264: 05 00 40 10  	beqz	$2, 0x1a327c <.text+0xa327c>
  1a3268: 00 14 19 00  	sll	$2, $25, 0x10
  1a326c: ff ff 08 25  	addiu	$8, $8, -0x1 <.text+0xffffffffffefffff>
  1a3270: 21 28 a7 00  	addu	$5, $5, $7
  1a3274: 00 00 00 00  	nop
  1a3278: 00 14 19 00  	sll	$2, $25, 0x10
  1a327c: 23 28 ab 00  	subu	$5, $5, $11
  1a3280: 25 50 48 00  	or	$10, $2, $8
  1a3284: 19 00 49 01  	multu	$10, $9
  1a3288: 10 18 00 00  	mfhi	$3
  1a328c: 12 58 00 00  	mflo	$11
  1a3290: 2b 10 a3 00  	sltu	$2, $5, $3
  1a3294: 1b 00 40 14  	bnez	$2, 0x1a3304 <.text+0xa3304>
  1a3298: 23 20 69 01  	subu	$4, $11, $9
  1a329c: 17 00 65 10  	beq	$3, $5, 0x1a32fc <.text+0xa32fc>
  1a32a0: 2b 10 ab 01  	sltu	$2, $13, $11
  1a32a4: e0 fe c0 10  	beqz	$6, 0x1a2e28 <.text+0xa2e28>
  1a32a8: 2d 80 00 00  	move	$16, $zero
  1a32ac: 23 20 ab 01  	subu	$4, $13, $11
  1a32b0: 23 28 a3 00  	subu	$5, $5, $3
  1a32b4: 2b 18 a4 01  	sltu	$3, $13, $4
  1a32b8: 06 20 e4 01  	srlv	$4, $4, $15
  1a32bc: 23 40 a3 00  	subu	$8, $5, $3
  1a32c0: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  1a32c4: 3c 18 03 00  	dsll32	$3, $3, 0x0
  1a32c8: 04 10 08 03  	sllv	$2, $8, $24
  1a32cc: 24 60 83 01  	and	$12, $12, $3
  1a32d0: 25 10 44 00  	or	$2, $2, $4
  1a32d4: 06 28 e8 01  	srlv	$5, $8, $15
  1a32d8: 3c 10 02 00  	dsll32	$2, $2, 0x0
  1a32dc: ff ff 03 3c  	lui	$3, 0xffff
  1a32e0: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  1a32e4: 3e 10 02 00  	dsrl32	$2, $2, 0x0
  1a32e8: 3c 28 05 00  	dsll32	$5, $5, 0x0
  1a32ec: 25 60 82 01  	or	$12, $12, $2
  1a32f0: 24 60 83 01  	and	$12, $12, $3
  1a32f4: cb fe 00 10  	b	0x1a2e24 <.text+0xa2e24>
  1a32f8: 25 60 85 01  	or	$12, $12, $5
  1a32fc: e9 ff 40 10  	beqz	$2, 0x1a32a4 <.text+0xa32a4>
  1a3300: 23 20 69 01  	subu	$4, $11, $9
  1a3304: 23 18 67 00  	subu	$3, $3, $7
  1a3308: 2b 10 64 01  	sltu	$2, $11, $4
  1a330c: ff ff 4a 25  	addiu	$10, $10, -0x1 <.text+0xffffffffffefffff>
  1a3310: 23 18 62 00  	subu	$3, $3, $2
  1a3314: e3 ff 00 10  	b	0x1a32a4 <.text+0xa32a4>
  1a3318: 2d 58 80 00  	move	$11, $4
  1a331c: 00 00 00 00  	nop
  1a3320: 10 00 03 24  	addiu	$3, $zero, 0x10
  1a3324: ff ff 42 34  	ori	$2, $2, 0xffff
  1a3328: 18 00 04 24  	addiu	$4, $zero, 0x18
  1a332c: 2b 10 47 00  	sltu	$2, $2, $7
  1a3330: 2d 28 60 00  	move	$5, $3
  1a3334: 73 ff 00 10  	b	0x1a3104 <.text+0xa3104>
  1a3338: 0b 28 82 00  	movn	$5, $4, $2
  1a333c: 00 00 00 00  	nop
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
  1a3dc0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a3dc4: ff 00 84 30  	andi	$4, $4, 0xff
  1a3dc8: ff 00 02 24  	addiu	$2, $zero, 0xff
  1a3dcc: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a3dd0: 0a 00 82 10  	beq	$4, $2, 0x1a3dfc <.text+0xa3dfc>
  1a3dd4: 2d 18 00 00  	move	$3, $zero
  1a3dd8: 07 00 84 30  	andi	$4, $4, 0x7
  1a3ddc: 02 00 02 24  	addiu	$2, $zero, 0x2
  1a3de0: 03 00 85 28  	slti	$5, $4, 0x3
  1a3de4: 05 00 82 10  	beq	$4, $2, 0x1a3dfc <.text+0xa3dfc>
  1a3de8: 02 00 03 24  	addiu	$3, $zero, 0x2
  1a3dec: 08 00 a0 10  	beqz	$5, 0x1a3e10 <.text+0xa3e10>
  1a3df0: 04 00 03 24  	addiu	$3, $zero, 0x4
  1a3df4: 0b 00 80 14  	bnez	$4, 0x1a3e24 <.text+0xa3e24>
  1a3df8: 00 00 00 00  	nop
  1a3dfc: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a3e00: 2d 10 60 00  	move	$2, $3
  1a3e04: 08 00 e0 03  	jr	$ra
  1a3e08: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a3e0c: 00 00 00 00  	nop
  1a3e10: 03 00 02 24  	addiu	$2, $zero, 0x3
  1a3e14: f9 ff 82 10  	beq	$4, $2, 0x1a3dfc <.text+0xa3dfc>
  1a3e18: 04 00 02 24  	addiu	$2, $zero, 0x4
  1a3e1c: f7 ff 82 10  	beq	$4, $2, 0x1a3dfc <.text+0xa3dfc>
  1a3e20: 08 00 03 24  	addiu	$3, $zero, 0x8
  1a3e24: 5e 1d 04 0c  	jal	0x107578 <.text+0x7578>
		...
  1a3e30: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1a3e34: ff 00 84 30  	andi	$4, $4, 0xff
  1a3e38: ff 00 02 24  	addiu	$2, $zero, 0xff
  1a3e3c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1a3e40: 0a 00 82 10  	beq	$4, $2, 0x1a3e6c <.text+0xa3e6c>
  1a3e44: 2d 18 00 00  	move	$3, $zero
  1a3e48: 70 00 84 30  	andi	$4, $4, 0x70
  1a3e4c: 20 00 02 24  	addiu	$2, $zero, 0x20
  1a3e50: 1f 00 82 10  	beq	$4, $2, 0x1a3ed0 <.text+0xa3ed0>
  1a3e54: 21 00 82 28  	slti	$2, $4, 0x21
  1a3e58: 0d 00 40 10  	beqz	$2, 0x1a3e90 <.text+0xa3e90>
  1a3e5c: 40 00 02 24  	addiu	$2, $zero, 0x40
  1a3e60: 07 00 80 14  	bnez	$4, 0x1a3e80 <.text+0xa3e80>
  1a3e64: 10 00 02 24  	addiu	$2, $zero, 0x10
  1a3e68: 2d 18 00 00  	move	$3, $zero
  1a3e6c: 00 00 bf df  	ld	$ra, 0x0($sp)
  1a3e70: 2d 10 60 00  	move	$2, $3
  1a3e74: 08 00 e0 03  	jr	$ra
  1a3e78: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  1a3e7c: 00 00 00 00  	nop
  1a3e80: fa ff 82 10  	beq	$4, $2, 0x1a3e6c <.text+0xa3e6c>
  1a3e84: 2d 18 00 00  	move	$3, $zero
  1a3e88: 15 00 00 10  	b	0x1a3ee0 <.text+0xa3ee0>
  1a3e8c: 00 00 00 00  	nop
  1a3e90: 0a 00 82 10  	beq	$4, $2, 0x1a3ebc <.text+0xa3ebc>
  1a3e94: 41 00 82 28  	slti	$2, $4, 0x41
  1a3e98: f9 ff 40 10  	beqz	$2, 0x1a3e80 <.text+0xa3e80>
  1a3e9c: 50 00 02 24  	addiu	$2, $zero, 0x50
  1a3ea0: 30 00 02 24  	addiu	$2, $zero, 0x30
  1a3ea4: 0e 00 82 14  	bne	$4, $2, 0x1a3ee0 <.text+0xa3ee0>
  1a3ea8: 00 00 00 00  	nop
  1a3eac: 3e 90 06 0c  	jal	0x1a40f8 <.text+0xa40f8>
  1a3eb0: 2d 20 a0 00  	move	$4, $5
  1a3eb4: ed ff 00 10  	b	0x1a3e6c <.text+0xa3e6c>
  1a3eb8: 2d 18 40 00  	move	$3, $2
  1a3ebc: 3c 90 06 0c  	jal	0x1a40f0 <.text+0xa40f0>
  1a3ec0: 2d 20 a0 00  	move	$4, $5
  1a3ec4: e9 ff 00 10  	b	0x1a3e6c <.text+0xa3e6c>
  1a3ec8: 2d 18 40 00  	move	$3, $2
  1a3ecc: 00 00 00 00  	nop
  1a3ed0: 40 90 06 0c  	jal	0x1a4100 <.text+0xa4100>
  1a3ed4: 2d 20 a0 00  	move	$4, $5
  1a3ed8: e4 ff 00 10  	b	0x1a3e6c <.text+0xa3e6c>
  1a3edc: 2d 18 40 00  	move	$3, $2
  1a3ee0: 5e 1d 04 0c  	jal	0x107578 <.text+0x7578>
  1a3ee4: 00 00 00 00  	nop
  1a3ee8: 2d 30 00 00  	move	$6, $zero
  1a3eec: 2d 38 00 00  	move	$7, $zero
  1a3ef0: 80 ff 08 24  	addiu	$8, $zero, -0x80 <.text+0xffffffffffefff80>
  1a3ef4: 00 00 00 00  	nop
  1a3ef8: 00 00 83 90  	lbu	$3, 0x0($4)
  1a3efc: 01 00 84 24  	addiu	$4, $4, 0x1
  1a3f00: 7f 00 62 30  	andi	$2, $3, 0x7f
  1a3f04: 24 18 68 00  	and	$3, $3, $8
  1a3f08: 04 10 c2 00  	sllv	$2, $2, $6
  1a3f0c: 07 00 c6 24  	addiu	$6, $6, 0x7
  1a3f10: f9 ff 60 14  	bnez	$3, 0x1a3ef8 <.text+0xa3ef8>
  1a3f14: 25 38 47 00  	or	$7, $2, $7
  1a3f18: 00 00 a7 fc  	sd	$7, 0x0($5)
  1a3f1c: 08 00 e0 03  	jr	$ra
  1a3f20: 2d 10 80 00  	move	$2, $4
  1a3f24: 00 00 00 00  	nop
  1a3f28: 2d 38 00 00  	move	$7, $zero
  1a3f2c: 2d 40 00 00  	move	$8, $zero
  1a3f30: 80 ff 09 24  	addiu	$9, $zero, -0x80 <.text+0xffffffffffefff80>
  1a3f34: 00 00 00 00  	nop
  1a3f38: 00 00 86 90  	lbu	$6, 0x0($4)
  1a3f3c: 01 00 84 24  	addiu	$4, $4, 0x1
  1a3f40: 7f 00 c2 30  	andi	$2, $6, 0x7f
  1a3f44: 24 18 c9 00  	and	$3, $6, $9
  1a3f48: 04 10 e2 00  	sllv	$2, $2, $7
  1a3f4c: 07 00 e7 24  	addiu	$7, $7, 0x7
  1a3f50: f9 ff 60 14  	bnez	$3, 0x1a3f38 <.text+0xa3f38>
  1a3f54: 25 40 48 00  	or	$8, $2, $8
  1a3f58: 40 00 e2 2c  	sltiu	$2, $7, 0x40
  1a3f5c: 08 00 40 50  	beqzl	$2, 0x1a3f80 <.text+0xa3f80>
  1a3f60: 00 00 a8 fc  	sd	$8, 0x0($5)
  1a3f64: 40 00 c2 30  	andi	$2, $6, 0x40
  1a3f68: 05 00 40 50  	beqzl	$2, 0x1a3f80 <.text+0xa3f80>
  1a3f6c: 00 00 a8 fc  	sd	$8, 0x0($5)
  1a3f70: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1a3f74: 14 10 e2 00  	dsllv	$2, $2, $7
  1a3f78: 25 40 02 01  	or	$8, $8, $2
  1a3f7c: 00 00 a8 fc  	sd	$8, 0x0($5)
  1a3f80: 08 00 e0 03  	jr	$ra
  1a3f84: 2d 10 80 00  	move	$2, $4
  1a3f88: a0 ff bd 27  	addiu	$sp, $sp, -0x60 <.text+0xffffffffffefffa0>
  1a3f8c: 50 00 02 24  	addiu	$2, $zero, 0x50
  1a3f90: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1a3f94: ff 00 90 30  	andi	$16, $4, 0xff
  1a3f98: 40 00 b3 ff  	sd	$19, 0x40($sp)
  1a3f9c: 2d 98 a0 00  	move	$19, $5
  1a3fa0: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1a3fa4: 2d 90 c0 00  	move	$18, $6
  1a3fa8: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1a3fac: 2d 88 e0 00  	move	$17, $7
  1a3fb0: 45 00 02 12  	beq	$16, $2, 0x1a40c8 <.text+0xa40c8>
  1a3fb4: 50 00 bf ff  	sd	$ra, 0x50($sp)
  1a3fb8: 0f 00 03 32  	andi	$3, $16, 0xf
  1a3fbc: 0d 00 62 2c  	sltiu	$2, $3, 0xd
  1a3fc0: 47 00 40 10  	beqz	$2, 0x1a40e0 <.text+0xa40e0>
  1a3fc4: 80 10 03 00  	sll	$2, $3, 0x2
  1a3fc8: 1c 00 03 3c  	lui	$3, 0x1c
  1a3fcc: f8 a7 63 24  	addiu	$3, $3, -0x5808 <.text+0xffffffffffefa7f8>
  1a3fd0: 21 10 43 00  	addu	$2, $2, $3
  1a3fd4: 00 00 44 8c  	lw	$4, 0x0($2)
  1a3fd8: 08 00 80 00  	jr	$4
  1a3fdc: 00 00 00 00  	nop
  1a3fe0: 2d 20 c0 00  	move	$4, $6
  1a3fe4: ba 8f 06 0c  	jal	0x1a3ee8 <.text+0xa3ee8>
  1a3fe8: 2d 28 a0 03  	move	$5, $sp
  1a3fec: 2d 30 40 00  	move	$6, $2
  1a3ff0: 00 00 a2 df  	ld	$2, 0x0($sp)
  1a3ff4: 3c 20 02 00  	dsll32	$4, $2, 0x0
  1a3ff8: 3f 20 04 00  	dsra32	$4, $4, 0x0
  1a3ffc: 08 00 80 10  	beqz	$4, 0x1a4020 <.text+0xa4020>
  1a4000: 70 00 02 32  	andi	$2, $16, 0x70
  1a4004: 21 28 93 00  	addu	$5, $4, $19
  1a4008: 10 00 42 38  	xori	$2, $2, 0x10
  1a400c: 80 00 03 32  	andi	$3, $16, 0x80
  1a4010: 21 20 92 00  	addu	$4, $4, $18
  1a4014: 02 00 60 10  	beqz	$3, 0x1a4020 <.text+0xa4020>
  1a4018: 0b 20 a2 00  	movn	$4, $5, $2
  1a401c: 00 00 84 8c  	lw	$4, 0x0($4)
  1a4020: 00 00 24 ae  	sw	$4, 0x0($17)
  1a4024: 2d 10 c0 00  	move	$2, $6
  1a4028: 50 00 bf df  	ld	$ra, 0x50($sp)
  1a402c: 40 00 b3 df  	ld	$19, 0x40($sp)
  1a4030: 30 00 b2 df  	ld	$18, 0x30($sp)
  1a4034: 20 00 b1 df  	ld	$17, 0x20($sp)
  1a4038: 10 00 b0 df  	ld	$16, 0x10($sp)
  1a403c: 08 00 e0 03  	jr	$ra
  1a4040: 60 00 bd 27  	addiu	$sp, $sp, 0x60
  1a4044: 01 00 c2 90  	lbu	$2, 0x1($6)
  1a4048: 00 00 c3 90  	lbu	$3, 0x0($6)
  1a404c: 02 00 c6 24  	addiu	$6, $6, 0x2
  1a4050: 38 12 02 00  	dsll	$2, $2, 0x8
  1a4054: 25 10 43 00  	or	$2, $2, $3
  1a4058: e8 ff 00 10  	b	0x1a3ffc <.text+0xa3ffc>
  1a405c: ff ff 44 30  	andi	$4, $2, 0xffff
  1a4060: 2d 20 c0 00  	move	$4, $6
  1a4064: ca 8f 06 0c  	jal	0x1a3f28 <.text+0xa3f28>
  1a4068: 08 00 a5 27  	addiu	$5, $sp, 0x8
  1a406c: 2d 30 40 00  	move	$6, $2
  1a4070: e0 ff 00 10  	b	0x1a3ff4 <.text+0xa3ff4>
  1a4074: 08 00 a2 df  	ld	$2, 0x8($sp)
  1a4078: 01 00 c2 90  	lbu	$2, 0x1($6)
  1a407c: 00 00 c3 90  	lbu	$3, 0x0($6)
  1a4080: 02 00 c6 24  	addiu	$6, $6, 0x2
  1a4084: 38 12 02 00  	dsll	$2, $2, 0x8
  1a4088: 25 10 43 00  	or	$2, $2, $3
  1a408c: 3c 14 02 00  	dsll32	$2, $2, 0x10
  1a4090: 3f 14 02 00  	dsra32	$2, $2, 0x10
  1a4094: ff ff 42 30  	andi	$2, $2, 0xffff
  1a4098: 00 14 02 00  	sll	$2, $2, 0x10
  1a409c: d7 ff 00 10  	b	0x1a3ffc <.text+0xa3ffc>
  1a40a0: 03 24 02 00  	sra	$4, $2, 0x10
  1a40a4: 00 00 00 00  	nop
  1a40a8: 03 00 c4 88  	lwl	$4, 0x3($6)
  1a40ac: 00 00 c4 98  	lwr	$4, 0x0($6)
  1a40b0: d2 ff 00 10  	b	0x1a3ffc <.text+0xa3ffc>
  1a40b4: 04 00 c6 24  	addiu	$6, $6, 0x4
  1a40b8: 07 00 c2 68  	ldl	$2, 0x7($6)
  1a40bc: 00 00 c2 6c  	ldr	$2, 0x0($6)
  1a40c0: cc ff 00 10  	b	0x1a3ff4 <.text+0xa3ff4>
  1a40c4: 08 00 c6 24  	addiu	$6, $6, 0x8
  1a40c8: 03 00 c2 24  	addiu	$2, $6, 0x3
  1a40cc: fc ff 03 24  	addiu	$3, $zero, -0x4 <.text+0xffffffffffeffffc>
  1a40d0: 24 10 43 00  	and	$2, $2, $3
  1a40d4: 00 00 44 8c  	lw	$4, 0x0($2)
  1a40d8: d1 ff 00 10  	b	0x1a4020 <.text+0xa4020>
  1a40dc: 04 00 46 24  	addiu	$6, $2, 0x4
  1a40e0: 5e 1d 04 0c  	jal	0x107578 <.text+0x7578>
  1a40e4: 00 00 00 00  	nop
  1a40e8: 08 00 e0 03  	jr	$ra
  1a40ec: 48 01 82 8c  	lw	$2, 0x148($4)
  1a40f0: 08 00 e0 03  	jr	$ra
  1a40f4: 54 01 82 8c  	lw	$2, 0x154($4)
  1a40f8: 08 00 e0 03  	jr	$ra
  1a40fc: 50 01 82 8c  	lw	$2, 0x150($4)
