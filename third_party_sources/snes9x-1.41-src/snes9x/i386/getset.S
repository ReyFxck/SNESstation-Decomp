/*
 * Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
 *
 * (c) Copyright 1996 - 2001 Gary Henderson (gary.henderson@ntlworld.com) and
 *                           Jerremy Koot (jkoot@snes9x.com)
 *
 * Super FX C emulator code 
 * (c) Copyright 1997 - 1999 Ivar (ivar@snes9x.com) and
 *                           Gary Henderson.
 * Super FX assembler emulator code (c) Copyright 1998 zsKnight and _Demo_.
 *
 * DSP1 emulator code (c) Copyright 1998 Ivar, _Demo_ and Gary Henderson.
 * C4 asm and some C emulation code (c) Copyright 2000 zsKnight and _Demo_.
 * C4 C code (c) Copyright 2001 Gary Henderson (gary.henderson@ntlworld.com).
 *
 * DOS port code contains the works of other authors. See headers in
 * individual files.
 *
 * Snes9x homepage: http://www.snes9x.com
 *
 * Permission to use, copy, modify and distribute Snes9x in both binary and
 * source form, for non-commercial purposes, is hereby granted without fee,
 * providing that this license information and copyright notice appear with
 * all copies and any derived work.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event shall the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Snes9x is freeware for PERSONAL USE only. Commercial users should
 * seek permission of the copyright holders first. Commercial use includes
 * charging money for Snes9x or software derived from Snes9x.
 *
 * The copyright holders request that bug fixes and improvements to the code
 * should be forwarded to them so everyone can benefit from the modifications
 * in future versions.
 *
 * Super NES and Super Nintendo Entertainment System are trademarks of
 * Nintendo Co., Limited and its subsidiary companies.
 */
.data
.BankSave:
	.long 0
.text

.globl S9xGetByte

S9xGetByte:
	movl %edx, %eax
	shrl $MEMMAP_SHIFT, %eax
	and $MEMMAP_MASK, %eax
#ifdef VAR_CYCLES
	movb MemorySpeed(%eax), %cl
#endif
#ifdef CPU_SHUTDOWN
	movb BlockIsRAM(%eax), %ch
#endif	
	movl Map(, %eax, 4), %eax
	cmpl $17, %eax
	jbe .GBSpecial
	andl $0xffff, %edx
#ifdef CPU_SHUTDOWN
	testb %ch, %ch
	jz .GBNotRAM
	pushl %eax
	movl PCAtOpcodeStart, %eax
	movl %eax, WaitAddress
	popl %eax
.GBNotRAM:
#endif
#ifdef VAR_CYCLES
	andl $0xff, %ecx
	addl %ecx, CYCLES
#endif
	movb (%eax, %edx), %al
	ret

	.align 4
.GBSpecial:
	jmp *.GBJmpTable(, %eax, 4)
.data
	.align 4
.GBJmpTable:
	.long .GBPPU
	.long .GBCPU
	.long .GBDSP
	.long .GBLSRAM
	.long .GBHSRAM
	.long .GBNONE
	.long .GBDEBUG
	.long .GBC4
	.long .GBBWRAM
	.long .GBNONE
	.long .GBNONE
	.long .GBNONE
	.long .GB7ROM
	.long .GB7RAM
	.long .GB7SRM
	.long .GBOBC1
	.long .GBSDSP
	.long .GBSRISC

.text
.GBPPU: /* MAP_PPU */
#ifdef VAR_CYCLES
	movb InDMA, %al
	testb %al, %al
	jne .GBPPU_NOADD
	addl $6, CYCLES
.GBPPU_NOADD:
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetPPU
	popl %edx
	LOAD_CYCLES
	ret

.GBCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	addl $6, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetCPU
	popl %edx
	LOAD_CYCLES
	ret

.GBDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetDSP
	LOAD_CYCLES
	popl %edx
	ret

.GBLSRAM: /* MAP_LOROM_SRAM */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
	movb (%ecx, %edx), %al
	ret

.GB7SRM:
.GBHSRAM: /* MAP_HIROM_SRAM */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAM, %eax
	andl SRAMMask, %edx
	movb (%eax, %edx), %al
	ret

.GBNONE:
	xorl %eax, %eax
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	movb OpenBus, %al
	ret

.GBDEBUG: /* MAP_DEBUG */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	xorl %eax, %eax
	movb OpenBus, %al
	ret

