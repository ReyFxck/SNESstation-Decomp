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

/* Native ROM and RAM mapping from Snes9x 1.41-1.
 * Reuses the original Memory, Settings, SA-1, SRAM and ROM scratch providers.
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


#define MEMMAP_NUM_BLOCKS 4096
#define ROM_NAME_LEN 23
#define MEMMAP_MAX_SDD1_LOGGED_ENTRIES (0x10000/8)
#define _MAX_PATH 1024
class CMemory { public:
    void MapRAM();
    void MapExtraRAM();
    void LoROMMap();
    void BSLoROMMap();
    void HiROMMap();
    void TalesROMMap(bool8);
    void AlphaROMMap();
    void SuperFXROMMap();
    void SA1ROMMap();
    void LoROM24MBSMap();
    void SufamiTurboLoROMMap();
    void SRAM512KLoROMMap();
    void BSHiROMMap();
    void JumboLoROMMap();
    void SPC7110HiROMMap();
    void WriteProtectROM();
    void FixROMSpeed();
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

extern SSettings Settings __asm__("DAT_003454e0");
extern CMemory Memory __asm__("DAT_0034e2b0");

typedef signed short int16;
typedef signed long long int64;
#define ONE_CYCLE 6
#define SLOW_ONE_CYCLE 8
#define ST_010 1
#define ST_011 2
#define ST_018 3
#define BUILD_PIXEL(r,g,b) (((r) & 31) | (((g) & 31) << 5) | (((b) & 31) << 10))
#define SET_UI_COLOR(r,g,b)
struct SSA1 {
    struct  SOpcodes *S9xOpcodes;
    uint8   _Carry;
    uint8   _Zero;
    uint8   _Negative;
    uint8   _Overflow;
    bool8   CPUExecuting;
    uint32  ShiftedPB;
    uint32  ShiftedDB;
    uint32  Flags;
    bool8   Executing;
    bool8   NMIActive;
    bool8   IRQActive;
    bool8   WaitingForInterrupt;
    bool8   Waiting;
//    uint8   WhichEvent;
    uint8   *PC;
    uint8   *PCBase;
    uint8   *BWRAM;
    uint8   *PCAtOpcodeStart;
    uint8   *WaitAddress;
    uint32  WaitCounter;
    uint8   *WaitByteAddress1;
    uint8   *WaitByteAddress2;
//    long    Cycles;
//    long    NextEvent;
//    long    V_Counter;
    uint8   *Map [MEMMAP_NUM_BLOCKS];
    uint8   *WriteMap [MEMMAP_NUM_BLOCKS];
    int16   op1;
    int16   op2;
    int     arithmetic_op;
    int64   sum;
    bool8   overflow;
    uint8   VirtualBitmapFormat;
    bool8   in_char_dma;
    uint8   variable_bit_pos;
};


extern SSA1 SA1 __asm__("g_SA1_blob");
extern uint8 *SRAM __asm__("DAT_0034e298");
/* Existing address contract names the bank-biased scratch pointer.
 * Its real 8 KiB buffer starts 0x6000 bytes later, at 0x0042c820. */
extern uint8 bytes0x2000[0x2000] __asm__("DAT_00426820+0x6000");
void S9xSpc7110Init();
extern "C" {
    void *memmove(void *, const void *, unsigned int);
    void *memset(void *, int, unsigned int);
    int strncmp(const char *, const char *, unsigned int);
}
typedef char NativeROMMapSettingsSize[(sizeof(SSettings)==328)?1:-1];
typedef char NativeROMMapMemorySize[(sizeof(CMemory)==54404)?1:-1];
typedef char NativeROMMapSA1Size[(sizeof(SSA1)==32856)?1:-1];
/* 0x00153674 */
void CMemory::MapRAM ()
{
    int c;

	if(Memory.LoROM)
	{
		// Banks 70->77, S-RAM
		for (c = 0; c < 0x0f; c++)
		{
			for(int i=0;i<8;i++)
			{
				Map [(c<<4) + 0xF00+i]=Map [(c<<4) + 0x700+i] = (uint8 *) MAP_LOROM_SRAM;
				BlockIsRAM [(c<<4) + 0xF00+i] =BlockIsRAM [(c<<4) + 0x700+i] = TRUE;
				BlockIsROM [(c<<4) + 0xF00+i] =BlockIsROM [(c<<4) + 0x700+i] = FALSE;
			}
		}
	}
	/*
	else
	{
		for (c = 0; c < 0x0f; c++)
		{
			for(int i=0;i<8;i++)
			{
				Map [(c<<4) + 0x700+i] = (uint8 *) MAP_LOROM_SRAM;
				BlockIsRAM [(c<<4) + 0x700+i] = TRUE;
				BlockIsROM [(c<<4) + 0x700+i] = FALSE;
			}
		}

	}
*/

    // Banks 7e->7f, RAM
    for (c = 0; c < 16; c++)
    {
		Map [c + 0x7e0] = RAM;
		Map [c + 0x7f0] = RAM + 0x10000;
		BlockIsRAM [c + 0x7e0] = TRUE;
		BlockIsRAM [c + 0x7f0] = TRUE;
		BlockIsROM [c + 0x7e0] = FALSE;
		BlockIsROM [c + 0x7f0] = FALSE;
    }

/*	if(Memory.LoROM)
	{
		for (int c=0xF00;c<0xF09;c++)
		{
			Map [c] = (uint8 *) MAP_LOROM_SRAM;
			BlockIsRAM [c] = TRUE;
			BlockIsROM [c] = FALSE;
		}
	}*/
	WriteProtectROM ();
}

