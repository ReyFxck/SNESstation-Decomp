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

/* Native PPU controller update paths from Snes9x 1.41-1.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Shared PPU, IPPU, Settings, Memory and Justifier state are imported.
 * PS2 frontend callback stubs retain the frozen eight-byte leaf behavior
 * and their original argument/return ABI. The joypad reader remains a
 * separately reviewed native frontend provider contract.
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
#define SNES_WIDTH 256
#define SNES_HEIGHT 224
#define MAX_2BIT_TILES 4096
#define MAX_4BIT_TILES 2048
#define MAX_8BIT_TILES 1024
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

#define MEMMAP_NUM_BLOCKS 4096
#define ROM_NAME_LEN 23
#define MEMMAP_MAX_SDD1_LOGGED_ENTRIES (0x10000/8)
#define _MAX_PATH 1024
class CMemory { public:
    enum {
	MAP_PPU, MAP_CPU, MAP_DSP, MAP_LOROM_SRAM, MAP_HIROM_SRAM,
	MAP_NONE, MAP_DEBUG, MAP_C4, MAP_BWRAM, MAP_BWRAM_BITMAP,
	MAP_BWRAM_BITMAP2, MAP_SA1RAM, MAP_SPC7110_ROM, MAP_SPC7110_DRAM,
	MAP_RONLY_SRAM, MAP_OBC_RAM, MAP_SETA_DSP, MAP_SETA_RISC, MAP_LAST
    };
    enum { MAX_ROM_SIZE = 0x800000 };

    uint8 *RAM;
    uint8 *ROM;
    uint8 *VRAM;
    uint8 *SRAM;
    uint8 *BWRAM;
    uint8 *FillRAM;
    uint8 *C4RAM;
    bool8 HiROM;
    bool8 LoROM;
    uint32 SRAMMask;
    uint8 SRAMSize;
    uint8 *Map [MEMMAP_NUM_BLOCKS];
    uint8 *WriteMap [MEMMAP_NUM_BLOCKS];
    uint8 MemorySpeed [MEMMAP_NUM_BLOCKS];
    uint8 BlockIsRAM [MEMMAP_NUM_BLOCKS];
    uint8 BlockIsROM [MEMMAP_NUM_BLOCKS];
    char  ROMName [ROM_NAME_LEN];
    char  ROMId [5];
    char  CompanyId [3];
    uint8 ROMSpeed;
    uint8 ROMType;
    uint8 ROMSize;
    int32 ROMFramesPerSecond;
    int32 HeaderCount;
    uint32 CalculatedSize;
    uint32 CalculatedChecksum;
    uint32 ROMChecksum;
    uint32 ROMComplementChecksum;
    uint8  *SDD1Index;
    uint8  *SDD1Data;
    uint32 SDD1Entries;
    uint32 SDD1LoggedDataCountPrev;
    uint32 SDD1LoggedDataCount;
    uint8  SDD1LoggedData [MEMMAP_MAX_SDD1_LOGGED_ENTRIES];
    char ROMFilename [_MAX_PATH];
	uint8 ROMRegion;
    uint32 ROMCRC32;
	uint8 *BSRAM;
//	bool8 Hacked;
//	bool8 Iffy;
//	uint8 Iformat;
};


enum { SNES_MULTIPLAYER5, SNES_JOYPAD, SNES_MOUSE_SWAPPED,
       SNES_MOUSE, SNES_SUPERSCOPE, SNES_JUSTIFIER, SNES_JUSTIFIER_2 };
#define SNES_RIGHT_MASK (1 << 8)
#define SNES_LEFT_MASK (1 << 9)
#define SNES_DOWN_MASK (1 << 10)
#define SNES_UP_MASK (1 << 11)
#define MAX_5C78_VERSION 3
extern SSettings Settings __asm__("DAT_003454e0");
extern SPPU PPU __asm__("DAT_0035b788");
extern InternalPPU IPPU __asm__("DAT_0035c268");
extern CMemory Memory __asm__("DAT_0034e2b0");
extern uint32 justifiers __asm__("DAT_003f4bf0");
extern uint8 in_bit __asm__("DAT_003f4bf4");
extern bool last_p1 __asm__("bRam0042e888");
extern "C" {
    uint32 S9xReadJoypad(int);
    void S9xProcessMouse(int);
    void S9xUpdateJoypads();
    bool8 S9xReadSuperScopePosition(int &, int &, uint32 &);
    bool8 S9xReadMousePosition(int, int &, int &, uint32 &);
}
bool JustifierOffscreen();
void JustifierButtons(uint32 &);

typedef char NativeControllerSettingsSize[(sizeof(SSettings)==328)?1:-1];
typedef char NativeControllerPPUSize[(sizeof(SPPU)==2780)?1:-1];
typedef char NativeControllerIPPUSize[(sizeof(InternalPPU)==4340)?1:-1];
typedef char NativeControllerMemorySize[(sizeof(CMemory)==54404)?1:-1];
typedef char NativeControllerBoolSize[(sizeof(bool)==1)?1:-1];

/* 0x00101880: target frontend reports the Justifier on-screen. */
bool JustifierOffscreen() { return false; }
/* 0x00101888: target frontend leaves Justifier button state unchanged. */
void JustifierButtons(uint32 &) {}
/* 0x00104e48: target frontend does not provide Super Scope coordinates. */
extern "C" bool8 S9xReadSuperScopePosition(int &, int &, uint32 &) { return 0; }
/* 0x00104e50: target frontend does not provide mouse coordinates. */
extern "C" bool8 S9xReadMousePosition(int, int &, int &, uint32 &) { return 0; }

