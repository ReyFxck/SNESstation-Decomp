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

/* Native main-CPU IRQ/NMI entries from Snes9x 1.41-1 CPUOPS.CPP.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Target entries: 0x00127b78 and 0x00127e00, 648 instruction bytes each.
 * Shared native state and original zero-argument C++ ABI are retained.
 * Proof: all raw bytes and a fully linked historical reference; frozen
 * normalized target witnesses. No complete private-target code digest inferred.
 */
typedef unsigned char uint8;
typedef unsigned char bool8;
typedef signed char int8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef int int32;
#define FALSE 0
#define TRUE 1
#define NULL 0
#define CHECK_SOUND()
#define STATIC static
#define FIRST_VISIBLE_LINE 1
#define MAX_5A22_VERSION 2
#define SNES_MAX_NTSC_VCOUNTER 262
#define SNES_MAX_PAL_VCOUNTER 312
#define PPU_H_BEAM_IRQ_SOURCE 1
#define PPU_V_BEAM_IRQ_SOURCE 2
#define DEBUG_MODE_FLAG	    (1 << 0)
#define TRACE_FLAG			(1 << 1)
#define SINGLE_STEP_FLAG    (1 << 2)
#define BREAK_FLAG			(1 << 3)
#define SCAN_KEYS_FLAG	    (1 << 4)
#define SAVE_SNAPSHOT_FLAG  (1 << 5)
#define DELAYED_NMI_FLAG    (1 << 6)
#define NMI_FLAG			(1 << 7)
#define PROCESS_SOUND_FLAG  (1 << 8)
#define FRAME_ADVANCE_FLAG  (1 << 9)
#define DELAYED_NMI_FLAG2   (1 << 10)
#define IRQ_PENDING_FLAG    (1 << 11)

#ifdef VAR_CYCLES
#define ONE_CYCLE 6
#define SLOW_ONE_CYCLE 8
#define TWO_CYCLES 12
#else
#define ONE_CYCLE 1
#define SLOW_ONE_CYCLE 1
#define TWO_CYCLES 2
#endif

struct SCPUState{
    uint32  Flags;
    bool8   BranchSkip;
    bool8   NMIActive;
    bool8   IRQActive;
    bool8   WaitingForInterrupt;
    bool8   InDMA;
    uint8   WhichEvent;
    uint8   *PC;
    uint8   *PCBase;
    uint8   *PCAtOpcodeStart;
    uint8   *WaitAddress;
    uint32  WaitCounter;
    long   Cycles;
    long   NextEvent;
    long   V_Counter;
    long   MemSpeed;
    long   MemSpeedx2;
    long   FastROMSpeed;
    uint32 AutoSaveTimer;
    bool8  SRAMModified;
    uint32 NMITriggerPoint;
    bool8  BRKTriggered;
    bool8  TriedInterleavedMode2;
    uint32 NMICycleCount;
    uint32 IRQCycleCount;
};

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

#define AL A.B.l
#define AH A.B.h
#define XL X.B.l
#define XH X.B.h
#define YL Y.B.l
#define YH Y.B.h
#define SL S.B.l
#define SH S.B.h
#define DL D.B.l
#define DH D.B.h
#define PL P.B.l
#define PH P.B.h

#define Carry       1
#define Zero        2
#define IRQ         4
#define Decimal     8
#define IndexFlag  16
#define MemoryFlag 32
#define Overflow   64
#define Negative  128
#define Emulation 256

#define ClearCarry() (ICPU._Carry = 0)
#define SetCarry() (ICPU._Carry = 1)
#define SetZero() (ICPU._Zero = 0)
#define ClearZero() (ICPU._Zero = 1)
#define SetIRQ() (Registers.PL |= IRQ)
#define ClearIRQ() (Registers.PL &= ~IRQ)
#define SetDecimal() (Registers.PL |= Decimal)
#define ClearDecimal() (Registers.PL &= ~Decimal)
#define SetIndex() (Registers.PL |= IndexFlag)
#define ClearIndex() (Registers.PL &= ~IndexFlag)
#define SetMemory() (Registers.PL |= MemoryFlag)
#define ClearMemory() (Registers.PL &= ~MemoryFlag)
#define SetOverflow() (ICPU._Overflow = 1)
#define ClearOverflow() (ICPU._Overflow = 0)
#define SetNegative() (ICPU._Negative = 0x80)
#define ClearNegative() (ICPU._Negative = 0)

