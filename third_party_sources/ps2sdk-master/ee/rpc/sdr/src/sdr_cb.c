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

struct libsdr_cb_data
{
	int m_cb_thid;
	char m_cb_thstack[4096] __attribute__((__aligned__(64)));
	SifRpcServerData_t m_sd;
	SifRpcDataQueue_t m_qd;
	SdrEECBData m_eecb_data __attribute__((__aligned__(64)));
	SdIntrCallback m_trans_intr_callback[2];
	SdIntrCallback m_irq_intr_callback;
	sceSdTransIntrHandler m_trans_intr_handler[2];
	void *m_trans_intr_handler_userdata[2];
	sceSdSpu2IntrHandler m_spu2_intr_handler;
	void *m_spu2_intr_handler_userdata;
};

extern struct libsdr_cb_data g_libsdr_cb_data;

static void *sdr_cb_rpcproc(int fno, void *buf, int bufsz)
{
	SdrEECBData *cbdata;

	(void)fno;
	(void)bufsz;
	cbdata = (SdrEECBData *)buf;
	if (cbdata->mode & SDR_CB_DMA0)
		if (g_libsdr_cb_data.m_trans_intr_callback[0])
			g_libsdr_cb_data.m_trans_intr_callback[0](NULL);
	if (cbdata->mode & SDR_CB_DMA1)
		if (g_libsdr_cb_data.m_trans_intr_callback[1])
			g_libsdr_cb_data.m_trans_intr_callback[1](NULL);
	if (cbdata->mode & SDR_CB_IRQ)
		if (g_libsdr_cb_data.m_irq_intr_callback)
			g_libsdr_cb_data.m_irq_intr_callback(NULL);
	if (cbdata->mode & SDR_CB_DMA0INT)
		if (g_libsdr_cb_data.m_trans_intr_handler[0])
			g_libsdr_cb_data.m_trans_intr_handler[0](0, g_libsdr_cb_data.m_trans_intr_handler_userdata[0]);
	if (cbdata->mode & SDR_CB_DMA1INT)
		if (g_libsdr_cb_data.m_trans_intr_handler[1])
			g_libsdr_cb_data.m_trans_intr_handler[1](1, g_libsdr_cb_data.m_trans_intr_handler_userdata[1]);
	if (cbdata->mode & SDR_CB_IRQINT)
		if (g_libsdr_cb_data.m_spu2_intr_handler)
			g_libsdr_cb_data.m_spu2_intr_handler(cbdata->voice_bit, g_libsdr_cb_data.m_spu2_intr_handler_userdata);
	return 0;
}

static void sdr_cb_thproc(void *userdata)
{
	(void)userdata;

	sceSifInitRpc(0);
	sceSifSetRpcQueue(&g_libsdr_cb_data.m_qd, GetThreadId());
	sceSifRegisterRpc(&g_libsdr_cb_data.m_sd, sce_SDRST_CB, &sdr_cb_rpcproc, &g_libsdr_cb_data.m_eecb_data, NULL, NULL, &g_libsdr_cb_data.m_qd);
	sceSifRpcLoop(&g_libsdr_cb_data.m_qd);
}

int sceSdRemoteCallbackInit(int pri)
{
	ee_thread_t thparam;

	if (HasIopRebootedSinceLastCall())
		sceSdRemoteCallbackQuit();

	if (g_libsdr_cb_data.m_cb_thid > 0)
		return -1;

	memset(&g_libsdr_cb_data, 0, sizeof(g_libsdr_cb_data));

	sceSdRemote(1, SDR_CMD_CB_INIT);

	thparam.func = &sdr_cb_thproc;
	thparam.stack = g_libsdr_cb_data.m_cb_thstack;
	thparam.stack_size = sizeof(g_libsdr_cb_data.m_cb_thstack);
	thparam.gp_reg = &_gp;
	thparam.initial_priority = pri;
	thparam.attr = 0;
	thparam.option = 0;

	g_libsdr_cb_data.m_cb_thid = CreateThread(&thparam);
	if (g_libsdr_cb_data.m_cb_thid <= 0)
		return -1;

	if (StartThread(g_libsdr_cb_data.m_cb_thid, NULL) < 0)
		return -1;

	return g_libsdr_cb_data.m_cb_thid;
}

int sceSdRemoteCallbackQuit(void)
{
	int ret;

	if (g_libsdr_cb_data.m_cb_thid <= 0)
		return -1;

	sceSdRemote(1, rSdSetIRQCallback, NULL);
	sceSdRemote(1, rSdSetTransCallback, 0, NULL);
	sceSdRemote(1, rSdSetTransCallback, 1, NULL);
	sceSdRemote(1, rSdSetSpu2IntrHandler, NULL, NULL);
	sceSdRemote(1, rSdSetTransIntrHandler, 0, NULL, NULL);
	sceSdRemote(1, rSdSetTransIntrHandler, 1, NULL, NULL);

	sceSdRemote(1, SDR_CMD_CB_DEINIT);

	ret = 0;
	if (!sceSifRemoveRpc(&g_libsdr_cb_data.m_sd, &g_libsdr_cb_data.m_qd))
		ret = -1;
	if (!sceSifRemoveRpcQueue(&g_libsdr_cb_data.m_qd))
		ret = -1;
	if (TerminateThread(g_libsdr_cb_data.m_cb_thid) < 0)
		ret = -1;
	if (DeleteThread(g_libsdr_cb_data.m_cb_thid) < 0)
		ret = -1;

	memset(&g_libsdr_cb_data, 0, sizeof(g_libsdr_cb_data));
	return ret;
}

SdIntrCallback SdrSetTransInterruptCallback(int idx, SdIntrCallback handler)
{
	SdIntrCallback old_handler;

	old_handler = g_libsdr_cb_data.m_trans_intr_callback[idx];
	g_libsdr_cb_data.m_trans_intr_callback[idx] = handler;

	return old_handler;
}

SdIntrCallback SdrSetIRQInterruptCallback(SdIntrCallback handler)
{
	SdIntrCallback old_handler;

	old_handler = g_libsdr_cb_data.m_irq_intr_callback;
	g_libsdr_cb_data.m_irq_intr_callback = handler;

	return old_handler;
}

// cppcheck-suppress funcArgNamesDifferentUnnamed
sceSdTransIntrHandler SdrSetTransInterruptHandler(int idx, sceSdTransIntrHandler handler, void *handler_userdata)
{
	sceSdTransIntrHandler old_handler;

	old_handler = g_libsdr_cb_data.m_trans_intr_handler[idx];
	g_libsdr_cb_data.m_trans_intr_handler[idx] = handler;
	g_libsdr_cb_data.m_trans_intr_handler_userdata[idx] = handler_userdata;

	return old_handler;
}

// cppcheck-suppress funcArgNamesDifferentUnnamed
sceSdSpu2IntrHandler SdrSetIRQInterruptHandler(sceSdSpu2IntrHandler handler, void *handler_userdata)
{
	sceSdSpu2IntrHandler old_handler;

	old_handler = g_libsdr_cb_data.m_spu2_intr_handler;
	g_libsdr_cb_data.m_spu2_intr_handler = handler;
	g_libsdr_cb_data.m_spu2_intr_handler_userdata = handler_userdata;

	return old_handler;
}
