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
/* Original native Snes9x 1.41-1 PPU resets and their palette/controller helpers.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Entries: 0x001591a8, 0x0015c124, 0x0015c624, 0x0015cae0, 0x0015cdf8.
 * Original native field layout, byte-count ABI and PS2 BGR555 palette retained.
 */
typedef unsigned char uint8;
typedef unsigned char bool8;
typedef signed char int8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef int int32;
#define TRUE 1
#define FALSE 0
#define NULL 0
#define ZeroMemory(p,n) memset((p),0,(n))
#define BUILD_PIXEL(R,G,B) (((int)(B)<<10)|((int)(G)<<5)|(int)(R))
#define CLIP_OR 0
#define TILE_2BIT 0
#define TILE_4BIT 1
#define TILE_8BIT 2
#define MAX_2BIT_TILES 4096
#define MAX_4BIT_TILES 2048
#define MAX_8BIT_TILES 1024
#define SNES_WIDTH 256
#define SNES_HEIGHT 224
enum { SNES_MULTIPLAYER5, SNES_JOYPAD, SNES_MOUSE_SWAPPED, SNES_MOUSE,
       SNES_SUPERSCOPE, SNES_JUSTIFIER, SNES_JUSTIFIER_2, SNES_MAX_CONTROLLER_OPTIONS };
struct SSettings{
    /* CPU options */
    bool8  APUEnabled;
    bool8  Shutdown;
    uint8  SoundSkipMethod;
    long   H_Max;
    long   HBlankStart;
    long   CyclesPercentage;
    bool8  DisableIRQ;
    bool8  Paused;
    bool8  ForcedPause;
    bool8  StopEmulation;

    /* Tracing options */
    bool8  TraceDMA;
    bool8  TraceHDMA;
    bool8  TraceVRAM;
    bool8  TraceUnknownRegisters;
    bool8  TraceDSP;

    /* Joystick options */
    bool8  SwapJoypads;
    bool8  JoystickEnabled;

    /* ROM timing options (see also H_Max above) */
    bool8  ForcePAL;
    bool8  ForceNTSC;
    bool8  PAL;
    uint32 FrameTimePAL;
    uint32 FrameTimeNTSC;
    uint32 FrameTime;
    uint32 SkipFrames;

    /* ROM image options */
    bool8  ForceLoROM;
    bool8  ForceHiROM;
    bool8  ForceHeader;
    bool8  ForceNoHeader;
    bool8  ForceInterleaved;
    bool8  ForceInterleaved2;
    bool8  ForceNotInterleaved;

    /* Peripherial options */
    bool8  ForceSuperFX;
    bool8  ForceNoSuperFX;
    bool8  ForceDSP1;
    bool8  ForceNoDSP1;
    bool8  ForceSA1;
    bool8  ForceNoSA1;
    bool8  ForceC4;
    bool8  ForceNoC4;
    bool8  ForceSDD1;
    bool8  ForceNoSDD1;
    bool8  MultiPlayer5;
    bool8  Mouse;
    bool8  SuperScope;
    bool8  SRTC;
    uint32 ControllerOption;
    
    bool8  ShutdownMaster;
    bool8  MultiPlayer5Master;
    bool8  SuperScopeMaster;
    bool8  MouseMaster;
    bool8  SuperFX;
    bool8  DSP1Master;
    bool8  SA1;
    bool8  C4;
    bool8  SDD1;
	bool8  SPC7110;
	bool8  SPC7110RTC;
	bool8  OBC1;
  /*   uint8  DSPVersion; //left comented out because it may be useful later.*/
    /* Sound options */
    uint32 SoundPlaybackRate;
    bool8  TraceSoundDSP;
    bool8  Stereo;
    bool8  ReverseStereo;
    bool8  SixteenBitSound;
    int    SoundBufferSize;
    int    SoundMixInterval;
    bool8  SoundEnvelopeHeightReading;
    bool8  DisableSoundEcho;
    bool8  DisableSampleCaching;
    bool8  DisableMasterVolume;
    bool8  SoundSync;
    bool8  InterpolatedSound;
    bool8  ThreadSound;
    bool8  Mute;
    bool8  NextAPUEnabled;
    uint8  AltSampleDecode;
    bool8  FixFrequency;
    
    /* Graphics options */
    bool8  SixteenBit;
    bool8  Transparency;
    bool8  SupportHiRes;
    bool8  Mode7Interpolate;

