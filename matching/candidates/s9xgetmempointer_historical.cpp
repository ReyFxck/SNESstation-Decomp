/*
 * Historical Snes9x 1.41/1.41-1 S9xGetMemPointer body, isolated from
 * getset.h for the SNES Station EE target.
 *
 * Target entry: 0x001ab4e8, exact target span: 340 bytes.
 * The compact structures below preserve only the target-proved field offsets
 * used by this function; the function body itself follows the historical
 * Snes9x implementation.
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

extern HistoricalMemory Memory __asm__("g_p12_memory");
extern HistoricalSettings Settings __asm__("g_Settings_blob");
extern uint8 s7r_bank50[] __asm__("DAT_00413544");
extern uint8 *GetMemPointerOBC1(uint32) __asm__("snes_leaf_00158fdc");

uint8 *S9xGetMemPointer(uint32 Address)
{
    uint8 *GetAddress = Memory.Map[(Address >> 12) & 0x0fff];

    if (GetAddress >= (uint8 *) MAP_LAST)
        return GetAddress + (Address & 0xffff);

    if (Settings.SPC7110 && ((Address & 0x7fffff) == 0x4800))
        return s7r_bank50;

    switch ((int) GetAddress)
    {
    case MAP_SPC7110_DRAM:
        return &s7r_bank50[Address & 0xffff];
    case MAP_PPU:
        return Memory.FillRAM - 0x2000 + (Address & 0xffff);
    case MAP_CPU:
        return Memory.FillRAM - 0x4000 + (Address & 0xffff);
    case MAP_DSP:
        return Memory.FillRAM - 0x6000 + (Address & 0xffff);
    case MAP_SA1RAM:
    case MAP_LOROM_SRAM:
        return Memory.SRAM + (Address & 0xffff);
    case MAP_HIROM_SRAM:
        return Memory.SRAM - 0x6000 + (Address & 0xffff);
    case MAP_BWRAM:
        return Memory.BWRAM - 0x6000 + (Address & 0xffff);
    case MAP_C4:
        return Memory.C4RAM - 0x6000 + (Address & 0xffff);
    case MAP_OBC_RAM:
        return GetMemPointerOBC1(Address);
    case MAP_SETA_DSP:
        return Memory.SRAM;
    case MAP_DEBUG:
    default:
    case MAP_NONE:
        return 0;
    }
}