/* 0x00153780 */
void CMemory::MapExtraRAM ()
{
    int c;

    // Banks 7e->7f, RAM
    for (c = 0; c < 16; c++)
    {
		Map [c + 0x7e0] = RAM;
		Map [c + 0x7f0] = RAM + 0x10000;
		BlockIsRAM [c + 0x7e0] = TRUE;
		BlockIsRAM [c + 0x7f0] = TRUE;
		BlockIsROM [c + 0x7e0] = FALSE;
		BlockIsROM [c + 0x7f0] = FALSE;
    }

    // Banks 70->73, S-RAM
    for (c = 0; c < 16; c++)
    {
		Map [c + 0x700] = ::SRAM;
		Map [c + 0x710] = ::SRAM + 0x8000;
		Map [c + 0x720] = ::SRAM + 0x10000;
		Map [c + 0x730] = ::SRAM + 0x18000;

		BlockIsRAM [c + 0x700] = TRUE;
		BlockIsROM [c + 0x700] = FALSE;
		BlockIsRAM [c + 0x710] = TRUE;
		BlockIsROM [c + 0x710] = FALSE;
		BlockIsRAM [c + 0x720] = TRUE;
		BlockIsROM [c + 0x720] = FALSE;
		BlockIsRAM [c + 0x730] = TRUE;
		BlockIsROM [c + 0x730] = FALSE;
    }
}

/* 0x001538b0 */
void CMemory::LoROMMap ()
{
    int c;
    int i;
    int j;
	int mask[4];
	for (j=0; j<4; j++)
		mask[j]=0x00ff;

	mask[0]=(CalculatedSize/0x8000)-1;

	int x;
	bool foundZeros;
	bool pastZeros;

	for(j=0;j<3;j++)
	{
		x=1;
		foundZeros=false;
		pastZeros=false;

		mask[j+1]=mask[j];

		while (x>0x100&&!pastZeros)
		{
			if(mask[j]&x)
			{
				x<<=1;
				if(foundZeros)
					pastZeros=true;
			}
			else
			{
				foundZeros=true;
				pastZeros=false;
				mask[j+1]|=x;
				x<<=1;
			}
		}
	}


    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		if(Settings.SETA==ST_018)
			Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_SETA_RISC;
		else Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
		if (Settings.DSP1Master)
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_DSP;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_DSP;
		}
		else if (Settings.C4)
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_C4;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_C4;
		}
		else if(Settings.OBC1)
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_OBC_RAM;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_OBC_RAM;
		}
		else
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) bytes0x2000 - 0x6000;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) bytes0x2000 - 0x6000;
		}

		for (i = c + 8; i < c + 16; i++)
		{
			int e=3;
			int d=c>>4;
			while(d>mask[0])
			{
				d&=mask[e];
				e--;
			}
			Map [i] = Map [i + 0x800] = ROM + (((d)-1)*0x8000);
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    if (Settings.DSP1Master)
    {
		// Banks 30->3f and b0->bf
		for (c = 0x300; c < 0x400; c += 16)
		{
			for (i = c + 8; i < c + 16; i++)
			{
				Map [i] = Map [i + 0x800] = (uint8 *) MAP_DSP;
				BlockIsROM [i] = BlockIsROM [i + 0x800] = FALSE;
			}
		}
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 8; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 11) % CalculatedSize];

		for (i = c + 8; i < c + 16; i++)
		{
			int e=3;
			int d=(c+0x400)>>4;
			while(d>mask[0])
			{
				d&=mask[e];
				e--;
			}

			Map [i + 0x400] = Map [i + 0xc00] = ROM + (((d)-1)*0x8000);
//			Map [i + 0x400] = Map [i + 0xc00] = &ROM [((c << 11) + 0x200000) % CalculatedSize - 0x8000];
		}

		for (i = c; i < c + 16; i++)
		{
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

	if(Settings.SETA!=0&&Settings.SETA!=ST_018)
	{
		memset(SRAM, 0, 0x1000);
		for(int q=0;q<0x08;q++)
		{
			//where does the SETA chip access, anyway?
			//please confirm this?
			Map[0x680+q]=(uint8*)MAP_SETA_DSP;
			MemorySpeed [0x680+q] = SLOW_ONE_CYCLE;
			BlockIsROM [0x680+q] = FALSE;
			BlockIsRAM [0x680+q] = TRUE;
			//and this!
			Map[0x600]=(uint8*)MAP_SETA_DSP;
			MemorySpeed [0x600] = SLOW_ONE_CYCLE;
			BlockIsROM [0x600] = FALSE;
		}

	//	if(Settings.SETA==ST_011)
	//	{
	//		Map[0x600]=(uint8*)MAP_SETA_DSP;
	//		MemorySpeed [0x600] = SLOW_ONE_CYCLE;
	//		BlockIsROM [0x600] = FALSE;
	//		BlockIsRAM [0x600] = TRUE;
	//	}
	}

    if (Settings.DSP1Master)
    {
		for (c = 0; c < 0x100; c++)
		{
			Map [c + 0xe00] = (uint8 *) MAP_DSP;
			MemorySpeed [c + 0xe00] = SLOW_ONE_CYCLE;
			BlockIsROM [c + 0xe00] = FALSE;
		}
    }

    MapRAM ();
    WriteProtectROM ();
}
/* 0x00153da4 */
void CMemory::BSLoROMMap ()
{
    int c;
    int i;

	if(Settings.BS)
		SRAMSize=5;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) RAM;