.GBC4: /* MAP_C4 */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetC4
	popl %edx
	LOAD_CYCLES
	ret

.GBBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	andl $0x7fff, %edx
	addl $-24576, %edx
	movl BWRAM, %eax
	movb (%eax, %edx), %al
	ret

.GB7ROM:
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetSPC7110Byte
	popl %edx
	LOAD_CYCLES
	ret

.GB7RAM:
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	SAVE_CYCLES
	movl $0x4800, %edx
	pushl %edx
	ccall S9xGetSPC7110
	popl %edx
	LOAD_CYCLES
	ret

.GBOBC1:
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall GetOBC1
	popl %edx
	LOAD_CYCLES
	ret

.GBSDSP:
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	pushl %edx
	ccall S9xGetSetaDSP
	popl %edx
	ret

.GBSRISC:
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	pushl %edx
	ccall S9xGetST018
	popl %edx
	ret


.globl S9xGetWord

S9xGetWord:
	movl %edx, %eax
	/*MK: blocks can be 0x1000 bytes, not a 0x2000 minimum*/
	andl $0x0fff, %eax
	cmpl $0x0fff, %eax
	/*MK*/
	jne .GWNotAtBlockBoundary
	pushl %edx
	call S9xGetByte
	popl %edx
        movb %al, OpenBus
	incl %edx
	call S9xGetByte
        movb %al, %ah
	movb OpenBus, %al
	ret

.GWNotAtBlockBoundary:
	movl %edx, %eax
	shrl $MEMMAP_SHIFT, %eax
	and $MEMMAP_MASK, %eax
#ifdef VAR_CYCLES
	movb MemorySpeed(%eax), %cl
#endif
#ifdef CPU_SHUTDOWN
	movb BlockIsRAM(%eax), %ch
#endif	
	movl Map(, %eax, 4), %eax
	cmpl $17, %eax
	jbe .GWSpecial
	andl $0xffff, %edx
#ifdef CPU_SHUTDOWN
	testb %ch, %ch
	jz .GWNotRAM
	pushl %eax
	movl PCAtOpcodeStart, %eax
	movl %eax, WaitAddress
	popl %eax
.GWNotRAM:
#endif	
#ifdef VAR_CYCLES
	andl $0xff, %ecx
	addl %ecx, CYCLES
	addl %ecx, CYCLES
#endif
	movw (%eax, %edx), %ax
	ret

	.align 4
.GWSpecial:
	jmp *.GWJmpTable(, %eax, 4)

.data
	.align 4
.GWJmpTable:
	.long .GWPPU
	.long .GWCPU
	.long .GWDSP
	.long .GWLSRAM
	.long .GWHSRAM
	.long .GWNONE
	.long .GWDEBUG
	.long .GWC4
	.long .GWBWRAM
	.long .GWNONE
	.long .GWNONE
	.long .GWNONE
	.long .GW7ROM
	.long .GW7RAM
	.long .GW7SRM
	.long .GWOBC1
	.long .GWSDSP
	.long .GWSRISC

.text
.GWPPU: /* MAP_PPU */
#ifdef VAR_CYCLES
	movb InDMA, %al
	testb %al, %al
	jne .GWPPU_NOADD
	addl $12, CYCLES
.GWPPU_NOADD:
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetPPU
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetPPU
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetCPU
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetCPU
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetDSP
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetDSP
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWLSRAM: /* MAP_LOROM_SRAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
	movb (%ecx, %edx), %al
	incl %edx
	andl SRAMMask, %edx
	movb (%ecx, %edx), %ah
	ret

.GW7SRM:
.GWHSRAM: /* MAP_HIROM_SRAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAM, %eax
	andl SRAMMask, %edx
	movw (%eax, %edx), %ax
	ret

.GWNONE:
	xorl %eax, %eax
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	movb OpenBus, %ah
	movb OpenBus, %al
	ret

.GWDEBUG: /* MAP_DEBUG */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	xorl %eax, %eax
	movb OpenBus, %ah
	movb OpenBus, %al
	ret

.GWC4:
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetC4
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetC4
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	andl $0x7fff, %edx
	addl $-24576, %edx
	movl BWRAM, %eax
	movw (%eax, %edx), %ax
	ret

