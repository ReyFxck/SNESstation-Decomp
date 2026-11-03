/*******************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
 
  (c) Copyright 1996 - 2002 Gary Henderson (gary.henderson@ntlworld.com) and
                            Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2001 - 2004 John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2004 Brad Jorsch (anomie@users.sourceforge.net),
                            funkyass (funkyass@spam.shaw.ca),
                            Joel Yliluoma (http://iki.fi/bisqwit/)
                            Kris Bleakley (codeviolation@hotmail.com),
                            Matthew Kendora,
                            Nach (n-a-c-h@users.sourceforge.net),
                            Peter Bortas (peter@bortas.org) and
                            zones (kasumitokoduck@yahoo.com)

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003 zsKnight (zsknight@zsnes.com),
                            _Demo_ (_demo_@zsnes.com), and Nach

  C4 C++ code
  (c) Copyright 2003 Brad Jorsch

  DSP-1 emulator code
  (c) Copyright 1998 - 2004 Ivar (ivar@snes9x.com), _Demo_, Gary Henderson,
                            John Weidman, neviksti (neviksti@hotmail.com),
                            Kris Bleakley, Andreas Naive

  DSP-2 emulator code
  (c) Copyright 2003 Kris Bleakley, John Weidman, neviksti, Matthew Kendora, and
                     Lord Nightmare (lord_nightmare@users.sourceforge.net

  OBC1 emulator code
  (c) Copyright 2001 - 2004 zsKnight, pagefault (pagefault@zsnes.com) and
                            Kris Bleakley
  Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002 Matthew Kendora with research by
                     zsKnight, John Weidman, and Dark Force

  S-DD1 C emulator code
  (c) Copyright 2003 Brad Jorsch with research by
                     Andreas Naive and John Weidman
 
  S-RTC C emulator code
  (c) Copyright 2001 John Weidman
  
  ST010 C++ emulator code
  (c) Copyright 2003 Feather, Kris Bleakley, John Weidman and Matthew Kendora

  Super FX x86 assembler emulator code 
  (c) Copyright 1998 - 2003 zsKnight, _Demo_, and pagefault 

  Super FX C emulator code 
  (c) Copyright 1997 - 1999 Ivar, Gary Henderson and John Weidman


  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004 Marcus Comstedt (marcus@mc.pp.se) 

 
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
/* Render.CPP
 * ----------
 * Using the idea from MAME32, use several different render routines to solve 
 * the different screen depth problems, also getting rid of the dreaded tripple 
 * buffer idea when possible
 */

#include "snes9x\port.h"
#include "wsnes9x.h"
#include "snes9x\snes9x.h"
#include <memory.h>
#include "Render.h"
#include "DirectX.h"
#include "snes9x/ppu.h"
#if defined(USE_GLIDE) || defined(USE_OPENGL)
#include "snes9x\3d.h"
#include "snes9x\gfx.h"
#include <math.h>
#include "auxmath.h"
#endif

// Private Prototypes, should not be called directly
void RenderNothing (SSurface Src, SSurface Dst, RECT *);
void Render8to8 (SSurface Src, SSurface Dst, RECT *);
void RenderFakeTV8to8 (SSurface Src, SSurface Dst, RECT *);
void Render16to16 (SSurface Src, SSurface Dst, RECT *);
void RenderFakeTV16to16 (SSurface Src, SSurface Dst, RECT *);
void RenderSuperEagle16to16 (SSurface Src, SSurface Dst, RECT *);
void Render2xSaI16to16 (SSurface Src, SSurface Dst, RECT *);
void RenderSuper2xSaI16to16 (SSurface Src, SSurface Dst, RECT *);
void RenderTVMode (SSurface Src, SSurface Dst, RECT *);
#ifdef USE_GLIDE
void RenderGlide (SSurface Src, SSurface Dst, RECT *);
#endif
#ifdef USE_OPENGL
void RenderOpenGL (SSurface Src, SSurface Dst, RECT *);
#endif
// Contains the pointer to the now active render method
TRenderMethod RenderMethod;

// Used as change log
static uint8 ChangeLog1 [EXT_PITCH * MAX_SNES_HEIGHT];
static uint8 ChangeLog2 [EXT_PITCH * MAX_SNES_HEIGHT];
static uint8 ChangeLog3 [EXT_PITCH * MAX_SNES_HEIGHT];

uint8 *ChangeLog [3] = {
    ChangeLog1, ChangeLog2, ChangeLog3
};

START_EXTERN_C
uint8 snes9x_clear_change_log = 0;
END_EXTERN_C

void SelectRenderMethod()
{
    TRenderMethod OldRenderMethod = RenderMethod;
    RenderMethod = RenderNothing;
    
    if (!Settings.SixteenBit)
    {
	switch (GUI.Scale)
	{
            case 0:
            case 1:
                RenderMethod = Render8to8;
                break;
            case 2:
                RenderMethod = RenderFakeTV8to8;
                break;
            default:
                RenderMethod = Render8to8;
                break;
	}
    }
    else
    {
	switch (GUI.Scale)
	{
            case 0: 
            case 1: 
                RenderMethod = Render16to16;
                break;
            case 2: 
                RenderMethod = RenderFakeTV16to16;
                break;
            case 3: 
                RenderMethod = RenderSuperEagle16to16;
                break;
            case 4:
                RenderMethod = Render2xSaI16to16;
                break;
            case 5:
                RenderMethod = RenderSuper2xSaI16to16;
                break;
            case 6:
                RenderMethod = RenderTVMode;
                break;
            case 7:
#ifdef USE_GLIDE
                RenderMethod = RenderGlide;
#else
#ifdef USE_OPENGL
				RenderMethod = RenderOpenGL;
#else
				RenderMethod = RenderTVMode;
#endif
#endif
                break;
            case 8:
#ifdef USE_OPENGL
                RenderMethod = RenderOpenGL;
#else
				RenderMethod = RenderTVMode;
#endif
                break;
            case 9:
#ifdef USE_OPENGL
                RenderMethod = RenderOpenGL;
#else
				RenderMethod = RenderTVMode;
#endif                break;
            default:
                RenderMethod = Render16to16;
                break;
	}
    }
    if (GUI.ScreenCleared || OldRenderMethod != RenderMethod)
        snes9x_clear_change_log = GUI.NumFlipFrames;

    GUI.ScreenCleared = false;
}

