/*
 * Small address-labelled leaves recovered directly from SNES Station v0.23.
 * Behavioural source models; names stay address-based where no original symbol is proven.
 */
#include <stddef.h>
#include <stdint.h>

void snes_leaf_00101858(void) {}
void snes_leaf_00101860(void) {}
void snes_leaf_00101878(void) {}
void snes_leaf_00101888(void) {}
void snes_leaf_001018fc(void) {}
void snes_leaf_00101904(void) {}
void snes_leaf_0010190c(void) {}
void snes_leaf_0010191c(void) {}
void snes_leaf_001056b0(void) {}
void snes_leaf_0012feb4(void) {}
void snes_leaf_0012febc(void) {}
void snes_leaf_0015e190(void) {}
void snes_leaf_00174720(void) {}
void snes_leaf_0018339c(void) {}
int snes_leaf_00101880(void) { return 0; }
int snes_leaf_00101914(void) { return 0; }
int snes_leaf_001019a0(void) { return 0; }
int snes_leaf_00104e48(void) { return 0; }
int snes_leaf_00104e50(void) { return 0; }
int snes_leaf_001056b8(void) { return 1; }
int snes_leaf_001701fc(void) { return 0x81; }
int snes_leaf_00193290(int32_t value) { if (value < 0) return 1; return 0; }

typedef struct SnesLeafObject3 { int32_t word0, word1, word2; } SnesLeafObject3;
extern char snes_vtable_00426c28[];
extern char snes_data_001b0000[];
void snes_leaf_0010421c(SnesLeafObject3 *obj) { obj->word1=0; obj->word2=0; obj->word0=(int32_t)(intptr_t)snes_vtable_00426c28; }
int32_t snes_leaf_0010e33c(void) { return (int32_t)(intptr_t)(snes_data_001b0000 + 0x1b98); }
int32_t snes_leaf_0010e348(void) { return (int32_t)(intptr_t)(snes_data_001b0000 + 0x1be0); }
int32_t snes_leaf_0010e354(void) { return (int32_t)(intptr_t)(snes_data_001b0000 + 0x1c00); }
int snes_leaf_00158b3c(uint32_t value) { if ((value&0x80u)==0) return 0; return ((value-0x20u)&0x40u)!=0; }
extern uint8_t *snes_ram_003f4be8;
extern uint32_t snes_state_0035e2c4;
uint8_t snes_leaf_00158b5c(uint32_t address) { return snes_ram_003f4be8[address&0x1fffu]; }
uint32_t snes_leaf_00158fd0(void) { return snes_state_0035e2c4; }
uint32_t snes_leaf_00158fdc(uint32_t offset) { return snes_state_0035e2c4+(offset&0xffffu); }
uint32_t snes_leaf_00171de8(uint32_t value,uint32_t mode){ if(mode==2)return value<<1; if(mode==3)return value<<2; return value; }
int32_t snes_leaf_00174204(int32_t value,int32_t *target){ if(value>=128)value=127; else if(value< -128)value=-128; *target=value; return value; }
uint32_t snes_leaf_001742e8(uint32_t value,uint32_t *target){ value&=0xfeu; *target=value; return value; }
void snes_leaf_00174860(uint8_t *table,uint32_t index,uint32_t value){ *(uint32_t *)(void *)(table+index*224u+0x34u)=value; }
uint8_t snes_leaf_0017487c(uint8_t *target,uint32_t replacement){ uint8_t old=*target; *target=(uint8_t)replacement; return old; }
void snes_leaf_0012e2c4(int16_t *s){*s=0;}
void snes_leaf_0012e2d0(int16_t *s){*s=0x100;}
typedef struct { uint8_t a[0x58]; uint32_t f58,f5c; uint8_t b[0x5ec-0x60]; uint8_t f5ec; } SnesLeafState12fe9c;
void snes_leaf_0012fe9c(SnesLeafState12fe9c *s){s->f5ec=0;s->f5c=0;s->f58=0;}
void snes_leaf_00173e4c(uint8_t *s,uint32_t bit){s[0x87]|=(uint8_t)(1u<<(bit&31u));}
void snes_leaf_00183660(uint8_t *s){s[0x0f]=0xff;s[0x10]=0;}

