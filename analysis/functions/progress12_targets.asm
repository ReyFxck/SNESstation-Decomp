# Progress 12 focused target evidence
# Generated from SNES_EMU.analysis.elf; R5900 unknowns conservatively annotated.

===== TARGET 0x0012d79c reachable=48 span=0xc0 =====
  12d79c: 34 00 05 3c  	lui	$5, 0x34
  12d7a0: 34 00 02 3c  	lui	$2, 0x34
  12d7a4: 30 15 a3 24  	addiu	$3, $5, 0x1530
  12d7a8: 68 15 44 84  	lh	$4, 0x1568($2)
  12d7ac: 0c 00 68 84  	lh	$8, 0xc($3)
  12d7b0: 34 00 02 3c  	lui	$2, 0x34
  12d7b4: 06 00 66 84  	lh	$6, 0x6($3)
  12d7b8: 30 15 a5 84  	lh	$5, 0x1530($5)
  12d7bc: 6a 15 47 84  	lh	$7, 0x156a($2)
  12d7c0: 18 30 86 00  	mult	$a2, $a0, $a2  # R5900 3-operand
  12d7c4: 02 00 6b 84  	lh	$11, 0x2($3)
  12d7c8: 34 00 02 3c  	lui	$2, 0x34
  12d7cc: 08 00 6a 84  	lh	$10, 0x8($3)
  12d7d0: 18 28 85 00  	mult	$a1, $a0, $a1  # R5900 3-operand
  12d7d4: 6c 15 42 84  	lh	$2, 0x156c($2)
  12d7d8: 18 20 88 00  	mult	$a0, $a0, $t0  # R5900 3-operand
  12d7dc: 0e 00 6c 84  	lh	$12, 0xe($3)
  12d7e0: 18 58 eb 00  	mult	$t3, $a3, $t3  # R5900 3-operand
  12d7e4: 10 00 69 84  	lh	$9, 0x10($3)
  12d7e8: 18 50 ea 00  	mult	$t2, $a3, $t2  # R5900 3-operand
  12d7ec: 04 00 68 84  	lh	$8, 0x4($3)
  12d7f0: 18 38 ec 00  	mult	$a3, $a3, $t4  # R5900 3-operand
  12d7f4: 0a 00 63 84  	lh	$3, 0xa($3)
  12d7f8: 18 48 49 00  	mult	$t1, $v0, $t1  # R5900 3-operand
  12d7fc: 18 40 48 00  	mult	$t0, $v0, $t0  # R5900 3-operand
  12d800: c3 2b 05 00  	sra	$5, $5, 0xf
  12d804: 18 10 43 00  	mult	$v0, $v0, $v1  # R5900 3-operand
  12d808: c3 5b 0b 00  	sra	$11, $11, 0xf
  12d80c: c3 33 06 00  	sra	$6, $6, 0xf
  12d810: c3 53 0a 00  	sra	$10, $10, 0xf
  12d814: 21 28 ab 00  	addu	$5, $5, $11
  12d818: 21 30 ca 00  	addu	$6, $6, $10
  12d81c: c3 43 08 00  	sra	$8, $8, 0xf
  12d820: c3 23 04 00  	sra	$4, $4, 0xf
  12d824: c3 13 02 00  	sra	$2, $2, 0xf
  12d828: c3 3b 07 00  	sra	$7, $7, 0xf
  12d82c: 21 30 c2 00  	addu	$6, $6, $2
  12d830: 21 28 a8 00  	addu	$5, $5, $8
  12d834: 34 00 02 3c  	lui	$2, 0x34
  12d838: 21 20 87 00  	addu	$4, $4, $7
  12d83c: c3 4b 09 00  	sra	$9, $9, 0xf
  12d840: 6e 15 45 a4  	sh	$5, 0x156e($2)
  12d844: 34 00 02 3c  	lui	$2, 0x34
  12d848: 21 20 89 00  	addu	$4, $4, $9
  12d84c: 70 15 46 a4  	sh	$6, 0x1570($2)
  12d850: 34 00 02 3c  	lui	$2, 0x34
  12d854: 08 00 e0 03  	jr	$ra
  12d858: 72 15 44 a4  	sh	$4, 0x1572($2)

===== TARGET 0x0012d85c reachable=48 span=0xc0 =====
  12d85c: 34 00 05 3c  	lui	$5, 0x34
  12d860: 34 00 02 3c  	lui	$2, 0x34
  12d864: 18 15 a3 24  	addiu	$3, $5, 0x1518
  12d868: 74 15 44 84  	lh	$4, 0x1574($2)
  12d86c: 0c 00 68 84  	lh	$8, 0xc($3)
  12d870: 34 00 02 3c  	lui	$2, 0x34
  12d874: 06 00 66 84  	lh	$6, 0x6($3)
  12d878: 18 15 a5 84  	lh	$5, 0x1518($5)
  12d87c: 76 15 47 84  	lh	$7, 0x1576($2)
  12d880: 18 30 86 00  	mult	$a2, $a0, $a2  # R5900 3-operand
  12d884: 02 00 6b 84  	lh	$11, 0x2($3)
  12d888: 34 00 02 3c  	lui	$2, 0x34
  12d88c: 08 00 6a 84  	lh	$10, 0x8($3)
  12d890: 18 28 85 00  	mult	$a1, $a0, $a1  # R5900 3-operand
  12d894: 78 15 42 84  	lh	$2, 0x1578($2)
  12d898: 18 20 88 00  	mult	$a0, $a0, $t0  # R5900 3-operand
  12d89c: 0e 00 6c 84  	lh	$12, 0xe($3)
  12d8a0: 18 58 eb 00  	mult	$t3, $a3, $t3  # R5900 3-operand
  12d8a4: 10 00 69 84  	lh	$9, 0x10($3)
  12d8a8: 18 50 ea 00  	mult	$t2, $a3, $t2  # R5900 3-operand
  12d8ac: 04 00 68 84  	lh	$8, 0x4($3)
  12d8b0: 18 38 ec 00  	mult	$a3, $a3, $t4  # R5900 3-operand
  12d8b4: 0a 00 63 84  	lh	$3, 0xa($3)
  12d8b8: 18 48 49 00  	mult	$t1, $v0, $t1  # R5900 3-operand
  12d8bc: 18 40 48 00  	mult	$t0, $v0, $t0  # R5900 3-operand
  12d8c0: c3 2b 05 00  	sra	$5, $5, 0xf
  12d8c4: 18 10 43 00  	mult	$v0, $v0, $v1  # R5900 3-operand
  12d8c8: c3 5b 0b 00  	sra	$11, $11, 0xf
  12d8cc: c3 33 06 00  	sra	$6, $6, 0xf
  12d8d0: c3 53 0a 00  	sra	$10, $10, 0xf
  12d8d4: 21 28 ab 00  	addu	$5, $5, $11
  12d8d8: 21 30 ca 00  	addu	$6, $6, $10
  12d8dc: c3 43 08 00  	sra	$8, $8, 0xf
  12d8e0: c3 23 04 00  	sra	$4, $4, 0xf
  12d8e4: c3 13 02 00  	sra	$2, $2, 0xf
  12d8e8: c3 3b 07 00  	sra	$7, $7, 0xf
  12d8ec: 21 30 c2 00  	addu	$6, $6, $2
  12d8f0: 21 28 a8 00  	addu	$5, $5, $8
  12d8f4: 34 00 02 3c  	lui	$2, 0x34
  12d8f8: 21 20 87 00  	addu	$4, $4, $7
  12d8fc: c3 4b 09 00  	sra	$9, $9, 0xf
  12d900: 7a 15 45 a4  	sh	$5, 0x157a($2)
  12d904: 34 00 02 3c  	lui	$2, 0x34
  12d908: 21 20 89 00  	addu	$4, $4, $9
  12d90c: 7c 15 46 a4  	sh	$6, 0x157c($2)
  12d910: 34 00 02 3c  	lui	$2, 0x34
  12d914: 08 00 e0 03  	jr	$ra
  12d918: 7e 15 44 a4  	sh	$4, 0x157e($2)