void RenderNothing( SSurface Src, SSurface Dst, RECT *)
{ // Just an empty procedure, so that it never happens that something 'bad' is rendered =)
}

// Needed by RenderDouble8to8, stretches a single line
inline void DoubleLine8( unsigned __int8 *lpDst, unsigned __int8 *lpSrc, unsigned int Width)
{
    for( unsigned int W = 0; W != Width; W ++)
    {
        lpDst[0] = lpSrc[0];
        lpDst[1] = lpSrc[0];
        
        lpDst += 2;
        lpSrc ++;
    }
}

void Render8to8 (SSurface Src, SSurface Dst, RECT *rect)
{
    unsigned char *lpSrc, *lpDst;

    lpSrc = Src.Surface;

    rect->left = (Dst.Width - Src.Width) >> 1;
    rect->right = rect->left + Src.Width;
    rect->top = (Dst.Height - Src.Height) >> 1;
    rect->bottom = rect->top + Src.Height;

    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left;
	
    for (unsigned int H = 0; H != Src.Height; H ++, lpDst += Dst.Pitch, lpSrc += Src.Pitch)
        memcpy( lpDst, lpSrc, Src.Width);
}

void RenderFakeTV8to8 (SSurface Src, SSurface Dst, RECT *rect)
{
    unsigned char *lpSrc, *lpDst;
    unsigned int H;

    // If Snes9x is rendering anything in HiRes, then just copy, don't stretch
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {	
        Render8to8 (Src, Dst, rect);
        return;
    }

    rect->left = (Dst.Width - 512) >> 1;
    rect->right = rect->left + 512;
    rect->top = (Dst.Height - Src.Height * 2) >> 1;
    rect->bottom = rect->top + Src.Height * 2;

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left;

    for (H = 0; H != Src.Height; H++)
    {
        DoubleLine8 ((unsigned __int8 *)lpDst, (unsigned __int8 *)lpSrc, Src.Width);
        lpSrc += Src.Pitch;
        lpDst += Dst.Pitch;
        
        memset (lpDst, 0, 512);
        lpDst += Dst.Pitch;
    }
}

// Needed by Render2xSaI16to16
void SuperEagle(uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch, int width, int height);
void _2xSaI(uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch, int width, int height);
void Super2xSaI(uint8 *srcPtr, uint32 srcPitch, uint8 *deltaPtr, uint8 *dstPtr, uint32 dstPitch, int width, int height);
bool8 mmx_checked = false;
bool8 mmx_enabled = true;
extern bool8 cpu_mmx;

// Needed by RenderDouble16to16, stretches a single line
inline void DoubleLine16( unsigned __int16 *lpDst, unsigned __int16 *lpSrc, unsigned int Width)
{
    for (unsigned int W = 0; W != Width; W ++)
    {
        lpDst[0] = lpSrc[0];
        lpDst[1] = lpSrc[0];

        lpDst += 2;
        lpSrc ++;
    }
}

// No enlargement, just render to the screen
void Render16to16 (SSurface Src, SSurface Dst, RECT *rect)
{
    unsigned char *lpSrc, *lpDst;

    rect->left = (Dst.Width - Src.Width) >> 1;
    rect->right = rect->left + Src.Width;
    rect->top = (Dst.Height - Src.Height) >> 1;
    rect->bottom = rect->top + Src.Height;

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left * 2;
	
    for (unsigned int H = 0; H != Src.Height; H++, lpDst += Dst.Pitch, lpSrc += Src.Pitch)
        memcpy (lpDst, lpSrc, Src.Width << 1);
}

// Enlarge 2x using Fake TV mode when the Snes9x is not rendering in HiRes
// FakeTV mode = a black line between each other line
void RenderFakeTV16to16( SSurface Src, SSurface Dst, RECT *rect)
{
    unsigned char *lpSrc, *lpDst;
    unsigned int H;

    // If Snes9x is rendering anything in HiRes, then just copy, don't stretch
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {	
        Render16to16 (Src, Dst, rect);
        return;
    }

    rect->left = (Dst.Width - Src.Width * 2) >> 1;
    rect->right = rect->left + Src.Width * 2;
    rect->top = (Dst.Height - Src.Height * 2) >> 1;
    rect->bottom = rect->top + Src.Height * 2;

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left * 2;

    for (H = 0; H < Src.Height; H++)
    {
        DoubleLine16 ((unsigned __int16 *)lpDst, (unsigned __int16 *)lpSrc, 
                      Src.Width);
        lpSrc += Src.Pitch;
        lpDst += Dst.Pitch;

        memset (lpDst, 0, 512*2);
        lpDst += Dst.Pitch;
    }
}

// As the name suggests, I (Jerremy) took this procedure from the MAME32, I hope that
// Michael Soderstrom and Chris Kirmse don't mind this.
static bool MAME32_DetectMMX()
{
#if defined(_M_IX86)

    bool result = false;
    int cpu_properties;


#define ASM_CPUID __asm _emit 0xf __asm _emit 0xa2

    __try
    {
        __asm
        {
            mov eax, 1
            ASM_CPUID
            mov cpu_properties, edx
        }
        /* if bit 23 is set, MMX is available */
        if (cpu_properties & 0x800000)
            result = true;
    }
    __except(1)
    {
        return false;
    }

    return result;

#else
    return false;
#endif
}

void RenderSuperEagle16to16 (SSurface Src, SSurface Dst, RECT *rect)
{
    unsigned char *lpSrc, *lpDst;

    // If Snes9x is rendering anything in HiRes, then just copy, don't interpolate
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {	
        RenderTVMode (Src, Dst, rect);
        return;
    }

    if (!mmx_checked)
    {
        mmx_checked = true;
        cpu_mmx = false;

        if (mmx_enabled)
            cpu_mmx = MAME32_DetectMMX();
    }

    rect->left = (Dst.Width - Src.Width * 2) >> 1;
    rect->right = rect->left + Src.Width * 2;
    rect->top = (Dst.Height - Src.Height * 2) >> 1;
    rect->bottom = rect->top + Src.Height * 2;

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left * 2;

    SuperEagle (lpSrc, Src.Pitch,
                ChangeLog [GUI.FlipCounter % GUI.NumFlipFrames],
                lpDst, Dst.Pitch, Src.Width, Src.Height);

    if (snes9x_clear_change_log)
        snes9x_clear_change_log--;
}

