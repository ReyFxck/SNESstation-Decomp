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

/* Original Snes9x 1.41-1 C4, DMA and SA1 reset implementations.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Target entries: 0x0010c300, 0x0012b9a4, 0x0015d8ec.
 * Shared native state is reused; no replacement state is allocated.
 */
typedef unsigned char uint8;
typedef unsigned char bool8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef long long int64;
#define MEMMAP_NUM_BLOCKS 4096
#define FALSE 0
#define TRUE 1
struct SOpcodes { void (*S9xOpcode)(); };
class CMemory { public:
    uint8 *RAM, *ROM, *VRAM, *SRAM, *BWRAM, *FillRAM, *C4RAM;
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
extern SDMA DMA[8] __asm__("DAT_0035d360");
extern SSA1 SA1 __asm__("g_SA1_blob");
extern "C" void *memset(void *, int, unsigned int);
extern "C" void S9xInitC4();
extern "C" void S9xResetDMA();
extern "C" void S9xSA1Init();
void S9xInitC4 ()
{
    // Stupid zsnes code, we can't do the logical thing without breaking
    // savestates
//    Memory.C4RAM = &Memory.FillRAM [0x6000];
    memset(Memory.C4RAM, 0, 0x2000);
}

void S9xResetDMA ()
{
    int d;
    for (d = 0; d < 8; d++)
    {
		DMA [d].TransferDirection = FALSE;
		DMA [d].HDMAIndirectAddressing = FALSE;
		DMA [d].AAddressFixed = TRUE;
		DMA [d].AAddressDecrement = FALSE;
		DMA [d].TransferMode = 0xff;
		DMA [d].ABank = 0xff;
		DMA [d].AAddress = 0xffff;
		DMA [d].Address = 0xffff;
		DMA [d].BAddress = 0xff;
		DMA [d].TransferBytes = 0xffff;
    }
    for (int c = 0x4300; c < 0x4380; c += 0x10)
    {
		for (d = c; d < c + 12; d++)
			Memory.FillRAM [d] = 0xff;
		
		Memory.FillRAM [c + 0xf] = 0xff;
    }
}

void S9xSA1Init ()
{
    SA1.NMIActive = FALSE;
    SA1.IRQActive = FALSE;
    SA1.WaitingForInterrupt = FALSE;
    SA1.Waiting = FALSE;
    SA1.Flags = 0;
    SA1.Executing = FALSE;
    memset (&Memory.FillRAM [0x2200], 0, 0x200);
    Memory.FillRAM [0x2200] = 0x20;
    Memory.FillRAM [0x2220] = 0x00;
    Memory.FillRAM [0x2221] = 0x01;
    Memory.FillRAM [0x2222] = 0x02;
    Memory.FillRAM [0x2223] = 0x03;
    Memory.FillRAM [0x2228] = 0xff;
    SA1.op1 = 0;
    SA1.op2 = 0;
    SA1.arithmetic_op = 0;
    SA1.sum = 0;
    SA1.overflow = FALSE;
}

