  1ad55c: 36 00 02 3c  	lui	$2, 0x36
  1ad560: 40 30 04 00  	sll	$6, $4, 0x1
  1ad564: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ad568: 2d 40 a0 00  	move	$8, $5
  1ad56c: 40 00 e2 8c  	lw	$2, 0x40($7)
  1ad570: 4c 00 e5 90  	lbu	$5, 0x4c($7)
  1ad574: 21 20 44 00  	addu	$4, $2, $4
  1ad578: 3c 00 e3 8c  	lw	$3, 0x3c($7)
  1ad57c: 00 00 82 90  	lbu	$2, 0x0($4)
  1ad580: 2b 10 45 00  	sltu	$2, $2, $5
  1ad584: 0b 00 40 10  	beqz	$2, 0x1ad5b4 <.text+0xad5b4>
  1ad588: 21 30 66 00  	addu	$6, $3, $6
  1ad58c: 00 00 02 91  	lbu	$2, 0x0($8)
  1ad590: 09 00 40 10  	beqz	$2, 0x1ad5b8 <.text+0xad5b8>
  1ad594: 36 00 03 3c  	lui	$3, 0x36
  1ad598: 44 00 e3 8c  	lw	$3, 0x44($7)
  1ad59c: 40 10 02 00  	sll	$2, $2, 0x1
  1ad5a0: 21 10 43 00  	addu	$2, $2, $3
  1ad5a4: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad5a8: 00 00 c2 a4  	sh	$2, 0x0($6)
  1ad5ac: 4d 00 e2 90  	lbu	$2, 0x4d($7)
  1ad5b0: 00 00 82 a0  	sb	$2, 0x0($4)
  1ad5b4: 36 00 03 3c  	lui	$3, 0x36
  1ad5b8: 01 00 82 90  	lbu	$2, 0x1($4)
  1ad5bc: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad5c0: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad5c4: 2b 10 43 00  	sltu	$2, $2, $3
  1ad5c8: 0c 00 40 10  	beqz	$2, 0x1ad5fc <.text+0xad5fc>
  1ad5cc: 36 00 03 3c  	lui	$3, 0x36
  1ad5d0: 01 00 02 91  	lbu	$2, 0x1($8)
  1ad5d4: 0a 00 40 50  	beqzl	$2, 0x1ad600 <.text+0xad600>
  1ad5d8: 02 00 82 90  	lbu	$2, 0x2($4)
  1ad5dc: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad5e0: 40 10 02 00  	sll	$2, $2, 0x1
  1ad5e4: 21 10 43 00  	addu	$2, $2, $3
  1ad5e8: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad5ec: 02 00 c2 a4  	sh	$2, 0x2($6)
  1ad5f0: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad5f4: 01 00 82 a0  	sb	$2, 0x1($4)
  1ad5f8: 36 00 03 3c  	lui	$3, 0x36
  1ad5fc: 02 00 82 90  	lbu	$2, 0x2($4)
  1ad600: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad604: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad608: 2b 10 43 00  	sltu	$2, $2, $3
  1ad60c: 0c 00 40 10  	beqz	$2, 0x1ad640 <.text+0xad640>
  1ad610: 36 00 03 3c  	lui	$3, 0x36
  1ad614: 02 00 02 91  	lbu	$2, 0x2($8)
  1ad618: 0a 00 40 50  	beqzl	$2, 0x1ad644 <.text+0xad644>
  1ad61c: 03 00 82 90  	lbu	$2, 0x3($4)
  1ad620: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad624: 40 10 02 00  	sll	$2, $2, 0x1
  1ad628: 21 10 43 00  	addu	$2, $2, $3
  1ad62c: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad630: 04 00 c2 a4  	sh	$2, 0x4($6)
  1ad634: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad638: 02 00 82 a0  	sb	$2, 0x2($4)
  1ad63c: 36 00 03 3c  	lui	$3, 0x36
  1ad640: 03 00 82 90  	lbu	$2, 0x3($4)
  1ad644: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad648: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad64c: 2b 10 43 00  	sltu	$2, $2, $3
  1ad650: 0a 00 40 10  	beqz	$2, 0x1ad67c <.text+0xad67c>
  1ad654: 00 00 00 00  	nop
  1ad658: 03 00 02 91  	lbu	$2, 0x3($8)
  1ad65c: 07 00 40 10  	beqz	$2, 0x1ad67c <.text+0xad67c>
  1ad660: 40 10 02 00  	sll	$2, $2, 0x1
  1ad664: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad668: 21 10 43 00  	addu	$2, $2, $3
  1ad66c: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad670: 06 00 c2 a4  	sh	$2, 0x6($6)
  1ad674: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad678: 03 00 82 a0  	sb	$2, 0x3($4)
  1ad67c: 08 00 e0 03  	jr	$ra
  1ad680: 00 00 00 00  	nop
  1ad684: 36 00 02 3c  	lui	$2, 0x36
  1ad688: 40 30 04 00  	sll	$6, $4, 0x1
  1ad68c: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ad690: 2d 40 a0 00  	move	$8, $5
  1ad694: 40 00 e2 8c  	lw	$2, 0x40($7)
  1ad698: 4c 00 e5 90  	lbu	$5, 0x4c($7)
  1ad69c: 21 20 44 00  	addu	$4, $2, $4
  1ad6a0: 3c 00 e3 8c  	lw	$3, 0x3c($7)
  1ad6a4: 00 00 82 90  	lbu	$2, 0x0($4)
  1ad6a8: 2b 10 45 00  	sltu	$2, $2, $5
  1ad6ac: 0b 00 40 10  	beqz	$2, 0x1ad6dc <.text+0xad6dc>
  1ad6b0: 21 30 66 00  	addu	$6, $3, $6
  1ad6b4: 03 00 02 91  	lbu	$2, 0x3($8)
  1ad6b8: 09 00 40 10  	beqz	$2, 0x1ad6e0 <.text+0xad6e0>
  1ad6bc: 36 00 03 3c  	lui	$3, 0x36
  1ad6c0: 44 00 e3 8c  	lw	$3, 0x44($7)
  1ad6c4: 40 10 02 00  	sll	$2, $2, 0x1
  1ad6c8: 21 10 43 00  	addu	$2, $2, $3
  1ad6cc: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad6d0: 00 00 c2 a4  	sh	$2, 0x0($6)
  1ad6d4: 4d 00 e2 90  	lbu	$2, 0x4d($7)
  1ad6d8: 00 00 82 a0  	sb	$2, 0x0($4)
  1ad6dc: 36 00 03 3c  	lui	$3, 0x36
  1ad6e0: 01 00 82 90  	lbu	$2, 0x1($4)
  1ad6e4: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad6e8: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad6ec: 2b 10 43 00  	sltu	$2, $2, $3
  1ad6f0: 0c 00 40 10  	beqz	$2, 0x1ad724 <.text+0xad724>
  1ad6f4: 36 00 03 3c  	lui	$3, 0x36
  1ad6f8: 02 00 02 91  	lbu	$2, 0x2($8)
  1ad6fc: 0a 00 40 50  	beqzl	$2, 0x1ad728 <.text+0xad728>
  1ad700: 02 00 82 90  	lbu	$2, 0x2($4)
  1ad704: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad708: 40 10 02 00  	sll	$2, $2, 0x1
  1ad70c: 21 10 43 00  	addu	$2, $2, $3
  1ad710: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad714: 02 00 c2 a4  	sh	$2, 0x2($6)
  1ad718: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad71c: 01 00 82 a0  	sb	$2, 0x1($4)
  1ad720: 36 00 03 3c  	lui	$3, 0x36
  1ad724: 02 00 82 90  	lbu	$2, 0x2($4)
  1ad728: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad72c: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad730: 2b 10 43 00  	sltu	$2, $2, $3
  1ad734: 0c 00 40 10  	beqz	$2, 0x1ad768 <.text+0xad768>
  1ad738: 36 00 03 3c  	lui	$3, 0x36
  1ad73c: 01 00 02 91  	lbu	$2, 0x1($8)
  1ad740: 0a 00 40 50  	beqzl	$2, 0x1ad76c <.text+0xad76c>
  1ad744: 03 00 82 90  	lbu	$2, 0x3($4)
  1ad748: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad74c: 40 10 02 00  	sll	$2, $2, 0x1
  1ad750: 21 10 43 00  	addu	$2, $2, $3
  1ad754: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad758: 04 00 c2 a4  	sh	$2, 0x4($6)
  1ad75c: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad760: 02 00 82 a0  	sb	$2, 0x2($4)
  1ad764: 36 00 03 3c  	lui	$3, 0x36
  1ad768: 03 00 82 90  	lbu	$2, 0x3($4)
  1ad76c: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad770: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad774: 2b 10 43 00  	sltu	$2, $2, $3
  1ad778: 0a 00 40 10  	beqz	$2, 0x1ad7a4 <.text+0xad7a4>
  1ad77c: 00 00 00 00  	nop
  1ad780: 00 00 02 91  	lbu	$2, 0x0($8)
  1ad784: 07 00 40 10  	beqz	$2, 0x1ad7a4 <.text+0xad7a4>
  1ad788: 40 10 02 00  	sll	$2, $2, 0x1
  1ad78c: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad790: 21 10 43 00  	addu	$2, $2, $3
  1ad794: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad798: 06 00 c2 a4  	sh	$2, 0x6($6)
  1ad79c: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad7a0: 03 00 82 a0  	sb	$2, 0x3($4)
  1ad7a4: 08 00 e0 03  	jr	$ra
  1ad7a8: 00 00 00 00  	nop
  1ad7ac: 36 00 02 3c  	lui	$2, 0x36
  1ad7b0: 40 30 04 00  	sll	$6, $4, 0x1
  1ad7b4: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ad7b8: 2d 40 a0 00  	move	$8, $5
  1ad7bc: 40 00 e2 8c  	lw	$2, 0x40($7)
  1ad7c0: 4c 00 e5 90  	lbu	$5, 0x4c($7)
  1ad7c4: 21 20 44 00  	addu	$4, $2, $4
  1ad7c8: 3c 00 e3 8c  	lw	$3, 0x3c($7)
  1ad7cc: 00 00 82 90  	lbu	$2, 0x0($4)
  1ad7d0: 2b 10 45 00  	sltu	$2, $2, $5
  1ad7d4: 0d 00 40 10  	beqz	$2, 0x1ad80c <.text+0xad80c>
  1ad7d8: 21 30 66 00  	addu	$6, $3, $6
  1ad7dc: 00 00 02 91  	lbu	$2, 0x0($8)
  1ad7e0: 0b 00 40 10  	beqz	$2, 0x1ad810 <.text+0xad810>
  1ad7e4: 36 00 03 3c  	lui	$3, 0x36
  1ad7e8: 44 00 e3 8c  	lw	$3, 0x44($7)
  1ad7ec: 40 10 02 00  	sll	$2, $2, 0x1
  1ad7f0: 21 10 43 00  	addu	$2, $2, $3
  1ad7f4: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad7f8: 00 00 c2 a4  	sh	$2, 0x0($6)
  1ad7fc: 02 00 c2 a4  	sh	$2, 0x2($6)
  1ad800: 4d 00 e2 90  	lbu	$2, 0x4d($7)
  1ad804: 00 00 82 a0  	sb	$2, 0x0($4)
  1ad808: 01 00 82 a0  	sb	$2, 0x1($4)
  1ad80c: 36 00 03 3c  	lui	$3, 0x36
  1ad810: 02 00 82 90  	lbu	$2, 0x2($4)
  1ad814: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad818: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad81c: 2b 10 43 00  	sltu	$2, $2, $3
  1ad820: 0e 00 40 10  	beqz	$2, 0x1ad85c <.text+0xad85c>
  1ad824: 36 00 03 3c  	lui	$3, 0x36
  1ad828: 01 00 02 91  	lbu	$2, 0x1($8)
  1ad82c: 0c 00 40 50  	beqzl	$2, 0x1ad860 <.text+0xad860>
  1ad830: 04 00 82 90  	lbu	$2, 0x4($4)
  1ad834: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad838: 40 10 02 00  	sll	$2, $2, 0x1
  1ad83c: 21 10 43 00  	addu	$2, $2, $3
  1ad840: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad844: 04 00 c2 a4  	sh	$2, 0x4($6)
  1ad848: 06 00 c2 a4  	sh	$2, 0x6($6)
  1ad84c: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad850: 02 00 82 a0  	sb	$2, 0x2($4)
  1ad854: 03 00 82 a0  	sb	$2, 0x3($4)
  1ad858: 36 00 03 3c  	lui	$3, 0x36
  1ad85c: 04 00 82 90  	lbu	$2, 0x4($4)
  1ad860: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad864: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad868: 2b 10 43 00  	sltu	$2, $2, $3
  1ad86c: 0e 00 40 10  	beqz	$2, 0x1ad8a8 <.text+0xad8a8>
  1ad870: 36 00 03 3c  	lui	$3, 0x36
  1ad874: 02 00 02 91  	lbu	$2, 0x2($8)
  1ad878: 0c 00 40 50  	beqzl	$2, 0x1ad8ac <.text+0xad8ac>
  1ad87c: 06 00 82 90  	lbu	$2, 0x6($4)
  1ad880: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad884: 40 10 02 00  	sll	$2, $2, 0x1
  1ad888: 21 10 43 00  	addu	$2, $2, $3
  1ad88c: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad890: 08 00 c2 a4  	sh	$2, 0x8($6)
  1ad894: 0a 00 c2 a4  	sh	$2, 0xa($6)
  1ad898: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad89c: 04 00 82 a0  	sb	$2, 0x4($4)
  1ad8a0: 05 00 82 a0  	sb	$2, 0x5($4)
  1ad8a4: 36 00 03 3c  	lui	$3, 0x36
  1ad8a8: 06 00 82 90  	lbu	$2, 0x6($4)
  1ad8ac: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad8b0: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad8b4: 2b 10 43 00  	sltu	$2, $2, $3
  1ad8b8: 0c 00 40 10  	beqz	$2, 0x1ad8ec <.text+0xad8ec>
  1ad8bc: 00 00 00 00  	nop
  1ad8c0: 03 00 02 91  	lbu	$2, 0x3($8)
  1ad8c4: 09 00 40 10  	beqz	$2, 0x1ad8ec <.text+0xad8ec>
  1ad8c8: 40 10 02 00  	sll	$2, $2, 0x1
  1ad8cc: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad8d0: 21 10 43 00  	addu	$2, $2, $3
  1ad8d4: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad8d8: 0c 00 c2 a4  	sh	$2, 0xc($6)
  1ad8dc: 0e 00 c2 a4  	sh	$2, 0xe($6)
  1ad8e0: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad8e4: 06 00 82 a0  	sb	$2, 0x6($4)
  1ad8e8: 07 00 82 a0  	sb	$2, 0x7($4)
  1ad8ec: 08 00 e0 03  	jr	$ra
  1ad8f0: 00 00 00 00  	nop
  1ad8f4: 36 00 02 3c  	lui	$2, 0x36
  1ad8f8: 40 30 04 00  	sll	$6, $4, 0x1
  1ad8fc: 80 d4 47 24  	addiu	$7, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ad900: 2d 40 a0 00  	move	$8, $5
  1ad904: 40 00 e2 8c  	lw	$2, 0x40($7)
  1ad908: 4c 00 e5 90  	lbu	$5, 0x4c($7)
  1ad90c: 21 20 44 00  	addu	$4, $2, $4
  1ad910: 3c 00 e3 8c  	lw	$3, 0x3c($7)
  1ad914: 00 00 82 90  	lbu	$2, 0x0($4)
  1ad918: 2b 10 45 00  	sltu	$2, $2, $5
  1ad91c: 0d 00 40 10  	beqz	$2, 0x1ad954 <.text+0xad954>
  1ad920: 21 30 66 00  	addu	$6, $3, $6
  1ad924: 03 00 02 91  	lbu	$2, 0x3($8)
  1ad928: 0b 00 40 10  	beqz	$2, 0x1ad958 <.text+0xad958>
  1ad92c: 36 00 03 3c  	lui	$3, 0x36
  1ad930: 44 00 e3 8c  	lw	$3, 0x44($7)
  1ad934: 40 10 02 00  	sll	$2, $2, 0x1
  1ad938: 21 10 43 00  	addu	$2, $2, $3
  1ad93c: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad940: 00 00 c2 a4  	sh	$2, 0x0($6)
  1ad944: 02 00 c2 a4  	sh	$2, 0x2($6)
  1ad948: 4d 00 e2 90  	lbu	$2, 0x4d($7)
  1ad94c: 00 00 82 a0  	sb	$2, 0x0($4)
  1ad950: 01 00 82 a0  	sb	$2, 0x1($4)
  1ad954: 36 00 03 3c  	lui	$3, 0x36
  1ad958: 02 00 82 90  	lbu	$2, 0x2($4)
  1ad95c: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad960: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad964: 2b 10 43 00  	sltu	$2, $2, $3
  1ad968: 0e 00 40 10  	beqz	$2, 0x1ad9a4 <.text+0xad9a4>
  1ad96c: 36 00 03 3c  	lui	$3, 0x36
  1ad970: 02 00 02 91  	lbu	$2, 0x2($8)
  1ad974: 0c 00 40 50  	beqzl	$2, 0x1ad9a8 <.text+0xad9a8>
  1ad978: 04 00 82 90  	lbu	$2, 0x4($4)
  1ad97c: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad980: 40 10 02 00  	sll	$2, $2, 0x1
  1ad984: 21 10 43 00  	addu	$2, $2, $3
  1ad988: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad98c: 04 00 c2 a4  	sh	$2, 0x4($6)
  1ad990: 06 00 c2 a4  	sh	$2, 0x6($6)
  1ad994: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad998: 02 00 82 a0  	sb	$2, 0x2($4)
  1ad99c: 03 00 82 a0  	sb	$2, 0x3($4)
  1ad9a0: 36 00 03 3c  	lui	$3, 0x36
  1ad9a4: 04 00 82 90  	lbu	$2, 0x4($4)
  1ad9a8: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad9ac: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad9b0: 2b 10 43 00  	sltu	$2, $2, $3
  1ad9b4: 0e 00 40 10  	beqz	$2, 0x1ad9f0 <.text+0xad9f0>
  1ad9b8: 36 00 03 3c  	lui	$3, 0x36
  1ad9bc: 01 00 02 91  	lbu	$2, 0x1($8)
  1ad9c0: 0c 00 40 50  	beqzl	$2, 0x1ad9f4 <.text+0xad9f4>
  1ad9c4: 06 00 82 90  	lbu	$2, 0x6($4)
  1ad9c8: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ad9cc: 40 10 02 00  	sll	$2, $2, 0x1
  1ad9d0: 21 10 43 00  	addu	$2, $2, $3
  1ad9d4: 00 00 42 94  	lhu	$2, 0x0($2)
  1ad9d8: 08 00 c2 a4  	sh	$2, 0x8($6)
  1ad9dc: 0a 00 c2 a4  	sh	$2, 0xa($6)
  1ad9e0: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ad9e4: 04 00 82 a0  	sb	$2, 0x4($4)
  1ad9e8: 05 00 82 a0  	sb	$2, 0x5($4)
  1ad9ec: 36 00 03 3c  	lui	$3, 0x36
  1ad9f0: 06 00 82 90  	lbu	$2, 0x6($4)
  1ad9f4: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1ad9f8: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1ad9fc: 2b 10 43 00  	sltu	$2, $2, $3
  1ada00: 0c 00 40 10  	beqz	$2, 0x1ada34 <.text+0xada34>
  1ada04: 00 00 00 00  	nop
  1ada08: 00 00 02 91  	lbu	$2, 0x0($8)
  1ada0c: 09 00 40 10  	beqz	$2, 0x1ada34 <.text+0xada34>
  1ada10: 40 10 02 00  	sll	$2, $2, 0x1
  1ada14: 44 00 a3 8c  	lw	$3, 0x44($5)
  1ada18: 21 10 43 00  	addu	$2, $2, $3
  1ada1c: 00 00 42 94  	lhu	$2, 0x0($2)
  1ada20: 0c 00 c2 a4  	sh	$2, 0xc($6)
  1ada24: 0e 00 c2 a4  	sh	$2, 0xe($6)
  1ada28: 4d 00 a2 90  	lbu	$2, 0x4d($5)
  1ada2c: 06 00 82 a0  	sb	$2, 0x6($4)
  1ada30: 07 00 82 a0  	sb	$2, 0x7($4)
  1ada34: 08 00 e0 03  	jr	$ra
  1ada38: 00 00 00 00  	nop
  1ada3c: 36 00 02 3c  	lui	$2, 0x36
  1ada40: 2d 48 a0 00  	move	$9, $5
  1ada44: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1ada48: 40 30 04 00  	sll	$6, $4, 0x1
  1ada4c: 40 00 02 8d  	lw	$2, 0x40($8)
  1ada50: 4c 00 05 91  	lbu	$5, 0x4c($8)
  1ada54: 21 38 44 00  	addu	$7, $2, $4
  1ada58: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1ada5c: 00 00 e2 90  	lbu	$2, 0x0($7)
  1ada60: 2b 10 45 00  	sltu	$2, $2, $5
  1ada64: 18 00 40 10  	beqz	$2, 0x1adac8 <.text+0xadac8>
  1ada68: 21 30 66 00  	addu	$6, $3, $6
  1ada6c: 00 00 23 91  	lbu	$3, 0x0($9)
  1ada70: 16 00 60 50  	beqzl	$3, 0x1adacc <.text+0xadacc>
  1ada74: 36 00 03 3c  	lui	$3, 0x36
  1ada78: 24 00 02 8d  	lw	$2, 0x24($8)
  1ada7c: 40 18 03 00  	sll	$3, $3, 0x1
  1ada80: 44 00 04 8d  	lw	$4, 0x44($8)
  1ada84: 42 10 02 00  	srl	$2, $2, 0x1
  1ada88: 21 18 64 00  	addu	$3, $3, $4
  1ada8c: 40 10 02 00  	sll	$2, $2, 0x1
  1ada90: 00 00 63 94  	lhu	$3, 0x0($3)
  1ada94: 21 10 46 00  	addu	$2, $2, $6
  1ada98: 00 00 43 a4  	sh	$3, 0x0($2)
  1ada9c: 02 00 43 a4  	sh	$3, 0x2($2)
  1adaa0: 00 00 c3 a4  	sh	$3, 0x0($6)
  1adaa4: 02 00 c3 a4  	sh	$3, 0x2($6)
  1adaa8: 24 00 02 8d  	lw	$2, 0x24($8)
  1adaac: 4d 00 03 91  	lbu	$3, 0x4d($8)
  1adab0: 42 10 02 00  	srl	$2, $2, 0x1
  1adab4: 21 10 e2 00  	addu	$2, $7, $2
  1adab8: 00 00 43 a0  	sb	$3, 0x0($2)
  1adabc: 01 00 43 a0  	sb	$3, 0x1($2)
  1adac0: 00 00 e3 a0  	sb	$3, 0x0($7)
  1adac4: 01 00 e3 a0  	sb	$3, 0x1($7)
  1adac8: 36 00 03 3c  	lui	$3, 0x36
  1adacc: 02 00 e2 90  	lbu	$2, 0x2($7)
  1adad0: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1adad4: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1adad8: 2b 10 43 00  	sltu	$2, $2, $3
  1adadc: 19 00 40 10  	beqz	$2, 0x1adb44 <.text+0xadb44>
  1adae0: 36 00 03 3c  	lui	$3, 0x36
  1adae4: 01 00 23 91  	lbu	$3, 0x1($9)
  1adae8: 16 00 60 50  	beqzl	$3, 0x1adb44 <.text+0xadb44>
  1adaec: 36 00 03 3c  	lui	$3, 0x36
  1adaf0: 24 00 a2 8c  	lw	$2, 0x24($5)
  1adaf4: 40 18 03 00  	sll	$3, $3, 0x1
  1adaf8: 44 00 a4 8c  	lw	$4, 0x44($5)
  1adafc: 42 10 02 00  	srl	$2, $2, 0x1
  1adb00: 21 18 64 00  	addu	$3, $3, $4
  1adb04: 40 10 02 00  	sll	$2, $2, 0x1
  1adb08: 00 00 63 94  	lhu	$3, 0x0($3)
  1adb0c: 21 10 46 00  	addu	$2, $2, $6
  1adb10: 04 00 43 a4  	sh	$3, 0x4($2)
  1adb14: 06 00 43 a4  	sh	$3, 0x6($2)
  1adb18: 04 00 c3 a4  	sh	$3, 0x4($6)
  1adb1c: 06 00 c3 a4  	sh	$3, 0x6($6)
  1adb20: 24 00 a2 8c  	lw	$2, 0x24($5)
  1adb24: 4d 00 a3 90  	lbu	$3, 0x4d($5)
  1adb28: 42 10 02 00  	srl	$2, $2, 0x1
  1adb2c: 21 10 e2 00  	addu	$2, $7, $2
  1adb30: 02 00 43 a0  	sb	$3, 0x2($2)
  1adb34: 03 00 43 a0  	sb	$3, 0x3($2)
  1adb38: 02 00 e3 a0  	sb	$3, 0x2($7)
  1adb3c: 03 00 e3 a0  	sb	$3, 0x3($7)
  1adb40: 36 00 03 3c  	lui	$3, 0x36
  1adb44: 04 00 e2 90  	lbu	$2, 0x4($7)
  1adb48: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1adb4c: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1adb50: 2b 10 43 00  	sltu	$2, $2, $3
  1adb54: 19 00 40 10  	beqz	$2, 0x1adbbc <.text+0xadbbc>
  1adb58: 36 00 03 3c  	lui	$3, 0x36
  1adb5c: 02 00 23 91  	lbu	$3, 0x2($9)
  1adb60: 16 00 60 50  	beqzl	$3, 0x1adbbc <.text+0xadbbc>
  1adb64: 36 00 03 3c  	lui	$3, 0x36
  1adb68: 24 00 a2 8c  	lw	$2, 0x24($5)
  1adb6c: 40 18 03 00  	sll	$3, $3, 0x1
  1adb70: 44 00 a4 8c  	lw	$4, 0x44($5)
  1adb74: 42 10 02 00  	srl	$2, $2, 0x1
  1adb78: 21 18 64 00  	addu	$3, $3, $4
  1adb7c: 40 10 02 00  	sll	$2, $2, 0x1
  1adb80: 00 00 63 94  	lhu	$3, 0x0($3)
  1adb84: 21 10 46 00  	addu	$2, $2, $6
  1adb88: 08 00 43 a4  	sh	$3, 0x8($2)
  1adb8c: 0a 00 43 a4  	sh	$3, 0xa($2)
  1adb90: 08 00 c3 a4  	sh	$3, 0x8($6)
  1adb94: 0a 00 c3 a4  	sh	$3, 0xa($6)
  1adb98: 24 00 a2 8c  	lw	$2, 0x24($5)
  1adb9c: 4d 00 a3 90  	lbu	$3, 0x4d($5)
  1adba0: 42 10 02 00  	srl	$2, $2, 0x1
  1adba4: 21 10 e2 00  	addu	$2, $7, $2
  1adba8: 04 00 43 a0  	sb	$3, 0x4($2)
  1adbac: 05 00 43 a0  	sb	$3, 0x5($2)
  1adbb0: 04 00 e3 a0  	sb	$3, 0x4($7)
  1adbb4: 05 00 e3 a0  	sb	$3, 0x5($7)
  1adbb8: 36 00 03 3c  	lui	$3, 0x36
  1adbbc: 06 00 e2 90  	lbu	$2, 0x6($7)
  1adbc0: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1adbc4: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1adbc8: 2b 10 43 00  	sltu	$2, $2, $3
  1adbcc: 17 00 40 10  	beqz	$2, 0x1adc2c <.text+0xadc2c>
  1adbd0: 00 00 00 00  	nop
  1adbd4: 03 00 23 91  	lbu	$3, 0x3($9)
  1adbd8: 14 00 60 10  	beqz	$3, 0x1adc2c <.text+0xadc2c>
  1adbdc: 40 18 03 00  	sll	$3, $3, 0x1
  1adbe0: 24 00 a2 8c  	lw	$2, 0x24($5)
  1adbe4: 44 00 a4 8c  	lw	$4, 0x44($5)
  1adbe8: 42 10 02 00  	srl	$2, $2, 0x1
  1adbec: 21 18 64 00  	addu	$3, $3, $4
  1adbf0: 40 10 02 00  	sll	$2, $2, 0x1
  1adbf4: 00 00 63 94  	lhu	$3, 0x0($3)
  1adbf8: 21 10 46 00  	addu	$2, $2, $6
  1adbfc: 0c 00 43 a4  	sh	$3, 0xc($2)
  1adc00: 0e 00 43 a4  	sh	$3, 0xe($2)
  1adc04: 0c 00 c3 a4  	sh	$3, 0xc($6)
  1adc08: 0e 00 c3 a4  	sh	$3, 0xe($6)
  1adc0c: 24 00 a2 8c  	lw	$2, 0x24($5)
  1adc10: 4d 00 a3 90  	lbu	$3, 0x4d($5)
  1adc14: 42 10 02 00  	srl	$2, $2, 0x1
  1adc18: 21 10 e2 00  	addu	$2, $7, $2
  1adc1c: 06 00 43 a0  	sb	$3, 0x6($2)
  1adc20: 07 00 43 a0  	sb	$3, 0x7($2)
  1adc24: 06 00 e3 a0  	sb	$3, 0x6($7)
  1adc28: 07 00 e3 a0  	sb	$3, 0x7($7)
  1adc2c: 08 00 e0 03  	jr	$ra
  1adc30: 00 00 00 00  	nop
  1adc34: 36 00 02 3c  	lui	$2, 0x36
  1adc38: 2d 48 a0 00  	move	$9, $5
  1adc3c: 80 d4 48 24  	addiu	$8, $2, -0x2b80 <.text+0xffffffffffefd480>
  1adc40: 40 30 04 00  	sll	$6, $4, 0x1
  1adc44: 40 00 02 8d  	lw	$2, 0x40($8)
  1adc48: 4c 00 05 91  	lbu	$5, 0x4c($8)
  1adc4c: 21 38 44 00  	addu	$7, $2, $4
  1adc50: 3c 00 03 8d  	lw	$3, 0x3c($8)
  1adc54: 00 00 e2 90  	lbu	$2, 0x0($7)
  1adc58: 2b 10 45 00  	sltu	$2, $2, $5
  1adc5c: 18 00 40 10  	beqz	$2, 0x1adcc0 <.text+0xadcc0>
  1adc60: 21 30 66 00  	addu	$6, $3, $6
  1adc64: 03 00 23 91  	lbu	$3, 0x3($9)
  1adc68: 16 00 60 50  	beqzl	$3, 0x1adcc4 <.text+0xadcc4>
  1adc6c: 36 00 03 3c  	lui	$3, 0x36
  1adc70: 24 00 02 8d  	lw	$2, 0x24($8)
  1adc74: 40 18 03 00  	sll	$3, $3, 0x1
  1adc78: 44 00 04 8d  	lw	$4, 0x44($8)
  1adc7c: 42 10 02 00  	srl	$2, $2, 0x1
  1adc80: 21 18 64 00  	addu	$3, $3, $4
  1adc84: 40 10 02 00  	sll	$2, $2, 0x1
  1adc88: 00 00 63 94  	lhu	$3, 0x0($3)
  1adc8c: 21 10 46 00  	addu	$2, $2, $6
  1adc90: 00 00 43 a4  	sh	$3, 0x0($2)
  1adc94: 02 00 43 a4  	sh	$3, 0x2($2)
  1adc98: 00 00 c3 a4  	sh	$3, 0x0($6)
  1adc9c: 02 00 c3 a4  	sh	$3, 0x2($6)
  1adca0: 24 00 02 8d  	lw	$2, 0x24($8)
  1adca4: 4d 00 03 91  	lbu	$3, 0x4d($8)
  1adca8: 42 10 02 00  	srl	$2, $2, 0x1
  1adcac: 21 10 e2 00  	addu	$2, $7, $2
  1adcb0: 00 00 43 a0  	sb	$3, 0x0($2)
  1adcb4: 01 00 43 a0  	sb	$3, 0x1($2)
  1adcb8: 00 00 e3 a0  	sb	$3, 0x0($7)
  1adcbc: 01 00 e3 a0  	sb	$3, 0x1($7)
  1adcc0: 36 00 03 3c  	lui	$3, 0x36
  1adcc4: 02 00 e2 90  	lbu	$2, 0x2($7)
  1adcc8: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1adccc: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1adcd0: 2b 10 43 00  	sltu	$2, $2, $3
  1adcd4: 19 00 40 10  	beqz	$2, 0x1add3c <.text+0xadd3c>
  1adcd8: 36 00 03 3c  	lui	$3, 0x36
  1adcdc: 02 00 23 91  	lbu	$3, 0x2($9)
  1adce0: 16 00 60 50  	beqzl	$3, 0x1add3c <.text+0xadd3c>
  1adce4: 36 00 03 3c  	lui	$3, 0x36
  1adce8: 24 00 a2 8c  	lw	$2, 0x24($5)
  1adcec: 40 18 03 00  	sll	$3, $3, 0x1
  1adcf0: 44 00 a4 8c  	lw	$4, 0x44($5)
  1adcf4: 42 10 02 00  	srl	$2, $2, 0x1
  1adcf8: 21 18 64 00  	addu	$3, $3, $4
  1adcfc: 40 10 02 00  	sll	$2, $2, 0x1
  1add00: 00 00 63 94  	lhu	$3, 0x0($3)
  1add04: 21 10 46 00  	addu	$2, $2, $6
  1add08: 04 00 43 a4  	sh	$3, 0x4($2)
  1add0c: 06 00 43 a4  	sh	$3, 0x6($2)
  1add10: 04 00 c3 a4  	sh	$3, 0x4($6)
  1add14: 06 00 c3 a4  	sh	$3, 0x6($6)
  1add18: 24 00 a2 8c  	lw	$2, 0x24($5)
  1add1c: 4d 00 a3 90  	lbu	$3, 0x4d($5)
  1add20: 42 10 02 00  	srl	$2, $2, 0x1
  1add24: 21 10 e2 00  	addu	$2, $7, $2
  1add28: 02 00 43 a0  	sb	$3, 0x2($2)
  1add2c: 03 00 43 a0  	sb	$3, 0x3($2)
  1add30: 02 00 e3 a0  	sb	$3, 0x2($7)
  1add34: 03 00 e3 a0  	sb	$3, 0x3($7)
  1add38: 36 00 03 3c  	lui	$3, 0x36
  1add3c: 04 00 e2 90  	lbu	$2, 0x4($7)
  1add40: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1add44: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1add48: 2b 10 43 00  	sltu	$2, $2, $3
  1add4c: 19 00 40 10  	beqz	$2, 0x1addb4 <.text+0xaddb4>
  1add50: 36 00 03 3c  	lui	$3, 0x36
  1add54: 01 00 23 91  	lbu	$3, 0x1($9)
  1add58: 16 00 60 50  	beqzl	$3, 0x1addb4 <.text+0xaddb4>
  1add5c: 36 00 03 3c  	lui	$3, 0x36
  1add60: 24 00 a2 8c  	lw	$2, 0x24($5)
  1add64: 40 18 03 00  	sll	$3, $3, 0x1
  1add68: 44 00 a4 8c  	lw	$4, 0x44($5)
  1add6c: 42 10 02 00  	srl	$2, $2, 0x1
  1add70: 21 18 64 00  	addu	$3, $3, $4
  1add74: 40 10 02 00  	sll	$2, $2, 0x1
  1add78: 00 00 63 94  	lhu	$3, 0x0($3)
  1add7c: 21 10 46 00  	addu	$2, $2, $6
  1add80: 08 00 43 a4  	sh	$3, 0x8($2)
  1add84: 0a 00 43 a4  	sh	$3, 0xa($2)
  1add88: 08 00 c3 a4  	sh	$3, 0x8($6)
  1add8c: 0a 00 c3 a4  	sh	$3, 0xa($6)
  1add90: 24 00 a2 8c  	lw	$2, 0x24($5)
  1add94: 4d 00 a3 90  	lbu	$3, 0x4d($5)
  1add98: 42 10 02 00  	srl	$2, $2, 0x1
  1add9c: 21 10 e2 00  	addu	$2, $7, $2
  1adda0: 04 00 43 a0  	sb	$3, 0x4($2)
  1adda4: 05 00 43 a0  	sb	$3, 0x5($2)
  1adda8: 04 00 e3 a0  	sb	$3, 0x4($7)
  1addac: 05 00 e3 a0  	sb	$3, 0x5($7)
  1addb0: 36 00 03 3c  	lui	$3, 0x36
  1addb4: 06 00 e2 90  	lbu	$2, 0x6($7)
  1addb8: 80 d4 65 24  	addiu	$5, $3, -0x2b80 <.text+0xffffffffffefd480>
  1addbc: 4c 00 a3 90  	lbu	$3, 0x4c($5)
  1addc0: 2b 10 43 00  	sltu	$2, $2, $3
  1addc4: 17 00 40 10  	beqz	$2, 0x1ade24 <.text+0xade24>
  1addc8: 00 00 00 00  	nop
  1addcc: 00 00 23 91  	lbu	$3, 0x0($9)
  1addd0: 14 00 60 10  	beqz	$3, 0x1ade24 <.text+0xade24>
  1addd4: 40 18 03 00  	sll	$3, $3, 0x1
  1addd8: 24 00 a2 8c  	lw	$2, 0x24($5)
  1adddc: 44 00 a4 8c  	lw	$4, 0x44($5)
  1adde0: 42 10 02 00  	srl	$2, $2, 0x1
  1adde4: 21 18 64 00  	addu	$3, $3, $4
  1adde8: 40 10 02 00  	sll	$2, $2, 0x1
  1addec: 00 00 63 94  	lhu	$3, 0x0($3)
  1addf0: 21 10 46 00  	addu	$2, $2, $6
  1addf4: 0c 00 43 a4  	sh	$3, 0xc($2)
  1addf8: 0e 00 43 a4  	sh	$3, 0xe($2)
  1addfc: 0c 00 c3 a4  	sh	$3, 0xc($6)
  1ade00: 0e 00 c3 a4  	sh	$3, 0xe($6)
  1ade04: 24 00 a2 8c  	lw	$2, 0x24($5)
  1ade08: 4d 00 a3 90  	lbu	$3, 0x4d($5)
  1ade0c: 42 10 02 00  	srl	$2, $2, 0x1
  1ade10: 21 10 e2 00  	addu	$2, $7, $2
  1ade14: 06 00 43 a0  	sb	$3, 0x6($2)
  1ade18: 07 00 43 a0  	sb	$3, 0x7($2)
  1ade1c: 06 00 e3 a0  	sb	$3, 0x6($7)
  1ade20: 07 00 e3 a0  	sb	$3, 0x7($7)
  1ade24: 08 00 e0 03  	jr	$ra
  1ade28: 00 00 00 00  	nop