    /* SNES graphics options */
    bool8  BGLayering;
    bool8  DisableGraphicWindows;
    bool8  ForceTransparency;
    bool8  ForceNoTransparency;
    bool8  DisableHDMA;
    bool8  DisplayFrameRate;
    bool8  DisableRangeTimeOver;

    /* Others */
    bool8  NetPlay;
    bool8  NetPlayServer;
    char   ServerName [128];
    int    Port;
    bool8  GlideEnable;
    bool8  OpenGLEnable;
    int32  AutoSaveDelay; /* Time in seconds before S-RAM auto-saved if modified. */
    bool8  ApplyCheats;
    bool8  TurboMode;
    uint32 TurboSkipFrames;
    uint32 AutoMaxSkipFrames;
    
/* Fixes for individual games */
  /*   uint32 StrikeGunnerOffsetHack; */
    bool8  PS2PortLayoutByte; /* Native PS2 Settings byte before game-specific options. */
    bool8  ChuckRock;
    bool8  StarfoxHack;
    bool8  WinterGold;
    bool8  Dezaemon;
  /*   bool8  WrestlemaniaArcade; */
    bool8  BS;	/* Japanese Satellite System games. */
    bool8  DaffyDuck;
    uint8  APURAMInitialValue;
    bool8  SampleCatchup;
	bool8  JustifierMaster;
	bool8  Justifier;
	bool8  SecondJustifier;
	int8   SETA;
    bool8  TakeScreenshot;
    int8   StretchScreenshots;
	uint16 DisplayColor;
    int    SoundDriver;
    int    AIDOShmId;
};


struct ClipData {
    uint32  Count [6];
    uint32  Left [6][6];
    uint32  Right [6][6];
};


struct SOBJ
{
    short  HPos;
    uint16 VPos;
    uint16 Name;
    uint8  VFlip;
    uint8  HFlip;
    uint8  Priority;
    uint8  Palette;
    uint8  Size;
};


struct SPPU {
    uint8  BGMode;
    uint8  BG3Priority;
    uint8  Brightness;

    struct {
	bool8 High;
	uint8 Increment;
	uint16 Address;
	uint16 Mask1;
	uint16 FullGraphicCount;
	uint16 Shift;
    } VMA;

    struct {
	uint16 SCBase;
	uint16 VOffset;
	uint16 HOffset;
	uint8 BGSize;
	uint16 NameBase;
	uint16 SCSize;
    } BG [4];

    bool8  CGFLIP;
    uint16 CGDATA [256]; 
    uint8  FirstSprite;
    uint8  LastSprite;
    struct SOBJ OBJ [128];
    uint8  OAMPriorityRotation;
    uint16 OAMAddr;
    uint8  RangeTimeOver;

    uint8  OAMFlip;
    uint16 OAMTileAddress;
    uint16 IRQVBeamPos;
    uint16 IRQHBeamPos;
    uint16 VBeamPosLatched;
    uint16 HBeamPosLatched;

    uint8  HBeamFlip;
    uint8  VBeamFlip;
    uint8  HVBeamCounterLatched;

    short  MatrixA;
    short  MatrixB;
    short  MatrixC;
    short  MatrixD;
    short  CentreX;
    short  CentreY;
    uint8  Joypad1ButtonReadPos;
    uint8  Joypad2ButtonReadPos;

    uint8  CGADD;
    uint8  FixedColourRed;
    uint8  FixedColourGreen;
    uint8  FixedColourBlue;
    uint16 SavedOAMAddr;
    uint16 ScreenHeight;
    uint32 WRAM;
    uint8  BG_Forced;
    bool8  ForcedBlanking;
    bool8  OBJThroughMain;
    bool8  OBJThroughSub;
    uint8  OBJSizeSelect;
    uint16 OBJNameBase;
    bool8  OBJAddition;
    uint8  OAMReadFlip;
    uint8  OAMData [512 + 32];
    bool8  VTimerEnabled;
    bool8  HTimerEnabled;
    short  HTimerPosition;
    uint8  Mosaic;
    bool8  BGMosaic [4];
    bool8  Mode7HFlip;
    bool8  Mode7VFlip;
    uint8  Mode7Repeat;
    uint8  Window1Left;
    uint8  Window1Right;
    uint8  Window2Left;
    uint8  Window2Right;
    uint8  ClipCounts [6];
    uint8  ClipWindowOverlapLogic [6];
    uint8  ClipWindow1Enable [6];
    uint8  ClipWindow2Enable [6];
    bool8  ClipWindow1Inside [6];
    bool8  ClipWindow2Inside [6];
    bool8  RecomputeClipWindows;
    uint8  CGFLIPRead;
    uint16 OBJNameSelect;
    bool8  Need16x8Mulitply;
    uint8  Joypad3ButtonReadPos;
    uint8  MouseSpeed[2];

