/*
 * Canonical historical-source reconstruction of Snes9x 1.41-1
 * C4DrawWireFrame, isolated from c4emu.cpp.
 * Target entry: 0x0010cdcc.
 *
 * The function body below is the literal 1.41-1 implementation recovered
 * from the hash-pinned upstream archive used by the V77 proof.  The packed
 * READ_3WORD form is the already-proven SNES Station PS2 code-generation
 * adaptation.  Linkage is exposed so the recovered function can own its
 * canonical source-tree entry.
 */

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef signed short   int16;
typedef unsigned int   uint32;
typedef signed int     int32;

struct C4HistoricalMemory
{
    uint8 *reserved_00;
    uint8 *reserved_04;
    uint8 *reserved_08;
    uint8 *reserved_0c;
    uint8 *reserved_10;
    uint8 *reserved_14;
    uint8 *C4RAM; /* historical Snes9x/PS2 layout: +0x18 */
};

extern C4HistoricalMemory Memory __asm__("g_p12_memory");
extern uint8 *S9xGetMemPointer(uint32);
extern void C4DrawLine(int32, int32, int16, int32, int32, int16, uint8);

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

void C4DrawWireFrame(void)
{
    uint8 *line=S9xGetMemPointer(READ_3WORD(Memory.C4RAM+0x1f80));
    uint8 *point1, *point2;
    int16 X1, Y1, Z1;
    int16 X2, Y2, Z2;
    uint8 Color;

#ifdef DEBUGGER
    if(READ_3WORD(Memory.C4RAM+0x1f8f)&0xff00ff) printf("wireframe: Unexpected value in $7f8f: %06x\n", READ_3WORD(Memory.C4RAM+0x1f8f));
    if(READ_3WORD(Memory.C4RAM+0x1fa4)!=0x001000) printf("wireframe: Unexpected value in $7fa4: %06x\n", READ_3WORD(Memory.C4RAM+0x1fa4));
#endif

    for(int i=Memory.C4RAM[0x0295]; i>0; i--, line+=5){
        if(line[0]==0xff && line[1]==0xff){
            uint8 *tmp=line-5;
            while(line[2]==0xff && line[3]==0xff) tmp-=5;
            point1=S9xGetMemPointer((Memory.C4RAM[0x1f82]<<16) | (tmp[2]<<8) | tmp[3]);
        } else {
            point1=S9xGetMemPointer((Memory.C4RAM[0x1f82]<<16) | (line[0]<<8) | line[1]);
        }
        point2=S9xGetMemPointer((Memory.C4RAM[0x1f82]<<16) | (line[2]<<8) | line[3]);

        X1=(point1[0]<<8) | point1[1];
        Y1=(point1[2]<<8) | point1[3];
        Z1=(point1[4]<<8) | point1[5];
        X2=(point2[0]<<8) | point2[1];
        Y2=(point2[2]<<8) | point2[3];
        Z2=(point2[4]<<8) | point2[5];
        Color=line[4];
        C4DrawLine(X1, Y1, Z1, X2, Y2, Z2, Color);
    }
}