===== TARGET 0x0012d91c reachable=48 span=0xc0 =====
  12d91c: 34 00 05 3c  	lui	$5, 0x34
  12d920: 34 00 02 3c  	lui	$2, 0x34
  12d924: 00 15 a3 24  	addiu	$3, $5, 0x1500
  12d928: 80 15 44 84  	lh	$4, 0x1580($2)
  12d92c: 0c 00 68 84  	lh	$8, 0xc($3)
  12d930: 34 00 02 3c  	lui	$2, 0x34
  12d934: 06 00 66 84  	lh	$6, 0x6($3)
  12d938: 00 15 a5 84  	lh	$5, 0x1500($5)
  12d93c: 82 15 47 84  	lh	$7, 0x1582($2)
  12d940: 18 30 86 00  	mult	$a2, $a0, $a2  # R5900 3-operand
  12d944: 02 00 6b 84  	lh	$11, 0x2($3)
  12d948: 34 00 02 3c  	lui	$2, 0x34
  12d94c: 08 00 6a 84  	lh	$10, 0x8($3)
  12d950: 18 28 85 00  	mult	$a1, $a0, $a1  # R5900 3-operand
  12d954: 84 15 42 84  	lh	$2, 0x1584($2)
  12d958: 18 20 88 00  	mult	$a0, $a0, $t0  # R5900 3-operand
  12d95c: 0e 00 6c 84  	lh	$12, 0xe($3)
  12d960: 18 58 eb 00  	mult	$t3, $a3, $t3  # R5900 3-operand
  12d964: 10 00 69 84  	lh	$9, 0x10($3)
  12d968: 18 50 ea 00  	mult	$t2, $a3, $t2  # R5900 3-operand
  12d96c: 04 00 68 84  	lh	$8, 0x4($3)
  12d970: 18 38 ec 00  	mult	$a3, $a3, $t4  # R5900 3-operand
  12d974: 0a 00 63 84  	lh	$3, 0xa($3)
  12d978: 18 48 49 00  	mult	$t1, $v0, $t1  # R5900 3-operand
  12d97c: 18 40 48 00  	mult	$t0, $v0, $t0  # R5900 3-operand
  12d980: c3 2b 05 00  	sra	$5, $5, 0xf
  12d984: 18 10 43 00  	mult	$v0, $v0, $v1  # R5900 3-operand
  12d988: c3 5b 0b 00  	sra	$11, $11, 0xf
  12d98c: c3 33 06 00  	sra	$6, $6, 0xf
  12d990: c3 53 0a 00  	sra	$10, $10, 0xf
  12d994: 21 28 ab 00  	addu	$5, $5, $11
  12d998: 21 30 ca 00  	addu	$6, $6, $10
  12d99c: c3 43 08 00  	sra	$8, $8, 0xf
  12d9a0: c3 23 04 00  	sra	$4, $4, 0xf
  12d9a4: c3 13 02 00  	sra	$2, $2, 0xf
  12d9a8: c3 3b 07 00  	sra	$7, $7, 0xf
  12d9ac: 21 30 c2 00  	addu	$6, $6, $2
  12d9b0: 21 28 a8 00  	addu	$5, $5, $8
  12d9b4: 34 00 02 3c  	lui	$2, 0x34
  12d9b8: 21 20 87 00  	addu	$4, $4, $7
  12d9bc: c3 4b 09 00  	sra	$9, $9, 0xf
  12d9c0: 86 15 45 a4  	sh	$5, 0x1586($2)
  12d9c4: 34 00 02 3c  	lui	$2, 0x34
  12d9c8: 21 20 89 00  	addu	$4, $4, $9
  12d9cc: 88 15 46 a4  	sh	$6, 0x1588($2)
  12d9d0: 34 00 02 3c  	lui	$2, 0x34
  12d9d4: 08 00 e0 03  	jr	$ra
  12d9d8: 8a 15 44 a4  	sh	$4, 0x158a($2)

===== TARGET 0x0012d9dc reachable=48 span=0xc0 =====
  12d9dc: 34 00 05 3c  	lui	$5, 0x34
  12d9e0: 34 00 02 3c  	lui	$2, 0x34
  12d9e4: 30 15 a3 24  	addiu	$3, $5, 0x1530
  12d9e8: 8c 15 44 84  	lh	$4, 0x158c($2)
  12d9ec: 04 00 68 84  	lh	$8, 0x4($3)
  12d9f0: 34 00 02 3c  	lui	$2, 0x34
  12d9f4: 02 00 66 84  	lh	$6, 0x2($3)
  12d9f8: 30 15 a5 84  	lh	$5, 0x1530($5)
  12d9fc: 8e 15 47 84  	lh	$7, 0x158e($2)
  12da00: 18 30 86 00  	mult	$a2, $a0, $a2  # R5900 3-operand
  12da04: 06 00 6b 84  	lh	$11, 0x6($3)
  12da08: 34 00 02 3c  	lui	$2, 0x34
  12da0c: 08 00 6a 84  	lh	$10, 0x8($3)
  12da10: 18 28 85 00  	mult	$a1, $a0, $a1  # R5900 3-operand
  12da14: 90 15 42 84  	lh	$2, 0x1590($2)
  12da18: 18 20 88 00  	mult	$a0, $a0, $t0  # R5900 3-operand
  12da1c: 0a 00 6c 84  	lh	$12, 0xa($3)
  12da20: 18 58 eb 00  	mult	$t3, $a3, $t3  # R5900 3-operand
  12da24: 10 00 69 84  	lh	$9, 0x10($3)
  12da28: 18 50 ea 00  	mult	$t2, $a3, $t2  # R5900 3-operand
  12da2c: 0c 00 68 84  	lh	$8, 0xc($3)
  12da30: 18 38 ec 00  	mult	$a3, $a3, $t4  # R5900 3-operand
  12da34: 0e 00 63 84  	lh	$3, 0xe($3)
  12da38: 18 48 49 00  	mult	$t1, $v0, $t1  # R5900 3-operand
  12da3c: 18 40 48 00  	mult	$t0, $v0, $t0  # R5900 3-operand
  12da40: c3 2b 05 00  	sra	$5, $5, 0xf
  12da44: 18 10 43 00  	mult	$v0, $v0, $v1  # R5900 3-operand
  12da48: c3 5b 0b 00  	sra	$11, $11, 0xf
  12da4c: c3 33 06 00  	sra	$6, $6, 0xf
  12da50: c3 53 0a 00  	sra	$10, $10, 0xf
  12da54: 21 28 ab 00  	addu	$5, $5, $11
  12da58: 21 30 ca 00  	addu	$6, $6, $10
  12da5c: c3 43 08 00  	sra	$8, $8, 0xf
  12da60: c3 23 04 00  	sra	$4, $4, 0xf
  12da64: c3 13 02 00  	sra	$2, $2, 0xf
  12da68: c3 3b 07 00  	sra	$7, $7, 0xf
  12da6c: 21 30 c2 00  	addu	$6, $6, $2
  12da70: 21 28 a8 00  	addu	$5, $5, $8
  12da74: 34 00 02 3c  	lui	$2, 0x34
  12da78: 21 20 87 00  	addu	$4, $4, $7
  12da7c: c3 4b 09 00  	sra	$9, $9, 0xf
  12da80: 92 15 45 a4  	sh	$5, 0x1592($2)
  12da84: 34 00 02 3c  	lui	$2, 0x34
  12da88: 21 20 89 00  	addu	$4, $4, $9
  12da8c: 94 15 46 a4  	sh	$6, 0x1594($2)
  12da90: 34 00 02 3c  	lui	$2, 0x34
  12da94: 08 00 e0 03  	jr	$ra
  12da98: 96 15 44 a4  	sh	$4, 0x1596($2)

