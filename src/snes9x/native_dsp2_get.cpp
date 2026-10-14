/* Original Snes9x 1.41-1 DSP2 byte reader. The reviewed entry is
 * 0x0012fb78, twelve bytes before the old annotated residual window.
 * SDSP1 layout and shared storage are retained; no local state is owned. */
typedef unsigned char uint8; typedef unsigned char bool8; typedef unsigned short uint16; typedef unsigned int uint32;
typedef double MATRIX[3][3]; typedef double VECTOR[3];
enum AttitudeMatrix { MatrixA, MatrixB, MatrixC };
struct SDSP1 {
    bool8 waiting4command;
    bool8 first_parameter;
    uint8 command;
    uint32 in_count;
    uint32 in_index;
    uint32 out_count;
    uint32 out_index;
    uint8 parameters [512];

    uint8 output [512];


    MATRIX vMa;
    MATRIX vMb;
    MATRIX vMc;




    MATRIX vM;
    VECTOR vT;


    double vFov;


    double vPlaneD;


    double vHorizon;


    void ScreenToGround(VECTOR &v, double X2d, double Y2d);

    MATRIX &GetMatrix( AttitudeMatrix Matrix );
};
extern SDSP1 DSP1 __asm__("DAT_00345628");

uint8 DSP2GetByte(uint16 address)
{
        uint8 t;
    if ((address & 0xf000) == 0x6000 ||
                (address >= 0x8000 && address < 0xc000))
    {
                if (DSP1.out_count)
                {
                        t = (uint8) DSP1.output [DSP1.out_index];
                        DSP1.out_index++;
                        if(DSP1.out_count==DSP1.out_index)
                                DSP1.out_count=0;
                }
                else
                {
                        t = 0xff;
                }
    }
    else t = 0x80;
        return t;
}