#define CheckZero() (ICPU._Zero == 0)
#define CheckCarry() (ICPU._Carry)
#define CheckIRQ() (Registers.PL & IRQ)
#define CheckDecimal() (Registers.PL & Decimal)
#define CheckIndex() (Registers.PL & IndexFlag)
#define CheckMemory() (Registers.PL & MemoryFlag)
#define CheckOverflow() (ICPU._Overflow)
#define CheckNegative() (ICPU._Negative & 0x80)
#define CheckEmulation() (Registers.P.W & Emulation)

#define ClearFlags(f) (Registers.P.W &= ~(f))
#define SetFlags(f)   (Registers.P.W |=  (f))
#define CheckFlag(f)  (Registers.PL & (f))

typedef union
{
#ifdef LSB_FIRST
    struct { uint8 l,h; } B;
#else
    struct { uint8 h,l; } B;
#endif
    uint16 W;
} pair;

struct SRegisters{
    uint8  PB;
    uint8  DB;
    pair   P;
    pair   A;
    pair   D;
    pair   S;
    pair   X;
    pair   Y;
    uint16 PC;
};

struct SOpcodes {
#ifdef __WIN32__
	void (__cdecl *S9xOpcode)( void);
#else
	void (*S9xOpcode)( void);
#endif
};

