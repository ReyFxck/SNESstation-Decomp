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
/* Original Snes9x 1.41-1 APU reset, PS2 32-bit byte-count ABI.
 * Source archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Native entry 0x0010a934, complete 1044-byte function.
 * Reuses the existing APU state, boot ROM and cycle tables.
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
#define ZeroMemory(p,n) memset((p),0,(n))
#define APU_ENDX 0x7c
#define APU_KOFF 0x5c
#define APU_KON 0x4c
#define APU_FLG 0x6c
#define APU_MUTE 0x40
#define APU_ECHO_DISABLED 0x20
#define Zero 2
#define Negative 0x80
#define Carry 1
#define Overflow 0x40
typedef union
{
#ifdef LSB_FIRST
    struct { uint8 A, Y; } B;
#else
    struct { uint8 Y, A; } B;
#endif
    uint16 W;
} YAndA;

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


struct SAPURegisters{
    uint8  P;
    YAndA YA;
    uint8  X;
    uint8  S;
    uint16  PC;
};

extern SSettings Settings __asm__("DAT_003454e0");
extern SAPU APU __asm__("g_APU_003453b8");
extern SIAPU IAPU __asm__("DAT_00345498");
extern SAPURegisters APURegisters __asm__("DAT_003454d8");
extern uint8 spc_dump_dsp[256] __asm__("DAT_00335378");
extern uint8 APUROM[64] __asm__("DAT_003f4068");
extern int32 S9xAPUCycleLengths[256] __asm__("DAT_003f40a8");
extern int32 S9xAPUCycles[256] __asm__("g_S9xAPUCycles_003f44a8");
extern "C" {
void *memcpy(void *,const void *,uint32);
void *memmove(void *,const void *,uint32);
void *memset(void *,int,uint32);
void S9xResetSound(bool8);
void S9xSetEchoEnable(uint8);
void S9xResetAPU();
}
static inline void S9xAPUUnpackStatus()
{
    IAPU._Zero = ((APURegisters.P & Zero) == 0) | (APURegisters.P & Negative);
    IAPU._Carry = (APURegisters.P & Carry);
    IAPU._Overflow = (APURegisters.P & Overflow) >> 6;
}

void S9xResetAPU ()
{

    int i;

    Settings.APUEnabled = Settings.NextAPUEnabled;

	ZeroMemory(spc_dump_dsp, 0x100);
	ZeroMemory(IAPU.RAM, 0x100);
	memset(IAPU.RAM+0x20, 0xFF, 0x20);
	memset(IAPU.RAM+0x60, 0xFF, 0x20);
	memset(IAPU.RAM+0xA0, 0xFF, 0x20);
	memset(IAPU.RAM+0xE0, 0xFF, 0x20);

	for(i=1;i<256;i++)
	{
		memcpy(IAPU.RAM+(i<<8), IAPU.RAM, 0x100);
	}

    memcpy (IAPU.ShadowRAM, IAPU.RAM, 0x10000);
	
    ZeroMemory (IAPU.CachedSamples, 0x40000);
    ZeroMemory (APU.OutPorts, 4);
    IAPU.DirectPage = IAPU.RAM;
    memmove (&IAPU.RAM [0xffc0], APUROM, sizeof (APUROM));
    memmove (APU.ExtraRAM, APUROM, sizeof (APUROM));
    IAPU.PC = IAPU.RAM + IAPU.RAM [0xfffe] + (IAPU.RAM [0xffff] << 8);
    APU.Cycles = 0;
    APURegisters.YA.W = 0;
    APURegisters.X = 0;
    APURegisters.S = 0xff;
    APURegisters.P = 0;
    S9xAPUUnpackStatus ();
    APURegisters.PC = 0;
    IAPU.APUExecuting = Settings.APUEnabled;
#ifdef SPC700_SHUTDOWN
    IAPU.WaitAddress1 = NULL;
    IAPU.WaitAddress2 = NULL;
    IAPU.WaitCounter = 0;
#endif
    APU.ShowROM = TRUE;
    IAPU.RAM [0xf1] = 0x80;
		
    for (i = 0; i < 3; i++)
    {
		APU.TimerEnabled [i] = FALSE;
		APU.TimerValueWritten [i] = 0;
		APU.TimerTarget [i] = 0;
		APU.Timer [i] = 0;
    }
    for (int j = 0; j < 0x80; j++)
		APU.DSP [j] = 0;
	
    IAPU.TwoCycles = IAPU.OneCycle * 2;
	
    for (i = 0; i < 256; i++)
		S9xAPUCycles [i] = S9xAPUCycleLengths [i] * IAPU.OneCycle;
	
    APU.DSP [APU_ENDX] = 0;
    APU.DSP [APU_KOFF] = 0;
    APU.DSP [APU_KON] = 0;
    APU.DSP [APU_FLG] = APU_MUTE | APU_ECHO_DISABLED;
    APU.KeyedChannels = 0;
	
    S9xResetSound (TRUE);
    S9xSetEchoEnable (0);
}
