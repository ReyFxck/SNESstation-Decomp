/* ROM map/metadata helpers recovered from the v0.23 EE image. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* V92: all three target call sites branch to the existing sprintf provider.
 * The bounded numeric outputs fit the source model's 32/32/16-byte buffers. */
/* 0x00153608 */
void CMemory_map_WriteProtectROM_00153608(uintptr_t*map,uintptr_t*write_map,const uint8_t*block_is_rom){unsigned i;for(i=0;i<4096;i++)write_map[i]=block_is_rom[i]?5u:map[i];}
/* 0x00156934 */
const char*CMemory_StaticRAMSize_00156934(uint8_t size,uint32_t mask,char out[32]){(void)mask;if(size>=17){strcpy(out,"Corrupt");return out;}if(size==0){strcpy(out,"0Kb");return out;}sprintf(out,"%uKb",1u<<size);return out;}
/* 0x00156994 */
const char*CMemory_Size_00156994(uint8_t size,char out[32]){if(size<7||size>30){strcpy(out,"Corrupt");return out;}sprintf(out,"%uMbits",1u<<(size-7));return out;}
/* 0x00156c0c */
const char*CMemory_MapMode_00156c0c(uint8_t mode,char out[16]){sprintf(out,"%02X",mode&0xefu);return out;}
