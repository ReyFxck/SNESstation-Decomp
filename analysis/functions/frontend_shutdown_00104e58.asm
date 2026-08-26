# SNES Station v0.23 WIP — frontend shutdown
# Target VA: 0x00104e58..0x00104e7f
# Extracted from the independently unpacked target image.

  104e58: 35 00 04 3c  	lui	$4, 0x35
  104e5c: f0 ff bd 27  	addiu	$sp, $sp, -0x10
  104e60: 00 00 bf ff  	sd	$ra, 0x0($sp)
  104e64: cc 44 05 0c  	jal	0x151330 <.text+0x51330>
  104e68: b0 e2 84 24  	addiu	$4, $4, -0x1d50 <.text+0xffffffffffefe2b0>
  104e6c: 2f 2a 04 0c  	jal	0x10a8bc <.text+0xa8bc>
  104e70: 00 00 00 00  	nop
  104e74: 73 71 06 0c  	jal	0x19c5cc <.text+0x9c5cc>
  104e78: 01 00 04 24  	addiu	$4, $zero, 0x1
  104e7c: 00 00 00 00  	nop