int snes_leaf_00101814(uint32_t selector,uint32_t command,uint32_t *target){if(selector==1u&&command==0xffffu)*target=0;return 1;}
int snes_leaf_001041c0(uint32_t selector,uint32_t command,uint32_t *target){if(selector==1u&&command==0xffffu)*target=0;return 1;}
int snes_leaf_00103cb0(const uint8_t *records,uint32_t index){return (records[index*0x90u+8u]&2u)!=0;}
static int leaf_relation_a(uint32_t a,uint32_t b,uint32_t c,uint32_t d){unsigned ac=(a==c)+(a==d),bc=(b==c)+(b==d);return (int)(ac<2u)-(int)(bc<2u);}
int snes_leaf_0010a768(uint32_t a,uint32_t b,uint32_t c,uint32_t d){return leaf_relation_a(a,b,c,d);}
int snes_leaf_0010a7fc(uint32_t a,uint32_t b,uint32_t c,uint32_t d){return leaf_relation_a(a,b,c,d);}
int snes_leaf_0010a7ac(uint32_t a,uint32_t b,uint32_t c,uint32_t d){unsigned ac=(a==c)+(a==d),bc=(b==c)+(b==d);return (int)(bc<2u)-(int)(ac>=2u);}
void snes_leaf_001140e0(uint32_t *dst,uint32_t a,uint32_t b,uint32_t c){dst[0]=a;dst[1]=b;dst[2]=c;}
typedef struct {uint32_t flags;uint8_t p[2],mask06,pending07;uint32_t pad08,counter0c;uint8_t pad10[0x54];uint32_t active64;} SnesLeaf116State;
void snes_leaf_00116128(SnesLeaf116State*s,uint8_t mask){s->flags|=0x800u;s->mask06|=mask;s->active64=3;if(s->pending07){s->active64=0;s->counter0c++;s->pending07=0;}}
void snes_leaf_00116174(SnesLeaf116State*s,uint8_t mask){s->mask06&=(uint8_t)~mask;if(!s->mask06)s->flags&=~0x800u;}
int16_t snes_leaf_0012c13c(int16_t a,int16_t b){return (int16_t)(((int32_t)a*b)>>15);}
int16_t snes_leaf_0012c160(int16_t a,int16_t b){return (int16_t)((((int32_t)a*b)>>15)+1);}
int16_t snes_leaf_0012de60(int16_t x0,int16_t y0,int16_t x1,int16_t y1,int16_t x2,int16_t y2){return (int16_t)(((int32_t)x0*y0+(int32_t)x1*y1+(int32_t)x2*y2)>>15);}
int16_t snes_leaf_0012deb0(int16_t x0,int16_t y0,int16_t x1,int16_t y1,int16_t x2,int16_t y2){return (int16_t)(((int32_t)x0*y0+(int32_t)x1*y1+(int32_t)x2*y2)>>15);}
uint64_t snes_leaf_0012df50(int16_t a,int16_t b,int16_t c,uint16_t*lo,uint16_t*hi){int64_t sum=(int64_t)a*a+(int64_t)b*b+(int64_t)c*c;uint64_t d=(uint64_t)(sum*2);*lo=(uint16_t)d;*hi=(uint16_t)(d>>16);return d;}
int16_t snes_leaf_0012dfb0(int16_t a,int16_t b,int16_t c,int16_t sub){int32_t sum=(int32_t)a*a+(int32_t)b*b+(int32_t)c*c;return (int16_t)((sum-sub)>>15);}
int16_t snes_leaf_0012dffc(int16_t a,int16_t b,int16_t c,int16_t sub){int32_t sum=(int32_t)a*a+(int32_t)b*b+(int32_t)c*c;return (int16_t)(((sum-sub)>>15)+1);}
void snes_leaf_0012e568(const uint8_t*src,uint8_t*dst,int count){int i;for(i=0;i<count;i++){uint8_t v=src[i];dst[count-1-i]=(uint8_t)((v<<4)|(v>>4));}}
void snes_leaf_0012fe5c(uint32_t*mask,uint16_t value){if((value&0xfu)==0xfu)*mask|=UINT32_C(1)<<((value&0x1f0u)>>4);}
uint32_t snes_leaf_00148338(uint32_t a,uint32_t b,uint32_t c,uint32_t d){uint32_t low=(a&0xc63u)+(b&0xc63u)+(c&0xc63u)+(d&0xc63u);uint32_t high=((a>>2)&0x1ce7u)+((b>>2)&0x1ce7u)+((c>>2)&0x1ce7u)+((d>>2)&0x1ce7u);return high+((low>>2)&0xc63u);}
int snes_leaf_00150c90(const uint8_t*bytes,int count){int i;if(count<=0)return 1;for(i=0;i<count;i++)if((uint8_t)(bytes[i]-0x20u)>=0x5fu)return 0;return 1;}
void snes_leaf_001535c0(uint8_t*object,uint8_t replacement){uint32_t i;for(i=0x800;i<0x1000;i++)if(object[i+0xa028u])object[i+0x8028u]=replacement;}
void snes_leaf_00156884(uint32_t*slots,uint32_t speed){uint32_t v=(speed&0x80u)?4u:14u;slots[0]=slots[1]=slots[2]=slots[3]=v;}
const char*snes_leaf_001568c4(int pal){return pal?"PAL":"NTSC";}
const char*snes_leaf_00156914(int hirom){return hirom?"HiROM":"LoROM";}
void snes_leaf_0016fcd4(int16_t scale,int16_t x,int16_t y,int32_t*outx,int32_t*outy){*outx=(int32_t)x*scale*2;*outy=(int32_t)y*scale*2;}
void snes_leaf_00173dfc(uint32_t bit,uint32_t*obj,uint8_t*ppu){uint8_t mask=(uint8_t)(1u<<(bit&7u)),inv=(uint8_t)~mask;obj[0]=0;obj[0xa4u/4u]=0;ppu[0x87]|=mask;ppu[0x57]&=inv;ppu[0x67]&=inv;ppu[0x06]&=inv;}
uint8_t snes_leaf_0017ec24(uint8_t*record,uint32_t*counter,uint8_t*shadow){uint8_t v=record[2];v=(uint8_t)((v<<4)|(v>>4));++*counter;*shadow=v;record[2]=v;return v;}
