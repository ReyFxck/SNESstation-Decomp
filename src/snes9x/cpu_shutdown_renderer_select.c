/*
 * SNES Station v0.23 -- compact CPU shutdown and renderer-selector helpers.
 * Target: 0x001ac604 and 0x001ac838.
 */
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t flags;
    uintptr_t pc;
    uintptr_t wait_address;
    uint32_t wait_counter;
    uint64_t cycles;
    uint64_t next_event;
} SnesShutdownCpu;

typedef struct {
    uint8_t shutdown;
    uint8_t sa1_enabled;
    uint8_t apu_executing;
    uint8_t *apu_pc;
    uint8_t *apu_run_flag;
    uint32_t *apu_cycles;
    const uint32_t *apu_cycle_table;
    void (**apu_opcode_table)(void *);
    void *opaque;
    void (*sa1_execute)(void *);
} SnesShutdownEnv;

/*
 * 0x001ac604. This target helper is reached from nine taken-branch opcode
 * handlers. Its shape is the old CPU_SHUTDOWN optimization: repeated branches
 * to WaitAddress can fast-forward CPU cycles and run the APU up to NextEvent.
 */
void CPUShutdown_001ac604(SnesShutdownCpu *cpu, SnesShutdownEnv *env)
{
    if (!env->shutdown || cpu->pc != cpu->wait_address)
        return;

    if (cpu->wait_counter == 0 && (cpu->flags & 0x880u) == 0) {
        cpu->wait_address = 0;
        if (env->sa1_enabled && env->sa1_execute != NULL)
            env->sa1_execute(env->opaque);
        cpu->cycles = cpu->next_event;

        if (env->apu_executing && env->apu_pc != NULL &&
            env->apu_cycles != NULL && env->apu_cycle_table != NULL &&
            env->apu_opcode_table != NULL) {
            /* Target drops APUExecuting-style run state around the catch-up. */
            if (env->apu_run_flag != NULL)
                *env->apu_run_flag = 0;
            while ((uint64_t)*env->apu_cycles < cpu->next_event) {
                uint8_t opcode = *env->apu_pc;
                *env->apu_cycles += env->apu_cycle_table[opcode];
                if (env->apu_opcode_table[opcode] == NULL)
                    break;
                env->apu_opcode_table[opcode](env->opaque);
            }
            if (env->apu_run_flag != NULL)
                *env->apu_run_flag = 1;
        }
        return;
    }

    cpu->wait_counter = cpu->wait_counter < 2 ? cpu->wait_counter - 1u : 1u;
}

typedef struct {
    uintptr_t draw_tile;
    uintptr_t draw_clipped_tile;
    uintptr_t draw_large_pixel;
} SnesTileRendererSelection;

/* Target function addresses installed by the selector. */
enum {
    R_DRAW_TILE16 = 0x00185d8c,
    R_DRAW_CLIPPED16 = 0x001860a8,
    R_DRAW_LARGE16 = 0x001874a8,
    R_ADD = 0x0018789c,
    R_ADD_CLIPPED = 0x00187bb8,
    R_ADD_LARGE = 0x0018a6d4,
    R_ADD_HALF = 0x00188050,
    R_ADD_HALF_CLIPPED = 0x0018836c,
    R_ADD_HALF_LARGE = 0x0018adb8,
    R_SUB = 0x00188804,
    R_SUB_CLIPPED = 0x00188b20,
    R_SUB_LARGE = 0x0018b43c,
    R_SUB_HALF = 0x00188fb8,
    R_SUB_HALF_CLIPPED = 0x001892d4,
    R_SUB_HALF_LARGE = 0x0018bac0,
    R_FIXED_ADD_HALF = 0x0018976c,
    R_FIXED_ADD_HALF_CLIPPED = 0x00189a88,
    R_FIXED_SUB_HALF = 0x00189f20,
    R_FIXED_SUB_HALF_CLIPPED = 0x0018a23c
};

/*
 * 0x001ac838. `normal` selects the plain 16-bit family directly. Otherwise
 * target PPU color-math bits choose add/sub/half/fixed-color renderer triples.
 */
SnesTileRendererSelection SelectTileRenderer_001ac838(
    uint8_t normal, uint8_t color_math_abe, uint8_t color_math_abf)
{
    SnesTileRendererSelection r;
    if (normal) {
        r.draw_tile = R_DRAW_TILE16;
        r.draw_clipped_tile = R_DRAW_CLIPPED16;
        r.draw_large_pixel = R_DRAW_LARGE16;
        return r;
    }

    if (color_math_abf & 0x80u) {
        if (color_math_abf & 0x40u) {
            if (color_math_abe & 0x02u) {
                r.draw_tile = R_SUB_HALF;
                r.draw_clipped_tile = R_SUB_HALF_CLIPPED;
                r.draw_large_pixel = R_SUB_HALF_LARGE;
            } else {
                r.draw_tile = R_FIXED_SUB_HALF;
                r.draw_clipped_tile = R_FIXED_SUB_HALF_CLIPPED;
                r.draw_large_pixel = R_SUB_HALF_LARGE;
            }
        } else {
            r.draw_tile = R_SUB;
            r.draw_clipped_tile = R_SUB_CLIPPED;
            r.draw_large_pixel = R_SUB_LARGE;
        }
    } else if (color_math_abf & 0x40u) {
        if (color_math_abe & 0x02u) {
            r.draw_tile = R_ADD_HALF;
            r.draw_clipped_tile = R_ADD_HALF_CLIPPED;
            r.draw_large_pixel = R_ADD_HALF_LARGE;
        } else {
            r.draw_tile = R_FIXED_ADD_HALF;
            r.draw_clipped_tile = R_FIXED_ADD_HALF_CLIPPED;
            r.draw_large_pixel = R_ADD_HALF_LARGE;
        }
    } else {
        r.draw_tile = R_ADD;
        r.draw_clipped_tile = R_ADD_CLIPPED;
        r.draw_large_pixel = R_ADD_LARGE;
    }
    return r;
}
