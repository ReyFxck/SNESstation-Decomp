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
/* Original native Snes9x 1.41-1 HDMA setup and scanline execution.
 * Entries: 0x0012b3e8 (176 bytes), 0x0012b498 (1292 bytes).
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Shared native state and 64-bit CPU cycle accounting are preserved.
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

class CMemory { public:
 uint8 *RAM,*ROM,*VRAM,*SRAM,*BWRAM,*FillRAM,*C4RAM;
};
extern SSettings Settings __asm__("DAT_003454e0");
extern SCPUState CPU __asm__("g_CPU_blob");
extern SPPU PPU __asm__("DAT_0035b788");
extern InternalPPU IPPU __asm__("DAT_0035c268");
extern CMemory Memory __asm__("DAT_0034e2b0");
extern Missing missing __asm__("DAT_00345268");
extern SDMA DMA[8] __asm__("DAT_0035d360");
extern SSNESGameFixes SNESGameFixes __asm__("DAT_0035b738");
extern uint8 *HDMAMemPointers[8] __asm__("DAT_0035d410");
extern uint8 *HDMABasePointers[8] __asm__("DAT_0035d430");
extern int HDMA_ModeByteCounts[8] __asm__("DAT_003f2eb8");
extern uint8 S9xGetByte(uint32);
extern uint16 S9xGetWord(uint32);
extern uint8 *S9xGetMemPointer(uint32);
extern "C" {
void S9xStartHDMA();
uint8 S9xDoHDMA(uint8);
void S9xSetPPU(uint8,uint16);
}
typedef char NativeSettingsSize[(sizeof(SSettings)==328)?1:-1];
typedef char NativeCPUCyclesSize[(sizeof(SCPUState)==104)?1:-1];
typedef char NativePPUSize[(sizeof(SPPU)==2780)?1:-1];
typedef char NativeIPPUSize[(sizeof(InternalPPU)==4340)?1:-1];
typedef char NativeMissingSize[(sizeof(Missing)==174)?1:-1];
typedef char NativeDMASize[(sizeof(SDMA)==22)?1:-1];
typedef char NativeGameFixesSize[(sizeof(SSNESGameFixes)==7)?1:-1];
void S9xStartHDMA ()
{
    if (Settings.DisableHDMA)
		IPPU.HDMA = 0;
    else
		missing.hdma_this_frame = IPPU.HDMA = Memory.FillRAM [0x420c];
	
    IPPU.HDMAStarted = TRUE;
	
    for (uint8 i = 0; i < 8; i++)
    {
		if (IPPU.HDMA & (1 << i))
		{
			DMA [i].LineCount = 0;
			DMA [i].FirstLine = TRUE;
			DMA [i].Address = DMA [i].AAddress;
		}
		HDMAMemPointers [i] = NULL;
    }
}

#ifdef DEBUGGER
void S9xTraceSoundDSP (const char *s, int i1 = 0, int i2 = 0, int i3 = 0,
					   int i4 = 0, int i5 = 0, int i6 = 0, int i7 = 0);
#endif


uint8 S9xDoHDMA (uint8 byte)
{
    struct SDMA *p = &DMA [0];
    
    int d = 0;
	
    for (uint8 mask = 1; mask; mask <<= 1, p++, d++)
	{
		if (byte & mask)
		{
			if (!p->LineCount)
			{
				uint8 line = S9xGetByte ((p->ABank << 16) + p->Address);
				if (line == 0x80)
				{
					p->Repeat = TRUE;
					p->LineCount = 128;
				}
				else
				{
					p->Repeat = !(line & 0x80);
					p->LineCount = line & 0x7f;
				}

				// Disable H-DMA'ing into V-RAM (register 2118) for Hook
				if (!p->LineCount || p->BAddress == 0x18)
				{
					byte &= ~mask;
					p->IndirectAddress += HDMAMemPointers [d] - HDMABasePointers [d];
					Memory.FillRAM [0x4305 + (d << 4)] = (uint8) p->IndirectAddress;
					Memory.FillRAM [0x4306 + (d << 4)] = p->IndirectAddress >> 8;
					continue;
				}

				p->Address++;
				p->FirstLine = 1;
				if (p->HDMAIndirectAddressing)
				{
					p->IndirectBank = Memory.FillRAM [0x4307 + (d << 4)];
					p->IndirectAddress = S9xGetWord ((p->ABank << 16) + p->Address);
					p->Address += 2;
				}
				else
				{
					p->IndirectBank = p->ABank;
					p->IndirectAddress = p->Address;
				}
				HDMABasePointers [d] = HDMAMemPointers [d] = 
					S9xGetMemPointer ((p->IndirectBank << 16) + p->IndirectAddress);
			}
			else
			{

				if (!HDMAMemPointers [d])
				{
					if (!p->HDMAIndirectAddressing)
					{
						p->IndirectBank = p->ABank;
						p->IndirectAddress = p->Address;
					}

					if (!(HDMABasePointers [d] = HDMAMemPointers [d] = 
								S9xGetMemPointer ((p->IndirectBank << 16) + p->IndirectAddress)))
					{
						byte &= ~mask;
						continue;
					}
					// Uncommenting the following line breaks Punchout - it starts
					// H-DMA during the frame.
					//p->FirstLine = TRUE;
				}
			}
			if (p->Repeat && !p->FirstLine)
			{
				p->LineCount--;
				continue;
			}

			if (p->BAddress == 0x04){
				if(SNESGameFixes.Uniracers){
					PPU.OAMAddr = 0x10c;
					PPU.OAMFlip=0;
				}
			}

#ifdef DEBUGGER
			if (Settings.TraceSoundDSP && p->FirstLine && 
					p->BAddress >= 0x40 && p->BAddress <= 0x43)
				S9xTraceSoundDSP ("Spooling data!!!\n");
			if (Settings.TraceHDMA && p->FirstLine)
			{
				sprintf (String, "H-DMA[%d] (%d) 0x%02X%04X->0x21%02X %s, Count: %3d, Rep: %s, V-LINE: %3ld %02X%04X",
						p-DMA, p->TransferMode, p->IndirectBank,
						p->IndirectAddress,
						p->BAddress,
						p->HDMAIndirectAddressing ? "ind" : "abs",
						p->LineCount,
						p->Repeat ? "yes" : "no ", CPU.V_Counter,
						p->ABank, p->Address);
				S9xMessage (S9X_TRACE, S9X_HDMA_TRACE, String);
			}
#endif

			switch (p->TransferMode)
			{
				case 0:
#ifndef VAR_CYCLES
					CPU.Cycles += 1;
#else
					CPU.Cycles += 8;
#endif
					S9xSetPPU (*HDMAMemPointers [d]++, 0x2100 + p->BAddress);
					break;
				case 1:
				case 5:
#ifndef VAR_CYCLES
					CPU.Cycles += 3;
#else
					CPU.Cycles += 16;
#endif
					S9xSetPPU (*(HDMAMemPointers [d] + 0), 0x2100 + p->BAddress);
					S9xSetPPU (*(HDMAMemPointers [d] + 1), 0x2101 + p->BAddress);
					HDMAMemPointers [d] += 2;
					break;
				case 2:
				case 6:
#ifndef VAR_CYCLES
					CPU.Cycles += 3;
#else
					CPU.Cycles += 16;
#endif
					S9xSetPPU (*(HDMAMemPointers [d] + 0), 0x2100 + p->BAddress);
					S9xSetPPU (*(HDMAMemPointers [d] + 1), 0x2100 + p->BAddress);
					HDMAMemPointers [d] += 2;
					break;
				case 3:
				case 7:
#ifndef VAR_CYCLES
					CPU.Cycles += 6;
#else
					CPU.Cycles += 32;
#endif
					S9xSetPPU (*(HDMAMemPointers [d] + 0), 0x2100 + p->BAddress);
					S9xSetPPU (*(HDMAMemPointers [d] + 1), 0x2100 + p->BAddress);
					S9xSetPPU (*(HDMAMemPointers [d] + 2), 0x2101 + p->BAddress);
					S9xSetPPU (*(HDMAMemPointers [d] + 3), 0x2101 + p->BAddress);
					HDMAMemPointers [d] += 4;
					break;
				case 4:
#ifndef VAR_CYCLES
					CPU.Cycles += 6;
#else
					CPU.Cycles += 32;
#endif
					S9xSetPPU (*(HDMAMemPointers [d] + 0), 0x2100 + p->BAddress);
					S9xSetPPU (*(HDMAMemPointers [d] + 1), 0x2101 + p->BAddress);
					S9xSetPPU (*(HDMAMemPointers [d] + 2), 0x2102 + p->BAddress);
					S9xSetPPU (*(HDMAMemPointers [d] + 3), 0x2103 + p->BAddress);
					HDMAMemPointers [d] += 4;
					break;
			}
			if (!p->HDMAIndirectAddressing)
				p->Address += HDMA_ModeByteCounts [p->TransferMode];
			p->IndirectAddress += HDMA_ModeByteCounts [p->TransferMode];
			p->FirstLine = FALSE;
			p->LineCount--;
		}
	}
    return (byte);
}

