/*
 * Standalone C++ recovery candidate for SNES Station CMemory::Init.
 *
 * The layout below records only fields touched by the 0x00151074 target
 * window. The original historical C++ body is retained as the semantic guide;
 * target-specific pointer preservation and cleanup behavior are explicit.
 */
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#define NULL 0

extern "C" void *malloc(unsigned int size);
extern "C" void *memset(void *destination, int value, unsigned int size);
extern "C" void rom_cleanup_00151330(void *memory);

#define MAX_ROM_SIZE 0x800000
#define MAX_2BIT_TILES 0x1000
#define MAX_4BIT_TILES 0x0800
#define MAX_8BIT_TILES 0x0400

struct RecoveredIPPU {
    uint8_t reserved_0000[0x1c];
    uint8_t *TileCache[3];
    uint8_t *TileCached[3];
};

struct RecoveredSuperFX {
    uint32_t reserved_0000;
    uint8_t *pvRegisters;
    uint32_t nRamBanks;
    uint8_t *pvRam;
    uint32_t nRomBanks;
    uint8_t *pvRom;
};

class CMemory {
public:
    uint8_t *RAM;                         // 0x0000
    uint8_t *ROM;                         // 0x0004
    uint8_t *VRAM;                        // 0x0008
    uint8_t *SRAM;                        // 0x000c
    uint32_t unknown_0010;
    uint8_t *FillRAM;                     // 0x0014
    uint8_t *C4RAM;                       // 0x0018
    uint8_t reserved_001c[0xb064 - 0x001c];
    uint8_t *SDD1Index;                   // 0xb064
    uint8_t *SDD1Data;                    // 0xb068
    uint8_t reserved_b06c[0xd480 - 0xb06c];
    uint8_t *BSRAM;                       // 0xd480

    unsigned char Init();
};

extern RecoveredIPPU IPPU;
extern RecoveredSuperFX SuperFX;
extern CMemory Memory;
extern uint8_t *ROM;
extern uint8_t *SRAM;
extern uint8_t *RegRAM;

unsigned char CMemory::Init()
{
    /* The target keeps the biased ROM pointer in EE register $7 (a3). */
    register uint8_t *rom_pointer __asm__("$7");

    if (!RAM)
        RAM = (uint8_t *) malloc(0x20000);
    if (!SRAM)
        SRAM = (uint8_t *) malloc(0x20000);
    if (!VRAM)
        VRAM = (uint8_t *) malloc(0x10000);
    if (!ROM)
        ROM = (uint8_t *) malloc(MAX_ROM_SIZE + 0x200 + 0x8000);

    if (!BSRAM)
        BSRAM = (uint8_t *) malloc(0x80000);
    memset(BSRAM, 0, 0x80000);

    FillRAM = NULL;

    if (!IPPU.TileCache[0])
        IPPU.TileCache[0] = (uint8_t *) malloc(MAX_2BIT_TILES * 128);
    if (!IPPU.TileCache[1])
        IPPU.TileCache[1] = (uint8_t *) malloc(MAX_4BIT_TILES * 128);
    if (!IPPU.TileCache[2])
        IPPU.TileCache[2] = (uint8_t *) malloc(MAX_8BIT_TILES * 128);

    if (!IPPU.TileCached[0])
        IPPU.TileCached[0] = (uint8_t *) malloc(MAX_2BIT_TILES);
    if (!IPPU.TileCached[1])
        IPPU.TileCached[1] = (uint8_t *) malloc(MAX_4BIT_TILES);
    if (!IPPU.TileCached[2])
        IPPU.TileCached[2] = (uint8_t *) malloc(MAX_8BIT_TILES);

    if (!RAM || !SRAM || !VRAM || !ROM || !BSRAM ||
        !IPPU.TileCache[0] || !IPPU.TileCache[1] || !IPPU.TileCache[2] ||
        !IPPU.TileCached[0] || !IPPU.TileCached[1] || !IPPU.TileCached[2]) {
        rom_cleanup_00151330(this);
        return 0;
    }

    FillRAM = ROM;
    rom_pointer = ROM + 0x8000;
    ROM = rom_pointer;
    C4RAM = rom_pointer + 0x400000 + 8192 * 8;
    ::ROM = rom_pointer;
    ::SRAM = SRAM;
    ::RegRAM = FillRAM;

    SuperFX.pvRegisters = &Memory.FillRAM[0x3000];
    SuperFX.nRamBanks = 1;
    SuperFX.pvRam = ::SRAM;
    SuperFX.nRomBanks = (2 * 1024 * 1024) / (32 * 1024);
    SuperFX.pvRom = ROM;

    memset(IPPU.TileCached[0], 0, MAX_2BIT_TILES);
    memset(IPPU.TileCached[1], 0, MAX_4BIT_TILES);
    memset(IPPU.TileCached[2], 0, MAX_8BIT_TILES);

    SDD1Data = NULL;
    SDD1Index = NULL;
    return 1;
}
