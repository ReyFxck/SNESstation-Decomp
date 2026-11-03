
#ifndef __SDR_I_H__
#define __SDR_I_H__

#include <string.h>
#include <kernel.h>
#include <sifrpc.h>
#include <libsdr.h>
#include <iopcontrol.h>

extern SdIntrCallback SdrSetTransInterruptCallback(int idx, SdIntrCallback handler);
extern SdIntrCallback SdrSetIRQInterruptCallback(SdIntrCallback handler);
extern sceSdTransIntrHandler SdrSetTransInterruptHandler(int idx, sceSdTransIntrHandler handler, void *handler_userdata);
extern sceSdSpu2IntrHandler SdrSetIRQInterruptHandler(sceSdSpu2IntrHandler handler, void *handler_userdata);

#endif
