/* NEW/XPADMAN multitap client leaves recovered from v0.23. */
#include <stddef.h>
#include <stdint.h>
typedef int(*MtapRpcFn)(int cmd,const uint32_t*send,unsigned words,uint32_t*recv,void*opaque);
typedef struct{MtapRpcFn call;void*opaque;}MtapRpc;
static int mt(MtapRpc*r,int c,uint32_t a,uint32_t b,uint32_t*out){uint32_t s[2]={a,b},v=0;int rc=r&&r->call?r->call(c,s,2,&v,r->opaque):-1;if(out)*out=v;return rc<0?rc:(int)v;}
/* 0x00104998 */ int mtapInit_00104998(MtapRpc*r){return mt(r,0,0,0,NULL);}
/* 0x001049f0 */ int mtapPortOpen_001049f0(MtapRpc*r,int port){return mt(r,1,(uint32_t)port,0,NULL);}
/* 0x00104a54 */ int mtapPortClose_00104a54(MtapRpc*r,int port){return mt(r,2,(uint32_t)port,0,NULL);}
/* 0x00104bbc */ int mtapGetConnection_00104bbc(MtapRpc*r,int port){return mt(r,3,(uint32_t)port,0,NULL);}
/* 0x00104e58 */ int mtapChangeSlot_00104e58(MtapRpc*r,int port,int slot){return mt(r,4,(uint32_t)port,(uint32_t)slot,NULL);}
