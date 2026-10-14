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

/* Original Snes9x 1.41-1 native general DMA.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * S9xDoDMA: 0x00129af4 (2316 bytes) + 0x0012a400 (4072 bytes).
 * Local OAM/CGRAM helpers: 0x0012ba5c (748) and 0x0012bd48 (532).
 * Native shared CPU/PPU/APU/SA1/SPC7110 state uses existing providers.
 * All 7668 instruction bytes match linked original historical source;
 * 6920 bytes also match complete previously frozen target-partition hashes.
 * The native allocation ABI is preserved rather than bound to context models.
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


struct HDMA
{
    uint8 used;
    uint8 bbus_address;
    uint8 abus_bank;
    uint16 abus_address;
    uint8 indirect_address;
    uint8 force_table_address_write;
    uint8 force_table_address_read;
    uint8 line_count_write;
    uint8 line_count_read;
};


struct Missing
{
    uint8 emulate6502;
    uint8 decimal_mode;
    uint8 mv_8bit_index;
    uint8 mv_8bit_acc;
    uint8 interlace;
    uint8 lines_239;
    uint8 pseudo_512;
    struct HDMA hdma [8];
    uint8 modes [8];
    uint8 mode7_fx;
    uint8 mode7_flip;
    uint8 mode7_bgmode;
    uint8 direct;
    uint8 matrix_multiply;
    uint8 oam_read;
    uint8 vram_read;
    uint8 cgram_read;
    uint8 wram_read;
    uint8 dma_read;
    uint8 vram_inc;
    uint8 vram_full_graphic_inc;
    uint8 virq;
    uint8 hirq;
    uint16 virq_pos;
    uint16 hirq_pos;
    uint8 h_v_latch;
    uint8 h_counter_read;
    uint8 v_counter_read;
    uint8 fast_rom;
    uint8 window1 [6];
    uint8 window2 [6];
    uint8 sprite_priority_rotation;
    uint8 subscreen;
    uint8 subscreen_add;
    uint8 subscreen_sub;
    uint8 fixed_colour_add;
    uint8 fixed_colour_sub;
    uint8 mosaic;
    uint8 sprite_double_height;
    uint8 dma_channels;
    uint8 dma_this_frame;
    uint8 oam_address_read;
    uint8 bg_offset_read;
    uint8 matrix_read;
    uint8 hdma_channels;
    uint8 hdma_this_frame;
    uint16 unknownppu_read;
    uint16 unknownppu_write;
    uint16 unknowncpu_read;
    uint16 unknowncpu_write;
    uint16 unknowndsp_read;
    uint16 unknowndsp_write;
};

struct SDMA {
    bool8  TransferDirection;
    bool8  AAddressFixed;
    bool8  AAddressDecrement;
    uint8  TransferMode;

    uint8  ABank;
    uint16 AAddress;
    uint16 Address;
    uint8  BAddress;

    // General DMA only:
    uint16 TransferBytes;

    // H-DMA only:
    bool8  HDMAIndirectAddressing;
    uint16 IndirectAddress;
    uint8  IndirectBank;
    uint8  Repeat;
    uint8  LineCount;
    uint8  FirstLine;
};

struct SSNESGameFixes
{
    uint8 NeedInit0x2137;
  /*   uint8 umiharakawaseFix; */
    uint8 alienVSpredetorFix;
    uint8 APU_OutPorts_ReturnValueFix;
  /*  uint8 Old_Read0x4200; */
  /*  uint8 _0x213E_ReturnValue; */
  /*  uint8 TouhaidenControllerFix; */
    uint8 SoundEnvelopeHeightReading2;
    uint8 SRAMInitialValue;
	uint8 Uniracers;
	uint8 Flintstones;
};

#define MEMMAP_NUM_BLOCKS 4096
#define MEMMAP_SHIFT 12
#define MEMMAP_MASK 0xfff
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

struct SIAPU
{
    uint8  *PC;
    uint8  *RAM;
    uint8  *DirectPage;
    bool8  APUExecuting;
    uint8  Bit;
    uint32 Address;
    uint8  *WaitAddress1;
    uint8  *WaitAddress2;
    uint32 WaitCounter;
    uint8  *ShadowRAM;
    uint8  *CachedSamples;
    uint8  _Carry;
    uint8  _Zero;
    uint8  _Overflow;
    uint32 TimerErrorCounter;
    uint32 Scanline;
    int32  OneCycle;
    int32  TwoCycles;
};


struct SAPU
{
    int32  Cycles;
    bool8  ShowROM;
    uint8  Flags;
    uint8  KeyedChannels;
    uint8  OutPorts [4];
    uint8  DSP [0x80];
    uint8  ExtraRAM [64];
    uint16 Timer [3];
    uint16 TimerTarget [3];
    bool8  TimerEnabled [3];
    bool8  TimerValueWritten [3];
};



#define STATIC static
#define BUILD_PIXEL(R,G,B) (((int)(B)<<10)|((int)(G)<<5)|(int)(R))
#define SNES_HCOUNTER_MAX 342
#define SNES_HEIGHT_EXTENDED 239
#define FIRST_VISIBLE_LINE 1
#define PPU_H_BEAM_IRQ_SOURCE 1
#define PPU_V_BEAM_IRQ_SOURCE 2
#define GSU_IRQ_SOURCE 4
#define MAX_5C77_VERSION 1
#define MAX_5C78_VERSION 3
#define MAX_5A22_VERSION 2
#define NMI_FLAG (1<<7)
#define IRQ_PENDING_FLAG (1<<11)
#define ONE_CYCLE 6
#define SLOW_ONE_CYCLE 8
#define TILE_2BIT 0
#define TILE_4BIT 1
#define TILE_8BIT 2
#define HBLANK_START_EVENT 0
#define HBLANK_END_EVENT 1
#define HTIMER_BEFORE_EVENT 2
#define HTIMER_AFTER_EVENT 3
#define FLG_G (1<<5)
#define GSU_SFR 0x30
#define CLIP_OR 0
#define CLIP_AND 1
#define CLIP_XOR 2
#define CLIP_XNOR 3
#define SNES_MAX_NTSC_VCOUNTER 262
#define SNES_MAX_PAL_VCOUNTER 312
#define SNES_MULTIPLAYER5 0
#define SNES_MOUSE_SWAPPED 2
#define SNES_MOUSE 3
#define SNES_JUSTIFIER 5
#define SNES_JUSTIFIER_2 6

