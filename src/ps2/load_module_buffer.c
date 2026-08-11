/*
 * SNES Station v0.23 WIP reconstruction
 * Original function VA: 0x00104e7c
 *
 * Status: EQUIVALENT / structure recovered directly from assembly.
 * Exact compiler matching has not yet been attempted.
 */

#include <stdint.h>

typedef struct SifDmaTransfer {
    const void *src;
    void *dest;
    int size;
    int attr;
} SifDmaTransfer_t;

/* Provisional names independently identified from call behaviour. */
extern void *SifAllocIopHeap(int size);                 /* 0x0019d63c */
extern int   SifSetDma(SifDmaTransfer_t *dmat, int len);/* 0x0019cee0 */
extern int   SifDmaStat(int id);                        /* 0x0019ced0 */
extern int   SifLoadModuleBuffer(void *ptr, int arg_len,
                                 const char *args);      /* 0x0019d620 */
extern int   SifFreeIopHeap(void *ptr);                 /* 0x0019d6b8 */

int loadModuleBuffer(const void *module, int size, int arg_len,
                     const char *args)
{
    SifDmaTransfer_t dma;
    void *iop_addr;
    int dma_id;

    iop_addr = SifAllocIopHeap(size);

    dma.src  = module;
    dma.dest = iop_addr;
    dma.size = size;
    dma.attr = 0;

    dma_id = SifSetDma(&dma, 1);
    while (SifDmaStat(dma_id) >= 0) {
        /* original binary spins here */
    }

    /*
     * The v0.23 assembly does not preserve this return value across the
     * following free call.  That means the observable C return is the value
     * left by SifFreeIopHeap(), not the module loader's result.  This oddity
     * is intentionally kept because the goal is preservation, not cleanup.
     */
    SifLoadModuleBuffer(iop_addr, arg_len, args);
    return SifFreeIopHeap(iop_addr);
}
