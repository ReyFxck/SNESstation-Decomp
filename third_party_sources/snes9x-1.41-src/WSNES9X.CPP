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

  Win32 GUI code
  (c) Copyright 2003 Matthew Kendora and Patrick Chee(funkyass@snes9x.com)



 
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
#include "Snes9X/Snes9X.H"
#include "Snes9X/MemMap.H"
#include "Snes9X/cpuexec.h"
#include "Snes9X/display.h"
#include "snes9x/3d.h"
#include "snes9x/cheats.h"
#include "snes9x/netplay.h"
#include "snes9x/apu.h"

#include <fstream.h>
#include <sys/stat.h>
#include "string_cache.h"

//uncomment to find memory leaks, with a line in WinMain
//#include <crtdbg.h>

#include <commctrl.h>
#include <io.h>
#ifdef RTC_DEBUGGER
#include <time.h>
#endif

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
	char* extension;
	bool compressed;
	struct sExtList* next;
} ExtList;

ExtList* valid_ext=NULL;
void MakeExtFile(void);
void LoadExts(void);
HWND meow;
extern FILE *trace_fs;
extern int superscope_turbo;
extern int superscope_pause;
extern SCheatData Cheat;

HINSTANCE g_hInst;

#ifdef DEBUGGER
extern "C" void Trace ();
#endif

typedef struct 
{
	long height;
	long width;
	long depth;
	int status;
}dMode;
dMode dm[64];
int dmindex =0;

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

// Languages supported by Snes9X: Windows
// 0 - English [Default]
// 1 - Dutch/Nederlands
struct sLanguages Languages[] = {
	{ IDR_MENU_US, 
		"DirectX failed to initialize!", 
		"DirectDraw failed to set the selected display mode!", 
		"DirectSound failed to initialize, no sound will be played.",
		"These settings won't take effect until you restart the emulator.",
		"The frame timer failed to initialize, please do NOT select the automatic framerate option or Snes9X will crash!"},
	{ IDR_MENU_NL, 
	"Er is een fout opgetreden tijdens het initalizeren van DirectX!", 
	"Er is een fout opgetreden tijdens het verander van scherm modus!", 
	"Er is een fout opgetreden tijdens het initializeren van DirectSound, er zal geen geluid worden afgespeeld.",
	"Deze opties worden pas toegepast als de emulator opnieuw is opgestart.",
	"Er is een fout opgetreden tijdens het initializeren van de frame timer, kies NIET de automatische framerate optie want dan zal Snes9X crashen!"}
};

extern "C" {
	//    bool __cdecl _OpenROM( char *, struct SSettings *);
//    void __cdecl _JoypadConfig( struct SJoypad *);
    void __cdecl _CheatCodeEntry( struct SCheatData *);
    void __cdecl _InitDLL( HANDLE hWnd);
    //void __cdecl _DirectXConfig( LPDIRECTDRAW plpDD, struct SSettings *Settings, struct sGUI *GUI, bool *showFPS);
    int __cdecl _GUIVersion ();
    void __cdecl _SearchForCheats (struct SCheatData *);
	//    void __cdecl _SettingsDialog (struct SSettings *, struct sGUI *);
	//    void __cdecl _AboutDialog (const char *);
	//    bool __cdecl _NetPlayConnect (char *, int *);
    //bool __cdecl _NetPlayOptions (struct SSettings *, struct SNetPlay *,
	//	struct SNPServer *);
	//    bool __cdecl _SoundOptions (struct SSettings *);
    //void __cdecl _ProgressDialog (const char *message, int progress);
	//    HWND __cdecl _MainWindow ();	
};

