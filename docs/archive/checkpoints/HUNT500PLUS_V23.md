# HUNT500+ V23 — target-guided gzio micro-surgery

- Base checkpoint: **467/1041**
- Final checkpoint: **469/1041 (45.05%)**
- New strict matches: **2**
- Scope: recovered `src/zlib/gzio_recovered.c` only.
- Source changes tested are derived from committed target assembly and historical zlib 1.1.3 source shape (direct errno/read behavior, TRYFREE cleanup, flush error semantics, and rewind control-flow shape).
- Promotion policy: exact next-boundary size + relocation-normalized equality + no unknown relocations.
- Evidence: `analysis/matching/hunt500plus-v23-validated-2.tsv`
