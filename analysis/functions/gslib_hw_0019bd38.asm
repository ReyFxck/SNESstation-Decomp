  19bd38: 42 00 03 3c  	lui	$3, 0x42
  19bd3c: 30 5a 62 8c  	lw	$2, 0x5a30($3)
  19bd40: 01 00 42 24  	addiu	$2, $2, 0x1
  19bd44: 08 00 e0 03  	jr	$ra
  19bd48: 30 5a 62 ac  	sw	$2, 0x5a30($3)
  19bd4c: 00 00 00 00  	nop
  19bd50: 42 00 02 3c  	lui	$2, 0x42
  19bd54: 2b 20 04 00  	sltu	$4, $zero, $4
  19bd58: 30 5a 40 ac  	sw	$zero, 0x5a30($2)
  19bd64: fe ff 80 14  	bnez	$4, 0x19bd60 <.text+0x9bd60>
  19bd68: 00 00 00 00  	nop
  19bd6c: 08 00 e0 03  	jr	$ra
  19bd78: 42 00 03 3c  	lui	$3, 0x42
  19bd7c: 08 00 e0 03  	jr	$ra
  19bd80: 30 5a 62 8c  	lw	$2, 0x5a30($3)
  19bd84: 00 00 00 00  	nop
  19bd88: 42 00 02 3c  	lui	$2, 0x42
  19bd8c: 08 00 e0 03  	jr	$ra
  19bd90: 30 5a 40 ac  	sw	$zero, 0x5a30($2)
  19bd94: 00 00 00 00  	nop
  19bd98: 01 10 01 3c  	lui	$1, 0x1001
  19bd9c: 80 a0 20 ac  	sw	$zero, -0x5f80($1)
  19bda0: 01 10 01 3c  	lui	$1, 0x1001
  19bda4: 00 a0 20 ac  	sw	$zero, -0x6000($1)
  19bda8: 01 10 01 3c  	lui	$1, 0x1001
  19bdac: 30 a0 20 ac  	sw	$zero, -0x5fd0($1)
  19bdb0: 01 10 01 3c  	lui	$1, 0x1001
  19bdb4: 10 a0 20 ac  	sw	$zero, -0x5ff0($1)
  19bdb8: 01 10 01 3c  	lui	$1, 0x1001
  19bdbc: 50 a0 20 ac  	sw	$zero, -0x5fb0($1)
  19bdc0: 01 10 01 3c  	lui	$1, 0x1001
  19bdc4: 40 a0 20 ac  	sw	$zero, -0x5fc0($1)
  19bdc8: 1f ff 02 34  	ori	$2, $zero, 0xff1f
  19bdcc: 01 10 01 3c  	lui	$1, 0x1001
  19bdd0: 10 e0 22 ac  	sw	$2, -0x1ff0($1)
  19bdd4: 01 10 01 3c  	lui	$1, 0x1001
  19bdd8: 00 e0 20 ac  	sw	$zero, -0x2000($1)
  19bddc: 01 10 01 3c  	lui	$1, 0x1001
  19bde0: 20 e0 20 ac  	sw	$zero, -0x1fe0($1)
  19bde4: 01 10 01 3c  	lui	$1, 0x1001
  19bde8: 30 e0 20 ac  	sw	$zero, -0x1fd0($1)
  19bdec: 01 10 01 3c  	lui	$1, 0x1001
  19bdf0: 50 e0 20 ac  	sw	$zero, -0x1fb0($1)
  19bdf4: 01 10 01 3c  	lui	$1, 0x1001
  19bdf8: 40 e0 20 ac  	sw	$zero, -0x1fc0($1)
  19bdfc: 01 10 02 3c  	lui	$2, 0x1001
  19be00: 00 e0 42 8c  	lw	$2, -0x2000($2)
  19be04: 01 00 43 34  	ori	$3, $2, 0x1
  19be08: 00 00 00 00  	nop
  19be0c: 01 10 01 3c  	lui	$1, 0x1001
  19be10: 00 e0 23 ac  	sw	$3, -0x2000($1)
  19be14: 08 00 e0 03  	jr	$ra
  19be20: 00 10 03 3c  	lui	$3, 0x1000
  19be24: 00 a0 63 34  	ori	$3, $3, 0xa000
  19be28: 30 00 64 ac  	sw	$4, 0x30($3)
  19be2c: 20 00 60 ac  	sw	$zero, 0x20($3)
  19be30: 00 00 62 8c  	lw	$2, 0x0($3)
  19be34: 05 01 42 34  	ori	$2, $2, 0x105
  19be38: 08 00 e0 03  	jr	$ra
  19be3c: 00 00 62 ac  	sw	$2, 0x0($3)
  19be40: fc ff bd 27  	addiu	$sp, $sp, -0x4 <.text+0xffffffffffeffffc>
  19be44: 00 00 a8 af  	sw	$8, 0x0($sp)
  19be48: 01 10 08 3c  	lui	$8, 0x1001
  19be4c: 00 a0 08 8d  	lw	$8, -0x6000($8)
  19be50: 00 00 00 00  	nop
  19be54: 00 01 08 31  	andi	$8, $8, 0x100
  19be58: fb ff 00 15  	bnez	$8, 0x19be48 <.text+0x9be48>
  19be64: 00 00 a8 8f  	lw	$8, 0x0($sp)
  19be68: 08 00 e0 03  	jr	$ra
  19be6c: 04 00 bd 27  	addiu	$sp, $sp, 0x4
