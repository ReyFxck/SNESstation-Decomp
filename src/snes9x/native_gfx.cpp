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

/*****************************************************************************/
/* CPU-S9xOpcodes.CPP                                                                            */
/* This file contains all the opcodes                                                         */
/*****************************************************************************/


/* Original native gfx routines; archive 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b. */
/* Native recovery proof: analysis/functions/native_gfx_exact_55256.tsv
 * 28 routines / 55256 new linked historical instruction bytes.
 * Frozen 1041-function audit entries (including original inline helpers):
 * 0x00142a78 S9xGraphicsInit (1436 bytes)
 * 0x0014308c S9xBuildDirectColourMaps (144 bytes)
 * 0x00143390 RenderLine (284 bytes)
 * 0x0014368c _Z11S9xSetupOBJv (332 bytes)
 * 0x001437d8 _Z8DrawOBJShh (1700 bytes)
 * 0x00143e7c _Z20DrawBackgroundMosaicjjhh (1392 bytes)
 * 0x001443ec _Z20DrawBackgroundOffsetjjhh (1772 bytes)
 * 0x00144ad8 _Z19DrawBackgroundMode5jjhh (2008 bytes)
 * 0x001452b0 _Z14DrawBackgroundjjhh (2392 bytes)
 * 0x00145c08 _Z21DrawBGMode7BackgroundPhi (1440 bytes)
 * 0x001461a8 _Z23DrawBGMode7Background16Phi (1400 bytes)
 * 0x00146720 _Z26DrawBGMode7Background16AddPhi (1868 bytes)
 * 0x00146e6c _Z29DrawBGMode7Background16Add1_2Phi (1804 bytes)
 * 0x00147578 _Z26DrawBGMode7Background16SubPhi (1776 bytes)
 * 0x00147c68 _Z29DrawBGMode7Background16Sub1_2Phi (1744 bytes)
 * 0x00148338 _Z13Q_INTERPOLATEjjjj (92 bytes)
 * 0x00148394 _Z25DrawBGMode7Background16_iPhi (4116 bytes)
 * 0x001493a8 _Z28DrawBGMode7Background16Add_iPhi (5084 bytes)
 * 0x0014a784 _Z31DrawBGMode7Background16Add1_2_iPhi (4920 bytes)
 * 0x0014babc _Z28DrawBGMode7Background16Sub_iPhi (4968 bytes)
 * 0x0014ce24 _Z31DrawBGMode7Background16Sub1_2_iPhi (4892 bytes)
 * 0x0014e140 _Z12RenderScreenPhhhh (1768 bytes)
 * 0x0014e828 _Z11DisplayCharPhh (420 bytes)
 * 0x0014eaa4 _Z16S9xDisplayStringPKc (432 bytes)
 * 0x0014ec54 S9xUpdateScreen (6712 bytes)
 */
#include "native_gfx_state.h"

bool8 S9xGraphicsInit ()
{
    register uint32 PixelOdd = 1;
    register uint32 PixelEven = 2;

#ifdef GFX_MULTI_FORMAT
    if (GFX.BuildPixel == NULL)
	S9xSetRenderPixelFormat (RGB565);
#endif

    for (uint8 bitshift = 0; bitshift < 4; bitshift++)
    {
	for (register char i = 0; i < 16; i++)
	{
	    register uint32 h = 0;
	    register uint32 l = 0;

#if defined(LSB_FIRST)
	    if (i & 8)
		h |= PixelOdd;
	    if (i & 4)
		h |= PixelOdd << 8;
	    if (i & 2)
		h |= PixelOdd << 16;
	    if (i & 1)
		h |= PixelOdd << 24;
	    if (i & 8)
		l |= PixelOdd;
	    if (i & 4)
		l |= PixelOdd << 8;
	    if (i & 2)
		l |= PixelOdd << 16;
	    if (i & 1)
		l |= PixelOdd << 24;
#else
	    if (i & 8)
		h |= (PixelOdd << 24);
	    if (i & 4)
		h |= (PixelOdd << 16);
	    if (i & 2)
		h |= (PixelOdd << 8);
	    if (i & 1)
		h |= PixelOdd;
	    if (i & 8)
		l |= (PixelOdd << 24);
	    if (i & 4)
		l |= (PixelOdd << 16);
	    if (i & 2)
		l |= (PixelOdd << 8);
	    if (i & 1)
		l |= PixelOdd;
#endif

	    odd_high[bitshift][i] = h;
	    odd_low[bitshift][i] = l;
	    h = l = 0;

#if defined(LSB_FIRST)
	    if (i & 8)
		h |= PixelEven;
	    if (i & 4)
		h |= PixelEven << 8;
	    if (i & 2)
		h |= PixelEven << 16;
	    if (i & 1)
		h |= PixelEven << 24;
	    if (i & 8)
		l |= PixelEven;
	    if (i & 4)
		l |= PixelEven << 8;
	    if (i & 2)
		l |= PixelEven << 16;
	    if (i & 1)
		l |= PixelEven << 24;
#else
	    if (i & 8)
		h |= (PixelEven << 24);
	    if (i & 4)
		h |= (PixelEven << 16);
	    if (i & 2)
		h |= (PixelEven << 8);
	    if (i & 1)
		h |= PixelEven;
	    if (i & 8)
		l |= (PixelEven << 24);
	    if (i & 4)
		l |= (PixelEven << 16);
	    if (i & 2)
		l |= (PixelEven << 8);
	    if (i & 1)
		l |= PixelEven;
#endif

	    even_high[bitshift][i] = h;
	    even_low[bitshift][i] = l;
	}
	PixelEven <<= 2;
	PixelOdd <<= 2;
    }

    GFX.RealPitch = GFX.Pitch2 = GFX.Pitch;
    GFX.ZPitch = GFX.Pitch;
    if (Settings.SixteenBit)
	GFX.ZPitch >>= 1;
    GFX.Delta = (GFX.SubScreen - GFX.Screen) >> 1;
    GFX.DepthDelta = GFX.SubZBuffer - GFX.ZBuffer;
    //GFX.InfoStringTimeout = 0;
    //GFX.InfoString = NULL;

    PPU.BG_Forced = 0;
    IPPU.OBJChanged = TRUE;
    if (Settings.Transparency)
	Settings.SixteenBit = TRUE;

    IPPU.DirectColourMapsNeedRebuild = TRUE;
    GFX.PixSize = 1;
    if (Settings.SixteenBit)
    {
	DrawTilePtr = DrawTile16;
	DrawClippedTilePtr = DrawClippedTile16;
	DrawLargePixelPtr = DrawLargePixel16;
	DrawHiResTilePtr= DrawTile16;
	DrawHiResClippedTilePtr = DrawClippedTile16;
	GFX.PPL = GFX.Pitch >> 1;
	GFX.PPLx2 = GFX.Pitch;
    }
    else
    {
	DrawTilePtr = DrawTile;
	DrawClippedTilePtr = DrawClippedTile;
	DrawLargePixelPtr = DrawLargePixel;
	DrawHiResTilePtr = DrawTile;
	DrawHiResClippedTilePtr = DrawClippedTile;
	GFX.PPL = GFX.Pitch;
	GFX.PPLx2 = GFX.Pitch * 2;
    }
    S9xFixColourBrightness ();

    if (Settings.SixteenBit)
    {
	if (!(GFX.X2 = (uint16 *) malloc (sizeof (uint16) * 0x10000)))
	    return (FALSE);

	if (!(GFX.ZERO_OR_X2 = (uint16 *) malloc (sizeof (uint16) * 0x10000)) ||
	    !(GFX.ZERO = (uint16 *) malloc (sizeof (uint16) * 0x10000)))
	{
	    if (GFX.ZERO_OR_X2)
	    {
		free ((char *) GFX.ZERO_OR_X2);
		GFX.ZERO_OR_X2 = NULL;
	    }
	    if (GFX.X2)
	    {
		free ((char *) GFX.X2);
		GFX.X2 = NULL;
	    }
	    return (FALSE);
	}
	uint32 r, g, b;

	// Build a lookup table that multiplies a packed RGB value by 2 with
	// saturation.
	for (r = 0; r <= MAX_RED; r++)
	{
	    uint32 r2 = r << 1;
	    if (r2 > MAX_RED)
		r2 = MAX_RED;
	    for (g = 0; g <= MAX_GREEN; g++)
	    {
		uint32 g2 = g << 1;
		if (g2 > MAX_GREEN)
		    g2 = MAX_GREEN;
		for (b = 0; b <= MAX_BLUE; b++)
		{
		    uint32 b2 = b << 1;
		    if (b2 > MAX_BLUE)
			b2 = MAX_BLUE;
		    GFX.X2 [BUILD_PIXEL2 (r, g, b)] = BUILD_PIXEL2 (r2, g2, b2);
		    GFX.X2 [BUILD_PIXEL2 (r, g, b) & ~ALPHA_BITS_MASK] = BUILD_PIXEL2 (r2, g2, b2);
		}
	    }
	}
	ZeroMemory (GFX.ZERO, 0x10000 * sizeof (uint16));
	ZeroMemory (GFX.ZERO_OR_X2, 0x10000 * sizeof (uint16));
	// Build a lookup table that if the top bit of the color value is zero
	// then the value is zero, otherwise multiply the value by 2. Used by
	// the color subtraction code.

#if defined(OLD_COLOUR_BLENDING)
	for (r = 0; r <= MAX_RED; r++)
	{
	    uint32 r2 = r;
	    if ((r2 & 0x10) == 0)
		r2 = 0;
	    else
		r2 = (r2 << 1) & MAX_RED;

	    for (g = 0; g <= MAX_GREEN; g++)
	    {
		uint32 g2 = g;
		if ((g2 & GREEN_HI_BIT) == 0)
		    g2 = 0;
		else
		    g2 = (g2 << 1) & MAX_GREEN;

		for (b = 0; b <= MAX_BLUE; b++)
		{
		    uint32 b2 = b;
		    if ((b2 & 0x10) == 0)
			b2 = 0;
		    else
			b2 = (b2 << 1) & MAX_BLUE;

		    GFX.ZERO_OR_X2 [BUILD_PIXEL2 (r, g, b)] = BUILD_PIXEL2 (r2, g2, b2);
		    GFX.ZERO_OR_X2 [BUILD_PIXEL2 (r, g, b) & ~ALPHA_BITS_MASK] = BUILD_PIXEL2 (r2, g2, b2);
		}
	    }
	}
#else
        for (r = 0; r <= MAX_RED; r++)
        {
            uint32 r2 = r;
            if ((r2 & 0x10) == 0)
                r2 = 0;
            else
                r2 = (r2 << 1) & MAX_RED;

            if (r2 == 0)
                r2 = 1;
            for (g = 0; g <= MAX_GREEN; g++)
            {
                uint32 g2 = g;
                if ((g2 & GREEN_HI_BIT) == 0)
                    g2 = 0;
                else
                    g2 = (g2 << 1) & MAX_GREEN;

                if (g2 == 0)
                    g2 = 1;
                for (b = 0; b <= MAX_BLUE; b++)
                {
                    uint32 b2 = b;
                    if ((b2 & 0x10) == 0)
                        b2 = 0;
                    else
                        b2 = (b2 << 1) & MAX_BLUE;

                    if (b2 == 0)
                        b2 = 1;
                    GFX.ZERO_OR_X2 [BUILD_PIXEL2 (r, g, b)] = BUILD_PIXEL2 (r2, g2, b2);
                    GFX.ZERO_OR_X2 [BUILD_PIXEL2 (r, g, b) & ~ALPHA_BITS_MASK] = BUILD_PIXEL2 (r2, g2, b2);
                }
            }
        }
#endif

	// Build a lookup table that if the top bit of the color value is zero
	// then the value is zero, otherwise its just the value.
	for (r = 0; r <= MAX_RED; r++)
	{
	    uint32 r2 = r;
	    if ((r2 & 0x10) == 0)
		r2 = 0;
	    else
		r2 &= ~0x10;

	    for (g = 0; g <= MAX_GREEN; g++)
	    {
		uint32 g2 = g;
		if ((g2 & GREEN_HI_BIT) == 0)
		    g2 = 0;
		else
		    g2 &= ~GREEN_HI_BIT;
		for (b = 0; b <= MAX_BLUE; b++)
		{
		    uint32 b2 = b;
		    if ((b2 & 0x10) == 0)
			b2 = 0;
		    else
			b2 &= ~0x10;

		    GFX.ZERO [BUILD_PIXEL2 (r, g, b)] = BUILD_PIXEL2 (r2, g2, b2);
		    GFX.ZERO [BUILD_PIXEL2 (r, g, b) & ~ALPHA_BITS_MASK] = BUILD_PIXEL2 (r2, g2, b2);
		}
	    }
	}
    }
    else
    {
	GFX.X2 = NULL;
	GFX.ZERO_OR_X2 = NULL;
	GFX.ZERO = NULL;
    }

    return (TRUE);
}

void S9xGraphicsDeinit (void)
{
    // Free any memory allocated in S9xGraphicsInit
    if (GFX.X2)
    {
	free ((char *) GFX.X2);
	GFX.X2 = NULL;
    }
    if (GFX.ZERO_OR_X2)
    {
	free ((char *) GFX.ZERO_OR_X2);
	GFX.ZERO_OR_X2 = NULL;
    }
    if (GFX.ZERO)
    {
	free ((char *) GFX.ZERO);
	GFX.ZERO = NULL;
    }
}

void S9xBuildDirectColourMaps ()
{
    for (uint32 p = 0; p < 8; p++)
    {
	for (uint32 c = 0; c < 256; c++)
	{
// XXX: Brightness
	    DirectColourMaps [p][c] = BUILD_PIXEL (((c & 7) << 2) | ((p & 1) << 1),
						   ((c & 0x38) >> 1) | (p & 2),
						   ((c & 0xc0) >> 3) | (p & 4));
	}
    }
    IPPU.DirectColourMapsNeedRebuild = FALSE;
}

void RenderLine (uint8 C)
{
    if (IPPU.RenderThisFrame)
    {
	LineData[C].BG[0].VOffset = PPU.BG[0].VOffset + 1;
	LineData[C].BG[0].HOffset = PPU.BG[0].HOffset;
	LineData[C].BG[1].VOffset = PPU.BG[1].VOffset + 1;
	LineData[C].BG[1].HOffset = PPU.BG[1].HOffset;

	if (PPU.BGMode == 7)
	{
	    struct SLineMatrixData *p = &LineMatrixData [C];
	    p->MatrixA = PPU.MatrixA;
	    p->MatrixB = PPU.MatrixB;
	    p->MatrixC = PPU.MatrixC;
	    p->MatrixD = PPU.MatrixD;
	    p->CentreX = PPU.CentreX;
	    p->CentreY = PPU.CentreY;
	}
	else
	{
	    if (Settings.StarfoxHack && PPU.BG[2].VOffset == 0 &&
		PPU.BG[2].HOffset == 0xe000)
	    {
		LineData[C].BG[2].VOffset = 0xe1;
		LineData[C].BG[2].HOffset = 0;
	    }
	    else
	    {
		LineData[C].BG[2].VOffset = PPU.BG[2].VOffset + 1;
		LineData[C].BG[2].HOffset = PPU.BG[2].HOffset;
		LineData[C].BG[3].VOffset = PPU.BG[3].VOffset + 1;
		LineData[C].BG[3].HOffset = PPU.BG[3].HOffset;
	    }
	}
	IPPU.CurrentLine = C + 1;
    }
}