    // XXX Do these need to be added to snapshot.cpp?
    uint16 SavedOAMAddr2;
    uint16 OAMWriteRegister;
    uint8 BGnxOFSbyte;
	uint8 OpenBus;
};


struct InternalPPU {
    bool8  ColorsChanged;
    uint8  HDMA;
    bool8  HDMAStarted;
    uint8  MaxBrightness;
    bool8  LatchedBlanking;
    bool8  OBJChanged;
    bool8  RenderThisFrame;
    bool8  DirectColourMapsNeedRebuild;
    uint32 FrameCount;
    uint32 RenderedFramesCount;
    uint32 DisplayedRenderedFrameCount;
    uint32 SkippedFrames;
    uint32 FrameSkip;
    uint8  *TileCache [3];
    uint8  *TileCached [3];
    bool8  FirstVRAMRead;
    bool8  DoubleHeightPixels;
    bool8  Interlace;
    bool8  InterlaceSprites;
    bool8  DoubleWidthPixels;
    int    RenderedScreenHeight;
    int    RenderedScreenWidth;
    uint32 Red [256];
    uint32 Green [256];
    uint32 Blue [256];
    uint8  *XB;
    uint16 ScreenColors [256];
    int	   PreviousLine;
    int	   CurrentLine;
    int	   Controller;
    uint32 Joypads[5];
    uint32 SuperScope;
    uint32 Mouse[2];
    int    PrevMouseX[2];
    int    PrevMouseY[2];
    struct ClipData Clip [2];
};

typedef char VerifyNativeSettingsSize[(sizeof(SSettings)==328)?1:-1];
typedef char VerifyNativePPUSize[(sizeof(SPPU)==2780)?1:-1];
typedef char VerifyNativeInternalPPUSize[(sizeof(InternalPPU)==4340)?1:-1];
class CMemory { public:
    uint8 *RAM,*ROM,*VRAM,*SRAM,*BWRAM,*FillRAM,*C4RAM;
};
extern SSettings Settings __asm__("DAT_003454e0");
extern SPPU PPU __asm__("DAT_0035b788");
extern InternalPPU IPPU __asm__("DAT_0035c268");
extern CMemory Memory __asm__("DAT_0034e2b0");
extern uint8 mul_brightness[16][32] __asm__("DAT_0033cac8");
/* The original mouse-position callee is the proved 8-byte zero-return leaf;
 * it ignores all four incoming arguments and writes no output references. */
extern "C" bool8 S9xReadMousePosition(int,int &,int &,uint32 &);
extern "C" {
void *memset(void *,int,uint32);
void S9xResetPPU();
void S9xSoftResetPPU();
void S9xFixColourBrightness();
void S9xNextController();
void S9xProcessMouse(int);
}
void S9xFixColourBrightness ()
{
    IPPU.XB = mul_brightness [PPU.Brightness];
    if (Settings.SixteenBit)
	{
		for (int i = 0; i < 256; i++)
		{
			IPPU.Red [i] = IPPU.XB [PPU.CGDATA [i] & 0x1f];
			IPPU.Green [i] = IPPU.XB [(PPU.CGDATA [i] >> 5) & 0x1f];
			IPPU.Blue [i] = IPPU.XB [(PPU.CGDATA [i] >> 10) & 0x1f];
			IPPU.ScreenColors [i] = BUILD_PIXEL (IPPU.Red [i], IPPU.Green [i],
												 IPPU.Blue [i]);
		}
	}
}

