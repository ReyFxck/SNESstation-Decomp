#include <stdint.h>
#include <stddef.h>

#include "../../include/ps2_libkernel_recovered.h"

/*
 * Old PS2DEV/PS2LIB heap allocator recovered from 0x0019e474..0x0019e85f.
 *
 * The target header is exactly 16 bytes because every pointer is a 32-bit EE
 * address.  This model therefore stores addresses explicitly instead of using
 * host-native pointers, which would silently double the structure on LP64.
 *
 * Unlike a later PS2SDK revision, the SNES Station target has no allocator
 * semaphore lock/unlock calls in these bodies.  That older behavior is kept.
 */

typedef struct heap_mem_header32 {
    ee_addr32_t ptr;   /* +0x00 user pointer */
    uint32_t size;     /* +0x04 user bytes */
    ee_addr32_t prev;  /* +0x08 header address */
    ee_addr32_t next;  /* +0x0c header address */
} heap_mem_header32;

typedef char recovered_assert_heap_header_size[
    (sizeof(heap_mem_header32) == 0x10) ? 1 : -1];

/* Target globals correspond to 0x00425a74 / 0x00425a78 / 0x00425a7c. */
static ee_addr32_t alloc_heap_base;
static ee_addr32_t alloc_heap_head;
static ee_addr32_t alloc_heap_tail;

/* Target: 0x0019f078, recovered separately later. */
extern ee_addr32_t ps2_sbrk_0019f078(int32_t increment);

static heap_mem_header32 *heap_header(ee_addr32_t address)
{
    return (heap_mem_header32 *)ee_ptr_from_addr32(address);
}

/* Target: 0x0019e474. */
ee_addr32_t heap_mem_fit_0019e474(ee_addr32_t head, uint32_t size)
{
    ee_addr32_t current = head;

    while (current != 0u) {
        heap_mem_header32 *cur = heap_header(current);
        ee_addr32_t next = cur->next;

        if (next != 0u) {
            uint32_t prev_top = cur->ptr + cur->size;
            uint32_t gap = next - prev_top;
            if (gap >= size)
                return current;
        }

        current = next;
    }

    return current;
}

/* Target: 0x0019e4b4. */
void *malloc_0019e4b4(size_t requested)
{
    uint32_t mem_size = (uint32_t)requested + 0x10u;
    ee_addr32_t mem_addr;
    ee_addr32_t user_addr = 0u;

    if ((mem_size & 0x0fu) != 0u)
        mem_size = (mem_size + 0x0fu) & ~0x0fu;

    if (alloc_heap_head == 0u) {
        if (alloc_heap_base == 0u) {
            /* Preserved target quirk: add the low alignment bits directly. */
            uint32_t align_bytes = ps2_sbrk_0019f078(0) & 0x0fu;
            (void)ps2_sbrk_0019f078((int32_t)align_bytes);
            alloc_heap_base = ps2_sbrk_0019f078(0);
        }

        mem_addr = ps2_sbrk_0019f078((int32_t)mem_size);
        if (mem_addr == 0xffffffffu)
            return NULL;

        user_addr = mem_addr + 0x10u;
        {
            heap_mem_header32 *node = heap_header(mem_addr);
            node->ptr = user_addr;
            node->size = mem_size - 0x10u;
            node->prev = 0u;
            node->next = 0u;
        }

        alloc_heap_head = mem_addr;
        alloc_heap_tail = mem_addr;
        return ee_ptr_from_addr32(user_addr);
    }

    /* Free space can exist before the current list head. */
    if (alloc_heap_base + mem_size < alloc_heap_head) {
        ee_addr32_t old_head = alloc_heap_head;
        heap_mem_header32 *node = heap_header(alloc_heap_base);
        heap_mem_header32 *next = heap_header(old_head);

        mem_addr = alloc_heap_base;
        user_addr = mem_addr + 0x10u;

        node->ptr = user_addr;
        node->size = mem_size - 0x10u;
        node->prev = 0u;
        node->next = old_head;
        next->prev = mem_addr;
        alloc_heap_head = mem_addr;

        return ee_ptr_from_addr32(user_addr);
    }

    {
        ee_addr32_t prev_addr = heap_mem_fit_0019e474(alloc_heap_head, mem_size);

        if (prev_addr != 0u) {
            heap_mem_header32 *prev = heap_header(prev_addr);
            ee_addr32_t next_addr = prev->next;
            heap_mem_header32 *next = heap_header(next_addr);
            heap_mem_header32 *node;

            mem_addr = prev->ptr + prev->size;
            user_addr = mem_addr + 0x10u;
            node = heap_header(mem_addr);

            node->ptr = user_addr;
            node->size = mem_size - 0x10u;
            node->prev = prev_addr;
            node->next = next_addr;
            next->prev = mem_addr;
            prev->next = mem_addr;

            return ee_ptr_from_addr32(user_addr);
        }
    }

    mem_addr = ps2_sbrk_0019f078((int32_t)mem_size);
    if (mem_addr == 0xffffffffu)
        return NULL;

    user_addr = mem_addr + 0x10u;
    {
        heap_mem_header32 *node = heap_header(mem_addr);
        heap_mem_header32 *tail = heap_header(alloc_heap_tail);

        node->ptr = user_addr;
        node->size = mem_size - 0x10u;
        node->prev = alloc_heap_tail;
        node->next = 0u;
        tail->next = mem_addr;
        alloc_heap_tail = mem_addr;
    }

    return ee_ptr_from_addr32(user_addr);
}