extern SSettings Settings __asm__("DAT_003454e0");
extern SCPUState CPU __asm__("g_CPU_blob");
extern SPPU PPU __asm__("DAT_0035b788");
extern InternalPPU IPPU __asm__("DAT_0035c268");
extern CMemory Memory __asm__("DAT_0034e2b0");
extern Missing missing __asm__("DAT_00345268");
extern SDMA DMA[8] __asm__("DAT_0035d360");
extern SSNESGameFixes SNESGameFixes __asm__("DAT_0035b738");
extern SIAPU IAPU __asm__("DAT_00345498");
extern SAPU APU __asm__("g_APU_003453b8");
extern uint8 OpenBus __asm__("g_OpenBus_byte");
extern uint16 SignExtend[2] __asm__("DAT_003f2eb0");
extern uint8 GetBank __asm__("DAT_0035fda0");
extern uint32 justifiers __asm__("DAT_003f4bf0");
extern uint8 in_bit __asm__("DAT_003f4bf4");
extern uint8 *HDMAMemPointers[8] __asm__("DAT_0035d410");
extern "C" {
    int rand();
    void S9xSetPPU(uint8,uint16);
    uint8 S9xGetPPU(uint16);
    void S9xSetCPU(uint8,uint16);
    uint8 S9xGetCPU(uint16);
    void S9xUpdateScreen();
    void S9xFixColourBrightness();
    void S9xSuperFXExec();
    void S9xSetIRQ(uint32);
    void S9xDoDMA(uint8);
    void S9xSetSA1(uint8,uint32);
    uint8 S9xGetSA1(uint32);

    uint8 S9xGetSPC7110(uint16);
}
void S9xSetSPC7110(uint8,uint16);
void S9xSetSRTC(uint8,uint16);
uint8 S9xGetSRTC(uint16);
void S9xSetSDD1MemoryMap(uint32,uint32);
void FxFlushCache();
void FxCacheWriteAccess(uint16);
void S9xUpdateHTimer();
typedef signed short int16;
typedef signed long long int64;
struct SOpcodes { void (*S9xOpcode)(); };
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

#define DECOMP_BUFFER_SIZE 0x10000
typedef struct SPC7110EmuVars
{
	unsigned char reg4800;
	unsigned char reg4801;
	unsigned char reg4802;
	unsigned char reg4803;
	unsigned char reg4804;
	unsigned char reg4805;
	unsigned char reg4806;
	unsigned char reg4807;
	unsigned char reg4808;
	unsigned char reg4809;
	unsigned char reg480A;
	unsigned char reg480B;
	unsigned char reg480C;
	unsigned char reg4811;
	unsigned char reg4812;
	unsigned char reg4813;
	unsigned char reg4814;
	unsigned char reg4815;
	unsigned char reg4816;
	unsigned char reg4817;
	unsigned char reg4818;
	unsigned char reg4820;
	unsigned char reg4821;
	unsigned char reg4822;
	unsigned char reg4823;
	unsigned char reg4824;
	unsigned char reg4825;
	unsigned char reg4826;
	unsigned char reg4827;
	unsigned char reg4828;
	unsigned char reg4829;
	unsigned char reg482A;
	unsigned char reg482B;
	unsigned char reg482C;
	unsigned char reg482D;
	unsigned char reg482E;
	unsigned char reg482F;
	unsigned char reg4830;
	unsigned char reg4831;
	unsigned char reg4832;
	unsigned char reg4833;
	unsigned char reg4834;
	unsigned char reg4840;
	unsigned char reg4841;
	unsigned char reg4842;
	uint8 AlignBy;
	uint8 written;
	uint8 offset_add;
	uint32 DataRomOffset;
	uint32 DataRomSize;
	uint32 bank50Internal;
	uint8 bank50[DECOMP_BUFFER_SIZE];

} SPC7110Regs;

extern SSA1 SA1 __asm__("g_SA1_blob");
extern SPC7110Regs s7r __asm__("g_s7r_blob");
extern int32 S9xAPUCycles[256] __asm__("g_S9xAPUCycles_003f44a8");
extern void (*S9xApuOpcodes[256])() __asm__("g_S9xApuOpcodes_00411010");
extern "C" void S9xDoHBlankProcessing();
extern "C" void *memcpy(void *,const void *,unsigned int);
extern uint8 *S9xGetBasePointer(uint32);
extern uint8 *GetBasePointer(uint32);
extern uint8 S9xGetByte(uint32);
extern void S9xSetByte(uint8,uint32);
#define CHECK_SOUND()
typedef char NativeDMASettingsSize[(sizeof(SSettings)==328)?1:-1];
typedef char NativeDMACPUSize[(sizeof(SCPUState)==104)?1:-1];
typedef char NativeDMAPPUSize[(sizeof(SPPU)==2780)?1:-1];
typedef char NativeDMAIPPUSize[(sizeof(InternalPPU)==4340)?1:-1];
typedef char NativeDMAAPUSize[(sizeof(SAPU)==224)?1:-1];
typedef char NativeDMAIAPUSize[(sizeof(SIAPU)==60)?1:-1];
typedef char NativeDMAChannelSize[(sizeof(SDMA)==22)?1:-1];
typedef char NativeDMAMemorySize[(sizeof(CMemory)==54404)?1:-1];
typedef char NativeDMASA1Size[(sizeof(SSA1)==32856)?1:-1];
typedef char NativeDMASPC7110Size[(sizeof(SPC7110Regs)==65596)?1:-1];