//		Map [c + 5] = Map [c + 0x805] = (uint8 *) SRAM;
BlockIsRAM [c + 5] = BlockIsRAM [c + 0x805] = TRUE;

//		Map [c + 6] = Map [c + 0x806] = (uint8 *)MAP_NONE;
//		Map [c + 7] = Map [c + 0x807] = (uint8 *)MAP_NONE;
				Map [c + 6] = Map [c + 0x806] = (uint8 *) RAM;
//		Map [c + 5] = Map [c + 0x805] = (uint8 *) SRAM;
BlockIsRAM [c + 6] = BlockIsRAM [c + 0x806] = TRUE;
		Map [c + 7] = Map [c + 0x807] = (uint8 *) RAM;
//		Map [c + 5] = Map [c + 0x805] = (uint8 *) SRAM;
BlockIsRAM [c + 7] = BlockIsRAM [c + 0x807] = TRUE;
		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [(c << 11) % CalculatedSize] - 0x8000;
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

	for(c=0;c<8;c++)
	{
		Map[(c<<4)+0x105]=(uint8*)MAP_LOROM_SRAM;
		MemorySpeed [(c<<4)+0x105] = SLOW_ONE_CYCLE;
		BlockIsROM [(c<<4)+0x105] = FALSE;
		BlockIsRAM [(c<<4)+0x105] = TRUE;
	}


  /*  // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 8; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 11) % CalculatedSize];

		for (i = c + 8; i < c + 16; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [((c << 11) + 0x200000) % CalculatedSize - 0x8000];

		for (i = c; i < c + 16; i++)
		{
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }
	*/
	for(c=1;c<=4;c++)
	{
		for(i=0;i<16; i++)
		{
			Map[0x400+i+(c<<4)]=(uint8*)MAP_LOROM_SRAM;
			BlockIsRAM[0x400+i+(c<<4)]=TRUE;
			BlockIsROM[0x400+i+(c<<4)]=FALSE;
		}
	}

	for(i=0;i<0x80;i++)
	{
		Map[0x700+i]=&BSRAM[0x10000*(i/16)];
		BlockIsRAM[0x700+i]=TRUE;
		BlockIsROM[0x700+i]=FALSE;
		MemorySpeed[0x700+i]=SLOW_ONE_CYCLE;
	}
	for (i=0; i<8;i++)
	{
		Map[0x205+(i<<4)]=Map[0x285+(i<<4)]=Map[0x305+(i<<4)]=Map[0x385+(i<<4)]=Map[0x705+(i<<4)];
		BlockIsRAM[0x205+(i<<4)]=BlockIsRAM[0x285+(i<<4)]=BlockIsRAM[0x305+(i<<4)]=BlockIsRAM[0x385+(i<<4)]=TRUE;
		BlockIsROM[0x205+(i<<4)]=BlockIsROM[0x285+(i<<4)]=BlockIsROM[0x305+(i<<4)]=BlockIsROM[0x385+(i<<4)]=FALSE;
		MemorySpeed[0x205+(i<<4)]=MemorySpeed[0x285+(i<<4)]=MemorySpeed[0x305+(i<<4)]=MemorySpeed[0x385+(i<<4)]=SLOW_ONE_CYCLE;
	}
	for(c=0;c<8;c++)
	{
		Map[(c<<4)+0x005]=BSRAM-0x5000;
		MemorySpeed [(c<<4)+0x005] = SLOW_ONE_CYCLE;
		BlockIsROM [(c<<4)+0x005] = FALSE;
		BlockIsRAM [(c<<4)+0x005] = TRUE;
	}
    MapRAM ();
    WriteProtectROM ();


}