void S9xSetInfoString (const char *string)
{
    GFX.InfoString = string;
    GFX.InfoStringTimeout = 120;
}

extern void SelectTileRenderer (bool8 normal);

void S9xSetupOBJ ()
{
    int SmallSize;
    int LargeSize;

    switch (PPU.OBJSizeSelect)
    {
    case 0:
	SmallSize = 8;
	LargeSize = 16;
	break;
    case 1:
	SmallSize = 8;
	LargeSize = 32;
	break;
    case 2:
	SmallSize = 8;
	LargeSize = 64;
	break;
    case 3:
	SmallSize = 16;
	LargeSize = 32;
	break;
    case 4:
	SmallSize = 16;
	LargeSize = 64;
	break;
    case 5:
    default:
	SmallSize = 32;
	LargeSize = 64;
	break;
    }

    int C = 0;

    int FirstSprite = PPU.FirstSprite & 0x7f;
    int S = FirstSprite;
    do
    {
	int Size;
	if (PPU.OBJ [S].Size)
	    Size = LargeSize;
	else
	    Size = SmallSize;

	long VPos = PPU.OBJ [S].VPos;

	if (VPos >= PPU.ScreenHeight)
	    VPos -= 256;
	if (PPU.OBJ [S].HPos < 256 && PPU.OBJ [S].HPos > -Size &&
	    VPos < PPU.ScreenHeight && VPos > -Size)
	{
	    GFX.OBJList [C++] = S;
	    GFX.Sizes[S] = Size;
	    GFX.VPositions[S] = VPos;
	}
	S = (S + 1) & 0x7f;
    } while (S != FirstSprite);

    // Terminate the list
    GFX.OBJList [C] = -1;
    IPPU.OBJChanged = FALSE;
}

void DrawOBJS (bool8 OnMain = FALSE, uint8 D = 0)
{
    uint32 O;
    uint32 BaseTile, Tile;

    CHECK_SOUND();

    BG.BitShift = 4;
    BG.TileShift = 5;
    BG.TileAddress = PPU.OBJNameBase;
    BG.StartPalette = 128;
    BG.PaletteShift = 4;
    BG.PaletteMask = 7;
    BG.Buffer = IPPU.TileCache [TILE_4BIT];
    BG.Buffered = IPPU.TileCached [TILE_4BIT];
    BG.NameSelect = PPU.OBJNameSelect;
    BG.DirectColourMode = FALSE;

    GFX.PixSize = 1;

    if (Settings.SupportHiRes)
    {
	if (PPU.BGMode == 5 || PPU.BGMode == 6)
	{
	    // Bah, OnMain is never used except to determine if calling
	    // SelectTileRenderer is necessary. So let's hack it to false here
	    // to stop SelectTileRenderer from being called when it causes
	    // problems.
	    OnMain = FALSE;
	    GFX.PixSize = 2;
	    if (IPPU.DoubleHeightPixels)

	    {
		if (Settings.SixteenBit)
		{
		    DrawTilePtr = DrawTile16x2x2;
		    DrawClippedTilePtr = DrawClippedTile16x2x2;
		}
		else
		{
		    DrawTilePtr = DrawTilex2x2;
		    DrawClippedTilePtr = DrawClippedTilex2x2;
		}
	    }
	    else
	    {
		if (Settings.SixteenBit)
		{
		    DrawTilePtr = DrawTile16x2;
		    DrawClippedTilePtr = DrawClippedTile16x2;
		}
		else
		{
		    DrawTilePtr = DrawTilex2;
		    DrawClippedTilePtr = DrawClippedTilex2;
		}
	    }
	}
	else
	{
	    if (Settings.SixteenBit)
	    {
		DrawTilePtr = DrawTile16;
		DrawClippedTilePtr = DrawClippedTile16;
	    }
	    else
	    {
		DrawTilePtr = DrawTile;
		DrawClippedTilePtr = DrawClippedTile;
	    }
	}
    }
    GFX.Z1 = D + 2;

    int I = 0;
    for (int S = GFX.OBJList [I++]; S >= 0; S = GFX.OBJList [I++])
    {
	int VPos = GFX.VPositions [S];
	int Size = GFX.Sizes[S];
	int TileInc = 1;
	int Offset;

	if (VPos + Size <= (int) GFX.StartY || VPos > (int) GFX.EndY)
	    continue;

	if (OnMain && SUB_OR_ADD(4))
	{
	    SelectTileRenderer (!GFX.Pseudo && PPU.OBJ [S].Palette < 4);
	}

	BaseTile = PPU.OBJ[S].Name | (PPU.OBJ[S].Palette << 10);

	if (PPU.OBJ[S].HFlip)
	{
	    BaseTile += ((Size >> 3) - 1) | H_FLIP;
	    TileInc = -1;
	}
	if (PPU.OBJ[S].VFlip)
	    BaseTile |= V_FLIP;

	int clipcount = GFX.pCurrentClip->Count [4];
	if (!clipcount)
	    clipcount = 1;

	GFX.Z2 = (PPU.OBJ[S].Priority + 1) * 4 + D;

	for (int clip = 0; clip < clipcount; clip++)
	{
	    int Left;
	    int Right;
	    if (!GFX.pCurrentClip->Count [4])
	    {
		Left = 0;
		Right = 256;
	    }
	    else
	    {
		Left = GFX.pCurrentClip->Left [clip][4];
		Right = GFX.pCurrentClip->Right [clip][4];
	    }

	    if (Right <= Left || PPU.OBJ[S].HPos + Size <= Left ||
		PPU.OBJ[S].HPos >= Right)
		continue;

	    for (int Y = 0; Y < Size; Y += 8)
	    {
		if (VPos + Y + 7 >= (int) GFX.StartY && VPos + Y <= (int) GFX.EndY)
		{
		    int StartLine;
		    int TileLine;
		    int LineCount;
		    int Last;

		    if ((StartLine = VPos + Y) < (int) GFX.StartY)
		    {
			StartLine = GFX.StartY - StartLine;
			LineCount = 8 - StartLine;
		    }
		    else
		    {
			StartLine = 0;
			LineCount = 8;
		    }
		    if ((Last = VPos + Y + 7 - GFX.EndY) > 0)
			if ((LineCount -= Last) <= 0)
			    break;

		    TileLine = StartLine << 3;
		    O = (VPos + Y + StartLine) * GFX.PPL;
		    if (!PPU.OBJ[S].VFlip)
			Tile = BaseTile + (Y << 1);
		    else
			Tile = BaseTile + ((Size - Y - 8) << 1);

		    int Middle = Size >> 3;
		    if (PPU.OBJ[S].HPos < Left)
		    {
			Tile += ((Left - PPU.OBJ[S].HPos) >> 3) * TileInc;
			Middle -= (Left - PPU.OBJ[S].HPos) >> 3;
			O += Left * GFX.PixSize;
			if ((Offset = (Left - PPU.OBJ[S].HPos) & 7))
			{
			    O -= Offset * GFX.PixSize;
			    int W = 8 - Offset;
			    int Width = Right - Left;
			    if (W > Width)
				W = Width;
			    (*DrawClippedTilePtr) (Tile, O, Offset, W,
						   TileLine, LineCount);

			    if (W >= Width)
				continue;
			    Tile += TileInc;
			    Middle--;
			    O += 8 * GFX.PixSize;
			}
		    }
		    else
			O += PPU.OBJ[S].HPos * GFX.PixSize;

		    if (PPU.OBJ[S].HPos + Size >= Right)
		    {
			Middle -= ((PPU.OBJ[S].HPos + Size + 7) -
				   Right) >> 3;
			Offset = (Right - (PPU.OBJ[S].HPos + Size)) & 7;
		    }
		    else
			Offset = 0;

		    for (int X = 0; X < Middle; X++, O += 8 * GFX.PixSize,
			 Tile += TileInc)
		    {
			(*DrawTilePtr) (Tile, O, TileLine, LineCount);
		    }
		    if (Offset)
		    {
			(*DrawClippedTilePtr) (Tile, O, 0, Offset,
					       TileLine, LineCount);
		    }
		}
	    }
	}
    }
}

void DrawBackgroundMosaic (uint32 BGMode, uint32 bg, uint8 Z1, uint8 Z2)
{
    CHECK_SOUND();

    uint32 Tile;
    uint16 *SC0;
    uint16 *SC1;
    uint16 *SC2;
    uint16 *SC3;
    uint8 depths [2] = {Z1, Z2};

    if (BGMode == 0)
	BG.StartPalette = bg << 5;
    else
	BG.StartPalette = 0;

    SC0 = (uint16 *) &Memory.VRAM[PPU.BG[bg].SCBase << 1];

    if (PPU.BG[bg].SCSize & 1)
	SC1 = SC0 + 1024;
    else
	SC1 = SC0;

	if(((uint8*)SC1-Memory.VRAM)>=0x10000)
		SC1-=0x08000;


    if (PPU.BG[bg].SCSize & 2)
	SC2 = SC1 + 1024;
    else
	SC2 = SC0;

	if(((uint8*)SC2-Memory.VRAM)>=0x10000)
		SC2-=0x08000;


    if (PPU.BG[bg].SCSize & 1)
	SC3 = SC2 + 1024;
    else
	SC3 = SC2;

	if(((uint8*)SC3-Memory.VRAM)>=0x10000)
		SC3-=0x08000;

    uint32 Lines;
    uint32 OffsetMask;
    uint32 OffsetShift;

    if (BG.TileSize == 16)
    {
	OffsetMask = 0x3ff;
	OffsetShift = 4;
    }
    else
    {
	OffsetMask = 0x1ff;
	OffsetShift = 3;
    }

    for (uint32 Y = GFX.StartY; Y <= GFX.EndY; Y += Lines)
    {
	uint32 VOffset = LineData [Y].BG[bg].VOffset;
	uint32 HOffset = LineData [Y].BG[bg].HOffset;
	uint32 MosaicOffset = Y % PPU.Mosaic;

	for (Lines = 1; Lines < PPU.Mosaic - MosaicOffset; Lines++)
	    if ((VOffset != LineData [Y + Lines].BG[bg].VOffset) ||
		(HOffset != LineData [Y + Lines].BG[bg].HOffset))
		break;

	uint32 MosaicLine = VOffset + Y - MosaicOffset;

	if (Y + Lines > GFX.EndY)
	    Lines = GFX.EndY + 1 - Y;
	uint32 VirtAlign = (MosaicLine & 7) << 3;

	uint16 *b1;
	uint16 *b2;

	uint32 ScreenLine = MosaicLine >> OffsetShift;
	uint32 Rem16 = MosaicLine & 15;

	if (ScreenLine & 0x20)
	    b1 = SC2, b2 = SC3;
	else
	    b1 = SC0, b2 = SC1;

	b1 += (ScreenLine & 0x1f) << 5;
	b2 += (ScreenLine & 0x1f) << 5;
	uint16 *t;
	uint32 Left = 0;
	uint32 Right = 256;

	uint32 ClipCount = GFX.pCurrentClip->Count [bg];
	uint32 HPos = HOffset;
	uint32 PixWidth = PPU.Mosaic;

	if (!ClipCount)
	    ClipCount = 1;

	for (uint32 clip = 0; clip < ClipCount; clip++)
	{
	    if (GFX.pCurrentClip->Count [bg])
	    {
		Left = GFX.pCurrentClip->Left [clip][bg];
		Right = GFX.pCurrentClip->Right [clip][bg];
		uint32 r = Left % PPU.Mosaic;
		HPos = HOffset + Left;
		PixWidth = PPU.Mosaic - r;
	    }
	    uint32 s = Y * GFX.PPL + Left * GFX.PixSize;
	    for (uint32 x = Left; x < Right; x += PixWidth,
		 s += PixWidth * GFX.PixSize,
		 HPos += PixWidth, PixWidth = PPU.Mosaic)
	    {
		uint32 Quot = (HPos & OffsetMask) >> 3;

		if (x + PixWidth >= Right)
		    PixWidth = Right - x;

		if (BG.TileSize == 8)
		{
		    if (Quot > 31)
			t = b2 + (Quot & 0x1f);
		    else
			t = b1 + Quot;
		}
		else
		{
		    if (Quot > 63)
			t = b2 + ((Quot >> 1) & 0x1f);
		    else
			t = b1 + (Quot >> 1);
		}

		Tile = READ_2BYTES (t);
		GFX.Z1 = GFX.Z2 = depths [(Tile & 0x2000) >> 13];

		// Draw tile...
		if (BG.TileSize != 8)
		{
		    if (Tile & H_FLIP)
		    {
			// Horizontal flip, but what about vertical flip ?
			if (Tile & V_FLIP)
			{
			    // Both horzontal & vertical flip
			    if (Rem16 < 8)
			    {
				(*DrawLargePixelPtr) (Tile + 17 - (Quot & 1), s,
						      HPos & 7, PixWidth,
						      VirtAlign, Lines);
			    }
			    else
			    {
				(*DrawLargePixelPtr) (Tile + 1 - (Quot & 1), s,
						      HPos & 7, PixWidth,
						      VirtAlign, Lines);
			    }
			}
			else
			{
			    // Horizontal flip only
			    if (Rem16 > 7)
			    {
				(*DrawLargePixelPtr) (Tile + 17 - (Quot & 1), s,
						      HPos & 7, PixWidth,
						      VirtAlign, Lines);
			    }
			    else
			    {
				(*DrawLargePixelPtr) (Tile + 1 - (Quot & 1), s,
						      HPos & 7, PixWidth,
						      VirtAlign, Lines);
			    }
			}
		    }
		    else
		    {
			// No horizontal flip, but is there a vertical flip ?
			if (Tile & V_FLIP)
			{
			    // Vertical flip only
			    if (Rem16 < 8)
			    {
				(*DrawLargePixelPtr) (Tile + 16 + (Quot & 1), s,
						      HPos & 7, PixWidth,
						      VirtAlign, Lines);
			    }
			    else
			    {
				(*DrawLargePixelPtr) (Tile + (Quot & 1), s,
						      HPos & 7, PixWidth,
						      VirtAlign, Lines);
			    }
			}
			else
			{
			    // Normal unflipped
			    if (Rem16 > 7)
			    {
				(*DrawLargePixelPtr) (Tile + 16 + (Quot & 1), s,
						      HPos & 7, PixWidth,
						      VirtAlign, Lines);
			    }
			    else
			    {
				(*DrawLargePixelPtr) (Tile + (Quot & 1), s,
						      HPos & 7, PixWidth,
						      VirtAlign, Lines);
			    }
			}
		    }
		}
		else
		    (*DrawLargePixelPtr) (Tile, s, HPos & 7, PixWidth,
					  VirtAlign, Lines);
	    }
	}
    }
}

