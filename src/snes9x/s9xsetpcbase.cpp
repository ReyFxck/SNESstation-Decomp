/*
 * Exact historical Snes9x 1.40 S9xSetPCBase body isolated from getset.h
 * for the SNES Station EE target.
 *
 * Official source: snes9x-1.40-src-2.tar.gz
 * Target entry: 0x001ac024, exact target span: 364 bytes.
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
    long NextEvent;
    long V_Counter;
    long MemSpeed;
    long MemSpeedx2;
    long FastROMSpeed;
    uint32 AutoSaveTimer;
    bool8 SRAMModified;
};

extern CMemory Memory __asm__("g_p12_memory");
extern HistoricalCPU CPU __asm__("g_CPU_blob");

void S9xSetPCBase (uint32 Address)
{
#ifdef VAR_CYCLES
    int block;
    uint8 *GetAddress = Memory.Map [block = (Address >> MEMMAP_SHIFT) & MEMMAP_MASK];
#else
    uint8 *GetAddress = Memory.Map [(Address >> MEMMAP_SHIFT) & MEMMAP_MASK];
#endif
    if (GetAddress >= (uint8 *) CMemory::MAP_LAST)
    {
#ifdef VAR_CYCLES
        CPU.MemSpeed = Memory.MemorySpeed [block];
        CPU.MemSpeedx2 = CPU.MemSpeed << 1;
#endif
        CPU.PCBase = GetAddress;
        CPU.PC = GetAddress + (Address & 0xffff);
        return;
    }

    switch ((int) GetAddress)
    {
    case CMemory::MAP_PPU:
#ifdef VAR_CYCLES
        CPU.MemSpeed = ONE_CYCLE;
        CPU.MemSpeedx2 = TWO_CYCLES;
#endif
        CPU.PCBase = Memory.FillRAM - 0x2000;
        CPU.PC = CPU.PCBase + (Address & 0xffff);
        return;

    case CMemory::MAP_CPU:
#ifdef VAR_CYCLES
        CPU.MemSpeed = ONE_CYCLE;
        CPU.MemSpeedx2 = TWO_CYCLES;
#endif
        CPU.PCBase = Memory.FillRAM - 0x4000;
        CPU.PC = CPU.PCBase + (Address & 0xffff);
        return;

    case CMemory::MAP_DSP:
#ifdef VAR_CYCLES
        CPU.MemSpeed = SLOW_ONE_CYCLE;
        CPU.MemSpeedx2 = SLOW_ONE_CYCLE * 2;
#endif
        CPU.PCBase = Memory.FillRAM - 0x6000;
        CPU.PC = CPU.PCBase + (Address & 0xffff);
        return;

    case CMemory::MAP_SA1RAM:
    case CMemory::MAP_LOROM_SRAM:
#ifdef VAR_CYCLES
        CPU.MemSpeed = SLOW_ONE_CYCLE;
        CPU.MemSpeedx2 = SLOW_ONE_CYCLE * 2;
#endif
        CPU.PCBase = Memory.SRAM;
        CPU.PC = CPU.PCBase + (Address & 0xffff);
        return;

    case CMemory::MAP_BWRAM:
#ifdef VAR_CYCLES
        CPU.MemSpeed = SLOW_ONE_CYCLE;
        CPU.MemSpeedx2 = SLOW_ONE_CYCLE * 2;
#endif
        CPU.PCBase = Memory.BWRAM - 0x6000;
        CPU.PC = CPU.PCBase + (Address & 0xffff);
        return;

    case CMemory::MAP_HIROM_SRAM:
#ifdef VAR_CYCLES
        CPU.MemSpeed = SLOW_ONE_CYCLE;
        CPU.MemSpeedx2 = SLOW_ONE_CYCLE * 2;
#endif
        CPU.PCBase = Memory.SRAM - 0x6000;
        CPU.PC = CPU.PCBase + (Address & 0xffff);
        return;

    case CMemory::MAP_C4:
#ifdef VAR_CYCLES
        CPU.MemSpeed = SLOW_ONE_CYCLE;
        CPU.MemSpeedx2 = SLOW_ONE_CYCLE * 2;
#endif
        CPU.PCBase = Memory.C4RAM - 0x6000;
        CPU.PC = CPU.PCBase + (Address & 0xffff);
        return;

    case CMemory::MAP_DEBUG:
#ifdef DEBUGGER
        printf ("SBP %06x\n", Address);
#endif

    default:
    case CMemory::MAP_NONE:
#ifdef VAR_CYCLES
        CPU.MemSpeed = SLOW_ONE_CYCLE;
        CPU.MemSpeedx2 = SLOW_ONE_CYCLE * 2;
#endif
#ifdef DEBUGGER
        printf ("SBP %06x\n", Address);
#endif
        CPU.PCBase = Memory.SRAM;
        CPU.PC = Memory.SRAM + (Address & 0xffff);
        return;
    }
}
