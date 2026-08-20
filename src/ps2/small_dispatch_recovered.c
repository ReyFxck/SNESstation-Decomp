#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef void(*VF)(void*);typedef int(*IF)(void*);
extern void *gzopen(const char *, const char *);
int snes_dispatch_00101890(const char*p,uint8_t read,void**out){
    if(read)return (*out=gzopen(p,"rb"))!=NULL;
    return (*out=gzopen(p,"wb"))!=NULL;
}
void snes_dispatch_001018e0(int h,void(*fn)(int,void*),void*o){if(fn)fn(h,o);}
int snes_dispatch_00103c7c(const char*p,void*b,int(*fn)(const char*,const char*,unsigned,void*,unsigned,const char*,void*),void*o){return fn?fn(p,".ZIP .SMC .SFC .SWC .FIG .058 .BIN",3,b,0xfa0u,p,o):0;}
int snes_dispatch_001056c0(VF fn,void*o){if(fn)fn(o);return 1;}
char*snes_dispatch_00105718(char*b,size_t n){if(n)snprintf(b,n,"%s","cdrom0:\\ROMS\\SNAP");return b;}
void snes_dispatch_00105d30(unsigned m,void*mem,void*(*pf)(void*),void(*lf)(void*,void*,void*),void*o){if(m==1&&pf&&lf)lf(mem,pf(o),o);}
/* `mod` is the ProTracker M.K. asset at 0x002ec540. Its exact logical size is
 * 0x3640c; the target updates the adjacent size word to aligned 0x36410. */
