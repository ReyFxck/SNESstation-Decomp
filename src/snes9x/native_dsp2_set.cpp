/*
 * Snes9x 1.41-1 DSP2 command writer, recovered for the PS2/R5900 target.
 *
 * The original port writes the opcode-09 product through DSP1.output, whose
 * declared element type is byte.  Preserve that alignment contract explicitly
 * so GCC emits the target's unaligned swl/swr pair instead of assuming a
 * naturally aligned uint32 pointer.
 */
typedef unsigned char uint8;
typedef unsigned char bool8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef double MATRIX[3][3];
typedef double VECTOR[3];

enum AttitudeMatrix { MatrixA, MatrixB, MatrixC };

struct UnalignedUint32 {
    uint32 value;
} __attribute__((packed));

static inline void StoreUnalignedUint32(void *destination, uint32 value)
{
    reinterpret_cast<UnalignedUint32 *>(destination)->value = value;
}

struct SDSP1 {
    bool8 waiting4command;
    bool8 first_parameter;
    uint8 command;
    uint32 in_count;
    uint32 in_index;
    uint32 out_count;
    uint32 out_index;
    uint8 parameters[512];
    uint8 output[512];
    MATRIX vMa;
    MATRIX vMb;
    MATRIX vMc;
    MATRIX vM;
    VECTOR vT;
    double vFov;
    double vPlaneD;
    double vHorizon;
    void ScreenToGround(VECTOR &, double, double);
    MATRIX &GetMatrix(AttitudeMatrix);
};

extern SDSP1 DSP1 __asm__("DAT_00345628");
extern uint16 DSP2Op09Word1 __asm__("DAT_0034163a");
extern uint16 DSP2Op09Word2 __asm__("DAT_0034163c");
extern bool DSP2Op05HasLen __asm__("DAT_0034163e");
extern int DSP2Op05Len __asm__("DAT_00341640");
extern bool DSP2Op06HasLen __asm__("DAT_00341644");
extern int DSP2Op06Len __asm__("DAT_00341648");
extern uint8 DSP2Op05Transparent __asm__("DAT_0034164c");
extern bool DSP2Op0DHasLen __asm__("DAT_0034164d");
extern int DSP2Op0DOutLen __asm__("DAT_00341650");
extern int DSP2Op0DInLen __asm__("DAT_00341654");
extern char DSP2UnknownOpcodeFormat[] __asm__("DAT_001b2420");

extern int printf(const char *, ...);
extern void DSP2_Op01(void);
extern void DSP2_Op05(void);
extern void DSP2_Op06(void);
extern void DSP2_Op0D(void);

void DSP2SetByte(uint8 byte, uint16 address)
{
    if ((address & 0xf000) == 0x6000 ||
        (address >= 0x8000 && address < 0xc000)) {
        if (DSP1.waiting4command) {
            DSP1.command = byte;
            DSP1.in_index = 0;
            DSP1.waiting4command = false;
            switch (byte) {
            case 0x01: DSP1.in_count = 32; break;
            case 0x03: DSP1.in_count = 1; break;
            case 0x05: DSP1.in_count = 1; break;
            case 0x09: DSP1.in_count = 4; break;
            case 0x06: DSP1.in_count = 1; break;
            case 0x0d: DSP1.in_count = 2; break;
            default:
                printf(DSP2UnknownOpcodeFormat, byte);
            case 0x0f: DSP1.in_count = 0; break;
            }
        } else {
            DSP1.parameters[DSP1.in_index] = byte;
            DSP1.in_index++;
        }

        if (DSP1.in_count == DSP1.in_index) {
            DSP1.waiting4command = true;
            DSP1.out_index = 0;
            switch (DSP1.command) {
            case 0x0d:
                if (DSP2Op0DHasLen) {
                    DSP2Op0DHasLen = false;
                    DSP1.out_count = DSP2Op0DOutLen;
                    DSP2_Op0D();
                } else {
                    DSP2Op0DInLen = DSP1.parameters[0];
                    DSP2Op0DOutLen = DSP1.parameters[1];
                    DSP1.in_index = 0;
                    DSP1.in_count = (DSP2Op0DInLen + 1) >> 1;
                    DSP2Op0DHasLen = true;
                    if (byte)
                        DSP1.waiting4command = false;
                }
                break;
            case 0x06:
                if (DSP2Op06HasLen) {
                    DSP2Op06HasLen = false;
                    DSP1.out_count = DSP2Op06Len;
                    DSP2_Op06();
                } else {
                    DSP2Op06Len = DSP1.parameters[0];
                    DSP1.in_index = 0;
                    DSP1.in_count = DSP2Op06Len;
                    DSP2Op06HasLen = true;
                    if (byte)
                        DSP1.waiting4command = false;
                }
                break;
            case 0x01:
                DSP1.out_count = 32;
                DSP2_Op01();
                break;
            case 0x09: {
                register SDSP1 *dsp __asm__("$2") = &DSP1;
                __asm__("" : "+r"(dsp));
                register uint8 *output __asm__("$7") = dsp->output;
                __asm__("" : "+r"(output));
                register uint32 word2 __asm__("$5") =
                    dsp->parameters[2] | (dsp->parameters[3] << 8);
                register uint32 word1 __asm__("$3") =
                    dsp->parameters[0] | (dsp->parameters[1] << 8);
                __asm__("" : "+r"(word2) : : "memory");
                dsp->out_count = 4;
                DSP2Op09Word1 = word1;
                __asm__("" : "+r"(word1) : : "memory");
                word1 *= word2;
                __asm__("" : "+r"(word1), "+r"(word2));
                DSP2Op09Word2 = word2;
                StoreUnalignedUint32(output, word1);
                break;
            }
            case 0x05:
                if (DSP2Op05HasLen) {
                    DSP2Op05HasLen = false;
                    DSP1.out_count = DSP2Op05Len;
                    DSP2_Op05();
                } else {
                    DSP2Op05Len = DSP1.parameters[0];
                    DSP1.in_index = 0;
                    DSP1.in_count = 2 * DSP2Op05Len;
                    DSP2Op05HasLen = true;
                    if (byte)
                        DSP1.waiting4command = false;
                }
                break;
            case 0x03:
                DSP2Op05Transparent = DSP1.parameters[0];
                break;
            case 0x0f:
            default:
                break;
            }
        }
    }
}