void Render2xSaI16to16 (SSurface Src, SSurface Dst, RECT *rect)
{
    unsigned char *lpSrc, *lpDst;

    // If Snes9x is rendering anything in HiRes, then just copy, don't interpolate
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {	
        RenderTVMode (Src, Dst, rect);
        return;
    }

    if (!mmx_checked)
    {
        mmx_checked = true;
        cpu_mmx = false;

        if( mmx_enabled)
            cpu_mmx = MAME32_DetectMMX();
    }

    rect->left = (Dst.Width - Src.Width * 2) >> 1;
    rect->right = rect->left + Src.Width * 2;
    rect->top = (Dst.Height - Src.Height * 2) >> 1;
    rect->bottom = rect->top + Src.Height * 2;

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left * 2;

    _2xSaI (lpSrc, Src.Pitch, ChangeLog [GUI.FlipCounter % GUI.NumFlipFrames],
            lpDst, Dst.Pitch, Src.Width, Src.Height);

    if (snes9x_clear_change_log)
        snes9x_clear_change_log--;
}

void RenderSuper2xSaI16to16 (SSurface Src, SSurface Dst, RECT *rect)
{
    unsigned char *lpSrc, *lpDst;

    // If Snes9x is rendering anything in HiRes, then just copy, don't interpolate
    if (Src.Height > SNES_HEIGHT_EXTENDED || Src.Width == 512)
    {	
        RenderTVMode (Src, Dst, rect);
        return;
    }

    if (!mmx_checked)
    {
        mmx_checked = true;
        cpu_mmx = false;

        if( mmx_enabled)
            cpu_mmx = MAME32_DetectMMX();
    }

    rect->left = (Dst.Width - Src.Width * 2) >> 1;
    rect->right = rect->left + Src.Width * 2;
    rect->top = (Dst.Height - Src.Height * 2) >> 1;
    rect->bottom = rect->top + Src.Height * 2;

    lpSrc = Src.Surface;
    lpDst = Dst.Surface;
    lpDst += rect->top * Dst.Pitch + rect->left * 2;

    Super2xSaI (lpSrc, Src.Pitch,
                ChangeLog [GUI.FlipCounter % GUI.NumFlipFrames], 
                lpDst, Dst.Pitch, Src.Width, Src.Height);

    if (snes9x_clear_change_log)
        snes9x_clear_change_log--;
}

void RenderTVMode ( SSurface Src, SSurface Dst, RECT *rect)
{
    if (Src.Height > SNES_HEIGHT_EXTENDED)
    {
        snes9x_clear_change_log = GUI.NumFlipFrames;
        Render16to16 (Src, Dst, rect);
        return;
    }

    uint8 *nextLine, *finish;
    uint32 colorMask = ~(RGB_LOW_BITS_MASK | (RGB_LOW_BITS_MASK << 16));
    uint32 lowPixelMask = RGB_LOW_BITS_MASK;
    uint8 *srcPtr = Src.Surface;
    uint8 *dstPtr = Dst.Surface;
    uint32 srcPitch = Src.Pitch;
    uint32 dstPitch = Dst.Pitch;
    int width = Src.Width;
    int height = Src.Height;
    uint8 *deltaPtr = ChangeLog [GUI.FlipCounter % GUI.NumFlipFrames];

    rect->left = (Dst.Width - 512) >> 1;
    rect->right = rect->left + 512;
    rect->top = (Dst.Height - height * 2) >> 1;
    rect->bottom = rect->top + height * 2;

    dstPtr += rect->top * Dst.Pitch + rect->left * 2;
    nextLine = dstPtr + dstPitch;

    if (width == 256)
    {
	do
	{
	    uint32 *bP = (uint32 *) srcPtr;
	    uint32 *xP = (uint32 *) deltaPtr;
	    uint32 *dP = (uint32 *) dstPtr;
	    uint32 *nL = (uint32 *) nextLine;
	    uint32 currentPixel;
	    uint32 nextPixel;
	    uint32 currentDelta;
	    uint32 nextDelta;

	    finish = (uint8 *) bP + ((width + 2) << 1);
	    nextPixel = *bP++;
	    nextDelta = *xP++;

	    do
	    {
		currentPixel = nextPixel;
		currentDelta = nextDelta;
		nextPixel = *bP++;
		nextDelta = *xP++;

		if (snes9x_clear_change_log ||
                    nextPixel != nextDelta || currentPixel != currentDelta)
		{
		    uint32 colorA, colorB, product, darkened;

		    *(xP - 2) = currentPixel;
#ifdef LSB_FIRST
		    colorA = currentPixel & 0xffff;
#else
		    colorA = (currentPixel & 0xffff0000) >> 16;
#endif

#ifdef LSB_FIRST
		    colorB = (currentPixel & 0xffff0000) >> 16;
		    *(dP) = product = colorA |
                        ((((colorA & colorMask) >> 1) +
                          ((colorB & colorMask) >> 1) +
                          (colorA & colorB & lowPixelMask)) << 16);
#else
		    colorB = currentPixel & 0xffff;
		    *(dP) = product = (colorA << 16) | 
                        (((colorA & colorMask) >> 1) +
                         ((colorB & colorMask) >> 1) +
                         (colorA & colorB & lowPixelMask));
#endif
		    darkened = (product = ((product & colorMask) >> 1));
		    darkened += (product = ((product & colorMask) >> 1));
		    darkened += (product & colorMask) >> 1;
		    *(nL) = darkened;

#ifdef LSB_FIRST
		    colorA = nextPixel & 0xffff;
		    *(dP + 1) = product = colorB |
                        ((((colorA & colorMask) >> 1) +
                          ((colorB & colorMask) >> 1) +
                          (colorA & colorB & lowPixelMask)) << 16);
#else
		    colorA = (nextPixel & 0xffff0000) >> 16;
		    *(dP + 1) = product = (colorB << 16) | 
                        (((colorA & colorMask) >> 1) +
                         ((colorB & colorMask) >> 1) + 
                         (colorA & colorB & lowPixelMask));
#endif
		    darkened = (product = ((product & colorMask) >> 1));
		    darkened += (product = ((product & colorMask) >> 1));
		    darkened += (product & colorMask) >> 1;
		    *(nL + 1) = darkened;
		}

		dP += 2;
		nL += 2;
	    } while ((uint8 *) bP < finish);

	    deltaPtr += srcPitch;
	    srcPtr += srcPitch;
	    dstPtr += dstPitch * 2;
	    nextLine += dstPitch * 2;
	}
	while (--height);

        if (snes9x_clear_change_log)
            snes9x_clear_change_log--;
    }
    else
    {
        snes9x_clear_change_log = GUI.NumFlipFrames;

	do
	{
	    uint32 *bP = (uint32 *) srcPtr;
	    uint32 *dP = (uint32 *) dstPtr;
	    uint32 currentPixel;

	    finish = (uint8 *) bP + (width << 1);

	    do
	    {
		currentPixel = *bP++;
                uint32 product, darkened;

                *dP = currentPixel;
                darkened = (product = ((currentPixel & colorMask) >> 1));
                darkened += (product = ((product & colorMask) >> 1));
                darkened += (product & colorMask) >> 1;
                *(uint32 *) ((uint8 *) dP + dstPitch) = darkened;
		dP++;
	    } while ((uint8 *) bP < finish);

	    srcPtr += srcPitch;
	    dstPtr += dstPitch * 2;
	}
	while (--height);
    }
}

