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
/* Historical Snes9x 1.41-1 RAM cheat searches.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Target build omits the three text-code parsers preceding these searches.
 * 0x0010e360 S9xStartCheatSearch; 0x0010e420 S9xSearchForChange;
 * 0x00111f88 S9xSearchForValue; 0x00113f64 S9xOutputCheatSearchResults.
 * The four bodies reproduce all 23936 instruction bytes after masking the
 * 15 known relocation fields. Both large searches are raw byte-identical.
 */
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned char bool8;
extern "C" void *memmove(void *, const void *, unsigned int);
extern "C" void *memset(void *, int, unsigned int);
extern "C" int printf(const char *, ...);
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

typedef enum
{
    S9X_LESS_THAN, S9X_GREATER_THAN, S9X_LESS_THAN_OR_EQUAL,
    S9X_GREATER_THAN_OR_EQUAL, S9X_EQUAL, S9X_NOT_EQUAL
} S9xCheatComparisonType;

typedef enum
{
    S9X_8_BITS, S9X_16_BITS, S9X_24_BITS, S9X_32_BITS
} S9xCheatDataSize;

void S9xStartCheatSearch (SCheatData *d)
{
    memmove (d->CWRAM, d->RAM, 0x20000);
    memmove (d->CSRAM, d->SRAM, 0x10000);
    memmove (d->CIRAM, &d->FillRAM [0x3000], 0x2000);
    memset ((char *) d->WRAM_BITS, 0xff, 0x20000 >> 3);
    memset ((char *) d->SRAM_BITS, 0xff, 0x10000 >> 3);
    memset ((char *) d->IRAM_BITS, 0xff, 0x2000 >> 3);
}

#define BIT_CLEAR(a,v) \
(a)[(v) >> 5] &= ~(1 << ((v) & 31))

#define BIT_SET(a,v) \
(a)[(v) >> 5] |= 1 << ((v) & 31)

#define TEST_BIT(a,v) \
((a)[(v) >> 5] & (1 << ((v) & 31)))

#define _C(c,a,b) \
((c) == S9X_LESS_THAN ? (a) < (b) : \
 (c) == S9X_GREATER_THAN ? (a) > (b) : \
 (c) == S9X_LESS_THAN_OR_EQUAL ? (a) <= (b) : \
 (c) == S9X_GREATER_THAN_OR_EQUAL ? (a) >= (b) : \
 (c) == S9X_EQUAL ? (a) == (b) : \
 (a) != (b))

#define _D(s,m,o) \
((s) == S9X_8_BITS ? (uint8) (*((m) + (o))) : \
 (s) == S9X_16_BITS ? ((uint16) (*((m) + (o)) + (*((m) + (o) + 1) << 8))) : \
 (s) == S9X_24_BITS ? ((uint32) (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16))) : \
((uint32)  (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16) + (*((m) + (o) + 3) << 24))))

#define _DS(s,m,o) \
((s) == S9X_8_BITS ? ((int8) *((m) + (o))) : \
 (s) == S9X_16_BITS ? ((int16) (*((m) + (o)) + (*((m) + (o) + 1) << 8))) : \
 (s) == S9X_24_BITS ? (((int32) ((*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16)) << 8)) >> 8): \
 ((int32) (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16) + (*((m) + (o) + 3) << 24))))

void S9xSearchForChange (SCheatData *d, S9xCheatComparisonType cmp,
                         S9xCheatDataSize size, bool8 is_signed, bool8 update)
{
    int l;

    switch (size)
    {
    case S9X_8_BITS: l = 0; break;
    case S9X_16_BITS: l = 1; break;
    case S9X_24_BITS: l = 2; break;
    default:
    case S9X_32_BITS: l = 3; break;
    }

    int i;
    if (is_signed)
    {
        for (i = 0; i < 0x20000 - l; i++)
        {
            if (TEST_BIT (d->WRAM_BITS, i) &&
                _C(cmp, _DS(size, d->RAM, i), _DS(size, d->CWRAM, i)))
            {
                if (update)
                    d->CWRAM [i] = d->RAM [i];
            }
            else
                BIT_CLEAR (d->WRAM_BITS, i);
        }

        for (i = 0; i < 0x10000 - l; i++)
        {
            if (TEST_BIT (d->SRAM_BITS, i) &&
                _C(cmp, _DS(size, d->SRAM, i), _DS(size, d->CSRAM, i)))
            {
                if (update)
                    d->CSRAM [i] = d->SRAM [i];
            }
            else
                BIT_CLEAR (d->SRAM_BITS, i);
        }

        for (i = 0; i < 0x2000 - l; i++)
        {
            if (TEST_BIT (d->IRAM_BITS, i) &&
                _C(cmp, _DS(size, d->FillRAM + 0x3000, i), _DS(size, d->CIRAM, i)))
            {
                if (update)
                    d->CIRAM [i] = d->FillRAM [i + 0x3000];
            }
            else
                BIT_CLEAR (d->IRAM_BITS, i);
        }
    }
    else
    {
        for (i = 0; i < 0x20000 - l; i++)
        {
            if (TEST_BIT (d->WRAM_BITS, i) &&
                _C(cmp, _D(size, d->RAM, i), _D(size, d->CWRAM, i)))
            {
                if (update)
                    d->CWRAM [i] = d->RAM [i];
            }
            else
                BIT_CLEAR (d->WRAM_BITS, i);
        }

        for (i = 0; i < 0x10000 - l; i++)
        {
            if (TEST_BIT (d->SRAM_BITS, i) &&
                _C(cmp, _D(size, d->SRAM, i), _D(size, d->CSRAM, i)))
            {
                if (update)
                    d->CSRAM [i] = d->SRAM [i];
            }
            else
                BIT_CLEAR (d->SRAM_BITS, i);
        }

        for (i = 0; i < 0x2000 - l; i++)
        {
            if (TEST_BIT (d->IRAM_BITS, i) &&
                _C(cmp, _D(size, d->FillRAM + 0x3000, i), _D(size, d->CIRAM, i)))
            {
                if (update)
                    d->CIRAM [i] = d->FillRAM [i + 0x3000];
            }
            else
                BIT_CLEAR (d->IRAM_BITS, i);
        }
    }
}