/* 0x001541e0 */
void CMemory::HiROMMap ()
{
    int i;
	int c;
        int j;

		int mask[4];
	for (j=0; j<4; j++)
		mask[j]=0x00ff;

	mask[0]=(CalculatedSize/0x10000)-1;

	if (Settings.ForceSA1 ||
			(!Settings.ForceNoSA1 && (ROMSpeed & ~0x10) == 0x23 &&
			(ROMType & 0xf) > 3 && (ROMType & 0xf0) == 0x30))
	{
			Settings.DisplayColor=BUILD_PIXEL(31,0,0);
			SET_UI_COLOR(255,0,0);
	}


	int x;
	bool foundZeros;
	bool pastZeros;

	for(j=0;j<3;j++)
	{
		x=1;
		foundZeros=false;
		pastZeros=false;

		mask[j+1]=mask[j];

		while (x>0x100&&!pastZeros)
		{
			if(mask[j]&x)
			{
				x<<=1;
				if(foundZeros)
					pastZeros=true;
			}
			else
			{
				foundZeros=true;
				pastZeros=false;
				mask[j+1]|=x;
				x<<=1;
			}
		}
	}

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
//		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_NONE;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;

		if (Settings.DSP1Master)
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_DSP;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_DSP;
		}
		else
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_NONE;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_NONE;
		}

		for (i = c + 8; i < c + 16; i++)
		{
			int e=3;
			int d=c>>4;
			while(d>mask[0])
			{
				d&=mask[e];
				e--;
			}
			Map [i] = Map [i + 0x800] = ROM + (d*0x10000);
	//		Map [i] = Map [i + 0x800] = &ROM [(c << 12) % CalculatedSize];
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 30->3f and b0->bf, address ranges 6000->7fff is S-RAM.
    for (c = 0; c < 16; c++)
    {
		Map [0x306 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		Map [0x307 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		Map [0xb06 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		Map [0xb07 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		BlockIsRAM [0x306 + (c << 4)] = TRUE;
		BlockIsRAM [0x307 + (c << 4)] = TRUE;
		BlockIsRAM [0xb06 + (c << 4)] = TRUE;
		BlockIsRAM [0xb07 + (c << 4)] = TRUE;
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 16; i++)
		{
			int e=3;
			int d=(c)>>4;
			while(d>mask[0])
			{
				d&=mask[e];
				e--;
			}
			Map [i + 0x400] = Map [i + 0xc00] = ROM + (d*0x10000);
//			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 12) % CalculatedSize];
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

    MapRAM ();
    WriteProtectROM ();
}

/* 0x0015458c */
void CMemory::TalesROMMap (bool8 Interleaved)
{
	  int c;
    int i;

	if(Interleaved)
	{
		if(Settings.DisplayColor==0xffff)
		{
			Settings.DisplayColor=BUILD_PIXEL(0,31,0);
			SET_UI_COLOR(0,255,0);
		}
	}
    uint32 OFFSET0 = 0x400000;
    uint32 OFFSET1 = 0x400000;
    uint32 OFFSET2 = 0x000000;

    if (Interleaved)
    {
		OFFSET0 = 0x000000;
		OFFSET1 = 0x000000;
		OFFSET2 = 0x200000;
    }

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
		Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_NONE;
		Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_NONE;
		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = &ROM [((c << 12) % (CalculatedSize-0x400000)) + OFFSET0];
			Map [i + 0x800] = &ROM [((c << 12) % 0x400000) + OFFSET2];
			BlockIsROM [i] = TRUE;
			BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 30->3f and b0->bf, address ranges 6000->7ffff is S-RAM.
    for (c = 0; c < 16; c++)
    {
		Map [0x306 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		Map [0x307 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		Map [0xb06 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		Map [0xb07 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		BlockIsRAM [0x306 + (c << 4)] = TRUE;
		BlockIsRAM [0x307 + (c << 4)] = TRUE;
		BlockIsRAM [0xb06 + (c << 4)] = TRUE;
		BlockIsRAM [0xb07 + (c << 4)] = TRUE;
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 8; i++)
		{
			Map [i + 0x400] = &ROM [((c << 12) % (CalculatedSize-0x400000)) + OFFSET1];
			Map [i + 0x408] = &ROM [((c << 12) % (CalculatedSize-0x400000)) + OFFSET1];
			Map [i + 0xc00] = &ROM [((c << 12) %0x400000)+ OFFSET2];
			Map [i + 0xc08] = &ROM [((c << 12) % 0x400000) + OFFSET2];
			BlockIsROM [i + 0x400] = TRUE;
			BlockIsROM [i + 0x408] = TRUE;
			BlockIsROM [i + 0xc00] = TRUE;
			BlockIsROM [i + 0xc08] = TRUE;
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			MemorySpeed [i + 0x408] = MemorySpeed [i + 0xc08] = SLOW_ONE_CYCLE;
		}
    }

	if((strncmp("TALES",(char*)Map[8]+0xFFC0, 5)==0))
	{
		if(((*(Map[8]+0xFFDE))==(*(Map[0x808]+0xFFDE))))
		{
			Settings.DisplayColor=BUILD_PIXEL(31,0,0);
			SET_UI_COLOR(255,0,0);
		}
	}

	ROMChecksum = *(Map[8]+0xFFDE) + (*(Map[8]+0xFFDF) << 8);
	ROMComplementChecksum = *(Map[8]+0xFFDC) + (*(Map[8]+0xFFDD) << 8);

int sum=0;
for(i=0x40;i<0x80; i++)
{
	uint8 * bank_low=(uint8*)Map[i<<4];
	uint8 * bank_high=(uint8*)Map[(i<<4)+0x800];
	for (c=0;c<0x10000; c++)
	{
		sum+=bank_low[c];
		sum+=bank_high[c];
	}
}

CalculatedChecksum=sum&0xFFFF;

    MapRAM ();
    WriteProtectROM ();
}

/* 0x00154a24 */
void CMemory::AlphaROMMap ()
{
    int c;
    int i;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
		Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_DSP;
		Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_DSP;

		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [c << 11] - 0x8000;
			BlockIsROM [i] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 40->7f and c0->ff

    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 16; i++)
		{
			Map [i + 0x400] = &ROM [(c << 12) % CalculatedSize];
			Map [i + 0xc00] = &ROM [(c << 12) % CalculatedSize];
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

    MapRAM ();
    WriteProtectROM ();
}

/* 0x00154c2c */
void CMemory::SuperFXROMMap ()
{
    int c;
    int i;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
		Map [0x006 + c] = Map [0x806 + c] = (uint8 *) ::SRAM - 0x6000;
		Map [0x007 + c] = Map [0x807 + c] = (uint8 *) ::SRAM - 0x6000;
		BlockIsRAM [0x006 + c] = BlockIsRAM [0x007 + c] = BlockIsRAM [0x806 + c] = BlockIsRAM [0x807 + c] = TRUE;

		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [c << 11] - 0x8000;
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 8; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 16; i++)
		{
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 12) % CalculatedSize];
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

    // Banks 7e->7f, RAM
    for (c = 0; c < 16; c++)
    {
		Map [c + 0x7e0] = RAM;
		Map [c + 0x7f0] = RAM + 0x10000;
		BlockIsRAM [c + 0x7e0] = TRUE;
		BlockIsRAM [c + 0x7f0] = TRUE;
		BlockIsROM [c + 0x7e0] = FALSE;
		BlockIsROM [c + 0x7f0] = FALSE;
    }

    // Banks 70->71, S-RAM
    for (c = 0; c < 32; c++)
    {
		Map [c + 0x700] = ::SRAM + (((c >> 4) & 1) << 16);
		BlockIsRAM [c + 0x700] = TRUE;
		BlockIsROM [c + 0x700] = FALSE;
    }

    // Replicate the first 2Mb of the ROM at ROM + 2MB such that each 32K
    // block is repeated twice in each 64K block.
    for (c = 0; c < 64; c++)
    {
		memmove (&ROM [0x200000 + c * 0x10000], &ROM [c * 0x8000], 0x8000);
		memmove (&ROM [0x208000 + c * 0x10000], &ROM [c * 0x8000], 0x8000);
    }

    WriteProtectROM ();
}

/* 0x00154f9c */
void CMemory::SA1ROMMap ()
{
    int c;
    int i;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) &Memory.FillRAM [0x3000] - 0x3000;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
		Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_BWRAM;
		Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_BWRAM;
		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [c << 11] - 0x8000;
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 40->7f
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 16; i++)
			Map [i + 0x400] = (uint8 *) &SRAM [(c << 12) & 0x1ffff];

		for (i = c; i < c + 16; i++)
		{
			MemorySpeed [i + 0x400] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = FALSE;
		}
    }

    // c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c;  i < c + 16; i++)
		{
			Map [i + 0xc00] = &ROM [(c << 12) % CalculatedSize];
			MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0xc00] = TRUE;
		}
    }

    for (c = 0; c < 16; c++)
    {
		Map [c + 0x7e0] = RAM;
		Map [c + 0x7f0] = RAM + 0x10000;
		BlockIsRAM [c + 0x7e0] = TRUE;
		BlockIsRAM [c + 0x7f0] = TRUE;
		BlockIsROM [c + 0x7e0] = FALSE;
		BlockIsROM [c + 0x7f0] = FALSE;
    }
    WriteProtectROM ();

    // Now copy the map and correct it for the SA1 CPU.
    memmove ((void *) SA1.WriteMap, (void *) WriteMap, sizeof (WriteMap));
    memmove ((void *) SA1.Map, (void *) Map, sizeof (Map));

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		SA1.Map [c + 0] = SA1.Map [c + 0x800] = &Memory.FillRAM [0x3000];
		SA1.Map [c + 1] = SA1.Map [c + 0x801] = (uint8 *) MAP_NONE;
		SA1.WriteMap [c + 0] = SA1.WriteMap [c + 0x800] = &Memory.FillRAM [0x3000];
		SA1.WriteMap [c + 1] = SA1.WriteMap [c + 0x801] = (uint8 *) MAP_NONE;
    }

    // Banks 60->6f
    for (c = 0; c < 0x100; c++)
		SA1.Map [c + 0x600] = SA1.WriteMap [c + 0x600] = (uint8 *) MAP_BWRAM_BITMAP;

    BWRAM = SRAM;
}