#ifdef USE_GLIDE
GlideData Glide;

#define TEXTURE_SIZE 256

static uint16 *texture_download_buffer = NULL;
static GrTexInfo texture_info;
static int texture_mem_start [4];

static void S9xGlideCalcSquare (int snes_width, int snes_height, 
				int x, int y, int width, int height,
				GrVertex *square)
{
    float snes_aspect_ratio = snes_width / (float) snes_height;
    float voodoo_aspect_ratio = Glide.voodoo_width / (float) Glide.voodoo_height;
    float virtual_height;
    float virtual_width;
    float virtual_x;
    float virtual_y;

#if 0
    if (snes_aspect_ratio < voodoo_aspect_ratio)
    {
	virtual_height = (float) Glide.voodoo_height;
	virtual_width = virtual_height * snes_aspect_ratio;
	virtual_x = ((float) Glide.voodoo_width - virtual_width) / 2.0;
	virtual_y = 0.0;
    }
    else
    {
	virtual_width = Glide.voodoo_width;
	virtual_height = virtual_width / snes_aspect_ratio;
	virtual_x = 0;
	virtual_y = (Glide.voodoo_height - virtual_height) / 2.0;
    }
#endif

    virtual_x = 0;
    virtual_y = 0;
    virtual_width = Glide.voodoo_width;
    virtual_height = Glide.voodoo_height;

    for (int i = 0; i < 4; i++)
    {
	square [i].oow = 1.0;
	square [i].ooz = 65535.0;
	square [i].tmuvtx [0].oow = 1.0;
    }

    square [0].tmuvtx [0].sow = 0.0;
    square [0].tmuvtx [0].tow = 0.0;
    square [1].tmuvtx [0].sow = (float) width;
    square [1].tmuvtx [0].tow = 0.0;
    square [2].tmuvtx [0].sow = (float) width;
    square [2].tmuvtx [0].tow = (float) height;
    square [3].tmuvtx [0].sow = 0.0;
    square [3].tmuvtx [0].tow = (float) height;
	
    float width_percent;
    float height_percent;
    float width_percent2;
    float height_percent2;

    if ((width_percent = virtual_width * TEXTURE_SIZE / snes_width) > virtual_width)
	width_percent = virtual_width;
    if ((height_percent = virtual_height * TEXTURE_SIZE / snes_height) > virtual_height)
	height_percent = virtual_height;

    width_percent2 = width_percent;
    height_percent2 = height_percent;
#if 1
    if (virtual_x + (x + 1) * width_percent2 > virtual_width)
        width_percent2 -= ((virtual_x + (x + 1) * width_percent2) - virtual_width) / (x + 1);
    if (virtual_y + (y + 1) * height_percent2 > virtual_height)
        height_percent2 -= ((virtual_y + (y + 1) * height_percent2) - virtual_height) / (y + 1);
#endif
    square [0].x = virtual_x + x * width_percent;
    square [0].y = virtual_y + y * height_percent;
    square [1].x = virtual_x + (x + 1) * width_percent2;
    square [1].y = virtual_y + y * height_percent;
    square [2].x = virtual_x + (x + 1) * width_percent2;
    square [2].y = virtual_y + (y + 1) * height_percent2;
    square [3].x = virtual_x + x * width_percent;
    square [3].y = virtual_y + (y + 1) * height_percent2;
}
typedef void (FX_CALL *tpColorCombine) (
               GrCombineFunction_t function, GrCombineFactor_t factor,
               GrCombineLocal_t local, GrCombineOther_t other,
               FxBool invert );
static tpColorCombine pgrColorCombine = NULL;

typedef void (FX_CALL *tpTexCombine) (
             GrChipID_t tmu,
             GrCombineFunction_t rgb_function,
             GrCombineFactor_t rgb_factor, 
             GrCombineFunction_t alpha_function,
             GrCombineFactor_t alpha_factor,
             FxBool rgb_invert,
             FxBool alpha_invert
             );
static tpTexCombine pgrTexCombine = NULL;

typedef void (FX_CALL *tpTexMipMapMode) (
                 GrChipID_t     tmu, 
                 GrMipMapMode_t mode,
                 FxBool         lodBlend );
static tpTexMipMapMode pgrTexMipMapMode = NULL;