void S9xSearchForValue (SCheatData *d, S9xCheatComparisonType cmp,
                        S9xCheatDataSize size, uint32 value,
                        bool8 is_signed, bool8 update)
{
    int l;

    switch (size)
    {
    case S9X_8_BITS: l = 0; break;
    case S9X_16_BITS: l = 1; break;
    case S9X_24_BITS: l = 2; break;
    default:
    case S9X_32_BITS: l = 3; break;
    }

    int i;

    if (is_signed)
    {
        for (i = 0; i < 0x20000 - l; i++)
        {
            if (TEST_BIT (d->WRAM_BITS, i) &&
                _C(cmp, _DS(size, d->RAM, i), (int32) value))
            {
                if (update)
                    d->CWRAM [i] = d->RAM [i];
            }
            else
                BIT_CLEAR (d->WRAM_BITS, i);
        }

        for (i = 0; i < 0x10000 - l; i++)
        {
            if (TEST_BIT (d->SRAM_BITS, i) &&
                _C(cmp, _DS(size, d->SRAM, i), (int32) value))
            {
                if (update)
                    d->CSRAM [i] = d->SRAM [i];
            }
            else
                BIT_CLEAR (d->SRAM_BITS, i);
        }

        for (i = 0; i < 0x2000 - l; i++)
        {
            if (TEST_BIT (d->IRAM_BITS, i) &&
                _C(cmp, _DS(size, d->FillRAM + 0x3000, i), (int32) value))
            {
                if (update)
                    d->CIRAM [i] = d->FillRAM [i + 0x3000];
            }
            else
                BIT_CLEAR (d->IRAM_BITS, i);
        }
    }
    else
    {
        for (i = 0; i < 0x20000 - l; i++)
        {
            if (TEST_BIT (d->WRAM_BITS, i) &&
                _C(cmp, _D(size, d->RAM, i), value))
            {
                if (update)
                    d->CWRAM [i] = d->RAM [i];
            }
            else
                BIT_CLEAR (d->WRAM_BITS, i);
        }

        for (i = 0; i < 0x10000 - l; i++)
        {
            if (TEST_BIT (d->SRAM_BITS, i) &&
                _C(cmp, _D(size, d->SRAM, i), value))
            {
                if (update)
                    d->CSRAM [i] = d->SRAM [i];
            }
            else
                BIT_CLEAR (d->SRAM_BITS, i);
        }

        for (i = 0; i < 0x2000 - l; i++)
        {
            if (TEST_BIT (d->IRAM_BITS, i) &&
                _C(cmp, _D(size, d->FillRAM + 0x3000, i), value))
            {
                if (update)
                    d->CIRAM [i] = d->FillRAM [i + 0x3000];
            }
            else
                BIT_CLEAR (d->IRAM_BITS, i);
        }
    }
}

void S9xOutputCheatSearchResults (SCheatData *d)
{
    int i;
    for (i = 0; i < 0x20000; i++)
    {
	if (TEST_BIT (d->WRAM_BITS, i))
	    printf ("WRAM: %05x: %02x\n", i, d->RAM [i]);
    }

    for (i = 0; i < 0x10000; i++)
    {
	if (TEST_BIT (d->SRAM_BITS, i))
	    printf ("SRAM: %04x: %02x\n", i, d->SRAM [i]);
    }

    for (i = 0; i < 0x2000; i++)
    {
	if (TEST_BIT (d->IRAM_BITS, i))
	    printf ("IRAM: %05x: %02x\n", i, d->FillRAM [i + 0x3000]);
    }
}