/* Target: 0x0019e648. */
void *calloc_0019e648(size_t count, size_t size)
{
    uint32_t bytes = (uint32_t)count * (uint32_t)size;
    void *ptr = malloc_0019e4b4(bytes);
    unsigned char *dst;
    uint32_t i;

    if (ptr == NULL)
        return NULL;

    dst = (unsigned char *)ptr;
    for (i = 0; i < bytes; i++)
        dst[i] = 0;

    return ptr;
}

/* Target: 0x0019e698. */
void *memalign_0019e698(size_t alignment, size_t size)
{
    ee_addr32_t raw_addr;
    ee_addr32_t aligned_addr;
    ee_addr32_t old_header_addr;
    ee_addr32_t new_header_addr;
    heap_mem_header32 saved;
    heap_mem_header32 *node;
    void *raw;

    if (alignment <= 0x10u)
        return malloc_0019e4b4(size);

    raw = malloc_0019e4b4(size + alignment);
    if (raw == NULL)
        return NULL;

    raw_addr = ee_addr32_from_ptr(raw);
    if ((raw_addr & ((uint32_t)alignment - 1u)) == 0u)
        return raw;

    old_header_addr = raw_addr - 0x10u;
    node = heap_header(old_header_addr);
    node->size -= (uint32_t)alignment;

    aligned_addr = (raw_addr + (uint32_t)alignment - 1u) &
                   ~((uint32_t)alignment - 1u);

    saved = *node;
    new_header_addr = aligned_addr - 0x10u;
    node = heap_header(new_header_addr);
    *node = saved;

    if (node->prev != 0u)
        heap_header(node->prev)->next = new_header_addr;
    if (node->next != 0u)
        heap_header(node->next)->prev = new_header_addr;

    if (alloc_heap_head == old_header_addr)
        alloc_heap_head = new_header_addr;
    if (alloc_heap_tail == old_header_addr)
        alloc_heap_tail = new_header_addr;

    node->ptr = aligned_addr;
    return ee_ptr_from_addr32(aligned_addr);
}

/* Target: 0x0019e784. */
void free_0019e784(void *ptr)
{
    ee_addr32_t ptr_addr;
    ee_addr32_t cur_addr;
    heap_mem_header32 *cur;

    if (ptr == NULL)
        return;
    if (alloc_heap_head == 0u)
        return;

    ptr_addr = ee_addr32_from_ptr(ptr);
    cur = heap_header(alloc_heap_head);

    /* Freeing the head block is handled specially in the target. */
    if (ptr_addr == cur->ptr) {
        uint32_t shrink = cur->size + (cur->ptr - alloc_heap_head);
        ee_addr32_t next = cur->next;

        alloc_heap_head = next;
        if (next != 0u) {
            heap_header(next)->prev = 0u;
        } else {
            alloc_heap_tail = 0u;
            (void)ps2_sbrk_0019f078(-(int32_t)shrink);
        }
        return;
    }

    cur_addr = alloc_heap_head;
    for (;;) {
        cur = heap_header(cur_addr);
        if (ptr_addr == cur->ptr)
            break;
        if (cur->next == 0u)
            return;
        cur_addr = cur->next;
    }

    if (cur->next != 0u) {
        heap_header(cur->next)->prev = cur->prev;
    } else {
        heap_mem_header32 *prev = heap_header(cur->prev);
        ee_addr32_t heap_top;
        uint32_t shrink;

        alloc_heap_tail = cur->prev;
        heap_top = ps2_sbrk_0019f078(0);
        shrink = heap_top - (prev->ptr + prev->size);
        (void)ps2_sbrk_0019f078(-(int32_t)shrink);
    }

    heap_header(cur->prev)->next = cur->next;
}