typedef void (FX_CALL *tpTexClampMode) (
               GrChipID_t tmu,
               GrTextureClampMode_t s_clampmode,
               GrTextureClampMode_t t_clampmode
               );
static tpTexClampMode pgrTexClampMode = NULL;

typedef void (FX_CALL *tpTexFilterMode) (
                GrChipID_t tmu,
                GrTextureFilterMode_t minfilter_mode,
                GrTextureFilterMode_t magfilter_mode
                );
static tpTexFilterMode pgrTexFilterMode = NULL;

typedef FxU32 (FX_CALL *tpTexTextureMemRequired) (
                FxU32     evenOdd,
                GrTexInfo *info   );

static tpTexTextureMemRequired pgrTexTextureMemRequired = NULL;

typedef FxU32 (FX_CALL *tpTexMinAddress) (
                GrChipID_t);
static tpTexMinAddress pgrTexMinAddress = NULL;

typedef void (FX_CALL *tpGlideInit) ();

static tpGlideInit pgrGlideInit = NULL;

typedef FxBool (FX_CALL *tpQueryHardware) (
                 GrHwConfiguration *hwconfig);
static tpQueryHardware pgrSstQueryHardware = NULL;

typedef void (FX_CALL *tpSelect) (int);
static tpSelect pgrSstSelect = NULL;

typedef FxBool (FX_CALL *tpWinOpen) (
          FxU32                hWnd,
          GrScreenResolution_t screen_resolution,
          GrScreenRefresh_t    refresh_rate,
          GrColorFormat_t      color_format,
          GrOriginLocation_t   origin_location,
          int                  nColBuffers,
          int                  nAuxBuffers);
static tpWinOpen pgrSstWinOpen = NULL;

typedef void (FX_CALL *tpBufferClear) (
               GrColor_t color, GrAlpha_t alpha, FxU16 depth);
static tpBufferClear pgrBufferClear = NULL;

typedef FxU32 (FX_CALL *tpScreenWidth) ();
static tpScreenWidth pgrSstScreenWidth = NULL;

typedef FxU32 (FX_CALL *tpScreenHeight) ();
static tpScreenHeight  pgrSstScreenHeight = NULL;

typedef void (FX_CALL *tpGlideShutdown) ();
static tpGlideShutdown pgrGlideShutdown = NULL;

typedef void (FX_CALL *tpTexDownloadMipMapLevel) (
               GrChipID_t        tmu,
               FxU32             startAddress,
               GrLOD_t           thisLod,
               GrLOD_t           largeLod,
               GrAspectRatio_t   aspectRatio,
               GrTextureFormat_t format,
               FxU32             evenOdd,
               void              *data );

static tpTexDownloadMipMapLevel pgrTexDownloadMipMapLevel = NULL;

typedef void (FX_CALL *tpTexSource) (
               GrChipID_t tmu,
               FxU32      startAddress,
               FxU32      evenOdd,
               GrTexInfo  *info );
static tpTexSource pgrTexSource = NULL;

typedef void (FX_CALL *tpDrawTriangle) (
               const GrVertex *a, 
               const GrVertex *b, 
               const GrVertex *c );
static tpDrawTriangle pgrDrawTriangle = NULL;

typedef void (FX_CALL *tpBufferSwap) (
               int swap_interval);
static tpBufferSwap pgrBufferSwap = NULL;

static void S9xGlideInitTextures ()
{
    (*pgrColorCombine) (GR_COMBINE_FUNCTION_SCALE_OTHER,
                        GR_COMBINE_FACTOR_ONE,
                        GR_COMBINE_LOCAL_NONE,
                        GR_COMBINE_OTHER_TEXTURE,
                        FXFALSE);

    (*pgrTexCombine) (GR_TMU0,
                      GR_COMBINE_FUNCTION_LOCAL,
                      GR_COMBINE_FACTOR_NONE,
                      GR_COMBINE_FUNCTION_NONE,
                      GR_COMBINE_FACTOR_NONE,
                      FXFALSE, 
                      FXFALSE);

    (*pgrTexMipMapMode) (GR_TMU0,
                         GR_MIPMAP_DISABLE,
                         FXFALSE);

    (*pgrTexClampMode) (GR_TMU0,
                        GR_TEXTURECLAMP_CLAMP,
                        GR_TEXTURECLAMP_CLAMP);

    (*pgrTexFilterMode) (GR_TMU0, GR_TEXTUREFILTER_BILINEAR,
                         GR_TEXTUREFILTER_BILINEAR);

    texture_info.smallLod = GR_LOD_256;
    texture_info.largeLod = GR_LOD_256;
    texture_info.aspectRatio = GR_ASPECT_1x1;
    texture_info.format = GR_TEXFMT_RGB_565;

    Glide.texture.smallLod = GR_LOD_8;
    Glide.texture.largeLod = GR_LOD_8;
    Glide.texture.aspectRatio = GR_ASPECT_1x1;
    Glide.texture.format = GR_TEXFMT_RGB_565;
    Glide.texture_mem_size = (*pgrTexTextureMemRequired) (GR_MIPMAPLEVELMASK_BOTH,
						      &Glide.texture);
    Glide.texture_mem_start = (*pgrTexMinAddress) (GR_TMU0);

    int texture_mem_size = (*pgrTexTextureMemRequired) (GR_MIPMAPLEVELMASK_BOTH,
						    &texture_info);
    int address = (*pgrTexMinAddress) (GR_TMU0);

    // At maximum SNES resolution (512x478) four seperate Voodoo textures will
    // be needed since the maximum texture size on Voodoo cards is 256x256.
    
    for (int t = 0; t < 4; t++)
    {
	texture_mem_start [t] = address;
	address += texture_mem_size;
    }
    
    texture_download_buffer = (uint16 *) malloc (texture_mem_size);

    for (int i = 0; i < 4; i++)
    {
	Glide.sq [i].oow = 1.0;
	Glide.sq [i].ooz = 65535.0;
	Glide.sq [i].tmuvtx [0].oow = 1.0;
    }

    Glide.x_offset = 0.0;
    Glide.y_offset = 0.0;
    // XXX: Do this when the SNES screen resolution is known.
    Glide.x_scale = (float) (Glide.voodoo_width * 8.0 / 256);
    Glide.y_scale = (float) (Glide.voodoo_height * 8.0 / 224);
}