===== TARGET 0x0012da9c reachable=48 span=0xc0 =====
  12da9c: 34 00 05 3c  	lui	$5, 0x34
  12daa0: 34 00 02 3c  	lui	$2, 0x34
  12daa4: 18 15 a3 24  	addiu	$3, $5, 0x1518
  12daa8: 98 15 44 84  	lh	$4, 0x1598($2)
  12daac: 04 00 68 84  	lh	$8, 0x4($3)
  12dab0: 34 00 02 3c  	lui	$2, 0x34
  12dab4: 02 00 66 84  	lh	$6, 0x2($3)
  12dab8: 18 15 a5 84  	lh	$5, 0x1518($5)
  12dabc: 9a 15 47 84  	lh	$7, 0x159a($2)
  12dac0: 18 30 86 00  	mult	$a2, $a0, $a2  # R5900 3-operand
  12dac4: 06 00 6b 84  	lh	$11, 0x6($3)
  12dac8: 34 00 02 3c  	lui	$2, 0x34
  12dacc: 08 00 6a 84  	lh	$10, 0x8($3)
  12dad0: 18 28 85 00  	mult	$a1, $a0, $a1  # R5900 3-operand
  12dad4: 9c 15 42 84  	lh	$2, 0x159c($2)
  12dad8: 18 20 88 00  	mult	$a0, $a0, $t0  # R5900 3-operand
  12dadc: 0a 00 6c 84  	lh	$12, 0xa($3)
  12dae0: 18 58 eb 00  	mult	$t3, $a3, $t3  # R5900 3-operand
  12dae4: 10 00 69 84  	lh	$9, 0x10($3)
  12dae8: 18 50 ea 00  	mult	$t2, $a3, $t2  # R5900 3-operand
  12daec: 0c 00 68 84  	lh	$8, 0xc($3)
  12daf0: 18 38 ec 00  	mult	$a3, $a3, $t4  # R5900 3-operand
  12daf4: 0e 00 63 84  	lh	$3, 0xe($3)
  12daf8: 18 48 49 00  	mult	$t1, $v0, $t1  # R5900 3-operand
  12dafc: 18 40 48 00  	mult	$t0, $v0, $t0  # R5900 3-operand
  12db00: c3 2b 05 00  	sra	$5, $5, 0xf
  12db04: 18 10 43 00  	mult	$v0, $v0, $v1  # R5900 3-operand
  12db08: c3 5b 0b 00  	sra	$11, $11, 0xf
  12db0c: c3 33 06 00  	sra	$6, $6, 0xf
  12db10: c3 53 0a 00  	sra	$10, $10, 0xf
  12db14: 21 28 ab 00  	addu	$5, $5, $11
  12db18: 21 30 ca 00  	addu	$6, $6, $10
  12db1c: c3 43 08 00  	sra	$8, $8, 0xf
  12db20: c3 23 04 00  	sra	$4, $4, 0xf
  12db24: c3 13 02 00  	sra	$2, $2, 0xf
  12db28: c3 3b 07 00  	sra	$7, $7, 0xf
  12db2c: 21 30 c2 00  	addu	$6, $6, $2
  12db30: 21 28 a8 00  	addu	$5, $5, $8
  12db34: 34 00 02 3c  	lui	$2, 0x34
  12db38: 21 20 87 00  	addu	$4, $4, $7
  12db3c: c3 4b 09 00  	sra	$9, $9, 0xf
  12db40: 9e 15 45 a4  	sh	$5, 0x159e($2)
  12db44: 34 00 02 3c  	lui	$2, 0x34
  12db48: 21 20 89 00  	addu	$4, $4, $9
  12db4c: a0 15 46 a4  	sh	$6, 0x15a0($2)
  12db50: 34 00 02 3c  	lui	$2, 0x34
  12db54: 08 00 e0 03  	jr	$ra
  12db58: a2 15 44 a4  	sh	$4, 0x15a2($2)

===== TARGET 0x0012db5c reachable=48 span=0xc0 =====
  12db5c: 34 00 05 3c  	lui	$5, 0x34
  12db60: 34 00 02 3c  	lui	$2, 0x34
  12db64: 00 15 a3 24  	addiu	$3, $5, 0x1500
  12db68: a4 15 44 84  	lh	$4, 0x15a4($2)
  12db6c: 04 00 68 84  	lh	$8, 0x4($3)
  12db70: 34 00 02 3c  	lui	$2, 0x34
  12db74: 02 00 66 84  	lh	$6, 0x2($3)
  12db78: 00 15 a5 84  	lh	$5, 0x1500($5)
  12db7c: a6 15 47 84  	lh	$7, 0x15a6($2)
  12db80: 18 30 86 00  	mult	$a2, $a0, $a2  # R5900 3-operand
  12db84: 06 00 6b 84  	lh	$11, 0x6($3)
  12db88: 34 00 02 3c  	lui	$2, 0x34
  12db8c: 08 00 6a 84  	lh	$10, 0x8($3)
  12db90: 18 28 85 00  	mult	$a1, $a0, $a1  # R5900 3-operand
  12db94: a8 15 42 84  	lh	$2, 0x15a8($2)
  12db98: 18 20 88 00  	mult	$a0, $a0, $t0  # R5900 3-operand
  12db9c: 0a 00 6c 84  	lh	$12, 0xa($3)
  12dba0: 18 58 eb 00  	mult	$t3, $a3, $t3  # R5900 3-operand
  12dba4: 10 00 69 84  	lh	$9, 0x10($3)
  12dba8: 18 50 ea 00  	mult	$t2, $a3, $t2  # R5900 3-operand
  12dbac: 0c 00 68 84  	lh	$8, 0xc($3)
  12dbb0: 18 38 ec 00  	mult	$a3, $a3, $t4  # R5900 3-operand
  12dbb4: 0e 00 63 84  	lh	$3, 0xe($3)
  12dbb8: 18 48 49 00  	mult	$t1, $v0, $t1  # R5900 3-operand
  12dbbc: 18 40 48 00  	mult	$t0, $v0, $t0  # R5900 3-operand
  12dbc0: c3 2b 05 00  	sra	$5, $5, 0xf
  12dbc4: 18 10 43 00  	mult	$v0, $v0, $v1  # R5900 3-operand
  12dbc8: c3 5b 0b 00  	sra	$11, $11, 0xf
  12dbcc: c3 33 06 00  	sra	$6, $6, 0xf
  12dbd0: c3 53 0a 00  	sra	$10, $10, 0xf
  12dbd4: 21 28 ab 00  	addu	$5, $5, $11
  12dbd8: 21 30 ca 00  	addu	$6, $6, $10
  12dbdc: c3 43 08 00  	sra	$8, $8, 0xf
  12dbe0: c3 23 04 00  	sra	$4, $4, 0xf
  12dbe4: c3 13 02 00  	sra	$2, $2, 0xf
  12dbe8: c3 3b 07 00  	sra	$7, $7, 0xf
  12dbec: 21 30 c2 00  	addu	$6, $6, $2
  12dbf0: 21 28 a8 00  	addu	$5, $5, $8
  12dbf4: 34 00 02 3c  	lui	$2, 0x34
  12dbf8: 21 20 87 00  	addu	$4, $4, $7
  12dbfc: c3 4b 09 00  	sra	$9, $9, 0xf
  12dc00: aa 15 45 a4  	sh	$5, 0x15aa($2)
  12dc04: 34 00 02 3c  	lui	$2, 0x34
  12dc08: 21 20 89 00  	addu	$4, $4, $9
  12dc0c: ac 15 46 a4  	sh	$6, 0x15ac($2)
  12dc10: 34 00 02 3c  	lui	$2, 0x34
  12dc14: 08 00 e0 03  	jr	$ra
  12dc18: ae 15 44 a4  	sh	$4, 0x15ae($2)

