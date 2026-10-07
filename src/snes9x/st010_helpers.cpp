/*******************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2003 Gary Henderson (gary.henderson@ntlworld.com) and
                            Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2003 Matthew Kendora and
                            Brad Jorsch (anomie@users.sourceforge.net)



  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003 zsKnight (zsknight@zsnes.com),
                            _Demo_ (_demo_@zsnes.com), and
                            Nach (n-a-c-h@users.sourceforge.net)

  C4 C++ code
  (c) Copyright 2003 Brad Jorsch

  DSP-1 emulator code
  (c) Copyright 1998 - 2003 Ivar (ivar@snes9x.com), _Demo_, Gary Henderson,
                            John Weidman (jweidman@slip.net),
                            neviksti (neviksti@hotmail.com), and
                            Kris Bleakley (stinkfish@bigpond.com)

  DSP-2 emulator code
  (c) Copyright 2003 Kris Bleakley, John Weidman, neviksti, Matthew Kendora, and
                     Lord Nightmare (lord_nightmare@users.sourceforge.net

  OBC1 emulator code
  (c) Copyright 2001 - 2003 zsKnight, pagefault (pagefault@zsnes.com)
  Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002 Matthew Kendora with research by
                     zsKnight, John Weidman, and Dark Force

  S-RTC C emulator code
  (c) Copyright 2001 John Weidman

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003 zsKnight, _Demo_, and pagefault

  Super FX C emulator code
  (c) Copyright 1997 - 1999 Ivar and Gary Henderson.




  Specific ports contains the works of other authors. See headers in
  individual files.

  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and distribute Snes9x in both binary and
  source form, for non-commercial purposes, is hereby granted without fee,
  providing that this license information and copyright notice appear with
  all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes
  charging money for Snes9x or software derived from Snes9x.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
*******************************************************************************/
/* Historical Snes9x 1.41-1 SETA/ST010 helpers. Archive SHA-256:
 * 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Existing target Memory and SETA function-pointer slots are shared.
 * The target uses -fshort-double: sin/cos have the single-precision ABI.
 */

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef short int16;
typedef int int32;
struct HistoricalSetaMemory { uint8 *RAM, *ROM, *VRAM, *SRAM; };
extern HistoricalSetaMemory Memory __asm__("DAT_0034e2b0");
extern "C" { int printf(const char *, ...); double sin(double) __asm__("sinf_001a0024"); double cos(double) __asm__("cosf_0019fddc"); }
#define PI 3.1415926535897932384626433832795
extern "C" uint8 S9xGetST010(uint32);
/* 0x0016fc90 -- historical S9xGetST010. */
uint8 S9xGetST010(uint32 Address)
{
	printf("Read %06X\n", Address);
		return Memory.SRAM[Address&0xFFFF];
}

/*
 *
 Inputs:
 *
 *   scale value: 680004-680005
 *   x:           680002-680003
 *   y:           680000-680001
 *
 * Outputs:
 *
 *   x_scaled: 680014-680017
 *   y_scaled: 680010-680013
 */
/* 0x0016fcd4 -- historical St010_Op03. */
void St010_Op03(int16 s, int16 x, int16 y, int32 &xs, int32 &ys )
{
	xs = (int32) x * (int32) s * 2;
	ys = (int32) y * (int32) s * 2;
	return;
}

/* 0x0016fd08 -- historical St010_Rotate. */
void St010_Rotate(short Theta, short X0, short Y0, short &X1, short &Y1)
{
	X1 =(short)( ((sin(((short)Theta) * ((float)PI/0x8000))*Y0)*2)  +  ((cos(((short)Theta) * ((float)PI/0x8000))*X0)*2));
	Y1 =(short)( ((cos(((short)Theta) * ((float)PI/0x8000))*Y0)*2) - 	((sin(((short)Theta) * ((float)PI/0x8000))*X0)*2));
}




/* Alias compiler-generated math libcalls without changing instructions. */
#if defined(PS2_EE)
__asm__(".equ sin, sinf_001a0024");
__asm__(".equ cos, cosf_0019fddc");
#endif