static HMODULE GlideDLL = NULL;

bool8 S9xVoodooInitialise ()
{
    static bool8 init = FALSE;

    if (!init)
    {
        init = TRUE;
        if (!(GlideDLL = LoadLibrary ("glide2x.dll")))
        {
            MessageBox (GUI.hWnd, "\
Snes9x can't find the \"glide2x.dll\" dynamic-link library.\n\n\
Are you sure you actually have a Voodoo 3dfx graphics card inside your PC?", 
                        "Snes9x - Missing Library(glide2x.dll) Error",
                        MB_OK | MB_ICONINFORMATION);
            return (FALSE);
        }

        pgrColorCombine = (tpColorCombine) GetProcAddress (GlideDLL, "_grColorCombine@20");
        pgrTexCombine = (tpTexCombine) GetProcAddress (GlideDLL, "_grTexCombine@28");
        pgrTexMipMapMode = (tpTexMipMapMode) GetProcAddress (GlideDLL, "_grTexMipMapMode@12");
        pgrTexClampMode = (tpTexClampMode) GetProcAddress (GlideDLL, "_grTexClampMode@12");
        pgrTexFilterMode = (tpTexFilterMode) GetProcAddress (GlideDLL, "_grTexFilterMode@12");
        pgrTexTextureMemRequired = (tpTexTextureMemRequired) GetProcAddress (GlideDLL, "_grTexTextureMemRequired@8");
        pgrTexMinAddress = (tpTexMinAddress) GetProcAddress (GlideDLL, "_grTexMinAddress@4");
        pgrGlideInit = (tpGlideInit) GetProcAddress (GlideDLL, "_grGlideInit@0");
        pgrSstQueryHardware = (tpQueryHardware) GetProcAddress (GlideDLL, "_grSstQueryHardware@4");
        pgrSstSelect = (tpSelect) GetProcAddress (GlideDLL, "_grSstSelect@4");
        pgrSstWinOpen = (tpWinOpen) GetProcAddress (GlideDLL, "_grSstWinOpen@28");
        pgrBufferClear = (tpBufferClear) GetProcAddress (GlideDLL, "_grBufferClear@12");
        pgrSstScreenWidth = (tpScreenWidth) GetProcAddress (GlideDLL, "_grSstScreenWidth@0");
        pgrSstScreenHeight = (tpScreenHeight) GetProcAddress (GlideDLL, "_grSstScreenHeight@0");
        pgrGlideShutdown = (tpGlideShutdown) GetProcAddress (GlideDLL, "_grGlideShutdown@0");
        pgrTexDownloadMipMapLevel = (tpTexDownloadMipMapLevel) GetProcAddress (GlideDLL, "_grTexDownloadMipMapLevel@32");
        pgrTexSource = (tpTexSource) GetProcAddress (GlideDLL, "_grTexSource@16");
        pgrDrawTriangle = (tpDrawTriangle) GetProcAddress (GlideDLL, "_grDrawTriangle@12");
        pgrBufferSwap = (tpBufferSwap) GetProcAddress (GlideDLL, "_grBufferSwap@4");
        //pgr = (tp) GetProcAddress (GlideDLL, "gr");

        if (!pgrColorCombine ||
            !pgrTexCombine ||
            !pgrTexMipMapMode ||
            !pgrTexClampMode ||
            !pgrTexFilterMode ||
            !pgrTexTextureMemRequired ||
            !pgrTexMinAddress ||
            !pgrGlideInit ||
            !pgrSstQueryHardware ||
            !pgrSstSelect ||
            !pgrSstWinOpen ||
            !pgrBufferClear ||
            !pgrSstScreenWidth ||
            !pgrSstScreenHeight ||
            !pgrGlideShutdown ||
            !pgrTexDownloadMipMapLevel ||
            !pgrTexSource ||
            !pgrDrawTriangle ||
            !pgrBufferSwap)
        {
            MessageBox (GUI.hWnd, "\
Snes9x found the \"glide2x.dll\" library it was looking for\n\
to enable 3dfx support, but it seems to be missing several\n\
important functions.\n\n\
3dfx support cannot be enabled, sorry.",
                        "Snes9x - Missing Symbols in Library (glide2x.dll) Error",
                        MB_OK | MB_ICONINFORMATION);
            return (FALSE);
        }
        (*pgrGlideInit) ();

        GrHwConfiguration hwconfig;
        Glide.voodoo_present = (*pgrSstQueryHardware) (&hwconfig);
        if (!Glide.voodoo_present)
        {
            MessageBox (GUI.hWnd, "\
Snes9x found the \"glide2x.dll\" library it was looking for\n\
to enable 3dfx support, but the library refused to initialise\n\
correctly.\n\n\
3dfx support cannot be enabled, sorry.",
                        "Snes9x - GLIDE Initialisation Error",
                        MB_OK | MB_ICONINFORMATION);
            return (FALSE);
        }
    }

    return (Glide.voodoo_present);
}

bool8 S9xGlideInit ()
{
    static GrHwConfiguration hwconfig;
    // XXX:
    GrScreenResolution_t resolution = GR_RESOLUTION_640x480;
    GrScreenRefresh_t refresh_rate = GR_REFRESH_75Hz;

    if ((*pgrSstQueryHardware) (&hwconfig))
    {
        FxU32 hwnd = (FxU32) GUI.hWnd;

        if (hwconfig.SSTs [0].type == GR_SSTTYPE_SST96 ||
            hwconfig.SSTs [0].type >GR_SSTTYPE_Voodoo2)
        {
            resolution = GR_RESOLUTION_NONE;
        }
	(*pgrSstSelect) (0);

	if ((*pgrSstWinOpen) (hwnd, resolution, refresh_rate, 
                              GR_COLORFORMAT_ABGR,
                              GR_ORIGIN_UPPER_LEFT, 2, 1))
	{
	    (*pgrBufferClear) (0, 0, GR_ZDEPTHVALUE_FARTHEST);
	    Glide.voodoo_width = (float) (*pgrSstScreenWidth) ();
	    Glide.voodoo_height = (float) (*pgrSstScreenHeight) ();
	    S9xGlideInitTextures ();
	    return (TRUE);
	}
    }
    
    Glide.voodoo_present = FALSE;
    return (FALSE);
}