void DrawBackgroundOffset (uint32 BGMode, uint32 bg, uint8 Z1, uint8 Z2)
{
    CHECK_SOUND();

    uint32 Tile;
    uint16 *SC0;
    uint16 *SC1;
    uint16 *SC2;
    uint16 *SC3;
    uint16 *BPS0;
    uint16 *BPS1;
    uint16 *BPS2;
    uint16 *BPS3;
    uint32 Width;
    int VOffsetOffset = BGMode == 4 ? 0 : 32;
    uint8 depths [2] = {Z1, Z2};

    BG.StartPalette = 0;

    BPS0 = (uint16 *) &Memory.VRAM[PPU.BG[2].SCBase << 1];

    if (PPU.BG[2].SCSize & 1)
		BPS1 = BPS0 + 1024;
    else
		BPS1 = BPS0;

    if (PPU.BG[2].SCSize & 2)
		BPS2 = BPS1 + 1024;
    else
		BPS2 = BPS0;

    if (PPU.BG[2].SCSize & 1)
		BPS3 = BPS2 + 1024;
    else
		BPS3 = BPS2;

    SC0 = (uint16 *) &Memory.VRAM[PPU.BG[bg].SCBase << 1];

    if (PPU.BG[bg].SCSize & 1)
		SC1 = SC0 + 1024;
    else
		SC1 = SC0;

	if(((uint8*)SC1-Memory.VRAM)>=0x10000)
		SC1-=0x08000;


    if (PPU.BG[bg].SCSize & 2)
		SC2 = SC1 + 1024;
    else
		SC2 = SC0;

	if(((uint8*)SC2-Memory.VRAM)>=0x10000)
		SC2-=0x08000;


    if (PPU.BG[bg].SCSize & 1)
		SC3 = SC2 + 1024;
    else
		SC3 = SC2;

	if(((uint8*)SC3-Memory.VRAM)>=0x10000)
		SC3-=0x08000;


    static const int Lines = 1;
    int OffsetMask;
    int OffsetShift;
    int OffsetEnableMask = 1 << (bg + 13);

    if (BG.TileSize == 16)
    {
		OffsetMask = 0x3ff;
		OffsetShift = 4;
    }
    else
    {
		OffsetMask = 0x1ff;
		OffsetShift = 3;
    }

    for (uint32 Y = GFX.StartY; Y <= GFX.EndY; Y++)
    {
		uint32 VOff = LineData [Y].BG[2].VOffset - 1;
//		uint32 VOff = LineData [Y].BG[2].VOffset;
		uint32 HOff = LineData [Y].BG[2].HOffset;

		int VirtAlign;
		int ScreenLine = VOff >> 3;
		int t1;
		int t2;
		uint16 *s0;
		uint16 *s1;
		uint16 *s2;

		if (ScreenLine & 0x20)
			s1 = BPS2, s2 = BPS3;
		else
			s1 = BPS0, s2 = BPS1;

		s1 += (ScreenLine & 0x1f) << 5;
		s2 += (ScreenLine & 0x1f) << 5;

		if(BGMode != 4)
		{
			if((ScreenLine & 0x1f) == 0x1f)
			{
				if(ScreenLine & 0x20)
					VOffsetOffset = BPS0 - BPS2 - 0x1f*32;
				else
					VOffsetOffset = BPS2 - BPS0 - 0x1f*32;
			}
			else
			{
				VOffsetOffset = 32;
			}
		}

		int clipcount = GFX.pCurrentClip->Count [bg];
		if (!clipcount)
			clipcount = 1;

		for (int clip = 0; clip < clipcount; clip++)
		{
			uint32 Left;
			uint32 Right;

			if (!GFX.pCurrentClip->Count [bg])
			{
				Left = 0;
				Right = 256;
			}
			else
			{
				Left = GFX.pCurrentClip->Left [clip][bg];
				Right = GFX.pCurrentClip->Right [clip][bg];

				if (Right <= Left)
					continue;
			}

			uint32 VOffset;
			uint32 HOffset;
			//added:
			uint32 LineHOffset=LineData [Y].BG[bg].HOffset;

			uint32 Offset;
			uint32 HPos;
			uint32 Quot;
			uint32 Count;
			uint16 *t;
			uint32 Quot2;
			uint32 VCellOffset;
			uint32 HCellOffset;
			uint16 *b1;
			uint16 *b2;
			uint32 TotalCount = 0;
			uint32 MaxCount = 8;

			uint32 s = Left * GFX.PixSize + Y * GFX.PPL;
			bool8 left_hand_edge = (Left == 0);
			Width = Right - Left;

			if (Left & 7)
				MaxCount = 8 - (Left & 7);

			while (Left < Right)
			{
				if (left_hand_edge)
				{
					// The SNES offset-per-tile background mode has a
					// hardware limitation that the offsets cannot be set
					// for the tile at the left-hand edge of the screen.
					VOffset = LineData [Y].BG[bg].VOffset;

					//MKendora; use temp var to reduce memory accesses
					//HOffset = LineData [Y].BG[bg].HOffset;

					HOffset = LineHOffset;
					//End MK

					left_hand_edge = FALSE;
				}
				else

				{
					// All subsequent offset tile data is shifted left by one,
					// hence the - 1 below.

					Quot2 = ((HOff + Left - 1) & OffsetMask) >> 3;

					if (Quot2 > 31)
						s0 = s2 + (Quot2 & 0x1f);
					else
						s0 = s1 + Quot2;

					HCellOffset = READ_2BYTES (s0);

					if (BGMode == 4)
					{
						VOffset = LineData [Y].BG[bg].VOffset;

						//MKendora another mem access hack
						//HOffset = LineData [Y].BG[bg].HOffset;
						HOffset=LineHOffset;
						//end MK

						if ((HCellOffset & OffsetEnableMask))
						{
							if (HCellOffset & 0x8000)
								VOffset = HCellOffset + 1;
							else
								HOffset = HCellOffset;
						}
					}
					else
					{
						VCellOffset = READ_2BYTES (s0 + VOffsetOffset);
						if ((VCellOffset & OffsetEnableMask))
							VOffset = VCellOffset + 1;
						else
							VOffset = LineData [Y].BG[bg].VOffset;

						//MKendora Strike Gunner fix
						if ((HCellOffset & OffsetEnableMask))
						{
							//HOffset= HCellOffset;

							HOffset = (HCellOffset & ~7)|(LineHOffset&7);
							//HOffset |= LineData [Y].BG[bg].HOffset&7;
						}
						else
							HOffset=LineHOffset;
							//HOffset = LineData [Y].BG[bg].HOffset -
							//Settings.StrikeGunnerOffsetHack;
						//HOffset &= (~7);
						//end MK
					}
				}
				VirtAlign = ((Y + VOffset) & 7) << 3;
				ScreenLine = (VOffset + Y) >> OffsetShift;

				if (((VOffset + Y) & 15) > 7)
				{
					t1 = 16;
					t2 = 0;
				}
				else
				{
					t1 = 0;
					t2 = 16;
				}

				if (ScreenLine & 0x20)
					b1 = SC2, b2 = SC3;
				else
					b1 = SC0, b2 = SC1;

				b1 += (ScreenLine & 0x1f) << 5;
				b2 += (ScreenLine & 0x1f) << 5;

				HPos = (HOffset + Left) & OffsetMask;

				Quot = HPos >> 3;

				if (BG.TileSize == 8)
				{
					if (Quot > 31)
						t = b2 + (Quot & 0x1f);
					else
						t = b1 + Quot;
				}
				else
				{
					if (Quot > 63)
						t = b2 + ((Quot >> 1) & 0x1f);
					else
						t = b1 + (Quot >> 1);
				}

				if (MaxCount + TotalCount > Width)
					MaxCount = Width - TotalCount;

				Offset = HPos & 7;

				//Count =1;
				Count = 8 - Offset;
				if (Count > MaxCount)
					Count = MaxCount;

				s -= Offset * GFX.PixSize;
				Tile = READ_2BYTES(t);
				GFX.Z1 = GFX.Z2 = depths [(Tile & 0x2000) >> 13];

				if (BG.TileSize == 8)
					(*DrawClippedTilePtr) (Tile, s, Offset, Count, VirtAlign, Lines);
				else
				{
					if (!(Tile & (V_FLIP | H_FLIP)))
					{
						// Normal, unflipped
						(*DrawClippedTilePtr) (Tile + t1 + (Quot & 1),
							s, Offset, Count, VirtAlign, Lines);
					}
					else
						if (Tile & H_FLIP)
						{
							if (Tile & V_FLIP)
							{
								// H & V flip
								(*DrawClippedTilePtr) (Tile + t2 + 1 - (Quot & 1),
									s, Offset, Count, VirtAlign, Lines);
							}
							else
							{
								// H flip only
								(*DrawClippedTilePtr) (Tile + t1 + 1 - (Quot & 1),
									s, Offset, Count, VirtAlign, Lines);
							}
						}
						else
						{
							// V flip only
							(*DrawClippedTilePtr) (Tile + t2 + (Quot & 1),
								s, Offset, Count, VirtAlign, Lines);
						}
				}

				Left += Count;
				TotalCount += Count;
				s += (Offset + Count) * GFX.PixSize;
				MaxCount = 8;
		}
	}
    }
}

void DrawBackgroundMode5 (uint32 /* BGMODE */, uint32 bg, uint8 Z1, uint8 Z2)
{
    CHECK_SOUND();

    if(IPPU.Interlace)
	{
		GFX.Pitch = GFX.RealPitch;
		GFX.PPL = GFX.PPLx2 >> 1;
	}
    GFX.PixSize = 1;
    uint8 depths [2] = {Z1, Z2};

    uint32 Tile;
    uint16 *SC0;
    uint16 *SC1;
    uint16 *SC2;
    uint16 *SC3;
    uint32 Width;

    BG.StartPalette = 0;

    SC0 = (uint16 *) &Memory.VRAM[PPU.BG[bg].SCBase << 1];

    if ((PPU.BG[bg].SCSize & 1))
		SC1 = SC0 + 1024;
    else
		SC1 = SC0;

	if((SC1-(unsigned short*)Memory.VRAM)>0x10000)
		SC1=(uint16*)&Memory.VRAM[(((uint8*)SC1)-Memory.VRAM)%0x10000];

    if ((PPU.BG[bg].SCSize & 2))
		SC2 = SC1 + 1024;
    else SC2 = SC0;

	if(((uint8*)SC2-Memory.VRAM)>=0x10000)
		SC2-=0x08000;



    if ((PPU.BG[bg].SCSize & 1))
		SC3 = SC2 + 1024;
    else
		SC3 = SC2;

	if(((uint8*)SC3-Memory.VRAM)>=0x10000)
		SC3-=0x08000;



    int Lines;
    int VOffsetMask;
    int VOffsetShift;

    if (BG.TileSize == 16)
    {
		VOffsetMask = 0x3ff;
		VOffsetShift = 4;
    }
    else
    {
		VOffsetMask = 0x1ff;
		VOffsetShift = 3;
    }
    int endy = IPPU.Interlace ? 1 + (GFX.EndY << 1) : GFX.EndY;

    for (int Y = IPPU.Interlace ? GFX.StartY << 1 : GFX.StartY; Y <= endy; Y += Lines)
    {
		int y = IPPU.Interlace ? (Y >> 1) : Y;
		uint32 VOffset = LineData [y].BG[bg].VOffset;
		uint32 HOffset = LineData [y].BG[bg].HOffset;
		int VirtAlign = (Y + VOffset) & 7;

		for (Lines = 1; Lines < 8 - VirtAlign; Lines++)
			if ((VOffset != LineData [y + Lines].BG[bg].VOffset) ||
				(HOffset != LineData [y + Lines].BG[bg].HOffset))
				break;

			HOffset <<= 1;
			if (Y + Lines > endy)
				Lines = endy + 1 - Y;
			VirtAlign <<= 3;

			int ScreenLine = (VOffset + Y) >> VOffsetShift;
			int t1;
			int t2;
			if (((VOffset + Y) & 15) > 7)
			{
				t1 = 16;
				t2 = 0;
			}
			else
			{
				t1 = 0;
				t2 = 16;
			}
			uint16 *b1;
			uint16 *b2;

			if (ScreenLine & 0x20)
				b1 = SC2, b2 = SC3;
			else
				b1 = SC0, b2 = SC1;

			b1 += (ScreenLine & 0x1f) << 5;
			b2 += (ScreenLine & 0x1f) << 5;

			int clipcount = GFX.pCurrentClip->Count [bg];
			if (!clipcount)
				clipcount = 1;
			for (int clip = 0; clip < clipcount; clip++)
			{
				int Left;
				int Right;

				if (!GFX.pCurrentClip->Count [bg])
				{
					Left = 0;
					Right = 512;
				}
				else
				{
					Left = GFX.pCurrentClip->Left [clip][bg] * 2;
					Right = GFX.pCurrentClip->Right [clip][bg] * 2;

					if (Right <= Left)
						continue;
				}

				uint32 s = Left * GFX.PixSize + Y * GFX.PPL;
				uint32 HPos = (HOffset + Left * GFX.PixSize) & 0x3ff;

				uint32 Quot = HPos >> 3;
				uint32 Count = 0;

				uint16 *t;
				if (Quot > 63)
					t = b2 + ((Quot >> 1) & 0x1f);
				else
					t = b1 + (Quot >> 1);

				Width = Right - Left;
				// Left hand edge clipped tile
				if (HPos & 7)
				{
					int Offset = (HPos & 7);
					Count = 8 - Offset;
					if (Count > Width)
						Count = Width;
					s -= Offset;
					Tile = READ_2BYTES (t);
					GFX.Z1 = GFX.Z2 = depths [(Tile & 0x2000) >> 13];

					if (BG.TileSize == 8)
					{
						if (!(Tile & H_FLIP))
						{
							// Normal, unflipped
							(*DrawHiResClippedTilePtr) (Tile + (Quot & 1),
								s, Offset, Count, VirtAlign, Lines);
						}
						else
						{
							// H flip
							(*DrawHiResClippedTilePtr) (Tile + 1 - (Quot & 1),
								s, Offset, Count, VirtAlign, Lines);
						}
					}
					else
					{
						if (!(Tile & (V_FLIP | H_FLIP)))
						{
							// Normal, unflipped
							(*DrawHiResClippedTilePtr) (Tile + t1 + (Quot & 1),
								s, Offset, Count, VirtAlign, Lines);
						}
						else
							if (Tile & H_FLIP)
							{
								if (Tile & V_FLIP)
								{
									// H & V flip
									(*DrawHiResClippedTilePtr) (Tile + t2 + 1 - (Quot & 1),
										s, Offset, Count, VirtAlign, Lines);
								}
								else
								{
									// H flip only
									(*DrawHiResClippedTilePtr) (Tile + t1 + 1 - (Quot & 1),
										s, Offset, Count, VirtAlign, Lines);
								}
							}
							else
							{
								// V flip only
								(*DrawHiResClippedTilePtr) (Tile + t2 + (Quot & 1),
									s, Offset, Count, VirtAlign, Lines);
							}
					}

					t += Quot & 1;
					if (Quot == 63)
						t = b2;
					else if (Quot == 127)
						t = b1;
					Quot++;
					s += 8;
				}

				// Middle, unclipped tiles
				Count = Width - Count;
				int Middle = Count >> 3;
				Count &= 7;
				for (int C = Middle; C > 0; s += 8, Quot++, C--)
				{
					Tile = READ_2BYTES(t);
					GFX.Z1 = GFX.Z2 = depths [(Tile & 0x2000) >> 13];
					if (BG.TileSize == 8)
					{
						if (!(Tile & H_FLIP))
						{
							// Normal, unflipped
							(*DrawHiResTilePtr) (Tile + (Quot & 1),
								s, VirtAlign, Lines);
						}
						else
						{
							// H flip
							(*DrawHiResTilePtr) (Tile + 1 - (Quot & 1),
								s, VirtAlign, Lines);
						}
					}
					else
					{
						if (!(Tile & (V_FLIP | H_FLIP)))
						{
							// Normal, unflipped
							(*DrawHiResTilePtr) (Tile + t1 + (Quot & 1),
								s, VirtAlign, Lines);
						}
						else
							if (Tile & H_FLIP)
							{
								if (Tile & V_FLIP)
								{
									// H & V flip
									(*DrawHiResTilePtr) (Tile + t2 + 1 - (Quot & 1),
										s, VirtAlign, Lines);
								}
								else
								{
									// H flip only
									(*DrawHiResTilePtr) (Tile + t1 + 1 - (Quot & 1),
										s, VirtAlign, Lines);
								}
							}
							else
							{
								// V flip only
								(*DrawHiResTilePtr) (Tile + t2 + (Quot & 1),
									s, VirtAlign, Lines);
							}
					}

					t += Quot & 1;
					if (Quot == 63)
						t = b2;
					else
						if (Quot == 127)
							t = b1;
				}

				// Right-hand edge clipped tiles
				if (Count)
				{
					Tile = READ_2BYTES(t);
					GFX.Z1 = GFX.Z2 = depths [(Tile & 0x2000) >> 13];
					if (BG.TileSize == 8)
					{
						if (!(Tile & H_FLIP))
						{
							// Normal, unflipped
							(*DrawHiResClippedTilePtr) (Tile + (Quot & 1),
								s, 0, Count, VirtAlign, Lines);
						}
						else
						{
							// H flip
							(*DrawHiResClippedTilePtr) (Tile + 1 - (Quot & 1),
								s, 0, Count, VirtAlign, Lines);
						}
					}
					else
					{
						if (!(Tile & (V_FLIP | H_FLIP)))
						{
							// Normal, unflipped
							(*DrawHiResClippedTilePtr) (Tile + t1 + (Quot & 1),
								s, 0, Count, VirtAlign, Lines);
						}
						else
							if (Tile & H_FLIP)
							{
								if (Tile & V_FLIP)
								{
									// H & V flip
									(*DrawHiResClippedTilePtr) (Tile + t2 + 1 - (Quot & 1),
										s, 0, Count, VirtAlign, Lines);
								}
								else
								{
									// H flip only
									(*DrawHiResClippedTilePtr) (Tile + t1 + 1 - (Quot & 1),
										s, 0, Count, VirtAlign, Lines);
								}
							}
							else
							{
								// V flip only
								(*DrawHiResClippedTilePtr) (Tile + t2 + (Quot & 1),
									s, 0, Count, VirtAlign, Lines);
							}
					}
				}
	}
    }
    GFX.Pitch = IPPU.DoubleHeightPixels ? GFX.RealPitch * 2 : GFX.RealPitch;
    GFX.PPL = IPPU.DoubleHeightPixels ? GFX.PPLx2 : (GFX.PPLx2 >> 1);

}