/*****************************************************************************/
/* WinProc                                                                   */
/*****************************************************************************/
bool8 SetupSound( long rate, bool8 sixteen_bit, bool8 stereo);
void RestoreGUIDisplay ();
void RestoreSNESDisplay ();
void FreezeUnfreeze (int slot, bool8 freeze);
void CheckDirectoryIsWritable (const char *filename);
static void CheckMenuStates ();
static void ResetFrameTimer ();
bool8 LoadROM (const char *filename);
static void EnableServer (bool8 enable);
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
    char buf [100];
    sprintf (buf, WINDOW_TITLE, VERSION);
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
				MessageBox( GUI.hWnd, Languages[ GUI.Language].errModeDD, "Snes9X - DirectDraw(2)", MB_OK | MB_ICONSTOP);
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
#ifdef USE_GLIDE
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
    char hostname [100];
	meow = hWnd;
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
					"Turbo Mode Activated");
				else
					S9xMessage (S9X_INFO, S9X_TURBO_MODE,
					"Turbo Mode Off");
			}
			else
			{
				Settings.TurboMode = TRUE;
				S9xMessage (S9X_INFO, S9X_TURBO_MODE, "Turbo Mode");
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
			S9xDisplayStateChange ("HDMA emulation", !Settings.DisableHDMA);
			break;
		case '1':
			PPU.BG_Forced ^= 1;
			S9xDisplayStateChange ("BG#1", !(PPU.BG_Forced & 1));
			break;
		case '2':
			PPU.BG_Forced ^= 2;
			S9xDisplayStateChange ("BG#2", !(PPU.BG_Forced & 2));
			break;
		case '3':
			PPU.BG_Forced ^= 4;
			S9xDisplayStateChange ("BG#3", !(PPU.BG_Forced & 4));
			break;
		case '4':
			PPU.BG_Forced ^= 8;
			S9xDisplayStateChange ("BG#4", !(PPU.BG_Forced & 8));
			break;
		case '5':
			PPU.BG_Forced ^= 16;
			S9xDisplayStateChange ("Sprites", !(PPU.BG_Forced & 16));
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
				S9xDisplayStateChange ("Joypad swapping", Settings.SwapJoypads);
			}
			break;
		case '7':
			{
				static char *controllers [] = {
					"Multiplayer 5 on #0", "Joypad on #0", "Mouse on #1",
						"Mouse on #0", "Superscope on #1", "Justifier 1 on #1", "Justifier 2 on #1"
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
			S9xDisplayStateChange ("Background layering hack", 
				Settings.BGLayering);
			break;
		case '9':
			if (GetKeyState (VK_SHIFT) & 0x80)
			{
				Settings.Mode7Interpolate ^= TRUE;
				S9xDisplayStateChange ("Mode 7 Interpolation", 
					Settings.Mode7Interpolate);
			}
			else
				if (Settings.SixteenBit)
				{	
					Settings.Transparency = !Settings.Transparency;
					S9xDisplayStateChange ("Transparency effects", 
						Settings.Transparency);
				}
				break;
				
		case VK_BACK:
			Settings.DisableGraphicWindows = !Settings.DisableGraphicWindows;
			S9xDisplayStateChange ("Graphic clip windows",
				!Settings.DisableGraphicWindows);
			break;
			
		case VK_PAUSE:
			Settings.Paused = Settings.Paused ^ true;
			S9xDisplayStateChange ("Pause", Settings.Paused);
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
			case '+':
			case '=':
				if (GetAsyncKeyState (VK_SHIFT))
				{
					// Increase emulated frame time by 1ms
					Settings.FrameTime += 1;
					ResetFrameTimer ();
					sprintf (InfoString, "Emulated frame time: %dms",
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
							S9xSetInfoString ("Auto frame skip");
						else
						{
							sprintf (InfoString, "Frame skip: %d",
								Settings.SkipFrames - 1);
							S9xSetInfoString (InfoString);
						}
				}
				break;
			case '-':
			case '_':
				if (GetAsyncKeyState (VK_SHIFT))
				{
					// Decrease emulated frame time by 1ms
					if (Settings.FrameTime >= 1)
						Settings.FrameTime -= 1;
					
					ResetFrameTimer ();
					sprintf (InfoString, "Emulated frame time: %dms",
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
							S9xSetInfoString ("Auto frame skip");
						else
						{
							sprintf (InfoString, "Frame skip: %d",
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
						S9xSetInfoString ("Turbo R Activated");
					else S9xSetInfoString ("Turbo R Deactivated");
					break;
				case ID_TURBO_L:
					GUI.TurboMask^=TURBO_L_MASK;
					if(GUI.TurboMask&TURBO_L_MASK)
						S9xSetInfoString ("Turbo L Activated");
					else S9xSetInfoString ("Turbo L Deactivated");
					break;
				case ID_TURBO_A:
					GUI.TurboMask^=TURBO_A_MASK;
					if(GUI.TurboMask&TURBO_A_MASK)
						S9xSetInfoString ("Turbo A Activated");
					else S9xSetInfoString ("Turbo A Deactivated");
					break;
				case ID_TURBO_B:
					GUI.TurboMask^=TURBO_B_MASK;
					if(GUI.TurboMask&TURBO_B_MASK)
						S9xSetInfoString ("Turbo B Activated");
					else S9xSetInfoString ("Turbo B Deactivated");
					break;
				case ID_TURBO_Y:
					GUI.TurboMask^=TURBO_Y_MASK;
					if(GUI.TurboMask&TURBO_Y_MASK)
						S9xSetInfoString ("Turbo Y Activated");
					else S9xSetInfoString ("Turbo Y Deactivated");
					break;
				case ID_TURBO_X:
					GUI.TurboMask^=TURBO_X_MASK;
					if(GUI.TurboMask&TURBO_X_MASK)
						S9xSetInfoString ("Turbo X Activated");
					else S9xSetInfoString ("Turbo X Deactivated");
					break;
				case ID_TURBO_START:
					GUI.TurboMask^=TURBO_STA_MASK;
					if(GUI.TurboMask&TURBO_STA_MASK)
						S9xSetInfoString ("Turbo Start Activated");
					else S9xSetInfoString ("Turbo Start Deactivated");
					break;
				case ID_TURBO_SELECT:
					GUI.TurboMask^=TURBO_SEL_MASK;
					if(GUI.TurboMask&TURBO_SEL_MASK)
						S9xSetInfoString ("Turbo Select Activated");
					else S9xSetInfoString ("Turbo Select Deactivated");
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
                            MessageBox (GUI.hWnd, "You will need to restart Snes9x before the output image\nprocessing option change will take effect.",
													 "Snes9x: Information",
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
								"Disconnect from the NetPlay server first.");
							break;
						}
#endif
						RestoreGUIDisplay ();
						INITCOMMONCONTROLSEX icex;
						
						icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
						icex.dwICC   = ICC_LISTVIEW_CLASSES|ICC_TREEVIEW_CLASSES;
						InitCommonControlsEx(&icex);
						
						if(1<=DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_OPEN_ROM), hWnd, DlgOpenROMProc, (LPARAM)filename))
							
							//						if (_OpenROM (filename, &Settings))
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
						//if (_NetPlayOptions (&Settings, &NetPlay, &NPServer))
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
					if (MessageBox (GUI.hWnd, "Your game will be reset after the ROM has been sent due to\nyour 'Sync Using Reset Game' setting.\n\n", "Snes9X: Warning",
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
                            {	MessageBox( GUI.hWnd, Languages[ GUI.Language].errInitDS, "Snes9X - DirectSound", MB_OK | MB_ICONINFORMATION);	}
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
					S9xDisplayStateChange ("Interpolated sound", Settings.InterpolatedSound);
                    break;
                case ID_SOUND_SYNC:
                    Settings.SoundSync = !Settings.SoundSync;
					S9xDisplayStateChange ("Sync sound", Settings.SoundSync);
                    break;
                case ID_SOUND_OPTIONS:
					{
						struct SSettings orig = Settings;
						RestoreGUIDisplay ();
						//if (_SoundOptions (&Settings))
						if(1<=DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_SOUND_OPTS),hWnd,DlgSoundConf, (LPARAM)&Settings))
						{
							if (orig.NextAPUEnabled != Settings.NextAPUEnabled)
							{
								if (!Settings.NextAPUEnabled)
								{
                                if (MessageBox (GUI.hWnd, "Disabling the sound CPU emulation will help to improve\nemulation speed but you will not hear any sound effects\nor music. If you later want to re-enable the sound CPU\nemulation you will need to reset your game before it will\ntake effect.\n\nAre you sure this is what you want?",
															 "Snes9X - Sound CPU Question",
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
                                    MessageBox (GUI.hWnd, "You will need to reset your game or load another one\nbefore enabling the sound CPU will take effect.", "Snes9X - Sound CPU Information",
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
											S9xSoftReset ();
											SetupSound (Settings.SoundPlaybackRate, 
												Settings.SixteenBitSound, 
												Settings.Stereo);
										}
										Settings.Paused = false;
										break;
								case ID_FILE_PAUSE:
									Settings.Paused = !Settings.Paused;
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
									_CheatCodeEntry (&Cheat);
									S9xSaveCheatFile (S9xGetFilename (".cht"));
									S9xApplyCheats ();
									RestoreSNESDisplay ();
									break;
								case ID_CHEAT_SEARCH:
									RestoreGUIDisplay ();
									_SearchForCheats (&Cheat);
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
									//                    _SettingsDialog (&Settings, &GUI);
									DialogBox(g_hInst, MAKEINTRESOURCE(IDD_EMU_SETTINGS), hWnd, DlgEmulatorProc);
									RestoreSNESDisplay ();
									break;
								case ID_HELP_ABOUT:
									RestoreGUIDisplay ();
									DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, DlgAboutProc);
									//_AboutDialog (VERSION);
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
														"Disconnect from the NetPlay server first.");
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
			S9xSetPause (PAUSE_INACTIVE_WINDOW);
		}
		else
		{
			S9xClearPause (PAUSE_INACTIVE_WINDOW);
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
			S9xClearPause (PAUSE_WINDOW_ICONISED);
		}
		if (wParam == SIZE_MINIMIZED || wParam == SIZE_MAXHIDE)
		{
#ifdef USE_GLIDE
			S9xGlideEnable (FALSE);
#endif
			S9xClearPause (PAUSE_WINDOW_ICONISED);
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
				if(!(GUI.Scale))
				{
					int x,y, startx, starty;
					x=GET_X_LPARAM(lParam);
					y=GET_Y_LPARAM(lParam);
					startx= size.right-IPPU.RenderedScreenWidth;
					startx/=2;
					starty= size.bottom-IPPU.RenderedScreenHeight;
					starty/=2;

					if(x<startx)
						GUI.MouseX=0;
					else if(x>(startx+IPPU.RenderedScreenWidth))
						GUI.MouseX=IPPU.RenderedScreenWidth;
					else GUI.MouseX=x-startx;

					if(y<starty)
						GUI.MouseY=0;
					else if(y>(starty+IPPU.RenderedScreenHeight))
						GUI.MouseY=IPPU.RenderedScreenHeight;
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
						sizey=IPPU.RenderedScreenHeight;
					else sizey=IPPU.RenderedScreenHeight*2;


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
					GUI.MouseY=(GUI.MouseY*IPPU.RenderedScreenHeight)/sizey;

				}
				else
				{
					GUI.MouseX=GET_X_LPARAM(lParam)*IPPU.RenderedScreenWidth/size.right;
					GUI.MouseY=GET_Y_LPARAM(lParam)*IPPU.RenderedScreenHeight/size.bottom;
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
			"Snes9X NetPlay Error", MB_OK | MB_ICONSTOP);
		RestoreSNESDisplay ();
		break;
	case WM_USER:
		if (NetPlay.ActionMsg [0] == 0)
			S9xRestoreWindowTitle ();
		else
		{
			char buf [NP_MAX_ACTION_LEN + 10];
			
			sprintf (buf, "%s %3d%%", NetPlay.ActionMsg, (int) lParam);
			SetWindowText (GUI.hWnd, buf);
		}
#if 0
		if ((int) lParam >= 0)
		{
			RestoreGUIDisplay ();
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NETPLAYPROGRESS), hWnd, DlgNPProgress);
			//_ProgressDialog (NetPlay.CurrentAction, (int) lParam);
		}
		else
		{
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_NETPLAYPROGRESS), hWnd, DlgNPProgress);
			//_ProgressDialog ("", (int) lParam);
			RestoreSNESDisplay ();
		}
#endif
		break;
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
    wndclass.hIcon = LoadIcon (NULL, "IDI_ICON1");
    wndclass.hCursor = NULL; //LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject (BLACK_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = "Snes9X: WndClass";
    
    GUI.hInstance = hInstance;
    
    if (!RegisterClass (&wndclass))
        return FALSE;
    
    if ((GUI.hMenu = LoadMenu (hInstance, MAKEINTRESOURCE( Languages[ GUI.Language].idMenu))) == NULL)
        return FALSE;
    
    char buf [100];
    sprintf (buf, WINDOW_TITLE, VERSION);
	
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
        "Snes9X: WndClass",
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
}

void S9xExtraUsage ()
{
}

/*****************************************************************************/
/* WinSave/WinLoad - Save/Load the settings to/from the registry             */
/*****************************************************************************/
#define SetKey( n, v)\
RegSetValueEx( hKey, n, 0, REG_BINARY, (const unsigned char *) &v, sizeof( v));
#define GetKey( n, v)\
	cbData = sizeof( v);\
RegQueryValueEx( hKey, n, 0, NULL, (unsigned char *) &v, &cbData);
#define GetKeyDef( n, v, d)\
	cbData = sizeof( v);\
	if( RegQueryValueEx( hKey, n, 0, NULL, (unsigned char *) &v, &cbData) != ERROR_SUCCESS)\
v = d;

void WinDeleteRegKey (HKEY hKeyRoot, const char *key)
{
    FILETIME last_update;
    DWORD count;
    HKEY hKey;
    char name_buf [100];
    char class_buf [200];
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
    char tmp [100];
	
    if (RegCreateKeyEx (HKEY_CURRENT_USER, 
		S9X_REG_KEY_BASE "\\" S9X_REG_KEY_VERSION, 0, NULL, 
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
        RegSetValueEx (hKey, "Freeze File Directory", 0, REG_SZ,
			(const unsigned char *) GUI.FreezeFileDir, 
			strlen (GUI.FreezeFileDir) + 1);
        SetKey( "Apply Cheats", Settings.ApplyCheats);
        SetKey( "Auto Save Delay", Settings.AutoSaveDelay);
		
        SetKey( "NetPlay Port", Settings.Port);
        SetKey( "NetPlay Sync By Reset", NPServer.SyncByReset);
        SetKey( "NetPlay Send ROM On Connect", NPServer.SendROMImageOnConnect);
        SetKey( "NetPlay Max Frame Skip", NetPlay.MaxFrameSkip);
        SetKey( "NetPlay Max Behind Frame Count", NetPlay.MaxBehindFrameCount);
        SetKey( "Bi-Linear Filter Mode7", Settings.Mode7Interpolate);
		
		RegSetValueEx (hKey, "StarOcean", 0, REG_SZ,
			(const unsigned char *) GUI.StarOceanPack, 
			strlen (GUI.StarOceanPack) + 1);
		RegSetValueEx (hKey, "FarEast", 0, REG_SZ,
			(const unsigned char *) GUI.FEOEZPack, 
			strlen (GUI.FEOEZPack) + 1);
		RegSetValueEx (hKey, "ShounenJump", 0, REG_SZ,
			(const unsigned char *) GUI.SJNSPack, 
			strlen (GUI.SJNSPack) + 1);
		RegSetValueEx (hKey, "SFA2PAL", 0, REG_SZ,
			(const unsigned char *) GUI.SFA2PALPack, 
			strlen (GUI.SFA2PALPack) + 1);
		RegSetValueEx (hKey, "SFA2NTSC", 0, REG_SZ,
			(const unsigned char *) GUI.SFA2NTSCPack, 
			strlen (GUI.SFA2NTSCPack) + 1);
		RegSetValueEx (hKey, "SPL4", 0, REG_SZ,
			(const unsigned char *) GUI.SPL4Pack, 
			strlen (GUI.SPL4Pack) + 1);
		RegSetValueEx (hKey, "Momotarou", 0, REG_SZ,
			(const unsigned char *) GUI.MDHPack, 
			strlen (GUI.MDHPack) + 1);
		RegSetValueEx (hKey, "SFZ2", 0, REG_SZ,
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
                sprintf (tmp, "Recent Game #%d", i + 1);
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
    GUI.IgnoreNextMouseMove = false;
    GUI.window_size.left = 0;
    GUI.window_size.right = 264 + 256;
    GUI.window_size.top = 0;
    GUI.window_size.bottom = 285 + 239;
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
	
    Settings.AutoSaveDelay = 0;
    Settings.ApplyCheats = true;
	
    Settings.TurboMode = false;
    Settings.TurboSkipFrames = 15;
    GUI.TurboModeToggle = true;
    Settings.AutoMaxSkipFrames = 10;
	
    Settings.Port = 1996;
    NetPlay.MaxFrameSkip = 10;
    NetPlay.MaxBehindFrameCount = 10;
    NPServer.SyncByReset = true;
    NPServer.SendROMImageOnConnect = false;
	
    GUI.FreezeFileDir [0] = 0;
	Settings.SampleCatchup=true;
	Settings.TakeScreenshot=false;
	Settings.StretchScreenshots=1;
}

void WinLoad( void)
{
    HKEY hKey;
    unsigned long cbData;
    char tmp [100];
    char path [_MAX_PATH + 1];
	
    WinSetDefaultValues ();
	
    if (RegOpenKeyEx (HKEY_CURRENT_USER, 
		S9X_REG_KEY_BASE "\\" S9X_REG_KEY_VERSION,
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
        
        GetKeyDef( "NetPlay Port", Settings.Port, 1996);
        GetKeyDef( "NetPlay Sync By Reset", NPServer.SyncByReset, true);
        GetKeyDef( "NetPlay Send ROM On Connect", NPServer.SendROMImageOnConnect, false);
        GetKeyDef( "NetPlay Max Frame Skip", NetPlay.MaxFrameSkip, 10);
        GetKeyDef( "NetPlay Max Behind Frame Count", NetPlay.MaxBehindFrameCount, 10);
		
        cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "Freeze File Directory", 0, NULL,
			(unsigned char *) GUI.FreezeFileDir, &cbData);
		
        cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "StarOcean", 0, NULL,
			(unsigned char *) GUI.StarOceanPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "FarEast", 0, NULL,
			(unsigned char *) GUI.FEOEZPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "SFA2NTSC", 0, NULL,
			(unsigned char *) GUI.SFA2NTSCPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "SFA2PAL", 0, NULL,
			(unsigned char *) GUI.SFA2PALPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "Momotarou", 0, NULL,
			(unsigned char *) GUI.MDHPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "SFZ2", 0, NULL,
			(unsigned char *) GUI.SFZ2Pack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "ShounenJump", 0, NULL,
			(unsigned char *) GUI.SJNSPack, &cbData);
		cbData = _MAX_PATH + 1;
        RegQueryValueEx (hKey, "SPL4", 0, NULL,
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
            sprintf (tmp, "Recent Game #%d", i + 1);
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
                sprintf (tmp, "Recent Game #%d", j + 1);
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
				Settings.Paused || Settings.StopEmulation || Settings.ForcedPause)
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
	// GUI.hWnd = _MainWindow ();
	//    SetMenu (GUI.hWnd, GUI.hMenu);
    _InitDLL (GUI.hWnd);
    
    if (_GUIVersion () != GUI_VERSION)
    {
	MessageBox (GUI.hWnd, "Incorrect version of Snes9XW.dll dynamic-link library file found.\n\nPlease check you extracted both the Snes9XW.exe and Snes9XW.dll\nfiles into the same directory from the Snes9XW distribution\nzip file.", "Snes9X - Incorrect Snes9XW.dll Version", MB_OK | MB_ICONSTOP);
	return false; 
    }
	
    DirectX.InitDirectSound ();
	
    if (!SetupSound (Settings.SoundPlaybackRate, Settings.SixteenBitSound, 
		Settings.Stereo))
    {	
        MessageBox( GUI.hWnd, Languages[ GUI.Language].errInitDS, "Snes9X - DirectSound", MB_OK | MB_ICONINFORMATION);
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
        MessageBox (GUI.hWnd, Languages[ GUI.Language].errInitDD, "Snes9X - DirectX", MB_OK | MB_ICONSTOP);
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
        MessageBox( GUI.hWnd, Languages[ GUI.Language].errModeDD, "Snes9X - DirectDraw(7)", MB_OK | MB_ICONSTOP);
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
        MessageBox (GUI.hWnd, "InitD3D failed.", "InitD3D Error", MB_OK | MB_ICONINFORMATION);
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
        MessageBox( GUI.hWnd, Languages[ GUI.Language].errFrameTimer, "Snes9X - Frame Timer", MB_OK | MB_ICONINFORMATION);
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
        while (Settings.StopEmulation || Settings.Paused ||
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
            S9xMainLoop();
            DirectX.FrameCount++;
#ifdef NETPLAY_SUPPORT
        }
#endif
        if (CPU.Flags & DEBUG_MODE_FLAG)
        {
            Settings.Paused = TRUE;
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
	
    if (OPENGL_MODE)
        S9xOpenGLDeinit ();
	
    WinSave ();
	Memory.Deinit();
#ifdef USE_GLIDE
	if(Settings.GlideEnable)
		S9xGlideDeinit();
	else if (Settings.OpenGLEnable)
#else
		if (Settings.OpenGLEnable)
#endif 
			S9xOpenGLDeinit();
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
        MessageBox (GUI.hWnd, Languages[ GUI.Language].errModeDD, "Snes9X - DirectDraw(1)", MB_OK | MB_ICONSTOP);
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
        MessageBox (GUI.hWnd, Languages[ GUI.Language].errModeDD, "Snes9X - DirectDraw(4)", MB_OK | MB_ICONSTOP);
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
    static char filename [_MAX_PATH + 1];
    char drive [_MAX_DRIVE + 1];
    char dir [_MAX_DIR + 1];
    char fname [_MAX_FNAME + 1];
    char ext [_MAX_EXT + 1];
    
#ifdef NETPLAY_SUPPORT
    if (!freeze && Settings.NetPlay && !Settings.NetPlayServer)
    {
        S9xMessage (S9X_INFO, S9X_NETPLAY_NOT_SERVER,
			"Only the server is allowed to load freeze files.");
        return;
    }
#endif
	
    _splitpath (Memory.ROMFilename, drive, dir, fname, ext);
    sprintf (ext, ".%03d", slot);
    if (GUI.FreezeFileDir [0])
    {
        strcpy (filename, GUI.FreezeFileDir);
        strcat (filename, "\\");
        strcat (filename, fname);
        strcat (filename, ext);
    }
    else
        _makepath (filename, drive, dir, fname, ext);
    
    if (!freeze && _access (filename, 0) != 0 && slot < 10)
    {
        static char *digits = "t123456789";
        sprintf (ext, ".zs%c", digits [slot]);
        if (GUI.FreezeFileDir [0])
        {
            strcpy (filename, GUI.FreezeFileDir);
            strcat (filename, "\\");
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
            S9xMessage (S9X_INFO, S9X_FREEZE_FILE_INFO, S9xBasename (filename));
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
	MessageBox (GUI.hWnd, "The folder where Snes9X saves emulated save RAM files and\ngame save positions (freeze files) is currently set to a\nread-only folder.\n\nIf you do not change the game save folder, Snes9X will be\nunable to save your progress in this game. Change the folder\nfrom the Settings Dialog available from the Options menu.\n\nThe default the save folder is the same as the ROM image\nfolder, if no value is set.\n",
							 "Snes9X: Unable to save file warning", 
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
        SetupSound (Settings.SoundPlaybackRate, 
			Settings.SixteenBitSound, 
			Settings.Stereo);
        ResetFrameTimer ();
        return (TRUE);
    }
    return (FALSE);
}

bool8 S9xLoadROMImage (const char *string)
{
    RestoreGUIDisplay ();
    char *buf = new char [200 + strlen (string)];
    sprintf (buf, "The NetPlay server is requesting you load the following game:\n '%s'",
		string);
	
    MessageBox (GUI.hWnd, buf,
		"Snes9X: Information",
		MB_OK | MB_ICONINFORMATION);
    
    delete buf;
	
    char FileName [_MAX_PATH];
	
	INITCOMMONCONTROLSEX icex;
	
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC   = ICC_LISTVIEW_CLASSES|ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	
	if(1<=DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_OPEN_ROM), GUI.hWnd, DlgOpenROMProc, (LPARAM)FileName))
		//    if (_OpenROM (FileName, &Settings))
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
        MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }
    
    if (!SetPixelFormat (GUI.hDC, PixelFormat, &pfd))
    {
        MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return (FALSE);
    }
    
    if (!(GUI.hRC = wglCreateContext (GUI.hDC)))
    {
        MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
        return (FALSE);
    }
    
    if (!wglMakeCurrent (GUI.hDC, GUI.hRC))
    {
        MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
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
            char name [256 + 10];
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
                sprintf (name, "&%c. %s", i < 9 ? '1' + i : 'A' + i - 9,
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
	static struct SSettings* set;
	switch(msg)
	{
	case WM_INITDIALOG:
		
		set= (struct SSettings *)lParam;
#ifndef MK_APU
		SetDlgItemText(hDlg,IDC_LINEAR_INTER, "Linear &Interpolation of Sample Data");
#endif
		SendDlgItemMessage(hDlg, IDC_DRIVER, CB_INSERTSTRING,0,(LPARAM)TEXT("Snes9x DirectSound"));
		SendDlgItemMessage(hDlg, IDC_DRIVER, CB_INSERTSTRING,1,(LPARAM)TEXT("FMOD DirectSound"));
		SendDlgItemMessage(hDlg, IDC_DRIVER, CB_INSERTSTRING,2,(LPARAM)TEXT("FMOD Windows Multimedia"));
		SendDlgItemMessage(hDlg, IDC_DRIVER, CB_INSERTSTRING,3,(LPARAM)TEXT("FMOD A3D"));
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
		
		if(!set->DisableSampleCaching)
			SendDlgItemMessage(hDlg,IDC_CACHING,BM_SETCHECK,BST_CHECKED,0);
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
				set->DisableSampleCaching=(!IsDlgButtonChecked(hDlg, IDC_CACHING));
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
								EnableWindow(GetDlgItem(hDlg, IDC_CACHING), TRUE);
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
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			if(LoadUp7110==&SPC7110Grab)
				SendDlgItemMessage(hDlg, IDC_SPC7110_SOME, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			else if(LoadUp7110==&SPC7110Open)
				SendDlgItemMessage(hDlg, IDC_SPC7110_FILE, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			else	SendDlgItemMessage(hDlg, IDC_SPC7110_ALL, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			SendDlgItemMessage(hDlg, IDC_SPIN_CACHE, UDM_SETRANGE, 0, MAKELONG(20,1));
			SendDlgItemMessage(hDlg, IDC_SPIN_CACHE, UDM_SETPOS, 0, MAKELONG(cacheMegs,0));
			return true;
		}
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

int CALLBACK DlgInfoProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
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
				/*
				if(Memory.Hacked)
				{
				SetTextColor((HDC)wParam, RGB(255,0,0));
				SetBkColor((HDC)wParam, RGB(0,0,0));
				}
				else if(Memory.Iffy)
				{
				SetTextColor((HDC)wParam, RGB(255,255,0));
				SetBkColor((HDC)wParam, RGB(0,0,0));
				}
				else if(Memory.Iformat==2)
				{
				SetTextColor((HDC)wParam, RGB(0,255,255));
				SetBkColor((HDC)wParam, RGB(0,0,0));
				}
				else if(Memory.Iformat==1)
				{
				SetTextColor((HDC)wParam, RGB(0,255,0));
				SetBkColor((HDC)wParam, RGB(0,0,0));
				}
				*/
				return true;break;
			case WM_COMMAND:
				{
					switch(wParam)
					{
					case IDOK:
					case IDCANCEL:
						EndDialog(hDlg, 0);
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
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			SetDlgItemText(hDlg, IDC_DISCLAIMER, DISCLAIMER_TEXT);
			SetWindowText(hDlg, "About " APP_NAME);
		}
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
			case IDCANCEL:
				EndDialog(hDlg, 0);
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
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			SetWindowText(hDlg, APP_NAME " - Emulator Settings");
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
					char path[MAX_PATH];
					char title[]="Select Freeze Folder";
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
				return true;
				break;
			default: return false; break;
			}
		}
	default:return false;
	}
}

#define SKIP_FLOPPY

bool ExtensionIsValid(const char * filename)
{
	ExtList* curr=valid_ext;
	while(curr!=NULL)
	{
		if(curr->extension==NULL)
		{
			if(NULL==strstr(filename, "."))
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

bool IsCompressed(char* filename)
{
	ExtList* curr=valid_ext;
	while(curr!=NULL)
	{
		if(curr->extension==NULL)
		{
			if(NULL==strstr(filename, "."))
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

void rominfo(const char *filename, char *namebuffer, char *sizebuffer)
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

void GetPathFromTree( HWND hDlg, UINT tree, char* selected, HTREEITEM hItem)
{
	TVITEM tv;
	char temp[MAX_PATH];
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
	
	sprintf(selected, "%s", temp);
	while(TreeView_GetParent(GetDlgItem(hDlg, tree), hTreeTemp)!=NULL)
	{
		temp[0]='\0';
		hTreeTemp=TreeView_GetParent(GetDlgItem(hDlg, tree), hTreeTemp);
		tv.mask=TVIF_HANDLE|TVIF_TEXT;
		tv.hItem=hTreeTemp;
		tv.pszText=temp;
		tv.cchTextMax =MAX_PATH;
		TreeView_GetItem(GetDlgItem(hDlg, tree), &tv);
		sprintf(temp, "%s\\%s",temp, selected);
		strcpy(selected, temp);
	}
}

int CALLBACK DlgOpenROMProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int rv=0;
	static HWND hSplit;
	static HIMAGELIST hIcons;
	static char *filename;
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			filename=(char*)lParam;
			RECT treeRect;
			RECT listRect;
			WNDCLASSEX wcex;
			char tempclassname[]="S9xSplitter";
			ZeroMemory(&wcex, sizeof(WNDCLASSEX));
			wcex.cbSize=sizeof(WNDCLASSEX);
			wcex.hInstance=g_hInst;
			wcex.lpfnWndProc=DlgChildSplitProc;
			wcex.lpszClassName=tempclassname;
			wcex.hCursor=LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
			ATOM aSplitter=RegisterClassEx(&wcex);
			GetWindowRect(GetDlgItem(hDlg, IDC_ROM_DIR), &treeRect);
			GetWindowRect(GetDlgItem(hDlg, IDC_ROMLIST), &listRect);
			POINT p;
			
			ListView_SetExtendedListViewStyle(GetDlgItem(hDlg, IDC_ROMLIST), LVS_EX_FULLROWSELECT);
			
			p.x=treeRect.right;
			p.y=treeRect.top;
			ScreenToClient(hDlg, &p);
			hSplit=CreateWindow("S9xSplitter", "",WS_CHILD|WS_VISIBLE , p.x, p.y, listRect.left-treeRect.right , listRect.bottom-listRect.top, hDlg,NULL, g_hInst,0);
			
			LVCOLUMN col;
			char temp[32];
			strcpy(temp,"File");
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=0;
			col.cx=122;
			col.cchTextMax=5;
			col.pszText=temp;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_ROMLIST),    0,   &col);
			
			strcpy(temp,"Description");
			ZeroMemory(&col, sizeof(LVCOLUMN));
			col.mask=LVCF_FMT|LVCF_ORDER|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
			col.fmt=LVCFMT_LEFT;
			col.iOrder=1;
			col.cx=150;
			col.cchTextMax=32;
			col.pszText=temp;
			col.iSubItem=1;
			
			ListView_InsertColumn(GetDlgItem(hDlg,IDC_ROMLIST),    1,   &col);
			
			
			strcpy(temp,"Size");
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
			
			DWORD driveMask=GetLogicalDrives();
#ifndef SKIP_FLOPPY
			for (int i=0;i<26;i++)
#else
				for (int i=2;i<26;i++)
#endif
				{
					if(driveMask&(1<<i))
					{
						char driveName[4];
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
						//							tvis.item.mask=TVIF_TEXT;
						tvis.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
						
						if(driveType==DRIVE_FIXED)
							tvis.item.iSelectedImage=tvis.item.iImage=0;
						else if(driveType==DRIVE_REMOTE)
							tvis.item.iSelectedImage=tvis.item.iImage=2;
						else tvis.item.iSelectedImage=tvis.item.iImage=1;
						
						tvis.item.pszText=driveName;

						HTREEITEM hTwee=TreeView_InsertItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tvis);
							
						char temp[10];
						strcpy(temp, driveName);
						strcat(temp, "\\*");
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
									if(strcmp(wfd2.cFileName, ".")&&strcmp(wfd2.cFileName, ".."))
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
				HKEY hKey;
				if(!RegOpenKeyEx(HKEY_CURRENT_USER, MY_REG_KEY, 0, KEY_ALL_ACCESS, &hKey))
				{
					DWORD dw=MAX_PATH;
					char buffer[MAX_PATH];
					buffer[0]='\0';
					char blah[MAX_PATH];
					RegQueryValueEx(hKey, "Last Directory", NULL,NULL, (uint8*)buffer,&dw);
					RegCloseKey(hKey);
					
					HTREEITEM hTreePrev,hTreeRoot;
					
					hTreePrev=TreeView_GetRoot(GetDlgItem(hDlg, IDC_ROM_DIR));
					hTreeRoot=hTreePrev;
					HTREEITEM hTemp=hTreePrev;
					char* temp=buffer;
					char * temp2;
					do
					{
						temp2=strstr(temp, "\\");
						
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
									TreeView_SelectItem(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev);
								else TreeView_Expand(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev, TVE_EXPAND);
								hTemp=TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR), hTreePrev);
							}
						}
						else
						{
							if(NULL==temp2)
								TreeView_SelectItem(GetDlgItem(hDlg, IDC_ROM_DIR), hTemp);
							else TreeView_Expand(GetDlgItem(hDlg, IDC_ROM_DIR), hTemp, TVE_EXPAND);
							
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
						char *tmp, *tmp2;
						tmp=Memory.ROMFilename;
						while(tmp2=strstr(tmp, "\\"))
							tmp=tmp2+1;

						lvfi.flags=LVFI_STRING;
						lvfi.psz=tmp;

						int idx=ListView_FindItem(GetDlgItem(hDlg,IDC_ROMLIST), -1, &lvfi);
						ListView_SetItemState(GetDlgItem(hDlg,IDC_ROMLIST), idx, LVIS_SELECTED|LVIS_FOCUSED,LVIS_FOCUSED|LVIS_SELECTED);
						ListView_EnsureVisible(GetDlgItem(hDlg,IDC_ROMLIST), idx, FALSE);

					}

				}
				return true; //true sets the keyboard focus, in case we need this elsewhere
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					LVITEM lvi;
					ZeroMemory(&lvi, sizeof(LVITEM));
					//get selections
					//strcpy(filename, ?);
					int list_index=ListView_GetSelectionMark(GetDlgItem(hDlg,IDC_ROMLIST));
					if(list_index!=-1)
					{
						rv=1;
						char temp[MAX_PATH];
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
						sprintf(temp, "%s\\%s", temp, filename);
						
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
							sprintf(temp, "%s\\%s",temp, filename);
							strcpy(filename, temp);
						}
						
						int iTemp=SendDlgItemMessage(hDlg, IDC_MEM_TYPE,CB_GETCURSEL,0,0);
						
						Settings.ForceHiROM=Settings.ForceLoROM=FALSE;
						if(iTemp==1)
							Settings.ForceHiROM=TRUE;
						else if(iTemp==2)
							Settings.ForceLoROM=TRUE;
						
						iTemp=SendDlgItemMessage(hDlg, IDC_INTERLEAVE,CB_GETCURSEL,0,0);
						
						Settings.ForceNotInterleaved=Settings.ForceInterleaved=Settings.ForceInterleaved2=FALSE;
						if(iTemp==1)
							Settings.ForceNotInterleaved=TRUE;
						else if(iTemp==2)
							Settings.ForceInterleaved=TRUE;
						else if(iTemp==3)
							Settings.ForceInterleaved2=TRUE;
						
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
				DestroyWindow(hSplit);
				UnregisterClass("S9xSplitter", g_hInst);
				TreeView_DeleteAllItems(GetDlgItem(hDlg, IDC_ROM_DIR));
				ListView_DeleteAllItems(GetDlgItem(hDlg, IDC_ROMLIST));
				return true;
				break;
			default: return false; break;
			}
		}
	case WM_NOTIFY:
		{
			NMHDR* pNmh=(NMHDR*)lParam;
			switch(pNmh->idFrom)
			{
			case IDC_ROMLIST:
				{
					switch(pNmh->code)
					{
					case NM_DBLCLK:
						{
							PostMessage(hDlg, WM_COMMAND, (WPARAM)((IDC_ROMLIST<<16)|(IDOK)),(LPARAM)GetDlgItem(hDlg, IDC_ROMLIST));
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
							char selected[MAX_PATH];
							char temp[MAX_PATH];
							NMTREEVIEW* nmTv=(NMTREEVIEW*)lParam;
							while(TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR),nmTv->itemNew.hItem))
							{
								TreeView_DeleteItem(GetDlgItem(hDlg, IDC_ROM_DIR), TreeView_GetChild(GetDlgItem(hDlg, IDC_ROM_DIR),nmTv->itemNew.hItem));
							}
							if(nmTv->action&TVE_EXPAND)
							{
								
								GetPathFromTree(hDlg, IDC_ROM_DIR, selected,nmTv->itemNew.hItem);
								WIN32_FIND_DATA wfd;
								ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
								strcat(selected, "\\*");
								HANDLE hFind=FindFirstFile(selected,&wfd);
								selected[(strlen(selected)-1)]='\0';
								
								do
								{
									if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
									{
										if(strcmp(wfd.cFileName, ".")&&strcmp(wfd.cFileName, ".."))
										{
											//skip these, add the rest.
											TV_INSERTSTRUCT tvis;
											ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
											tvis.hParent=nmTv->itemNew.hItem;
											tvis.hInsertAfter=TVI_SORT;
											tvis.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
											tvis.item.pszText=wfd.cFileName;
											tvis.item.cchTextMax=MAX_PATH;
											tvis.item.iImage=4;
											tvis.item.iSelectedImage=3;
											HTREEITEM hNewTree=TreeView_InsertItem(GetDlgItem(hDlg, IDC_ROM_DIR),&tvis);
											
											strcpy(temp, selected);
											strcat(temp, wfd.cFileName);
											strcat(temp, "\\*");
											
											bool subdir=false;
											WIN32_FIND_DATA wfd2;
											ZeroMemory(&wfd2, sizeof(WIN32_FIND_DATA));
											HANDLE hFind2=FindFirstFile(temp,&wfd2);											
											do
											{
												if(wfd2.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
												{
													if(strcmp(wfd2.cFileName, ".")&&strcmp(wfd2.cFileName, ".."))
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
							TVITEM tv;
							char temp[MAX_PATH];
							char selected[MAX_PATH];
							temp[0]='\0';
							ZeroMemory(&tv, sizeof(TVITEM));
							HTREEITEM hTreeItem=TreeView_GetSelection(GetDlgItem(hDlg, IDC_ROM_DIR));
							HTREEITEM hTreeTemp=hTreeItem;
							
							GetPathFromTree(hDlg, IDC_ROM_DIR, selected, hTreeItem);
							SendDlgItemMessage(hDlg, IDC_ROMLIST, WM_SETREDRAW, 0, 0);
							
							ListView_DeleteAllItems(GetDlgItem(hDlg, IDC_ROMLIST));
							
							//Add items here.
							
							WIN32_FIND_DATA wfd;
							ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
							
							strcat(selected, "\\*");
							
							HANDLE hFind=FindFirstFile(selected, &wfd);
							selected[(strlen(selected)-1)]='\0';
							do
							{
								if(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
									continue;
								if(ExtensionIsValid(wfd.cFileName))
								{
									int index;
									LVITEM lvi;
									ZeroMemory(&lvi, sizeof(LVITEM));
									lvi.mask=LVIF_TEXT;
									lvi.pszText=wfd.cFileName;
									lvi.cchTextMax=strlen(wfd.cFileName);
									index=ListView_InsertItem(GetDlgItem(hDlg,IDC_ROMLIST), &lvi);
									
									if(IsCompressed(wfd.cFileName))
									{
										char buffer[32];
										strcpy(buffer, "Not Available");
										ZeroMemory(&lvi, sizeof(LVITEM));
										lvi.iItem=index;
										lvi.iSubItem=1;
										lvi.mask=LVIF_TEXT;
										lvi.pszText=buffer;
										lvi.cchTextMax=32;
										ListView_SetItem(GetDlgItem(hDlg,IDC_ROMLIST), &lvi);
										
										strcpy(buffer, "? Mbits");
										lvi.iSubItem=2;
										ListView_SetItem(GetDlgItem(hDlg,IDC_ROMLIST), &lvi);
									}
									else
									{
										char path[MAX_PATH];
										char buffer[32];
										char buffer2[32];
										strcpy(path, selected);
										strcat(path, wfd.cFileName);
										rominfo(path, buffer, buffer2);
										ZeroMemory(&lvi, sizeof(LVITEM));
										lvi.iItem=index;
										lvi.iSubItem=1;
										lvi.mask=LVIF_TEXT;
										lvi.pszText=buffer;
										lvi.cchTextMax=32;
										ListView_SetItem(GetDlgItem(hDlg,IDC_ROMLIST), &lvi);
										
										lvi.iSubItem=2;
										lvi.pszText=buffer2;
										ListView_SetItem(GetDlgItem(hDlg,IDC_ROMLIST), &lvi);
										
									}
								}
							}
							while(FindNextFile(hFind, &wfd));
							
							FindClose(hFind);
							
							ListView_SetItemState(GetDlgItem(hDlg,IDC_ROMLIST), 0, LVIS_SELECTED|LVIS_FOCUSED,LVIS_FOCUSED|LVIS_SELECTED);
							SendDlgItemMessage(hDlg, IDC_ROMLIST, WM_SETREDRAW, (WPARAM)TRUE, 0);
							
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
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
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
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			SetDlgItemText(hDlg, IDC_STAR_OCEAN, GUI.StarOceanPack);
			SetDlgItemText(hDlg, IDC_SFA2, GUI.SFA2NTSCPack);
			SetDlgItemText(hDlg, IDC_SFA2E, GUI.SFA2PALPack);
			SetDlgItemText(hDlg, IDC_SJNS, GUI.SJNSPack);
			SetDlgItemText(hDlg, IDC_SFZ2, GUI.SFZ2Pack);
			SetDlgItemText(hDlg, IDC_MDH, GUI.MDHPack);
			SetDlgItemText(hDlg, IDC_SPL4, GUI.SPL4Pack);
			SetDlgItemText(hDlg, IDC_FEOEZ, GUI.FEOEZPack);
		}
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

int CALLBACK DlgNetConnect(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
					return TRUE;
				}
				else
				{
					EndDialog(hDlg,0);
					return TRUE;
				}

				break;
				//}
			}
		case IDCANCEL:
			{
				EndDialog(hDlg, 0);
				return TRUE;
			}
		default:break;
		}
	}
	return FALSE;
}
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
			curr->extension=new char[strlen(buffer)-1];
			strncpy(curr->extension, buffer, strlen(buffer)-1);
			curr->extension[strlen(buffer)-1]='\0';
		}
	}
	while(!in.eof());
	in.close();
}

void MakeExtFile(void)
{
	ofstream out;
	out.open("Valid.Ext");
	
	out<<"smcN"<<endl<<"zipY"<<endl<<"gzY"<<endl<<"swcN"<<endl<<"figN"<<endl;
	out<<"058N"<<endl<<"078N"<<endl<<"japN"<<endl<<"usaN"<<endl<<"048N"<<endl;
	out<<"eurN"<<endl<<"sfcN"<<endl<<"1N"<<endl<<"mgdN"<<endl<<"ufoN"<<endl;
	out<<"binN"<<endl<<"gd3N"<<endl<<"mghN"<<endl<<"gd7N"<<endl<<"ausN"<<endl;
	out<<"dx2N"<<endl<<"aN";
	out.close();
	SetFileAttributes("Valid.Ext", FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_READONLY);
};

int CALLBACK DlgNPOptions(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
				MessageBox(hDlg,defPort,defPort,MB_OK);	
				Settings.NetPlayServer = IsDlgButtonChecked(hDlg,IDC_ACTASSERVER);
				NPServer.SendROMImageOnConnect = IsDlgButtonChecked(hDlg,IDC_SENDROM);
				NPServer.SyncByReset = IsDlgButtonChecked(hDlg,IDC_SYNCBYRESET);
				
				EndDialog(hDlg,0);
				return TRUE;
			}
		case IDCANCEL:
			{
				EndDialog(hDlg,0);
				return TRUE;
			}
		}
	}
	return FALSE;
}

HRESULT CALLBACK EnumModesCallback( LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext)
{
	char s[80];
	
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
        (lpDDSurfaceDesc->dwWidth < 256 ||
		lpDDSurfaceDesc->dwHeight < 239))
	{
        return DDENUMRET_OK;
	}
	sprintf( s, "%dx%dx%d", lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight, lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount);
	RegOpenKeyEx(HKEY_CURRENT_USER,MY_REG_KEY  "\\1.x\\DisplayModes",0,KEY_ALL_ACCESS, &hKey);
	
	
	LVITEM lvi;
	
	ZeroMemory(&lvi, sizeof(LVITEM));
	lvi.iItem=dmindex;
	lvi.mask=LVIF_TEXT;
	lvi.pszText=s;
	lvi.cchTextMax=80;
	//lvi.lParam=dmindex;
	index=ListView_InsertItem(hDlg, &lvi);
	dm[dmindex].width=lpDDSurfaceDesc->dwWidth;
	dm[dmindex].height=lpDDSurfaceDesc->dwHeight;
	dm[dmindex].depth=lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;
	
	if(RegQueryValueEx(hKey,s,0,&type,val,&nuv)==ERROR_SUCCESS)
	{
		//sprintf(s,"%d %d %d %d",val[0],val[1],val[2],val[3]);
		//MessageBox(hDlg,s,temp,MB_OK);
		switch(val[0])
		{
		case 1:
			strcpy(s,"Failed");
			dm[dmindex].status=1;
			break;
		case 3:
			strcpy(s,"Works");
			dm[dmindex].status=3;
			break;
		default:
			strcpy(s,"Untested");
			dm[dmindex].status=0;
		}
	}
	else
	{
		strcpy(s,"Untested");
		dm[dmindex].status=0;
		//MessageBox(hDlg,s,temp,MB_OK);
	}
	dmindex++;
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
	case WM_INITDIALOG:
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
		if(Settings.SixteenBit)
		{
			SendDlgItemMessage(hDlg, IDC_RENDER16BIT, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
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
		strcpy(temp,"Normal");
		SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
		/* Forward thinking by Gary? Kreed's Super2xSaI
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
		strcpy(temp,"3D/FX bi-linear");
		SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
		strcpy(temp,"OpenGL bi-linear");
		SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_ADDSTRING,0,(LPARAM) (LPCTSTR)temp);
		SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_SETCURSEL,(WPARAM)GUI.NextScale,0);
		
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
		dmindex=0;
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

			
		case IDOK:
			Settings.SixteenBit = IsDlgButtonChecked(hDlg, IDC_RENDER16BIT);
			Settings.Transparency = IsDlgButtonChecked(hDlg, IDC_TRANS);
			Settings.SupportHiRes = IsDlgButtonChecked(hDlg, IDC_HIRES);
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
			GUI.NextScale = (int)SendDlgItemMessage(hDlg,IDC_FILTERBOX,CB_GETCURSEL,0,0);
			dmindex=0;
			EndDialog(hDlg,0);
			return false;
			
			
			
		case IDCANCEL:
			dmindex=0;
			EndDialog(hDlg,0);
			return false;
		case IDC_TESTMODE:
			
			//DirectX.lpDD->SetCooperativeLevel(hDlg,DDSCL_FULLSCREEN|DDSCL_ALLOWMODEX|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT);
			index=ListView_GetSelectionMark(GetDlgItem(hDlg,IDC_VIDMODELIST));
			if(index != -1)
			{
				sprintf(temp,"%dx%dx%d",dm[index].width,dm[index].height,dm[index].depth);
				if(DirectX.lpDD->SetDisplayMode(dm[index].width,dm[index].height,dm[index].depth)!=DD_OK)
				{
					
					MessageBox(hDlg,"There was an error testing the selected mode","DD_NOTOK",MB_OK);
					LVITEM lvi;
					dm[index].status=1;
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
						
						RegSetValueEx(hKey,temp,0,REG_DWORD,(LPBYTE)&dm[index].status,sizeof(DWORD));
						
						RegCloseKey(hKey);
						
					}
				}
				else
				{
					


					
					if(MessageBox(hDlg,"Did it Work?","Test Display Mode",MB_YESNO)==IDYES)
					{	
						dm[index].status=3;
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
							
							if(RegSetValueEx(hKey,temp,0,REG_DWORD,(LPBYTE)&dm[index].status,sizeof(DWORD))==ERROR_SUCCESS)
							{
								
							}
							
							RegCloseKey(hKey);
							
						}
					}
					else
					{
						dm[index].status=1;
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
							RegSetValueEx(hKey,temp,0,REG_DWORD,(LPBYTE)&dm[index].status,sizeof(DWORD));
							
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
			
			switch (dm[index].status)
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
					RegSetValueEx(hKey,"Width",0,REG_DWORD,(LPBYTE)&dm[index].width,sizeof(DWORD));
					RegSetValueEx(hKey,"Height",0,REG_DWORD,(LPBYTE)&dm[index].height,sizeof(DWORD));
					RegSetValueEx(hKey,"Depth",0,REG_DWORD,(LPBYTE)&dm[index].depth,sizeof(DWORD));
					RegCloseKey(hKey);
					
				}
				
				GUI.Width=dm[index].width;
				GUI.Height=dm[index].height;
				GUI.Depth=dm[index].depth;
			}
			sprintf(s,"Current Mode: %dx%dx%d",GUI.Width,GUI.Height,GUI.Depth);
			SendDlgItemMessage(hDlg,IDC_CURRMODE,WM_SETTEXT,0,(LPARAM)s);
			return false;
			
			
		}
		
		
	}
	
	return false;
}
void TranslateKey(WORD keyz,char *out);
HWND funky;
SJoyState JoystickA [16];
WORD *currentButton;





void JoystickChanged( short ID, short Movement)
{	
	WORD JoyKey;
	char temp[100];
    JoyKey = 0x8000;
	JoyKey |= (WORD)(ID << 8);
	JoyKey |= Movement;
	TranslateKey(JoyKey,temp);
	*currentButton = JoyKey;
	SendMessage(funky,WM_SETTEXT,0,(LPARAM)temp);
}

int FunkyNormalize(int cur, int min, int max)
{
	int Result = 0;

    if ((max - min) == 0)
	
        return (Result);

    Result = cur - min;
    Result = (Result * 200) / (max - min);
    Result -= 100;

    return Result;
}


void CheckAxis (short joy, short control, int val, 
                                       int min, int max,
                                       bool &first, bool &second)
{


	
    if (FunkyNormalize (val, min, max) < -S9X_JOY_NEUTRAL)
	//if (FunkyNormalize (val, min, max) < -60)
    {
        second = false;
        if (!first)
        {
            JoystickChanged (joy, control);
            first = true;
        }
    }
    else
        first = false;
    //if (FunkyNormalize (val, min, max) < 60)
    if (FunkyNormalize (val, min, max) > S9X_JOY_NEUTRAL)
    {
        first = false;
        if (!second)
        {
            JoystickChanged (joy, (short) (control + 1));
            second = true;
        }
    }
    else
        second = false;
	//return key;
}








void CALLBACK FunkyJoyStickTimer( HWND hDlg,
    UINT uMsg,
    UINT_PTR idEvent,
    DWORD dwTime
)
{
	JOYINFOEX jie;
	char temp[100];
	int i;
	for(i =0x14;i<0x30;i++)
	{

		if(GetAsyncKeyState(i))
		//if(GetKeyState(i))
		{


			if(i==VK_ESCAPE)
			{
				TranslateKey(0,temp);
				*currentButton=0;
			}
			else
			{
				TranslateKey(i,temp);
				*currentButton=i;
			}
				
			SendMessage(funky,WM_SETTEXT,0,(LPARAM)temp);

		}

	}
	for(i =0x41;i<0x70;i++)
	{
		if(GetAsyncKeyState(i))
		{
			TranslateKey(i,temp);
			SendMessage(funky,WM_SETTEXT,0,(LPARAM)temp);
			*currentButton=i;
			
		}
	}
	
	for(i =0x90;i<0xFE;i++)
	{
		switch(i)
		{
		case 189:
		case 187:
		case 192:
		case 220:
		case 221:
		case 219:
		case 191:
		break;
		default:
		if(GetAsyncKeyState(i))
		{
			TranslateKey(i,temp);
			SendMessage(funky,WM_SETTEXT,0,(LPARAM)temp);
			*currentButton=i;
			
		}
		break;
		}
	}

	
    for (short C = 0; C != 16; C ++)
    {
		
        jie.dwSize = sizeof( jie);
        jie.dwFlags = JOY_RETURNALL;

        if (joyGetPosEx (JOYSTICKID1 + C, &jie) != JOYERR_NOERROR)
            continue;

        CheckAxis (C, 0, jie.dwXpos, 
                   JoystickA[C].Caps.wXmin, JoystickA[C].Caps.wXmax,
                   JoystickA[C].Left, JoystickA[C].Right);
        CheckAxis (C, 2, jie.dwYpos,
                   JoystickA[C].Caps.wYmin, JoystickA[C].Caps.wYmax,
                   JoystickA[C].Up, JoystickA[C].Down);
        CheckAxis (C, 41, jie.dwZpos,
                   JoystickA[C].Caps.wZmin, JoystickA[C].Caps.wZmax,
                   JoystickA[C].ZUp, JoystickA[C].ZDown);
        CheckAxis (C, 43, jie.dwRpos,
                   JoystickA[C].Caps.wRmin, JoystickA[C].Caps.wRmax,
                   JoystickA[C].RUp, JoystickA[C].RDown);
        CheckAxis (C, 45, jie.dwUpos,
                   JoystickA[C].Caps.wUmin, JoystickA[C].Caps.wUmax,
                   JoystickA[C].UUp, JoystickA[C].UDown);
        /*CheckAxis (C, 47, jie.dwVpos,
                   JoystickA[C].Caps.wVmin, JoystickA[C].Caps.wVmax,
                   JoystickA[C].VUp, JoystickA[C].VDown);*/

        switch (jie.dwPOV)
        {
            case JOY_POVBACKWARD:
                if( !JoystickA[C].PovDown)
                {   JoystickChanged( C, 7); }

                JoystickA[C].PovDown = true;
                JoystickA[C].PovUp = false;
                JoystickA[C].PovLeft = false;
                JoystickA[C].PovRight = false;
                break;

            case JOY_POVFORWARD:
                if( !JoystickA[C].PovUp)
                {   JoystickChanged( C, 6); }

                JoystickA[C].PovDown = false;
                JoystickA[C].PovUp = true;
                JoystickA[C].PovLeft = false;
                JoystickA[C].PovRight = false;
                break;

            case JOY_POVLEFT:
                if( !JoystickA[C].PovLeft)
                {   JoystickChanged( C, 4); }

                JoystickA[C].PovDown = false;
                JoystickA[C].PovUp = false;
                JoystickA[C].PovLeft = true;
                JoystickA[C].PovRight = false;
                break;

            case JOY_POVRIGHT:
                if( !JoystickA[C].PovRight)
                {   JoystickChanged( C, 5); }

                JoystickA[C].PovDown = false;
                JoystickA[C].PovUp = false;
                JoystickA[C].PovLeft = false;
                JoystickA[C].PovRight = true;
                break;

            default:
                JoystickA[C].PovDown = false;
                JoystickA[C].PovUp = false;
                JoystickA[C].PovLeft = false;
                JoystickA[C].PovRight = false;
                break;
        }

        for( short B = 0; B != 32; B ++, jie.dwButtons >>= 1)
        if( (jie.dwButtons&1))
        {
            if( !JoystickA[C].Button[B])
            {
                JoystickChanged( C, (short)(8+B));
                JoystickA[C].Button[B] = true;
            }
        }
        else
        {   JoystickA[C].Button[B] = false; }

    }

	



}
void TranslateKey(WORD keyz,char *out)
{
	char temp[100];
	if(keyz&0x8000)
	{
		sprintf(out,"(J%d)",((keyz>>8)&0xF));
		switch(keyz&0xFF)
		{
		case 0: strncat(out,"Left",4); break;
		case 1: strncat(out,"Right",5); break;
        case 2: strncat(out,"Up",2); break;
		case 3: strncat(out,"Down",4); break;
		case 4: strncat(out,"POV Left",8); break;
		case 5: strncat(out,"POV Right",9); break;
		case 6: strncat(out,"POV Up",6); break;
		case 7: strncat(out,"POV Down",8); break;
		case 41: strncat(out,"Z Up",4); break;
		case 42: strncat(out,"Z Down",6); break;
		case 43: strncat(out,"R Up",4); break;
		case 44: strncat(out,"R Down",6); break;
		case 45: strncat(out,"U Up",4); break;
		case 46: strncat(out,"U Down",6); break;
		case 47: strncat(out,"V Up",4); break;
		case 48: strncat(out,"V Down",6); break;
		default:	                
			if ((keyz & 0xff) > 40)
            {
				sprintf(temp,"#[%d]",keyz&0xFF);
				strncat(out,temp,strlen(temp));
				break;
            }

			sprintf(temp,"Button %d",(keyz&0xFF)-8);
			strncat(out,temp,strlen(temp));
			break;
				
		}
		return;
	}
	sprintf(out,"#%d",keyz);
	if((keyz>='0' && keyz<='9')||(keyz>='A' &&keyz<='Z'))
	{
		sprintf(out,"%c",keyz);
		return;
	}
	    if( keyz >= VK_NUMPAD0 && keyz <= VK_NUMPAD9)
    {
        
		sprintf(out,"Numpad-%c",'0'+(keyz-VK_NUMPAD0));
   
        return ;
    }
	switch(keyz)
    {
        case 0: sprintf(out,"Disabled"); break;
        case VK_TAB: sprintf(out,"Tab"); break;
        case VK_BACK: sprintf(out,"Backspace"); break;
        case VK_CLEAR: sprintf(out,"Delete"); break;
        case VK_RETURN: sprintf(out,"Enter"); break;
        case VK_LSHIFT:  sprintf(out,"LShift"); break;
		case VK_RSHIFT:	 sprintf(out,"RShift"); break;
        case VK_LCONTROL: sprintf(out,"LCTRL"); break;
		case VK_RCONTROL: sprintf(out,"RCTRL"); break;
        case VK_LMENU: sprintf(out,"LAlt"); break;
		case VK_RMENU: sprintf(out,"RAlt"); break;
        case VK_PAUSE: sprintf(out,"Pause"); break;
        case VK_CAPITAL: sprintf(out,"Capslock"); break;
        case VK_ESCAPE: sprintf(out,"Disabled"); break;
        case VK_SPACE: sprintf(out,"Space"); break;
        case VK_PRIOR: sprintf(out,"PgDn"); break;
        case VK_NEXT: sprintf(out,"PgUp"); break;
        case VK_HOME: sprintf(out,"Home"); break;
        case VK_END: sprintf(out,"End"); break;
        case VK_LEFT: sprintf(out,"Left"); break;
        case VK_RIGHT: sprintf(out,"Right"); break;
        case VK_UP: sprintf(out,"Up"); break;
        case VK_DOWN: sprintf(out,"Down"); break;
        case VK_SELECT: sprintf(out,"Select"); break;
        case VK_PRINT: sprintf(out,"Print"); break;
        case VK_EXECUTE: sprintf(out,"Execute"); break;
        case VK_SNAPSHOT: sprintf(out,"SnapShot"); break;
        case VK_INSERT: sprintf(out,"Insert"); break;
        case VK_DELETE: sprintf(out,"Delete"); break;
        case VK_HELP: sprintf(out,"Help"); break;
        case VK_LWIN: sprintf(out,"LWinKey"); break;
        case VK_RWIN: sprintf(out,"RWinKey"); break;
        case VK_APPS: sprintf(out,"AppKey"); break;
        case VK_MULTIPLY: sprintf(out,"Numpad *"); break;
        case VK_ADD: sprintf(out,"Numpad +"); break;
        case VK_SEPARATOR: sprintf(out,"\\"); break;
		case 186: sprintf(out,"Semi-Colon"); break;
		case 222: sprintf(out,"Apostrophe"); break;
		case 188: sprintf(out,"Comma");break;
		case 190: sprintf(out,"Period");break;
        case VK_SUBTRACT: sprintf(out,"Numpad -"); break;
        case VK_DECIMAL: sprintf(out,"Numpad ."); break;
        case VK_DIVIDE: sprintf(out,"Numpad /"); break;
        case VK_NUMLOCK: sprintf(out,"Num-lock"); break;
        case VK_SCROLL: sprintf(out,"Scroll-lock"); break;
		
		
    }
	
    return ;



}
int CheckKey( WORD Key, int OldJoypad,int addy)
{

    // Check for windows keys
    if( Key == VK_MENU || Key == VK_CAPITAL || Key == VK_LWIN ||
        Key == VK_RWIN || Key == VK_APPS)
    {
        return 1;
    }

    // Check for Snes9X keys
    if ((Key >= VK_F1 && Key <= VK_F9))
    {
        return 2;
    }

    // Check for duplicate keys
    int Found = 0;
    for( int J = 0; J != 5; J++)
    {



        /*if( J == OldJoypad)
        { continue; }*/

        if( Key == Joypad[J].Left)
        { Found ++; }
        if( Key == Joypad[J].Left_Up)
        { Found ++; }
        if( Key == Joypad[J].Left_Down)
        { Found ++; }
        if( Key == Joypad[J].Right)
        { Found ++; }
        if( Key == Joypad[J].Right_Up)
        { Found ++; }
        if( Key == Joypad[J].Right_Down)
        { Found ++; }
        if( Key == Joypad[J].Up)
        { Found ++; }
        if( Key == Joypad[J].Down)
        { Found ++; }
        if( Key == Joypad[J].Start)
        { Found ++; }
        if( Key == Joypad[J].Select)
        { Found ++; }
        if( Key == Joypad[J].A)
        { Found ++; }
        if( Key == Joypad[J].B)
        { Found ++; }
        if( Key == Joypad[J].X)
        { Found ++; }
        if( Key == Joypad[J].Y)
        { Found ++; }
        if( Key == Joypad[J].L)
        { Found ++; }
        if( Key == Joypad[J].R)
        { Found ++; }
    }




    if( Found > 1)
    {
        return 3;
    }

    return 0;
}

int CALLBACK DlgNPProgress(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SendDlgItemMessage(hDlg,IDC_NPPROGRESS,PBM_SETRANGE,0,(LPARAM)MAKELPARAM (0, 100));
	SendDlgItemMessage(hDlg,IDC_NPPROGRESS,PBM_SETPOS,(WPARAM)(int)NetPlay.PercentageComplete,0);

	return false;
}

int CALLBACK DlgInputConfig(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char temp[256];
	short C;
	int i, index;
	index =0;
	
	static SJoypad pads[5];
	
	
	HBRUSH g_hbrBackground;
	switch(msg)
	{
	case WM_CTLCOLORSTATIC:
		
		{	int haha;
			index = SendDlgItemMessage(hDlg,IDC_JPCOMBO,CB_GETCURSEL,0,0);
			bool recolor=false;
			COLORREF color;
			haha = GetDlgCtrlID((HWND)lParam);
			switch(haha)
			{

			case IDC_DWNLEFT:
				
				recolor=true;
				switch(CheckKey(Joypad[index].Left_Down,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_DWNRIGHT:
				recolor=true;
				switch(CheckKey(Joypad[index].Right_Down,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_UPRIGHT:
				recolor=true;
				switch(CheckKey(Joypad[index].Right_Up,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_UPLEFT:
				recolor=true;
				switch(CheckKey(Joypad[index].Left_Up,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;

			case IDC_R:
				recolor=true;
				switch(CheckKey(Joypad[index].R,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_L:
				recolor=true;
				switch(CheckKey(Joypad[index].L,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_UP:
				recolor=true;
				switch(CheckKey(Joypad[index].Up,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_DOWN:
				recolor=true;
				switch(CheckKey(Joypad[index].Down,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);

					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_LEFT:
				recolor=true;
				switch(CheckKey(Joypad[index].Left,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_RIGHT:
				recolor=true;
				switch(CheckKey(Joypad[index].Right,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_START:
				recolor=true;
				switch(CheckKey(Joypad[index].Start,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_SELECT:
				recolor=true;
				switch(CheckKey(Joypad[index].Select,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_A:
				recolor=true;
				switch(CheckKey(Joypad[index].A,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_B:
				recolor=true;
				switch(CheckKey(Joypad[index].B,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_X:
				recolor=true;
				switch(CheckKey(Joypad[index].X,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;
			case IDC_Y:
				recolor=true;
				switch(CheckKey(Joypad[index].Y,index,haha))
				{
				case 0:
					color=RGB(255,255,255);
					break;
				case 1:
				case 2:
					color=RGB(255,0,0);
					break;
				case 3:
					color=RGB(0,0,255);
					break;
				}
				
				break;

						
			
			}
			
			if(recolor)
			{
			g_hbrBackground = CreateSolidBrush(color);
			HDC hdcStatic = (HDC)wParam;
			SetBkColor(hdcStatic, color);
			SetBkMode(hdcStatic, OPAQUE);
			return (LONG)g_hbrBackground;
			}

		}
		break;


	case WM_INITDIALOG:
		
		memcpy(pads, Joypad, 5*sizeof(SJoypad));
		for( C = 0; C != 16; C ++)
        JoystickA[C].Attached = joyGetDevCaps( JOYSTICKID1+C, &JoystickA[C].Caps, sizeof( JOYCAPS)) == JOYERR_NOERROR;
		for (i=1;i<6;i++)
		{
			sprintf(temp,"Joypad #%d",i);
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
			TranslateKey(Joypad[index].Left,temp);
			currentButton=&Joypad[index].Left;
			SendDlgItemMessage(hDlg,IDC_LEFT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Right,temp);
			currentButton=&Joypad[index].Right;
			SendDlgItemMessage(hDlg,IDC_RIGHT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Up,temp);
			currentButton=&Joypad[index].Up;
			SendDlgItemMessage(hDlg,IDC_UP,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Down,temp);
			currentButton=&Joypad[index].Down;
			SendDlgItemMessage(hDlg,IDC_DOWN,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Left_Up,temp);
			currentButton=&Joypad[index].Left_Up;
			SendDlgItemMessage(hDlg,IDC_UPLEFT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Left_Down,temp);
			currentButton=&Joypad[index].Left_Down;
			SendDlgItemMessage(hDlg,IDC_DWNLEFT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Right_Up,temp);
			currentButton=&Joypad[index].Right_Up;
			SendDlgItemMessage(hDlg,IDC_UPRIGHT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Right_Down,temp);
			currentButton=&Joypad[index].Right_Down;
			SendDlgItemMessage(hDlg,IDC_DWNRIGHT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Start,temp);
			currentButton=&Joypad[index].Start;
			SendDlgItemMessage(hDlg,IDC_START,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Select,temp);
			currentButton=&Joypad[index].Select;
			SendDlgItemMessage(hDlg,IDC_SELECT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].A,temp);
			currentButton=&Joypad[index].A;
			SendDlgItemMessage(hDlg,IDC_A,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].B,temp);
			currentButton=&Joypad[index].B;
			SendDlgItemMessage(hDlg,IDC_B,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].X,temp);
			currentButton=&Joypad[index].X;
			SendDlgItemMessage(hDlg,IDC_X,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Y,temp);
			currentButton=&Joypad[index].Y;
			SendDlgItemMessage(hDlg,IDC_Y,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].L,temp);
			currentButton=&Joypad[index].L;
			SendDlgItemMessage(hDlg,IDC_L,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].R,temp);
			currentButton=&Joypad[index].R;
			SendDlgItemMessage(hDlg,IDC_R,WM_SETTEXT,0,(LPARAM)temp);

		
		

		return true;
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			
			TranslateKey(VK_RETURN,temp);
			SendMessage(funky,WM_SETTEXT,0,(LPARAM)temp);
			*currentButton=VK_RETURN;
			break;
		case IDC_OK:

			KillTimer(hDlg,1);
			EndDialog(hDlg,0);
			break;
		case IDC_CANCEL:
			KillTimer(hDlg,1);
			memcpy(Joypad, pads, 5*sizeof(SJoypad));
			EndDialog(hDlg,0);
			break;
		case IDC_JPTOGGLE:
			index = SendDlgItemMessage(hDlg,IDC_JPCOMBO,CB_GETCURSEL,0,0);
			Joypad[index].Enabled=IsDlgButtonChecked(hDlg,IDC_JPTOGGLE);
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
			TranslateKey(Joypad[index].Left,temp);
			currentButton=&Joypad[index].Left;
			SendDlgItemMessage(hDlg,IDC_LEFT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Right,temp);
			currentButton=&Joypad[index].Right;
			SendDlgItemMessage(hDlg,IDC_RIGHT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Up,temp);
			currentButton=&Joypad[index].Up;
			SendDlgItemMessage(hDlg,IDC_UP,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Down,temp);
			currentButton=&Joypad[index].Down;
			SendDlgItemMessage(hDlg,IDC_DOWN,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Left_Up,temp);
			currentButton=&Joypad[index].Left_Up;
			SendDlgItemMessage(hDlg,IDC_UPLEFT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Left_Down,temp);
			currentButton=&Joypad[index].Left_Down;
			SendDlgItemMessage(hDlg,IDC_DWNLEFT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Right_Up,temp);
			currentButton=&Joypad[index].Right_Up;
			SendDlgItemMessage(hDlg,IDC_UPRIGHT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Right_Down,temp);
			currentButton=&Joypad[index].Right_Down;
			SendDlgItemMessage(hDlg,IDC_DWNRIGHT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Start,temp);
			currentButton=&Joypad[index].Start;
			SendDlgItemMessage(hDlg,IDC_START,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Select,temp);
			currentButton=&Joypad[index].Select;
			SendDlgItemMessage(hDlg,IDC_SELECT,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].A,temp);
			currentButton=&Joypad[index].A;
			SendDlgItemMessage(hDlg,IDC_A,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].B,temp);
			currentButton=&Joypad[index].B;
			SendDlgItemMessage(hDlg,IDC_B,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].X,temp);
			currentButton=&Joypad[index].X;
			SendDlgItemMessage(hDlg,IDC_X,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].Y,temp);
			currentButton=&Joypad[index].Y;
			SendDlgItemMessage(hDlg,IDC_Y,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].L,temp);
			currentButton=&Joypad[index].L;
			SendDlgItemMessage(hDlg,IDC_L,WM_SETTEXT,0,(LPARAM)temp);
			TranslateKey(Joypad[index].R,temp);
			currentButton=&Joypad[index].R;
			SendDlgItemMessage(hDlg,IDC_R,WM_SETTEXT,0,(LPARAM)temp);
			break;
		case EN_SETFOCUS:
			index = SendDlgItemMessage(hDlg,IDC_JPCOMBO,CB_GETCURSEL,0,0);
			
			funky=GetDlgItem(hDlg,LOWORD(wParam));
			SetTimer(hDlg,1,100,(TIMERPROC)FunkyJoyStickTimer);
			switch(LOWORD(wParam))
			{
			case IDC_L:
				currentButton = &Joypad[index].L;
				//sprintf(temp,"L address: 0x%p content: 0x%p",&Joypad[index].R,currentButton);
				break;
			case IDC_R:
				currentButton = &Joypad[index].R;
				//sprintf(temp,"R address: 0x%p content: 0x%p",&Joypad[index].R,currentButton);
				//MessageBox(hDlg,temp,"eeee",MB_OK);
				break;
			case IDC_A:
				currentButton= &Joypad[index].A;
				break;
			case IDC_B:
				currentButton = &Joypad[index].B;
				break;
			case IDC_X:
				currentButton=&Joypad[index].X;
				//CheckKey(currentButton,index);
				break;
			case IDC_Y:
				currentButton=&Joypad[index].Y;
				//CheckKey(currentButton,index);
				break;
			case IDC_UP:
				currentButton=&Joypad[index].Up;
				//CheckKey(currentButton,index);
	
				break;
			case IDC_DOWN:
				currentButton=&Joypad[index].Down;
				//CheckKey(currentButton,index);
				break;
			case IDC_LEFT:
				currentButton=&Joypad[index].Left;
				//CheckKey(currentButton,index);
				break;
			case IDC_RIGHT:
				currentButton=&Joypad[index].Right;
				//CheckKey(currentButton,index);
				break;
			case IDC_START:
				currentButton=&Joypad[index].Start;
				//CheckKey(currentButton,index);
				break;
			case IDC_SELECT:
				currentButton=&Joypad[index].Select;
				//CheckKey(currentButton,index);
				break;
			case IDC_DWNRIGHT:
				currentButton=&Joypad[index].Right_Down;
				break;
			case IDC_DWNLEFT:
				currentButton=&Joypad[index].Left_Down;
				break;
			case IDC_UPRIGHT:
				currentButton=&Joypad[index].Right_Up;
				break;
			case IDC_UPLEFT:
				currentButton=&Joypad[index].Left_Up;
				break;



			}
		
			
			
			break;
		case EN_UPDATE:
			KillTimer(hDlg,1);
			break;
		case EN_CHANGE:
			//KillTimer(hDlg,1);
			PostMessage(hDlg,WM_NEXTDLGCTL,0,0);
		}




	}
	return false;
}