.GW7ROM:
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetSPC7110Byte
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetSPC7110Byte
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GW7RAM:
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	SAVE_CYCLES
	movl $0x4800, %edx
	pushl %edx
	ccall S9xGetSPC7110
	popl %edx
	pushl %eax
	pushl %edx
	ccall S9xGetSPC7110
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWOBC1: /* MAP_OBC_RAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall GetOBC1
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall GetOBC1
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWSDSP: /* MAP_OBC_RAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetSetaDSP
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetSetaDSP
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret

.GWSRISC: /* MAP_OBC_RAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	SAVE_CYCLES
	pushl %edx
	ccall S9xGetST018
	popl %edx
	pushl %eax
	incl %edx
	pushl %edx
	ccall S9xGetST018
	LOAD_CYCLES
	popl %edx
	popl %edx
	movb %al, %ah
	movb %dl, %al
	ret


.globl S9xSetByte
S9xSetByte:
	pushl %eax
#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif	
	movl %edx, %eax
	shrl $MEMMAP_SHIFT, %eax
	and $MEMMAP_MASK, %eax
#ifdef VAR_CYCLES
	movb MemorySpeed(%eax), %cl
#endif
	movl WriteMap(, %eax, 4), %eax
	cmpl $17, %eax
	jbe .SBSpecial
	andl $0xffff, %edx
#ifdef VAR_CYCLES
	andl $0xff, %ecx
	addl %ecx, CYCLES
#endif
#ifdef CPU_SHUTDOWN
	addl %eax, %edx
	cmpl SA1WaitByteAddress1, %edx
	jz .Matched
	cmpl SA1WaitByteAddress2, %edx
	jnz .NoMatch
.Matched:
	cmpl $0, SA1Opcodes
	movl $0, SA1WaitCounter
	setnz %al
	movb %al, SA1Executing
.NoMatch:
	popl %ecx
	movb %cl, (%edx)
#else
	popl %ecx
	movb %cl, (%eax, %edx)
#endif
	ret

.SBSpecial:
	jmp *.SBJmpTable(, %eax, 4)
.data
	.align 4
.SBJmpTable:
	.long .SBPPU
	.long .SBCPU
	.long .SBDSP
	.long .SBLSRAM
	.long .SBHSRAM
	.long .SBNONE
	.long .SBDEBUG
	.long .SBC4
	.long .SBBWRAM
	.long .SBNONE
	.long .SBNONE
	.long .SBNONE
	.long .SBNONE
	.long .SBNONE
	.long .SBNONE
	.long .SBOBC1
	.long .SBSDSP
	.long .SBSRISC

.text
.SBPPU: /* MAP_PPU */
#ifdef VAR_CYCLES
	movb InDMA, %al
	testb %al, %al
	jne .SBPPU_NOADD
	addl $6, CYCLES
.SBPPU_NOADD:
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetPPU
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SBCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	addl $6, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetCPU
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SBDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	SAVE_CYCLES
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetDSP
	LOAD_CYCLES
	addl $8, %esp
	ret

.SBLSRAM: /* MAP_LOROM_SRAM */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	popl %eax
	movl SRAMMask, %ecx
	orl %ecx, %ecx
	jz .SBLSRAM_SKIP
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
	movb %al, (%ecx, %edx)
	movb $1, SRAMModified
.SBLSRAM_SKIP:
	ret

.SBHSRAM: /* MAP_HIROM_SRAM */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	popl %eax
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAMMask, %ecx
	orl %ecx, %ecx
	jz .SBHSRAM_SKIP
	andl %ecx, %edx
	movl SRAM, %ecx
	movb %al, (%ecx, %edx)
	movb $1, SRAMModified
.SBHSRAM_SKIP:
	ret

.SBNONE:
.SBDEBUG: /* MAP_DEBUG */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	popl %eax
	ret

.SBC4: /* MAP_C4 */
#ifdef VAR_CYCLES
	addl $6, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetC4
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SBBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	addl $8, CYCLES
#endif
	andl $0x7fff, %edx
	popl %eax
	addl $-24576, %edx
	movl BWRAM, %ecx
	movb %al, (%ecx, %edx)
	movb $1, SRAMModified
	ret

.SBOBC1:
#ifdef VAR_CYCLES
	addl $6, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall SetOBC1
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SBSDSP:
#ifdef VAR_CYCLES
	addl $6, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetSetaDSP
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SBSRISC:
#ifdef VAR_CYCLES
	addl $6, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetST018
	LOAD_REGISTERS
	addl $8, %esp
	ret

.globl S9xSetWord

