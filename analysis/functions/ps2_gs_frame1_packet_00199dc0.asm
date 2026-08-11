  199dc0: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  199dc4: ff ff 02 3c  	lui	$2, 0xffff
  199dc8: 00 00 bf ff  	sd	$ra, 0x0($sp)
  199dcc: ba 31 06 00  	dsrl	$6, $6, 0x6
  199dd0: 38 34 06 00  	dsll	$6, $6, 0x10
  199dd4: 7a 2b 05 00  	dsrl	$5, $5, 0xd
  199dd8: 10 00 8b 8c  	lw	$11, 0x10($4)
  199ddc: 25 28 a6 00  	or	$5, $5, $6
  199de0: 3c 40 08 00  	dsll32	$8, $8, 0x0
  199de4: 00 00 69 dd  	ld	$9, 0x0($11)
  199de8: 38 3e 07 00  	dsll	$7, $7, 0x18
  199dec: 25 28 a7 00  	or	$5, $5, $7
  199df0: ff ff 23 31  	andi	$3, $9, 0xffff
  199df4: 25 28 a8 00  	or	$5, $5, $8
  199df8: 24 48 22 01  	and	$9, $9, $2
  199dfc: 02 00 63 64  	daddiu	$3, $3, 0x2
  199e00: 25 48 23 01  	or	$9, $9, $3
  199e04: 00 10 02 3c  	lui	$2, 0x1000
  199e08: 3c 10 02 00  	dsll32	$2, $2, 0x0
  199e0c: 01 80 42 34  	ori	$2, $2, 0x8001
  199e10: 00 00 69 fd  	sd	$9, 0x0($11)
  199e14: 14 00 83 8c  	lw	$3, 0x14($4)
  199e18: 00 00 62 fc  	sd	$2, 0x0($3)
  199e1c: fe ff 02 24  	addiu	$2, $zero, -0x2 <.text+0xffffffffffeffffe>
  199e20: 14 00 86 8c  	lw	$6, 0x14($4)
  199e24: 08 00 c2 fc  	sd	$2, 0x8($6)
  199e28: 14 00 83 8c  	lw	$3, 0x14($4)
  199e2c: 10 00 65 fc  	sd	$5, 0x10($3)
  199e30: 4c 00 03 24  	addiu	$3, $zero, 0x4c
  199e34: 14 00 85 8c  	lw	$5, 0x14($4)
  199e38: 18 00 a3 fc  	sd	$3, 0x18($5)
  199e3c: 14 00 82 8c  	lw	$2, 0x14($4)
  199e40: 20 00 42 24  	addiu	$2, $2, 0x20
  199e44: 2e 66 06 0c  	jal	0x1998b8 <.text+0x998b8>
  199e48: 14 00 82 ac  	sw	$2, 0x14($4)
  199e4c: 00 00 bf df  	ld	$ra, 0x0($sp)
  199e50: 08 00 e0 03  	jr	$ra
  199e54: 10 00 bd 27  	addiu	$sp, $sp, 0x10
