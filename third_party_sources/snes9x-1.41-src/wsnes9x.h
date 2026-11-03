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
/*  Snes9X: Win32                                                            */
/*****************************************************************************/
#if !defined(SNES9X_H_INCLUDED)
#define SNES9X_H_INCLUDED
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <mmsystem.h>
#ifndef __BORLANDC__
#include <afxres.h>
#include <dsound.h>
#endif
#include "resource.h"
#include "string_cache.h"

#define COUNT(a) (sizeof (a) / sizeof (a[0]))
#define GUI_VERSION 1008

extern unsigned char* SoundBuffer;
#define MAX_RECENT_GAMES_LIST_SIZE 10

/****************************************************************************/
inline static void Log (const char *str)
{
    FILE *fs = fopen ("snes9x.log", "a");

    if (fs)
    {
      fprintf (fs, "%s\n", str);
      fflush (fs);
      fclose (fs);
    }

}

struct sGUI {
    HWND hWnd;
    HMENU hMenu;
    HINSTANCE hInstance;

    DWORD hFrameTimer;
    DWORD hSoundTimer;
    HANDLE ClientSemaphore;
    HANDLE FrameTimerSemaphore;
    HANDLE ServerTimerSemaphore;

    BYTE Language;

    unsigned long PausedFramesBeforeMutingSound;
    int  Width;
    int  Height;
    int  Depth;
    int  Scale;
    int  NextScale;
    bool DoubleBuffered;
    bool FullScreen;
    bool Stretch;
    bool ScreenCleared;
    bool IgnoreNextMouseMove;
    RECT window_size;
    int  MouseX;
    int  MouseY;
    unsigned int MouseButtons;
    HCURSOR Blank;
    HCURSOR GunSight;
    HCURSOR Arrow;
    int CursorTimer;
    HDC  hDC;
    HACCEL Accelerators;
    bool NeedDepthConvert;
    bool BGR;
    bool TurboModeToggle;
    int  ScreenDepth;
    int  RedShift;
    int  GreenShift;
    int  BlueShift;
    RECT SizeHistory [10];
    unsigned int FlipCounter;
    unsigned int NumFlipFrames;
    char FreezeFileDir [_MAX_PATH];
#ifdef USE_OPENGL    
    HGLRC hRC;
#endif
    char **RecentGames;

	//turbo switches -- SNES-wide
	unsigned char TurboMask;
	char StarOceanPack[MAX_PATH];
	char SFA2PALPack[MAX_PATH];
	char SFA2NTSCPack[MAX_PATH];
	char SFZ2Pack[MAX_PATH];
	char SJNSPack[MAX_PATH];
	char FEOEZPack[MAX_PATH];
	char SPL4Pack[MAX_PATH];
	char MDHPack[MAX_PATH];
	COLORREF InfoColor;
};

//TURBO masks
#define TURBO_A_MASK 0x01
#define TURBO_B_MASK 0x02
#define TURBO_X_MASK 0x04
#define TURBO_Y_MASK 0x08
#define TURBO_L_MASK 0x10
#define TURBO_R_MASK 0x20
#define TURBO_STA_MASK 0x40
#define TURBO_SEL_MASK 0x80

struct sLanguages {
    int idMenu;
    char *errInitDD;
    char *errModeDD;
    char *errInitDS;
    char *ApplyNeedRestart;
    char *errFrameTimer;
};

struct SJoypad {
    BOOL Enabled;
    WORD Left;
    WORD Right;
    WORD Up;
    WORD Down;
    WORD Left_Up;
    WORD Left_Down;
    WORD Right_Up;
    WORD Right_Down;
    WORD Start;
    WORD Select;
    WORD A;
    WORD B;
    WORD X;
    WORD Y;
    WORD L;
    WORD R;
};

#define S9X_JOY_NEUTRAL 60

struct SJoyState{
    bool Attached;
    JOYCAPS Caps;
    int Threshold;
    bool Left;
    bool Right;
    bool Up;
    bool Down;
    bool PovLeft;
    bool PovRight;
    bool PovUp;
    bool PovDown;
    bool RUp;
    bool RDown;
    bool UUp;
    bool UDown;
    bool VUp;
    bool VDown;
    bool ZUp;
    bool ZDown;
    bool Button[32];
};

/*****************************************************************************/

void SetInfoDlgColor(unsigned char r, unsigned char g, unsigned char b);

extern struct sGUI GUI;
extern struct sLanguages Languages[];
extern struct SJoypad Joypad[5];

enum
{
    WIN_SNES9X_DIRECT_SOUND_DRIVER,
    WIN_FMOD_DIRECT_SOUND_DRIVER,
    WIN_FMOD_WAVE_SOUND_DRIVER,
    WIN_FMOD_A3D_SOUND_DRIVER
};

#define S9X_REG_KEY_BASE MY_REG_KEY
#define S9X_REG_KEY_VERSION REG_KEY_VER

#define MAX_SNES_WIDTH 512
#define MAX_SNES_HEIGHT 478

#define EXT_WIDTH (MAX_SNES_WIDTH + 4)
#define EXT_PITCH (EXT_WIDTH * 2)
#define EXT_HEIGHT (MAX_SNES_HEIGHT + 4)
// Offset into buffer to allow a two pixel border around the whole rendered
// SNES image. This is a speed up hack to allow some of the image processing
// routines to access black pixel data outside the normal bounds of the buffer.
#define EXT_OFFSET (EXT_PITCH * 2 + 2 * 2)

#define WIN32_WHITE RGB(255,255,255)

#define SET_UI_COLOR(r,g,b) SetInfoDlgColor(r,g,b)

/*****************************************************************************/

void S9xSetWinPixelFormat ();
#define VOODOO_MODE (GUI.Scale == 7)
#define OPENGL_MODE (GUI.Scale == 8 || GUI.Scale == 9)

#endif // !defined(SNES9X_H_INCLUDED)