===== TARGET 0x00173f24 reachable=11 span=0x2c =====
  173f24: e0 00 03 24  	addiu	$3, $zero, 0xe0
  173f28: 35 00 02 3c  	lui	$2, 0x35
  173f2c: 18 40 83 00  	mult	$t0, $a0, $v1  # R5900 3-operand
  173f30: 80 db 42 24  	addiu	$2, $2, -0x2480 <.text+0xffffffffffefdb80>
  173f34: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  173f38: 00 00 bf ff  	sd	$ra, 0x0($sp)
  173f3c: 9b cf 05 0c  	jal	0x173e6c <.text+0x73e6c>
  173f40: 21 20 02 01  	addu	$4, $8, $2
  173f44: 00 00 bf df  	ld	$ra, 0x0($sp)
  173f48: 08 00 e0 03  	jr	$ra
  173f4c: 10 00 bd 27  	addiu	$sp, $sp, 0x10

===== TARGET 0x00173f50 reachable=40 span=0xa0 =====
  173f50: e0 00 03 24  	addiu	$3, $zero, 0xe0
  173f54: 3b 00 02 3c  	lui	$2, 0x3b
  173f58: 18 50 83 00  	mult	$t2, $a0, $v1  # R5900 3-operand
  173f5c: 18 b7 42 24  	addiu	$2, $2, -0x48e8 <.text+0xffffffffffefb718>
  173f60: 18 00 47 8c  	lw	$7, 0x18($2)
  173f64: 00 2c 05 00  	sll	$5, $5, 0x10
  173f68: 35 00 02 3c  	lui	$2, 0x35
  173f6c: 00 34 06 00  	sll	$6, $6, 0x10
  173f70: 03 2c 05 00  	sra	$5, $5, 0x10
  173f74: 80 db 42 24  	addiu	$2, $2, -0x2480 <.text+0xffffffffffefdb80>
  173f78: 02 00 a1 04  	bgez	$5, 0x173f84 <.text+0x73f84>
  173f7c: 2d 48 a0 00  	move	$9, $5
  173f80: 23 48 09 00  	negu	$9, $9
  173f84: 21 40 42 01  	addu	$8, $10, $2
  173f88: 09 00 e0 14  	bnez	$7, 0x173fb0 <.text+0x73fb0>
  173f8c: 03 34 06 00  	sra	$6, $6, 0x10
  173f90: 02 00 c1 04  	bgez	$6, 0x173f9c <.text+0x73f9c>
  173f94: 2d 10 c0 00  	move	$2, $6
  173f98: 23 10 02 00  	negu	$2, $2
  173f9c: 21 10 49 00  	addu	$2, $2, $9
  173fa0: c2 1f 02 00  	srl	$3, $2, 0x1f
  173fa4: 21 10 43 00  	addu	$2, $2, $3
  173fa8: c0 13 02 00  	sll	$2, $2, 0xf
  173fac: 03 2c 02 00  	sra	$5, $2, 0x10
  173fb0: 1c 00 03 8d  	lw	$3, 0x1c($8)
  173fb4: 08 00 05 a5  	sh	$5, 0x8($8)
  173fb8: 18 10 66 00  	mult	$v0, $v1, $a2  # R5900 3-operand
  173fbc: 0a 00 06 a5  	sh	$6, 0xa($8)
  173fc0: 18 18 65 00  	mult	$v1, $v1, $a1  # R5900 3-operand
  173fc4: 00 00 44 28  	slti	$4, $2, 0x0
  173fc8: 7f 00 45 24  	addiu	$5, $2, 0x7f
  173fcc: 0b 10 a4 00  	movn	$2, $5, $4
  173fd0: 00 00 64 28  	slti	$4, $3, 0x0
  173fd4: 7f 00 65 24  	addiu	$5, $3, 0x7f
  173fd8: c3 11 02 00  	sra	$2, $2, 0x7
  173fdc: 0b 18 a4 00  	movn	$3, $5, $4
  173fe0: 22 00 02 a5  	sh	$2, 0x22($8)
  173fe4: c3 19 03 00  	sra	$3, $3, 0x7
  173fe8: 08 00 e0 03  	jr	$ra
  173fec: 20 00 03 a5  	sh	$3, 0x20($8)

===== TARGET 0x0017422c reachable=34 span=0x88 =====
  17422c: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  174230: 3b 00 02 3c  	lui	$2, 0x3b
  174234: 00 00 bf ff  	sd	$ra, 0x0($sp)
  174238: 18 b7 42 24  	addiu	$2, $2, -0x48e8 <.text+0xffffffffffefb718>
  17423c: 00 7d 03 24  	addiu	$3, $zero, 0x7d00
  174240: 08 00 45 8c  	lw	$5, 0x8($2)
  174244: 01 00 60 50  	beqzl	$3, 0x17424c <.text+0x7424c>
  174248: cd 01 00 00  	break	0x0, 0x7
  17424c: 18 00 46 8c  	lw	$6, 0x18($2)
  174250: 35 00 02 3c  	lui	$2, 0x35
  174254: 18 20 85 00  	mult	$a0, $a0, $a1  # R5900 3-operand
  174258: 50 db 45 24  	addiu	$5, $2, -0x24b0 <.text+0xffffffffffefdb50>
  17425c: 40 22 04 00  	sll	$4, $4, 0x9
  174260: 1a 00 83 00  	div	$zero, $4, $3
  174264: 12 20 00 00  	mflo	$4
  174268: 40 10 04 00  	sll	$2, $4, 0x1
  17426c: 02 00 c0 10  	beqz	$6, 0x174278 <.text+0x74278>
  174270: 14 00 a4 ac  	sw	$4, 0x14($5)
  174274: 14 00 a2 ac  	sw	$2, 0x14($5)
  174278: 14 00 a3 8c  	lw	$3, 0x14($5)
  17427c: 07 00 60 50  	beqzl	$3, 0x17429c <.text+0x7429c>
  174280: 10 00 a0 ac  	sw	$zero, 0x10($5)
  174284: 10 00 a2 8c  	lw	$2, 0x10($5)
  174288: 01 00 60 50  	beqzl	$3, 0x174290 <.text+0x74290>
  17428c: cd 01 00 00  	break	0x0, 0x7
  174290: 1a 00 43 00  	div	$zero, $2, $3
  174294: 10 10 00 00  	mfhi	$2
  174298: 10 00 a2 ac  	sw	$2, 0x10($5)
  17429c: 34 00 02 3c  	lui	$2, 0x34
  1742a0: 48 d0 05 0c  	jal	0x174120 <.text+0x74120>
  1742a4: 10 54 44 90  	lbu	$4, 0x5410($2)
  1742a8: 00 00 bf df  	ld	$ra, 0x0($sp)
  1742ac: 08 00 e0 03  	jr	$ra
  1742b0: 10 00 bd 27  	addiu	$sp, $sp, 0x10

