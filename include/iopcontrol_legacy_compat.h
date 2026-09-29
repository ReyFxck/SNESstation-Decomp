#ifndef SNESSTATION_IOPCONTROL_LEGACY_COMPAT_H
#define SNESSTATION_IOPCONTROL_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for the April 18 2004 PS2LIB EE iopcontrol.c lineage
 * recovered for SNES Station v0.23.
 */
#include "sifcmd_legacy_compat.h"

#define ALIGNED(x) __attribute__((aligned((x))))

void SifStopDma(void);

#endif