void DrawBackground (uint32 BGMode, uint32 bg, uint8 Z1, uint8 Z2)
{
    GFX.PixSize = 1;

    BG.TileSize = BGSizes [PPU.BG[bg].BGSize];
    BG.BitShift = BitShifts[BGMode][bg];
    BG.TileShift = TileShifts[BGMode][bg];
    BG.TileAddress = PPU.BG[bg].NameBase << 1;
    BG.NameSelect = 0;
    BG.Buffer = IPPU.TileCache [Depths [BGMode][bg]];
    BG.Buffered = IPPU.TileCached [Depths [BGMode][bg]];
    BG.PaletteShift = PaletteShifts[BGMode][bg];
    BG.PaletteMask = PaletteMasks[BGMode][bg];
    BG.DirectColourMode = (BGMode == 3 || BGMode == 4) && bg == 0 &&
		(GFX.r2130 & 1);

    if (PPU.BGMosaic [bg] && PPU.Mosaic > 1)
    {
		DrawBackgroundMosaic (BGMode, bg, Z1, Z2);
		return;

    }
    switch (BGMode)
    {
    case 2:
    case 4: // Used by Puzzle Bobble
        DrawBackgroundOffset (BGMode, bg, Z1, Z2);
		return;

    case 5:
    case 6: // XXX: is also offset per tile.
		if (Settings.SupportHiRes)
		{
			DrawBackgroundMode5 (BGMode, bg, Z1, Z2);
			return;
		}
		break;
    }
    CHECK_SOUND();

    uint32 Tile;
    uint16 *SC0;
    uint16 *SC1;
    uint16 *SC2;
    uint16 *SC3;
    uint32 Width;
    uint8 depths [2] = {Z1, Z2};

    if (BGMode == 0)
		BG.StartPalette = bg << 5;
    else BG.StartPalette = 0;

    SC0 = (uint16 *) &Memory.VRAM[PPU.BG[bg].SCBase << 1];

    if (PPU.BG[bg].SCSize & 1)
		SC1 = SC0 + 1024;
    else
		SC1 = SC0;

	if(SC1>=(unsigned short*)(Memory.VRAM+0x10000))
		SC1=(uint16*)&Memory.VRAM[((uint8*)SC1-&Memory.VRAM[0])%0x10000];

    if (PPU.BG[bg].SCSize & 2)
		SC2 = SC1 + 1024;
    else
		SC2 = SC0;

	if(((uint8*)SC2-Memory.VRAM)>=0x10000)
		SC2-=0x08000;

    if (PPU.BG[bg].SCSize & 1)
		SC3 = SC2 + 1024;
    else
		SC3 = SC2;

	if(((uint8*)SC3-Memory.VRAM)>=0x10000)
		SC3-=0x08000;



    int Lines;
    int OffsetMask;
    int OffsetShift;

    if (BG.TileSize == 16)
    {
		OffsetMask = 0x3ff;
		OffsetShift = 4;
    }
    else
    {
		OffsetMask = 0x1ff;
		OffsetShift = 3;
    }

    for (uint32 Y = GFX.StartY; Y <= GFX.EndY; Y += Lines)
    {
		uint32 VOffset = LineData [Y].BG[bg].VOffset;
		uint32 HOffset = LineData [Y].BG[bg].HOffset;
		int VirtAlign = (Y + VOffset) & 7;

		for (Lines = 1; Lines < 8 - VirtAlign; Lines++)
			if ((VOffset != LineData [Y + Lines].BG[bg].VOffset) ||
				(HOffset != LineData [Y + Lines].BG[bg].HOffset))
				break;

			if (Y + Lines > GFX.EndY)
				Lines = GFX.EndY + 1 - Y;

			VirtAlign <<= 3;

			uint32 ScreenLine = (VOffset + Y) >> OffsetShift;
			uint32 t1;
			uint32 t2;
			if (((VOffset + Y) & 15) > 7)
			{
				t1 = 16;
				t2 = 0;
			}
			else
			{
				t1 = 0;
				t2 = 16;
			}
			uint16 *b1;
			uint16 *b2;

			if (ScreenLine & 0x20)
				b1 = SC2, b2 = SC3;
			else
				b1 = SC0, b2 = SC1;

			b1 += (ScreenLine & 0x1f) << 5;
			b2 += (ScreenLine & 0x1f) << 5;

			int clipcount = GFX.pCurrentClip->Count [bg];
			if (!clipcount)
				clipcount = 1;
			for (int clip = 0; clip < clipcount; clip++)
			{
				uint32 Left;
				uint32 Right;

				if (!GFX.pCurrentClip->Count [bg])
				{
					Left = 0;
					Right = 256;
				}
				else
				{
					Left = GFX.pCurrentClip->Left [clip][bg];
					Right = GFX.pCurrentClip->Right [clip][bg];

					if (Right <= Left)
						continue;
				}

				uint32 s = Left * GFX.PixSize + Y * GFX.PPL;
				uint32 HPos = (HOffset + Left) & OffsetMask;

				uint32 Quot = HPos >> 3;
				uint32 Count = 0;

				uint16 *t;
				if (BG.TileSize == 8)
				{
					if (Quot > 31)
						t = b2 + (Quot & 0x1f);
					else
						t = b1 + Quot;
				}
				else
				{
					if (Quot > 63)
						t = b2 + ((Quot >> 1) & 0x1f);
					else
						t = b1 + (Quot >> 1);
				}

				Width = Right - Left;
				// Left hand edge clipped tile
				if (HPos & 7)
				{
					uint32 Offset = (HPos & 7);
					Count = 8 - Offset;
					if (Count > Width)
						Count = Width;
					s -= Offset * GFX.PixSize;
					Tile = READ_2BYTES(t);
					GFX.Z1 = GFX.Z2 = depths [(Tile & 0x2000) >> 13];

					if (BG.TileSize == 8)
					{
						(*DrawClippedTilePtr) (Tile, s, Offset, Count, VirtAlign,
							Lines);
					}
					else
					{
						if (!(Tile & (V_FLIP | H_FLIP)))
						{
							// Normal, unflipped
							(*DrawClippedTilePtr) (Tile + t1 + (Quot & 1),
								s, Offset, Count, VirtAlign, Lines);
						}
						else
							if (Tile & H_FLIP)
							{
								if (Tile & V_FLIP)
								{
									// H & V flip
									(*DrawClippedTilePtr) (Tile + t2 + 1 - (Quot & 1),
										s, Offset, Count, VirtAlign, Lines);
								}
								else
								{
									// H flip only
									(*DrawClippedTilePtr) (Tile + t1 + 1 - (Quot & 1),
										s, Offset, Count, VirtAlign, Lines);
								}
							}
							else
							{
								// V flip only
								(*DrawClippedTilePtr) (Tile + t2 + (Quot & 1), s,
									Offset, Count, VirtAlign, Lines);
							}
					}

					if (BG.TileSize == 8)
					{
						t++;
						if (Quot == 31)
							t = b2;
						else if (Quot == 63)
							t = b1;
					}
					else
					{
						t += Quot & 1;
						if (Quot == 63)
							t = b2;
						else if (Quot == 127)
							t = b1;
					}
					Quot++;
					s += 8 * GFX.PixSize;
				}

				// Middle, unclipped tiles
				Count = Width - Count;
				int Middle = Count >> 3;
				Count &= 7;
				for (int C = Middle; C > 0; s += 8 * GFX.PixSize, Quot++, C--)
				{
					Tile = READ_2BYTES(t);
					GFX.Z1 = GFX.Z2 = depths [(Tile & 0x2000) >> 13];

					if (BG.TileSize != 8)
					{
						if (Tile & H_FLIP)
						{
							// Horizontal flip, but what about vertical flip ?
							if (Tile & V_FLIP)
							{
								// Both horzontal & vertical flip
								(*DrawTilePtr) (Tile + t2 + 1 - (Quot & 1), s,
									VirtAlign, Lines);
							}
							else
							{
								// Horizontal flip only
								(*DrawTilePtr) (Tile + t1 + 1 - (Quot & 1), s,
									VirtAlign, Lines);
							}
						}
						else
						{
							// No horizontal flip, but is there a vertical flip ?
							if (Tile & V_FLIP)
							{
								// Vertical flip only
								(*DrawTilePtr) (Tile + t2 + (Quot & 1), s,
									VirtAlign, Lines);
							}
							else
							{
								// Normal unflipped
								(*DrawTilePtr) (Tile + t1 + (Quot & 1), s,
									VirtAlign, Lines);
							}
						}
					}
					else
					{
						(*DrawTilePtr) (Tile, s, VirtAlign, Lines);
					}

					if (BG.TileSize == 8)
					{
						t++;
						if (Quot == 31)
							t = b2;
						else
							if (Quot == 63)
								t = b1;
					}
					else
					{
						t += Quot & 1;
						if (Quot == 63)
							t = b2;
						else
							if (Quot == 127)
								t = b1;
					}
				}
				// Right-hand edge clipped tiles
				if (Count)
				{
					Tile = READ_2BYTES(t);
					GFX.Z1 = GFX.Z2 = depths [(Tile & 0x2000) >> 13];

					if (BG.TileSize == 8)
						(*DrawClippedTilePtr) (Tile, s, 0, Count, VirtAlign,
						Lines);
					else
					{
						if (!(Tile & (V_FLIP | H_FLIP)))
						{
							// Normal, unflipped
							(*DrawClippedTilePtr) (Tile + t1 + (Quot & 1), s, 0,
								Count, VirtAlign, Lines);
						}
						else if (Tile & H_FLIP)
						{
							if (Tile & V_FLIP)
							{
								// H & V flip
								(*DrawClippedTilePtr) (Tile + t2 + 1 - (Quot & 1),
									s, 0, Count, VirtAlign,
									Lines);
							}
							else
							{
								// H flip only
								(*DrawClippedTilePtr) (Tile + t1 + 1 - (Quot & 1),
									s, 0, Count, VirtAlign,
									Lines);
							}
						}
						else
						{
							// V flip only
							(*DrawClippedTilePtr) (Tile + t2 + (Quot & 1),
								s, 0, Count, VirtAlign,
								Lines);
						}
					}
				}
		}
    }
}

#define RENDER_BACKGROUND_MODE7(TYPE,FUNC) \
    CHECK_SOUND(); \
\
    uint8 *VRAM1 = Memory.VRAM + 1; \
    if (GFX.r2130 & 1) \
    { \
	if (IPPU.DirectColourMapsNeedRebuild) \
	    S9xBuildDirectColourMaps (); \
	GFX.ScreenColors = DirectColourMaps [0]; \
    } \
    else \
	GFX.ScreenColors = IPPU.ScreenColors; \
\
    int aa, cc; \
    int dir; \
    int startx, endx; \
    uint32 Left = 0; \
    uint32 Right = 256; \
    uint32 ClipCount = GFX.pCurrentClip->Count [bg]; \
\
    if (!ClipCount) \
	ClipCount = 1; \
\
    Screen += GFX.StartY * GFX.Pitch; \
    uint8 *Depth = GFX.DB + GFX.StartY * GFX.PPL; \
    struct SLineMatrixData *l = &LineMatrixData [GFX.StartY]; \
