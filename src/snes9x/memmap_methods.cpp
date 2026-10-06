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

/* Historical Snes9x 1.41-1 memory-map and ROM metadata methods.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * KartContents retains the V51-proved PS2 Settings layout byte before game
 * flags (BS at 0x12d; SETA at 0x134). The original lowercase map-mode format,
 * SRAMMask-based size calculation and static return buffers are preserved.
 */
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned char bool8;
#define ROM_NAME_LEN 23
#define MEMMAP_NUM_BLOCKS 4096
#define ST_010 1
#define ST_011 2
#define ST_018 3
extern "C" int sprintf(char *, const char *, ...);
extern "C" void *memmove(void *, const void *, unsigned int);
class CMemory {
public:
    enum { MAP_HIROM_SRAM = 4, MAP_NONE = 5, MAP_RONLY_SRAM = 14 };
    void FixROMSpeed();
    void WriteProtectROM();
    void SPC7110Sram(uint8);
    const char *TVStandard();
    const char *Speed();
    const char *MapType();
    const char *StaticRAMSize();
    const char *Size();
    const char *KartContents();
    const char *MapMode();
    const char *ROMID();
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
 };
struct SCPUState { uint8 reserved[0x48]; long FastROMSpeed; };
struct SSettings {
    uint8 reserved_00[0x2d]; bool8 PAL;
    uint8 reserved_2e[0x65 - 0x2e]; bool8 SPC7110, SPC7110RTC;
    uint8 reserved_67[0x12d - 0x67]; bool8 BS;
    uint8 reserved_12e[0x134 - 0x12e]; int8 SETA;
};
extern CMemory Memory __asm__("DAT_0034e2b0");
extern SCPUState CPU __asm__("DAT_00345340");
extern SSettings Settings __asm__("DAT_003454e0");

/* 0x001535c0 */
void CMemory::FixROMSpeed ()
{
    int c;

    for (c = 0x800; c < 0x1000; c++)
    {
		if (BlockIsROM [c])
			MemorySpeed [c] = (uint8) CPU.FastROMSpeed;
    }
}

/* 0x00153608 */
void CMemory::WriteProtectROM ()
{
    memmove ((void *) WriteMap, (void *) Map, sizeof (Map));
    for (int c = 0; c < 0x1000; c++)
    {
		if (BlockIsROM [c])
			WriteMap [c] = (uint8 *) MAP_NONE;
    }
}

/* 0x00156884 */
void CMemory::SPC7110Sram(uint8 newstate)
{
	if(newstate&0x80)
	{
		Memory.Map[6]=(uint8 *)MAP_HIROM_SRAM;
		Memory.Map[7]=(uint8 *)MAP_HIROM_SRAM;
		Memory.Map[0x306]=(uint8 *)MAP_HIROM_SRAM;
		Memory.Map[0x307]=(uint8 *)MAP_HIROM_SRAM;


	}
	else
	{
		Memory.Map[6]=(uint8 *)MAP_RONLY_SRAM;
		Memory.Map[7]=(uint8 *)MAP_RONLY_SRAM;
		Memory.Map[0x306]=(uint8 *)MAP_RONLY_SRAM;
		Memory.Map[0x307]=(uint8 *)MAP_RONLY_SRAM;
	}
}

/* 0x001568c4 */
const char *CMemory::TVStandard ()
{
    return (Settings.PAL ? "PAL" : "NTSC");
}

/* 0x001568e8 */
const char *CMemory::Speed ()
{
    return (ROMSpeed & 0x10 ? "120ns" : "200ns");
}

/* 0x00156914 */
const char *CMemory::MapType ()
{
    return (HiROM ? "HiROM" : "LoROM");
}

/* 0x00156934 */
const char *CMemory::StaticRAMSize ()
{
    static char tmp [20];

    if (Memory.SRAMSize > 16)
		return ("Corrupt");
    sprintf (tmp, "%dKb", (SRAMMask + 1) / 1024);
    return (tmp);
}

/* 0x00156994 */
const char *CMemory::Size ()
{
    static char tmp [20];

    if (ROMSize < 7 || ROMSize - 7 > 23)
		return ("Corrupt");
    sprintf (tmp, "%dMbits", 1 << (ROMSize - 7));
    return (tmp);
}

/* 0x00156a04 */
const char *CMemory::KartContents ()
{
    static char tmp [30];
    static const char *CoPro [16] = {
		"DSP1", "SuperFX", "OBC1", "SA-1", "S-DD1", "S-RTC", "CoPro#6",
			"CoPro#7", "CoPro#8", "CoPro#9", "CoPro#10", "CoPro#11", "CoPro#12",
			"CoPro#13", "CoPro#14", "CoPro-Custom"
    };
    static const char *Contents [3] = {
		"ROM", "ROM+RAM", "ROM+RAM+BAT"
    };
    if (ROMType == 0&&!Settings.BS)
		return ("ROM only");

    sprintf (tmp, "%s", Contents [(ROMType & 0xf) % 3]);


	if(Settings.BS)
		sprintf (tmp, "%s+%s", tmp, "BSX");
	if(Settings.SPC7110&&Settings.SPC7110RTC)
		sprintf (tmp, "%s+%s", tmp, "SPC7110+RTC");
	else if(Settings.SPC7110)
		sprintf (tmp, "%s+%s", tmp, "SPC7110");
	else if(Settings.SETA!=0)
	{
		switch(Settings.SETA)
		{
		case ST_010:
			sprintf (tmp, "%s+%s", tmp, "ST-010");
			break;
		case ST_011:
			sprintf (tmp, "%s+%s", tmp, "ST-011");
			break;

		case ST_018:
			sprintf (tmp, "%s+%s", tmp, "ST-018");
			break;

		}
	}
    else if ((ROMType & 0xf) >= 3)
		sprintf (tmp, "%s+%s", tmp, CoPro [(ROMType & 0xf0) >> 4]);

    return (tmp);
}

/* 0x00156c0c */
const char *CMemory::MapMode ()
{
    static char tmp [4];
    sprintf (tmp, "%02x", ROMSpeed & ~0x10);
    return (tmp);
}

/* 0x00156c54 */
const char *CMemory::ROMID ()
{
    return (ROMId);
}
