# HUNT500+ V25 — FileIO seek32 ABI + get_byte finisher

- Base checkpoint: **469/1041**
- Final checkpoint: **470/1041 (45.15%)**
- New strict matches: **1**
- Scope: recovered `src/zlib/gzio_recovered.c` only.
- Source changes tested are derived from committed target assembly and historical zlib 1.1.3 source shape (historical local-next_out read shape, direct errno/read behavior, and 32-bit rewind seek argument/control-flow shape).
- Promotion policy: exact next-boundary size + relocation-normalized equality + no unknown relocations.
- Evidence: `analysis/matching/hunt500plus-v25-validated-1.tsv`
