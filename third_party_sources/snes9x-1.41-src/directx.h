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
// DirectX.h: interface for the CDirectX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DIRECTX_H_INCLUDED)
#define DIRECTX_H_INCLUDED

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDirectX  
{
public:
	LPDIRECTSOUND lpDS;
	LPDIRECTSOUNDBUFFER lpDSB;
	LPDIRECTSOUNDBUFFER lpDSBPrimary;

	bool DSAvailable;
	WAVEFORMATEX wfx;

public:
	HRESULT dErr;
	LPDIRECTDRAW lpDD;
	LPDIRECTDRAWCLIPPER lpDDClipper;
	LPDIRECTDRAWPALETTE lpDDPalette;

//	LPDIRECTDRAWSURFACE lpDDSPrimary;
//	LPDIRECTDRAWSURFACE lpDDSBack;
//	LPDIRECTDRAWSURFACE lpDDSOffScreen;

	LPDIRECTDRAWSURFACE2 lpDDSPrimary2;
	LPDIRECTDRAWSURFACE2 lpDDSOffScreen2;

	int Width, Height;
	char Depth;
	bool DoubleBuffered;

	DDPIXELFORMAT DDPixelFormat;
        long FrameCount;
        long LastFrameCount;
        unsigned long IdleCount;
public:
	bool SetSoundMode( void);
	bool SetDisplayMode( 
		int pWidth, int pHeight, 
		char pDepth, bool pWindowed, 
		bool pDoubleBuffered);
        void GetPixelFormat ();
	void DeInitialize();
	bool InitDirectX ();
	bool InitDirectSound ();
        
	CDirectX();
	virtual ~CDirectX();

private:
	bool CreateSoundThread( void);
	bool CreateSoundTimer( void);
};

extern CDirectX DirectX;

#endif // !defined(DIRECTX_H_INCLUDED)