===== TARGET 0x001742f8 reachable=22 span=0x58 =====
  1742f8: e0 00 03 24  	addiu	$3, $zero, 0xe0
  1742fc: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  174300: 18 28 83 00  	mult	$a1, $a0, $v1  # R5900 3-operand
  174304: 35 00 02 3c  	lui	$2, 0x35
  174308: 80 db 42 24  	addiu	$2, $2, -0x2480 <.text+0xffffffffffefdb80>
  17430c: 00 00 bf ff  	sd	$ra, 0x0($sp)
  174310: 04 00 08 24  	addiu	$8, $zero, 0x4
  174314: ff ff 06 24  	addiu	$6, $zero, -0x1 <.text+0xffffffffffefffff>
  174318: 2d 38 00 00  	move	$7, $zero
  17431c: 21 18 a2 00  	addu	$3, $5, $2
  174320: 08 00 05 24  	addiu	$5, $zero, 0x8
  174324: 00 00 62 8c  	lw	$2, 0x0($3)
  174328: 04 00 40 14  	bnez	$2, 0x17433c <.text+0x7433c>
  17432c: 2d 20 60 00  	move	$4, $3
  174330: 00 00 bf df  	ld	$ra, 0x0($sp)
  174334: 08 00 e0 03  	jr	$ra
  174338: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  17433c: a4 00 68 ac  	sw	$8, 0xa4($3)
  174340: 9b cf 05 0c  	jal	0x173e6c <.text+0x73e6c>
  174344: 00 00 68 ac  	sw	$8, 0x0($3)
  174348: fa ff 00 10  	b	0x174334 <.text+0x74334>
  17434c: 00 00 bf df  	ld	$ra, 0x0($sp)

===== TARGET 0x00174618 reachable=35 span=0x8c =====
  174618: e0 00 03 24  	addiu	$3, $zero, 0xe0
  17461c: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  174620: 18 38 83 00  	mult	$a3, $a0, $v1  # R5900 3-operand
  174624: 35 00 02 3c  	lui	$2, 0x35
  174628: 00 00 bf ff  	sd	$ra, 0x0($sp)
  17462c: 80 db 42 24  	addiu	$2, $2, -0x2480 <.text+0xffffffffffefdb80>
  174630: 00 36 05 00  	sll	$6, $5, 0x18
  174634: 21 50 e2 00  	addu	$10, $7, $2
  174638: 08 00 42 85  	lh	$2, 0x8($10)
  17463c: 0a 00 43 85  	lh	$3, 0xa($10)
  174640: 18 10 a2 00  	mult	$v0, $a1, $v0  # R5900 3-operand
  174644: a8 00 46 ad  	sw	$6, 0xa8($10)
  174648: 18 18 a3 00  	mult	$v1, $a1, $v1  # R5900 3-operand
  17464c: 1c 00 45 ad  	sw	$5, 0x1c($10)
  174650: 7f 00 49 24  	addiu	$9, $2, 0x7f
  174654: 00 00 47 28  	slti	$7, $2, 0x0
  174658: 7f 00 68 24  	addiu	$8, $3, 0x7f
  17465c: 00 00 66 28  	slti	$6, $3, 0x0
  174660: 0b 10 27 01  	movn	$2, $9, $7
  174664: 0b 18 06 01  	movn	$3, $8, $6
  174668: c3 11 02 00  	sra	$2, $2, 0x7
  17466c: c3 19 03 00  	sra	$3, $3, 0x7
  174670: 20 00 42 a5  	sh	$2, 0x20($10)
  174674: 08 00 a0 14  	bnez	$5, 0x174698 <.text+0x74698>
  174678: 22 00 43 a5  	sh	$3, 0x22($10)
  17467c: 00 00 43 8d  	lw	$3, 0x0($10)
  174680: 05 00 60 10  	beqz	$3, 0x174698 <.text+0x74698>
  174684: 05 00 02 24  	addiu	$2, $zero, 0x5
  174688: 03 00 62 10  	beq	$3, $2, 0x174698 <.text+0x74698>
  17468c: 2d 28 40 01  	move	$5, $10
  174690: 7f cf 05 0c  	jal	0x173dfc <.text+0x73dfc>
  174694: 00 00 00 00  	nop
  174698: 00 00 bf df  	ld	$ra, 0x0($sp)
  17469c: 08 00 e0 03  	jr	$ra
  1746a0: 10 00 bd 27  	addiu	$sp, $sp, 0x10

===== TARGET 0x001746a4 reachable=31 span=0x7c =====
  1746a4: e0 00 03 24  	addiu	$3, $zero, 0xe0
  1746a8: 34 00 02 3c  	lui	$2, 0x34
  1746ac: 18 40 83 00  	mult	$t0, $a0, $v1  # R5900 3-operand
  1746b0: 58 55 45 90  	lbu	$5, 0x5558($2)
  1746b4: 35 00 02 3c  	lui	$2, 0x35
  1746b8: 2d 38 00 00  	move	$7, $zero
  1746bc: 50 db 42 24  	addiu	$2, $2, -0x24b0 <.text+0xffffffffffefdb50>
  1746c0: 21 30 02 01  	addu	$6, $8, $2
  1746c4: 05 00 a0 14  	bnez	$5, 0x1746dc <.text+0x746dc>
  1746c8: 2d 20 c0 00  	move	$4, $6
  1746cc: 36 00 02 3c  	lui	$2, 0x36
  1746d0: 3b b7 42 90  	lbu	$2, -0x48c5($2)
  1746d4: 0a 00 40 10  	beqz	$2, 0x174700 <.text+0x74700>
  1746d8: 36 00 02 3c  	lui	$2, 0x36
  1746dc: 30 00 83 8c  	lw	$3, 0x30($4)
  1746e0: 07 00 60 10  	beqz	$3, 0x174700 <.text+0x74700>
  1746e4: 36 00 02 3c  	lui	$2, 0x36
  1746e8: 05 00 02 24  	addiu	$2, $zero, 0x5
  1746ec: 04 00 62 50  	beql	$3, $2, 0x174700 <.text+0x74700>
  1746f0: 36 00 02 3c  	lui	$2, 0x36
  1746f4: 4c 00 87 8c  	lw	$7, 0x4c($4)
  1746f8: 08 00 e0 03  	jr	$ra
  1746fc: 2d 10 e0 00  	move	$2, $7
  174700: 3b b7 42 90  	lbu	$2, -0x48c5($2)
  174704: fc ff 40 10  	beqz	$2, 0x1746f8 <.text+0x746f8>
  174708: 00 00 00 00  	nop
  17470c: 30 00 c2 8c  	lw	$2, 0x30($6)
  174710: f9 ff 40 54  	bnezl	$2, 0x1746f8 <.text+0x746f8>
  174714: 4c 00 c7 8c  	lw	$7, 0x4c($6)
  174718: f7 ff 00 10  	b	0x1746f8 <.text+0x746f8>
  17471c: 00 00 00 00  	nop

