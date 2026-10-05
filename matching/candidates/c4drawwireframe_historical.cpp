/*
 * Historical-source reconstruction of Snes9x 1.41-1 C4DrawWireFrame,
 * isolated as a canonical SNES Station translation unit.
 *
 * The function body is the historical C++ implementation.  The only
 * target-specific code-generation adaptation used by the V77 proof is the
 * packed PS2 READ_3WORD form below.  The compatibility memory layout carries
 * only the fields this function needs; symbol addends are relocation fields
 * and are checked against the already target-proved V77 object.
 */

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef signed short   int16;
typedef unsigned int   uint32;
typedef signed int     int32;

struct C4HistoricalMemory
{
    uint8 *ROM;
    uint8 *C4RAM;
};

extern C4HistoricalMemory Memory;

struct C4PackedU32
{
    uint32 value;
} __attribute__((packed));

#define READ_3WORD(s) ({ \
    register uint8 *source = (uint8 *) (s); \
    __asm__ volatile ("" : "+r" (source)); \
    register uint32 mask = 0x00ffffff; \
    register uint32 value = ((C4PackedU32 *) source)->value; \
    mask &= value; \
    mask; \
})

static inline uint8 *C4GetMemPointer(uint32 Address)
{
    return Memory.ROM + ((Address & 0xff0000) >> 1) + (Address & 0x7fff);
}

extern void C4DrawLine(int32, int32, int16, int32, int32, int16, uint8);

void C4DrawWireFrame(void)
{
    uint8 *line = C4GetMemPointer(READ_3WORD(Memory.C4RAM + 0x1f80));
    uint8 *point1, *point2;
    int16 X1, Y1, Z1;
    int16 X2, Y2, Z2;
    uint8 Color;

    for (int i = Memory.C4RAM[0x0295]; i > 0; i--, line += 5)
    {
        if (line[0] == 0xff && line[1] == 0xff)
        {
            uint8 *tmp = line - 5;
            while (tmp[2] == 0xff && tmp[3] == 0xff)
                tmp -= 5;
            point1 = C4GetMemPointer((Memory.C4RAM[0x1f82] << 16) |
                                     (tmp[2] << 8) | tmp[3]);
        }
        else
            point1 = C4GetMemPointer((Memory.C4RAM[0x1f82] << 16) |
                                     (line[0] << 8) | line[1]);

        point2 = C4GetMemPointer((Memory.C4RAM[0x1f82] << 16) |
                                 (line[2] << 8) | line[3]);

        X1 = (point1[0] << 8) | point1[1];
        Y1 = (point1[2] << 8) | point1[3];
        Z1 = (point1[4] << 8) | point1[5];
        X2 = (point2[0] << 8) | point2[1];
        Y2 = (point2[2] << 8) | point2[3];
        Z2 = (point2[4] << 8) | point2[5];

        Color = line[4];

        C4DrawLine(X1, Y1, Z1, X2, Y2, Z2, Color);
    }
}