\
    for (uint32 Line = GFX.StartY; Line <= GFX.EndY; Line++, Screen += GFX.Pitch, Depth += GFX.PPL, l++) \
    { \
	int yy; \
\
	int32 HOffset = ((int32) LineData [Line].BG[0].HOffset << M7) >> M7; \
	int32 VOffset = ((int32) LineData [Line].BG[0].VOffset << M7) >> M7; \
\
	int32 CentreX = ((int32) l->CentreX << M7) >> M7; \
	int32 CentreY = ((int32) l->CentreY << M7) >> M7; \
\
	if (PPU.Mode7VFlip) \
	    yy = 255 - (int) Line; \
	else \
	    yy = Line; \
\
    yy += CLIP_10_BIT_SIGNED(VOffset - CentreY); \
\
  int BB = l->MatrixB * yy + (CentreX << 8); \
	int DD = l->MatrixD * yy + (CentreY << 8); \
\
	for (uint32 clip = 0; clip < ClipCount; clip++) \
	{ \
	    if (GFX.pCurrentClip->Count [bg]) \
	    { \
		Left = GFX.pCurrentClip->Left [clip][bg]; \
		Right = GFX.pCurrentClip->Right [clip][bg]; \
		if (Right <= Left) \
		    continue; \
	    } \
	    TYPE *p = (TYPE *) Screen + Left; \
	    uint8 *d = Depth + Left; \
\
	    if (PPU.Mode7HFlip) \
	    { \
		startx = Right - 1; \
		endx = Left - 1; \
		dir = -1; \
		aa = -l->MatrixA; \
		cc = -l->MatrixC; \
	    } \
	    else \
	    { \
		startx = Left; \
		endx = Right; \
		dir = 1; \
		aa = l->MatrixA; \
		cc = l->MatrixC; \
	    } \
\
	int xx = startx + CLIP_10_BIT_SIGNED(HOffset - CentreX); \
	    int AA = l->MatrixA * xx; \
	    int CC = l->MatrixC * xx; \
\
	    if (!PPU.Mode7Repeat) \
	    { \
		for (int x = startx; x != endx; x += dir, AA += aa, CC += cc, p++, d++) \
		{ \
		    int X = ((AA + BB) >> 8) & 0x3ff; \
		    int Y = ((CC + DD) >> 8) & 0x3ff; \
		    uint8 *TileData = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
		    uint32 b = *(TileData + ((Y & 7) << 4) + ((X & 7) << 1)); \
		    GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
		    if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
		    { \
			*p = (FUNC); \
			*d = GFX.Z1; \
		    } \
		} \
	    } \
	    else \
	    { \
		for (int x = startx; x != endx; x += dir, AA += aa, CC += cc, p++, d++) \
		{ \
		    int X = ((AA + BB) >> 8); \
		    int Y = ((CC + DD) >> 8); \
\
		    if (Settings.Dezaemon && PPU.Mode7Repeat == 2) \
		    { \
			X &= 0x7ff; \
			Y &= 0x7ff; \
		    } \
\
		    if (((X | Y) & ~0x3ff) == 0) \
		    { \
			uint8 *TileData = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
			uint32 b = *(TileData + ((Y & 7) << 4) + ((X & 7) << 1)); \
			GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
			if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
			{ \
			    *p = (FUNC); \
			    *d = GFX.Z1; \
			} \
		    } \
		    else \
		    { \
			if (PPU.Mode7Repeat == 3) \
			{ \
			    X = (x + HOffset) & 7; \
			    Y = (yy + CentreY) & 7; \
			    uint32 b = *(VRAM1 + ((Y & 7) << 4) + ((X & 7) << 1)); \
			    GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
			    if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
			    { \
				*p = (FUNC); \
				*d = GFX.Z1; \
			    } \
			} \
		    } \
		} \
	    } \
	} \
    }

void DrawBGMode7Background (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7 (uint8, (uint8) (b & GFX.Mode7Mask))
}

void DrawBGMode7Background16 (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7 (uint16, GFX.ScreenColors [b & GFX.Mode7Mask]);
}

void DrawBGMode7Background16Add (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7 (uint16, *(d + GFX.DepthDelta) ?
					(*(d + GFX.DepthDelta) != 1 ?
					    COLOR_ADD (GFX.ScreenColors [b & GFX.Mode7Mask],
						       p [GFX.Delta]) :
					    COLOR_ADD (GFX.ScreenColors [b & GFX.Mode7Mask],
						       GFX.FixedColour)) :
					 GFX.ScreenColors [b & GFX.Mode7Mask]);
}

void DrawBGMode7Background16Add1_2 (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7 (uint16, *(d + GFX.DepthDelta) ?
					(*(d + GFX.DepthDelta) != 1 ?
					    COLOR_ADD1_2 (GFX.ScreenColors [b & GFX.Mode7Mask],
						       p [GFX.Delta]) :
					    COLOR_ADD (GFX.ScreenColors [b & GFX.Mode7Mask],
						       GFX.FixedColour)) :
					 GFX.ScreenColors [b & GFX.Mode7Mask]);
}

void DrawBGMode7Background16Sub (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7 (uint16, *(d + GFX.DepthDelta) ?
					(*(d + GFX.DepthDelta) != 1 ?
					    COLOR_SUB (GFX.ScreenColors [b & GFX.Mode7Mask],
						       p [GFX.Delta]) :
					    COLOR_SUB (GFX.ScreenColors [b & GFX.Mode7Mask],
						       GFX.FixedColour)) :
					 GFX.ScreenColors [b & GFX.Mode7Mask]);
}

void DrawBGMode7Background16Sub1_2 (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7 (uint16, *(d + GFX.DepthDelta) ?
					(*(d + GFX.DepthDelta) != 1 ?
					    COLOR_SUB1_2 (GFX.ScreenColors [b & GFX.Mode7Mask],
						       p [GFX.Delta]) :
					    COLOR_SUB (GFX.ScreenColors [b & GFX.Mode7Mask],
						       GFX.FixedColour)) :
					 GFX.ScreenColors [b & GFX.Mode7Mask]);
}

#define RENDER_BACKGROUND_MODE7_i(TYPE,FUNC,COLORFUNC) \
    CHECK_SOUND(); \
\
    uint8 *VRAM1 = Memory.VRAM + 1; \
    if (GFX.r2130 & 1) \
    { \
        if (IPPU.DirectColourMapsNeedRebuild) \
            S9xBuildDirectColourMaps (); \
        GFX.ScreenColors = DirectColourMaps [0]; \
    } \
    else \
        GFX.ScreenColors = IPPU.ScreenColors; \
    \
    int aa, cc; \
    int dir; \
    int startx, endx; \
    uint32 Left = 0; \
    uint32 Right = 256; \
    uint32 ClipCount = GFX.pCurrentClip->Count [bg]; \
    \
    if (!ClipCount) \
        ClipCount = 1; \
    \
    Screen += GFX.StartY * GFX.Pitch; \
    uint8 *Depth = GFX.DB + GFX.StartY * GFX.PPL; \
    struct SLineMatrixData *l = &LineMatrixData [GFX.StartY]; \
    bool8 allowSimpleCase = FALSE; \
    if (!l->MatrixB && !l->MatrixC && (l->MatrixA == 0x0100) && (l->MatrixD == 0x0100) \
        && !LineMatrixData[GFX.EndY].MatrixB && !LineMatrixData[GFX.EndY].MatrixC \
        && (LineMatrixData[GFX.EndY].MatrixA == 0x0100) && (LineMatrixData[GFX.EndY].MatrixD == 0x0100) \
        ) \
        allowSimpleCase = TRUE;  \
    \
    for (uint32 Line = GFX.StartY; Line <= GFX.EndY; Line++, Screen += GFX.Pitch, Depth += GFX.PPL, l++) \
    { \
        int yy; \
        \
        int HOffset = ((int) LineData [Line].BG[0].HOffset << M7) >> M7; \
        int VOffset = ((int) LineData [Line].BG[0].VOffset << M7) >> M7; \
        \
        int CentreX = ((int) l->CentreX << M7) >> M7; \
        int CentreY = ((int) l->CentreY << M7) >> M7; \
        \
        if (PPU.Mode7VFlip) \
            yy = 255 - (int) Line; \
        else \
            yy = Line; \
        \
	\
	    yy += CLIP_10_BIT_SIGNED(VOffset - CentreY); \
        bool8 simpleCase = FALSE; \
        int BB; \
        int DD; \
        /* Make a special case for the identity matrix, since it's a common case and */ \
        /* can be done much more quickly without special effects */ \
        if (allowSimpleCase && !l->MatrixB && !l->MatrixC && (l->MatrixA == 0x0100) && (l->MatrixD == 0x0100)) \
        { \
            BB = CentreX << 8; \
            DD = (yy + CentreY) << 8; \
            simpleCase = TRUE; \
        } \
        else \
        { \
            BB = l->MatrixB * yy + (CentreX << 8); \
            DD = l->MatrixD * yy + (CentreY << 8); \
        } \
        \
        for (uint32 clip = 0; clip < ClipCount; clip++) \
        { \
            if (GFX.pCurrentClip->Count [bg]) \
            { \
                Left = GFX.pCurrentClip->Left [clip][bg]; \
                Right = GFX.pCurrentClip->Right [clip][bg]; \
                if (Right <= Left) \
                    continue; \
            } \
            TYPE *p = (TYPE *) Screen + Left; \
            uint8 *d = Depth + Left; \
            \
            if (PPU.Mode7HFlip) \
            { \
                startx = Right - 1; \
                endx = Left - 1; \
                dir = -1; \
                aa = -l->MatrixA; \
                cc = -l->MatrixC; \
            } \
            else \
            { \
                startx = Left; \
                endx = Right; \
                dir = 1; \
                aa = l->MatrixA; \
                cc = l->MatrixC; \
            } \
            int xx; \
	\
			xx = startx + CLIP_10_BIT_SIGNED(HOffset - CentreX); \
            int AA, CC = 0; \
            if (simpleCase) \
            { \
                AA = xx << 8; \
            } \
            else \
            { \
                AA = l->MatrixA * xx; \
                CC = l->MatrixC * xx; \
            } \
            if (simpleCase) \
            { \
                if (!PPU.Mode7Repeat) \
                { \
                    int x = startx; \
                    do \
                    { \
                        int X = ((AA + BB) >> 8) & 0x3ff; \
                        int Y = (DD >> 8) & 0x3ff; \
                        uint8 *TileData = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
                        uint32 b = *(TileData + ((Y & 7) << 4) + ((X & 7) << 1)); \
			GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
                        if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
                        { \
                            TYPE theColor = COLORFUNC; \
                            *p = (FUNC) | ALPHA_BITS_MASK; \
                            *d = GFX.Z1; \
                        } \
                        AA += aa, p++, d++; \
			x += dir; \
                    } while (x != endx); \
                } \
                else \
                { \
                    int x = startx; \
                    do { \
                        int X = (AA + BB) >> 8; \
                        int Y = DD >> 8; \
\
			if(Settings.Dezaemon && PPU.Mode7Repeat == 2) \
			{ \
			    X &= 0x7ff; \
			    Y &= 0x7ff; \
			} \
\
                        if (((X | Y) & ~0x3ff) == 0) \
                        { \
                            uint8 *TileData = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
			    uint32 b = *(TileData + ((Y & 7) << 4) + ((X & 7) << 1)); \
			    GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
                            if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
                            { \
                                TYPE theColor = COLORFUNC; \
                                *p = (FUNC) | ALPHA_BITS_MASK; \
                                *d = GFX.Z1; \
                            } \
                        } \
                        else if (PPU.Mode7Repeat == 3) \
                        { \
                            X = (x + HOffset) & 7; \
                            Y = (yy + CentreY) & 7; \
			    uint8 *TileData = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
			    uint32 b = *(TileData + ((Y & 7) << 4) + ((X & 7) << 1)); \
			    GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
                            if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
                            { \
                                TYPE theColor = COLORFUNC; \
                                *p = (FUNC) | ALPHA_BITS_MASK; \
                                *d = GFX.Z1; \
                            } \
                        } \
                        AA += aa; p++; d++; \
			x += dir; \
                    } while (x != endx); \
                } \
            } \
            else if (!PPU.Mode7Repeat) \
            { \
                /* The bilinear interpolator: get the colors at the four points surrounding */ \
                /* the location of one point in the _sampled_ image, and weight them according */ \
                /* to their (city block) distance.  It's very smooth, but blurry with "close up" */ \
                /* points. */ \
                \
                /* 460 (slightly less than 2 source pixels per displayed pixel) is an educated */ \
                /* guess for where bilinear filtering will become a poor method for averaging. */ \
                /* (When reducing the image, the weighting used by a bilinear filter becomes */ \
                /* arbitrary, and a simple mean is a better way to represent the source image.) */ \
                /* You can think of this as a kind of mipmapping. */ \
                if ((aa < 460 && aa > -460) && (cc < 460 && cc > -460)) \
                {\
                    for (int x = startx; x != endx; x += dir, AA += aa, CC += cc, p++, d++) \
                    { \
                        uint32 xPos = AA + BB; \
                        uint32 xPix = xPos >> 8; \
                        uint32 yPos = CC + DD; \
                        uint32 yPix = yPos >> 8; \
                        uint32 X = xPix & 0x3ff; \
                        uint32 Y = yPix & 0x3ff; \
                        uint8 *TileData = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
			uint32 b = *(TileData + ((Y & 7) << 4) + ((X & 7) << 1)); \
			GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
                        if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
                        { \
                            /* X10 and Y01 are the X and Y coordinates of the next source point over. */ \
                            uint32 X10 = (xPix + dir) & 0x3ff; \
                            uint32 Y01 = (yPix + (PPU.Mode7VFlip?-1:1)) & 0x3ff; \
                            uint8 *TileData10 = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X10 >> 2) & ~1)] << 7); \
                            uint8 *TileData11 = VRAM1 + (Memory.VRAM[((Y01 & ~7) << 5) + ((X10 >> 2) & ~1)] << 7); \
                            uint8 *TileData01 = VRAM1 + (Memory.VRAM[((Y01 & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
                            uint32 p1 = COLORFUNC; \
                            p1 = (p1 & FIRST_THIRD_COLOR_MASK) | ((p1 & SECOND_COLOR_MASK) << 16); \
                            b = *(TileData10 + ((Y & 7) << 4) + ((X10 & 7) << 1)); \
                            uint32 p2 = COLORFUNC; \
                            p2 = (p2 & FIRST_THIRD_COLOR_MASK) | ((p2 & SECOND_COLOR_MASK) << 16); \
                            b = *(TileData11 + ((Y01 & 7) << 4) + ((X10 & 7) << 1)); \
                            uint32 p4 = COLORFUNC; \
                            p4 = (p4 & FIRST_THIRD_COLOR_MASK) | ((p4 & SECOND_COLOR_MASK) << 16); \
                            b = *(TileData01 + ((Y01 & 7) << 4) + ((X & 7) << 1)); \
                            uint32 p3 = COLORFUNC; \
                            p3 = (p3 & FIRST_THIRD_COLOR_MASK) | ((p3 & SECOND_COLOR_MASK) << 16); \
                            /* Xdel, Ydel: position (in 1/32nds) between the points */ \
                            uint32 Xdel = (xPos >> 3) & 0x1F; \
                            uint32 Ydel = (yPos >> 3) & 0x1F; \
                            uint32 XY = (Xdel*Ydel) >> 5; \
                            uint32 area1 = 0x20 + XY - Xdel - Ydel; \
                            uint32 area2 = Xdel - XY; \
                            uint32 area3 = Ydel - XY; \
                            uint32 area4 = XY; \
                            if(PPU.Mode7HFlip){ \
                                uint32 tmp=area1; area1=area2; area2=tmp; \
                                tmp=area3; area3=area4; area4=tmp; \
                            } \
                            if(PPU.Mode7VFlip){ \
                                uint32 tmp=area1; area1=area3; area3=tmp; \
                                tmp=area2; area2=area4; area4=tmp; \
                            } \
                            uint32 tempColor = ((area1 * p1) + \
                                                (area2 * p2) + \
                                                (area3 * p3) + \
                                                (area4 * p4)) >> 5; \
                            TYPE theColor = (tempColor & FIRST_THIRD_COLOR_MASK) | ((tempColor >> 16) & SECOND_COLOR_MASK); \
                            *p = (FUNC) | ALPHA_BITS_MASK; \
                            *d = GFX.Z1; \
                        } \
                    } \
                } \
                else \
                    /* The oversampling method: get the colors at four corners of a square */ \
                    /* in the _displayed_ image, and average them.  It's sharp and clean, but */ \
                    /* gives the usual huge pixels when the source image gets "close." */ \
                { \
                    /* Find the dimensions of the square in the source image whose corners will be examined. */ \
                    uint32 aaDelX = aa >> 1; \
                    uint32 ccDelX = cc >> 1; \
                    uint32 bbDelY = l->MatrixB >> 1; \
                    uint32 ddDelY = l->MatrixD >> 1; \
                    /* Offset the location within the source image so that the four sampled points */ \
                    /* center around where the single point would otherwise have been drawn. */ \
                    BB -= (bbDelY >> 1); \
                    DD -= (ddDelY >> 1); \
                    AA -= (aaDelX >> 1); \
                    CC -= (ccDelX >> 1); \
                    uint32 BB10 = BB + aaDelX; \
                    uint32 BB01 = BB + bbDelY; \
                    uint32 BB11 = BB + aaDelX + bbDelY; \
                    uint32 DD10 = DD + ccDelX; \
                    uint32 DD01 = DD + ddDelY; \
                    uint32 DD11 = DD + ccDelX + ddDelY; \
                    for (int x = startx; x != endx; x += dir, AA += aa, CC += cc, p++, d++) \
                    { \
                        uint32 X = ((AA + BB) >> 8) & 0x3ff; \
                        uint32 Y = ((CC + DD) >> 8) & 0x3ff; \
                        uint8 *TileData = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
			uint32 b = *(TileData + ((Y & 7) << 4) + ((X & 7) << 1)); \
			GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
                        if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
                        { \
                            /* X, Y, X10, Y10, etc. are the coordinates of the four pixels within the */ \
                            /* source image that we're going to examine. */ \
                            uint32 X10 = ((AA + BB10) >> 8) & 0x3ff; \
                            uint32 Y10 = ((CC + DD10) >> 8) & 0x3ff; \
                            uint32 X01 = ((AA + BB01) >> 8) & 0x3ff; \
                            uint32 Y01 = ((CC + DD01) >> 8) & 0x3ff; \
                            uint32 X11 = ((AA + BB11) >> 8) & 0x3ff; \
                            uint32 Y11 = ((CC + DD11) >> 8) & 0x3ff; \
                            uint8 *TileData10 = VRAM1 + (Memory.VRAM[((Y10 & ~7) << 5) + ((X10 >> 2) & ~1)] << 7); \
                            uint8 *TileData01 = VRAM1 + (Memory.VRAM[((Y01 & ~7) << 5) + ((X01 >> 2) & ~1)] << 7); \
                            uint8 *TileData11 = VRAM1 + (Memory.VRAM[((Y11 & ~7) << 5) + ((X11 >> 2) & ~1)] << 7); \
                            TYPE p1 = COLORFUNC; \
                            b = *(TileData10 + ((Y10 & 7) << 4) + ((X10 & 7) << 1)); \
                            TYPE p2 = COLORFUNC; \
                            b = *(TileData01 + ((Y01 & 7) << 4) + ((X01 & 7) << 1)); \
                            TYPE p3 = COLORFUNC; \
                            b = *(TileData11 + ((Y11 & 7) << 4) + ((X11 & 7) << 1)); \
                            TYPE p4 = COLORFUNC; \
                            TYPE theColor = Q_INTERPOLATE(p1, p2, p3, p4); \
                            *p = (FUNC) | ALPHA_BITS_MASK; \
                            *d = GFX.Z1; \
                        } \
                    } \
                } \
            } \
            else \
            { \
                for (int x = startx; x != endx; x += dir, AA += aa, CC += cc, p++, d++) \
                { \
                    uint32 xPos = AA + BB; \
                    uint32 xPix = xPos >> 8; \
                    uint32 yPos = CC + DD; \
                    uint32 yPix = yPos >> 8; \
                    uint32 X = xPix; \
                    uint32 Y = yPix; \
                    \
\
		    if(Settings.Dezaemon && PPU.Mode7Repeat == 2) \
		    { \
			X &= 0x7ff; \
			Y &= 0x7ff; \
		    } \
\
                    if (((X | Y) & ~0x3ff) == 0) \
                    { \
                        uint8 *TileData = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
			uint32 b = *(TileData + ((Y & 7) << 4) + ((X & 7) << 1)); \
			GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
                        if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
                        { \
                            /* X10 and Y01 are the X and Y coordinates of the next source point over. */ \
                            uint32 X10 = (xPix + dir) & 0x3ff; \
                            uint32 Y01 = (yPix + dir) & 0x3ff; \
                            uint8 *TileData10 = VRAM1 + (Memory.VRAM[((Y & ~7) << 5) + ((X10 >> 2) & ~1)] << 7); \
                            uint8 *TileData11 = VRAM1 + (Memory.VRAM[((Y01 & ~7) << 5) + ((X10 >> 2) & ~1)] << 7); \
                            uint8 *TileData01 = VRAM1 + (Memory.VRAM[((Y01 & ~7) << 5) + ((X >> 2) & ~1)] << 7); \
                            uint32 p1 = COLORFUNC; \
                            p1 = (p1 & FIRST_THIRD_COLOR_MASK) | ((p1 & SECOND_COLOR_MASK) << 16); \
                            b = *(TileData10 + ((Y & 7) << 4) + ((X10 & 7) << 1)); \
                            uint32 p2 = COLORFUNC; \
                            p2 = (p2 & FIRST_THIRD_COLOR_MASK) | ((p2 & SECOND_COLOR_MASK) << 16); \
                            b = *(TileData11 + ((Y01 & 7) << 4) + ((X10 & 7) << 1)); \
                            uint32 p4 = COLORFUNC; \
                            p4 = (p4 & FIRST_THIRD_COLOR_MASK) | ((p4 & SECOND_COLOR_MASK) << 16); \
                            b = *(TileData01 + ((Y01 & 7) << 4) + ((X & 7) << 1)); \
                            uint32 p3 = COLORFUNC; \
                            p3 = (p3 & FIRST_THIRD_COLOR_MASK) | ((p3 & SECOND_COLOR_MASK) << 16); \
                            /* Xdel, Ydel: position (in 1/32nds) between the points */ \
                            uint32 Xdel = (xPos >> 3) & 0x1F; \
                            uint32 Ydel = (yPos >> 3) & 0x1F; \
                            uint32 XY = (Xdel*Ydel) >> 5; \
                            uint32 area1 = 0x20 + XY - Xdel - Ydel; \
                            uint32 area2 = Xdel - XY; \
                            uint32 area3 = Ydel - XY; \
                            uint32 area4 = XY; \
                            uint32 tempColor = ((area1 * p1) + \
                                                (area2 * p2) + \
                                                (area3 * p3) + \
                                                (area4 * p4)) >> 5; \
                            TYPE theColor = (tempColor & FIRST_THIRD_COLOR_MASK) | ((tempColor >> 16) & SECOND_COLOR_MASK); \
                            *p = (FUNC) | ALPHA_BITS_MASK; \
                            *d = GFX.Z1; \
                        } \
                    } \
                    else \
                    { \
                        if (PPU.Mode7Repeat == 3) \
                        { \
                            X = (x + HOffset) & 7; \
                            Y = (yy + CentreY) & 7; \
			    uint32 b = *(VRAM1 + ((Y & 7) << 4) + ((X & 7) << 1)); \
			    GFX.Z1 = Mode7Depths [(b & GFX.Mode7PriorityMask) >> 7]; \
                            if (GFX.Z1 > *d && (b & GFX.Mode7Mask) ) \
                            { \
                                TYPE theColor = COLORFUNC; \
                                *p = (FUNC) | ALPHA_BITS_MASK; \
                                *d = GFX.Z1; \
                            } \
                        } \
                    } \
                } \
            } \
        } \
    }