===== TARGET 0x00174830 reachable=12 span=0x30 =====
  174830: e0 00 06 24  	addiu	$6, $zero, 0xe0
  174834: 35 00 02 3c  	lui	$2, 0x35
  174838: 18 38 86 00  	mult	$a3, $a0, $a2  # R5900 3-operand
  17483c: 50 db 42 24  	addiu	$2, $2, -0x24b0 <.text+0xffffffffffefdb50>
  174840: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  174844: 00 00 bf ff  	sd	$ra, 0x0($sp)
  174848: 21 18 e2 00  	addu	$3, $7, $2
  17484c: ca d1 05 0c  	jal	0x174728 <.text+0x74728>
  174850: 3c 00 65 ac  	sw	$5, 0x3c($3)
  174854: 00 00 bf df  	ld	$ra, 0x0($sp)
  174858: 08 00 e0 03  	jr	$ra
  17485c: 10 00 bd 27  	addiu	$sp, $sp, 0x10

===== TARGET 0x00103d90 reachable=17 span=0x44 =====
  103d90: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  103d94: 42 00 02 3c  	lui	$2, 0x42
  103d98: 00 00 bf ff  	sd	$ra, 0x0($sp)
  103d9c: 1c 00 06 3c  	lui	$6, 0x1c
  103da0: 40 b3 c6 24  	addiu	$6, $6, -0x4cc0 <.text+0xffffffffffefb340>
  103da4: 10 70 43 8c  	lw	$3, 0x7010($2)
  103da8: 90 00 02 24  	addiu	$2, $zero, 0x90
  103dac: 18 20 82 00  	mult	$a0, $a0, $v0  # R5900 3-operand
  103db0: 21 38 83 00  	addu	$7, $4, $3
  103db4: 2d 20 a0 00  	move	$4, $5
  103db8: 1b 00 05 3c  	lui	$5, 0x1b
  103dbc: 7c ff e7 24  	addiu	$7, $7, -0x84 <.text+0xffffffffffefff7c>
  103dc0: f4 78 06 0c  	jal	0x19e3d0 <.text+0x9e3d0>
  103dc4: a0 0d a5 24  	addiu	$5, $5, 0xda0
  103dc8: 00 00 bf df  	ld	$ra, 0x0($sp)
  103dcc: 08 00 e0 03  	jr	$ra
  103dd0: 10 00 bd 27  	addiu	$sp, $sp, 0x10

===== TARGET 0x00172174 reachable=30 span=0x78 =====
  172174: c0 fd bd 27  	addiu	$sp, $sp, -0x240 <.text+0xffffffffffeffdc0>
  172178: 2d 10 a0 00  	move	$2, $5
  17217c: 1c 00 05 3c  	lui	$5, 0x1c
  172180: 20 02 b2 ff  	sd	$18, 0x220($sp)
  172184: 00 02 b0 ff  	sd	$16, 0x200($sp)
  172188: 80 83 a5 24  	addiu	$5, $5, -0x7c80 <.text+0xffffffffffef8380>
  17218c: 2d 80 80 00  	move	$16, $4
  172190: 2d 90 c0 00  	move	$18, $6
  172194: 2d 20 a0 03  	move	$4, $sp
  172198: 2d 30 40 00  	move	$6, $2
  17219c: 30 02 bf ff  	sd	$ra, 0x230($sp)
  1721a0: 10 02 b1 ff  	sd	$17, 0x210($sp)
  1721a4: f4 78 06 0c  	jal	0x19e3d0 <.text+0x9e3d0>
  1721a8: 2d 88 e0 00  	move	$17, $7
  1721ac: 7a 71 06 0c  	jal	0x19c5e8 <.text+0x9c5e8>
  1721b0: 2d 20 a0 03  	move	$4, $sp
  1721b4: 2d 28 a0 03  	move	$5, $sp
  1721b8: 2d 20 00 02  	move	$4, $16
  1721bc: 6f 4f 06 0c  	jal	0x193dbc <.text+0x93dbc>
  1721c0: 2d 30 40 00  	move	$6, $2
  1721c4: 2d 30 20 02  	move	$6, $17
  1721c8: 2d 20 00 02  	move	$4, $16
  1721cc: 6f 4f 06 0c  	jal	0x193dbc <.text+0x93dbc>
  1721d0: 2d 28 40 02  	move	$5, $18
  1721d4: 00 02 b0 df  	ld	$16, 0x200($sp)
  1721d8: 30 02 bf df  	ld	$ra, 0x230($sp)
  1721dc: 20 02 b2 df  	ld	$18, 0x220($sp)
  1721e0: 10 02 b1 df  	ld	$17, 0x210($sp)
  1721e4: 08 00 e0 03  	jr	$ra
  1721e8: 40 02 bd 27  	addiu	$sp, $sp, 0x240

===== TARGET 0x00106824 reachable=30 span=0x78 =====
  106824: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  106828: 2d 20 00 00  	move	$4, $zero
  10682c: 20 00 b1 ff  	sd	$17, 0x20($sp)
  106830: 1f 00 11 3c  	lui	$17, 0x1f
  106834: c8 c1 22 8e  	lw	$2, -0x3e38($17)
  106838: 10 00 b0 ff  	sd	$16, 0x10($sp)
  10683c: 01 00 10 24  	addiu	$16, $zero, 0x1
  106840: 04 00 50 10  	beq	$2, $16, 0x106854 <.text+0x6854>
  106844: 30 00 bf ff  	sd	$ra, 0x30($sp)
  106848: 3b 82 06 0c  	jal	0x1a08ec <.text+0xa08ec>
  10684c: 00 00 00 00  	nop
  106850: c8 c1 30 ae  	sw	$16, -0x3e38($17)
  106854: 2d 30 a0 03  	move	$6, $sp
  106858: 04 00 a7 27  	addiu	$7, $sp, 0x4
  10685c: 08 00 a8 27  	addiu	$8, $sp, 0x8
  106860: 2d 20 00 00  	move	$4, $zero
  106864: 9a 82 06 0c  	jal	0x1a0a68 <.text+0xa0a68>
  106868: 2d 28 00 00  	move	$5, $zero
  10686c: 2d 28 00 00  	move	$5, $zero
  106870: 0c 00 a6 27  	addiu	$6, $sp, 0xc
  106874: 5d 86 06 0c  	jal	0x1a1974 <.text+0xa1974>
  106878: 2d 20 00 00  	move	$4, $zero
  10687c: 10 00 b0 df  	ld	$16, 0x10($sp)
  106880: 00 00 a2 8f  	lw	$2, 0x0($sp)
  106884: 30 00 bf df  	ld	$ra, 0x30($sp)
  106888: 02 00 42 38  	xori	$2, $2, 0x2
  10688c: 20 00 b1 df  	ld	$17, 0x20($sp)
  106890: 01 00 42 2c  	sltiu	$2, $2, 0x1
  106894: 08 00 e0 03  	jr	$ra
  106898: 40 00 bd 27  	addiu	$sp, $sp, 0x40