void S9xGlideDeinit ()
{
    free ((char *) texture_download_buffer);
	texture_download_buffer = NULL;
    (*pgrGlideShutdown) ();
}

void S9xGlidePutImage (int snes_width, int snes_height)
{
    GrVertex square [4];

    Glide.voodoo_width = (float) (*pgrSstScreenWidth) ();
    Glide.voodoo_height = (float) (*pgrSstScreenHeight) ();

    //(*pgrBufferClear) (0, 0, GR_ZDEPTHVALUE_FARTHEST);

    if (snes_width <= TEXTURE_SIZE && snes_height <= TEXTURE_SIZE)
    {
	// Special case 256x224 and 256x239 SNES resolutions.
	texture_info.data = (void *) GFX.Screen;

	(*pgrTexDownloadMipMapLevel) (GR_TMU0, texture_mem_start [0],
				  GR_LOD_256, GR_LOD_256, GR_ASPECT_1x1,
				  GR_TEXFMT_RGB_565,
				  GR_MIPMAPLEVELMASK_BOTH,
				  (void *) GFX.Screen);

	(*pgrTexSource) (GR_TMU0, texture_mem_start [0],
		     GR_MIPMAPLEVELMASK_BOTH, &texture_info);

	S9xGlideCalcSquare (snes_width, snes_height, 0, 0, 
			    snes_width, snes_height, square);
	(*pgrDrawTriangle) (&square [0], &square [3], &square [2]);
	(*pgrDrawTriangle) (&square [0], &square [1], &square [2]);
    }
    else
    {
	// Have to chop the rendered SNES screen into a maximum 256x256 size
	// chunks because that's all the Voodoo card can cope with.
	int t = 0;
	int vert_squares = (snes_height + (TEXTURE_SIZE - 1)) / TEXTURE_SIZE;
	int horiz_squares = (snes_width + (TEXTURE_SIZE - 1)) / TEXTURE_SIZE;

	texture_info.data = (void *) texture_download_buffer;

	for (int y = 0; y < vert_squares; y++)
	{
	    for (int x = 0; x < horiz_squares; x++, t++)
	    {
		int square_height = y != vert_squares - 1 ? TEXTURE_SIZE : 
				    snes_height - (TEXTURE_SIZE * y);
		int square_width = x != horiz_squares - 1 ? TEXTURE_SIZE :
				   snes_width - (TEXTURE_SIZE * x);
		int offset = x * TEXTURE_SIZE * sizeof (uint16) +
			     y * TEXTURE_SIZE * GFX.Pitch;

		for (int l = 0; l < square_height; l++)
		{
		    memmove ((void *) (texture_download_buffer + l * 256),
			     GFX.Screen + l * GFX.RealPitch + offset,
			     sizeof (uint16) * 256);
		}
		(*pgrTexDownloadMipMapLevel) (GR_TMU0, texture_mem_start [t],
					  GR_LOD_256, GR_LOD_256, 
					  GR_ASPECT_1x1,
					  GR_TEXFMT_RGB_565,
					  GR_MIPMAPLEVELMASK_BOTH,
					  (void *) texture_download_buffer);

		(*pgrTexSource) (GR_TMU0, texture_mem_start [t],
                                 GR_MIPMAPLEVELMASK_BOTH, &texture_info);

		S9xGlideCalcSquare (snes_width, snes_height, x, y,
				    square_width, square_height,
				    square);
		(*pgrDrawTriangle) (&square [0], &square [3], &square [2]);
		(*pgrDrawTriangle) (&square [0], &square [1], &square [2]);
	    }
	}
    }

    (*pgrBufferSwap) (1);
}

bool8 S9xGlideEnable (bool8 enable)
{
    static uint32 previous_graphic_format = RGB565;
    extern uint32 current_graphic_format;

    if (Settings.GlideEnable != enable)
    {
        WINDOWPLACEMENT place;
        place.length = sizeof (WINDOWPLACEMENT);
        GetWindowPlacement (GUI.hWnd, &place);

	if (enable)
	{
	    if (S9xGlideInit ())
	    {
		previous_graphic_format = current_graphic_format;
		S9xGraphicsDeinit ();
		S9xSetRenderPixelFormat (RGB565);
		S9xGraphicsInit ();
		Settings.GlideEnable = TRUE;
                if (place.showCmd == SW_SHOWNORMAL)
                {
                    place.showCmd = SW_SHOWMAXIMIZED;
                    SetWindowPlacement (GUI.hWnd, &place);
                }
	    }
	}
	else
	{
	    Settings.GlideEnable = FALSE;
	    S9xGlideDeinit ();
	    S9xSetRenderPixelFormat (previous_graphic_format);
	    S9xGraphicsDeinit ();
	    S9xGraphicsInit ();
            if (place.showCmd == SW_SHOWMAXIMIZED && !VOODOO_MODE)
            {
                place.showCmd = SW_SHOWNORMAL;
                SetWindowPlacement (GUI.hWnd, &place);
            }
	}
    }
    return (TRUE);
}
#endif

void RenderGlide (SSurface Src, SSurface Dst, RECT *rect)
{
    rect->left = 0;
    rect->right = DirectX.Width;
    rect->top = 0;
    rect->bottom = DirectX.Height;

#ifdef USE_GLIDE
    if (Glide.voodoo_present && !Settings.GlideEnable)
        S9xGlideEnable (TRUE);

    if (Settings.GlideEnable && Glide.voodoo_present)
        S9xGlidePutImage (Src.Width, Src.Height);
#endif
}

#ifdef USE_OPENGL
#define RED(color) ((color&0x7C00) >> 7)
#define GREEN(color) ((color&0x03E0) >> 2)
#define BLUE(color) ((color&0x001F) << 3 )