STATIC inline uint8 REGISTER_4212()
{
    GetBank = 0;
    if (CPU.V_Counter >= PPU.ScreenHeight + FIRST_VISIBLE_LINE &&
	CPU.V_Counter < PPU.ScreenHeight + FIRST_VISIBLE_LINE + 3)
	GetBank = 1;

    GetBank |= CPU.Cycles >= Settings.HBlankStart ? 0x40 : 0;
    if (CPU.V_Counter >= PPU.ScreenHeight + FIRST_VISIBLE_LINE)
	GetBank |= 0x80; /* XXX: 0x80 or 0xc0 ? */

    return (GetBank);
}

STATIC inline void FLUSH_REDRAW ()
{
    if (IPPU.PreviousLine != IPPU.CurrentLine)
	S9xUpdateScreen ();
}

STATIC inline void REGISTER_2104 (uint8 byte)
{
    if (PPU.OAMAddr & 0x100)
    {
        int addr = ((PPU.OAMAddr & 0x10f) << 1) + (PPU.OAMFlip & 1);
        if (byte != PPU.OAMData [addr]){
            FLUSH_REDRAW ();
            PPU.OAMData [addr] = byte;
            IPPU.OBJChanged = TRUE;

            // X position high bit, and sprite size (x4)
            struct SOBJ *pObj = &PPU.OBJ [(addr & 0x1f) * 4];

            pObj->HPos = (pObj->HPos & 0xFF) | SignExtend[(byte >> 0) & 1];
            pObj++->Size = byte & 2;
            pObj->HPos = (pObj->HPos & 0xFF) | SignExtend[(byte >> 2) & 1];
            pObj++->Size = byte & 8;
            pObj->HPos = (pObj->HPos & 0xFF) | SignExtend[(byte >> 4) & 1];
            pObj++->Size = byte & 32;
            pObj->HPos = (pObj->HPos & 0xFF) | SignExtend[(byte >> 6) & 1];
            pObj->Size = byte & 128;
        }
        PPU.OAMFlip ^= 1;
        if(!(PPU.OAMFlip & 1)){
            ++PPU.OAMAddr;
            PPU.OAMAddr &= 0x1ff;
        }
    } else if(!(PPU.OAMFlip & 1)){
        PPU.OAMWriteRegister &= 0xff00;
        PPU.OAMWriteRegister |= byte;
        PPU.OAMFlip |= 1;
    } else {
        PPU.OAMWriteRegister &= 0x00ff;
        uint8 lowbyte = (uint8)(PPU.OAMWriteRegister);
        uint8 highbyte = byte;
        PPU.OAMWriteRegister |= byte << 8;

        int addr = (PPU.OAMAddr << 1);

        if (lowbyte != PPU.OAMData [addr] ||
            highbyte != PPU.OAMData [addr+1])
        {
            FLUSH_REDRAW ();
            PPU.OAMData [addr] = lowbyte;
            PPU.OAMData [addr+1] = highbyte;
            IPPU.OBJChanged = TRUE;
            if (addr & 2)
            {
                // Tile
                PPU.OBJ[addr = PPU.OAMAddr >> 1].Name = PPU.OAMWriteRegister & 0x1ff;

                // priority, h and v flip.
                PPU.OBJ[addr].Palette = (highbyte >> 1) & 7;
                PPU.OBJ[addr].Priority = (highbyte >> 4) & 3;
                PPU.OBJ[addr].HFlip = (highbyte >> 6) & 1;
                PPU.OBJ[addr].VFlip = (highbyte >> 7) & 1;
            }
            else
            {
                // X position (low)
                PPU.OBJ[addr = PPU.OAMAddr >> 1].HPos &= 0xFF00;
                PPU.OBJ[addr].HPos |= lowbyte;

                // Sprite Y position
                PPU.OBJ[addr].VPos = highbyte;
            }
        }
        PPU.OAMFlip &= ~1;
        ++PPU.OAMAddr;
    }

    Memory.FillRAM [0x2104] = byte;
}

STATIC inline void REGISTER_2118 (uint8 Byte)
{
    uint32 address;
    if (PPU.VMA.FullGraphicCount)
    {
	uint32 rem = PPU.VMA.Address & PPU.VMA.Mask1;
	address = (((PPU.VMA.Address & ~PPU.VMA.Mask1) +
			 (rem >> PPU.VMA.Shift) +
			 ((rem & (PPU.VMA.FullGraphicCount - 1)) << 3)) << 1) & 0xffff;
	Memory.VRAM [address] = Byte;
    }
    else
    {
	Memory.VRAM[address = (PPU.VMA.Address << 1) & 0xFFFF] = Byte;
    }
    IPPU.TileCached [TILE_2BIT][address >> 4] = FALSE;
    IPPU.TileCached [TILE_4BIT][address >> 5] = FALSE;
    IPPU.TileCached [TILE_8BIT][address >> 6] = FALSE;
    if (!PPU.VMA.High)
    {
#ifdef DEBUGGER
	if (Settings.TraceVRAM && !CPU.InDMA)
	{
	    printf ("VRAM write byte: $%04X (%d,%d)\n", PPU.VMA.Address,
		    Memory.FillRAM[0x2115] & 3,
		    (Memory.FillRAM [0x2115] & 0x0c) >> 2);
	}
#endif	
	PPU.VMA.Address += PPU.VMA.Increment;
    }
//    Memory.FillRAM [0x2118] = Byte;
}

