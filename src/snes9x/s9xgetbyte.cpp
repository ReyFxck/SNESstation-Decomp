/*
 * Historical Snes9x 1.40 S9xGetByte body, isolated from getset.h for the
 * SNES Station EE target.
 *
 * Target entry: 0x001ab63c, exact target span: 708 bytes.
 * The compact state layouts below preserve the official 1.40 offsets used by
 * this function (also published by the historical i386/offsets.h).
 */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned char bool8;

enum {
    MEMMAP_SHIFT = 12,
    MEMMAP_MASK = 0x0fff,
    MEMMAP_NUM_BLOCKS = 4096,
    ONE_CYCLE = 6,
    SLOW_ONE_CYCLE = 8
};

class CMemory
{
public:
    enum {
        MAP_PPU,
        MAP_CPU,
        MAP_DSP,
        MAP_LOROM_SRAM,
        MAP_HIROM_SRAM,
        MAP_NONE,
        MAP_DEBUG,
        MAP_C4,
        MAP_BWRAM,
        MAP_BWRAM_BITMAP,
        MAP_BWRAM_BITMAP2,
        MAP_SA1RAM,
        MAP_SPC7110_ROM,
        MAP_SPC7110_DRAM,
        MAP_RONLY_SRAM,
        MAP_OBC_RAM,
        MAP_SETA_DSP,
        MAP_SETA_RISC,
        MAP_LAST
    };

    uint8 *RAM;                       /* +0x0000 */
    uint8 *ROM;                       /* +0x0004 */
    uint8 *VRAM;                      /* +0x0008 */
    uint8 *SRAM;                      /* +0x000c */
    uint8 *BWRAM;                     /* +0x0010 */
    uint8 *FillRAM;                   /* +0x0014 */
    uint8 *C4RAM;                     /* +0x0018 */
    bool8 HiROM;                      /* +0x001c */
    bool8 LoROM;                      /* +0x001d */
    uint8 reserved_1e[2];
    uint32 SRAMMask;                  /* +0x0020 */
    uint8 SRAMSize;                   /* +0x0024 */
    uint8 reserved_25[3];
    uint8 *Map[MEMMAP_NUM_BLOCKS];    /* +0x0028 */
    uint8 *WriteMap[MEMMAP_NUM_BLOCKS]; /* +0x4028 */
    uint8 MemorySpeed[MEMMAP_NUM_BLOCKS]; /* +0x8028 */
    uint8 BlockIsRAM[MEMMAP_NUM_BLOCKS];  /* +0x9028 */
    uint8 BlockIsROM[MEMMAP_NUM_BLOCKS];  /* +0xa028 */
};

struct HistoricalCPU
{
    uint32 Flags;                     /* +0x00 */
    uint8 BranchSkip;                 /* +0x04 */
    uint8 NMIActive;                  /* +0x05 */
    uint8 IRQActive;                  /* +0x06 */
    uint8 WaitingForInterrupt;        /* +0x07 */
    uint8 InDMA;                      /* +0x08 */
    uint8 WhichEvent;                 /* +0x09 */
    uint8 reserved_0a[2];
    uint8 *PC;                        /* +0x0c */
    uint8 *PCBase;                    /* +0x10 */
    uint8 *PCAtOpcodeStart;           /* +0x14 */
    uint8 *WaitAddress;               /* +0x18 */
    uint32 WaitCounter;               /* +0x1c */
    long Cycles;                      /* +0x20, 64-bit with -mlong64 */
};

extern CMemory Memory __asm__("g_p12_memory");
extern HistoricalCPU CPU __asm__("g_CPU_blob");
extern "C" uint8 OpenBus __asm__("g_OpenBus_byte");

extern "C" uint8 S9xGetPPU(uint16);
extern "C" uint8 S9xGetCPU(uint16);
extern "C" uint8 S9xGetDSP(uint16);
extern "C" uint8 S9xGetC4(uint16);
extern "C" uint8 S9xGetSPC7110Byte(uint32);
extern "C" uint8 S9xGetSPC7110(uint16);
extern "C" uint8 GetOBC1(uint16);
extern "C" uint8 S9xGetSetaDSP(uint32);
extern "C" uint8 S9xGetST018(uint32);

uint8 S9xGetByte (uint32 Address)
{
#if defined(VAR_CYCLES) || defined(CPU_SHUTDOWN)
    int block;
    uint8 *GetAddress = Memory.Map [block = (Address >> MEMMAP_SHIFT) & MEMMAP_MASK];
#else
    uint8 *GetAddress = Memory.Map [(Address >> MEMMAP_SHIFT) & MEMMAP_MASK];
#endif
    if (GetAddress >= (uint8 *) CMemory::MAP_LAST)
    {
#ifdef VAR_CYCLES
        CPU.Cycles += Memory.MemorySpeed [block];
#endif
#ifdef CPU_SHUTDOWN
        if (Memory.BlockIsRAM [block])
            CPU.WaitAddress = CPU.PCAtOpcodeStart;
#endif
        return (*(GetAddress + (Address & 0xffff)));
    }

    switch ((int) GetAddress)
    {
    case CMemory::MAP_PPU:
#ifdef VAR_CYCLES
        if (!CPU.InDMA)
            CPU.Cycles += ONE_CYCLE;
#endif
        return (S9xGetPPU (Address & 0xffff));
    case CMemory::MAP_CPU:
#ifdef VAR_CYCLES
        CPU.Cycles += ONE_CYCLE;
#endif
        return (S9xGetCPU (Address & 0xffff));
    case CMemory::MAP_DSP:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        return (S9xGetDSP (Address & 0xffff));
    case CMemory::MAP_SA1RAM:
    case CMemory::MAP_LOROM_SRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        return (*(Memory.SRAM + ((((Address&0xFF0000)>>1) |(Address&0x7FFF)) &Memory.SRAMMask)));

    case CMemory::MAP_RONLY_SRAM:
    case CMemory::MAP_HIROM_SRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        return (*(Memory.SRAM + (((Address & 0x7fff) - 0x6000 +
            ((Address & 0xf0000) >> 3)) & Memory.SRAMMask)));

    case CMemory::MAP_BWRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        return (*(Memory.BWRAM + ((Address & 0x7fff) - 0x6000)));

    case CMemory::MAP_C4:
        return (S9xGetC4 (Address & 0xffff));

    case CMemory::MAP_SPC7110_ROM:
        CPU.Cycles += SLOW_ONE_CYCLE;
        return S9xGetSPC7110Byte(Address);

    case CMemory::MAP_SPC7110_DRAM:
        CPU.Cycles += SLOW_ONE_CYCLE;
        return S9xGetSPC7110(0x4800);
    case CMemory::MAP_OBC_RAM:
        CPU.Cycles += SLOW_ONE_CYCLE;
        return GetOBC1(Address & 0xffff);

    case CMemory::MAP_SETA_DSP:
        CPU.Cycles += SLOW_ONE_CYCLE;
        return S9xGetSetaDSP(Address);

    case CMemory::MAP_SETA_RISC:
        CPU.Cycles += SLOW_ONE_CYCLE;
        return S9xGetST018(Address);

    case CMemory::MAP_DEBUG:
        return OpenBus;

    default:
    case CMemory::MAP_NONE:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        return OpenBus;
    }
}