/* 0x00155334 */
void CMemory::LoROM24MBSMap ()
{
    int c;
    int i;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
        Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_NONE;
        Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_NONE;

		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [c << 11] - 0x8000;
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x200; c += 16)
    {
		Map [c + 0x800] = RAM;
		Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 0x805] = (uint8 *) MAP_CPU;
        Map [c + 0x806] = (uint8 *) MAP_NONE;
		Map [c + 0x807] = (uint8 *) MAP_NONE;

		for (i = c + 8; i < c + 16; i++)
		{
			Map [i + 0x800] = &ROM [c << 11] - 0x8000 + 0x200000;
			BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 8; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 11) + 0x200000];

		for (i = c + 8; i < c + 16; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 11) + 0x200000 - 0x8000];

		for (i = c; i < c + 16; i++)
		{
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

    MapExtraRAM ();
    WriteProtectROM ();
}

/* 0x0015568c */
void CMemory::SufamiTurboLoROMMap ()
{
    int c;
    int i;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
		if (Settings.DSP1Master)
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_DSP;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_DSP;
		}
		else
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_NONE;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_NONE;
		}
		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [c << 11] - 0x8000;
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    if (Settings.DSP1Master)
    {
		// Banks 30->3f and b0->bf
		for (c = 0x300; c < 0x400; c += 16)
		{
			for (i = c + 8; i < c + 16; i++)
			{
				Map [i] = Map [i + 0x800] = (uint8 *) MAP_DSP;
				BlockIsROM [i] = BlockIsROM [i + 0x800] = FALSE;
			}
		}
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 8; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 11) + 0x200000];

		for (i = c + 8; i < c + 16; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 11) + 0x200000 - 0x8000];

		for (i = c; i < c + 16; i++)
		{
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

    if (Settings.DSP1Master)
    {
		for (c = 0; c < 0x100; c++)
		{
			Map [c + 0xe00] = (uint8 *) MAP_DSP;
			MemorySpeed [c + 0xe00] = SLOW_ONE_CYCLE;
			BlockIsROM [c + 0xe00] = FALSE;
		}
    }

    // Banks 7e->7f, RAM
    for (c = 0; c < 16; c++)
    {
		Map [c + 0x7e0] = RAM;
		Map [c + 0x7f0] = RAM + 0x10000;
		BlockIsRAM [c + 0x7e0] = TRUE;
		BlockIsRAM [c + 0x7f0] = TRUE;
		BlockIsROM [c + 0x7e0] = FALSE;
		BlockIsROM [c + 0x7f0] = FALSE;
    }

    // Banks 60->67, S-RAM
    for (c = 0; c < 0x80; c++)
    {
		Map [c + 0x600] = (uint8 *) MAP_LOROM_SRAM;
		BlockIsRAM [c + 0x600] = TRUE;
		BlockIsROM [c + 0x600] = FALSE;
    }

    WriteProtectROM ();
}

