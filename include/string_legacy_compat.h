#ifndef SNESSTATION_STRING_LEGACY_COMPAT_H
#define SNESSTATION_STRING_LEGACY_COMPAT_H

/*
 * Only the F_strrchr historical variant is selected in this source tree.
 * Its PS2LIB body only requires the C string declarations. Historical
 * non-string PS2SDK headers were dependencies of other, disabled variants.
 */
#include <string.h>

#endif