void snes_dispatch_00105d78(uint32_t*n,void(*v)(uint32_t,void*),VF q,void(*mi)(uint32_t,void*),void(*mp)(uint32_t,void*),void(*ml)(const void*,uint32_t,void*),const void*mod,void(*play)(uint32_t,void*),void(*vol)(uint32_t,void*),void*o){uint32_t x=*n;if(v)v(0,o);if(q)q(o);x=(x+15u)&~15u;*n=x;if(mi)mi(0,o);if(mp)mp(0,o);if(ml)ml(mod,x,o);if(play)play(1,o);if(vol)vol(0x3fffu,o);}
void snes_dispatch_00105e04(void(*mv)(uint32_t,void*),VF mq,void(*si)(uint32_t,void*),void(*sv)(uint32_t,void*),void*o){if(mv)mv(0,o);if(mq)mq(o);if(si)si(0,o);if(sv)sv(0x3fffu,o);}
void snes_dispatch_0010c300(uint8_t*b){memset(b,0,0x2000u);}uint8_t snes_dispatch_0010c328(const uint8_t*b,uint16_t off){return b[(uint16_t)(off-0x6000u)];}
void snes_dispatch_001159f4(uint32_t*s,void(*r)(void*,void*),void*o){*s=0;if(r)r(s,o);}
void snes_dispatch_0011c050(uint16_t v,uint32_t a,uint8_t s[2],void(*w)(uint8_t,uint32_t,void*),void*o){if(w)w((uint8_t)v,a,o);s[1]=(uint8_t)(v>>8);s[0]=v!=0;}
void snes_dispatch_0012e688(uint8_t*i,VF f,void*o){if(!*i){if(f)f(o);*i=1;}}
typedef struct{uint8_t a,b;uint16_t p;uint32_t c,d,e,f;}S12;
void snes_dispatch_0012e6c4(uint8_t*i,VF f,S12*s,void*o){snes_dispatch_0012e688(i,f,o);s->a=s->b=1;s->c=s->d=s->e=s->f=0;}
uint32_t snes_dispatch_0012e704(uint32_t a,uint32_t(*f)(uint16_t,void*),void*o){return f?f((uint16_t)a,o):0;}
void snes_dispatch_0012e728(uint32_t v,uint32_t a,void(*f)(uint8_t,uint16_t,void*),void*o){if(f)f((uint8_t)v,(uint16_t)a,o);}
void snes_dispatch_0012fec4(VF h,uint8_t*f,uint32_t*w,void*o){if(h)h(o);*f=0;*w=0;}
typedef struct{uint32_t pc_index,flags,src18,src30,*pc_ptr,current,saved,fast;uint16_t fetch_index;uint8_t open_bus,*fetch;}Op;
static void ot(Op*s,uint32_t sp){if((uintptr_t)s->pc_ptr==(uintptr_t)sp&&s->fetch)s->open_bus=s->fetch[s->fetch_index];s->flags&=UINT32_C(0xffffecff);s->pc_ptr=(uint32_t*)s;}
void snes_dispatch_0013e014(Op*s,uint32_t p){uint32_t v=s->src18;++s->pc_index;if(s->pc_ptr)*s->pc_ptr=v;s->fast=(v&0x80u)<<16;s->saved=s->current=v;ot(s,p);}
void snes_dispatch_0013e024(Op*s,uint32_t next,uint32_t v,uint32_t p){s->pc_index=next;if(s->pc_ptr)*s->pc_ptr=v;s->fast=(v&0x80u)<<16;s->saved=s->current=v;ot(s,p);}
void snes_dispatch_0013e3a8(Op*s,uint32_t p){uint32_t v=s->src30;++s->pc_index;if(s->pc_ptr)*s->pc_ptr=v;s->fast=(v&0x80u)<<16;s->saved=s->current=v;ot(s,p);}
void snes_dispatch_0013fc10(Op*s,uint32_t m){s->flags&=m;s->pc_ptr=(uint32_t*)s;}
void snes_dispatch_00140040(Op*s,uint32_t v,uint32_t p){v^=0x0e;++s->pc_index;if(s->pc_ptr)*s->pc_ptr=v;s->saved=s->current=v;ot(s,p);}
void snes_dispatch_0014080c(Op*s,uint32_t*c){--*c;++s->pc_index;s->flags&=UINT32_C(0xffffecff);s->saved=s->current=*c;s->pc_ptr=(uint32_t*)s;}
void snes_dispatch_00140c30(Op*s,const uint8_t*b,uint32_t i,uint32_t pc,uint32_t fl,uint32_t cur){s->open_bus=b[i];s->pc_index=pc;s->flags=fl;s->current=cur;s->pc_ptr=(uint32_t*)s;}
void snes_dispatch_00158ff0(uint8_t*a,uint8_t**w){*w=a+0x6000;memset(a+0x6000,0,0x2000);memset(a,0,8);a[8]=0;a[9]=0x18;}
void snes_dispatch_0016fa7c(const uint8_t*t,size_t st,size_t off,void(*f)(unsigned,uint8_t,void*),void*o){unsigned i;for(i=0;i<4;i++)if(f)f(i,t[i*st+off],o);}
void snes_dispatch_0016fc48(VF f,void*o){if(f)f(o);}void snes_dispatch_0016fc6c(uint32_t a,uint32_t b,void(*f)(uint32_t,uint8_t,void*),void*o){if(f)f(a,(uint8_t)b,o);}
void snes_dispatch_00170204(uint32_t a,uint32_t v,void(*f)(uint32_t,uint8_t,void*),void*o){if(f)f(a,(uint8_t)v,o);}
void snes_dispatch_00176578(uint32_t a,uint32_t b,void(*f)(uint32_t,uint32_t,uint32_t,void*),void*o){if(f)f(a,b,0,o);}
void snes_dispatch_00183678(uint8_t s[0x1c]){memset(s,0,0x1c);s[0]=1;s[0x0f]=0xff;}
int snes_dispatch_0018faa4(void*file,void*info,char*name,uint32_t ns,void*ex,uint32_t es,char*com,uint32_t cs,int(*f)(void*,void*,void*,char*,uint32_t,void*,uint32_t,char*,uint32_t,void*),void*o){return f?f(file,info,NULL,name,ns,ex,es,com,cs,o):-1;}
void snes_dispatch_0019c5cc(VF f,void*o){if(f)f(o);}
void snes_dispatch_001a51d0(uint32_t*row,void*ctx,uint8_t sel,uint64_t*const*slots,void(*prep)(void*,void*),void*o){uint64_t v;if(prep)prep(row,o);(void)ctx;v=*slots[sel];row[0x144u/4u]=(uint32_t)((int64_t)v&~INT64_C(1));}