S9xSetWord:
	pushl %eax

	movl %edx, %eax
	andl $0x0fff, %eax
	cmpl $0x0fff, %eax
	jne .SWNotAtBlockBoundary
	xorl %eax, %eax
	pushl %edx
	xorl %eax, %eax
	movb 4(%esp), %al
	call S9xSetByte
	popl %edx
	xorl %eax, %eax
	incl %edx
	movb 1(%esp), %al
	call S9xSetByte
	popl %ecx
	ret

.SWNotAtBlockBoundary:

#ifdef CPU_SHUTDOWN
	movl $0, WaitAddress
#endif	
	movl %edx, %eax
	shrl $MEMMAP_SHIFT, %eax
	and $MEMMAP_MASK, %eax
#ifdef VAR_CYCLES
	movb MemorySpeed(%eax), %cl
#endif
	movl WriteMap(, %eax, 4), %eax
	cmpl $17, %eax
	jbe .SWSpecial
	andl $0xffff, %edx
#ifdef VAR_CYCLES
	andl $0xff, %ecx
	addl %ecx, CYCLES
	addl %ecx, CYCLES
#endif
#ifdef CPU_SHUTDOWN
	addl %eax, %edx
	cmpl SA1WaitByteAddress1, %edx
	jz .Matched2
	cmpl SA1WaitByteAddress2, %edx
	jnz .NoMatch2
.Matched2:
	cmpl $0, SA1Opcodes
	movl $0, SA1WaitCounter
	setnz %al
	movb %al, SA1Executing
.NoMatch2:
	popl %ecx
	movw %cx, (%edx)
#else
	popl %ecx
	movw %cx, (%eax, %edx)
#endif
	ret

.SWSpecial:
	jmp *.SWJmpTable(, %eax, 4)
.data
	.align 4
.SWJmpTable:
	.long .SWPPU
	.long .SWCPU
	.long .SWDSP
	.long .SWLSRAM
	.long .SWHSRAM
	.long .SWNONE
	.long .SWDEBUG
	.long .SWC4
	.long .SWBWRAM
	.long .SWNONE
	.long .SWNONE
	.long .SWNONE
	.long .SWNONE
	.long .SWNONE
	.long .SWNONE
	.long .SWOBC1
	.long .SWSDSP
	.long .SWSRISC

.text
.SWPPU: /* MAP_PPU */
#ifdef VAR_CYCLES
	movb InDMA, %al
	testb %al, %al
	jne .SWPPU_NOADD
	addl $12, CYCLES
.SWPPU_NOADD:
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetPPU
	popl %eax
	popl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetPPU
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SWCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetCPU
	popl %eax
	popl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetCPU
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SWDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	SAVE_CYCLES
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetDSP
	popl %eax
	popl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetDSP
	LOAD_CYCLES
	addl $8, %esp
	ret

.SWLSRAM: /* MAP_LOROM_SRAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	popl %eax
	movl SRAMMask, %ecx
	orl %ecx, %ecx
	jz .SWLSRAM_SKIP
	movl %edx, %ecx
	andl $0x7fff, %edx
	andl $0x00ff0000, %ecx
	sarl $1, %ecx
	orl  %ecx, %edx
	movl SRAM, %ecx
	andl SRAMMask, %edx
	movb %al, (%ecx, %edx)
	incl %edx
	andl SRAMMask, %edx
	movb %ah, (%ecx, %edx)
	movb $1, SRAMModified
.SWLSRAM_SKIP:
	ret

.SWHSRAM: /* MAP_HIROM_SRAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	popl %eax
	movl %edx, %ecx
	andl $0xf0000, %ecx
	andl $0x7fff, %edx
	sarl $3, %ecx
	addl $-24576, %edx
	addl %ecx, %edx
	movl SRAMMask, %ecx
	orl %ecx, %ecx
	jz .SWHSRAM_SKIP
	andl %ecx, %edx
	movl SRAM, %ecx
	movw %ax, (%ecx, %edx)
	movb $1, SRAMModified
.SWHSRAM_SKIP:
	ret

.SWNONE:
.SWDEBUG: /* MAP_DEBUG */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	popl %eax
	ret

.SWC4: /* MAP_C4 */
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetC4
	popl %eax
	popl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetC4
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SWBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	addl $16, CYCLES
#endif
	andl $0x7fff, %edx
	popl %eax
	addl $-24576, %edx
	movl BWRAM, %ecx
	movw %ax, (%ecx, %edx)
	movb $1, SRAMModified
	ret