BYTE basetexbuffer [512 * 512 * 3];

GLfloat	xrot;			// X Rotation
GLfloat	yrot;			// Y Rotation
GLfloat	zrot;			// Z Rotation
#endif

void RenderOpenGL (SSurface Src, SSurface Dst, RECT *rect)
{
    rect->left = 0;
    rect->right = DirectX.Width;
    rect->top = 0;
    rect->bottom = DirectX.Height;
	
#ifdef USE_OPENGL
    int width, height;
	
    RECT r;
    GetClientRect (GUI.hWnd, &r);
	
    width = r.right - r.left;
    height = r.bottom - r.top;
	
    glViewport (0, 0, width, height);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	
    glFrustum (-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 100.0f);
    glMatrixMode (GL_MODELVIEW);
	
    extern GLuint texture[1];
	    uint8 *data = (uint8 *) Src.Surface;
	
    uint32 swidth = Src.Width > 256 ? 512 : 256;
    int startl = 0;
	
    int texture_size;
	
    if (OpenGL.num_textures == 2 && Src.Width == 256)
        texture_size = 256;
    else
        texture_size = OpenGL.texture_size;

    uint32 pheight = Src.Height % texture_size;
    if (pheight == 0)
        pheight = texture_size;
    
    if (OpenGL.draw_cube)
    {
        xrot += 0.3f;
        yrot += 0.2f;
        zrot += 0.4f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
	
    for (int starty = Src.Height - pheight; starty >= 0; 
	startl += pheight, pheight = texture_size, starty -= texture_size)
    {
        for (int startx = 0; startx < (int) swidth; startx += texture_size)
        {
            uint32 pwidth;
            if ((pwidth = Src.Width - startx) > (uint32) texture_size)
                pwidth = texture_size;
			
            float fx1 = (float) startx / Src.Width;
            float fy1 = (float) starty / Src.Height;
            float fx2 = fx1 + (float) pwidth / Src.Width;
            float fy2 = fy1 + (float) (pheight+1) / Src.Height;
			
            if (!OpenGL.packed_pixels_extension_present)
            {
                int i = 0;
                for (uint32 y = 0; y < pheight; y++)
                {
                    uint16 *ptr = (uint16 *) (GFX.Screen + (y + startl) * GFX.Pitch2) + startx;
                    for (uint32 x = 0; x < pwidth; x++)
                    {
                        int color = *ptr++;
                        basetexbuffer [i  ] = RED(color);
                        basetexbuffer [i+1] = GREEN(color);
                        basetexbuffer [i+2] = BLUE(color);
                        i += 3;
                    }
                }
                data = basetexbuffer;
            }
			else if (Src.Width > (unsigned int)texture_size)
			{
				for (uint32 y = 0; y < pheight; y++)
				{
					/*memmove*/ MoveMemory (basetexbuffer + sizeof (uint16) * texture_size * y,
						(GFX.Screen + (y + startl) * GFX.Pitch2) + startx,
						sizeof (uint16) * texture_size);
				}
				data = basetexbuffer;
			}
			
			if (OpenGL.num_textures == 2 && Src.Width == 256)
				glBindTexture (GL_TEXTURE_2D, OpenGL.textures [1]);
			else
				glBindTexture (GL_TEXTURE_2D, OpenGL.textures [0]);
			
			glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, pwidth, pheight,
				OpenGL.format, OpenGL.type, data);
			
			float maxx = (float) (pwidth - 1) / texture_size;
			float maxy = (float) (pheight - 1) / texture_size;
			
			if (OpenGL.draw_cube)
			{
				glLoadIdentity();
				glTranslatef (0.0f, 0.0f, -3.5f);
				
				glRotatef (xrot, 1.0f, 0.0f, 0.0f);
				glRotatef (yrot, 0.0f, 1.0f, 0.0f);
				glRotatef (zrot, 0.0f, 0.0f, 1.0f);
				
				glBegin(GL_QUADS);
				{
					// Front Face
					glNormal3f (0.0f, 0.0f, 1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f, 1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f, 1.0f);
					// Back Face
					glNormal3f (0.0f, 0.0f, -1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f,-1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f,-1.0f);
					// Top Face
					glNormal3f (0.0f, 1.0f, 0.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f, 1.0f, 1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f, 1.0f, 1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
					// Bottom Face
					glNormal3f (0.0f, -1.0f, 0.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f,-1.0f,-1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f,-1.0f,-1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
					// Right face
					glNormal3f (1.0f, 0.0f, 0.0f);
					glTexCoord2f (maxx, maxy); glVertex3f ( 1.0f,-1.0f,-1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f ( 1.0f, 1.0f,-1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f ( 1.0f, 1.0f, 1.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f ( 1.0f,-1.0f, 1.0f);
					// Left Face
					glNormal3f (-1.0f, 0.0f, 0.0f);
					glTexCoord2f (0.0f, maxy); glVertex3f (-1.0f,-1.0f,-1.0f);
					glTexCoord2f (maxx, maxy); glVertex3f (-1.0f,-1.0f, 1.0f);
					glTexCoord2f (maxx, 0.0f); glVertex3f (-1.0f, 1.0f, 1.0f);
					glTexCoord2f (0.0f, 0.0f); glVertex3f (-1.0f, 1.0f,-1.0f);
				}
				glEnd();
			}
			else
			{
				glLoadIdentity ();
				glTranslatef (-0.5, -0.5, -1.0);
				glDisable (GL_DEPTH_TEST);
				
				glBegin(GL_TRIANGLE_STRIP);
				{
					glTexCoord2f (0.0, maxy);
					glVertex3f (fx1, fy1, 0.0);
					
					glTexCoord2f (maxx, maxy);
					glVertex3f (fx2, fy1, 0.0);
					
					glTexCoord2f (0.0, 0.0);
					glVertex3f (fx1, fy2, 0.0);
					
					glTexCoord2f (maxx, 0.0);
					glVertex3f (fx2, fy2, 0.0);
				}
				glEnd();
			}
        }
    }
	
    SwapBuffers (GUI.hDC);
#endif
}
