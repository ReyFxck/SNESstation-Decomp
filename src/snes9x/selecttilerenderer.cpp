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
/* Historical Snes9x 1.42 gfx.cpp selector, with its out-of-line body restored.
 * Archive SHA-256: 136c7c9bf826bf9dba91073aae14e4b315ab78e67eee189322dec2637e949ffb.
 * Only the two observed SGFX fields are exposed; target offsets are 0xabe/0xabf.
 * Assembly names bind historical declarations to existing canonical owners.
 * Full target bytes are checked by tools/run-selecttilerenderer-source-recovery.py.
 */
typedef unsigned char bool8;
struct SGFXSelector {
    unsigned char prefix[0xabe];
    unsigned char r2130;
    unsigned char r2131;
};
extern SGFXSelector GFX __asm__("DAT_0035d480");
typedef void (*TileRenderer)();
extern TileRenderer DrawTilePtr __asm__("DAT_0035f98c");
extern TileRenderer DrawClippedTilePtr __asm__("DAT_0035f990");
extern TileRenderer DrawLargePixelPtr __asm__("DAT_0035f99c");
extern "C" void DrawTile16() __asm__("_Z10DrawTile16jjjj");
extern "C" void DrawClippedTile16() __asm__("_Z17DrawClippedTile16jjjjjj");
extern "C" void DrawLargePixel16() __asm__("_Z16DrawLargePixel16jjjjjj");
extern "C" void DrawTile16Add() __asm__("_Z13DrawTile16Addjjjj");
extern "C" void DrawClippedTile16Add() __asm__("_Z20DrawClippedTile16Addjjjjjj");
extern "C" void DrawLargePixel16Add() __asm__("_Z19DrawLargePixel16Addjjjjjj");
extern "C" void DrawTile16Add1_2() __asm__("_Z16DrawTile16Add1_2jjjj");
extern "C" void DrawClippedTile16Add1_2() __asm__("_Z23DrawClippedTile16Add1_2jjjjjj");
extern "C" void DrawLargePixel16Add1_2() __asm__("_Z22DrawLargePixel16Add1_2jjjjjj");
extern "C" void DrawTile16Sub() __asm__("_Z13DrawTile16Subjjjj");
extern "C" void DrawClippedTile16Sub() __asm__("_Z20DrawClippedTile16Subjjjjjj");
extern "C" void DrawLargePixel16Sub() __asm__("_Z19DrawLargePixel16Subjjjjjj");
extern "C" void DrawTile16Sub1_2() __asm__("_Z16DrawTile16Sub1_2jjjj");
extern "C" void DrawClippedTile16Sub1_2() __asm__("_Z23DrawClippedTile16Sub1_2jjjjjj");
extern "C" void DrawLargePixel16Sub1_2() __asm__("_Z22DrawLargePixel16Sub1_2jjjjjj");
extern "C" void DrawTile16FixedAdd1_2() __asm__("_Z21DrawTile16FixedAdd1_2jjjj");
extern "C" void DrawClippedTile16FixedAdd1_2() __asm__("_Z28DrawClippedTile16FixedAdd1_2jjjjjj");
extern "C" void DrawTile16FixedSub1_2() __asm__("_Z21DrawTile16FixedSub1_2jjjj");
extern "C" void DrawClippedTile16FixedSub1_2() __asm__("_Z28DrawClippedTile16FixedSub1_2jjjjjj");

/* 0x001ac838 */
void SelectTileRenderer (bool8 normal)
{
    if (normal)
    {
	DrawTilePtr = DrawTile16;
	DrawClippedTilePtr = DrawClippedTile16;
	DrawLargePixelPtr = DrawLargePixel16;
    }
    else
    {
	if (GFX.r2131 & 0x80)
	{
	    if (GFX.r2131 & 0x40)
	    {
		if (GFX.r2130 & 2)
		{
		    DrawTilePtr = DrawTile16Sub1_2;
		    DrawClippedTilePtr = DrawClippedTile16Sub1_2;
		}
		else
		{
		    // Fixed colour substraction
		    DrawTilePtr = DrawTile16FixedSub1_2;
		    DrawClippedTilePtr = DrawClippedTile16FixedSub1_2;
		}
		DrawLargePixelPtr = DrawLargePixel16Sub1_2;
	    }
	    else
	    {
		DrawTilePtr = DrawTile16Sub;
		DrawClippedTilePtr = DrawClippedTile16Sub;
		DrawLargePixelPtr = DrawLargePixel16Sub;
	    }
	}
	else
	{
	    if (GFX.r2131 & 0x40)
	    {
		if (GFX.r2130 & 2)
		{
		    DrawTilePtr = DrawTile16Add1_2;
		    DrawClippedTilePtr = DrawClippedTile16Add1_2;
		}
		else
		{
		    // Fixed colour addition
		    DrawTilePtr = DrawTile16FixedAdd1_2;
		    DrawClippedTilePtr = DrawClippedTile16FixedAdd1_2;
		}
		DrawLargePixelPtr = DrawLargePixel16Add1_2;
	    }
	    else
	    {
		DrawTilePtr = DrawTile16Add;
		DrawClippedTilePtr = DrawClippedTile16Add;
		DrawLargePixelPtr = DrawLargePixel16Add;
	    }
	}
    }
}