.SWOBC1: /* MAP_OBC1 */
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall SetOBC1
	popl %eax
	popl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall SetOBC1
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SWSDSP: /* MAP_OBC1 */
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetSetaDSP
	popl %eax
	popl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetSetaDSP
	LOAD_REGISTERS
	addl $8, %esp
	ret

.SWSRISC: /* MAP_OBC1 */
#ifdef VAR_CYCLES
	addl $12, CYCLES
#endif
	STORE_REGISTERS
	popl %eax
	pushl %edx
	pushl %eax
	ccall S9xSetST018
	popl %eax
	popl %edx
	movb %ah, %al
	incl %edx
	pushl %edx
	pushl %eax
	ccall S9xSetST018
	LOAD_REGISTERS
	addl $8, %esp
	ret


.globl S9xSetPCBase

S9xSetPCBase:
	movl %edx, %eax
	shrl $MEMMAP_SHIFT, %eax
	and $MEMMAP_MASK, %eax
#ifdef VAR_CYCLES
	movb MemorySpeed(%eax), %cl
#endif
	movl Map(, %eax, 4), %eax
	cmpl $11, %eax
	jbe .SPCSpecial
	andl $0xffff, %edx
#ifdef VAR_CYCLES
	andl $0xff, %ecx
	movl %ecx, MemSpeed
	addl %ecx, %ecx
	movl %ecx, MemSpeedx2
#endif
	movl %eax, PCBase
	addl %edx, %eax
	movl %eax, PC
	ret

	.align 4
.SPCSpecial:
	jmp *.SPCJmpTable(, %eax, 4)
.data
	.align 4
.SPCJmpTable:
	.long .SPCPPU
	.long .SPCCPU
	.long .SPCDSP
	.long .SPCLSRAM
	.long .SPCHSRAM
	.long .SPCNONE
	.long .SPCDEBUG
	.long .SPCC4
	.long .SPCBWRAM
	.long .SPCNONE
	.long .SPCNONE
	.long .SPCNONE

.text
.SPCPPU: /* MAP_PPU */
#ifdef VAR_CYCLES
	movl $6, MemSpeed
	movl $12, MemSpeedx2
#endif
	movl FillRAM, %ecx
	andl $0xffff, %edx
	movl %ecx, PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCCPU: /* MAP_CPU */
#ifdef VAR_CYCLES
	movl $6, MemSpeed
	movl $12, MemSpeedx2
#endif
	movl FillRAM, %ecx
	andl $0xffff, %edx
	movl %ecx, PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCDSP: /* MAP_DSP */
#ifdef VAR_CYCLES
	movl $8, MemSpeed
	movl $16, MemSpeedx2
#endif
	movl FillRAM, %ecx
	andl $0xffff, %edx
	movl %ecx, PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCLSRAM: /* MAP_LOROM_SRAM */
#ifdef VAR_CYCLES
	movl $8, MemSpeed
	movl $16, MemSpeedx2
#endif
	movl SRAM, %ecx
	andl $0xffff, %edx
	movl %ecx, PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCHSRAM: /* MAP_HIROM_SRAM */
#ifdef VAR_CYCLES
	movl $8, MemSpeed
	movl $16, MemSpeedx2
#endif
	movl SRAM, %eax
	andl $0xffff, %edx
	addl $-24576, %eax
	movl %eax, PCBase
	addl %eax, %edx
	movl %edx, PC
	ret

.SPCNONE:
.SPCDEBUG: /* MAP_DEBUG */
#ifdef VAR_CYCLES
	movl $8, MemSpeed
	movl $16, MemSpeedx2
#endif
	movl SRAM, %eax
	andl $0xffff, %edx
	movl %eax, PCBase
	addl %eax, %edx
	movl %edx, PC
	ret

.SPCC4: /* MAP_C4 */
#ifdef VAR_CYCLES
	movl $8, MemSpeed
	movl $16, MemSpeedx2
#endif
	movl C4RAM, %ecx
	andl $0xffff, %edx
	addl $-0x6000, %ecx
	movl %ecx, PCBase
	addl %edx, %ecx
	movl %ecx, PC
	ret

.SPCBWRAM: /* MAP_BWRAM */
#ifdef VAR_CYCLES
	movl $8, MemSpeed
	movl $16, MemSpeedx2
#endif
	movl SRAM, %eax
	andl $0xffff, %edx
	addl $-24576, %eax
	movl %eax, PCBase
	addl %eax, %edx
	movl %edx, PC
	ret
