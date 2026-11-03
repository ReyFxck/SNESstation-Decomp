/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

#include <sdr_i.h>

struct libsdr_data
{
	int m_sbuff[16] __attribute__((__aligned__(64)));
	SifRpcClientData_t m_cd __attribute__((__aligned__(64)));
	SifRpcEndFunc_t m_endfunc;
};

static struct libsdr_data g_libsdr_data;

int sceSdRemoteInit(void)
{
	if (HasIopRebootedSinceLastCall())
		memset(&g_libsdr_data.m_cd, 0, sizeof(g_libsdr_data.m_cd));
	for (;;)
	{
		if (sceSifBindRpc(&g_libsdr_data.m_cd, sce_SDR_DEV, 0) < 0)
			return -1;

		if (g_libsdr_data.m_cd.server != NULL)
			break;

		nopdelay();
	}
	return 0;
}

int sceSdTransToIOP(void *eeaddr, void *iopaddr, u32 size, u32 blocking)
{
	SifDmaTransfer_t dmat[1];
	int trid;
	if (!blocking)
		return -1;

	dmat[0].src = eeaddr;
	dmat[0].dest = iopaddr;
	dmat[0].size = size;
	dmat[0].attr = 0;

	while (!(trid = sceSifSetDma(dmat, sizeof(dmat)/sizeof(dmat[0]))));

	if (blocking == 1)
		while (sceSifDmaStat(trid) >= 0);

	return 0;
}

SifRpcEndFunc_t sceSdCallBack(SifRpcEndFunc_t endfunc)
{
	SifRpcEndFunc_t old_endfunc;

	old_endfunc = g_libsdr_data.m_endfunc;
	g_libsdr_data.m_endfunc = endfunc;

	return old_endfunc;
}

int sceSdRemote(int arg0, ...)
{
	va_list ap;
	int blocking;
	int cmd;
	SifRpcEndFunc_t endfunc;
	int i;
	int ret;
	const void *old_handler;

	va_start(ap, arg0);
	blocking = arg0;
	cmd = va_arg(ap, int);
	g_libsdr_data.m_sbuff[0] = (uiptr)g_libsdr_data.m_sbuff;
	for (i = 1; i < 7; i += 1)
		g_libsdr_data.m_sbuff[i] = va_arg(ap, int);
	va_end(ap);
	blocking = blocking ? 0 : SIF_RPC_M_NOWAIT;
	endfunc = blocking ? g_libsdr_data.m_endfunc : NULL;

	switch (cmd)
	{
	case rSdSetTransCallback:
		old_handler = SdrSetTransInterruptCallback(g_libsdr_data.m_sbuff[1] ? 1 : 0, (void *)(uiptr)g_libsdr_data.m_sbuff[2]);
		break;
	case rSdSetIRQCallback:
		old_handler = SdrSetIRQInterruptCallback((void *)(uiptr)g_libsdr_data.m_sbuff[1]);
		break;
	case rSdSetTransIntrHandler:
		old_handler = SdrSetTransInterruptHandler(g_libsdr_data.m_sbuff[1] ? 1 : 0, (void *)(uiptr)g_libsdr_data.m_sbuff[2], (void *)(uiptr)g_libsdr_data.m_sbuff[3]);
		break;
	case rSdSetSpu2IntrHandler:
		old_handler = SdrSetIRQInterruptHandler((void *)(uiptr)g_libsdr_data.m_sbuff[1], (void *)(uiptr)g_libsdr_data.m_sbuff[2]);
		break;
	default:
		old_handler = NULL;
		break;
	}

	ret = 0;
	switch (cmd)
	{
	case rSdSetEffectAttr:
	case rSdSetEffectMode:
	case rSdSetEffectModeParams:
		sceSifCallRpc(&g_libsdr_data.m_cd, cmd | g_libsdr_data.m_sbuff[1], blocking, (void *)(uiptr)g_libsdr_data.m_sbuff[2], 64, &g_libsdr_data.m_sbuff[0], sizeof(g_libsdr_data.m_sbuff), endfunc, &g_libsdr_data.m_sbuff[0]);
		ret = g_libsdr_data.m_sbuff[0];
		break;
	case rSdGetEffectAttr:
		sceSifCallRpc(&g_libsdr_data.m_cd, cmd | g_libsdr_data.m_sbuff[1], blocking, &g_libsdr_data.m_sbuff[0], sizeof(g_libsdr_data.m_sbuff), (void *)(uiptr)g_libsdr_data.m_sbuff[2], 64, endfunc, (void *)(uiptr)g_libsdr_data.m_sbuff[2]);
		break;
	case rSdProcBatch2:
	case rSdProcBatchEx2:
		{
			sceSdBatch *batch_p;
			int *retptr;
			int retptr_sz;
			batch_p = (sceSdBatch *)g_libsdr_data.m_sbuff[1];
			batch_p[0].entry = g_libsdr_data.m_sbuff[2];
			if (cmd == rSdProcBatchEx2)
				batch_p[0].value = g_libsdr_data.m_sbuff[5];
			retptr = (void *)(uiptr)g_libsdr_data.m_sbuff[3];
			retptr_sz = retptr ? g_libsdr_data.m_sbuff[4] : (int)sizeof(ret);
			if (!retptr)
				retptr = &ret;
			sceSifCallRpc(&g_libsdr_data.m_cd, cmd, blocking, &g_libsdr_data.m_sbuff[1], sizeof(sceSdBatch) * (g_libsdr_data.m_sbuff[2] + 1), retptr, retptr_sz, endfunc, &g_libsdr_data.m_sbuff[0]);
			ret = *retptr;
			break;
		}
	case rSdUserCommand0:
	case rSdUserCommand1:
	case rSdUserCommand2:
	case rSdUserCommand3:
	case rSdUserCommand4:
	case rSdUserCommand5:
	case rSdUserCommand6:
	case rSdUserCommand7:
	case rSdUserCommand8:
	case rSdUserCommand9:
	case rSdUserCommandA:
	case rSdUserCommandB:
	case rSdUserCommandC:
	case rSdUserCommandD:
	case rSdUserCommandE:
	case rSdUserCommandF:
		sceSifCallRpc(&g_libsdr_data.m_cd, cmd, blocking, &g_libsdr_data.m_sbuff[1], g_libsdr_data.m_sbuff[2], &g_libsdr_data.m_sbuff[0], 16, endfunc, &g_libsdr_data.m_sbuff[0]);
		ret = g_libsdr_data.m_sbuff[0];
		break;
	default:
		sceSifCallRpc(&g_libsdr_data.m_cd, cmd, blocking, &g_libsdr_data.m_sbuff[0], sizeof(g_libsdr_data.m_sbuff), &g_libsdr_data.m_sbuff[0], 16, endfunc, &g_libsdr_data.m_sbuff[0]);
		ret = g_libsdr_data.m_sbuff[0];
		break;
	}

	switch (cmd)
	{
	case rSdSetTransCallback:
	case rSdSetIRQCallback:
	case rSdSetTransIntrHandler:
	case rSdSetSpu2IntrHandler:
		ret = (int)(uiptr)old_handler;
		break;
	default:
		break;
	}

	return ret;
}