/* 0x00155a44 */
void CMemory::SRAM512KLoROMMap ()
{
    int c;
    int i;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
		Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_NONE;
		Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_NONE;

		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [c << 11] - 0x8000;
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 8; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 11) + 0x200000];

		for (i = c + 8; i < c + 16; i++)
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 11) + 0x200000 - 0x8000];

		for (i = c; i < c + 16; i++)
		{
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

    MapExtraRAM ();
    WriteProtectROM ();
}

/* 0x00155ca0 */
void CMemory::BSHiROMMap ()
{
    int c;
    int i;

	SRAMSize=5;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		// XXX: How large is SRAM??
				Map [c + 5] = Map [c + 0x805] = (uint8 *) RAM;
//		Map [c + 5] = Map [c + 0x805] = (uint8 *) SRAM;
		BlockIsRAM [c + 5] = BlockIsRAM [c + 0x805] = TRUE;
//		Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_NONE;
//		Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_NONE;

						Map [c + 6] = Map [c + 0x806] = (uint8 *) RAM;
//		Map [c + 5] = Map [c + 0x805] = (uint8 *) SRAM;
BlockIsRAM [c + 6] = BlockIsRAM [c + 0x806] = TRUE;
		Map [c + 7] = Map [c + 0x807] = (uint8 *) RAM;
//		Map [c + 5] = Map [c + 0x805] = (uint8 *) SRAM;
BlockIsRAM [c + 7] = BlockIsRAM [c + 0x807] = TRUE;

		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [(c << 12) % CalculatedSize];
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 60->7d offset 0000->7fff & 60->7f offset 8000->ffff PSRAM
    // XXX: How large is PSRAM?

	//not adjusted, but The Dumper says "4 Mbits"
    for (c = 0x600; c < 0x7e0; c += 16)
    {
		for (i = c; i < c + 8; i++)
		{
			Map [i] = &ROM [0x400000 + (c << 11)];
			BlockIsRAM [i] = TRUE;
		}
		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = &ROM [0x400000 + (c << 11) - 0x8000];
			BlockIsRAM [i] = TRUE;
		}
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 16; i++)
		{
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 12) % CalculatedSize];
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }
	for(i=0;i<0x80;i++)
	{
		Map[0x700+i]=&BSRAM[0x10000*(i/16)];
		BlockIsRAM[0x700+i]=TRUE;
		BlockIsROM[0x700+i]=FALSE;
		MemorySpeed[0x700+i]=SLOW_ONE_CYCLE;
	}
	for (i=0; i<8;i++)
	{
		Map[0x205+(i<<4)]=Map[0x285+(i<<4)]=Map[0x305+(i<<4)]=Map[0x385+(i<<4)]=Map[0x705+(i<<4)];
		BlockIsRAM[0x205+(i<<4)]=BlockIsRAM[0x285+(i<<4)]=BlockIsRAM[0x305+(i<<4)]=BlockIsRAM[0x385+(i<<4)]=TRUE;
		BlockIsROM[0x205+(i<<4)]=BlockIsROM[0x285+(i<<4)]=BlockIsROM[0x305+(i<<4)]=BlockIsROM[0x385+(i<<4)]=FALSE;
		MemorySpeed[0x205+(i<<4)]=MemorySpeed[0x285+(i<<4)]=MemorySpeed[0x305+(i<<4)]=MemorySpeed[0x385+(i<<4)]=SLOW_ONE_CYCLE;
	}

    MapRAM ();
    WriteProtectROM ();
}