/* 0x0015cce4: original Super Scope processing. */
void ProcessSuperScope ()
{
    int x, y;
    uint32 buttons;

    if (IPPU.Controller == SNES_SUPERSCOPE &&
		S9xReadSuperScopePosition (x, y, buttons))
	{
#define SUPERSCOPE_SIGNATURE 0x00ff
		uint32 scope;

		scope = SUPERSCOPE_SIGNATURE | ((buttons & 1) << (7 + 8)) |
			((buttons & 2) << (5 + 8)) | ((buttons & 4) << (3 + 8)) |
			((buttons & 8) << (1 + 8));
		if(Memory.FillRAM[0x4201]&0x80){
			x+=40;
			if (x > 295)
				x = 295;
			if (x < 40)
				x = 40;
			if (y > PPU.ScreenHeight - 1)
				y = PPU.ScreenHeight - 1;
			if (y < 0)
				y = 0;

			PPU.VBeamPosLatched = (uint16) (y + 1);
			PPU.HBeamPosLatched = (uint16) x;
			PPU.HVBeamCounterLatched = TRUE;
			Memory.FillRAM [0x213F] |= 0x40 | MAX_5C78_VERSION;
		}
		IPPU.Joypads [1] = scope;
	}
}

/* 0x0015cea4: original Justifier processing and player alternation. */
void S9xUpdateJustifiers()
{
	/* Shared original zero-filled last_p1 byte imported above. */
	in_bit=0;
//	static int p1count;
	justifiers=0xFFFF00AA;

	bool offscreen=JustifierOffscreen();

	JustifierButtons(justifiers);
//	if(p1count==32)
//	{
		last_p1=!last_p1;
//		p1count=0;
//	}
//	p1count++;

	if(!last_p1)
		justifiers|=0x1000;

	int x,y;
	uint32 buttons;

	if(Memory.FillRAM[0x4201]&0x80)
	{

		S9xReadSuperScopePosition(x,y,buttons);

		x+=40;
		if (x > 295)
			x = 295;
		if (x < 40)
			x = 40;
		if (y > PPU.ScreenHeight - 1)
			y = PPU.ScreenHeight - 1;
		if (y < 0)
			y = 0;

		if(last_p1)
		{

			PPU.HVBeamCounterLatched = FALSE;
			Memory.FillRAM [0x213F] = MAX_5C78_VERSION;

			//process latch as Justifier 2
			if(Settings.SecondJustifier)
			{
				if(IPPU.Controller==SNES_JUSTIFIER_2)
				{
					if(!offscreen)
					{

						PPU.VBeamPosLatched = (uint16) (y + 1);
						PPU.HBeamPosLatched = (uint16) x;
						PPU.HVBeamCounterLatched = TRUE;
						Memory.FillRAM [0x213F] |= 0x40 | MAX_5C78_VERSION;
					}
				}
			}
		}
		else
		{

			PPU.HVBeamCounterLatched = FALSE;
			Memory.FillRAM [0x213F] = MAX_5C78_VERSION;

			//emulate player 1.
			if(IPPU.Controller==SNES_JUSTIFIER)
			{
				if(!offscreen)
				{
					PPU.VBeamPosLatched = (uint16) (y + 1);
					PPU.HBeamPosLatched = (uint16) x;
					PPU.HVBeamCounterLatched = TRUE;
					Memory.FillRAM [0x213F] |= 0x40 | MAX_5C78_VERSION;
				}
			}
		}

		//needs restructure
		if(!offscreen)
		{

			if((!last_p1&&IPPU.Controller==SNES_JUSTIFIER)||(last_p1&&IPPU.Controller==SNES_JUSTIFIER_2))
			{
				PPU.VBeamPosLatched = (uint16) (y + 1);
				PPU.HBeamPosLatched = (uint16) x;
				PPU.HVBeamCounterLatched = TRUE;
				Memory.FillRAM [0x213F] |= 0x40 | MAX_5C78_VERSION;
			}
			else
			{
				PPU.HVBeamCounterLatched = FALSE;
				Memory.FillRAM [0x213F] = MAX_5C78_VERSION;

			}
		}
		else
		{
			PPU.HVBeamCounterLatched = FALSE;
			Memory.FillRAM [0x213F] = MAX_5C78_VERSION;
		}
	}
}

