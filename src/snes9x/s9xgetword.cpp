/*
 * Snes9x 1.41-1 S9xGetWord body isolated from getset.h for the
 * SNES Station EE target.
 *
 * Official source: snes9x-1.41-1-src.tar.gz
 * Target entry: 0x001abc28, exact target span: 1020 bytes.
 *
 * The historical body is preserved below. The direct FAST_LSB_WORD_ACCESS
 * load uses the target-proved packed R5900 access shape recovered by the
 * HUNT1041 V48 proof: a 32-bit unaligned load into $16 followed by low-16
 * truncation.
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
    TWO_CYCLES = 12,
    SLOW_ONE_CYCLE = 8
};

class CMemory
{
public:
    enum {
        MAP_PPU, MAP_CPU, MAP_DSP, MAP_LOROM_SRAM, MAP_HIROM_SRAM,
        MAP_NONE, MAP_DEBUG, MAP_C4, MAP_BWRAM, MAP_BWRAM_BITMAP,
        MAP_BWRAM_BITMAP2, MAP_SA1RAM, MAP_SPC7110_ROM, MAP_SPC7110_DRAM,
        MAP_RONLY_SRAM, MAP_OBC_RAM, MAP_SETA_DSP, MAP_SETA_RISC, MAP_LAST
    };

    uint8 *RAM;
    uint8 *ROM;
    uint8 *VRAM;
    uint8 *SRAM;
    uint8 *BWRAM;
    uint8 *FillRAM;
    uint8 *C4RAM;
    bool8 HiROM;
    bool8 LoROM;
    uint8 reserved_1e[2];
    uint32 SRAMMask;
    uint8 SRAMSize;
    uint8 reserved_25[3];
    uint8 *Map[MEMMAP_NUM_BLOCKS];
    uint8 *WriteMap[MEMMAP_NUM_BLOCKS];
    uint8 MemorySpeed[MEMMAP_NUM_BLOCKS];
    uint8 BlockIsRAM[MEMMAP_NUM_BLOCKS];
    uint8 BlockIsROM[MEMMAP_NUM_BLOCKS];
};

struct HistoricalCPU
{
    uint32 Flags;
    uint8 BranchSkip;
    uint8 NMIActive;
    uint8 IRQActive;
    uint8 WaitingForInterrupt;
    uint8 InDMA;
    uint8 WhichEvent;
    uint8 reserved_0a[2];
    uint8 *PC;
    uint8 *PCBase;
    uint8 *PCAtOpcodeStart;
    uint8 *WaitAddress;
    uint32 WaitCounter;
    long Cycles;
};

struct Hunt1041UnalignedUint32
{
    uint32 value;
} __attribute__((packed));

extern CMemory Memory __asm__("g_p12_memory");
extern HistoricalCPU CPU __asm__("g_CPU_blob");
extern "C" uint8 OpenBus __asm__("g_OpenBus_byte");

extern uint8 S9xGetByte(uint32);
extern "C" uint8 S9xGetPPU(uint16);
extern "C" uint8 S9xGetCPU(uint16);
extern "C" uint8 S9xGetDSP(uint16);
extern "C" uint8 S9xGetC4(uint16);
extern "C" uint8 S9xGetSPC7110Byte(uint32);
extern "C" uint8 S9xGetSPC7110(uint16);
extern "C" uint8 GetOBC1(uint16);
extern "C" uint8 S9xGetSetaDSP(uint32);
extern "C" uint8 S9xGetST018(uint32);

uint16 S9xGetWord (uint32 Address)
{
    if ((Address & 0x0fff) == 0x0fff)
    {
        OpenBus=S9xGetByte (Address);
        return (OpenBus | (S9xGetByte (Address + 1) << 8));
    }
#if defined(VAR_CYCLES) || defined(CPU_SHUTDOWN)
    int block;
    uint8 *GetAddress = Memory.Map [block = (Address >> MEMMAP_SHIFT) & MEMMAP_MASK];
#else
    uint8 *GetAddress = Memory.Map [(Address >> MEMMAP_SHIFT) & MEMMAP_MASK];
#endif
    if (GetAddress >= (uint8 *) CMemory::MAP_LAST)
    {
#ifdef VAR_CYCLES
        CPU.Cycles += Memory.MemorySpeed [block] << 1;
#endif
#ifdef CPU_SHUTDOWN
        if (Memory.BlockIsRAM [block])
            CPU.WaitAddress = CPU.PCAtOpcodeStart;
#endif
#ifdef FAST_LSB_WORD_ACCESS
        register uint32 value __asm__ ("$16") =
            ((Hunt1041UnalignedUint32 *)
             (GetAddress + (Address & 0xffff)))->value;
        value &= 0xffff;
        __asm__ ("" : "+r" (value));
        return value;
#else
        return (*(GetAddress + (Address & 0xffff)) |
            (*(GetAddress + (Address & 0xffff) + 1) << 8));
#endif
    }

    switch ((int) GetAddress)
    {
    case CMemory::MAP_PPU:
#ifdef VAR_CYCLES
        if (!CPU.InDMA)
            CPU.Cycles += TWO_CYCLES;
#endif
        return (S9xGetPPU (Address & 0xffff) |
            (S9xGetPPU ((Address + 1) & 0xffff) << 8));
    case CMemory::MAP_CPU:
#ifdef VAR_CYCLES
        CPU.Cycles += TWO_CYCLES;
#endif
        return (S9xGetCPU (Address & 0xffff) |
            (S9xGetCPU ((Address + 1) & 0xffff) << 8));
    case CMemory::MAP_DSP:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
#endif
        return (S9xGetDSP (Address & 0xffff) |
            (S9xGetDSP ((Address + 1) & 0xffff) << 8));
    case CMemory::MAP_SA1RAM:
    case CMemory::MAP_LOROM_SRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
#endif
        return
            (*(Memory.SRAM + ((((Address&0xFF0000)>>1) |(Address&0x7FFF)) &Memory.SRAMMask)))|
            ((*(Memory.SRAM + (((((Address+1)&0xFF0000)>>1) |((Address+1)&0x7FFF)) &Memory.SRAMMask)))<<8);

    case CMemory::MAP_RONLY_SRAM:
    case CMemory::MAP_HIROM_SRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
#endif
        return (*(Memory.SRAM +
            (((Address & 0x7fff) - 0x6000 +
            ((Address & 0xf0000) >> 3)) & Memory.SRAMMask)) |
            (*(Memory.SRAM +
            ((((Address + 1) & 0x7fff) - 0x6000 +
            (((Address + 1) & 0xf0000) >> 3)) & Memory.SRAMMask)) << 8));

    case CMemory::MAP_BWRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
#endif
        return (*(Memory.BWRAM + ((Address & 0x7fff) - 0x6000)) |
            (*(Memory.BWRAM + (((Address + 1) & 0x7fff) - 0x6000)) << 8));

    case CMemory::MAP_C4:
        return (S9xGetC4 (Address & 0xffff) |
            (S9xGetC4 ((Address + 1) & 0xffff) << 8));

    case CMemory::MAP_SPC7110_ROM:
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
        return (S9xGetSPC7110Byte(Address)|
            (S9xGetSPC7110Byte (Address+1))<<8);
    case CMemory::MAP_SPC7110_DRAM:
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
        return (S9xGetSPC7110(0x4800)|
            (S9xGetSPC7110 (0x4800) << 8));
    case CMemory::MAP_OBC_RAM:
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
        return GetOBC1(Address&0xFFFF)| GetOBC1((Address+1)&0xFFFF);

    case CMemory::MAP_SETA_DSP:
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
        return S9xGetSetaDSP(Address)| (S9xGetSetaDSP((Address+1))<<8);

    case CMemory::MAP_SETA_RISC:
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
        return S9xGetST018(Address)| S9xGetST018((Address+1));

    case CMemory::MAP_DEBUG:
        return (OpenBus | (OpenBus<<8));

    default:
    case CMemory::MAP_NONE:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE * 2;
#endif
        return (OpenBus | (OpenBus<<8));
    }
}
