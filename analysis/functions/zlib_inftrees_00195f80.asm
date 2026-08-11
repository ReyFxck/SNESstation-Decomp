  195f80: 70 fe bd 27  	addiu	$sp, $sp, -0x190 <.text+0xffffffffffeffe70>
  195f84: 2d 70 80 00  	move	$14, $4
  195f88: 50 01 b5 ff  	sd	$21, 0x150($sp)
  195f8c: 2d 68 a0 00  	move	$13, $5
  195f90: 70 01 b7 ff  	sd	$23, 0x170($sp)
  195f94: 40 01 b4 ff  	sd	$20, 0x140($sp)
  195f98: 10 01 b1 ff  	sd	$17, 0x110($sp)
  195f9c: 00 01 b0 ff  	sd	$16, 0x100($sp)
  195fa0: 00 00 a0 af  	sw	$zero, 0x0($sp)
  195fa4: 04 00 a0 af  	sw	$zero, 0x4($sp)
  195fa8: d0 00 a7 af  	sw	$7, 0xd0($sp)
  195fac: 08 00 a0 af  	sw	$zero, 0x8($sp)
  195fb0: 0c 00 a0 af  	sw	$zero, 0xc($sp)
  195fb4: 10 00 a0 af  	sw	$zero, 0x10($sp)
  195fb8: 14 00 a0 af  	sw	$zero, 0x14($sp)
  195fbc: f0 00 ab af  	sw	$11, 0xf0($sp)
  195fc0: 18 00 a0 af  	sw	$zero, 0x18($sp)
  195fc4: 1c 00 a0 af  	sw	$zero, 0x1c($sp)
  195fc8: 20 00 a0 af  	sw	$zero, 0x20($sp)
  195fcc: 24 00 a0 af  	sw	$zero, 0x24($sp)
  195fd0: 28 00 a0 af  	sw	$zero, 0x28($sp)
  195fd4: 2c 00 a0 af  	sw	$zero, 0x2c($sp)
  195fd8: 30 00 a0 af  	sw	$zero, 0x30($sp)
  195fdc: 34 00 a0 af  	sw	$zero, 0x34($sp)
  195fe0: 38 00 a0 af  	sw	$zero, 0x38($sp)
  195fe4: 3c 00 a0 af  	sw	$zero, 0x3c($sp)
  195fe8: 98 01 b5 8f  	lw	$21, 0x198($sp)
  195fec: 80 01 be ff  	sd	$fp, 0x180($sp)
  195ff0: 2d f0 00 01  	move	$fp, $8
  195ff4: 60 01 b6 ff  	sd	$22, 0x160($sp)
  195ff8: 2d b0 20 01  	move	$22, $9
  195ffc: 30 01 b3 ff  	sd	$19, 0x130($sp)
  196000: 2d 98 c0 00  	move	$19, $6
  196004: 20 01 b2 ff  	sd	$18, 0x120($sp)
  196008: 2d 90 a0 00  	move	$18, $5
  19600c: 00 00 c3 8d  	lw	$3, 0x0($14)
  196010: ff ff ad 25  	addiu	$13, $13, -0x1 <.text+0xffffffffffefffff>
  196014: 04 00 ce 25  	addiu	$14, $14, 0x4
  196018: 80 18 03 00  	sll	$3, $3, 0x2
  19601c: 21 18 7d 00  	addu	$3, $3, $sp
  196020: 00 00 62 8c  	lw	$2, 0x0($3)
  196024: 01 00 42 24  	addiu	$2, $2, 0x1
  196028: f8 ff a0 15  	bnez	$13, 0x19600c <.text+0x9600c>
  19602c: 00 00 62 ac  	sw	$2, 0x0($3)
  196030: 00 00 a2 8f  	lw	$2, 0x0($sp)
  196034: 23 01 52 10  	beq	$2, $18, 0x1964c4 <.text+0x964c4>
  196038: 01 00 0c 24  	addiu	$12, $zero, 0x1
  19603c: 00 00 4f 8d  	lw	$15, 0x0($10)
  196040: 04 00 a3 27  	addiu	$3, $sp, 0x4
  196044: 00 00 62 8c  	lw	$2, 0x0($3)
  196048: 05 00 40 14  	bnez	$2, 0x196060 <.text+0x96060>
  19604c: 04 00 63 24  	addiu	$3, $3, 0x4
  196050: 01 00 8c 25  	addiu	$12, $12, 0x1
  196054: 10 00 82 2d  	sltiu	$2, $12, 0x10
  196058: fb ff 40 54  	bnezl	$2, 0x196048 <.text+0x96048>
  19605c: 00 00 62 8c  	lw	$2, 0x0($3)
  196060: 2b 10 ec 01  	sltu	$2, $15, $12
  196064: 2d 38 80 01  	move	$7, $12
  196068: 0f 00 0d 24  	addiu	$13, $zero, 0xf
  19606c: 3c 00 a3 27  	addiu	$3, $sp, 0x3c
  196070: 0b 78 82 01  	movn	$15, $12, $2
  196074: 00 00 62 8c  	lw	$2, 0x0($3)
  196078: 04 00 40 14  	bnez	$2, 0x19608c <.text+0x9608c>
  19607c: fc ff 63 24  	addiu	$3, $3, -0x4 <.text+0xffffffffffeffffc>
  196080: ff ff ad 25  	addiu	$13, $13, -0x1 <.text+0xffffffffffefffff>
  196084: fc ff a0 55  	bnezl	$13, 0x196078 <.text+0x96078>
  196088: 00 00 62 8c  	lw	$2, 0x0($3)
  19608c: 2b 10 af 01  	sltu	$2, $13, $15
  196090: 2b 18 8d 01  	sltu	$3, $12, $13
  196094: 0b 78 a2 01  	movn	$15, $13, $2
  196098: 2d 58 a0 01  	move	$11, $13
  19609c: 01 00 02 24  	addiu	$2, $zero, 0x1
  1960a0: 00 00 4f ad  	sw	$15, 0x0($10)
  1960a4: 0b 00 60 10  	beqz	$3, 0x1960d4 <.text+0x960d4>
  1960a8: 04 40 82 01  	sllv	$8, $2, $12
  1960ac: 80 10 0c 00  	sll	$2, $12, 0x2
  1960b0: 21 18 5d 00  	addu	$3, $2, $sp
  1960b4: 00 00 62 8c  	lw	$2, 0x0($3)
  1960b8: 01 00 8c 25  	addiu	$12, $12, 0x1
  1960bc: 2b 50 8d 01  	sltu	$10, $12, $13
  1960c0: 23 40 02 01  	subu	$8, $8, $2
  1960c4: fd 00 00 05  	bltz	$8, 0x1964bc <.text+0x964bc>
  1960c8: 04 00 63 24  	addiu	$3, $3, 0x4
  1960cc: f9 ff 40 15  	bnez	$10, 0x1960b4 <.text+0x960b4>
  1960d0: 40 40 08 00  	sll	$8, $8, 0x1
  1960d4: 80 10 0d 00  	sll	$2, $13, 0x2
  1960d8: 21 50 5d 00  	addu	$10, $2, $sp
  1960dc: 00 00 42 8d  	lw	$2, 0x0($10)
  1960e0: 23 40 02 01  	subu	$8, $8, $2
  1960e4: c9 00 00 05  	bltz	$8, 0x19640c <.text+0x9640c>
  1960e8: fd ff 03 24  	addiu	$3, $zero, -0x3 <.text+0xffffffffffeffffd>
  1960ec: 21 10 48 00  	addu	$2, $2, $8
  1960f0: ff ff ad 25  	addiu	$13, $13, -0x1 <.text+0xffffffffffefffff>
  1960f4: 00 00 42 ad  	sw	$2, 0x0($10)
  1960f8: 04 00 ae 27  	addiu	$14, $sp, 0x4
  1960fc: 94 00 a0 af  	sw	$zero, 0x94($sp)
  196100: 2d 60 00 00  	move	$12, $zero
  196104: 09 00 a0 11  	beqz	$13, 0x19612c <.text+0x9612c>
  196108: 98 00 aa 27  	addiu	$10, $sp, 0x98
  19610c: 00 00 c2 8d  	lw	$2, 0x0($14)
  196110: ff ff ad 25  	addiu	$13, $13, -0x1 <.text+0xffffffffffefffff>
  196114: 04 00 ce 25  	addiu	$14, $14, 0x4
  196118: 21 60 82 01  	addu	$12, $12, $2
  19611c: 00 00 4c ad  	sw	$12, 0x0($10)
  196120: 00 00 00 00  	nop
  196124: f9 ff a0 15  	bnez	$13, 0x19610c <.text+0x9610c>
  196128: 04 00 4a 25  	addiu	$10, $10, 0x4
  19612c: 2d 70 80 00  	move	$14, $4
  196130: 2d 68 00 00  	move	$13, $zero
  196134: 00 00 cc 8d  	lw	$12, 0x0($14)
  196138: 04 00 ce 25  	addiu	$14, $14, 0x4
  19613c: 08 00 80 11  	beqz	$12, 0x196160 <.text+0x96160>
  196140: 80 10 0c 00  	sll	$2, $12, 0x2
  196144: 21 10 5d 00  	addu	$2, $2, $sp
  196148: 90 00 43 8c  	lw	$3, 0x90($2)
  19614c: 80 20 03 00  	sll	$4, $3, 0x2
  196150: 01 00 63 24  	addiu	$3, $3, 0x1
  196154: 21 20 95 00  	addu	$4, $4, $21
  196158: 90 00 43 ac  	sw	$3, 0x90($2)
  19615c: 00 00 8d ac  	sw	$13, 0x0($4)
  196160: 01 00 ad 25  	addiu	$13, $13, 0x1
  196164: 2b 10 b2 01  	sltu	$2, $13, $18
  196168: f3 ff 40 54  	bnezl	$2, 0x196138 <.text+0x96138>
  19616c: 00 00 cc 8d  	lw	$12, 0x0($14)
  196170: 80 10 0b 00  	sll	$2, $11, 0x2
  196174: 2a 18 67 01  	slt	$3, $11, $7
  196178: 21 10 5d 00  	addu	$2, $2, $sp
  19617c: 50 00 a0 af  	sw	$zero, 0x50($sp)
  196180: 90 00 52 8c  	lw	$18, 0x90($2)
  196184: 2d 68 00 00  	move	$13, $zero
  196188: 90 00 a0 af  	sw	$zero, 0x90($sp)
  19618c: 2d 70 a0 02  	move	$14, $21
  196190: ff ff 10 24  	addiu	$16, $zero, -0x1 <.text+0xffffffffffefffff>
  196194: 23 c8 0f 00  	negu	$25, $15
  196198: 2d 50 00 00  	move	$10, $zero
  19619c: 96 00 60 14  	bnez	$3, 0x1963f8 <.text+0x963f8>
  1961a0: 2d 48 00 00  	move	$9, $zero
  1961a4: 80 a0 07 00  	sll	$20, $7, 0x2
  1961a8: 21 10 9d 02  	addu	$2, $20, $sp
  1961ac: 00 00 46 8c  	lw	$6, 0x0($2)
  1961b0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1961b4: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  1961b8: 8b 00 c2 10  	beq	$6, $2, 0x1963e8 <.text+0x963e8>
  1961bc: 80 10 10 00  	sll	$2, $16, 0x2
  1961c0: 21 88 5d 00  	addu	$17, $2, $sp
  1961c4: 21 c0 2f 03  	addu	$24, $25, $15
  1961c8: 2a 10 07 03  	slt	$2, $24, $7
  1961cc: 44 00 40 10  	beqz	$2, 0x1962e0 <.text+0x962e0>
  1961d0: 80 10 10 00  	sll	$2, $16, 0x2
  1961d4: 21 28 5d 00  	addu	$5, $2, $sp
  1961d8: 01 00 02 24  	addiu	$2, $zero, 0x1
  1961dc: 23 60 f8 00  	subu	$12, $7, $24
  1961e0: 23 48 78 01  	subu	$9, $11, $24
  1961e4: 04 20 82 01  	sllv	$4, $2, $12
  1961e8: 01 00 c3 24  	addiu	$3, $6, 0x1
  1961ec: 2b 10 e9 01  	sltu	$2, $15, $9
  1961f0: 2b 18 64 00  	sltu	$3, $3, $4
  1961f4: 2d c8 00 03  	move	$25, $24
  1961f8: 0b 48 e2 01  	movn	$9, $15, $2
  1961fc: 04 00 a5 24  	addiu	$5, $5, 0x4
  196200: 04 00 31 26  	addiu	$17, $17, 0x4
  196204: 14 00 60 10  	beqz	$3, 0x196258 <.text+0x96258>
  196208: 01 00 10 26  	addiu	$16, $16, 0x1
  19620c: 23 10 86 00  	subu	$2, $4, $6
  196210: 2b 18 89 01  	sltu	$3, $12, $9
  196214: ff ff 44 24  	addiu	$4, $2, -0x1 <.text+0xffffffffffefffff>
  196218: 0f 00 60 10  	beqz	$3, 0x196258 <.text+0x96258>
  19621c: 21 50 b4 03  	addu	$10, $sp, $20
  196220: 01 00 8c 25  	addiu	$12, $12, 0x1
  196224: 2b 10 89 01  	sltu	$2, $12, $9
  196228: 0c 00 40 10  	beqz	$2, 0x19625c <.text+0x9625c>
  19622c: 90 01 a2 8f  	lw	$2, 0x190($sp)
  196230: 04 00 4a 25  	addiu	$10, $10, 0x4
  196234: 40 20 04 00  	sll	$4, $4, 0x1
  196238: 00 00 42 8d  	lw	$2, 0x0($10)
  19623c: 2b 18 44 00  	sltu	$3, $2, $4
  196240: 05 00 60 10  	beqz	$3, 0x196258 <.text+0x96258>
  196244: 23 20 82 00  	subu	$4, $4, $2
  196248: 01 00 8c 25  	addiu	$12, $12, 0x1
  19624c: 2b 10 89 01  	sltu	$2, $12, $9
  196250: f8 ff 40 54  	bnezl	$2, 0x196234 <.text+0x96234>
  196254: 04 00 4a 25  	addiu	$10, $10, 0x4
  196258: 90 01 a2 8f  	lw	$2, 0x190($sp)
  19625c: 00 00 44 8c  	lw	$4, 0x0($2)
  196260: 01 00 02 24  	addiu	$2, $zero, 0x1
  196264: 04 48 82 01  	sllv	$9, $2, $12
  196268: 21 18 89 00  	addu	$3, $4, $9
  19626c: a1 05 62 2c  	sltiu	$2, $3, 0x5a1
  196270: 90 00 40 10  	beqz	$2, 0x1964b4 <.text+0x964b4>
  196274: f0 00 b7 8f  	lw	$23, 0xf0($sp)
  196278: c0 10 04 00  	sll	$2, $4, 0x3
  19627c: 90 01 a4 8f  	lw	$4, 0x190($sp)
  196280: 21 50 e2 02  	addu	$10, $23, $2
  196284: 00 00 83 ac  	sw	$3, 0x0($4)
  196288: 88 00 00 12  	beqz	$16, 0x1964ac <.text+0x964ac>
  19628c: 50 00 aa ac  	sw	$10, 0x50($5)
  196290: 4c 00 a4 8c  	lw	$4, 0x4c($5)
  196294: 23 10 0f 03  	subu	$2, $24, $15
  196298: 40 00 ac a3  	sb	$12, 0x40($sp)
  19629c: 06 60 4d 00  	srlv	$12, $13, $2
  1962a0: 23 10 44 01  	subu	$2, $10, $4
  1962a4: c0 18 0c 00  	sll	$3, $12, 0x3
  1962a8: c3 10 02 00  	sra	$2, $2, 0x3
  1962ac: 41 00 af a3  	sb	$15, 0x41($sp)
  1962b0: 23 10 4c 00  	subu	$2, $2, $12
  1962b4: 21 18 64 00  	addu	$3, $3, $4
  1962b8: 44 00 a2 af  	sw	$2, 0x44($sp)
  1962bc: 90 00 ad ac  	sw	$13, 0x90($5)
  1962c0: 47 00 a2 6b  	ldl	$2, 0x47($sp)
  1962c4: 40 00 a2 6f  	ldr	$2, 0x40($sp)
  1962c8: 07 00 62 b0  	sdl	$2, 0x7($3)
  1962cc: 00 00 62 b4  	sdr	$2, 0x0($3)
  1962d0: 21 c0 0f 03  	addu	$24, $24, $15
  1962d4: 2a 10 07 03  	slt	$2, $24, $7
  1962d8: c0 ff 40 14  	bnez	$2, 0x1961dc <.text+0x961dc>
  1962dc: 01 00 02 24  	addiu	$2, $zero, 0x1
  1962e0: 80 10 12 00  	sll	$2, $18, 0x2
  1962e4: 23 20 f9 00  	subu	$4, $7, $25
  1962e8: 21 10 a2 02  	addu	$2, $21, $2
  1962ec: 2b 10 c2 01  	sltu	$2, $14, $2
  1962f0: 54 00 40 14  	bnez	$2, 0x196444 <.text+0x96444>
  1962f4: 41 00 a4 a3  	sb	$4, 0x41($sp)
  1962f8: c0 ff 02 24  	addiu	$2, $zero, -0x40 <.text+0xffffffffffefffc0>
  1962fc: 40 00 a2 a3  	sb	$2, 0x40($sp)
  196300: 06 60 2d 03  	srlv	$12, $13, $25
  196304: 01 00 02 24  	addiu	$2, $zero, 0x1
  196308: 2b 18 89 01  	sltu	$3, $12, $9
  19630c: 0f 00 60 10  	beqz	$3, 0x19634c <.text+0x9634c>
  196310: 04 20 82 00  	sllv	$4, $2, $4
  196314: c0 10 0c 00  	sll	$2, $12, 0x3
  196318: 21 60 84 01  	addu	$12, $12, $4
  19631c: 21 10 4a 00  	addu	$2, $2, $10
  196320: e4 00 a2 af  	sw	$2, 0xe4($sp)
  196324: 2b 10 89 01  	sltu	$2, $12, $9
  196328: e0 00 a2 af  	sw	$2, 0xe0($sp)
  19632c: e4 00 a3 8f  	lw	$3, 0xe4($sp)
  196330: 47 00 a5 6b  	ldl	$5, 0x47($sp)
  196334: 40 00 a5 6f  	ldr	$5, 0x40($sp)
  196338: 07 00 65 b0  	sdl	$5, 0x7($3)
  19633c: 00 00 65 b4  	sdr	$5, 0x0($3)
  196340: e0 00 a3 8f  	lw	$3, 0xe0($sp)
  196344: f4 ff 60 14  	bnez	$3, 0x196318 <.text+0x96318>
  196348: c0 10 0c 00  	sll	$2, $12, 0x3
  19634c: ff ff e2 24  	addiu	$2, $7, -0x1 <.text+0xffffffffffefffff>
  196350: 01 00 03 24  	addiu	$3, $zero, 0x1
  196354: 04 60 43 00  	sllv	$12, $3, $2
  196358: 24 10 ac 01  	and	$2, $13, $12
  19635c: 0b 00 40 10  	beqz	$2, 0x19638c <.text+0x9638c>
  196360: 01 00 02 24  	addiu	$2, $zero, 0x1
  196364: 26 68 ac 01  	xor	$13, $13, $12
  196368: 42 60 0c 00  	srl	$12, $12, 0x1
  19636c: 24 10 ac 01  	and	$2, $13, $12
  196380: f9 ff 40 54  	bnezl	$2, 0x196368 <.text+0x96368>
  196384: 26 68 ac 01  	xor	$13, $13, $12
  196388: 01 00 02 24  	addiu	$2, $zero, 0x1
  19638c: 26 68 ac 01  	xor	$13, $13, $12
  196390: 04 10 22 03  	sllv	$2, $2, $25
  196394: 90 00 23 8e  	lw	$3, 0x90($17)
  196398: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  19639c: 24 10 a2 01  	and	$2, $13, $2
  1963a0: 0d 00 43 10  	beq	$2, $3, 0x1963d8 <.text+0x963d8>
  1963a4: 80 10 10 00  	sll	$2, $16, 0x2
  1963a8: 21 10 5d 00  	addu	$2, $2, $sp
  1963ac: 90 00 44 24  	addiu	$4, $2, 0x90
  1963b0: 23 c8 2f 03  	subu	$25, $25, $15
  1963b4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1963b8: 04 10 22 03  	sllv	$2, $2, $25
  1963bc: fc ff 84 24  	addiu	$4, $4, -0x4 <.text+0xffffffffffeffffc>
  1963c0: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  1963c4: 00 00 83 8c  	lw	$3, 0x0($4)
  1963c8: 24 10 a2 01  	and	$2, $13, $2
  1963cc: fc ff 31 26  	addiu	$17, $17, -0x4 <.text+0xffffffffffeffffc>
  1963d0: f7 ff 43 14  	bne	$2, $3, 0x1963b0 <.text+0x963b0>
  1963d4: ff ff 10 26  	addiu	$16, $16, -0x1 <.text+0xffffffffffefffff>
  1963d8: ff ff c6 24  	addiu	$6, $6, -0x1 <.text+0xffffffffffefffff>
  1963dc: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1963e0: 79 ff c2 14  	bne	$6, $2, 0x1961c8 <.text+0x961c8>
  1963e4: 21 c0 2f 03  	addu	$24, $25, $15
  1963e8: 01 00 e7 24  	addiu	$7, $7, 0x1
  1963ec: 2a 10 67 01  	slt	$2, $11, $7
  1963f0: 6d ff 40 50  	beqzl	$2, 0x1961a8 <.text+0x961a8>
  1963f4: 80 a0 07 00  	sll	$20, $7, 0x2
  1963f8: 04 00 00 11  	beqz	$8, 0x19640c <.text+0x9640c>
  1963fc: 2d 18 00 00  	move	$3, $zero
  196400: 01 00 02 24  	addiu	$2, $zero, 0x1
  196404: 0d 00 62 11  	beq	$11, $2, 0x19643c <.text+0x9643c>
  196408: fb ff 03 24  	addiu	$3, $zero, -0x5 <.text+0xffffffffffeffffb>
  19640c: 80 01 be df  	ld	$fp, 0x180($sp)
  196410: 2d 10 60 00  	move	$2, $3
  196414: 70 01 b7 df  	ld	$23, 0x170($sp)
  196418: 60 01 b6 df  	ld	$22, 0x160($sp)
  19641c: 50 01 b5 df  	ld	$21, 0x150($sp)
  196420: 40 01 b4 df  	ld	$20, 0x140($sp)
  196424: 30 01 b3 df  	ld	$19, 0x130($sp)
  196428: 20 01 b2 df  	ld	$18, 0x120($sp)
  19642c: 10 01 b1 df  	ld	$17, 0x110($sp)
  196430: 00 01 b0 df  	ld	$16, 0x100($sp)
  196434: 08 00 e0 03  	jr	$ra
  196438: 90 01 bd 27  	addiu	$sp, $sp, 0x190
  19643c: f3 ff 00 10  	b	0x19640c <.text+0x9640c>
  196440: 2d 18 00 00  	move	$3, $zero
  196444: 00 00 c3 8d  	lw	$3, 0x0($14)
  196448: 2b 10 73 00  	sltu	$2, $3, $19
  19644c: 0a 00 40 10  	beqz	$2, 0x196478 <.text+0x96478>
  196450: 23 10 73 00  	subu	$2, $3, $19
  196454: 00 01 62 2c  	sltiu	$2, $3, 0x100
  196458: 02 00 40 10  	beqz	$2, 0x196464 <.text+0x96464>
  19645c: 60 00 03 24  	addiu	$3, $zero, 0x60
  196460: 2d 18 00 00  	move	$3, $zero
  196464: 40 00 a3 a3  	sb	$3, 0x40($sp)
  196468: 00 00 c2 8d  	lw	$2, 0x0($14)
  19646c: 04 00 ce 25  	addiu	$14, $14, 0x4
  196470: a3 ff 00 10  	b	0x196300 <.text+0x96300>
  196474: 44 00 a2 af  	sw	$2, 0x44($sp)
  196478: d0 00 b8 8f  	lw	$24, 0xd0($sp)
  19647c: 80 10 02 00  	sll	$2, $2, 0x2
  196480: 21 10 5e 00  	addu	$2, $2, $fp
  196484: 00 00 42 90  	lbu	$2, 0x0($2)
  196488: 50 00 42 24  	addiu	$2, $2, 0x50
  19648c: 40 00 a2 a3  	sb	$2, 0x40($sp)
  196490: 00 00 c2 8d  	lw	$2, 0x0($14)
  196494: 04 00 ce 25  	addiu	$14, $14, 0x4
  196498: 23 10 53 00  	subu	$2, $2, $19
  19649c: 80 10 02 00  	sll	$2, $2, 0x2
  1964a0: 21 10 58 00  	addu	$2, $2, $24
  1964a4: f2 ff 00 10  	b	0x196470 <.text+0x96470>
  1964a8: 00 00 42 8c  	lw	$2, 0x0($2)
  1964ac: 88 ff 00 10  	b	0x1962d0 <.text+0x962d0>
  1964b0: 00 00 ca ae  	sw	$10, 0x0($22)
  1964b4: d5 ff 00 10  	b	0x19640c <.text+0x9640c>
  1964b8: fc ff 03 24  	addiu	$3, $zero, -0x4 <.text+0xffffffffffeffffc>
  1964bc: d3 ff 00 10  	b	0x19640c <.text+0x9640c>
  1964c0: fd ff 03 24  	addiu	$3, $zero, -0x3 <.text+0xffffffffffeffffd>
  1964c4: 2d 18 00 00  	move	$3, $zero
  1964c8: 00 00 c0 ae  	sw	$zero, 0x0($22)
  1964cc: cf ff 00 10  	b	0x19640c <.text+0x9640c>
  1964d0: 00 00 40 ad  	sw	$zero, 0x0($10)
  1964d4: 70 ff bd 27  	addiu	$sp, $sp, -0x90 <.text+0xffffffffffefff70>
  1964d8: 70 00 b5 ff  	sd	$21, 0x70($sp)
  1964dc: 2d a8 a0 00  	move	$21, $5
  1964e0: 60 00 b4 ff  	sd	$20, 0x60($sp)
  1964e4: 13 00 05 24  	addiu	$5, $zero, 0x13
  1964e8: 50 00 b3 ff  	sd	$19, 0x50($sp)
  1964ec: 2d a0 80 00  	move	$20, $4
  1964f0: 40 00 b2 ff  	sd	$18, 0x40($sp)
  1964f4: 2d 98 c0 00  	move	$19, $6
  1964f8: 30 00 b1 ff  	sd	$17, 0x30($sp)
  1964fc: 04 00 06 24  	addiu	$6, $zero, 0x4
  196500: 20 00 b0 ff  	sd	$16, 0x20($sp)
  196504: 2d 88 00 01  	move	$17, $8
  196508: 80 00 bf ff  	sd	$ra, 0x80($sp)
  19650c: 2d 80 e0 00  	move	$16, $7
  196510: 10 00 a0 af  	sw	$zero, 0x10($sp)
  196514: 28 00 02 8d  	lw	$2, 0x28($8)
  196518: 09 f8 40 00  	jalr	$2
  19651c: 30 00 04 8d  	lw	$4, 0x30($8)
  196520: 2d 90 40 00  	move	$18, $2
  196524: 20 00 40 12  	beqz	$18, 0x1965a8 <.text+0x965a8>
  196528: fc ff 02 24  	addiu	$2, $zero, -0x4 <.text+0xffffffffffeffffc>
  19652c: 2d 58 00 02  	move	$11, $16
  196530: 10 00 a2 27  	addiu	$2, $sp, 0x10
  196534: 2d 20 80 02  	move	$4, $20
  196538: 2d 48 60 02  	move	$9, $19
  19653c: 13 00 05 24  	addiu	$5, $zero, 0x13
  196540: 13 00 06 24  	addiu	$6, $zero, 0x13
  196544: 2d 38 00 00  	move	$7, $zero
  196548: 2d 40 00 00  	move	$8, $zero
  19654c: 2d 50 a0 02  	move	$10, $21
  196550: 08 00 b2 af  	sw	$18, 0x8($sp)
  196554: e0 57 06 0c  	jal	0x195f80 <.text+0x95f80>
  196558: 00 00 a2 af  	sw	$2, 0x0($sp)
  19655c: 2d 80 40 00  	move	$16, $2
  196560: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  196564: 19 00 02 12  	beq	$16, $2, 0x1965cc <.text+0x965cc>
  196568: 1c 00 02 3c  	lui	$2, 0x1c
  19656c: fb ff 02 24  	addiu	$2, $zero, -0x5 <.text+0xffffffffffeffffb>
  196570: 05 00 02 12  	beq	$16, $2, 0x196588 <.text+0x96588>
  196574: 1c 00 02 3c  	lui	$2, 0x1c
  196578: 00 00 a2 8e  	lw	$2, 0x0($21)
  19657c: 06 00 40 54  	bnezl	$2, 0x196598 <.text+0x96598>
  196580: 2c 00 22 8e  	lw	$2, 0x2c($17)
  196584: 1c 00 02 3c  	lui	$2, 0x1c
  196588: fd ff 10 24  	addiu	$16, $zero, -0x3 <.text+0xffffffffffeffffd>
  19658c: 90 96 42 24  	addiu	$2, $2, -0x6970 <.text+0xffffffffffef9690>
  196590: 20 00 22 ae  	sw	$2, 0x20($17)
  196594: 2c 00 22 8e  	lw	$2, 0x2c($17)
  196598: 2d 28 40 02  	move	$5, $18
  19659c: 09 f8 40 00  	jalr	$2
  1965a0: 30 00 24 8e  	lw	$4, 0x30($17)
  1965a4: 2d 10 00 02  	move	$2, $16
  1965a8: 80 00 bf df  	ld	$ra, 0x80($sp)
  1965ac: 70 00 b5 df  	ld	$21, 0x70($sp)
  1965b0: 60 00 b4 df  	ld	$20, 0x60($sp)
  1965b4: 50 00 b3 df  	ld	$19, 0x50($sp)
  1965b8: 40 00 b2 df  	ld	$18, 0x40($sp)
  1965bc: 30 00 b1 df  	ld	$17, 0x30($sp)
  1965c0: 20 00 b0 df  	ld	$16, 0x20($sp)
  1965c4: 08 00 e0 03  	jr	$ra
  1965c8: 90 00 bd 27  	addiu	$sp, $sp, 0x90
  1965cc: f0 ff 00 10  	b	0x196590 <.text+0x96590>
  1965d0: 68 96 42 24  	addiu	$2, $2, -0x6998 <.text+0xffffffffffef9668>
  1965d4: 40 ff bd 27  	addiu	$sp, $sp, -0xc0 <.text+0xffffffffffefff40>
  1965d8: 90 00 b7 ff  	sd	$23, 0x90($sp)
  1965dc: 2d b8 00 01  	move	$23, $8
  1965e0: 80 00 b6 ff  	sd	$22, 0x80($sp)
  1965e4: 2d b0 60 01  	move	$22, $11
  1965e8: 70 00 b5 ff  	sd	$21, 0x70($sp)
  1965ec: 2d a8 c0 00  	move	$21, $6
  1965f0: 60 00 b4 ff  	sd	$20, 0x60($sp)
  1965f4: 04 00 06 24  	addiu	$6, $zero, 0x4
  1965f8: 50 00 b3 ff  	sd	$19, 0x50($sp)
  1965fc: 2d a0 e0 00  	move	$20, $7
  196600: 40 00 b2 ff  	sd	$18, 0x40($sp)
  196604: 2d 98 80 00  	move	$19, $4
  196608: 30 00 b1 ff  	sd	$17, 0x30($sp)
  19660c: 20 00 b0 ff  	sd	$16, 0x20($sp)
  196610: 2d 80 20 01  	move	$16, $9
  196614: b0 00 bf ff  	sd	$ra, 0xb0($sp)
  196618: a0 00 be ff  	sd	$fp, 0xa0($sp)
  19661c: c0 00 b1 8f  	lw	$17, 0xc0($sp)
  196620: 14 00 a5 af  	sw	$5, 0x14($sp)
  196624: 20 01 05 24  	addiu	$5, $zero, 0x120
  196628: 10 00 a0 af  	sw	$zero, 0x10($sp)
  19662c: 28 00 22 8e  	lw	$2, 0x28($17)
  196630: 30 00 24 8e  	lw	$4, 0x30($17)
  196634: 09 f8 40 00  	jalr	$2
  196638: 18 00 aa af  	sw	$10, 0x18($sp)
  19663c: 2d 90 40 00  	move	$18, $2
  196640: 22 00 40 12  	beqz	$18, 0x1966cc <.text+0x966cc>
  196644: fc ff 02 24  	addiu	$2, $zero, -0x4 <.text+0xffffffffffeffffc>
  196648: 1c 00 07 3c  	lui	$7, 0x1c
  19664c: 1c 00 08 3c  	lui	$8, 0x1c
  196650: 2d 48 00 02  	move	$9, $16
  196654: 78 94 e7 24  	addiu	$7, $7, -0x6b88 <.text+0xffffffffffef9478>
  196658: f8 94 08 25  	addiu	$8, $8, -0x6b08 <.text+0xffffffffffef94f8>
  19665c: 10 00 be 27  	addiu	$fp, $sp, 0x10
  196660: 2d 20 a0 02  	move	$4, $21
  196664: 2d 28 60 02  	move	$5, $19
  196668: 01 01 06 24  	addiu	$6, $zero, 0x101
  19666c: 2d 50 80 02  	move	$10, $20
  196670: 2d 58 c0 02  	move	$11, $22
  196674: 00 00 be af  	sw	$fp, 0x0($sp)
  196678: e0 57 06 0c  	jal	0x195f80 <.text+0x95f80>
  19667c: 08 00 b2 af  	sw	$18, 0x8($sp)
  196680: 04 00 40 14  	bnez	$2, 0x196694 <.text+0x96694>
  196684: 2d 80 40 00  	move	$16, $2
  196688: 00 00 82 8e  	lw	$2, 0x0($20)
  19668c: 1d 00 40 14  	bnez	$2, 0x196704 <.text+0x96704>
  196690: 80 20 13 00  	sll	$4, $19, 0x2
  196694: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  196698: 18 00 02 12  	beq	$16, $2, 0x1966fc <.text+0x966fc>
  19669c: 1c 00 02 3c  	lui	$2, 0x1c
  1966a0: fc ff 02 24  	addiu	$2, $zero, -0x4 <.text+0xffffffffffeffffc>
  1966a4: 04 00 02 12  	beq	$16, $2, 0x1966b8 <.text+0x966b8>
  1966a8: 1c 00 02 3c  	lui	$2, 0x1c
  1966ac: fd ff 10 24  	addiu	$16, $zero, -0x3 <.text+0xffffffffffeffffd>
  1966b0: e0 96 42 24  	addiu	$2, $2, -0x6920 <.text+0xffffffffffef96e0>
  1966b4: 20 00 22 ae  	sw	$2, 0x20($17)
  1966b8: 2c 00 22 8e  	lw	$2, 0x2c($17)
  1966bc: 2d 28 40 02  	move	$5, $18
  1966c0: 09 f8 40 00  	jalr	$2
  1966c4: 30 00 24 8e  	lw	$4, 0x30($17)
  1966c8: 2d 10 00 02  	move	$2, $16
  1966cc: b0 00 bf df  	ld	$ra, 0xb0($sp)
  1966d0: a0 00 be df  	ld	$fp, 0xa0($sp)
  1966d4: 90 00 b7 df  	ld	$23, 0x90($sp)
  1966d8: 80 00 b6 df  	ld	$22, 0x80($sp)
  1966dc: 70 00 b5 df  	ld	$21, 0x70($sp)
  1966e0: 60 00 b4 df  	ld	$20, 0x60($sp)
  1966e4: 50 00 b3 df  	ld	$19, 0x50($sp)
  1966e8: 40 00 b2 df  	ld	$18, 0x40($sp)
  1966ec: 30 00 b1 df  	ld	$17, 0x30($sp)
  1966f0: 20 00 b0 df  	ld	$16, 0x20($sp)
  1966f4: 08 00 e0 03  	jr	$ra
  1966f8: c0 00 bd 27  	addiu	$sp, $sp, 0xc0
  1966fc: ed ff 00 10  	b	0x1966b4 <.text+0x966b4>
  196700: b8 96 42 24  	addiu	$2, $2, -0x6948 <.text+0xffffffffffef96b8>
  196704: 14 00 a5 8f  	lw	$5, 0x14($sp)
  196708: 1c 00 07 3c  	lui	$7, 0x1c
  19670c: 1c 00 08 3c  	lui	$8, 0x1c
  196710: 18 00 a9 8f  	lw	$9, 0x18($sp)
  196714: 21 20 a4 02  	addu	$4, $21, $4
  196718: 78 95 e7 24  	addiu	$7, $7, -0x6a88 <.text+0xffffffffffef9578>
  19671c: f0 95 08 25  	addiu	$8, $8, -0x6a10 <.text+0xffffffffffef95f0>
  196720: 2d 58 c0 02  	move	$11, $22
  196724: 2d 30 00 00  	move	$6, $zero
  196728: 2d 50 e0 02  	move	$10, $23
  19672c: 00 00 be af  	sw	$fp, 0x0($sp)
  196730: e0 57 06 0c  	jal	0x195f80 <.text+0x95f80>
  196734: 08 00 b2 af  	sw	$18, 0x8($sp)
  196738: 07 00 40 14  	bnez	$2, 0x196758 <.text+0x96758>
  19673c: 2d 80 40 00  	move	$16, $2
  196740: 00 00 e2 8e  	lw	$2, 0x0($23)
  196744: 15 00 40 54  	bnezl	$2, 0x19679c <.text+0x9679c>
  196748: 2c 00 22 8e  	lw	$2, 0x2c($17)
  19674c: 02 01 62 2e  	sltiu	$2, $19, 0x102
  196750: 12 00 40 54  	bnezl	$2, 0x19679c <.text+0x9679c>
  196754: 2c 00 22 8e  	lw	$2, 0x2c($17)
  196758: fd ff 02 24  	addiu	$2, $zero, -0x3 <.text+0xffffffffffeffffd>
  19675c: 0d 00 02 12  	beq	$16, $2, 0x196794 <.text+0x96794>
  196760: 1c 00 02 3c  	lui	$2, 0x1c
  196764: fb ff 02 24  	addiu	$2, $zero, -0x5 <.text+0xffffffffffeffffb>
  196768: 07 00 02 12  	beq	$16, $2, 0x196788 <.text+0x96788>
  19676c: 1c 00 02 3c  	lui	$2, 0x1c
  196770: fc ff 02 24  	addiu	$2, $zero, -0x4 <.text+0xffffffffffeffffc>
  196774: d0 ff 02 12  	beq	$16, $2, 0x1966b8 <.text+0x966b8>
  196778: 1c 00 02 3c  	lui	$2, 0x1c
  19677c: fd ff 10 24  	addiu	$16, $zero, -0x3 <.text+0xffffffffffeffffd>
  196780: cc ff 00 10  	b	0x1966b4 <.text+0x966b4>
  196784: 40 97 42 24  	addiu	$2, $2, -0x68c0 <.text+0xffffffffffef9740>
  196788: fd ff 10 24  	addiu	$16, $zero, -0x3 <.text+0xffffffffffeffffd>
  19678c: c9 ff 00 10  	b	0x1966b4 <.text+0x966b4>
  196790: 20 97 42 24  	addiu	$2, $2, -0x68e0 <.text+0xffffffffffef9720>
  196794: c7 ff 00 10  	b	0x1966b4 <.text+0x966b4>
  196798: 00 97 42 24  	addiu	$2, $2, -0x6900 <.text+0xffffffffffef9700>
  19679c: 2d 28 40 02  	move	$5, $18
  1967a0: 09 f8 40 00  	jalr	$2
  1967a4: 30 00 24 8e  	lw	$4, 0x30($17)
  1967a8: c8 ff 00 10  	b	0x1966cc <.text+0x966cc>
  1967ac: 2d 10 00 00  	move	$2, $zero
  1967b0: 42 00 02 3c  	lui	$2, 0x42
  1967b4: 68 48 42 8c  	lw	$2, 0x4868($2)
  1967b8: 00 00 82 ac  	sw	$2, 0x0($4)
  1967bc: 42 00 02 3c  	lui	$2, 0x42
  1967c0: 6c 48 43 8c  	lw	$3, 0x486c($2)
  1967c4: 42 00 02 3c  	lui	$2, 0x42
  1967c8: 70 48 42 24  	addiu	$2, $2, 0x4870
  1967cc: 00 00 a3 ac  	sw	$3, 0x0($5)
  1967d0: 42 00 03 3c  	lui	$3, 0x42
  1967d4: 70 58 63 24  	addiu	$3, $3, 0x5870
  1967d8: 00 00 c2 ac  	sw	$2, 0x0($6)
  1967dc: 2d 10 00 00  	move	$2, $zero
  1967e0: 08 00 e0 03  	jr	$ra
  1967e4: 00 00 e3 ac  	sw	$3, 0x0($7)
  1967e8: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  1967ec: 50 00 b5 ff  	sd	$21, 0x50($sp)
  1967f0: 2d a8 c0 00  	move	$21, $6
  1967f4: 30 00 b3 ff  	sd	$19, 0x30($sp)
  1967f8: 2d 98 a0 00  	move	$19, $5
  1967fc: 20 00 b2 ff  	sd	$18, 0x20($sp)
  196800: 2d 90 80 00  	move	$18, $4
  196804: 00 00 b0 ff  	sd	$16, 0x0($sp)
  196808: 60 00 bf ff  	sd	$ra, 0x60($sp)
  19680c: 40 00 b4 ff  	sd	$20, 0x40($sp)
  196810: 10 00 b1 ff  	sd	$17, 0x10($sp)
  196814: 34 00 91 8c  	lw	$17, 0x34($4)
  196818: 38 00 82 8c  	lw	$2, 0x38($4)
  19681c: 10 00 b4 8c  	lw	$20, 0x10($5)
  196820: 2b 18 51 00  	sltu	$3, $2, $17
  196824: 03 00 60 10  	beqz	$3, 0x196834 <.text+0x96834>
  196828: 23 80 51 00  	subu	$16, $2, $17
  19682c: 30 00 82 8c  	lw	$2, 0x30($4)
  196830: 23 80 51 00  	subu	$16, $2, $17
  196834: 14 00 64 8e  	lw	$4, 0x14($19)
  196838: 2b 10 90 00  	sltu	$2, $4, $16
  19683c: 0b 80 82 00  	movn	$16, $4, $2
  196840: 05 00 00 52  	beqzl	$16, 0x196858 <.text+0x96858>
  196844: 18 00 62 de  	ld	$2, 0x18($19)
  196848: fb ff 02 24  	addiu	$2, $zero, -0x5 <.text+0xffffffffffeffffb>
  19684c: 26 10 a2 02  	xor	$2, $21, $2
  196850: 0a a8 02 00  	movz	$21, $zero, $2
  196854: 18 00 62 de  	ld	$2, 0x18($19)
  196858: 3c 18 10 00  	dsll32	$3, $16, 0x0
  19685c: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  196860: 3c 00 47 8e  	lw	$7, 0x3c($18)
  196864: 2d 10 43 00  	daddu	$2, $2, $3
  196868: 23 18 90 00  	subu	$3, $4, $16
  19686c: 14 00 63 ae  	sw	$3, 0x14($19)
  196870: 3c 00 e0 14  	bnez	$7, 0x196964 <.text+0x96964>
  196874: 18 00 62 fe  	sd	$2, 0x18($19)
  196878: 2d 20 80 02  	move	$4, $20
  19687c: 2d 28 20 02  	move	$5, $17
  196880: 2d 30 00 02  	move	$6, $16
  196884: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  196888: 21 88 30 02  	addu	$17, $17, $16
  19688c: 30 00 43 8e  	lw	$3, 0x30($18)
  196890: 0d 00 23 12  	beq	$17, $3, 0x1968c8 <.text+0x968c8>
  196894: 21 a0 90 02  	addu	$20, $20, $16
  196898: 10 00 74 ae  	sw	$20, 0x10($19)
  19689c: 2d 10 a0 02  	move	$2, $21
  1968a0: 34 00 51 ae  	sw	$17, 0x34($18)
  1968a4: 60 00 bf df  	ld	$ra, 0x60($sp)
  1968a8: 50 00 b5 df  	ld	$21, 0x50($sp)
  1968ac: 40 00 b4 df  	ld	$20, 0x40($sp)
  1968b0: 30 00 b3 df  	ld	$19, 0x30($sp)
  1968b4: 20 00 b2 df  	ld	$18, 0x20($sp)
  1968b8: 10 00 b1 df  	ld	$17, 0x10($sp)
  1968bc: 00 00 b0 df  	ld	$16, 0x0($sp)
  1968c0: 08 00 e0 03  	jr	$ra
  1968c4: 70 00 bd 27  	addiu	$sp, $sp, 0x70
  1968c8: 38 00 42 8e  	lw	$2, 0x38($18)
  1968cc: 23 00 43 10  	beq	$2, $3, 0x19695c <.text+0x9695c>
  1968d0: 2c 00 51 8e  	lw	$17, 0x2c($18)
  1968d4: 38 00 42 8e  	lw	$2, 0x38($18)
  1968d8: 14 00 64 8e  	lw	$4, 0x14($19)
  1968dc: 23 80 51 00  	subu	$16, $2, $17
  1968e0: 2b 10 90 00  	sltu	$2, $4, $16
  1968e4: 0b 80 82 00  	movn	$16, $4, $2
  1968e8: 05 00 00 52  	beqzl	$16, 0x196900 <.text+0x96900>
  1968ec: 18 00 62 de  	ld	$2, 0x18($19)
  1968f0: fb ff 02 24  	addiu	$2, $zero, -0x5 <.text+0xffffffffffeffffb>
  1968f4: 26 10 a2 02  	xor	$2, $21, $2
  1968f8: 0a a8 02 00  	movz	$21, $zero, $2
  1968fc: 18 00 62 de  	ld	$2, 0x18($19)
  196900: 3c 18 10 00  	dsll32	$3, $16, 0x0
  196904: 3e 18 03 00  	dsrl32	$3, $3, 0x0
  196908: 3c 00 47 8e  	lw	$7, 0x3c($18)
  19690c: 2d 10 43 00  	daddu	$2, $2, $3
  196910: 23 18 90 00  	subu	$3, $4, $16
  196914: 14 00 63 ae  	sw	$3, 0x14($19)
  196918: 09 00 e0 14  	bnez	$7, 0x196940 <.text+0x96940>
  19691c: 18 00 62 fe  	sd	$2, 0x18($19)
  196920: 2d 20 80 02  	move	$4, $20
  196924: 2d 28 20 02  	move	$5, $17
  196928: 2d 30 00 02  	move	$6, $16
  19692c: 21 a0 90 02  	addu	$20, $20, $16
  196930: d9 70 06 0c  	jal	0x19c364 <.text+0x9c364>
  196934: 21 88 30 02  	addu	$17, $17, $16
  196938: d8 ff 00 10  	b	0x19689c <.text+0x9689c>
  19693c: 10 00 74 ae  	sw	$20, 0x10($19)
  196940: 40 00 44 de  	ld	$4, 0x40($18)
  196944: 2d 28 20 02  	move	$5, $17
  196948: 09 f8 e0 00  	jalr	$7
  19694c: 2d 30 00 02  	move	$6, $16
  196950: 40 00 42 fe  	sd	$2, 0x40($18)
  196954: f2 ff 00 10  	b	0x196920 <.text+0x96920>
  196958: 38 00 62 fe  	sd	$2, 0x38($19)
  19695c: dd ff 00 10  	b	0x1968d4 <.text+0x968d4>
  196960: 38 00 51 ae  	sw	$17, 0x38($18)
  196964: 40 00 44 de  	ld	$4, 0x40($18)
  196968: 2d 28 20 02  	move	$5, $17
  19696c: 09 f8 e0 00  	jalr	$7
  196970: 2d 30 00 02  	move	$6, $16
  196974: 40 00 42 fe  	sd	$2, 0x40($18)
  196978: bf ff 00 10  	b	0x196878 <.text+0x96878>
  19697c: 38 00 62 fe  	sd	$2, 0x38($19)
  196980: 08 00 e0 03  	jr	$ra
  196984: 00 00 00 00  	nop