struct SICPU
{
    uint8  *Speed;
    struct SOpcodes *S9xOpcodes;
    uint8  _Carry;
    uint8  _Zero;
    uint8  _Negative;
    uint8  _Overflow;
    bool8  CPUExecuting;
    uint32 ShiftedPB;
    uint32 ShiftedDB;
    uint32 Frame;
    uint32 Scanline;
    uint32 FrameAdvanceCount;
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

extern SCPUState CPU __asm__("g_CPU_blob");
extern SSettings Settings __asm__("DAT_003454e0");
extern SICPU ICPU __asm__("g_ICPU_00345318");
extern SRegisters Registers __asm__("DAT_003453a8");
extern CMemory Memory __asm__("DAT_0034e2b0");
extern uint8 OpenBus __asm__("g_OpenBus_byte");
uint16 S9xGetWord(uint32);
void S9xSetByte(uint8, uint32);
void S9xSetPCBase(uint32);

typedef char NativeCPUInterruptCPUSize[(sizeof(SCPUState)==104)?1:-1];
typedef char NativeCPUInterruptSettingsSize[(sizeof(SSettings)==328)?1:-1];
typedef char NativeCPUInterruptInternalSize[(sizeof(SICPU)==36)?1:-1];
typedef char NativeCPUInterruptRegisterSize[(sizeof(SRegisters)==16)?1:-1];
typedef char NativeCPUInterruptMemorySize[(sizeof(CMemory)==54404)?1:-1];

STATIC inline void S9xPackStatus()
{
    Registers.PL &= ~(Zero | Negative | Carry | Overflow);
    Registers.PL |= ICPU._Carry | ((ICPU._Zero == 0) << 1) |
		    (ICPU._Negative & 0x80) | (ICPU._Overflow << 6);
}

#define PushB(b)\
    S9xSetByte (b, Registers.S.W--);

#define PushW(w) \
    S9xSetByte ((w)>>8, Registers.S.W);\
	S9xSetByte ((w)&0xff, Registers.S.W - 1);\
    Registers.S.W -= 2;

void S9xOpcode_IRQ (void)
{
#ifdef DEBUGGER
    if (CPU.Flags & TRACE_FLAG)
	S9xTraceMessage ("*** IRQ");
#endif
    if (!CheckEmulation())
    {
	PushB (Registers.PB);
	PushW (CPU.PC - CPU.PCBase);
	S9xPackStatus ();
	PushB (Registers.PL);
	OpenBus = Registers.PL;
	ClearDecimal ();
	SetIRQ ();

	Registers.PB = 0;
	ICPU.ShiftedPB = 0;
#ifdef SA1_OPCODES
	S9xSA1SetPCBase (Memory.FillRAM [0x2207] |
			 (Memory.FillRAM [0x2208] << 8));
#else
	if (Settings.SA1 && (Memory.FillRAM [0x2209] & 0x40))
	    S9xSetPCBase (Memory.FillRAM [0x220e] |
			  (Memory.FillRAM [0x220f] << 8));
	else
	    S9xSetPCBase (S9xGetWord (0xFFEE));
#endif
#ifdef VAR_CYCLES
        CPU.Cycles += TWO_CYCLES;
#else
#ifndef SA1_OPCODES
	CPU.Cycles += 8;
#endif
#endif
    }
    else
    {
	PushW (CPU.PC - CPU.PCBase);
	S9xPackStatus ();
	PushB (Registers.PL);
	OpenBus = Registers.PL;
	ClearDecimal ();
	SetIRQ ();

	Registers.PB = 0;
	ICPU.ShiftedPB = 0;
#ifdef SA1_OPCODES
	S9xSA1SetPCBase (Memory.FillRAM [0x2207] |
			 (Memory.FillRAM [0x2208] << 8));
#else
	if (Settings.SA1 && (Memory.FillRAM [0x2209] & 0x40))
	    S9xSetPCBase (Memory.FillRAM [0x220e] |
			  (Memory.FillRAM [0x220f] << 8));
	else
	    S9xSetPCBase (S9xGetWord (0xFFFE));
#endif
#ifdef VAR_CYCLES
	CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
	CPU.Cycles += 6;
#endif
#endif
    }
}

/**********************************************************************************************/

/* NMI *************************************************************************************** */
void S9xOpcode_NMI (void)
{
#ifdef DEBUGGER
    if (CPU.Flags & TRACE_FLAG)
	S9xTraceMessage ("*** NMI");
#endif
    if (!CheckEmulation())
    {
	PushB (Registers.PB);
	PushW (CPU.PC - CPU.PCBase);
	S9xPackStatus ();
	PushB (Registers.PL);
	OpenBus = Registers.PL;
	ClearDecimal ();
	SetIRQ ();

	Registers.PB = 0;
	ICPU.ShiftedPB = 0;
#ifdef SA1_OPCODES
	S9xSA1SetPCBase (Memory.FillRAM [0x2205] |
			 (Memory.FillRAM [0x2206] << 8));
#else
	if (Settings.SA1 && (Memory.FillRAM [0x2209] & 0x20))
	    S9xSetPCBase (Memory.FillRAM [0x220c] |
			  (Memory.FillRAM [0x220d] << 8));
	else
	    S9xSetPCBase (S9xGetWord (0xFFEA));
#endif
#ifdef VAR_CYCLES
	CPU.Cycles += TWO_CYCLES;
#else
#ifndef SA1_OPCODES
	CPU.Cycles += 8;
#endif
#endif
    }
    else
    {
	PushW (CPU.PC - CPU.PCBase);
	S9xPackStatus ();
	PushB (Registers.PL);
	OpenBus = Registers.PL;
	ClearDecimal ();
	SetIRQ ();

	Registers.PB = 0;
	ICPU.ShiftedPB = 0;
#ifdef SA1_OPCODES
	S9xSA1SetPCBase (Memory.FillRAM [0x2205] |
			 (Memory.FillRAM [0x2206] << 8));
#else
	if (Settings.SA1 && (Memory.FillRAM [0x2209] & 0x20))
	    S9xSetPCBase (Memory.FillRAM [0x220c] |
			  (Memory.FillRAM [0x220d] << 8));
	else
	    S9xSetPCBase (S9xGetWord (0xFFFA));
#endif
#ifdef VAR_CYCLES
	CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
	CPU.Cycles += 6;
#endif
#endif
    }
}
/**********************************************************************************************/