/******************************************************************************/
/* S9xSetPPU()                                                                */
/* This function sets a PPU Register to a specific byte                       */
/******************************************************************************/
void S9xResetPPU ()
{
	PPU.BGMode = 0;
	PPU.BG3Priority = 0;
	PPU.Brightness = 0;
	PPU.VMA.High = 0;
	PPU.VMA.Increment = 1;
	PPU.VMA.Address = 0;
	PPU.VMA.FullGraphicCount = 0;
	PPU.VMA.Shift = 0;

	for (uint8 B = 0; B != 4; B++)
	{
		PPU.BG[B].SCBase = 0;
		PPU.BG[B].VOffset = 0;
		PPU.BG[B].HOffset = 0;
		PPU.BG[B].BGSize = 0;
		PPU.BG[B].NameBase = 0;
		PPU.BG[B].SCSize = 0;

		PPU.ClipCounts[B] = 0;
		PPU.ClipWindowOverlapLogic [B] = CLIP_OR;
		PPU.ClipWindow1Enable[B] = FALSE;
		PPU.ClipWindow2Enable[B] = FALSE;
		PPU.ClipWindow1Inside[B] = TRUE;
		PPU.ClipWindow2Inside[B] = TRUE;
	}

	PPU.ClipCounts[4] = 0;
	PPU.ClipCounts[5] = 0;
	PPU.ClipWindowOverlapLogic[4] = PPU.ClipWindowOverlapLogic[5] = CLIP_OR;
	PPU.ClipWindow1Enable[4] = PPU.ClipWindow1Enable[5] = FALSE;
	PPU.ClipWindow2Enable[4] = PPU.ClipWindow2Enable[5] = FALSE;
	PPU.ClipWindow1Inside[4] = PPU.ClipWindow1Inside[5] = TRUE;
	PPU.ClipWindow2Inside[4] = PPU.ClipWindow2Inside[5] = TRUE;

	PPU.CGFLIP = 0;
	int c;
	for (c = 0; c < 256; c++)
	{
		IPPU.Red [c] = (c & 7) << 2;
		IPPU.Green [c] = ((c >> 3) & 7) << 2;
		IPPU.Blue [c] = ((c >> 6) & 2) << 3;
		PPU.CGDATA [c] = IPPU.Red [c] | (IPPU.Green [c] << 5) |
			(IPPU.Blue [c] << 10);
	}

	PPU.FirstSprite = 0;
	PPU.LastSprite = 127;
	for (int Sprite = 0; Sprite < 128; Sprite++)
	{
		PPU.OBJ[Sprite].HPos = 0;
		PPU.OBJ[Sprite].VPos = 0;
		PPU.OBJ[Sprite].VFlip = 0;
		PPU.OBJ[Sprite].HFlip = 0;
		PPU.OBJ[Sprite].Priority = 0;
		PPU.OBJ[Sprite].Palette = 0;
		PPU.OBJ[Sprite].Name = 0;
		PPU.OBJ[Sprite].Size = 0;
	}
	PPU.OAMPriorityRotation = 0;
	PPU.OAMWriteRegister = 0;
	PPU.RangeTimeOver = 0;
	PPU.OpenBus = 0;

	PPU.OAMFlip = 0;
	PPU.OAMTileAddress = 0;
	PPU.OAMAddr = 0;
	PPU.IRQVBeamPos = 0;
	PPU.IRQHBeamPos = 0;
	PPU.VBeamPosLatched = 0;
	PPU.HBeamPosLatched = 0;

	PPU.HBeamFlip = 0;
	PPU.VBeamFlip = 0;
	PPU.HVBeamCounterLatched = 0;

	PPU.MatrixA = PPU.MatrixB = PPU.MatrixC = PPU.MatrixD = 0;
	PPU.CentreX = PPU.CentreY = 0;
	PPU.Joypad1ButtonReadPos = 0;
	PPU.Joypad2ButtonReadPos = 0;
	PPU.Joypad3ButtonReadPos = 0;

	PPU.CGADD = 0;
	PPU.FixedColourRed = PPU.FixedColourGreen = PPU.FixedColourBlue = 0;
	PPU.SavedOAMAddr = 0;
	PPU.SavedOAMAddr2 = 0;
	PPU.ScreenHeight = SNES_HEIGHT;
	PPU.WRAM = 0;
	PPU.BG_Forced = 0;
	PPU.ForcedBlanking = TRUE;
	PPU.OBJThroughMain = FALSE;
	PPU.OBJThroughSub = FALSE;
	PPU.OBJSizeSelect = 0;
	PPU.OBJNameSelect = 0;
	PPU.OBJNameBase = 0;
	PPU.OBJAddition = FALSE;
	PPU.OAMReadFlip = 0;
	PPU.BGnxOFSbyte = 0;
	ZeroMemory (PPU.OAMData, 512 + 32);

	PPU.VTimerEnabled = FALSE;
	PPU.HTimerEnabled = FALSE;
	PPU.HTimerPosition = Settings.H_Max + 1;
	PPU.Mosaic = 0;
	PPU.BGMosaic [0] = PPU.BGMosaic [1] = FALSE;
	PPU.BGMosaic [2] = PPU.BGMosaic [3] = FALSE;
	PPU.Mode7HFlip = FALSE;
	PPU.Mode7VFlip = FALSE;
	PPU.Mode7Repeat = 0;
	PPU.Window1Left = 1;
	PPU.Window1Right = 0;
	PPU.Window2Left = 1;
	PPU.Window2Right = 0;
	PPU.RecomputeClipWindows = TRUE;
	PPU.CGFLIPRead = 0;
	PPU.Need16x8Mulitply = FALSE;
	PPU.MouseSpeed[0] = PPU.MouseSpeed[1] = 0;

	IPPU.ColorsChanged = TRUE;
	IPPU.HDMA = 0;
	IPPU.HDMAStarted = FALSE;
	IPPU.MaxBrightness = 0;
	IPPU.LatchedBlanking = 0;
	IPPU.OBJChanged = TRUE;
	IPPU.RenderThisFrame = TRUE;
	IPPU.DirectColourMapsNeedRebuild = TRUE;
	IPPU.FrameCount = 0;
	IPPU.RenderedFramesCount = 0;
	IPPU.DisplayedRenderedFrameCount = 0;
	IPPU.SkippedFrames = 0;
	IPPU.FrameSkip = 0;
	ZeroMemory (IPPU.TileCached [TILE_2BIT], MAX_2BIT_TILES);
	ZeroMemory (IPPU.TileCached [TILE_4BIT], MAX_4BIT_TILES);
	ZeroMemory (IPPU.TileCached [TILE_8BIT], MAX_8BIT_TILES);
	IPPU.FirstVRAMRead = FALSE;
	IPPU.Interlace = FALSE;
	IPPU.InterlaceSprites = FALSE;
	IPPU.DoubleWidthPixels = FALSE;
	IPPU.DoubleHeightPixels = FALSE;
	IPPU.RenderedScreenWidth = SNES_WIDTH;
	IPPU.RenderedScreenHeight = SNES_HEIGHT;
	IPPU.XB = NULL;
	for (c = 0; c < 256; c++)
		IPPU.ScreenColors [c] = c;
	S9xFixColourBrightness ();
	IPPU.PreviousLine = IPPU.CurrentLine = 0;
	IPPU.Joypads[0] = IPPU.Joypads[1] = IPPU.Joypads[2] = 0;
	IPPU.Joypads[3] = IPPU.Joypads[4] = 0;
	IPPU.SuperScope = 0;
	IPPU.Mouse[0] = IPPU.Mouse[1] = 0;
	IPPU.PrevMouseX[0] = IPPU.PrevMouseX[1] = 256 / 2;
	IPPU.PrevMouseY[0] = IPPU.PrevMouseY[1] = 224 / 2;

	if (Settings.ControllerOption == 0)
		IPPU.Controller = SNES_MAX_CONTROLLER_OPTIONS - 1;
	else
		IPPU.Controller = Settings.ControllerOption - 1;
	S9xNextController ();

	for (c = 0; c < 2; c++)
		memset (&IPPU.Clip [c], 0, sizeof (struct ClipData));

	if (Settings.MouseMaster)
	{
		S9xProcessMouse (0);
		S9xProcessMouse (1);
	}
	for (c = 0; c < 0x8000; c += 0x100)
		memset (&Memory.FillRAM [c], c >> 8, 0x100);

	ZeroMemory (&Memory.FillRAM [0x2100], 0x100);
	ZeroMemory (&Memory.FillRAM [0x4200], 0x100);
	ZeroMemory (&Memory.FillRAM [0x4000], 0x100);
	// For BS Suttehakkun 2...
	ZeroMemory (&Memory.FillRAM [0x1000], 0x1000);

	Memory.FillRAM[0x4201]=Memory.FillRAM[0x4213]=0xFF;
}

