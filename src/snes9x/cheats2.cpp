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
/* Full historical Snes9x 1.41-1 cheat management source.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Target entries: 0x001140e0, 0x00114118, 0x001141cc, 0x00114260,
 * 0x00114284, 0x001142d0, 0x00114328, 0x0011439c, 0x0011444c,
 * 0x001144d0, 0x00114544, 0x00114674.
 * PS2 deltas: FIO descriptors; zero cheats needs no file removal.
 * Memory lengths use the target's 32-bit PS2LIB ABI.
 */
typedef unsigned char uint8;
typedef unsigned char bool8;
typedef unsigned int uint32;
#define TRUE 1
#define FALSE 0
#define MEMMAP_SHIFT 12
#define MEMMAP_MASK 0x0fff
#define MEMMAP_NUM_BLOCKS 4096
class CMemory {
public:
    enum { MAP_LAST = 18 };
    uint8 *RAM, *ROM, *VRAM, *SRAM, *BWRAM, *FillRAM, *C4RAM;
    bool8 HiROM, LoROM;
    uint32 SRAMMask;
    uint8 SRAMSize;
    uint8 *Map[MEMMAP_NUM_BLOCKS];
};
struct SSettings { uint8 reserved[0x11c]; bool8 ApplyCheats; };
extern CMemory Memory __asm__("DAT_0034e2b0");
extern SSettings Settings __asm__("DAT_003454e0");
extern uint8 *SRAM __asm__("DAT_0034e298");
extern "C" void *memmove(void *, const void *, unsigned int);
extern "C" void *memset(void *, int, unsigned int);
extern "C" int fioOpen(const char *, int);
extern "C" int fioClose(int);
extern "C" int fioRead(int, void *, int);
extern "C" int fioWrite(int, const void *, int);
uint8 S9xGetByte(uint32);
void S9xSetByte(uint8, uint32);
struct SCheat
{
    uint32  address;
    uint8   byte;
    uint8   saved_byte;
    bool8   enabled;
    bool8   saved;
    char    name [22];
};

#define MAX_CHEATS 75

struct SCheatData
{
    struct SCheat   c [MAX_CHEATS];
    uint32	    num_cheats;
    uint8	    CWRAM [0x20000];
    uint8	    CSRAM [0x10000];
    uint8	    CIRAM [0x2000];
    uint8           *RAM;
    uint8           *FillRAM;
    uint8           *SRAM;
    uint32	    WRAM_BITS [0x20000 >> 3];
    uint32	    SRAM_BITS [0x10000 >> 3];
    uint32	    IRAM_BITS [0x2000 >> 3];
};

void S9xRemoveCheat (uint32);
void S9xApplyCheat (uint32);
void S9xRemoveCheats();
extern SCheatData Cheat __asm__("DAT_0035fda8");

void S9xInitCheatData ()
{
    Cheat.RAM = Memory.RAM;
    Cheat.SRAM = ::SRAM;
    Cheat.FillRAM = Memory.FillRAM;
}

void S9xAddCheat (bool8 enable, bool8 save_current_value,
		  uint32 address, uint8 byte)
{
    if (Cheat.num_cheats < sizeof (Cheat.c) / sizeof (Cheat. c [0]))
    {
	Cheat.c [Cheat.num_cheats].address = address;
	Cheat.c [Cheat.num_cheats].byte = byte;
	Cheat.c [Cheat.num_cheats].enabled = TRUE;
	if (save_current_value)
	{
	    Cheat.c [Cheat.num_cheats].saved_byte = S9xGetByte (address);
	    Cheat.c [Cheat.num_cheats].saved = TRUE;
	}
	Cheat.num_cheats++;
    }
}

void S9xDeleteCheat (uint32 which1)
{
    if (which1 < Cheat.num_cheats)
    {
	if (Cheat.c [which1].enabled)
	    S9xRemoveCheat (which1);

	memmove (&Cheat.c [which1], &Cheat.c [which1 + 1],
		 sizeof (Cheat.c [0]) * (Cheat.num_cheats - which1 - 1));
	Cheat.num_cheats = 0;
    }
}

void S9xDeleteCheats ()
{
    S9xRemoveCheats ();
    Cheat.num_cheats = 0;
}

