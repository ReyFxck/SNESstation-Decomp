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


  Win32 GUI code
  (c) Copyright 2003-2004 blip, Nach, Matthew Kendora and funkyass

   
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

#include <shlobj.h>
#include <objidl.h>


#include "wsnes9x.h"
#include "DirectX.h"
#include "snes9x/snes9x.h"
#include "snes9x/memmap.h"
#include "snes9x/cpuexec.h"
#include "snes9x/display.h"
#include "snes9x/3d.h"
#include "snes9x/cheats.h"
#include "snes9x/netplay.h"
#include "snes9x/apu.h"
#include "snes9x/movie.h"
#include "AVIOutput.h"
#include "InputCustom.h"
#include <vector>

#include <TCHAR.H>

#include <fstream.h>
#include <sys/stat.h>
//#include "string_cache.h"
#include "language.h"

//uncomment to find memory leaks, with a line in WinMain
//#include <crtdbg.h>

#include <commctrl.h>
#include <io.h>
#include <time.h>

extern SNPServer NPServer;

#ifdef USE_OPENGL
OpenGLData OpenGL;
#endif

#include <ctype.h>
#ifdef FMOD_SUPPORT
#include "fmod.h"
#endif

__int64 PCBase, PCFrameTime, PCFrameTimeNTSC, PCFrameTimePAL, PCStart, PCEnd;

#ifdef RTC_DEBUGGER
int CALLBACK SPC7110rtc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
int CALLBACK DlgSP7PackConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgSoundConf(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgInfoProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgAboutProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgEmulatorProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

int CALLBACK DlgOpenROMProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgChildSplitProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgNPProgress(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgPackConfigProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgNetConnect(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgNPOptions(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgFunky(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgInputConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgCheater(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgCheatSearch(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgCheatSearchAdd(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgCreateMovie(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK DlgOpenMovie(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT CALLBACK EnumModesCallback( LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext);

int CALLBACK test(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


#ifdef RTC_DEBUGGER
//Messages for sliders (for some reason they don't get included during the build)
#define TBM_GETPOS			(WM_USER)
#define TBM_GETRANGEMIN		(WM_USER+1)
#define TBM_GETRANGEMAX		(WM_USER+2)
#define TBM_GETTIC			(WM_USER+3)
#define TBM_SETTIC			(WM_USER+4)
#define TBM_SETPOS			(WM_USER+5)
#define TBM_SETRANGE			(WM_USER+6)
#define TBM_SETRANGEMIN		(WM_USER+7)
#define TBM_SETRANGEMAX		(WM_USER+8)
#define TBM_CLEARTICS		(WM_USER+9)
#define TBM_SETSEL			(WM_USER+10)
#define TBM_SETSELSTART		(WM_USER+11)
#define TBM_SETSELEND		(WM_USER+12)
#define TBM_GETPTICS			(WM_USER+14)
#define TBM_GETTICPOS		(WM_USER+15)
#define TBM_GETNUMTICS		(WM_USER+16)
#define TBM_GETSELSTART		(WM_USER+17)
#define TBM_GETSELEND		(WM_USER+18)
#define TBM_CLEARSEL			(WM_USER+19)
#define TBM_SETTICFREQ		(WM_USER+20)
#define TBM_SETPAGESIZE		(WM_USER+21)
#define TBM_GETPAGESIZE		(WM_USER+22)
#define TBM_SETLINESIZE		(WM_USER+23)
#define TBM_GETLINESIZE		(WM_USER+24)
#define TBM_GETTHUMBRECT	(WM_USER+25)
#define TBM_GETCHANNELRECT	(WM_USER+26)
#define TBM_SETTHUMBLENGTH	(WM_USER+27)
#define TBM_GETTHUMBLENGTH	(WM_USER+28)
#endif

#define NOTKNOWN "Unknown Company "
#define HEADER_SIZE 512
#define INFO_LEN (0xFF - 0xC0)


/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/
struct sGUI GUI;
typedef struct sExtList
{
	TCHAR* extension;
	bool compressed;
	struct sExtList* next;
} ExtList;

ExtList* valid_ext=NULL;
void MakeExtFile(void);
void LoadExts(void);

extern FILE *trace_fs;
extern int superscope_turbo;
extern int superscope_pause;
extern SCheatData Cheat;
extern bool8 do_frame_adjust;

HINSTANCE g_hInst;

#ifdef DEBUGGER
extern "C" void Trace ();
#endif



static const char *rom_filename = NULL;

CDirectX DirectX;
struct SJoypad Joypad[5] = {
    {
        true,					/* Joypad 1 enabled */
			VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,	/* Left, Right, Up, Down */
			0, 0,                                   /* Left_Up, Left_Down */
			0, 0,                                   /* Right_Up, Right_Down */
			VK_RETURN, VK_SPACE,                    /* Start, Select */
			'D', 'C',				/* A B */
			'S', 'X',				/* X Y */
			'A', 'Z'				/* L R */
    },
    { 
			false,                                  /* Joypad 2 disabled */
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0,
				0, 0, 0, 0, 0, 0
		}, 
		{ 
				false,                                  /* Joypad 3 disabled */
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0,
					0, 0, 0, 0, 0, 0
			}, 
			{ 
					false,                                  /* Joypad 4 disabled */
						0, 0, 0, 0,
						0, 0, 0, 0,
						0, 0,
						0, 0, 0, 0, 0, 0
				}, 
				{ 
						false,                                  /* Joypad 5 disabled */
							0, 0, 0, 0,
							0, 0, 0, 0,
							0, 0,
							0, 0, 0, 0, 0, 0
					}
};

struct SSoundRates 
{
    uint32 rate;
    int ident;
} SoundRates[9] = {
    { 8000, ID_SOUND_8000HZ},
    {11025, ID_SOUND_11025HZ},
    {16000, ID_SOUND_16000HZ},
    {22050, ID_SOUND_22050HZ},
    {30000, ID_SOUND_30000HZ},
	{32000, ID_SOUND_32000HZ},
    {35000, ID_SOUND_35000HZ},
    {44100, ID_SOUND_44100HZ},
    {48000, ID_SOUND_48000HZ}
};

static uint32 FrameTimings[] = {
	1, 1, 4, 8, 17, 20, 35, 70, 140, 300, 500, 1000, 1000
};

// Languages supported by Snes9X: Windows
// 0 - English [Default]
// 1 - Dutch/Nederlands
struct sLanguages Languages[] = {
	{ IDR_MENU_US, 
		TEXT("DirectX failed to initialize!"), 
		TEXT("DirectDraw failed to set the selected display mode!"), 
		TEXT("DirectSound failed to initialize, no sound will be played."),
		TEXT("These settings won't take effect until you restart the emulator."),
		TEXT("The frame timer failed to initialize, please do NOT select the automatic framerate option or Snes9X will crash!")},
	{ IDR_MENU_NL, 
	TEXT("Er is een fout opgetreden tijdens het initalizeren van DirectX!"), 
	TEXT("Er is een fout opgetreden tijdens het verander van scherm modus!"), 
	TEXT("Er is een fout opgetreden tijdens het initializeren van DirectSound, er zal geen geluid worden afgespeeld."),
	TEXT("Deze opties worden pas toegepast als de emulator opnieuw is opgestart."),
	TEXT("Er is een fout opgetreden tijdens het initializeren van de frame timer, kies NIET de automatische framerate optie want dan zal Snes9X crashen!")}
};

struct OpenMovieParams
{
	char Path[_MAX_PATH];
	bool8 ReadOnly;
	uint8 ControllersMask;
	uint8 Opts;
	wchar_t Metadata[MOVIE_MAX_METADATA];
};
struct dMode
{
	long height;
	long width;
	long depth;
	int status;
};

std::vector<dMode> dm;
/*****************************************************************************/
/* WinProc                                                                   */
/*****************************************************************************/
void DoAVIOpen(const char* filename);
void DoAVIClose(int reason);
bool8 SetupSound( long rate, bool8 sixteen_bit, bool8 stereo);
void RestoreGUIDisplay ();
void RestoreSNESDisplay ();
void FreezeUnfreeze (int slot, bool8 freeze);
void CheckDirectoryIsWritable (const char *filename);
static void CheckMenuStates ();
static void ResetFrameTimer ();
bool8 LoadROM (const char *filename);
#ifdef NETPLAY_SUPPORT
static void EnableServer (bool8 enable);
#endif
static void WinDeleteRegistryEntries ();
static void WinSetDefaultValues ();
static void WinDeleteRecentGamesList ();

#ifdef USE_DIRECTX3D
void D3DMove (HWND, LPARAM);
void D3DSizeChange (HWND, WPARAM);
#endif

#include "snes9x/ppu.h"
#include "snes9x/snapshot.h"
extern "C" const char *S9xGetFilename (const char *);
const char *S9xGetFilenameInc (const char *);
void S9xSetRecentGames ();
void S9xAddToRecentGames (const char *filename);

void S9xMouseOn ()
{
    if (IPPU.Controller == SNES_MOUSE || IPPU.Controller == SNES_MOUSE_SWAPPED)
    {
        SetCursor (NULL);
    }
    else if (IPPU.Controller != SNES_SUPERSCOPE&&IPPU.Controller != SNES_JUSTIFIER&&IPPU.Controller != SNES_JUSTIFIER_2)
    {
        SetCursor (GUI.Arrow);
        GUI.CursorTimer = 60;
    }
    else SetCursor (GUI.GunSight);
}

void S9xRestoreWindowTitle ()
{
    TCHAR buf [100];
    sprintf (buf, TEXT(WINDOW_TITLE), VERSION);
    SetWindowText (GUI.hWnd, buf);
}

void SwitchToGDI()
{
    if (!VOODOO_MODE && !OPENGL_MODE)
    {
        IPPU.ColorsChanged = true;
        DirectX.lpDD->FlipToGDISurface();
        GUI.FlipCounter = 0;
        DirectX.lpDDSPrimary2->SetPalette (NULL);
    }
}

static void S9xClearSurface (LPDIRECTDRAWSURFACE2 lpDDSurface)
{
    DDBLTFX fx;
	
    memset (&fx, 0, sizeof (fx));
    fx.dwSize = sizeof (fx);
	
    while (lpDDSurface->Blt (NULL, DirectX.lpDDSPrimary2, NULL, DDBLT_WAIT, NULL) == DDERR_SURFACELOST)
        lpDDSurface->Restore ();
}

void UpdateBackBuffer()
{
    GUI.ScreenCleared = true;
    
    if (!VOODOO_MODE && !OPENGL_MODE && GUI.FullScreen)
    {
        SwitchToGDI();
		
        DDBLTFX fx;
		
        memset (&fx, 0, sizeof (fx));
        fx.dwSize = sizeof (fx);
		
        while (DirectX.lpDDSPrimary2->Blt (NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx) == DDERR_SURFACELOST)
            DirectX.lpDDSPrimary2->Restore ();
        
        if (GetMenu (GUI.hWnd) != NULL)
            DrawMenuBar (GUI.hWnd);
		
        GUI.FlipCounter = 0;
        DDSCAPS caps;
        caps.dwCaps = DDSCAPS_BACKBUFFER;
		
        LPDIRECTDRAWSURFACE2 pDDSurface;
		
        if (DirectX.lpDDSPrimary2->GetAttachedSurface (&caps, &pDDSurface) == DD_OK &&
            pDDSurface != NULL)
        {
            S9xClearSurface (pDDSurface);
            DirectX.lpDDSPrimary2->Flip (NULL, DDFLIP_WAIT);
            while (DirectX.lpDDSPrimary2->GetFlipStatus (DDGFS_ISFLIPDONE) != DD_OK)
                Sleep (0);
            S9xClearSurface (pDDSurface);
        }
    }
    else
    { 
        if (GetMenu( GUI.hWnd) != NULL)
            DrawMenuBar (GUI.hWnd);
    }
}

void ToggleFullScreen ()
{
    S9xSetPause (PAUSE_TOGGLE_FULL_SCREEN);
	
#ifdef USE_GLIDE
    if (VOODOO_MODE)
    {
		//        S9xGlideEnable (FALSE);
		//        GUI.Scale = 0;
		//        MoveWindow (GUI.hWnd, GUI.window_size.left,
		//                    GUI.window_size.top, 
		//                    GUI.window_size.right - GUI.window_size.left,
		//                    GUI.window_size.bottom - GUI.window_size.top, TRUE);
    }
    else
#endif
#ifdef USE_OPENGL
		if (OPENGL_MODE)
		{
			if (!GUI.FullScreen)
			{
				DEVMODE dmScreenSettings;
				
				memset (&dmScreenSettings, 0, sizeof(dmScreenSettings));
				dmScreenSettings.dmSize = sizeof(dmScreenSettings);
				dmScreenSettings.dmPelsWidth = GUI.Width;
				dmScreenSettings.dmPelsHeight = GUI.Height;
				dmScreenSettings.dmBitsPerPel = 16; //bits;
				dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH |
					DM_PELSHEIGHT;
				if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
				{
					GUI.FullScreen = TRUE;
					SetWindowLong (GUI.hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
					SetWindowPos (GUI.hWnd, HWND_TOP, 0, 0, GUI.Width,
						GUI.Height,
						SWP_DRAWFRAME|SWP_FRAMECHANGED);
				}
			}
			else
			{
				SetWindowLong (GUI.hWnd, GWL_STYLE, WS_POPUPWINDOW|WS_CAPTION|
					WS_THICKFRAME|WS_VISIBLE|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
				SetWindowPos (GUI.hWnd, HWND_TOP, 
					GUI.window_size.left,
					GUI.window_size.top,
					GUI.window_size.right - GUI.window_size.left,
					GUI.window_size.bottom - GUI.window_size.top,
					SWP_DRAWFRAME|SWP_FRAMECHANGED);
				ChangeDisplaySettings (NULL, 0);
				GUI.FullScreen = FALSE;
			}
		}
		else
		{
#endif
			if (!VOODOO_MODE && !GUI.FullScreen)
				GetWindowRect (GUI.hWnd, &GUI.window_size);
			
			if (!DirectX.SetDisplayMode (GUI.Width, GUI.Height, GUI.Depth, GUI.FullScreen, GUI.DoubleBuffered))
			{
				MessageBox( GUI.hWnd, Languages[ GUI.Language].errModeDD, TEXT("Snes9X - DirectDraw(2)"), MB_OK | MB_ICONSTOP);
				S9xClearPause (PAUSE_TOGGLE_FULL_SCREEN);
				return;
			}
			GUI.FullScreen = !GUI.FullScreen;
			if (!GUI.FullScreen)
			{
				SwitchToGDI();
				MoveWindow (GUI.hWnd, GUI.window_size.left,
					GUI.window_size.top,
					GUI.window_size.right - GUI.window_size.left,
					GUI.window_size.bottom - GUI.window_size.top, TRUE);
			}
#if defined(USE_GLIDE) || defined (USE_OPENGL)
		}
#endif
		S9xGraphicsDeinit();
		S9xSetWinPixelFormat ();
		S9xGraphicsInit();
		
		IPPU.RenderThisFrame = true;
		UpdateBackBuffer();
		
		S9xClearPause (PAUSE_TOGGLE_FULL_SCREEN);
}

void S9xDisplayStateChange (const char *str, bool8 on)
{
    static char string [100];
	
    sprintf (string, "%s %s", str, on ? "on" : "off");
    S9xSetInfoString (string);
}

LRESULT CALLBACK WinProc(
						 HWND hWnd,
						 UINT uMsg,
						 WPARAM wParam,
						 LPARAM lParam)
{
    int i;
    //bool showFPS;
    static char InfoString [100];
#ifdef NETPLAY_SUPPORT
    char hostname [100];
#endif
    switch (uMsg)
    {
	case WM_CREATE:
		g_hInst = ((LPCREATESTRUCT)lParam)->hInstance;
#ifndef MK_APU
		DeleteMenu(GUI.hMenu,IDM_CATCH_UP_SOUND,MF_BYCOMMAND);
#endif
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_TAB:
			//until I decide how to really handle this
			if(Settings.SPC7110RTC)
				break;
			if (GUI.TurboModeToggle)
			{
				Settings.TurboMode ^= TRUE;
				if (Settings.TurboMode)
					S9xMessage (S9X_INFO, S9X_TURBO_MODE,
					WINPROC_TURBOMODE_ON);
				else
					S9xMessage (S9X_INFO, S9X_TURBO_MODE,
					WINPROC_TURBOMODE_OFF);
			}
			else
			{
				Settings.TurboMode = TRUE;
				S9xMessage (S9X_INFO, S9X_TURBO_MODE, WINPROC_TURBOMODE_TEXT);
			}
			
			break;
		case VK_ESCAPE:
			if (GetMenu (GUI.hWnd) == NULL)
				SetMenu (GUI.hWnd, GUI.hMenu);
			else
				SetMenu (GUI.hWnd, NULL);
			
			UpdateBackBuffer();
			break;
			
		case VK_SCROLL:
			break;
		case '0':
			Settings.DisableHDMA = !Settings.DisableHDMA;
			S9xDisplayStateChange (WINPROC_HDMA_TEXT, !Settings.DisableHDMA);
			break;
		case '1':
			PPU.BG_Forced ^= 1;
			S9xDisplayStateChange (WINPROC_BG1, !(PPU.BG_Forced & 1));
			break;
		case '2':
			PPU.BG_Forced ^= 2;
			S9xDisplayStateChange (WINPROC_BG2, !(PPU.BG_Forced & 2));
			break;
		case '3':
			PPU.BG_Forced ^= 4;
			S9xDisplayStateChange (WINPROC_BG3, !(PPU.BG_Forced & 4));
			break;
		case '4':
			PPU.BG_Forced ^= 8;
			S9xDisplayStateChange (WINPROC_BG4, !(PPU.BG_Forced & 8));
			break;
		case '5':
			PPU.BG_Forced ^= 16;
			S9xDisplayStateChange (WINPROC_SPRITES, !(PPU.BG_Forced & 16));
			break;
		case '6':
#ifdef USE_OPENGL
			if (Settings.OpenGLEnable && 
				(GetKeyState (VK_SHIFT) & 0x80))
			{
				OpenGL.draw_cube ^= TRUE;
			}
			else
#endif
			{
				Settings.SwapJoypads = !Settings.SwapJoypads;
				S9xDisplayStateChange (WINPROC_PADSWAP, Settings.SwapJoypads);
			}
			break;
		case '7':
			{
				static char *controllers [] = {
					WINPROC_CONTROLERS0, 
					WINPROC_CONTROLERS1, 
					WINPROC_CONTROLERS2,
					WINPROC_CONTROLERS3, 
					WINPROC_CONTROLERS4, 
					WINPROC_CONTROLERS5, 
					WINPROC_CONTROLERS6
				};
				S9xNextController ();
				//load new game sanity check
				Settings.ControllerOption=IPPU.Controller;
				S9xSetInfoString (controllers [IPPU.Controller]);
				if (IPPU.Controller == SNES_SUPERSCOPE||IPPU.Controller==SNES_JUSTIFIER||IPPU.Controller==SNES_JUSTIFIER_2)
				{
					SetCursor (GUI.GunSight);
				}
				else
				{
					SetCursor (GUI.Arrow);
					GUI.CursorTimer = 60;
				}
				
				if (IPPU.Controller == SNES_MOUSE || 
					IPPU.Controller == SNES_MOUSE_SWAPPED)
				{
					POINT middle;
					RECT size;
					
					GetClientRect (GUI.hWnd, &size);
					middle.x = (size.right - size.left) >> 1;
					middle.y = (size.bottom - size.top) >> 1;
					ClientToScreen (GUI.hWnd, &middle);
					SetCursorPos (middle.x, middle.y);
				}
				break;
			}
		case '8':
			Settings.BGLayering = !Settings.BGLayering;
			S9xDisplayStateChange (WINPROC_BGHACK, 
				Settings.BGLayering);
			break;
		case '9':
			if (GetKeyState (VK_SHIFT) & 0x80)
			{
				Settings.Mode7Interpolate ^= TRUE;
				S9xDisplayStateChange (WINPROC_MODE7INTER, 
					Settings.Mode7Interpolate);
			}
			else
				if (Settings.SixteenBit)
				{	
					Settings.Transparency = !Settings.Transparency;
					S9xDisplayStateChange (WINPROC_TRANSPARENCY, 
						Settings.Transparency);
				}
				break;
				
		case VK_BACK:
			Settings.DisableGraphicWindows = !Settings.DisableGraphicWindows;
			S9xDisplayStateChange (WINPROC_CLIPWIN,
				!Settings.DisableGraphicWindows);
			break;
			
		case VK_PAUSE:
			Settings.Paused = Settings.Paused ^ true;
			Settings.FrameAdvance = false;
//			S9xDisplayStateChange (WINPROC_PAUSE, Settings.Paused);
			break;
			
		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
			FreezeUnfreeze (wParam - VK_F1,
				(GetKeyState (VK_SHIFT) & 0x80) != 0);
			break;
            }
            break;
			
	case WM_KEYUP:
		switch( wParam)
		{
		case VK_TAB:
			if (!GUI.TurboModeToggle)
				Settings.TurboMode = FALSE;
			break;
		}
		break;
        case WM_CHAR:
            switch (wParam)
            {
			case '`':
			case '#':
			case '~':
				superscope_turbo ^= 1;
				break;
			case '/':
				superscope_pause ^= 1;
				break;
			case '.':
				S9xMovieToggleFrameDisplay ();
				break;
			case '|':
			case '\\':
				if(Settings.Paused)
				{
					Settings.FrameAdvance = true;
					// kick the main thread out of GetMessage (just in case)
					SendMessage(GUI.hWnd, WM_NULL, 0, 0);
				}
				break;
			case '+':
			case '=':
				if (GetAsyncKeyState (VK_SHIFT))
				{
					// Increase emulated frame time
					int i;
					for(i=1; FrameTimings[i]<Settings.FrameTime; ++i)
						;
					Settings.FrameTime = FrameTimings[i+1];
					ResetFrameTimer ();
					sprintf (InfoString, WINPROC_EMUFRAMETIME,
						Settings.FrameTime / 1);
					S9xSetInfoString (InfoString);
				}
				else
				{
					if (Settings.SkipFrames == AUTO_FRAMERATE)
						Settings.SkipFrames = 1;
					else
						if (Settings.SkipFrames < 10)
							Settings.SkipFrames++;
						
						if (Settings.SkipFrames == AUTO_FRAMERATE)
							S9xSetInfoString (WINPROC_AUTOSKIP);
						else
						{
							sprintf (InfoString, WINPROC_FRAMESKIP,
								Settings.SkipFrames - 1);
							S9xSetInfoString (InfoString);
						}
				}
				break;
			case '-':
			case '_':
				if (GetAsyncKeyState (VK_SHIFT))
				{
					// Decrease emulated frame time
					int i;
					for(i=1; FrameTimings[i]<Settings.FrameTime; ++i)
						;
					Settings.FrameTime = FrameTimings[i-1];
					
					ResetFrameTimer ();
					sprintf (InfoString, WINPROC_EMUFRAMETIME,
						Settings.FrameTime / 1);
					S9xSetInfoString (InfoString);
				}
				else
				{
					if (Settings.SkipFrames <= 1)
						Settings.SkipFrames = AUTO_FRAMERATE;
					else
						if (Settings.SkipFrames != AUTO_FRAMERATE)
							Settings.SkipFrames--;
						
						if (Settings.SkipFrames == AUTO_FRAMERATE)
							S9xSetInfoString (WINPROC_AUTOSKIP);
						else
						{
							sprintf (InfoString, WINPROC_FRAMESKIP,
								Settings.SkipFrames - 1);
							S9xSetInfoString (InfoString);
						}
				}
				break;
            }
            break;
			case WM_COMMAND:
				switch (wParam & 0xffff)
				{
				case ID_FILE_WRITE_AVI:
					{
						RestoreGUIDisplay ();  //exit DirectX
						OPENFILENAME  ofn;
						char  szFileName[MAX_PATH];

						szFileName[0] = '\0';

						ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME) );
						ofn.lStructSize = sizeof(OPENFILENAME);
						ofn.hwndOwner = GUI.hWnd;
						ofn.lpstrFilter = "AVI File\0*.avi\0All files\0*.*\0\0";
						ofn.lpstrFile = szFileName;
						ofn.lpstrDefExt = "avi";
						ofn.nMaxFile = MAX_PATH;
						ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
						if(GetSaveFileName( &ofn ))
						{
							DoAVIOpen(szFileName);
						}
						RestoreSNESDisplay ();// re-enter after dialog
					}
					break;
				case ID_FILE_STOP_AVI:
					DoAVIClose(0);
					break;
				case ID_FILE_MOVIE_STOP:
					S9xMovieStop(FALSE);
					break;
				case ID_FILE_MOVIE_PLAY:
					{
						RestoreGUIDisplay ();  //exit DirectX
						OpenMovieParams op;
						memset(&op, 0, sizeof(op));
						if(DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_OPENMOVIE), hWnd, DlgOpenMovie, (LPARAM)&op) &&
							op.Path[0]!='\0')
						{
							int err=S9xMovieOpen (op.Path, op.ReadOnly);
							if(err!=SUCCESS)
							{
								_TCHAR* err_string=TEXT(MOVIE_ERR_COULD_NOT_OPEN);
								switch(err)
								{
								case FILE_NOT_FOUND:
									err_string=TEXT(MOVIE_ERR_NOT_FOUND);
									break;
								case WRONG_FORMAT:
									err_string=TEXT(MOVIE_ERR_WRONG_FORMAT);
									break;
								case WRONG_VERSION:
									err_string=TEXT(MOVIE_ERR_WRONG_VERSION);
									break;
								}
								MessageBox( hWnd, err_string, TEXT(SNES9X_INFO), MB_OK);
							}
						}
						RestoreSNESDisplay ();// re-enter after dialog
					}
					break;
				case ID_FILE_MOVIE_RECORD:
					{
						RestoreGUIDisplay ();  //exit DirectX
						OpenMovieParams op;
						memset(&op, 0, sizeof(op));
						if(DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_CREATEMOVIE), hWnd, DlgCreateMovie, (LPARAM)&op) &&
							op.Path[0]!='\0')
						{
							int err=S9xMovieCreate (op.Path, op.ControllersMask, op.Opts, op.Metadata, wcslen(op.Metadata));
							if(err!=SUCCESS)
							{
								_TCHAR* err_string=TEXT(MOVIE_ERR_COULD_NOT_OPEN);
								switch(err)
								{
								case FILE_NOT_FOUND:
									err_string=TEXT(MOVIE_ERR_NOT_FOUND);
									break;
								case WRONG_FORMAT:
									err_string=TEXT(MOVIE_ERR_WRONG_FORMAT);
									break;
								case WRONG_VERSION:
									err_string=TEXT(MOVIE_ERR_WRONG_VERSION);
									break;
								}
								MessageBox( hWnd, err_string, TEXT(SNES9X_INFO), MB_OK);
							}
						}
						RestoreSNESDisplay ();// re-enter after dialog
					}
					break;
				case ID_SCREENSHOT:
					Settings.TakeScreenshot=true;
					break;
				case IDM_GFX_PACKS:
					RestoreGUIDisplay ();  //exit DirectX
					DialogBox(g_hInst, MAKEINTRESOURCE(IDD_GFX_PACK), hWnd, DlgPackConfigProc);
					RestoreSNESDisplay ();// re-enter after dialog
					break;
				case IDM_CATCH_UP_SOUND:
					Settings.SampleCatchup=!Settings.SampleCatchup;
					if(Settings.SampleCatchup)
						CheckMenuItem(GUI.hMenu, IDM_CATCH_UP_SOUND, MFS_CHECKED);
					else CheckMenuItem(GUI.hMenu, IDM_CATCH_UP_SOUND, MFS_UNCHECKED);
					break;
				case IDM_ENABLE_MULTITAP:
					Settings.MultiPlayer5=Settings.MultiPlayer5Master=!Settings.MultiPlayer5Master;
					if(Settings.MultiPlayer5Master)
						CheckMenuItem(GUI.hMenu, IDM_ENABLE_MULTITAP, MFS_CHECKED);
					else CheckMenuItem(GUI.hMenu, IDM_ENABLE_MULTITAP, MFS_UNCHECKED);
					
					break;
				case IDM_SCOPE_TOGGLE:
					Settings.SuperScope=Settings.SuperScopeMaster=!Settings.SuperScopeMaster;
					if(Settings.SuperScopeMaster)
						CheckMenuItem(GUI.hMenu, IDM_SCOPE_TOGGLE, MFS_CHECKED);
					else CheckMenuItem(GUI.hMenu, IDM_SCOPE_TOGGLE, MFS_UNCHECKED);
					break;
					
				case IDM_JUSTIFIER:
					Settings.Justifier=Settings.JustifierMaster=!Settings.JustifierMaster;
					if(Settings.JustifierMaster)
						CheckMenuItem(GUI.hMenu, IDM_JUSTIFIER, MFS_CHECKED);
					else CheckMenuItem(GUI.hMenu, IDM_JUSTIFIER, MFS_UNCHECKED);
					break;
					
				case IDM_MOUSE_TOGGLE:
					Settings.Mouse=Settings.MouseMaster=!Settings.MouseMaster;
					if(Settings.MouseMaster)
						CheckMenuItem(GUI.hMenu, IDM_MOUSE_TOGGLE, MFS_CHECKED);
					else CheckMenuItem(GUI.hMenu, IDM_MOUSE_TOGGLE, MFS_UNCHECKED);
					break;
					
					//start turbo
				case ID_TURBO_R:
					GUI.TurboMask^=TURBO_R_MASK;
					if(GUI.TurboMask&TURBO_R_MASK)
						S9xSetInfoString (WINPROC_TURBO_R_ON);
					else S9xSetInfoString (WINPROC_TURBO_R_OFF);
					break;
				case ID_TURBO_L:
					GUI.TurboMask^=TURBO_L_MASK;
					if(GUI.TurboMask&TURBO_L_MASK)
						S9xSetInfoString (WINPROC_TURBO_L_ON);
					else S9xSetInfoString (WINPROC_TURBO_L_OFF);
					break;
				case ID_TURBO_A:
					GUI.TurboMask^=TURBO_A_MASK;
					if(GUI.TurboMask&TURBO_A_MASK)
						S9xSetInfoString (WINPROC_TURBO_A_ON);
					else S9xSetInfoString (WINPROC_TURBO_A_OFF);
					break;
				case ID_TURBO_B:
					GUI.TurboMask^=TURBO_B_MASK;
					if(GUI.TurboMask&TURBO_B_MASK)
						S9xSetInfoString (WINPROC_TURBO_B_ON);
					else S9xSetInfoString (WINPROC_TURBO_B_OFF);
					break;
				case ID_TURBO_Y:
					GUI.TurboMask^=TURBO_Y_MASK;
					if(GUI.TurboMask&TURBO_Y_MASK)
						S9xSetInfoString (WINPROC_TURBO_Y_ON);
					else S9xSetInfoString (WINPROC_TURBO_Y_OFF);
					break;
				case ID_TURBO_X:
					GUI.TurboMask^=TURBO_X_MASK;
					if(GUI.TurboMask&TURBO_X_MASK)
						S9xSetInfoString (WINPROC_TURBO_X_ON);
					else S9xSetInfoString (WINPROC_TURBO_X_OFF);
					break;
				case ID_TURBO_START:
					GUI.TurboMask^=TURBO_STA_MASK;
					if(GUI.TurboMask&TURBO_STA_MASK)
						S9xSetInfoString (WINPROC_TURBO_START_ON);
					else S9xSetInfoString (WINPROC_TURBO_START_OFF);
					break;
				case ID_TURBO_SELECT:
					GUI.TurboMask^=TURBO_SEL_MASK;
					if(GUI.TurboMask&TURBO_SEL_MASK)
						S9xSetInfoString (WINPROC_TURBO_SEL_ON);
					else S9xSetInfoString (WINPROC_TURBO_SEL_OFF);
					break;
					//end turbo
				case ID_OPTIONS_DISPLAY:
					{
						int old_scale = GUI.NextScale;
						bool old_stretch = GUI.Stretch;
						
						RestoreGUIDisplay ();
						//showFPS = Settings.DisplayFrameRate ? true : false;
						if (!VOODOO_MODE && !GUI.FullScreen)
							GetWindowRect (GUI.hWnd, &GUI.window_size);
						DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NEWDISPLAY), hWnd, DlgFunky);
						//_DirectXConfig (DirectX.lpDD, &Settings, &GUI, &showFPS);
						
						//Settings.DisplayFrameRate = showFPS;
						SwitchToGDI();
						if (GUI.NextScale != old_scale)
						{
							if ((GUI.Scale < 7 && GUI.NextScale < 7) ||
								(GUI.Scale > 7 && GUI.NextScale > 7))
								GUI.Scale = GUI.NextScale;
							else
							{
                            MessageBox (GUI.hWnd, TEXT(WINPROC_FILTER_RESTART),
													 TEXT(SNES9X_INFO),
													 MB_OK | MB_ICONINFORMATION);
							}
						}
						RestoreSNESDisplay ();
						if (!GUI.FullScreen)
						{
							MoveWindow (GUI.hWnd, GUI.window_size.left,
								GUI.window_size.top, 
								GUI.window_size.right - GUI.window_size.left,
								GUI.window_size.bottom - GUI.window_size.top, TRUE);
						}
						S9xGraphicsDeinit();
						S9xSetWinPixelFormat ();
						S9xGraphicsInit();
						
						IPPU.RenderThisFrame = false;
						
						if (old_stretch != GUI.Stretch || old_scale != GUI.Scale)
						{
							RECT rect;
							GetClientRect (GUI.hWnd, &rect);
							InvalidateRect (GUI.hWnd, &rect, true);
						}
						break;
					}
				case ID_OPTIONS_JOYPAD:
                    RestoreGUIDisplay ();
					DialogBox(g_hInst, MAKEINTRESOURCE(IDD_INPUTCONFIG), hWnd, DlgInputConfig);
                    RestoreSNESDisplay ();
                    break;
					
				case ID_FILE_OPEN:
					{
						char filename [_MAX_PATH];
						
#ifdef NETPLAY_SUPPORT
						if (Settings.NetPlay && !Settings.NetPlayServer)
						{
							S9xMessage (S9X_INFO, S9X_NETPLAY_NOT_SERVER,
								WINPROC_DISCONNECT);
							break;
						}
#endif
						RestoreGUIDisplay ();
						INITCOMMONCONTROLSEX icex;
						
						icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
						icex.dwICC   = ICC_LISTVIEW_CLASSES|ICC_TREEVIEW_CLASSES;
						InitCommonControlsEx(&icex);
						
						if(1<=DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_OPEN_ROM), hWnd, DlgOpenROMProc, (LPARAM)filename))
							
						{
							if (!Settings.StopEmulation)
							{
								Memory.SaveSRAM (S9xGetFilename (".srm"));
								S9xSaveCheatFile (S9xGetFilename (".cht"));
							}
							Settings.StopEmulation = !LoadROM (filename);
							if (!Settings.StopEmulation) 
							{
								Memory.LoadSRAM (S9xGetFilename (".srm"));
								S9xLoadCheatFile (S9xGetFilename (".cht"));
								S9xAddToRecentGames (filename);
								CheckDirectoryIsWritable (S9xGetFilename (".---"));
								CheckMenuStates ();
#ifdef NETPLAY_SUPPORT
								if (NPServer.SendROMImageOnConnect)
									S9xNPServerQueueSendingROMImage ();
								else
									S9xNPServerQueueSendingLoadROMRequest (Memory.ROMName);
#endif
							}
							
							if (IPPU.Controller == SNES_SUPERSCOPE)
								SetCursor (GUI.GunSight);
							else
							{
								SetCursor (GUI.Arrow);
								GUI.CursorTimer = 60;
							}
							Settings.Paused = false;
						}
						
						RestoreSNESDisplay ();
						GUI.ScreenCleared = true;
					}
					break;
					
				case ID_FILE_EXIT:
                    S9xSetPause (PAUSE_EXIT);
#ifdef USE_GLIDE
                    S9xGlideEnable (FALSE);
#endif
                    PostMessage (hWnd, WM_DESTROY, 0, 0);
                    break;
					
				case ID_WINDOW_HIDEMENUBAR:
                    if( GetMenu( GUI.hWnd) == NULL)
                        SetMenu( GUI.hWnd, GUI.hMenu);
                    else
                        SetMenu( GUI.hWnd, NULL);
                    GUI.ScreenCleared = true;
                    break;
					
				case ID_LANGUAGE_ENGLISH:
                    GUI.Language = 0;
					
                    SetMenu( GUI.hWnd, LoadMenu( GUI.hInstance, MAKEINTRESOURCE( Languages[ GUI.Language].idMenu)));
                    DestroyMenu( GUI.hMenu);
                    GUI.hMenu = GetMenu( GUI.hWnd);
                    break;
				case ID_LANGUAGE_NEDERLANDS:
                    GUI.Language = 1;
					
                    SetMenu( GUI.hWnd, LoadMenu( GUI.hInstance, MAKEINTRESOURCE( Languages[ GUI.Language].idMenu)));
                    DestroyMenu( GUI.hMenu);
                    GUI.hMenu = GetMenu( GUI.hWnd);
                    break;
#ifdef NETPLAY_SUPPORT					
				case ID_NETPLAY_SERVER:
                    S9xRestoreWindowTitle ();
                    EnableServer (!Settings.NetPlayServer);
                    break;
                case ID_NETPLAY_CONNECT:
                    RestoreGUIDisplay ();
					if(1<=DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_NETCONNECT), hWnd, DlgNetConnect,(LPARAM)&hostname))

                    {


						S9xSetPause (PAUSE_NETPLAY_CONNECT);

                        if (!S9xNPConnectToServer (hostname, Settings.Port,
							Memory.ROMName))
                        {
                            S9xClearPause (PAUSE_NETPLAY_CONNECT);
                        }
                    }

					RestoreSNESDisplay ();
                    break;
                case ID_NETPLAY_DISCONNECT:
                    if (Settings.NetPlay)
                    {
                        Settings.NetPlay = FALSE;
                        S9xNPDisconnect ();
                    }
                    if (Settings.NetPlayServer)
                    {
                        Settings.NetPlayServer = FALSE;
                        S9xNPStopServer ();
                    }
                    break;
                case ID_NETPLAY_OPTIONS:
					{
						bool8 old_netplay_server = Settings.NetPlayServer;
						RestoreGUIDisplay ();
						if(1<=DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NPOPTIONS), hWnd, DlgNPOptions))
						{
							if (old_netplay_server != Settings.NetPlayServer)
							{
								Settings.NetPlayServer = old_netplay_server;
								S9xRestoreWindowTitle ();
								EnableServer (!Settings.NetPlayServer);
							}
						}
						RestoreSNESDisplay ();
						break;
					}
                case ID_NETPLAY_SYNC:
                    S9xNPServerQueueSyncAll ();
                    break;
                case ID_NETPLAY_ROM:
                    if (NPServer.SyncByReset)
                    {
					if (MessageBox (GUI.hWnd, TEXT(WINPROC_NET_RESTART), TEXT(SNES9X_WARN),
												 MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
												 break;
                    }
                    S9xNPServerQueueSendingROMImage ();
                    break;
                case ID_NETPLAY_SEND_ROM_ON_CONNECT:
                    NPServer.SendROMImageOnConnect ^= TRUE;
                    break;
                case ID_NETPLAY_SYNC_BY_RESET:
                    NPServer.SyncByReset ^= TRUE;
                    break;
#endif
                case ID_SOUND_8000HZ:
				case ID_SOUND_11025HZ:
				case ID_SOUND_16000HZ:
				case ID_SOUND_22050HZ:
				case ID_SOUND_30000HZ:
				case ID_SOUND_35000HZ:
				case ID_SOUND_44100HZ:
				case ID_SOUND_48000HZ:
				case ID_SOUND_32000HZ:
                    for( i = 0; i < COUNT(SoundRates); i ++)
						if (SoundRates[i].ident == (int) wParam)
						{
                            Settings.SoundPlaybackRate = SoundRates [i].rate;
                            if (!SetupSound (Settings.SoundPlaybackRate, 
								Settings.SixteenBitSound, 
								Settings.Stereo))
                            {	MessageBox( GUI.hWnd, Languages[ GUI.Language].errInitDS, TEXT(SNES9X_DXS), MB_OK | MB_ICONINFORMATION);	}
                            break;
						}
						break;
						
				case ID_SOUND_NOSOUND:
                    Settings.SoundPlaybackRate = 0;
                    SetupSound( 0, FALSE, FALSE);
                    break;
                case ID_SOUND_25MS:
                    Settings.SoundBufferSize = 1;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
                case ID_SOUND_50MS:
                    Settings.SoundBufferSize = 2;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
                case ID_SOUND_100MS:
                    Settings.SoundBufferSize = 4;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
                case ID_SOUND_200MS:
                    Settings.SoundBufferSize = 8;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
                case ID_SOUND_500MS:
                    Settings.SoundBufferSize = 16;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
                case ID_SOUND_1S:
                    Settings.SoundBufferSize = 32;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
                case ID_SOUND_2S:
                    Settings.SoundBufferSize = 64;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
					
                case ID_SOUND_STEREO:
                    Settings.Stereo = !Settings.Stereo;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
                case ID_SOUND_REVERSE_STEREO:
                    Settings.ReverseStereo = !Settings.ReverseStereo;
                    break;
                case ID_SOUND_16BIT:
                    Settings.SixteenBitSound = !Settings.SixteenBitSound;
                    SetupSound (Settings.SoundPlaybackRate, 
						Settings.SixteenBitSound, Settings.Stereo);
                    break;
                case ID_SOUND_INTERPOLATED:
                    Settings.InterpolatedSound = !Settings.InterpolatedSound;
					S9xDisplayStateChange (WINPROC_INTERPOLATED_SND, Settings.InterpolatedSound);
                    break;
                case ID_SOUND_SYNC:
                    Settings.SoundSync = !Settings.SoundSync;
					S9xDisplayStateChange (WINPROC_SYNC_SND, Settings.SoundSync);
                    break;
                case ID_SOUND_OPTIONS:
					{
						struct SSettings orig = Settings;
						RestoreGUIDisplay ();
						if(1<=DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_SOUND_OPTS),hWnd,DlgSoundConf, (LPARAM)&Settings))
						{
							if (orig.NextAPUEnabled != Settings.NextAPUEnabled)
							{
								if (!Settings.NextAPUEnabled)
								{
                                if (MessageBox (GUI.hWnd, TEXT(WINPROC_SND_OFF),
															 TEXT(SNES9X_SNDQ),
															 MB_YESNO | MB_ICONQUESTION) == IDNO)
                                {
                                    Settings.NextAPUEnabled = orig.NextAPUEnabled;
                                }
                                else
                                {
                                    Settings.APUEnabled = FALSE;
                                    SetupSound (0, FALSE, FALSE);
                                }
								}
								else
								{
									if (!Settings.StopEmulation)
									{
                                    MessageBox (GUI.hWnd, TEXT(WINPROC_SND_RESTART), TEXT(SNES9X_SNDQ),
															 MB_OK | MB_ICONINFORMATION);
									}
									else
										Settings.APUEnabled = Settings.NextAPUEnabled;
								}
							}
							else
								if (orig.SixteenBitSound != Settings.SixteenBitSound ||
									orig.Stereo != Settings.Stereo ||
									orig.SoundPlaybackRate != Settings.SoundPlaybackRate ||
									orig.SoundBufferSize != Settings.SoundBufferSize ||
									orig.SoundDriver != Settings.SoundDriver ||
									orig.SoundMixInterval != Settings.SoundMixInterval)
								{
									if (Settings.SoundDriver != WIN_SNES9X_DIRECT_SOUND_DRIVER)
										Settings.SoundSync = FALSE;
									
									SetupSound (Settings.SoundPlaybackRate, 
                                        Settings.SixteenBitSound, 
                                        Settings.Stereo);
								}
						}
						RestoreSNESDisplay ();
						break;
					}
#ifdef RTC_DEBUGGER
						case IDM_7110_RTC:
							{
								struct SPC7110RTC origrtc = s7r.rtc;
								RestoreGUIDisplay ();
								if(1<=DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_7110_RTC),hWnd,SPC7110rtc, (LPARAM)&origrtc))
								{
									rtc_f9.reg[0x00]=origrtc.reg[0x00];
									rtc_f9.reg[0x01]=origrtc.reg[0x01];
									rtc_f9.reg[0x02]=origrtc.reg[0x02];
									rtc_f9.reg[0x03]=origrtc.reg[0x03];
									rtc_f9.reg[0x04]=origrtc.reg[0x04];
									rtc_f9.reg[0x05]=origrtc.reg[0x05];
									rtc_f9.reg[0x06]=origrtc.reg[0x06];
									rtc_f9.reg[0x07]=origrtc.reg[0x07];
									rtc_f9.reg[0x08]=origrtc.reg[0x08];
									rtc_f9.reg[0x09]=origrtc.reg[0x09];
									rtc_f9.reg[0x0A]=origrtc.reg[0x0A];
									rtc_f9.reg[0x0B]=origrtc.reg[0x0B];
									rtc_f9.reg[0x0C]=origrtc.reg[0x0C];
									
									rtc_f9.reg[0x0D]=origrtc.reg[0x0D];
									
									rtc_f9.reg[0x0E]=origrtc.reg[0x0E];
									rtc_f9.reg[0x0F]=origrtc.reg[0x0F];
									rtc_f9.last_used=time(NULL);
									
								}
								RestoreSNESDisplay ();
								break;
							}
#endif
								case ID_WINDOW_FULLSCREEN:
									ToggleFullScreen ();
									break;
								case ID_WINDOW_STRETCH:
									GUI.Stretch = !GUI.Stretch;
									RECT rect;
									GetClientRect (GUI.hWnd, &rect);
									InvalidateRect (GUI.hWnd, &rect, true);
									break;
								case ID_FILE_SAVE_SPC_DATA:
									spc_is_dumping = 1;
									//                    S9xSPCDump (S9xGetFilenameInc (".spc"));
									break;
								case ID_FILE_SAVE_SRAM_DATA:
									Memory.SaveSRAM (S9xGetFilename (".srm"));
									break;
								case ID_FILE_RESET:
#ifdef NETPLAY_SUPPORT
									if (Settings.NetPlayServer)
									{
										S9xNPReset ();
										SetupSound (Settings.SoundPlaybackRate, 
											Settings.SixteenBitSound, 
											Settings.Stereo);
									}
									else
										if (!Settings.NetPlay)
#endif
										{
											S9xMovieStop (TRUE);
											S9xSoftReset ();
											SetupSound (Settings.SoundPlaybackRate, 
												Settings.SixteenBitSound, 
												Settings.Stereo);
										}
										Settings.Paused = false;
										break;
								case ID_FILE_PAUSE:
									Settings.Paused = !Settings.Paused;
									Settings.FrameAdvance = false;
									break;
								case ID_FILE_LOAD1:
									FreezeUnfreeze (0, FALSE);
									break;
								case ID_FILE_LOAD2:
									FreezeUnfreeze (1, FALSE);
									break;
								case ID_FILE_LOAD3:
									FreezeUnfreeze (2, FALSE);
									break;
								case ID_FILE_LOAD4:
									FreezeUnfreeze (3, FALSE);
									break;
								case ID_FILE_LOAD5:
									FreezeUnfreeze (4, FALSE);
									break;
								case ID_FILE_LOAD6:
									FreezeUnfreeze (5, FALSE);
									break;
								case ID_FILE_LOAD7:
									FreezeUnfreeze (6, FALSE);
									break;
								case ID_FILE_LOAD8:
									FreezeUnfreeze (7, FALSE);
									break;
								case ID_FILE_LOAD9:
									FreezeUnfreeze (8, FALSE);
									break;
								case ID_FILE_SAVE1:
									FreezeUnfreeze (0, TRUE);
									break;
								case ID_FILE_SAVE2:
									FreezeUnfreeze (1, TRUE);
									break;
								case ID_FILE_SAVE3:
									FreezeUnfreeze (2, TRUE);
									break;
								case ID_FILE_SAVE4:
									FreezeUnfreeze (3, TRUE);
									break;
								case ID_FILE_SAVE5:
									FreezeUnfreeze (4, TRUE);
									break;
								case ID_FILE_SAVE6:
									FreezeUnfreeze (5, TRUE);
									break;
								case ID_FILE_SAVE7:
									FreezeUnfreeze (6, TRUE);
									break;
								case ID_FILE_SAVE8:
									FreezeUnfreeze (7, TRUE);
									break;
								case ID_FILE_SAVE9:
									FreezeUnfreeze (8, TRUE);
									break;
								case ID_CHEAT_ENTER:
									RestoreGUIDisplay ();
									S9xRemoveCheats ();
									DialogBox(g_hInst, MAKEINTRESOURCE(IDD_CHEATER), hWnd, DlgCheater);
									S9xSaveCheatFile (S9xGetFilename (".cht"));
									S9xApplyCheats ();
									RestoreSNESDisplay ();
									break;
								case ID_CHEAT_SEARCH:
									RestoreGUIDisplay ();
									DialogBox(g_hInst, MAKEINTRESOURCE(IDD_CHEAT_SEARCH), hWnd, DlgCheatSearch);
									S9xSaveCheatFile (S9xGetFilename (".cht"));
									RestoreSNESDisplay ();
									break;
								case ID_CHEAT_APPLY:
									Settings.ApplyCheats = !Settings.ApplyCheats;
									if (!Settings.ApplyCheats)
										S9xRemoveCheats ();
									else
										S9xApplyCheats ();
									break;
								case ID_OPTIONS_SETTINGS:
									RestoreGUIDisplay ();
									DialogBox(g_hInst, MAKEINTRESOURCE(IDD_EMU_SETTINGS), hWnd, DlgEmulatorProc);
									RestoreSNESDisplay ();
									break;
								case ID_HELP_ABOUT:
									RestoreGUIDisplay ();
									DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, DlgAboutProc);
									RestoreSNESDisplay ();
									break;
#ifdef DEBUGGER
								case ID_DEBUG_TRACE:
									{
										Trace ();
										break;
									}
								case ID_DEBUG_FRAME_ADVANCE:
									CPU.Flags |= FRAME_ADVANCE_FLAG;
									ICPU.FrameAdvanceCount = 1;
									Settings.Paused = FALSE;
									break;
#endif
								case IDM_7110_CACHE:
									RestoreGUIDisplay ();
									DialogBox(g_hInst, MAKEINTRESOURCE(IDD_SPC7110_CACHE), hWnd, DlgSP7PackConfig);
									RestoreSNESDisplay ();
									break;
								case IDM_LOG_7110:
									Do7110Logging();
									break;
								case IDM_ROM_INFO:
									RestoreGUIDisplay ();
									DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ROM_INFO), hWnd, DlgInfoProc);
									RestoreSNESDisplay ();
									break;
								default:
									if ((wParam & 0xffff) >= 0xFF00)
									{
										int i = (wParam & 0xffff) - 0xFF00;
										int j = 0;
										if (GUI.RecentGames)
										{
											while (GUI.RecentGames [j] && j != i)
												j++;
											if (i == j)
											{
#ifdef NETPLAY_SUPPORT
												if (Settings.NetPlay && !Settings.NetPlayServer)
												{
													S9xMessage (S9X_INFO, S9X_NETPLAY_NOT_SERVER,
														WINPROC_DISCONNECT);
													break;
												}
#endif
												if (!Settings.StopEmulation)
												{
													Memory.SaveSRAM (S9xGetFilename (".srm"));
													S9xSaveCheatFile (S9xGetFilename (".cht"));
												}
												Settings.StopEmulation = !LoadROM (GUI.RecentGames [i]);
												if (!Settings.StopEmulation) 
												{
													Memory.LoadSRAM (S9xGetFilename (".srm"));
													S9xLoadCheatFile (S9xGetFilename (".cht"));
													S9xAddToRecentGames (GUI.RecentGames [i]);
													CheckDirectoryIsWritable (S9xGetFilename (".---"));
													CheckMenuStates ();
#ifdef NETPLAY_SUPPORT
													if (NPServer.SendROMImageOnConnect)
														S9xNPServerQueueSendingROMImage ();
													else
														S9xNPServerQueueSendingLoadROMRequest (Memory.ROMName);
#endif
													Settings.Paused = false;
												}
											}
										}
									}
									break;
            }
            break;
			
	case WM_EXITMENULOOP:
		UpdateBackBuffer();
		S9xClearPause (PAUSE_MENU);
		break;
		
	case WM_ENTERMENULOOP:
		S9xSetPause (PAUSE_MENU);
#ifdef USE_GLIDE
		S9xGlideEnable (FALSE);
#endif
		CheckMenuStates ();
		
		SwitchToGDI();
		DrawMenuBar( GUI.hWnd);
		break;
		
	case WM_DESTROY:
		Memory.SaveSRAM(S9xGetFilename(".srm"));
		if(CleanUp7110)
			(*CleanUp7110)();
		PostQuitMessage (0);
		return (0);
	case WM_PAINT:
        {
            PAINTSTRUCT paint;
			
            BeginPaint (GUI.hWnd, &paint);
            EndPaint (GUI.hWnd, &paint);
            break;
        }
	case WM_SYSCOMMAND:
        {
            // Prevent screen saver from starting if not paused
			//kode54 says add the ! to fix the screensaver pevention.
            if (!(Settings.ForcedPause || Settings.StopEmulation ||
				Settings.Paused) &&
                (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER))
                return (0);
            break;
        }
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
#ifdef USE_GLIDE
			if (VOODOO_MODE)
			{
				S9xGlideEnable (FALSE);
#if 0
				MoveWindow (GUI.hWnd, GUI.window_size.left,
					GUI.window_size.top, 
					GUI.window_size.right - GUI.window_size.left,
					GUI.window_size.bottom - GUI.window_size.top,
					TRUE);
#endif
			}
#endif
#ifndef NOPAUSE
		S9xSetPause (PAUSE_INACTIVE_WINDOW);
#endif
		}
		else
		{
#ifndef NOPAUSE
			S9xClearPause (PAUSE_INACTIVE_WINDOW);
#endif
			IPPU.ColorsChanged = TRUE;
		}
		break;
	case WM_QUERYNEWPALETTE:
		//            if (!GUI.FullScreen && GUI.ScreenDepth == 8)
		//                RealizePalette (GUI.WindowDC);
		break;
	case WM_SIZE:
		if (wParam == SIZE_RESTORED)
		{
#ifndef NOPAUSE
			S9xClearPause (PAUSE_WINDOW_ICONISED);
#endif
		}
		if (wParam == SIZE_MINIMIZED || wParam == SIZE_MAXHIDE)
		{
#ifdef USE_GLIDE
			S9xGlideEnable (FALSE);
#endif
#ifndef NOPAUSE
			S9xClearPause (PAUSE_WINDOW_ICONISED);
#endif
		}
#ifdef USE_DIRECTX3D
		D3DSizeChange (GUI.hWnd, wParam);
#endif
		break;
	case WM_MOVE:
		if (!VOODOO_MODE && !GUI.FullScreen && !Settings.ForcedPause)
		{
			GetWindowRect (GUI.hWnd, &GUI.window_size);
		}
#ifdef USE_DIRECTX3D            
		D3DMove (GUI.hWnd, lParam);
#endif
		
		break;
	case WM_DISPLAYCHANGE:
		if (!GUI.FullScreen)
		{
			if (!VOODOO_MODE && !OPENGL_MODE &&
				DirectX.SetDisplayMode (GUI.Width, GUI.Height, GUI.Depth,
				!GUI.FullScreen, GUI.DoubleBuffered))
			{
				S9xGraphicsDeinit();
				S9xSetWinPixelFormat ();
				S9xGraphicsInit();
			}
		}
		break;
	case WM_MOUSEMOVE:
		// Lo-word of lparam is xpos, hi-word is ypos
		if (!GUI.IgnoreNextMouseMove)
		{
			POINT p;
			p.x = (int16) (lParam & 0xffff);
			p.y = (int16) ((lParam >> 16) & 0xffff);
			ClientToScreen (GUI.hWnd, &p);
			if ((!Settings.ForcedPause && !Settings.StopEmulation && 
				!Settings.Paused) &&
				(IPPU.Controller == SNES_MOUSE ||
				IPPU.Controller == SNES_MOUSE_SWAPPED))
			{
				POINT middle;
				RECT size;
				
				GetClientRect (GUI.hWnd, &size);
				middle.x = (size.right - size.left) >> 1;
				middle.y = (size.bottom - size.top) >> 1;
				ClientToScreen (GUI.hWnd, &middle);
				GUI.MouseX += p.x - middle.x;
				GUI.MouseY += p.y - middle.y;
				SetCursorPos (middle.x, middle.y);
				GUI.IgnoreNextMouseMove = true;
			}
			else if (IPPU.Controller==SNES_SUPERSCOPE||Settings.Justifier||Settings.SecondJustifier)
			{
				RECT size;
				GetClientRect (GUI.hWnd, &size);
				if(!(GUI.Scale)&&!(GUI.Stretch))
				{
					int x,y, startx, starty;
					x=GET_X_LPARAM(lParam);
					y=GET_Y_LPARAM(lParam);
					
					int theight;
					(IPPU.RenderedScreenHeight> 256)? theight= SNES_HEIGHT_EXTENDED<<1: theight = SNES_HEIGHT_EXTENDED;
					
					startx= size.right-IPPU.RenderedScreenWidth;
					startx/=2;
					starty= size.bottom-theight;
					starty/=2;

					if(x<startx)
						GUI.MouseX=0;
					else if(x>(startx+IPPU.RenderedScreenWidth))
						GUI.MouseX=IPPU.RenderedScreenWidth;
					else GUI.MouseX=x-startx;

					if(y<starty)
						GUI.MouseY=0;
					else if(y>(starty+theight))
						GUI.MouseY=theight;
					else GUI.MouseY=y-starty;
				}
				else if(!(GUI.Stretch)&&!VOODOO_MODE && !OPENGL_MODE)
				{
					int x,y, startx, starty, sizex, sizey;
					x=GET_X_LPARAM(lParam);
					y=GET_Y_LPARAM(lParam);

					if (IPPU.RenderedScreenWidth>256)
						sizex=IPPU.RenderedScreenWidth;
					else sizex=IPPU.RenderedScreenWidth*2;

					if (IPPU.RenderedScreenHeight>256)
						sizey=SNES_HEIGHT_EXTENDED;
					else sizey=SNES_HEIGHT_EXTENDED*2;


					startx= size.right-sizex;
					startx/=2;
					starty= size.bottom-sizey;
					starty/=2;
					if(x<startx)
						GUI.MouseX=0;
					else if(x>(startx+sizex))
						GUI.MouseX=sizex;
					else GUI.MouseX=x-startx;

					if(y<starty)
						GUI.MouseY=0;
					else if(y>(starty+sizey))
						GUI.MouseY=sizey;
					else GUI.MouseY=y-starty;

					GUI.MouseX=(GUI.MouseX*IPPU.RenderedScreenWidth)/sizex;
					GUI.MouseY=(GUI.MouseY*SNES_HEIGHT_EXTENDED)/sizey;

				}
				else
				{
					int theight;
					(IPPU.RenderedScreenHeight> 256)? theight= SNES_HEIGHT_EXTENDED<<1: theight = SNES_HEIGHT_EXTENDED;
					GUI.MouseX=GET_X_LPARAM(lParam)*IPPU.RenderedScreenWidth/size.right;
					GUI.MouseY=GET_Y_LPARAM(lParam)*theight/size.bottom;
				}
			}
			else
			{
				GUI.MouseX = p.x;
				GUI.MouseY = p.y;
			}
		}
		else
			GUI.IgnoreNextMouseMove = false;
		
		S9xMouseOn ();
		return 0;
	case WM_LBUTTONDOWN:
		S9xMouseOn ();
		GUI.MouseButtons |= 1;
		break;
	case WM_LBUTTONUP:
		S9xMouseOn ();
		GUI.MouseButtons &= ~1;
		break;
	case WM_RBUTTONDOWN:
		S9xMouseOn ();
		GUI.MouseButtons |= 2;
		break;
	case WM_RBUTTONUP:
		S9xMouseOn ();
		GUI.MouseButtons &= ~2;
		if(Settings.Justifier||Settings.SecondJustifier)
		{
			RECT size;
			GetClientRect (GUI.hWnd, &size);
			GUI.MouseButtons&=~1;
			GUI.MouseX=(IPPU.RenderedScreenWidth*(lParam & 0xffff))/(size.right-size.left);
			GUI.MouseY=(((lParam >> 16) & 0xffff)*IPPU.RenderedScreenHeight)/(size.bottom-size.top);
		}
		break;
	case WM_MBUTTONDOWN:
		S9xMouseOn ();
		GUI.MouseButtons |= 4;
		break;
	case WM_MBUTTONUP:
		S9xMouseOn ();
		GUI.MouseButtons &= ~4;
		break;
#ifdef NETPLAY_SUPPORT
	case WM_USER + 3:
		NetPlay.Answer = S9xLoadROMImage ((const char *) lParam);
		SetEvent (NetPlay.ReplyEvent);
		break;
	case WM_USER + 2:
		S9xMessage (0, 0, NetPlay.WarningMsg);
		break;
	case WM_USER + 1:
		RestoreGUIDisplay ();
		S9xRestoreWindowTitle ();
		MessageBox (GUI.hWnd, NetPlay.ErrorMsg, 
			TEXT(SNES9X_NP_ERROR), MB_OK | MB_ICONSTOP);
		RestoreSNESDisplay ();
		break;
	case WM_USER:
		if (NetPlay.ActionMsg [0] == 0)
			S9xRestoreWindowTitle ();
		else
		{
			TCHAR buf [NP_MAX_ACTION_LEN + 10];
			
			sprintf (buf, TEXT("%s %3d%%"), NetPlay.ActionMsg, (int) lParam);
			SetWindowText (GUI.hWnd, buf);
		}
#if 0
		if ((int) lParam >= 0)
		{
			RestoreGUIDisplay ();
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NETPLAYPROGRESS), hWnd, DlgNPProgress);
		}
		else
		{
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NETPLAYPROGRESS), hWnd, DlgNPProgress);
			RestoreSNESDisplay ();
		}
#endif
		break;
#endif
    }
    return DefWindowProc (hWnd, uMsg, wParam, lParam);
}

/*****************************************************************************/
/* WinInit                                                                   */
/*****************************************************************************/
BOOL WinInit( HINSTANCE hInstance)
{
    WNDCLASS wndclass;
	
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndclass.lpfnWndProc = WinProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon (NULL, TEXT("IDI_ICON1"));
    wndclass.hCursor = NULL; //LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject (BLACK_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = TEXT("Snes9X: WndClass");
    
    GUI.hInstance = hInstance;
    
    if (!RegisterClass (&wndclass))
        return FALSE;
    
    if ((GUI.hMenu = LoadMenu (hInstance, MAKEINTRESOURCE( Languages[ GUI.Language].idMenu))) == NULL)
        return FALSE;
    
    TCHAR buf [100];
    sprintf (buf, TEXT(WINDOW_TITLE), VERSION);
	
    DWORD dwExStyle;
    DWORD dwStyle;
    RECT rect;
	
    rect.left = rect.top = 0;
    rect.right = MAX_SNES_WIDTH;
    rect.bottom = MAX_SNES_HEIGHT;
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;
	
    AdjustWindowRectEx (&rect, dwStyle, FALSE, dwExStyle);
    if ((GUI.hWnd = CreateWindowEx (
        dwExStyle,
        TEXT("Snes9X: WndClass"),
        buf,
        WS_CLIPSIBLINGS | 
        WS_CLIPCHILDREN |
        dwStyle,
        0, 0,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL,
        GUI.hMenu,
        hInstance,
        NULL)) == NULL)
        return FALSE;
    
    GUI.hDC = GetDC (GUI.hWnd);
	LoadExts();
    GUI.GunSight = LoadCursor (hInstance, MAKEINTRESOURCE (IDC_CURSOR_SCOPE));
    GUI.Arrow = LoadCursor (NULL, IDC_ARROW);
    GUI.Accelerators = LoadAccelerators (hInstance, MAKEINTRESOURCE (IDR_SNES9X_ACCELERATORS));
    Settings.ForcedPause = 0;
    Settings.StopEmulation = TRUE;
    Settings.Paused = FALSE;

	GUI.AVIOut = NULL;

    return TRUE;
}

static inline char *SkipSpaces (char *p)
{
    while (*p && isspace (*p))
        p++;
	
    return (p);
}

void WinParseCommandLine (char *line)
{
    // Break the command line up into an array of string pointers, each pointer
    // points at a separate word or character sequence enclosed in quotes.
	
#define MAX_PARAMETERS 100
    char *p = line;
    static char *parameters [MAX_PARAMETERS];
    int count = 0;
	
    parameters [count++] = "Snes9XW";
	
    while (count < MAX_PARAMETERS && *p)
    {
        p = SkipSpaces (p);
        if (*p == '"')
        {
            p++;
            parameters [count++] = p;
            while (*p && *p != '"')
                p++;
            *p++ = 0;
        }
        else
			if (*p == '\'')
			{
				p++;
				parameters [count++] = p;
				while (*p && *p != '\'')
					p++;
				*p++ = 0;
			}
			else
			{
				parameters [count++] = p;
				while (*p && !isspace (*p))
					p++;
				if (!*p)
					break;
				*p++ = 0;
			}
    }
    rom_filename = S9xParseArgs (parameters, count);
}

void S9xParseArg (char **argv, int &i, int argc)
{
    if (strcasecmp (argv [i], "-restore") == 0)
    {
        WinDeleteRegistryEntries ();
        WinSetDefaultValues ();
	}
	if (strcasecmp (argv[i], "-hidemenu") == 0)
	{
		GUI.HideMenu = true;
	}
}

void S9xExtraUsage ()
{
}

/*****************************************************************************/
/* WinSave/WinLoad - Save/Load the settings to/from the registry             */
/*****************************************************************************/
#define SetKey( n, v)\
RegSetValueEx( hKey, TEXT(n), 0, REG_BINARY, (const unsigned char *) &v, sizeof( v));
#define GetKey( n, v)\
	cbData = sizeof( v);\
RegQueryValueEx( hKey, TEXT(n), 0, NULL, (unsigned char *) &v, &cbData);
#define GetKeyDef( n, v, d)\
	cbData = sizeof( v);\
	if( RegQueryValueEx( hKey, TEXT(n), 0, NULL, (unsigned char *) &v, &cbData) != ERROR_SUCCESS)\
v = d;

void WinDeleteRegKey (HKEY hKeyRoot, const TCHAR *key)
{
    FILETIME last_update;
    DWORD count;
    HKEY hKey;
    TCHAR name_buf [100];
    TCHAR class_buf [200];
    DWORD name_len;
    DWORD class_len;
	
    if (RegOpenKeyEx (hKeyRoot, key, 0, KEY_READ | KEY_WRITE, &hKey) == ERROR_SUCCESS)
    {
        bool8 found = TRUE;
		
        count = 0;
        RegQueryInfoKey (hKey, NULL, NULL, NULL, &count,
			NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        
        for (; found && count != 0; count--)
        {
            name_len = sizeof (name_buf);
            class_len = sizeof (class_buf);
            if (RegEnumKeyEx (hKey, count - 1, name_buf, &name_len, NULL, 
				class_buf, &class_len, &last_update) == ERROR_SUCCESS)
            {
                found = TRUE;
                WinDeleteRegKey (hKey, name_buf);
                RegDeleteKey (hKey, name_buf);
            }
            else
                found = FALSE;
        }
    }
}

static void WinDeleteRegistryEntries ()
{
    WinDeleteRegKey (HKEY_CURRENT_USER, S9X_REG_KEY_BASE);
}

void WinSave( void)
{
    HKEY hKey;
    TCHAR tmp [100];
	
    if (RegCreateKeyEx (HKEY_CURRENT_USER, 
		S9X_REG_KEY_BASE TEXT("\\") S9X_REG_KEY_VERSION, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
		NULL) == ERROR_SUCCESS)
    {
        SetKey( "Language", GUI.Language);
        SetKey( "Playback Rate", Settings.SoundPlaybackRate);
        SetKey( "Stereo Sound", Settings.Stereo);
        SetKey( "16bit Sound", Settings.SixteenBitSound);
        SetKey( "Interpolated Sound", Settings.InterpolatedSound);
        SetKey( "Sync Sound", Settings.SoundSync);
        SetKey( "Sound Envelope Height Reading", Settings.SoundEnvelopeHeightReading);
        SetKey( "Reverse Stereo", Settings.ReverseStereo);
        SetKey( "Disable Echo", Settings.DisableSoundEcho);
        SetKey( "Disable Sample Caching", Settings.DisableSampleCaching);
        SetKey( "Disable Master Volume", Settings.DisableMasterVolume);
        SetKey( "Mute Sound", Settings.Mute);
        SetKey( "Alt Sample Code Method", Settings.AltSampleDecode);
        SetKey( "Enable SPC700", Settings.NextAPUEnabled);
        SetKey( "Sound CPU Skip Method", Settings.SoundSkipMethod);
        SetKey( "Paused Frames Before Muting Sound", GUI.PausedFramesBeforeMutingSound);
        SetKey( "Sound Buffer Length", Settings.SoundBufferSize);
        SetKey( "Sound Mix Interval", Settings.SoundMixInterval);
        SetKey( "Sound Driver", Settings.SoundDriver);
		
        SetKey( "Width", GUI.Width);
        SetKey( "Height", GUI.Height);
        SetKey( "Depth", GUI.Depth);
        SetKey( "Scale", GUI.NextScale);
        SetKey( "DoubleBuffered", GUI.DoubleBuffered);
        SetKey( "Show FPS", Settings.DisplayFrameRate);
		
        SetKey( "Render SixteenBit", Settings.SixteenBit);
        SetKey( "Enable Transparency", Settings.Transparency);
        SetKey( "Support HiRes", Settings.SupportHiRes);
        SetKey( "Skip Frames", Settings.SkipFrames);
        SetKey( "Turbo Skip Frames", Settings.TurboSkipFrames);
        SetKey( "Turbo Mode Toggle", GUI.TurboModeToggle);
        SetKey( "Auto Max Skip Frames", Settings.AutoMaxSkipFrames);
		
        SetKey( "Joypad #1", Joypad[0]);
        SetKey( "Joypad #2", Joypad[1]);
        SetKey( "Joypad #3", Joypad[2]);
        SetKey( "Joypad #4", Joypad[3]);
        SetKey( "Joypad #5", Joypad[4]);
        SetKey( "Window Geometry", GUI.window_size);
        SetKey( "Stretch", GUI.Stretch);
        SetKey( "Fullscreen", GUI.FullScreen);
        RegSetValueEx (hKey, TEXT("Freeze File Directory"), 0, REG_SZ,
			(const unsigned char *) GUI.FreezeFileDir, 
			strlen (GUI.FreezeFileDir) + 1);
        SetKey( "Apply Cheats", Settings.ApplyCheats);
        SetKey( "Auto Save Delay", Settings.AutoSaveDelay);
#ifdef NETPLAY_SUPPORT		
        SetKey( "NetPlay Port", Settings.Port);
        SetKey( "NetPlay Sync By Reset", NPServer.SyncByReset);
        SetKey( "NetPlay Send ROM On Connect", NPServer.SendROMImageOnConnect);
        SetKey( "NetPlay Max Frame Skip", NetPlay.MaxFrameSkip);
        SetKey( "NetPlay Max Behind Frame Count", NetPlay.MaxBehindFrameCount);
#endif
        SetKey( "Bi-Linear Filter Mode7", Settings.Mode7Interpolate);
		SetKey( "Use Hardware Video Memory",GUI.VideoMemory);
		
		RegSetValueEx (hKey, TEXT("StarOcean"), 0, REG_SZ,
			(const unsigned char *) GUI.StarOceanPack, 
			strlen (GUI.StarOceanPack) + 1);
		RegSetValueEx (hKey, TEXT("FarEast"), 0, REG_SZ,
			(const unsigned char *) GUI.FEOEZPack, 
			strlen (GUI.FEOEZPack) + 1);
		RegSetValueEx (hKey, TEXT("ShounenJump"), 0, REG_SZ,
			(const unsigned char *) GUI.SJNSPack, 
			strlen (GUI.SJNSPack) + 1);
		RegSetValueEx (hKey, TEXT("SFA2PAL"), 0, REG_SZ,
			(const unsigned char *) GUI.SFA2PALPack, 
			strlen (GUI.SFA2PALPack) + 1);
		RegSetValueEx (hKey, TEXT("SFA2NTSC"), 0, REG_SZ,
			(const unsigned char *) GUI.SFA2NTSCPack, 
			strlen (GUI.SFA2NTSCPack) + 1);
		RegSetValueEx (hKey, TEXT("SPL4"), 0, REG_SZ,
			(const unsigned char *) GUI.SPL4Pack, 
			strlen (GUI.SPL4Pack) + 1);
		RegSetValueEx (hKey, TEXT("Momotarou"), 0, REG_SZ,
			(const unsigned char *) GUI.MDHPack, 
			strlen (GUI.MDHPack) + 1);
		RegSetValueEx (hKey, TEXT("SFZ2"), 0, REG_SZ,
			(const unsigned char *) GUI.SFZ2Pack, 
			strlen (GUI.SFZ2Pack) + 1);
		
		DWORD mk_temp;
		if(LoadUp7110==&SPC7110Open)
			mk_temp=1;
		else if(LoadUp7110==&SPC7110Grab)
			mk_temp=2;
		else mk_temp=0;
		SetKey( "SPC7110 Cache", mk_temp);
		
		SetKey ( "SPC7110 Size", cacheMegs);
		
        if (GUI.RecentGames)
        {
            int i = 0;
			
            while (GUI.RecentGames [i])
            {
                sprintf (tmp, TEXT("Recent Game #%d"), i + 1);
                RegSetValueEx (hKey, tmp, 0, REG_SZ,
					(const unsigned char *) GUI.RecentGames [i],
					strlen (GUI.RecentGames [i]) + 1);
                i++;
            }
        }
        RegCloseKey( hKey);
    }
}

static void WinSetDefaultValues ()

{
	HKEY hKey;
	char temp[4];
	strcpy(temp,"C:\\");
	if (RegCreateKeyEx (HKEY_CURRENT_USER, 
		S9X_REG_KEY_BASE TEXT("\\") S9X_REG_KEY_VERSION, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
		NULL) == ERROR_SUCCESS)
	RegCloseKey(hKey);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, MY_REG_KEY, 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,TEXT("Last Directory"),NULL,NULL,NULL,NULL)!=ERROR_SUCCESS)
		{
			RegSetValueEx(hKey,TEXT("Last Directory"),NULL,REG_SZ,(uint8*)temp,strlen(temp)+1);
		}
		RegCloseKey(hKey);
	}
	else



    GUI.IgnoreNextMouseMove = false;
    GUI.window_size.left = 0;
    GUI.window_size.right = 264 + SNES_WIDTH;
    GUI.window_size.top = 0;
    GUI.window_size.bottom = 285 + SNES_HEIGHT_EXTENDED;
    GUI.Width = 640;
    GUI.Height = 480;
    GUI.Depth = 16;
    GUI.Scale = 0;
    GUI.NextScale = 0;
    GUI.DoubleBuffered = false;
    GUI.FullScreen = false;
    GUI.Stretch = false;
    GUI.PausedFramesBeforeMutingSound = 20;
    GUI.FlipCounter = 0;
    GUI.NumFlipFrames = 1;
	GUI.VideoMemory = false;
    WinDeleteRecentGamesList ();
	
    // ROM Options
    memset (&Settings, 0, sizeof (Settings));
	
    Settings.ForceLoROM = false;
    Settings.ForceInterleaved = false;
	
    Settings.ForceNotInterleaved = false;
    Settings.ForceInterleaved = false;
    Settings.ForceInterleaved2 = false;
	
    Settings.ForcePAL = false;
    Settings.ForceNTSC = false;
    Settings.ForceHeader = false;
    Settings.ForceNoHeader = false;
	
    // Sound options
    Settings.SoundSync = FALSE;
    Settings.InterpolatedSound = TRUE;
    Settings.SoundEnvelopeHeightReading = TRUE;
    Settings.DisableSoundEcho = FALSE;
    Settings.DisableMasterVolume = FALSE;
    Settings.Mute = FALSE;
    Settings.SoundSkipMethod = 0;
    Settings.SoundPlaybackRate = 32000;
    Settings.SixteenBitSound = TRUE;
    Settings.Stereo = TRUE;
    Settings.AltSampleDecode = FALSE;
    Settings.ReverseStereo = FALSE;
    Settings.SoundDriver = WIN_SNES9X_DIRECT_SOUND_DRIVER;
    Settings.SoundBufferSize = 4;
    Settings.SoundMixInterval = 20;
	Settings.DisableSampleCaching=TRUE;
    
	// Tracing options
    Settings.TraceDMA = false;
    Settings.TraceHDMA = false;
    Settings.TraceVRAM = false;
    Settings.TraceUnknownRegisters = false;
    Settings.TraceDSP = false;
	
    // Joystick options
    Settings.SwapJoypads = false;
    Settings.JoystickEnabled = false;
	
    // ROM timing options (see also H_Max above)
    Settings.PAL = false;
    Settings.FrameTimePAL = 20;
    Settings.FrameTimeNTSC = 17;
    Settings.FrameTime = 17;
	
    // CPU options 
    Settings.CyclesPercentage = 100;
    Settings.Shutdown = true;
    Settings.ShutdownMaster = true;
    Settings.NextAPUEnabled = Settings.APUEnabled = TRUE;
    Settings.DisableIRQ = false;
    Settings.Paused = false;
    Settings.H_Max = SNES_CYCLES_PER_SCANLINE;
    Settings.HBlankStart = (256 * Settings.H_Max) / SNES_HCOUNTER_MAX;
    Settings.SkipFrames = AUTO_FRAMERATE;
	
    // ROM image and peripheral options
    Settings.ForceSuperFX = false;
    Settings.ForceNoSuperFX = false;
    Settings.MultiPlayer5 = false;
    Settings.Mouse = false;
    Settings.SuperScope = false;
    Settings.MultiPlayer5Master = false;
    Settings.SuperScopeMaster = false;
    Settings.MouseMaster = false;
    Settings.SuperFX = false;
	
    // SNES graphics options
    Settings.BGLayering = false;
    Settings.DisableGraphicWindows = false;
    Settings.ForceTransparency = false;
    Settings.ForceNoTransparency = false;
    Settings.DisableHDMA = false;
    Settings.Mode7Interpolate = false;
    Settings.DisplayFrameRate = false;
    Settings.SixteenBit = true;
    Settings.Transparency = true;
    Settings.SupportHiRes = true;
	
    Settings.AutoSaveDelay = 15;
    Settings.ApplyCheats = true;
	
    Settings.TurboMode = false;
    Settings.TurboSkipFrames = 15;
    GUI.TurboModeToggle = true;
    Settings.AutoMaxSkipFrames = 10;

#ifdef NETPLAY_SUPPORT	
    Settings.Port = 1996;
    NetPlay.MaxFrameSkip = 10;
    NetPlay.MaxBehindFrameCount = 10;
    NPServer.SyncByReset = true;
    NPServer.SendROMImageOnConnect = false;
#endif
	
    GUI.FreezeFileDir [0] = 0;
	Settings.SampleCatchup=false;
	Settings.TakeScreenshot=false;
	Settings.StretchScreenshots=1;
	

}

void WinLoad( void)
{
    HKEY hKey;
    unsigned long cbData;
    TCHAR tmp [100];
    char path [_MAX_PATH + 1];
	
    WinSetDefaultValues ();

    if (RegOpenKeyEx (HKEY_CURRENT_USER, 
		S9X_REG_KEY_BASE TEXT("\\") S9X_REG_KEY_VERSION,
		0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
		
        GetKey( "Language", GUI.Language);
        GetKey( "Stereo Sound", Settings.Stereo);
		
        GetKeyDef( "Width", GUI.Width, 640);
        GetKeyDef( "Height", GUI.Height, 480);
        GetKeyDef( "Depth", GUI.Depth, 16);
        GetKeyDef( "Scale", GUI.Scale, 0);
        GUI.NextScale = GUI.Scale;
        GetKeyDef( "DoubleBuffered", GUI.DoubleBuffered, false);
        GetKeyDef( "Show FPS", Settings.DisplayFrameRate, false);
		
        GetKeyDef( "Render SixteenBit", Settings.SixteenBit, true);
        GetKeyDef( "Enable Transparency", Settings.Transparency, true);
        GetKeyDef( "Support HiRes", Settings.SupportHiRes, true);
        GetKeyDef( "Skip Frames", Settings.SkipFrames, AUTO_FRAMERATE);
        GetKeyDef( "Turbo Skip Frames", Settings.TurboSkipFrames, 15);
        GetKeyDef( "Turbo Mode Toggle", GUI.TurboModeToggle, FALSE);
        GetKeyDef( "Auto Max Skip Frames", Settings.AutoMaxSkipFrames, 10);
        GetKeyDef( "Bi-Linear Filter Mode7", Settings.Mode7Interpolate, false);
		
        GetKey( "Joypad #1", Joypad[0]);
        GetKey( "Joypad #2", Joypad[1]);
        GetKey( "Joypad #3", Joypad[2]);
        GetKey( "Joypad #4", Joypad[3]);
        GetKey( "Joypad #5", Joypad[4]);
        GetKey( "Window Geometry", GUI.window_size);
        GetKeyDef( "Stretch", GUI.Stretch, false);
        GetKeyDef( "Fullscreen", GUI.FullScreen, false);
        GetKeyDef( "Playback Rate", Settings.SoundPlaybackRate, 22050);
        GetKeyDef( "Stereo Sound", Settings.Stereo, true);
        GetKeyDef( "16bit Sound", Settings.SixteenBitSound, true);
        GetKeyDef( "Interpolated Sound", Settings.InterpolatedSound, true);
        GetKeyDef( "Sync Sound", Settings.SoundSync, false);
        GetKeyDef( "Sound Envelope Height Reading", Settings.SoundEnvelopeHeightReading, true);
        GetKeyDef( "Reverse Stereo", Settings.ReverseStereo, false);
        GetKeyDef( "Disable Echo", Settings.DisableSoundEcho, false);
        GetKeyDef( "Disable Sample Caching", Settings.DisableSampleCaching, false);
        GetKeyDef( "Disable Master Volume", Settings.DisableMasterVolume, false);
        GetKeyDef( "Mute Sound", Settings.Mute, false);
        GetKeyDef( "Alt Sample Code Method", Settings.AltSampleDecode, false);
        GetKeyDef( "Enable SPC700", Settings.NextAPUEnabled, true);
        Settings.APUEnabled = Settings.NextAPUEnabled;
        GetKeyDef( "Sound CPU Skip Method", Settings.SoundSkipMethod, 0);
        GetKeyDef( "Paused Frames Before Muting Sound", GUI.PausedFramesBeforeMutingSound, 20);
        GetKeyDef( "Sound Buffer Length", Settings.SoundBufferSize, 1);
        GetKeyDef( "Sound Mix Interval", Settings.SoundMixInterval, 10);
        GetKeyDef( "Sound Driver", Settings.SoundDriver, WIN_SNES9X_DIRECT_SOUND_DRIVER);
        GetKeyDef( "Auto Save Delay", Settings.AutoSaveDelay, 30);
        GetKeyDef( "Apply Cheats", Settings.ApplyCheats, true);
#ifdef NETPLAY_SUPPORT        
        GetKeyDef( "NetPlay Port", Settings.Port, 1996);
        GetKeyDef( "NetPlay Sync By Reset", NPServer.SyncByReset, true);
        GetKeyDef( "NetPlay Send ROM On Connect", NPServer.SendROMImageOnConnect, false);
        GetKeyDef( "NetPlay Max Frame Skip", NetPlay.MaxFrameSkip, 10);
        GetKeyDef( "NetPlay Max Behind Frame Count", NetPlay.MaxBehindFrameCount, 10);
#endif	
		GetKeyDef( "Use Hardware Video Memory",GUI.VideoMemory, false);
        cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("Freeze File Directory"), 0, NULL,
			(unsigned char *) GUI.FreezeFileDir, &cbData);
		
        cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("StarOcean"), 0, NULL,
			(unsigned char *) GUI.StarOceanPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("FarEast"), 0, NULL,
			(unsigned char *) GUI.FEOEZPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("SFA2NTSC"), 0, NULL,
			(unsigned char *) GUI.SFA2NTSCPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("SFA2PAL"), 0, NULL,
			(unsigned char *) GUI.SFA2PALPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("Momotarou"), 0, NULL,
			(unsigned char *) GUI.MDHPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("SFZ2"), 0, NULL,
			(unsigned char *) GUI.SFZ2Pack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("ShounenJump"), 0, NULL,
			(unsigned char *) GUI.SJNSPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, TEXT("SPL4"), 0, NULL,
			(unsigned char *) GUI.SPL4Pack, &cbData);
		
		
		DWORD mk_temp;
		GetKeyDef( "SPC7110 Cache", mk_temp, 0);
		switch(mk_temp)
		{
		case 2:
			LoadUp7110=&SPC7110Grab;
			break;
		case 1:
			LoadUp7110=&SPC7110Open;
			break;
		case 0:
		default:LoadUp7110=&SPC7110Load;break;
		}
		GetKeyDef ( "SPC7110 Size", cacheMegs, 5);
        // Count how many recent games filenames are saved in the registry
        int i = -1;
        do
        {
            i++;
            sprintf (tmp, TEXT("Recent Game #%d"), i + 1);
            cbData = _MAX_PATH + 1;
        } while (RegQueryValueEx (hKey, tmp, 0, NULL,
			(unsigned char *) path, &cbData) == ERROR_SUCCESS);
        if (i > 0)
        {
            // Now populate the RecentGames array.
            GUI.RecentGames = new char *[i + 1];
            GUI.RecentGames [i] = NULL;
            for (int j = 0; j < i; j++)
            {
                sprintf (tmp, TEXT("Recent Game #%d"), j + 1);
                cbData = _MAX_PATH + 1;
                RegQueryValueEx (hKey, tmp, 0, NULL,
					(unsigned char *) path, &cbData);
                GUI.RecentGames [j] = strdup (path);
            }
        }
        RegCloseKey( hKey);
    }
	


	
    GUI.ScreenCleared = true;
}

extern long _rate;
extern bool _sixteen_bit;
extern bool _stereo;
extern long _buffernos;
extern long _buffersize;
extern long _samplecount;
extern long _bytecount;
extern unsigned long _interval;

volatile unsigned long LastSound = 0;
uint32 next_frame_time = 0;

VOID CALLBACK SoundTimer (UINT idEvent, UINT uMsg, DWORD dwUser, 
                          DWORD dw1, DWORD dw2)
{
    void ProcessSound (void);
	
    if (Settings.SoundDriver == WIN_SNES9X_DIRECT_SOUND_DRIVER)
        ProcessSound ();
}

VOID CALLBACK FrameTimer( UINT idEvent, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	//    if (DirectX.FrameCount == DirectX.LastFrameCount)
	//        DirectX.IdleCount++;
	//    else
    QueryPerformanceCounter((LARGE_INTEGER*)&PCEnd);
    while ((PCEnd - PCStart) >= PCFrameTime)
	{
		//        DirectX.IdleCount = 0;
		//        DirectX.LastFrameCount = DirectX.FrameCount;
		//    }
        if (DirectX.FrameCount == DirectX.LastFrameCount)
            DirectX.IdleCount++;
        else
        {
            DirectX.IdleCount = 0;
            DirectX.LastFrameCount = DirectX.FrameCount;
        }
		
#ifdef NETPLAY_SUPPORT
		//    if (Settings.NetPlay && !Settings.NetPlayServer)
		//        return;
        if (Settings.NetPlay && !Settings.NetPlayServer)
            return;
#endif
		
#ifdef NETPLAY_SUPPORT
		//-    if (Settings.NetPlayServer)
		//-    {
		//-        if (Settings.Paused || Settings.StopEmulation || Settings.ForcedPause)
        if (Settings.NetPlayServer)
		{
			//-            WaitForSingleObject (GUI.ServerTimerSemaphore, 0);
            if (Settings.Paused || Settings.StopEmulation || Settings.ForcedPause)
            {
                WaitForSingleObject (GUI.ServerTimerSemaphore, 0);
                return;
            }
            ReleaseSemaphore (GUI.ServerTimerSemaphore, 1, NULL);
			
            if (Settings.NetPlay)
                return;
        }
        else
#endif
			if (Settings.SkipFrames != AUTO_FRAMERATE || Settings.TurboMode ||
				(Settings.Paused && !Settings.FrameAdvance) || Settings.StopEmulation || Settings.ForcedPause)
			{
				WaitForSingleObject (GUI.FrameTimerSemaphore, 0);
				PCStart = PCEnd;
				return;
			}
			//        ReleaseSemaphore (GUI.ServerTimerSemaphore, 1, NULL);
			ReleaseSemaphore (GUI.FrameTimerSemaphore, 1, NULL);
			
			//        if (Settings.NetPlay)
			//            return;
			//    }
			//    else
			//#endif
			//    if (Settings.SkipFrames != AUTO_FRAMERATE || Settings.TurboMode ||
			//        Settings.Paused || Settings.StopEmulation || Settings.ForcedPause)
			//    {
			//        WaitForSingleObject (GUI.FrameTimerSemaphore, 0);
			//        return;
			//    }
			//    ReleaseSemaphore (GUI.FrameTimerSemaphore, 1, NULL);
			PCStart += PCFrameTime;
	}
}

/*****************************************************************************/
/* WinMain                                                                   */
/*****************************************************************************/
extern "C" void S9xMainLoop(void);
int Init3d (HWND);
extern "C" void DeinitS9x(void);
int WINAPI WinMain( 
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
    DWORD wSoundTimerRes;
	
    GUI.RecentGames = NULL;
    WinLoad();
    WinParseCommandLine (lpCmdLine);
	
    WinInit (hInstance);
	if(GUI.HideMenu)
	{
		SetMenu (GUI.hWnd, NULL);
	}
	// GUI.hWnd = _MainWindow ();
	//    SetMenu (GUI.hWnd, GUI.hMenu);
    //_InitDLL (GUI.hWnd);
    
/*    if (_GUIVersion () != GUI_VERSION)
    {
	MessageBox (GUI.hWnd, "Incorrect version of Snes9XW.dll dynamic-link library file found.\n\nPlease check you extracted both the Snes9XW.exe and Snes9XW.dll\nfiles into the same directory from the Snes9XW distribution\nzip file.", "Snes9X - Incorrect Snes9XW.dll Version", MB_OK | MB_ICONSTOP);
	return false; 
    }
*/	
    DirectX.InitDirectSound ();
	
    if (!SetupSound (Settings.SoundPlaybackRate, Settings.SixteenBitSound, 
		Settings.Stereo))
    {	
        MessageBox( GUI.hWnd, Languages[ GUI.Language].errInitDS, TEXT("Snes9X - DirectSound"), MB_OK | MB_ICONINFORMATION);
    }
	
#ifdef USE_GLIDE
    if (VOODOO_MODE)
    {
        if (!S9xVoodooInitialise ())
        {
            GUI.Scale = 0;
            GUI.NextScale = 0;
        }
        else
        {
            GUI.FullScreen = FALSE;
            Settings.SixteenBit = TRUE;
        }
    }
#endif
	
#ifdef USE_OPENGL
    if (OPENGL_MODE)
    {
        if (!S9xOpenGLInit ())
        {
            GUI.Scale = 0;
            GUI.NextScale = 0;
        }
    }
#endif
	
    if (!VOODOO_MODE && !OPENGL_MODE && !DirectX.InitDirectX ())
    {
        MessageBox (GUI.hWnd, Languages[ GUI.Language].errInitDD, TEXT("Snes9X - DirectX"), MB_OK | MB_ICONSTOP);
        return false; 
    }
    
    if (!GUI.FullScreen)
    {
        MoveWindow (GUI.hWnd, GUI.window_size.left,
			GUI.window_size.top, 
			GUI.window_size.right - GUI.window_size.left,
			GUI.window_size.bottom - GUI.window_size.top, TRUE);
    }
	
    if (!VOODOO_MODE && !OPENGL_MODE && 
        !DirectX.SetDisplayMode (GUI.Width, GUI.Height, GUI.Depth, 
		!GUI.FullScreen, GUI.DoubleBuffered))
    {
        MessageBox( GUI.hWnd, Languages[ GUI.Language].errModeDD, TEXT("Snes9X - DirectDraw(7)"), MB_OK | MB_ICONSTOP);
        GUI.FullScreen = FALSE;
        if (!DirectX.SetDisplayMode (GUI.Width, GUI.Height, GUI.Depth, 
			!GUI.FullScreen, GUI.DoubleBuffered))
            return (false);
    }
	
    if (!GUI.FullScreen)
    {
        RECT rect;
        GetClientRect (GUI.hWnd, &rect);
        InvalidateRect (GUI.hWnd, &rect, true);
    }
	
    void InitSnes9X( void);
    S9xSetRecentGames ();
    ShowWindow (GUI.hWnd, SW_SHOWNORMAL);
    SetForegroundWindow (GUI.hWnd);
    SetFocus (GUI.hWnd);
	
#ifdef USE_DIRECTX3D
    if (Init3d (GUI.hWnd) != -100)
    {
        MessageBox (GUI.hWnd, TEXT("InitD3D failed."), TEXT("InitD3D Error"), MB_OK | MB_ICONINFORMATION);
        exit (1);
    }
#endif
	
    InitSnes9X ();
	
    TIMECAPS tc;
    if (timeGetDevCaps(&tc, sizeof(TIMECAPS))== TIMERR_NOERROR)
    {
        wSoundTimerRes = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
        timeBeginPeriod (wSoundTimerRes);
    }
	
    QueryPerformanceFrequency((LARGE_INTEGER*)&PCBase);
    QueryPerformanceCounter((LARGE_INTEGER*)&PCStart);
    PCFrameTime = PCFrameTimeNTSC = (__int64)((float)PCBase / 59.948743718592964824120603015098f);
    PCFrameTimePAL = PCBase / 50;
	
	
    Settings.StopEmulation = true;
    GUI.hFrameTimer = timeSetEvent (20, 0, FrameTimer, 0, TIME_PERIODIC);
    GUI.hSoundTimer = timeSetEvent (5, 0, SoundTimer, 0, TIME_PERIODIC);
	
    GUI.FrameTimerSemaphore = CreateSemaphore (NULL, 0, 10, NULL);
    GUI.ServerTimerSemaphore = CreateSemaphore (NULL, 0, 10, NULL);
	
    if (GUI.hFrameTimer == NULL)
    {
        MessageBox( GUI.hWnd, Languages[ GUI.Language].errFrameTimer, TEXT("Snes9X - Frame Timer"), MB_OK | MB_ICONINFORMATION);
    }
	
    Settings.StopEmulation = !rom_filename || !LoadROM (rom_filename);
	
    if (!Settings.StopEmulation)
    {
        Memory.LoadSRAM (S9xGetFilename (".srm"));
        S9xLoadCheatFile (S9xGetFilename (".cht"));
        CheckDirectoryIsWritable (S9xGetFilename (".---"));
        CheckMenuStates ();
    }
	
    if (!Settings.StopEmulation)
    {
        if (IPPU.Controller == SNES_SUPERSCOPE)
            SetCursor (GUI.GunSight);
        else
        {
            SetCursor (GUI.Arrow);
            GUI.CursorTimer = 60;
        }
    }
	
    MSG msg;
	
    while (TRUE)
    {
        // Changed 9/1/2001
        while (Settings.StopEmulation || (Settings.Paused && !Settings.FrameAdvance) ||
			Settings.ForcedPause ||
			PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage (&msg, NULL, 0, 0))
                goto loop_exit;
            
            if (!TranslateAccelerator (GUI.hWnd, GUI.Accelerators, &msg))
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
		
#ifdef NETPLAY_SUPPORT
        if (!Settings.NetPlay || !NetPlay.PendingWait4Sync ||
            WaitForSingleObject (GUI.ClientSemaphore, 100) != WAIT_TIMEOUT)
        {
            if (NetPlay.PendingWait4Sync)
            {
                NetPlay.PendingWait4Sync = FALSE;
                NetPlay.FrameCount++;
                S9xNPStepJoypadHistory ();
            }
#endif      

			// the following is a hack to allow frametimes greater than 100ms,
			// without affecting the responsiveness of the GUI
			BOOL run_loop=false;
			do_frame_adjust=false;
			if (Settings.TurboMode || Settings.FrameAdvance || Settings.SkipFrames != AUTO_FRAMERATE)
			{
				run_loop=true;
			}
			else
			{
				LONG prev;
				BOOL success;
				if ((success = ReleaseSemaphore (GUI.FrameTimerSemaphore, 1, &prev)) &&
					prev == 0)
				{
					WaitForSingleObject (GUI.FrameTimerSemaphore, 0);
					if (WaitForSingleObject (GUI.FrameTimerSemaphore, 100) == WAIT_OBJECT_0)
					{
						run_loop=true;
					}
				}
				else
				{
					if (success)
						WaitForSingleObject (GUI.FrameTimerSemaphore, 0);
					WaitForSingleObject (GUI.FrameTimerSemaphore, 0);

					run_loop=true;
					do_frame_adjust=true;
				}
			}

			if(run_loop)
			{
				S9xMainLoop();
				DirectX.FrameCount++;
			}

			if(Settings.FrameAdvance)
			{
				Settings.FrameAdvance = false;
			}

#ifdef NETPLAY_SUPPORT
        }
#endif
        if (CPU.Flags & DEBUG_MODE_FLAG)
        {
            Settings.Paused = TRUE;
            Settings.FrameAdvance = false;
            CPU.Flags &= ~DEBUG_MODE_FLAG;
        }
        if (GUI.CursorTimer)
        {
            if (--GUI.CursorTimer == 0)
            {
                if (IPPU.Controller != SNES_SUPERSCOPE)
                    SetCursor (NULL);
            }
        }
    }
	
loop_exit:
#ifdef USE_GLIDE
    S9xGlideEnable (FALSE);
#endif
	
    if (GUI.hSoundTimer)
        timeKillEvent (GUI.hSoundTimer);
	
    if( GUI.hFrameTimer)
    {	
        timeKillEvent (GUI.hFrameTimer);
        timeEndPeriod (wSoundTimerRes);
    }
	
    if (!Settings.StopEmulation)
    {
        Memory.SaveSRAM (S9xGetFilename (".srm"));
        S9xSaveCheatFile (S9xGetFilename (".cht"));
    }
    if (!VOODOO_MODE && !GUI.FullScreen)
        GetWindowRect (GUI.hWnd, &GUI.window_size);
	
#ifdef USE_OPENGL
    if (OPENGL_MODE)
        S9xOpenGLDeinit ();
#endif
	
	// this goes here, because the avi
	// recording might have messed with
	// the auto frame skip setting
	// (it needs to come before WinSave)
	DoAVIClose(0);

    WinSave ();
	Memory.Deinit();
#ifdef USE_GLIDE
	if(Settings.GlideEnable)
		S9xGlideDeinit();
	else if (Settings.OpenGLEnable)
#else
		if (Settings.OpenGLEnable)
#endif 
#ifdef USE_OPENGL
			S9xOpenGLDeinit();
#endif
		S9xGraphicsDeinit();
		S9xDeinitAPU();
		WinDeleteRecentGamesList ();
		DeinitS9x();
		
		//uncomment to find memory leaks.
		//	_CrtDumpMemoryLeaks();
		return msg.wParam;
}

void RestoreGUIDisplay ()
{
    S9xSetPause (PAUSE_RESTORE_GUI);
#ifdef USE_GLIDE
    S9xGlideEnable (FALSE);
#endif
    if (!VOODOO_MODE && !OPENGL_MODE && GUI.FullScreen && 
        (GUI.Width < 640 || GUI.Height < 400) &&
        !DirectX.SetDisplayMode (640, 480, 0, !GUI.FullScreen, false))
    {
        MessageBox (GUI.hWnd, Languages[ GUI.Language].errModeDD, TEXT("Snes9X - DirectDraw(1)"), MB_OK | MB_ICONSTOP);
        S9xClearPause (PAUSE_RESTORE_GUI);
        return;
    }
    SwitchToGDI();
    S9xClearPause (PAUSE_RESTORE_GUI);
}

void RestoreSNESDisplay ()
{
    if (!VOODOO_MODE && !OPENGL_MODE && 
        !DirectX.SetDisplayMode (GUI.Width, GUI.Height, GUI.Depth,
		!GUI.FullScreen, GUI.DoubleBuffered))
    {
        MessageBox (GUI.hWnd, Languages[ GUI.Language].errModeDD, TEXT("Snes9X - DirectDraw(4)"), MB_OK | MB_ICONSTOP);
        return;
    }
#ifdef USE_GLIDE
    if (VOODOO_MODE && Glide.voodoo_present)
    {
        //S9xGlideEnable (TRUE);
    }
#endif
    
    UpdateBackBuffer();
}

void FreezeUnfreeze (int slot, bool8 freeze)
{
    static TCHAR filename [_MAX_PATH + 1];
    TCHAR drive [_MAX_DRIVE + 1];
    TCHAR dir [_MAX_DIR + 1];
    TCHAR fname [_MAX_FNAME + 1];
    TCHAR ext [_MAX_EXT + 1];
    
#ifdef NETPLAY_SUPPORT
    if (!freeze && Settings.NetPlay && !Settings.NetPlayServer)
    {
        S9xMessage (S9X_INFO, S9X_NETPLAY_NOT_SERVER,
			"Only the server is allowed to load freeze files.");
        return;
    }
#endif
	
    _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
    sprintf (ext, TEXT(".%03d"), slot);
    if (GUI.FreezeFileDir [0])
    {
        strcpy (filename, GUI.FreezeFileDir);
        strcat (filename, TEXT("\\"));
        strcat (filename, fname);
        strcat (filename, ext);
    }
    else
        _makepath (filename, drive, dir, fname, ext);
    
    if (!freeze && _access (filename, 0) != 0 && slot < 10)
    {
        static char *digits = "t123456789";
        sprintf (ext, TEXT(".zs%c"), digits [slot]);
        if (GUI.FreezeFileDir [0])
        {
            strcpy (filename, GUI.FreezeFileDir);
            strcat (filename, TEXT("\\"));
            strcat (filename, fname);
            strcat (filename, ext);
        }
        else
            _makepath (filename, drive, dir, fname, ext);
    }
	
    S9xSetPause (PAUSE_FREEZE_FILE);
    
    if (freeze)
        S9xFreezeGame (filename);
    else
    {
        if (S9xUnfreezeGame (filename))
        {
//	        S9xMessage (S9X_INFO, S9X_FREEZE_FILE_INFO, S9xBasename (filename));
#ifdef NETPLAY_SUPPORT
            S9xNPServerQueueSendingFreezeFile (filename);
#endif
            UpdateBackBuffer();
        }
    }
    
    S9xClearPause (PAUSE_FREEZE_FILE);
}

void CheckDirectoryIsWritable (const char *filename)
{
    FILE *fs = fopen (filename, "w+");
	
    if (fs == NULL)
	MessageBox (GUI.hWnd, TEXT("The folder where Snes9X saves emulated save RAM files and\ngame save positions (freeze files) is currently set to a\nread-only folder.\n\nIf you do not change the game save folder, Snes9X will be\nunable to save your progress in this game. Change the folder\nfrom the Settings Dialog available from the Options menu.\n\nThe default the save folder is the same as the ROM image\nfolder, if no value is set.\n"),
							 TEXT("Snes9X: Unable to save file warning"), 
							 MB_OK | MB_ICONINFORMATION);
    else
    {
        fclose (fs);
        remove (filename);
    }
}

static void CheckMenuStates ()
{
    MENUITEMINFO mii;
    int i;
	
    ZeroMemory( &mii, sizeof( mii));
    mii.cbSize = sizeof( mii);
    mii.fMask = MIIM_STATE;
    
    mii.fState = GUI.FullScreen ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_WINDOW_FULLSCREEN, FALSE, &mii);
    mii.fState = GUI.Stretch ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_WINDOW_STRETCH, FALSE, &mii);
    mii.fState = Settings.Paused ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_PAUSE, FALSE, &mii);
    
    mii.fState = MFS_UNCHECKED;
    if (Settings.StopEmulation)
        mii.fState |= MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE_SPC_DATA, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE_SRAM_DATA, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE1, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE2, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE3, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE4, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE5, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE6, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE7, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE8, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_SAVE9, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD1, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD2, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD3, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD4, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD5, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD6, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD7, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD8, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_LOAD9, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_PAUSE, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_RESET, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_CHEAT_ENTER, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_CHEAT_SEARCH, FALSE, &mii);

#ifdef NETPLAY_SUPPORT    
    if (Settings.NetPlay && !Settings.NetPlayServer)
        mii.fState = MFS_DISABLED;
    else
        mii.fState = Settings.NetPlayServer ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_SERVER, FALSE, &mii);
	
    mii.fState = Settings.NetPlay && !Settings.NetPlayServer ? 0 : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_DISCONNECT, FALSE, &mii);
	
    mii.fState = Settings.NetPlay || Settings.NetPlayServer ? MFS_DISABLED : 0;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_CONNECT, FALSE, &mii);
	
    mii.fState = NPServer.SendROMImageOnConnect ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_SEND_ROM_ON_CONNECT, FALSE, &mii);
	
    mii.fState = NPServer.SyncByReset ? MFS_CHECKED : MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_SYNC_BY_RESET, FALSE, &mii);
	
    mii.fState = Settings.NetPlayServer ? 0 : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_SYNC, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_NETPLAY_ROM, FALSE, &mii);
#endif
	
    mii.fState = Settings.ApplyCheats ? MFS_CHECKED : MFS_UNCHECKED;
    if (Settings.StopEmulation)
        mii.fState |= MFS_DISABLED;
    SetMenuItemInfo( GUI.hMenu, ID_CHEAT_APPLY, FALSE, &mii);
    
    mii.fState = MFS_UNCHECKED;
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_OPTIONS, FALSE, &mii);
	
    if (!Settings.APUEnabled)
        mii.fState |= MFS_DISABLED;
    
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_NOSOUND, FALSE, &mii);
    for (i = 0; i < COUNT(SoundRates); i++)
        SetMenuItemInfo (GUI.hMenu, SoundRates[i].ident, FALSE, &mii);
    
    if (Settings.SoundPlaybackRate == 0)
        mii.fState |= MFS_DISABLED;
	
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_16BIT, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_STEREO, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_SYNC, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_INTERPOLATED, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_25MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_50MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_100MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_200MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_500MS, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_1S, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_2S, FALSE, &mii);
    
    if (!Settings.Stereo)
        mii.fState |= MFS_DISABLED;
	
    SetMenuItemInfo (GUI.hMenu, ID_SOUND_REVERSE_STEREO, FALSE, &mii);
	
    mii.fState = !Settings.APUEnabled ? MFS_CHECKED | MFS_DISABLED : MFS_CHECKED;
    
    if (!DirectX.DSAvailable || Settings.SoundPlaybackRate == 0 || !Settings.APUEnabled)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_NOSOUND, FALSE, &mii);
    else
    {
        for (i = 0; i < COUNT(SoundRates); i++)
        {
            if (SoundRates [i].rate == Settings.SoundPlaybackRate)
            {
                SetMenuItemInfo (GUI.hMenu, SoundRates[i].ident, FALSE, &mii);
                break;
            }
        }
    }
    if (Settings.SoundPlaybackRate == 0)
        mii.fState |= MFS_DISABLED;
	
    int id;
    switch (Settings.SoundBufferSize)
    {
	case 1:  id = ID_SOUND_25MS; break;
	case 2:  id = ID_SOUND_50MS; break;
	default:
	case 4:  id = ID_SOUND_100MS; break;
	case 8:  id = ID_SOUND_200MS; break;
	case 16: id = ID_SOUND_500MS; break;
	case 32: id = ID_SOUND_1S; break;
	case 64: id = ID_SOUND_2S; break;
    }
    SetMenuItemInfo (GUI.hMenu, id, FALSE, &mii);
    
    if (Settings.SixteenBitSound)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_16BIT, FALSE, &mii);
    if (Settings.Stereo)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_STEREO, FALSE, &mii);
    if (Settings.InterpolatedSound)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_INTERPOLATED, FALSE,&mii);
    if (Settings.SoundSync)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_SYNC, FALSE, &mii);
	
    if (!Settings.Stereo)
        mii.fState |= MFS_DISABLED;
    if (Settings.ReverseStereo)
        SetMenuItemInfo (GUI.hMenu, ID_SOUND_REVERSE_STEREO, FALSE, &mii);
	
#ifndef DEBUGGER
    mii.fState = MFS_DISABLED;
#else
    mii.fState = MFS_UNCHECKED;
#endif
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_TRACE, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_TRACE_SPC, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_TRACE_SA1, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_TRACE_DSP1, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_FRAME_ADVANCE, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_DEBUG_SNES_STATUS, FALSE, &mii);

	mii.fState = (!Settings.StopEmulation) ? MFS_ENABLED : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_MOVIE_PLAY, FALSE, &mii);
    SetMenuItemInfo (GUI.hMenu, ID_FILE_MOVIE_RECORD, FALSE, &mii);

	mii.fState = (S9xMovieActive () && !Settings.StopEmulation) ? MFS_ENABLED : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_MOVIE_STOP, FALSE, &mii);

	mii.fState = (!Settings.StopEmulation) ? MFS_ENABLED : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_WRITE_AVI, FALSE, &mii);

	mii.fState = (!Settings.StopEmulation && (GUI.AVIOut)) ? MFS_ENABLED : MFS_DISABLED;
    SetMenuItemInfo (GUI.hMenu, ID_FILE_STOP_AVI, FALSE, &mii);
}

static void ResetFrameTimer ()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&PCStart);
    if (Settings.FrameTime == Settings.FrameTimeNTSC) PCFrameTime = PCFrameTimeNTSC;
    else if (Settings.FrameTime == Settings.FrameTimePAL) PCFrameTime = PCFrameTimePAL;
    else PCFrameTime = (__int64)((float)(PCBase * Settings.FrameTime) * .001f);
	
	
    if (GUI.hFrameTimer)
        timeKillEvent (GUI.hFrameTimer);
	
    GUI.hFrameTimer = timeSetEvent (Settings.FrameTime, 0, FrameTimer,
		0, TIME_PERIODIC);
}

bool8 LoadROM (const char *filename)
{
    if (Memory.LoadROM (filename))
    {
		S9xMovieStop (TRUE);
	S9xStartCheatSearch(&Cheat);
        SetupSound (Settings.SoundPlaybackRate, 
			Settings.SixteenBitSound, 
			Settings.Stereo);
        ResetFrameTimer ();
        return (TRUE);
    }
    return (FALSE);
}

bool8 S9xLoadROMImage (const TCHAR *string)
{
    RestoreGUIDisplay ();
    TCHAR *buf = new TCHAR [200 + strlen (string)];
    sprintf (buf, TEXT("The NetPlay server is requesting you load the following game:\n '%s'"),
		string);
	
    MessageBox (GUI.hWnd, buf,
		TEXT(SNES9X_INFO),
		MB_OK | MB_ICONINFORMATION);
    
    delete buf;
	
    TCHAR FileName [_MAX_PATH];
	
	INITCOMMONCONTROLSEX icex;
	
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC   = ICC_LISTVIEW_CLASSES|ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	
	if(1<=DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_OPEN_ROM), GUI.hWnd, DlgOpenROMProc, (LPARAM)FileName))
    {
        if (!Settings.StopEmulation)
        {
            Memory.SaveSRAM (S9xGetFilename (".srm"));
            S9xSaveCheatFile (S9xGetFilename (".cht"));
        }
        Settings.StopEmulation = !LoadROM (FileName);
        if (!Settings.StopEmulation) 
        {
            Memory.LoadSRAM( S9xGetFilename (".srm"));
            S9xLoadCheatFile (S9xGetFilename (".cht"));
            CheckDirectoryIsWritable (S9xGetFilename (".---"));
            CheckMenuStates ();
        }
        else
            return (FALSE);
    }
    else
        return (FALSE);
	
    return (TRUE);
}

/*****************************************************************************/
#ifdef NETPLAY_SUPPORT
void EnableServer (bool8 enable)
{
    if (enable != Settings.NetPlayServer)
    {
        if (Settings.NetPlay && !Settings.NetPlayServer)
        {
            Settings.NetPlay = FALSE;
            S9xNPDisconnect ();
        }
		
        if (enable)
        {
            S9xSetPause (PAUSE_NETPLAY_CONNECT);
            Settings.NetPlayServer = S9xNPStartServer (Settings.Port);
            Sleep (1000);
            
            if (!S9xNPConnectToServer ("127.0.0.1", Settings.Port,
				Memory.ROMName))
            {
                S9xClearPause (PAUSE_NETPLAY_CONNECT);
            }
        }
        else
        {
            Settings.NetPlayServer = FALSE;
            S9xNPStopServer ();
        }
    }
}
#endif
#ifdef USE_OPENGL
bool8 S9xOpenGLInit ()
{
    int PixelFormat;
	
    if (GUI.FullScreen)
    {
        DEVMODE dmScreenSettings;
		
        memset (&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = GUI.Width;
        dmScreenSettings.dmPelsHeight = GUI.Height;
        dmScreenSettings.dmBitsPerPel = 16; //bits;
        dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		
        if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            GUI.FullScreen = FALSE;
        }
    }
    if (GUI.FullScreen)
    {
        SetWindowLong (GUI.hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos (GUI.hWnd, HWND_TOP, 0, 0, GUI.Width,
			GUI.Height,
			SWP_DRAWFRAME|SWP_FRAMECHANGED);
    }
    else
    {
        SetWindowLong (GUI.hWnd, GWL_STYLE, WS_POPUPWINDOW|WS_CAPTION|
			WS_THICKFRAME|WS_VISIBLE|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
        SetWindowPos (GUI.hWnd, HWND_TOP, 
			GUI.window_size.left,
			GUI.window_size.top,
			GUI.window_size.right - GUI.window_size.left,
			GUI.window_size.bottom - GUI.window_size.top,
			SWP_DRAWFRAME|SWP_FRAMECHANGED);
    }
    
    static PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			16, //bits,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
    };
	
    if (!(PixelFormat = ChoosePixelFormat (GUI.hDC, &pfd)))
    {
        MessageBox(NULL,TEXT("Can't Find A Suitable PixelFormat."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }
    
    if (!SetPixelFormat (GUI.hDC, PixelFormat, &pfd))
    {
        MessageBox(NULL,TEXT("Can't Set The PixelFormat."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return (FALSE);
    }
    
    if (!(GUI.hRC = wglCreateContext (GUI.hDC)))
    {
        MessageBox(NULL,TEXT("Can't Create A GL Rendering Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return (FALSE);
    }
    
    if (!wglMakeCurrent (GUI.hDC, GUI.hRC))
    {
        MessageBox(NULL,TEXT("Can't Activate The GL Rendering Context."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
        return (FALSE);
    }
    
    glGetIntegerv (GL_MAX_TEXTURE_SIZE, &OpenGL.max_texture_size);
	
    if (OpenGL.max_texture_size >= 512)
    {
        OpenGL.texture_size = 512;
        OpenGL.num_textures = 2;
    }
    else
    {
        OpenGL.texture_size = OpenGL.max_texture_size;
        OpenGL.num_textures = 1;
    }
	
    Settings.OpenGLEnable = TRUE;
	
    const char *ext = (const char *) glGetString (GL_EXTENSIONS);
	
    if (ext && strstr (ext, "EXT_packed_pixels") != NULL)
        OpenGL.packed_pixels_extension_present = TRUE;
	
    const char *version = (const char *) glGetString (GL_VERSION);
	
    if (version && strlen (version) < 100)
    {
		char ver [100];
		strcpy (ver,  version);
		
		// Strip dots from the version string
		char *ptr;
		while (ptr = strchr (ver, '.'))
			memmove (ptr, ptr + 1, strlen (ptr + 1) + 1);
		
		// Pad the string with zeros to 4 digits
		while (strlen (ver) < 4)
			strcat (ver, "0");
		
		OpenGL.version = atoi (ver);
    }
    else
		OpenGL.version = 1100;
	
#ifndef GL_UNSIGNED_SHORT_5_5_5_1_EXT
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT     0x8034
#endif
	
    if (OpenGL.version >= 1200)
    {
        OpenGL.internal_format = GL_RGB5_A1;
        OpenGL.format = GL_RGBA;
        OpenGL.type = GL_UNSIGNED_SHORT_5_5_5_1_EXT;
    }
    else
		if (OpenGL.packed_pixels_extension_present)
		{
			OpenGL.internal_format = GL_RGB5_A1;
			OpenGL.format = GL_RGBA;
			OpenGL.type = GL_UNSIGNED_SHORT_5_5_5_1_EXT;
		}
		else
		{
			OpenGL.internal_format = GL_RGB;
			OpenGL.format = GL_RGB;
			OpenGL.type = GL_UNSIGNED_BYTE;
		}
		
		glGenTextures (OpenGL.num_textures, OpenGL.textures);
		
		if (OpenGL.num_textures == 2)
		{
			glBindTexture (GL_TEXTURE_2D, OpenGL.textures [1]);
			glTexImage2D (GL_TEXTURE_2D, 0, OpenGL.internal_format, 256, 256, 0,
				OpenGL.format, OpenGL.type, NULL);
			
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
		
		glBindTexture (GL_TEXTURE_2D, OpenGL.textures [0]);
		glTexImage2D (GL_TEXTURE_2D, 0, OpenGL.internal_format, 
			OpenGL.texture_size, OpenGL.texture_size, 0, 
			OpenGL.format, OpenGL.type, NULL);
		
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		glPolygonMode (GL_FRONT, GL_FILL);
		glEnable (GL_CULL_FACE);
		glCullFace (GL_BACK);
		
		glEnable (GL_DITHER);
		glEnable (GL_LIGHTING);
		glEnable (GL_LIGHT0);
		
		glEnable (GL_POINT_SMOOTH);
		glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
		
		glEnable (GL_TEXTURE_2D);
		
		glShadeModel (GL_SMOOTH);
		glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth (1.0f);
		glDisable (GL_DEPTH_TEST);
		glDepthFunc (GL_LESS);
		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
		GLfloat LightAmbient[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat LightDiffuse[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightSpecular[]   = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat LightPosition[]   = { 0.0f, 0.0f, 2.0f, 1.0f };
		
		glLightfv (GL_LIGHT0, GL_AMBIENT, LightAmbient);
		glLightfv (GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
		glLightfv (GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glLightfv (GL_LIGHT0, GL_POSITION, LightPosition);
		
		//Set common material properties
		GLfloat MatSpecular[]    = { 1.0f, 1.0f, 1.0f, 1.1f };
		GLfloat WhMat[]          = { 1.0f, 1.0f, 1.0f, 1.0f };
		
		glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
		glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
		glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, WhMat);
		
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glMatrixMode (GL_MODELVIEW);
		
	    glDisable (GL_DEPTH_TEST);
	    glDisable (GL_LIGHTING);
		glEnable (GL_TEXTURE_2D);
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glDisable (GL_BLEND);

		return (TRUE);
}

void S9xOpenGLDeinit ()
{
    if (GUI.FullScreen)
        ChangeDisplaySettings (NULL, 0);
	
    if (GUI.hRC)
    {
        wglMakeCurrent (NULL, NULL);
        wglDeleteContext (GUI.hRC);
        GUI.hRC = NULL;
    }
}

void S9xOpenGLResize (int width, int height)
{
    if (height == 0)
        height = 1;
	
    glViewport (0, 0, width, height);
	
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
	
    gluPerspective (45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode (GL_MODELVIEW);
}
#endif

void S9xAddToRecentGames (const char *filename)
{
    if (!GUI.RecentGames)
    {
        GUI.RecentGames = new char *[2];
        GUI.RecentGames [0] = strdup (filename);
        GUI.RecentGames [1] = NULL;
    }
    else
    {
        int i = -1;
		
        // Make sure its not in the list already
        while (GUI.RecentGames [++i])
            if (strcmp (filename, GUI.RecentGames [i]) == 0)
                break;
			
			if (GUI.RecentGames [i])
			{
				// It is in the list, move it to the head of the list.
				char *t = GUI.RecentGames [i];
				for (int j = i; j > 0; j--)
					GUI.RecentGames [j] = GUI.RecentGames [j - 1];
				
				GUI.RecentGames [0] = t;
			}
			else
			{
				// Not in the list, add it.
				if (i < MAX_RECENT_GAMES_LIST_SIZE)
				{
					// Extend the recent game list length by 1.
					char **t = new char *[i + 2];
					
					memmove (&t [1], GUI.RecentGames, sizeof (char *) * (i + 1));
					delete GUI.RecentGames;
					GUI.RecentGames = t;
					GUI.RecentGames [0] = strdup (filename);
				}
				else
				{
					// Throw the last item off the end of the list
					free (GUI.RecentGames [i - 1]);
					memmove (&GUI.RecentGames [1], &GUI.RecentGames [0],
						sizeof (char *) * (MAX_RECENT_GAMES_LIST_SIZE - 1));
					GUI.RecentGames [0] = strdup (filename);
				}
			}
    }
    S9xSetRecentGames ();
}

void S9xSetRecentGames ()
{
    HMENU file = GetSubMenu (GUI.hMenu, 0);
    if (file)
    {
        HMENU recent = GetSubMenu (file, 1);
        if (recent)
        {
            MENUITEMINFO mii;
            TCHAR name [256 + 10];
            int i;
			
            // Clear out the menu first
            for (i = GetMenuItemCount (recent) - 1; i >= 0; i--)
                RemoveMenu (recent, i, MF_BYPOSITION);
			
            mii.cbSize = sizeof (mii);
            mii.fMask = MIIM_TYPE | MIIM_DATA | MIIM_STATE | MIIM_ID;
            mii.fType = MFT_STRING;
            mii.fState = MFS_UNCHECKED;
			
            for (i = 0; i < 10 && GUI.RecentGames && GUI.RecentGames [i]; i++)
            {
                // Build up a menu item string in the form:
                // 1. <basename of ROM image name>
                sprintf (name, TEXT("&%c. %s"), i < 9 ? '1' + i : 'A' + i - 9,
					S9xBasename (GUI.RecentGames [i]));
                mii.dwTypeData = name;
                mii.cch = strlen (name) + 1;
                mii.wID = 0xFF00 + i;
				
                InsertMenuItem (recent, 0xFF00 + i, FALSE, &mii);
            }
        }
    }
}

static void WinDeleteRecentGamesList ()
{
    if (GUI.RecentGames)
    {
        int i = 0;
		
        while (GUI.RecentGames [i])
		{
            free (GUI.RecentGames [i]);
			GUI.RecentGames[i]=NULL;
			i++;
		}
		
        delete GUI.RecentGames;
        GUI.RecentGames = NULL;
    }
}

int CALLBACK DlgSoundConf(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	static struct SSettings* set;
	switch(msg)
	{
	case WM_INITDIALOG:
		hBmp=(HBITMAP)LoadImage(NULL, TEXT("Jerremy.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		set= (struct SSettings *)lParam;
#ifndef MK_APU
		SetDlgItemText(hDlg,IDC_LINEAR_INTER, "Linear &Interpolation of Sample Data");
#endif
		SendDlgItemMessage(hDlg, IDC_DRIVER, CB_INSERTSTRING,0,(LPARAM)TEXT("Snes9x DirectSound"));
#ifdef FMOD_SUPPORT
		SendDlgItemMessage(hDlg, IDC_DRIVER, CB_INSERTSTRING,1,(LPARAM)TEXT("FMOD DirectSound"));
		SendDlgItemMessage(hDlg, IDC_DRIVER, CB_INSERTSTRING,2,(LPARAM)TEXT("FMOD Windows Multimedia"));
		SendDlgItemMessage(hDlg, IDC_DRIVER, CB_INSERTSTRING,3,(LPARAM)TEXT("FMOD A3D"));
#endif
		SendDlgItemMessage(hDlg, IDC_DRIVER,CB_SETCURSEL,set->SoundDriver,0);
		
		SendDlgItemMessage(hDlg, IDC_SKIP_TYPE, CB_INSERTSTRING,0,(LPARAM)TEXT("Skip style #1"));
		SendDlgItemMessage(hDlg, IDC_SKIP_TYPE, CB_INSERTSTRING,1,(LPARAM)TEXT("Skip style #2"));
		SendDlgItemMessage(hDlg, IDC_SKIP_TYPE, CB_INSERTSTRING,2,(LPARAM)TEXT("Skip style #3"));
		SendDlgItemMessage(hDlg, IDC_SKIP_TYPE, CB_INSERTSTRING,3,(LPARAM)TEXT("Skip style #4"));
		SendDlgItemMessage(hDlg, IDC_SKIP_TYPE, CB_INSERTSTRING,4,(LPARAM)TEXT("Skip style #5"));
		
		SendDlgItemMessage(hDlg,IDC_SKIP_TYPE,CB_SETCURSEL,set->SoundSkipMethod,0);
		
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,0,(LPARAM)TEXT("<No Sound>"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,1,(LPARAM)TEXT("8 KHz"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,2,(LPARAM)TEXT("11 KHz"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,3,(LPARAM)TEXT("16 KHz"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,4,(LPARAM)TEXT("22 KHz"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,5,(LPARAM)TEXT("30 KHz"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,6,(LPARAM)TEXT("32 KHz"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,7,(LPARAM)TEXT("35 KHz"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,8,(LPARAM)TEXT("44 KHz"));
		SendDlgItemMessage(hDlg, IDC_RATE, CB_INSERTSTRING,9,(LPARAM)TEXT("48 KHz"));
		
		int temp;
		switch(set->SoundPlaybackRate)
		{
		case 8000:temp=1;break;
		case 11025:temp=2;break;
		case 16000:temp=3;break;
		case 22050:temp=4;break;
		case 30000:temp=5;break;
		case 32000:temp=6;break;
		case 35000:temp=7;break;
		case 44000:
		case 44100:temp=8;break;
		case 48000:temp=9;break;
		case 0:
		default:temp=0;break;
		}
		SendDlgItemMessage(hDlg,IDC_RATE,CB_SETCURSEL,temp,0);
		
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,0,(LPARAM)TEXT("10 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,1,(LPARAM)TEXT("20 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,2,(LPARAM)TEXT("30 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,3,(LPARAM)TEXT("40 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,4,(LPARAM)TEXT("50 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,5,(LPARAM)TEXT("60 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,6,(LPARAM)TEXT("70 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,7,(LPARAM)TEXT("80 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,8,(LPARAM)TEXT("90 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,9,(LPARAM)TEXT("100 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,10,(LPARAM)TEXT("110 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,11,(LPARAM)TEXT("120 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,12,(LPARAM)TEXT("130 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,13,(LPARAM)TEXT("140 ms"));
		SendDlgItemMessage(hDlg, IDC_MIX, CB_INSERTSTRING,14,(LPARAM)TEXT("150 ms"));
		
		SendDlgItemMessage(hDlg,IDC_MIX,CB_SETCURSEL,((set->SoundMixInterval/10)-1),0);
		
		SendDlgItemMessage(hDlg, IDC_BUFLEN, CB_INSERTSTRING,0,(LPARAM)TEXT("10 ms"));
		SendDlgItemMessage(hDlg, IDC_BUFLEN, CB_INSERTSTRING,1,(LPARAM)TEXT("20 ms"));
		SendDlgItemMessage(hDlg, IDC_BUFLEN, CB_INSERTSTRING,2,(LPARAM)TEXT("40 ms"));
		SendDlgItemMessage(hDlg, IDC_BUFLEN, CB_INSERTSTRING,3,(LPARAM)TEXT("80 ms"));
		SendDlgItemMessage(hDlg, IDC_BUFLEN, CB_INSERTSTRING,4,(LPARAM)TEXT("160 ms"));
		SendDlgItemMessage(hDlg, IDC_BUFLEN, CB_INSERTSTRING,5,(LPARAM)TEXT("320 ms"));
		SendDlgItemMessage(hDlg, IDC_BUFLEN, CB_INSERTSTRING,6,(LPARAM)TEXT("640 ms"));
		
		switch(set->SoundBufferSize)
		{
		case 2:temp=1; break;
		case 4:temp=2; break;
		case 8:temp=3; break;
		case 16:temp=4; break;
		case 32:temp=5; break;
		case 64: temp=6;break;
		case 1:
		default:temp=0;break;
		}
		SendDlgItemMessage(hDlg,IDC_BUFLEN,CB_SETCURSEL,temp,0);
		
		if(set->SixteenBitSound)
			SendDlgItemMessage(hDlg,IDC_16BIT,BM_SETCHECK,BST_CHECKED,0);
		if(set->Stereo)
			SendDlgItemMessage(hDlg,IDC_STEREO,BM_SETCHECK,BST_CHECKED,0);
		else EnableWindow(GetDlgItem(hDlg, IDC_REV_STEREO), FALSE);
		if(set->ReverseStereo)
			SendDlgItemMessage(hDlg,IDC_REV_STEREO,BM_SETCHECK,BST_CHECKED,0);
		if(set->AltSampleDecode)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CACHING), FALSE);
			SendDlgItemMessage(hDlg,IDC_ANTIRES,BM_SETCHECK,BST_CHECKED,0);
		}
		if(set->Mute)
			SendDlgItemMessage(hDlg,IDC_MUTE,BM_SETCHECK,BST_CHECKED,0);
		
		if(set->NextAPUEnabled)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_SKIP_TYPE), FALSE);
			SendDlgItemMessage(hDlg,IDC_SPC700ON,BM_SETCHECK,BST_CHECKED,0);
			//Gray out the skip feature!
		}
		if(set->InterpolatedSound)
			SendDlgItemMessage(hDlg,IDC_LINEAR_INTER,BM_SETCHECK,BST_CHECKED,0);
		if(set->SoundSync)
			SendDlgItemMessage(hDlg,IDC_SYNC_TO_SOUND_CPU,BM_SETCHECK,BST_CHECKED,0);
		if(set->SoundEnvelopeHeightReading)
			SendDlgItemMessage(hDlg,IDC_ENVX,BM_SETCHECK,BST_CHECKED,0);
		if(!set->DisableSoundEcho)
			SendDlgItemMessage(hDlg,IDC_ECHO,BM_SETCHECK,BST_CHECKED,0);
		
//		if(!set->DisableSampleCaching)
//			SendDlgItemMessage(hDlg,IDC_CACHING,BM_SETCHECK,BST_CHECKED,0);
		if(!set->DisableMasterVolume)
			SendDlgItemMessage(hDlg,IDC_MASTER_VOL,BM_SETCHECK,BST_CHECKED,0);
		
		if(set->SoundDriver)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_MIX), false);
			EnableWindow(GetDlgItem(hDlg, IDC_BUFLEN), false);
			EnableWindow(GetDlgItem(hDlg, IDC_SYNC_TO_SOUND_CPU), false);
		}
		
		if(!IsDlgButtonChecked(hDlg,IDC_SPC700ON))
		{
			EnableWindow(GetDlgItem(hDlg, IDC_CACHING), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_MUTE), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_ANTIRES), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_SKIP_TYPE), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_DRIVER), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_16BIT), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_ENVX), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_STEREO), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_MASTER_VOL), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_RATE), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_ECHO), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_LINEAR_INTER), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_REV_STEREO), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_BUFLEN), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_MIX), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_SYNC_TO_SOUND_CPU), FALSE);
			
		}
		
		return true;
		case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		};

		EndPaint (hDlg, &ps);
		}
		return true;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDOK:
				
				set->SoundDriver=SendDlgItemMessage(hDlg, IDC_DRIVER,CB_GETCURSEL,0,0);
				set->SoundSkipMethod=(unsigned char)SendDlgItemMessage(hDlg,IDC_SKIP_TYPE,CB_GETCURSEL,0,0);
				set->SixteenBitSound=IsDlgButtonChecked(hDlg, IDC_16BIT);
				set->AltSampleDecode=IsDlgButtonChecked(hDlg, IDC_ANTIRES);
				set->SoundSync=IsDlgButtonChecked(hDlg, IDC_SYNC_TO_SOUND_CPU);
				set->InterpolatedSound=IsDlgButtonChecked(hDlg, IDC_LINEAR_INTER);
				set->Stereo=IsDlgButtonChecked(hDlg, IDC_STEREO);
				set->ReverseStereo=IsDlgButtonChecked(hDlg, IDC_REV_STEREO);
				set->Mute=IsDlgButtonChecked(hDlg, IDC_MUTE);
				set->NextAPUEnabled=IsDlgButtonChecked(hDlg, IDC_SPC700ON);
				set->SoundEnvelopeHeightReading=IsDlgButtonChecked(hDlg, IDC_ENVX);
				
				set->DisableSoundEcho=(!IsDlgButtonChecked(hDlg, IDC_ECHO));
				//set->DisableSampleCaching=(!IsDlgButtonChecked(hDlg, IDC_CACHING));
				set->DisableMasterVolume=(!IsDlgButtonChecked(hDlg, IDC_MASTER_VOL));
				
				
				switch(SendDlgItemMessage(hDlg, IDC_RATE,CB_GETCURSEL,0,0))
				{
				case 0: set->SoundPlaybackRate=0;		break;
				case 1: set->SoundPlaybackRate=8000;	break;
				case 2: set->SoundPlaybackRate=11025;	break;
				case 3: set->SoundPlaybackRate=16000;	break;
				case 4: set->SoundPlaybackRate=22050;	break;
				case 5: set->SoundPlaybackRate=30000;	break;
				case 6: set->SoundPlaybackRate=32000;	break;
				case 7: set->SoundPlaybackRate=35000;	break;
				case 8: set->SoundPlaybackRate=44100;	break;
				case 9: set->SoundPlaybackRate=48000;	break;
				}
				set->SoundMixInterval=(10*(1+(SendDlgItemMessage(hDlg,IDC_MIX,CB_GETCURSEL,0,0))));
				
				set->SoundBufferSize=1<<SendDlgItemMessage(hDlg,IDC_BUFLEN,CB_GETCURSEL,0,0);
				case IDCANCEL:
					EndDialog(hDlg, 1);
					if(hBmp)
					{
						DeleteObject(hBmp);
						hBmp=NULL;
					}
					return true;
					
				case IDC_SPC700ON:
					if(BN_CLICKED==HIWORD(wParam)||BN_DBLCLK==HIWORD(wParam))
					{
						if(!IsDlgButtonChecked(hDlg,IDC_SPC700ON))
						{
							EnableWindow(GetDlgItem(hDlg, IDC_CACHING), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_MUTE), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_ANTIRES), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_SKIP_TYPE), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_DRIVER), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_16BIT), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_ENVX), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_STEREO), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_MASTER_VOL), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_RATE), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_ECHO), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_LINEAR_INTER), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_REV_STEREO), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_BUFLEN), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_MIX), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_SYNC_TO_SOUND_CPU), FALSE);
							
						}
						else
						{
							EnableWindow(GetDlgItem(hDlg, IDC_LINEAR_INTER), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_ECHO), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_RATE), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_MASTER_VOL), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_STEREO), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_DRIVER), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_16BIT), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_ENVX), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_SKIP_TYPE), FALSE);
							EnableWindow(GetDlgItem(hDlg, IDC_ANTIRES), TRUE);
							EnableWindow(GetDlgItem(hDlg, IDC_MUTE), TRUE);
							if(!IsDlgButtonChecked(hDlg,IDC_ANTIRES))
								EnableWindow(GetDlgItem(hDlg, IDC_CACHING), TRUE);
							if(IsDlgButtonChecked(hDlg,IDC_STEREO))
								EnableWindow(GetDlgItem(hDlg, IDC_REV_STEREO), TRUE);
							if(0==SendDlgItemMessage(hDlg, IDC_DRIVER, CB_GETCURSEL, 0,0))
							{
								//enable stuff.
								EnableWindow(GetDlgItem(hDlg, IDC_MIX), true);
								EnableWindow(GetDlgItem(hDlg, IDC_BUFLEN), true);
								EnableWindow(GetDlgItem(hDlg, IDC_SYNC_TO_SOUND_CPU), true);
								
							}
							return true;
							//Check Driver States for other options.
						}
						return true;
					}
					else return false;
				case IDC_DRIVER:
					if(CBN_SELCHANGE==HIWORD(wParam))
					{
						//get index
						int i;
						i=SendDlgItemMessage(hDlg, IDC_DRIVER, CB_GETCURSEL, 0,0);
						if(i)
						{
							EnableWindow(GetDlgItem(hDlg, IDC_MIX), false);
							EnableWindow(GetDlgItem(hDlg, IDC_BUFLEN), false);
							EnableWindow(GetDlgItem(hDlg, IDC_SYNC_TO_SOUND_CPU), false);
						}
						//disable if index...
						else
						{
							//enable stuff.
							EnableWindow(GetDlgItem(hDlg, IDC_MIX), true);
							EnableWindow(GetDlgItem(hDlg, IDC_BUFLEN), true);
							EnableWindow(GetDlgItem(hDlg, IDC_SYNC_TO_SOUND_CPU), true);
							
						}
						return true;
					}
					else return false;
				case IDC_ANTIRES:
					{
						if(BN_CLICKED==HIWORD(wParam)||BN_DBLCLK==HIWORD(wParam))
						{
							if(!IsDlgButtonChecked(hDlg,IDC_ANTIRES))
							{
							//	EnableWindow(GetDlgItem(hDlg, IDC_CACHING), TRUE);
							}
							else EnableWindow(GetDlgItem(hDlg, IDC_CACHING), FALSE);
							return true;
							
						}
						else return false;
					}
				case IDC_STEREO:
					{
						if(BN_CLICKED==HIWORD(wParam)||BN_DBLCLK==HIWORD(wParam))
						{
							if(IsDlgButtonChecked(hDlg,IDC_STEREO))
							{
								EnableWindow(GetDlgItem(hDlg, IDC_REV_STEREO), TRUE);
							}
							else EnableWindow(GetDlgItem(hDlg, IDC_REV_STEREO), FALSE);
							return true;
							
						}
						else return false;
					}
				default: return false;
					
					
		}
	}
	return false;
}

int CALLBACK DlgSP7PackConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("Gogo.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

			if(LoadUp7110==&SPC7110Grab)
				SendDlgItemMessage(hDlg, IDC_SPC7110_SOME, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			else if(LoadUp7110==&SPC7110Open)
				SendDlgItemMessage(hDlg, IDC_SPC7110_FILE, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			else	SendDlgItemMessage(hDlg, IDC_SPC7110_ALL, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			SendDlgItemMessage(hDlg, IDC_SPIN_CACHE, UDM_SETRANGE, 0, MAKELONG(20,1));
			SendDlgItemMessage(hDlg, IDC_SPIN_CACHE, UDM_SETPOS, 0, MAKELONG(cacheMegs,0));
			return true;
		}
	case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
				if(BST_CHECKED==SendDlgItemMessage(hDlg, IDC_SPC7110_ALL, BM_GETCHECK,0,0))
				{
					LoadUp7110=&SPC7110Load;
				}
				else if(BST_CHECKED==SendDlgItemMessage(hDlg, IDC_SPC7110_FILE, BM_GETCHECK,0,0))
				{
					LoadUp7110=&SPC7110Open;
				}
				else
				{
					LoadUp7110=&SPC7110Grab;
					cacheMegs=(uint16)SendDlgItemMessage(hDlg,IDC_SPIN_CACHE,UDM_GETPOS,0,0);
				}
			case IDCANCEL:
				EndDialog(hDlg,0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
			default: return false;
			}
		}
	default: return false;
	}
}

#ifdef RTC_DEBUGGER
int CALLBACK SPC7110rtc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int month;
	static int day;
	static int year;
	static int hour;
	static int minutes;
	static int seconds;
	static int dayinmonth;
	static struct SPC7110RTC* rtc;
	switch(msg)
	{
	case WM_INITDIALOG:
		
		rtc= (struct SPC7110RTC *)lParam;
		
		seconds=rtc->reg[0]+rtc->reg[1]*10;
		minutes=rtc->reg[2]+rtc->reg[3]*10;
		hour=rtc->reg[4]+rtc->reg[5]*10;
		day=rtc->reg[6]+rtc->reg[7]*10;
		month=rtc->reg[8]+rtc->reg[9]*10;
		year=rtc->reg[10]+rtc->reg[11]*10;
		dayinmonth=S9xRTCDaysInMonth(month,year);
		
		
		SendDlgItemMessage(hDlg,IDC_MONTH, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,11));
		SendDlgItemMessage(hDlg,IDC_MONTH, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_MONTH, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)month-1);
		SetDlgItemInt(hDlg,IDC_LBLMONTH,month,0);
		
		SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,dayinmonth-1));
		SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)day-1);
		SetDlgItemInt(hDlg,IDC_LBLDAY,day,0);
		
		SendDlgItemMessage(hDlg,IDC_YEAR, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,99));
		SendDlgItemMessage(hDlg,IDC_YEAR, TBM_SETTICFREQ,(WPARAM)5,0);
		if(year<95)
		{
			SendDlgItemMessage(hDlg,IDC_YEAR, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)year+5);
			SetDlgItemInt(hDlg,IDC_LBLYEAR,year+2000,0);
		}
		else
		{
			SendDlgItemMessage(hDlg,IDC_YEAR, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)year-95);
			SetDlgItemInt(hDlg,IDC_LBLYEAR,year+1900,0);
		}
		
		SendDlgItemMessage(hDlg, IDC_HOUR, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,23));
		SendDlgItemMessage(hDlg,IDC_HOUR, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_HOUR, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)hour);
		SetDlgItemInt(hDlg,IDC_LBLHOUR,hour,0);
		
		SendDlgItemMessage(hDlg, IDC_MINUTE, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,59));
		SendDlgItemMessage(hDlg,IDC_MINUTE, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_MINUTE, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)minutes);
		SetDlgItemInt(hDlg,IDC_LBLMINUTE,minutes,0);
		
		SendDlgItemMessage(hDlg, IDC_SECOND, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,59));
		SendDlgItemMessage(hDlg,IDC_SECOND, TBM_SETTICFREQ,(WPARAM)5,0);
		SendDlgItemMessage(hDlg,IDC_SECOND, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)seconds);
		SetDlgItemInt(hDlg,IDC_LBLSECOND,seconds,0);
		
		
		
		if(rtc->reg[0x0D]&0x01)
			SendDlgItemMessage(hDlg,IDC_RTC_D1,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0D]&0x02)
			SendDlgItemMessage(hDlg,IDC_RTC_D2,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0D]&0x04)
			SendDlgItemMessage(hDlg,IDC_RTC_D4,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0D]&0x08)
			SendDlgItemMessage(hDlg,IDC_RTC_D8,BM_SETCHECK,BST_CHECKED,0);
		
		if(rtc->reg[0x0E]&0x01)
			SendDlgItemMessage(hDlg,IDC_RTC_E1,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0E]&0x02)
			SendDlgItemMessage(hDlg,IDC_RTC_E2,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0E]&0x04)
			SendDlgItemMessage(hDlg,IDC_RTC_E4,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0E]&0x08)
			SendDlgItemMessage(hDlg,IDC_RTC_E8,BM_SETCHECK,BST_CHECKED,0);
		
		if(rtc->reg[0x0F]&0x01)
			SendDlgItemMessage(hDlg,IDC_RTC_F1,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0F]&0x02)
			SendDlgItemMessage(hDlg,IDC_RTC_F2,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0F]&0x04)
			SendDlgItemMessage(hDlg,IDC_RTC_F4,BM_SETCHECK,BST_CHECKED,0);
		if(rtc->reg[0x0F]&0x08)
			SendDlgItemMessage(hDlg,IDC_RTC_F8,BM_SETCHECK,BST_CHECKED,0);
		
		return true;
	case  WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			rtc->reg[0x0]=seconds%10;
			rtc->reg[0x1]=seconds/10;
			rtc->reg[0x2]=minutes%10;
			rtc->reg[0x3]=minutes/10;
			rtc->reg[0x4]=hour%10;
			rtc->reg[0x5]=hour/10;
			rtc->reg[0x6]=day%10;
			rtc->reg[0x7]=day/10;
			rtc->reg[0x8]=month%10;
			rtc->reg[0x9]=month/10;
			rtc->reg[0xA]=year%10;
			rtc->reg[0xB]=year/10;
			
			if(IsDlgButtonChecked(hDlg, IDC_RTC_D1))
				rtc->reg[0x0D]|=0x01;
			else
				rtc->reg[0x0D]&=0x0E;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_D2))
				rtc->reg[0x0D]|=0x02;
			else
				rtc->reg[0x0D]&=0x0D;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_D4))
				rtc->reg[0x0D]|=0x04;
			else
				rtc->reg[0x0D]&=0x0B;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_D8))
				rtc->reg[0x0D]|=0x08;
			else
				rtc->reg[0x0D]&=0x07;
			
			if(IsDlgButtonChecked(hDlg, IDC_RTC_E1))
				rtc->reg[0x0E]|=0x01;
			else
				rtc->reg[0x0E]&=0x0E;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_E2))
				rtc->reg[0x0E]|=0x02;
			else
				rtc->reg[0x0E]&=0x0D;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_E4))
				rtc->reg[0x0E]|=0x04;
			else
				rtc->reg[0x0E]&=0x0B;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_E8))
				rtc->reg[0x0E]|=0x08;
			else
				rtc->reg[0x0E]&=0x07;
			
			if(IsDlgButtonChecked(hDlg, IDC_RTC_F1))
				rtc->reg[0x0F]|=0x01;
			else
				rtc->reg[0x0F]&=0x0E;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_F2))
				rtc->reg[0x0F]|=0x02;
			else
				rtc->reg[0x0F]&=0x0D;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_F4))
				rtc->reg[0x0F]|=0x04;
			else
				rtc->reg[0x0F]&=0x0B;
			if(IsDlgButtonChecked(hDlg, IDC_RTC_F8))
				rtc->reg[0x0F]|=0x08;
			else
				rtc->reg[0x0F]&=0x07;
			
			
			
			
		case IDCANCEL:
			EndDialog(hDlg, 1);
			return true;
			/*case IDC_MONTH:
			int dinmonth;
			dinmonth = S9xRTCDaysInMonth(
			*/
		default: return false;
		}
		case WM_HSCROLL:
			month=1+SendDlgItemMessage(hDlg,IDC_MONTH,TBM_GETPOS,0,0);
			SetDlgItemInt(hDlg,IDC_LBLMONTH,month,0);
			day=1+SendDlgItemMessage(hDlg,IDC_DAY,TBM_GETPOS,0,0);
			year=SendDlgItemMessage(hDlg,IDC_YEAR,TBM_GETPOS,0,0);
			hour=SendDlgItemMessage(hDlg,IDC_HOUR,TBM_GETPOS,0,0);
			minutes=SendDlgItemMessage(hDlg,IDC_MINUTE,TBM_GETPOS,0,0);
			seconds=SendDlgItemMessage(hDlg,IDC_SECOND,TBM_GETPOS,0,0);
			SetDlgItemInt(hDlg,IDC_LBLHOUR,hour,0);
			SetDlgItemInt(hDlg,IDC_LBLMINUTE,minutes,0);
			SetDlgItemInt(hDlg,IDC_LBLSECOND,seconds,0);
			if(year<5)
			{
				year+=95;
				SetDlgItemInt(hDlg,IDC_LBLYEAR,year+1900,0);
			}
			else
			{
				year-=5;
				SetDlgItemInt(hDlg,IDC_LBLYEAR,year+2000,0);
			}
			dayinmonth=S9xRTCDaysInMonth(month,year);
			if(day>dayinmonth)
			{
				day=dayinmonth;
				SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETPOS,(WPARAM)(BOOL)TRUE,(LPARAM)day-1);
			}
			SendDlgItemMessage(hDlg,IDC_DAY, TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,dayinmonth-1));
			SetDlgItemInt(hDlg,IDC_LBLDAY,day,0);
			return true;
			
		default: return false;
	}
	
}
#endif

const char *StaticRAMBitSize ()
{
    static char tmp [20];
	
    sprintf (tmp, " (%dKbit)", 8*(Memory.SRAMMask + 1) / 1024);
    return (tmp);
}

int CALLBACK DlgInfoProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("anomie.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			char temp[100];
			char romtext[4096];
			sprintf(romtext, "File: %s\r\nName: %s\r\n", Memory.ROMFilename, Memory.ROMName);
			sprintf(temp, "Speed: %02X/%s\r\nROM Map: %s\r\nType: %02x\r\n", Memory.ROMSpeed, ((Memory.ROMSpeed&0x10)!=0)?"FastROM":"SlowROM",(Memory.HiROM)?"HiROM":"LoROM",Memory.ROMType);
			strcat(romtext, temp);
			strcat(romtext, "Kart contents: ");
			strcat(romtext, Memory.KartContents ());
			strcat(romtext, "\r\nHeader ROM Size: ");
			strcat(romtext, Memory.Size());
			sprintf(temp, "\r\nCalculated ROM Size: %d Mbits", Memory.CalculatedSize/0x20000);
			strcat(romtext, temp);
			
			strcat(romtext, "\r\nSRAM size: ");
			strcat(romtext, Memory.StaticRAMSize ());
			strcat(romtext, StaticRAMBitSize());
			strcat(romtext, "\r\nActual Checksum: ");
			sprintf(temp, "%04X", Memory.CalculatedChecksum);
			strcat(romtext, temp);
			strcat(romtext, "\r\nHeader Checksum: ");
			sprintf(temp, "%04X", Memory.ROMChecksum);
			strcat(romtext, temp);
			strcat(romtext, "\r\nHeader Checksum Compliment: ");
			sprintf(temp, "%04X", Memory.ROMComplementChecksum);
			strcat(romtext, temp);
			strcat(romtext, "\r\nOutput: ");
			if(Memory.ROMRegion>12||Memory.ROMRegion<2)
				strcat(romtext, "NTSC 60Hz");
			else strcat(romtext, "PAL 50Hz");
			
			sprintf(temp, "\r\nCRC32:\t%08X", Memory.ROMCRC32);
			strcat(romtext, temp);
			
			
			strcat(romtext, "\r\nLicensee: ");
			
			int tmp=-1;
			//				sscanf(Memory.CompanyId, "%02X", &tmp);
			if(Memory.CompanyId[0]=='0')
				tmp=0;
			if(Memory.CompanyId[0]=='1')
				tmp=16;
			if(Memory.CompanyId[0]=='2')
				tmp=32;
			if(Memory.CompanyId[0]=='3')
				tmp=48;
			if(Memory.CompanyId[0]=='4')
				tmp=64;
			if(Memory.CompanyId[0]=='5')
				tmp=80;
			if(Memory.CompanyId[0]=='6')
				tmp=96;
			if(Memory.CompanyId[0]=='7')
				tmp=112;
			if(Memory.CompanyId[0]=='8')
				tmp=128;
			if(Memory.CompanyId[0]=='9')
				tmp=144;
			if(Memory.CompanyId[0]=='A')
				tmp=160;
			if(Memory.CompanyId[0]=='B')
				tmp=176;
			if(Memory.CompanyId[0]=='C')
				tmp=192;
			if(Memory.CompanyId[0]=='D')
				tmp=208;
			if(Memory.CompanyId[0]=='E')
				tmp=224;
			if(Memory.CompanyId[0]=='F')
				tmp=240;
			if(tmp!=-1)
			{
				if(Memory.CompanyId[1]=='0')
					tmp+=0;
				else if(Memory.CompanyId[1]=='1')
					tmp+=1;
				else if(Memory.CompanyId[1]=='2')
					tmp+=2;
				else if(Memory.CompanyId[1]=='3')
					tmp+=3;
				else if(Memory.CompanyId[1]=='4')
					tmp+=4;
				else if(Memory.CompanyId[1]=='5')
					tmp+=5;
				else if(Memory.CompanyId[1]=='6')
					tmp+=6;
				else if(Memory.CompanyId[1]=='7')
					tmp+=7;
				else if(Memory.CompanyId[1]=='8')
					tmp+=8;
				else if(Memory.CompanyId[1]=='9')
					tmp+=9;
				else if(Memory.CompanyId[1]=='A')
					tmp+=10;
				else if(Memory.CompanyId[1]=='B')
					tmp+=11;
				else if(Memory.CompanyId[1]=='C')
					tmp+=12;
				else if(Memory.CompanyId[1]=='D')
					tmp+=13;
				else if(Memory.CompanyId[1]=='E')
					tmp+=14;
				else if(Memory.CompanyId[1]=='F')
					tmp+=15;
				else tmp=0;
			}
			else tmp=0;
			if(tmp==0)
				tmp=(Memory.HiROM)?Memory.ROM[0x0FFDA]:Memory.ROM[0x7FDA];
			switch(tmp)
				//				switch(((Memory.ROMSpeed&0x0F)!=0)?Memory.ROM[0x0FFDA]:Memory.ROM[0x7FDA])
				//				switch(atoi(Memory.CompanyId))
				//				switch(((Memory.CompanyId[0]-'0')*16)+(Memory.CompanyId[1]-'0'))
			{
			case 0:strcat(romtext, "INVALID COMPANY");break;
			case 1:strcat(romtext, "Nintendo");break;
			case 2:strcat(romtext, "Ajinomoto");break;
			case 3:strcat(romtext, "Imagineer-Zoom");break;
			case 4:strcat(romtext, "Chris Gray Enterprises Inc.");break;
			case 5:strcat(romtext, "Zamuse");break;
			case 6:strcat(romtext, "Falcom");break;
			case 7:strcat(romtext, NOTKNOWN "7");break;
			case 8:strcat(romtext, "Capcom");break;
			case 9:strcat(romtext, "HOT-B");break;
			case 10:strcat(romtext, "Jaleco");break;
			case 11:strcat(romtext, "Coconuts");break;
			case 12:strcat(romtext, "Rage Software");break;
			case 13:strcat(romtext, "Micronet"); break; //Acc. ZFE
			case 14:strcat(romtext, "Technos");break;
			case 15:strcat(romtext, "Mebio Software");break;
			case 16:strcat(romtext, "SHOUEi System"); break; //Acc. ZFE
			case 17:strcat(romtext, "Starfish");break; //UCON 64
			case 18:strcat(romtext, "Gremlin Graphics");break;
			case 19:strcat(romtext, "Electronic Arts");break;
			case 20:strcat(romtext, "NCS / Masaya"); break; //Acc. ZFE
			case 21:strcat(romtext, "COBRA Team");break;
			case 22:strcat(romtext, "Human/Field");break;
			case 23:strcat(romtext, "KOEI");break;
			case 24:strcat(romtext, "Hudson Soft");break;
			case 25:strcat(romtext, "Game Village");break;//uCON64
			case 26:strcat(romtext, "Yanoman");break;
			case 27:strcat(romtext, NOTKNOWN "27");break;
			case 28:strcat(romtext, "Tecmo");break;
			case 29:strcat(romtext, NOTKNOWN "29");break;
			case 30:strcat(romtext, "Open System");break;
			case 31:strcat(romtext, "Virgin Games");break;
			case 32:strcat(romtext, "KSS");break;
			case 33:strcat(romtext, "Sunsoft");break;
			case 34:strcat(romtext, "POW");break;
			case 35:strcat(romtext, "Micro World");break;
			case 36:strcat(romtext, NOTKNOWN "36");break;
			case 37:strcat(romtext, NOTKNOWN "37");break;
			case 38:strcat(romtext, "Enix");break;
			case 39:strcat(romtext, "Loriciel/Electro Brain");break;//uCON64
			case 40:strcat(romtext, "Kemco");break;
			case 41:strcat(romtext, "Seta Co.,Ltd.");break;
			case 42:strcat(romtext, "Culture Brain"); break; //Acc. ZFE
			case 43:strcat(romtext, "Irem Japan");break;//Irem? Gun Force J
			case 44:strcat(romtext, "Pal Soft"); break; //Acc. ZFE
			case 45:strcat(romtext, "Visit Co.,Ltd.");break;
			case 46:strcat(romtext, "INTEC Inc."); break; //Acc. ZFE
			case 47:strcat(romtext, "System Sacom Corp."); break; //Acc. ZFE
			case 48:strcat(romtext, "Viacom New Media");break; //Zoop!
			case 49:strcat(romtext, "Carrozzeria");break;
			case 50:strcat(romtext, "Dynamic");break;
			case 51:strcat(romtext, "Nintendo");break;
			case 52:strcat(romtext, "Magifact");break;
			case 53:strcat(romtext, "Hect");break;
			case 54:strcat(romtext, NOTKNOWN "54");break;
			case 55:strcat(romtext, NOTKNOWN "55");break;
			case 56:strcat(romtext, "Capcom Europe");break;//Capcom? BOF2(E) MM7 (E)
			case 57:strcat(romtext, "Accolade Europe");break;//Accolade?Bubsy 2 (E)
			case 58:strcat(romtext, NOTKNOWN "58");break;
			case 59:strcat(romtext, "Arcade Zone");break;//uCON64
			case 60:strcat(romtext, "Empire Software");break;
			case 61:strcat(romtext, "Loriciel");break;
			case 62:strcat(romtext, "Gremlin Graphics"); break; //Acc. ZFE
			case 63:strcat(romtext, NOTKNOWN "63");break;
			case 64:strcat(romtext, "Seika Corp.");break;
			case 65:strcat(romtext, "UBI Soft");break;
			case 66:strcat(romtext, NOTKNOWN "66");break;
			case 67:strcat(romtext, NOTKNOWN "67");break;
			case 68:strcat(romtext, "LifeFitness Exertainment");break;//?? Exertainment Mountain Bike Rally (U).zip
			case 69:strcat(romtext, NOTKNOWN "69");break;
			case 70:strcat(romtext, "System 3");break;
			case 71:strcat(romtext, "Spectrum Holobyte");break;
			case 72:strcat(romtext, NOTKNOWN "72");break;
			case 73:strcat(romtext, "Irem");break;
			case 74:strcat(romtext, NOTKNOWN "74");break;
			case 75:strcat(romtext, "Raya Systems/Sculptured Software");break;
			case 76:strcat(romtext, "Renovation Products");break;
			case 77:strcat(romtext, "Malibu Games/Black Pearl");break;
			case 78:strcat(romtext, NOTKNOWN "78");break;
			case 79:strcat(romtext, "U.S. Gold");break;
			case 80:strcat(romtext, "Absolute Entertainment");break;
			case 81:strcat(romtext, "Acclaim");break;
			case 82:strcat(romtext, "Activision");break;
			case 83:strcat(romtext, "American Sammy");break;
			case 84:strcat(romtext, "GameTek");break;
			case 85:strcat(romtext, "Hi Tech Expressions");break;
			case 86:strcat(romtext, "LJN Toys");break;
			case 87:strcat(romtext, NOTKNOWN "87");break;
			case 88:strcat(romtext, NOTKNOWN "88");break;
			case 89:strcat(romtext, NOTKNOWN "89");break;
			case 90:strcat(romtext, "Mindscape");break;
			case 91:strcat(romtext, "Romstar, Inc."); break; //Acc. ZFE
			case 92:strcat(romtext, NOTKNOWN "92");break;
			case 93:strcat(romtext, "Tradewest");break;
			case 94:strcat(romtext, NOTKNOWN "94");break;
			case 95:strcat(romtext, "American Softworks Corp.");break;
			case 96:strcat(romtext, "Titus");break;
			case 97:strcat(romtext, "Virgin Interactive Entertainment");break;
			case 98:strcat(romtext, "Maxis");break;
			case 99:strcat(romtext, "Origin/FCI/Pony Canyon");break;//uCON64
			case 100:strcat(romtext, NOTKNOWN "100");break;
			case 101:strcat(romtext, NOTKNOWN "101");break;
			case 102:strcat(romtext, NOTKNOWN "102");break;
			case 103:strcat(romtext, "Ocean");break;
			case 104:strcat(romtext, NOTKNOWN "104");break;
			case 105:strcat(romtext, "Electronic Arts");break;
			case 106:strcat(romtext, NOTKNOWN "106");break;
			case 107:strcat(romtext, "Laser Beam");break;
			case 108:strcat(romtext, NOTKNOWN "108");break;
			case 109:strcat(romtext, NOTKNOWN "109");break;
			case 110:strcat(romtext, "Elite");break;
			case 111:strcat(romtext, "Electro Brain");break;
			case 112:strcat(romtext, "Infogrames");break;
			case 113:strcat(romtext, "Interplay");break;
			case 114:strcat(romtext, "LucasArts");break;
			case 115:strcat(romtext, "Parker Brothers");break;
			case 116:strcat(romtext, "Konami");break;//uCON64
			case 117:strcat(romtext, "STORM");break;
			case 118:strcat(romtext, NOTKNOWN "118");break;
			case 119:strcat(romtext, NOTKNOWN "119");break;
			case 120:strcat(romtext, "THQ Software");break;
			case 121:strcat(romtext, "Accolade Inc.");break;
			case 122:strcat(romtext, "Triffix Entertainment");break;
			case 123:strcat(romtext, NOTKNOWN "123");break;
			case 124:strcat(romtext, "Microprose");break;
			case 125:strcat(romtext, NOTKNOWN "125");break;
			case 126:strcat(romtext, NOTKNOWN "126");break;
			case 127:strcat(romtext, "Kemco");break;
			case 128:strcat(romtext, "Misawa");break;
			case 129:strcat(romtext, "Teichio");break;
			case 130:strcat(romtext, "Namco Ltd.");break;
			case 131:strcat(romtext, "Lozc");break;
			case 132:strcat(romtext, "Koei");break;
			case 133:strcat(romtext, NOTKNOWN "133");break;
			case 134:strcat(romtext, "Tokuma Shoten Intermedia");break;
			case 135:strcat(romtext, "Tsukuda Original"); break; //Acc. ZFE
			case 136:strcat(romtext, "DATAM-Polystar");break;
			case 137:strcat(romtext, NOTKNOWN "137");break;
			case 138:strcat(romtext, NOTKNOWN "138");break;
			case 139:strcat(romtext, "Bullet-Proof Software");break;
			case 140:strcat(romtext, "Vic Tokai");break;
			case 141:strcat(romtext, NOTKNOWN "141");break;
			case 142:strcat(romtext, "Character Soft");break;
			case 143:strcat(romtext, "I\'\'Max");break;
			case 144:strcat(romtext, "Takara");break;
			case 145:strcat(romtext, "CHUN Soft");break;
			case 146:strcat(romtext, "Video System Co., Ltd.");break;
			case 147:strcat(romtext, "BEC");break;
			case 148:strcat(romtext, NOTKNOWN "148");break;
			case 149:strcat(romtext, "Varie");break;
			case 150:strcat(romtext, "Yonezawa / S'Pal Corp."); break; //Acc. ZFE
			case 151:strcat(romtext, "Kaneco");break;
			case 152:strcat(romtext, NOTKNOWN "152");break;
			case 153:strcat(romtext, "Pack in Video");break;
			case 154:strcat(romtext, "Nichibutsu");break;
			case 155:strcat(romtext, "TECMO");break;
			case 156:strcat(romtext, "Imagineer Co.");break;
			case 157:strcat(romtext, NOTKNOWN "157");break;
			case 158:strcat(romtext, NOTKNOWN "158");break;
			case 159:strcat(romtext, NOTKNOWN "159");break;
			case 160:strcat(romtext, "Telenet");break;
			case 161:strcat(romtext, "Hori"); break; //Acc. uCON64
			case 162:strcat(romtext, NOTKNOWN "162");break;
			case 163:strcat(romtext, NOTKNOWN "163");break;
			case 164:strcat(romtext, "Konami");break;
			case 165:strcat(romtext, "K.Amusement Leasing Co.");break;
			case 166:strcat(romtext, NOTKNOWN "166");break;
			case 167:strcat(romtext, "Takara");break;
			case 168:strcat(romtext, NOTKNOWN "168");break;
			case 169:strcat(romtext, "Technos Jap.");break;
			case 170:strcat(romtext, "JVC");break;
			case 171:strcat(romtext, NOTKNOWN "171");break;
			case 172:strcat(romtext, "Toei Animation");break;
			case 173:strcat(romtext, "Toho");break;
			case 174:strcat(romtext, NOTKNOWN "174");break;
			case 175:strcat(romtext, "Namco Ltd.");break;
			case 176:strcat(romtext, "Media Rings Corp."); break; //Acc. ZFE
			case 177:strcat(romtext, "ASCII Co. Activison");break;
			case 178:strcat(romtext, "Bandai");break;
			case 179:strcat(romtext, NOTKNOWN "179");break;
			case 180:strcat(romtext, "Enix America");break;
			case 181:strcat(romtext, NOTKNOWN "181");break;
			case 182:strcat(romtext, "Halken");break;
			case 183:strcat(romtext, NOTKNOWN "183");break;
			case 184:strcat(romtext, NOTKNOWN "184");break;
			case 185:strcat(romtext, NOTKNOWN "185");break;
			case 186:strcat(romtext, "Culture Brain");break;
			case 187:strcat(romtext, "Sunsoft");break;
			case 188:strcat(romtext, "Toshiba EMI");break;
			case 189:strcat(romtext, "Sony Imagesoft");break;
			case 190:strcat(romtext, NOTKNOWN "190");break;
			case 191:strcat(romtext, "Sammy");break;
			case 192:strcat(romtext, "Taito");break;
			case 193:strcat(romtext, NOTKNOWN "193");break;
			case 194:strcat(romtext, "Kemco");break;
			case 195:strcat(romtext, "Square");break;
			case 196:strcat(romtext, "Tokuma Soft");break;
			case 197:strcat(romtext, "Data East");break;
			case 198:strcat(romtext, "Tonkin House");break;
			case 199:strcat(romtext, NOTKNOWN "199");break;
			case 200:strcat(romtext, "KOEI");break;
			case 201:strcat(romtext, NOTKNOWN "201");break;
			case 202:strcat(romtext, "Konami USA");break;
			case 203:strcat(romtext, "NTVIC");break;
			case 204:strcat(romtext, NOTKNOWN "204");break;
			case 205:strcat(romtext, "Meldac");break;
			case 206:strcat(romtext, "Pony Canyon");break;
			case 207:strcat(romtext, "Sotsu Agency/Sunrise");break;
			case 208:strcat(romtext, "Disco/Taito");break;
			case 209:strcat(romtext, "Sofel");break;
			case 210:strcat(romtext, "Quest Corp.");break;
			case 211:strcat(romtext, "Sigma");break;
			case 212:strcat(romtext, "Ask Kodansha Co., Ltd."); break; //Acc. ZFE
			case 213:strcat(romtext, NOTKNOWN "213");break;
			case 214:strcat(romtext, "Naxat");break;
			case 215:strcat(romtext, NOTKNOWN "215");break;
			case 216:strcat(romtext, "Capcom Co., Ltd.");break;
			case 217:strcat(romtext, "Banpresto");break;
			case 218:strcat(romtext, "Tomy");break;
			case 219:strcat(romtext, "Acclaim");break;
			case 220:strcat(romtext, NOTKNOWN "220");break;
			case 221:strcat(romtext, "NCS");break;
			case 222:strcat(romtext, "Human Entertainment");break;
			case 223:strcat(romtext, "Altron");break;
			case 224:strcat(romtext, "Jaleco");break;
			case 225:strcat(romtext, NOTKNOWN "225");break;
			case 226:strcat(romtext, "Yutaka");break;
			case 227:strcat(romtext, NOTKNOWN "227");break;
			case 228:strcat(romtext, "T&ESoft");break;
			case 229:strcat(romtext, "EPOCH Co.,Ltd.");break;
			case 230:strcat(romtext, NOTKNOWN "230");break;
			case 231:strcat(romtext, "Athena");break;
			case 232:strcat(romtext, "Asmik");break;
			case 233:strcat(romtext, "Natsume");break;
			case 234:strcat(romtext, "King Records");break;
			case 235:strcat(romtext, "Atlus");break;
			case 236:strcat(romtext, "Sony Music Entertainment");break;
			case 237:strcat(romtext, NOTKNOWN "237");break;
			case 238:strcat(romtext, "IGS");break;
			case 239:strcat(romtext, NOTKNOWN "239");break;
			case 240:strcat(romtext, NOTKNOWN "240");break;
			case 241:strcat(romtext, "Motown Software");break;
			case 242:strcat(romtext, "Left Field Entertainment");break;
			case 243:strcat(romtext, "Beam Software");break;
			case 244:strcat(romtext, "Tec Magik");break;
			case 245:strcat(romtext, NOTKNOWN "245");break;
			case 246:strcat(romtext, NOTKNOWN "246");break;
			case 247:strcat(romtext, NOTKNOWN "247");break;
			case 248:strcat(romtext, NOTKNOWN "248");break;
			case 249:strcat(romtext, "Cybersoft");break;
			case 250:strcat(romtext, NOTKNOWN "250");break;
			case 251:strcat(romtext, "Psygnosis"); break; //Acc. ZFE
			case 252:strcat(romtext, NOTKNOWN "252");break;
			case 253:strcat(romtext, NOTKNOWN "253");break;
			case 254:strcat(romtext, "Davidson"); break; //Acc. uCON64
			case 255:strcat(romtext, NOTKNOWN "255");break;
			default:strcat(romtext, NOTKNOWN);break;
				}
				
				strcat(romtext, "\r\nROM Version: ");
				sprintf(temp, "1.%d", (Memory.HiROM)?Memory.ROM[0x0FFDB]:Memory.ROM[0x7FDB]);
				strcat(romtext, temp);
				strcat(romtext, "\r\nRegion: ");
				switch(Memory.ROMRegion)
				{
				case 0:
					strcat(romtext, "Japan");
					break;
				case 1:
					strcat(romtext, "USA/Canada");
					break;
				case 2:
					strcat(romtext, "Oceania, Europe, and Asia");
					break;
				case 3:
					strcat(romtext, "Sweden");
					break;
				case 4:
					strcat(romtext, "Finland");
					break;
				case 5:
					strcat(romtext, "Denmark");
					break;
				case 6:
					strcat(romtext, "France");
					break;
				case 7:
					strcat(romtext, "Holland");
					break;
				case 8:
					strcat(romtext, "Spain");
					break;
				case 9:
					strcat(romtext, "Germany, Austria, and Switzerland");
					break;
				case 10:
					strcat(romtext, "Italy");
					break;
				case 11:
					strcat(romtext, "Hong Kong and China");
					break;
				case 12:
					strcat(romtext, "Indonesia");
					break;
				case 13:
					strcat(romtext, "South Korea");
					break;
				case 14:strcat(romtext, "Unknown region 14");break;
				default:strcat(romtext, "Unknown region 15");break;
				}
				SendDlgItemMessage(hDlg, IDC_ROM_DATA, WM_SETTEXT, 0, (LPARAM)romtext);
				return true;
				break;
			}
			case WM_CTLCOLORSTATIC:
				
				if(GUI.InfoColor!=WIN32_WHITE)
				{
					SetTextColor((HDC)wParam, GUI.InfoColor);
					SetBkColor((HDC)wParam, RGB(0,0,0));
				}
				return true;break;
		case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;

			case WM_COMMAND:
				{
					switch(wParam)
					{
					case IDOK:
					case IDCANCEL:
						EndDialog(hDlg, 0);
						if(hBmp)
						{
							DeleteObject(hBmp);
							hBmp=NULL;
						}
						return true;
						break;
					default: return false; break;
					}
				}
			default:return false;
	}
}

int CALLBACK DlgAboutProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;

	switch(msg)
	{
	case WM_INITDIALOG:
		{
			TCHAR buf[512];//find better way of dealing.
			sprintf(buf,TEXT(DISCLAIMER_TEXT),VERSION);
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("RedChaos1.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			SetDlgItemText(hDlg, IDC_DISCLAIMER, buf);
			SetWindowText(hDlg, TEXT("About ") TEXT(APP_NAME));
		}
		return true;
	case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
			case IDCANCEL:
				EndDialog(hDlg, 0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				return true;
				break;
			default: return false; break;
			}
		}
	default:return false;
	}
}
int CALLBACK DlgEmulatorProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("MKendora.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			SetWindowText(hDlg, TEXT(EMUSET_TITLE));
			SetDlgItemText(hDlg, IDC_TOGGLE_TURBO, EMUSET_TOGGLE_TURBO);
			SetDlgItemText(hDlg, IDC_LABEL_FREEZE, EMUSET_LABEL_FREEZE);
			SetDlgItemText(hDlg, IDOK, BUTTON_OK);
			SetDlgItemText(hDlg, IDCANCEL, BUTTON_CANCEL);
			SetDlgItemText(hDlg, IDC_LABEL_ASRAM, EMUSET_LABEL_ASRAM);
			SetDlgItemText(hDlg, IDC_LABEL_ASRAM_TEXT, EMUSET_LABEL_ASRAM_TEXT);
			SetDlgItemText(hDlg, IDC_LABEL_SMAX, EMUSET_LABEL_SMAX);
			SetDlgItemText(hDlg, IDC_LABEL_SMAX_TEXT, EMUSET_LABEL_SMAX_TEXT);
			SetDlgItemText(hDlg, IDC_LABEL_STURBO_TEXT, EMUSET_LABEL_STURBO_TEXT);
			SetDlgItemText(hDlg, IDC_LABEL_STURBO, EMUSET_LABEL_STURBO);
			SetDlgItemText(hDlg, IDC_BROWSE, EMUSET_BROWSE);
			SetDlgItemText(hDlg, IDC_FREEZE_FOLDER, GUI.FreezeFileDir);
			SendDlgItemMessage(hDlg, IDC_SRAM_SPIN, UDM_SETRANGE, 0, MAKELPARAM((short)99, (short)0));
			SendDlgItemMessage(hDlg, IDC_SRAM_SPIN,UDM_SETPOS,0, Settings.AutoSaveDelay);
			SendDlgItemMessage(hDlg, IDC_SPIN_MAX_SKIP, UDM_SETRANGE, 0, MAKELPARAM((short)59, (short)0));
			SendDlgItemMessage(hDlg, IDC_SPIN_MAX_SKIP,UDM_SETPOS,0, Settings.AutoMaxSkipFrames);
			SendDlgItemMessage(hDlg, IDC_SPIN_TURBO_SKIP, UDM_SETRANGE, 0, MAKELPARAM((short)59, (short)0));
			SendDlgItemMessage(hDlg, IDC_SPIN_TURBO_SKIP,UDM_SETPOS,0, Settings.TurboSkipFrames);
			if(GUI.TurboModeToggle)
			{
				CheckDlgButton(hDlg,IDC_TOGGLE_TURBO,BST_CHECKED);
			}
		}
		case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDC_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					TCHAR path[MAX_PATH];
					TCHAR title[]=TEXT("Select Freeze Folder");
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_FREEZE_FOLDER, path);
				}
				break;
			case IDOK:
				GetDlgItemText(hDlg, IDC_FREEZE_FOLDER,GUI.FreezeFileDir, MAX_PATH);
				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_TOGGLE_TURBO))
					GUI.TurboModeToggle=true;
				else GUI.TurboModeToggle=false;
				Settings.TurboSkipFrames=SendDlgItemMessage(hDlg, IDC_SPIN_TURBO_SKIP, UDM_GETPOS, 0,0);
				Settings.AutoMaxSkipFrames=SendDlgItemMessage(hDlg, IDC_SPIN_MAX_SKIP, UDM_GETPOS, 0,0);
				Settings.AutoSaveDelay=SendDlgItemMessage(hDlg, IDC_SRAM_SPIN, UDM_GETPOS, 0,0);
			case IDCANCEL:
				EndDialog(hDlg, 0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				return true;
				break;
			default: return false; break;
			}
		}
	default:return false;
	}
}

#define SKIP_FLOPPY

bool ExtensionIsValid(const TCHAR * filename)
{
	ExtList* curr=valid_ext;
	while(curr!=NULL)
	{
		if(curr->extension==NULL)
		{
			if(NULL==strstr(filename, TEXT(".")))
				return true;
		}
		else if(filename[(strlen(filename)-1)-strlen(curr->extension)]=='.')
		{
			if(0==_strnicmp(&filename[(strlen(filename))-strlen(curr->extension)],
				curr->extension, strlen(curr->extension)))
				return true;
		}
		curr=curr->next;
	}
	return false;
}

bool IsCompressed(TCHAR* filename)
{
	ExtList* curr=valid_ext;
	while(curr!=NULL)
	{
		if(curr->extension==NULL)
		{
			if(NULL==strstr(filename, TEXT(".")))
				return curr->compressed;
		}
		else if(filename[(strlen(filename)-1)-strlen(curr->extension)]=='.')
		{
			if(0==_strnicmp(&filename[(strlen(filename))-strlen(curr->extension)],
				curr->extension, strlen(curr->extension)))
				return curr->compressed;
		}
		curr=curr->next;
	}
	return false;
}

inline bool AllASCII(char *b, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (b[i] < 32 || b[i] > 126)
		{
			return(false);
		}
	}
	return(true);
}

inline int InfoScore(char *Buffer)
{
	int score = 0;
	if (Buffer[28] + (Buffer[29] << 8) +
		Buffer[30] + (Buffer[31] << 8) == 0xFFFF)
	{	score += 3; }
	
	if (Buffer[26] == 0x33) { score += 2; }
	if ((Buffer[21] & 0xf) < 4) {	score += 2; }
	if (!(Buffer[61] & 0x80)) { score -= 4; }
	if ((1 << (Buffer[23] - 7)) > 48) { score -= 1; }
	if (Buffer[25] < 14) { score += 1; }
	if (!AllASCII(Buffer, 20)) { score -= 1; }
	
	return (score);
}

inline unsigned short sum(unsigned char *array, unsigned int size = HEADER_SIZE)
{
	register unsigned short theSum = 0;
	for (register unsigned int i = 0; i < size; i++)
	{
		theSum += array[i];
	}
	return(theSum);
}

void rominfo(const TCHAR *filename, TCHAR *namebuffer, TCHAR *sizebuffer)
{
	struct stat filestats;
	stat(filename, &filestats);
	
	int HeaderSize = 0;
	
	if (filestats.st_size >= 0x8000)
	{ 
		ifstream ROMFile(filename, ios::in | ios::binary);
		if (ROMFile)
		{
			int HasHeadScore = 0, NoHeadScore = 0,
				HeadRemain = filestats.st_size & 0x7FFF;
			
			switch(HeadRemain)
			{
			case 0:
				NoHeadScore += 3;
				break;
				
			case HEADER_SIZE:
				HasHeadScore += 2;
				break;
			}
			
			unsigned char HeaderBuffer[HEADER_SIZE];
			ROMFile.read((char *)HeaderBuffer, HEADER_SIZE);
			
			if (sum(HeaderBuffer) < 2500) { HasHeadScore += 2; }
			
			//SMC/SWC Header
			if (HeaderBuffer[8] == 0xAA &&
				HeaderBuffer[9] == 0xBB &&
				HeaderBuffer[10]== 4)
			{ HasHeadScore += 3; }
			//FIG Header
			else if ((HeaderBuffer[4] == 0x77 && HeaderBuffer[5] == 0x83) ||
				(HeaderBuffer[4] == 0xDD && HeaderBuffer[5] == 0x82) ||
				(HeaderBuffer[4] == 0xDD && HeaderBuffer[5] == 2) ||
				(HeaderBuffer[4] == 0xF7 && HeaderBuffer[5] == 0x83) ||
				(HeaderBuffer[4] == 0xFD && HeaderBuffer[5] == 0x82) ||
				(HeaderBuffer[4] == 0x00 && HeaderBuffer[5] == 0x80) ||
				(HeaderBuffer[4] == 0x47 && HeaderBuffer[5] == 0x83) ||
				(HeaderBuffer[4] == 0x11 && HeaderBuffer[5] == 2))
			{ HasHeadScore += 2; }
			else if (!strncmp("GAME DOCTOR SF 3", (char *)HeaderBuffer, 16))
			{ HasHeadScore += 5; }
			
			HeaderSize = HasHeadScore > NoHeadScore ? HEADER_SIZE : 0;
			
			bool EHi = false;
			if (filestats.st_size - HeaderSize >= 0x500000)    
			{
				ROMFile.seekg(0x40FFC0 + HeaderSize, ios::beg);
				ROMFile.read((char *)HeaderBuffer, INFO_LEN);
				if (InfoScore((char *)HeaderBuffer) > 1)
				{
					EHi = true;
					strncpy(namebuffer, (char *)HeaderBuffer, 21);
				}
			}
			
			if (!EHi)
			{
				if (filestats.st_size - HeaderSize >= 0x10000)
				{
					char LoHead[INFO_LEN], HiHead[INFO_LEN];
					
					ROMFile.seekg(0x7FC0 + HeaderSize, ios::beg);
					ROMFile.read(LoHead, INFO_LEN);
					int LoScore = InfoScore(LoHead);
					
					ROMFile.seekg(0xFFC0 + HeaderSize, ios::beg);
					ROMFile.read(HiHead, INFO_LEN);
					int HiScore = InfoScore(HiHead);
					
					strncpy(namebuffer, LoScore > HiScore ? LoHead : HiHead, 21);
					
					if (filestats.st_size - HeaderSize >= 0x20000)
					{
						ROMFile.seekg((filestats.st_size - HeaderSize) / 2 + 0x7FC0 + HeaderSize, ios::beg);
						ROMFile.read(LoHead, INFO_LEN);
						int IntLScore = InfoScore(LoHead) / 2;
						
						if (IntLScore > LoScore && IntLScore > HiScore)
						{
							strncpy(namebuffer, LoHead, 21);
						}
					}
				}
				else //ROM only has one block
				{
					ROMFile.seekg(0x7FC0 + HeaderSize, ios::beg);
					ROMFile.read(namebuffer, 21);
				}
			}
			ROMFile.close();
		}
		else //Couldn't open file
		{
			strcpy(namebuffer, "Can't Open File      ");
		}
	}
	else //Smaller than a block
	{
		strcpy(namebuffer, "Not a ROM            ");
	}
	
	double MBitD = (double)(filestats.st_size - HeaderSize) / 0x100000 * 8;  
	int MBitI = (int)MBitD;
	if(0!=(MBitI / 10))
	{
		sizebuffer[0] = MBitI / 10 + '0';
		sizebuffer[1] = MBitI % 10 + '0';
		sizebuffer[2] = '.';
		sizebuffer[3] = (char)((MBitD - MBitI) * 10) + '0';
		sizebuffer[4] = (char)((int)((MBitD - MBitI) * 100) % 10) + '0';
		sizebuffer[5] = ' ';
		sizebuffer[6] = 'M';
		sizebuffer[7] = 'b';
		sizebuffer[8] = 'i';
		sizebuffer[9] = 't';
		sizebuffer[10] = '\0';
		namebuffer[21] = '\0';
	}
	else
	{
		sizebuffer[0] = MBitI % 10 + '0';
		sizebuffer[1] = '.';
		sizebuffer[2] = (char)((MBitD - MBitI) * 10) + '0';
		sizebuffer[3] = (char)((int)((MBitD - MBitI) * 100) % 10) + '0';
		sizebuffer[4] = ' ';
		sizebuffer[5] = 'M';
		sizebuffer[6] = 'b';
		sizebuffer[7] = 'i';
		sizebuffer[8] = 't';
		sizebuffer[9] = '\0';
		namebuffer[21] = '\0';
	}
}

void GetPathFromTree( HWND hDlg, UINT tree, TCHAR* selected, HTREEITEM hItem)
{
	TVITEM tv;
	TCHAR temp[MAX_PATH];
	temp[0]='\0';
	ZeroMemory(&tv, sizeof(TVITEM));
	HTREEITEM hTreeTemp=hItem;
	
	if(tv.iImage==4)
	{
		tv.mask=TVIF_HANDLE|TVIF_IMAGE;
		tv.hItem=hTreeTemp;
		tv.iImage=3;
		TreeView_SetItem(GetDlgItem(hDlg, tree),&tv);
		ZeroMemory(&tv, sizeof(TVITEM));
	}
	
	tv.mask=TVIF_HANDLE|TVIF_TEXT;
	tv.hItem=hTreeTemp;
	tv.pszText=temp;
	tv.cchTextMax =MAX_PATH;
	TreeView_GetItem(GetDlgItem(hDlg, tree), &tv);
	
	sprintf(selected, TEXT("%s"), temp);
	while(TreeView_GetParent(GetDlgItem(hDlg, tree), hTreeTemp)!=NULL)
	{
		temp[0]='\0';
		hTreeTemp=TreeView_GetParent(GetDlgItem(hDlg, tree), hTreeTemp);
		tv.mask=TVIF_HANDLE|TVIF_TEXT;
		tv.hItem=hTreeTemp;
		tv.pszText=temp;
		tv.cchTextMax =MAX_PATH;
		TreeView_GetItem(GetDlgItem(hDlg, tree), &tv);
		sprintf(temp, TEXT("%s\\%s"),temp, selected);
		strcpy(selected, temp);
	}
}

typedef struct RomDataCacheNode
{
	char* fname;
	char* rname;
	char* rmbits;
	struct RomDataCacheNode* next;
} RomDataList;

void ClearCacheList(RomDataList* rdl)
{
	RomDataList* temp=rdl;
	RomDataList* temp2=NULL;
	if(rdl==NULL)
		return;
	do
	{
		temp2=temp->next;
		if(temp->fname)
			delete [] temp->fname;
		if(temp->rmbits)
			delete [] temp->rmbits;
		if(temp->rname)
			delete [] temp->rname;
		delete temp;
		temp=temp2;
	}
	while(temp!=NULL);
}


void ExpandDir(char * selected, HTREEITEM hParent, HWND hDlg)
{
	TCHAR temp[MAX_PATH];
	WIN32_FIND_DATA wfd;
	ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
	strcat(selected, TEXT("\\*"));
	HANDLE hFind=FindFirstFile(selected,&wfd);
	selected[(strlen(selected)-1)]='\0';
	
	do
	{
		if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			if(strcmp(wfd.cFileName, TEXT("."))&&strcmp(wfd.cFileName, TEXT("..")))
			{
				//skip these, add the rest.
				TV_INSERTSTRUCT tvis;
				ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
				tvis.hParent=hParent;
				tvis.hInsertAfter=TVI_SORT;
				tvis.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
				tvis.item.pszText=wfd.cFileName;
				tvis.item.cchTextMax=MAX_PATH;
				tvis.item.iImage=4;
				tvis.item.iSelectedImage=3;
				HTREEITEM hNewTree=TreeView_InsertItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tvis);
				
				strcpy(temp, selected);
				strcat(temp, wfd.cFileName);
				strcat(temp, TEXT("\\*"));
				
				bool subdir=false;
				WIN32_FIND_DATA wfd2;
				ZeroMemory(&wfd2, sizeof(WIN32_FIND_DATA));
				HANDLE hFind2=FindFirstFile(temp,&wfd2);											
				do
				{
					if(wfd2.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
					{
						if(strcmp(wfd2.cFileName, TEXT("."))&&strcmp(wfd2.cFileName, TEXT("..")))
						{
							subdir=true;
						}
					}
				}
				while(FindNextFile(hFind2, &wfd2)&&!subdir);
				
				if(subdir)
				{
					TV_INSERTSTRUCT tvis;
					ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
					tvis.hParent=hNewTree;
					tvis.hInsertAfter=TVI_SORT;
					TreeView_InsertItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tvis);
					
				}
				FindClose(hFind2);
				
			}
		}
	}
	while(FindNextFile(hFind, &wfd));
	
	FindClose(hFind);
	//scan for folders
}

void ListFilesFromFolder(HWND hDlg, RomDataList** prdl)
{
	RomDataList* rdl= *prdl;
	RomDataList* current=NULL;
	int count=0;
	TVITEM tv;
	TCHAR temp[MAX_PATH];
	TCHAR selected[MAX_PATH];
	temp[0]='\0';
	ZeroMemory(&tv, sizeof(TVITEM));
	HTREEITEM hTreeItem=TreeView_GetSelection(GetDlgItem(hDlg, IDC_ROM_DIR));
	HTREEITEM hTreeTemp=hTreeItem;
	
	GetPathFromTree(hDlg, IDC_ROM_DIR, selected, hTreeItem);
	
	SendDlgItemMessage(hDlg, IDC_ROMLIST, WM_SETREDRAW, FALSE, 0);
	ListView_DeleteAllItems(GetDlgItem(hDlg, IDC_ROMLIST));
	ClearCacheList(rdl);
	rdl=NULL;
	//Add items here.
	
	WIN32_FIND_DATA wfd;
	ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
	
	strcat(selected, TEXT("\\*"));
	
	HANDLE hFind=FindFirstFile(selected, &wfd);
	selected[(strlen(selected)-1)]='\0';
	do
	{
		if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			continue;
		if(ExtensionIsValid(wfd.cFileName))
								{
			RomDataList* newitem=new RomDataList;
			ZeroMemory(newitem, sizeof(RomDataList));
			newitem->fname=new char[1+strlen(wfd.cFileName)];
			strcpy(newitem->fname, wfd.cFileName);
			count++;
			
			if(IsCompressed(wfd.cFileName))
			{
				newitem->rname=new char[14];
				strcpy(newitem->rname, "Not Available");
				newitem->rmbits=new char[8];
				strcpy(newitem->rmbits, "? Mbits");
			}
			
			if(!rdl)
				rdl=newitem;
			else
			{
				if(0>stricmp(newitem->fname,rdl->fname))
				{
					newitem->next=rdl;
					rdl=newitem;
				}
				else
				{
					RomDataList* trail=rdl;
					current=rdl->next;
					while(current!=NULL&&0<stricmp(newitem->fname,current->fname))
					{
						trail=current;
						current=current->next;
					}
					newitem->next=current;
					trail->next=newitem;
				}
			}
								}
	}
	while(FindNextFile(hFind, &wfd));
	
	FindClose(hFind);
	
		SendDlgItemMessage(hDlg, IDC_ROMLIST, WM_SETREDRAW, TRUE, 0);
	*prdl=rdl;
ListView_SetItemCount (GetDlgItem(hDlg, IDC_ROMLIST), count);
	ListView_SetItemState(GetDlgItem(hDlg,IDC_ROMLIST), 0, LVIS_SELECTED|LVIS_FOCUSED,LVIS_FOCUSED|LVIS_SELECTED);
}


int CALLBACK DlgOpenROMProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	int rv=0;
	static HWND hSplit;
	static HIMAGELIST hIcons;
	static TCHAR *filename;
	static RomDataList* rdl;
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("Koji.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			filename=(TCHAR*)lParam;
			RECT treeRect;
			RECT listRect;
			WNDCLASSEX wcex;
			TCHAR tempclassname[]=TEXT("S9xSplitter");
			ZeroMemory(&wcex, sizeof(WNDCLASSEX));
			wcex.cbSize=sizeof(WNDCLASSEX);
			wcex.hInstance=g_hInst;
			wcex.lpfnWndProc=DlgChildSplitProc;
			wcex.lpszClassName=tempclassname;
			wcex.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
			wcex.hCursor=LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
			ATOM aSplitter=RegisterClassEx(&wcex);
			GetWindowRect(GetDlgItem(hDlg, IDC_ROM_DIR), &treeRect);
			GetWindowRect(GetDlgItem(hDlg, IDC_ROMLIST), &listRect);
			POINT p;
			
			ListView_SetExtendedListViewStyle(GetDlgItem(hDlg, IDC_ROMLIST), LVS_EX_FULLROWSELECT);
			
			p.x=treeRect.right;
			p.y=treeRect.top;
			ScreenToClient(hDlg, &p);
			hSplit=CreateWindow(TEXT("S9xSplitter"), TEXT(""),WS_CHILD|WS_VISIBLE , p.x, p.y, listRect.left-treeRect.right , listRect.bottom-listRect.top, hDlg,NULL, g_hInst,0);
			
			LVCOLUMN col;
			TCHAR temp[32];
			strcpy(temp,TEXT("File"));
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=0;
			col.cx=122;
			col.cchTextMax=5;
			col.pszText=temp;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_ROMLIST),    0,   &col);
			
			strcpy(temp,TEXT("Description"));
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=1;
			col.cx=150;
			col.cchTextMax=32;
			col.pszText=temp;
			col.iSubItem=1;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_ROMLIST),    1,   &col);
			
			
			strcpy(temp,TEXT("Size"));
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=2;
			col.cx=67;
			col.cchTextMax=32;
			col.pszText=temp;
			col.iSubItem=2;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_ROMLIST),    2,   &col);
			
			
			SendDlgItemMessage(hDlg, IDC_MEM_TYPE,CB_INSERTSTRING,0,(LPARAM)TEXT("Auto-Detect"));
			SendDlgItemMessage(hDlg, IDC_MEM_TYPE,CB_INSERTSTRING,1,(LPARAM)TEXT("Force HiROM"));
			SendDlgItemMessage(hDlg, IDC_MEM_TYPE,CB_INSERTSTRING,2,(LPARAM)TEXT("Force LoROM"));
			SendDlgItemMessage(hDlg, IDC_MEM_TYPE,CB_SETCURSEL,0,0);
			
			SendDlgItemMessage(hDlg, IDC_VIDEO_MODE,CB_INSERTSTRING,0,(LPARAM)TEXT("Auto-Detect"));
			SendDlgItemMessage(hDlg, IDC_VIDEO_MODE,CB_INSERTSTRING,1,(LPARAM)TEXT("Force PAL"));
			SendDlgItemMessage(hDlg, IDC_VIDEO_MODE,CB_INSERTSTRING,2,(LPARAM)TEXT("Force NTSC"));
			SendDlgItemMessage(hDlg, IDC_VIDEO_MODE,CB_SETCURSEL,0,0);
			
			SendDlgItemMessage(hDlg, IDC_HEADER,CB_INSERTSTRING,0,(LPARAM)TEXT("Auto-Detect"));
			SendDlgItemMessage(hDlg, IDC_HEADER,CB_INSERTSTRING,1,(LPARAM)TEXT("Force Header"));
			SendDlgItemMessage(hDlg, IDC_HEADER,CB_INSERTSTRING,2,(LPARAM)TEXT("Force No Header"));
			SendDlgItemMessage(hDlg, IDC_HEADER,CB_SETCURSEL,0,0);
			
			SendDlgItemMessage(hDlg, IDC_INTERLEAVE,CB_INSERTSTRING,0,(LPARAM)TEXT("Auto-Detect"));
			SendDlgItemMessage(hDlg, IDC_INTERLEAVE,CB_INSERTSTRING,1,(LPARAM)TEXT("Force not interleaved"));
			SendDlgItemMessage(hDlg, IDC_INTERLEAVE,CB_INSERTSTRING,2,(LPARAM)TEXT("Force mode 1"));
			SendDlgItemMessage(hDlg, IDC_INTERLEAVE,CB_INSERTSTRING,3,(LPARAM)TEXT("Force mode 2"));
			SendDlgItemMessage(hDlg, IDC_INTERLEAVE,CB_INSERTSTRING,4,(LPARAM)TEXT("Force GD24"));
			SendDlgItemMessage(hDlg, IDC_INTERLEAVE,CB_SETCURSEL,0,0);
			
			hIcons=ImageList_Create(16,16,ILC_COLOR,5, 8);
			
			HANDLE hBitmap;
			hBitmap=LoadImage(g_hInst, MAKEINTRESOURCE(IDB_HARDDRIVE), IMAGE_BITMAP, 0,0, LR_DEFAULTCOLOR);
			ImageList_Add(hIcons, (HBITMAP)hBitmap, NULL);
			DeleteObject(hBitmap);
			
			hBitmap=LoadImage(g_hInst, MAKEINTRESOURCE(IDB_CDDRIVE), IMAGE_BITMAP, 0,0, LR_DEFAULTCOLOR);
			ImageList_Add(hIcons, (HBITMAP)hBitmap, NULL);
			DeleteObject(hBitmap);
			
			hBitmap=LoadImage(g_hInst, MAKEINTRESOURCE(IDB_NETDRIVE), IMAGE_BITMAP, 0,0, LR_DEFAULTCOLOR);
			ImageList_Add(hIcons, (HBITMAP)hBitmap, NULL);
			DeleteObject(hBitmap);
			
			hBitmap=LoadImage(g_hInst, MAKEINTRESOURCE(IDB_OPENFOLDER), IMAGE_BITMAP, 0,0, LR_DEFAULTCOLOR);
			ImageList_Add(hIcons, (HBITMAP)hBitmap, NULL);
			DeleteObject(hBitmap);
			
			hBitmap=LoadImage(g_hInst, MAKEINTRESOURCE(IDB_CLOSEDFOLDER), IMAGE_BITMAP, 0,0, LR_DEFAULTCOLOR);
			ImageList_Add(hIcons, (HBITMAP)hBitmap, NULL);
			DeleteObject(hBitmap);
			
			TreeView_SetImageList(GetDlgItem(hDlg,IDC_ROM_DIR), hIcons, TVSIL_NORMAL);
			
			DWORD dw;
			TCHAR buffer[MAX_PATH];
			TCHAR blah[MAX_PATH];
			long result=ERROR_SUCCESS-1;
			HTREEITEM hTreeDrive=NULL;

			HKEY hKey;
			char drive [_MAX_DRIVE + 1];
			strcpy(drive,"C:\\");

			if(!RegOpenKeyEx(HKEY_CURRENT_USER, MY_REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
			{
				dw=MAX_PATH;
				buffer[0]='\0';
				result=RegQueryValueEx(hKey, TEXT("Last Directory"), NULL,NULL, (uint8*)buffer,&dw);
				RegCloseKey(hKey);
				_splitpath(buffer,drive, NULL, NULL, NULL);
			}

			DWORD driveMask=GetLogicalDrives();
#ifndef SKIP_FLOPPY
			for (int i=0;i<26;i++)
#else
				for (int i=2;i<26;i++)
#endif
				{
					if(driveMask&(1<<i))
					{
						TCHAR driveName[4];
						driveName[0]='A'+i;
						driveName[1]=':';
						driveName[2]='\\';
						driveName[3]='\0';
						UINT driveType=GetDriveType(driveName);
						driveName[2]='\0';
						TVINSERTSTRUCT tvis;
						ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
						tvis.hParent=NULL;
						tvis.hInsertAfter=TVI_ROOT;
						tvis.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
						
						if(driveType==DRIVE_FIXED)
							tvis.item.iSelectedImage=tvis.item.iImage=0;
						else if(driveType==DRIVE_REMOTE)
							tvis.item.iSelectedImage=tvis.item.iImage=2;
						else tvis.item.iSelectedImage=tvis.item.iImage=1;
						
						tvis.item.pszText=driveName;

						HTREEITEM hTwee=TreeView_InsertItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tvis);
						
						if(result==ERROR_SUCCESS && !strncmp(drive, driveName, 2))
							hTreeDrive=hTwee;
						
						TCHAR temp[10];
						strcpy(temp, driveName);
						strcat(temp, TEXT("\\*"));
						bool subdir=false;

						
						if(driveType==DRIVE_REMOVABLE || driveType == DRIVE_CDROM || driveType == DRIVE_UNKNOWN)
						{
								TV_INSERTSTRUCT tvis;
								ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
								tvis.hParent=hTwee;
								tvis.hInsertAfter=TVI_SORT;
								TreeView_InsertItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tvis);

						}
						else
						{
							WIN32_FIND_DATA wfd2;
							ZeroMemory(&wfd2, sizeof(WIN32_FIND_DATA));
							HANDLE hFind2=FindFirstFile(temp,&wfd2);											
							do
							{
								if(wfd2.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
								{
									if(strcmp(wfd2.cFileName, TEXT("."))&&strcmp(wfd2.cFileName, TEXT("..")))
									{
										subdir=true;
									}
								}
							}
							while(FindNextFile(hFind2, &wfd2)&&!subdir);
						
							if(subdir)
							{
								TV_INSERTSTRUCT tvis;
								ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
								tvis.hParent=hTwee;
								tvis.hInsertAfter=TVI_SORT;
								TreeView_InsertItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tvis);
							}	
							FindClose(hFind2);
						}
					}
				}

				SendDlgItemMessage(hDlg, IDC_ROM_DIR, WM_SETREDRAW, FALSE, 0);
				if(result==ERROR_SUCCESS)
				{
					HTREEITEM hTreePrev;//,hTreeRoot;
				//	hTreePrev=TreeView_GetRoot(GetDlgItem(hDlg, IDC_ROM_DIR));
				//	hTreeRoot=hTreeDrive;
					hTreePrev=hTreeDrive;
					HTREEITEM hTemp=hTreePrev;
					TCHAR* temp=buffer;
					TCHAR * temp2;
					do
					{
						temp2=strstr(temp, TEXT("\\"));
						
						TVITEM tvi;
						ZeroMemory(&tvi, sizeof(TVITEM));
						tvi.mask=TVIF_TEXT;
						tvi.pszText=blah;
						tvi.cchTextMax=MAX_PATH;
						blah[0]='\0';
						if(temp2)
							*temp2='\0';

						tvi.hItem=hTemp;
						TreeView_GetItem(GetDlgItem(hDlg, IDC_ROM_DIR), &tvi);
						if(strcmp(blah, temp))
						{
							do
							{	
								tvi.mask=TVIF_TEXT;
								tvi.pszText=blah;
								tvi.cchTextMax=MAX_PATH;
								hTemp=TreeView_GetNextSibling(GetDlgItem(hDlg, IDC_ROM_DIR), hTemp);
								tvi.hItem=hTemp;
								TreeView_GetItem(GetDlgItem(hDlg, IDC_ROM_DIR), &tvi);
							}
							while(hTemp!=NULL&&strcmp(blah, temp));
							if(hTemp!=NULL)
							{
								hTreePrev=hTemp;
								if(temp2==NULL)
								{
									TreeView_SelectItem(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev);
									TreeView_EnsureVisible (GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev);
								}
								else
								{
									TCHAR get_dir[MAX_PATH];
									while(TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR),hTreePrev))
									{
										TreeView_DeleteItem(GetDlgItem(hDlg, IDC_ROM_DIR), TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR),hTreePrev));
									}
									TreeView_SetItemState(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev, TVIS_EXPANDED ,TVIS_EXPANDED );
									GetPathFromTree(hDlg, IDC_ROM_DIR, get_dir, hTreePrev);
									ExpandDir(get_dir, hTreePrev, hDlg);
									//TreeView_Expand(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev, TVE_EXPAND);
								}
								hTemp=TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev);
							}
						}
						else
						{
							if(NULL==temp2)
							{
								TreeView_SelectItem(GetDlgItem(hDlg, IDC_ROM_DIR), hTemp);
							}
							else
							{
								TCHAR get_dir[MAX_PATH];
								while(TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR),hTreePrev))
								{
									TreeView_DeleteItem(GetDlgItem(hDlg, IDC_ROM_DIR), TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR),hTreePrev));
								}
								TreeView_SetItemState(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev, TVIS_EXPANDED ,TVIS_EXPANDED );
								GetPathFromTree(hDlg, IDC_ROM_DIR, get_dir, hTreePrev);
								ExpandDir(get_dir, hTreePrev, hDlg);
								//TreeView_Expand(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev, TVE_EXPAND);
							}
							
							hTemp=TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR), hTemp);
						}
						if(temp2)
							temp=temp2+1;
						else temp=NULL;
					}
					while(temp);
					
					if(Memory.ROMFilename[0]!='\0')
					{
						LVFINDINFO lvfi;
						ZeroMemory(&lvfi, sizeof(LVFINDINFO));
						TCHAR *tmp, *tmp2;
						tmp=Memory.ROMFilename;
						while(tmp2=strstr(tmp, TEXT("\\")))
							tmp=tmp2+1;
						
						lvfi.flags=LVFI_STRING;
						lvfi.psz=tmp;
						
						int idx=ListView_FindItem(GetDlgItem(hDlg,IDC_ROMLIST), -1, &lvfi);
						ListView_SetSelectionMark(GetDlgItem(hDlg,IDC_ROMLIST), idx);
						ListView_SetItemState(GetDlgItem(hDlg,IDC_ROMLIST), idx, LVIS_SELECTED|LVIS_FOCUSED,LVIS_FOCUSED|LVIS_SELECTED);
						ListView_EnsureVisible(GetDlgItem(hDlg,IDC_ROMLIST), idx, FALSE);
						
					}
					SendDlgItemMessage(hDlg, IDC_ROM_DIR, WM_SETREDRAW, TRUE, 0);
					TreeView_EnsureVisible (GetDlgItem(hDlg, IDC_ROM_DIR), TreeView_GetSelection(GetDlgItem(hDlg, IDC_ROM_DIR)));
				}
				return true; //true sets the keyboard focus, in case we need this elsewhere
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					LVITEM lvi;
					ZeroMemory(&lvi, sizeof(LVITEM));
					//get selections
					int list_index=ListView_GetSelectionMark(GetDlgItem(hDlg,IDC_ROMLIST));
					if(list_index!=-1)
					{
						rv=1;
						TCHAR temp[MAX_PATH];
						temp[0]='\0';
						lvi.iItem=list_index;
						lvi.mask=LVIF_TEXT;
						lvi.pszText=filename;
						lvi.cchTextMax=MAX_PATH;
						ListView_GetItem(GetDlgItem(hDlg, IDC_ROMLIST), &lvi);
						
						strcpy(temp, filename);
						
						HTREEITEM hTreeTemp=TreeView_GetSelection(GetDlgItem(hDlg, IDC_ROM_DIR));
						TVITEM tv;
						ZeroMemory(&tv, sizeof(TVITEM));
						
						tv.mask=TVIF_HANDLE|TVIF_TEXT;
						tv.hItem=hTreeTemp;
						tv.pszText=temp;
						tv.cchTextMax =MAX_PATH;
						TreeView_GetItem(GetDlgItem(hDlg, IDC_ROM_DIR), &tv);
						sprintf(temp, TEXT("%s\\%s"), temp, filename);
						
						strcpy(filename, temp);
						
						while(TreeView_GetParent(GetDlgItem(hDlg, IDC_ROM_DIR), hTreeTemp)!=NULL)
						{
							temp[0]='\0';
							hTreeTemp=TreeView_GetParent(GetDlgItem(hDlg, IDC_ROM_DIR), hTreeTemp);
							tv.mask=TVIF_HANDLE|TVIF_TEXT;
							tv.hItem=hTreeTemp;
							tv.pszText=temp;
							tv.cchTextMax =MAX_PATH;
							TreeView_GetItem(GetDlgItem(hDlg, IDC_ROM_DIR), &tv);
							sprintf(temp, TEXT("%s\\%s"),temp, filename);
							strcpy(filename, temp);
						}
						
						int iTemp=SendDlgItemMessage(hDlg, IDC_MEM_TYPE,CB_GETCURSEL,0,0);
						
						Settings.ForceHiROM=Settings.ForceLoROM=FALSE;
						if(iTemp==1)
							Settings.ForceHiROM=TRUE;
						else if(iTemp==2)
							Settings.ForceLoROM=TRUE;
						
						iTemp=SendDlgItemMessage(hDlg, IDC_INTERLEAVE,CB_GETCURSEL,0,0);
						
						Settings.ForceNotInterleaved=Settings.ForceInterleaved=Settings.ForceInterleaved2=Settings.ForceInterleaveGD24=FALSE;
						if(iTemp==1)
							Settings.ForceNotInterleaved=TRUE;
						else if(iTemp==2)
							Settings.ForceInterleaved=TRUE;
						else if(iTemp==3)
							Settings.ForceInterleaved2=TRUE;
						else if(iTemp==4)
							Settings.ForceInterleaveGD24=TRUE;
						
						iTemp=SendDlgItemMessage(hDlg, IDC_VIDEO_MODE,CB_GETCURSEL,0,0);
						
						Settings.ForceNTSC=Settings.ForcePAL=FALSE;
						if(iTemp==1)
							Settings.ForcePAL=TRUE;
						else if(iTemp==2)
							Settings.ForceNTSC=TRUE;
						
						
						iTemp=SendDlgItemMessage(hDlg, IDC_HEADER,CB_GETCURSEL,0,0);
						
						Settings.ForceNoHeader=Settings.ForceHeader=FALSE;
						if(iTemp==1)
							Settings.ForceHeader=TRUE;
						else if(iTemp==2)
							Settings.ForceNoHeader=TRUE;
						
						strcpy(temp, filename);
						int i=strlen(temp);
						while(temp[i]!='\\')
						{
							temp[i]='\0';
							i--;
						}
						temp[i]='\0';
						HKEY hKey;
						if(!RegOpenKeyEx(HKEY_CURRENT_USER, MY_REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
						{
							RegSetValueEx(hKey, TEXT("Last Directory"), NULL, REG_SZ, (uint8*)temp, strlen(temp)+1);
							RegCloseKey(hKey);
						}
					}
				}
			case IDCANCEL:
				EndDialog(hDlg, rv);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				ClearCacheList(rdl);
				rdl=NULL;
				DestroyWindow(hSplit);
				UnregisterClass(TEXT("S9xSplitter"), g_hInst);
				TreeView_DeleteAllItems(GetDlgItem(hDlg, IDC_ROM_DIR));
				ListView_DeleteAllItems(GetDlgItem(hDlg, IDC_ROMLIST));
				return true;
				break;
			default: return false; break;
			}
		}
	case WM_NOTIFY:
		{
			if(lParam == NULL)
				return false;
			NMHDR* pNmh=(NMHDR*)lParam;
			switch(pNmh->idFrom)
			{
			case IDC_ROMLIST:
				{
					switch(pNmh->code)
					{
					case LVN_GETDISPINFO:
						{
							int i, j;
							RomDataList* curr=rdl;
							if(rdl==NULL)
								return false;
							NMLVDISPINFO * nmlvdi=(NMLVDISPINFO*)lParam;
							j=nmlvdi->item.iItem;
							for(i=0;i<j;i++)
								curr=curr->next;
							if(curr->rname==NULL)
							{
								TCHAR path[MAX_PATH];
								TCHAR buffer[32];
								TCHAR buffer2[32];
								GetPathFromTree(hDlg, IDC_ROM_DIR, path, TreeView_GetSelection(GetDlgItem(hDlg, IDC_ROM_DIR)));
								strcat(path, "\\");
								strcat(path, curr->fname);
								rominfo(path, buffer, buffer2);
								curr->rname=new char[strlen(buffer)+1];
								strcpy(curr->rname, buffer);
								curr->rmbits=new char[strlen(buffer2)+1];
								strcpy(curr->rmbits, buffer2);
							}

							if(nmlvdi->item.iSubItem==0)
							{
								nmlvdi->item.pszText=curr->fname;
								nmlvdi->item.cchTextMax=MAX_PATH;
							}
							if(nmlvdi->item.iSubItem==1)
							{
								nmlvdi->item.pszText=curr->rname;
								nmlvdi->item.cchTextMax=24;
							}

							if(nmlvdi->item.iSubItem==2)
							{
								nmlvdi->item.pszText=curr->rmbits;
								nmlvdi->item.cchTextMax=11;
							}
							nmlvdi->item.mask=LVIF_TEXT;
						}
						break;
					case NM_DBLCLK:
						{
							PostMessage(hDlg, WM_COMMAND, (WPARAM)(IDOK),(LPARAM)(NULL));
						}
					default:break;
					}
				}
				break;
			case IDC_ROM_DIR:
				{
					switch(pNmh->code)
					{
					case TVN_ITEMEXPANDING:
						{
							TCHAR selected[MAX_PATH];
							NMTREEVIEW* nmTv=(NMTREEVIEW*)lParam;
							while(TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR),nmTv->itemNew.hItem))
							{
								TreeView_DeleteItem(GetDlgItem(hDlg, IDC_ROM_DIR), TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR),nmTv->itemNew.hItem));
							}
							if(nmTv->action&TVE_EXPAND)
							{
								
								GetPathFromTree(hDlg, IDC_ROM_DIR, selected,nmTv->itemNew.hItem);
								
								ExpandDir(selected, nmTv->itemNew.hItem, hDlg);
							}
							else
							{
								TVITEM tv;
								ZeroMemory(&tv, sizeof(TVITEM));
								HTREEITEM hTreeTemp=nmTv->itemNew.hItem;
								
								if(tv.iImage==3)
								{
									tv.mask=TVIF_HANDLE|TVIF_IMAGE;
									tv.hItem=hTreeTemp;
									tv.iImage=4;
									TreeView_SetItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tv);
								}
								
								
								TV_INSERTSTRUCT tvis;
								ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
								tvis.hParent=nmTv->itemNew.hItem;
								tvis.hInsertAfter=TVI_SORT;
								TreeView_InsertItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tvis);
								
							}
						}
						return false;
						break;
					case TVN_SELCHANGED:
						{
							ListFilesFromFolder(hDlg, &rdl);
						}
					default:return false;
					}
				}
			default:return false;
			}
		}
	default:return false;
	}
}
LRESULT CALLBACK DlgChildSplitProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool PaintSpecial;
	static short drag_x;
	short temp_x;
	switch(msg)
	{
	case WM_CREATE:
		return 0;
	case WM_SIZE:
        return 0;
    case WM_PAINT:
		PAINTSTRUCT ps;
		GetUpdateRect (hWnd, &ps.rcPaint, true);
		ps.hdc=GetDC(hWnd);
		ps.fErase=true;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, ps.hdc);
        return 0;
    case WM_LBUTTONDOWN:
		PaintSpecial=true;
		drag_x=GET_X_LPARAM(lParam);
		SetCapture(hWnd);
        return 0;
    case WM_LBUTTONUP:
		PaintSpecial=false;
		temp_x=(GET_X_LPARAM(lParam)-drag_x);
		HWND hDlg,hTree,hList;
		RECT treeRect;
		RECT listRect;
		hDlg=GetParent(hWnd);
		hTree=GetDlgItem(hDlg, IDC_ROM_DIR);
		hList=GetDlgItem(hDlg, IDC_ROMLIST);
		GetWindowRect(hTree, &treeRect);
		
		POINT p;
		p.x=temp_x+treeRect.right;
		p.y=treeRect.top;
		
		GetWindowRect(hList, &listRect);
		
		if(p.x>(listRect.right-50))
		{
			temp_x-=(p.x-(listRect.right-50));
			p.x=listRect.right-50;
		}
		
		
		ScreenToClient(hDlg, &p);
		
		if(p.x<50)
		{
			temp_x+=(short)(50-p.x);
			p.x=50;
		}
		
		
		MoveWindow( hWnd, p.x, p.y, listRect.left-treeRect.right, listRect.bottom-listRect.top, FALSE);
		MoveWindow(hList, p.x+(listRect.left-treeRect.right), p.y,listRect.right-listRect.left-temp_x, listRect.bottom-listRect.top, TRUE);
		p.x=treeRect.left;
		p.y=treeRect.top;
		ScreenToClient(hDlg, &p);
		MoveWindow(hTree, p.x, p.y,treeRect.right-treeRect.left+temp_x,treeRect.bottom-treeRect.top, true);
		InvalidateRect(hWnd,NULL, true);
		ReleaseCapture();
        return 0;
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
		{
			//move paint location
			PaintSpecial=true;
			temp_x=(GET_X_LPARAM(lParam)-drag_x);
			hDlg=GetParent(hWnd);
			hTree=GetDlgItem(hDlg, IDC_ROM_DIR);
			hList=GetDlgItem(hDlg, IDC_ROMLIST);
			GetWindowRect(hTree, &treeRect);
			
			p.x=temp_x+treeRect.right;
			
			p.y=treeRect.top;
			GetWindowRect(hList, &listRect);
			
			if(p.x>(listRect.right-50))
			{
				temp_x-=(p.x-(listRect.right-50));
				p.x=listRect.right-50;
			}
			
			
			ScreenToClient(hDlg, &p);
			
			if(p.x<50)
			{
				temp_x+=(short)(50-p.x);
				p.x=50;
			}
			
			MoveWindow(hWnd, p.x, p.y, listRect.left-treeRect.right, listRect.bottom-listRect.top, FALSE);
			MoveWindow(hList, p.x+(listRect.left-treeRect.right), p.y,listRect.right-listRect.left-temp_x, listRect.bottom-listRect.top, TRUE);
			p.x=treeRect.left;
			p.y=treeRect.top;
			ScreenToClient(hDlg, &p);
			MoveWindow(hTree, p.x, p.y,treeRect.right-treeRect.left+temp_x,treeRect.bottom-treeRect.top, true);
			InvalidateRect(hWnd,NULL, true);
		}
        return 0;
    case WM_CAPTURECHANGED:
		PaintSpecial=false;
		ReleaseCapture();
		return 0;
    case WM_DESTROY:
        return 0;
	default:return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}





int CALLBACK DlgPackConfigProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("LinkHylia.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			SetDlgItemText(hDlg, IDC_STAR_OCEAN, GUI.StarOceanPack);
			SetDlgItemText(hDlg, IDC_SFA2, GUI.SFA2NTSCPack);
			SetDlgItemText(hDlg, IDC_SFA2E, GUI.SFA2PALPack);
			SetDlgItemText(hDlg, IDC_SJNS, GUI.SJNSPack);
			SetDlgItemText(hDlg, IDC_SFZ2, GUI.SFZ2Pack);
			SetDlgItemText(hDlg, IDC_MDH, GUI.MDHPack);
			SetDlgItemText(hDlg, IDC_SPL4, GUI.SPL4Pack);
			SetDlgItemText(hDlg, IDC_FEOEZ, GUI.FEOEZPack);
		}
		case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDC_SO_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					char path[MAX_PATH];
					char title[]="Star Ocean Graphics Pack";
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_STAR_OCEAN, path);
				}
				break;
			case IDC_SFA2_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					char path[MAX_PATH];
					char title[]="Street Fighter Alpha 2 Graphics Pack";
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_SFA2, path);
				}
				break;
			case IDC_SFA2E_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					char path[MAX_PATH];
					char title[]="Street Fighter Alpha 2 Graphics Pack";
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_SFA2E, path);
				}
				break;
			case IDC_SFZ2_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					char path[MAX_PATH];
					char title[]="Street Fighter Zero 2 Graphics Pack";
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_SFZ2, path);
				}
				break;
			case IDC_FEOEZ_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					char path[MAX_PATH];
					char title[]="Far East of Eden Zero Graphics Pack";
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_FEOEZ, path);
				}
				break;
			case IDC_MDH_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					char path[MAX_PATH];
					char title[]="Momotarou Densetsu Happy Graphics Pack";
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_MDH, path);
				}
				break;
			case IDC_FEOEZ_SJNS_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					char path[MAX_PATH];
					char title[]="FEOEZ - Shounen Jump no Shou Graphics Pack";
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_SJNS, path);
				}
				break;
			case IDC_SPL4_BROWSE:
				{
					LPMALLOC lpm=NULL;
					LPITEMIDLIST iidl=NULL;
					BROWSEINFO bi;
					ZeroMemory(&bi, sizeof(BROWSEINFO));
					char path[MAX_PATH];
					char title[]="FEOEZ - Shounen Jump no Shou Graphics Pack";
					CoInitialize(NULL);
					bi.hwndOwner=hDlg;
					bi.pszDisplayName=path;
					bi.lpszTitle=title;
					iidl=SHBrowseForFolder(&bi);
					SHGetPathFromIDList(iidl, path);
					SHGetMalloc(&lpm);
					lpm->Free(iidl);
					CoUninitialize();
					SetDlgItemText(hDlg, IDC_SPL4, path);
				}
				break;
			case IDOK:
				GetDlgItemText(hDlg, IDC_STAR_OCEAN, GUI.StarOceanPack, MAX_PATH);
				GetDlgItemText(hDlg, IDC_SFA2, GUI.SFA2NTSCPack, MAX_PATH);
				GetDlgItemText(hDlg, IDC_SPL4, GUI.SPL4Pack, MAX_PATH);
				GetDlgItemText(hDlg, IDC_SJNS, GUI.SJNSPack, MAX_PATH);
				GetDlgItemText(hDlg, IDC_SFZ2, GUI.SFZ2Pack, MAX_PATH);
				GetDlgItemText(hDlg, IDC_SFA2E, GUI.SFA2PALPack, MAX_PATH);
				GetDlgItemText(hDlg, IDC_MDH, GUI.MDHPack, MAX_PATH);
				GetDlgItemText(hDlg, IDC_FEOEZ, GUI.FEOEZPack, MAX_PATH);
			case IDCANCEL:
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				EndDialog(hDlg, 0);
				return true;
				break;
			default: return false; break;
				}
			}
		default:return false;
	}
}

extern "C"
{
	char*osd_GetPackDir()
	{
		static char filename[MAX_PATH];
		memset(filename, 0, MAX_PATH);
		
		if(strlen(GUI.FreezeFileDir)!=0)
			strcpy (filename, GUI.FreezeFileDir);
		else
		{
			char dir [_MAX_DIR + 1];
			char drive [_MAX_DRIVE + 1];
			char name [_MAX_FNAME + 1];
			char ext [_MAX_EXT + 1];
			_splitpath(Memory.ROMFilename,drive, dir, name, ext);
			_makepath(filename,drive, dir, NULL, NULL);
		}
		
		if(!strncmp((char*)&Memory.ROM [0xffc0], "SUPER POWER LEAG 4   ", 21))
		{
			if(strlen(GUI.SPL4Pack))
				return GUI.SPL4Pack;
			else strcat(filename, "\\SPL4-SP7");
		}
		else if(!strncmp((char*)&Memory.ROM [0xffc0], "MOMOTETSU HAPPY      ",21))
		{
			if(strlen(GUI.MDHPack))
				return GUI.MDHPack;
			else strcat(filename, "\\SMHT-SP7");
		}
		else if(!strncmp((char*)&Memory.ROM [0xffc0], "HU TENGAI MAKYO ZERO ", 21))
		{
			if(strlen(GUI.FEOEZPack))
				return GUI.FEOEZPack;
			else strcat(filename, "\\FEOEZSP7");
		}
		else if(!strncmp((char*)&Memory.ROM [0xffc0], "JUMP TENGAIMAKYO ZERO",21))
		{
			if(strlen(GUI.SJNSPack))
				return GUI.SJNSPack;
			else strcat(filename, "\\SJUMPSP7");
		}
		else strcat(filename, "\\MISC-SP7");
		return filename;
	}
}
#ifdef NETPLAY_SUPPORT
int CALLBACK DlgNetConnect(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	HKEY hKey;
	char defPort[5];
	char portTemp[5];
	char temp[100];
	char temp2[5];
	static char* hostname;
	unsigned long cbData;
	static int i;
	if(Settings.Port==0)
	{
		_itoa(1996,defPort,10);
	}
	else
	{
		_itoa(Settings.Port,defPort,10);
	}
	
	WORD chkLength;
	if(RegCreateKeyEx(HKEY_CURRENT_USER,MY_REG_KEY "\\1.x\\NetPlayServerHistory",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS, NULL, &hKey,NULL) == ERROR_SUCCESS){}
	
	switch (msg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg,NPCON_TITLE);
		SetDlgItemText(hDlg,IDC_LABEL_SERVERADDY,NPCON_LABEL_SERVERADDY);
		SetDlgItemText(hDlg,IDC_LABEL_PORTNUM,NPCON_LABEL_PORTNUM);
		SetDlgItemText(hDlg,IDC_CLEARHISTORY, NPCON_CLEARHISTORY);
		SetDlgItemText(hDlg,IDOK,BUTTON_OK);
		SetDlgItemText(hDlg,IDCANCEL,BUTTON_CANCEL);
		hBmp=(HBITMAP)LoadImage(NULL, TEXT("Overload.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		hostname = (char *)lParam;
		if(RegOpenKeyEx(HKEY_CURRENT_USER,
			MY_REG_KEY "\\1.x\\NetPlayServerHistory",
			0,KEY_READ, &hKey)==ERROR_SUCCESS)
		{
			i=-1;
			sprintf(temp,"%d",i);
			do
			//while(RegQueryValueEx(hKey,temp,0,NULL,NULL,NULL)==ERROR_SUCCESS)
			{
				//MessageBox(hDlg,temp,"Caption",MB_OK);
				//MessageBox(hDlg,temp,"Caption",MB_OK);
				i++;
				sprintf(temp,"%d",i);
			}while(RegQueryValueEx(hKey,temp,0,NULL,NULL,NULL)==ERROR_SUCCESS);
			//MessageBox(hDlg,temp,"Caption",MB_OK);
			//i--;
			int j;
			for(j=0;j<i;j++)
			{
				sprintf(temp2,"%d",j);
				cbData = _MAX_PATH+1;
				RegQueryValueEx(hKey,temp2,0,NULL,(unsigned char *)temp,&cbData);
				SendDlgItemMessage(hDlg, IDC_HOSTNAME, CB_INSERTSTRING,j,(LPARAM)temp);
				
			}
			RegCloseKey(hKey);
		}
		
		SendDlgItemMessage(hDlg, IDC_PORTNUMBER, WM_SETTEXT, 0, (LPARAM)defPort);
		return TRUE;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint (hDlg, &ps);
			if(hBmp)
			{
				BITMAP bmp;
				ZeroMemory(&bmp, sizeof(BITMAP));
				RECT r;
				GetClientRect(hDlg, &r);
				HDC hdc=GetDC(hDlg);
				HDC hDCbmp=CreateCompatibleDC(hdc);
				GetObject(hBmp, sizeof(BITMAP), &bmp);
				HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
				StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
				SelectObject(hDCbmp, hOldBmp);
				DeleteDC(hDCbmp);
				ReleaseDC(hDlg, hdc);
			}
			
			EndPaint (hDlg, &ps);
		}
		return true;
	
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_CLEARHISTORY:
			{
				if(RegOpenKeyEx(HKEY_CURRENT_USER,
					MY_REG_KEY  "\\1.x\\NetPlayServerHistory",
					0,KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
				{
					int j;
					for(j=0;j<i;j++)
					{
						sprintf(temp2,"%d",j);
						RegDeleteValue(hKey,temp2);
					}
					SendDlgItemMessage(hDlg,IDC_HOSTNAME,CB_RESETCONTENT,0,0);
				}
				break;
			}
		case IDOK:
			{
				
				chkLength = (WORD) SendDlgItemMessage(hDlg,IDC_PORTNUMBER,EM_LINELENGTH,0,0);
				*((LPWORD)portTemp) = chkLength;
				SendDlgItemMessage(hDlg,IDC_PORTNUMBER,EM_GETLINE,0,(LPARAM)(LPCTSTR)portTemp);
				
				if(atoi(portTemp)>65535||atoi(portTemp)<1024)
				{
					MessageBox(hDlg,"Port Number needs to be between 1024 and 65535","Error",MB_OK);
					break;
				}
				else
				{
					Settings.Port = atoi(portTemp);

				}
				//chkLength = (WORD) SendDlgItemMessage(hDlg,IDC_HOSTNAME,EM_LINELENGTH,0,0);
				//if(chkLength > 0)
				//{
				//SendDlgItemMessage(hDlg,IDC_HOSTNAME,EM_GETLINE,0,(LPARAM)hostname);
				SendDlgItemMessage(hDlg,IDC_HOSTNAME,WM_GETTEXT,100,(LPARAM)temp);
				strcpy(hostname,temp);
				//MessageBox(hDlg,temp,"hola",MB_OK);
				unsigned long len;
				len = strlen(temp);
				if(len > 0)
				{
					if(RegOpenKeyEx(HKEY_CURRENT_USER,
						MY_REG_KEY "\\1.x\\NetPlayServerHistory",
						0,KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
					{
						
						sprintf(temp2,"%d",i);
						
						if(RegSetValueEx(hKey,temp2,0,REG_SZ,(const unsigned char*)temp,len+1)==ERROR_SUCCESS)
						{
							
						}
						else
						{
						}
						RegCloseKey(hKey);
					}
					EndDialog(hDlg,1);
					if(hBmp)
					{
						DeleteObject(hBmp);
						hBmp=NULL;
					}
					return TRUE;
				}
				else
				{
					EndDialog(hDlg,0);
					if(hBmp)
					{
						DeleteObject(hBmp);
						hBmp=NULL;
					}
					return TRUE;
				}

				break;
				//}
			}
		case IDCANCEL:
			{
				EndDialog(hDlg, 0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				return TRUE;
			}
		default:break;
		}
	}
	return FALSE;
}
#endif
void SetInfoDlgColor(unsigned char r, unsigned char g, unsigned char b)
{
	GUI.InfoColor=RGB(r,g,b);
}

void ClearExts(void)
{
	ExtList* temp;
	ExtList* curr=valid_ext;
	while(curr!=NULL)
	{
		temp=curr->next;
		if(curr->extension)
			delete [] curr->extension;
		delete curr;
		curr=temp;
	}
	valid_ext=NULL;
	
}

void LoadExts(void)
{
	char buffer[MAX_PATH+2];
	if(valid_ext!=NULL)
	{
		ClearExts();
	}
	ExtList* curr;
	valid_ext=new ExtList;
	curr=valid_ext;
	ZeroMemory(curr, sizeof(ExtList));
	ifstream in;
	in.open("Valid.Ext", ios::in|ios::nocreate);
	if (!in.is_open())
	{
		in.clear();
		MakeExtFile();
		in.open("Valid.Ext", ios::in|ios::nocreate);
		if(!in.is_open())
		{
			MessageBox(GUI.hWnd, "Fatal Error: The File \"Valid.Ext\" could not be found or created.", "Error", MB_ICONERROR|MB_OK);
			PostQuitMessage(-1);
		}
	}
	
	do
	{
		buffer[0]='\0';
		in.getline(buffer,MAX_PATH+2);
		if((*buffer)!='\0')
		{
			curr->next=new ExtList;
			curr=curr->next;
			ZeroMemory(curr, sizeof(ExtList));
			if(_strnicmp(buffer+strlen(buffer)-1, "Y", 1)==0)
				curr->compressed=true;
			if(strlen(buffer)>1)
			{
				curr->extension=new char[strlen(buffer)-1];
				strncpy(curr->extension, buffer, strlen(buffer)-1);
				curr->extension[strlen(buffer)-1]='\0';
			}
			else curr->extension=NULL;
		}
	}
	while(!in.eof());
	in.close();
	curr=valid_ext;
	valid_ext=valid_ext->next;
	delete curr;
}

void MakeExtFile(void)
{
	ofstream out;
	out.open("Valid.Ext");
	
	out<<"N"   <<endl<<"smcN"<<endl<<"zipY"<<endl<<"gzY" <<endl<<"swcN"<<endl<<"figN"<<endl;
	out<<"058N"<<endl<<"078N"<<endl<<"japN"<<endl<<"usaN"<<endl<<"048N"<<endl;
	out<<"eurN"<<endl<<"sfcN"<<endl<<"1N"  <<endl<<"mgdN"<<endl<<"ufoN"<<endl;
	out<<"binN"<<endl<<"gd3N"<<endl<<"mghN"<<endl<<"gd7N"<<endl<<"ausN"<<endl;
	out<<"dx2N"<<endl<<"aN"<<endl<<"jmaY";
	out.close();
	SetFileAttributes("Valid.Ext", FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_READONLY);
};
#ifdef NETPLAY_SUPPORT
int CALLBACK DlgNPOptions(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	char defPort[5];
	WORD chkLength;
	if(Settings.Port==0)
	{
		_itoa(1996,defPort,10);
	}
	else
	{
		_itoa(Settings.Port,defPort,10);
	}
	
	switch (msg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg,NPOPT_TITLE);
		SetDlgItemText(hDlg,IDC_LABEL_PORTNUM,NPOPT_LABEL_PORTNUM);
		SetDlgItemText(hDlg,IDC_LABEL_PAUSEINTERVAL,NPOPT_LABEL_PAUSEINTERVAL);
		SetDlgItemText(hDlg,IDC_LABEL_PAUSEINTERVAL_TEXT,NPOPT_LABEL_PAUSEINTERVAL_TEXT);
		SetDlgItemText(hDlg,IDC_LABEL_MAXSKIP,NPOPT_LABEL_MAXSKIP);
		SetDlgItemText(hDlg,IDC_SYNCBYRESET,NPOPT_SYNCBYRESET);
		SetDlgItemText(hDlg,IDC_SENDROM,NPOPT_SENDROM);
		SetDlgItemText(hDlg,IDC_ACTASSERVER,NPOPT_ACTASSERVER);
		SetDlgItemText(hDlg,IDC_PORTNUMBLOCK,NPOPT_PORTNUMBLOCK);
		SetDlgItemText(hDlg,IDC_CLIENTSETTINGSBLOCK,NPOPT_CLIENTSETTINGSBLOCK);
		SetDlgItemText(hDlg,IDC_SERVERSETTINGSBLOCK,NPOPT_SERVERSETTINGSBLOCK);
		SetDlgItemText(hDlg,IDOK,BUTTON_OK);
		SetDlgItemText(hDlg,IDCANCEL,BUTTON_CANCEL);

		hBmp=(HBITMAP)LoadImage(NULL, TEXT("TheDumper.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		SendDlgItemMessage(hDlg, IDC_PORTNUMBERA, WM_SETTEXT, 0, (LPARAM)defPort);
		if(Settings.NetPlayServer)
		{
			SendDlgItemMessage(hDlg, IDC_ACTASSERVER, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);	
		}
		if(NPServer.SendROMImageOnConnect)	
		{
			SendDlgItemMessage(hDlg, IDC_SENDROM, BM_SETCHECK, BST_CHECKED,0);	
		}
		
		if(NPServer.SyncByReset)
		{
			SendDlgItemMessage(hDlg, IDC_SYNCBYRESET, BM_SETCHECK, BST_CHECKED,0);	
		}
		SendDlgItemMessage(hDlg, IDC_MAXSPIN, UDM_SETRANGE,0,MAKELPARAM((short)60,(short)0));
		SendDlgItemMessage(hDlg, IDC_MAXSPIN, UDM_SETPOS,0,MAKELONG(NetPlay.MaxFrameSkip,0));
		SendDlgItemMessage(hDlg, IDC_PAUSESPIN, UDM_SETRANGE,0,MAKELONG(30,0));
		SendDlgItemMessage(hDlg, IDC_PAUSESPIN, UDM_SETPOS,0,MAKELONG(NetPlay.MaxBehindFrameCount,0));
		return TRUE;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint (hDlg, &ps);
			if(hBmp)
			{
				BITMAP bmp;
				ZeroMemory(&bmp, sizeof(BITMAP));
				RECT r;
				GetClientRect(hDlg, &r);
				HDC hdc=GetDC(hDlg);
				HDC hDCbmp=CreateCompatibleDC(hdc);
				GetObject(hBmp, sizeof(BITMAP), &bmp);
				HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
				StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
				SelectObject(hDCbmp, hOldBmp);
				DeleteDC(hDCbmp);
				ReleaseDC(hDlg, hdc);
			}
			
			EndPaint (hDlg, &ps);
		}
		return true;

	case WM_COMMAND:		
		switch(LOWORD(wParam))
		{
		case IDOK:	
			{	
				NetPlay.MaxFrameSkip=(uint32)SendDlgItemMessage(hDlg, IDC_MAXSPIN, UDM_GETPOS,0,0);	
				NetPlay.MaxBehindFrameCount=(uint32)SendDlgItemMessage(hDlg, IDC_PAUSESPIN, UDM_GETPOS,0,0);
				chkLength=(WORD)SendDlgItemMessage(hDlg,IDC_PORTNUMBERA,EM_LINELENGTH,0,0);
				*((LPWORD)defPort) = chkLength;
				SendDlgItemMessage(hDlg,IDC_PORTNUMBERA,EM_GETLINE,0,(LPARAM)defPort);
				if(atoi(defPort)<1024||atoi(defPort)>65535)
				{
					MessageBox(hDlg,"Port Number needs to be betweeb 1024 and 65535","Error",MB_OK);
					break;
				}
				else
				{
					Settings.Port = atoi(defPort);
				}
				//MessageBox(hDlg,defPort,defPort,MB_OK);	
				Settings.NetPlayServer = IsDlgButtonChecked(hDlg,IDC_ACTASSERVER);
				NPServer.SendROMImageOnConnect = IsDlgButtonChecked(hDlg,IDC_SENDROM);
				NPServer.SyncByReset = IsDlgButtonChecked(hDlg,IDC_SYNCBYRESET);
				
				EndDialog(hDlg,0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				return TRUE;
			}
		case IDCANCEL:
			{
				EndDialog(hDlg,0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}
#endif
HRESULT CALLBACK EnumModesCallback( LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext)
{
	char depmode[80];
	char s[80];
	dMode curmode;
	HWND hDlg = (HWND)lpContext;
	int index;
	HKEY hKey;
	DWORD type;
	BYTE val[4];
	DWORD nuv=4;
	type=REG_DWORD;
	
	
	
	if( (lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount != 8 &&
		lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount != 15 &&
		lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount != 16) ||
        (lpDDSurfaceDesc->dwWidth < SNES_WIDTH ||
		lpDDSurfaceDesc->dwHeight < SNES_HEIGHT_EXTENDED))
	{
        return DDENUMRET_OK;
	}
	sprintf( depmode, "%dx%dx%d", lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight, lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount);
	RegOpenKeyEx(HKEY_CURRENT_USER,MY_REG_KEY  "\\1.x\\DisplayModes",0,KEY_ALL_ACCESS, &hKey);
	
	
	
	curmode.width=lpDDSurfaceDesc->dwWidth;
	curmode.height=lpDDSurfaceDesc->dwHeight;
	curmode.depth=lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;
	if(RegQueryValueEx(hKey,depmode,0,&type,val,&nuv)==ERROR_SUCCESS)
	{
		//sprintf(s,"%d %d %d %d",val[0],val[1],val[2],val[3]);
		//MessageBox(hDlg,s,temp,MB_OK);
		switch(val[0])
		{
		case 1:
			strcpy(s,"Failed");
			curmode.status=1;
			break;
		case 3:
			strcpy(s,"Works");
			curmode.status=3;
			break;
		default:
			strcpy(s,"Untested");
			curmode.status=0;
		}
	}
	else
	{
		strcpy(s,"Untested");
		curmode.status=0;
		//MessageBox(hDlg,s,temp,MB_OK);
	}
	dm.push_back(curmode);
	LVITEM lvi;
	ZeroMemory(&lvi, sizeof(LVITEM));
	lvi.iItem=dm.size();
	lvi.mask=LVIF_TEXT;
	lvi.pszText=depmode;
	lvi.cchTextMax=80;
	//lvi.lParam=dmindex;
	index=ListView_InsertItem(hDlg, &lvi);

	

	//dmindex++;
	ZeroMemory(&lvi, sizeof(LVITEM));
	lvi.mask=LVIF_TEXT;
	lvi.iItem=index;
	lvi.iSubItem=1;
	lvi.pszText=s;
	lvi.cchTextMax=10;
	ListView_SetItem(hDlg, &lvi);
	RegCloseKey(hKey);
	
	return DDENUMRET_OK;
}


int CALLBACK DlgFunky(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	HKEY hKey;
	int index;
	static bool state = false;
	char s[80],temp[80];

	
	
	
	if(RegCreateKeyEx(HKEY_CURRENT_USER,MY_REG_KEY "\\1.x\\DisplayModes",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS, NULL, &hKey,NULL) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		
	}
	
	switch(msg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint (hDlg, &ps);
			if(hBmp)
			{
				BITMAP bmp;
				ZeroMemory(&bmp, sizeof(BITMAP));
				RECT r;
				GetClientRect(hDlg, &r);
				HDC hdc=GetDC(hDlg);
				HDC hDCbmp=CreateCompatibleDC(hdc);
				GetObject(hBmp, sizeof(BITMAP), &bmp);
				HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
				StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
				SelectObject(hDCbmp, hOldBmp);
				DeleteDC(hDCbmp);
				ReleaseDC(hDlg, hdc);
			}
			
			EndPaint (hDlg, &ps);
		}
		return true;

	case WM_INITDIALOG:
		hBmp=(HBITMAP)LoadImage(NULL, TEXT("lantus.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		sprintf(s,"Current Mode: %dx%dx%d",GUI.Width,GUI.Height,GUI.Depth);
		SendDlgItemMessage(hDlg,IDC_CURRMODE,WM_SETTEXT,0,(LPARAM)s);
		if(DirectX.lpDD==NULL)
		{
			DirectDrawCreate( NULL, &DirectX.lpDD, NULL);
		}
		
		if(GUI.DoubleBuffered)
		{
			SendDlgItemMessage(hDlg, IDC_DBLBUFFER, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		SendDlgItemMessage(hDlg,IDC_FRAMERATESKIPSLIDER,TBM_SETRANGE,(WPARAM)true,(LPARAM)MAKELONG(0,9));
		if(Settings.SkipFrames!=AUTO_FRAMERATE)
		{
			SendDlgItemMessage(hDlg,IDC_FRAMERATESKIPSLIDER,TBM_SETPOS,(WPARAM)true,(LPARAM)Settings.SkipFrames);
		}
		EnableWindow(GetDlgItem(hDlg, IDC_TRANS), FALSE);
		if(Settings.SixteenBit)
		{
			SendDlgItemMessage(hDlg, IDC_RENDER16BIT, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			EnableWindow(GetDlgItem(hDlg, IDC_TRANS), TRUE);
		}
		if(Settings.Transparency)
		{
			SendDlgItemMessage(hDlg, IDC_TRANS, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		
		if(Settings.SupportHiRes)
		{
			SendDlgItemMessage(hDlg, IDC_HIRES, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		if(Settings.Mode7Interpolate)
		{
			SendDlgItemMessage(hDlg, IDC_BILINEARMD7, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		if(Settings.SkipFrames==AUTO_FRAMERATE)
		{
			SendDlgItemMessage(hDlg, IDC_AUTOFRAME, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		if(GUI.Stretch)
		{
			SendDlgItemMessage(hDlg, IDC_STRETCH, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		if(GUI.FullScreen)
		{
			SendDlgItemMessage(hDlg, IDC_FULLSCREEN, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		if(GUI.VideoMemory)
		{
			SendDlgItemMessage(hDlg,IDC_VIDEOCARD, BM_SETCHECK, (WPARAM)BST_CHECKED,0);
		}
		if(Settings.DisplayFrameRate)
		{
			SendDlgItemMessage(hDlg, IDC_SHOWFPS, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		if(Settings.AutoMaxSkipFrames==0)
		{
			SendDlgItemMessage(hDlg, IDC_LIMITFRAMES, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		}
		if(IsDlgButtonChecked(hDlg,IDC_AUTOFRAME))
		{
			EnableWindow(GetDlgItem(hDlg, IDC_LIMITFRAMES), TRUE);
		}
		

		strcpy(temp,"None");
		SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
		if(Settings.SixteenBit)
		{
		
			strcpy(temp,"Normal");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
			/* Forward thinking by Gary?
			strcpy(temp,"Scale X2");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
			*/
			strcpy(temp,"Scanlines");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
			strcpy(temp,"Kreed's SuperEagle");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
			strcpy(temp,"Kreed's 2xSaI");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
			strcpy(temp,"Kreed's Super2xSaI");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
			strcpy(temp,"TV Mode");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
#ifdef USE_GLIDE
			strcpy(temp,"3D/FX bi-linear");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
#endif
#ifdef USE_OPENGL
			strcpy(temp,"OpenGL bi-linear");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
#endif
			
		}
		else
		{
			strcpy(temp,"Normal");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
			strcpy(temp,"Scanlines");
			SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
		}
		SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_SETCURSEL,(WPARAM)GUI.NextScale,0);
		
		if(GUI.Width >= 512 && GUI.Height >= 478)
		{
			EnableWindow(GetDlgItem(hDlg,IDC_FILTERBOX),true);
			EnableWindow(GetDlgItem(hDlg,IDC_HIRES),true);
		}
		else
		{
			EnableWindow(GetDlgItem(hDlg,IDC_FILTERBOX),false);
			EnableWindow(GetDlgItem(hDlg,IDC_HIRES),false);
		}




		
		
		LVCOLUMN col;
		
		
		strcpy(s,"Status");
		ZeroMemory(&col, sizeof(LVCOLUMN));
		col.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
		col.fmt=LVCFMT_LEFT;
		col.iOrder=0;
		col.cx=60;
		col.cchTextMax=80;
		col.pszText=s;
		
		ListView_InsertColumn(GetDlgItem(hDlg,IDC_VIDMODELIST),    1,   &col);
		
		
		strcpy(s,"Video Mode");
		ZeroMemory(&col, sizeof(LVCOLUMN));
		col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
		col.fmt=LVCFMT_LEFT;
		col.iOrder=0;
		col.cx=125;
		col.cchTextMax=80;
		col.pszText=s;
		
		ListView_InsertColumn(GetDlgItem(hDlg,IDC_VIDMODELIST),    0,   &col);
		
		DirectX.lpDD->EnumDisplayModes(0,NULL,GetDlgItem(hDlg,IDC_VIDMODELIST),(LPDDENUMMODESCALLBACK)EnumModesCallback);
		

		break;
	case WM_CLOSE:
	case WM_DESTROY:
//		dmindex=0;
		break;
	case WM_COMMAND:
		
		switch(LOWORD(wParam))
		{
		case IDC_AUTOFRAME:
			if(BN_CLICKED==HIWORD(wParam)||BN_DBLCLK==HIWORD(wParam))
			{
				if(IsDlgButtonChecked(hDlg,IDC_AUTOFRAME))
				{
					EnableWindow(GetDlgItem(hDlg, IDC_LIMITFRAMES), TRUE);
				}
				else EnableWindow(GetDlgItem(hDlg, IDC_LIMITFRAMES), FALSE);
				return true;
						
			}
			else return false;

			break;
		case IDC_RENDER16BIT:

			if(BN_CLICKED==HIWORD(wParam)||BN_DBLCLK==HIWORD(wParam))
			{
				SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_RESETCONTENT,0,0);
				strcpy(temp,"None");
				SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
				if(IsDlgButtonChecked(hDlg,IDC_RENDER16BIT))
				{
					EnableWindow(GetDlgItem(hDlg, IDC_TRANS), TRUE);
					strcpy(temp,"Normal");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					/* Forward thinking by Gary?
					strcpy(temp,"Scale X2");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					*/
					strcpy(temp,"Scanlines");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					strcpy(temp,"Kreed's SuperEagle");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					strcpy(temp,"Kreed's 2xSaI");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					strcpy(temp,"Kreed's Super2xSaI");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					strcpy(temp,"TV Mode");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
#ifdef USE_GLIDE
					strcpy(temp,"3D/FX bi-linear");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
#endif
#ifdef USE_OPENGL
					strcpy(temp,"OpenGL bi-linear");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_SETCURSEL,(WPARAM)GUI.NextScale,0);
#endif
					
					
				}
				else
				{
					EnableWindow(GetDlgItem(hDlg, IDC_TRANS), FALSE);
					SendDlgItemMessage(hDlg, IDC_TRANS, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					strcpy(temp,"Normal");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					strcpy(temp,"Scanlines");
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
					SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_SETCURSEL,(WPARAM)0,0);
				}
				
				return true;
						
			}
			else return false;

			break;

			
		case IDOK:
			Settings.SixteenBit = IsDlgButtonChecked(hDlg, IDC_RENDER16BIT);
			Settings.Transparency = IsDlgButtonChecked(hDlg, IDC_TRANS);
			GUI.VideoMemory = (bool)(IsDlgButtonChecked(hDlg,IDC_VIDEOCARD)==BST_CHECKED);
			if(GUI.Width >= 512 && GUI.Height >= 478)
			{
				Settings.SupportHiRes = IsDlgButtonChecked(hDlg, IDC_HIRES);
				GUI.NextScale = (int)SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_GETCURSEL,0,0);
			}
			else
			{
				Settings.SupportHiRes = false;
				GUI.NextScale = 0;
			}
			Settings.Mode7Interpolate = IsDlgButtonChecked(hDlg, IDC_BILINEARMD7);
			Settings.AutoMaxSkipFrames = IsDlgButtonChecked(hDlg, IDC_AUTOFRAME);
			GUI.DoubleBuffered = (bool)(IsDlgButtonChecked(hDlg, IDC_DBLBUFFER)==BST_CHECKED);
			if(IsDlgButtonChecked(hDlg, IDC_AUTOFRAME))
			{
				Settings.SkipFrames=AUTO_FRAMERATE;
			}
			else
			{
				Settings.SkipFrames=(uint32)SendDlgItemMessage(hDlg,IDC_FRAMERATESKIPSLIDER,TBM_GETPOS,0,0);
			}
			if(IsDlgButtonChecked(hDlg, IDC_AUTOFRAME)&&IsDlgButtonChecked(hDlg, IDC_LIMITFRAMES))
			{
				Settings.AutoMaxSkipFrames=0;
			}
			else
			{
				if(Settings.AutoMaxSkipFrames==0)
				{
					Settings.AutoMaxSkipFrames=10;
				}
			}
			
			
			GUI.Stretch = (bool)(IsDlgButtonChecked(hDlg, IDC_STRETCH)==BST_CHECKED);
			GUI.FullScreen = (bool)(IsDlgButtonChecked(hDlg, IDC_FULLSCREEN)==BST_CHECKED);
			Settings.DisplayFrameRate = IsDlgButtonChecked(hDlg, IDC_SHOWFPS);
			
//			dmindex=0;
			EndDialog(hDlg,0);
			if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
			return false;
			
			
			
		case IDCANCEL:
//			dmindex=0;
			EndDialog(hDlg,0);
			if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
			return false;
		case IDC_TESTMODE:
			
			//DirectX.lpDD->SetCooperativeLevel(hDlg,DDSCL_FULLSCREEN|DDSCL_ALLOWMODEX|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT);
			index=ListView_GetSelectionMark(GetDlgItem(hDlg,IDC_VIDMODELIST));
			if(index != -1)
			{
				sprintf(temp,"%dx%dx%d",dm.at(index).width,dm.at(index).height,dm.at(index).depth);
				if(DirectX.lpDD->SetDisplayMode(dm.at(index).width,dm.at(index).height,dm.at(index).depth)!=DD_OK)
				{
					
					MessageBox(hDlg,"There was an error testing the selected mode","DD_NOTOK",MB_OK);
					LVITEM lvi;
					dm.at(index).status=1;
					strcpy(s,"Failed");
					ZeroMemory(&lvi, sizeof(LVITEM));
					lvi.mask=LVIF_TEXT;
					lvi.iItem=index;
					lvi.iSubItem=1;
					lvi.pszText=s;
					lvi.cchTextMax=10;
					ListView_SetItem(GetDlgItem(hDlg,IDC_VIDMODELIST), &lvi);
					if(RegOpenKeyEx(HKEY_CURRENT_USER,MY_REG_KEY  "\\1.x\\DisplayModes",0,KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
					{
						
						RegSetValueEx(hKey,temp,0,REG_DWORD,(LPBYTE)&dm.at(index).status,sizeof(DWORD));
						
						RegCloseKey(hKey);
						
					}
				}
				else
				{
					


					
					if(MessageBox(hDlg,"Did it Work?","Test Display Mode",MB_YESNO)==IDYES)
					{	
						dm.at(index).status=3;
						LVITEM lvi;
						strcpy(s,"Works");
						ZeroMemory(&lvi, sizeof(LVITEM));
						lvi.mask=LVIF_TEXT;
						lvi.iItem=index;
						lvi.iSubItem=1;
						lvi.pszText=s;
						lvi.cchTextMax=10;
						ListView_SetItem(GetDlgItem(hDlg,IDC_VIDMODELIST), &lvi);
						if(RegOpenKeyEx(HKEY_CURRENT_USER,MY_REG_KEY  "\\1.x\\DisplayModes",0,KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
						{
							
							if(RegSetValueEx(hKey,temp,0,REG_DWORD,(LPBYTE)&dm.at(index).status,sizeof(DWORD))==ERROR_SUCCESS)
							{
								
							}
							
							RegCloseKey(hKey);
							
						}
					}
					else
					{
						dm.at(index).status=1;
						LVITEM lvi;
						strcpy(s,"Failed");
						ZeroMemory(&lvi, sizeof(LVITEM));
						lvi.mask=LVIF_TEXT;
						lvi.iItem=index;
						lvi.iSubItem=1;
						lvi.pszText=s;
						lvi.cchTextMax=10;
						ListView_SetItem(GetDlgItem(hDlg,IDC_VIDMODELIST), &lvi);
						if(RegOpenKeyEx(HKEY_CURRENT_USER,
							MY_REG_KEY  "\\1.x\\DisplayModes",
							0,KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
						{
							RegSetValueEx(hKey,temp,0,REG_DWORD,(LPBYTE)&dm.at(index).status,sizeof(DWORD));
							
							RegCloseKey(hKey);
							
						}

					}
				
					
				}

		
				DirectX.lpDD->SetDisplayMode(GUI.Width,GUI.Height,GUI.Depth);
			}
			else
			{
				MessageBox(hDlg,"Please select a mode to test","No Mode Selected",MB_OK);
				
			}
			return false;
		case IDC_SETDMODE:
			bool go;


			index=ListView_GetSelectionMark(GetDlgItem(hDlg,IDC_VIDMODELIST));
			
			switch (dm.at(index).status)
			{
			case 3:
				if(MessageBox(hDlg,"Are you sure?","Confirm Set Display Mode",MB_YESNO)==IDYES)
				{
					go=true;
				}
				else
				{
					go=false;
				}
				break;
			case 1:
				if(MessageBox(hDlg,"This mode failed in testing, are you sure?","Confirm Set Display Mode",MB_YESNO)==IDYES)
				{
					go=true;
				}
				else
				{
					go=false;
				}
				break;
			case 0:
				if(MessageBox(hDlg,"This Mode Hasn't been tested, are you sure?","Confirm Set Display Mode",MB_YESNO)==IDYES)
				{
					go=true;
				}
				else
				{
					go=false;
				}
				break;
			}
			if(go)
			{
				
				if(RegOpenKeyEx(HKEY_CURRENT_USER,
					MY_REG_KEY  "\\1.x",
					0,KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS)
				{
					RegSetValueEx(hKey,"Width",0,REG_DWORD,(LPBYTE)&dm.at(index).width,sizeof(DWORD));
					RegSetValueEx(hKey,"Height",0,REG_DWORD,(LPBYTE)&dm.at(index).height,sizeof(DWORD));
					RegSetValueEx(hKey,"Depth",0,REG_DWORD,(LPBYTE)&dm.at(index).depth,sizeof(DWORD));
					RegCloseKey(hKey);
					
				}
				
				GUI.Width=dm.at(index).width;
				GUI.Height=dm.at(index).height;
				GUI.Depth=dm.at(index).depth;
			}
			if(GUI.Width >= 512 && GUI.Height >= 478)
			{
				EnableWindow(GetDlgItem(hDlg,IDC_FILTERBOX),true);
				EnableWindow(GetDlgItem(hDlg,IDC_HIRES),true);
			}
			else
			{
				EnableWindow(GetDlgItem(hDlg,IDC_FILTERBOX),false);
				EnableWindow(GetDlgItem(hDlg,IDC_HIRES),false);
			}
			sprintf(s,"Current Mode: %dx%dx%d",GUI.Width,GUI.Height,GUI.Depth);
			SendDlgItemMessage(hDlg,IDC_CURRMODE,WM_SETTEXT,0,(LPARAM)s);
			return false;
			
			
		}
		
		
	}
	
	return false;
}
void TranslateKey(WORD keyz,char *out);
//HWND funky;
SJoyState JoystickF [16];


#ifdef NETPLAY_SUPPORT
int CALLBACK DlgNPProgress(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendDlgItemMessage(hDlg,IDC_NPPROGRESS,PBM_SETRANGE,0,(LPARAM)MAKELPARAM (0, 100));
	SendDlgItemMessage(hDlg,IDC_NPPROGRESS,PBM_SETPOS,(WPARAM)(int)NetPlay.PercentageComplete,0);

	return false;
}
#endif
int CALLBACK DlgInputConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	char temp[256];
	short C;
	int i, which;
	static int index=0;
	
	
	static SJoypad pads[5];
	
	
	//HBRUSH g_hbrBackground;

	InitInputCustomControl();
switch(msg)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint (hDlg, &ps);
			if(hBmp)
			{
				BITMAP bmp;
				ZeroMemory(&bmp, sizeof(BITMAP));
				RECT r;
				GetClientRect(hDlg, &r);
				HDC hdc=GetDC(hDlg);
				HDC hDCbmp=CreateCompatibleDC(hdc);
				GetObject(hBmp, sizeof(BITMAP), &bmp);
				HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
				StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
				SelectObject(hDCbmp, hOldBmp);
				DeleteDC(hDCbmp);
				ReleaseDC(hDlg, hdc);
			}
			
			EndPaint (hDlg, &ps);
		}
		return true;
	case WM_INITDIALOG:
	SetWindowText(hDlg,INPUTCONFIG_TITLE);
		SetDlgItemText(hDlg,IDC_JPTOGGLE,INPUTCONFIG_JPTOGGLE);
		SetDlgItemText(hDlg,IDC_OK,BUTTON_OK);
		SetDlgItemText(hDlg,IDC_CANCEL,BUTTON_CANCEL);
		SetDlgItemText(hDlg,IDC_DIAGTOGGLE,INPUTCONFIG_DIAGTOGGLE);
		SetDlgItemText(hDlg,IDC_LABEL_UP,INPUTCONFIG_LABEL_UP);
		SetDlgItemText(hDlg,IDC_LABEL_DOWN,INPUTCONFIG_LABEL_DOWN);
		SetDlgItemText(hDlg,IDC_LABEL_LEFT,INPUTCONFIG_LABEL_LEFT);
		SetDlgItemText(hDlg,IDC_LABEL_RIGHT,INPUTCONFIG_LABEL_RIGHT);
		SetDlgItemText(hDlg,IDC_LABEL_A,INPUTCONFIG_LABEL_A);
		SetDlgItemText(hDlg,IDC_LABEL_B,INPUTCONFIG_LABEL_B);
		SetDlgItemText(hDlg,IDC_LABEL_X,INPUTCONFIG_LABEL_X);
		SetDlgItemText(hDlg,IDC_LABEL_Y,INPUTCONFIG_LABEL_Y);
		SetDlgItemText(hDlg,IDC_LABEL_L,INPUTCONFIG_LABEL_L);
		SetDlgItemText(hDlg,IDC_LABEL_R,INPUTCONFIG_LABEL_R);
		SetDlgItemText(hDlg,IDC_LABEL_START,INPUTCONFIG_LABEL_START);
		SetDlgItemText(hDlg,IDC_LABEL_SELECT,INPUTCONFIG_LABEL_SELECT);
		SetDlgItemText(hDlg,IDC_LABEL_UPRIGHT,INPUTCONFIG_LABEL_UPRIGHT);
		SetDlgItemText(hDlg,IDC_LABEL_UPLEFT,INPUTCONFIG_LABEL_UPLEFT);
		SetDlgItemText(hDlg,IDC_LABEL_DOWNRIGHT,INPUTCONFIG_LABEL_DOWNRIGHT);
		SetDlgItemText(hDlg,IDC_LABEL_DOWNRIGHT,INPUTCONFIG_LABEL_DOWNRIGHT);
		SetDlgItemText(hDlg,IDC_LABEL_BLUE,INPUTCONFIG_LABEL_BLUE);

		hBmp=(HBITMAP)LoadImage(NULL, TEXT("PBortas.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		memcpy(pads, Joypad, 5*sizeof(SJoypad));
		for( i=0;i<256;i++)
		{
			GetAsyncKeyState(i);
		}
		for( C = 0; C != 16; C ++)
        JoystickF[C].Attached = joyGetDevCaps( JOYSTICKID1+C, &JoystickF[C].Caps, sizeof( JOYCAPS)) == JOYERR_NOERROR;
		for (i=1;i<6;i++)
		{
			
			sprintf(temp,INPUTCONFIG_JPCOMBO,i);
			SendDlgItemMessage(hDlg,IDC_JPCOMBO,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)temp);
		}

		SendDlgItemMessage(hDlg,IDC_JPCOMBO,CB_SETCURSEL,(WPARAM)0,0);
		if(Joypad[index].Enabled)
		{
			SendDlgItemMessage(hDlg,IDC_JPTOGGLE,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
		}
		else
		{
			SendDlgItemMessage(hDlg,IDC_JPTOGGLE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
		}
		SendDlgItemMessage(hDlg,IDC_UP,WM_USER+44,Joypad[index].Up,0);
		SendDlgItemMessage(hDlg,IDC_LEFT,WM_USER+44,Joypad[index].Left,0);
		SendDlgItemMessage(hDlg,IDC_DOWN,WM_USER+44,Joypad[index].Down,0);
		SendDlgItemMessage(hDlg,IDC_RIGHT,WM_USER+44,Joypad[index].Right,0);
		SendDlgItemMessage(hDlg,IDC_A,WM_USER+44,Joypad[index].A,0);
		SendDlgItemMessage(hDlg,IDC_B,WM_USER+44,Joypad[index].B,0);
		SendDlgItemMessage(hDlg,IDC_X,WM_USER+44,Joypad[index].X,0);
		SendDlgItemMessage(hDlg,IDC_Y,WM_USER+44,Joypad[index].Y,0);
		SendDlgItemMessage(hDlg,IDC_L,WM_USER+44,Joypad[index].L,0);
		SendDlgItemMessage(hDlg,IDC_R,WM_USER+44,Joypad[index].R,0);
		SendDlgItemMessage(hDlg,IDC_START,WM_USER+44,Joypad[index].Start,0);
		SendDlgItemMessage(hDlg,IDC_SELECT,WM_USER+44,Joypad[index].Select,0);
		SendDlgItemMessage(hDlg,IDC_UPLEFT,WM_USER+44,Joypad[index].Left_Up,0);
		SendDlgItemMessage(hDlg,IDC_UPRIGHT,WM_USER+44,Joypad[index].Right_Up,0);
		SendDlgItemMessage(hDlg,IDC_DWNLEFT,WM_USER+44,Joypad[index].Left_Down,0);
		SendDlgItemMessage(hDlg,IDC_DWNRIGHT,WM_USER+44,Joypad[index].Right_Down,0);

		
		SetFocus(GetDlgItem(hDlg,IDC_JPCOMBO));
		
		
		return true;
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	case WM_USER+43:
		//MessageBox(hDlg,"USER+43 CAUGHT","moo",MB_OK);
		which = GetDlgCtrlID((HWND)lParam);
		switch(which)
		{
		case IDC_UP:
			Joypad[index].Up = wParam;

			break;
		case IDC_DOWN:
			Joypad[index].Down = wParam;

			break;
		case IDC_LEFT:
			Joypad[index].Left = wParam;

			break;
		case IDC_RIGHT:
			Joypad[index].Right = wParam;

			break;
		case IDC_A:
			Joypad[index].A = wParam;

			break;
		case IDC_B:
			Joypad[index].B = wParam;

			break;
		case IDC_X:
			Joypad[index].X = wParam;

			break;		
		case IDC_Y:
			Joypad[index].Y = wParam;

			break;
		case IDC_L:
			Joypad[index].L = wParam;
			break;

		case IDC_R:
			Joypad[index].R = wParam;
	
			break;
		case IDC_SELECT:
			Joypad[index].Select = wParam;

			break;
		case IDC_START:
			Joypad[index].Start = wParam;

			break;
		case IDC_UPLEFT:
			Joypad[index].Left_Up = wParam;

			break;
		case IDC_UPRIGHT:
			Joypad[index].Right_Up = wParam;

			break;
		case IDC_DWNLEFT:
			Joypad[index].Left_Down = wParam;

		case IDC_DWNRIGHT:
			Joypad[index].Right_Down = wParam;

			break;

		}
		SendDlgItemMessage(hDlg,IDC_UP,WM_USER+44,Joypad[index].Up,0);
		SendDlgItemMessage(hDlg,IDC_LEFT,WM_USER+44,Joypad[index].Left,0);
		SendDlgItemMessage(hDlg,IDC_DOWN,WM_USER+44,Joypad[index].Down,0);
		SendDlgItemMessage(hDlg,IDC_RIGHT,WM_USER+44,Joypad[index].Right,0);
		SendDlgItemMessage(hDlg,IDC_A,WM_USER+44,Joypad[index].A,0);
		SendDlgItemMessage(hDlg,IDC_B,WM_USER+44,Joypad[index].B,0);
		SendDlgItemMessage(hDlg,IDC_X,WM_USER+44,Joypad[index].X,0);
		SendDlgItemMessage(hDlg,IDC_Y,WM_USER+44,Joypad[index].Y,0);
		SendDlgItemMessage(hDlg,IDC_L,WM_USER+44,Joypad[index].L,0);
		SendDlgItemMessage(hDlg,IDC_R,WM_USER+44,Joypad[index].R,0);
		SendDlgItemMessage(hDlg,IDC_START,WM_USER+44,Joypad[index].Start,0);
		SendDlgItemMessage(hDlg,IDC_SELECT,WM_USER+44,Joypad[index].Select,0);
		SendDlgItemMessage(hDlg,IDC_UPLEFT,WM_USER+44,Joypad[index].Left_Up,0);
		SendDlgItemMessage(hDlg,IDC_UPRIGHT,WM_USER+44,Joypad[index].Right_Up,0);
		SendDlgItemMessage(hDlg,IDC_DWNLEFT,WM_USER+44,Joypad[index].Left_Down,0);
		SendDlgItemMessage(hDlg,IDC_DWNRIGHT,WM_USER+44,Joypad[index].Right_Down,0);
		PostMessage(hDlg,WM_NEXTDLGCTL,0,0);
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			memcpy(Joypad, pads, 5*sizeof(SJoypad));
			EndDialog(hDlg,0);
			if(hBmp)
			{
				DeleteObject(hBmp);
				hBmp=NULL;
			}
			break;
		case IDOK:
			EndDialog(hDlg,0);
			if(hBmp)
			{
				DeleteObject(hBmp);
				hBmp=NULL;
			}
			break;
		case IDC_JPTOGGLE:
			index = SendDlgItemMessage(hDlg,IDC_JPCOMBO,CB_GETCURSEL,0,0);
			Joypad[index].Enabled=IsDlgButtonChecked(hDlg,IDC_JPTOGGLE);
			break;
		case IDC_DIAGTOGGLE:
			if(IsWindowEnabled(GetDlgItem(hDlg,IDC_UPLEFT))==0)
			{
			
				EnableWindow(GetDlgItem(hDlg,IDC_UPLEFT),TRUE);
			
				EnableWindow(GetDlgItem(hDlg,IDC_UPRIGHT),TRUE);
			
				EnableWindow(GetDlgItem(hDlg,IDC_DWNLEFT),TRUE);
			
				EnableWindow(GetDlgItem(hDlg,IDC_DWNRIGHT),TRUE);

			}
			else
			{
			
				EnableWindow(GetDlgItem(hDlg,IDC_UPLEFT),FALSE);

				EnableWindow(GetDlgItem(hDlg,IDC_UPRIGHT),FALSE);
		
				EnableWindow(GetDlgItem(hDlg,IDC_DWNLEFT),FALSE);
			
				EnableWindow(GetDlgItem(hDlg,IDC_DWNRIGHT),FALSE);
			}
			break;
		}
		switch(HIWORD(wParam))
		{
			case CBN_SELCHANGE:
				index = SendDlgItemMessage(hDlg,IDC_JPCOMBO,CB_GETCURSEL,0,0);
				SendDlgItemMessage(hDlg,IDC_JPCOMBO,CB_SETCURSEL,(WPARAM)index,0);
				if(Joypad[index].Enabled)
				{
					SendDlgItemMessage(hDlg,IDC_JPTOGGLE,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				}
				else
				{
					SendDlgItemMessage(hDlg,IDC_JPTOGGLE,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
				} 
				SendDlgItemMessage(hDlg,IDC_UP,WM_USER+44,Joypad[index].Up,0);
				SendDlgItemMessage(hDlg,IDC_LEFT,WM_USER+44,Joypad[index].Left,0);
				SendDlgItemMessage(hDlg,IDC_DOWN,WM_USER+44,Joypad[index].Down,0);
				SendDlgItemMessage(hDlg,IDC_RIGHT,WM_USER+44,Joypad[index].Right,0);
				SendDlgItemMessage(hDlg,IDC_A,WM_USER+44,Joypad[index].A,0);
				SendDlgItemMessage(hDlg,IDC_B,WM_USER+44,Joypad[index].B,0);
				SendDlgItemMessage(hDlg,IDC_X,WM_USER+44,Joypad[index].X,0);
				SendDlgItemMessage(hDlg,IDC_Y,WM_USER+44,Joypad[index].Y,0);
				SendDlgItemMessage(hDlg,IDC_L,WM_USER+44,Joypad[index].L,0);
				SendDlgItemMessage(hDlg,IDC_R,WM_USER+44,Joypad[index].R,0);
				SendDlgItemMessage(hDlg,IDC_START,WM_USER+44,Joypad[index].Start,0);
				SendDlgItemMessage(hDlg,IDC_SELECT,WM_USER+44,Joypad[index].Select,0);
				SendDlgItemMessage(hDlg,IDC_UPLEFT,WM_USER+44,Joypad[index].Left_Up,0);
				SendDlgItemMessage(hDlg,IDC_UPRIGHT,WM_USER+44,Joypad[index].Right_Up,0);
				SendDlgItemMessage(hDlg,IDC_DWNLEFT,WM_USER+44,Joypad[index].Left_Down,0);
				SendDlgItemMessage(hDlg,IDC_DWNRIGHT,WM_USER+44,Joypad[index].Right_Down,0);
				break;
		}
		return FALSE;

	}

	return FALSE;
}



enum CheatStatus{
	Untouched,
	Deleted,
	Modified
};
typedef struct{
int* index;
DWORD* state;
}CheatTracker;

#define ITEM_QUERY(a, b, c, d, e)  ZeroMemory(&a, sizeof(LV_ITEM)); \
						a.iItem= ListView_GetSelectionMark(GetDlgItem(hDlg, b)); \
						a.iSubItem=c; \
						a.mask=LVIF_TEXT; \
						a.pszText=d; \
						a.cchTextMax=e; \
						ListView_GetItem(GetDlgItem(hDlg, b), &a);

int CALLBACK DlgCheater(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	static bool internal_change;
	static bool has_sel;
	static int  sel_idx;
	static uint8 new_sel;
	static CheatTracker ct;
	switch(msg)
	{
	case WM_INITDIALOG:
			
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("funkyass.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			ListView_SetExtendedListViewStyle(GetDlgItem(hDlg, IDC_CHEAT_LIST), LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);

			SendDlgItemMessage(hDlg, IDC_CHEAT_CODE, EM_LIMITTEXT, 14, 0);
			SendDlgItemMessage(hDlg, IDC_CHEAT_DESCRIPTION, EM_LIMITTEXT, 22, 0);
			SendDlgItemMessage(hDlg, IDC_CHEAT_ADDRESS, EM_LIMITTEXT, 6, 0);
			SendDlgItemMessage(hDlg, IDC_CHEAT_BYTE, EM_LIMITTEXT, 3, 0);

			LVCOLUMN col;
			char temp[32];
			strcpy(temp,"Address");
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=0;
			col.cx=70;
			col.cchTextMax=7;
			col.pszText=temp;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_CHEAT_LIST),    0,   &col);
			
			strcpy(temp,"Value");
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=1;
			col.cx=43;
			col.cchTextMax=3;
			col.pszText=temp;
			col.iSubItem=1;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_CHEAT_LIST),    1,   &col);

			strcpy(temp,"Description");
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=2;
			col.cx=165;
			col.cchTextMax=32;
			col.pszText=temp;
			col.iSubItem=2;

			ListView_InsertColumn(GetDlgItem(hDlg,IDC_CHEAT_LIST),    2,   &col);
			
			ct.index=new int[Cheat.num_cheats];
			ct.state=new DWORD[Cheat.num_cheats];

			uint32 counter;
			for(counter=0; counter<Cheat.num_cheats; counter++)
			{
				char buffer[7];
				int curr_idx=-1;
				sprintf(buffer, "%06X", Cheat.c[counter].address);
				LVITEM lvi;
				ZeroMemory(&lvi, sizeof(LVITEM));
				lvi.mask=LVIF_TEXT;
				lvi.pszText=buffer;
				lvi.cchTextMax=7;
				lvi.iItem=counter;
				curr_idx=ListView_InsertItem(GetDlgItem(hDlg,IDC_CHEAT_LIST), &lvi);

				unsigned int k;
				for(k=0;k<counter;k++)
				{
					if(ct.index[k]>=curr_idx)
						ct.index[k]++;
				}
				ct.index[counter]=curr_idx;
				ct.state[counter]=Untouched;

				sprintf(buffer, "%02X", Cheat.c[counter].byte);
				ZeroMemory(&lvi, sizeof(LVITEM));
				lvi.iItem=curr_idx;
				lvi.iSubItem=1;
				lvi.mask=LVIF_TEXT;
				lvi.pszText=buffer;
				lvi.cchTextMax=3;
				SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);

				ZeroMemory(&lvi, sizeof(LVITEM));
				lvi.iItem=curr_idx;
				lvi.iSubItem=2;
				lvi.mask=LVIF_TEXT;
				lvi.pszText=Cheat.c[counter].name;
				lvi.cchTextMax=23;
				SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);

				ListView_SetCheckState(GetDlgItem(hDlg,IDC_CHEAT_LIST), curr_idx, Cheat.c[counter].enabled);

			}
		return true;
	case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;
	case WM_NOTIFY:
		{
			switch(wParam)
			{
			case IDC_CHEAT_LIST:
				if(0==ListView_GetSelectedCount(GetDlgItem(hDlg, IDC_CHEAT_LIST)))
				{
					EnableWindow(GetDlgItem(hDlg, IDC_DELETE_CHEAT), false);
					EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), false);
					has_sel=false;
					sel_idx=-1;
				}
				else
				{
					EnableWindow(GetDlgItem(hDlg, IDC_DELETE_CHEAT), true);
					if(!has_sel||sel_idx!=ListView_GetSelectionMark(GetDlgItem(hDlg, IDC_CHEAT_LIST)))
					{
						new_sel=3;
						//change
						char buf[25];
						LV_ITEM lvi;

						ITEM_QUERY (lvi, IDC_CHEAT_LIST, 0, buf, 7);

						SetDlgItemText(hDlg, IDC_CHEAT_ADDRESS, lvi.pszText);

						ITEM_QUERY (lvi, IDC_CHEAT_LIST, 1, &buf[strlen(buf)], 3);

						SetDlgItemText(hDlg, IDC_CHEAT_CODE, buf);
						char temp[4];
						int q;
						sscanf(lvi.pszText, "%02X", &q);
						sprintf(temp, "%d", q);
						SetDlgItemText(hDlg, IDC_CHEAT_BYTE, temp);
						
						ITEM_QUERY (lvi, IDC_CHEAT_LIST, 2, buf, 24);

						internal_change=true;
						SetDlgItemText(hDlg, IDC_CHEAT_DESCRIPTION, lvi.pszText);

					}
					sel_idx=ListView_GetSelectionMark(GetDlgItem(hDlg, IDC_CHEAT_LIST));
					has_sel=true;
				}

					return true;
			default: return false;
			}
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_CHEAT_DESCRIPTION:
				{
					switch(HIWORD(wParam))
					{
					case EN_CHANGE:
						
						if(internal_change)
						{
							internal_change=!internal_change;
							return false;
						}
						if(!has_sel)
							return true;
							EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), true);
							return true;
					}
					break;

				}
			case IDC_ADD_CHEAT:
				{
					char temp[24];
					GetDlgItemText(hDlg, IDC_CHEAT_CODE, temp, 23);
					if(strcmp(temp, ""))
					{
						int j;
						bool skip=false;
						int count=1;
						uint32 addy;
						uint8 byte[3];
						//test game genie
						if (NULL==S9xGameGenieToRaw (temp, addy, byte[0]))
							skip=true;
						//test goldfinger

					//	if(!skip

						//test PAR

						if(!skip)
						{
							if(NULL==S9xProActionReplayToRaw(temp, addy, byte[0]))
								skip=true;
						}

						if(!skip)
							return 0;

						for(j=0; j<count; j++)
						{
							char buffer[7];
							int curr_idx=-1;
							sprintf(buffer, "%06X", addy);
							LVITEM lvi;
							ZeroMemory(&lvi, sizeof(LVITEM));
							lvi.mask=LVIF_TEXT;
							lvi.pszText=buffer;
							lvi.cchTextMax=6;
							lvi.iItem=ListView_GetItemCount(GetDlgItem(hDlg,IDC_CHEAT_LIST));
							curr_idx=ListView_InsertItem(GetDlgItem(hDlg,IDC_CHEAT_LIST), &lvi);
							
							unsigned int k;
							for(k=0;k<Cheat.num_cheats;k++)
							{
								if(ct.index[k]>=curr_idx)
									ct.index[k]++;
							}


							sprintf(buffer, "%02X", byte[j]);
							ZeroMemory(&lvi, sizeof(LVITEM));
							lvi.iItem=curr_idx;
							lvi.iSubItem=1;
							lvi.mask=LVIF_TEXT;
							lvi.pszText=buffer;
							lvi.cchTextMax=2;
							SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);
							
							GetDlgItemText(hDlg, IDC_CHEAT_DESCRIPTION, temp, 23);
							
							ZeroMemory(&lvi, sizeof(LVITEM));
							lvi.iItem=curr_idx;
							lvi.iSubItem=2;
							lvi.mask=LVIF_TEXT;
							lvi.pszText=temp;
							lvi.cchTextMax=23;
							SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);
							
							addy++;
							
							
						}
					}
					else
					{
						uint8 byte;
						char buffer[7];
						
						GetDlgItemText(hDlg, IDC_CHEAT_ADDRESS, buffer, 7);
						
						int curr_idx=-1;
						LVITEM lvi;
						ZeroMemory(&lvi, sizeof(LVITEM));
						lvi.mask=LVIF_TEXT;
						lvi.pszText=buffer;
						lvi.cchTextMax=6;
						lvi.iItem=0;
						curr_idx=ListView_InsertItem(GetDlgItem(hDlg,IDC_CHEAT_LIST), &lvi);
						
						
						GetDlgItemText(hDlg, IDC_CHEAT_BYTE, buffer, 7);
						
						if(buffer[0]=='$')
							sscanf(buffer,"$%2X", &byte);
						else sscanf(buffer,"%d", &byte);
						
						sprintf(buffer, "%02X", byte);
						
						ZeroMemory(&lvi, sizeof(LVITEM));
						lvi.iItem=curr_idx;
						lvi.iSubItem=1;
						lvi.mask=LVIF_TEXT;
						lvi.pszText=buffer;
						lvi.cchTextMax=2;
						SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);
						
						GetDlgItemText(hDlg, IDC_CHEAT_DESCRIPTION, temp, 23);
						
						ZeroMemory(&lvi, sizeof(LVITEM));
						lvi.iItem=curr_idx;
						lvi.iSubItem=2;
						lvi.mask=LVIF_TEXT;
						lvi.pszText=temp;
						lvi.cchTextMax=23;
						SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);
					}
				}
				break;
			case IDC_UPDATE_CHEAT:
				{
									char temp[24];
					GetDlgItemText(hDlg, IDC_CHEAT_CODE, temp, 23);
					if(strcmp(temp, ""))
					{
						int j;
						bool skip=false;
						int count=1;
						uint32 addy;
						uint8 byte[3];
						//test game genie
						if (NULL==S9xGameGenieToRaw (temp, addy, byte[0]))
							skip=true;
						//test goldfinger

					//	if(!skip

						//test PAR

						if(!skip)
						{
							if(NULL==S9xProActionReplayToRaw(temp, addy, byte[0]))
								skip=true;
						}

						if(!skip)
							return 0;

						for(j=0;j<(int)Cheat.num_cheats;j++)
						{
							if(ct.index[j]==sel_idx)
								ct.state[j]=Modified;
						}

						for(j=0; j<count; j++)
						{
							char buffer[7];
							int curr_idx=-1;
							sprintf(buffer, "%06X", addy);
							LVITEM lvi;
							ZeroMemory(&lvi, sizeof(LVITEM));
							lvi.mask=LVIF_TEXT;
							lvi.pszText=buffer;
							lvi.cchTextMax=6;
							lvi.iItem=sel_idx;
							ListView_SetItem(GetDlgItem(hDlg,IDC_CHEAT_LIST), &lvi);
							
							sprintf(buffer, "%02X", byte[j]);
							ZeroMemory(&lvi, sizeof(LVITEM));
							lvi.iItem=sel_idx;
							lvi.iSubItem=1;
							lvi.mask=LVIF_TEXT;
							lvi.pszText=buffer;
							lvi.cchTextMax=2;
							SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);
							
							GetDlgItemText(hDlg, IDC_CHEAT_DESCRIPTION, temp, 23);
							
							ZeroMemory(&lvi, sizeof(LVITEM));
							lvi.iItem=sel_idx;
							lvi.iSubItem=2;
							lvi.mask=LVIF_TEXT;
							lvi.pszText=temp;
							lvi.cchTextMax=23;
							SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);
							
							addy++;
							
							
						}
					}
					else
					{
												uint8 byte;
						char buffer[7];
						
						GetDlgItemText(hDlg, IDC_CHEAT_ADDRESS, buffer, 7);
						int j;
						for(j=0;j<(int)Cheat.num_cheats;j++)
						{
							if(ct.index[j]==sel_idx)
								ct.state[j]=Modified;
						}

						int curr_idx=-1;
						LVITEM lvi;
						ZeroMemory(&lvi, sizeof(LVITEM));
						lvi.mask=LVIF_TEXT;
						lvi.pszText=buffer;
						lvi.cchTextMax=6;
						lvi.iItem=sel_idx;
						ListView_SetItem(GetDlgItem(hDlg,IDC_CHEAT_LIST), &lvi);
						
						
						GetDlgItemText(hDlg, IDC_CHEAT_BYTE, buffer, 7);
						
						if(buffer[0]=='$')
							sscanf(buffer,"$%2X", &byte);
						else sscanf(buffer,"%d", &byte);
						
						sprintf(buffer, "%02X", byte);
						
						ZeroMemory(&lvi, sizeof(LVITEM));
						lvi.iItem=sel_idx;
						lvi.iSubItem=1;
						lvi.mask=LVIF_TEXT;
						lvi.pszText=buffer;
						lvi.cchTextMax=2;
						SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);
						
						GetDlgItemText(hDlg, IDC_CHEAT_DESCRIPTION, temp, 23);
						
						ZeroMemory(&lvi, sizeof(LVITEM));
						lvi.iItem=sel_idx;
						lvi.iSubItem=2;
						lvi.mask=LVIF_TEXT;
						lvi.pszText=temp;
						lvi.cchTextMax=23;
						SendDlgItemMessage(hDlg,IDC_CHEAT_LIST, LVM_SETITEM, 0, (LPARAM)&lvi);
					}
				}
				
				break;
			case IDC_DELETE_CHEAT:
				{
					unsigned int j;
				for(j=0;j<Cheat.num_cheats;j++)
				{
					if(ct.index[j]==sel_idx)
						ct.state[j]=Deleted;
				}
				for(j=0;j<Cheat.num_cheats;j++)
				{
					if(ct.index[j]>sel_idx)
						ct.index[j]--;
				}
				}
				ListView_DeleteItem(GetDlgItem(hDlg, IDC_CHEAT_LIST), sel_idx);
				
				break;
			case IDC_CLEAR_CHEATS:
				internal_change = true;
				SetDlgItemText(hDlg,IDC_CHEAT_CODE,"");
				SetDlgItemText(hDlg,IDC_CHEAT_ADDRESS,"");
				SetDlgItemText(hDlg,IDC_CHEAT_BYTE,"");
				SetDlgItemText(hDlg,IDC_CHEAT_DESCRIPTION,"");
				ListView_SetItemState(GetDlgItem(hDlg, IDC_CHEAT_LIST),sel_idx, 0, LVIS_SELECTED|LVIS_FOCUSED);
				ListView_SetSelectionMark(GetDlgItem(hDlg, IDC_CHEAT_LIST), -1);
				sel_idx=-1;
				has_sel=false;
				break;
			case IDC_CHEAT_CODE:
				{
					uint32 j, k;
					long index;
					char buffer[15];
					char buffer2[15];
					POINT point;
					switch(HIWORD(wParam))
					{
					case EN_CHANGE:
						if(internal_change)
						{
							internal_change=false;
							return true;
						}
						SendMessage((HWND)lParam, WM_GETTEXT, 15,(LPARAM)buffer);
						GetCaretPos(&point);

						index = SendMessage((HWND)lParam,(UINT) EM_CHARFROMPOS, 0, (LPARAM) ((point.x&0x0000FFFF) | (((point.y&0x0000FFFF))<<16)));  

						k=0;
						for(j=0; j<strlen(buffer);j++)
						{
							if( (buffer[j]>='0' && buffer[j]<='9') || (buffer[j]>='A' && buffer[j]<='F') || buffer[j]=='-' || buffer[j]=='X')
							{
								buffer2[k]=buffer[j];
								k++;
							}
							else index --;
						}
						buffer2[k]='\0';
						
						if(has_sel&&!new_sel&&strlen(buffer2)!=0)
						{
							SetDlgItemText(hDlg, IDC_CHEAT_ADDRESS, "");
							SetDlgItemText(hDlg, IDC_CHEAT_BYTE, "");

						}

						if(new_sel!=0)
							new_sel--;

						internal_change=true;
						SendMessage((HWND)lParam, WM_SETTEXT, 0,(LPARAM)buffer2);
						SendMessage((HWND)lParam,  (UINT) EM_SETSEL, (WPARAM) (index), index);
						
						uint32 addy;
						uint8 val;
						bool8 sram;
						uint8 bytes[3];
						if(NULL==S9xGameGenieToRaw(buffer2, addy, val)||NULL==S9xProActionReplayToRaw(buffer2, addy, val)||NULL==S9xGoldFingerToRaw(buffer2, addy, sram, val, bytes))
						{
							if(has_sel)
								EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), true);
							else EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), false);
							EnableWindow(GetDlgItem(hDlg, IDC_ADD_CHEAT), true);
						}
						else
						{
							EnableWindow(GetDlgItem(hDlg, IDC_ADD_CHEAT), false);
							EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), false);
						}

					//	SetDlgItemText(hDlg, IDC_CHEAT_ADDRESS, "");
					//	SetDlgItemText(hDlg, IDC_CHEAT_BYTE, "");
						break;
					}
					break;
				}
			case IDC_CHEAT_ADDRESS:
				{
					uint32 j, k;
					long index;
					char buffer[7];
					char buffer2[7];
					POINT point;
					switch(HIWORD(wParam))
					{
					case EN_CHANGE:
						if(internal_change)
						{
							internal_change=false;
							return true;
						}
						SendMessage((HWND)lParam, WM_GETTEXT, 7,(LPARAM)buffer);
						GetCaretPos(&point);

						index = SendMessage((HWND)lParam,(UINT) EM_CHARFROMPOS, 0, (LPARAM) ((point.x&0x0000FFFF) | (((point.y&0x0000FFFF))<<16)));  

						k=0;
						for(j=0; j<strlen(buffer);j++)
						{
							if( (buffer[j]>='0' && buffer[j]<='9') || (buffer[j]>='A' && buffer[j]<='F'))
							{
								buffer2[k]=buffer[j];
								k++;
							}
							else index --;
						}
						buffer2[k]='\0';

					
						internal_change=true;
						SendMessage((HWND)lParam, WM_SETTEXT, 0,(LPARAM)buffer2);
						SendMessage((HWND)lParam,  (UINT) EM_SETSEL, (WPARAM) (index), index);

						SendMessage(GetDlgItem(hDlg, IDC_CHEAT_BYTE), WM_GETTEXT, 4,(LPARAM)buffer);
						
						if(has_sel&&!new_sel&&0!=strlen(buffer2))
							SetDlgItemText(hDlg, IDC_CHEAT_CODE, "");

						if(new_sel!=0)
							new_sel--;

						if(strlen(buffer2)!=0 && strlen(buffer) !=0)
						{
							if(has_sel)
								EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), true);
							else EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), false);
							EnableWindow(GetDlgItem(hDlg, IDC_ADD_CHEAT), true);
						}
						else
						{
							EnableWindow(GetDlgItem(hDlg, IDC_ADD_CHEAT), false);
							EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), false);
						}
						//SetDlgItemText(hDlg, IDC_CHEAT_CODE, "");
						break;
					}
					break;
				}
				case IDC_CHEAT_BYTE:
				{
					uint32 j, k;
					long index;
					char buffer[4];
					char buffer2[4];
					POINT point;
					switch(HIWORD(wParam))
					{
					case EN_CHANGE:
						if(internal_change)
						{
							internal_change=false;
							return true;
						}
						SendMessage((HWND)lParam, WM_GETTEXT, 4,(LPARAM)buffer);
						GetCaretPos(&point);

						index = SendMessage((HWND)lParam,(UINT) EM_CHARFROMPOS, 0, (LPARAM) ((point.x&0x0000FFFF) | (((point.y&0x0000FFFF))<<16)));  

						k=0;
						for(j=0; j<strlen(buffer);j++)
						{
							if( (buffer[j]>='0' && buffer[j]<='9') || (buffer[j]>='A' && buffer[j]<='F') || buffer[j]=='$')
							{
								buffer2[k]=buffer[j];
								k++;
							}
							else index --;
						}
						buffer2[k]='\0';
						
						if(has_sel&&!new_sel&&0!=strlen(buffer2))
							SetDlgItemText(hDlg, IDC_CHEAT_CODE, "");
						
						if(new_sel!=0)
							new_sel--;

						internal_change=true;
						SendMessage((HWND)lParam, WM_SETTEXT, 0,(LPARAM)buffer2);
						SendMessage((HWND)lParam,  (UINT) EM_SETSEL, (WPARAM) (index), index);
						
						SendMessage(GetDlgItem(hDlg, IDC_CHEAT_ADDRESS), WM_GETTEXT, 7,(LPARAM)buffer);
						if(strlen(buffer2)!=0 && strlen(buffer) !=0)
						{
							if(has_sel)
								EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), true);
							else EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), false);
							EnableWindow(GetDlgItem(hDlg, IDC_ADD_CHEAT), true);
						}
						else
						{
							EnableWindow(GetDlgItem(hDlg, IDC_ADD_CHEAT), false);
							EnableWindow(GetDlgItem(hDlg, IDC_UPDATE_CHEAT), false);
						}
						//SetDlgItemText(hDlg, IDC_CHEAT_CODE, "");
						break;
					}
					break;
				}
				case IDOK:
					{
						int k,l;
						bool hit;
						for(k=0;k<ListView_GetItemCount(GetDlgItem(hDlg, IDC_CHEAT_LIST)); k++)
						{
							hit=false;
							for(l=0;l<(int)Cheat.num_cheats;l++)
							{
								if(ct.index[l]==k)
								{
									hit=true;
									Cheat.c[l].enabled=ListView_GetCheckState(GetDlgItem(hDlg, IDC_CHEAT_LIST),l);
									if(ct.state[l]==Untouched)
										l=Cheat.num_cheats;
									else if(ct.state[l]==(unsigned long)Modified)
									{
										if(Cheat.c[l].enabled)
											S9xDisableCheat(l);
										//update me!
										//get these!
										
										char buf[25];
										LV_ITEM lvi;
										ZeroMemory(&lvi, sizeof(LV_ITEM));
										lvi.iItem= k;
										lvi.mask=LVIF_TEXT;
										lvi.pszText=buf;
										lvi.cchTextMax=7;
										
										ListView_GetItem(GetDlgItem(hDlg, IDC_CHEAT_LIST), &lvi);
										
										sscanf(lvi.pszText, "%06X", &Cheat.c[l].address);
										
										ZeroMemory(&lvi, sizeof(LV_ITEM));
										lvi.iItem= k;
										lvi.iSubItem=1;
										lvi.mask=LVIF_TEXT;
										lvi.pszText=buf;
										lvi.cchTextMax=3;
										
										ListView_GetItem(GetDlgItem(hDlg, IDC_CHEAT_LIST), &lvi);
										
										sscanf(lvi.pszText, "%02X", &Cheat.c[l].byte);
										
										ZeroMemory(&lvi, sizeof(LV_ITEM));
										lvi.iItem= k;
										lvi.iSubItem=2;
										lvi.mask=LVIF_TEXT;
										lvi.pszText=buf;
										lvi.cchTextMax=24;
										
										ListView_GetItem(GetDlgItem(hDlg, IDC_CHEAT_LIST), &lvi);
										
										strcpy(Cheat.c[l].name,lvi.pszText);
										
										Cheat.c[l].enabled=ListView_GetCheckState(GetDlgItem(hDlg, IDC_CHEAT_LIST),k);
										
										if(Cheat.c[l].enabled)
											S9xEnableCheat(l);
									}
									
								}
							}
							if(!hit)
							{
								uint32 address;
								uint8 byte;
								bool8 enabled;
								char buf[25];
								LV_ITEM lvi;
								ZeroMemory(&lvi, sizeof(LV_ITEM));
								lvi.iItem= k;
								lvi.mask=LVIF_TEXT;
								lvi.pszText=buf;
								lvi.cchTextMax=7;
								
								ListView_GetItem(GetDlgItem(hDlg, IDC_CHEAT_LIST), &lvi);
								
								sscanf(lvi.pszText, "%06X", &address);
								
								ZeroMemory(&lvi, sizeof(LV_ITEM));
								lvi.iItem= k;
								lvi.iSubItem=1;
								lvi.mask=LVIF_TEXT;
								lvi.pszText=buf;
								lvi.cchTextMax=3;
								
								ListView_GetItem(GetDlgItem(hDlg, IDC_CHEAT_LIST), &lvi);
								
								sscanf(lvi.pszText, "%02X", &byte);
								
								enabled=ListView_GetCheckState(GetDlgItem(hDlg, IDC_CHEAT_LIST),k);

								S9xAddCheat(enabled,true,address,byte);

								ZeroMemory(&lvi, sizeof(LV_ITEM));
								lvi.iItem= k;
								lvi.iSubItem=2;
								lvi.mask=LVIF_TEXT;
								lvi.pszText=buf;
								lvi.cchTextMax=24;
								
								ListView_GetItem(GetDlgItem(hDlg, IDC_CHEAT_LIST), &lvi);
								
								strcpy(Cheat.c[Cheat.num_cheats-1].name, lvi.pszText);
								
								
							}
						}

						for(l=(int)Cheat.num_cheats;l>=0;l--)
						{
							if(ct.state[l]==Deleted)
							{
								S9xDeleteCheat(l);
							}
						}
					}
				case IDCANCEL:
					delete [] ct.index;
					delete [] ct.state;
					EndDialog(hDlg, 0);
					if(hBmp)
					{
						DeleteObject(hBmp);
						hBmp=NULL;
					}
					return true;
				default:return false;
					}
		}
	default: return false;
	}
}


#define TEST_BIT(a,v) \
((a)[(v) >> 5] & (1 << ((v) & 31)))

#define CHEAT_COUNT(a, b) for(a=0;a<0x20000-bytes;a++) \
					{ \
						if(TEST_BIT(Cheat.WRAM_BITS, a)) \
							b++; \
					}

struct ICheat
{
    uint32  address;
    uint32  new_val;
    uint32  saved_val;
	int		size;
    bool8   enabled;
    bool8   saved;
    char    name [22];
	int format;
};

bool TestRange(int val_type, S9xCheatDataSize bytes,  uint32 value)
{
	if(val_type!=2)
	{
		if(bytes==S9X_8_BITS)
		{
			if(value<256)
				return true;
			else return false;
		}
		if(bytes==S9X_16_BITS)
		{
			if(value<65536)
				return true;
			else return false;
		}
		if(bytes==S9X_24_BITS)
		{
			if(value<16777216)
				return true;
			else return false;
		}
		//if it reads in, it's a valid 32-bit unsigned!
		return true;
	}
	else
	{
		if(bytes==S9X_8_BITS)
		{
			if(value<128 && value >= -128)
				return true;
			else return false;
		}
		if(bytes==S9X_16_BITS)
		{
			if(value<32768 && value >= -32768)
				return true;
			else return false;
		}
		if(bytes==S9X_24_BITS)
		{
			if(value<8388608&& value >= -8388608)
				return true;
			else return false;
		}
		//should be handled by sscanf
		return true;
	}
}


int CALLBACK DlgCheatSearch(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBmp;
	static S9xCheatDataSize bytes;
	static int val_type;
	static bool use_entered;
	static S9xCheatComparisonType comp_type;
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			if(val_type==0)
				val_type=1;
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("Raptor.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			ListView_SetExtendedListViewStyle(GetDlgItem(hDlg, IDC_ADDYS), LVS_EX_FULLROWSELECT);
			
			//defaults
			SendDlgItemMessage(hDlg, IDC_LESS_THAN, BM_SETCHECK, BST_CHECKED, 0);
			if(!use_entered)
				SendDlgItemMessage(hDlg, IDC_PREV, BM_SETCHECK, BST_CHECKED, 0);
			else
			{
				SendDlgItemMessage(hDlg, IDC_ENTERED, BM_SETCHECK, BST_CHECKED, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_VALUE_ENTER), true);
				EnableWindow(GetDlgItem(hDlg, IDC_ENTER_LABEL), true);
			}
			SendDlgItemMessage(hDlg, IDC_UNSIGNED, BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage(hDlg, IDC_1_BYTE, BM_SETCHECK, BST_CHECKED, 0);

			if(comp_type==S9X_GREATER_THAN)
			{
				SendDlgItemMessage(hDlg, IDC_LESS_THAN, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_GREATER_THAN, BM_SETCHECK, BST_CHECKED, 0);
			}
			else if(comp_type==S9X_LESS_THAN_OR_EQUAL)
			{
				SendDlgItemMessage(hDlg, IDC_LESS_THAN, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_LESS_THAN_EQUAL, BM_SETCHECK, BST_CHECKED, 0);
				
			}
			else if(comp_type==S9X_GREATER_THAN_OR_EQUAL)
			{
				SendDlgItemMessage(hDlg, IDC_LESS_THAN, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_GREATER_THAN_EQUAL, BM_SETCHECK, BST_CHECKED, 0);
				
			}
			else if(comp_type==S9X_EQUAL)
			{
				SendDlgItemMessage(hDlg, IDC_LESS_THAN, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_EQUAL, BM_SETCHECK, BST_CHECKED, 0);
				
			}
			else if(comp_type==S9X_NOT_EQUAL)
			{
				SendDlgItemMessage(hDlg, IDC_LESS_THAN, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_NOT_EQUAL, BM_SETCHECK, BST_CHECKED, 0);
				
			}
			
			if(val_type==2)
			{
				SendDlgItemMessage(hDlg, IDC_UNSIGNED, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_SIGNED, BM_SETCHECK, BST_CHECKED, 0);
				
			}
			else if(val_type==3)
			{
				SendDlgItemMessage(hDlg, IDC_UNSIGNED, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_HEX, BM_SETCHECK, BST_CHECKED, 0);
			}

			if(bytes==S9X_16_BITS)
			{
				SendDlgItemMessage(hDlg, IDC_1_BYTE, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_2_BYTE, BM_SETCHECK, BST_CHECKED, 0);
			}
			else if(bytes==S9X_24_BITS)
			{
				SendDlgItemMessage(hDlg, IDC_1_BYTE, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_3_BYTE, BM_SETCHECK, BST_CHECKED, 0);
			}
			else if(bytes==S9X_32_BITS)
			{
				SendDlgItemMessage(hDlg, IDC_1_BYTE, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_4_BYTE, BM_SETCHECK, BST_CHECKED, 0);
			}

			LVCOLUMN col;
			char temp[32];
			strcpy(temp,"Address");
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=0;
			col.cx=70;
			col.cchTextMax=7;
			col.pszText=temp;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_ADDYS),   0,   &col);
			
			strcpy(temp,"Curr. Value");
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=1;
			col.cx=104;
			col.cchTextMax=3;
			col.pszText=temp;
			col.iSubItem=1;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_ADDYS),    1,   &col);

			strcpy(temp,"Prev. Value");
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=2;
			col.cx=104;
			col.cchTextMax=32;
			col.pszText=temp;
			col.iSubItem=2;

			ListView_InsertColumn(GetDlgItem(hDlg,IDC_ADDYS),    2,   &col);
			
			{
					int k=0, l=0;
					CHEAT_COUNT(k,l);
					ListView_SetItemCount (GetDlgItem(hDlg, IDC_ADDYS), l);
			}

		}
			return true;
		case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;
	case WM_NOTIFY:
		{
			if(wParam == IDC_ADDYS)
			{
				NMHDR * nmh=(NMHDR*)lParam;
				if(nmh->hwndFrom == GetDlgItem(hDlg, IDC_ADDYS) && nmh->code == LVN_GETDISPINFO)
				{
					TCHAR buf[8];
					int i, j;
					NMLVDISPINFO * nmlvdi=(NMLVDISPINFO*)lParam;
					j=nmlvdi->item.iItem;
					j++;
					for(i=0;i<(0x20000-bytes)&& j>0;i++)
					{
						if(TEST_BIT(Cheat.WRAM_BITS, i))
							j--;
					}
					if (i>=0x20000 && j!=0)
					{
						return false;
					}
					i--;
					if(j=nmlvdi->item.iSubItem==0)
					{
						sprintf(buf, "%06X", i+0x7E0000);
						nmlvdi->item.pszText=buf;
						nmlvdi->item.cchTextMax=8;
					}
					if(j=nmlvdi->item.iSubItem==1)
					{
						int q=0, r=0;
						for(r=0;r<=bytes;r++)
							q+=(Cheat.RAM[i+r])<<(8*r);
						//needs to account for size
						switch(val_type)
						{
						case 1:
							sprintf(buf, "%u", q);
							break;
						case 3:
							{
								switch(bytes)
								{
									default:
									case S9X_8_BITS:sprintf(buf, "%02X", q&0xFF);break;
									case S9X_16_BITS: sprintf(buf, "%04X", q&0xFFFF); break;
									case S9X_24_BITS: sprintf(buf, "%06X", q&0xFFFFFF);break;
									case S9X_32_BITS: sprintf(buf, "%08X", q);break;
								}
							}
							break;
						case 2:
							default:
								switch(bytes)
								{
									default:
									case S9X_8_BITS:  
										if((q-128)<0)
											sprintf(buf, "%d", q&0xFF);
										else sprintf(buf, "%d", q-256);
										break;
									case S9X_16_BITS:
										if((q-32768)<0)
											sprintf(buf, "%d", q&0xFFFF);
										else sprintf(buf, "%d", q-65536);
										break;
									case S9X_24_BITS:
										if((q-0x800000)<0)
											sprintf(buf, "%d", q&0xFFFFFF);
										else sprintf(buf, "%d", q-0x1000000);
										break;

									case S9X_32_BITS: sprintf(buf, "%d", q);break;
								}
								break;
						}
						nmlvdi->item.pszText=buf;
						nmlvdi->item.cchTextMax=4;
					}
					if(j=nmlvdi->item.iSubItem==2)
					{
						int q=0, r=0;
						for(r=0;r<=bytes;r++)
							q+=(Cheat.CWRAM[i+r])<<(8*r);
						//needs to account for size
						switch(val_type)
						{
						case 1:
							sprintf(buf, "%u", q);
							break;
						case 3:
							{
								switch(bytes)
								{
									default:
									case S9X_8_BITS:sprintf(buf, "%02X", q&0xFF);break;
									case S9X_16_BITS: sprintf(buf, "%04X", q&0xFFFF); break;
									case S9X_24_BITS: sprintf(buf, "%06X", q&0xFFFFFF);break;
									case S9X_32_BITS: sprintf(buf, "%08X", q);break;
								}
								break;
							}
						case 2:
							default:
								switch(bytes)
								{
									default:
									case S9X_8_BITS:  
										if((q-128)<0)
											sprintf(buf, "%d", q&0xFF);
										else sprintf(buf, "%d", q-256);
										break;
									case S9X_16_BITS:
										if((q-32768)<0)
											sprintf(buf, "%d", q&0xFFFF);
										else sprintf(buf, "%d", q-65536);
										break;
									case S9X_24_BITS:
										if((q-0x800000)<0)
											sprintf(buf, "%d", q&0xFFFFFF);
										else sprintf(buf, "%d", q-0x1000000);
										break;

									case S9X_32_BITS: sprintf(buf, "%d", q);break;
								}
								break;
						}
						nmlvdi->item.pszText=buf;
						nmlvdi->item.cchTextMax=4;
					}
					nmlvdi->item.mask=LVIF_TEXT;
					
				}
				else if(nmh->hwndFrom == GetDlgItem(hDlg, IDC_ADDYS) && (nmh->code == LVN_ITEMACTIVATE||nmh->code == NM_CLICK))
				{
					bool enable=true;
					if(-1==ListView_GetSelectionMark(nmh->hwndFrom))
					{
						enable=false;
					}
					EnableWindow(GetDlgItem(hDlg, IDC_C_ADD), enable);
				}
			}
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_LESS_THAN:
			case IDC_GREATER_THAN:
			case IDC_LESS_THAN_EQUAL:
			case IDC_GREATER_THAN_EQUAL:
			case IDC_EQUAL:
			case IDC_NOT_EQUAL:
				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_LESS_THAN))
					comp_type=S9X_LESS_THAN;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_GREATER_THAN))
					comp_type=S9X_GREATER_THAN;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_LESS_THAN_EQUAL))
					comp_type=S9X_LESS_THAN_OR_EQUAL;	
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_GREATER_THAN_EQUAL))
					comp_type=S9X_GREATER_THAN_OR_EQUAL;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_EQUAL))
					comp_type=S9X_EQUAL;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_NOT_EQUAL))
					comp_type=S9X_NOT_EQUAL;

				break;
			case IDC_1_BYTE:
			case IDC_2_BYTE:
			case IDC_3_BYTE:
			case IDC_4_BYTE:
				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_1_BYTE))
					bytes=S9X_8_BITS;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_2_BYTE))
					bytes=S9X_16_BITS;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_3_BYTE))
					bytes=S9X_24_BITS;
				else bytes=S9X_32_BITS;
				{
					int k=0, l=0;
					CHEAT_COUNT(k,l);
					ListView_SetItemCount (GetDlgItem(hDlg, IDC_ADDYS), l);
				}

				break;

			case IDC_SIGNED:
			case IDC_UNSIGNED:
			case IDC_HEX:
				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_UNSIGNED))
					val_type=1;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_SIGNED))
					val_type=2;
				else val_type=3;
				ListView_RedrawItems(GetDlgItem(hDlg, IDC_ADDYS),0, 0x20000);
				break;
				case IDC_C_ADD:
					{
						// account for size
						struct ICheat cht;
						int idx=-1;
						LVITEM lvi;
						TCHAR buf[12];
						ZeroMemory(&cht, sizeof(struct SCheat));

						//retrieve and convert to SCheat

						if(bytes==S9X_8_BITS)
							cht.size=1;
						else if (bytes==S9X_16_BITS)
							cht.size=2;
						else if (bytes==S9X_24_BITS)
							cht.size=3;
						else if (bytes==S9X_32_BITS)
							cht.size=4;


						ITEM_QUERY(lvi, IDC_ADDYS, 0, buf, 7);
						sscanf(buf, "%6X", &cht.address);
						memset(buf, 0, 7);
						if(val_type==1)
						{
							ITEM_QUERY(lvi, IDC_ADDYS, 1, buf, 12);
							sscanf(buf, "%u", &cht.new_val);
							memset(buf, 0, 7);
							ITEM_QUERY(lvi, IDC_ADDYS, 2, buf, 12);
							sscanf(buf, "%u", &cht.saved_val);
						}
						else if(val_type==3)
						{
							ITEM_QUERY(lvi, IDC_ADDYS, 1, buf, 12);
							sscanf(buf, "%x", &cht.new_val);
							memset(buf, 0, 7);
							ITEM_QUERY(lvi, IDC_ADDYS, 2, buf, 12);
							sscanf(buf, "%x", &cht.saved_val);
						}
						else
						{
							ITEM_QUERY(lvi, IDC_ADDYS, 1, buf, 12);
							sscanf(buf, "%d", &cht.new_val);
							memset(buf, 0, 7);
							ITEM_QUERY(lvi, IDC_ADDYS, 2, buf, 12);
							sscanf(buf, "%d", &cht.saved_val);
						}
						cht.format=val_type;
						//invoke dialog
						if(!DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_CHEAT_FROM_SEARCH), hDlg, DlgCheatSearchAdd, (LPARAM)&cht))
						{
							int p;
							for(p=0; p<cht.size; p++)
							{
								S9xAddCheat(TRUE, cht.saved, cht.address +p, ((cht.new_val>>(8*p))&0xFF));
								//add cheat
								strcpy(Cheat.c[Cheat.num_cheats-1].name, cht.name);
							}
						}
					}
					break;
			case IDC_C_RESET:
				S9xStartCheatSearch(&Cheat);
				{
					int k=0, l=0;
					CHEAT_COUNT(k,l);
					ListView_SetItemCount (GetDlgItem(hDlg, IDC_ADDYS), l);
				}
				ListView_RedrawItems(GetDlgItem(hDlg, IDC_ADDYS),0, 0x20000);
				val_type=1;
				SendDlgItemMessage(hDlg, IDC_UNSIGNED, BM_SETCHECK, BST_CHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_SIGNED, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_HEX, BM_SETCHECK, BST_UNCHECKED, 0);

				bytes=S9X_8_BITS;
				SendDlgItemMessage(hDlg, IDC_1_BYTE, BM_SETCHECK, BST_CHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_2_BYTE, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_3_BYTE, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_4_BYTE, BM_SETCHECK, BST_UNCHECKED, 0);


				use_entered=false;
				SendDlgItemMessage(hDlg, IDC_PREV, BM_SETCHECK, BST_CHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_ENTERED, BM_SETCHECK, BST_UNCHECKED, 0);
				EnableWindow(GetDlgItem(hDlg, IDC_VALUE_ENTER), false);
				EnableWindow(GetDlgItem(hDlg, IDC_ENTER_LABEL), false);

				comp_type=S9X_LESS_THAN;
				SendDlgItemMessage(hDlg, IDC_LESS_THAN, BM_SETCHECK, BST_CHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_GREATER_THAN, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_LESS_THAN_EQUAL, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_GREATER_THAN_EQUAL, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_EQUAL, BM_SETCHECK, BST_UNCHECKED, 0);
				SendDlgItemMessage(hDlg, IDC_NOT_EQUAL, BM_SETCHECK, BST_UNCHECKED, 0);
				return true;

			case IDC_ENTERED:
			case IDC_PREV:
				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_ENTERED))
				{
					use_entered=true;
					EnableWindow(GetDlgItem(hDlg, IDC_VALUE_ENTER), true);
					EnableWindow(GetDlgItem(hDlg, IDC_ENTER_LABEL), true);
				}
				else
				{
					use_entered=false;
					EnableWindow(GetDlgItem(hDlg, IDC_VALUE_ENTER), false);
					EnableWindow(GetDlgItem(hDlg, IDC_ENTER_LABEL), false);
				}
				return true;
				break;
			case IDC_C_SEARCH:
				{
				val_type=0;

				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_LESS_THAN))
					comp_type=S9X_LESS_THAN;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_GREATER_THAN))
					comp_type=S9X_GREATER_THAN;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_LESS_THAN_EQUAL))
					comp_type=S9X_LESS_THAN_OR_EQUAL;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_GREATER_THAN_EQUAL))
					comp_type=S9X_GREATER_THAN_OR_EQUAL;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_EQUAL))
					comp_type=S9X_EQUAL;
				else comp_type=S9X_NOT_EQUAL;

				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_UNSIGNED))
					val_type=1;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_SIGNED))
					val_type=2;
				else val_type=3;



				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_1_BYTE))
					bytes=S9X_8_BITS;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_2_BYTE))
					bytes=S9X_16_BITS;
				else if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_3_BYTE))
					bytes=S9X_24_BITS;
				else bytes=S9X_32_BITS;


				if(BST_CHECKED==IsDlgButtonChecked(hDlg, IDC_ENTERED))
				{
					TCHAR buf[20];
					GetDlgItemText(hDlg, IDC_VALUE_ENTER, buf, 20);
					uint32 value;
					int ret;
					if(val_type==1)
						ret=_stscanf(buf, "%ul", &value);
					else if (val_type==2)
						ret=_stscanf(buf, "%d", &value);
					else ret=_stscanf(buf, "%x", &value);

					

					if(ret!=1||!TestRange(val_type, bytes, value))
					{
						MessageBox(hDlg, TEXT("Please enter a valid value for a search!"), TEXT("Snes9x Cheat Error"), MB_OK);
						return true;
					}

					S9xSearchForValue (&Cheat, comp_type,
                        bytes, value,
                        (val_type==2), FALSE);
					
				}
				else
				{
					S9xSearchForChange (&Cheat, comp_type,
                         bytes, (val_type==2), FALSE);
				}
				int k=0, l=0;
				CHEAT_COUNT(k,l);
				ListView_SetItemCount (GetDlgItem(hDlg, IDC_ADDYS), l);
				}


				ListView_RedrawItems(GetDlgItem(hDlg, IDC_ADDYS),0, 0x20000);
				return true;
				break;
			case IDOK:
				CopyMemory(Cheat.CWRAM, Cheat.RAM, 0x20000);
				CopyMemory(Cheat.CSRAM, Cheat.SRAM, 0x10000);
				CopyMemory(Cheat.CIRAM, Cheat.FillRAM, 0x2000);
			case IDCANCEL:
				EndDialog(hDlg, 0);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				};
				return true;
			default: break;
			}
		}
	default: return false;
	}
}

int CALLBACK DlgCheatSearchAdd(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static struct ICheat* new_cheat;
	int ret=-1;
	static HBITMAP hBmp;
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			TCHAR buf [12];
			new_cheat=(struct ICheat*)lParam;
			hBmp=(HBITMAP)LoadImage(NULL, TEXT("Gary.bmp"), IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			sprintf(buf, "%06X", new_cheat->address);
			SetDlgItemText(hDlg, IDC_NC_ADDRESS, buf);
			switch(new_cheat->format)
			{
			default:
			case 1://UNSIGNED
				memset(buf,0,12);
				sprintf(buf, "%u", new_cheat->new_val);
				SetDlgItemText(hDlg, IDC_NC_CURRVAL, buf);
				memset(buf,0,12);
				sprintf(buf, "%u", new_cheat->saved_val);
				SetDlgItemText(hDlg, IDC_NC_PREVVAL, buf);
				SetWindowLong(GetDlgItem(hDlg, IDC_NC_NEWVAL), GWL_STYLE, ES_NUMBER |GetWindowLong(GetDlgItem(hDlg, IDC_NC_NEWVAL),GWL_STYLE));
				SetWindowLong(GetDlgItem(hDlg, IDC_NC_CURRVAL), GWL_STYLE, ES_NUMBER |GetWindowLong(GetDlgItem(hDlg, IDC_NC_CURRVAL),GWL_STYLE));
				SetWindowLong(GetDlgItem(hDlg, IDC_NC_PREVVAL), GWL_STYLE, ES_NUMBER |GetWindowLong(GetDlgItem(hDlg, IDC_NC_PREVVAL),GWL_STYLE));
				if(new_cheat->size==1)
				{
					SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, 3, 0);
					SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, 3, 0);
					SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, 3, 0);

				}
				if(new_cheat->size==2)
				{
					SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, 5, 0);
					SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, 5, 0);
					SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, 5, 0);

				}
				if(new_cheat->size==3)
				{
					SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, 8, 0);
					SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, 8, 0);
					SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, 8, 0);

				}
				if(new_cheat->size==4)
				{
					SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, 10, 0);
					SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, 10, 0);
					SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, 10, 0);

				}
				break;
			case 3:
				{
					char formatstring[10];
					sprintf(formatstring, "%%%02dX",new_cheat->size*2);
					memset(buf,0,12);
					sprintf(buf, formatstring, new_cheat->new_val);
					SetDlgItemText(hDlg, IDC_NC_CURRVAL, buf);
					memset(buf,0,12);
					sprintf(buf, formatstring, new_cheat->saved_val);
					SetDlgItemText(hDlg, IDC_NC_PREVVAL, buf);
					SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, new_cheat->size*2, 0);
					SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, new_cheat->size*2, 0);
					SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, new_cheat->size*2, 0);

				}
				break; //HEX
			case 2:
			memset(buf,0,12);
			sprintf(buf, "%d", new_cheat->new_val);
			SetDlgItemText(hDlg, IDC_NC_CURRVAL, buf);
			memset(buf,0,12);
			sprintf(buf, "%d", new_cheat->saved_val);
			SetDlgItemText(hDlg, IDC_NC_PREVVAL, buf);
			if(new_cheat->size==1)
			{
				//-128
				SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, 4, 0);
				SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, 4, 0);
				SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, 4, 0);
			}
			if(new_cheat->size==2)
			{
				//-32768
				SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, 6, 0);
				SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, 6, 0);
				SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, 6, 0);
			}
			if(new_cheat->size==3)
			{
				//-8388608
				SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, 8, 0);
				SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, 8, 0);
				SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, 8, 0);
			}
			if(new_cheat->size==4)
			{
				//-2147483648
				SendDlgItemMessage(hDlg, IDC_NC_CURRVAL,EM_SETLIMITTEXT, 11, 0);
				SendDlgItemMessage(hDlg, IDC_NC_PREVVAL,EM_SETLIMITTEXT, 11, 0);
				SendDlgItemMessage(hDlg, IDC_NC_NEWVAL,EM_SETLIMITTEXT, 11, 0);
			}
			break;
			}
		}
			return true;
		case WM_PAINT:
		{
		PAINTSTRUCT ps;
		BeginPaint (hDlg, &ps);
		if(hBmp)
		{
			BITMAP bmp;
			ZeroMemory(&bmp, sizeof(BITMAP));
			RECT r;
			GetClientRect(hDlg, &r);
			HDC hdc=GetDC(hDlg);
			HDC hDCbmp=CreateCompatibleDC(hdc);
			GetObject(hBmp, sizeof(BITMAP), &bmp);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCbmp, hBmp);
			StretchBlt(hdc, 0,0,r.right,r.bottom,hDCbmp,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCbmp, hOldBmp);
			DeleteDC(hDCbmp);
			ReleaseDC(hDlg, hdc);
		}

		EndPaint (hDlg, &ps);
		}
		return true;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					int ret;
					TCHAR buf[23];
					int temp=new_cheat->size;
					S9xCheatDataSize tmp;
					ZeroMemory(new_cheat, sizeof(struct SCheat));
					new_cheat->size=temp;
					GetDlgItemText(hDlg, IDC_NC_ADDRESS, buf, 7);
					sscanf(buf, "%6X", &new_cheat->address);
					
					if(temp==1)
						tmp=S9X_8_BITS;
					if(temp==2)
						tmp=S9X_16_BITS;
					if(temp==3)
						tmp=S9X_24_BITS;
					if(temp==4)
						tmp=S9X_32_BITS;
					
					
					if(0!=GetDlgItemText(hDlg, IDC_NC_NEWVAL, buf, 12))
					{
						if(new_cheat->format==2)
							ret=sscanf(buf, "%d", &new_cheat->new_val);
						else if(new_cheat->format==1)
							ret=sscanf(buf, "%u", &new_cheat->new_val);
						else if(new_cheat->format==3)
							ret=sscanf(buf, "%x", &new_cheat->new_val);

						if(ret!=1 || !TestRange(new_cheat->format, tmp, new_cheat->new_val))
						{
							MessageBox(hDlg, "You have entered an out of range or invalid value for the new value", "Range Error", MB_OK);
							return true;
						}


						if(0==GetDlgItemText(hDlg, IDC_NC_CURRVAL, buf, 12))
							new_cheat->saved=FALSE;
						else
						{
							int i;
							if(new_cheat->format==2)
								ret=sscanf(buf, "%d", &i);
							else if(new_cheat->format==1)
								ret=sscanf(buf, "%u", &i);
							else if(new_cheat->format==3)
								ret=sscanf(buf, "%x", &i);

							if(ret!=1 || !TestRange(new_cheat->format, tmp, i))
							{
								MessageBox(hDlg, "You have entered an out of range or invalid value for\n"\
									"the current value. This value is used when a cheat is unapplied.\n"\
									"(If left blank, no value is restored when the cheat is unapplied)", "Range Error", MB_OK);
								return true;
							}


							new_cheat->saved_val=i;
							new_cheat->saved=TRUE;
						}
						GetDlgItemText(hDlg, IDC_NC_DESC, new_cheat->name, 23);

						new_cheat->enabled=TRUE;
						S9xAddCheat(new_cheat->enabled,new_cheat->saved_val,new_cheat->address,new_cheat->new_val);
						strcpy(Cheat.c[Cheat.num_cheats-1].name,new_cheat->name);
						ret=0;
					}
				}
				
			case IDCANCEL:
				EndDialog(hDlg, ret);
				if(hBmp)
				{
					DeleteObject(hBmp);
					hBmp=NULL;
				}
				return true;
			default: break;
			}
		}
	default: return false;
	}
}

static void set_movieinfo(const char* path, HWND hDlg)
{
	MovieInfo m;
	int i;

	if(strlen(path) && S9xMovieGetInfo(path, &m)==SUCCESS)
	{
		char* p;
		char tmpstr[128];
		strncpy(tmpstr, ctime(&m.TimeCreated), 127);
		tmpstr[127]='\0';
		if((p=strrchr(tmpstr, '\n')))
			*p='\0';
		SetWindowTextA(GetDlgItem(hDlg, IDC_MOVIE_DATE), tmpstr);

		uint32 div = Settings.PAL ? 50 : 60;
		uint32 l=(m.LengthFrames+(div>>1))/div;
		uint32 seconds=l%60;
		l/=60;
		uint32 minutes=l%60;
		l/=60;
		uint32 hours=l%60;
		sprintf(tmpstr, "%02d:%02d:%02d", hours, minutes, seconds);
		SetWindowTextA(GetDlgItem(hDlg, IDC_MOVIE_LENGTH), tmpstr);
		sprintf(tmpstr, "%ld", m.LengthFrames);
		SetWindowTextA(GetDlgItem(hDlg, IDC_MOVIE_FRAMES), tmpstr);
		sprintf(tmpstr, "%ld", m.RerecordCount);
		SetWindowTextA(GetDlgItem(hDlg, IDC_MOVIE_RERECORD), tmpstr);
		SetWindowTextW(GetDlgItem(hDlg, IDC_MOVIE_METADATA), m.Metadata);

		if(m.ReadOnly)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_READONLY), FALSE);
			SendDlgItemMessage(hDlg,IDC_READONLY,BM_SETCHECK,BST_CHECKED,0);
		}
		else
		{
			EnableWindow(GetDlgItem(hDlg, IDC_READONLY), TRUE);
			SendDlgItemMessage(hDlg,IDC_READONLY,BM_SETCHECK,BST_UNCHECKED,0);
		}

		for(i=0; i<5; ++i)
		{
			SendDlgItemMessage(hDlg,IDC_JOY1+i,BM_SETCHECK,(m.ControllersMask & (1<<i)) ? BST_CHECKED : BST_UNCHECKED,0);
		}

		if(m.Opts & MOVIE_OPT_FROM_RESET)
		{
			SendDlgItemMessage(hDlg,IDC_RECORD_NOW,BM_SETCHECK,BST_UNCHECKED,0);
			SendDlgItemMessage(hDlg,IDC_RECORD_RESET,BM_SETCHECK,BST_CHECKED,0);
		}
		else
		{
			SendDlgItemMessage(hDlg,IDC_RECORD_RESET,BM_SETCHECK,BST_UNCHECKED,0);
			SendDlgItemMessage(hDlg,IDC_RECORD_NOW,BM_SETCHECK,BST_CHECKED,0);
		}
	}
	else
	{
		SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_DATE), _T(""));
		SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_LENGTH), _T(""));
		SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_FRAMES), _T(""));
		SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_RERECORD), _T(""));
		SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_METADATA), _T(""));
		EnableWindow(GetDlgItem(hDlg, IDC_READONLY), FALSE);
		SendDlgItemMessage(hDlg,IDC_READONLY,BM_SETCHECK,BST_UNCHECKED,0);
	}
}

int CALLBACK DlgOpenMovie(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static OpenMovieParams* op=NULL;

	switch(msg)
	{
	case WM_INITDIALOG:
		{
			op=(OpenMovieParams*)lParam;

			// get default filename
			if(Memory.ROMFilename[0]!='\0')
			{
				static TCHAR filename [_MAX_PATH + 1];
				TCHAR drive [_MAX_DRIVE + 1];
				TCHAR dir [_MAX_DIR + 1];
				TCHAR fname [_MAX_FNAME + 1];
				TCHAR ext [_MAX_EXT + 1];
				_splitpath (Memory.ROMFilename, drive, dir, fname, ext);
				_makepath (filename, "", "", fname, "smv");
				SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_PATH), filename);
				set_movieinfo(filename, hDlg);
			}
			else
			{
				set_movieinfo("", hDlg);
			}
		}
		return true;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BROWSE_MOVIE:
				{
					OPENFILENAME  ofn;
					char  szFileName[MAX_PATH];

					szFileName[0] = '\0';

					ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME) );
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = hDlg;
					ofn.lpstrFilter = "Snes9x Movie File\0*.smv\0All files\0*.*\0\0";
					ofn.lpstrFile = szFileName;
					ofn.lpstrDefExt = "smv";
					ofn.nMaxFile = MAX_PATH;
					if(GetOpenFileName( &ofn ))
					{
						SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_PATH), szFileName);
						set_movieinfo(szFileName, hDlg);
					}
				}
				return true;

			case IDOK:
				{
					if(BST_CHECKED==SendDlgItemMessage(hDlg, IDC_READONLY, BM_GETCHECK,0,0))
						op->ReadOnly=TRUE;
					GetDlgItemText(hDlg, IDC_MOVIE_PATH, op->Path, MAX_PATH);
				}
				EndDialog(hDlg, 1);
				return true;
				
			case IDCANCEL:
				EndDialog(hDlg, 0);
				return true;

			default:
				break;
			}
		}

	default:
		return false;
	}
}

int CALLBACK DlgCreateMovie(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static OpenMovieParams* op=NULL;

	switch(msg)
	{
	case WM_INITDIALOG:
		{
			op=(OpenMovieParams*)lParam;

			SendDlgItemMessage(hDlg,IDC_RECORD_RESET,BM_SETCHECK,BST_CHECKED,0);

			int i;
			for(i=1; i<5; ++i)
			{
				SendDlgItemMessage(hDlg,IDC_JOY1+i,BM_SETCHECK,BST_UNCHECKED,0);
			}
			SendDlgItemMessage(hDlg,IDC_JOY1,BM_SETCHECK,BST_CHECKED,0);

			// get default filename
			if(Memory.ROMFilename[0]!='\0')
			{
				static TCHAR filename [_MAX_PATH + 1];
				TCHAR drive [_MAX_DRIVE + 1];
				TCHAR dir [_MAX_DIR + 1];
				TCHAR fname [_MAX_FNAME + 1];
				TCHAR ext [_MAX_EXT + 1];
				_splitpath (Memory.ROMFilename, drive, dir, fname, ext);
				_makepath (filename, "", "", fname, "smv");
				SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_PATH), filename);
			}
		}
		return true;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BROWSE_MOVIE:
				{
					OPENFILENAME  ofn;
					char  szFileName[MAX_PATH];

					szFileName[0] = '\0';

					ZeroMemory( (LPVOID)&ofn, sizeof(OPENFILENAME) );
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.hwndOwner = hDlg;
					ofn.lpstrFilter = "Snes9x Movie File\0*.smv\0All files\0*.*\0\0";
					ofn.lpstrFile = szFileName;
					ofn.lpstrDefExt = "smv";
					ofn.nMaxFile = MAX_PATH;
					ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
					if(GetSaveFileName( &ofn ))
					{
						SetWindowText(GetDlgItem(hDlg, IDC_MOVIE_PATH), szFileName);
					}
				}
				return true;

			case IDOK:
				{
					GetDlgItemText(hDlg, IDC_MOVIE_PATH, op->Path, MAX_PATH);
					GetDlgItemTextW(hDlg, IDC_MOVIE_METADATA, op->Metadata, MOVIE_MAX_METADATA);
					op->ControllersMask=0;
					op->Opts=0;
					int i;
					for(i=0; i<5; ++i)
					{
						if(BST_CHECKED==SendDlgItemMessage(hDlg, IDC_JOY1+i, BM_GETCHECK,0,0))
							op->ControllersMask |= (1<<i);
					}
					if(BST_CHECKED==SendDlgItemMessage(hDlg, IDC_RECORD_RESET, BM_GETCHECK,0,0))
						op->Opts |= MOVIE_OPT_FROM_RESET;
				}
				EndDialog(hDlg, 1);
				return true;
				
			case IDCANCEL:
				EndDialog(hDlg, 0);
				return true;

			default:
				break;
			}
		}

	default:
		return false;
	}
}
