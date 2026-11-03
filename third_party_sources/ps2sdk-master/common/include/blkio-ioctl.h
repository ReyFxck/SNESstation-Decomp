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
 * Definitions for block I/O using iomanX devctl.
 */

#ifndef __BLKIO_IOCTL__
#define __BLKIO_IOCTL__

#define FDIOC_BLKIO 0x4601

typedef enum sceFsRWTYPE_
{
	sceFsREADING,
	sceFsWRITING,
} sceFsRWTYPE;

typedef struct sceFsDevctlBlkIO_
{
	u32 lbn;
	u32 nblk;
	void *addr;
	u32 blksiz;
	sceFsRWTYPE type;
	u32 mode;
} sceFsDevctlBlkIO;

#endif /* __BLKIO_IOCTL__ */