===== TARGET 0x0016fb04 reachable=44 span=0xb0 =====
  16fb04: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  16fb08: 36 00 02 3c  	lui	$2, 0x36
  16fb0c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  16fb10: 00 80 03 34  	ori	$3, $zero, 0x8000
  16fb14: 28 93 52 24  	addiu	$18, $2, -0x6cd8 <.text+0xffffffffffef9328>
  16fb18: 10 00 b1 ff  	sd	$17, 0x10($sp)
  16fb1c: 35 00 02 3c  	lui	$2, 0x35
  16fb20: 30 00 bf ff  	sd	$ra, 0x30($sp)
  16fb24: 00 00 b0 ff  	sd	$16, 0x0($sp)
  16fb28: b0 e2 42 24  	addiu	$2, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  16fb2c: 21 88 43 00  	addu	$17, $2, $3
  16fb30: 17 00 07 3c  	lui	$7, 0x17
  16fb34: 74 30 22 8e  	lw	$2, 0x3074($17)
  16fb38: c4 fa e7 24  	addiu	$7, $7, -0x53c <.text+0xffffffffffeffac4>
  16fb3c: 70 30 23 8e  	lw	$3, 0x3070($17)
  16fb40: 08 00 06 24  	addiu	$6, $zero, 0x8
  16fb44: 2d 20 40 02  	move	$4, $18
  16fb48: 14 00 43 10  	beq	$2, $3, 0x16fb9c <.text+0x6fb9c>
  16fb4c: 2d 28 40 00  	move	$5, $2
  16fb50: 33 20 04 0c  	jal	0x1080cc <.text+0x80cc>
  16fb54: 00 00 00 00  	nop
  16fb58: 1c 00 04 3c  	lui	$4, 0x1c
  16fb5c: 49 06 04 0c  	jal	0x101924 <.text+0x1924>
  16fb60: 50 80 84 24  	addiu	$4, $4, -0x7fb0 <.text+0xffffffffffef8050>
  16fb64: 02 02 05 24  	addiu	$5, $zero, 0x202
  16fb68: f0 73 06 0c  	jal	0x19cfc0 <.text+0x9cfc0>
  16fb6c: 2d 20 40 00  	move	$4, $2
  16fb70: 2d 28 40 02  	move	$5, $18
  16fb74: 2d 80 40 00  	move	$16, $2
  16fb78: 06 00 40 04  	bltz	$2, 0x16fb94 <.text+0x6fb94>
  16fb7c: 2d 20 40 00  	move	$4, $2
  16fb80: 74 30 26 8e  	lw	$6, 0x3074($17)
  16fb84: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  16fb88: c0 30 06 00  	sll	$6, $6, 0x3
  16fb8c: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  16fb90: 2d 20 00 02  	move	$4, $16
  16fb94: 74 30 22 8e  	lw	$2, 0x3074($17)
  16fb98: 70 30 22 ae  	sw	$2, 0x3070($17)
  16fb9c: 30 00 bf df  	ld	$ra, 0x30($sp)
  16fba0: 20 00 b2 df  	ld	$18, 0x20($sp)
  16fba4: 10 00 b1 df  	ld	$17, 0x10($sp)
  16fba8: 00 00 b0 df  	ld	$16, 0x0($sp)
  16fbac: 08 00 e0 03  	jr	$ra
  16fbb0: 40 00 bd 27  	addiu	$sp, $sp, 0x40

===== TARGET 0x0016fbb4 reachable=37 span=0x94 =====
  16fbb4: 1c 00 04 3c  	lui	$4, 0x1c
  16fbb8: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  16fbbc: 50 80 84 24  	addiu	$4, $4, -0x7fb0 <.text+0xffffffffffef8050>
  16fbc0: 20 00 bf ff  	sd	$ra, 0x20($sp)
  16fbc4: 10 00 b1 ff  	sd	$17, 0x10($sp)
  16fbc8: 49 06 04 0c  	jal	0x101924 <.text+0x1924>
  16fbcc: 00 00 b0 ff  	sd	$16, 0x0($sp)
  16fbd0: 2d 20 40 00  	move	$4, $2
  16fbd4: f0 73 06 0c  	jal	0x19cfc0 <.text+0x9cfc0>
  16fbd8: 01 00 05 24  	addiu	$5, $zero, 0x1
  16fbdc: 00 80 03 34  	ori	$3, $zero, 0x8000
  16fbe0: 2d 80 40 00  	move	$16, $2
  16fbe4: 2d 20 40 00  	move	$4, $2
  16fbe8: 35 00 02 3c  	lui	$2, 0x35
  16fbec: 36 00 05 3c  	lui	$5, 0x36
  16fbf0: b0 e2 42 24  	addiu	$2, $2, -0x1d50 <.text+0xffffffffffefe2b0>
  16fbf4: 28 93 a5 24  	addiu	$5, $5, -0x6cd8 <.text+0xffffffffffef9328>
  16fbf8: 21 88 43 00  	addu	$17, $2, $3
  16fbfc: 01 00 06 3c  	lui	$6, 0x1
  16fc00: 70 30 20 ae  	sw	$zero, 0x3070($17)
  16fc04: 0b 00 00 06  	bltz	$16, 0x16fc34 <.text+0x6fc34>
  16fc08: 74 30 20 ae  	sw	$zero, 0x3074($17)
  16fc0c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  16fc10: 00 00 00 00  	nop
  16fc14: 2d 18 40 00  	move	$3, $2
  16fc18: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  16fc1c: 03 00 62 10  	beq	$3, $2, 0x16fc2c <.text+0x6fc2c>
  16fc20: 2d 20 00 02  	move	$4, $16
  16fc24: 74 30 23 ae  	sw	$3, 0x3074($17)
  16fc28: 70 30 23 ae  	sw	$3, 0x3070($17)
  16fc2c: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  16fc30: 00 00 00 00  	nop
  16fc34: 20 00 bf df  	ld	$ra, 0x20($sp)
  16fc38: 10 00 b1 df  	ld	$17, 0x10($sp)
  16fc3c: 00 00 b0 df  	ld	$16, 0x0($sp)
  16fc40: 08 00 e0 03  	jr	$ra
  16fc44: 30 00 bd 27  	addiu	$sp, $sp, 0x30

