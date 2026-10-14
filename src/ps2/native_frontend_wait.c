/*
 * Recovered PS2 frontend wait loop at 0x001029c4.
 *
 * This is the isolated, target-proven source body promoted from the broader
 * Progress-28 structural lift. The historical EE build used GNU89 implicit
 * declarations for the address-labelled frontend helpers. Old-style
 * declarations preserve that ABI while remaining valid to modern compilers.
 */

typedef unsigned short uint16_t;

extern unsigned char V68_DAT_001bb748 __asm__("DAT_001bb748");
extern unsigned char V68_DAT_001b0a58 __asm__("DAT_001b0a58");
extern uint16_t _DAT_001bb340;
extern int FUN_00101b64();
extern int FUN_0019b948();
extern int FUN_00101e8c();
extern int FUN_0019c2ac();
extern int FUN_001a88a8();
extern int FUN_001a8864();

void snes_p28_001029c4(void)
{
    long state;
    unsigned char status[32];

    FUN_00101b64();
    FUN_0019b948((void *)&V68_DAT_001bb748, 0, 0x280, 0xdc, 4,
                 0x80ffffffUL, 2, (void *)&V68_DAT_001b0a58);
    FUN_00101e8c();
    FUN_0019c2ac();
    _DAT_001bb340 = 0x2f;

    while ((state = FUN_001a88a8(0, 0), state != 6 ||
            (FUN_001a8864(0, 0, status),
             ((~(((unsigned int)status[2] << 8) |
                  (unsigned int)status[3])) & 0x40) == 0))) {
        *(volatile unsigned long *)0x12001000UL =
            *(volatile unsigned long *)0x12001000UL & 8UL;
        do {
        } while ((*(volatile unsigned long *)0x12001000UL & 8UL) == 0);
    }
}
