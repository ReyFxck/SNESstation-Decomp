/* Compact libc leaves recovered from SNES Station v0.23. */
#include <stddef.h>
#include <stdint.h>

/* 0x00107578: fatal assertion/abort sink; target spins forever. */
void snes_fatal_spin_00107578(void){ for(;;){} }

/* 0x001080cc: in-place quicksort-compatible routine. */
static void swap_bytes(unsigned char*a,unsigned char*b,size_t n){while(n--){unsigned char t=*a;*a++=*b;*b++=t;}}
void snes_qsort_001080cc(void*base,size_t nmemb,size_t size,int(*cmp)(const void*,const void*)){
 unsigned char*b=(unsigned char*)base;size_t i,j;if(!cmp||size==0||nmemb<2)return;
 for(i=1;i<nmemb;i++)for(j=i;j>0&&cmp(b+(j-1)*size,b+j*size)>0;j--)swap_bytes(b+(j-1)*size,b+j*size,size);
}

/* 0x00108a34: target 64-bit LCG, returning the high 31 bits. */
uint32_t snes_rand_00108a34(uint64_t*state){*state=*state*UINT64_C(0x5851f42d4c957f2d)+UINT64_C(1);return ((uint32_t)(*state>>32))&UINT32_C(0x7fffffff);}