void S9xEnableCheat (uint32 which1)
{
    if (which1 < Cheat.num_cheats && !Cheat.c [which1].enabled)
    {
	Cheat.c [which1].enabled = TRUE;
	S9xApplyCheat (which1);
    }
}

void S9xDisableCheat (uint32 which1)
{
    if (which1 < Cheat.num_cheats && Cheat.c [which1].enabled)
    {
	S9xRemoveCheat (which1);
	Cheat.c [which1].enabled = FALSE;
    }
}

void S9xRemoveCheat (uint32 which1)
{
    if (Cheat.c [which1].saved)
    {
	uint32 address = Cheat.c [which1].address;

	int block = (address >> MEMMAP_SHIFT) & MEMMAP_MASK;
	uint8 *ptr = Memory.Map [block];

	if (ptr >= (uint8 *) CMemory::MAP_LAST)
	    *(ptr + (address & 0xffff)) = Cheat.c [which1].saved_byte;
	else
	    S9xSetByte (Cheat.c [which1].saved_byte, address);
    }
}

void S9xApplyCheat (uint32 which1)
{
    uint32 address = Cheat.c [which1].address;

    if (!Cheat.c [which1].saved)
	Cheat.c [which1].saved_byte = S9xGetByte (address);

    int block = (address >> MEMMAP_SHIFT) & MEMMAP_MASK;
    uint8 *ptr = Memory.Map [block];

    if (ptr >= (uint8 *) CMemory::MAP_LAST)
	*(ptr + (address & 0xffff)) = Cheat.c [which1].byte;
    else
	S9xSetByte (Cheat.c [which1].byte, address);
    Cheat.c [which1].saved = TRUE;
}

void S9xApplyCheats ()
{
    if (Settings.ApplyCheats)
    {
        for (uint32 i = 0; i < Cheat.num_cheats; i++)
            if (Cheat.c [i].enabled)
                S9xApplyCheat (i);
    }
}

void S9xRemoveCheats ()
{
    for (uint32 i = 0; i < Cheat.num_cheats; i++)
	if (Cheat.c [i].enabled)
	    S9xRemoveCheat (i);
}

bool8 S9xLoadCheatFile (const char *filename)
{
    Cheat.num_cheats = 0;

    int fs = fioOpen (filename, 1);
    uint8 data [28];

    if (fs < 0)
	return (FALSE);

    while (fioRead (fs, (void *) data, 28) == 28)
    {
	Cheat.c [Cheat.num_cheats].enabled = (data [0] & 4) == 0;
	Cheat.c [Cheat.num_cheats].byte = data [1];
	Cheat.c [Cheat.num_cheats].address = data [2] | (data [3] << 8) |  (data [4] << 16);
	Cheat.c [Cheat.num_cheats].saved_byte = data [5];
	Cheat.c [Cheat.num_cheats].saved = (data [0] & 8) != 0;
	memmove (Cheat.c [Cheat.num_cheats].name, &data [8], 20);
	Cheat.c [Cheat.num_cheats++].name [20] = 0;
    }
    fioClose (fs);

    return (TRUE);
}

bool8 S9xSaveCheatFile (const char *filename)
{
    if (Cheat.num_cheats == 0)
    {
	return (TRUE);
    }

    int fs = fioOpen (filename, 0x202);
    uint8 data [28];

    if (fs < 0)
	return (FALSE);

    uint32 i;
    for (i = 0; i < Cheat.num_cheats; i++)
    {
	memset (data, 0, 28);
	if (i == 0)
	{
	    data [6] = 254;
	    data [7] = 252;
	}
	if (!Cheat.c [i].enabled)
	    data [0] |= 4;

	if (Cheat.c [i].saved)
	    data [0] |= 8;

	data [1] = Cheat.c [i].byte;
	data [2] = (uint8) Cheat.c [i].address;
	data [3] = (uint8) (Cheat.c [i].address >> 8);
	data [4] = (uint8) (Cheat.c [i].address >> 16);
	data [5] = Cheat.c [i].saved_byte;

	memmove (&data [8], Cheat.c [i].name, 19);
	if (fioWrite (fs, data, 28) != 28)
	{
	    fioClose (fs);
	    return (FALSE);
	}
    }
    return (fioClose (fs) == 0);
}
