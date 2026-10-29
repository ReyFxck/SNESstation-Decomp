/* Compact SjPCM / AmigaMod EE-side RPC wrappers recovered from v0.23. */
#include <stddef.h>
#include <stdint.h>
typedef int(*RpcFn)(int cmd,const void*send,unsigned ss,void*recv,unsigned rs,void*opaque);
typedef struct{RpcFn call;void*opaque;}AudioRpc;
static int call(AudioRpc*r,int c,const void*s,unsigned ss,void*d,unsigned ds){return r&&r->call?r->call(c,s,ss,d,ds,r->opaque):-1;}
extern char *strrchr(const char *text, int needle);
extern int strcasecmp(const char *left, const char *right);

/* 0x00106bcc: recognize an SRAM file extension; this is not SjPCM_Init. */
int is_sram_extension_00106bcc(const char *path)
{
    char *extension = strrchr(path, '.');

    if (extension == NULL)
        return 0;
    return strcasecmp(extension + 1, "srm") == 0;
}
/* 0x001077f8 */ int SjPCM_Quit_001077f8(AudioRpc*r){return call(r,1,NULL,0,NULL,0);}
/* 0x001078f8 */ int SjPCM_Setvol_001078f8(AudioRpc*r,uint32_t v){return call(r,2,&v,4,NULL,0);}
/* 0x00107b1c */ int amigaModInit_00107b1c(AudioRpc*r,uint32_t mode){return call(r,0,&mode,4,NULL,0);}
/* 0x00107b7c */ int amigaModPause_00107b7c(AudioRpc*r,uint32_t p){return call(r,1,&p,4,NULL,0);}
/* 0x00107c64 */ int amigaModLoad_00107c64(AudioRpc*r,const void*data,uint32_t bytes){uint32_t a[2]={(uint32_t)(uintptr_t)data,bytes};return call(r,2,a,8,NULL,0);}
/* 0x00107d44 */ int amigaModPlay_00107d44(AudioRpc*r,uint32_t p){return call(r,3,&p,4,NULL,0);}
/* 0x00107db4 */ int amigaModSetVolume_00107db4(AudioRpc*r,uint32_t v){return call(r,4,&v,4,NULL,0);}
/* 0x00107e14 */ int amigaModGetPosition_00107e14(AudioRpc*r,uint32_t*out){return call(r,5,NULL,0,out,4);}
/* 0x00107f18 */ int amigaModQuit_00107f18(AudioRpc*r){return call(r,6,NULL,0,NULL,0);}