void S9xSoftResetPPU ()
{
	PPU.BGMode = 0;
	PPU.BG3Priority = 0;
	PPU.Brightness = 0;
	PPU.VMA.High = 0;
	PPU.VMA.Increment = 1;
	PPU.VMA.Address = 0;
	PPU.VMA.FullGraphicCount = 0;
	PPU.VMA.Shift = 0;

	for (uint8 B = 0; B != 4; B++)
	{
		PPU.BG[B].SCBase = 0;
		PPU.BG[B].VOffset = 0;
		PPU.BG[B].HOffset = 0;
		PPU.BG[B].BGSize = 0;
		PPU.BG[B].NameBase = 0;
		PPU.BG[B].SCSize = 0;

		PPU.ClipCounts[B] = 0;
		PPU.ClipWindowOverlapLogic [B] = CLIP_OR;
		PPU.ClipWindow1Enable[B] = FALSE;
		PPU.ClipWindow2Enable[B] = FALSE;
		PPU.ClipWindow1Inside[B] = TRUE;
		PPU.ClipWindow2Inside[B] = TRUE;
	}

	PPU.ClipCounts[4] = 0;
	PPU.ClipCounts[5] = 0;
	PPU.ClipWindowOverlapLogic[4] = PPU.ClipWindowOverlapLogic[5] = CLIP_OR;
	PPU.ClipWindow1Enable[4] = PPU.ClipWindow1Enable[5] = FALSE;
	PPU.ClipWindow2Enable[4] = PPU.ClipWindow2Enable[5] = FALSE;
	PPU.ClipWindow1Inside[4] = PPU.ClipWindow1Inside[5] = TRUE;
	PPU.ClipWindow2Inside[4] = PPU.ClipWindow2Inside[5] = TRUE;

	PPU.CGFLIP = 0;
	int c;
	for (c = 0; c < 256; c++)
	{
		IPPU.Red [c] = (c & 7) << 2;
		IPPU.Green [c] = ((c >> 3) & 7) << 2;
		IPPU.Blue [c] = ((c >> 6) & 2) << 3;
		PPU.CGDATA [c] = IPPU.Red [c] | (IPPU.Green [c] << 5) |
			(IPPU.Blue [c] << 10);
	}

	PPU.FirstSprite = 0;
	PPU.LastSprite = 127;
	for (int Sprite = 0; Sprite < 128; Sprite++)
	{
		PPU.OBJ[Sprite].HPos = 0;
		PPU.OBJ[Sprite].VPos = 0;
		PPU.OBJ[Sprite].VFlip = 0;
		PPU.OBJ[Sprite].HFlip = 0;
		PPU.OBJ[Sprite].Priority = 0;
		PPU.OBJ[Sprite].Palette = 0;
		PPU.OBJ[Sprite].Name = 0;
		PPU.OBJ[Sprite].Size = 0;
	}
	PPU.OAMPriorityRotation = 0;
	PPU.OAMWriteRegister = 0;
	PPU.RangeTimeOver = 0;
	PPU.OpenBus = 0;

	PPU.OAMFlip = 0;
	PPU.OAMTileAddress = 0;
	PPU.OAMAddr = 0;
	PPU.IRQVBeamPos = 0;
	PPU.IRQHBeamPos = 0;
	PPU.VBeamPosLatched = 0;
	PPU.HBeamPosLatched = 0;

	PPU.HBeamFlip = 0;
	PPU.VBeamFlip = 0;
	PPU.HVBeamCounterLatched = 0;

	PPU.MatrixA = PPU.MatrixB = PPU.MatrixC = PPU.MatrixD = 0;
	PPU.CentreX = PPU.CentreY = 0;
//	PPU.Joypad1ButtonReadPos = 0;
//	PPU.Joypad2ButtonReadPos = 0;
//	PPU.Joypad3ButtonReadPos = 0;
	PPU.CGADD = 0;
	PPU.FixedColourRed = PPU.FixedColourGreen = PPU.FixedColourBlue = 0;
	PPU.SavedOAMAddr = 0;
	PPU.SavedOAMAddr2 = 0;
	PPU.ScreenHeight = SNES_HEIGHT;
	PPU.WRAM = 0;
	PPU.BG_Forced = 0;
	PPU.ForcedBlanking = TRUE;
	PPU.OBJThroughMain = FALSE;
	PPU.OBJThroughSub = FALSE;
	PPU.OBJSizeSelect = 0;
	PPU.OBJNameSelect = 0;
	PPU.OBJNameBase = 0;
	PPU.OBJAddition = FALSE;
	PPU.OAMReadFlip = 0;
	PPU.BGnxOFSbyte = 0;
	ZeroMemory (PPU.OAMData, 512 + 32);

	PPU.VTimerEnabled = FALSE;
	PPU.HTimerEnabled = FALSE;
	PPU.HTimerPosition = Settings.H_Max + 1;
	PPU.Mosaic = 0;
	PPU.BGMosaic [0] = PPU.BGMosaic [1] = FALSE;
	PPU.BGMosaic [2] = PPU.BGMosaic [3] = FALSE;
	PPU.Mode7HFlip = FALSE;
	PPU.Mode7VFlip = FALSE;
	PPU.Mode7Repeat = 0;
	PPU.Window1Left = 1;
	PPU.Window1Right = 0;
	PPU.Window2Left = 1;
	PPU.Window2Right = 0;
	PPU.RecomputeClipWindows = TRUE;
	PPU.CGFLIPRead = 0;
	PPU.Need16x8Mulitply = FALSE;
	PPU.MouseSpeed[0] = PPU.MouseSpeed[1] = 0;

	IPPU.ColorsChanged = TRUE;
	IPPU.HDMA = 0;
	IPPU.HDMAStarted = FALSE;
	IPPU.MaxBrightness = 0;
	IPPU.LatchedBlanking = 0;
	IPPU.OBJChanged = TRUE;
	IPPU.RenderThisFrame = TRUE;
	IPPU.DirectColourMapsNeedRebuild = TRUE;
	IPPU.FrameCount = 0;
	IPPU.RenderedFramesCount = 0;
	IPPU.DisplayedRenderedFrameCount = 0;
	IPPU.SkippedFrames = 0;
	IPPU.FrameSkip = 0;
	ZeroMemory (IPPU.TileCached [TILE_2BIT], MAX_2BIT_TILES);
	ZeroMemory (IPPU.TileCached [TILE_4BIT], MAX_4BIT_TILES);
	ZeroMemory (IPPU.TileCached [TILE_8BIT], MAX_8BIT_TILES);
	IPPU.FirstVRAMRead = FALSE;
	IPPU.Interlace = FALSE;
	IPPU.InterlaceSprites = FALSE;
	IPPU.DoubleWidthPixels = FALSE;
	IPPU.DoubleHeightPixels = FALSE;
	IPPU.RenderedScreenWidth = SNES_WIDTH;
	IPPU.RenderedScreenHeight = SNES_HEIGHT;
	IPPU.XB = NULL;
	for (c = 0; c < 256; c++)
		IPPU.ScreenColors [c] = c;
	S9xFixColourBrightness ();
	IPPU.PreviousLine = IPPU.CurrentLine = 0;
//	IPPU.Joypads[0] = IPPU.Joypads[1] = IPPU.Joypads[2] = 0;
//	IPPU.Joypads[3] = IPPU.Joypads[4] = 0;
//	IPPU.SuperScope = 0;
//	IPPU.Mouse[0] = IPPU.Mouse[1] = 0;
//	IPPU.PrevMouseX[0] = IPPU.PrevMouseX[1] = 256 / 2;
//	IPPU.PrevMouseY[0] = IPPU.PrevMouseY[1] = 224 / 2;

	if (Settings.ControllerOption == 0)
		IPPU.Controller = SNES_MAX_CONTROLLER_OPTIONS - 1;
	else
		IPPU.Controller = Settings.ControllerOption - 1;
	S9xNextController ();

	for (c = 0; c < 2; c++)
		memset (&IPPU.Clip [c], 0, sizeof (struct ClipData));

	if (Settings.MouseMaster)
	{
		S9xProcessMouse (0);
		S9xProcessMouse (1);
	}
	for (c = 0; c < 0x8000; c += 0x100)
		memset (&Memory.FillRAM [c], c >> 8, 0x100);

	ZeroMemory (&Memory.FillRAM [0x2100], 0x100);
	ZeroMemory (&Memory.FillRAM [0x4200], 0x100);
	ZeroMemory (&Memory.FillRAM [0x4000], 0x100);
	// For BS Suttehakkun 2...
	ZeroMemory (&Memory.FillRAM [0x1000], 0x1000);

	Memory.FillRAM[0x4201]=Memory.FillRAM[0x4213]=0xFF;
}

