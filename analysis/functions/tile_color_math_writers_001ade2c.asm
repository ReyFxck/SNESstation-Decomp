  1ade2c: 36 00 02 3c  	lui	$2, 0x36
  1ade30: 2d 68 a0 00  	move	$13, $5
  1ade34: 80 d4 49 24  	addiu	$9, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ade38: 40 38 04 00  	sll	$7, $4, 0x1
  1ade3c: 08 00 22 8d  	lw	$2, 0x8($9)
  1ade40: 4c 00 26 91  	lbu	$6, 0x4c($9)
  1ade44: 21 58 44 00  	addu	$11, $2, $4
  1ade48: 3c 00 23 8d  	lw	$3, 0x3c($9)
  1ade4c: 00 00 62 91  	lbu	$2, 0x0($11)
  1ade50: 0c 00 25 8d  	lw	$5, 0xc($9)
  1ade54: 21 50 67 00  	addu	$10, $3, $7
  1ade58: 2b 10 46 00  	sltu	$2, $2, $6
  1ade5c: 26 00 40 10  	beqz	$2, 0x1adef8 <.text+0xadef8>
  1ade60: 21 60 a4 00  	addu	$12, $5, $4
  1ade64: 00 00 a3 91  	lbu	$3, 0x0($13)
  1ade68: 24 00 60 50  	beqzl	$3, 0x1adefc <.text+0xadefc>
  1ade6c: 36 00 03 3c  	lui	$3, 0x36
  1ade70: 00 00 84 91  	lbu	$4, 0x0($12)
  1ade74: 0e 01 80 50  	beqzl	$4, 0x1ae2b0 <.text+0xae2b0>
  1ade78: 44 00 22 8d  	lw	$2, 0x44($9)
  1ade7c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ade80: f4 00 82 10  	beq	$4, $2, 0x1ae254 <.text+0xae254>
  1ade84: de fb 06 24  	addiu	$6, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ade88: 14 00 22 8d  	lw	$2, 0x14($9)
  1ade8c: 40 18 03 00  	sll	$3, $3, 0x1
  1ade90: 44 00 24 8d  	lw	$4, 0x44($9)
  1ade94: 40 10 02 00  	sll	$2, $2, 0x1
  1ade98: 18 00 28 8d  	lw	$8, 0x18($9)
  1ade9c: 21 18 64 00  	addu	$3, $3, $4
  1adea0: 21 10 4a 00  	addu	$2, $2, $10
  1adea4: 00 00 47 94  	lhu	$7, 0x0($2)
  1adea8: 00 00 65 94  	lhu	$5, 0x0($3)
  1adeac: ff ff e4 30  	andi	$4, $7, 0xffff
  1adeb0: ff ff a3 30  	andi	$3, $5, 0xffff
  1adeb4: 26 28 a7 00  	xor	$5, $5, $7
  1adeb8: 24 10 66 00  	and	$2, $3, $6
  1adebc: 21 04 a5 30  	andi	$5, $5, 0x421
  1adec0: 24 30 86 00  	and	$6, $4, $6
  1adec4: 24 18 64 00  	and	$3, $3, $4
  1adec8: 21 10 46 00  	addu	$2, $2, $6
  1adecc: 21 04 63 30  	andi	$3, $3, 0x421
  1aded0: 43 10 02 00  	sra	$2, $2, 0x1
  1aded4: 21 10 43 00  	addu	$2, $2, $3
  1aded8: 40 10 02 00  	sll	$2, $2, 0x1
  1adedc: 21 10 48 00  	addu	$2, $2, $8
  1adee0: 00 00 42 94  	lhu	$2, 0x0($2)
  1adee4: 25 10 45 00  	or	$2, $2, $5
  1adee8: 00 00 42 a5  	sh	$2, 0x0($10)
  1adeec: 36 00 02 3c  	lui	$2, 0x36
  1adef0: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1adef4: 00 00 62 a1  	sb	$2, 0x0($11)
  1adef8: 36 00 03 3c  	lui	$3, 0x36
  1adefc: 01 00 62 91  	lbu	$2, 0x1($11)
  1adf00: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1adf04: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1adf08: 2b 10 43 00  	sltu	$2, $2, $3
  1adf0c: 27 00 40 10  	beqz	$2, 0x1adfac <.text+0xadfac>
  1adf10: 36 00 03 3c  	lui	$3, 0x36
  1adf14: 01 00 a3 91  	lbu	$3, 0x1($13)
  1adf18: 24 00 60 50  	beqzl	$3, 0x1adfac <.text+0xadfac>
  1adf1c: 36 00 03 3c  	lui	$3, 0x36
  1adf20: 01 00 84 91  	lbu	$4, 0x1($12)
  1adf24: c7 00 80 50  	beqzl	$4, 0x1ae244 <.text+0xae244>
  1adf28: 44 00 e2 8c  	lw	$2, 0x44($7)
  1adf2c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1adf30: ad 00 82 10  	beq	$4, $2, 0x1ae1e8 <.text+0xae1e8>
  1adf34: de fb 06 24  	addiu	$6, $zero, -0x422 <.text+0xffffffffffeffbde>
  1adf38: 14 00 e2 8c  	lw	$2, 0x14($7)
  1adf3c: 40 18 03 00  	sll	$3, $3, 0x1
  1adf40: 44 00 e4 8c  	lw	$4, 0x44($7)
  1adf44: 40 10 02 00  	sll	$2, $2, 0x1
  1adf48: 18 00 e8 8c  	lw	$8, 0x18($7)
  1adf4c: 21 18 64 00  	addu	$3, $3, $4
  1adf50: 21 10 4a 00  	addu	$2, $2, $10
  1adf54: 02 00 47 94  	lhu	$7, 0x2($2)
  1adf58: 00 00 65 94  	lhu	$5, 0x0($3)
  1adf5c: ff ff e4 30  	andi	$4, $7, 0xffff
  1adf60: ff ff a3 30  	andi	$3, $5, 0xffff
  1adf64: 26 28 a7 00  	xor	$5, $5, $7
  1adf68: 24 10 66 00  	and	$2, $3, $6
  1adf6c: 21 04 a5 30  	andi	$5, $5, 0x421
  1adf70: 24 30 86 00  	and	$6, $4, $6
  1adf74: 24 18 64 00  	and	$3, $3, $4
  1adf78: 21 10 46 00  	addu	$2, $2, $6
  1adf7c: 21 04 63 30  	andi	$3, $3, 0x421
  1adf80: 43 10 02 00  	sra	$2, $2, 0x1
  1adf84: 21 10 43 00  	addu	$2, $2, $3
  1adf88: 40 10 02 00  	sll	$2, $2, 0x1
  1adf8c: 21 10 48 00  	addu	$2, $2, $8
  1adf90: 00 00 42 94  	lhu	$2, 0x0($2)
  1adf94: 25 10 45 00  	or	$2, $2, $5
  1adf98: 02 00 42 a5  	sh	$2, 0x2($10)
  1adf9c: 36 00 02 3c  	lui	$2, 0x36
  1adfa0: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1adfa4: 01 00 62 a1  	sb	$2, 0x1($11)
  1adfa8: 36 00 03 3c  	lui	$3, 0x36
  1adfac: 02 00 62 91  	lbu	$2, 0x2($11)
  1adfb0: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1adfb4: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1adfb8: 2b 10 43 00  	sltu	$2, $2, $3
  1adfbc: 27 00 40 10  	beqz	$2, 0x1ae05c <.text+0xae05c>
  1adfc0: 36 00 03 3c  	lui	$3, 0x36
  1adfc4: 02 00 a3 91  	lbu	$3, 0x2($13)
  1adfc8: 24 00 60 50  	beqzl	$3, 0x1ae05c <.text+0xae05c>
  1adfcc: 36 00 03 3c  	lui	$3, 0x36
  1adfd0: 02 00 84 91  	lbu	$4, 0x2($12)
  1adfd4: 80 00 80 50  	beqzl	$4, 0x1ae1d8 <.text+0xae1d8>
  1adfd8: 44 00 e2 8c  	lw	$2, 0x44($7)
  1adfdc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1adfe0: 66 00 82 10  	beq	$4, $2, 0x1ae17c <.text+0xae17c>
  1adfe4: de fb 06 24  	addiu	$6, $zero, -0x422 <.text+0xffffffffffeffbde>
  1adfe8: 14 00 e2 8c  	lw	$2, 0x14($7)
  1adfec: 40 18 03 00  	sll	$3, $3, 0x1
  1adff0: 44 00 e4 8c  	lw	$4, 0x44($7)
  1adff4: 40 10 02 00  	sll	$2, $2, 0x1
  1adff8: 18 00 e8 8c  	lw	$8, 0x18($7)
  1adffc: 21 18 64 00  	addu	$3, $3, $4
  1ae000: 21 10 4a 00  	addu	$2, $2, $10
  1ae004: 04 00 47 94  	lhu	$7, 0x4($2)
  1ae008: 00 00 65 94  	lhu	$5, 0x0($3)
  1ae00c: ff ff e4 30  	andi	$4, $7, 0xffff
  1ae010: ff ff a3 30  	andi	$3, $5, 0xffff
  1ae014: 26 28 a7 00  	xor	$5, $5, $7
  1ae018: 24 10 66 00  	and	$2, $3, $6
  1ae01c: 21 04 a5 30  	andi	$5, $5, 0x421
  1ae020: 24 30 86 00  	and	$6, $4, $6
  1ae024: 24 18 64 00  	and	$3, $3, $4
  1ae028: 21 10 46 00  	addu	$2, $2, $6
  1ae02c: 21 04 63 30  	andi	$3, $3, 0x421
  1ae030: 43 10 02 00  	sra	$2, $2, 0x1
  1ae034: 21 10 43 00  	addu	$2, $2, $3
  1ae038: 40 10 02 00  	sll	$2, $2, 0x1
  1ae03c: 21 10 48 00  	addu	$2, $2, $8
  1ae040: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae044: 25 10 45 00  	or	$2, $2, $5
  1ae048: 04 00 42 a5  	sh	$2, 0x4($10)
  1ae04c: 36 00 02 3c  	lui	$2, 0x36
  1ae050: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae054: 02 00 62 a1  	sb	$2, 0x2($11)
  1ae058: 36 00 03 3c  	lui	$3, 0x36
  1ae05c: 03 00 62 91  	lbu	$2, 0x3($11)
  1ae060: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ae064: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1ae068: 2b 10 43 00  	sltu	$2, $2, $3
  1ae06c: 26 00 40 10  	beqz	$2, 0x1ae108 <.text+0xae108>
  1ae070: 00 00 00 00  	nop
  1ae074: 03 00 a3 91  	lbu	$3, 0x3($13)
  1ae078: 23 00 60 10  	beqz	$3, 0x1ae108 <.text+0xae108>
  1ae07c: 00 00 00 00  	nop
  1ae080: 03 00 85 91  	lbu	$5, 0x3($12)
  1ae084: 39 00 a0 50  	beqzl	$5, 0x1ae16c <.text+0xae16c>
  1ae088: 44 00 e2 8c  	lw	$2, 0x44($7)
  1ae08c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae090: 1f 00 a2 10  	beq	$5, $2, 0x1ae110 <.text+0xae110>
  1ae094: de fb 06 24  	addiu	$6, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae098: 14 00 e2 8c  	lw	$2, 0x14($7)
  1ae09c: 40 18 03 00  	sll	$3, $3, 0x1
  1ae0a0: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae0a4: 40 10 02 00  	sll	$2, $2, 0x1
  1ae0a8: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae0ac: 21 18 64 00  	addu	$3, $3, $4
  1ae0b0: 21 10 4a 00  	addu	$2, $2, $10
  1ae0b4: 06 00 47 94  	lhu	$7, 0x6($2)
  1ae0b8: 00 00 65 94  	lhu	$5, 0x0($3)
  1ae0bc: ff ff e4 30  	andi	$4, $7, 0xffff
  1ae0c0: ff ff a3 30  	andi	$3, $5, 0xffff
  1ae0c4: 26 28 a7 00  	xor	$5, $5, $7
  1ae0c8: 24 10 66 00  	and	$2, $3, $6
  1ae0cc: 21 04 a5 30  	andi	$5, $5, 0x421
  1ae0d0: 24 30 86 00  	and	$6, $4, $6
  1ae0d4: 24 18 64 00  	and	$3, $3, $4
  1ae0d8: 21 10 46 00  	addu	$2, $2, $6
  1ae0dc: 21 04 63 30  	andi	$3, $3, 0x421
  1ae0e0: 43 10 02 00  	sra	$2, $2, 0x1
  1ae0e4: 21 10 43 00  	addu	$2, $2, $3
  1ae0e8: 40 10 02 00  	sll	$2, $2, 0x1
  1ae0ec: 21 10 48 00  	addu	$2, $2, $8
  1ae0f0: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae0f4: 25 10 45 00  	or	$2, $2, $5
  1ae0f8: 06 00 42 a5  	sh	$2, 0x6($10)
  1ae0fc: 36 00 02 3c  	lui	$2, 0x36
  1ae100: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae104: 03 00 62 a1  	sb	$2, 0x3($11)
  1ae108: 08 00 e0 03  	jr	$ra
  1ae10c: 00 00 00 00  	nop
  1ae110: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae114: 40 18 03 00  	sll	$3, $3, 0x1
  1ae118: 50 00 e6 8c  	lw	$6, 0x50($7)
  1ae11c: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae120: 21 18 64 00  	addu	$3, $3, $4
  1ae124: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae128: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae12c: 24 28 c2 00  	and	$5, $6, $2
  1ae130: 50 00 e7 94  	lhu	$7, 0x50($7)
  1ae134: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae138: 24 10 62 00  	and	$2, $3, $2
  1ae13c: 26 20 87 00  	xor	$4, $4, $7
  1ae140: 24 18 66 00  	and	$3, $3, $6
  1ae144: 21 10 45 00  	addu	$2, $2, $5
  1ae148: 21 04 63 30  	andi	$3, $3, 0x421
  1ae14c: 42 10 02 00  	srl	$2, $2, 0x1
  1ae150: 21 10 43 00  	addu	$2, $2, $3
  1ae154: 21 04 84 30  	andi	$4, $4, 0x421
  1ae158: 40 10 02 00  	sll	$2, $2, 0x1
  1ae15c: 21 10 48 00  	addu	$2, $2, $8
  1ae160: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae164: e4 ff 00 10  	b	0x1ae0f8 <.text+0xae0f8>
  1ae168: 25 10 44 00  	or	$2, $2, $4
  1ae16c: 40 18 03 00  	sll	$3, $3, 0x1
  1ae170: 21 18 62 00  	addu	$3, $3, $2
  1ae174: e0 ff 00 10  	b	0x1ae0f8 <.text+0xae0f8>
  1ae178: 00 00 62 94  	lhu	$2, 0x0($3)
  1ae17c: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae180: 40 18 03 00  	sll	$3, $3, 0x1
  1ae184: 50 00 e6 8c  	lw	$6, 0x50($7)
  1ae188: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae18c: 21 18 64 00  	addu	$3, $3, $4
  1ae190: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae194: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae198: 24 28 c2 00  	and	$5, $6, $2
  1ae19c: 50 00 e7 94  	lhu	$7, 0x50($7)
  1ae1a0: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae1a4: 24 10 62 00  	and	$2, $3, $2
  1ae1a8: 26 20 87 00  	xor	$4, $4, $7
  1ae1ac: 24 18 66 00  	and	$3, $3, $6
  1ae1b0: 21 10 45 00  	addu	$2, $2, $5
  1ae1b4: 21 04 63 30  	andi	$3, $3, 0x421
  1ae1b8: 42 10 02 00  	srl	$2, $2, 0x1
  1ae1bc: 21 10 43 00  	addu	$2, $2, $3
  1ae1c0: 21 04 84 30  	andi	$4, $4, 0x421
  1ae1c4: 40 10 02 00  	sll	$2, $2, 0x1
  1ae1c8: 21 10 48 00  	addu	$2, $2, $8
  1ae1cc: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae1d0: 9d ff 00 10  	b	0x1ae048 <.text+0xae048>
  1ae1d4: 25 10 44 00  	or	$2, $2, $4
  1ae1d8: 40 18 03 00  	sll	$3, $3, 0x1
  1ae1dc: 21 18 62 00  	addu	$3, $3, $2
  1ae1e0: 99 ff 00 10  	b	0x1ae048 <.text+0xae048>
  1ae1e4: 00 00 62 94  	lhu	$2, 0x0($3)
  1ae1e8: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae1ec: 40 18 03 00  	sll	$3, $3, 0x1
  1ae1f0: 50 00 e6 8c  	lw	$6, 0x50($7)
  1ae1f4: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae1f8: 21 18 64 00  	addu	$3, $3, $4
  1ae1fc: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae200: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae204: 24 28 c2 00  	and	$5, $6, $2
  1ae208: 50 00 e7 94  	lhu	$7, 0x50($7)
  1ae20c: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae210: 24 10 62 00  	and	$2, $3, $2
  1ae214: 26 20 87 00  	xor	$4, $4, $7
  1ae218: 24 18 66 00  	and	$3, $3, $6
  1ae21c: 21 10 45 00  	addu	$2, $2, $5
  1ae220: 21 04 63 30  	andi	$3, $3, 0x421
  1ae224: 42 10 02 00  	srl	$2, $2, 0x1
  1ae228: 21 10 43 00  	addu	$2, $2, $3
  1ae22c: 21 04 84 30  	andi	$4, $4, 0x421
  1ae230: 40 10 02 00  	sll	$2, $2, 0x1
  1ae234: 21 10 48 00  	addu	$2, $2, $8
  1ae238: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae23c: 56 ff 00 10  	b	0x1adf98 <.text+0xadf98>
  1ae240: 25 10 44 00  	or	$2, $2, $4
  1ae244: 40 18 03 00  	sll	$3, $3, 0x1
  1ae248: 21 18 62 00  	addu	$3, $3, $2
  1ae24c: 52 ff 00 10  	b	0x1adf98 <.text+0xadf98>
  1ae250: 00 00 62 94  	lhu	$2, 0x0($3)
  1ae254: 44 00 24 8d  	lw	$4, 0x44($9)
  1ae258: 40 18 03 00  	sll	$3, $3, 0x1
  1ae25c: 50 00 26 8d  	lw	$6, 0x50($9)
  1ae260: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae264: 21 18 64 00  	addu	$3, $3, $4
  1ae268: 18 00 28 8d  	lw	$8, 0x18($9)
  1ae26c: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae270: 24 28 c2 00  	and	$5, $6, $2
  1ae274: 50 00 27 95  	lhu	$7, 0x50($9)
  1ae278: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae27c: 24 10 62 00  	and	$2, $3, $2
  1ae280: 26 20 87 00  	xor	$4, $4, $7
  1ae284: 24 18 66 00  	and	$3, $3, $6
  1ae288: 21 10 45 00  	addu	$2, $2, $5
  1ae28c: 21 04 63 30  	andi	$3, $3, 0x421
  1ae290: 42 10 02 00  	srl	$2, $2, 0x1
  1ae294: 21 10 43 00  	addu	$2, $2, $3
  1ae298: 21 04 84 30  	andi	$4, $4, 0x421
  1ae29c: 40 10 02 00  	sll	$2, $2, 0x1
  1ae2a0: 21 10 48 00  	addu	$2, $2, $8
  1ae2a4: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae2a8: 0f ff 00 10  	b	0x1adee8 <.text+0xadee8>
  1ae2ac: 25 10 44 00  	or	$2, $2, $4
  1ae2b0: 40 18 03 00  	sll	$3, $3, 0x1
  1ae2b4: 21 18 62 00  	addu	$3, $3, $2
  1ae2b8: 0b ff 00 10  	b	0x1adee8 <.text+0xadee8>
  1ae2bc: 00 00 62 94  	lhu	$2, 0x0($3)
  1ae2c0: 36 00 02 3c  	lui	$2, 0x36
  1ae2c4: 2d 68 a0 00  	move	$13, $5
  1ae2c8: 80 d4 49 24  	addiu	$9, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ae2cc: 40 38 04 00  	sll	$7, $4, 0x1
  1ae2d0: 08 00 22 8d  	lw	$2, 0x8($9)
  1ae2d4: 4c 00 26 91  	lbu	$6, 0x4c($9)
  1ae2d8: 21 58 44 00  	addu	$11, $2, $4
  1ae2dc: 3c 00 23 8d  	lw	$3, 0x3c($9)
  1ae2e0: 00 00 62 91  	lbu	$2, 0x0($11)
  1ae2e4: 0c 00 25 8d  	lw	$5, 0xc($9)
  1ae2e8: 21 50 67 00  	addu	$10, $3, $7
  1ae2ec: 2b 10 46 00  	sltu	$2, $2, $6
  1ae2f0: 26 00 40 10  	beqz	$2, 0x1ae38c <.text+0xae38c>
  1ae2f4: 21 60 a4 00  	addu	$12, $5, $4
  1ae2f8: 03 00 a3 91  	lbu	$3, 0x3($13)
  1ae2fc: 24 00 60 50  	beqzl	$3, 0x1ae390 <.text+0xae390>
  1ae300: 36 00 03 3c  	lui	$3, 0x36
  1ae304: 00 00 84 91  	lbu	$4, 0x0($12)
  1ae308: 0e 01 80 50  	beqzl	$4, 0x1ae744 <.text+0xae744>
  1ae30c: 44 00 22 8d  	lw	$2, 0x44($9)
  1ae310: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae314: f4 00 82 10  	beq	$4, $2, 0x1ae6e8 <.text+0xae6e8>
  1ae318: de fb 06 24  	addiu	$6, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae31c: 14 00 22 8d  	lw	$2, 0x14($9)
  1ae320: 40 18 03 00  	sll	$3, $3, 0x1
  1ae324: 44 00 24 8d  	lw	$4, 0x44($9)
  1ae328: 40 10 02 00  	sll	$2, $2, 0x1
  1ae32c: 18 00 28 8d  	lw	$8, 0x18($9)
  1ae330: 21 18 64 00  	addu	$3, $3, $4
  1ae334: 21 10 4a 00  	addu	$2, $2, $10
  1ae338: 00 00 47 94  	lhu	$7, 0x0($2)
  1ae33c: 00 00 65 94  	lhu	$5, 0x0($3)
  1ae340: ff ff e4 30  	andi	$4, $7, 0xffff
  1ae344: ff ff a3 30  	andi	$3, $5, 0xffff
  1ae348: 26 28 a7 00  	xor	$5, $5, $7
  1ae34c: 24 10 66 00  	and	$2, $3, $6
  1ae350: 21 04 a5 30  	andi	$5, $5, 0x421
  1ae354: 24 30 86 00  	and	$6, $4, $6
  1ae358: 24 18 64 00  	and	$3, $3, $4
  1ae35c: 21 10 46 00  	addu	$2, $2, $6
  1ae360: 21 04 63 30  	andi	$3, $3, 0x421
  1ae364: 43 10 02 00  	sra	$2, $2, 0x1
  1ae368: 21 10 43 00  	addu	$2, $2, $3
  1ae36c: 40 10 02 00  	sll	$2, $2, 0x1
  1ae370: 21 10 48 00  	addu	$2, $2, $8
  1ae374: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae378: 25 10 45 00  	or	$2, $2, $5
  1ae37c: 00 00 42 a5  	sh	$2, 0x0($10)
  1ae380: 36 00 02 3c  	lui	$2, 0x36
  1ae384: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae388: 00 00 62 a1  	sb	$2, 0x0($11)
  1ae38c: 36 00 03 3c  	lui	$3, 0x36
  1ae390: 01 00 62 91  	lbu	$2, 0x1($11)
  1ae394: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ae398: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1ae39c: 2b 10 43 00  	sltu	$2, $2, $3
  1ae3a0: 27 00 40 10  	beqz	$2, 0x1ae440 <.text+0xae440>
  1ae3a4: 36 00 03 3c  	lui	$3, 0x36
  1ae3a8: 02 00 a3 91  	lbu	$3, 0x2($13)
  1ae3ac: 24 00 60 50  	beqzl	$3, 0x1ae440 <.text+0xae440>
  1ae3b0: 36 00 03 3c  	lui	$3, 0x36
  1ae3b4: 01 00 84 91  	lbu	$4, 0x1($12)
  1ae3b8: c7 00 80 50  	beqzl	$4, 0x1ae6d8 <.text+0xae6d8>
  1ae3bc: 44 00 e2 8c  	lw	$2, 0x44($7)
  1ae3c0: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae3c4: ad 00 82 10  	beq	$4, $2, 0x1ae67c <.text+0xae67c>
  1ae3c8: de fb 06 24  	addiu	$6, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae3cc: 14 00 e2 8c  	lw	$2, 0x14($7)
  1ae3d0: 40 18 03 00  	sll	$3, $3, 0x1
  1ae3d4: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae3d8: 40 10 02 00  	sll	$2, $2, 0x1
  1ae3dc: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae3e0: 21 18 64 00  	addu	$3, $3, $4
  1ae3e4: 21 10 4a 00  	addu	$2, $2, $10
  1ae3e8: 02 00 47 94  	lhu	$7, 0x2($2)
  1ae3ec: 00 00 65 94  	lhu	$5, 0x0($3)
  1ae3f0: ff ff e4 30  	andi	$4, $7, 0xffff
  1ae3f4: ff ff a3 30  	andi	$3, $5, 0xffff
  1ae3f8: 26 28 a7 00  	xor	$5, $5, $7
  1ae3fc: 24 10 66 00  	and	$2, $3, $6
  1ae400: 21 04 a5 30  	andi	$5, $5, 0x421
  1ae404: 24 30 86 00  	and	$6, $4, $6
  1ae408: 24 18 64 00  	and	$3, $3, $4
  1ae40c: 21 10 46 00  	addu	$2, $2, $6
  1ae410: 21 04 63 30  	andi	$3, $3, 0x421
  1ae414: 43 10 02 00  	sra	$2, $2, 0x1
  1ae418: 21 10 43 00  	addu	$2, $2, $3
  1ae41c: 40 10 02 00  	sll	$2, $2, 0x1
  1ae420: 21 10 48 00  	addu	$2, $2, $8
  1ae424: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae428: 25 10 45 00  	or	$2, $2, $5
  1ae42c: 02 00 42 a5  	sh	$2, 0x2($10)
  1ae430: 36 00 02 3c  	lui	$2, 0x36
  1ae434: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae438: 01 00 62 a1  	sb	$2, 0x1($11)
  1ae43c: 36 00 03 3c  	lui	$3, 0x36
  1ae440: 02 00 62 91  	lbu	$2, 0x2($11)
  1ae444: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ae448: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1ae44c: 2b 10 43 00  	sltu	$2, $2, $3
  1ae450: 27 00 40 10  	beqz	$2, 0x1ae4f0 <.text+0xae4f0>
  1ae454: 36 00 03 3c  	lui	$3, 0x36
  1ae458: 01 00 a3 91  	lbu	$3, 0x1($13)
  1ae45c: 24 00 60 50  	beqzl	$3, 0x1ae4f0 <.text+0xae4f0>
  1ae460: 36 00 03 3c  	lui	$3, 0x36
  1ae464: 02 00 84 91  	lbu	$4, 0x2($12)
  1ae468: 80 00 80 50  	beqzl	$4, 0x1ae66c <.text+0xae66c>
  1ae46c: 44 00 e2 8c  	lw	$2, 0x44($7)
  1ae470: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae474: 66 00 82 10  	beq	$4, $2, 0x1ae610 <.text+0xae610>
  1ae478: de fb 06 24  	addiu	$6, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae47c: 14 00 e2 8c  	lw	$2, 0x14($7)
  1ae480: 40 18 03 00  	sll	$3, $3, 0x1
  1ae484: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae488: 40 10 02 00  	sll	$2, $2, 0x1
  1ae48c: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae490: 21 18 64 00  	addu	$3, $3, $4
  1ae494: 21 10 4a 00  	addu	$2, $2, $10
  1ae498: 04 00 47 94  	lhu	$7, 0x4($2)
  1ae49c: 00 00 65 94  	lhu	$5, 0x0($3)
  1ae4a0: ff ff e4 30  	andi	$4, $7, 0xffff
  1ae4a4: ff ff a3 30  	andi	$3, $5, 0xffff
  1ae4a8: 26 28 a7 00  	xor	$5, $5, $7
  1ae4ac: 24 10 66 00  	and	$2, $3, $6
  1ae4b0: 21 04 a5 30  	andi	$5, $5, 0x421
  1ae4b4: 24 30 86 00  	and	$6, $4, $6
  1ae4b8: 24 18 64 00  	and	$3, $3, $4
  1ae4bc: 21 10 46 00  	addu	$2, $2, $6
  1ae4c0: 21 04 63 30  	andi	$3, $3, 0x421
  1ae4c4: 43 10 02 00  	sra	$2, $2, 0x1
  1ae4c8: 21 10 43 00  	addu	$2, $2, $3
  1ae4cc: 40 10 02 00  	sll	$2, $2, 0x1
  1ae4d0: 21 10 48 00  	addu	$2, $2, $8
  1ae4d4: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae4d8: 25 10 45 00  	or	$2, $2, $5
  1ae4dc: 04 00 42 a5  	sh	$2, 0x4($10)
  1ae4e0: 36 00 02 3c  	lui	$2, 0x36
  1ae4e4: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae4e8: 02 00 62 a1  	sb	$2, 0x2($11)
  1ae4ec: 36 00 03 3c  	lui	$3, 0x36
  1ae4f0: 03 00 62 91  	lbu	$2, 0x3($11)
  1ae4f4: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ae4f8: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1ae4fc: 2b 10 43 00  	sltu	$2, $2, $3
  1ae500: 26 00 40 10  	beqz	$2, 0x1ae59c <.text+0xae59c>
  1ae504: 00 00 00 00  	nop
  1ae508: 00 00 a3 91  	lbu	$3, 0x0($13)
  1ae50c: 23 00 60 10  	beqz	$3, 0x1ae59c <.text+0xae59c>
  1ae510: 00 00 00 00  	nop
  1ae514: 03 00 85 91  	lbu	$5, 0x3($12)
  1ae518: 39 00 a0 50  	beqzl	$5, 0x1ae600 <.text+0xae600>
  1ae51c: 44 00 e2 8c  	lw	$2, 0x44($7)
  1ae520: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae524: 1f 00 a2 10  	beq	$5, $2, 0x1ae5a4 <.text+0xae5a4>
  1ae528: de fb 06 24  	addiu	$6, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae52c: 14 00 e2 8c  	lw	$2, 0x14($7)
  1ae530: 40 18 03 00  	sll	$3, $3, 0x1
  1ae534: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae538: 40 10 02 00  	sll	$2, $2, 0x1
  1ae53c: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae540: 21 18 64 00  	addu	$3, $3, $4
  1ae544: 21 10 4a 00  	addu	$2, $2, $10
  1ae548: 06 00 47 94  	lhu	$7, 0x6($2)
  1ae54c: 00 00 65 94  	lhu	$5, 0x0($3)
  1ae550: ff ff e4 30  	andi	$4, $7, 0xffff
  1ae554: ff ff a3 30  	andi	$3, $5, 0xffff
  1ae558: 26 28 a7 00  	xor	$5, $5, $7
  1ae55c: 24 10 66 00  	and	$2, $3, $6
  1ae560: 21 04 a5 30  	andi	$5, $5, 0x421
  1ae564: 24 30 86 00  	and	$6, $4, $6
  1ae568: 24 18 64 00  	and	$3, $3, $4
  1ae56c: 21 10 46 00  	addu	$2, $2, $6
  1ae570: 21 04 63 30  	andi	$3, $3, 0x421
  1ae574: 43 10 02 00  	sra	$2, $2, 0x1
  1ae578: 21 10 43 00  	addu	$2, $2, $3
  1ae57c: 40 10 02 00  	sll	$2, $2, 0x1
  1ae580: 21 10 48 00  	addu	$2, $2, $8
  1ae584: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae588: 25 10 45 00  	or	$2, $2, $5
  1ae58c: 06 00 42 a5  	sh	$2, 0x6($10)
  1ae590: 36 00 02 3c  	lui	$2, 0x36
  1ae594: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae598: 03 00 62 a1  	sb	$2, 0x3($11)
  1ae59c: 08 00 e0 03  	jr	$ra
  1ae5a0: 00 00 00 00  	nop
  1ae5a4: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae5a8: 40 18 03 00  	sll	$3, $3, 0x1
  1ae5ac: 50 00 e6 8c  	lw	$6, 0x50($7)
  1ae5b0: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae5b4: 21 18 64 00  	addu	$3, $3, $4
  1ae5b8: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae5bc: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae5c0: 24 28 c2 00  	and	$5, $6, $2
  1ae5c4: 50 00 e7 94  	lhu	$7, 0x50($7)
  1ae5c8: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae5cc: 24 10 62 00  	and	$2, $3, $2
  1ae5d0: 26 20 87 00  	xor	$4, $4, $7
  1ae5d4: 24 18 66 00  	and	$3, $3, $6
  1ae5d8: 21 10 45 00  	addu	$2, $2, $5
  1ae5dc: 21 04 63 30  	andi	$3, $3, 0x421
  1ae5e0: 42 10 02 00  	srl	$2, $2, 0x1
  1ae5e4: 21 10 43 00  	addu	$2, $2, $3
  1ae5e8: 21 04 84 30  	andi	$4, $4, 0x421
  1ae5ec: 40 10 02 00  	sll	$2, $2, 0x1
  1ae5f0: 21 10 48 00  	addu	$2, $2, $8
  1ae5f4: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae5f8: e4 ff 00 10  	b	0x1ae58c <.text+0xae58c>
  1ae5fc: 25 10 44 00  	or	$2, $2, $4
  1ae600: 40 18 03 00  	sll	$3, $3, 0x1
  1ae604: 21 18 62 00  	addu	$3, $3, $2
  1ae608: e0 ff 00 10  	b	0x1ae58c <.text+0xae58c>
  1ae60c: 00 00 62 94  	lhu	$2, 0x0($3)
  1ae610: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae614: 40 18 03 00  	sll	$3, $3, 0x1
  1ae618: 50 00 e6 8c  	lw	$6, 0x50($7)
  1ae61c: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae620: 21 18 64 00  	addu	$3, $3, $4
  1ae624: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae628: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae62c: 24 28 c2 00  	and	$5, $6, $2
  1ae630: 50 00 e7 94  	lhu	$7, 0x50($7)
  1ae634: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae638: 24 10 62 00  	and	$2, $3, $2
  1ae63c: 26 20 87 00  	xor	$4, $4, $7
  1ae640: 24 18 66 00  	and	$3, $3, $6
  1ae644: 21 10 45 00  	addu	$2, $2, $5
  1ae648: 21 04 63 30  	andi	$3, $3, 0x421
  1ae64c: 42 10 02 00  	srl	$2, $2, 0x1
  1ae650: 21 10 43 00  	addu	$2, $2, $3
  1ae654: 21 04 84 30  	andi	$4, $4, 0x421
  1ae658: 40 10 02 00  	sll	$2, $2, 0x1
  1ae65c: 21 10 48 00  	addu	$2, $2, $8
  1ae660: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae664: 9d ff 00 10  	b	0x1ae4dc <.text+0xae4dc>
  1ae668: 25 10 44 00  	or	$2, $2, $4
  1ae66c: 40 18 03 00  	sll	$3, $3, 0x1
  1ae670: 21 18 62 00  	addu	$3, $3, $2
  1ae674: 99 ff 00 10  	b	0x1ae4dc <.text+0xae4dc>
  1ae678: 00 00 62 94  	lhu	$2, 0x0($3)
  1ae67c: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae680: 40 18 03 00  	sll	$3, $3, 0x1
  1ae684: 50 00 e6 8c  	lw	$6, 0x50($7)
  1ae688: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae68c: 21 18 64 00  	addu	$3, $3, $4
  1ae690: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae694: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae698: 24 28 c2 00  	and	$5, $6, $2
  1ae69c: 50 00 e7 94  	lhu	$7, 0x50($7)
  1ae6a0: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae6a4: 24 10 62 00  	and	$2, $3, $2
  1ae6a8: 26 20 87 00  	xor	$4, $4, $7
  1ae6ac: 24 18 66 00  	and	$3, $3, $6
  1ae6b0: 21 10 45 00  	addu	$2, $2, $5
  1ae6b4: 21 04 63 30  	andi	$3, $3, 0x421
  1ae6b8: 42 10 02 00  	srl	$2, $2, 0x1
  1ae6bc: 21 10 43 00  	addu	$2, $2, $3
  1ae6c0: 21 04 84 30  	andi	$4, $4, 0x421
  1ae6c4: 40 10 02 00  	sll	$2, $2, 0x1
  1ae6c8: 21 10 48 00  	addu	$2, $2, $8
  1ae6cc: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae6d0: 56 ff 00 10  	b	0x1ae42c <.text+0xae42c>
  1ae6d4: 25 10 44 00  	or	$2, $2, $4
  1ae6d8: 40 18 03 00  	sll	$3, $3, 0x1
  1ae6dc: 21 18 62 00  	addu	$3, $3, $2
  1ae6e0: 52 ff 00 10  	b	0x1ae42c <.text+0xae42c>
  1ae6e4: 00 00 62 94  	lhu	$2, 0x0($3)
  1ae6e8: 44 00 24 8d  	lw	$4, 0x44($9)
  1ae6ec: 40 18 03 00  	sll	$3, $3, 0x1
  1ae6f0: 50 00 26 8d  	lw	$6, 0x50($9)
  1ae6f4: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae6f8: 21 18 64 00  	addu	$3, $3, $4
  1ae6fc: 18 00 28 8d  	lw	$8, 0x18($9)
  1ae700: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae704: 24 28 c2 00  	and	$5, $6, $2
  1ae708: 50 00 27 95  	lhu	$7, 0x50($9)
  1ae70c: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae710: 24 10 62 00  	and	$2, $3, $2
  1ae714: 26 20 87 00  	xor	$4, $4, $7
  1ae718: 24 18 66 00  	and	$3, $3, $6
  1ae71c: 21 10 45 00  	addu	$2, $2, $5
  1ae720: 21 04 63 30  	andi	$3, $3, 0x421
  1ae724: 42 10 02 00  	srl	$2, $2, 0x1
  1ae728: 21 10 43 00  	addu	$2, $2, $3
  1ae72c: 21 04 84 30  	andi	$4, $4, 0x421
  1ae730: 40 10 02 00  	sll	$2, $2, 0x1
  1ae734: 21 10 48 00  	addu	$2, $2, $8
  1ae738: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae73c: 0f ff 00 10  	b	0x1ae37c <.text+0xae37c>
  1ae740: 25 10 44 00  	or	$2, $2, $4
  1ae744: 40 18 03 00  	sll	$3, $3, 0x1
  1ae748: 21 18 62 00  	addu	$3, $3, $2
  1ae74c: 0b ff 00 10  	b	0x1ae37c <.text+0xae37c>
  1ae750: 00 00 62 94  	lhu	$2, 0x0($3)
  1ae754: 36 00 02 3c  	lui	$2, 0x36
  1ae758: 2d 68 a0 00  	move	$13, $5
  1ae75c: 80 d4 49 24  	addiu	$9, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ae760: 40 38 04 00  	sll	$7, $4, 0x1
  1ae764: 08 00 22 8d  	lw	$2, 0x8($9)
  1ae768: 4c 00 26 91  	lbu	$6, 0x4c($9)
  1ae76c: 21 58 44 00  	addu	$11, $2, $4
  1ae770: 3c 00 23 8d  	lw	$3, 0x3c($9)
  1ae774: 00 00 62 91  	lbu	$2, 0x0($11)
  1ae778: 0c 00 25 8d  	lw	$5, 0xc($9)
  1ae77c: 21 50 67 00  	addu	$10, $3, $7
  1ae780: 2b 10 46 00  	sltu	$2, $2, $6
  1ae784: 1d 00 40 10  	beqz	$2, 0x1ae7fc <.text+0xae7fc>
  1ae788: 21 60 a4 00  	addu	$12, $5, $4
  1ae78c: 00 00 a3 91  	lbu	$3, 0x0($13)
  1ae790: 1b 00 60 50  	beqzl	$3, 0x1ae800 <.text+0xae800>
  1ae794: 36 00 03 3c  	lui	$3, 0x36
  1ae798: 00 00 84 91  	lbu	$4, 0x0($12)
  1ae79c: ea 00 80 50  	beqzl	$4, 0x1aeb48 <.text+0xaeb48>
  1ae7a0: 44 00 22 8d  	lw	$2, 0x44($9)
  1ae7a4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae7a8: d0 00 82 10  	beq	$4, $2, 0x1aeaec <.text+0xaeaec>
  1ae7ac: de fb 04 24  	addiu	$4, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae7b0: 14 00 22 8d  	lw	$2, 0x14($9)
  1ae7b4: 40 18 03 00  	sll	$3, $3, 0x1
  1ae7b8: 44 00 25 8d  	lw	$5, 0x44($9)
  1ae7bc: 40 10 02 00  	sll	$2, $2, 0x1
  1ae7c0: 21 18 65 00  	addu	$3, $3, $5
  1ae7c4: 21 10 4a 00  	addu	$2, $2, $10
  1ae7c8: 00 00 45 94  	lhu	$5, 0x0($2)
  1ae7cc: 00 00 63 94  	lhu	$3, 0x0($3)
  1ae7d0: 24 10 64 00  	and	$2, $3, $4
  1ae7d4: 24 18 65 00  	and	$3, $3, $5
  1ae7d8: 24 20 a4 00  	and	$4, $5, $4
  1ae7dc: 21 04 63 30  	andi	$3, $3, 0x421
  1ae7e0: 21 10 44 00  	addu	$2, $2, $4
  1ae7e4: 43 10 02 00  	sra	$2, $2, 0x1
  1ae7e8: 21 10 43 00  	addu	$2, $2, $3
  1ae7ec: 00 00 42 a5  	sh	$2, 0x0($10)
  1ae7f0: 36 00 02 3c  	lui	$2, 0x36
  1ae7f4: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae7f8: 00 00 62 a1  	sb	$2, 0x0($11)
  1ae7fc: 36 00 03 3c  	lui	$3, 0x36
  1ae800: 01 00 62 91  	lbu	$2, 0x1($11)
  1ae804: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ae808: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1ae80c: 2b 10 43 00  	sltu	$2, $2, $3
  1ae810: 1e 00 40 10  	beqz	$2, 0x1ae88c <.text+0xae88c>
  1ae814: 36 00 03 3c  	lui	$3, 0x36
  1ae818: 01 00 a3 91  	lbu	$3, 0x1($13)
  1ae81c: 1b 00 60 50  	beqzl	$3, 0x1ae88c <.text+0xae88c>
  1ae820: 36 00 03 3c  	lui	$3, 0x36
  1ae824: 01 00 84 91  	lbu	$4, 0x1($12)
  1ae828: ac 00 80 50  	beqzl	$4, 0x1aeadc <.text+0xaeadc>
  1ae82c: 44 00 e2 8c  	lw	$2, 0x44($7)
  1ae830: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae834: 92 00 82 10  	beq	$4, $2, 0x1aea80 <.text+0xaea80>
  1ae838: de fb 04 24  	addiu	$4, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae83c: 14 00 e2 8c  	lw	$2, 0x14($7)
  1ae840: 40 18 03 00  	sll	$3, $3, 0x1
  1ae844: 44 00 e5 8c  	lw	$5, 0x44($7)
  1ae848: 40 10 02 00  	sll	$2, $2, 0x1
  1ae84c: 21 18 65 00  	addu	$3, $3, $5
  1ae850: 21 10 4a 00  	addu	$2, $2, $10
  1ae854: 02 00 45 94  	lhu	$5, 0x2($2)
  1ae858: 00 00 63 94  	lhu	$3, 0x0($3)
  1ae85c: 24 10 64 00  	and	$2, $3, $4
  1ae860: 24 18 65 00  	and	$3, $3, $5
  1ae864: 24 20 a4 00  	and	$4, $5, $4
  1ae868: 21 04 63 30  	andi	$3, $3, 0x421
  1ae86c: 21 10 44 00  	addu	$2, $2, $4
  1ae870: 43 10 02 00  	sra	$2, $2, 0x1
  1ae874: 21 10 43 00  	addu	$2, $2, $3
  1ae878: 02 00 42 a5  	sh	$2, 0x2($10)
  1ae87c: 36 00 02 3c  	lui	$2, 0x36
  1ae880: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae884: 01 00 62 a1  	sb	$2, 0x1($11)
  1ae888: 36 00 03 3c  	lui	$3, 0x36
  1ae88c: 02 00 62 91  	lbu	$2, 0x2($11)
  1ae890: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ae894: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1ae898: 2b 10 43 00  	sltu	$2, $2, $3
  1ae89c: 1e 00 40 10  	beqz	$2, 0x1ae918 <.text+0xae918>
  1ae8a0: 36 00 03 3c  	lui	$3, 0x36
  1ae8a4: 02 00 a3 91  	lbu	$3, 0x2($13)
  1ae8a8: 1b 00 60 50  	beqzl	$3, 0x1ae918 <.text+0xae918>
  1ae8ac: 36 00 03 3c  	lui	$3, 0x36
  1ae8b0: 02 00 84 91  	lbu	$4, 0x2($12)
  1ae8b4: 6e 00 80 50  	beqzl	$4, 0x1aea70 <.text+0xaea70>
  1ae8b8: 44 00 e2 8c  	lw	$2, 0x44($7)
  1ae8bc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae8c0: 54 00 82 10  	beq	$4, $2, 0x1aea14 <.text+0xaea14>
  1ae8c4: de fb 04 24  	addiu	$4, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae8c8: 14 00 e2 8c  	lw	$2, 0x14($7)
  1ae8cc: 40 18 03 00  	sll	$3, $3, 0x1
  1ae8d0: 44 00 e5 8c  	lw	$5, 0x44($7)
  1ae8d4: 40 10 02 00  	sll	$2, $2, 0x1
  1ae8d8: 21 18 65 00  	addu	$3, $3, $5
  1ae8dc: 21 10 4a 00  	addu	$2, $2, $10
  1ae8e0: 04 00 45 94  	lhu	$5, 0x4($2)
  1ae8e4: 00 00 63 94  	lhu	$3, 0x0($3)
  1ae8e8: 24 10 64 00  	and	$2, $3, $4
  1ae8ec: 24 18 65 00  	and	$3, $3, $5
  1ae8f0: 24 20 a4 00  	and	$4, $5, $4
  1ae8f4: 21 04 63 30  	andi	$3, $3, 0x421
  1ae8f8: 21 10 44 00  	addu	$2, $2, $4
  1ae8fc: 43 10 02 00  	sra	$2, $2, 0x1
  1ae900: 21 10 43 00  	addu	$2, $2, $3
  1ae904: 04 00 42 a5  	sh	$2, 0x4($10)
  1ae908: 36 00 02 3c  	lui	$2, 0x36
  1ae90c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae910: 02 00 62 a1  	sb	$2, 0x2($11)
  1ae914: 36 00 03 3c  	lui	$3, 0x36
  1ae918: 03 00 62 91  	lbu	$2, 0x3($11)
  1ae91c: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ae920: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1ae924: 2b 10 43 00  	sltu	$2, $2, $3
  1ae928: 1d 00 40 10  	beqz	$2, 0x1ae9a0 <.text+0xae9a0>
  1ae92c: 00 00 00 00  	nop
  1ae930: 03 00 a3 91  	lbu	$3, 0x3($13)
  1ae934: 1a 00 60 10  	beqz	$3, 0x1ae9a0 <.text+0xae9a0>
  1ae938: 00 00 00 00  	nop
  1ae93c: 03 00 85 91  	lbu	$5, 0x3($12)
  1ae940: 30 00 a0 50  	beqzl	$5, 0x1aea04 <.text+0xaea04>
  1ae944: 44 00 e2 8c  	lw	$2, 0x44($7)
  1ae948: 01 00 02 24  	addiu	$2, $zero, 0x1
  1ae94c: 16 00 a2 10  	beq	$5, $2, 0x1ae9a8 <.text+0xae9a8>
  1ae950: de fb 04 24  	addiu	$4, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae954: 14 00 e2 8c  	lw	$2, 0x14($7)
  1ae958: 40 18 03 00  	sll	$3, $3, 0x1
  1ae95c: 44 00 e5 8c  	lw	$5, 0x44($7)
  1ae960: 40 10 02 00  	sll	$2, $2, 0x1
  1ae964: 21 18 65 00  	addu	$3, $3, $5
  1ae968: 21 10 4a 00  	addu	$2, $2, $10
  1ae96c: 06 00 45 94  	lhu	$5, 0x6($2)
  1ae970: 00 00 63 94  	lhu	$3, 0x0($3)
  1ae974: 24 10 64 00  	and	$2, $3, $4
  1ae978: 24 18 65 00  	and	$3, $3, $5
  1ae97c: 24 20 a4 00  	and	$4, $5, $4
  1ae980: 21 04 63 30  	andi	$3, $3, 0x421
  1ae984: 21 10 44 00  	addu	$2, $2, $4
  1ae988: 43 10 02 00  	sra	$2, $2, 0x1
  1ae98c: 21 10 43 00  	addu	$2, $2, $3
  1ae990: 06 00 42 a5  	sh	$2, 0x6($10)
  1ae994: 36 00 02 3c  	lui	$2, 0x36
  1ae998: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1ae99c: 03 00 62 a1  	sb	$2, 0x3($11)
  1ae9a0: 08 00 e0 03  	jr	$ra
  1ae9a4: 00 00 00 00  	nop
  1ae9a8: 44 00 e4 8c  	lw	$4, 0x44($7)
  1ae9ac: 40 18 03 00  	sll	$3, $3, 0x1
  1ae9b0: 50 00 e6 8c  	lw	$6, 0x50($7)
  1ae9b4: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1ae9b8: 21 18 64 00  	addu	$3, $3, $4
  1ae9bc: 18 00 e8 8c  	lw	$8, 0x18($7)
  1ae9c0: 00 00 64 94  	lhu	$4, 0x0($3)
  1ae9c4: 24 28 c2 00  	and	$5, $6, $2
  1ae9c8: 50 00 e7 94  	lhu	$7, 0x50($7)
  1ae9cc: ff ff 83 30  	andi	$3, $4, 0xffff
  1ae9d0: 24 10 62 00  	and	$2, $3, $2
  1ae9d4: 26 20 87 00  	xor	$4, $4, $7
  1ae9d8: 24 18 66 00  	and	$3, $3, $6
  1ae9dc: 21 10 45 00  	addu	$2, $2, $5
  1ae9e0: 21 04 63 30  	andi	$3, $3, 0x421
  1ae9e4: 42 10 02 00  	srl	$2, $2, 0x1
  1ae9e8: 21 10 43 00  	addu	$2, $2, $3
  1ae9ec: 21 04 84 30  	andi	$4, $4, 0x421
  1ae9f0: 40 10 02 00  	sll	$2, $2, 0x1
  1ae9f4: 21 10 48 00  	addu	$2, $2, $8
  1ae9f8: 00 00 42 94  	lhu	$2, 0x0($2)
  1ae9fc: e4 ff 00 10  	b	0x1ae990 <.text+0xae990>
  1aea00: 25 10 44 00  	or	$2, $2, $4
  1aea04: 40 18 03 00  	sll	$3, $3, 0x1
  1aea08: 21 18 62 00  	addu	$3, $3, $2
  1aea0c: e0 ff 00 10  	b	0x1ae990 <.text+0xae990>
  1aea10: 00 00 62 94  	lhu	$2, 0x0($3)
  1aea14: 44 00 e4 8c  	lw	$4, 0x44($7)
  1aea18: 40 18 03 00  	sll	$3, $3, 0x1
  1aea1c: 50 00 e6 8c  	lw	$6, 0x50($7)
  1aea20: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aea24: 21 18 64 00  	addu	$3, $3, $4
  1aea28: 18 00 e8 8c  	lw	$8, 0x18($7)
  1aea2c: 00 00 64 94  	lhu	$4, 0x0($3)
  1aea30: 24 28 c2 00  	and	$5, $6, $2
  1aea34: 50 00 e7 94  	lhu	$7, 0x50($7)
  1aea38: ff ff 83 30  	andi	$3, $4, 0xffff
  1aea3c: 24 10 62 00  	and	$2, $3, $2
  1aea40: 26 20 87 00  	xor	$4, $4, $7
  1aea44: 24 18 66 00  	and	$3, $3, $6
  1aea48: 21 10 45 00  	addu	$2, $2, $5
  1aea4c: 21 04 63 30  	andi	$3, $3, 0x421
  1aea50: 42 10 02 00  	srl	$2, $2, 0x1
  1aea54: 21 10 43 00  	addu	$2, $2, $3
  1aea58: 21 04 84 30  	andi	$4, $4, 0x421
  1aea5c: 40 10 02 00  	sll	$2, $2, 0x1
  1aea60: 21 10 48 00  	addu	$2, $2, $8
  1aea64: 00 00 42 94  	lhu	$2, 0x0($2)
  1aea68: a6 ff 00 10  	b	0x1ae904 <.text+0xae904>
  1aea6c: 25 10 44 00  	or	$2, $2, $4
  1aea70: 40 18 03 00  	sll	$3, $3, 0x1
  1aea74: 21 18 62 00  	addu	$3, $3, $2
  1aea78: a2 ff 00 10  	b	0x1ae904 <.text+0xae904>
  1aea7c: 00 00 62 94  	lhu	$2, 0x0($3)
  1aea80: 44 00 e4 8c  	lw	$4, 0x44($7)
  1aea84: 40 18 03 00  	sll	$3, $3, 0x1
  1aea88: 50 00 e6 8c  	lw	$6, 0x50($7)
  1aea8c: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aea90: 21 18 64 00  	addu	$3, $3, $4
  1aea94: 18 00 e8 8c  	lw	$8, 0x18($7)
  1aea98: 00 00 64 94  	lhu	$4, 0x0($3)
  1aea9c: 24 28 c2 00  	and	$5, $6, $2
  1aeaa0: 50 00 e7 94  	lhu	$7, 0x50($7)
  1aeaa4: ff ff 83 30  	andi	$3, $4, 0xffff
  1aeaa8: 24 10 62 00  	and	$2, $3, $2
  1aeaac: 26 20 87 00  	xor	$4, $4, $7
  1aeab0: 24 18 66 00  	and	$3, $3, $6
  1aeab4: 21 10 45 00  	addu	$2, $2, $5
  1aeab8: 21 04 63 30  	andi	$3, $3, 0x421
  1aeabc: 42 10 02 00  	srl	$2, $2, 0x1
  1aeac0: 21 10 43 00  	addu	$2, $2, $3
  1aeac4: 21 04 84 30  	andi	$4, $4, 0x421
  1aeac8: 40 10 02 00  	sll	$2, $2, 0x1
  1aeacc: 21 10 48 00  	addu	$2, $2, $8
  1aead0: 00 00 42 94  	lhu	$2, 0x0($2)
  1aead4: 68 ff 00 10  	b	0x1ae878 <.text+0xae878>
  1aead8: 25 10 44 00  	or	$2, $2, $4
  1aeadc: 40 18 03 00  	sll	$3, $3, 0x1
  1aeae0: 21 18 62 00  	addu	$3, $3, $2
  1aeae4: 64 ff 00 10  	b	0x1ae878 <.text+0xae878>
  1aeae8: 00 00 62 94  	lhu	$2, 0x0($3)
  1aeaec: 44 00 24 8d  	lw	$4, 0x44($9)
  1aeaf0: 40 18 03 00  	sll	$3, $3, 0x1
  1aeaf4: 50 00 26 8d  	lw	$6, 0x50($9)
  1aeaf8: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aeafc: 21 18 64 00  	addu	$3, $3, $4
  1aeb00: 18 00 28 8d  	lw	$8, 0x18($9)
  1aeb04: 00 00 64 94  	lhu	$4, 0x0($3)
  1aeb08: 24 28 c2 00  	and	$5, $6, $2
  1aeb0c: 50 00 27 95  	lhu	$7, 0x50($9)
  1aeb10: ff ff 83 30  	andi	$3, $4, 0xffff
  1aeb14: 24 10 62 00  	and	$2, $3, $2
  1aeb18: 26 20 87 00  	xor	$4, $4, $7
  1aeb1c: 24 18 66 00  	and	$3, $3, $6
  1aeb20: 21 10 45 00  	addu	$2, $2, $5
  1aeb24: 21 04 63 30  	andi	$3, $3, 0x421
  1aeb28: 42 10 02 00  	srl	$2, $2, 0x1
  1aeb2c: 21 10 43 00  	addu	$2, $2, $3
  1aeb30: 21 04 84 30  	andi	$4, $4, 0x421
  1aeb34: 40 10 02 00  	sll	$2, $2, 0x1
  1aeb38: 21 10 48 00  	addu	$2, $2, $8
  1aeb3c: 00 00 42 94  	lhu	$2, 0x0($2)
  1aeb40: 2a ff 00 10  	b	0x1ae7ec <.text+0xae7ec>
  1aeb44: 25 10 44 00  	or	$2, $2, $4
  1aeb48: 40 18 03 00  	sll	$3, $3, 0x1
  1aeb4c: 21 18 62 00  	addu	$3, $3, $2
  1aeb50: 26 ff 00 10  	b	0x1ae7ec <.text+0xae7ec>
  1aeb54: 00 00 62 94  	lhu	$2, 0x0($3)
  1aeb58: 36 00 02 3c  	lui	$2, 0x36
  1aeb5c: 2d 68 a0 00  	move	$13, $5
  1aeb60: 80 d4 49 24  	addiu	$9, $2, -0x2b80 <.text+0xffffffffffefd480>
  1aeb64: 40 38 04 00  	sll	$7, $4, 0x1
  1aeb68: 08 00 22 8d  	lw	$2, 0x8($9)
  1aeb6c: 4c 00 26 91  	lbu	$6, 0x4c($9)
  1aeb70: 21 58 44 00  	addu	$11, $2, $4
  1aeb74: 3c 00 23 8d  	lw	$3, 0x3c($9)
  1aeb78: 00 00 62 91  	lbu	$2, 0x0($11)
  1aeb7c: 0c 00 25 8d  	lw	$5, 0xc($9)
  1aeb80: 21 50 67 00  	addu	$10, $3, $7
  1aeb84: 2b 10 46 00  	sltu	$2, $2, $6
  1aeb88: 1d 00 40 10  	beqz	$2, 0x1aec00 <.text+0xaec00>
  1aeb8c: 21 60 a4 00  	addu	$12, $5, $4
  1aeb90: 03 00 a3 91  	lbu	$3, 0x3($13)
  1aeb94: 1b 00 60 50  	beqzl	$3, 0x1aec04 <.text+0xaec04>
  1aeb98: 36 00 03 3c  	lui	$3, 0x36
  1aeb9c: 00 00 84 91  	lbu	$4, 0x0($12)
  1aeba0: ea 00 80 50  	beqzl	$4, 0x1aef4c <.text+0xaef4c>
  1aeba4: 44 00 22 8d  	lw	$2, 0x44($9)
  1aeba8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aebac: d0 00 82 10  	beq	$4, $2, 0x1aeef0 <.text+0xaeef0>
  1aebb0: de fb 04 24  	addiu	$4, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aebb4: 14 00 22 8d  	lw	$2, 0x14($9)
  1aebb8: 40 18 03 00  	sll	$3, $3, 0x1
  1aebbc: 44 00 25 8d  	lw	$5, 0x44($9)
  1aebc0: 40 10 02 00  	sll	$2, $2, 0x1
  1aebc4: 21 18 65 00  	addu	$3, $3, $5
  1aebc8: 21 10 4a 00  	addu	$2, $2, $10
  1aebcc: 00 00 45 94  	lhu	$5, 0x0($2)
  1aebd0: 00 00 63 94  	lhu	$3, 0x0($3)
  1aebd4: 24 10 64 00  	and	$2, $3, $4
  1aebd8: 24 18 65 00  	and	$3, $3, $5
  1aebdc: 24 20 a4 00  	and	$4, $5, $4
  1aebe0: 21 04 63 30  	andi	$3, $3, 0x421
  1aebe4: 21 10 44 00  	addu	$2, $2, $4
  1aebe8: 43 10 02 00  	sra	$2, $2, 0x1
  1aebec: 21 10 43 00  	addu	$2, $2, $3
  1aebf0: 00 00 42 a5  	sh	$2, 0x0($10)
  1aebf4: 36 00 02 3c  	lui	$2, 0x36
  1aebf8: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1aebfc: 00 00 62 a1  	sb	$2, 0x0($11)
  1aec00: 36 00 03 3c  	lui	$3, 0x36
  1aec04: 01 00 62 91  	lbu	$2, 0x1($11)
  1aec08: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1aec0c: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1aec10: 2b 10 43 00  	sltu	$2, $2, $3
  1aec14: 1e 00 40 10  	beqz	$2, 0x1aec90 <.text+0xaec90>
  1aec18: 36 00 03 3c  	lui	$3, 0x36
  1aec1c: 02 00 a3 91  	lbu	$3, 0x2($13)
  1aec20: 1b 00 60 50  	beqzl	$3, 0x1aec90 <.text+0xaec90>
  1aec24: 36 00 03 3c  	lui	$3, 0x36
  1aec28: 01 00 84 91  	lbu	$4, 0x1($12)
  1aec2c: ac 00 80 50  	beqzl	$4, 0x1aeee0 <.text+0xaeee0>
  1aec30: 44 00 e2 8c  	lw	$2, 0x44($7)
  1aec34: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aec38: 92 00 82 10  	beq	$4, $2, 0x1aee84 <.text+0xaee84>
  1aec3c: de fb 04 24  	addiu	$4, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aec40: 14 00 e2 8c  	lw	$2, 0x14($7)
  1aec44: 40 18 03 00  	sll	$3, $3, 0x1
  1aec48: 44 00 e5 8c  	lw	$5, 0x44($7)
  1aec4c: 40 10 02 00  	sll	$2, $2, 0x1
  1aec50: 21 18 65 00  	addu	$3, $3, $5
  1aec54: 21 10 4a 00  	addu	$2, $2, $10
  1aec58: 02 00 45 94  	lhu	$5, 0x2($2)
  1aec5c: 00 00 63 94  	lhu	$3, 0x0($3)
  1aec60: 24 10 64 00  	and	$2, $3, $4
  1aec64: 24 18 65 00  	and	$3, $3, $5
  1aec68: 24 20 a4 00  	and	$4, $5, $4
  1aec6c: 21 04 63 30  	andi	$3, $3, 0x421
  1aec70: 21 10 44 00  	addu	$2, $2, $4
  1aec74: 43 10 02 00  	sra	$2, $2, 0x1
  1aec78: 21 10 43 00  	addu	$2, $2, $3
  1aec7c: 02 00 42 a5  	sh	$2, 0x2($10)
  1aec80: 36 00 02 3c  	lui	$2, 0x36
  1aec84: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1aec88: 01 00 62 a1  	sb	$2, 0x1($11)
  1aec8c: 36 00 03 3c  	lui	$3, 0x36
  1aec90: 02 00 62 91  	lbu	$2, 0x2($11)
  1aec94: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1aec98: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1aec9c: 2b 10 43 00  	sltu	$2, $2, $3
  1aeca0: 1e 00 40 10  	beqz	$2, 0x1aed1c <.text+0xaed1c>
  1aeca4: 36 00 03 3c  	lui	$3, 0x36
  1aeca8: 01 00 a3 91  	lbu	$3, 0x1($13)
  1aecac: 1b 00 60 50  	beqzl	$3, 0x1aed1c <.text+0xaed1c>
  1aecb0: 36 00 03 3c  	lui	$3, 0x36
  1aecb4: 02 00 84 91  	lbu	$4, 0x2($12)
  1aecb8: 6e 00 80 50  	beqzl	$4, 0x1aee74 <.text+0xaee74>
  1aecbc: 44 00 e2 8c  	lw	$2, 0x44($7)
  1aecc0: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aecc4: 54 00 82 10  	beq	$4, $2, 0x1aee18 <.text+0xaee18>
  1aecc8: de fb 04 24  	addiu	$4, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aeccc: 14 00 e2 8c  	lw	$2, 0x14($7)
  1aecd0: 40 18 03 00  	sll	$3, $3, 0x1
  1aecd4: 44 00 e5 8c  	lw	$5, 0x44($7)
  1aecd8: 40 10 02 00  	sll	$2, $2, 0x1
  1aecdc: 21 18 65 00  	addu	$3, $3, $5
  1aece0: 21 10 4a 00  	addu	$2, $2, $10
  1aece4: 04 00 45 94  	lhu	$5, 0x4($2)
  1aece8: 00 00 63 94  	lhu	$3, 0x0($3)
  1aecec: 24 10 64 00  	and	$2, $3, $4
  1aecf0: 24 18 65 00  	and	$3, $3, $5
  1aecf4: 24 20 a4 00  	and	$4, $5, $4
  1aecf8: 21 04 63 30  	andi	$3, $3, 0x421
  1aecfc: 21 10 44 00  	addu	$2, $2, $4
  1aed00: 43 10 02 00  	sra	$2, $2, 0x1
  1aed04: 21 10 43 00  	addu	$2, $2, $3
  1aed08: 04 00 42 a5  	sh	$2, 0x4($10)
  1aed0c: 36 00 02 3c  	lui	$2, 0x36
  1aed10: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1aed14: 02 00 62 a1  	sb	$2, 0x2($11)
  1aed18: 36 00 03 3c  	lui	$3, 0x36
  1aed1c: 03 00 62 91  	lbu	$2, 0x3($11)
  1aed20: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1aed24: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1aed28: 2b 10 43 00  	sltu	$2, $2, $3
  1aed2c: 1d 00 40 10  	beqz	$2, 0x1aeda4 <.text+0xaeda4>
  1aed30: 00 00 00 00  	nop
  1aed34: 00 00 a3 91  	lbu	$3, 0x0($13)
  1aed38: 1a 00 60 10  	beqz	$3, 0x1aeda4 <.text+0xaeda4>
  1aed3c: 00 00 00 00  	nop
  1aed40: 03 00 85 91  	lbu	$5, 0x3($12)
  1aed44: 30 00 a0 50  	beqzl	$5, 0x1aee08 <.text+0xaee08>
  1aed48: 44 00 e2 8c  	lw	$2, 0x44($7)
  1aed4c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aed50: 16 00 a2 10  	beq	$5, $2, 0x1aedac <.text+0xaedac>
  1aed54: de fb 04 24  	addiu	$4, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aed58: 14 00 e2 8c  	lw	$2, 0x14($7)
  1aed5c: 40 18 03 00  	sll	$3, $3, 0x1
  1aed60: 44 00 e5 8c  	lw	$5, 0x44($7)
  1aed64: 40 10 02 00  	sll	$2, $2, 0x1
  1aed68: 21 18 65 00  	addu	$3, $3, $5
  1aed6c: 21 10 4a 00  	addu	$2, $2, $10
  1aed70: 06 00 45 94  	lhu	$5, 0x6($2)
  1aed74: 00 00 63 94  	lhu	$3, 0x0($3)
  1aed78: 24 10 64 00  	and	$2, $3, $4
  1aed7c: 24 18 65 00  	and	$3, $3, $5
  1aed80: 24 20 a4 00  	and	$4, $5, $4
  1aed84: 21 04 63 30  	andi	$3, $3, 0x421
  1aed88: 21 10 44 00  	addu	$2, $2, $4
  1aed8c: 43 10 02 00  	sra	$2, $2, 0x1
  1aed90: 21 10 43 00  	addu	$2, $2, $3
  1aed94: 06 00 42 a5  	sh	$2, 0x6($10)
  1aed98: 36 00 02 3c  	lui	$2, 0x36
  1aed9c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1aeda0: 03 00 62 a1  	sb	$2, 0x3($11)
  1aeda4: 08 00 e0 03  	jr	$ra
  1aeda8: 00 00 00 00  	nop
  1aedac: 44 00 e4 8c  	lw	$4, 0x44($7)
  1aedb0: 40 18 03 00  	sll	$3, $3, 0x1
  1aedb4: 50 00 e6 8c  	lw	$6, 0x50($7)
  1aedb8: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aedbc: 21 18 64 00  	addu	$3, $3, $4
  1aedc0: 18 00 e8 8c  	lw	$8, 0x18($7)
  1aedc4: 00 00 64 94  	lhu	$4, 0x0($3)
  1aedc8: 24 28 c2 00  	and	$5, $6, $2
  1aedcc: 50 00 e7 94  	lhu	$7, 0x50($7)
  1aedd0: ff ff 83 30  	andi	$3, $4, 0xffff
  1aedd4: 24 10 62 00  	and	$2, $3, $2
  1aedd8: 26 20 87 00  	xor	$4, $4, $7
  1aeddc: 24 18 66 00  	and	$3, $3, $6
  1aede0: 21 10 45 00  	addu	$2, $2, $5
  1aede4: 21 04 63 30  	andi	$3, $3, 0x421
  1aede8: 42 10 02 00  	srl	$2, $2, 0x1
  1aedec: 21 10 43 00  	addu	$2, $2, $3
  1aedf0: 21 04 84 30  	andi	$4, $4, 0x421
  1aedf4: 40 10 02 00  	sll	$2, $2, 0x1
  1aedf8: 21 10 48 00  	addu	$2, $2, $8
  1aedfc: 00 00 42 94  	lhu	$2, 0x0($2)
  1aee00: e4 ff 00 10  	b	0x1aed94 <.text+0xaed94>
  1aee04: 25 10 44 00  	or	$2, $2, $4
  1aee08: 40 18 03 00  	sll	$3, $3, 0x1
  1aee0c: 21 18 62 00  	addu	$3, $3, $2
  1aee10: e0 ff 00 10  	b	0x1aed94 <.text+0xaed94>
  1aee14: 00 00 62 94  	lhu	$2, 0x0($3)
  1aee18: 44 00 e4 8c  	lw	$4, 0x44($7)
  1aee1c: 40 18 03 00  	sll	$3, $3, 0x1
  1aee20: 50 00 e6 8c  	lw	$6, 0x50($7)
  1aee24: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aee28: 21 18 64 00  	addu	$3, $3, $4
  1aee2c: 18 00 e8 8c  	lw	$8, 0x18($7)
  1aee30: 00 00 64 94  	lhu	$4, 0x0($3)
  1aee34: 24 28 c2 00  	and	$5, $6, $2
  1aee38: 50 00 e7 94  	lhu	$7, 0x50($7)
  1aee3c: ff ff 83 30  	andi	$3, $4, 0xffff
  1aee40: 24 10 62 00  	and	$2, $3, $2
  1aee44: 26 20 87 00  	xor	$4, $4, $7
  1aee48: 24 18 66 00  	and	$3, $3, $6
  1aee4c: 21 10 45 00  	addu	$2, $2, $5
  1aee50: 21 04 63 30  	andi	$3, $3, 0x421
  1aee54: 42 10 02 00  	srl	$2, $2, 0x1
  1aee58: 21 10 43 00  	addu	$2, $2, $3
  1aee5c: 21 04 84 30  	andi	$4, $4, 0x421
  1aee60: 40 10 02 00  	sll	$2, $2, 0x1
  1aee64: 21 10 48 00  	addu	$2, $2, $8
  1aee68: 00 00 42 94  	lhu	$2, 0x0($2)
  1aee6c: a6 ff 00 10  	b	0x1aed08 <.text+0xaed08>
  1aee70: 25 10 44 00  	or	$2, $2, $4
  1aee74: 40 18 03 00  	sll	$3, $3, 0x1
  1aee78: 21 18 62 00  	addu	$3, $3, $2
  1aee7c: a2 ff 00 10  	b	0x1aed08 <.text+0xaed08>
  1aee80: 00 00 62 94  	lhu	$2, 0x0($3)
  1aee84: 44 00 e4 8c  	lw	$4, 0x44($7)
  1aee88: 40 18 03 00  	sll	$3, $3, 0x1
  1aee8c: 50 00 e6 8c  	lw	$6, 0x50($7)
  1aee90: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aee94: 21 18 64 00  	addu	$3, $3, $4
  1aee98: 18 00 e8 8c  	lw	$8, 0x18($7)
  1aee9c: 00 00 64 94  	lhu	$4, 0x0($3)
  1aeea0: 24 28 c2 00  	and	$5, $6, $2
  1aeea4: 50 00 e7 94  	lhu	$7, 0x50($7)
  1aeea8: ff ff 83 30  	andi	$3, $4, 0xffff
  1aeeac: 24 10 62 00  	and	$2, $3, $2
  1aeeb0: 26 20 87 00  	xor	$4, $4, $7
  1aeeb4: 24 18 66 00  	and	$3, $3, $6
  1aeeb8: 21 10 45 00  	addu	$2, $2, $5
  1aeebc: 21 04 63 30  	andi	$3, $3, 0x421
  1aeec0: 42 10 02 00  	srl	$2, $2, 0x1
  1aeec4: 21 10 43 00  	addu	$2, $2, $3
  1aeec8: 21 04 84 30  	andi	$4, $4, 0x421
  1aeecc: 40 10 02 00  	sll	$2, $2, 0x1
  1aeed0: 21 10 48 00  	addu	$2, $2, $8
  1aeed4: 00 00 42 94  	lhu	$2, 0x0($2)
  1aeed8: 68 ff 00 10  	b	0x1aec7c <.text+0xaec7c>
  1aeedc: 25 10 44 00  	or	$2, $2, $4
  1aeee0: 40 18 03 00  	sll	$3, $3, 0x1
  1aeee4: 21 18 62 00  	addu	$3, $3, $2
  1aeee8: 64 ff 00 10  	b	0x1aec7c <.text+0xaec7c>
  1aeeec: 00 00 62 94  	lhu	$2, 0x0($3)
  1aeef0: 44 00 24 8d  	lw	$4, 0x44($9)
  1aeef4: 40 18 03 00  	sll	$3, $3, 0x1
  1aeef8: 50 00 26 8d  	lw	$6, 0x50($9)
  1aeefc: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1aef00: 21 18 64 00  	addu	$3, $3, $4
  1aef04: 18 00 28 8d  	lw	$8, 0x18($9)
  1aef08: 00 00 64 94  	lhu	$4, 0x0($3)
  1aef0c: 24 28 c2 00  	and	$5, $6, $2
  1aef10: 50 00 27 95  	lhu	$7, 0x50($9)
  1aef14: ff ff 83 30  	andi	$3, $4, 0xffff
  1aef18: 24 10 62 00  	and	$2, $3, $2
  1aef1c: 26 20 87 00  	xor	$4, $4, $7
  1aef20: 24 18 66 00  	and	$3, $3, $6
  1aef24: 21 10 45 00  	addu	$2, $2, $5
  1aef28: 21 04 63 30  	andi	$3, $3, 0x421
  1aef2c: 42 10 02 00  	srl	$2, $2, 0x1
  1aef30: 21 10 43 00  	addu	$2, $2, $3
  1aef34: 21 04 84 30  	andi	$4, $4, 0x421
  1aef38: 40 10 02 00  	sll	$2, $2, 0x1
  1aef3c: 21 10 48 00  	addu	$2, $2, $8
  1aef40: 00 00 42 94  	lhu	$2, 0x0($2)
  1aef44: 2a ff 00 10  	b	0x1aebf0 <.text+0xaebf0>
  1aef48: 25 10 44 00  	or	$2, $2, $4
  1aef4c: 40 18 03 00  	sll	$3, $3, 0x1
  1aef50: 21 18 62 00  	addu	$3, $3, $2
  1aef54: 26 ff 00 10  	b	0x1aebf0 <.text+0xaebf0>
  1aef58: 00 00 62 94  	lhu	$2, 0x0($3)
  1aef5c: 36 00 02 3c  	lui	$2, 0x36
  1aef60: 2d 60 a0 00  	move	$12, $5
  1aef64: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1aef68: 40 38 04 00  	sll	$7, $4, 0x1
  1aef6c: 08 00 02 8d  	lw	$2, 0x8($8)
  1aef70: 4c 00 06 91  	lbu	$6, 0x4c($8)
  1aef74: 21 50 44 00  	addu	$10, $2, $4
  1aef78: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1aef7c: 00 00 42 91  	lbu	$2, 0x0($10)
  1aef80: 0c 00 05 8d  	lw	$5, 0xc($8)
  1aef84: 21 48 67 00  	addu	$9, $3, $7
  1aef88: 2b 10 46 00  	sltu	$2, $2, $6
  1aef8c: 21 00 40 10  	beqz	$2, 0x1af014 <.text+0xaf014>
  1aef90: 21 58 a4 00  	addu	$11, $5, $4
  1aef94: 00 00 83 91  	lbu	$3, 0x0($12)
  1aef98: 1f 00 60 50  	beqzl	$3, 0x1af018 <.text+0xaf018>
  1aef9c: 36 00 03 3c  	lui	$3, 0x36
  1aefa0: 00 00 64 91  	lbu	$4, 0x0($11)
  1aefa4: d6 00 80 50  	beqzl	$4, 0x1af300 <.text+0xaf300>
  1aefa8: 44 00 02 8d  	lw	$2, 0x44($8)
  1aefac: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aefb0: c5 00 82 10  	beq	$4, $2, 0x1af2c8 <.text+0xaf2c8>
  1aefb4: 44 00 04 8d  	lw	$4, 0x44($8)
  1aefb8: 14 00 02 8d  	lw	$2, 0x14($8)
  1aefbc: 40 18 03 00  	sll	$3, $3, 0x1
  1aefc0: 40 10 02 00  	sll	$2, $2, 0x1
  1aefc4: 1c 00 06 8d  	lw	$6, 0x1c($8)
  1aefc8: 21 18 64 00  	addu	$3, $3, $4
  1aefcc: 21 10 49 00  	addu	$2, $2, $9
  1aefd0: 00 00 64 94  	lhu	$4, 0x0($3)
  1aefd4: 00 00 45 94  	lhu	$5, 0x0($2)
  1aefd8: 20 84 82 34  	ori	$2, $4, 0x8420
  1aefdc: 21 04 84 30  	andi	$4, $4, 0x421
  1aefe0: de fb a3 30  	andi	$3, $5, 0xfbde
  1aefe4: 21 04 a5 30  	andi	$5, $5, 0x421
  1aefe8: 23 10 43 00  	subu	$2, $2, $3
  1aefec: 43 10 02 00  	sra	$2, $2, 0x1
  1aeff0: 40 10 02 00  	sll	$2, $2, 0x1
  1aeff4: 21 10 46 00  	addu	$2, $2, $6
  1aeff8: 00 00 42 94  	lhu	$2, 0x0($2)
  1aeffc: 21 10 44 00  	addu	$2, $2, $4
  1af000: 23 10 45 00  	subu	$2, $2, $5
  1af004: 00 00 22 a5  	sh	$2, 0x0($9)
  1af008: 36 00 02 3c  	lui	$2, 0x36
  1af00c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af010: 00 00 42 a1  	sb	$2, 0x0($10)
  1af014: 36 00 03 3c  	lui	$3, 0x36
  1af018: 01 00 42 91  	lbu	$2, 0x1($10)
  1af01c: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af020: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1af024: 2b 10 43 00  	sltu	$2, $2, $3
  1af028: 22 00 40 10  	beqz	$2, 0x1af0b4 <.text+0xaf0b4>
  1af02c: 36 00 03 3c  	lui	$3, 0x36
  1af030: 01 00 83 91  	lbu	$3, 0x1($12)
  1af034: 1f 00 60 50  	beqzl	$3, 0x1af0b4 <.text+0xaf0b4>
  1af038: 36 00 03 3c  	lui	$3, 0x36
  1af03c: 01 00 64 91  	lbu	$4, 0x1($11)
  1af040: 9d 00 80 50  	beqzl	$4, 0x1af2b8 <.text+0xaf2b8>
  1af044: 44 00 a2 8c  	lw	$2, 0x44($5)
  1af048: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af04c: 8c 00 82 10  	beq	$4, $2, 0x1af280 <.text+0xaf280>
  1af050: 44 00 a4 8c  	lw	$4, 0x44($5)
  1af054: 14 00 a2 8c  	lw	$2, 0x14($5)
  1af058: 40 18 03 00  	sll	$3, $3, 0x1
  1af05c: 40 10 02 00  	sll	$2, $2, 0x1
  1af060: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af064: 21 18 64 00  	addu	$3, $3, $4
  1af068: 21 10 49 00  	addu	$2, $2, $9
  1af06c: 00 00 64 94  	lhu	$4, 0x0($3)
  1af070: 02 00 45 94  	lhu	$5, 0x2($2)
  1af074: 20 84 82 34  	ori	$2, $4, 0x8420
  1af078: 21 04 84 30  	andi	$4, $4, 0x421
  1af07c: de fb a3 30  	andi	$3, $5, 0xfbde
  1af080: 21 04 a5 30  	andi	$5, $5, 0x421
  1af084: 23 10 43 00  	subu	$2, $2, $3
  1af088: 43 10 02 00  	sra	$2, $2, 0x1
  1af08c: 40 10 02 00  	sll	$2, $2, 0x1
  1af090: 21 10 46 00  	addu	$2, $2, $6
  1af094: 00 00 42 94  	lhu	$2, 0x0($2)
  1af098: 21 10 44 00  	addu	$2, $2, $4
  1af09c: 23 10 45 00  	subu	$2, $2, $5
  1af0a0: 02 00 22 a5  	sh	$2, 0x2($9)
  1af0a4: 36 00 02 3c  	lui	$2, 0x36
  1af0a8: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af0ac: 01 00 42 a1  	sb	$2, 0x1($10)
  1af0b0: 36 00 03 3c  	lui	$3, 0x36
  1af0b4: 02 00 42 91  	lbu	$2, 0x2($10)
  1af0b8: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af0bc: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1af0c0: 2b 10 43 00  	sltu	$2, $2, $3
  1af0c4: 22 00 40 10  	beqz	$2, 0x1af150 <.text+0xaf150>
  1af0c8: 36 00 03 3c  	lui	$3, 0x36
  1af0cc: 02 00 83 91  	lbu	$3, 0x2($12)
  1af0d0: 1f 00 60 50  	beqzl	$3, 0x1af150 <.text+0xaf150>
  1af0d4: 36 00 03 3c  	lui	$3, 0x36
  1af0d8: 02 00 64 91  	lbu	$4, 0x2($11)
  1af0dc: 64 00 80 50  	beqzl	$4, 0x1af270 <.text+0xaf270>
  1af0e0: 44 00 a2 8c  	lw	$2, 0x44($5)
  1af0e4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af0e8: 53 00 82 10  	beq	$4, $2, 0x1af238 <.text+0xaf238>
  1af0ec: 44 00 a4 8c  	lw	$4, 0x44($5)
  1af0f0: 14 00 a2 8c  	lw	$2, 0x14($5)
  1af0f4: 40 18 03 00  	sll	$3, $3, 0x1
  1af0f8: 40 10 02 00  	sll	$2, $2, 0x1
  1af0fc: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af100: 21 18 64 00  	addu	$3, $3, $4
  1af104: 21 10 49 00  	addu	$2, $2, $9
  1af108: 00 00 64 94  	lhu	$4, 0x0($3)
  1af10c: 04 00 45 94  	lhu	$5, 0x4($2)
  1af110: 20 84 82 34  	ori	$2, $4, 0x8420
  1af114: 21 04 84 30  	andi	$4, $4, 0x421
  1af118: de fb a3 30  	andi	$3, $5, 0xfbde
  1af11c: 21 04 a5 30  	andi	$5, $5, 0x421
  1af120: 23 10 43 00  	subu	$2, $2, $3
  1af124: 43 10 02 00  	sra	$2, $2, 0x1
  1af128: 40 10 02 00  	sll	$2, $2, 0x1
  1af12c: 21 10 46 00  	addu	$2, $2, $6
  1af130: 00 00 42 94  	lhu	$2, 0x0($2)
  1af134: 21 10 44 00  	addu	$2, $2, $4
  1af138: 23 10 45 00  	subu	$2, $2, $5
  1af13c: 04 00 22 a5  	sh	$2, 0x4($9)
  1af140: 36 00 02 3c  	lui	$2, 0x36
  1af144: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af148: 02 00 42 a1  	sb	$2, 0x2($10)
  1af14c: 36 00 03 3c  	lui	$3, 0x36
  1af150: 03 00 42 91  	lbu	$2, 0x3($10)
  1af154: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af158: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1af15c: 2b 10 43 00  	sltu	$2, $2, $3
  1af160: 21 00 40 10  	beqz	$2, 0x1af1e8 <.text+0xaf1e8>
  1af164: 00 00 00 00  	nop
  1af168: 03 00 83 91  	lbu	$3, 0x3($12)
  1af16c: 1e 00 60 10  	beqz	$3, 0x1af1e8 <.text+0xaf1e8>
  1af170: 00 00 00 00  	nop
  1af174: 03 00 65 91  	lbu	$5, 0x3($11)
  1af178: 2b 00 a0 50  	beqzl	$5, 0x1af228 <.text+0xaf228>
  1af17c: 44 00 e2 8c  	lw	$2, 0x44($7)
  1af180: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af184: 1a 00 a2 10  	beq	$5, $2, 0x1af1f0 <.text+0xaf1f0>
  1af188: 44 00 e4 8c  	lw	$4, 0x44($7)
  1af18c: 14 00 e2 8c  	lw	$2, 0x14($7)
  1af190: 40 18 03 00  	sll	$3, $3, 0x1
  1af194: 40 10 02 00  	sll	$2, $2, 0x1
  1af198: 1c 00 e6 8c  	lw	$6, 0x1c($7)
  1af19c: 21 18 64 00  	addu	$3, $3, $4
  1af1a0: 21 10 49 00  	addu	$2, $2, $9
  1af1a4: 00 00 64 94  	lhu	$4, 0x0($3)
  1af1a8: 06 00 45 94  	lhu	$5, 0x6($2)
  1af1ac: 20 84 82 34  	ori	$2, $4, 0x8420
  1af1b0: 21 04 84 30  	andi	$4, $4, 0x421
  1af1b4: de fb a3 30  	andi	$3, $5, 0xfbde
  1af1b8: 21 04 a5 30  	andi	$5, $5, 0x421
  1af1bc: 23 10 43 00  	subu	$2, $2, $3
  1af1c0: 43 10 02 00  	sra	$2, $2, 0x1
  1af1c4: 40 10 02 00  	sll	$2, $2, 0x1
  1af1c8: 21 10 46 00  	addu	$2, $2, $6
  1af1cc: 00 00 42 94  	lhu	$2, 0x0($2)
  1af1d0: 21 10 44 00  	addu	$2, $2, $4
  1af1d4: 23 10 45 00  	subu	$2, $2, $5
  1af1d8: 06 00 22 a5  	sh	$2, 0x6($9)
  1af1dc: 36 00 02 3c  	lui	$2, 0x36
  1af1e0: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af1e4: 03 00 42 a1  	sb	$2, 0x3($10)
  1af1e8: 08 00 e0 03  	jr	$ra
  1af1ec: 00 00 00 00  	nop
  1af1f0: 40 10 03 00  	sll	$2, $3, 0x1
  1af1f4: 50 00 e3 8c  	lw	$3, 0x50($7)
  1af1f8: 21 10 44 00  	addu	$2, $2, $4
  1af1fc: 50 00 e5 94  	lhu	$5, 0x50($7)
  1af200: 00 00 44 94  	lhu	$4, 0x0($2)
  1af204: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af208: 24 18 62 00  	and	$3, $3, $2
  1af20c: 1c 00 e6 8c  	lw	$6, 0x1c($7)
  1af210: 20 84 82 34  	ori	$2, $4, 0x8420
  1af214: 21 04 a5 30  	andi	$5, $5, 0x421
  1af218: 23 10 43 00  	subu	$2, $2, $3
  1af21c: 21 04 84 30  	andi	$4, $4, 0x421
  1af220: e8 ff 00 10  	b	0x1af1c4 <.text+0xaf1c4>
  1af224: 42 10 02 00  	srl	$2, $2, 0x1
  1af228: 40 18 03 00  	sll	$3, $3, 0x1
  1af22c: 21 18 62 00  	addu	$3, $3, $2
  1af230: e9 ff 00 10  	b	0x1af1d8 <.text+0xaf1d8>
  1af234: 00 00 62 94  	lhu	$2, 0x0($3)
  1af238: 40 10 03 00  	sll	$2, $3, 0x1
  1af23c: 50 00 a3 8c  	lw	$3, 0x50($5)
  1af240: 21 10 44 00  	addu	$2, $2, $4
  1af244: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af248: 00 00 44 94  	lhu	$4, 0x0($2)
  1af24c: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af250: 24 18 62 00  	and	$3, $3, $2
  1af254: 50 00 a5 94  	lhu	$5, 0x50($5)
  1af258: 20 84 82 34  	ori	$2, $4, 0x8420
  1af25c: 21 04 84 30  	andi	$4, $4, 0x421
  1af260: 23 10 43 00  	subu	$2, $2, $3
  1af264: 21 04 a5 30  	andi	$5, $5, 0x421
  1af268: af ff 00 10  	b	0x1af128 <.text+0xaf128>
  1af26c: 42 10 02 00  	srl	$2, $2, 0x1
  1af270: 40 18 03 00  	sll	$3, $3, 0x1
  1af274: 21 18 62 00  	addu	$3, $3, $2
  1af278: b0 ff 00 10  	b	0x1af13c <.text+0xaf13c>
  1af27c: 00 00 62 94  	lhu	$2, 0x0($3)
  1af280: 40 10 03 00  	sll	$2, $3, 0x1
  1af284: 50 00 a3 8c  	lw	$3, 0x50($5)
  1af288: 21 10 44 00  	addu	$2, $2, $4
  1af28c: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af290: 00 00 44 94  	lhu	$4, 0x0($2)
  1af294: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af298: 24 18 62 00  	and	$3, $3, $2
  1af29c: 50 00 a5 94  	lhu	$5, 0x50($5)
  1af2a0: 20 84 82 34  	ori	$2, $4, 0x8420
  1af2a4: 21 04 84 30  	andi	$4, $4, 0x421
  1af2a8: 23 10 43 00  	subu	$2, $2, $3
  1af2ac: 21 04 a5 30  	andi	$5, $5, 0x421
  1af2b0: 76 ff 00 10  	b	0x1af08c <.text+0xaf08c>
  1af2b4: 42 10 02 00  	srl	$2, $2, 0x1
  1af2b8: 40 18 03 00  	sll	$3, $3, 0x1
  1af2bc: 21 18 62 00  	addu	$3, $3, $2
  1af2c0: 77 ff 00 10  	b	0x1af0a0 <.text+0xaf0a0>
  1af2c4: 00 00 62 94  	lhu	$2, 0x0($3)
  1af2c8: 40 10 03 00  	sll	$2, $3, 0x1
  1af2cc: 50 00 03 8d  	lw	$3, 0x50($8)
  1af2d0: 21 10 44 00  	addu	$2, $2, $4
  1af2d4: 50 00 05 95  	lhu	$5, 0x50($8)
  1af2d8: 00 00 44 94  	lhu	$4, 0x0($2)
  1af2dc: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af2e0: 24 18 62 00  	and	$3, $3, $2
  1af2e4: 1c 00 06 8d  	lw	$6, 0x1c($8)
  1af2e8: 20 84 82 34  	ori	$2, $4, 0x8420
  1af2ec: 21 04 a5 30  	andi	$5, $5, 0x421
  1af2f0: 23 10 43 00  	subu	$2, $2, $3
  1af2f4: 21 04 84 30  	andi	$4, $4, 0x421
  1af2f8: 3d ff 00 10  	b	0x1aeff0 <.text+0xaeff0>
  1af2fc: 42 10 02 00  	srl	$2, $2, 0x1
  1af300: 40 18 03 00  	sll	$3, $3, 0x1
  1af304: 21 18 62 00  	addu	$3, $3, $2
  1af308: 3e ff 00 10  	b	0x1af004 <.text+0xaf004>
  1af30c: 00 00 62 94  	lhu	$2, 0x0($3)
  1af310: 36 00 02 3c  	lui	$2, 0x36
  1af314: 2d 60 a0 00  	move	$12, $5
  1af318: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1af31c: 40 38 04 00  	sll	$7, $4, 0x1
  1af320: 08 00 02 8d  	lw	$2, 0x8($8)
  1af324: 4c 00 06 91  	lbu	$6, 0x4c($8)
  1af328: 21 50 44 00  	addu	$10, $2, $4
  1af32c: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1af330: 00 00 42 91  	lbu	$2, 0x0($10)
  1af334: 0c 00 05 8d  	lw	$5, 0xc($8)
  1af338: 21 48 67 00  	addu	$9, $3, $7
  1af33c: 2b 10 46 00  	sltu	$2, $2, $6
  1af340: 21 00 40 10  	beqz	$2, 0x1af3c8 <.text+0xaf3c8>
  1af344: 21 58 a4 00  	addu	$11, $5, $4
  1af348: 03 00 83 91  	lbu	$3, 0x3($12)
  1af34c: 1f 00 60 50  	beqzl	$3, 0x1af3cc <.text+0xaf3cc>
  1af350: 36 00 03 3c  	lui	$3, 0x36
  1af354: 00 00 64 91  	lbu	$4, 0x0($11)
  1af358: d6 00 80 50  	beqzl	$4, 0x1af6b4 <.text+0xaf6b4>
  1af35c: 44 00 02 8d  	lw	$2, 0x44($8)
  1af360: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af364: c5 00 82 10  	beq	$4, $2, 0x1af67c <.text+0xaf67c>
  1af368: 44 00 04 8d  	lw	$4, 0x44($8)
  1af36c: 14 00 02 8d  	lw	$2, 0x14($8)
  1af370: 40 18 03 00  	sll	$3, $3, 0x1
  1af374: 40 10 02 00  	sll	$2, $2, 0x1
  1af378: 1c 00 06 8d  	lw	$6, 0x1c($8)
  1af37c: 21 18 64 00  	addu	$3, $3, $4
  1af380: 21 10 49 00  	addu	$2, $2, $9
  1af384: 00 00 64 94  	lhu	$4, 0x0($3)
  1af388: 00 00 45 94  	lhu	$5, 0x0($2)
  1af38c: 20 84 82 34  	ori	$2, $4, 0x8420
  1af390: 21 04 84 30  	andi	$4, $4, 0x421
  1af394: de fb a3 30  	andi	$3, $5, 0xfbde
  1af398: 21 04 a5 30  	andi	$5, $5, 0x421
  1af39c: 23 10 43 00  	subu	$2, $2, $3
  1af3a0: 43 10 02 00  	sra	$2, $2, 0x1
  1af3a4: 40 10 02 00  	sll	$2, $2, 0x1
  1af3a8: 21 10 46 00  	addu	$2, $2, $6
  1af3ac: 00 00 42 94  	lhu	$2, 0x0($2)
  1af3b0: 21 10 44 00  	addu	$2, $2, $4
  1af3b4: 23 10 45 00  	subu	$2, $2, $5
  1af3b8: 00 00 22 a5  	sh	$2, 0x0($9)
  1af3bc: 36 00 02 3c  	lui	$2, 0x36
  1af3c0: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af3c4: 00 00 42 a1  	sb	$2, 0x0($10)
  1af3c8: 36 00 03 3c  	lui	$3, 0x36
  1af3cc: 01 00 42 91  	lbu	$2, 0x1($10)
  1af3d0: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af3d4: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1af3d8: 2b 10 43 00  	sltu	$2, $2, $3
  1af3dc: 22 00 40 10  	beqz	$2, 0x1af468 <.text+0xaf468>
  1af3e0: 36 00 03 3c  	lui	$3, 0x36
  1af3e4: 02 00 83 91  	lbu	$3, 0x2($12)
  1af3e8: 1f 00 60 50  	beqzl	$3, 0x1af468 <.text+0xaf468>
  1af3ec: 36 00 03 3c  	lui	$3, 0x36
  1af3f0: 01 00 64 91  	lbu	$4, 0x1($11)
  1af3f4: 9d 00 80 50  	beqzl	$4, 0x1af66c <.text+0xaf66c>
  1af3f8: 44 00 a2 8c  	lw	$2, 0x44($5)
  1af3fc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af400: 8c 00 82 10  	beq	$4, $2, 0x1af634 <.text+0xaf634>
  1af404: 44 00 a4 8c  	lw	$4, 0x44($5)
  1af408: 14 00 a2 8c  	lw	$2, 0x14($5)
  1af40c: 40 18 03 00  	sll	$3, $3, 0x1
  1af410: 40 10 02 00  	sll	$2, $2, 0x1
  1af414: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af418: 21 18 64 00  	addu	$3, $3, $4
  1af41c: 21 10 49 00  	addu	$2, $2, $9
  1af420: 00 00 64 94  	lhu	$4, 0x0($3)
  1af424: 02 00 45 94  	lhu	$5, 0x2($2)
  1af428: 20 84 82 34  	ori	$2, $4, 0x8420
  1af42c: 21 04 84 30  	andi	$4, $4, 0x421
  1af430: de fb a3 30  	andi	$3, $5, 0xfbde
  1af434: 21 04 a5 30  	andi	$5, $5, 0x421
  1af438: 23 10 43 00  	subu	$2, $2, $3
  1af43c: 43 10 02 00  	sra	$2, $2, 0x1
  1af440: 40 10 02 00  	sll	$2, $2, 0x1
  1af444: 21 10 46 00  	addu	$2, $2, $6
  1af448: 00 00 42 94  	lhu	$2, 0x0($2)
  1af44c: 21 10 44 00  	addu	$2, $2, $4
  1af450: 23 10 45 00  	subu	$2, $2, $5
  1af454: 02 00 22 a5  	sh	$2, 0x2($9)
  1af458: 36 00 02 3c  	lui	$2, 0x36
  1af45c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af460: 01 00 42 a1  	sb	$2, 0x1($10)
  1af464: 36 00 03 3c  	lui	$3, 0x36
  1af468: 02 00 42 91  	lbu	$2, 0x2($10)
  1af46c: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af470: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1af474: 2b 10 43 00  	sltu	$2, $2, $3
  1af478: 22 00 40 10  	beqz	$2, 0x1af504 <.text+0xaf504>
  1af47c: 36 00 03 3c  	lui	$3, 0x36
  1af480: 01 00 83 91  	lbu	$3, 0x1($12)
  1af484: 1f 00 60 50  	beqzl	$3, 0x1af504 <.text+0xaf504>
  1af488: 36 00 03 3c  	lui	$3, 0x36
  1af48c: 02 00 64 91  	lbu	$4, 0x2($11)
  1af490: 64 00 80 50  	beqzl	$4, 0x1af624 <.text+0xaf624>
  1af494: 44 00 a2 8c  	lw	$2, 0x44($5)
  1af498: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af49c: 53 00 82 10  	beq	$4, $2, 0x1af5ec <.text+0xaf5ec>
  1af4a0: 44 00 a4 8c  	lw	$4, 0x44($5)
  1af4a4: 14 00 a2 8c  	lw	$2, 0x14($5)
  1af4a8: 40 18 03 00  	sll	$3, $3, 0x1
  1af4ac: 40 10 02 00  	sll	$2, $2, 0x1
  1af4b0: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af4b4: 21 18 64 00  	addu	$3, $3, $4
  1af4b8: 21 10 49 00  	addu	$2, $2, $9
  1af4bc: 00 00 64 94  	lhu	$4, 0x0($3)
  1af4c0: 04 00 45 94  	lhu	$5, 0x4($2)
  1af4c4: 20 84 82 34  	ori	$2, $4, 0x8420
  1af4c8: 21 04 84 30  	andi	$4, $4, 0x421
  1af4cc: de fb a3 30  	andi	$3, $5, 0xfbde
  1af4d0: 21 04 a5 30  	andi	$5, $5, 0x421
  1af4d4: 23 10 43 00  	subu	$2, $2, $3
  1af4d8: 43 10 02 00  	sra	$2, $2, 0x1
  1af4dc: 40 10 02 00  	sll	$2, $2, 0x1
  1af4e0: 21 10 46 00  	addu	$2, $2, $6
  1af4e4: 00 00 42 94  	lhu	$2, 0x0($2)
  1af4e8: 21 10 44 00  	addu	$2, $2, $4
  1af4ec: 23 10 45 00  	subu	$2, $2, $5
  1af4f0: 04 00 22 a5  	sh	$2, 0x4($9)
  1af4f4: 36 00 02 3c  	lui	$2, 0x36
  1af4f8: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af4fc: 02 00 42 a1  	sb	$2, 0x2($10)
  1af500: 36 00 03 3c  	lui	$3, 0x36
  1af504: 03 00 42 91  	lbu	$2, 0x3($10)
  1af508: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af50c: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1af510: 2b 10 43 00  	sltu	$2, $2, $3
  1af514: 21 00 40 10  	beqz	$2, 0x1af59c <.text+0xaf59c>
  1af518: 00 00 00 00  	nop
  1af51c: 00 00 83 91  	lbu	$3, 0x0($12)
  1af520: 1e 00 60 10  	beqz	$3, 0x1af59c <.text+0xaf59c>
  1af524: 00 00 00 00  	nop
  1af528: 03 00 65 91  	lbu	$5, 0x3($11)
  1af52c: 2b 00 a0 50  	beqzl	$5, 0x1af5dc <.text+0xaf5dc>
  1af530: 44 00 e2 8c  	lw	$2, 0x44($7)
  1af534: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af538: 1a 00 a2 10  	beq	$5, $2, 0x1af5a4 <.text+0xaf5a4>
  1af53c: 44 00 e4 8c  	lw	$4, 0x44($7)
  1af540: 14 00 e2 8c  	lw	$2, 0x14($7)
  1af544: 40 18 03 00  	sll	$3, $3, 0x1
  1af548: 40 10 02 00  	sll	$2, $2, 0x1
  1af54c: 1c 00 e6 8c  	lw	$6, 0x1c($7)
  1af550: 21 18 64 00  	addu	$3, $3, $4
  1af554: 21 10 49 00  	addu	$2, $2, $9
  1af558: 00 00 64 94  	lhu	$4, 0x0($3)
  1af55c: 06 00 45 94  	lhu	$5, 0x6($2)
  1af560: 20 84 82 34  	ori	$2, $4, 0x8420
  1af564: 21 04 84 30  	andi	$4, $4, 0x421
  1af568: de fb a3 30  	andi	$3, $5, 0xfbde
  1af56c: 21 04 a5 30  	andi	$5, $5, 0x421
  1af570: 23 10 43 00  	subu	$2, $2, $3
  1af574: 43 10 02 00  	sra	$2, $2, 0x1
  1af578: 40 10 02 00  	sll	$2, $2, 0x1
  1af57c: 21 10 46 00  	addu	$2, $2, $6
  1af580: 00 00 42 94  	lhu	$2, 0x0($2)
  1af584: 21 10 44 00  	addu	$2, $2, $4
  1af588: 23 10 45 00  	subu	$2, $2, $5
  1af58c: 06 00 22 a5  	sh	$2, 0x6($9)
  1af590: 36 00 02 3c  	lui	$2, 0x36
  1af594: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af598: 03 00 42 a1  	sb	$2, 0x3($10)
  1af59c: 08 00 e0 03  	jr	$ra
  1af5a0: 00 00 00 00  	nop
  1af5a4: 40 10 03 00  	sll	$2, $3, 0x1
  1af5a8: 50 00 e3 8c  	lw	$3, 0x50($7)
  1af5ac: 21 10 44 00  	addu	$2, $2, $4
  1af5b0: 50 00 e5 94  	lhu	$5, 0x50($7)
  1af5b4: 00 00 44 94  	lhu	$4, 0x0($2)
  1af5b8: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af5bc: 24 18 62 00  	and	$3, $3, $2
  1af5c0: 1c 00 e6 8c  	lw	$6, 0x1c($7)
  1af5c4: 20 84 82 34  	ori	$2, $4, 0x8420
  1af5c8: 21 04 a5 30  	andi	$5, $5, 0x421
  1af5cc: 23 10 43 00  	subu	$2, $2, $3
  1af5d0: 21 04 84 30  	andi	$4, $4, 0x421
  1af5d4: e8 ff 00 10  	b	0x1af578 <.text+0xaf578>
  1af5d8: 42 10 02 00  	srl	$2, $2, 0x1
  1af5dc: 40 18 03 00  	sll	$3, $3, 0x1
  1af5e0: 21 18 62 00  	addu	$3, $3, $2
  1af5e4: e9 ff 00 10  	b	0x1af58c <.text+0xaf58c>
  1af5e8: 00 00 62 94  	lhu	$2, 0x0($3)
  1af5ec: 40 10 03 00  	sll	$2, $3, 0x1
  1af5f0: 50 00 a3 8c  	lw	$3, 0x50($5)
  1af5f4: 21 10 44 00  	addu	$2, $2, $4
  1af5f8: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af5fc: 00 00 44 94  	lhu	$4, 0x0($2)
  1af600: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af604: 24 18 62 00  	and	$3, $3, $2
  1af608: 50 00 a5 94  	lhu	$5, 0x50($5)
  1af60c: 20 84 82 34  	ori	$2, $4, 0x8420
  1af610: 21 04 84 30  	andi	$4, $4, 0x421
  1af614: 23 10 43 00  	subu	$2, $2, $3
  1af618: 21 04 a5 30  	andi	$5, $5, 0x421
  1af61c: af ff 00 10  	b	0x1af4dc <.text+0xaf4dc>
  1af620: 42 10 02 00  	srl	$2, $2, 0x1
  1af624: 40 18 03 00  	sll	$3, $3, 0x1
  1af628: 21 18 62 00  	addu	$3, $3, $2
  1af62c: b0 ff 00 10  	b	0x1af4f0 <.text+0xaf4f0>
  1af630: 00 00 62 94  	lhu	$2, 0x0($3)
  1af634: 40 10 03 00  	sll	$2, $3, 0x1
  1af638: 50 00 a3 8c  	lw	$3, 0x50($5)
  1af63c: 21 10 44 00  	addu	$2, $2, $4
  1af640: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af644: 00 00 44 94  	lhu	$4, 0x0($2)
  1af648: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af64c: 24 18 62 00  	and	$3, $3, $2
  1af650: 50 00 a5 94  	lhu	$5, 0x50($5)
  1af654: 20 84 82 34  	ori	$2, $4, 0x8420
  1af658: 21 04 84 30  	andi	$4, $4, 0x421
  1af65c: 23 10 43 00  	subu	$2, $2, $3
  1af660: 21 04 a5 30  	andi	$5, $5, 0x421
  1af664: 76 ff 00 10  	b	0x1af440 <.text+0xaf440>
  1af668: 42 10 02 00  	srl	$2, $2, 0x1
  1af66c: 40 18 03 00  	sll	$3, $3, 0x1
  1af670: 21 18 62 00  	addu	$3, $3, $2
  1af674: 77 ff 00 10  	b	0x1af454 <.text+0xaf454>
  1af678: 00 00 62 94  	lhu	$2, 0x0($3)
  1af67c: 40 10 03 00  	sll	$2, $3, 0x1
  1af680: 50 00 03 8d  	lw	$3, 0x50($8)
  1af684: 21 10 44 00  	addu	$2, $2, $4
  1af688: 50 00 05 95  	lhu	$5, 0x50($8)
  1af68c: 00 00 44 94  	lhu	$4, 0x0($2)
  1af690: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af694: 24 18 62 00  	and	$3, $3, $2
  1af698: 1c 00 06 8d  	lw	$6, 0x1c($8)
  1af69c: 20 84 82 34  	ori	$2, $4, 0x8420
  1af6a0: 21 04 a5 30  	andi	$5, $5, 0x421
  1af6a4: 23 10 43 00  	subu	$2, $2, $3
  1af6a8: 21 04 84 30  	andi	$4, $4, 0x421
  1af6ac: 3d ff 00 10  	b	0x1af3a4 <.text+0xaf3a4>
  1af6b0: 42 10 02 00  	srl	$2, $2, 0x1
  1af6b4: 40 18 03 00  	sll	$3, $3, 0x1
  1af6b8: 21 18 62 00  	addu	$3, $3, $2
  1af6bc: 3e ff 00 10  	b	0x1af3b8 <.text+0xaf3b8>
  1af6c0: 00 00 62 94  	lhu	$2, 0x0($3)
  1af6c4: 36 00 02 3c  	lui	$2, 0x36
  1af6c8: 2d 60 a0 00  	move	$12, $5
  1af6cc: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1af6d0: 40 38 04 00  	sll	$7, $4, 0x1
  1af6d4: 08 00 02 8d  	lw	$2, 0x8($8)
  1af6d8: 4c 00 06 91  	lbu	$6, 0x4c($8)
  1af6dc: 21 50 44 00  	addu	$10, $2, $4
  1af6e0: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1af6e4: 00 00 42 91  	lbu	$2, 0x0($10)
  1af6e8: 0c 00 05 8d  	lw	$5, 0xc($8)
  1af6ec: 21 48 67 00  	addu	$9, $3, $7
  1af6f0: 2b 10 46 00  	sltu	$2, $2, $6
  1af6f4: 1d 00 40 10  	beqz	$2, 0x1af76c <.text+0xaf76c>
  1af6f8: 21 58 a4 00  	addu	$11, $5, $4
  1af6fc: 00 00 86 91  	lbu	$6, 0x0($12)
  1af700: 1b 00 c0 10  	beqz	$6, 0x1af770 <.text+0xaf770>
  1af704: 36 00 03 3c  	lui	$3, 0x36
  1af708: 00 00 63 91  	lbu	$3, 0x0($11)
  1af70c: da 00 60 50  	beqzl	$3, 0x1afa78 <.text+0xafa78>
  1af710: 44 00 02 8d  	lw	$2, 0x44($8)
  1af714: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af718: c4 00 62 10  	beq	$3, $2, 0x1afa2c <.text+0xafa2c>
  1af71c: 44 00 04 8d  	lw	$4, 0x44($8)
  1af720: 14 00 03 8d  	lw	$3, 0x14($8)
  1af724: 40 10 06 00  	sll	$2, $6, 0x1
  1af728: 40 18 03 00  	sll	$3, $3, 0x1
  1af72c: 20 00 05 8d  	lw	$5, 0x20($8)
  1af730: 21 10 44 00  	addu	$2, $2, $4
  1af734: 21 18 69 00  	addu	$3, $3, $9
  1af738: 00 00 42 94  	lhu	$2, 0x0($2)
  1af73c: 00 00 63 94  	lhu	$3, 0x0($3)
  1af740: 20 84 42 34  	ori	$2, $2, 0x8420
  1af744: de fb 63 30  	andi	$3, $3, 0xfbde
  1af748: 23 10 43 00  	subu	$2, $2, $3
  1af74c: 43 10 02 00  	sra	$2, $2, 0x1
  1af750: 40 10 02 00  	sll	$2, $2, 0x1
  1af754: 21 10 45 00  	addu	$2, $2, $5
  1af758: 00 00 42 94  	lhu	$2, 0x0($2)
  1af75c: 00 00 22 a5  	sh	$2, 0x0($9)
  1af760: 36 00 02 3c  	lui	$2, 0x36
  1af764: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af768: 00 00 42 a1  	sb	$2, 0x0($10)
  1af76c: 36 00 03 3c  	lui	$3, 0x36
  1af770: 01 00 42 91  	lbu	$2, 0x1($10)
  1af774: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af778: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1af77c: 2b 10 43 00  	sltu	$2, $2, $3
  1af780: 1e 00 40 10  	beqz	$2, 0x1af7fc <.text+0xaf7fc>
  1af784: 36 00 03 3c  	lui	$3, 0x36
  1af788: 01 00 86 91  	lbu	$6, 0x1($12)
  1af78c: 1c 00 c0 50  	beqzl	$6, 0x1af800 <.text+0xaf800>
  1af790: 02 00 42 91  	lbu	$2, 0x2($10)
  1af794: 01 00 63 91  	lbu	$3, 0x1($11)
  1af798: a0 00 60 50  	beqzl	$3, 0x1afa1c <.text+0xafa1c>
  1af79c: 44 00 a2 8c  	lw	$2, 0x44($5)
  1af7a0: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af7a4: 8a 00 62 10  	beq	$3, $2, 0x1af9d0 <.text+0xaf9d0>
  1af7a8: 44 00 a4 8c  	lw	$4, 0x44($5)
  1af7ac: 14 00 a3 8c  	lw	$3, 0x14($5)
  1af7b0: 40 10 06 00  	sll	$2, $6, 0x1
  1af7b4: 40 18 03 00  	sll	$3, $3, 0x1
  1af7b8: 20 00 a5 8c  	lw	$5, 0x20($5)
  1af7bc: 21 10 44 00  	addu	$2, $2, $4
  1af7c0: 21 18 69 00  	addu	$3, $3, $9
  1af7c4: 00 00 42 94  	lhu	$2, 0x0($2)
  1af7c8: 02 00 63 94  	lhu	$3, 0x2($3)
  1af7cc: 20 84 42 34  	ori	$2, $2, 0x8420
  1af7d0: de fb 63 30  	andi	$3, $3, 0xfbde
  1af7d4: 23 10 43 00  	subu	$2, $2, $3
  1af7d8: 43 10 02 00  	sra	$2, $2, 0x1
  1af7dc: 40 10 02 00  	sll	$2, $2, 0x1
  1af7e0: 21 10 45 00  	addu	$2, $2, $5
  1af7e4: 00 00 42 94  	lhu	$2, 0x0($2)
  1af7e8: 02 00 22 a5  	sh	$2, 0x2($9)
  1af7ec: 36 00 02 3c  	lui	$2, 0x36
  1af7f0: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af7f4: 01 00 42 a1  	sb	$2, 0x1($10)
  1af7f8: 36 00 03 3c  	lui	$3, 0x36
  1af7fc: 02 00 42 91  	lbu	$2, 0x2($10)
  1af800: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af804: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1af808: 2b 10 43 00  	sltu	$2, $2, $3
  1af80c: 1e 00 40 10  	beqz	$2, 0x1af888 <.text+0xaf888>
  1af810: 36 00 03 3c  	lui	$3, 0x36
  1af814: 02 00 86 91  	lbu	$6, 0x2($12)
  1af818: 1c 00 c0 50  	beqzl	$6, 0x1af88c <.text+0xaf88c>
  1af81c: 03 00 42 91  	lbu	$2, 0x3($10)
  1af820: 02 00 63 91  	lbu	$3, 0x2($11)
  1af824: 66 00 60 50  	beqzl	$3, 0x1af9c0 <.text+0xaf9c0>
  1af828: 44 00 a2 8c  	lw	$2, 0x44($5)
  1af82c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af830: 50 00 62 10  	beq	$3, $2, 0x1af974 <.text+0xaf974>
  1af834: 44 00 a4 8c  	lw	$4, 0x44($5)
  1af838: 14 00 a3 8c  	lw	$3, 0x14($5)
  1af83c: 40 10 06 00  	sll	$2, $6, 0x1
  1af840: 40 18 03 00  	sll	$3, $3, 0x1
  1af844: 20 00 a5 8c  	lw	$5, 0x20($5)
  1af848: 21 10 44 00  	addu	$2, $2, $4
  1af84c: 21 18 69 00  	addu	$3, $3, $9
  1af850: 00 00 42 94  	lhu	$2, 0x0($2)
  1af854: 04 00 63 94  	lhu	$3, 0x4($3)
  1af858: 20 84 42 34  	ori	$2, $2, 0x8420
  1af85c: de fb 63 30  	andi	$3, $3, 0xfbde
  1af860: 23 10 43 00  	subu	$2, $2, $3
  1af864: 43 10 02 00  	sra	$2, $2, 0x1
  1af868: 40 10 02 00  	sll	$2, $2, 0x1
  1af86c: 21 10 45 00  	addu	$2, $2, $5
  1af870: 00 00 42 94  	lhu	$2, 0x0($2)
  1af874: 04 00 22 a5  	sh	$2, 0x4($9)
  1af878: 36 00 02 3c  	lui	$2, 0x36
  1af87c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af880: 02 00 42 a1  	sb	$2, 0x2($10)
  1af884: 36 00 03 3c  	lui	$3, 0x36
  1af888: 03 00 42 91  	lbu	$2, 0x3($10)
  1af88c: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1af890: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1af894: 2b 10 43 00  	sltu	$2, $2, $3
  1af898: 1d 00 40 10  	beqz	$2, 0x1af910 <.text+0xaf910>
  1af89c: 00 00 00 00  	nop
  1af8a0: 03 00 86 91  	lbu	$6, 0x3($12)
  1af8a4: 1a 00 c0 10  	beqz	$6, 0x1af910 <.text+0xaf910>
  1af8a8: 00 00 00 00  	nop
  1af8ac: 03 00 65 91  	lbu	$5, 0x3($11)
  1af8b0: 2c 00 a0 50  	beqzl	$5, 0x1af964 <.text+0xaf964>
  1af8b4: 44 00 e2 8c  	lw	$2, 0x44($7)
  1af8b8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1af8bc: 16 00 a2 10  	beq	$5, $2, 0x1af918 <.text+0xaf918>
  1af8c0: 44 00 e4 8c  	lw	$4, 0x44($7)
  1af8c4: 14 00 e3 8c  	lw	$3, 0x14($7)
  1af8c8: 40 10 06 00  	sll	$2, $6, 0x1
  1af8cc: 40 18 03 00  	sll	$3, $3, 0x1
  1af8d0: 20 00 e5 8c  	lw	$5, 0x20($7)
  1af8d4: 21 10 44 00  	addu	$2, $2, $4
  1af8d8: 21 18 69 00  	addu	$3, $3, $9
  1af8dc: 00 00 42 94  	lhu	$2, 0x0($2)
  1af8e0: 06 00 63 94  	lhu	$3, 0x6($3)
  1af8e4: 20 84 42 34  	ori	$2, $2, 0x8420
  1af8e8: de fb 63 30  	andi	$3, $3, 0xfbde
  1af8ec: 23 10 43 00  	subu	$2, $2, $3
  1af8f0: 43 10 02 00  	sra	$2, $2, 0x1
  1af8f4: 40 10 02 00  	sll	$2, $2, 0x1
  1af8f8: 21 10 45 00  	addu	$2, $2, $5
  1af8fc: 00 00 42 94  	lhu	$2, 0x0($2)
  1af900: 06 00 22 a5  	sh	$2, 0x6($9)
  1af904: 36 00 02 3c  	lui	$2, 0x36
  1af908: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1af90c: 03 00 42 a1  	sb	$2, 0x3($10)
  1af910: 08 00 e0 03  	jr	$ra
  1af914: 00 00 00 00  	nop
  1af918: 40 10 06 00  	sll	$2, $6, 0x1
  1af91c: 50 00 e3 8c  	lw	$3, 0x50($7)
  1af920: 21 10 44 00  	addu	$2, $2, $4
  1af924: 1c 00 e6 8c  	lw	$6, 0x1c($7)
  1af928: 00 00 44 94  	lhu	$4, 0x0($2)
  1af92c: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af930: 24 18 62 00  	and	$3, $3, $2
  1af934: 50 00 e5 94  	lhu	$5, 0x50($7)
  1af938: 20 84 82 34  	ori	$2, $4, 0x8420
  1af93c: 21 04 84 30  	andi	$4, $4, 0x421
  1af940: 23 10 43 00  	subu	$2, $2, $3
  1af944: 21 04 a5 30  	andi	$5, $5, 0x421
  1af948: 42 10 02 00  	srl	$2, $2, 0x1
  1af94c: 40 10 02 00  	sll	$2, $2, 0x1
  1af950: 21 10 46 00  	addu	$2, $2, $6
  1af954: 00 00 42 94  	lhu	$2, 0x0($2)
  1af958: 21 10 44 00  	addu	$2, $2, $4
  1af95c: e8 ff 00 10  	b	0x1af900 <.text+0xaf900>
  1af960: 23 10 45 00  	subu	$2, $2, $5
  1af964: 40 18 06 00  	sll	$3, $6, 0x1
  1af968: 21 18 62 00  	addu	$3, $3, $2
  1af96c: e4 ff 00 10  	b	0x1af900 <.text+0xaf900>
  1af970: 00 00 62 94  	lhu	$2, 0x0($3)
  1af974: 40 10 06 00  	sll	$2, $6, 0x1
  1af978: 50 00 a3 8c  	lw	$3, 0x50($5)
  1af97c: 21 10 44 00  	addu	$2, $2, $4
  1af980: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af984: 00 00 44 94  	lhu	$4, 0x0($2)
  1af988: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af98c: 24 18 62 00  	and	$3, $3, $2
  1af990: 50 00 a5 94  	lhu	$5, 0x50($5)
  1af994: 20 84 82 34  	ori	$2, $4, 0x8420
  1af998: 21 04 84 30  	andi	$4, $4, 0x421
  1af99c: 23 10 43 00  	subu	$2, $2, $3
  1af9a0: 21 04 a5 30  	andi	$5, $5, 0x421
  1af9a4: 42 10 02 00  	srl	$2, $2, 0x1
  1af9a8: 40 10 02 00  	sll	$2, $2, 0x1
  1af9ac: 21 10 46 00  	addu	$2, $2, $6
  1af9b0: 00 00 42 94  	lhu	$2, 0x0($2)
  1af9b4: 21 10 44 00  	addu	$2, $2, $4
  1af9b8: ae ff 00 10  	b	0x1af874 <.text+0xaf874>
  1af9bc: 23 10 45 00  	subu	$2, $2, $5
  1af9c0: 40 18 06 00  	sll	$3, $6, 0x1
  1af9c4: 21 18 62 00  	addu	$3, $3, $2
  1af9c8: aa ff 00 10  	b	0x1af874 <.text+0xaf874>
  1af9cc: 00 00 62 94  	lhu	$2, 0x0($3)
  1af9d0: 40 10 06 00  	sll	$2, $6, 0x1
  1af9d4: 50 00 a3 8c  	lw	$3, 0x50($5)
  1af9d8: 21 10 44 00  	addu	$2, $2, $4
  1af9dc: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1af9e0: 00 00 44 94  	lhu	$4, 0x0($2)
  1af9e4: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1af9e8: 24 18 62 00  	and	$3, $3, $2
  1af9ec: 50 00 a5 94  	lhu	$5, 0x50($5)
  1af9f0: 20 84 82 34  	ori	$2, $4, 0x8420
  1af9f4: 21 04 84 30  	andi	$4, $4, 0x421
  1af9f8: 23 10 43 00  	subu	$2, $2, $3
  1af9fc: 21 04 a5 30  	andi	$5, $5, 0x421
  1afa00: 42 10 02 00  	srl	$2, $2, 0x1
  1afa04: 40 10 02 00  	sll	$2, $2, 0x1
  1afa08: 21 10 46 00  	addu	$2, $2, $6
  1afa0c: 00 00 42 94  	lhu	$2, 0x0($2)
  1afa10: 21 10 44 00  	addu	$2, $2, $4
  1afa14: 74 ff 00 10  	b	0x1af7e8 <.text+0xaf7e8>
  1afa18: 23 10 45 00  	subu	$2, $2, $5
  1afa1c: 40 18 06 00  	sll	$3, $6, 0x1
  1afa20: 21 18 62 00  	addu	$3, $3, $2
  1afa24: 70 ff 00 10  	b	0x1af7e8 <.text+0xaf7e8>
  1afa28: 00 00 62 94  	lhu	$2, 0x0($3)
  1afa2c: 40 10 06 00  	sll	$2, $6, 0x1
  1afa30: 50 00 03 8d  	lw	$3, 0x50($8)
  1afa34: 21 10 44 00  	addu	$2, $2, $4
  1afa38: 1c 00 06 8d  	lw	$6, 0x1c($8)
  1afa3c: 00 00 44 94  	lhu	$4, 0x0($2)
  1afa40: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1afa44: 24 18 62 00  	and	$3, $3, $2
  1afa48: 50 00 05 95  	lhu	$5, 0x50($8)
  1afa4c: 20 84 82 34  	ori	$2, $4, 0x8420
  1afa50: 21 04 84 30  	andi	$4, $4, 0x421
  1afa54: 23 10 43 00  	subu	$2, $2, $3
  1afa58: 21 04 a5 30  	andi	$5, $5, 0x421
  1afa5c: 42 10 02 00  	srl	$2, $2, 0x1
  1afa60: 40 10 02 00  	sll	$2, $2, 0x1
  1afa64: 21 10 46 00  	addu	$2, $2, $6
  1afa68: 00 00 42 94  	lhu	$2, 0x0($2)
  1afa6c: 21 10 44 00  	addu	$2, $2, $4
  1afa70: 3a ff 00 10  	b	0x1af75c <.text+0xaf75c>
  1afa74: 23 10 45 00  	subu	$2, $2, $5
  1afa78: 40 18 06 00  	sll	$3, $6, 0x1
  1afa7c: 21 18 62 00  	addu	$3, $3, $2
  1afa80: 36 ff 00 10  	b	0x1af75c <.text+0xaf75c>
  1afa84: 00 00 62 94  	lhu	$2, 0x0($3)
  1afa88: 36 00 02 3c  	lui	$2, 0x36
  1afa8c: 2d 60 a0 00  	move	$12, $5
  1afa90: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1afa94: 40 38 04 00  	sll	$7, $4, 0x1
  1afa98: 08 00 02 8d  	lw	$2, 0x8($8)
  1afa9c: 4c 00 06 91  	lbu	$6, 0x4c($8)
  1afaa0: 21 50 44 00  	addu	$10, $2, $4
  1afaa4: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1afaa8: 00 00 42 91  	lbu	$2, 0x0($10)
  1afaac: 0c 00 05 8d  	lw	$5, 0xc($8)
  1afab0: 21 48 67 00  	addu	$9, $3, $7
  1afab4: 2b 10 46 00  	sltu	$2, $2, $6
  1afab8: 1d 00 40 10  	beqz	$2, 0x1afb30 <.text+0xafb30>
  1afabc: 21 58 a4 00  	addu	$11, $5, $4
  1afac0: 03 00 86 91  	lbu	$6, 0x3($12)
  1afac4: 1b 00 c0 10  	beqz	$6, 0x1afb34 <.text+0xafb34>
  1afac8: 36 00 03 3c  	lui	$3, 0x36
  1afacc: 00 00 63 91  	lbu	$3, 0x0($11)
  1afad0: da 00 60 50  	beqzl	$3, 0x1afe3c <.text+0xafe3c>
  1afad4: 44 00 02 8d  	lw	$2, 0x44($8)
  1afad8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1afadc: c4 00 62 10  	beq	$3, $2, 0x1afdf0 <.text+0xafdf0>
  1afae0: 44 00 04 8d  	lw	$4, 0x44($8)
  1afae4: 14 00 03 8d  	lw	$3, 0x14($8)
  1afae8: 40 10 06 00  	sll	$2, $6, 0x1
  1afaec: 40 18 03 00  	sll	$3, $3, 0x1
  1afaf0: 20 00 05 8d  	lw	$5, 0x20($8)
  1afaf4: 21 10 44 00  	addu	$2, $2, $4
  1afaf8: 21 18 69 00  	addu	$3, $3, $9
  1afafc: 00 00 42 94  	lhu	$2, 0x0($2)
  1afb00: 00 00 63 94  	lhu	$3, 0x0($3)
  1afb04: 20 84 42 34  	ori	$2, $2, 0x8420
  1afb08: de fb 63 30  	andi	$3, $3, 0xfbde
  1afb0c: 23 10 43 00  	subu	$2, $2, $3
  1afb10: 43 10 02 00  	sra	$2, $2, 0x1
  1afb14: 40 10 02 00  	sll	$2, $2, 0x1
  1afb18: 21 10 45 00  	addu	$2, $2, $5
  1afb1c: 00 00 42 94  	lhu	$2, 0x0($2)
  1afb20: 00 00 22 a5  	sh	$2, 0x0($9)
  1afb24: 36 00 02 3c  	lui	$2, 0x36
  1afb28: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1afb2c: 00 00 42 a1  	sb	$2, 0x0($10)
  1afb30: 36 00 03 3c  	lui	$3, 0x36
  1afb34: 01 00 42 91  	lbu	$2, 0x1($10)
  1afb38: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1afb3c: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1afb40: 2b 10 43 00  	sltu	$2, $2, $3
  1afb44: 1e 00 40 10  	beqz	$2, 0x1afbc0 <.text+0xafbc0>
  1afb48: 36 00 03 3c  	lui	$3, 0x36
  1afb4c: 02 00 86 91  	lbu	$6, 0x2($12)
  1afb50: 1c 00 c0 50  	beqzl	$6, 0x1afbc4 <.text+0xafbc4>
  1afb54: 02 00 42 91  	lbu	$2, 0x2($10)
  1afb58: 01 00 63 91  	lbu	$3, 0x1($11)
  1afb5c: a0 00 60 50  	beqzl	$3, 0x1afde0 <.text+0xafde0>
  1afb60: 44 00 a2 8c  	lw	$2, 0x44($5)
  1afb64: 01 00 02 24  	addiu	$2, $zero, 0x1
  1afb68: 8a 00 62 10  	beq	$3, $2, 0x1afd94 <.text+0xafd94>
  1afb6c: 44 00 a4 8c  	lw	$4, 0x44($5)
  1afb70: 14 00 a3 8c  	lw	$3, 0x14($5)
  1afb74: 40 10 06 00  	sll	$2, $6, 0x1
  1afb78: 40 18 03 00  	sll	$3, $3, 0x1
  1afb7c: 20 00 a5 8c  	lw	$5, 0x20($5)
  1afb80: 21 10 44 00  	addu	$2, $2, $4
  1afb84: 21 18 69 00  	addu	$3, $3, $9
  1afb88: 00 00 42 94  	lhu	$2, 0x0($2)
  1afb8c: 02 00 63 94  	lhu	$3, 0x2($3)
  1afb90: 20 84 42 34  	ori	$2, $2, 0x8420
  1afb94: de fb 63 30  	andi	$3, $3, 0xfbde
  1afb98: 23 10 43 00  	subu	$2, $2, $3
  1afb9c: 43 10 02 00  	sra	$2, $2, 0x1
  1afba0: 40 10 02 00  	sll	$2, $2, 0x1
  1afba4: 21 10 45 00  	addu	$2, $2, $5
  1afba8: 00 00 42 94  	lhu	$2, 0x0($2)
  1afbac: 02 00 22 a5  	sh	$2, 0x2($9)
  1afbb0: 36 00 02 3c  	lui	$2, 0x36
  1afbb4: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1afbb8: 01 00 42 a1  	sb	$2, 0x1($10)
  1afbbc: 36 00 03 3c  	lui	$3, 0x36
  1afbc0: 02 00 42 91  	lbu	$2, 0x2($10)
  1afbc4: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1afbc8: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1afbcc: 2b 10 43 00  	sltu	$2, $2, $3
  1afbd0: 1e 00 40 10  	beqz	$2, 0x1afc4c <.text+0xafc4c>
  1afbd4: 36 00 03 3c  	lui	$3, 0x36
  1afbd8: 01 00 86 91  	lbu	$6, 0x1($12)
  1afbdc: 1c 00 c0 50  	beqzl	$6, 0x1afc50 <.text+0xafc50>
  1afbe0: 03 00 42 91  	lbu	$2, 0x3($10)
  1afbe4: 02 00 63 91  	lbu	$3, 0x2($11)
  1afbe8: 66 00 60 50  	beqzl	$3, 0x1afd84 <.text+0xafd84>
  1afbec: 44 00 a2 8c  	lw	$2, 0x44($5)
  1afbf0: 01 00 02 24  	addiu	$2, $zero, 0x1
  1afbf4: 50 00 62 10  	beq	$3, $2, 0x1afd38 <.text+0xafd38>
  1afbf8: 44 00 a4 8c  	lw	$4, 0x44($5)
  1afbfc: 14 00 a3 8c  	lw	$3, 0x14($5)
  1afc00: 40 10 06 00  	sll	$2, $6, 0x1
  1afc04: 40 18 03 00  	sll	$3, $3, 0x1
  1afc08: 20 00 a5 8c  	lw	$5, 0x20($5)
  1afc0c: 21 10 44 00  	addu	$2, $2, $4
  1afc10: 21 18 69 00  	addu	$3, $3, $9
  1afc14: 00 00 42 94  	lhu	$2, 0x0($2)
  1afc18: 04 00 63 94  	lhu	$3, 0x4($3)
  1afc1c: 20 84 42 34  	ori	$2, $2, 0x8420
  1afc20: de fb 63 30  	andi	$3, $3, 0xfbde
  1afc24: 23 10 43 00  	subu	$2, $2, $3
  1afc28: 43 10 02 00  	sra	$2, $2, 0x1
  1afc2c: 40 10 02 00  	sll	$2, $2, 0x1
  1afc30: 21 10 45 00  	addu	$2, $2, $5
  1afc34: 00 00 42 94  	lhu	$2, 0x0($2)
  1afc38: 04 00 22 a5  	sh	$2, 0x4($9)
  1afc3c: 36 00 02 3c  	lui	$2, 0x36
  1afc40: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1afc44: 02 00 42 a1  	sb	$2, 0x2($10)
  1afc48: 36 00 03 3c  	lui	$3, 0x36
  1afc4c: 03 00 42 91  	lbu	$2, 0x3($10)
  1afc50: 80 d4 67 24  	addiu	$7, $3, -0x2b80 <.text+0xffffffffffefd480>
  1afc54: 4c 00 e3 90  	lbu	$3, 0x4c($7)
  1afc58: 2b 10 43 00  	sltu	$2, $2, $3
  1afc5c: 1d 00 40 10  	beqz	$2, 0x1afcd4 <.text+0xafcd4>
  1afc60: 00 00 00 00  	nop
  1afc64: 00 00 86 91  	lbu	$6, 0x0($12)
  1afc68: 1a 00 c0 10  	beqz	$6, 0x1afcd4 <.text+0xafcd4>
  1afc6c: 00 00 00 00  	nop
  1afc70: 03 00 65 91  	lbu	$5, 0x3($11)
  1afc74: 2c 00 a0 50  	beqzl	$5, 0x1afd28 <.text+0xafd28>
  1afc78: 44 00 e2 8c  	lw	$2, 0x44($7)
  1afc7c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1afc80: 16 00 a2 10  	beq	$5, $2, 0x1afcdc <.text+0xafcdc>
  1afc84: 44 00 e4 8c  	lw	$4, 0x44($7)
  1afc88: 14 00 e3 8c  	lw	$3, 0x14($7)
  1afc8c: 40 10 06 00  	sll	$2, $6, 0x1
  1afc90: 40 18 03 00  	sll	$3, $3, 0x1
  1afc94: 20 00 e5 8c  	lw	$5, 0x20($7)
  1afc98: 21 10 44 00  	addu	$2, $2, $4
  1afc9c: 21 18 69 00  	addu	$3, $3, $9
  1afca0: 00 00 42 94  	lhu	$2, 0x0($2)
  1afca4: 06 00 63 94  	lhu	$3, 0x6($3)
  1afca8: 20 84 42 34  	ori	$2, $2, 0x8420
  1afcac: de fb 63 30  	andi	$3, $3, 0xfbde
  1afcb0: 23 10 43 00  	subu	$2, $2, $3
  1afcb4: 43 10 02 00  	sra	$2, $2, 0x1
  1afcb8: 40 10 02 00  	sll	$2, $2, 0x1
  1afcbc: 21 10 45 00  	addu	$2, $2, $5
  1afcc0: 00 00 42 94  	lhu	$2, 0x0($2)
  1afcc4: 06 00 22 a5  	sh	$2, 0x6($9)
  1afcc8: 36 00 02 3c  	lui	$2, 0x36
  1afccc: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1afcd0: 03 00 42 a1  	sb	$2, 0x3($10)
  1afcd4: 08 00 e0 03  	jr	$ra
  1afcd8: 00 00 00 00  	nop
  1afcdc: 40 10 06 00  	sll	$2, $6, 0x1
  1afce0: 50 00 e3 8c  	lw	$3, 0x50($7)
  1afce4: 21 10 44 00  	addu	$2, $2, $4
  1afce8: 1c 00 e6 8c  	lw	$6, 0x1c($7)
  1afcec: 00 00 44 94  	lhu	$4, 0x0($2)
  1afcf0: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1afcf4: 24 18 62 00  	and	$3, $3, $2
  1afcf8: 50 00 e5 94  	lhu	$5, 0x50($7)
  1afcfc: 20 84 82 34  	ori	$2, $4, 0x8420
  1afd00: 21 04 84 30  	andi	$4, $4, 0x421
  1afd04: 23 10 43 00  	subu	$2, $2, $3
  1afd08: 21 04 a5 30  	andi	$5, $5, 0x421
  1afd0c: 42 10 02 00  	srl	$2, $2, 0x1
  1afd10: 40 10 02 00  	sll	$2, $2, 0x1
  1afd14: 21 10 46 00  	addu	$2, $2, $6
  1afd18: 00 00 42 94  	lhu	$2, 0x0($2)
  1afd1c: 21 10 44 00  	addu	$2, $2, $4
  1afd20: e8 ff 00 10  	b	0x1afcc4 <.text+0xafcc4>
  1afd24: 23 10 45 00  	subu	$2, $2, $5
  1afd28: 40 18 06 00  	sll	$3, $6, 0x1
  1afd2c: 21 18 62 00  	addu	$3, $3, $2
  1afd30: e4 ff 00 10  	b	0x1afcc4 <.text+0xafcc4>
  1afd34: 00 00 62 94  	lhu	$2, 0x0($3)
  1afd38: 40 10 06 00  	sll	$2, $6, 0x1
  1afd3c: 50 00 a3 8c  	lw	$3, 0x50($5)
  1afd40: 21 10 44 00  	addu	$2, $2, $4
  1afd44: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1afd48: 00 00 44 94  	lhu	$4, 0x0($2)
  1afd4c: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1afd50: 24 18 62 00  	and	$3, $3, $2
  1afd54: 50 00 a5 94  	lhu	$5, 0x50($5)
  1afd58: 20 84 82 34  	ori	$2, $4, 0x8420
  1afd5c: 21 04 84 30  	andi	$4, $4, 0x421
  1afd60: 23 10 43 00  	subu	$2, $2, $3
  1afd64: 21 04 a5 30  	andi	$5, $5, 0x421
  1afd68: 42 10 02 00  	srl	$2, $2, 0x1
  1afd6c: 40 10 02 00  	sll	$2, $2, 0x1
  1afd70: 21 10 46 00  	addu	$2, $2, $6
  1afd74: 00 00 42 94  	lhu	$2, 0x0($2)
  1afd78: 21 10 44 00  	addu	$2, $2, $4
  1afd7c: ae ff 00 10  	b	0x1afc38 <.text+0xafc38>
  1afd80: 23 10 45 00  	subu	$2, $2, $5
  1afd84: 40 18 06 00  	sll	$3, $6, 0x1
  1afd88: 21 18 62 00  	addu	$3, $3, $2
  1afd8c: aa ff 00 10  	b	0x1afc38 <.text+0xafc38>
  1afd90: 00 00 62 94  	lhu	$2, 0x0($3)
  1afd94: 40 10 06 00  	sll	$2, $6, 0x1
  1afd98: 50 00 a3 8c  	lw	$3, 0x50($5)
  1afd9c: 21 10 44 00  	addu	$2, $2, $4
  1afda0: 1c 00 a6 8c  	lw	$6, 0x1c($5)
  1afda4: 00 00 44 94  	lhu	$4, 0x0($2)
  1afda8: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1afdac: 24 18 62 00  	and	$3, $3, $2
  1afdb0: 50 00 a5 94  	lhu	$5, 0x50($5)
  1afdb4: 20 84 82 34  	ori	$2, $4, 0x8420
  1afdb8: 21 04 84 30  	andi	$4, $4, 0x421
  1afdbc: 23 10 43 00  	subu	$2, $2, $3
  1afdc0: 21 04 a5 30  	andi	$5, $5, 0x421
  1afdc4: 42 10 02 00  	srl	$2, $2, 0x1
  1afdc8: 40 10 02 00  	sll	$2, $2, 0x1
  1afdcc: 21 10 46 00  	addu	$2, $2, $6
  1afdd0: 00 00 42 94  	lhu	$2, 0x0($2)
  1afdd4: 21 10 44 00  	addu	$2, $2, $4
  1afdd8: 74 ff 00 10  	b	0x1afbac <.text+0xafbac>
  1afddc: 23 10 45 00  	subu	$2, $2, $5
  1afde0: 40 18 06 00  	sll	$3, $6, 0x1
  1afde4: 21 18 62 00  	addu	$3, $3, $2
  1afde8: 70 ff 00 10  	b	0x1afbac <.text+0xafbac>
  1afdec: 00 00 62 94  	lhu	$2, 0x0($3)
  1afdf0: 40 10 06 00  	sll	$2, $6, 0x1
  1afdf4: 50 00 03 8d  	lw	$3, 0x50($8)
  1afdf8: 21 10 44 00  	addu	$2, $2, $4
  1afdfc: 1c 00 06 8d  	lw	$6, 0x1c($8)
  1afe00: 00 00 44 94  	lhu	$4, 0x0($2)
  1afe04: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1afe08: 24 18 62 00  	and	$3, $3, $2
  1afe0c: 50 00 05 95  	lhu	$5, 0x50($8)
  1afe10: 20 84 82 34  	ori	$2, $4, 0x8420
  1afe14: 21 04 84 30  	andi	$4, $4, 0x421
  1afe18: 23 10 43 00  	subu	$2, $2, $3
  1afe1c: 21 04 a5 30  	andi	$5, $5, 0x421
  1afe20: 42 10 02 00  	srl	$2, $2, 0x1
  1afe24: 40 10 02 00  	sll	$2, $2, 0x1
  1afe28: 21 10 46 00  	addu	$2, $2, $6
  1afe2c: 00 00 42 94  	lhu	$2, 0x0($2)
  1afe30: 21 10 44 00  	addu	$2, $2, $4
  1afe34: 3a ff 00 10  	b	0x1afb20 <.text+0xafb20>
  1afe38: 23 10 45 00  	subu	$2, $2, $5
  1afe3c: 40 18 06 00  	sll	$3, $6, 0x1
  1afe40: 21 18 62 00  	addu	$3, $3, $2
  1afe44: 36 ff 00 10  	b	0x1afb20 <.text+0xafb20>
  1afe48: 00 00 62 94  	lhu	$2, 0x0($3)
  1afe4c: 36 00 02 3c  	lui	$2, 0x36
  1afe50: 2d 58 a0 00  	move	$11, $5
  1afe54: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1afe58: 40 38 04 00  	sll	$7, $4, 0x1
  1afe5c: 08 00 02 8d  	lw	$2, 0x8($8)
  1afe60: 4c 00 06 91  	lbu	$6, 0x4c($8)
  1afe64: 21 48 44 00  	addu	$9, $2, $4
  1afe68: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1afe6c: 00 00 22 91  	lbu	$2, 0x0($9)
  1afe70: 0c 00 05 8d  	lw	$5, 0xc($8)
  1afe74: 21 38 67 00  	addu	$7, $3, $7
  1afe78: 2b 10 46 00  	sltu	$2, $2, $6
  1afe7c: 0f 00 40 10  	beqz	$2, 0x1afebc <.text+0xafebc>
  1afe80: 21 50 a4 00  	addu	$10, $5, $4
  1afe84: 00 00 66 91  	lbu	$6, 0x0($11)
  1afe88: 0d 00 c0 10  	beqz	$6, 0x1afec0 <.text+0xafec0>
  1afe8c: 36 00 03 3c  	lui	$3, 0x36
  1afe90: 00 00 43 91  	lbu	$3, 0x0($10)
  1afe94: 01 00 02 24  	addiu	$2, $zero, 0x1
  1afe98: 6f 00 62 10  	beq	$3, $2, 0x1b0058 <.text+0xb0058>
  1afe9c: 40 18 06 00  	sll	$3, $6, 0x1
  1afea0: 44 00 02 8d  	lw	$2, 0x44($8)
  1afea4: 21 18 62 00  	addu	$3, $3, $2
  1afea8: 00 00 62 94  	lhu	$2, 0x0($3)
  1afeac: 00 00 e2 a4  	sh	$2, 0x0($7)
  1afeb0: 36 00 02 3c  	lui	$2, 0x36
  1afeb4: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1afeb8: 00 00 22 a1  	sb	$2, 0x0($9)
  1afebc: 36 00 03 3c  	lui	$3, 0x36
  1afec0: 01 00 22 91  	lbu	$2, 0x1($9)
  1afec4: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1afec8: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1afecc: 2b 10 43 00  	sltu	$2, $2, $3
  1afed0: 10 00 40 10  	beqz	$2, 0x1aff14 <.text+0xaff14>
  1afed4: 36 00 03 3c  	lui	$3, 0x36
  1afed8: 01 00 66 91  	lbu	$6, 0x1($11)
  1afedc: 0e 00 c0 50  	beqzl	$6, 0x1aff18 <.text+0xaff18>
  1afee0: 02 00 22 91  	lbu	$2, 0x2($9)
  1afee4: 01 00 43 91  	lbu	$3, 0x1($10)
  1afee8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1afeec: 4d 00 62 10  	beq	$3, $2, 0x1b0024 <.text+0xb0024>
  1afef0: 40 18 06 00  	sll	$3, $6, 0x1
  1afef4: 44 00 a2 8c  	lw	$2, 0x44($5)
  1afef8: 21 18 62 00  	addu	$3, $3, $2
  1afefc: 00 00 62 94  	lhu	$2, 0x0($3)
  1aff00: 02 00 e2 a4  	sh	$2, 0x2($7)
  1aff04: 36 00 02 3c  	lui	$2, 0x36
  1aff08: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1aff0c: 01 00 22 a1  	sb	$2, 0x1($9)
  1aff10: 36 00 03 3c  	lui	$3, 0x36
  1aff14: 02 00 22 91  	lbu	$2, 0x2($9)
  1aff18: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1aff1c: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1aff20: 2b 10 43 00  	sltu	$2, $2, $3
  1aff24: 10 00 40 10  	beqz	$2, 0x1aff68 <.text+0xaff68>
  1aff28: 36 00 03 3c  	lui	$3, 0x36
  1aff2c: 02 00 66 91  	lbu	$6, 0x2($11)
  1aff30: 0e 00 c0 50  	beqzl	$6, 0x1aff6c <.text+0xaff6c>
  1aff34: 03 00 22 91  	lbu	$2, 0x3($9)
  1aff38: 02 00 43 91  	lbu	$3, 0x2($10)
  1aff3c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aff40: 2b 00 62 10  	beq	$3, $2, 0x1afff0 <.text+0xafff0>
  1aff44: 40 18 06 00  	sll	$3, $6, 0x1
  1aff48: 44 00 a2 8c  	lw	$2, 0x44($5)
  1aff4c: 21 18 62 00  	addu	$3, $3, $2
  1aff50: 00 00 62 94  	lhu	$2, 0x0($3)
  1aff54: 04 00 e2 a4  	sh	$2, 0x4($7)
  1aff58: 36 00 02 3c  	lui	$2, 0x36
  1aff5c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1aff60: 02 00 22 a1  	sb	$2, 0x2($9)
  1aff64: 36 00 03 3c  	lui	$3, 0x36
  1aff68: 03 00 22 91  	lbu	$2, 0x3($9)
  1aff6c: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1aff70: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1aff74: 2b 10 43 00  	sltu	$2, $2, $3
  1aff78: 0e 00 40 10  	beqz	$2, 0x1affb4 <.text+0xaffb4>
  1aff7c: 00 00 00 00  	nop
  1aff80: 03 00 66 91  	lbu	$6, 0x3($11)
  1aff84: 0b 00 c0 10  	beqz	$6, 0x1affb4 <.text+0xaffb4>
  1aff88: 01 00 02 24  	addiu	$2, $zero, 0x1
  1aff8c: 03 00 43 91  	lbu	$3, 0x3($10)
  1aff90: 0a 00 62 10  	beq	$3, $2, 0x1affbc <.text+0xaffbc>
  1aff94: 40 18 06 00  	sll	$3, $6, 0x1
  1aff98: 44 00 a2 8c  	lw	$2, 0x44($5)
  1aff9c: 21 18 62 00  	addu	$3, $3, $2
  1affa0: 00 00 62 94  	lhu	$2, 0x0($3)
  1affa4: 06 00 e2 a4  	sh	$2, 0x6($7)
  1affa8: 36 00 02 3c  	lui	$2, 0x36
  1affac: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1affb0: 03 00 22 a1  	sb	$2, 0x3($9)
  1affb4: 08 00 e0 03  	jr	$ra
  1affb8: 00 00 00 00  	nop
  1affbc: 44 00 a4 8c  	lw	$4, 0x44($5)
  1affc0: 50 00 a5 8c  	lw	$5, 0x50($5)
  1affc4: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1affc8: 21 18 64 00  	addu	$3, $3, $4
  1affcc: 00 00 63 94  	lhu	$3, 0x0($3)
  1affd0: 24 20 a2 00  	and	$4, $5, $2
  1affd4: 24 10 62 00  	and	$2, $3, $2
  1affd8: 24 18 65 00  	and	$3, $3, $5
  1affdc: 21 10 44 00  	addu	$2, $2, $4
  1affe0: 21 04 63 30  	andi	$3, $3, 0x421
  1affe4: 42 10 02 00  	srl	$2, $2, 0x1
  1affe8: ee ff 00 10  	b	0x1affa4 <.text+0xaffa4>
  1affec: 21 10 43 00  	addu	$2, $2, $3
  1afff0: 44 00 a4 8c  	lw	$4, 0x44($5)
  1afff4: 50 00 a5 8c  	lw	$5, 0x50($5)
  1afff8: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1afffc: 21 18 64 00  	addu	$3, $3, $4
  1b0000: 00 00 63 94  	lhu	$3, 0x0($3)
  1b0004: 24 20 a2 00  	and	$4, $5, $2
  1b0008: 24 10 62 00  	and	$2, $3, $2
  1b000c: 24 18 65 00  	and	$3, $3, $5
  1b0010: 21 10 44 00  	addu	$2, $2, $4
  1b0014: 21 04 63 30  	andi	$3, $3, 0x421
  1b0018: 42 10 02 00  	srl	$2, $2, 0x1
  1b001c: cd ff 00 10  	b	0x1aff54 <.text+0xaff54>
  1b0020: 21 10 43 00  	addu	$2, $2, $3
  1b0024: 44 00 a4 8c  	lw	$4, 0x44($5)
  1b0028: 50 00 a5 8c  	lw	$5, 0x50($5)
  1b002c: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b0030: 21 18 64 00  	addu	$3, $3, $4
  1b0034: 00 00 63 94  	lhu	$3, 0x0($3)
  1b0038: 24 20 a2 00  	and	$4, $5, $2
  1b003c: 24 10 62 00  	and	$2, $3, $2
  1b0040: 24 18 65 00  	and	$3, $3, $5
  1b0044: 21 10 44 00  	addu	$2, $2, $4
  1b0048: 21 04 63 30  	andi	$3, $3, 0x421
  1b004c: 42 10 02 00  	srl	$2, $2, 0x1
  1b0050: ab ff 00 10  	b	0x1aff00 <.text+0xaff00>
  1b0054: 21 10 43 00  	addu	$2, $2, $3
  1b0058: 44 00 04 8d  	lw	$4, 0x44($8)
  1b005c: 50 00 05 8d  	lw	$5, 0x50($8)
  1b0060: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b0064: 21 18 64 00  	addu	$3, $3, $4
  1b0068: 00 00 63 94  	lhu	$3, 0x0($3)
  1b006c: 24 20 a2 00  	and	$4, $5, $2
  1b0070: 24 10 62 00  	and	$2, $3, $2
  1b0074: 24 18 65 00  	and	$3, $3, $5
  1b0078: 21 10 44 00  	addu	$2, $2, $4
  1b007c: 21 04 63 30  	andi	$3, $3, 0x421
  1b0080: 42 10 02 00  	srl	$2, $2, 0x1
  1b0084: 89 ff 00 10  	b	0x1afeac <.text+0xafeac>
  1b0088: 21 10 43 00  	addu	$2, $2, $3
  1b008c: 36 00 02 3c  	lui	$2, 0x36
  1b0090: 2d 58 a0 00  	move	$11, $5
  1b0094: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1b0098: 40 38 04 00  	sll	$7, $4, 0x1
  1b009c: 08 00 02 8d  	lw	$2, 0x8($8)
  1b00a0: 4c 00 06 91  	lbu	$6, 0x4c($8)
  1b00a4: 21 48 44 00  	addu	$9, $2, $4
  1b00a8: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1b00ac: 00 00 22 91  	lbu	$2, 0x0($9)
  1b00b0: 0c 00 05 8d  	lw	$5, 0xc($8)
  1b00b4: 21 38 67 00  	addu	$7, $3, $7
  1b00b8: 2b 10 46 00  	sltu	$2, $2, $6
  1b00bc: 0f 00 40 10  	beqz	$2, 0x1b00fc <.text+0xb00fc>
  1b00c0: 21 50 a4 00  	addu	$10, $5, $4
  1b00c4: 03 00 66 91  	lbu	$6, 0x3($11)
  1b00c8: 0d 00 c0 10  	beqz	$6, 0x1b0100 <.text+0xb0100>
  1b00cc: 36 00 03 3c  	lui	$3, 0x36
  1b00d0: 00 00 43 91  	lbu	$3, 0x0($10)
  1b00d4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b00d8: 6f 00 62 10  	beq	$3, $2, 0x1b0298 <.text+0xb0298>
  1b00dc: 40 18 06 00  	sll	$3, $6, 0x1
  1b00e0: 44 00 02 8d  	lw	$2, 0x44($8)
  1b00e4: 21 18 62 00  	addu	$3, $3, $2
  1b00e8: 00 00 62 94  	lhu	$2, 0x0($3)
  1b00ec: 00 00 e2 a4  	sh	$2, 0x0($7)
  1b00f0: 36 00 02 3c  	lui	$2, 0x36
  1b00f4: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b00f8: 00 00 22 a1  	sb	$2, 0x0($9)
  1b00fc: 36 00 03 3c  	lui	$3, 0x36
  1b0100: 01 00 22 91  	lbu	$2, 0x1($9)
  1b0104: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b0108: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b010c: 2b 10 43 00  	sltu	$2, $2, $3
  1b0110: 10 00 40 10  	beqz	$2, 0x1b0154 <.text+0xb0154>
  1b0114: 36 00 03 3c  	lui	$3, 0x36
  1b0118: 02 00 66 91  	lbu	$6, 0x2($11)
  1b011c: 0e 00 c0 50  	beqzl	$6, 0x1b0158 <.text+0xb0158>
  1b0120: 02 00 22 91  	lbu	$2, 0x2($9)
  1b0124: 01 00 43 91  	lbu	$3, 0x1($10)
  1b0128: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b012c: 4d 00 62 10  	beq	$3, $2, 0x1b0264 <.text+0xb0264>
  1b0130: 40 18 06 00  	sll	$3, $6, 0x1
  1b0134: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b0138: 21 18 62 00  	addu	$3, $3, $2
  1b013c: 00 00 62 94  	lhu	$2, 0x0($3)
  1b0140: 02 00 e2 a4  	sh	$2, 0x2($7)
  1b0144: 36 00 02 3c  	lui	$2, 0x36
  1b0148: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b014c: 01 00 22 a1  	sb	$2, 0x1($9)
  1b0150: 36 00 03 3c  	lui	$3, 0x36
  1b0154: 02 00 22 91  	lbu	$2, 0x2($9)
  1b0158: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b015c: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b0160: 2b 10 43 00  	sltu	$2, $2, $3
  1b0164: 10 00 40 10  	beqz	$2, 0x1b01a8 <.text+0xb01a8>
  1b0168: 36 00 03 3c  	lui	$3, 0x36
  1b016c: 01 00 66 91  	lbu	$6, 0x1($11)
  1b0170: 0e 00 c0 50  	beqzl	$6, 0x1b01ac <.text+0xb01ac>
  1b0174: 03 00 22 91  	lbu	$2, 0x3($9)
  1b0178: 02 00 43 91  	lbu	$3, 0x2($10)
  1b017c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b0180: 2b 00 62 10  	beq	$3, $2, 0x1b0230 <.text+0xb0230>
  1b0184: 40 18 06 00  	sll	$3, $6, 0x1
  1b0188: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b018c: 21 18 62 00  	addu	$3, $3, $2
  1b0190: 00 00 62 94  	lhu	$2, 0x0($3)
  1b0194: 04 00 e2 a4  	sh	$2, 0x4($7)
  1b0198: 36 00 02 3c  	lui	$2, 0x36
  1b019c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b01a0: 02 00 22 a1  	sb	$2, 0x2($9)
  1b01a4: 36 00 03 3c  	lui	$3, 0x36
  1b01a8: 03 00 22 91  	lbu	$2, 0x3($9)
  1b01ac: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b01b0: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b01b4: 2b 10 43 00  	sltu	$2, $2, $3
  1b01b8: 0e 00 40 10  	beqz	$2, 0x1b01f4 <.text+0xb01f4>
  1b01bc: 00 00 00 00  	nop
  1b01c0: 00 00 66 91  	lbu	$6, 0x0($11)
  1b01c4: 0b 00 c0 10  	beqz	$6, 0x1b01f4 <.text+0xb01f4>
  1b01c8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b01cc: 03 00 43 91  	lbu	$3, 0x3($10)
  1b01d0: 0a 00 62 10  	beq	$3, $2, 0x1b01fc <.text+0xb01fc>
  1b01d4: 40 18 06 00  	sll	$3, $6, 0x1
  1b01d8: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b01dc: 21 18 62 00  	addu	$3, $3, $2
  1b01e0: 00 00 62 94  	lhu	$2, 0x0($3)
  1b01e4: 06 00 e2 a4  	sh	$2, 0x6($7)
  1b01e8: 36 00 02 3c  	lui	$2, 0x36
  1b01ec: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b01f0: 03 00 22 a1  	sb	$2, 0x3($9)
  1b01f4: 08 00 e0 03  	jr	$ra
  1b01f8: 00 00 00 00  	nop
  1b01fc: 44 00 a4 8c  	lw	$4, 0x44($5)
  1b0200: 50 00 a5 8c  	lw	$5, 0x50($5)
  1b0204: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b0208: 21 18 64 00  	addu	$3, $3, $4
  1b020c: 00 00 63 94  	lhu	$3, 0x0($3)
  1b0210: 24 20 a2 00  	and	$4, $5, $2
  1b0214: 24 10 62 00  	and	$2, $3, $2
  1b0218: 24 18 65 00  	and	$3, $3, $5
  1b021c: 21 10 44 00  	addu	$2, $2, $4
  1b0220: 21 04 63 30  	andi	$3, $3, 0x421
  1b0224: 42 10 02 00  	srl	$2, $2, 0x1
  1b0228: ee ff 00 10  	b	0x1b01e4 <.text+0xb01e4>
  1b022c: 21 10 43 00  	addu	$2, $2, $3
  1b0230: 44 00 a4 8c  	lw	$4, 0x44($5)
  1b0234: 50 00 a5 8c  	lw	$5, 0x50($5)
  1b0238: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b023c: 21 18 64 00  	addu	$3, $3, $4
  1b0240: 00 00 63 94  	lhu	$3, 0x0($3)
  1b0244: 24 20 a2 00  	and	$4, $5, $2
  1b0248: 24 10 62 00  	and	$2, $3, $2
  1b024c: 24 18 65 00  	and	$3, $3, $5
  1b0250: 21 10 44 00  	addu	$2, $2, $4
  1b0254: 21 04 63 30  	andi	$3, $3, 0x421
  1b0258: 42 10 02 00  	srl	$2, $2, 0x1
  1b025c: cd ff 00 10  	b	0x1b0194 <.text+0xb0194>
  1b0260: 21 10 43 00  	addu	$2, $2, $3
  1b0264: 44 00 a4 8c  	lw	$4, 0x44($5)
  1b0268: 50 00 a5 8c  	lw	$5, 0x50($5)
  1b026c: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b0270: 21 18 64 00  	addu	$3, $3, $4
  1b0274: 00 00 63 94  	lhu	$3, 0x0($3)
  1b0278: 24 20 a2 00  	and	$4, $5, $2
  1b027c: 24 10 62 00  	and	$2, $3, $2
  1b0280: 24 18 65 00  	and	$3, $3, $5
  1b0284: 21 10 44 00  	addu	$2, $2, $4
  1b0288: 21 04 63 30  	andi	$3, $3, 0x421
  1b028c: 42 10 02 00  	srl	$2, $2, 0x1
  1b0290: ab ff 00 10  	b	0x1b0140 <.text+0xb0140>
  1b0294: 21 10 43 00  	addu	$2, $2, $3
  1b0298: 44 00 04 8d  	lw	$4, 0x44($8)
  1b029c: 50 00 05 8d  	lw	$5, 0x50($8)
  1b02a0: de fb 02 24  	addiu	$2, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b02a4: 21 18 64 00  	addu	$3, $3, $4
  1b02a8: 00 00 63 94  	lhu	$3, 0x0($3)
  1b02ac: 24 20 a2 00  	and	$4, $5, $2
  1b02b0: 24 10 62 00  	and	$2, $3, $2
  1b02b4: 24 18 65 00  	and	$3, $3, $5
  1b02b8: 21 10 44 00  	addu	$2, $2, $4
  1b02bc: 21 04 63 30  	andi	$3, $3, 0x421
  1b02c0: 42 10 02 00  	srl	$2, $2, 0x1
  1b02c4: 89 ff 00 10  	b	0x1b00ec <.text+0xb00ec>
  1b02c8: 21 10 43 00  	addu	$2, $2, $3
  1b02cc: 36 00 02 3c  	lui	$2, 0x36
  1b02d0: 2d 50 a0 00  	move	$10, $5
  1b02d4: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1b02d8: 40 38 04 00  	sll	$7, $4, 0x1
  1b02dc: 08 00 02 8d  	lw	$2, 0x8($8)
  1b02e0: 4c 00 06 91  	lbu	$6, 0x4c($8)
  1b02e4: 21 48 44 00  	addu	$9, $2, $4
  1b02e8: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1b02ec: 00 00 22 91  	lbu	$2, 0x0($9)
  1b02f0: 0c 00 05 8d  	lw	$5, 0xc($8)
  1b02f4: 21 38 67 00  	addu	$7, $3, $7
  1b02f8: 2b 10 46 00  	sltu	$2, $2, $6
  1b02fc: 0f 00 40 10  	beqz	$2, 0x1b033c <.text+0xb033c>
  1b0300: 21 30 a4 00  	addu	$6, $5, $4
  1b0304: 00 00 44 91  	lbu	$4, 0x0($10)
  1b0308: 0d 00 80 10  	beqz	$4, 0x1b0340 <.text+0xb0340>
  1b030c: 36 00 03 3c  	lui	$3, 0x36
  1b0310: 00 00 c3 90  	lbu	$3, 0x0($6)
  1b0314: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b0318: 75 00 62 10  	beq	$3, $2, 0x1b04f0 <.text+0xb04f0>
  1b031c: 40 18 04 00  	sll	$3, $4, 0x1
  1b0320: 44 00 02 8d  	lw	$2, 0x44($8)
  1b0324: 21 18 62 00  	addu	$3, $3, $2
  1b0328: 00 00 62 94  	lhu	$2, 0x0($3)
  1b032c: 00 00 e2 a4  	sh	$2, 0x0($7)
  1b0330: 36 00 02 3c  	lui	$2, 0x36
  1b0334: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b0338: 00 00 22 a1  	sb	$2, 0x0($9)
  1b033c: 36 00 03 3c  	lui	$3, 0x36
  1b0340: 01 00 22 91  	lbu	$2, 0x1($9)
  1b0344: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b0348: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b034c: 2b 10 43 00  	sltu	$2, $2, $3
  1b0350: 10 00 40 10  	beqz	$2, 0x1b0394 <.text+0xb0394>
  1b0354: 36 00 03 3c  	lui	$3, 0x36
  1b0358: 01 00 44 91  	lbu	$4, 0x1($10)
  1b035c: 0e 00 80 50  	beqzl	$4, 0x1b0398 <.text+0xb0398>
  1b0360: 02 00 22 91  	lbu	$2, 0x2($9)
  1b0364: 01 00 c3 90  	lbu	$3, 0x1($6)
  1b0368: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b036c: 51 00 62 10  	beq	$3, $2, 0x1b04b4 <.text+0xb04b4>
  1b0370: 40 18 04 00  	sll	$3, $4, 0x1
  1b0374: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b0378: 21 18 62 00  	addu	$3, $3, $2
  1b037c: 00 00 62 94  	lhu	$2, 0x0($3)
  1b0380: 02 00 e2 a4  	sh	$2, 0x2($7)
  1b0384: 36 00 02 3c  	lui	$2, 0x36
  1b0388: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b038c: 01 00 22 a1  	sb	$2, 0x1($9)
  1b0390: 36 00 03 3c  	lui	$3, 0x36
  1b0394: 02 00 22 91  	lbu	$2, 0x2($9)
  1b0398: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b039c: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b03a0: 2b 10 43 00  	sltu	$2, $2, $3
  1b03a4: 10 00 40 10  	beqz	$2, 0x1b03e8 <.text+0xb03e8>
  1b03a8: 36 00 03 3c  	lui	$3, 0x36
  1b03ac: 02 00 44 91  	lbu	$4, 0x2($10)
  1b03b0: 0e 00 80 50  	beqzl	$4, 0x1b03ec <.text+0xb03ec>
  1b03b4: 03 00 22 91  	lbu	$2, 0x3($9)
  1b03b8: 02 00 c3 90  	lbu	$3, 0x2($6)
  1b03bc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b03c0: 2d 00 62 10  	beq	$3, $2, 0x1b0478 <.text+0xb0478>
  1b03c4: 40 18 04 00  	sll	$3, $4, 0x1
  1b03c8: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b03cc: 21 18 62 00  	addu	$3, $3, $2
  1b03d0: 00 00 62 94  	lhu	$2, 0x0($3)
  1b03d4: 04 00 e2 a4  	sh	$2, 0x4($7)
  1b03d8: 36 00 02 3c  	lui	$2, 0x36
  1b03dc: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b03e0: 02 00 22 a1  	sb	$2, 0x2($9)
  1b03e4: 36 00 03 3c  	lui	$3, 0x36
  1b03e8: 03 00 22 91  	lbu	$2, 0x3($9)
  1b03ec: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b03f0: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b03f4: 2b 10 43 00  	sltu	$2, $2, $3
  1b03f8: 0e 00 40 10  	beqz	$2, 0x1b0434 <.text+0xb0434>
  1b03fc: 00 00 00 00  	nop
  1b0400: 03 00 44 91  	lbu	$4, 0x3($10)
  1b0404: 0b 00 80 10  	beqz	$4, 0x1b0434 <.text+0xb0434>
  1b0408: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b040c: 03 00 c3 90  	lbu	$3, 0x3($6)
  1b0410: 0a 00 62 10  	beq	$3, $2, 0x1b043c <.text+0xb043c>
  1b0414: 40 18 04 00  	sll	$3, $4, 0x1
  1b0418: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b041c: 21 18 62 00  	addu	$3, $3, $2
  1b0420: 00 00 62 94  	lhu	$2, 0x0($3)
  1b0424: 06 00 e2 a4  	sh	$2, 0x6($7)
  1b0428: 36 00 02 3c  	lui	$2, 0x36
  1b042c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b0430: 03 00 22 a1  	sb	$2, 0x3($9)
  1b0434: 08 00 e0 03  	jr	$ra
  1b0438: 00 00 00 00  	nop
  1b043c: 44 00 a3 8c  	lw	$3, 0x44($5)
  1b0440: 40 10 04 00  	sll	$2, $4, 0x1
  1b0444: 50 00 a4 8c  	lw	$4, 0x50($5)
  1b0448: 21 10 43 00  	addu	$2, $2, $3
  1b044c: 20 00 a5 8c  	lw	$5, 0x20($5)
  1b0450: 00 00 42 94  	lhu	$2, 0x0($2)
  1b0454: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b0458: 24 20 83 00  	and	$4, $4, $3
  1b045c: 20 84 42 34  	ori	$2, $2, 0x8420
  1b0460: 23 10 44 00  	subu	$2, $2, $4
  1b0464: 42 10 02 00  	srl	$2, $2, 0x1
  1b0468: 40 10 02 00  	sll	$2, $2, 0x1
  1b046c: 21 10 45 00  	addu	$2, $2, $5
  1b0470: ec ff 00 10  	b	0x1b0424 <.text+0xb0424>
  1b0474: 00 00 42 94  	lhu	$2, 0x0($2)
  1b0478: 44 00 a3 8c  	lw	$3, 0x44($5)
  1b047c: 40 10 04 00  	sll	$2, $4, 0x1
  1b0480: 50 00 a4 8c  	lw	$4, 0x50($5)
  1b0484: 21 10 43 00  	addu	$2, $2, $3
  1b0488: 20 00 a5 8c  	lw	$5, 0x20($5)
  1b048c: 00 00 42 94  	lhu	$2, 0x0($2)
  1b0490: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b0494: 24 20 83 00  	and	$4, $4, $3
  1b0498: 20 84 42 34  	ori	$2, $2, 0x8420
  1b049c: 23 10 44 00  	subu	$2, $2, $4
  1b04a0: 42 10 02 00  	srl	$2, $2, 0x1
  1b04a4: 40 10 02 00  	sll	$2, $2, 0x1
  1b04a8: 21 10 45 00  	addu	$2, $2, $5
  1b04ac: c9 ff 00 10  	b	0x1b03d4 <.text+0xb03d4>
  1b04b0: 00 00 42 94  	lhu	$2, 0x0($2)
  1b04b4: 44 00 a3 8c  	lw	$3, 0x44($5)
  1b04b8: 40 10 04 00  	sll	$2, $4, 0x1
  1b04bc: 50 00 a4 8c  	lw	$4, 0x50($5)
  1b04c0: 21 10 43 00  	addu	$2, $2, $3
  1b04c4: 20 00 a5 8c  	lw	$5, 0x20($5)
  1b04c8: 00 00 42 94  	lhu	$2, 0x0($2)
  1b04cc: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b04d0: 24 20 83 00  	and	$4, $4, $3
  1b04d4: 20 84 42 34  	ori	$2, $2, 0x8420
  1b04d8: 23 10 44 00  	subu	$2, $2, $4
  1b04dc: 42 10 02 00  	srl	$2, $2, 0x1
  1b04e0: 40 10 02 00  	sll	$2, $2, 0x1
  1b04e4: 21 10 45 00  	addu	$2, $2, $5
  1b04e8: a5 ff 00 10  	b	0x1b0380 <.text+0xb0380>
  1b04ec: 00 00 42 94  	lhu	$2, 0x0($2)
  1b04f0: 44 00 03 8d  	lw	$3, 0x44($8)
  1b04f4: 40 10 04 00  	sll	$2, $4, 0x1
  1b04f8: 50 00 04 8d  	lw	$4, 0x50($8)
  1b04fc: 21 10 43 00  	addu	$2, $2, $3
  1b0500: 20 00 05 8d  	lw	$5, 0x20($8)
  1b0504: 00 00 42 94  	lhu	$2, 0x0($2)
  1b0508: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b050c: 24 20 83 00  	and	$4, $4, $3
  1b0510: 20 84 42 34  	ori	$2, $2, 0x8420
  1b0514: 23 10 44 00  	subu	$2, $2, $4
  1b0518: 42 10 02 00  	srl	$2, $2, 0x1
  1b051c: 40 10 02 00  	sll	$2, $2, 0x1
  1b0520: 21 10 45 00  	addu	$2, $2, $5
  1b0524: 81 ff 00 10  	b	0x1b032c <.text+0xb032c>
  1b0528: 00 00 42 94  	lhu	$2, 0x0($2)
  1b052c: 36 00 02 3c  	lui	$2, 0x36
  1b0530: 2d 50 a0 00  	move	$10, $5
  1b0534: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1b0538: 40 38 04 00  	sll	$7, $4, 0x1
  1b053c: 08 00 02 8d  	lw	$2, 0x8($8)
  1b0540: 4c 00 06 91  	lbu	$6, 0x4c($8)
  1b0544: 21 48 44 00  	addu	$9, $2, $4
  1b0548: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1b054c: 00 00 22 91  	lbu	$2, 0x0($9)
  1b0550: 0c 00 05 8d  	lw	$5, 0xc($8)
  1b0554: 21 38 67 00  	addu	$7, $3, $7
  1b0558: 2b 10 46 00  	sltu	$2, $2, $6
  1b055c: 0f 00 40 10  	beqz	$2, 0x1b059c <.text+0xb059c>
  1b0560: 21 30 a4 00  	addu	$6, $5, $4
  1b0564: 03 00 44 91  	lbu	$4, 0x3($10)
  1b0568: 0d 00 80 10  	beqz	$4, 0x1b05a0 <.text+0xb05a0>
  1b056c: 36 00 03 3c  	lui	$3, 0x36
  1b0570: 00 00 c3 90  	lbu	$3, 0x0($6)
  1b0574: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b0578: 75 00 62 10  	beq	$3, $2, 0x1b0750 <.text+0xb0750>
  1b057c: 40 18 04 00  	sll	$3, $4, 0x1
  1b0580: 44 00 02 8d  	lw	$2, 0x44($8)
  1b0584: 21 18 62 00  	addu	$3, $3, $2
  1b0588: 00 00 62 94  	lhu	$2, 0x0($3)
  1b058c: 00 00 e2 a4  	sh	$2, 0x0($7)
  1b0590: 36 00 02 3c  	lui	$2, 0x36
  1b0594: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b0598: 00 00 22 a1  	sb	$2, 0x0($9)
  1b059c: 36 00 03 3c  	lui	$3, 0x36
  1b05a0: 01 00 22 91  	lbu	$2, 0x1($9)
  1b05a4: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b05a8: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b05ac: 2b 10 43 00  	sltu	$2, $2, $3
  1b05b0: 10 00 40 10  	beqz	$2, 0x1b05f4 <.text+0xb05f4>
  1b05b4: 36 00 03 3c  	lui	$3, 0x36
  1b05b8: 02 00 44 91  	lbu	$4, 0x2($10)
  1b05bc: 0e 00 80 50  	beqzl	$4, 0x1b05f8 <.text+0xb05f8>
  1b05c0: 02 00 22 91  	lbu	$2, 0x2($9)
  1b05c4: 01 00 c3 90  	lbu	$3, 0x1($6)
  1b05c8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b05cc: 51 00 62 10  	beq	$3, $2, 0x1b0714 <.text+0xb0714>
  1b05d0: 40 18 04 00  	sll	$3, $4, 0x1
  1b05d4: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b05d8: 21 18 62 00  	addu	$3, $3, $2
  1b05dc: 00 00 62 94  	lhu	$2, 0x0($3)
  1b05e0: 02 00 e2 a4  	sh	$2, 0x2($7)
  1b05e4: 36 00 02 3c  	lui	$2, 0x36
  1b05e8: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b05ec: 01 00 22 a1  	sb	$2, 0x1($9)
  1b05f0: 36 00 03 3c  	lui	$3, 0x36
  1b05f4: 02 00 22 91  	lbu	$2, 0x2($9)
  1b05f8: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b05fc: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b0600: 2b 10 43 00  	sltu	$2, $2, $3
  1b0604: 10 00 40 10  	beqz	$2, 0x1b0648 <.text+0xb0648>
  1b0608: 36 00 03 3c  	lui	$3, 0x36
  1b060c: 01 00 44 91  	lbu	$4, 0x1($10)
  1b0610: 0e 00 80 50  	beqzl	$4, 0x1b064c <.text+0xb064c>
  1b0614: 03 00 22 91  	lbu	$2, 0x3($9)
  1b0618: 02 00 c3 90  	lbu	$3, 0x2($6)
  1b061c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b0620: 2d 00 62 10  	beq	$3, $2, 0x1b06d8 <.text+0xb06d8>
  1b0624: 40 18 04 00  	sll	$3, $4, 0x1
  1b0628: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b062c: 21 18 62 00  	addu	$3, $3, $2
  1b0630: 00 00 62 94  	lhu	$2, 0x0($3)
  1b0634: 04 00 e2 a4  	sh	$2, 0x4($7)
  1b0638: 36 00 02 3c  	lui	$2, 0x36
  1b063c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b0640: 02 00 22 a1  	sb	$2, 0x2($9)
  1b0644: 36 00 03 3c  	lui	$3, 0x36
  1b0648: 03 00 22 91  	lbu	$2, 0x3($9)
  1b064c: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1b0650: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1b0654: 2b 10 43 00  	sltu	$2, $2, $3
  1b0658: 0e 00 40 10  	beqz	$2, 0x1b0694 <.text+0xb0694>
  1b065c: 00 00 00 00  	nop
  1b0660: 00 00 44 91  	lbu	$4, 0x0($10)
  1b0664: 0b 00 80 10  	beqz	$4, 0x1b0694 <.text+0xb0694>
  1b0668: 01 00 02 24  	addiu	$2, $zero, 0x1
  1b066c: 03 00 c3 90  	lbu	$3, 0x3($6)
  1b0670: 0a 00 62 10  	beq	$3, $2, 0x1b069c <.text+0xb069c>
  1b0674: 40 18 04 00  	sll	$3, $4, 0x1
  1b0678: 44 00 a2 8c  	lw	$2, 0x44($5)
  1b067c: 21 18 62 00  	addu	$3, $3, $2
  1b0680: 00 00 62 94  	lhu	$2, 0x0($3)
  1b0684: 06 00 e2 a4  	sh	$2, 0x6($7)
  1b0688: 36 00 02 3c  	lui	$2, 0x36
  1b068c: cd d4 42 90  	lbu	$2, -0x2b33($2)
  1b0690: 03 00 22 a1  	sb	$2, 0x3($9)
  1b0694: 08 00 e0 03  	jr	$ra
  1b0698: 00 00 00 00  	nop
  1b069c: 44 00 a3 8c  	lw	$3, 0x44($5)
  1b06a0: 40 10 04 00  	sll	$2, $4, 0x1
  1b06a4: 50 00 a4 8c  	lw	$4, 0x50($5)
  1b06a8: 21 10 43 00  	addu	$2, $2, $3
  1b06ac: 20 00 a5 8c  	lw	$5, 0x20($5)
  1b06b0: 00 00 42 94  	lhu	$2, 0x0($2)
  1b06b4: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b06b8: 24 20 83 00  	and	$4, $4, $3
  1b06bc: 20 84 42 34  	ori	$2, $2, 0x8420
  1b06c0: 23 10 44 00  	subu	$2, $2, $4
  1b06c4: 42 10 02 00  	srl	$2, $2, 0x1
  1b06c8: 40 10 02 00  	sll	$2, $2, 0x1
  1b06cc: 21 10 45 00  	addu	$2, $2, $5
  1b06d0: ec ff 00 10  	b	0x1b0684 <.text+0xb0684>
  1b06d4: 00 00 42 94  	lhu	$2, 0x0($2)
  1b06d8: 44 00 a3 8c  	lw	$3, 0x44($5)
  1b06dc: 40 10 04 00  	sll	$2, $4, 0x1
  1b06e0: 50 00 a4 8c  	lw	$4, 0x50($5)
  1b06e4: 21 10 43 00  	addu	$2, $2, $3
  1b06e8: 20 00 a5 8c  	lw	$5, 0x20($5)
  1b06ec: 00 00 42 94  	lhu	$2, 0x0($2)
  1b06f0: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b06f4: 24 20 83 00  	and	$4, $4, $3
  1b06f8: 20 84 42 34  	ori	$2, $2, 0x8420
  1b06fc: 23 10 44 00  	subu	$2, $2, $4
  1b0700: 42 10 02 00  	srl	$2, $2, 0x1
  1b0704: 40 10 02 00  	sll	$2, $2, 0x1
  1b0708: 21 10 45 00  	addu	$2, $2, $5
  1b070c: c9 ff 00 10  	b	0x1b0634 <.text+0xb0634>
  1b0710: 00 00 42 94  	lhu	$2, 0x0($2)
  1b0714: 44 00 a3 8c  	lw	$3, 0x44($5)
  1b0718: 40 10 04 00  	sll	$2, $4, 0x1
  1b071c: 50 00 a4 8c  	lw	$4, 0x50($5)
  1b0720: 21 10 43 00  	addu	$2, $2, $3
  1b0724: 20 00 a5 8c  	lw	$5, 0x20($5)
  1b0728: 00 00 42 94  	lhu	$2, 0x0($2)
  1b072c: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b0730: 24 20 83 00  	and	$4, $4, $3
  1b0734: 20 84 42 34  	ori	$2, $2, 0x8420
  1b0738: 23 10 44 00  	subu	$2, $2, $4
  1b073c: 42 10 02 00  	srl	$2, $2, 0x1
  1b0740: 40 10 02 00  	sll	$2, $2, 0x1
  1b0744: 21 10 45 00  	addu	$2, $2, $5
  1b0748: a5 ff 00 10  	b	0x1b05e0 <.text+0xb05e0>
  1b074c: 00 00 42 94  	lhu	$2, 0x0($2)
  1b0750: 44 00 03 8d  	lw	$3, 0x44($8)
  1b0754: 40 10 04 00  	sll	$2, $4, 0x1
  1b0758: 50 00 04 8d  	lw	$4, 0x50($8)
  1b075c: 21 10 43 00  	addu	$2, $2, $3
  1b0760: 20 00 05 8d  	lw	$5, 0x20($8)
  1b0764: 00 00 42 94  	lhu	$2, 0x0($2)
  1b0768: de fb 03 24  	addiu	$3, $zero, -0x422 <.text+0xffffffffffeffbde>
  1b076c: 24 20 83 00  	and	$4, $4, $3
  1b0770: 20 84 42 34  	ori	$2, $2, 0x8420
  1b0774: 23 10 44 00  	subu	$2, $2, $4
  1b0778: 42 10 02 00  	srl	$2, $2, 0x1
  1b077c: 40 10 02 00  	sll	$2, $2, 0x1
  1b0780: 21 10 45 00  	addu	$2, $2, $5
  1b0784: 81 ff 00 10  	b	0x1b058c <.text+0xb058c>
  1b0788: 00 00 42 94  	lhu	$2, 0x0($2)
  1b078c: 00 00 00 00  	nop