STATIC uint32 Q_INTERPOLATE(uint32 A, uint32 B, uint32 C, uint32 D)
{
    register uint32 x = ((A >> 2) & HIGH_BITS_SHIFTED_TWO_MASK) +
                            ((B >> 2) & HIGH_BITS_SHIFTED_TWO_MASK) +
                            ((C >> 2) & HIGH_BITS_SHIFTED_TWO_MASK) +
                            ((D >> 2) & HIGH_BITS_SHIFTED_TWO_MASK);
    register uint32 y = (A & TWO_LOW_BITS_MASK) +
                            (B & TWO_LOW_BITS_MASK) +
                            (C & TWO_LOW_BITS_MASK) +
                            (D & TWO_LOW_BITS_MASK);
    y = (y>>2) & TWO_LOW_BITS_MASK;
    return x+y;
}

void DrawBGMode7Background16_i (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7_i (uint16, theColor, (GFX.ScreenColors[b & GFX.Mode7Mask]));
}

void DrawBGMode7Background16Add_i (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7_i (uint16, *(d + GFX.DepthDelta) ?
					(*(d + GFX.DepthDelta) != 1 ?
					    (COLOR_ADD (theColor,
						       p [GFX.Delta])) :
					    (COLOR_ADD (theColor,
						       GFX.FixedColour))) :
					 theColor, (GFX.ScreenColors[b & GFX.Mode7Mask]));
}

void DrawBGMode7Background16Add1_2_i (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7_i (uint16, *(d + GFX.DepthDelta) ?
					(*(d + GFX.DepthDelta) != 1 ?
					    COLOR_ADD1_2 (theColor,
						          p [GFX.Delta]) :
					    COLOR_ADD (theColor,
						       GFX.FixedColour)) :
					 theColor, (GFX.ScreenColors[b & GFX.Mode7Mask]));
}

void DrawBGMode7Background16Sub_i (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7_i (uint16, *(d + GFX.DepthDelta) ?
					(*(d + GFX.DepthDelta) != 1 ?
					    COLOR_SUB (theColor,
						       p [GFX.Delta]) :
					    COLOR_SUB (theColor,
						       GFX.FixedColour)) :
					 theColor, (GFX.ScreenColors[b & GFX.Mode7Mask]));
}

void DrawBGMode7Background16Sub1_2_i (uint8 *Screen, int bg)
{
    RENDER_BACKGROUND_MODE7_i (uint16, *(d + GFX.DepthDelta) ?
					(*(d + GFX.DepthDelta) != 1 ?
					    COLOR_SUB1_2 (theColor,
						       p [GFX.Delta]) :
					    COLOR_SUB (theColor,
						       GFX.FixedColour)) :
					 theColor, (GFX.ScreenColors[b & GFX.Mode7Mask]));
}

#define _BUILD_SETUP(F) \
GFX.BuildPixel = BuildPixel##F; \
GFX.BuildPixel2 = BuildPixel2##F; \
GFX.DecomposePixel = DecomposePixel##F; \
RED_LOW_BIT_MASK = RED_LOW_BIT_MASK_##F; \
GREEN_LOW_BIT_MASK = GREEN_LOW_BIT_MASK_##F; \
BLUE_LOW_BIT_MASK = BLUE_LOW_BIT_MASK_##F; \
RED_HI_BIT_MASK = RED_HI_BIT_MASK_##F; \
GREEN_HI_BIT_MASK = GREEN_HI_BIT_MASK_##F; \
BLUE_HI_BIT_MASK = BLUE_HI_BIT_MASK_##F; \
MAX_RED = MAX_RED_##F; \
MAX_GREEN = MAX_GREEN_##F; \
MAX_BLUE = MAX_BLUE_##F; \
GREEN_HI_BIT = ((MAX_GREEN_##F + 1) >> 1); \
SPARE_RGB_BIT_MASK = SPARE_RGB_BIT_MASK_##F; \
RGB_LOW_BITS_MASK = (RED_LOW_BIT_MASK_##F | \
		     GREEN_LOW_BIT_MASK_##F | \
		     BLUE_LOW_BIT_MASK_##F); \
RGB_HI_BITS_MASK = (RED_HI_BIT_MASK_##F | \
		    GREEN_HI_BIT_MASK_##F | \
		    BLUE_HI_BIT_MASK_##F); \
RGB_HI_BITS_MASKx2 = ((RED_HI_BIT_MASK_##F | \
		       GREEN_HI_BIT_MASK_##F | \
		       BLUE_HI_BIT_MASK_##F) << 1); \
RGB_REMOVE_LOW_BITS_MASK = ~RGB_LOW_BITS_MASK; \
FIRST_COLOR_MASK = FIRST_COLOR_MASK_##F; \
SECOND_COLOR_MASK = SECOND_COLOR_MASK_##F; \
THIRD_COLOR_MASK = THIRD_COLOR_MASK_##F; \
ALPHA_BITS_MASK = ALPHA_BITS_MASK_##F; \
FIRST_THIRD_COLOR_MASK = FIRST_COLOR_MASK | THIRD_COLOR_MASK; \
TWO_LOW_BITS_MASK = RGB_LOW_BITS_MASK | (RGB_LOW_BITS_MASK << 1); \
HIGH_BITS_SHIFTED_TWO_MASK = (( (FIRST_COLOR_MASK | SECOND_COLOR_MASK | THIRD_COLOR_MASK) & \
                                ~TWO_LOW_BITS_MASK ) >> 2);

void RenderScreen (uint8 *Screen, bool8 sub, bool8 force_no_add, uint8 D)
{
    bool8 BG0;
    bool8 BG1;
    bool8 BG2;
    bool8 BG3;
    bool8 OB;

    GFX.S = Screen;

    if (!sub)
    {
		GFX.pCurrentClip = &IPPU.Clip [0];
		BG0 = ON_MAIN (0);
		BG1 = ON_MAIN (1);
		BG2 = ON_MAIN (2);
		BG3 = ON_MAIN (3);
		OB  = ON_MAIN (4);
    }
    else
    {
		GFX.pCurrentClip = &IPPU.Clip [1];
		BG0 = ON_SUB (0);
		BG1 = ON_SUB (1);
		BG2 = ON_SUB (2);
		BG3 = ON_SUB (3);
		OB  = ON_SUB (4);
    }

    sub |= force_no_add;

    if (PPU.BGMode <= 1)
    {
		if (OB)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(4));
			DrawOBJS (!sub, D);
		}
		if (BG0)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(0));
			DrawBackground (PPU.BGMode, 0, D + 10, D + 14);
		}
		if (BG1)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(1));
			DrawBackground (PPU.BGMode, 1, D + 9, D + 13);
		}
		if (BG2)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(2));
			DrawBackground (PPU.BGMode, 2, D + 3,
				PPU.BG3Priority ? D + 17 : D + 6);
		}
		if (BG3 && PPU.BGMode == 0)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(3));
			DrawBackground (PPU.BGMode, 3, D + 2, D + 5);
		}
    }
    else if (PPU.BGMode != 7)
    {
		if (OB)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(4));
			DrawOBJS (!sub, D);
		}
		if (BG0)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(0));
			DrawBackground (PPU.BGMode, 0, D + 5, D + 13);
		}
		if (PPU.BGMode != 6 && BG1)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(1));
			DrawBackground (PPU.BGMode, 1, D + 2, D + 9);
		}
    }
    else
    {
		if (OB)
		{
			SelectTileRenderer (sub || !SUB_OR_ADD(4));
			DrawOBJS (!sub, D);
		}
		if (BG0 || ((Memory.FillRAM [0x2133] & 0x40) && BG1))
		{
			int bg;

			if ((Memory.FillRAM [0x2133] & 0x40)&&BG1)
			{
				GFX.Mode7Mask = 0x7f;
				GFX.Mode7PriorityMask = 0x80;
				Mode7Depths [0] = (BG0?5:1) + D;
				Mode7Depths [1] = 9 + D;
				bg = 1;
			}
			else
			{
				GFX.Mode7Mask = 0xff;
				GFX.Mode7PriorityMask = 0;
				Mode7Depths [0] = 5 + D;
				Mode7Depths [1] = 5 + D;
				bg = 0;
			}
			if (sub || !SUB_OR_ADD(0))
			{
				if (!Settings.Mode7Interpolate)
					DrawBGMode7Background16 (Screen, bg);
				else
					DrawBGMode7Background16_i (Screen, bg);
			}
			else
			{
				if (GFX.r2131 & 0x80)
				{
					if (GFX.r2131 & 0x40)
					{
						if (!Settings.Mode7Interpolate)
							DrawBGMode7Background16Sub1_2 (Screen, bg);
						else
							DrawBGMode7Background16Sub1_2_i (Screen, bg);
					}
					else
					{
						if (!Settings.Mode7Interpolate)
							DrawBGMode7Background16Sub (Screen, bg);
						else
							DrawBGMode7Background16Sub_i (Screen, bg);
					}
				}
				else
				{
					if (GFX.r2131 & 0x40)
					{
						if (!Settings.Mode7Interpolate)
							DrawBGMode7Background16Add1_2 (Screen, bg);
						else
							DrawBGMode7Background16Add1_2_i (Screen, bg);
					}
					else
					{
						if (!Settings.Mode7Interpolate)
							DrawBGMode7Background16Add (Screen, bg);
						else
							DrawBGMode7Background16Add_i (Screen, bg);
					}
				}
			}
		}
    }
}