void S9xProcessMouse (int which1)
{
    int x, y;
    uint32 buttons;
    
    if ((IPPU.Controller == SNES_MOUSE || IPPU.Controller == SNES_MOUSE_SWAPPED) && S9xReadMousePosition (which1, x, y, buttons))
    {
		int delta_x, delta_y;
#define MOUSE_SIGNATURE 0x1
		IPPU.Mouse [which1] = MOUSE_SIGNATURE | 
			      (PPU.MouseSpeed [which1] << 4) |
		              ((buttons & 1) << 6) | ((buttons & 2) << 6);

	delta_x = x - IPPU.PrevMouseX[which1];
	delta_y = y - IPPU.PrevMouseY[which1];

	if (delta_x > 63)
	{
	    delta_x = 63;
	    IPPU.PrevMouseX[which1] += 63;
	}
	else
	if (delta_x < -63)
	{
	    delta_x = -63;
	    IPPU.PrevMouseX[which1] -= 63;
	}
	else
	    IPPU.PrevMouseX[which1] = x;

	if (delta_y > 63)
	{
	    delta_y = 63;
	    IPPU.PrevMouseY[which1] += 63;
	}
	else
	if (delta_y < -63)
	{
	    delta_y = -63;
	    IPPU.PrevMouseY[which1] -= 63;
	}
	else
	    IPPU.PrevMouseY[which1] = y;

	if (delta_x < 0)
	{
	    delta_x = -delta_x;
	    IPPU.Mouse [which1] |= (delta_x | 0x80) << 16;
	}
	else
	    IPPU.Mouse [which1] |= delta_x << 16;

	if (delta_y < 0)
	{
	    delta_y = -delta_y;
	    IPPU.Mouse [which1] |= (delta_y | 0x80) << 24;
	}
	else
	    IPPU.Mouse [which1] |= delta_y << 24;

	if (IPPU.Controller == SNES_MOUSE_SWAPPED)
	    IPPU.Joypads [0] = IPPU.Mouse [which1];
	else
	    IPPU.Joypads [1] = IPPU.Mouse [which1];
    }
}

