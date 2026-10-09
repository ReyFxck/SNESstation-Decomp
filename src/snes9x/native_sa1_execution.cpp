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

/* Original Snes9x 1.41-1 SA-1 execution loop and interrupt entry.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * 0x0016ded4 _Z16S9xSA1Opcode_IRQv (296 bytes).
 * 0x0016efa0 S9xSA1MainLoop (256 bytes).
 * Reuses the original SA-1/Memory/register/OpenBus providers. No state or
 * opcode tables are allocated. All 552 bytes match linked historical source;
 * frozen normalized witnesses are retained without a new private-image claim.
 */
typedef unsigned char uint8;
typedef unsigned char bool8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef int int32;
typedef signed long long int64;
#define FALSE 0
#define MEMMAP_NUM_BLOCKS 4096
#define ROM_NAME_LEN 23
#define MEMMAP_MAX_SDD1_LOGGED_ENTRIES (0x10000/8)
#define _MAX_PATH 1024
#define IRQ_PENDING_FLAG (1 << 11)
#define IRQ 4
#define Decimal 8
#define Negative 128
#define Carry 1
#define Zero 2
#define Overflow 64
#define Emulation 256
#define PL P.B.l
#define CPU SA1
#define ICPU SA1
#define Registers SA1Registers
#define S9xSetByte S9xSA1SetByte
#define S9xPackStatus S9xSA1PackStatus
#define S9xOpcode_IRQ S9xSA1Opcode_IRQ
#define SA1_OPCODES
#undef VAR_CYCLES
#define SetIRQ() (Registers.PL |= IRQ)
#define ClearDecimal() (Registers.PL &= ~Decimal)
#define CheckEmulation() (Registers.P.W & Emulation)
#define SA1CheckFlag(f) (SA1Registers.PL & (f))
typedef union
{
#ifdef LSB_FIRST
    struct { uint8 l,h; } B;
#else
    struct { uint8 h,l; } B;
#endif
    uint16 W;
} pair;
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

struct SOpcodes { void (*S9xOpcode)(); };
struct SSA1Registers {
    uint8   PB;
    uint8   DB;
    pair    P;
    pair    A;
    pair    D;
    pair    S;
    pair    X;
    pair    Y;
    uint16  PC;
};

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

extern CMemory Memory __asm__("DAT_0034e2b0");
extern SSA1 SA1 __asm__("g_SA1_blob");
extern SSA1Registers SA1Registers __asm__("DAT_00345ae8");
extern uint8 OpenBus __asm__("g_OpenBus_byte");
extern "C" {
void S9xSA1SetByte(uint8, uint32);
void S9xSA1SetPCBase(uint32);
void S9xSA1MainLoop();
}
typedef char NativeSA1StateSize[(sizeof(SSA1)==32856)?1:-1];
typedef char NativeSA1RegisterSize[(sizeof(SSA1Registers)==16)?1:-1];
typedef char NativeSA1MemorySize[(sizeof(CMemory)==54404)?1:-1];
typedef char NativeSA1OpcodeSize[(sizeof(SOpcodes)==4)?1:-1];

static inline void S9xSA1PackStatus()
{
    SA1Registers.PL &= ~(Zero | Negative | Carry | Overflow);
    SA1Registers.PL |= SA1._Carry | ((SA1._Zero == 0) << 1) |
		       (SA1._Negative & 0x80) | (SA1._Overflow << 6);
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

void S9xSA1MainLoop ()
{
    int i;

#if 0
    if (SA1.Flags & NMI_FLAG)
    {
	SA1.Flags &= ~NMI_FLAG;
	if (SA1.WaitingForInterrupt)
	{
	    SA1.WaitingForInterrupt = FALSE;
	    SA1.PC++;
	}
	S9xSA1Opcode_NMI ();
    }
#endif
    if (SA1.Flags & IRQ_PENDING_FLAG)
    {
	if (SA1.IRQActive)
	{
	    if (SA1.WaitingForInterrupt)
	    {
		SA1.WaitingForInterrupt = FALSE;
		SA1.PC++;
	    }
	    if (!SA1CheckFlag (IRQ))
		S9xSA1Opcode_IRQ ();
	}
	else
	    SA1.Flags &= ~IRQ_PENDING_FLAG;
    }
#ifdef DEBUGGER
    if (SA1.Flags & TRACE_FLAG)
    {
	for (i = 0; i < 3 && SA1.Executing; i++)
	{
	    S9xSA1Trace ();
#ifdef CPU_SHUTDOWN
	    SA1.PCAtOpcodeStart = SA1.PC;
#endif
	    (*SA1.S9xOpcodes [*SA1.PC++].S9xOpcode) ();
	}
    }
    else
#endif
    for (i = 0; i < 3 && SA1.Executing; i++)
    {
#ifdef CPU_SHUTDOWN
	SA1.PCAtOpcodeStart = SA1.PC;
#endif
	(*SA1.S9xOpcodes [*SA1.PC++].S9xOpcode) ();
    }
}
