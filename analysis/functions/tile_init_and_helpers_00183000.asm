
/mnt/data/snesdec_work/SNESStation-Decomp-Starter/build/SNES_EMU.analysis.elf:	file format elf32-mips

Disassembly of section .text:

00100000 <.text>:
  183000: 21 10 62 02  	addu	$2, $19, $2
  183004: bd ff 00 10  	b	0x182efc <.text+0x82efc>
  183008: c4 00 40 a0  	sb	$zero, 0xc4($2)
  18300c: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  183010: 34 00 02 3c  	lui	$2, 0x34
  183014: 30 00 bf ff  	sd	$ra, 0x30($sp)
  183018: 20 00 b2 ff  	sd	$18, 0x20($sp)
  18301c: 10 00 b1 ff  	sd	$17, 0x10($sp)
  183020: 00 00 b0 ff  	sd	$16, 0x0($sp)
  183024: 45 55 42 90  	lbu	$2, 0x5545($2)
  183028: 28 00 40 14  	bnez	$2, 0x1830cc <.text+0x830cc>
  18302c: 00 00 00 00  	nop
  183030: 2d 80 00 00  	move	$16, $zero
  183034: 41 00 12 3c  	lui	$18, 0x41
  183038: 80 88 10 00  	sll	$17, $16, 0x2
  18303c: 04 35 42 8e  	lw	$2, 0x3504($18)
  183040: 01 00 10 26  	addiu	$16, $16, 0x1
  183044: 21 10 22 02  	addu	$2, $17, $2
  183048: 00 00 42 8c  	lw	$2, 0x0($2)
  18304c: 19 00 40 14  	bnez	$2, 0x1830b4 <.text+0x830b4>
  183050: 2d 20 40 00  	move	$4, $2
  183054: 30 00 02 2a  	slti	$2, $16, 0x30
  183058: f7 ff 40 54  	bnezl	$2, 0x183038 <.text+0x83038>
  18305c: 41 00 12 3c  	lui	$18, 0x41
  183060: 34 00 02 3c  	lui	$2, 0x34
  183064: 04 35 44 8e  	lw	$4, 0x3504($18)
  183068: e0 54 42 24  	addiu	$2, $2, 0x54e0
  18306c: 66 00 40 a0  	sb	$zero, 0x66($2)
  183070: 0c 00 80 14  	bnez	$4, 0x1830a4 <.text+0x830a4>
  183074: 65 00 40 a0  	sb	$zero, 0x65($2)
  183078: 04 35 40 ae  	sw	$zero, 0x3504($18)
  18307c: 41 00 02 3c  	lui	$2, 0x41
  183080: 30 00 bf df  	ld	$ra, 0x30($sp)
  183084: 20 00 b2 df  	ld	$18, 0x20($sp)
  183088: 10 00 b1 df  	ld	$17, 0x10($sp)
  18308c: 00 00 b0 df  	ld	$16, 0x0($sp)
  183090: ec 34 40 ac  	sw	$zero, 0x34ec($2)
  183094: 41 00 02 3c  	lui	$2, 0x41
  183098: f4 34 40 ac  	sw	$zero, 0x34f4($2)
  18309c: 08 00 e0 03  	jr	$ra
  1830a0: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  1830a4: 3e a4 06 0c  	jal	0x1a90f8 <.text+0xa90f8>
  1830a8: 00 00 00 00  	nop
  1830ac: f3 ff 00 10  	b	0x18307c <.text+0x8307c>
  1830b0: 04 35 40 ae  	sw	$zero, 0x3504($18)
  1830b4: 46 a4 06 0c  	jal	0x1a9118 <.text+0xa9118>
  1830b8: 00 00 00 00  	nop
  1830bc: 04 35 42 8e  	lw	$2, 0x3504($18)
  1830c0: 21 10 22 02  	addu	$2, $17, $2
  1830c4: e3 ff 00 10  	b	0x183054 <.text+0x83054>
  1830c8: 00 00 40 ac  	sw	$zero, 0x0($2)
  1830cc: e7 0c 06 0c  	jal	0x18339c <.text+0x8339c>
  1830d0: 2d 80 00 00  	move	$16, $zero
  1830d4: d8 ff 00 10  	b	0x183038 <.text+0x83038>
  1830d8: 41 00 12 3c  	lui	$18, 0x41
  1830dc: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1830e0: 34 00 02 3c  	lui	$2, 0x34
  1830e4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1830e8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1830ec: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1830f0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1830f4: 45 55 42 90  	lbu	$2, 0x5545($2)
  1830f8: 28 00 40 14  	bnez	$2, 0x18319c <.text+0x8319c>
  1830fc: 00 00 00 00  	nop
  183100: 2d 80 00 00  	move	$16, $zero
  183104: 41 00 12 3c  	lui	$18, 0x41
  183108: 80 88 10 00  	sll	$17, $16, 0x2
  18310c: 04 35 42 8e  	lw	$2, 0x3504($18)
  183110: 01 00 10 26  	addiu	$16, $16, 0x1
  183114: 21 10 22 02  	addu	$2, $17, $2
  183118: 00 00 42 8c  	lw	$2, 0x0($2)
  18311c: 19 00 40 14  	bnez	$2, 0x183184 <.text+0x83184>
  183120: 2d 20 40 00  	move	$4, $2
  183124: 30 00 02 2a  	slti	$2, $16, 0x30
  183128: f7 ff 40 54  	bnezl	$2, 0x183108 <.text+0x83108>
  18312c: 41 00 12 3c  	lui	$18, 0x41
  183130: 34 00 02 3c  	lui	$2, 0x34
  183134: 04 35 44 8e  	lw	$4, 0x3504($18)
  183138: e0 54 42 24  	addiu	$2, $2, 0x54e0
  18313c: 66 00 40 a0  	sb	$zero, 0x66($2)
  183140: 0c 00 80 14  	bnez	$4, 0x183174 <.text+0x83174>
  183144: 65 00 40 a0  	sb	$zero, 0x65($2)
  183148: 04 35 40 ae  	sw	$zero, 0x3504($18)
  18314c: 41 00 02 3c  	lui	$2, 0x41
  183150: 30 00 bf df  	ld	$ra, 0x30($sp)
  183154: 20 00 b2 df  	ld	$18, 0x20($sp)
  183158: 10 00 b1 df  	ld	$17, 0x10($sp)
  18315c: 00 00 b0 df  	ld	$16, 0x0($sp)
  183160: ec 34 40 ac  	sw	$zero, 0x34ec($2)
  183164: 41 00 02 3c  	lui	$2, 0x41
  183168: f4 34 40 ac  	sw	$zero, 0x34f4($2)
  18316c: 08 00 e0 03  	jr	$ra
  183170: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  183174: 3e a4 06 0c  	jal	0x1a90f8 <.text+0xa90f8>
  183178: 00 00 00 00  	nop
  18317c: f3 ff 00 10  	b	0x18314c <.text+0x8314c>
  183180: 04 35 40 ae  	sw	$zero, 0x3504($18)
  183184: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  183188: 00 00 00 00  	nop
  18318c: 04 35 42 8e  	lw	$2, 0x3504($18)
  183190: 21 10 22 02  	addu	$2, $17, $2
  183194: e3 ff 00 10  	b	0x183124 <.text+0x83124>
  183198: 00 00 40 ac  	sw	$zero, 0x0($2)
  18319c: e7 0c 06 0c  	jal	0x18339c <.text+0x8339c>
  1831a0: 2d 80 00 00  	move	$16, $zero
  1831a4: d8 ff 00 10  	b	0x183108 <.text+0x83108>
  1831a8: 41 00 12 3c  	lui	$18, 0x41
  1831ac: c0 ff bd 27  	addiu	$sp, $sp, -0x40 <.text+0xffffffffffefffc0>
  1831b0: 34 00 02 3c  	lui	$2, 0x34
  1831b4: 30 00 bf ff  	sd	$ra, 0x30($sp)
  1831b8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1831bc: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1831c0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1831c4: 45 55 42 90  	lbu	$2, 0x5545($2)
  1831c8: 32 00 40 14  	bnez	$2, 0x183294 <.text+0x83294>
  1831cc: 00 00 00 00  	nop
  1831d0: 2d 90 00 00  	move	$18, $zero
  1831d4: 08 0c 03 24  	addiu	$3, $zero, 0xc08
  1831d8: 41 00 11 3c  	lui	$17, 0x41
  1831dc: 18 20 43 02  	<unknown>
  1831e0: 04 35 22 8e  	lw	$2, 0x3504($17)
  1831e4: 80 80 12 00  	sll	$16, $18, 0x2
  1831e8: 21 28 82 00  	addu	$5, $4, $2
  1831ec: 21 10 02 02  	addu	$2, $16, $2
  1831f0: 00 00 43 8c  	lw	$3, 0x0($2)
  1831f4: 0a 00 60 50  	beqzl	$3, 0x183220 <.text+0x83220>
  1831f8: 01 00 52 26  	addiu	$18, $18, 0x1
  1831fc: c4 00 a2 90  	lbu	$2, 0xc4($5)
  183200: 20 00 40 10  	beqz	$2, 0x183284 <.text+0x83284>
  183204: 2d 20 60 00  	move	$4, $3
  183208: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  18320c: 00 00 00 00  	nop
  183210: 04 35 22 8e  	lw	$2, 0x3504($17)
  183214: 21 10 02 02  	addu	$2, $16, $2
  183218: 00 00 40 ac  	sw	$zero, 0x0($2)
  18321c: 01 00 52 26  	addiu	$18, $18, 0x1
  183220: 30 00 42 2a  	slti	$2, $18, 0x30
  183224: ec ff 40 14  	bnez	$2, 0x1831d8 <.text+0x831d8>
  183228: 08 0c 03 24  	addiu	$3, $zero, 0xc08
  18322c: 41 00 10 3c  	lui	$16, 0x41
  183230: 34 00 02 3c  	lui	$2, 0x34
  183234: 04 35 04 8e  	lw	$4, 0x3504($16)
  183238: e0 54 42 24  	addiu	$2, $2, 0x54e0
  18323c: 66 00 40 a0  	sb	$zero, 0x66($2)
  183240: 0c 00 80 14  	bnez	$4, 0x183274 <.text+0x83274>
  183244: 65 00 40 a0  	sb	$zero, 0x65($2)
  183248: 04 35 00 ae  	sw	$zero, 0x3504($16)
  18324c: 41 00 02 3c  	lui	$2, 0x41
  183250: 30 00 bf df  	ld	$ra, 0x30($sp)
  183254: 20 00 b2 df  	ld	$18, 0x20($sp)
  183258: 10 00 b1 df  	ld	$17, 0x10($sp)
  18325c: 00 00 b0 df  	ld	$16, 0x0($sp)
  183260: ec 34 40 ac  	sw	$zero, 0x34ec($2)
  183264: 41 00 02 3c  	lui	$2, 0x41
  183268: f4 34 40 ac  	sw	$zero, 0x34f4($2)
  18326c: 08 00 e0 03  	jr	$ra
  183270: 40 00 bd 27  	addiu	$sp, $sp, 0x40
  183274: 3e a4 06 0c  	jal	0x1a90f8 <.text+0xa90f8>
  183278: 00 00 00 00  	nop
  18327c: f3 ff 00 10  	b	0x18324c <.text+0x8324c>
  183280: 04 35 00 ae  	sw	$zero, 0x3504($16)
  183284: 46 a4 06 0c  	jal	0x1a9118 <.text+0xa9118>
  183288: 2d 20 60 00  	move	$4, $3
  18328c: e1 ff 00 10  	b	0x183214 <.text+0x83214>
  183290: 04 35 22 8e  	lw	$2, 0x3504($17)
  183294: e7 0c 06 0c  	jal	0x18339c <.text+0x8339c>
  183298: 2d 90 00 00  	move	$18, $zero
  18329c: ce ff 00 10  	b	0x1831d8 <.text+0x831d8>
  1832a0: 08 0c 03 24  	addiu	$3, $zero, 0xc08
  1832a4: 41 00 03 3c  	lui	$3, 0x41
  1832a8: f0 ff bd 27  	addiu	$sp, $sp, -0x10 <.text+0xffffffffffeffff0>
  1832ac: 08 35 62 24  	addiu	$2, $3, 0x3508
  1832b0: 01 00 07 24  	addiu	$7, $zero, 0x1
  1832b4: 08 35 60 a0  	sb	$zero, 0x3508($3)
  1832b8: 3c 00 44 24  	addiu	$4, $2, 0x3c
  1832bc: 02 00 03 24  	addiu	$3, $zero, 0x2
  1832c0: 00 00 bf ff  	sd	$ra, 0x0($sp)
  1832c4: 01 00 40 a0  	sb	$zero, 0x1($2)
  1832c8: 2d 28 00 00  	move	$5, $zero
  1832cc: 02 00 40 a0  	sb	$zero, 0x2($2)
  1832d0: 01 00 06 3c  	lui	$6, 0x1
  1832d4: 03 00 40 a0  	sb	$zero, 0x3($2)
  1832d8: 04 00 40 a0  	sb	$zero, 0x4($2)
  1832dc: 05 00 40 a0  	sb	$zero, 0x5($2)
  1832e0: 06 00 40 a0  	sb	$zero, 0x6($2)
  1832e4: 07 00 40 a0  	sb	$zero, 0x7($2)
  1832e8: 08 00 40 a0  	sb	$zero, 0x8($2)
  1832ec: 09 00 40 a0  	sb	$zero, 0x9($2)
  1832f0: 0a 00 40 a0  	sb	$zero, 0xa($2)
  1832f4: 0b 00 40 a0  	sb	$zero, 0xb($2)
  1832f8: 0c 00 40 a0  	sb	$zero, 0xc($2)
  1832fc: 0d 00 40 a0  	sb	$zero, 0xd($2)
  183300: 0e 00 40 a0  	sb	$zero, 0xe($2)
  183304: 0f 00 40 a0  	sb	$zero, 0xf($2)
  183308: 10 00 40 a0  	sb	$zero, 0x10($2)
  18330c: 11 00 40 a0  	sb	$zero, 0x11($2)
  183310: 12 00 40 a0  	sb	$zero, 0x12($2)
  183314: 13 00 40 a0  	sb	$zero, 0x13($2)
  183318: 14 00 40 a0  	sb	$zero, 0x14($2)
  18331c: 15 00 40 a0  	sb	$zero, 0x15($2)
  183320: 16 00 40 a0  	sb	$zero, 0x16($2)
  183324: 17 00 40 a0  	sb	$zero, 0x17($2)
  183328: 18 00 40 a0  	sb	$zero, 0x18($2)
  18332c: 19 00 40 a0  	sb	$zero, 0x19($2)
  183330: 1a 00 40 a0  	sb	$zero, 0x1a($2)
  183334: 1b 00 40 a0  	sb	$zero, 0x1b($2)
  183338: 1c 00 40 a0  	sb	$zero, 0x1c($2)
  18333c: 1d 00 40 a0  	sb	$zero, 0x1d($2)
  183340: 1e 00 40 a0  	sb	$zero, 0x1e($2)
  183344: 1f 00 40 a0  	sb	$zero, 0x1f($2)
  183348: 20 00 40 a0  	sb	$zero, 0x20($2)
  18334c: 28 00 43 a0  	sb	$3, 0x28($2)
  183350: 2d 00 47 a0  	sb	$7, 0x2d($2)
  183354: 27 00 47 a0  	sb	$7, 0x27($2)
  183358: 21 00 40 a0  	sb	$zero, 0x21($2)
  18335c: 22 00 40 a0  	sb	$zero, 0x22($2)
  183360: 23 00 40 a0  	sb	$zero, 0x23($2)
  183364: 24 00 40 a0  	sb	$zero, 0x24($2)
  183368: 25 00 40 a0  	sb	$zero, 0x25($2)
  18336c: 26 00 40 a0  	sb	$zero, 0x26($2)
  183370: 29 00 40 a0  	sb	$zero, 0x29($2)
  183374: 2a 00 40 a0  	sb	$zero, 0x2a($2)
  183378: 2b 00 40 a0  	sb	$zero, 0x2b($2)
  18337c: 2c 00 40 a0  	sb	$zero, 0x2c($2)
  183380: 2e 00 40 a0  	sb	$zero, 0x2e($2)
  183384: 2f 00 40 a0  	sb	$zero, 0x2f($2)
  183388: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18338c: 38 00 40 ac  	sw	$zero, 0x38($2)
  183390: 00 00 bf df  	ld	$ra, 0x0($sp)
  183394: 08 00 e0 03  	jr	$ra
  183398: 10 00 bd 27  	addiu	$sp, $sp, 0x10
  18339c: 08 00 e0 03  	jr	$ra
  1833a0: 00 00 00 00  	nop
  1833a4: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1833a8: 30 00 b2 ff  	sd	$18, 0x30($sp)
  1833ac: 2d 90 80 00  	move	$18, $4
  1833b0: 1c 00 04 3c  	lui	$4, 0x1c
  1833b4: 40 00 bf ff  	sd	$ra, 0x40($sp)
  1833b8: 70 88 84 24  	addiu	$4, $4, -0x7790 <.text+0xffffffffffef8870>
  1833bc: 10 00 b0 ff  	sd	$16, 0x10($sp)
  1833c0: 49 06 04 0c  	jal	0x101924 <.text+0x1924>
  1833c4: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1833c8: 2d 20 40 00  	move	$4, $2
  1833cc: f0 73 06 0c  	jal	0x19cfc0 <.text+0x9cfc0>
  1833d0: 02 02 05 24  	addiu	$5, $zero, 0x202
  1833d4: 2d 80 40 00  	move	$16, $2
  1833d8: 3f 00 00 06  	bltz	$16, 0x1834d8 <.text+0x834d8>
  1833dc: 2d 10 00 00  	move	$2, $zero
  1833e0: 00 00 a0 a3  	sb	$zero, 0x0($sp)
  1833e4: 2d 88 00 00  	move	$17, $zero
  1833e8: 21 28 51 02  	addu	$5, $18, $17
  1833ec: 2d 20 00 02  	move	$4, $16
  1833f0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1833f4: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  1833f8: 01 00 31 26  	addiu	$17, $17, 0x1
  1833fc: 10 00 22 2a  	slti	$2, $17, 0x10
  183400: fa ff 40 14  	bnez	$2, 0x1833ec <.text+0x833ec>
  183404: 21 28 51 02  	addu	$5, $18, $17
  183408: 10 00 42 92  	lbu	$2, 0x10($18)
  18340c: 2d 20 00 02  	move	$4, $16
  183410: 2d 28 a0 03  	move	$5, $sp
  183414: 01 00 06 24  	addiu	$6, $zero, 0x1
  183418: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  18341c: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183420: 11 00 42 82  	lb	$2, 0x11($18)
  183424: 2d 20 00 02  	move	$4, $16
  183428: 2d 28 a0 03  	move	$5, $sp
  18342c: 01 00 06 24  	addiu	$6, $zero, 0x1
  183430: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  183434: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183438: 12 00 42 92  	lbu	$2, 0x12($18)
  18343c: 2d 20 00 02  	move	$4, $16
  183440: 2d 28 a0 03  	move	$5, $sp
  183444: 01 00 06 24  	addiu	$6, $zero, 0x1
  183448: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  18344c: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183450: 13 00 42 92  	lbu	$2, 0x13($18)
  183454: 2d 20 00 02  	move	$4, $16
  183458: 2d 28 a0 03  	move	$5, $sp
  18345c: 01 00 06 24  	addiu	$6, $zero, 0x1
  183460: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  183464: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183468: 14 00 42 92  	lbu	$2, 0x14($18)
  18346c: 2d 20 00 02  	move	$4, $16
  183470: 2d 28 a0 03  	move	$5, $sp
  183474: 01 00 06 24  	addiu	$6, $zero, 0x1
  183478: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  18347c: 00 00 a2 a3  	sb	$2, 0x0($sp)
  183480: 14 00 42 8e  	lw	$2, 0x14($18)
  183484: 2d 20 00 02  	move	$4, $16
  183488: 2d 28 a0 03  	move	$5, $sp
  18348c: 01 00 06 24  	addiu	$6, $zero, 0x1
  183490: 03 12 02 00  	sra	$2, $2, 0x8
  183494: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  183498: 00 00 a2 a3  	sb	$2, 0x0($sp)
  18349c: 16 00 42 86  	lh	$2, 0x16($18)
  1834a0: 2d 20 00 02  	move	$4, $16
  1834a4: 2d 28 a0 03  	move	$5, $sp
  1834a8: 01 00 06 24  	addiu	$6, $zero, 0x1
  1834ac: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  1834b0: 00 00 a2 a3  	sb	$2, 0x0($sp)
  1834b4: 17 00 42 82  	lb	$2, 0x17($18)
  1834b8: 2d 28 a0 03  	move	$5, $sp
  1834bc: 01 00 06 24  	addiu	$6, $zero, 0x1
  1834c0: 2d 20 00 02  	move	$4, $16
  1834c4: 91 74 06 0c  	jal	0x19d244 <.text+0x9d244>
  1834c8: 00 00 a2 a3  	sb	$2, 0x0($sp)
  1834cc: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  1834d0: 2d 20 00 02  	move	$4, $16
  1834d4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1834d8: 40 00 bf df  	ld	$ra, 0x40($sp)
  1834dc: 30 00 b2 df  	ld	$18, 0x30($sp)
  1834e0: 20 00 b1 df  	ld	$17, 0x20($sp)
  1834e4: 10 00 b0 df  	ld	$16, 0x10($sp)
  1834e8: 08 00 e0 03  	jr	$ra
  1834ec: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  1834f0: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  1834f4: 20 00 b1 ff  	sd	$17, 0x20($sp)
  1834f8: 2d 88 80 00  	move	$17, $4
  1834fc: 1c 00 04 3c  	lui	$4, 0x1c
  183500: 40 00 bf ff  	sd	$ra, 0x40($sp)
  183504: 70 88 84 24  	addiu	$4, $4, -0x7790 <.text+0xffffffffffef8870>
  183508: 10 00 b0 ff  	sd	$16, 0x10($sp)
  18350c: 49 06 04 0c  	jal	0x101924 <.text+0x1924>
  183510: 30 00 b2 ff  	sd	$18, 0x30($sp)
  183514: 2d 20 40 00  	move	$4, $2
  183518: f0 73 06 0c  	jal	0x19cfc0 <.text+0x9cfc0>
  18351c: 01 00 05 24  	addiu	$5, $zero, 0x1
  183520: 2d 80 40 00  	move	$16, $2
  183524: 46 00 00 06  	bltz	$16, 0x183640 <.text+0x83640>
  183528: 2d 10 00 00  	move	$2, $zero
  18352c: 2d 90 00 00  	move	$18, $zero
  183530: 21 28 32 02  	addu	$5, $17, $18
  183534: 2d 20 00 02  	move	$4, $16
  183538: 01 00 06 24  	addiu	$6, $zero, 0x1
  18353c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  183540: 01 00 52 26  	addiu	$18, $18, 0x1
  183544: 10 00 42 2a  	slti	$2, $18, 0x10
  183548: fa ff 40 54  	bnezl	$2, 0x183534 <.text+0x83534>
  18354c: 21 28 32 02  	addu	$5, $17, $18
  183550: 2d 20 00 02  	move	$4, $16
  183554: 2d 28 a0 03  	move	$5, $sp
  183558: 01 00 06 24  	addiu	$6, $zero, 0x1
  18355c: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  183560: 00 00 a0 a3  	sb	$zero, 0x0($sp)
  183564: 2d 28 a0 03  	move	$5, $sp
  183568: 00 00 a2 93  	lbu	$2, 0x0($sp)
  18356c: 01 00 06 24  	addiu	$6, $zero, 0x1
  183570: 2d 20 00 02  	move	$4, $16
  183574: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  183578: 10 00 22 a6  	sh	$2, 0x10($17)
  18357c: 2d 20 00 02  	move	$4, $16
  183580: 00 00 a3 93  	lbu	$3, 0x0($sp)
  183584: 01 00 06 24  	addiu	$6, $zero, 0x1
  183588: 10 00 22 96  	lhu	$2, 0x10($17)
  18358c: 12 00 25 26  	addiu	$5, $17, 0x12
  183590: 00 1a 03 00  	sll	$3, $3, 0x8
  183594: 25 10 43 00  	or	$2, $2, $3
  183598: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18359c: 10 00 22 a6  	sh	$2, 0x10($17)
  1835a0: 2d 20 00 02  	move	$4, $16
  1835a4: 01 00 06 24  	addiu	$6, $zero, 0x1
  1835a8: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1835ac: 13 00 25 26  	addiu	$5, $17, 0x13
  1835b0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1835b4: 2d 20 00 02  	move	$4, $16
  1835b8: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1835bc: 2d 28 a0 03  	move	$5, $sp
  1835c0: 2d 28 a0 03  	move	$5, $sp
  1835c4: 00 00 a2 93  	lbu	$2, 0x0($sp)
  1835c8: 01 00 06 24  	addiu	$6, $zero, 0x1
  1835cc: 2d 20 00 02  	move	$4, $16
  1835d0: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1835d4: 14 00 22 ae  	sw	$2, 0x14($17)
  1835d8: 2d 28 a0 03  	move	$5, $sp
  1835dc: 00 00 a3 93  	lbu	$3, 0x0($sp)
  1835e0: 01 00 06 24  	addiu	$6, $zero, 0x1
  1835e4: 14 00 22 8e  	lw	$2, 0x14($17)
  1835e8: 2d 20 00 02  	move	$4, $16
  1835ec: 00 1a 03 00  	sll	$3, $3, 0x8
  1835f0: 25 10 43 00  	or	$2, $2, $3
  1835f4: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  1835f8: 14 00 22 ae  	sw	$2, 0x14($17)
  1835fc: 2d 28 a0 03  	move	$5, $sp
  183600: 00 00 a3 93  	lbu	$3, 0x0($sp)
  183604: 01 00 06 24  	addiu	$6, $zero, 0x1
  183608: 14 00 22 8e  	lw	$2, 0x14($17)
  18360c: 2d 20 00 02  	move	$4, $16
  183610: 00 1c 03 00  	sll	$3, $3, 0x10
  183614: 25 10 43 00  	or	$2, $2, $3
  183618: 48 74 06 0c  	jal	0x19d120 <.text+0x9d120>
  18361c: 14 00 22 ae  	sw	$2, 0x14($17)
  183620: 14 00 23 8e  	lw	$3, 0x14($17)
  183624: 00 00 a2 93  	lbu	$2, 0x0($sp)
  183628: 2d 20 00 02  	move	$4, $16
  18362c: 00 16 02 00  	sll	$2, $2, 0x18
  183630: 25 18 62 00  	or	$3, $3, $2
  183634: 24 74 06 0c  	jal	0x19d090 <.text+0x9d090>
  183638: 14 00 23 ae  	sw	$3, 0x14($17)
  18363c: 01 00 02 24  	addiu	$2, $zero, 0x1
  183640: 40 00 bf df  	ld	$ra, 0x40($sp)
  183644: 30 00 b2 df  	ld	$18, 0x30($sp)
  183648: 20 00 b1 df  	ld	$17, 0x20($sp)
  18364c: 10 00 b0 df  	ld	$16, 0x10($sp)
  183650: 08 00 e0 03  	jr	$ra
  183654: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  183658: 08 00 e0 03  	jr	$ra
  18365c: 00 00 00 00  	nop
  183660: 42 00 02 3c  	lui	$2, 0x42
  183664: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  183668: 58 38 42 24  	addiu	$2, $2, 0x3858
  18366c: 0f 00 43 a0  	sb	$3, 0xf($2)
  183670: 08 00 e0 03  	jr	$ra
  183674: 10 00 40 a0  	sb	$zero, 0x10($2)
  183678: d0 ff bd 27  	addiu	$sp, $sp, -0x30 <.text+0xffffffffffefffd0>
  18367c: 2d 28 00 00  	move	$5, $zero
  183680: 10 00 b1 ff  	sd	$17, 0x10($sp)
  183684: 1c 00 06 24  	addiu	$6, $zero, 0x1c
  183688: 42 00 11 3c  	lui	$17, 0x42
  18368c: 00 00 b0 ff  	sd	$16, 0x0($sp)
  183690: 58 38 30 26  	addiu	$16, $17, 0x3858
  183694: 20 00 bf ff  	sd	$ra, 0x20($sp)
  183698: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  18369c: 2d 20 00 02  	move	$4, $16
  1836a0: 20 00 bf df  	ld	$ra, 0x20($sp)
  1836a4: 01 00 02 24  	addiu	$2, $zero, 0x1
  1836a8: 14 00 00 ae  	sw	$zero, 0x14($16)
  1836ac: 58 38 22 a2  	sb	$2, 0x3858($17)
  1836b0: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  1836b4: 0f 00 02 a2  	sb	$2, 0xf($16)
  1836b8: 10 00 00 a2  	sb	$zero, 0x10($16)
  1836bc: 01 00 00 a2  	sb	$zero, 0x1($16)
  1836c0: 10 00 b1 df  	ld	$17, 0x10($sp)
  1836c4: 00 00 b0 df  	ld	$16, 0x0($sp)
  1836c8: 08 00 e0 03  	jr	$ra
  1836cc: 30 00 bd 27  	addiu	$sp, $sp, 0x30
  1836d0: 42 00 02 3c  	lui	$2, 0x42
  1836d4: 0a 00 08 24  	addiu	$8, $zero, 0xa
  1836d8: 58 38 42 24  	addiu	$2, $2, 0x3858
  1836dc: 07 00 0a 24  	addiu	$10, $zero, 0x7
  1836e0: 0c 00 44 90  	lbu	$4, 0xc($2)
  1836e4: 0b 00 43 90  	lbu	$3, 0xb($2)
  1836e8: 18 30 88 00  	<unknown>
  1836ec: 0d 00 45 90  	lbu	$5, 0xd($2)
  1836f0: 0a 00 44 90  	lbu	$4, 0xa($2)
  1836f4: 08 00 47 90  	lbu	$7, 0x8($2)
  1836f8: 21 48 c3 00  	addu	$9, $6, $3
  1836fc: 0d 00 83 2c  	sltiu	$3, $4, 0xd
  183700: 09 00 46 90  	lbu	$6, 0x9($2)
  183704: 01 00 02 24  	addiu	$2, $zero, 0x1
  183708: 0a 20 43 00  	movz	$4, $2, $3
  18370c: 64 00 02 24  	addiu	$2, $zero, 0x64
  183710: 18 18 a2 00  	<unknown>
  183714: 18 10 c8 00  	<unknown>
  183718: 21 30 47 00  	addu	$6, $2, $7
  18371c: 21 28 69 00  	addu	$5, $3, $9
  183720: 42 00 02 3c  	lui	$2, 0x42
  183724: 80 18 04 00  	sll	$3, $4, 0x2
  183728: 78 38 42 24  	addiu	$2, $2, 0x3878
  18372c: 7c fc a9 24  	addiu	$9, $5, -0x384 <.text+0xffffffffffeffc7c>
  183730: 21 18 62 00  	addu	$3, $3, $2
  183734: 03 00 87 2c  	sltiu	$7, $4, 0x3
  183738: fc ff 63 8c  	lw	$3, -0x4($3)
  18373c: 82 10 09 00  	srl	$2, $9, 0x2
  183740: 21 10 22 01  	addu	$2, $9, $2
  183744: 03 00 24 31  	andi	$4, $9, 0x3
  183748: 21 10 43 00  	addu	$2, $2, $3
  18374c: 21 10 46 00  	addu	$2, $2, $6
  183750: fe ff 43 24  	addiu	$3, $2, -0x2 <.text+0xffffffffffeffffe>
  183754: 02 00 80 14  	bnez	$4, 0x183760 <.text+0x83760>
  183758: ff ff 42 24  	addiu	$2, $2, -0x1 <.text+0xffffffffffefffff>
  18375c: 0b 10 67 00  	movn	$2, $3, $7
  183760: 1b 00 4a 00  	divu	$zero, $2, $10
  183764: 01 00 40 51  	beqzl	$10, 0x18376c <.text+0x8376c>
  183768: cd 01 00 00  	break	0x0, 0x7
  18376c: 10 10 00 00  	mfhi	$2
  183770: 08 00 e0 03  	jr	$ra
  183774: 00 00 00 00  	nop
  183778: fe ff 84 24  	addiu	$4, $4, -0x2 <.text+0xffffffffffeffffe>
  18377c: 0a 00 82 2c  	sltiu	$2, $4, 0xa
  183780: 0c 00 40 50  	beqzl	$2, 0x1837b4 <.text+0x837b4>
  183784: 1f 00 04 24  	addiu	$4, $zero, 0x1f
  183788: 1c 00 03 3c  	lui	$3, 0x1c
  18378c: 80 10 04 00  	sll	$2, $4, 0x2
  183790: 78 88 63 24  	addiu	$3, $3, -0x7788 <.text+0xffffffffffef8878>
  183794: 21 10 43 00  	addu	$2, $2, $3
  183798: 00 00 42 8c  	lw	$2, 0x0($2)
  18379c: 08 00 40 00  	jr	$2
  1837a0: 00 00 00 00  	nop
  1837a4: 03 00 a3 30  	andi	$3, $5, 0x3
  1837a8: 1d 00 04 24  	addiu	$4, $zero, 0x1d
  1837ac: 1c 00 02 24  	addiu	$2, $zero, 0x1c
  1837b0: 0b 20 43 00  	movn	$4, $2, $3
  1837b4: 08 00 e0 03  	jr	$ra
  1837b8: 2d 10 80 00  	move	$2, $4
  1837bc: fd ff 00 10  	b	0x1837b4 <.text+0x837b4>
  1837c0: 1f 00 04 24  	addiu	$4, $zero, 0x1f
  1837c4: fb ff 00 10  	b	0x1837b4 <.text+0x837b4>
  1837c8: 1e 00 04 24  	addiu	$4, $zero, 0x1e
  1837cc: 90 ff bd 27  	addiu	$sp, $sp, -0x70 <.text+0xffffffffffefff90>
  1837d0: 42 00 04 3c  	lui	$4, 0x42
  1837d4: 60 00 bf ff  	sd	$ra, 0x60($sp)
  1837d8: 58 38 83 24  	addiu	$3, $4, 0x3858
  1837dc: 50 00 b5 ff  	sd	$21, 0x50($sp)
  1837e0: 40 00 b4 ff  	sd	$20, 0x40($sp)
  1837e4: 30 00 b3 ff  	sd	$19, 0x30($sp)
  1837e8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  1837ec: 10 00 b1 ff  	sd	$17, 0x10($sp)
  1837f0: 00 00 b0 ff  	sd	$16, 0x0($sp)
  1837f4: 01 00 62 90  	lbu	$2, 0x1($3)
  1837f8: a1 00 40 10  	beqz	$2, 0x183a80 <.text+0x83a80>
  1837fc: 58 38 82 90  	lbu	$2, 0x3858($4)
  183800: a0 00 40 54  	bnezl	$2, 0x183a84 <.text+0x83a84>
  183804: 60 00 bf df  	ld	$ra, 0x60($sp)
  183808: 14 00 62 8c  	lw	$2, 0x14($3)
  18380c: 23 10 02 00  	negu	$2, $2
  183810: 2d 80 40 00  	move	$16, $2
  183814: 9a 00 00 1a  	blez	$16, 0x183a80 <.text+0x83a80>
  183818: 14 00 60 ac  	sw	$zero, 0x14($3)
  18381c: 01 00 02 3c  	lui	$2, 0x1
  183820: 80 51 42 34  	ori	$2, $2, 0x5180
  183824: 2a 10 50 00  	slt	$2, $2, $16
  183828: 0b 00 40 10  	beqz	$2, 0x183858 <.text+0x83858>
  18382c: 2d 88 00 00  	move	$17, $zero
  183830: 01 00 05 3c  	lui	$5, 0x1
  183834: 80 51 a5 34  	ori	$5, $5, 0x5180
  183838: 6e 87 06 0c  	jal	0x1a1db8 <.text+0xa1db8>
  18383c: 2d 20 00 02  	move	$4, $16
  183840: 01 00 03 3c  	lui	$3, 0x1
  183844: 3c 88 02 00  	dsll32	$17, $2, 0x0
  183848: 3f 88 11 00  	dsra32	$17, $17, 0x0
  18384c: 80 51 63 34  	ori	$3, $3, 0x5180
  183850: 18 18 23 02  	<unknown>
  183854: 2f 80 03 02  	dsubu	$16, $16, $3
  183858: 11 0e 02 2a  	slti	$2, $16, 0xe11
  18385c: 09 00 40 14  	bnez	$2, 0x183884 <.text+0x83884>
  183860: 2d a0 00 00  	move	$20, $zero
  183864: 2d 20 00 02  	move	$4, $16
  183868: 6e 87 06 0c  	jal	0x1a1db8 <.text+0xa1db8>
  18386c: 10 0e 05 24  	addiu	$5, $zero, 0xe10
  183870: 3c a0 02 00  	dsll32	$20, $2, 0x0
  183874: 3f a0 14 00  	dsra32	$20, $20, 0x0
  183878: 10 0e 02 24  	addiu	$2, $zero, 0xe10
  18387c: 18 10 82 02  	<unknown>
  183880: 2f 80 02 02  	dsubu	$16, $16, $2
  183884: 3d 00 02 2a  	slti	$2, $16, 0x3d
  183888: 09 00 40 14  	bnez	$2, 0x1838b0 <.text+0x838b0>
  18388c: 2d 98 00 00  	move	$19, $zero
  183890: 2d 20 00 02  	move	$4, $16
  183894: 6e 87 06 0c  	jal	0x1a1db8 <.text+0xa1db8>
  183898: 3c 00 05 24  	addiu	$5, $zero, 0x3c
  18389c: 3c 98 02 00  	dsll32	$19, $2, 0x0
  1838a0: 3f 98 13 00  	dsra32	$19, $19, 0x0
  1838a4: 3c 00 02 24  	addiu	$2, $zero, 0x3c
  1838a8: 18 10 62 02  	<unknown>
  1838ac: 2f 80 02 02  	dsubu	$16, $16, $2
  1838b0: 42 00 02 3c  	lui	$2, 0x42
  1838b4: 3c 18 10 00  	dsll32	$3, $16, 0x0
  1838b8: 3f 18 03 00  	dsra32	$3, $3, 0x0
  1838bc: 58 38 47 24  	addiu	$7, $2, 0x3858
  1838c0: 0a 00 06 24  	addiu	$6, $zero, 0xa
  1838c4: 03 00 e2 90  	lbu	$2, 0x3($7)
  1838c8: 2a 28 10 00  	slt	$5, $zero, $16
  1838cc: 2d a8 00 00  	move	$21, $zero
  1838d0: 02 00 e4 90  	lbu	$4, 0x2($7)
  1838d4: 0b a8 65 00  	movn	$21, $3, $5
  1838d8: 18 18 46 00  	<unknown>
  1838dc: 21 10 64 00  	addu	$2, $3, $4
  1838e0: 21 a8 a2 02  	addu	$21, $21, $2
  1838e4: 3c 00 a2 2a  	slti	$2, $21, 0x3c
  1838e8: 03 00 40 14  	bnez	$2, 0x1838f8 <.text+0x838f8>
  1838ec: 05 00 e2 90  	lbu	$2, 0x5($7)
  1838f0: c4 ff b5 26  	addiu	$21, $21, -0x3c <.text+0xffffffffffefffc4>
  1838f4: 01 00 73 26  	addiu	$19, $19, 0x1
  1838f8: 04 00 e3 90  	lbu	$3, 0x4($7)
  1838fc: 18 20 46 00  	<unknown>
  183900: 21 10 83 00  	addu	$2, $4, $3
  183904: 21 98 62 02  	addu	$19, $19, $2
  183908: 3c 00 62 2a  	slti	$2, $19, 0x3c
  18390c: 03 00 40 14  	bnez	$2, 0x18391c <.text+0x8391c>
  183910: 07 00 e2 90  	lbu	$2, 0x7($7)
  183914: c4 ff 73 26  	addiu	$19, $19, -0x3c <.text+0xffffffffffefffc4>
  183918: 01 00 94 26  	addiu	$20, $20, 0x1
  18391c: 06 00 e3 90  	lbu	$3, 0x6($7)
  183920: 18 20 46 00  	<unknown>
  183924: 21 10 83 00  	addu	$2, $4, $3
  183928: 21 a0 82 02  	addu	$20, $20, $2
  18392c: 18 00 82 2a  	slti	$2, $20, 0x18
  183930: 03 00 40 14  	bnez	$2, 0x183940 <.text+0x83940>
  183934: 00 00 00 00  	nop
  183938: e8 ff 94 26  	addiu	$20, $20, -0x18 <.text+0xffffffffffefffe8>
  18393c: 01 00 31 26  	addiu	$17, $17, 0x1
  183940: 3b 00 20 1a  	blez	$17, 0x183a30 <.text+0x83a30>
  183944: 0a 00 03 24  	addiu	$3, $zero, 0xa
  183948: 0c 00 e3 90  	lbu	$3, 0xc($7)
  18394c: 0b 00 e2 90  	lbu	$2, 0xb($7)
  183950: 18 20 66 00  	<unknown>
  183954: 08 00 e5 90  	lbu	$5, 0x8($7)
  183958: 09 00 e3 90  	lbu	$3, 0x9($7)
  18395c: 0a 00 f2 90  	lbu	$18, 0xa($7)
  183960: 21 80 82 00  	addu	$16, $4, $2
  183964: 18 10 66 00  	<unknown>
  183968: 0d 00 e4 90  	lbu	$4, 0xd($7)
  18396c: 21 18 45 00  	addu	$3, $2, $5
  183970: 64 00 02 24  	addiu	$2, $zero, 0x64
  183974: 18 28 82 00  	<unknown>
  183978: 21 88 23 02  	addu	$17, $17, $3
  18397c: 21 20 b0 00  	addu	$4, $5, $16
  183980: e8 03 90 24  	addiu	$16, $4, 0x3e8
  183984: 2d 20 40 02  	move	$4, $18
  183988: de 0d 06 0c  	jal	0x183778 <.text+0x83778>
  18398c: 2d 28 00 02  	move	$5, $16
  183990: 2d 18 40 00  	move	$3, $2
  183994: 2a 10 51 00  	slt	$2, $2, $17
  183998: 08 00 40 10  	beqz	$2, 0x1839bc <.text+0x839bc>
  18399c: 64 00 02 24  	addiu	$2, $zero, 0x64
  1839a0: 01 00 52 26  	addiu	$18, $18, 0x1
  1839a4: 0d 00 42 2a  	slti	$2, $18, 0xd
  1839a8: f6 ff 40 14  	bnez	$2, 0x183984 <.text+0x83984>
  1839ac: 23 88 23 02  	subu	$17, $17, $3
  1839b0: 01 00 10 26  	addiu	$16, $16, 0x1
  1839b4: f3 ff 00 10  	b	0x183984 <.text+0x83984>
  1839b8: 01 00 12 24  	addiu	$18, $zero, 0x1
  1839bc: 18 fc 04 26  	addiu	$4, $16, -0x3e8 <.text+0xffffffffffeffc18>
  1839c0: 1a 00 02 02  	div	$zero, $16, $2
  1839c4: 0a 00 03 24  	addiu	$3, $zero, 0xa
  1839c8: 01 00 40 50  	beqzl	$2, 0x1839d0 <.text+0x839d0>
  1839cc: cd 01 00 00  	break	0x0, 0x7
  1839d0: 42 00 10 3c  	lui	$16, 0x42
  1839d4: 58 38 10 26  	addiu	$16, $16, 0x3858
  1839d8: 10 30 00 00  	mfhi	$6
  1839dc: 1a 00 82 00  	div	$zero, $4, $2
  1839e0: 0a 00 02 24  	addiu	$2, $zero, 0xa
  1839e4: 64 00 02 24  	addiu	$2, $zero, 0x64
  1839e8: 0a 00 02 24  	addiu	$2, $zero, 0xa
  1839ec: 12 20 00 00  	mflo	$4
  1839f0: 1a 00 23 02  	div	$zero, $17, $3
  1839f4: 10 10 00 00  	mfhi	$2
  1839f8: 12 28 00 00  	mflo	$5
  1839fc: 08 00 02 a2  	sb	$2, 0x8($16)
  183a00: 0a 00 02 24  	addiu	$2, $zero, 0xa
  183a04: 0d 00 04 a2  	sb	$4, 0xd($16)
  183a08: 1a 00 c3 00  	div	$zero, $6, $3
  183a0c: 09 00 05 a2  	sb	$5, 0x9($16)
  183a10: 0a 00 12 a2  	sb	$18, 0xa($16)
  183a14: 10 30 00 00  	mfhi	$6
  183a18: 12 18 00 00  	mflo	$3
  183a1c: 0b 00 06 a2  	sb	$6, 0xb($16)
  183a20: b4 0d 06 0c  	jal	0x1836d0 <.text+0x836d0>
  183a24: 0c 00 03 a2  	sb	$3, 0xc($16)
  183a28: 0e 00 02 a2  	sb	$2, 0xe($16)
  183a2c: 0a 00 03 24  	addiu	$3, $zero, 0xa
  183a30: 42 00 04 3c  	lui	$4, 0x42
  183a34: 1a 00 83 02  	div	$zero, $20, $3
  183a38: 58 38 84 24  	addiu	$4, $4, 0x3858
  183a3c: 01 00 60 50  	beqzl	$3, 0x183a44 <.text+0x83a44>
  183a40: cd 01 00 00  	break	0x0, 0x7
  183a44: 10 38 00 00  	mfhi	$7
  183a48: 12 40 00 00  	mflo	$8
  183a4c: 1a 00 a3 02  	div	$zero, $21, $3
  183a50: 10 10 00 00  	mfhi	$2
  183a54: 12 30 00 00  	mflo	$6
  183a58: 02 00 82 a0  	sb	$2, 0x2($4)
  183a5c: 0a 00 02 24  	addiu	$2, $zero, 0xa
  183a60: 03 00 86 a0  	sb	$6, 0x3($4)
  183a64: 1a 00 63 02  	div	$zero, $19, $3
  183a68: 10 28 00 00  	mfhi	$5
  183a6c: 12 18 00 00  	mflo	$3
  183a70: 04 00 85 a0  	sb	$5, 0x4($4)
  183a74: 05 00 83 a0  	sb	$3, 0x5($4)
  183a78: 06 00 87 a0  	sb	$7, 0x6($4)
  183a7c: 07 00 88 a0  	sb	$8, 0x7($4)
  183a80: 60 00 bf df  	ld	$ra, 0x60($sp)
  183a84: 50 00 b5 df  	ld	$21, 0x50($sp)
  183a88: 40 00 b4 df  	ld	$20, 0x40($sp)
  183a8c: 30 00 b3 df  	ld	$19, 0x30($sp)
  183a90: 20 00 b2 df  	ld	$18, 0x20($sp)
  183a94: 10 00 b1 df  	ld	$17, 0x10($sp)
  183a98: 00 00 b0 df  	ld	$16, 0x0($sp)
  183a9c: 08 00 e0 03  	jr	$ra
  183aa0: 70 00 bd 27  	addiu	$sp, $sp, 0x70
  183aa4: 0f 00 84 30  	andi	$4, $4, 0xf
  183aa8: b0 ff bd 27  	addiu	$sp, $sp, -0x50 <.text+0xffffffffffefffb0>
  183aac: 0d 00 82 2c  	sltiu	$2, $4, 0xd
  183ab0: 40 00 bf ff  	sd	$ra, 0x40($sp)
  183ab4: 30 00 b3 ff  	sd	$19, 0x30($sp)
  183ab8: 20 00 b2 ff  	sd	$18, 0x20($sp)
  183abc: 10 00 b1 ff  	sd	$17, 0x10($sp)
  183ac0: 16 00 40 14  	bnez	$2, 0x183b1c <.text+0x83b1c>
  183ac4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  183ac8: 0d 00 02 24  	addiu	$2, $zero, 0xd
  183acc: 0d 00 82 10  	beq	$4, $2, 0x183b04 <.text+0x83b04>
  183ad0: 0e 00 02 24  	addiu	$2, $zero, 0xe
  183ad4: 08 00 82 10  	beq	$4, $2, 0x183af8 <.text+0x83af8>
  183ad8: 02 00 03 24  	addiu	$3, $zero, 0x2
  183adc: 40 00 bf df  	ld	$ra, 0x40($sp)
  183ae0: 30 00 b3 df  	ld	$19, 0x30($sp)
  183ae4: 20 00 b2 df  	ld	$18, 0x20($sp)
  183ae8: 10 00 b1 df  	ld	$17, 0x10($sp)
  183aec: 00 00 b0 df  	ld	$16, 0x0($sp)
  183af0: 08 00 e0 03  	jr	$ra
  183af4: 50 00 bd 27  	addiu	$sp, $sp, 0x50
  183af8: 42 00 02 3c  	lui	$2, 0x42
  183afc: f7 ff 00 10  	b	0x183adc <.text+0x83adc>
  183b00: 68 38 43 a0  	sb	$3, 0x3868($2)
  183b04: 42 00 02 3c  	lui	$2, 0x42
  183b08: ff ff 03 24  	addiu	$3, $zero, -0x1 <.text+0xffffffffffefffff>
  183b0c: 58 38 42 24  	addiu	$2, $2, 0x3858
  183b10: 0f 00 43 a0  	sb	$3, 0xf($2)
  183b14: f1 ff 00 10  	b	0x183adc <.text+0x83adc>
  183b18: 10 00 40 a0  	sb	$zero, 0x10($2)
  183b1c: 42 00 13 3c  	lui	$19, 0x42
  183b20: 01 00 12 24  	addiu	$18, $zero, 0x1
  183b24: 58 38 71 26  	addiu	$17, $19, 0x3858
  183b28: 10 00 23 92  	lbu	$3, 0x10($17)
  183b2c: 17 00 72 10  	beq	$3, $18, 0x183b8c <.text+0x83b8c>
  183b30: 02 00 02 24  	addiu	$2, $zero, 0x2
  183b34: ea ff 62 14  	bne	$3, $2, 0x183ae0 <.text+0x83ae0>
  183b38: 40 00 bf df  	ld	$ra, 0x40($sp)
  183b3c: 10 00 80 50  	beqzl	$4, 0x183b80 <.text+0x83b80>
  183b40: 10 00 32 a2  	sb	$18, 0x10($17)
  183b44: 04 00 02 24  	addiu	$2, $zero, 0x4
  183b48: 03 00 82 10  	beq	$4, $2, 0x183b58 <.text+0x83b58>
  183b4c: 03 00 02 24  	addiu	$2, $zero, 0x3
  183b50: e3 ff 00 10  	b	0x183ae0 <.text+0x83ae0>
  183b54: 10 00 22 a2  	sb	$2, 0x10($17)
  183b58: 02 00 24 26  	addiu	$4, $17, 0x2
  183b5c: 2d 28 00 00  	move	$5, $zero
  183b60: 0d 00 06 24  	addiu	$6, $zero, 0xd
  183b64: e7 70 06 0c  	jal	0x19c39c <.text+0x9c39c>
  183b68: 01 00 20 a2  	sb	$zero, 0x1($17)
  183b6c: 03 00 02 24  	addiu	$2, $zero, 0x3
  183b70: 10 00 22 a2  	sb	$2, 0x10($17)
  183b74: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  183b78: d8 ff 00 10  	b	0x183adc <.text+0x83adc>
  183b7c: 0f 00 22 a2  	sb	$2, 0xf($17)
  183b80: 01 00 20 a2  	sb	$zero, 0x1($17)
  183b84: d5 ff 00 10  	b	0x183adc <.text+0x83adc>
  183b88: 0f 00 20 a2  	sb	$zero, 0xf($17)
  183b8c: 0f 00 23 92  	lbu	$3, 0xf($17)
  183b90: 00 16 03 00  	sll	$2, $3, 0x18
  183b94: 01 00 65 24  	addiu	$5, $3, 0x1
  183b98: 03 16 02 00  	sra	$2, $2, 0x18
  183b9c: 00 1e 05 00  	sll	$3, $5, 0x18
  183ba0: 21 10 51 00  	addu	$2, $2, $17
  183ba4: 03 1e 03 00  	sra	$3, $3, 0x18
  183ba8: 02 00 44 a0  	sb	$4, 0x2($2)
  183bac: 0c 00 02 24  	addiu	$2, $zero, 0xc
  183bb0: ca ff 62 14  	bne	$3, $2, 0x183adc <.text+0x83adc>
  183bb4: 0f 00 25 a2  	sb	$5, 0xf($17)
  183bb8: 01 00 a2 24  	addiu	$2, $5, 0x1
  183bbc: 14 00 20 ae  	sw	$zero, 0x14($17)
  183bc0: 0f 00 22 a2  	sb	$2, 0xf($17)
  183bc4: b4 0d 06 0c  	jal	0x1836d0 <.text+0x836d0>
  183bc8: 0c 00 30 26  	addiu	$16, $17, 0xc
  183bcc: 01 00 32 a2  	sb	$18, 0x1($17)
  183bd0: 58 38 60 a2  	sb	$zero, 0x3858($19)
  183bd4: c1 ff 00 10  	b	0x183adc <.text+0x83adc>
  183bd8: 02 00 02 a2  	sb	$2, 0x2($16)
  183bdc: e0 ff bd 27  	addiu	$sp, $sp, -0x20 <.text+0xffffffffffefffe0>
  183be0: 42 00 02 3c  	lui	$2, 0x42
  183be4: 00 00 b0 ff  	sd	$16, 0x0($sp)
  183be8: 58 38 50 24  	addiu	$16, $2, 0x3858
  183bec: 10 00 bf ff  	sd	$ra, 0x10($sp)
  183bf0: 10 00 02 92  	lbu	$2, 0x10($16)
  183bf4: 0b 00 40 14  	bnez	$2, 0x183c24 <.text+0x83c24>
  183bf8: 2d 18 00 00  	move	$3, $zero
  183bfc: 0f 00 02 82  	lb	$2, 0xf($16)
  183c00: 0f 00 03 92  	lbu	$3, 0xf($16)
  183c04: 0e 00 40 04  	bltz	$2, 0x183c40 <.text+0x83c40>
  183c08: 0d 00 45 28  	slti	$5, $2, 0xd
  183c0c: 21 20 50 00  	addu	$4, $2, $16
  183c10: 01 00 62 24  	addiu	$2, $3, 0x1
  183c14: 08 00 a0 14  	bnez	$5, 0x183c38 <.text+0x83c38>
  183c18: 0f 00 03 24  	addiu	$3, $zero, 0xf
  183c1c: ff ff 02 24  	addiu	$2, $zero, -0x1 <.text+0xffffffffffefffff>
  183c20: 0f 00 02 a2  	sb	$2, 0xf($16)
  183c24: 10 00 bf df  	ld	$ra, 0x10($sp)
  183c28: 2d 10 60 00  	move	$2, $3
  183c2c: 00 00 b0 df  	ld	$16, 0x0($sp)
  183c30: 08 00 e0 03  	jr	$ra
  183c34: 20 00 bd 27  	addiu	$sp, $sp, 0x20
  183c38: f9 ff 00 10  	b	0x183c20 <.text+0x83c20>
  183c3c: 02 00 83 90  	lbu	$3, 0x2($4)
  183c40: f3 0d 06 0c  	jal	0x1837cc <.text+0x837cc>
  183c44: 00 00 00 00  	nop
  183c48: 0f 00 02 92  	lbu	$2, 0xf($16)
  183c4c: 0f 00 03 24  	addiu	$3, $zero, 0xf
  183c50: f3 ff 00 10  	b	0x183c20 <.text+0x83c20>
  183c54: 01 00 42 24  	addiu	$2, $2, 0x1
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
