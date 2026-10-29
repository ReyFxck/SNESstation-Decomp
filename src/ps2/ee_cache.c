/*
 * SNES Station v0.23 -- EE data-cache synchronization leaves.
 * Target: 0x001ab3c0..0x001ab4dc.
 *
 * Independently reconstructed from the target R5900 instruction stream.
 * Host validation injects CP0 TagLo reads and cache operations as callbacks;
 * the target algorithm itself (both ways, line range and interrupt wrapper) is
 * preserved without pretending that a normal host can execute EE cache ops.
 */
#include <stddef.h>
#include <stdint.h>

typedef uint32_t (*SnesCacheTagRead)(uintptr_t probe_address, void *opaque);
typedef void (*SnesCacheOp)(uintptr_t probe_address, unsigned op, void *opaque);

/* 0x001ab440: interrupt-free inner cache walk. */
void iSyncDCache_001ab440(uintptr_t start, uintptr_t end,
                          SnesCacheTagRead read_tag,
                          SnesCacheOp cache_op, void *opaque)
{
    uintptr_t index;

    for (index = 0; index < 0x1000u; index += 0x40u) {
        unsigned way;
        for (way = 0; way < 2u; ++way) {
            uintptr_t probe = index + way;
            uint32_t tag = read_tag != NULL ? read_tag(probe, opaque) : 0u;
            uintptr_t line = ((uintptr_t)tag & UINT32_C(0xfffff000)) + index;

            /* Target uses unsigned comparisons: start <= line <= end. */
            if (line >= start && line <= end && cache_op != NULL)
                cache_op(probe, 0x14u, opaque);
        }
    }
}

/*
 * 0x001ab3c0: save CP0 Status bit 0x10000, conditionally DIntr(), align both
 * bounds down to 64-byte lines, walk both D-cache ways, conditionally EIntr().
 */
void SyncDCache_001ab3c0(uintptr_t start, uintptr_t end,
                         uint32_t cp0_status,
                         void (*dintr)(void *), void (*eintr)(void *),
                         SnesCacheTagRead read_tag,
                         SnesCacheOp cache_op, void *opaque)
{
    int restore_interrupts = (cp0_status & UINT32_C(0x00010000)) != 0;

    if (restore_interrupts && dintr != NULL)
        dintr(opaque);

    iSyncDCache_001ab440(start & ~(uintptr_t)0x3fu,
                         end & ~(uintptr_t)0x3fu,
                         read_tag, cache_op, opaque);

    if (restore_interrupts && eintr != NULL)
        eintr(opaque);
}