/* 0x00156118 */
void CMemory::JumboLoROMMap ()
{
    int c;
    int i;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;
		if (Settings.DSP1Master)
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_DSP;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_DSP;
		}
		else if (Settings.C4)
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) MAP_C4;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) MAP_C4;
		}
		else
		{
			Map [c + 6] = Map [c + 0x806] = (uint8 *) bytes0x2000 - 0x6000;
			Map [c + 7] = Map [c + 0x807] = (uint8 *) bytes0x2000 - 0x6000;
		}

		for (i = c + 8; i < c + 16; i++)
		{
			Map [i + 0x800] = &ROM [(c << 11) % 0x400000] - 0x8000;
			BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    if (Settings.DSP1Master)
    {
		// Banks 30->3f and b0->bf
		for (c = 0x300; c < 0x400; c += 16)
		{
			for (i = c + 8; i < c + 16; i++)
			{
				Map [i + 0x800] = (uint8 *) MAP_DSP;
				BlockIsROM [i] = BlockIsROM [i + 0x800] = FALSE;
			}
		}
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		//are we sure?
		for (i = c; i < c + 8; i++)
			Map [i + 0xc00] = &ROM [(c << 11) % 0x400000];

		for (i = c + 8; i < c + 16; i++)
			Map [i + 0xc00] = &ROM [((c << 11) + 0x200000) % 0x400000] - 0x8000;

		for (i = c; i < c + 16; i++)
		{
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

	//now map the rest

	for(i=0;i<0x7E0;i++)
	{
		if((i%16)>7)
		{
			int tmp=(i/16)*0x8000;
			tmp%=(CalculatedSize-0x400000);
			Map[i]=&ROM[tmp+0x400000]-0x8000;
			BlockIsROM[i]=true;
			if(0==MemorySpeed[i])
				MemorySpeed [i] = SLOW_ONE_CYCLE;
		}
	}

    MapRAM ();
    WriteProtectROM ();
}
/* 0x00156514 */
void CMemory::SPC7110HiROMMap ()
{
    int c;
    int i;

    // Banks 00->3f and 80->bf
    for (c = 0; c < 0x400; c += 16)
    {
		Map [c + 0] = Map [c + 0x800] = RAM;
		BlockIsRAM [c + 0] = BlockIsRAM [c + 0x800] = TRUE;
		Map [c + 1] = Map [c + 0x801] = RAM;
		BlockIsRAM [c + 1] = BlockIsRAM [c + 0x801] = TRUE;

		Map [c + 2] = Map [c + 0x802] = (uint8 *) MAP_PPU;
		Map [c + 3] = Map [c + 0x803] = (uint8 *) MAP_PPU;
		Map [c + 4] = Map [c + 0x804] = (uint8 *) MAP_CPU;
		Map [c + 5] = Map [c + 0x805] = (uint8 *) MAP_CPU;

		Map [c + 6] /*= Map [c + 0x806]*/ = (uint8 *) MAP_HIROM_SRAM;
		Map [c + 7] /*= Map [c + 0x807]*/ = (uint8 *) MAP_HIROM_SRAM;
		Map [c + 0x806]=Map [c + 0x807]= (uint8 *) MAP_NONE;

		for (i = c + 8; i < c + 16; i++)
		{
			Map [i] = Map [i + 0x800] = &ROM [(c << 12) % CalculatedSize];
			BlockIsROM [i] = BlockIsROM [i + 0x800] = TRUE;
		}

		for (i = c; i < c + 16; i++)
		{
			int ppu = i & 15;

			MemorySpeed [i] =
				MemorySpeed [i + 0x800] = ppu >= 2 && ppu <= 3 ? ONE_CYCLE : SLOW_ONE_CYCLE;
		}
    }

    // Banks 30->3f and b0->bf, address ranges 6000->7fff is S-RAM.
    for (c = 0; c < 16; c++)
    {
		Map [0x306 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		Map [0x307 + (c << 4)] = (uint8 *) MAP_HIROM_SRAM;
		Map [0xb06 + (c << 4)] = (uint8 *) MAP_NONE;
		Map [0xb07 + (c << 4)] = (uint8 *) MAP_NONE;
		BlockIsRAM [0x306 + (c << 4)] = TRUE;
		BlockIsRAM [0x307 + (c << 4)] = TRUE;
		//	BlockIsRAM [0xb06 + (c << 4)] = TRUE;
		//	BlockIsRAM [0xb07 + (c << 4)] = TRUE;
    }

    // Banks 40->7f and c0->ff
    for (c = 0; c < 0x400; c += 16)
    {
		for (i = c; i < c + 16; i++)
		{
			Map [i + 0x400] = Map [i + 0xc00] = &ROM [(c << 12) % CalculatedSize];
			MemorySpeed [i + 0x400] = MemorySpeed [i + 0xc00] = SLOW_ONE_CYCLE;
			BlockIsROM [i + 0x400] = BlockIsROM [i + 0xc00] = TRUE;
		}
    }

	for (c=0;c<0x10;c++)
	{
		Map [0x500+c]=(uint8 *)MAP_SPC7110_DRAM;
		BlockIsROM [0x500+c]=TRUE;
	}

	for (c=0;c<0x100;c++)
	{
		Map [0xD00+c] = (uint8 *) MAP_SPC7110_ROM;
		Map [0xE00+c] = (uint8 *) MAP_SPC7110_ROM;
		Map [0xF00+c] = (uint8 *) MAP_SPC7110_ROM;
		BlockIsROM [0xD00+c] = BlockIsROM [0xE00+c] = BlockIsROM [0xF00+c] = TRUE;

	}
	S9xSpc7110Init();

int sum=0;
for(i=0;i<(int)CalculatedSize; i++)
{
	sum+=ROM[i];
}

if(CalculatedSize==0x300000)
	sum<<=1;
CalculatedChecksum=sum&0xFFFF;

    MapRAM ();
    WriteProtectROM ();
}