/* 0x0015d0bc: original joypad refresh and automatic register reads. */
void S9xUpdateJoypads ()
{
    int i;

    for (i = 0; i < 5; i++)
	{
		IPPU.Joypads [i] = S9xReadJoypad (i);
		if (IPPU.Joypads [i] & SNES_LEFT_MASK)
			IPPU.Joypads [i] &= ~SNES_RIGHT_MASK;
		if (IPPU.Joypads [i] & SNES_UP_MASK)
			IPPU.Joypads [i] &= ~SNES_DOWN_MASK;
	}


    // BJ: This is correct behavior AFAICT (used to be Touhaiden hack)
    if (IPPU.Controller == SNES_JOYPAD || IPPU.Controller == SNES_MULTIPLAYER5)
    {
		for (i = 0; i < 5; i++)
		{
			if (IPPU.Joypads [i])
				IPPU.Joypads [i] |= 0xffff0000;
		}
    }

    // Read mouse position if enabled
    if (Settings.MouseMaster)
    {
		for (i = 0; i < 2; i++)
			S9xProcessMouse (i);
    }

    // Read SuperScope if enabled
    if (Settings.SuperScopeMaster)
		ProcessSuperScope ();

    if (Memory.FillRAM [0x4200] & 1)
    {
		PPU.Joypad1ButtonReadPos = 16;
		if (Memory.FillRAM [0x4201] & 0x80)
		{
			PPU.Joypad2ButtonReadPos = 16;
			PPU.Joypad3ButtonReadPos = 0;
		}
		else
		{
			PPU.Joypad2ButtonReadPos = 0;
			PPU.Joypad3ButtonReadPos = 16;
		}
		int ind = Settings.SwapJoypads ? 1 : 0;

		Memory.FillRAM [0x4218] = (uint8) IPPU.Joypads [ind];
		Memory.FillRAM [0x4219] = (uint8) (IPPU.Joypads [ind] >> 8);
		Memory.FillRAM [0x421a] = (uint8) IPPU.Joypads [ind ^ 1];
		Memory.FillRAM [0x421b] = (uint8) (IPPU.Joypads [ind ^ 1] >> 8);
		if (Memory.FillRAM [0x4201] & 0x80)
		{
			Memory.FillRAM [0x421c] = (uint8) IPPU.Joypads [ind];
			Memory.FillRAM [0x421d] = (uint8) (IPPU.Joypads [ind] >> 8);
			Memory.FillRAM [0x421e] = (uint8) IPPU.Joypads [2];
			Memory.FillRAM [0x421f] = (uint8) (IPPU.Joypads [2] >> 8);
		}
		else
		{
			Memory.FillRAM [0x421c] = (uint8) IPPU.Joypads [3];
			Memory.FillRAM [0x421d] = (uint8) (IPPU.Joypads [3] >> 8);
			Memory.FillRAM [0x421e] = (uint8) IPPU.Joypads [4];
			Memory.FillRAM [0x421f] = (uint8) (IPPU.Joypads [4] >> 8);
		}
    }
	if(Settings.Justifier||Settings.SecondJustifier)
	{
		Memory.FillRAM [0x421a] = 0x0E;
		Memory.FillRAM [0x421b] = 0;
		S9xUpdateJustifiers();
	}

}