STATIC inline void REGISTER_2118_tile (uint8 Byte)
{
    uint32 address;
    uint32 rem = PPU.VMA.Address & PPU.VMA.Mask1;
    address = (((PPU.VMA.Address & ~PPU.VMA.Mask1) +
		 (rem >> PPU.VMA.Shift) +
		 ((rem & (PPU.VMA.FullGraphicCount - 1)) << 3)) << 1) & 0xffff;
    Memory.VRAM [address] = Byte;
    IPPU.TileCached [TILE_2BIT][address >> 4] = FALSE;
    IPPU.TileCached [TILE_4BIT][address >> 5] = FALSE;
    IPPU.TileCached [TILE_8BIT][address >> 6] = FALSE;
    if (!PPU.VMA.High)
	PPU.VMA.Address += PPU.VMA.Increment;
//    Memory.FillRAM [0x2118] = Byte;
}

STATIC inline void REGISTER_2118_linear (uint8 Byte)
{
    uint32 address;
    Memory.VRAM[address = (PPU.VMA.Address << 1) & 0xFFFF] = Byte;
    IPPU.TileCached [TILE_2BIT][address >> 4] = FALSE;
    IPPU.TileCached [TILE_4BIT][address >> 5] = FALSE;
    IPPU.TileCached [TILE_8BIT][address >> 6] = FALSE;
    if (!PPU.VMA.High)
	PPU.VMA.Address += PPU.VMA.Increment;
//    Memory.FillRAM [0x2118] = Byte;
}

STATIC inline void REGISTER_2119 (uint8 Byte)
{
    uint32 address;
    if (PPU.VMA.FullGraphicCount)
    {
	uint32 rem = PPU.VMA.Address & PPU.VMA.Mask1;
	address = ((((PPU.VMA.Address & ~PPU.VMA.Mask1) +
		    (rem >> PPU.VMA.Shift) +
		    ((rem & (PPU.VMA.FullGraphicCount - 1)) << 3)) << 1) + 1) & 0xFFFF;
	Memory.VRAM [address] = Byte;
    }
    else
    {
	Memory.VRAM[address = ((PPU.VMA.Address << 1) + 1) & 0xFFFF] = Byte;
    }
    IPPU.TileCached [TILE_2BIT][address >> 4] = FALSE;
    IPPU.TileCached [TILE_4BIT][address >> 5] = FALSE;
    IPPU.TileCached [TILE_8BIT][address >> 6] = FALSE;
    if (PPU.VMA.High)
    {
#ifdef DEBUGGER
	if (Settings.TraceVRAM && !CPU.InDMA)
	{
	    printf ("VRAM write word: $%04X (%d,%d)\n", PPU.VMA.Address,
		    Memory.FillRAM[0x2115] & 3,
		    (Memory.FillRAM [0x2115] & 0x0c) >> 2);
	}
#endif	
	PPU.VMA.Address += PPU.VMA.Increment;
    }
//    Memory.FillRAM [0x2119] = Byte;
}

STATIC inline void REGISTER_2119_tile (uint8 Byte)
{
    uint32 rem = PPU.VMA.Address & PPU.VMA.Mask1;
    uint32 address = ((((PPU.VMA.Address & ~PPU.VMA.Mask1) +
		    (rem >> PPU.VMA.Shift) +
		    ((rem & (PPU.VMA.FullGraphicCount - 1)) << 3)) << 1) + 1) & 0xFFFF;
    Memory.VRAM [address] = Byte;
    IPPU.TileCached [TILE_2BIT][address >> 4] = FALSE;
    IPPU.TileCached [TILE_4BIT][address >> 5] = FALSE;
    IPPU.TileCached [TILE_8BIT][address >> 6] = FALSE;
    if (PPU.VMA.High)
	PPU.VMA.Address += PPU.VMA.Increment;
//    Memory.FillRAM [0x2119] = Byte;
}

STATIC inline void REGISTER_2119_linear (uint8 Byte)
{
    uint32 address;
    Memory.VRAM[address = ((PPU.VMA.Address << 1) + 1) & 0xFFFF] = Byte;
    IPPU.TileCached [TILE_2BIT][address >> 4] = FALSE;
    IPPU.TileCached [TILE_4BIT][address >> 5] = FALSE;
    IPPU.TileCached [TILE_8BIT][address >> 6] = FALSE;
    if (PPU.VMA.High)
	PPU.VMA.Address += PPU.VMA.Increment;
//    Memory.FillRAM [0x2119] = Byte;
}

STATIC inline void REGISTER_2122(uint8 Byte)
{
    // CG-RAM (palette) write

    if (PPU.CGFLIP)
    {
	if ((Byte & 0x7f) != (PPU.CGDATA[PPU.CGADD] >> 8))
	{
	    if (Settings.SixteenBit)
		FLUSH_REDRAW ();
	    PPU.CGDATA[PPU.CGADD] &= 0x00FF;
	    PPU.CGDATA[PPU.CGADD] |= (Byte & 0x7f) << 8;
	    IPPU.ColorsChanged = TRUE;
	    if (Settings.SixteenBit)
	    {
		IPPU.Blue [PPU.CGADD] = IPPU.XB [(Byte >> 2) & 0x1f];
		IPPU.Green [PPU.CGADD] = IPPU.XB [(PPU.CGDATA[PPU.CGADD] >> 5) & 0x1f];
		IPPU.ScreenColors [PPU.CGADD] = (uint16) BUILD_PIXEL (IPPU.Red [PPU.CGADD],
							     IPPU.Green [PPU.CGADD],
							     IPPU.Blue [PPU.CGADD]);
	    }
	}
	PPU.CGADD++;
    }
    else
    {
	if (Byte != (uint8) (PPU.CGDATA[PPU.CGADD] & 0xff))
	{
	    if (Settings.SixteenBit)
		FLUSH_REDRAW ();
	    PPU.CGDATA[PPU.CGADD] &= 0x7F00;
	    PPU.CGDATA[PPU.CGADD] |= Byte;
	    IPPU.ColorsChanged = TRUE;
	    if (Settings.SixteenBit)
	    {
		IPPU.Red [PPU.CGADD] = IPPU.XB [Byte & 0x1f];
		IPPU.Green [PPU.CGADD] = IPPU.XB [(PPU.CGDATA[PPU.CGADD] >> 5) & 0x1f];
		IPPU.ScreenColors [PPU.CGADD] = (uint16) BUILD_PIXEL (IPPU.Red [PPU.CGADD],
							     IPPU.Green [PPU.CGADD],
							     IPPU.Blue [PPU.CGADD]);
	    }
	}
    }
    PPU.CGFLIP ^= 1;
//    Memory.FillRAM [0x2122] = Byte;
}