void S9xNextController ()
{
    switch (IPPU.Controller)
    {
	  case SNES_MULTIPLAYER5:
		IPPU.Controller = SNES_JOYPAD;
		break;
	  case SNES_JOYPAD:
		if (Settings.MouseMaster)
		{
			IPPU.Controller = SNES_MOUSE_SWAPPED;
			break;
		}
	  case SNES_MOUSE_SWAPPED:
		if (Settings.MouseMaster)
		{
			IPPU.Controller = SNES_MOUSE;
			break;
		}
	  case SNES_MOUSE:
		if (Settings.SuperScopeMaster)
		{
			IPPU.Controller = SNES_SUPERSCOPE;
			break;
		}
	  case SNES_SUPERSCOPE:
		if (Settings.JustifierMaster)
		{
			IPPU.Controller = SNES_JUSTIFIER;
			break;
		}
	  case SNES_JUSTIFIER:
		if(Settings.JustifierMaster)
		{
			IPPU.Controller=SNES_JUSTIFIER_2;
			break;
		}
	  case SNES_JUSTIFIER_2:
		if(Settings.MultiPlayer5Master)
		{
			IPPU.Controller=SNES_MULTIPLAYER5;
			break;
		}
	  default:
		IPPU.Controller = SNES_JOYPAD;
		break;
    }
}

