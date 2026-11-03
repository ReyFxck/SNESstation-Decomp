/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * Definitions for libsdr on the EE
 */

#ifndef __LIBSDR_H__
#define __LIBSDR_H__

#include <libsd-common.h>
#include <libsdr-common.h>
#include <sifrpc.h>

extern int sceSdRemoteInit(void);
extern int sceSdTransToIOP(void *eeaddr, void *iopaddr, u32 size, u32 blocking);
extern SifRpcEndFunc_t sceSdCallBack(SifRpcEndFunc_t endfunc);
extern int sceSdRemote(int arg0, ...);

extern int sceSdRemoteCallbackInit(int pri);
extern int sceSdRemoteCallbackQuit(void);

#endif