===== TARGET 0x00183c58 reachable=58 span=0xe8 =====
  183c58: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  183c5c: 34 00 02 3c  	lui	$2, 0x34
  183c60: 30 00 bf ff  	sd	$ra, 0x30($sp)
  183c64: 20 00 b2 ff  	sd	$18, 0x20($sp)
  183c68: 10 00 b1 ff  	sd	$17, 0x10($sp)
  183c6c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  183c70: 34 55 42 90  	lbu	$2, 0x5534($2)
  183c74: 06 00 40 14  	bnez	$2, 0x183c90 <.text+0x83c90>
  183c78: 30 00 bf df  	ld	$ra, 0x30($sp)
  183c7c: 20 00 b2 df  	ld	$18, 0x20($sp)
  183c80: 10 00 b1 df  	ld	$17, 0x10($sp)
  183c84: 00 00 b0 df  	ld	$16, 0x0($sp)
  183c88: 08 00 e0 03  	jr	$ra
  183c8c: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  183c90: f3 0d 06 0c  	jal	0x1837cc <.text+0x837cc>
  183c94: 2d 90 00 00  	move	$18, $zero
  183c98: 35 00 02 3c  	lui	$2, 0x35
  183c9c: d4 e2 42 90  	lbu	$2, -0x1d2c($2)
  183ca0: 03 00 40 10  	beqz	$2, 0x183cb0 <.text+0x83cb0>
  183ca4: 03 00 43 24  	addiu	$3, $2, 0x3
  183ca8: 80 00 02 24  	addiu	$2, $zero, 0x80
  183cac: 04 90 62 00  	sllv	$18, $2, $3
  183cb0: 35 00 11 3c  	lui	$17, 0x35
  183cb4: 02 00 05 3c  	lui	$5, 0x2
  183cb8: 98 e2 22 8e  	lw	$2, -0x1d68($17)
  183cbc: 2a 18 b2 00  	slt	$3, $5, $18
  183cc0: 42 00 04 3c  	lui	$4, 0x42
  183cc4: 0b 90 a3 00  	movn	$18, $5, $3
  183cc8: 58 38 90 24  	addiu	$16, $4, 0x3858
  183ccc: 21 10 52 00  	addu	$2, $2, $18
  183cd0: 58 38 84 90  	lbu	$4, 0x3858($4)
  183cd4: 0d 00 06 24  	addiu	$6, $zero, 0xd
  183cd8: 02 00 05 26  	addiu	$5, $16, 0x2
  183cdc: 00 00 44 a0  	sb	$4, 0x0($2)
  183ce0: 98 e2 22 8e  	lw	$2, -0x1d68($17)
  183ce4: 01 00 03 92  	lbu	$3, 0x1($16)
  183ce8: 21 10 52 00  	addu	$2, $2, $18
  183cec: 01 00 43 a0  	sb	$3, 0x1($2)
  183cf0: 98 e2 24 8e  	lw	$4, -0x1d68($17)
  183cf4: 21 20 92 00  	addu	$4, $4, $18
  183cf8: 28 71 06 0c  	jal	0x19c4a0 <.text+0x9c4a0>
  183cfc: 02 00 84 24  	addiu	$4, $4, 0x2
  183d00: 0f 00 03 92  	lbu	$3, 0xf($16)
  183d04: 98 e2 22 8e  	lw	$2, -0x1d68($17)
  183d08: 08 00 06 24  	addiu	$6, $zero, 0x8
  183d0c: 14 00 05 26  	addiu	$5, $16, 0x14
  183d10: 21 10 52 00  	addu	$2, $2, $18
  183d14: 0f 00 43 a0  	sb	$3, 0xf($2)
  183d18: 98 e2 22 8e  	lw	$2, -0x1d68($17)
  183d1c: 10 00 03 92  	lbu	$3, 0x10($16)
  183d20: 21 10 52 00  	addu	$2, $2, $18
  183d24: 10 00 43 a0  	sb	$3, 0x10($2)
  183d28: 98 e2 24 8e  	lw	$4, -0x1d68($17)
  183d2c: 21 20 92 00  	addu	$4, $4, $18
  183d30: 28 71 06 0c  	jal	0x19c4a0 <.text+0x9c4a0>
  183d34: 11 00 84 24  	addiu	$4, $4, 0x11
  183d38: d0 ff 00 10  	b	0x183c7c <.text+0x83c7c>
  183d3c: 30 00 bf df  	ld	$ra, 0x30($sp)

===== TARGET 0x00183d40 reachable=49 span=0xc4 =====
  183d40: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  183d44: 34 00 02 3c  	lui	$2, 0x34
  183d48: 30 00 bf ff  	sd	$ra, 0x30($sp)
  183d4c: 20 00 b2 ff  	sd	$18, 0x20($sp)
  183d50: 10 00 b1 ff  	sd	$17, 0x10($sp)
  183d54: 00 00 b0 ff  	sd	$16, 0x0($sp)
  183d58: 34 55 42 90  	lbu	$2, 0x5534($2)
  183d5c: 23 00 40 10  	beqz	$2, 0x183dec <.text+0x83dec>
  183d60: 35 00 02 3c  	lui	$2, 0x35
  183d64: 2d 90 00 00  	move	$18, $zero
  183d68: d4 e2 42 90  	lbu	$2, -0x1d2c($2)
  183d6c: 03 00 40 10  	beqz	$2, 0x183d7c <.text+0x83d7c>
  183d70: 03 00 43 24  	addiu	$3, $2, 0x3
  183d74: 80 00 02 24  	addiu	$2, $zero, 0x80
  183d78: 04 90 62 00  	sllv	$18, $2, $3
  183d7c: 02 00 05 3c  	lui	$5, 0x2
  183d80: 35 00 11 3c  	lui	$17, 0x35
  183d84: 2a 18 b2 00  	slt	$3, $5, $18
  183d88: 98 e2 22 8e  	lw	$2, -0x1d68($17)
  183d8c: 0b 90 a3 00  	movn	$18, $5, $3
  183d90: 42 00 07 3c  	lui	$7, 0x42
  183d94: 21 10 52 00  	addu	$2, $2, $18
  183d98: 58 38 f0 24  	addiu	$16, $7, 0x3858
  183d9c: 00 00 43 90  	lbu	$3, 0x0($2)
  183da0: 02 00 45 24  	addiu	$5, $2, 0x2
  183da4: 0d 00 06 24  	addiu	$6, $zero, 0xd
  183da8: 02 00 04 26  	addiu	$4, $16, 0x2
  183dac: 58 38 e3 a0  	sb	$3, 0x3858($7)
  183db0: 01 00 42 90  	lbu	$2, 0x1($2)
  183db4: 28 71 06 0c  	jal	0x19c4a0 <.text+0x9c4a0>
  183db8: 01 00 02 a2  	sb	$2, 0x1($16)
  183dbc: 98 e2 22 8e  	lw	$2, -0x1d68($17)
  183dc0: 14 00 04 26  	addiu	$4, $16, 0x14
  183dc4: 08 00 06 24  	addiu	$6, $zero, 0x8
  183dc8: 21 10 52 00  	addu	$2, $2, $18
  183dcc: 0f 00 43 90  	lbu	$3, 0xf($2)
  183dd0: 11 00 45 24  	addiu	$5, $2, 0x11
  183dd4: 0f 00 03 a2  	sb	$3, 0xf($16)
  183dd8: 10 00 42 90  	lbu	$2, 0x10($2)
  183ddc: 28 71 06 0c  	jal	0x19c4a0 <.text+0x9c4a0>
  183de0: 10 00 02 a2  	sb	$2, 0x10($16)
  183de4: f3 0d 06 0c  	jal	0x1837cc <.text+0x837cc>
  183de8: 00 00 00 00  	nop
  183dec: 30 00 bf df  	ld	$ra, 0x30($sp)
  183df0: 20 00 b2 df  	ld	$18, 0x20($sp)
  183df4: 10 00 b1 df  	ld	$17, 0x10($sp)
  183df8: 00 00 b0 df  	ld	$16, 0x0($sp)
  183dfc: 08 00 e0 03  	jr	$ra
  183e00: 40 00 bd 27  	addiu	$sp, $sp, 0x40

