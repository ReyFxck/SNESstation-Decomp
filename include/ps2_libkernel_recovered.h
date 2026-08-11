#ifndef SNESSTATION_PS2_LIBKERNEL_RECOVERED_H
#define SNESSTATION_PS2_LIBKERNEL_RECOVERED_H

#include <stdint.h>
#include <stddef.h>

/* Explicit 32-bit EE address used when host-validation pointer width differs. */
typedef uint32_t ee_addr32_t;

static inline ee_addr32_t ee_addr32_from_ptr(const void *p)
{
    return (ee_addr32_t)(uintptr_t)p;
}

static inline void *ee_ptr_from_addr32(ee_addr32_t p)
{
    return (void *)(uintptr_t)p;
}

/* SifRpcClientData_t layout from the old PS2DEV ABI: sizeof == 0x28. */
typedef struct SifRpcClientData32 {
    ee_addr32_t pkt_addr;      /* +0x00 */
    uint32_t rpc_id;           /* +0x04 */
    int32_t sema_id;           /* +0x08 */
    uint32_t mode;             /* +0x0c */
    uint32_t command;          /* +0x10 */
    ee_addr32_t buff;          /* +0x14 */
    ee_addr32_t cbuff;         /* +0x18 */
    ee_addr32_t end_function;  /* +0x1c */
    ee_addr32_t end_param;     /* +0x20 */
    ee_addr32_t server;        /* +0x24 */
} SifRpcClientData32;

typedef struct ee_sema32 {
    int32_t count;
    int32_t max_count;
    int32_t init_count;
    int32_t wait_threads;
    uint32_t attr;
    uint32_t option;
} ee_sema32;

typedef struct SifDmaTransfer32 {
    ee_addr32_t src;
    ee_addr32_t dest;
    int32_t size;
    int32_t attr;
} SifDmaTransfer32;

typedef char recovered_assert_rpc_client_size[(sizeof(SifRpcClientData32) == 0x28) ? 1 : -1];
typedef char recovered_assert_dma_transfer_size[(sizeof(SifDmaTransfer32) == 0x10) ? 1 : -1];

#endif