#include "native_gfx_font.h"

void DisplayChar (uint8 *Screen, uint8 c)
{
    int line = (((c & 0x7f) - 32) >> 4) * font_height;
    int offset = (((c & 0x7f) - 32) & 15) * font_width;
    if (Settings.SixteenBit)
    {
		int h, w;
		uint16 *s = (uint16 *) Screen;
		for (h = 0; h < font_height; h++, line++,
			s += GFX.PPL - font_width)
		{
			for (w = 0; w < font_width; w++, s++)
			{
				uint8 p = font [line][offset + w];

				if (p == '#')
				{
					/*
					if(Memory.Hacked)
						*s= BUILD_PIXEL(31,0,0);
					else if(Memory.Iffy)
						*s= BUILD_PIXEL(31,31,0);
					else if(Memory.Iformat==1)
						*s= BUILD_PIXEL(0,31,0);
					else if(Memory.Iformat==2)
						*s= BUILD_PIXEL(0,31,31);
					else *s = 0xffff;
					*/
					*s=Settings.DisplayColor;
				}
				else
					if (p == '.')
						*s = BLACK;
			}
		}
    }
    else
    {
		int h, w;
		uint8 *s = Screen;
		for (h = 0; h < font_height; h++, line++,
			s += GFX.PPL - font_width)
		{
			for (w = 0; w < font_width; w++, s++)
			{
				uint8 p = font [line][offset + w];

				if (p == '#')
					*s = 255;
				else
					if (p == '.')
						*s = BLACK;
			}
		}
    }
}

static void S9xDisplayFrameRate ()
{
    uint8 *Screen = GFX.Screen + 2 +
		    (IPPU.RenderedScreenHeight - font_height - 1) * GFX.Pitch2;
    char string [10];
    int len = 5;

    sprintf (string, "%02d/%02d", IPPU.DisplayedRenderedFrameCount,
	     (int) Memory.ROMFramesPerSecond);

    int i;
    for (i = 0; i < len; i++)
    {
	DisplayChar (Screen, string [i]);
	Screen += Settings.SixteenBit ? (font_width - 1) * sizeof (uint16) :
		  (font_width - 1);
    }
}

static void S9xDisplayString (const char *string)
{
    uint8 *Screen = GFX.Screen + 2 +
		    (IPPU.RenderedScreenHeight - font_height * 5) * GFX.Pitch2;
    int len = strlen (string);
    int max_chars = IPPU.RenderedScreenWidth / (font_width - 1);
    int char_count = 0;
    int i;

    for (i = 0; i < len; i++, char_count++)
    {
	if (char_count >= max_chars || string [i] < 32)
	{
	    Screen -= Settings.SixteenBit ?
			(font_width - 1) * sizeof (uint16) * max_chars :
			(font_width - 1) * max_chars;
	    Screen += font_height * GFX.Pitch;
	    if (Screen >= GFX.Screen + GFX.Pitch * IPPU.RenderedScreenHeight)
		break;
	    char_count -= max_chars;
	}
	if (string [i] < 32)
	    continue;
	DisplayChar (Screen, string [i]);
	Screen += Settings.SixteenBit ? (font_width - 1) * sizeof (uint16) :
		  (font_width - 1);
    }
}

