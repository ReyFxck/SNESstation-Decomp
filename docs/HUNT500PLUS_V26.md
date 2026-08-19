# HUNT500+ V26 — get_byte success-first + getLong native-width finisher

- Base checkpoint: **470/1041**
- Final checkpoint: **471/1041 (45.24%)**
- New strict matches: **1**
- Scope: recovered `src/zlib/gzio_recovered.c` only.
- Source changes tested are derived from committed target assembly: success-first get_byte refill control flow and native-width getLong accumulation without forced uint32 zero-extension casts.
- Promotion policy: exact next-boundary size + relocation-normalized equality + no unknown relocations.
- Evidence: `analysis/matching/hunt500plus-v26-validated-1.tsv`
