#ifndef SNESSTATION_GLUE_LEGACY_COMPAT_H
#define SNESSTATION_GLUE_LEGACY_COMPAT_H

typedef signed int s32;
typedef unsigned int u32;

#define DI DIntr
#define EI EIntr
#define EE_SYNC() __asm__ volatile ("sync")

int DIntr(void);
int EIntr(void);
s32 _EnableDmac(s32 channel);
s32 _DisableDmac(s32 channel);

#endif
