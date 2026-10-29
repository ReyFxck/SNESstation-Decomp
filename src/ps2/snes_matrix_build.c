/*
 * SNES Station v0.23 PS2 - 3x3 Matrix Builders (from trig tables)
 * Canonical source from progress26_dsp_runtime_batch.c @ 0x0012d334, 0x0012d4ac, 0x0012d624
 */

#include <stdint.h>

typedef struct {
    int16_t out[9];
    int16_t scale;
    int16_t angle_a;
    int16_t angle_c;
    int16_t angle_b;
} SnesMatrixGroup;

typedef struct {
    int16_t fine[256];
    int16_t coarse[256];
    int16_t reciprocal[128];
} SnesTrigTables;

/* Forward declarations for trig lookup helpers */
extern int16_t snes_trig_sine_0012c2dc(const SnesTrigTables *t, int16_t angle);
extern int16_t snes_trig_cosine_0012c398(const SnesTrigTables *t, int16_t angle);

static void snes_matrix_build(SnesMatrixGroup *g, const SnesTrigTables *t)
{
    int32_t sa = snes_trig_sine_0012c2dc(t, g->angle_a);
    int32_t ca = snes_trig_cosine_0012c398(t, g->angle_a);
    int32_t sb = snes_trig_sine_0012c2dc(t, g->angle_b);
    int32_t cb = snes_trig_cosine_0012c398(t, g->angle_b);
    int32_t sc = snes_trig_sine_0012c2dc(t, g->angle_c);
    int32_t cc = snes_trig_cosine_0012c398(t, g->angle_c);
    int32_t half = (int16_t)g->scale >> 1;
    int32_t hca, hsa, hca_cc, hsa_sc, hsa_cc, hca_sc;

    g->scale = (int16_t)(g->scale >> 1);

    hca = (half * ca) >> 15;
    hsa = (half * sa) >> 15;
    hca_cc = (hca * cc) >> 15;
    hsa_sc = (hsa * sc) >> 15;
    hsa_cc = (hsa * cc) >> 15;
    hca_sc = (hca * sc) >> 15;

    g->out[0] = (int16_t)((hca * cb) >> 15);
    g->out[1] = (int16_t)-((hsa * cb) >> 15);
    g->out[7] = (int16_t)(hca_sc + ((hsa_cc * sb) >> 15));
    g->out[8] = (int16_t)((((half * cc) >> 15) * cb) >> 15);
    g->out[2] = (int16_t)((half * sb) >> 15);
    g->out[3] = (int16_t)(hsa_cc + ((hca_sc * sb) >> 15));
    g->out[4] = (int16_t)(hca_cc - ((hsa_sc * sb) >> 15));
    g->out[5] = (int16_t)-((((half * sc) >> 15) * cb) >> 15);
    g->out[6] = (int16_t)(hsa_sc - ((hca_cc * sb) >> 15));
}

/* 0x0012d334 / 0x0012d4ac / 0x0012d624: three identical matrix builders */
void snes_matrix_build_0012d334(SnesMatrixGroup *g, const SnesTrigTables *t)
{
    snes_matrix_build(g, t);
}

void snes_matrix_build_0012d4ac(SnesMatrixGroup *g, const SnesTrigTables *t)
{
    snes_matrix_build(g, t);
}

void snes_matrix_build_0012d624(SnesMatrixGroup *g, const SnesTrigTables *t)
{
    snes_matrix_build(g, t);
}