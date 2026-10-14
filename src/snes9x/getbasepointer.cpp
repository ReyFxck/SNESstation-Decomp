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
/* Historical Snes9x 1.41-1 getset.h body, isolated for the EE target.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Compact layouts retain the offsets proved by the public target listing.
 * Original declarations are bound to existing canonical source owners.
 */
typedef unsigned char uint8;
typedef unsigned int uint32;

enum S9xHistoricalMapCode {
    MAP_PPU = 0,
    MAP_CPU = 1,
    MAP_DSP = 2,
    MAP_LOROM_SRAM = 3,
    MAP_HIROM_SRAM = 4,
    MAP_NONE = 5,
    MAP_DEBUG = 6,
    MAP_C4 = 7,
    MAP_BWRAM = 8,
    MAP_BWRAM_BITMAP = 9,
    MAP_BWRAM_BITMAP2 = 10,
    MAP_SA1RAM = 11,
    MAP_SPC7110_ROM = 12,
    MAP_SPC7110_DRAM = 13,
    MAP_RONLY_SRAM = 14,
    MAP_OBC_RAM = 15,
    MAP_SETA_DSP = 16,
    MAP_SETA_RISC = 17,
    MAP_LAST = 18
};

struct HistoricalMemory {
    uint8 *reserved_00;
    uint8 *reserved_04;
    uint8 *reserved_08;
    uint8 *SRAM;       /* +0x0c */
    uint8 *BWRAM;      /* +0x10 */
    uint8 *FillRAM;    /* +0x14 */
    uint8 *C4RAM;      /* +0x18 */
    uint8 *reserved_1c;
    uint8 *reserved_20;
    uint8 *reserved_24;
    uint8 *Map[4096];  /* +0x28 */
};

struct HistoricalSettings {
    uint8 reserved_00_64[0x65];
    uint8 SPC7110;     /* Settings + 0x65 */
};

extern HistoricalMemory Memory __asm__("DAT_0034e2b0");
extern HistoricalSettings Settings __asm__("DAT_003454e0");
extern uint8 s7r_bank50[] __asm__("DAT_00413544");
enum { MEMMAP_SHIFT = 12, MEMMAP_MASK = 0x0fff };
extern uint8 *Get7110BasePtr(uint32) __asm__("snes_p11_00182910");
extern uint8 *GetBasePointerOBC1(uint32) __asm__("snes_leaf_00158fd0");

/* 0x001ac734 */
uint8 *GetBasePointer (uint32 Address)
{
    uint8 *GetAddress = Memory.Map [(Address >> MEMMAP_SHIFT) & MEMMAP_MASK];
    if (GetAddress >= (uint8 *) MAP_LAST)
                return (GetAddress);
        if(Settings.SPC7110&&((Address&0x7FFFFF)==0x4800))
        {
                return s7r_bank50;
        }
    switch ((int) GetAddress)
    {
        case MAP_SPC7110_DRAM:
#ifdef SPC7110_DEBUG
                printf("Getting Base pointer to DRAM\n");
#endif
                {
                        return s7r_bank50;
                }
        case MAP_SPC7110_ROM:
#ifdef SPC7110_DEBUG
                printf("Getting Base pointer to SPC7110ROM\n");
#endif
                return Get7110BasePtr(Address);
    case MAP_PPU:
//just a guess, but it looks like this should match the CPU as a source.
                return (Memory.FillRAM);
//		return (Memory.FillRAM - 0x2000);
    case MAP_CPU:
//fixes Ogre Battle's green lines
                return (Memory.FillRAM);
//		return (Memory.FillRAM - 0x4000);
    case MAP_DSP:
                return (Memory.FillRAM - 0x6000);
    case MAP_SA1RAM:
    case MAP_LOROM_SRAM:
                return (Memory.SRAM);
    case MAP_BWRAM:
                return (Memory.BWRAM - 0x6000);
    case MAP_HIROM_SRAM:
                return (Memory.SRAM - 0x6000);
    case MAP_C4:
                return (Memory.C4RAM - 0x6000);
        case MAP_OBC_RAM:
                return GetBasePointerOBC1(Address);
        case MAP_SETA_DSP:
                return Memory.SRAM;
    case MAP_DEBUG:
#ifdef DEBUGGER
                printf ("GBP %06x\n", Address);
#endif

    default:
    case MAP_NONE:
#ifdef DEBUGGER
                printf ("GBP %06x\n", Address);
#endif
                return (0);
    }
}