STATIC inline void REGISTER_2180(uint8 Byte)
{
    Memory.RAM[PPU.WRAM++] = Byte;
    PPU.WRAM &= 0x1FFFF;
    Memory.FillRAM [0x2180] = Byte;
}


//Platform specific input functions used by PPU.CPP
#ifdef DEBUGGER
#define APU_EXECUTE1() \
{ \
    if (APU.Flags & TRACE_FLAG) \
	S9xTraceAPU ();\
    APU.Cycles += S9xAPUCycles [*IAPU.PC]; \
    (*S9xApuOpcodes[*IAPU.PC]) (); \
}
#else
#define APU_EXECUTE1() \
{ \
    APU.Cycles += S9xAPUCycles [*IAPU.PC]; \
    (*S9xApuOpcodes[*IAPU.PC]) (); \
}
#endif

#define APU_EXECUTE() \
if (IAPU.APUExecuting) \
{\
    while (APU.Cycles <= CPU.Cycles) \
	APU_EXECUTE1(); \
}
void S9xDoDMA (uint8 Channel)
{
    uint8 Work;
	
    if (Channel > 7 || CPU.InDMA)
		return;
	
    CPU.InDMA = TRUE;
    bool8 in_sa1_dma = FALSE;
    uint8 *in_sdd1_dma = NULL;
	uint8 *spc7110_dma=NULL;
	bool s7_wrap=false;
    SDMA *d = &DMA[Channel];
	

    int count = d->TransferBytes;
	
    if (count == 0)
		count = 0x10000;
	
    int inc = d->AAddressFixed ? 0 : (!d->AAddressDecrement ? 1 : -1);

	if((d->ABank==0x7E||d->ABank==0x7F)&&d->BAddress==0x80)
	{
		d->AAddress+= d->TransferBytes;
		goto update_address;
	}
    switch (d->BAddress)
    {
    case 0x18:
    case 0x19:
		if (IPPU.RenderThisFrame)
			FLUSH_REDRAW ();
		break;
    }
    if (Settings.SDD1)
    {
		if (d->AAddressFixed && Memory.FillRAM [0x4801] > 0)
		{
			// Hacky support for pre-decompressed S-DD1 data
			inc = !d->AAddressDecrement ? 1 : -1;
			uint32 address = (((d->ABank << 16) | d->AAddress) & 0xfffff) << 4;
			
			address |= Memory.FillRAM [0x4804 + ((d->ABank - 0xc0) >> 4)];
#if defined(__linux__) || defined (__WIN32__)
			void *ptr = bsearch (&address, Memory.SDD1Index, 
				Memory.SDD1Entries, 12, S9xCompareSDD1IndexEntries);
			if (ptr)
				in_sdd1_dma = *(uint32 *) ((uint8 *) ptr + 4) + Memory.SDD1Data;
#else
			uint8 *ptr = Memory.SDD1Index;
			
			for (uint32 e = 0; e < Memory.SDD1Entries; e++, ptr += 12)
			{
				if (address == *(uint32 *) ptr)
				{
					in_sdd1_dma = *(uint32 *) (ptr + 4) + Memory.SDD1Data;
					break;
				}
			}
#endif
			
			if (!in_sdd1_dma)
			{
				// No matching decompressed data found. Must be some new 
				// graphics not encountered before. Log it if it hasn't been
				// already.
				uint8 *p = Memory.SDD1LoggedData;
				bool8 found = FALSE;
				uint8 SDD1Bank = Memory.FillRAM [0x4804 + ((d->ABank - 0xc0) >> 4)] | 0xf0;
				
				for (uint32 i = 0; i < Memory.SDD1LoggedDataCount; i++, p += 8)
				{
					if (*p == d->ABank ||
						*(p + 1) == (d->AAddress >> 8) &&
						*(p + 2) == (d->AAddress & 0xff) &&
						*(p + 3) == (count >> 8) &&
						*(p + 4) == (count & 0xff) &&
						*(p + 7) == SDD1Bank)
					{
						found = TRUE;
						break;
					}
				}
				if (!found && Memory.SDD1LoggedDataCount < MEMMAP_MAX_SDD1_LOGGED_ENTRIES)
				{
					*p = d->ABank;
					*(p + 1) = d->AAddress >> 8;
					*(p + 2) = d->AAddress & 0xff;
					*(p + 3) = count >> 8;
					*(p + 4) = count & 0xff;
					*(p + 7) = SDD1Bank;
					Memory.SDD1LoggedDataCount += 1;
				}
			}
		}
		Memory.FillRAM [0x4801] = 0;
    }
	if(Settings.SPC7110&&(d->AAddress==0x4800||d->ABank==0x50))
	{
		uint32 i,j;
		i=(s7r.reg4805|(s7r.reg4806<<8));
#ifdef SPC7110_DEBUG
		printf("DMA Transfer of %04X bytes from %02X%02X%02X:%02X, offset of %04X, internal bank of %04X, multiplier %02X\n",d->TransferBytes,s7r.reg4803,s7r.reg4802,s7r.reg4801, s7r.reg4804,i,  s7r.bank50Internal, s7r.AlignBy);
#endif
		i*=s7r.AlignBy;
		i+=s7r.bank50Internal;
		i%=DECOMP_BUFFER_SIZE;
		j=0;
		if((i+d->TransferBytes)<DECOMP_BUFFER_SIZE)
		{
			spc7110_dma=&s7r.bank50[i];
		}
		else
		{
			spc7110_dma=new uint8[d->TransferBytes];
			j=DECOMP_BUFFER_SIZE-i;
			memcpy(spc7110_dma, &s7r.bank50[i], j);
			memcpy(&spc7110_dma[j],s7r.bank50,d->TransferBytes-j);
			s7_wrap=true;
		}
		int icount=s7r.reg4809|(s7r.reg480A<<8);
		icount-=d->TransferBytes;
		s7r.reg4809=0x00ff&icount;
		s7r.reg480A=(0xff00&icount)>>8;

		s7r.bank50Internal+=d->TransferBytes;
		s7r.bank50Internal%=DECOMP_BUFFER_SIZE;
		inc=1;
		d->AAddress-=count;
	}
    if (d->BAddress == 0x18 && SA1.in_char_dma && (d->ABank & 0xf0) == 0x40)
    {
		// Perform packed bitmap to PPU character format conversion on the
		// data before transmitting it to V-RAM via-DMA.
		int num_chars = 1 << ((Memory.FillRAM [0x2231] >> 2) & 7);
		int depth = (Memory.FillRAM [0x2231] & 3) == 0 ? 8 :
		(Memory.FillRAM [0x2231] & 3) == 1 ? 4 : 2;
		
		int bytes_per_char = 8 * depth;
		int bytes_per_line = depth * num_chars;
		int char_line_bytes = bytes_per_char * num_chars;
		uint32 addr = (d->AAddress / char_line_bytes) * char_line_bytes;
		uint8 *base = GetBasePointer ((d->ABank << 16) + addr) + addr;
		uint8 *buffer = &Memory.ROM [CMemory::MAX_ROM_SIZE - 0x10000];
		uint8 *p = buffer;
		uint32 inc = char_line_bytes - (d->AAddress % char_line_bytes);
		uint32 char_count = inc / bytes_per_char;
		
		in_sa1_dma = TRUE;
		
		//printf ("%08x,", base); fflush (stdout);
		//printf ("depth = %d, count = %d, bytes_per_char = %d, bytes_per_line = %d, num_chars = %d, char_line_bytes = %d\n",
		//depth, count, bytes_per_char, bytes_per_line, num_chars, char_line_bytes);
		int i;
		
		switch (depth)
		{
		case 2:
			for (i = 0; i < count; i += inc, base += char_line_bytes, 
				inc = char_line_bytes, char_count = num_chars)
			{
				uint8 *line = base + (num_chars - char_count) * 2;
				for (uint32 j = 0; j < char_count && p - buffer < count; 
				j++, line += 2)
				{
					uint8 *q = line;
					for (int l = 0; l < 8; l++, q += bytes_per_line)
					{
						for (int b = 0; b < 2; b++)
						{
							uint8 r = *(q + b);
							*(p + 0) = (*(p + 0) << 1) | ((r >> 0) & 1);
							*(p + 1) = (*(p + 1) << 1) | ((r >> 1) & 1);
							*(p + 0) = (*(p + 0) << 1) | ((r >> 2) & 1);
							*(p + 1) = (*(p + 1) << 1) | ((r >> 3) & 1);
							*(p + 0) = (*(p + 0) << 1) | ((r >> 4) & 1);
							*(p + 1) = (*(p + 1) << 1) | ((r >> 5) & 1);
							*(p + 0) = (*(p + 0) << 1) | ((r >> 6) & 1);
							*(p + 1) = (*(p + 1) << 1) | ((r >> 7) & 1);
						}
						p += 2;
					}
				}
			}
			break;
		case 4:
			for (i = 0; i < count; i += inc, base += char_line_bytes, 
				inc = char_line_bytes, char_count = num_chars)
			{
				uint8 *line = base + (num_chars - char_count) * 4;
				for (uint32 j = 0; j < char_count && p - buffer < count; 
				j++, line += 4)
				{
					uint8 *q = line;
					for (int l = 0; l < 8; l++, q += bytes_per_line)
					{
						for (int b = 0; b < 4; b++)
						{
							uint8 r = *(q + b);
							*(p +  0) = (*(p +  0) << 1) | ((r >> 0) & 1);
							*(p +  1) = (*(p +  1) << 1) | ((r >> 1) & 1);
							*(p + 16) = (*(p + 16) << 1) | ((r >> 2) & 1);
							*(p + 17) = (*(p + 17) << 1) | ((r >> 3) & 1);
							*(p +  0) = (*(p +  0) << 1) | ((r >> 4) & 1);
							*(p +  1) = (*(p +  1) << 1) | ((r >> 5) & 1);
							*(p + 16) = (*(p + 16) << 1) | ((r >> 6) & 1);
							*(p + 17) = (*(p + 17) << 1) | ((r >> 7) & 1);
						}
						p += 2;
					}
					p += 32 - 16;
				}
			}
			break;
		case 8:
			for (i = 0; i < count; i += inc, base += char_line_bytes, 
				inc = char_line_bytes, char_count = num_chars)
			{
				uint8 *line = base + (num_chars - char_count) * 8;
				for (uint32 j = 0; j < char_count && p - buffer < count; 
				j++, line += 8)
				{
					uint8 *q = line;
					for (int l = 0; l < 8; l++, q += bytes_per_line)
					{
						for (int b = 0; b < 8; b++)
						{
							uint8 r = *(q + b);
							*(p +  0) = (*(p +  0) << 1) | ((r >> 0) & 1);
							*(p +  1) = (*(p +  1) << 1) | ((r >> 1) & 1);
							*(p + 16) = (*(p + 16) << 1) | ((r >> 2) & 1);
							*(p + 17) = (*(p + 17) << 1) | ((r >> 3) & 1);
							*(p + 32) = (*(p + 32) << 1) | ((r >> 4) & 1);
							*(p + 33) = (*(p + 33) << 1) | ((r >> 5) & 1);
							*(p + 48) = (*(p + 48) << 1) | ((r >> 6) & 1);
							*(p + 49) = (*(p + 49) << 1) | ((r >> 7) & 1);
						}
						p += 2;
					}
					p += 64 - 16;
				}
			}
			break;
		}
    }
	
#ifdef DEBUGGER
    if (Settings.TraceDMA)
    {
		sprintf (String, "DMA[%d]: %s Mode: %d 0x%02X%04X->0x21%02X Bytes: %d (%s) V-Line:%ld",
			Channel, d->TransferDirection ? "read" : "write",
			d->TransferMode, d->ABank, d->AAddress,
			d->BAddress, d->TransferBytes,
			d->AAddressFixed ? "fixed" :
		(d->AAddressDecrement ? "dec" : "inc"),
			CPU.V_Counter);
		if (d->BAddress == 0x18 || d->BAddress == 0x19 || d->BAddress == 0x39 || d->BAddress == 0x3a)
			sprintf (String, "%s VRAM: %04X (%d,%d) %s", String,
				PPU.VMA.Address,
				PPU.VMA.Increment, PPU.VMA.FullGraphicCount,
				PPU.VMA.High ? "word" : "byte");

		else
			if (d->BAddress == 0x22 || d->BAddress == 0x3b)
			
				sprintf (String, "%s CGRAM: %02X (%x)", String, PPU.CGADD,
					PPU.CGFLIP);			
			else
				if (d->BAddress == 0x04 || d->BAddress == 0x38)
					sprintf (String, "%s OBJADDR: %04X", String, PPU.OAMAddr);
				S9xMessage (S9X_TRACE, S9X_DMA_TRACE, String);
    }
#endif
	
    if (!d->TransferDirection)
    {
#ifdef VAR_CYCLES
		//reflects extra cycle used by DMA
		CPU.Cycles += 8 * (count+1);
#else
		//needs fixing for the extra DMA cycle
		CPU.Cycles += (1+count) + ((1+count) >> 2);
#endif
		uint8 *base = GetBasePointer ((d->ABank << 16) + d->AAddress);
		uint16 p = d->AAddress;
		
		if (!base)
			base = Memory.ROM;
		
		if (in_sa1_dma)
		{
			base = &Memory.ROM [CMemory::MAX_ROM_SIZE - 0x10000];
			p = 0;
		}
		
		if (in_sdd1_dma)
		{
			base = in_sdd1_dma;
			p = 0;
		}
		if(spc7110_dma)
		{
			base=spc7110_dma;
			p = 0;
		}
		if (inc > 0)
			d->AAddress += count;
		else
			if (inc < 0)
				d->AAddress -= count;
			
			if (d->TransferMode == 0 || d->TransferMode == 2)
			{
				switch (d->BAddress)
				{
				case 0x04:
					do
					{
						Work = *(base + p);
						REGISTER_2104(Work);
						p += inc;
						CHECK_SOUND();
					} while (--count > 0);
					break;
				case 0x18:
					IPPU.FirstVRAMRead = TRUE;
					if (!PPU.VMA.FullGraphicCount)
					{
						do
						{
							Work = *(base + p);
							REGISTER_2118_linear(Work);
							p += inc;
							CHECK_SOUND();
						} while (--count > 0);
					}
					else
					{
						do
						{
							Work = *(base + p);
							REGISTER_2118_tile(Work);
							p += inc;
							CHECK_SOUND();
						} while (--count > 0);
					}
					break;
				case 0x19:
					IPPU.FirstVRAMRead = TRUE;
					if (!PPU.VMA.FullGraphicCount)
					{
						do
						{
							Work = *(base + p);
							REGISTER_2119_linear(Work);
							p += inc;
							CHECK_SOUND();
						} while (--count > 0);
					}
					else
					{
						do
						{
							Work = *(base + p);
							REGISTER_2119_tile(Work);
							p += inc;
							CHECK_SOUND();
						} while (--count > 0);
					}
					break;
				case 0x22:
					do
					{
						Work = *(base + p);
						REGISTER_2122(Work);
						p += inc;
						CHECK_SOUND();
					} while (--count > 0);
					break;
				case 0x80:
					do
					{
						Work = *(base + p);
						REGISTER_2180(Work);
						p += inc;
						CHECK_SOUND();
					} while (--count > 0);
					break;
				default:
					do
					{
						Work = *(base + p);
						S9xSetPPU (Work, 0x2100 + d->BAddress);
						p += inc;
						CHECK_SOUND();
					} while (--count > 0);
					break;
				}
			}
			else
				if (d->TransferMode == 1 || d->TransferMode == 5)
				{
					if (d->BAddress == 0x18)
					{
						// Write to V-RAM
						IPPU.FirstVRAMRead = TRUE;
						if (!PPU.VMA.FullGraphicCount)
						{
							while (count > 1)
							{
								Work = *(base + p);
								REGISTER_2118_linear(Work);
								p += inc;
								
								Work = *(base + p);
								REGISTER_2119_linear(Work);
								p += inc;
								CHECK_SOUND();
								count -= 2;
							}
							if (count == 1)
							{
								Work = *(base + p);
								REGISTER_2118_linear(Work);
								p += inc;
							}
						}
						else
						{
							while (count > 1)
							{
								Work = *(base + p);
								REGISTER_2118_tile(Work);
								p += inc;
								
								Work = *(base + p);
								REGISTER_2119_tile(Work);
								p += inc;
								CHECK_SOUND();
								count -= 2;
							}
							if (count == 1)
							{
								Work = *(base + p);
								REGISTER_2118_tile(Work);
								p += inc;
							}
						}
					}
					else
					{
						// DMA mode 1 general case
						while (count > 1)
						{
							Work = *(base + p);
							S9xSetPPU (Work, 0x2100 + d->BAddress);
							p += inc;
							
							Work = *(base + p);
							S9xSetPPU (Work, 0x2101 + d->BAddress);
							p += inc;
							CHECK_SOUND();
							count -= 2;
						}
						if (count == 1)
						{
							Work = *(base + p);
							S9xSetPPU (Work, 0x2100 + d->BAddress);
							p += inc;
						}
					}
				}
				else
					if (d->TransferMode == 3)
					{
						do
						{
							Work = *(base + p);
							S9xSetPPU (Work, 0x2100 + d->BAddress);
							p += inc;
							if (count <= 1)
								break;
							
							Work = *(base + p);
							S9xSetPPU (Work, 0x2100 + d->BAddress);
							p += inc;
							if (count <= 2)
								break;
							
							Work = *(base + p);
							S9xSetPPU (Work, 0x2101 + d->BAddress);
							p += inc;
							if (count <= 3)
								break;
							
							Work = *(base + p);
							S9xSetPPU (Work, 0x2101 + d->BAddress);
							p += inc;
							CHECK_SOUND();
							count -= 4;
						} while (count > 0);
					}
					else
						if (d->TransferMode == 4)
						{
							do
							{
								Work = *(base + p);
								S9xSetPPU (Work, 0x2100 + d->BAddress);
								p += inc;
								if (count <= 1)
									break;
								
								Work = *(base + p);
								S9xSetPPU (Work, 0x2101 + d->BAddress);
								p += inc;
								if (count <= 2)
									break;
								
								Work = *(base + p);
								S9xSetPPU (Work, 0x2102 + d->BAddress);
								p += inc;
								if (count <= 3)
									break;
								
								Work = *(base + p);
								S9xSetPPU (Work, 0x2103 + d->BAddress);
								p += inc;
								CHECK_SOUND();
								count -= 4;
							} while (count > 0);
						}
						else
						{
#ifdef DEBUGGER
							//	    if (Settings.TraceDMA)
							{
								sprintf (String, "Unknown DMA transfer mode: %d on channel %d\n",
									d->TransferMode, Channel);
								S9xMessage (S9X_TRACE, S9X_DMA_TRACE, String);
							}
#endif
						}
    }
    else
    {
		do
		{
			switch (d->TransferMode)
			{
			case 0:
			case 2:
#ifndef VAR_CYCLES
				CPU.Cycles += 1;
#endif
				Work = S9xGetPPU (0x2100 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				--count;
				break;
				
			case 1:
			case 5:
#ifndef VAR_CYCLES
				CPU.Cycles += 3;
#endif
				Work = S9xGetPPU (0x2100 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				if (!--count)
					break;
				
				Work = S9xGetPPU (0x2101 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				count--;
				break;
				
			case 3:
#ifndef VAR_CYCLES
				CPU.Cycles += 6;
#endif
				Work = S9xGetPPU (0x2100 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				if (!--count)
					break;
				
				Work = S9xGetPPU (0x2100 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				if (!--count)
					break;
				
				Work = S9xGetPPU (0x2101 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				if (!--count)
					break;
				
				Work = S9xGetPPU (0x2101 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				count--;
				break;
				
			case 4:
#ifndef VAR_CYCLES
				CPU.Cycles += 6;
#endif
				Work = S9xGetPPU (0x2100 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				if (!--count)
					break;
				
				Work = S9xGetPPU (0x2101 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				if (!--count)
					break;
				
				Work = S9xGetPPU (0x2102 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				if (!--count)
					break;
				
				Work = S9xGetPPU (0x2103 + d->BAddress);
				S9xSetByte (Work, (d->ABank << 16) + d->AAddress);
				d->AAddress += inc;
				count--;
				break;
				
			default:
#ifdef DEBUGGER
				if (1) //Settings.TraceDMA)
				{
					sprintf (String, "Unknown DMA transfer mode: %d on channel %d\n",
						d->TransferMode, Channel);
					S9xMessage (S9X_TRACE, S9X_DMA_TRACE, String);
				}
#endif
				count = 0;
				break;
			}
			CHECK_SOUND();
		} while (count);
    }
    
#ifdef SPC700_C
    IAPU.APUExecuting = Settings.APUEnabled;
    APU_EXECUTE ();
#endif
    while (CPU.Cycles > CPU.NextEvent)
		S9xDoHBlankProcessing ();

	if(Settings.SPC7110&&spc7110_dma)
	{
		if(spc7110_dma&&s7_wrap)
			delete [] spc7110_dma;
	}

update_address:
    // Super Punch-Out requires that the A-BUS address be updated after the
    // DMA transfer.
    Memory.FillRAM[0x4302 + (Channel << 4)] = (uint8) d->AAddress;
    Memory.FillRAM[0x4303 + (Channel << 4)] = d->AAddress >> 8;
	
    // Secret of the Mana requires that the DMA bytes transfer count be set to
    // zero when DMA has completed.
    Memory.FillRAM [0x4305 + (Channel << 4)] = 0;
    Memory.FillRAM [0x4306 + (Channel << 4)] = 0;
	
    DMA[Channel].IndirectAddress = 0;
    d->TransferBytes = 0;
    
    CPU.InDMA = FALSE;


}