void S9xUpdateScreen ()
{
    int32 x2 = 1;

    GFX.S = GFX.Screen;
    GFX.r2131 = Memory.FillRAM [0x2131];
    GFX.r212c = Memory.FillRAM [0x212c];
    GFX.r212d = Memory.FillRAM [0x212d];
    GFX.r2130 = Memory.FillRAM [0x2130];
    GFX.Pseudo = (Memory.FillRAM [0x2133] & 8) != 0 &&
		(GFX.r212c & 15) != (GFX.r212d & 15) &&
		(GFX.r2131 & 0x3f) == 0;

    if (IPPU.OBJChanged)
		S9xSetupOBJ ();

    if (PPU.RecomputeClipWindows)
    {
		ComputeClipWindows ();
		PPU.RecomputeClipWindows = FALSE;
    }

    GFX.StartY = IPPU.PreviousLine;
    if ((GFX.EndY = IPPU.CurrentLine - 1) >= PPU.ScreenHeight)
		GFX.EndY = PPU.ScreenHeight - 1;

    uint32 starty = GFX.StartY;
    uint32 endy = GFX.EndY;

    if (Settings.SupportHiRes &&
		(PPU.BGMode == 5 || PPU.BGMode == 6 ||
		IPPU.Interlace || IPPU.DoubleHeightPixels))
    {
		if (PPU.BGMode == 5 || PPU.BGMode == 6|| IPPU.Interlace)
		{
			IPPU.RenderedScreenWidth = 512;
			x2 = 2;
		}
		if (IPPU.DoubleHeightPixels)
		{
			starty = GFX.StartY * 2;
			endy = GFX.EndY * 2 + 1;
		}
		if ((PPU.BGMode == 5 || PPU.BGMode == 6) && !IPPU.DoubleWidthPixels)
		{
			// The game has switched from lo-res to hi-res mode part way down
			// the screen. Scale any existing lo-res pixels on screen
			if (Settings.SixteenBit)
			{
#if defined (USE_GLIDE) || defined (USE_OPENGL)
                if (
#ifdef USE_GLIDE
                    (Settings.GlideEnable && GFX.Pitch == 512) ||
#endif
#ifdef USE_OPENGL
                    (Settings.OpenGLEnable && GFX.Pitch == 512) ||
#endif
                    0)
				{
					// Have to back out of the speed up hack where the low res.
					// SNES image was rendered into a 256x239 sized buffer,
					// ignoring the true, larger size of the buffer.
					for (register int32 y = (int32) starty - 1; y >= 0; y--)
					{
						register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + 255;
						register uint16 *q = (uint16 *) (GFX.Screen + y * GFX.RealPitch) + 510;
						for (register int x = 255; x >= 0; x--, p--, q -= 2)
							*q = *(q + 1) = *p;
					}

					GFX.Pitch = GFX.Pitch2 = GFX.RealPitch;
                    GFX.PPL = GFX.Pitch >> 1;
                    GFX.PPLx2 = GFX.Pitch;
                    GFX.ZPitch = GFX.PPL;
				}
				else
#endif
					for (register uint32 y = 0; y < starty; y++)
					{
						register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + 255;
						register uint16 *q = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + 510;

						for (register int x = 255; x >= 0; x--, p--, q -= 2)
							*q = *(q + 1) = *p;
					}
			}
			else
			{
				for (register uint32 y = 0; y < starty; y++)
				{
					register uint8 *p = GFX.Screen + y * GFX.Pitch2 + 255;
					register uint8 *q = GFX.Screen + y * GFX.Pitch2 + 510;
					for (register int x = 255; x >= 0; x--, p--, q -= 2)
						*q = *(q + 1) = *p;
				}
			}
			IPPU.DoubleWidthPixels = TRUE;
		}
        // BJ: And we have to change the height if Interlace gets set,
        //     too.
		if (IPPU.Interlace && !IPPU.DoubleHeightPixels)
		{
			starty = GFX.StartY * 2;
			endy = GFX.EndY * 2 + 1;
            IPPU.RenderedScreenHeight = PPU.ScreenHeight << 1;
            IPPU.DoubleHeightPixels = TRUE;
            GFX.Pitch2 = GFX.RealPitch;
            GFX.Pitch = GFX.RealPitch * 2;
            if (Settings.SixteenBit)
                GFX.PPL = GFX.PPLx2 = GFX.RealPitch;
            else
                GFX.PPL = GFX.PPLx2 = GFX.RealPitch << 1;

            // The game has switched from non-interlaced to interlaced mode
            // part way down the screen. Scale everything.
            for (register int32 y = (int32) GFX.StartY - 1; y >= 0; y--)
			{
				memmove (GFX.Screen + y * 2 * GFX.Pitch2,
					GFX.Screen + y * GFX.Pitch2,
					GFX.Pitch2);
				memmove (GFX.Screen + (y * 2 + 1) * GFX.Pitch2,
					GFX.Screen + y * GFX.Pitch2,
					GFX.Pitch2);
			}
		}

    }

    uint32 black = BLACK | (BLACK << 16);

    if (Settings.Transparency && Settings.SixteenBit)
    {
		if (GFX.Pseudo)
		{
			GFX.r2131 = 0x5f;
            GFX.r212c &= (Memory.FillRAM [0x212d] | 0xf0);
            GFX.r212d |= (Memory.FillRAM [0x212c] & 0x0f);
			GFX.r2130 |= 2;
		}

		if (!PPU.ForcedBlanking && ADD_OR_SUB_ON_ANYTHING &&
			(GFX.r2130 & 0x30) != 0x30 &&
			!((GFX.r2130 & 0x30) == 0x10 && IPPU.Clip[1].Count[5] == 0))
		{
			struct ClipData *pClip;

			GFX.FixedColour = BUILD_PIXEL (IPPU.XB [PPU.FixedColourRed],
				IPPU.XB [PPU.FixedColourGreen],
				IPPU.XB [PPU.FixedColourBlue]);

			// Clear the z-buffer, marking areas 'covered' by the fixed
			// colour as depth 1.
			pClip = &IPPU.Clip [1];

			// Clear the z-buffer
			if (pClip->Count [5])
			{
				// Colour window enabled.
				for (uint32 y = starty; y <= endy; y++)
				{
					ZeroMemory (GFX.SubZBuffer + y * GFX.ZPitch,
						IPPU.RenderedScreenWidth);
					ZeroMemory (GFX.ZBuffer + y * GFX.ZPitch,
						IPPU.RenderedScreenWidth);
					if (IPPU.Clip [0].Count [5])
					{
						uint32 *p = (uint32 *) (GFX.SubScreen + y * GFX.Pitch2);
						uint32 *q = (uint32 *) ((uint16 *) p + IPPU.RenderedScreenWidth);
						while (p < q)
							*p++ = black;
					}
					for (uint32 c = 0; c < pClip->Count [5]; c++)
					{
						if (pClip->Right [c][5] > pClip->Left [c][5])
						{
							memset (GFX.SubZBuffer + y * GFX.ZPitch + pClip->Left [c][5] * x2,
								1, (pClip->Right [c][5] - pClip->Left [c][5]) * x2);
							if (IPPU.Clip [0].Count [5])
							{
								// Blast, have to clear the sub-screen to the fixed-colour
								// because there is a colour window in effect clipping
								// the main screen that will allow the sub-screen
								// 'underneath' to show through.

								uint16 *p = (uint16 *) (GFX.SubScreen + y * GFX.Pitch2);
								uint16 *q = p + pClip->Right [c][5] * x2;
								p += pClip->Left [c][5] * x2;

								while (p < q)
									*p++ = (uint16) GFX.FixedColour;
							}
						}
					}
				}
			}
			else
			{
				for (uint32 y = starty; y <= endy; y++)
				{
					ZeroMemory (GFX.ZBuffer + y * GFX.ZPitch,
						IPPU.RenderedScreenWidth);
					memset (GFX.SubZBuffer + y * GFX.ZPitch, 1,
						IPPU.RenderedScreenWidth);

					if (IPPU.Clip [0].Count [5])
					{
						// Blast, have to clear the sub-screen to the fixed-colour
						// because there is a colour window in effect clipping
						// the main screen that will allow the sub-screen
						// 'underneath' to show through.

						uint32 b = GFX.FixedColour | (GFX.FixedColour << 16);
						uint32 *p = (uint32 *) (GFX.SubScreen + y * GFX.Pitch2);
						uint32 *q = (uint32 *) ((uint16 *) p + IPPU.RenderedScreenWidth);

						while (p < q)
							*p++ = b;
					}
				}
			}
			if (ANYTHING_ON_SUB)
			{
				GFX.DB = GFX.SubZBuffer;
				RenderScreen (GFX.SubScreen, TRUE, TRUE, SUB_SCREEN_DEPTH);
			}

			if (IPPU.Clip [0].Count [5])
			{
				for (uint32 y = starty; y <= endy; y++)
				{
					register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2);
					register uint8 *d = GFX.SubZBuffer + y * GFX.ZPitch;
					register uint8 *e = d + IPPU.RenderedScreenWidth;

					while (d < e)
					{
						if (*d > 1)
							*p = *(p + GFX.Delta);
						else
							*p = BLACK;
						d++;
						p++;
					}
				}
			}

			GFX.DB = GFX.ZBuffer;
			RenderScreen (GFX.Screen, FALSE, FALSE, MAIN_SCREEN_DEPTH);

			if (SUB_OR_ADD(5))
			{
				uint32 back = IPPU.ScreenColors [0];
				uint32 Left = 0;
				uint32 Right = 256;
				uint32 Count;

				pClip = &IPPU.Clip [0];
				for (uint32 y = starty; y <= endy; y++)
				{
					if (!(Count = pClip->Count [5]))
					{
						Left = 0;
						Right = 256 * x2;
						Count = 1;
					}

					for (uint32 b = 0; b < Count; b++)
					{
						if (pClip->Count [5])
						{
							Left = pClip->Left [b][5] * x2;
							Right = pClip->Right [b][5] * x2;
							if (Right <= Left)
								continue;
						}

						if (GFX.r2131 & 0x80)
						{
							if (GFX.r2131 & 0x40)
							{
								// Subtract, halving the result.
								register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + Left;
								register uint8 *d = GFX.ZBuffer + y * GFX.ZPitch;
								register uint8 *s = GFX.SubZBuffer + y * GFX.ZPitch + Left;
								register uint8 *e = d + Right;
								uint16 back_fixed = COLOR_SUB (back, GFX.FixedColour);

								d += Left;
								while (d < e)
								{
									if (*d == 0)
									{
										if (*s)
										{
											if (*s != 1)
												*p = COLOR_SUB1_2 (back, *(p + GFX.Delta));
											else
												*p = back_fixed;
										}
										else
											*p = (uint16) back;
									}
									d++;
									p++;
									s++;
								}
							}
							else
							{
								// Subtract
								register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + Left;
								register uint8 *s = GFX.SubZBuffer + y * GFX.ZPitch + Left;
								register uint8 *d = GFX.ZBuffer + y * GFX.ZPitch;
								register uint8 *e = d + Right;
								uint16 back_fixed = COLOR_SUB (back, GFX.FixedColour);

								d += Left;
								while (d < e)
								{
									if (*d == 0)
									{
										if (*s)
										{
											if (*s != 1)
												*p = COLOR_SUB (back, *(p + GFX.Delta));
											else
												*p = back_fixed;
										}
										else
											*p = (uint16) back;
									}
									d++;
									p++;
									s++;
								}
							}
						}
						else
							if (GFX.r2131 & 0x40)
							{
								register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + Left;
								register uint8 *d = GFX.ZBuffer + y * GFX.ZPitch;
								register uint8 *s = GFX.SubZBuffer + y * GFX.ZPitch + Left;
								register uint8 *e = d + Right;
								uint16 back_fixed = COLOR_ADD (back, GFX.FixedColour);
								d += Left;
								while (d < e)
								{
									if (*d == 0)
									{
										if (*s)
										{
											if (*s != 1)
												*p = COLOR_ADD1_2 (back, *(p + GFX.Delta));
											else
												*p = back_fixed;
										}
										else
											*p = (uint16) back;
									}
									d++;
									p++;
									s++;
								}
							}
							else
								if (back != 0)
								{
									register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + Left;
									register uint8 *d = GFX.ZBuffer + y * GFX.ZPitch;
									register uint8 *s = GFX.SubZBuffer + y * GFX.ZPitch + Left;
									register uint8 *e = d + Right;
									uint16 back_fixed = COLOR_ADD (back, GFX.FixedColour);
									d += Left;
									while (d < e)
									{
										if (*d == 0)
										{
											if (*s)
											{
												if (*s != 1)
													*p = COLOR_ADD (back, *(p + GFX.Delta));
												else
													*p = back_fixed;
											}
											else
												*p = (uint16) back;
										}
										d++;
										p++;
										s++;
									}
								}
								else
								{
									if (!pClip->Count [5])
									{
										// The backdrop has not been cleared yet - so
										// copy the sub-screen to the main screen
										// or fill it with the back-drop colour if the
										// sub-screen is clear.
										register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + Left;
										register uint8 *d = GFX.ZBuffer + y * GFX.ZPitch;
										register uint8 *s = GFX.SubZBuffer + y * GFX.ZPitch + Left;
										register uint8 *e = d + Right;
										d += Left;
										while (d < e)
										{
											if (*d == 0)
											{
												if (*s)
												{
													if (*s != 1)
														*p = *(p + GFX.Delta);
													else
														*p = GFX.FixedColour;
												}
												else
													*p = (uint16) back;
											}
											d++;
											p++;
											s++;
										}
									}
								}
			}
		}
		}
		else
		{
			// Subscreen not being added to back
			uint32 back = IPPU.ScreenColors [0] | (IPPU.ScreenColors [0] << 16);
			pClip = &IPPU.Clip [0];

			if (pClip->Count [5])
			{
				for (uint32 y = starty; y <= endy; y++)
				{
					for (uint32 b = 0; b < pClip->Count [5]; b++)
					{
						uint32 Left = pClip->Left [b][5] * x2;
						uint32 Right = pClip->Right [b][5] * x2;
						uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + Left;
						uint8 *d = GFX.ZBuffer + y * GFX.ZPitch;
						uint8 *e = d + Right;
						d += Left;

						while (d < e)
						{
							if (*d == 0)
								*p = (int16) back;
							d++;
							p++;
						}
					}
				}
			}
			else
			{
				for (uint32 y = starty; y <= endy; y++)
				{
					uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2);
					uint8 *d = GFX.ZBuffer + y * GFX.ZPitch;
					uint8 *e = d + 256 * x2;

					while (d < e)
					{
						if (*d == 0)
							*p = (int16) back;
						d++;
						p++;
					}
				}
			}
		}
	}
	else
	{
		// 16bit and transparency but currently no transparency effects in
		// operation.

		uint32 back = IPPU.ScreenColors [0] |
			(IPPU.ScreenColors [0] << 16);

		if (PPU.ForcedBlanking)
			back = black;

		if (IPPU.Clip [0].Count[5])
		{
			for (uint32 y = starty; y <= endy; y++)
			{
				uint32 *p = (uint32 *) (GFX.Screen + y * GFX.Pitch2);
				uint32 *q = (uint32 *) ((uint16 *) p + IPPU.RenderedScreenWidth);

				while (p < q)
					*p++ = black;

				for (uint32 c = 0; c < IPPU.Clip [0].Count [5]; c++)
				{
					if (IPPU.Clip [0].Right [c][5] > IPPU.Clip [0].Left [c][5])
					{
						uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2);
						uint16 *q = p + IPPU.Clip [0].Right [c][5] * x2;
						p += IPPU.Clip [0].Left [c][5] * x2;

						while (p < q)
							*p++ = (uint16) back;
					}
				}
			}
		}
		else
		{
			for (uint32 y = starty; y <= endy; y++)
			{
				uint32 *p = (uint32 *) (GFX.Screen + y * GFX.Pitch2);
				uint32 *q = (uint32 *) ((uint16 *) p + IPPU.RenderedScreenWidth);
				while (p < q)
					*p++ = back;
			}
		}
		if (!PPU.ForcedBlanking)
		{
			for (uint32 y = starty; y <= endy; y++)
			{
				ZeroMemory (GFX.ZBuffer + y * GFX.ZPitch,
					IPPU.RenderedScreenWidth);
			}
			GFX.DB = GFX.ZBuffer;
			RenderScreen (GFX.Screen, FALSE, TRUE, SUB_SCREEN_DEPTH);
		}
	}
    }
    else
    {
		if (Settings.SixteenBit)
		{
			uint32 back = IPPU.ScreenColors [0] | (IPPU.ScreenColors [0] << 16);
			if (PPU.ForcedBlanking)
				back = black;
			else
				SelectTileRenderer (TRUE);

			for (uint32 y = starty; y <= endy; y++)
			{
				uint32 *p = (uint32 *) (GFX.Screen + y * GFX.Pitch2);
				uint32 *q = (uint32 *) ((uint16 *) p + IPPU.RenderedScreenWidth);
				while (p < q)
					*p++ = back;
			}
		}
		else
		{
			for (uint32 y = starty; y <= endy; y++)
			{
				ZeroMemory (GFX.Screen + y * GFX.Pitch2,
					IPPU.RenderedScreenWidth);
			}
		}
		if (!PPU.ForcedBlanking)
		{
			for (uint32 y = starty; y <= endy; y++)
			{
				ZeroMemory (GFX.ZBuffer + y * GFX.ZPitch,
					IPPU.RenderedScreenWidth);
			}
			GFX.DB = GFX.ZBuffer;
			GFX.pCurrentClip = &IPPU.Clip [0];

#define FIXCLIP(n)\
	if (GFX.r212c & (1 << (n))) \
    GFX.pCurrentClip = &IPPU.Clip [0]; \
	else \
			GFX.pCurrentClip = &IPPU.Clip [1]


#define DISPLAY(n)\
    (!(PPU.BG_Forced & n) && \
	(GFX.r212c & n) || \
			((GFX.r212d & n) && subadd))

			uint8 subadd = GFX.r2131 & 0x3f;

			bool8 BG0 = DISPLAY(1);
			bool8 BG1 = DISPLAY(2);
			bool8 BG2 = DISPLAY(4);
			bool8 BG3 = DISPLAY(8);
			bool8 OB  = DISPLAY(16);

			if (PPU.BGMode <= 1)
			{
				if (OB)
				{
					FIXCLIP(4);
					DrawOBJS ();
				}
				if (BG0)
				{
					FIXCLIP(0);
					DrawBackground (PPU.BGMode, 0, 10, 14);
				}
				if (BG1)
				{
					FIXCLIP(1);
					DrawBackground (PPU.BGMode, 1, 9, 13);
				}
				if (BG2)
				{
					FIXCLIP(2);
					DrawBackground (PPU.BGMode, 2, 3,
						PPU.BG3Priority ? 17 : 6);
				}
				if (BG3 && PPU.BGMode == 0)
				{
					FIXCLIP(3);
					DrawBackground (PPU.BGMode, 3, 2, 5);
				}
			}
			else if (PPU.BGMode != 7)
			{
				if (OB)
				{
					FIXCLIP(4);
					DrawOBJS ();
				}
				if (BG0)
				{
					FIXCLIP(0);
					DrawBackground (PPU.BGMode, 0, 5, 13);
				}
				if (BG1 && PPU.BGMode != 6)
				{
					FIXCLIP(1);
					DrawBackground (PPU.BGMode, 1, 2, 9);
				}
			}
			else
			{
				if (OB)
				{
					FIXCLIP(4);
					DrawOBJS ();
				}
				if (BG0 || ((Memory.FillRAM [0x2133] & 0x40) && BG1))
				{
					int bg;
					FIXCLIP(0);
					if ((Memory.FillRAM [0x2133] & 0x40) && BG1)
					{
						GFX.Mode7Mask = 0x7f;
						GFX.Mode7PriorityMask = 0x80;
						Mode7Depths [0] = (BG0?5:1);
						Mode7Depths [1] = 9;
						bg = 1;
					}
					else
					{
						GFX.Mode7Mask = 0xff;
						GFX.Mode7PriorityMask = 0;
						Mode7Depths [0] = 5;
						Mode7Depths [1] = 5;
						bg = 0;
					}

					if (!Settings.SixteenBit)
						DrawBGMode7Background (GFX.Screen, bg);
					else
					{
						if (!Settings.Mode7Interpolate)
							DrawBGMode7Background16 (GFX.Screen, bg);
						else
							DrawBGMode7Background16_i (GFX.Screen, bg);
					}
				}
			}
	}
    }
    if (Settings.SupportHiRes)
    {
		if (PPU.BGMode != 5 && PPU.BGMode != 6 && IPPU.DoubleWidthPixels)
		{
			// Mixure of background modes used on screen - scale width
			// of all non-mode 5 and 6 pixels.
			if (Settings.SixteenBit)
			{
				for (register uint32 y = starty; y <= endy; y++)
				{
					register uint16 *p = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + 255;
					register uint16 *q = (uint16 *) (GFX.Screen + y * GFX.Pitch2) + 510;
					for (register int x = 255; x >= 0; x--, p--, q -= 2)
						*q = *(q + 1) = *p;
				}
			}
			else
			{
				for (register uint32 y = starty; y <= endy; y++)
				{
					register uint8 *p = GFX.Screen + y * GFX.Pitch2 + 255;
					register uint8 *q = GFX.Screen + y * GFX.Pitch2 + 510;
					for (register int x = 255; x >= 0; x--, p--, q -= 2)
						*q = *(q + 1) = *p;
				}
			}
		}

        // Double the height of the pixels just drawn
		FIX_INTERLACE(GFX.Screen, FALSE, GFX.ZBuffer);

    }
    IPPU.PreviousLine = IPPU.CurrentLine;
}

/*void output_png()
{
	int i, j;
	uint16 screen=(uint16*)GFX.Screen;
	uint8** png_buffer;//[448][1536];
	png_buffer=new uint8*[IPPU.RenderedScreenHeight];
	for(i=0; i<IPPU.RenderedScreenHeight;i++)
	{
		png_buffer[i]=new uint8[3*IPPU.RenderedScreenWidth];
	}

	for(i=0;i<IPPU.RenderedScreenWidth;i++)
	{
		for(j=0;j<IPPU.RenderedScreenHeight;j++)
		{
			png_buffer[i][j*3]=(screen[i*GFX.RealPitch+j]&0x001f)<<3;
			png_buffer[i][j*3+1]=(screen[i*GFX.RealPitch+j]&0x03E0)>>2;
			png_buffer[i][j*3+2]=(screen[i*GFX.RealPitch+j]&0x7C00)>>7;
		}
	}

	//make png

	for(i=0; i<IPPU.RenderedScreenHeight;i++)
	{
		delete [] png_buffer[i];
	}
	delete [] png_buffer;

}
*/
#ifdef GFX_MULTI_FORMAT

#define _BUILD_PIXEL(F) \
uint32 BuildPixel##F(uint32 R, uint32 G, uint32 B) \
{ \
    return (BUILD_PIXEL_##F(R,G,B)); \
}\
uint32 BuildPixel2##F(uint32 R, uint32 G, uint32 B) \
{ \
    return (BUILD_PIXEL2_##F(R,G,B)); \
} \
void DecomposePixel##F(uint32 pixel, uint32 &R, uint32 &G, uint32 &B) \
{ \
    DECOMPOSE_PIXEL_##F(pixel,R,G,B); \
}

_BUILD_PIXEL(RGB565)
_BUILD_PIXEL(RGB555)
_BUILD_PIXEL(BGR565)
_BUILD_PIXEL(BGR555)
_BUILD_PIXEL(GBR565)
_BUILD_PIXEL(GBR555)
_BUILD_PIXEL(RGB5551)

bool8 S9xSetRenderPixelFormat (int format)
{
    extern uint32 current_graphic_format;

    current_graphic_format = format;

    switch (format)
    {
    case RGB565:
	_BUILD_SETUP(RGB565)
	return (TRUE);
    case RGB555:
	_BUILD_SETUP(RGB555)
	return (TRUE);
    case BGR565:
	_BUILD_SETUP(BGR565)
	return (TRUE);
    case BGR555:
	_BUILD_SETUP(BGR555)
	return (TRUE);
    case GBR565:
	_BUILD_SETUP(GBR565)
	return (TRUE);
    case GBR555:
	_BUILD_SETUP(GBR555)
	return (TRUE);
    case RGB5551:
        _BUILD_SETUP(RGB5551)
        return (TRUE);
    default:
	break;
    }
    return (FALSE);
}
#endif
