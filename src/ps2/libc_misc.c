/* Compact libc leaves recovered from SNES Station v0.23. */
#include <stddef.h>
#include <stdint.h>

/* 0x00107578: fatal assertion/abort sink; target spins forever. */
void snes_fatal_spin_00107578(void){ for(;;){} }

/* 0x00108a34: target 64-bit LCG, returning the high 31 bits. */
uint32_t snes_rand_00108a34(uint64_t*state){*state=*state*UINT64_C(0x5851f42d4c957f2d)+UINT64_C(1);return ((uint32_t)(*state>>32))&UINT32_C(0x7fffffff);}