/* 0x0015d334 — original native Super FX execution/IRQ bridge, 184 bytes.
 * Calls the original FxEmulate(uint32) ABI and canonical CPU IRQ setter.
 * This is the active historical PS2 branch of S9xSuperFXExec.
 */
extern int FxEmulate(uint32);
extern "C" void S9xSetIRQ(uint32);
extern "C" void S9xSuperFXExec();
#define GSU_SFR 0x30
#define GSU_SCMR 0x3a
#define GSU_CLSR 0x39
#define FLG_G 0x20
#define FLG_IRQ 0x8000
#define GSU_IRQ_SOURCE 4
void S9xSuperFXExec ()
{
    if (Settings.SuperFX)
    {
	if ((Memory.FillRAM [0x3000 + GSU_SFR] & FLG_G) &&
	    (Memory.FillRAM [0x3000 + GSU_SCMR] & 0x18) == 0x18)
	{
	    if (!Settings.WinterGold||Settings.StarfoxHack)
		FxEmulate (~0);
	    else
		FxEmulate ((Memory.FillRAM [0x3000 + GSU_CLSR] & 1) ? 700 : 350);
	    int GSUStatus = Memory.FillRAM [0x3000 + GSU_SFR] |
			    (Memory.FillRAM [0x3000 + GSU_SFR + 1] << 8);
	    if ((GSUStatus & (FLG_G | FLG_IRQ)) == FLG_IRQ)
	    {
		// Trigger a GSU IRQ.
		S9xSetIRQ (GSU_IRQ_SOURCE);
	    }
	}
    }
}
