# Progress 71 — HUNT350

Checkpoint: **352/1041 = 33.81%** strict function-level matching.

Promoted **18** strict matches from the authoritative 334/1041 checkpoint.

Progress 71 adds two conservative discovery gates to the prior historical evidence:

- historical-symbol matches with only a proven zero alignment gap before the next target boundary;
- raw anchored `.text` subranges with zero relocations, for internal archive helpers that have no usable STT_FUNC symbol.

The second gate is especially relevant to GCC 3.2.2 libgcc object-internal helpers.

Evidence:

- `analysis/matching/progress71-validated-18.tsv`
- `analysis/matching/progress71-gap-aware-hits.tsv`
- `analysis/matching/progress71-raw-object-layout.tsv`
- `analysis/matching/progress71-object-sha256.tsv`

## Functions

- `0x00198c58` `gsDriver_ctor_A` — pgen-libgs-prebuilt / anonymous-historical-fingerprint / exact-next-boundary
- `0x00198cc8` `gsDriver_ctor_B` — pgen-libgs-prebuilt / anonymous-historical-fingerprint / exact-next-boundary
- `0x00199480` `gsPipe_ctor_A` — pgen-libgs-prebuilt / anonymous-historical-fingerprint / exact-next-boundary
- `0x00199590` `gsPipe_ctor_B` — pgen-libgs-prebuilt / anonymous-historical-fingerprint / exact-next-boundary
- `0x00199b80` `gsPipe_setAlphaEnable` — pgen-libgs-prebuilt / gap-aware-historical-symbol / historical-symbol+target-zero-gap:0x4
- `0x0019a240` `gsPipe_TextureDownload` — pgen-libgs-prebuilt / gap-aware-historical-symbol / historical-symbol+target-zero-gap:0x4
- `0x0019e288` `snprintf_putc` — ps2lib-20040415-whole-tu / anonymous-historical-fingerprint / exact-next-boundary
- `0x0019e99c` `strtok` — ps2lib-20040415-whole-tu / p69-os / exact-next-boundary
- `0x001a08ec` `mcInit` — historical-object-rescan / prebuilt-or-prior-historical / exact-next-boundary
- `0x001a1974` `mcSync` — historical-object-rescan / prebuilt-or-prior-historical / exact-next-boundary
- `0x001a1b98` `__floatdisf` — ee-gcc-3.2.2-libgcc-a / gap-aware-historical-symbol / historical-symbol+target-zero-gap:0x4
- `0x001a1f08` `__udivmoddi4_div_clone` — ee-gcc-3.2.2-libgcc-a / gap-aware-historical-symbol / historical-symbol+target-zero-gap:0x4
- `0x001a25d0` `__udivmoddi4_udiv_clone` — ee-gcc-3.2.2-libgcc-a / gap-aware-historical-symbol / historical-symbol+target-zero-gap:0x4
- `0x001a2c98` `__udivmoddi4_umod_clone` — ee-gcc-3.2.2-libgcc-a / gap-aware-historical-symbol / historical-symbol+target-zero-gap:0x4
- `0x001a3380` `_fpadd_parts_d` — ee-gcc-3.2.2-libgcc-a / gap-aware-historical-symbol / historical-symbol+target-zero-gap:0x4
- `0x001a36e0` `snes_p16_001a36e0` — ee-gcc-3.2.2-libgcc-a / anchored-object-layout / exact-next-boundary
- `0x001a39b0` `snes_p16_001a39b0` — ee-gcc-3.2.2-libgcc-a / gap-aware-historical-symbol / historical-symbol+target-zero-gap:0x4
- `0x001a3b30` `litodp` — ee-gcc-3.2.2-libgcc-a / prebuilt-archive / exact-next-boundary
